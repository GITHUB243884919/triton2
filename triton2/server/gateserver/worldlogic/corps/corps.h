#pragma  once
#include "corpstype.h"
#include "object.h"
#include "family.h"
#include "lk_hashmap.h"
#include "servertool.h"
#include  "corpsinfo_pb.hxx.pb.h"
#include "coretype.h"
#include "gateobj_define.h"
#include "protype.h"
#include "taskmodule.h"


struct NpcStatus;

enum EChallegeEvent
{
	EVENT_ADD = 0,
	EVENT_DEL,
	EVENT_CLEAR,
};

enum EChallegeType
{
	TYPE_SEND = 0,
	TYPE_REC,
};

struct CCorpsFamiyInfo
{ 
	int mContribute;
};

enum ECorpsStateTag
{
	ST_Occupy	= 0x01,	//占领了城池
	ST_InBid	= 0x02,	//参与了竞价
	ST_WaitBtl	= 0x04,	//等待开战
	ST_InBtl	= 0x08,	//战斗中
	ST_Single	= 0x10, //单独的军团
	ST_Union	= 0x20, //军团联盟
	ST_CityMap	= 0x40,	//主城地图, 不指定表示全部
	ST_AllState	= 0xFF, //所有状态
};
class CCorpsRepetion
{
private:
	int mCorpsBossID;
	int mStartTime;
	bool mIsOpen;
	KEY_TYPE mKey;

public:
	CCorpsRepetion()
	{
		mCorpsBossID = 0;
		mStartTime = 0;
		mIsOpen = false;
		mKey = INVITED_KEY_VALUE;
	}
public:
	void SetCorpsBossID( int nBossID)
	{
		mCorpsBossID = nBossID;
	}
	int GetCorpsBossID(){ return mCorpsBossID ;}

	void SetStartTime()
	{
		mStartTime = time(NULL);
	}

	int GetStartTime(){ return mStartTime;}

	void SetCorpsIsOpen( bool nIsOpen)
	{
		mIsOpen = nIsOpen;		
	}
	bool IsOpen(){ return mIsOpen;}
	void SetRepetionKey( KEY_TYPE vKey) { mKey  = vKey;}
	KEY_TYPE GetRepetionKey() { return mKey; }
};

class CCorps:public CObj
{
public:
	enum ECORPS_SAVESTATUS
	{
		CORPS_INFONOCHANGE = 0 ,			// 数据信息没有改变		
		CORPS_ALLINFOCHANGED,				// 所有信息都改变
	};
		
	// 军团操作的一些错误码
	enum
	{
		EM_MONEYNOTENOUGH	=	1,			// 金钱不够
	};
public:
	CCorps( )
	{
		if ( msCreateMode )
		{
			Initialize();
		}
		else
		{
			Resume();
		}
	}
	~CCorps( ){ }
	virtual int Initialize();
	virtual int Resume(){ return 0; } 
public:
	typedef lk::hash_map< int, CCorpsFamiyInfo, MAX_CORPS_FAMILYNUM >  FamilyList;
	typedef lk::hash_map< int, int, MAX_CORPSAPPLY_NUM >			   ApplyList;
	typedef lk::hash_map< int, int, MAX_CORPS_BOSS_NUM >			   CorpsBossList;

	// 参战成员
	typedef lk::vector< int, CORPS_BATTLE_NUM > BattleList;
	// 挑战列表
	typedef lk::vector< int, SERVER_CAP_CORPS > ChallegeList;

	
	enum EFamilyList
	{
		FAMILYLISTFULL	 = 1,
		FAMILYHASEXIST	 = 2,
	};	
	enum ECorpsApplyList
	{
		APPLYLISTFULL	 = 1,	
	};	
	enum
	{
		EM_CORPS_STATUS_ALIVE	= 0,	///< 军团状态正常
		EM_CORPS_STATUS_DISBAND	= 1,	///< 军团处于解散状态
	};
	
public:
	FamilyList		m_tFamilyList;										// 军团中的家族列表
	ApplyList		m_tApplyList;										// 军团中的申请列表信息
	int				m_nRooterID;										// 大将军角色ID
	char			m_sRooterName[NAME_LENGTH];							// 大将军名称
	int				m_nCorpsID;											// 军团ID
	char			m_sCorpsName[MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES];	// 军团名称
	organization_t 	m_tOfficers[MAX_OFFICER_NUM];						// 职位体系	
	int				m_nCountryID;										// 国家ID
	char			m_sBulletin[CORPS_BULLETIN_LENGTH*MAX_UTF8_BYTES];	// 公告
	int				m_nCorpsMoney;										// 军团金钱
	int				m_nCorpsStatus;										// 军团状态
	int				m_nCorpsDisbandTime;								// 军团解散时间
	int				m_nSaveStatus;
	
