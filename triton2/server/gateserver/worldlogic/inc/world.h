#pragma  once
#include "family.h"
#include "familytype.h"
#include "shm.h"
#include "lk_hashmap.h"
#include "gateobj_define.h"
#include "corpsinfo_pb.hxx.pb.h"  
#include "countrymessage_pb.hxx.pb.h"
#include "entity.h"
#include "module_manager.h"
#include "protype.h"
#include "template.h"
#include "servermessage_pb.hxx.pb.h"
#include "weakprotecter.h"
#include "corps.h"
#include "corpsmanager.h"

#define KING_POWER_INIVALUE1	2000		// 天子初始声望
#define KING_POWER_INIVALUE2	50			// 国王初始声望
#define WORLD_SAVE_TIME			300000		// 世界存盘时间5分钟
#define WAR_CHECK_TIME			50000		// 50秒检查一次战争状态、弱势阵营计算状态
#define WAR_SEED_TIME			10000		// 国战刷新时间
#define ACCUSE_PRENOTICE_TIME	600000		// 弹劾预提示时间
#define ACCUSE_NOTICE_TIME		1800000		// 弹劾实际提示时间
#define ACCUSE_PREPARE_TIME		3600000		// 弹劾提前时间
#define ACCUSE_CHECK_TIME		30000		// 弹劾检查时间
#define ACCUSE_SYNC_TIME		180000		// 弹劾同步数据时间
#define ACCUSE_FORBID_TIME		604800		// 弹劾禁止再次竞价时间
#define GTLW_CHECK_TIME			5000		// 检查国战乱舞状态时间

#define MAX_LINEID_NUM			20			// 最大LineID数量
#define MAX_MAPID_NUM			200			// 最大MapID数量
#define MAX_WAR_WINNER			3			// 最大获胜人数
#define MAX_WAR_CANUM			4			// 最大营地数

#define	MAIN_WIN_POINT			3			// 主战场积分
#define OTHER_WIN_POINT			2			// 其它战场积分
#define	AWARD_PARAM				20			// 奖励参数
#define WIN_PERCENT				4000		// 奖励百分比
#define FIRST_PERCENT			500			// 第一名百分比
#define SECOND_PERCENT			300			// 第二名百分比
#define THIRD_PERCENT			200			// 第三名百分比
#define BEST_PARAM				24			// 门派数参数

#define MAX_SETTAX_TIMES		3			// 设置次数

enum GMWorldState
{
	STATE_DISABLE	= 0,					// 停止GM设置	
	STATE_CONTROL	= 1,					// 控制国战
	STATE_SERIAL	= 2,					// 修改国战编号
	STATE_NOSUBMIT	= 3,					// 禁用报名功能
	STATE_FORCEACCUSE	= 4,				// 强制弹劾
	STATE_FORCEEND		= 5,				// 强制弹劾结束
};

enum AccuseState
{
	STATE_NOACCUSE	= 0,					// 无弹劾状态
	STATE_TOBE		= 1,					// 确定今晚弹劾
	STATE_PREPARE	= 2,					// 预备弹劾
	STATE_ACCUSING	= 3,					// 弹劾进行中
	STATE_ACCUSED	= 4,					// 本周弹劾过
};

using namespace GateServer;

class OfficialFunction
{
public:
	int		mOfficialID;										// 官职者CharID
	char	mOfficialName[ NAME_LENGTH ];						// 官员名字
	int		mUsetimes[ OFFICIAL_FUNCTION_NUM ][ MAX_POWER ];		// 使用次数
};

class CCountry
{
private:
	// 国王声望
	int	mKingPower;		

	// 才华值
	int mWizard[ WIZARD_NUM ];

	// 建设值
	int mConstructValue[ OFFICAL_NUM ];

	// 才华建设值
	int mConstructWizard[ OFFICAL_NUM ];

	// 建设等级
	int mConstructLevel[ OFFICAL_NUM ];

	// 国家人数
	int mPeopleNum;

	// 地图ID
	int mMapID;

	// 占领军团ID
	int mCorpsID;

