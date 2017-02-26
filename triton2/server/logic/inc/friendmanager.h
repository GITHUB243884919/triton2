#pragma once

#include "friendtype.h"
#include "westserver.h"
#include "property_pb.hxx.pb.h"
#include "servercore.h"
#include "teamtype.h"
struct st_settings
{
	unsigned char nIsRefuseAllAddInf;  // 是否自动拒绝所有的添加好友信息
	EPlayerShow   euMyStatus;		   // 我的状态
	char          szGroupName[PLAYER_GROUP_COUNT + 1][GROUP_NAME_LENGTH];	   // 4个组名 下标访问！！
	bool		  bIstoSave;			// 有发生改变，要存入服务器
	unsigned int  nLastRefrershTime;	// 上次刷新好友时间
};

struct st_friend
{
	//char		  szName[ NAME_LENGTH ]; // 名字，临时存
	int			  nCharID;
	unsigned char nGroupId;   // 所在组的ID (0=默认分组)
	EPlayerShow   enStatus;   // 状态，包括是否在线
	EPlayerRels   enRel2Me;   // 和我的关系
	int			  nIntimacyDegree; // 好友亲密度	
	int			  nLastTimeAllOnline; // 最后一次同时在线的时间点
	int			  nDailyIntimacy;	// 当天组队获取的亲密度
	int			  nLastUpdateTime;	// 最后一次更新的时间
	st_friend( )
	{
		if (CObj::msCreateMode)
		{ enStatus = AT_OFFLINE ; }
		nIntimacyDegree = 0;
		nLastTimeAllOnline=0;
	}
	st_friend( unsigned int vPlayerId )
		: nCharID( vPlayerId ) , enStatus(AT_OFFLINE)
	{}

	bool operator < ( const struct st_friend& other )  const
	{
		return nCharID < other.nCharID;
	}
	
	int AddIntimacy( int nIntimacy, int nMaxIntimacy )
	{
		if ( IsANewDay( nLastUpdateTime, time(NULL) ) == true )
		{
			nDailyIntimacy = 0;
		}		
		if ( nDailyIntimacy + nIntimacy > nMaxIntimacy )
		{
			nIntimacy = nMaxIntimacy - nDailyIntimacy;			
		}	
		if ( nIntimacy > 0 )
		{
			nIntimacyDegree += nIntimacy;
			nDailyIntimacy += nIntimacy;
		}			
		nLastUpdateTime = time( NULL );
		return nIntimacy;
	}
};

struct st_enemy
{
	unsigned int   nPlayerId;
	unsigned int   nKilled;
	unsigned int   nBeKilled;
	unsigned char  nIsLocked;
	unsigned char  nState;	// 在线状态 只有在线和不在线
	unsigned long  nKillTime;  // 时间戳

	st_enemy()
	{	
		if (CObj::msCreateMode)
		{ nPlayerId = 0; nKilled = 0; nBeKilled = 0; nIsLocked =  0 ; nState = 0; nKillTime = 0; }
	}
	st_enemy(unsigned int vPlayerId, unsigned short vKilled = 0,  unsigned short vBeKilled = 0, unsigned char vIsLocked = 0
		, unsigned char vState = 0, unsigned long vKillTime = 0 )
		: nPlayerId(vPlayerId), nKilled(vKilled), nBeKilled(vBeKilled), nIsLocked( vIsLocked ), nState(vState), nKillTime(vKillTime) {}
	bool operator < ( const struct st_enemy& other )  const
	{
		return nPlayerId < other.nPlayerId;
	}
};

// 为了快速找到哪个仇敌该干掉了 用时间戳索引仇敌
struct st_enemy_map
{
	unsigned long  nKillTime;			// 时间戳
	int			   nCharID;				// 仇敌ID

	st_enemy_map()
	{
		if (CObj::msCreateMode)
		{ nCharID = 0; nKillTime = 0; }
	}

	st_enemy_map( unsigned long vKillTime, int vCharID )
		: nKillTime(vKillTime), nCharID(vCharID)
	{	}

	bool operator < ( const struct st_enemy_map& other )  const
	{
		return (nKillTime == other.nKillTime)?(nCharID < other.nCharID):(nKillTime < other.nKillTime);
	}
};