	int				m_nCorpsLevel;										// 军团等级	
	int				m_nCorpsGlory;										// 军团荣耀	
			
	int				m_nPVPWinNum;										// pvp胜利的场数
	int				m_nPVPFailedNum;									// pvp失败的场数
	int				m_nPVPScore;										// pvp积分
	int				m_nPVPStarLevel;									// pvp星级	
	int				m_nBidNpcID;										// 竞价报名的NPCID
	int				m_nBidMoney;										// 参与竞价消耗的金钱
	int				m_nVictoryTimes;									// 战胜次数
	int				m_nLoserTimes;										// 战败丢城次数
	bool			m_bCanBattle;										// 是否获得战斗资格
	int				m_nCampID;											// 阵营ID
	int				m_nBattleCity;										// 城池ID
	KEY_TYPE		m_BattleKey;										// 战场key

	int				m_WarRank;
	int				m_WarStatus;
	int				m_EnemyCorpsID;										// 对战军团
	BattleList		m_BattleList;										// 参战成员列表
	int				m_UnionID;											// 联军ID
	int				m_BOSSRefineValue;									// 军团boss炼化值
//	int				m_RefineValueLimit;									// 炼化值上限
	int				m_nCorpsBossScore;								//  军团boss积分
	int				m_nCorpsCollectActive;								// 军团采集活跃度
	

	// 挑战赛数据
	char			m_RoomName[ NAME_LENGTH ];
	int				m_RoomMoney;
	ChallegeList	m_ChallegeList;
	ChallegeList	m_SendChallegeList;
	int				m_RoomState;
	
	int				m_nCorpsWelfareGetTime;

	char			m_sTmpUnionName[MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES];	// 军团名称
	char			m_sTmpUnionNotice[CORPS_BULLETIN_LENGTH*MAX_UTF8_BYTES];	// 公告
	int				m_OtherCorpsID[ MAX_OTHER_UNION ];
	bool			m_InviteIdle;
	int				m_Color;											// 军团颜色	
	int				m_nForbidTime;										// 禁止报名长安的时间
	
	CLeagueShopsManager	m_tCorpsLeagueShop;								// 军团的商店信息

	CorpsBossList	m_CorpsBossList;									// 军团boss类表
	CCorpsRepetion  m_CorpsRepetion;									// 军团副本


	CGroupPublishInfo mCorpsPublishTask;								// 军团发布
public:

	void OnNewDayCome()
	{
		mCorpsPublishTask.OnNewDayCome();
	}

	void OnNewWeekCome()
	{
		mCorpsPublishTask.OnNewWeekCome();
	}

	// set & get
	inline void SetColor( int nColor ) 
	{ 
		m_Color = nColor; 
		SetSaveStatus( CORPS_ALLINFOCHANGED ); 
	}
	inline int  GetColor() { return m_Color; }

	void SetRoomName( const char* vpName )
	{ 
		if ( vpName == NULL ) return ;
		strncpy( m_RoomName, vpName, sizeof( m_RoomName ) - 1 );
	}
	char* GetRoomName(){ return m_RoomName; }

	void SetRoomMoney( int nMoney ) 
	{
		 m_RoomMoney = nMoney;
		 SetSaveStatus( CORPS_ALLINFOCHANGED );
	}
	int  GetRoomMoney(){ return m_RoomMoney; }

	void SetRoomState( int nState ) { m_RoomState = nState; }
	int  GetRoomState(){ return m_RoomState; }

	void SetEnemyCorpsID( int nCorpsID ) { m_EnemyCorpsID = nCorpsID; }
	int  GetEnemyCorpsID(){ return m_EnemyCorpsID; }

	void SetWarRank( int nRank ) { m_WarRank = nRank; }
	int  GetWarRank(){ return m_WarRank; }