	// 上次发送的声望
	int	mLastPower;

public:
	// 官职信息
	OfficialFunction mOfficialData[ OFFICIAL_LEVEL_NUM ][ MAX_OFFICAL_NUM ];
public:
	CCountry( )
	{
		if ( CObj::msCreateMode )
		{	
			Initialize();
		}
		else
		{			

		}	
	}
	~CCountry(){ }
public:
	int Initialize( )
	{
		mKingPower = 0;
		memset( mWizard, 0 ,sizeof(mWizard) );
		memset( mConstructValue, 0 ,sizeof(mConstructValue) );
		memset( mConstructLevel, 0 ,sizeof(mConstructLevel) );
		mPeopleNum = 0;
		mMapID = 0;
		mCorpsID = 0;
		memset( mOfficialData, 0 ,sizeof(mOfficialData) );
		mLastPower = 0;
		return 0;
	}	
	int Resume( ){ return 0; }
public:

	void SetKingPower( int vPower ) { mKingPower = std::max( vPower, 0 ); }
	int  GetKingPower(){ return mKingPower; } 

	void SetWizard( int vValue, int vIndex )
	{
		if ( vIndex < 0 || vIndex > (int) ARRAY_CNT(mWizard) )
			return;
		mWizard[ vIndex ] = vValue > 0 ? vValue : 0;
	}
	int  GetWizard( int vIndex )
	{ 
		if ( vIndex < 0 || vIndex > (int) ARRAY_CNT(mWizard) )
			return 0;
		return mWizard[ vIndex ];
	}

	void SetPeopleNum( int vNum ) { mPeopleNum = std::max( vNum, 0 ); }
	int  GetPeopleNum( ){ return mPeopleNum; }
	void AddPeopleNum( ){ ++mPeopleNum; mPeopleNum = std::max( mPeopleNum, 0 ); }
	void DelPeopleNum( ){ --mPeopleNum; mPeopleNum = std::max( mPeopleNum, 0 ); }

	void SetMapID( int vID ) { mMapID = vID; }
	int  GetMapID(){ return mMapID; }

	void SetConstructValue( int vValue, int vIndex )
	{
		if ( vIndex < 0 || vIndex > (int) ARRAY_CNT(mConstructValue) )
			return;
		mConstructValue[ vIndex ] = vValue > 0 ? vValue: 0;
	}
	int  GetConstructValue( int vIndex )
	{ 
		if ( vIndex < 0 || vIndex > (int) ARRAY_CNT(mConstructValue) )
			return 0;
		return mConstructValue[ vIndex ];
	}

	void SetConstructWizard( int vValue, int vIndex )
	{
		if ( vIndex < 0 || vIndex > (int) ARRAY_CNT(mConstructWizard) )
			return;
		mConstructWizard[ vIndex ] = vValue > 0 ? vValue: 0;
	}
	int  GetConstructWizard( int vIndex )
	{ 
		if ( vIndex < 0 || vIndex > (int) ARRAY_CNT(mConstructWizard) )
			return 0;
		return mConstructWizard[ vIndex ];
	}

	int  GetConstructTotal( int vIndex )
	{ 
		if ( vIndex < 0 || vIndex > (int) ARRAY_CNT(mConstructValue) )
			return 0;
		return mConstructWizard[ vIndex ] + mConstructValue[ vIndex ];
	}

	void SetConstructLevel( int vValue, int vIndex )
	{
		if ( vIndex < 0 || vIndex > (int) ARRAY_CNT(mConstructLevel) )
			return;
		mConstructLevel[ vIndex ] = vValue;
	}
	int  GetConstructLevel( int vIndex )
	{ 
		if ( vIndex < 0 || vIndex > (int) ARRAY_CNT(mConstructLevel) )
			return 0;
		CTemplateConstructExp *tpExp = (CTemplateConstructExp*)CDataStatic::GetTemp( TEMP_CONSTRUCTEXP );
		if ( tpExp == NULL )
			return 0;
		int i = 1;
		CTemplateMapTable *tpTable = (CTemplateMapTable*)CDataStatic::GetTemp( TEMP_MAPTABLE );
		if ( tpTable == NULL )
			return 0;
		for (; i < (int) ARRAY_CNT(tpExp->mExp); i++ )
		{
			// 如果高于限制等级，不能升级
			if ( i >= tpTable->mLevel[ mMapID ] )
			{
				break;
			}
			if ( GetConstructTotal( vIndex ) <= tpExp->mExp[ i ] )
			{
				break;
			}
		}
		return i;
	}