class CFriendItem
{
private:
	CSearchArray<st_friend , FRIENDS_LIMIT> mFriends;     // 我的好友列表   用charid
	CSearchArray<int , BLACKLIST_LIMIT>		mBlacks;      // 我的黑名单列表 用charid
	CSearchArray<st_enemy , ENEMIES_LIMIT>	mEnemys;      // 我的仇敌列表   用charid
	CSearchArray<st_enemy_map , ENEMIES_LIMIT>	mEnemyKillTimeMap;      // 仇敌根据时间索引列表
	int										mBlessTimes;  // 好友祝福升级的次数
	st_settings		 mSettings;		// 我的设置
	typedef lk::vector<int, LATEST_CONTACT_MAX> LatestContactType;
	LatestContactType m_LatestContactList;  // 最近联系人

public:
	CFriendItem( )
	{
		if (CObj::msCreateMode)
		{ 
			mSettings.nIsRefuseAllAddInf = 0;
			mSettings.euMyStatus = AT_ONLINE;
			mSettings.bIstoSave  = false;
			mSettings.nLastRefrershTime = 0;
			memset( mSettings.szGroupName, 0, (PLAYER_GROUP_COUNT + 1)*GROUP_NAME_LENGTH );
			mBlessTimes = 0;
		}
		m_LatestContactList.initailize();
	}

public:
	// 所有操作，成功的话返回0，否则错误码 取值不限
	int		AddFriend( int nSrcCharID, EPlayerShow enStat = AT_ONLINE, EPlayerRels enRel = YOUR_FRIEND ) ;
	int		AddFriend( int nSrcCharID, EPlayerShow enStat, EPlayerRels enRel, short nGroupID ) ;

	int		DelFriend( int nSrcCharID ) ;
	int		MoveFriend( int nCharID, int nDesGroupID );

	int		AddBlack( int nSrcCharID ) ;
	int		DelBlack( int nSrcCharID ) ;
	bool	IsBlack( int nSrcCharID );

	unsigned char	GetRefuseAll( ) const;
	unsigned int	GetLastRefresh( ) const;
	int		SetLastRefresh( unsigned int vTime );

	int		SetMyStatus( EPlayerShow enStat );
	EPlayerShow  GetMyStatus();

	// 改变好友状态
	int		SetFriendStatus( int nSrcCharID, EPlayerShow enStat ); 

	int		GetFriendCharIDList( std::vector<int>& rList, bool bIsOnlyOnline = false ) const;
	int		GetBlackCharIDList( int* pBlacks, int& nNum ) const ;

	//int		GetAllFriendsInfo( Friend_Map& rList );
	const char*	GetGroupName( unsigned char nGroupID ) const;
	int		GetFriend( int nCharID, st_friend& rFriend );

	// 是不是好友
	bool	IsFriend( int nCharID );
	bool	IsEnemy( int nCharID );

	// 基本信息操作
	int		GetFriendNum( ); 
	//int		GetBlackNum( );

	//bool	IstoSave();
	//void	SetIstoSave();

	// 处理仇敌，如果是已经存在的仇敌，返回true
	int		OnBeKilled( int nCharID, int& nBeKilled, int& nKilled, int& nOutDelCharID );

	bool    OnKilled( int nCharID, int& nKillHim, int& nBeKilled );

	void    SetEnemyState( int nCharID, unsigned char nState );
	int		GetEnemyCharIDList( std::vector<int>& rList, bool bIsOnlyOnline = false ) const ;

	// 得到仇敌
	int		GetEnemy( int nCharID, st_enemy& rFriend );
	int		DelEnemy( int nSrcCharID );

	void	GetTlvBuffer2( PBFriend* pBuffer );
	void	CreateFromTlv2( PBFriend* pBuffer );

	int		LockEnemy( int nDesCharID, int nLockType );
	int		SetGroupName( unsigned char nGroupID, const char* szGroupName );
	void	SetBlessTimes( int nTimes ){ mBlessTimes = nTimes; }
	int		GetBlessTimes() { return mBlessTimes; }	
	st_friend *GetFriendByRoleID( int nRoleID );
	// 增加最近联系人
	int		AddLatestContacter(int nCharID);
	int		GetContacterCount();
	int		GetContacter(int nIndex);
	
