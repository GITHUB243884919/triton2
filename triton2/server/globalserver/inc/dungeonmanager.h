#pragma once
#include "dungeontype.h"
#include "servertool.h"
#include "lk_hashmap.h"
#include "lk_assert.h"
#include "lk_vector.h"
#include "object.h"
// 报名信息
struct CRoleInfo
{
public:
	enum ERoleStatus
	{
		ESTATUS_NORMAL	= 0,	// 正常
		ESTATUS_OFFLINE = 1,	// 离线状态,准备踢出
		ESTATUS_WAIT    = 2,	// 等待进入副本状态,准备踢出
	};
public:
	int mRoleID;
	int mGameID;
	int mTeamFlag;		
	int mRoleStatus;
	int mOffTime;				// 离线的时间
public:
	int Initialize()
	{
		mRoleID = 0;
		mGameID = 0;
		mTeamFlag = 0;
		mRoleStatus = ESTATUS_NORMAL;
		mOffTime =0;
	}
public:
	CRoleInfo()
	{
		Initialize();
	}
};

// 战场注册信息
class CWarRegInfo : public CObj
{
public:
	CRoleInfo mRoleInfo[MAX_WAR_PLAYER_NUM];// 报名的角色ID	
	KEY_TYPE mReptionKey;					// 副本key
	int mRoleNum;							// 分组中的人数
	int mLevelID;							// 根据等级所在的等级ID
	int mCreateTime;						// 注册信息的创建时间
	int mServerKey;							// 分配到场景的ID(根据ID获取场景信息)
	int mRegKey;							// 注册的key
	int mStatus;							// 状态	
	int mServerListID;						// 服务器区组ID
public:
	enum EWarStatus
	{
		EWARSTATUS_INITIAL			= 0,	//	初始状态
		EWARSTATUS_CREATEREPTION	= 1,	// 创建副本状态
		EWARSTATUS_START			= 2,	// 战场开始阶段
		EWARSTATUS_BATTLEEND		= 3,	// 战斗结束，战场关闭倒计时阶段
	};
public:
	CWarRegInfo()
	{
		if ( CObj::msCreateMode )
		{
			Initialize();
		}
		else
		{
			
		}
	}
public:
	int Initialize()
	{
		memset( mRoleInfo, 0, sizeof( mRoleInfo ) );
		mRoleNum = 0;
		mReptionKey = 0;
		mLevelID = 0;
		mCreateTime = 0;
		mServerKey = 0;
		mRegKey = 0;
		mStatus = EWARSTATUS_INITIAL;
		mServerListID = 0;
		return 0;
	}
	int Resume(){ return 0; }
public:
	enum ETeamFlag
	{
		ETEAMFLAG_ONE = 1,	// 队伍1
		ETEAMFLAG_TWO = 2,	// 队伍2
	};
public:
	bool IsRepetionStart(){ return mReptionKey > 0; }
	void ClearReption(){ Initialize(); }
	bool IsFull( int nStartNum ){  return mRoleNum >= ARRAY_CNT( mRoleInfo ) || mRoleNum >= nStartNum; } 
	int InsertRole( CRoleInfo &rRoleInfo ); 	
	CRoleInfo *GetRoleInfo( int nRoleID );		
	int DeleteRoleInfo( int nRoleID );
	int AssignTeam( int nStartNum );	// 把队伍自动分成两拨,战场开始的时候进行(各自1/2 WAR_START_NUM)
	int GetTeamNum( ETeamFlag eFlag, int nStarNum ); // 获取某一分队的人数
	ETeamFlag GetTeamFlag( int nStartNum );	// 根据人数获取组队标识
	bool HasRole( int nRoleID );			// 判断注册信息中是否有该角色	
	int ChangeRoleStatus( int nRoleID, int nStatus );	// 改变玩家状态
	void RemoveTimeOutRole( int nMaxOfflineTime, int nMaxWaitTime,std::vector<int> &rRoleIDList );	// 删除离线时间超过最大离线时间的战场玩家		
	bool InProtecTime( int nProtectTime );		// 检查是否在副本保护时间内
}; 

