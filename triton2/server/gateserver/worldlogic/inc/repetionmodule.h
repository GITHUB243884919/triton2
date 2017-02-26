#ifndef _REPETION_GATE_H_
#define _REPETION_GATE_H_

#include "gateteam.h"

#include "module_manager.h"
#include "entity.h"
#include "lk_hashmap.h"
#include "lk_string.h"
#include "lk_vector.h"
#include "lk_queue.h"
#include "gateobj_define.h"
#include "gateobj_manager.h"
#include "servertool.h"
//#include "gatelogic.h"

#define REPE_LOG_KEY "rep"

#define REPE_ASSERT( a, fail_handle )   LK_ASSERT_LOG( REPE_LOG_KEY, a, fail_handle )
#define REPE_ASSERT_STR( a, fail_handle, str, ... ) LK_ASSERT_LOG_STR( REPE_LOG_KEY, a, fail_handle, str, \
                                                                        ##__VA_ARGS__ )


#define REPE_LOG_INFO( str, ... )	LOG_INFO( REPETION_LOG, "[ %s : %d ][ %s ]"str, __LK_FILE__, __LINE__,\
										 __FUNCTION__, ##__VA_ARGS__ )

enum LOG_LEVEL
{
	_LOG_NOTICE_		=	1,
	_LOG_INFO_			=	2,
	_LOG_ERROR_			= 	3,
	_LOG_DEBUG_			=	4,
	_LOG_WARN_			=	5
};