	int		UpdateLastTimeAllOnLine( int nRoleID, int nTime );	// 更新所有好友的最后一次同时在线时间
	int		UpdateIntimacyDegree( int nRoleID, int nValue ); // 设置某个好友的亲密度
	int		GetFriendIntimacyDegree( int nRoleID );	// 获取某个好友的亲密度
	int		GetLastTimeAllOnLine( int nRoleID );	// 获取和某个好友的最后一次共同在线时间
private:
	int		SetRefuseAll( unsigned char nFlag ) ;
	int		GetEarlyestEnemyNoLocked();			
};

// 好友升级祝福配置信息
struct CBlessExpInfo
{
public:
	int mTimes;			// 多少次数以内
	int mOgreNum;		// 奖励经验的百分比
public:
	CBlessExpInfo( )
	{
		mTimes = 0;
		mOgreNum = 0;
	}
};
struct CMarriageIntimacy
{
public:
	int mIntimacy;	// 结婚减少道具需求需要的亲密度
	int mItemReduced;// 因为亲密的可以减少的道具数量
public:
	CMarriageIntimacy()
	{
		mIntimacy = 0;
		mItemReduced = 0;
	}	
};

#define MAX_MARRIAGEINTIMACY_NUM 20
struct CIntimacyConfig
{
public:
	int mOffLineDays;
	int mDayReduced;
	int mKillReduced;
	int mDisbandReduced; // 解散结拜减少的亲密度
	int mLeaveReduced;	 // 离开结拜减少的亲密度
	int	mKickReduced;
	int mSwornIntimacy;	// 结拜需要的亲密度	
	int	mTeamIntimacyTime; // 每隔多少秒队伍玩家的亲密度增加一点
	int mMaxIntimacy;	// 亲密度上限	
	int mDivorceReduced;// 因为离婚而减少亲密度	
	int mSwornTitleID;	// 称号ID
	int mItemCreateSworn;	// 创建结拜需要的卷轴
	int mItemDisbandSworn; // 解散需要的卷轴
	int mItemKickMember;  // 踢人需要的卷轴
	int mItemChangeName;  // 改名道具
	int mMaxDailyDegree;  // 每天获取亲密度的上限
	int mRebuildItemID;	  // 加入结拜的道具ID
	CMarriageIntimacy mMarriageIntimacy[MAX_MARRIAGEINTIMACY_NUM];
public:
	CIntimacyConfig()
	{
		mOffLineDays = 3;
		mDisbandReduced = 20;
		mKickReduced = 10;
		mDisbandReduced = 20; // 百分比
		mLeaveReduced = 20;	 
		mKickReduced =10;
		mSwornIntimacy=100;
		mTeamIntimacyTime = 30;
		mDayReduced = 4;
		mMaxIntimacy = 5000;
		mKillReduced = 30;		
		mDivorceReduced = 200;
		mSwornTitleID = 0;
		mItemCreateSworn = 0;
		mItemDisbandSworn = 0;
		mItemKickMember = 0;
		mItemChangeName = 0;
		mMaxDailyDegree = 30;
		mRebuildItemID = 0;
	}
public:
	int InsertMarriageIntimacy( int nIntimacy, int nItemNum )
	{
		for ( int i = 0; i < ARRAY_CNT( mMarriageIntimacy ); ++i )
		{
			if ( mMarriageIntimacy[i].mIntimacy == 0 )
			{
				mMarriageIntimacy[i].mIntimacy = nIntimacy;
				mMarriageIntimacy[i].mItemReduced = nItemNum;
				break;
			}													
		}
		return SUCCESS;
	}
	int GetReducedNumOfMarriageItem( int nIntimacy )
	{
		int tNum = 0;
		for ( int i = 0; i < ARRAY_CNT( mMarriageIntimacy ); ++i )
		{
			if ( nIntimacy >= mMarriageIntimacy[i].mIntimacy && mMarriageIntimacy[i].mItemReduced > tNum )
			{
				tNum = mMarriageIntimacy[i].mItemReduced;
			}													
		}
		return tNum;
	}		  
	int InitializeMarriageIntimacy()
	{
		memset( mMarriageIntimacy, 0, sizeof( mMarriageIntimacy ) );
		mItemCreateSworn = 0;
		mItemDisbandSworn = 0;
		mItemKickMember = 0;
		mItemChangeName = 0;
		return 0;
	}
};
class CFriendBlessConfig
{
public:
	int mLevelInterval;		// 没多少级给好友发一次祝福
	int mOgreNum;			// 给好友多少只同等级怪的经验
	int mOgreExpTableID;	// 怪物等级经验的模板ID			
	CBlessExpInfo mBlessExpInfoList[MAX_FRIENDBLESS_NUM];
	int mLevel;				// 多少级以上有祝福
	CIntimacyConfig mIntimacyConfig;
public:
	void SetLevelInterval( int nLevelInterval ){ mLevelInterval = nLevelInterval; }
	int GetLevelInterval(){ return mLevelInterval; }