// 战场信息管理
enum EWarRegStatus
{
	EWARREG_STATUS_READY = 0,	// 准备阶段
	EWARREG_STATUS_START = 1,	// 战场已经开始	
};


struct CWaitPlayer  
{
public:
	int mLevelID;
	int mRoleID;
	int mGameID;
public:
	CWaitPlayer()
	{
		mLevelID = 0;
		mRoleID = 0;
		mGameID = 0;
	}
};

// 战场信息(每个全局副本一个manager)
class CWarRegInfoManager
{	
public:
	enum 
	{
		EMANAGER_STATUS_UNUSED = 0,	// 未被使用状态
		EMANAGER_STATUS_USED = 1,	// 被使用状态
	};
public:
	CWarRegInfoManager()
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
	~CWarRegInfoManager(){ }
public:

	// 每个等待队列增加一个时间戳，存活时间为最大活动时间
	class WaitPlayer : public lk::vector< CWaitPlayer, MAX_PLAYER_INLIST >
	{
	private:
		int mCreateTime;					
	public:
		bool TimeOut( int nMaxTime )
		{
			return time( NULL ) - mCreateTime > nMaxTime;
		}
		int GetCreateTime(){ return mCreateTime; }
		void SetCreateTime( int nTime ){ mCreateTime = nTime; }		
		void GetRoleIDList( std::vector<int> &rRoleIDList )
		{			
			for ( iterator it = begin(); it != end(); ++it )
			{
				rRoleIDList.push_back( it->mRoleID );
			}
		}
	public: 
		WaitPlayer()
		{
			mCreateTime = time(NULL);			
		}
	};	
public:	
	typedef lk::vector< int, MAX_WARREGINFO_INLIST > WarRegInfoList;
public:	
	typedef lk::hash_map< int, WaitPlayer, MAX_LEVLESECTION_NUM > WaitPlayerSet;	 // 每个等级段的等级列表	
	typedef lk::hash_map< int, WarRegInfoList, MAX_LEVLESECTION_NUM > WarRegInfoSet; // 每个等级段开启的战场
	typedef lk::vector< int, MAX_WARNUM_CANSTART > WarEnableList;					  // 报名人数满可以开启的战场信息列表
public:
	int Initialize();		// 初始化	
	int Resume(){ return 0; }	
	CWarRegInfo* FindWarRegInfoWithoutFull( int nLevelID ); // 获取某种类型人数未满的战争注册信息
	int RegRoleInfo( CRoleInfo &rRoleInfo, int &rRegKey, int nLevelID );	// 注册某个角色信息到战场
	CWarRegInfo *GetWarRegInfo( EWarRegStatus eStatus, int nLevelID );	// 获取某个等级段的战场信息	
	CWarRegInfo *CreateWarRegInfo(  );									// 创建一个战场的注册信息
	int DeleteWarRegInfo( int nRegKey );								// 注销掉已经结束的战场信息
	CWarRegInfo *GetWarRegInfoByRoleID( int nRoleID );					// 获取某个角色的战场注册信息
	CWarRegInfo *GetWarRegInfoByKey( KEY_TYPE nRepetionKey );			// 根据副本的key获取战场注册信息
	CWarRegInfo *GetWarRegInfoByRegKey( int nKey );						// 根据注册的key值获取注册信息
	int FetchRoleIntoWarFromWaitlist( CRoleInfo *pRoleInfo, int *pWarRegKey, int &rNum ); // 把角色从等待队列移动到战场
	int CheckWarRegInfo( int nWarTimeInterval );		// 检查是否有可以开启的战场
	CWarRegInfo *DeleteRoleWarRegInfo( int nRoleID );					// 删除某个角色的注册信息
	bool CheckApply( int nRoleID );										// 检查角色是否已经报名
	bool InWaritList( int nRoleID );									// 是否在等待列表里面		
	bool DeleteFromWaitList( int nRoleID );								// 从队列中删除某个玩家的报名信息
	int InsertWaitPlayer2List( CWaitPlayer &rPlayer );	// 插入一个角色信息到列表中
	int InsertWarRegInfo2List( CWarRegInfo *pWarRegInfo );	// 插入一个角色信息到列表中
	int GetWarNumOfCertainLevel( int nLevelID );						// 获取某个等级段开启的战场数目	
	WarRegInfoList* GetWarRegInfoListByLevelID( int nLevelID );		// 获取某个等级段的战场信息队列
	WaitPlayer* GetWaitPlayerListByLevelID( int nLevelID );		// 获取某个等级段的等待队列信息			
	int DeleteWarRegInfoListByLevelID( int nLevelID );					// 根据等级ID删除一个战场队列
	int DeleteWaitPlayerListByLevelID( int nLevelID );					// 根据等级ID删除一个等待队列
	int EraseWarInfoFromList( WarRegInfoList &rList, int nWarRegInfoID ); // 清除某个战场的注册信息
	bool InWaitList( WaitPlayer &rList, int nRoleID );					 // 
	bool InWaitList( int nRoleID );					 // 检查列表中是否有某个角色信息
	int RemoveTimeOutRole( int nMaxOfflineTime, int nMaxWaitTime, std::vector<int> &rRoleIDList );		 // 删除超时的玩家
	void DeleteValidWarRegInfo( int nMaxWarAliveTime, int nProtectTime, std::vector<int> &rDeleteWarInfo ); // 删除过期的活动时间
	void DeleteTimeOutWaitList( int nMaxTime, std::vector<int> &rRoleIDList );
	bool IsManagerFree();							// 判断管理器是否空闲来回收使用
public:
	void SetLastWarStartTime( int nTime ){ mLastWarStartTime = nTime; }
	int GetLastWarStartTime(){ return mLastWarStartTime; }	
	void SetWarStartNum( int nNum ){ mWarStartNum = nNum; }				// 设置开启战场需要的人数
	int GetWarStartNum(){ return mWarStartNum; }						// 设置开启战场需要的人数	
	WarEnableList& GetWarEnableList(){ return mWarEnableList; }			// 获取可以开启的战场列表
	void SetWarManagerStatus( int nStatus ) { mWarManagerStatus = nStatus; } // 设置战场管理器的状态
	int GetWarManagerStatus(){ return mWarManagerStatus; }				// 获取战场管理器的状态
	void SetRepetionIndex( int nIndex ){ mRepetionIndex = nIndex; }		// 设置副本索引
	int GetRepetionIndex(){ return mRepetionIndex; }					// 获取副本索引
	void SetServerListID( int nID ){ mServerListID = nID; }				// 设置服务器列表ID
	int GetServerListID(){ return mServerListID; }						// 获取服务器列表ID
private:	
	int mRepetionIndex;						// 副本索引
	int mServerListID;						// 服务器列表ID
	int mWarStartNum;	
	int mLastWarStartTime;					// 最后一次战场的开启时间
	int mWarManagerStatus;					// 
	WaitPlayerSet mWaitPlayerSet;			// 等待队列中的玩家				
	WarRegInfoSet mWarRegInfoSet;			// 战场注册信息
	WarEnableList mWarEnableList;			// 报名人数满可以开启的战场信息列表
};