	void SetCorpsID( int vID ) { mCorpsID = vID; }
	int  GetCorpsID(){ return mCorpsID; }

	void LoadCorps( );
	void LoadOfficial( PBOfficialList vList );
	void SaveOfficial( PBOfficialList &vList );

	void LoadOfficial( PBOfficialList vList, int vOfficial );
	void SaveOfficial( PBOfficialList &vList, int vOfficial );

	CCorps *GetCorps( ){ return CCorpsManager::GetSingleton().GetCorpsByID( mCorpsID ); }
	inline int GetKingID( )
	{
		CCorps *tpCorps = GetCorps( );
		if ( tpCorps != NULL )
		{
			return tpCorps->GetRooterID( );
		}
		return 0;
	}
	inline const char *GetKingName( )
	{
		CCorps *tpCorps = GetCorps( );
		if ( tpCorps != NULL )
		{
			return tpCorps->GetRooterName( );
		}
		return "";
	}
	inline int GetOfficialID( int vCharID )
	{
		for ( int i = 0; i < OFFICIAL_LEVEL_NUM; i++ )
		{
			for ( int j = 0; j < MAX_OFFICAL_NUM; j++ )
			{
				if ( mOfficialData[ i ][ j ].mOfficialID == vCharID )
				{					
					CTemplateOfficialTable *tpOfficialTable = (CTemplateOfficialTable*)CDataStatic::GetTemp( TEMP_OFFICALLIST );
					if ( tpOfficialTable != NULL )
					{
						int tOfficialID = tpOfficialTable->mOfficial[ i ][ j ];
						CTemplateOfficial *tpOfficial = (CTemplateOfficial *)CDataStatic::SearchTpl( tOfficialID );
						if ( tpOfficial == NULL )
						{
							return -1;
						}
						return tpOfficial->mTempID;
					}
				}
			}
		}
		return 0;
	}
	inline void ClearOfficialID( int vCharID, int &vLevel, int vOrder )
	{
		for ( int i = 0; i < OFFICIAL_LEVEL_NUM; i++ )
		{
			for ( int j = 0; j < MAX_OFFICAL_NUM; j++ )
			{
				if ( mOfficialData[ i ][ j ].mOfficialID == vCharID )
				{
					vLevel = i;
					vOrder = j;
					mOfficialData[ i ][ j ].mOfficialID = 0;
					memset( mOfficialData[ i ][ j ].mOfficialName, 0, sizeof( mOfficialData[ i ][ j ].mOfficialName ) );
					return;
				}
			}
		}
	}
	inline int IsHighOfficial( int vCharID )
	{
		for ( int i = OFFICIAL_LEVEL_TWO; i < OFFICIAL_LEVEL_KING; i++ )
		{
			for ( int j = 0; j < MAX_OFFICAL_NUM; j++ )
			{
				if ( mOfficialData[ i ][ j ].mOfficialID == vCharID )
				{
					CTemplateOfficialTable *tpOfficialTable = (CTemplateOfficialTable*)CDataStatic::GetTemp( TEMP_OFFICALLIST );
					if ( tpOfficialTable != NULL )
					{
						return tpOfficialTable->mOfficial[ i ][ j ];
					}
				}
			}
		}
		return 0;
	}
	inline void ChangeConstructByWizard( int vIndex, int vValue )
	{
		if ( vIndex == WIZARD_ARTS )
		{
			mConstructWizard[ OFFICAL_LIBIN ] += vValue;
			mConstructWizard[ OFFICAL_LIBIN ] = std::max( mConstructWizard[ OFFICAL_LIBIN ], 0 );
		}
		else if ( vIndex == WIZARD_TACTICS )
		{
			mConstructWizard[ OFFICAL_GUOFANG ] += vValue;
			mConstructWizard[ OFFICAL_GUOFANG ] = std::max( mConstructWizard[ OFFICAL_GUOFANG ], 0 );
		}	
		else if ( vIndex == WIZARD_POWER )
		{
			mConstructWizard[ OFFICAL_FAYUAN ] += vValue;
			mConstructWizard[ OFFICAL_FAYUAN ] = std::max( mConstructWizard[ OFFICAL_FAYUAN ], 0 );
		}
		else if ( vIndex == WIZARD_WISDOM )
		{
			mConstructWizard[ OFFICAL_GONGCHENG ] += vValue;
			mConstructWizard[ OFFICAL_GONGCHENG ] = std::max( mConstructWizard[ OFFICAL_GONGCHENG ], 0 );
		}
		else if ( vIndex == WIZARD_POLITICS )
		{
			mConstructWizard[ OFFICAL_RENSHI ] += vValue;
			mConstructWizard[ OFFICAL_RENSHI ] = std::max( mConstructWizard[ OFFICAL_RENSHI ], 0 );
		}
		else if ( vIndex == WIZARD_EDUCATE )
		{
			mConstructWizard[ OFFICAL_HUKOU ] += vValue;
			mConstructWizard[ OFFICAL_HUKOU ] = std::max( mConstructWizard[ OFFICAL_HUKOU ], 0 );
		}
	}