	void SetWarStatus( int nStatus ) { m_WarStatus = nStatus; }
	int  GetWarStatus(){ return m_WarStatus; }

	// 设置和获取军团ID
	void SetCorpsID( int nCorpsID ){ m_nCorpsID = nCorpsID; }
	int	 GetCorpsID(){ return m_nCorpsID; }	 

	void SetCorpsCamp( int nCampID ) 
	{	
		m_nCampID = nCampID; 
		SetSaveStatus( CORPS_ALLINFOCHANGED );
	}
	int  GetCorpsCamp() { return m_nCampID; }

	void SetBattleCity( int nCity );

	int  GetBattleCity() { return m_nBattleCity; }

	void SetBattleKey( KEY_TYPE nKey )
	{
		 m_BattleKey = nKey;
	}
	KEY_TYPE GetBattleKey(){ return m_BattleKey; }
	
	// 设置&获取军团福利的领取时间
	void SetCorpsWelfareGetTime( int nTime ){ m_nCorpsWelfareGetTime = nTime; }
	int	 GetCorpsWelfareGetTime(){ return m_nCorpsWelfareGetTime; }
	
	// 获取军团的npc金钱
	int GetCorpsWelfare( int nNpcID, int &rNpcMoney, int &rNpcGlory, bool bRefresh = true );	
	
	// 设置和获取军团名称
	void SetCorpsName( const char  *pName )
	{ 
		strncpy( m_sCorpsName, pName, MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES -1 );
		m_sCorpsName[MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES -1] = '\0';		
	}
	
	// 社会&获取军团荣耀
	void SetCorpsGlory( int nGlory )
	{ 
		m_nCorpsGlory = nGlory; 
		SetSaveStatus( CORPS_ALLINFOCHANGED );
	}
	int GetCorpsGlory(){ return m_nCorpsGlory; }	  
	
	// 设置&获取pvp积分
	void SetPVPScore( int nScore )
	{
		 m_nPVPScore = nScore;
		 SetSaveStatus( CORPS_ALLINFOCHANGED );
	}
	int	 GetPVPScore(){ return m_nPVPScore; }
	
	// 设置&获取pvp星级
	void SetPVPStarLevel( int nLevel )
	{
		m_nPVPStarLevel = nLevel;
		SetSaveStatus( CORPS_ALLINFOCHANGED );
	}
	int	 GetPVPStarLevel( ){ return m_nPVPStarLevel; }	
	
	const char *GetCorpsName( ){ return m_sCorpsName; }
	
	const char *GetCorpsNameShow()
	{
		// 字符集用于显示
#ifdef CHARSET_SHOW_GBK
		static char tCorpsName[ MAX_CORPSNAME_LENGTH ] = {0};
		int nNameLen = sizeof(tCorpsName)-1;
		charset_convert( "utf8", "gbk", (char*)m_sCorpsName, strlen( m_sCorpsName ),
			tCorpsName, (size_t*)&nNameLen );
		return tCorpsName;
#else
		return m_sCorpsName;
#endif		
	}
	
	// 获取军团的商店信息
	CLeagueShopsManager *GetCorpsShop(){ return &m_tCorpsLeagueShop; }
	
	void SetBidNpcID(int nNpcID)
	{
		 m_nBidNpcID = nNpcID;
		 SetSaveStatus( CORPS_ALLINFOCHANGED );
	}
	int GetBidNpcID(){ return m_nBidNpcID; }

	void SetBidMoney(int nMoney)
	{
		m_nBidMoney = nMoney;
	}
	int GetBidMoney(){ return m_nBidMoney; }

	// 设置&获取军团胜利的场数
	void SetVictoryNum( int nNum )
	{
		m_nVictoryTimes = nNum;
		SetSaveStatus( CORPS_ALLINFOCHANGED );
	}	
	void IncVictoryTimes()
	{
		m_nVictoryTimes += 1;
		SetSaveStatus( CORPS_ALLINFOCHANGED );
	}
	
	void IncLoserTimes()
	{
		m_nLoserTimes += 1;
		SetSaveStatus( CORPS_ALLINFOCHANGED );
	}
	void SetLoserTimes( int nTimes )
	{
		m_nLoserTimes = nTimes;
		SetSaveStatus( CORPS_ALLINFOCHANGED );
	}
	
	int GetVictoryTimes(){ return m_nVictoryTimes; }
	int GetLoserTimers(){ return m_nLoserTimes; }