// 用于建立副本的每个服务器的信息
struct CMapInfo  
{
public:
	int mMapID;
	int mPosX;
	int mPosY;
public:
	CMapInfo()
	{
		mMapID = 0;
		mPosX = 0;
		mPosY = 0;
	}	
};
struct CGlobalServerInfo
{
public:
	int mServerType;	// 服务器类型
	int mWorldID;		// 服务器的游戏id
	int mLineID;		// 所在区组的serverid
						// 电信

	CMapInfo mGlobalMapInfo;	// 要到达的全局服务器的地图信息
	CMapInfo mOriginalMapInfo;	// 原始服务器的地图信息
	lk::CString64 mAddr;// 服务器地址
	int mReptionIndex;	//副本索引
public:
	CGlobalServerInfo()
	{
		mServerType = 0;
		mWorldID = 0;
		mLineID = 1;		
		mAddr.clear();
		mReptionIndex = 0;
	}
};
struct CGameConfig
{
public:
	int mServerListID;
};

class CGlobalServerConfig
{
public:
	typedef lk::hash_map< int, CGlobalServerInfo, MAX_GLOBALSERVER_NUM > GlobalServerInfolist;
	typedef lk::hash_map< int, CGameConfig, MAX_GAMECONFIG_NUM > GameConfigList;
public:
	CGlobalServerConfig()
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
	~CGlobalServerConfig(){}
	int Initialize();
	int Resume(){ return 0; }
	int InitializeServerList();
	int InitializeGameConfig();
public:
	int InsertGlobalServerInfo( int nServerListID, int nKey, CGlobalServerInfo &rServerInfo );
	CGlobalServerInfo *GetGlobalServerInfo( int nKey, int &rServerListID );
	