	void SetLastPower( int vPower ) { mLastPower = std::max( vPower, 0 ); }
	int  GetLastPower(){ return mLastPower; } 
	inline void AddAllCount( int nFunctionType, int nFunctionOrder )
	{
		for ( int i = OFFICIAL_LEVEL_NUM - 1; i >= 0; i-- )
		{
			for ( int j = 0; j < MAX_OFFICAL_NUM; j++ )
			{
				mOfficialData[ i ][ j ].mUsetimes[ nFunctionType ][ nFunctionOrder ]++;
			}
		}
	}
};

class MetierRank
{
public:
	int		mCharID;
	int		mHonor;
	int		mKill;
};

class CWorld
{
public:
	// 国家结构
	CCountry mCountry[ COUNTRY_NUM ];

	// 友好度关系
	int	mFriendStatus[ WORLD_NUM ];

	// 外交关系
	int mNationStatus[ WORLD_NUM ];

	// 操作列表
	int	mOperationList[ WORLD_NUM ];

	// 操作次数
	int mOpNumber;

	// 报名的玩家总等级
	int mTotalLevel;

	// 报名的玩家
	int mTotalNum;

	// 该阵营在各服务器各地图玩家数量
	int mPlayerNum[ MAX_LINEID_NUM ][ MAX_MAPID_NUM ];

	// 阵营阵地ID
	int mCampID;

	// 战场内数值
	int mSource[ MAX_WAR_CAMP ];

	// 战场内增长值
	int mSeed[ MAX_WAR_CAMP ];

	// 竞价金额
	int mBidMoney;

	// 竞价的阵地ID
	int mBidCampID;

	// 获胜人
	MetierRank mWinChar[ METIER_NOVICE ][ MAX_WAR_WINNER ];

	// 战斗胜利基础金额
	int mBasicAward;

	// 优秀胜利基础金额
	int mBestAward;

	// 报名总金额
	int	mSubmitMoney;

	// 占领据点数
	int mSeedNum[ MAX_WAR_CAMP ];

	// 击杀总数
	int mKill[ MAX_WAR_CAMP ];

	//用于统计弱势阵营的当周个人荣誉累计值
	int m_nHonerInc;
	
	//上次统计的等级积分
	int m_nCampLevelScore;

	//上次统计的个人荣誉累计值
	int m_nCampHonerScore;

	//上次统计的总分，不保存数据库，最近一次统计得到
	int m_nCampTotalScore;

	//弱势级别，不保存数据库，启动时根据积分计算，以保证修改数据库数据后不冲突
	int m_nWeekLevel;

	// 弹劾状态
	int mAccuseState;

	// 弹劾赞成票
	int mAccuseYes;

	// 弹劾反对票
	int mAccuseNo;

	// 弹劾弃权票
	int mAccuseAbort;

	// 城门计数
	int mGateTimes;

	// 大旗计数
	int mFlagTimes;

	// 弹劾定时
	CWTimer mAccusePreNoticeTimer;
	CWTimer mAccuseNoticeTimer;
	CWTimer mAccusePrepareTimer;
	CWTimer mAccuseSyncTimer;

	// GM弹劾
	int	mGMAccuse;

	// 国探阵营
	int	mGTWorld;

	// 国探结束时间
	int	mGTEndTime;

	// 乱舞阵营
	int mLWWorld;