	bool GetCanBattle(){ return m_bCanBattle; }
	void SetCanBattle( bool bFlag )
	{
		m_bCanBattle = bFlag;
		SetSaveStatus( CORPS_ALLINFOCHANGED );
	}
	
	// 设置和获取大将军ID
	void SetRooterID( int nRooterID ){ m_nRooterID = nRooterID; SetSaveStatus( CORPS_ALLINFOCHANGED ); }
	int	 GetRooterID( ){ return m_nRooterID; }
	
	// 设置和获取大将军姓名
	void SetRooterName( const char *pName )
	{ 
		strncpy( m_sRooterName, pName, NAME_LENGTH -1 );
		m_sRooterName[ NAME_LENGTH -1] = '\0'; 
		SetSaveStatus( CORPS_ALLINFOCHANGED );
	}
	const char *GetRooterName( ){ return m_sRooterName; }	 	
	
	// 设置和获取国家ID
	void SetCorpsCountryID( int nCountryID ){ m_nCountryID = nCountryID;  }
	int	 GetCorpsCountryID( ){ return m_nCountryID; }
	
	// 设置和获取家族等级
	void SetCorpsLevel( int nCorpsLevel ){ m_nCorpsLevel = nCorpsLevel; SetSaveStatus( CORPS_ALLINFOCHANGED ); }
	int	 GetCorpsLevel(){ return m_nCorpsLevel; }
	
	// 设置和获取军团金钱
	void SetCorpsMoney( int nMoney )
	{			
		m_nCorpsMoney = nMoney;
		if ( m_nCorpsMoney > MAX_FAMILYCORPS_MONEY || m_nCorpsMoney < 0 )
		{
			m_nCorpsMoney = nMoney;
		}
		SetSaveStatus( CORPS_ALLINFOCHANGED );
	}
	
	int	 GetCorpsMoney( ){ return m_nCorpsMoney; }
	
	// 设置和获取军团状态
	void SetCorpsStatus( int nStatus ){ m_nCorpsStatus = nStatus; SetSaveStatus( CORPS_ALLINFOCHANGED ); }
	int	 GetCorpsStatus(){  return m_nCorpsStatus; }
	
	// 设置和获取军团解散时间
	void SetCorpsDisbandTime( int nDisbandTime ){ m_nCorpsDisbandTime = nDisbandTime; SetSaveStatus( CORPS_ALLINFOCHANGED ); }
	int	 GetCorpsDisbandTime( ){ return m_nCorpsDisbandTime; } 
	
	// 设置和获取公告
	void SetBulletin( const char *pBulletin )
	{ 
		strncpy( m_sBulletin, pBulletin, CORPS_BULLETIN_LENGTH*MAX_UTF8_BYTES-1 );
		m_sBulletin[CORPS_BULLETIN_LENGTH*MAX_UTF8_BYTES-1] = '\0';
		SetSaveStatus( CORPS_ALLINFOCHANGED );
	}
	const char *GetBulletin(){ return m_sBulletin; }
	
	// 设置&获取军团的存储状态
	int		GetSaveStatus(){ return m_nSaveStatus; }
	void	SetSaveStatus( int nStatus ){ m_nSaveStatus = nStatus; }


	// 获取&设置军团的pvp星级
	int		GetCorpsPvPStarLevel( ){ return m_nPVPStarLevel; }
	void	SetCorpsPvPSetrLevel( int nLevel ){ m_nPVPStarLevel = nLevel; }

	void SetUnionID( int nUnionID ) { m_UnionID = nUnionID; }
	int  GetUnionID(){ return m_UnionID; }

	void SetBossRefineValue(int nValue)
	{
		if (nValue < 0 )
		{
			m_BOSSRefineValue = 0;
		}else if( nValue > GetCorpsBossRefineValueLimit() )
		{
			m_BOSSRefineValue = GetCorpsBossRefineValueLimit();
		}else{
			m_BOSSRefineValue = nValue;
		}
		SetSaveStatus( CORPS_ALLINFOCHANGED );
	}
	int GetBossRefineValue(){ return m_BOSSRefineValue; }

	int GetBossCDCoolTime();