	// 服务器配置信息
	int InsertGameConfig( int nWorldID, CGameConfig &rConfig );
	CGameConfig *GetGameConfig( int nWorldID );	
	int SetWarStartNum( int nServerListID, int nNum );
	int GetWarStartNum( int nServerListID );
public:
	GlobalServerInfolist mGlobalServerInfoList[MAX_GLOBAL_SERVERLIST_NUM];
	int mWarStartNum[MAX_GLOBAL_SERVERLIST_NUM];
	GameConfigList mGameConfigList;
};

// 管理每个服务器的负载情况
struct CServerStatus
{
public:	
	int mRoleNum;			// 在场景中的人数
	int mWarNum;			// 开启战场数量		
};
class CServerManager
{
public:
	typedef lk::hash_map< int, CServerStatus, MAX_GLOBALSERVER_NUM > ServerStatusList;
public:
	CServerManager()
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
	~CServerManager(){ }
	int Initialize();
	int Resume(){ return 0; }
public:
	enum ESlectType
	{
		ESELECT_TYPE_BYWARNUM = 0,
		ESELECT_TYPE_BYROLENUM = 1,
	};	
public:
	int SelectServerWithMinNum( int nServerListID, EServerType eServerType, ESlectType eType, CGlobalServerConfig &rConfig, int nRepetionIndex = 0 );
	int ChangeServerSatus( int nServerListID, int &rKey, int nRoleNum, int nWarNum, CGlobalServerConfig &rConfig, ESlectType eSelectType = ESELECT_TYPE_BYWARNUM, EServerType eServerType = ESERVERTYPE_DUNGEON );
	CServerStatus *GetServerStatus( int nKey );
public:
	ServerStatusList mServerStatusList;
};

// 等级段的配置
struct CLevelSection
{
public:
	int mMinLevel;	 
	int mLevelID;
	int mMaxWarNum;	// 可以同时开启的最大战场数目
public:
	CLevelSection()
	{		
		mMinLevel = 0;
		mLevelID = 0;
		mMaxWarNum = 20;
	}
};

class CLevelConfig
{
public:
	CLevelConfig()
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
	~CLevelConfig(){} 
public:
	int Initialize();
	int Resume(){ return 0; }
	void SetMaxLevel( int nLevel ) { mMaxLevel = nLevel; }
	int GetMaxLevel(){ return mMaxLevel; }
	int InsertLevelSection( CLevelSection &rLevelSection );
	int GetIDByLevel( int nLevel );
	CLevelSection *GetLeveSection( int nLevelID );	
	int GetLevelRange( int nLevelID, int &rMinLevel, int &rMaxLevel );
private:
	CLevelSection mLevelSectionList[MAX_LEVLESECTION_NUM];
	int mMaxLevel;
};