	// 乱舞结束时间
	int mLWEndTime;
	
public:
	CWorld( )
	{
		if ( CObj::msCreateMode )
		{	
			Initialize();
		}
		else
		{			
			Resume();
		}	
	}
	~CWorld(){ }
public:
	int Initialize( )
	{
		memset( mCountry, 0, sizeof(mCountry) );
		memset( mFriendStatus, 0, sizeof(mFriendStatus) );
		memset( mNationStatus, 0, sizeof(mNationStatus) );
		memset( mOperationList, 0, sizeof(mOperationList) );
		mCountry[ COUNTRY_CHANGAN ].SetMapID( COUNTRY_MAP_CHANGAN );
		mCountry[ COUNTRY_WUJI ].SetMapID( COUNTRY_MAP_WUJI );
		mCountry[ COUNTRY_CHECHI ].SetMapID( COUNTRY_MAP_CHECHI );
		mCountry[ COUNTRY_BAOXIANG ].SetMapID( COUNTRY_MAP_BAOXIANG );
		mCountry[ COUNTRY_NVER ].SetMapID( COUNTRY_MAP_NVER );
		mOpNumber = 0;
		
		// 人数容器，长期用
		memset( mPlayerNum, 0, sizeof(mPlayerNum) );
		// 以下战斗结束后清除 
		mTotalLevel = 0;
		mTotalNum = 0;		
		mCampID = -1;
		memset( mSource, 0, sizeof(mSource) );		// 这两个开始时再清一次
		memset( mSeed, 0, sizeof(mSeed) );
		mBidMoney = 0;
		mBidCampID = -1;
		mSubmitMoney = 0;
		memset( mSeedNum, 0, sizeof(mSeedNum) );		// 这两个开始时再清一次
		memset( mKill, 0, sizeof(mKill) );
		// 以下战斗开始时清除
		memset( mWinChar, 0, sizeof(mWinChar) );
		mBasicAward = 0;
		mBestAward = 0;

		mAccuseState = 0;
		mAccuseYes = 0;
		mAccuseNo = 0;
		mAccuseAbort = 0;
		mGateTimes = 0;
		mFlagTimes = 0;
		mAccusePreNoticeTimer = CWTimer( ACCUSE_PRENOTICE_TIME );
		mAccusePrepareTimer = CWTimer( ACCUSE_PREPARE_TIME );
		mAccuseNoticeTimer = CWTimer( ACCUSE_NOTICE_TIME );
		mAccuseSyncTimer = CWTimer( ACCUSE_SYNC_TIME );
		mGMAccuse = 0;

		// 初始化弱国保护相关数据
		m_nHonerInc = 0;
		m_nCampLevelScore = 0;
		m_nCampHonerScore = 0;
		m_nCampTotalScore = 0;
		m_nWeekLevel = 0;

		mGTWorld = 0;
		mGTEndTime = 0;
		mLWWorld = 0;
		mLWEndTime = 0;

		return 0;
	}	
	int Resume( ){ return 0; }
public:	
};

class CWorldManager : public CSingleton< CWorldManager >
{
public:
	CWorld	mWorld[ WORLD_NUM ];
	int		mWarSerial;
	int		mWinWorld;
	int		mTitleTime;
	int		mSetRate;			// 预定税率
	int		mTaxRate;			// 税率
	int		mTaxMoney;			// 税金
	int		mCorpsMoney;		// 税金折合金锭
	int		mSetTimes;			// 设置次数
public:
	static CSharedMem* mShmPtr; 
	void * operator new( size_t tsize );
	void   operator delete( void *pPointer );			    
	static unsigned int CountSize();
	CWorldManager();
	~CWorldManager(){}
	int Initailize( );
	int Resume( );	
public:
	int UpdateWorldDBInfo( int vWorldID, int vCountryID );
	int UpdateWorldStatusDBInfo( int vWorldID );
	void LoadCorpsFromMap( int vWorldID, int vMapID, int vCorpsID );
	void SyncNation( int vServerID );
	void DailyRefresh( );
	int GetCountryIDByMap( int vMapID );
	void ClearOfficialID( CGatePlayer *pPlayer );
	void ChangePeopleNum( int vWorldID, int vCountryID, int vChangeType );
	int UpdateOfficialDBInfo( int vWorldID, int vCountryID );
	CCountry* GetCountryByPlayer( CGatePlayer *pPlayer );
	void CheckAccuseTimer( int vWorldID, int vOffset );
	void SysForceAccuse( int vWorldID, int vMapID );
	void ResetAccuseState( int vWorldID );

