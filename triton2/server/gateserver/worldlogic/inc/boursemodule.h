#ifndef _BROUSE_H_
#define _BROUSE_H_

#include <protype.h>

#define BOURSE_LOG_KEY "bourse"

#define BOURSE_ASSERT( a, fail_handle )   LK_ASSERT_LOG( BOURSE_LOG_KEY, a, fail_handle )
#define BOURSE_ASSERT_STR( a, fail_handle, str, ... ) LK_ASSERT_LOG_STR( BOURSE_LOG_KEY, a, fail_handle, str, \
                                                                        ##__VA_ARGS__ )

#define BOURSE_LOG( level, str, ... )	\
	( ( level == _LOG_NOTICE_ ) ? LOG_NOTICE( BOURSE_LOG_KEY, "[ %s : %d ][ %s ] "str, __LK_FILE__, __LINE__, \
								__FUNCTION__, ##__VA_ARGS__ ) : \
	( ( level == _LOG_INFO_ ) ? LOG_INFO( BOURSE_LOG_KEY, "[ %s : %d ][ %s ] "str, __LK_FILE__, __LINE__, __FUNCTION__,\
                                ##__VA_ARGS__ ) : \
	( ( level == _LOG_ERROR_ ) ? LOG_ERROR( BOURSE_LOG_KEY, "[ %s : %d ][ %s ] "str, __LK_FILE__, __LINE__, \
								__FUNCTION__, ##__VA_ARGS__ ) : \
	( ( level == _LOG_DEBUG_ ) ? LOG_DEBUG( BOURSE_LOG_KEY, "[ %s : %d ][ %s ] "str, __LK_FILE__, __LINE__, \
								__FUNCTION__, ##__VA_ARGS__ ) : \
	( ( level == _LOG_WARN_ ) ? LOG_WARN( BOURSE_LOG_KEY, "[ %s : %d ][ %s ] "str, __LK_FILE__, __LINE__, __FUNCTION__,\
                                ##__VA_ARGS__ ) : 0\
	) ) ) ) )

struct BourseDetail
{
	int mBourseDocNo;
	int mAccountID;
	int mPrice;
	int mYbCount;
	int mBourseDate;

	BourseDetail( )
	{
		if( CObj::msCreateMode )
		{
			Clear( );
		}
	}

	void Clear( )
	{
		mAccountID = 0;
		mPrice = 0;
		mYbCount = 0;
		mBourseDate = 0;
		mBourseDocNo = 0;
	}
};

class BourseDetailList
{
private:
	typedef lk::vector< BourseDetail, 5 > BOURSEDETAIL_LIST_TYPE;
private:
	BOURSEDETAIL_LIST_TYPE mList;
	PBBourseDetailList mListPB;

public:
	BourseDetailList( );
	int Clear( );
	bool CompareWithPB( PBBourseDetailList* vSrcList );
	int SerializeFromPB(  PBBourseDetailList* vSrcList );
	int SerializeToPB( PBBourseDetailList* vSrcList );	
	int GetPriceMax( );
	int GetPriceMin( );
};


class CBourseModule  : public CDataModule, public CSingleton< CBourseModule >
{
private:
	static int sIsLaunch;

//	bool mIsLaunch;
	int	mLaunchFlag;

	uint64_t	 mListID;

//	BourseDetail mBuyList[ BROUSE_LIST_COUNT_MAX ];
//	BourseDetail mSellList[ BROUSE_LIST_COUNT_MAX ];
//	PBBourseDetailList mBuyList;
//	PBBourseDetailList mSellList;

	BourseDetailList mBuyList;
	BourseDetailList mSellList;

	CWTimer		mTimerOut;	

private:
	int GetBourseList( );
	int RefreshClientList( );
	bool LoadBourseList( 	PBBourseDetailList* vSrcList, BourseDetailList* vDesList );
	int BourseListNotice(  CSceneServer* pHost );

public:
	static int CountSize( ) { return sizeof( CBourseModule ); }
    void* operator new( size_t size );
    void  operator delete( void* pointer );

	CBourseModule( );
	int OnMessageBourseListNotice( CMessage* vpMsg );

	int OnMessageBourseLoadList( int vServerID, CMessage* vpMsg );
	int OnMessageBourseTradeNotice( int vServerID, CMessage* vpMsg );
	void OnMessageBourseLog( CMessage* vpMsg );

	virtual void OnLaunchServer();
	virtual void OnExitServer(){ }
	virtual void OnTimer(unsigned int, unsigned int);
	virtual void OnMessage(int, CMessage*){ }
	virtual bool IsLaunched(  ) { return mLaunchFlag == sIsLaunch; }

	int OnServerMessage( int vServerID, CMessage* vpMsg );

	int BourseTradeNoticeAccount( int vAccountID, CMessage* vpMsgDetail, int vMsgID );

	int OnHostDown( CSceneServer* pHost );

	int OnSessionCheckFire( CMessage* pMessage, int vParame1, int vParame2 );
};

#endif