	int GetCorpsBossScore() { return m_nCorpsBossScore; }
	void SetCorpsBossScore( int nScore )
	{
		if ( nScore <= 0 )
		{
			m_nCorpsBossScore = 0;
		}
		else
		{
			m_nCorpsBossScore = nScore;
		}
		SetSaveStatus( CORPS_ALLINFOCHANGED );
	}

	int GetCorpsCollectActive() { return m_nCorpsCollectActive; }
	void SetCorpsCollectActive( int nActiveValue )
	{
		if ( nActiveValue <= 0 )
		{
			m_nCorpsCollectActive = 0;
		}
		else
		{
			m_nCorpsCollectActive = nActiveValue;
		}
		SetSaveStatus( CORPS_ALLINFOCHANGED );
	}

	void SetTmpUnionName( const char *pName )
	{ 
		if ( pName == NULL )
			return;
		strncpy( m_sTmpUnionName, pName, MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES -1 );
		m_sTmpUnionName[ MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES -1 ] = '\0'; 
	}
	const char *GetTmpUnionName( ){ return m_sTmpUnionName; }	

	void SetTmpUnionNotice( const char *pNotice )
	{ 
		if ( pNotice == NULL )
			return;
		strncpy( m_sTmpUnionNotice, pNotice, CORPS_BULLETIN_LENGTH*MAX_UTF8_BYTES -1 );
		m_sTmpUnionNotice[ CORPS_BULLETIN_LENGTH*MAX_UTF8_BYTES -1] = '\0'; 
	}
	const char *GetTmpUnionNotice( ){ return m_sTmpUnionNotice; }

	void AddTmpCorpsID( int vID )
	{
		for ( size_t i = 0; i < ARRAY_CNT(m_OtherCorpsID); i++ )
		{
			if ( m_OtherCorpsID[ i ] == vID )
				return;
			if ( m_OtherCorpsID[ i ] == 0 )
			{
				m_OtherCorpsID[ i ] = vID;
				return;
			}
		}
	}

	void ClearTempUnion(  )
	{
		memset( m_sTmpUnionName, 0, sizeof(m_sTmpUnionName) );
		memset( m_sTmpUnionNotice, 0, sizeof(m_sTmpUnionNotice) );
		memset( m_OtherCorpsID, 0, sizeof(m_OtherCorpsID) );
	}

	void SetInviteIdle( int bInvite ) { m_InviteIdle = bInvite; }
	bool  GetInviteIdle(){ return m_InviteIdle; }

	void SetForbidTime( int nForbidTime ){ m_nForbidTime = nForbidTime;  }
	int	 GetForbidTime( ){ return m_nForbidTime; }
	
	// 给军团中插入一个家族
	int InsertFamily( int nFamilyID, CCorpsFamiyInfo &sFamilyInfo );
	
	// 删除军团中的一个家族
	int DeletFamily( int nFamilyID );
	
	// 获取军团中的家族个数
	int GetFamilyNum(){ return m_tFamilyList.size(); }
	
	// 检查军团的家族个数是否已经满了
	bool CheckMemberFull(){ return m_tFamilyList.size() >= MAX_CORPS_FAMILYNUM;  }
	
	// 获取军团家族列表
	FamilyList & GetFamilyList(){ return m_tFamilyList; }
	
	// 检查列表中是否还有某个家族
	bool CheckHasFamily( int nFamilyID );
	
	// 检查申请列表中是否含有某个家族
	bool ChekcFamilyApply( int nFamilyID );
	
	// 插入一个家族的申请信息
	int InsertFamilyApply( int nFamilyID, int nApplyingID );
	
	// 删除一个家族的申请消息
	int DeletFamilyApply( int nFammilyID );
	
	// 判断申请列表是否已经满了
	bool CheckApplyListIfFull(){ return m_tApplyList.size() >= MAX_CORPSAPPLY_NUM; }	
	
	// 获得军团的人数
	int GetCorpsMemberNum();
	
	// 获取军团的成员信息列表
	void GetCorpsMemberList( PBCorpsMemberList &pbCorpsMember );
	
	// 获取军团的属性信息
	void GetCorpsBasicInfo( PBCorpsBasicInfo &pbBasicInfo );
	
	// 获取家族的申请者
	int GetApplyingRoleID( int nFamilyID );
	
	// 获得军团的申请列表
	int GetApplyList( PBCorpsApplyList &pbCorpsApply );
	