	void AddPlayerNum( int vWorldID, int vLineID, int vMapID );
	void DelPlayerNum( int vWorldID, int vLineID, int vMapID );
	int GetPlayerTotal( int vLineID, int vMapID );
	int GetPlayerNum( int vWorldID, int vLineID, int vMapID );
	int GetLineID( int vLineID );
	void OnSubmit( int vWorldID, int vLevel, int vMoney );
	void EndClearWar( );
	void StartClearWar( );
	void SetCampID( int vWorldID, int vCampID );
	int GetCampID( int vWorldID );
	void AddSource( int vWorldID, int vCampID, int vValue );
	void DelSource(	int vWorldID, int vCampID, int vValue );	
	void AddSeed( int vWorldID, int vCampID, int vValue );
	void DelSeed( int vWorldID, int vCampID, int vValue );	
	int GetSource( int vWorldID, int vCampID );
	int GetSeed( int vWorldID, int vCampID );
	int GetSeedNum( int vWorldID, int vCampID );
	void ClearSource( );
	void SetBidMoney( int vWorldID, int vMoney );
	int GetBidMoney( int vWorldID );
	void SetBidCampID( int vWorldID, int vCampID );
	int GetBidCampID( int vWorldID );
	void SetSubmitMoney( int vWorldID, int vMoney );
	int GetSubmitMoney( int vWorldID );
	int GetWinWorld( );
	void CheckBidWorld( );
	void AddWinner( int vWorldID, int vCharID, int vHonor, int vKill, int vMetierID );
	int UpdateWorldWarDBInfo( int vWorldID );
	int UpdateWorldGlobalDBInfo( );
	int	CheckAward( );	
	int GetBasicAward( int vWorldID );
	int GetBestAward( int vWorldID );
	int GetRank( int vWorldID, int vCharID, int vMetierID );	
	void SwapWinner( int vWorldID, int vMetierID, int vSrcIndex, int vDesIndex );
	void ReorderWinner( int vWorldID, int vMetierID, int vFirst );
	int UpdateWorldWarCharDBInfo( int vWorldID );
	void GetAwardBySingle( int vWorldID, int vLevel, int vCharID, int vMetierID, int &vBasicAward, int &vBestAward );
	void AddKill( int vWorldID, int vCampID );
	int GetKill( int vWorldID, int vCampID );

	int GetGTTarget( int nWorldID );
	int GetGTEndTime( int nWorldID );
	int GetLWTarget( int nWorldID );
	int GetLWEndTime( int nWorldID );
	int SetGT( int nSrcWorld, int nDesWorld, int nTime );
	int SetLW( int nSrcWorld, int nDesWorld, int nTime );
	int CheckGTAndLW( );
	int SendGTAndLWToScene( int nServerID );
public:
	bool IsWinner(int nWorldID);
	CWorld* GetWorldByID(int nWorldID);
};

class CWorldModule: public CDataModule, public CSingleton< CWorldModule >
{
public:
	CWTimer mTimer;
	int		mTokenID;
	int		mWorldLaunched;
	int		mOfficialLaunched;
	int		mStatusLaunched;
	CWTimer mWarTimer;
	int		mWarStatus;
	CWTimer mSeedTimer;	
	CWTimer	mAccuseTimer;
	CWTimer mGTLWTimer;

	int		mWarLaunched;
	int		mCharLaunched;
	int		mGlobalLaunched;

	int		mGMDay;
	int		mGMHour;
	int		mGMMin;
	int		mNoSubmit;
	CWeakProtecter		m_WeakProtecter;	// 弱国保护机制
public:
	void OnLaunchServer();
	void OnExitServer();
	void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
	void OnMessage( int nServerID, CMessage* pMessage );
	void OnSeverMessage(int nServerID, CMessage* pMessage );
	CWorldModule();
	~CWorldModule(){ };
	int Initialize();
	int Resume(); 	
	static unsigned int CountSize();