#define REPE_LOG( level, str, ... )	\
	( ( level == _LOG_NOTICE_ ) ? LOG_NOTICE( REPE_LOG_KEY, "[ %s : %d ][ %s ] "str, __LK_FILE__, __LINE__, \
								__FUNCTION__, ##__VA_ARGS__ ) : \
	( ( level == _LOG_INFO_ ) ? LOG_INFO( REPE_LOG_KEY, "[ %s : %d ][ %s ] "str, __LK_FILE__, __LINE__, __FUNCTION__,\
                                ##__VA_ARGS__ ) : \
	( ( level == _LOG_ERROR_ ) ? LOG_ERROR( REPE_LOG_KEY, "[ %s : %d ][ %s ] "str, __LK_FILE__, __LINE__, __FUNCTION__,\
                                ##__VA_ARGS__ ) : \
	( ( level == _LOG_DEBUG_ ) ? LOG_DEBUG( REPE_LOG_KEY, "[ %s : %d ][ %s ] "str, __LK_FILE__, __LINE__, __FUNCTION__,\
                                ##__VA_ARGS__ ) : \
	( ( level == _LOG_WARN_ ) ? LOG_WARN( REPE_LOG_KEY, "[ %s : %d ][ %s ] "str, __LK_FILE__, __LINE__, __FUNCTION__,\
                                ##__VA_ARGS__ ) : 0\
	) ) ) ) )

using namespace GateServer;

enum REPETION_END_REASON
{
	REPETION_END_COMMON		= 0,	// 正常结束
	REPETION_END_TIMEOUT	= 1,	// 超时
	REPETION_END_REBOOT		= 2,		// 副本重新启动连接
	REPETION_END_CREATEERR	= 3		// 创建失败
};

//typedef long long KEY_TYPE;
//#define INVITED_KEY_VALUE		( 0 )

class CRepetionInfo
{
private:
	typedef lk::vector< DateSpace, 10 > DATE_SPACE_LIST;
	bool 			mBattleOn;

	unsigned int mPlayerCountMax;

	unsigned int mPlayerNumMax;                 //人数上限
	unsigned int mPlayerNumMin;                 //人数下限

	unsigned short mBackMapID;
	unsigned short mBackPosX;
	unsigned short mBackPosY;

	int			mRepetionIndex;
	int			mMapIDList[ REPE_MAP_COUNT_MAX ];
	
	DATE_SPACE_LIST mOpenDateSpace;
	bool		 mIsOnTimeOpen;					//是否是按时开放的，报名副本不算在内
	bool		 mNotLimitTime;					//不限制传入时间
	int			 mRelogin;
public:
	CRepetionInfo( )
	{ 
		if( CObj::msCreateMode == 1)
		{
			mPlayerCountMax = 0;
			mPlayerNumMax = 0;
			mPlayerNumMin = 0;
			mRepetionIndex = INVALID_MAP_ID;
			mOpenDateSpace.initailize( );
			mBattleOn = false;
			mIsOnTimeOpen = false;
			mNotLimitTime = false;
			mRelogin = 0;
		}
	}

	int SetRepetionIndex( int vRepetionIndex ) { mRepetionIndex = vRepetionIndex; return SUCCESS; }
	int GetRepetionIndex( ) { return mRepetionIndex; }
	int GetPlayerCountMax( ){ return mPlayerCountMax; }
	int SetPlayerCountMax( unsigned int vPlayerCountMax ){ mPlayerCountMax = vPlayerCountMax; return 0; }

        int GetPlayerNumMax() {return mPlayerNumMax; }
        int SetPlayerNumMax(unsigned int vPlayerNumMax) { mPlayerNumMax = vPlayerNumMax;  return 0; }
        int GetPlayerNumMin() {return mPlayerNumMin; }
        int SetPlayerNumMin(unsigned int vPlayerNumMin) { mPlayerNumMin= vPlayerNumMin;  return 0; }

	int SetMapID( int vMapID ) 
	{ 
		for( unsigned int i = 0; i < ARRAY_CNT( mMapIDList ); i++ )
		{
			if( mMapIDList[ i ] == INVALID_MAP_INDEX )
			{
				mMapIDList[ i ] = vMapID;
				return SUCCESS;;
			}
		}
		return ERR_FULL; 
	}
	int GetMapID( int vIndex )
	{
		REPE_ASSERT_STR( vIndex >= 0, return INVALID_MAP_INDEX, "vIndex %d", vIndex );
		REPE_ASSERT_STR( vIndex < ( int )ARRAY_CNT( mMapIDList ), return INVALID_MAP_INDEX, "vIndex %d", vIndex );
		
		return mMapIDList[ vIndex ];
	}
	unsigned int GetMapListSizeMax( )
	{
		return ARRAY_CNT( mMapIDList );
	}
	int GetBeginMapID( )
	{
		return GetMapID( 0 );
	}

	bool MapIsInRepetion( int vMapID ) 
	{
		for( unsigned int i = 0; i < ARRAY_CNT( mMapIDList ); i++ )
		{
			if( mMapIDList[ i ] == INVALID_MAP_INDEX )
			{
				return false;
			}
			if( mMapIDList[ i ] == vMapID )
			{
				return true;
			}
		} 
		return false;
	}

	int SetBackMapID( unsigned short vMapID ) { mBackMapID = vMapID; return SUCCESS; }
	int SetBackPos( unsigned short vPosX, unsigned short vPosY ) 
		{ mBackPosX = vPosX; mBackPosY = vPosY; return SUCCESS; }
	unsigned short GetBackMapID( ) { return mBackMapID; }
	unsigned short GetBackPosX( ) { return mBackPosX; }
	unsigned short GetBackPosY( ) { return mBackPosY; }

	int RemoveFromWait( int vCharID );
        int RemoveFromFixWait(int vCharID);

	int SetBattleOpenSpace( DateSpace* vSpace );
	int ClearBattleOpenSpace( );
	bool InBattleTime( struct tm* vTm );
	bool IsBattleOn( );
	int SetBattleOnStatus( bool IsOn );
	void SetOnTimeOpen( int OnTimeOpen )
	{
		if ( OnTimeOpen != 0 )
		{
			mIsOnTimeOpen = true;
		}
	}

	bool IsOnTimeOpen( ){  return mIsOnTimeOpen; }
	void SetNotLimitTime( int NotLimitTime )
	{
		if( NotLimitTime != 0)
		{
			mNotLimitTime = true;
		}
	}

	bool NotLimitTime(){ return mNotLimitTime;}
	void SetRelogin( int nRelogin ){ mRelogin = nRelogin; }
	int GetRelogin(){ return mRelogin; }
};

typedef lk::vector< int, 200 > DELETE_VECTOR;

class CRepetion : public CObj
{
private:
	struct CampInfo
	{
		int		mMaxCount;
		int		mCampID;

		CampInfo( )
		{
			if( CObj::msCreateMode )
			{
				mMaxCount = 0;
				mCampID	= 0;	
			}
		}
	};
private:
	typedef lk::vector< unsigned int, REPETION_PLAYER_COUNT_MAX > PLAYER_LIST_TYPE;

	int					mRepetionIndex;
	KEY_TYPE	 		mKey;
	unsigned int		mInitTime;
	unsigned int		mTeamID;
	unsigned int		mLineID;
	unsigned short		mMapID;
	unsigned int		mPlayerCountMax;
	PLAYER_LIST_TYPE	mPlayers;
	unsigned int		mMapIndex;

	int					mTimer;

	int					mType;		// notice to client

	CampInfo			mCampInfo[ 5 ];


	int					mParam1; 
	int					mParam2;

public:
	CRepetion( ) 
	{ 
        if ( CObj::msCreateMode )
        {
            Initialize( );
        }
        else
        {
            Resume( );
        }	
	}

	int Initialize( )
	{
		mKey = INVITED_KEY_VALUE;
		mInitTime = time(NULL);
		mTeamID = 0;
		mLineID = INVALID_LINE_ID;
		mMapIndex = INVALID_MAP_INDEX;
		mMapID = INVALID_MAP_ID;
		mPlayers.initailize( );
		mRepetionIndex = INVALID_REPETION_INDEX;
		mPlayerCountMax = 0;

		mTimer = 120 * 60 * 1000;
		mType = 0;

		return SUCCESS;
	}

	int Resume( )
	{
		return SUCCESS;	
	}

	int Final( );
//	{
//		mKey = INVITED_KEY_VALUE;
//		mMapID = INVALID_MAP_ID;
//		mPlayers.clear( );
//		return SUCCESS;
//	}
	int SetType( int vType ) { mType = vType; return SUCCESS; }	
	int GetType( ) { return mType; }
	int SetLineID( unsigned int vLineID ) { mLineID = vLineID; return mLineID; }
	unsigned int GetLineID( ) { return mLineID; }
	int SetMapID( unsigned int vMapID ){ mMapID = vMapID; return SUCCESS; }
	unsigned int GetPlayerCountMax( );
	unsigned int GetMapID( ){ return mMapID; }
	int InsertPlayer( unsigned int vCharID );
	int RemovePlayer( unsigned int vCharID );
	bool InRepetion( unsigned int vCharID );
	KEY_TYPE GetKey( ){ return mKey; }
	unsigned int GetPlayerCountCur( );
	int SetKey( KEY_TYPE vKey ){ mKey = vKey; return SUCCESS; }
	int SetMapIndex( unsigned int vMapIndex ) { mMapIndex = vMapIndex; return SUCCESS; }
	unsigned int GetMapIndex( ) { return mMapIndex; }
	int OnTimer( unsigned int vTickOffset, DELETE_VECTOR* vDeleteList );
	int SetRepetionIndex( int vRepetionIndex ) { mRepetionIndex = vRepetionIndex; return SUCCESS; }
	int GetRepetionIndex( ) { return mRepetionIndex; }
	CRepetionInfo* GetRepetionInfo( );

	int GetPlayerCountByCamp( int vCampID );
	bool IsCanIntoRepeByCamp( CGatePlayer* vpPlayer );
	int SetCampCountMax( int vCampID, int vMaxCount );

	// 设置和获取参数
	void SetParam1( int nParam1 ) { mParam1 = nParam1; }
	int GetParam1( ) { return mParam1; }

	void SetParam2( int nParam2 ) { mParam2 = nParam2; }
	int GetParam2( ) { return mParam2; }

	int GetInitTime() { return mInitTime; }
	// 获取玩家角色ID
	int GetRoleIDList( int *pRoleIDList, int &rNum );
};



struct CLineRepetionInfo
{
	int mLineID;
	int mServerID;
	int mRepetionCountMax;
	int mRepetionCountCur;
	unsigned int mMapList[ MAP_INDEX_MAX ];
	bool mRun;
	
	CLineRepetionInfo( )
	{
		if( CObj::msCreateMode )
		{
			Clear( );
		}
	}

	int Clear( )
	{
		mLineID = INVALID_LINE_ID;
		mRepetionCountCur = 0;
		mRepetionCountMax = 0;
		memset( mMapList, 0, sizeof( mMapList ) );
		mRun = false;
		return SUCCESS;
	}

	int SetLineID( int vLineID ) { mLineID = vLineID; return SUCCESS; }
	int GetLineID( ) { return mLineID; }
	
	int GetFreeCount( ) { return ( mRun == true ) ? mRepetionCountMax - mRepetionCountCur : 0 ; }
	bool IsRegisteMap( unsigned int vMapID ) 
	{ 
		REPE_ASSERT_STR( vMapID < ARRAY_CNT( mMapList ), return false, "vMapID %d", vMapID );
		return mMapList[ vMapID ] == vMapID; 
	}
	int RegisteMap( unsigned int vMapID ) 
	{  
		REPE_ASSERT_STR( vMapID < ARRAY_CNT( mMapList ), return ERR_INVALID_PARAMETER, "vMapID %d", vMapID );
		mMapList[ vMapID ] = vMapID;
//		LOG_NOTICE( "default", "[ %s : %d ][ %s ] Registe LineID %d Map %d LineRepetion", __LK_FILE__, __LINE__, 
//			__FUNCTION__, mLineID, vMapID );
		REPE_LOG( _LOG_NOTICE_, "Registe LineID %d Map %d LineRepetion", mLineID, vMapID );
		return SUCCESS;	
	}
	
	int SetLineRepetionCountMax( int vMax )
	{
		REPE_ASSERT_STR( vMax >= mRepetionCountCur, return ERR_INVALID_PARAMETER,  "CurCount %d, MaxCount %d",
			mRepetionCountCur, vMax );
		mRepetionCountMax = vMax;
		mRun = true;
//		LOG_NOTICE( "default", "[ %s : %d ][ %s ] Registe LineID %d LineRepetionCount Max %d", __LK_FILE__, 
//			__LINE__, __FUNCTION__, mLineID, mRepetionCountMax );
		REPE_LOG( _LOG_NOTICE_, "Registe LineID %d LineRepetionCount Max %d", mLineID, mRepetionCountMax );
		return SUCCESS;
	}

	int AddLineRepetionCount( )
	{
		REPE_ASSERT_STR( mRepetionCountMax > mRepetionCountCur, return ERR_FULL, "CurCount %d, MaxCount %d", 
			mRepetionCountCur, mRepetionCountMax );
		mRepetionCountCur++;
		return SUCCESS;
	}

	int SubLineRepetionCount( )
	{
		if( mRepetionCountCur > 0 )
		{
			mRepetionCountCur--;
		}
	
		return mRepetionCountCur;
	}

	int SetDown( )
	{
		mRun = false;
		return SUCCESS;
	}
};

class CRepetionModule : public CDataModule, public CSingleton< CRepetionModule >
{
private:
	struct BattleWaitPlayer
	{
		int					mPlayerCount;
		int					mPlayers[ TEAM_NUMBER_MAX ];

		BattleWaitPlayer( )
		{
			if( CObj::msCreateMode )
			{
				Clear( );
			}
		};

		int Clear( )
		{
			mPlayerCount = 0;
			for( unsigned int i = 0; i < ARRAY_CNT( mPlayers ); i++ )
			{
				mPlayers[ i ] = 0;
			}

			return SUCCESS;
		}
	};

	typedef lk::vector< BattleWaitPlayer, 100 >		BATTLE_WAIT_LIST;

	struct BattleWait
	{
		int					mLevelMin;					//最低等级
		int					mLevelMax;					//最大等级
		BATTLE_WAIT_LIST	mList;						//等待队列

		BattleWait( )
		{
			if( CObj::msCreateMode )
			{
				mLevelMax = 0;
				mLevelMin = 0;
				mList.initailize( );
			}
		}
	};

	typedef lk::vector< BattleWaitPlayer, 400 >		BATTLE_FIX_WAIT_LIST;

	struct BattleFixWait
	{
		int		        mLevelMin;					//最低等级
		int			mLevelMax;					//最大等级
		BATTLE_FIX_WAIT_LIST    mList;						//等待队列

		BattleFixWait( )
		{
			if( CObj::msCreateMode )
			{
				mLevelMax = 0;
				mLevelMin = 0;
				mList.initailize( );
			}
		}
	};

	struct OnTimeOpenLevelLimit
	{
		int mLevelMin;
		int mLevelMax;
		bool mIsCreate;
		KEY_TYPE mKey;
	
		OnTimeOpenLevelLimit()
		{
			if( CObj::msCreateMode )
			{
				mLevelMin = 0;
				mLevelMax = 0;
				mIsCreate = false;
				mKey = INVITED_KEY_VALUE;
			}
		}
	};

	struct OnTimeRepetion
	{
		bool mShouldOpen;
		OnTimeOpenLevelLimit mOnTimeLevelLimit[ LEVEL_SEGMENT_MAX ];
		OnTimeRepetion()
		{
			if( CObj::msCreateMode )
			{
				mShouldOpen = false;
			}
		}
	};
	


private:
	CRepetionInfo		mRepetionInfo[ REPE_MAP_COUNT_MAX ];		
	CWTimer				mTimeOut;

	CLineRepetionInfo	mLineRepetion[ 20 ];
	BattleWait			mWait[ REPE_MAP_COUNT_MAX ][ WAIT_LIST_MAX ]; 
	BattleFixWait       mFixWait[ REPE_MAP_COUNT_MAX ][ WAIT_LIST_MAX ];
	CWTimer 			mTimer;
	
	OnTimeRepetion		mOnTimeRepetion[ _REPE_MAX_COUNT_ ];

private:	
	KEY_TYPE MakeKey( unsigned int vParame1, unsigned int vParame2 );
	CRepetion* CreateRepetion(	CRepetionInfo* vpInfo, 
								unsigned short vParame,
								int vCreator,
								int vType = 0,
								int vParame1 = 0,
								int vParame2 = 0,
								KEY_TYPE vKey = INVITED_KEY_VALUE,
								int vRemoveItem = 0
							);


	int CreatRepetionLog( CRepetion* vpRepetion, int vCreator, int vParame );
	int DestroyRepetionLog( CRepetion* vpRepetion, int vReason, int vParame ); 
public:
    void* operator new( size_t size );
    void  operator delete( void* pointer );

	virtual bool IsLaunched() { return true; }
 
	CRepetionModule( );
	CRepetion* GetRepetion( KEY_TYPE vKey );
//	CRepetion* GetRepetion( unsigned short vMapID, unsigned int vCharID );

	CRepetion* CreateRepetionByMapID( 	unsigned short vMapID, 
										unsigned short vParame, 
										int vCreator,
										int vType = 0,
										int vParame1 = 0, 
										int vParame2 = 0,
										KEY_TYPE vKey = INVITED_KEY_VALUE, 
										int vRemoveItem = 0
									);
	CRepetion* CreateRepetionByIndex(	int vRepetionIndex,
										unsigned short vParame,
										int vCreator,
										int vType = 0,
										int vParame1 = 0,
										int vParame2 = 0,
										KEY_TYPE vKey = INVITED_KEY_VALUE
									);
	int DestroyRepetion( CRepetion* vpRepetion, REPETION_END_REASON vReason, int vParame = 0 );

	int	OnMessageBattleCreate( int vServerID, CMessageBattleCreateRequest* vpMsg );

//	int LoadRepetionInfo( );
	CRepetionInfo* GetRepetionMapInfoByMapID( int vMapID );
	CRepetionInfo* GetRepetionMapInfo( int vRepetionIndex );

	int PlayerIntoRepetion( CGatePlayer* vpPlayer, unsigned short vMapID, unsigned short vTeamLevel, KEY_TYPE vKey, 
		int* vLineID );

	int PlayerOutRepetion( CGatePlayer* vpPlayer, unsigned short vMapID );

	virtual void OnLaunchServer() { }//LoadRepetionInfo( ); }
	virtual void OnExitServer() { }
	virtual void OnTimer(unsigned int, unsigned int);
	virtual void OnMessage(int, CMessage*);

	static int CountSize( ) { return sizeof( CRepetionModule ); }

	int SetPlayerJoin( int vCharID, int vBattleID, KEY_TYPE vKey, int vTime, int vCampID, int vCampCountMax = 0, bool bIntoLastRepetion = false );
	bool IsBattleMap( int vMapID );

	// use by family
	KEY_TYPE CreateBattleByIndex( int vBattleID, int vLevel, int vCreator, int vType = 0, int vParame1 = 0, int vParame2 = 0 );
	int PlayerJoin( int vCharID, int vBattleID, KEY_TYPE vKey, int vCampID, int vCampCountMax = 0, bool nIntoLastRepetion = false );

	CLineRepetionInfo* RegistRepetionMap( int vLineID, int vMapID );
	CLineRepetionInfo* GetLineRepetionInfo( int vLineID );
	CLineRepetionInfo* GetMostIdleLineID( int vMapID );

	bool InWaitList( int vCharID, unsigned int vBattleID );						// 玩家是否在等待队列
	int RemoveFromWait( int vCharID, unsigned int vBattleID );					// 将玩家从等待队列中删除
	int RemoveFromAllWait( int vCharID );
	int TeamWaitBattle( int vCharID, int vLevel, unsigned int vBattleID );		//队伍进入战场等待队列
	int PlayerJoinBattle( int vCharID, unsigned int vBattleID );					// 玩家进入战场
	int SetWaitListLevel( int vRepetionIndex, int vLevelMin, int vLevelMax );
	int CleanWaitList( int vRepetinoIndex );
	int CleanFixWaitList(int vRepetinoIndex);                     //清楚等待固定报名的等待list


	int IntoRepetionTeam( CMessageTeamIntoRepetion* vpMsg );
	int NotifyPlayerIntoRepetion( CGatePlayer* vpPlayer, int vLineID, int vMapID, KEY_TYPE vKey, int vPosX, int vPosY );
	int IntoRepetionSignle( CMessageSingleIntoRepetion* vpMsg );

	int OnHostDown( CSceneServer *pHost );

	int SetBattleOpenSpace( int vRepetionIndex, const PBDateSpace* vSpace );
	int ClearBattleOpenSpace( int vRepetionIndex );

	bool IsRepetionMap( int vMapID );

        int RemoveFromFixWait( int vCharID, unsigned int vBattleID );
        int TeamFixWaitBattle(int vCharID, int vLevel, unsigned int vBattleID);
        int SingleFixWaitBattle(int vCharID, int vLevel, unsigned int vBattleID);
	bool InFixWaitList( int vCharID, unsigned int vBattleID );						// 玩家是否在等待队列
	int SetFixWaitListLevel( int vRepetionIndex, int vLevelMin, int vLevelMax );
	int SendEventCode2Scene( CGatePlayer* vpPlayer, int vEventCode, int vRepetionID );
	int SetOnTimeOpenListLevel( int vRepetionIndex, int vLevelMin, int vLevelMax );
	int JoinOnTimeOpenBattle(  CMessageS2GJoinOnTimeOpenBattleRequest *vtpMsg );  //进入固定时间开发的副本
	int CreateOnTimeOpenRepetion( int vRepetionIndex ); //创建定时开放的副本 ，不包括报名
	void OnMessageIntoCorpsBossRepetion( int nServerID, CMessage* npMessage ); //处理进入军团boss副本
	void OnMessageEnterLastRepetion( int nServerID, CMessage* npMessage ); //副本重入的消息
	void SendLastRepetionIndex( CGatePlayer *pPlayer, int nIndex, ERepetionIndexType eType );
};

#endif