	void SetOgreNum( int nNum ){ mOgreNum = nNum; }
	int GetOgreNum(){ return mOgreNum; }
	
	void SetOgreExpTableID( int nID ) { mOgreExpTableID = nID; }
	int GetOgreExpTableID(){ return mOgreExpTableID; }

	void SetLevel( int nLevel ){ mLevel = nLevel; }
	int GetLevel(){ return mLevel; }
public:
	int Initialize()
	{
		mLevelInterval = 0;
		mOgreNum = 0;
		mOgreExpTableID = 0;
		memset( mBlessExpInfoList, 0, sizeof( mBlessExpInfoList ) );
		return 0;
	}	
	int Resume(){ return 0; }
	CFriendBlessConfig()
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
public:
	int InsertBlessExpInfo( int nTimes, int nOgreNum );
	int GetBlessExpInfo( int nTimes );
	CIntimacyConfig *GetIntimacyConfig(){ return &mIntimacyConfig; }
};

// 结拜
struct CSwornMember 
{
public:
	int mRoleID;		//	角色ID
	int mTeamPos;		//	结拜中排序
	int mRoleStatus;
public:
	CSwornMember()
	{
		mRoleStatus = 0;
		mRoleID = 0;
		mTeamPos = 0;
	}
};

// 结拜团体
class CSwornTeam
{
public:
	enum
	{
		EROLE_STATUS_OFFLINE = 0,
		EROLE_STATUS_ONLINE = 1,
	};
public:
	typedef lk::vector< CSwornMember, TEAM_NUMBER > SwornMemberList;
public:
	CSwornTeam()
	{
		if ( CObj::msCreateMode )
		{
			Initialize();	
		}			
	}
	~CSwornTeam(){}
public:
	int GetSwornID(){ return mSwornID; }
	void SetSwornID( int nSwornID ){ mSwornID = nSwornID; }
	const char * GetSwornName(){ return mSwornName; }
	void SetSwornName( const char *pName )
	{
		if ( pName != NULL )
		{
			memset( mSwornName, 0, sizeof( mSwornName ) );
			strncpy( mSwornName, pName, sizeof(mSwornName) - 1 );
		}		
	}
	int InsertMember( CSwornMember &rMember )
	{
		mSwornMemberList.push_back( rMember );
		return 0;
	}
	int GetCampID(){ return mCampID; }
	void SetCampID( int nCampID ){ mCampID = nCampID; }
	int Initialize();
	int KickMember( int nRoleID, int nKickedRoleID );
	int LeaveSwornTeam( int nRoleID );
	int DisbandSwornTeam( int nRoleID );
	bool HasMember( int nRoleID );
	int DeleteMember( int nRoleID );	
	int FindFirstPos();	// 按排序选出老大
	int GetTeamPos( int nRoleID );
	int ChangeRoleStatus( int nRoleID, int nStatus );
	int GetOnLineNum( );
	int SetFromPB( PBSwornTeam &rSwornTeam );
	int CreateTlvPB( PBSwornTeam &rSwornTeam );
	int GetRoleIDList( int *pRoleIDList, int &rNum );
	int GetNumber(){ return mSwornMemberList.size(); }
	SwornMemberList *GetSwornMember(){ return &mSwornMemberList; }
	int GetMaxPos();	// 获取最后一位结拜位置
	int ChangePos( );// 因为离开而改变排位
public:
	char mSwornName[NAME_LENGTH];		// 结拜团体名称
	SwornMemberList mSwornMemberList;	// 结拜成员列表
	int	mSwornID;						// 结拜团体ID(首领角色ID)
	int mCampID;					// 阵营
};