	// 设置军团成员信息的PB结构
	int CreatCorpsFamilyPBInfo( PBCorpsFamilyInfoList &pbCorpsFamilyInfo );
	
	// 从PB结构创建军团的家族成员信息
	int GetCorpsFamilyInfoFromPB( PBCorpsFamilyInfoList &pbCorpsFamilyInfo );

	// 从PB数据创建军团的发布信息
	int GetCorpsPublishInfoFromPB( PBCorpsPublishInfo &pbPublishInfo );

	// 创建军团发布信息的PB信息
	int CreateCorpsPublishDB(PBCorpsPublishInfo &pbPublishInfo);
	
	// 获取申请的数量
	int GetApplyNum( ){ return m_tApplyList.size(); }
	
	// 清除家族的军团信息
	int ClearCorpsInfoOfFamily( );
	
	// 检查军团解散时间是否已到
	bool CheckCorpsDisbandTime( );
	
	// 清空军团申请列表
	int ClearCorpsApplyList(){ m_tApplyList.clear(); return 0; }
	
	// 获取玩家的军团职位
	int GetMemberPost( int nMemberID, int &rPost );

	int MakeCorpsPbPropert( CorpsProperty* vpProperty );

	int InsertBattleMember( int nRoleID );
	int ClearBattleList();
	int GetBattleListSize();
	int GetBattleMemberList( int* vpList, int& nSize );
	bool IsInBattleList( int nRoleID );
	int DelBattleRole( int nRoleID );

	int HasOwnThisMap( int nMapID );

	// 获得npc福利的领取时间 
	int GetNpcWelfareTime( int nNpcID, int &rTime );
	
	// 设置npc福利的领取时间
	int SetNpcWelfareTime( int nNpcID, int nTime );

	int ClearNpcBattleInfo( int nNpcID );

	// 返回军团是否在战斗中，nStateType表示查询条件
	bool IsInBattle(int nBattleType, int nStateType);

	int GetPBBuffer( PBCorpsBattleList* npList, bool bAll = true );
	int SetFromPB( PBCorpsBattleList* npList );

	inline NpcStatus* GetNpcState(int _NpcID);
	
	int GetMapList( int* npMapList, int& nNum );
	int GetMap( int nNpcID );
	

	//设置本军团战争列表，已失效from 2010.6.13
	int SetNpcBattleStatus( int nNpcID, int nNpcStat, int nWarStat ) {return 0;};

	// 军团是否可以升级
	bool CanCorpsLevelUP( );
	
	// 获取升级需要的金钱和荣耀
	int GetLevelCondition( int &rMoney, int &rGlory );

	// 是否是大唐
	inline bool IsCoreOwner( ) { return m_nBattleCity == COUNTRY_MAP_CHANGAN; }

	// 是否是一般占领
	inline bool IsNormalOwner( )
	{ 
		return ( m_nBattleCity == COUNTRY_MAP_WUJI || m_nBattleCity == COUNTRY_MAP_CHECHI 
			|| m_nBattleCity == COUNTRY_MAP_BAOXIANG || m_nBattleCity == COUNTRY_MAP_NVER ) ;
	}

	// 是否是执政军团
	inline bool IsOwner( ) { return IsCoreOwner( ) || IsNormalOwner( ); }	

	// 是否可以竞价 [12/12/2009 Macro]
	bool CanBidThisMap( int nMapID );
	
public:	
	// 家族升级
	int	LevelUPCorps( int &rLevel, int &rMoney );
	
	// 职位设置
	int SetMemberPost( int nRoleID, int nPost, int &rOldPost );
	
	// 增加军团荣耀
	int AddCorpsGlory( int nValue, int nType = 0 );
	
	// 减少军团荣耀
	bool ReduceCorpsGlory( int nValue, int nType = 0 ); 
	
	// 捐献军团金钱
	int ContributeCorpsMoney( int nMoney, int nType = 0 );
	
	// 增加pvp积分
	bool AddPvPScore( int nValue, int nFailedCorpsLevel );
	
	// 减少pvp积分
	bool ReducePvPScore( int nValue, int nWinCorpsLevel );
	
	// 判断某个玩家是否有权限
	bool HasPost( int nRoleID, int nCorpsPost );
	
	// 获取某个职位成员列表
	int GetMemberListOfPost( int *pList, int& rNum, int nPost, int nType );
	