	virtual bool IsLaunched();

	virtual void OnNewDayCome( );

	inline int GetTokenID( ){ return mTokenID; }
	inline void SetTokenID( int vTokenID ){ mTokenID = vTokenID; }

	void LoadWorld( );
	void LoadWorldStatus( );
	void OnSessionLoadWorld( CMessage* pMessage );
	void OnSessionLoadWorldStatus( CMessage* pMessage );
	void AddOperation( int vSrcWorldID, int vDesWorldID, int vOpType );
	void SetOperation( int vSrcWorldID, int vCharID );
	bool HasOperation( int vSrcWorldID, int vDesWorldID, int vOpType );
	void ClearOperation( int vSrcWorldID, int vDesWorldID, int vOpType );
	void LoadOfficial( );
	void LoadOfficial( int vWorldID, int vCountryID );
	void OnSessionLoadOfficial( CMessage* pMessage, int nParam1 );	
	void LoadWorldWar( );
	void OnSessionLoadWorldWar( CMessage* pMessage );
	void LoadWorldGlobal( );
	void OnSessionLoadWorldGlobal( CMessage* pMessage );
	void LoadWorldWarChar( );
	void OnSessionLoadWorldWarChar( CMessage* pMessage );

	void CheckWarTime( );
	void UpdateAllWar( );
	bool OnUseGTFunction( int nSrcWorldID, int nDesWorldID, int nTime, CGatePlayer *pPlayer );
	bool OnUseLWFunction( int nSrcWorldID, int nDesWorldID, int nTime, CGatePlayer *pPlayer );
	
public:
	void OnMessageCountryInfo( int nServerID, CMessage *pMessage );
	void OnMessageJoinCountry( int nServerID, CMessage *pMessage );
	void OnMessageLeaveCountry( int nServerID, CMessage *pMessage );

	void OnMessageSetOfficial( int nServerID, CMessage *pMessage );
	void OnMessageUnSetOfficial( int nServerID, CMessage *pMessage );
	void OnMessageUseFunction( int nServerID, CMessage *pMessage );
	void OnMessageOfficialInfo( int nServerID, CMessage *pMessage );
	void OnMessageUseTimes( int nServerID, CMessage *pMessage );

	void OnMessageChangeWizard( int nServerID, CMessage *pMessage );
	void OnMessageChangeConstruct( int nServerID, CMessage *pMessage );
	void OnMessageChangePower( int nServerID, CMessage *pMessage );
	void OnMessageChangeFriend( int nServerID, CMessage *pMessage );
	void OnMessageKillPerson( int nServerID, CMessage *pMessage );

	void OnMessageWorldQuest( int nServerID, CMessage *pMessage );
	void OnMessageWorldAnswer( int nServerID, CMessage *pMessage );
	void OnMessageWorldInfo( int nServerID, CMessage *pMessage );
	void OnMessagePlayerInfo( int nServerID, CMessage *pMessage );
	void OnMessageTrySummon( int nServerID, CMessage *pMessage );
	void OnMessageRealSummon( int nServerID, CMessage *pMessage );
	void OnMessageRefuseSummon( int nServerID, CMessage *pMessage );
	void OnMessageAccuseKing( int nServerID, CMessage *pMessage );
	void OnMessagePowerInfo( int nServerID, CMessage *pMessage );
	void OnMessageSpecialEvent( int nServerID, CMessage *pMessage );

	void SendCountryInfoResponse( int vCharID, int vWorldID, int vCountryID, int vNeedFull );
	void SendJoinCountryResponse( int vCharID, int vResult, int vCountryID );
	void SendLeaveCountryResponse( int vCharID,  int vResult );
	void SendJoinCountryNotify( int vCharID, int vCountryID );
	void SendOfficialChangeNotify( int vCharID, int vOfficialLevel, int vOfficialOrder, int vWorldID, int vCountryID, int vType );

	void SendSetOfficialResponse( int vCharID, int vResult, char *vName, int vLevel, int vOrder, int vNormalSet, int vTargetID );
	void SendUnSetOfficialResponse( int vCharID, int vResult, int vLevel, int vOrder );
	void SendUseFunctionResponse( int vCharID, int vResult, char * vName, int vType, int vOrder );
	void SendOfficialInfoResponse( int vCharID, int vWorldID, int vCountryID );
	void SendUseTimesResponse( int vCharID, int vWorldID, int vCountryID );
	void SendUseFunctionNotify( int vCharID, int vTargetID, int vType, int vOrder, int vWorldID, int vCountryID, char *vSrcName, TeleportData tData );
	void SendKillPersonResponse( int vServerID, int vSrcCharID, int vDesCharID, int vPKValue );

	void SendWorldQuestResponse( int vCharID, int vResult, int vWorldID, int vOpType );
	void SendWorldAnswerResponse( int vCharID, int vResult, int vWorldID, int vOpType, int vAnswer );
	void SendWorldQuestNotify( int vCharID, int vWorldID, int vOpType );
	void SendWorldResultNotify( int vSrcWorldID, int vDesWorldID, int vOpType );
	void SendWorldInfoResponse( int vCharID, int vWorldID );	
	void SendEventNotify( int vEventID, int vWorldID, int vCountryID, int *vIntParam, int vSize, char *vStringParam1, char *vStringParam2, int vCharID = 0 );
	void SendWorldAnswerNotify( int vCharID, int vSrcWorldID, int vDesWorldID, int vOpType, int vAnswer );
	void SendKingChangeNotify( int vWorldID, int vCountryID, int nServerID );
	void SendAllKingNotify( int nServerID );
	void SendWarNotify( int vEventID, int vInt0, int vInt1, int vInt2, char *vName );
	void SendWarSource( int vWorldID, int vMapID, int vSource );
	void SendReliveChange( int nServerID );
	void SendWarSourceToSingle( int vCharID, int vMapID );
	void SendSyncWarStatus( int nServerID );
	void SendWarCamp( );
	void SendCreateWarStoneNotify( int vMapID, int vServerID );
	void SendModifyRateNotify( int vRate, CGatePlayer *pPlayer, int vReal, int vWinWorld );
	void SendAccuseKingNotify( int vWorldID, CGatePlayer *pPlayer );
	void SendAccuseInfoNotify( int vEventID, int vWorldID, int vInt0, int vInt1, int vInt2, int vInt3, const char *vName, int vCharID = 0 );
	void SendAccuseStatusNotify( int vWorldID, CGatePlayer *pPlayer );
	void SendMakeGTNotify( int nSrcWorldID, int nDesWorldID, int nTime, CGatePlayer *pPlayer );
	void SendMakeLWNotify( int nSrcWorldID, int nDesWorldID, int nTime, CGatePlayer *pPlayer );

	void OnMessageSubmitWar( int nServerID, CMessage *pMessage );
	void OnMessageEnterWar( int nServerID, CMessage *pMessage );
	void OnMessageQueryWarCamp( int nServerID, CMessage *pMessage );
	void OnMessageQueryWarNum( int nServerID, CMessage *pMessage );
	void OnMessageQueryWarInfo( int nServerID, CMessage *pMessage );
	void OnMessageQueryWarRank( int nServerID, CMessage *pMessage );
	void OnMessageBidCamp( int nServerID, CMessage *pMessage );
	void OnMessageWarChange( int nServerID, CMessage *pMessage );
	void OnMessageObtainWarHonor( int nServerID, CMessage *pMessage );
	void OnMessageCampBidInfo( int nServerID, CMessage *pMessage );
	void OnMessageCampResInfo( int nServerID, CMessage *pMessage );
	void OnMessageWarControl( int nServerID, CMessage *pMessage );
	void OnMessageGetWarAward( int nServerID, CMessage *pMessage );
	void OnMessageQueryWarAward( int nServerID, CMessage *pMessage );
	void OnMessageWarStoneOver( int nServerID, CMessage *pMessage );
	void OnMessageAddTax( int nServerID, CMessage *pMessage );
	void OnMessageModifyRate( int nServerID, CMessage *pMessage );
	void OnMessageGetTax( int nServerID, CMessage *pMessage );

public:
	void OnEventUpdateNationTitle( int nRoleID, int nMapID, int nAction, bool bAdd );
	void SendNationTitleUpdateNotice( CGatePlayer* npPlayer, int nTitleID, bool bAdd );
	void OnMessageGetNationTitleList( int nServerID, CMessage* npMessage );
};