	// 消耗军团金钱
	int ConsumeCorpsMoney( int nMoney, int nType = 0 );
	
	// 获取某个职位的个数
	int GetPostNum( int nPost );
	
	// 获取某个职位的人数上限
	int GetPostLimitNum( int nPost );
	
	// 增加军团成员的贡献度
	int AddCorpsMemberContribute( int nMemberID, int nContribute );
	
	// 增加军团成员的贡献度
	int SetCorpsMemberName( int nMemberID, const char *pName );
	
	// 设置玩家的贡献度
	int SetCorpsContribute( int nMemberID, int nContribute );

	// 挑战列表处理
	
	// 检查是否有某个成员
	bool HasMember( int nRoleID );
	
	// 获取个人的军团贡献度
	int GetMemberContribute( int nRoleID );
	
	// 设置&获取家族成员的经验
	int	GetMemberExp( int nRoleID );

	// 增加成员的经验 
	void AddMemberExp( int nRoleID, int nExp ); 

	// 设置成员经验
	void SetMemberExp( int nRoleID, int nExp ); 

	// 获取成员AP值
	int GetMemberAP( int nRoleID );

	// 设置成员AP值
	void SetMemberAP( int nRoleID, int nAP );

	// 增加成员AP
	void AddMemberAP( int nRoleID, int nAP );
	
	// 某个职位是否有某项权利
	bool PostHasRight( int nPost, int nRight );
	
	// 成员是否含有某项权利
	bool MemberHasRight( int nRoleID, int nRight );
	
	// 列出有某项权的玩家列表
	int ListMemberOfRight( int *pIDList, int *pPostList, int *pContributeList, int &rNum, int nRight );
	
	// 获取玩家的家族
	CFamily *GetMemberFamily( int nMemberID );

	// 获取军团发布信息

	CGroupPublishInfo *GetCorpsPublishInfo(){ return &mCorpsPublishTask;}
	
	// 初始化家族列表
	int InitialOrgnization(  );
	
	// 检查金钱是否足够
	bool CheckMoneyEnough( int nMoney );
	
	// 检查军团是否能够使用某个商店
	int CheckCorpsCanUseShop( int nNpcTmpID, int nWarCityMap );
	
	// 获取使用商店错误码
	int GetCorpsShopErrcode( int nErrcode, int &rShopsErrcode );

	// 国家类称号处理
	int UpdateNationTitle( int nMapID, bool bIsAdd );

	// 得到军团势力值
	int GetPower();
	

	//结构化存储NPC竞价信息到DB，DLC@2010-06-07
	//int SaveBattleBidListToDB();
	//int LoadBattleBidListFromDB();


	int CheckIfHasBidRight(int _RoleID, int _MapID);

	//添加bosstimes
	void ChangeCorpsBossTimes(int nBossID, int nTimes );

	//
	int GetCorpsBossTimes( int nBossID );

	//
	int GetCorpsBossRefineValueLimit( );

public:
	template< typename TYPE_T >
	void AddChallege( TYPE_T& nList, int nCorpsID )
	{
		nList.push_back( nCorpsID );
	}

	template< typename TYPE_T >
	void ClearChallege( TYPE_T& nList )
	{
		nList.clear();
	}

	template< typename TYPE_T >
	int GetChallegeList( TYPE_T& nList,  int* vpList, int& nSize )
	{
		if ( vpList == NULL )
		{
			return -1;
		}

		int tSize = nSize;
		nSize = 0;

		ChallegeList::iterator it = nList.begin();
		for ( ; it != nList.end() && nSize < tSize; ++ it )
		{
			vpList[ nSize ++ ] = *it;
		}
		return 0;
	}

	template< typename TYPE_T >
	void DelChallege( TYPE_T& nList, int nCorpsID )
	{
		ChallegeList::iterator it = nList.begin();
		for ( ; it != nList.end(); ++ it )
		{
			if ( *it == nCorpsID )
			{
				nList.erase( it );
				break;
			}
		}
		return ;
	}

	template< typename TYPE_T >
	bool InChallege( TYPE_T& nList, int nCorpsID )
	{
		ChallegeList::iterator it = nList.begin();
		for ( ; it != nList.end(); ++ it )
		{
			if ( *it == nCorpsID )
			{
				return true;
			}
		}
		return false;
	}
};



