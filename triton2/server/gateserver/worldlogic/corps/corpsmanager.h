#pragma  once
#include "corpstype.h"
#include "corps.h"
#include "family.h"
#include "familytype.h"
#include "shm.h"
#include "lk_hashmap.h"
#include "gateobj_define.h"
#include "corpsinfo_pb.hxx.pb.h"   
#include "entity.h"



#include "union.h"



//#include "gatelogic.h"	
//
//using namespace GateServer;	

// 军团配置信息
struct CCorpsConfigInfo 
{
	int mLevel;					// 等级
	int mFamilyNumLimit;		// 每一个等级下家族个数上限
	int mGloryLimit;			// 荣耀上限
	int	mGeneralNumLimit;		// 当前等级下前锋将军人数上线
	int mCounSellorNumLimit;	// 当前等级下军师的人数上线
	int	mPvPStarScore;			// 升级到下一星级需要的pvp积分	
	int	mGloryLevelupNeeded;	// 军团升级到下一级需要的荣耀
	int mMoneyLevelupNeeded;	// 军团升级到下一级需要的家族金钱	
	int mBossRefineValueLimit;	// boss炼化值上限 
	int	mBossCDLimit;			// 军团boss的cd
};  

// 军团属性的配置信息
struct CCorpsPropertyConfigInfo 
{
	int mContributePerGold;
	int mScorePerFight;			// pvp每胜一场的积分
	int mGloryPerFight;			// 每胜利一场获取的荣耀值
	int mExpPerGlory;			// 每点荣耀值需要的经验值
	int mExploitPerGlory;		// 每点荣耀值需要的功勋值
	int mNpcGlory;				// 占领npc处每天可以领取的荣耀
	int mNpcMoney;				// 占领npc处每天可以领取的金钱
	int mChallangeMoney;		// 挑战赛金锭数
	int mChallangeTimesOfDay;	// 每天的挑战次数 
};


class CCorpsConfig
{
public:
	typedef lk::hash_map< int, CCorpsConfigInfo, CORPS_LEVEL_LIMIT > CorpsConfig;
private:
	CorpsConfig mConfigList;
	CCorpsPropertyConfigInfo mPropertyConfig;
public:
	CCorpsConfig( )
	{
		if ( CObj::msCreateMode )
		{	
			Initialize();
		}
		else
		{			

		}	
	}
	~CCorpsConfig(){ }
public:
	int Initialize( )
	{
		mConfigList.initailize();
		memset( &mPropertyConfig, 0, sizeof( mPropertyConfig ) );
		return 0;
	}	
	int Resume( ){ return 0; }
public:
	// 插入一条军团配置信息
	int InsertCorpsConfig( CCorpsConfigInfo& rCorpsConfig );
	
	// 设置家族的基本属性配置
	int SetCorpsPropertyConfig( CCorpsPropertyConfigInfo &rPropertyConfig );

	// 获取某个等级的军团的配置信息
	int GetCorpsConfigByLevel( int nLevel, CCorpsConfigInfo& rCorpsConfig );
	
	// 获取某个等级下军团的家族数量上限
	int GetFamilyNumLimitByLevel( int nLevel, int &rFamilyNum );
	
	// 获取某个等级下军团的荣耀上限
	int GetCorpsGloryLimitByLevel( int nLevel, int &rGlory );
	
	// 获取某个等级军团前锋将军的人数上限
	int GetGeneralNumLimitByLevel( int nLevel, int &rGeneralNum );
	
	// 获取某个等级下军团的军师人数上限
	int GetCounSellorNumLimitByLevel( int nLevel, int &rCounSellorNum );
	
	// 获取某星级下升级到下一星级的需要的pvp积分
	int GetPvPStarScore( int nLevel, int &rPvPScore );
	
	int GetCorpsRefineValueLimitByLevel( int nLevel, int &rRefineValue); 

	// 获得某个等级下的baoss   cd
	int GetCorpsBossCDLimit( int nLevel, int &rBossCD );

	// 获取某个等级到下一等级所需要的荣耀上限
	int GetGloryLevelupNeeded( int nLevel, int &rGlory );
	
	// 获取贡献一金锭的贡献度
	int GetContributePerGold();
	
	// 获取每胜利一场的积分
	int GetScorePerFight();
	
	// 获取每胜利一场获取的荣耀值
	int GetGloryPerFight(){ return mPropertyConfig.mGloryPerFight; }
	
	// 获取某个等级到下一等级需要的金钱
	int GetMoneyLevelUpNeeded( int nLevel, int &rMoney );
	
	// 获取每点荣耀值需要的经验值
	int GetExpPerGlory(){ return mPropertyConfig.mExpPerGlory; }
	
	// 获取每点荣耀值需要的功勋值
	int GetExploitPerGlory(){ return mPropertyConfig.mExploitPerGlory; } 
	
	// 获取npc处每天可以领取的金钱
	int GetNpcMoney(){ return mPropertyConfig.mNpcMoney; }
	
	// 获取npc处每天可以领取的荣耀
	int GetNpcGlory( ){ return mPropertyConfig.mNpcGlory; }

	// 获取挑战赛金锭
	int GetChallangeMoney() { return mPropertyConfig.mChallangeMoney; }

	// 获取挑战赛每日次数
	int GetChallangeTimesOfDay() { return mPropertyConfig.mChallangeTimesOfDay; }

	//

};	


class CCorpsManager : public CSingleton< CCorpsManager >
{
public:													     
	typedef lk::hash_map< int, int, SERVER_CAP_CORPS > CorpsList;		/// List of all corps
	typedef lk::vector< int, SERVER_CAP_CORPS > RoomList;				// 房间列表
	typedef lk::hash_map< int, int, MAX_UNION_NUM > UnionList;
	
public:
	CCorpsConfig mCorpsConfig;

	CorpsList m_tCorpsList;
	RoomList m_tRoomList;
	UnionList m_UnionList;

public:
	static CSharedMem* mShmPtr; 
	void * operator new( size_t tsize );
	void   operator delete( void *pPointer );			    
	static unsigned int CountSize();
	CCorpsManager();
	~CCorpsManager(){}
	int Initailize( );
	int Resume( );	
public:
	CCorps *GetCorpsByID( int nCorpsID );
	/// 获取玩家所属军团
	CCorps *GetCorpsOfPlayer( int nRoleID );
	CCorps *GetCorpsByName( const char *pCorpsName );

	CUnion *GetUnionByID( int nUnionID );
	CUnion *GetUnionOfPlayer( int nRoleID );
	CUnion *GetUnionByName( const char *pUnionName );
	int InsertCoprsInfo( CCorps *pCorps );
	int DeleteCorpsInfo( CCorps *pCorps ); 
	
	CCorpsConfig *GetCorpsConfig(){ return &mCorpsConfig; } 
		
	// 获取军团的成员列表
	void GetCorpsMemberList( CCorps *tpCorps, int *pMemberList, int &nNum, int nExceptFamiyID = 0, int nExceptRoleID = 0 );
	
	// 通过家族ID获取军团的成员列表
	void GetCorpsMemberListByFamilyID( CCorps *tpCorps );
	
public:
	// 创建军团
	int OnEventCreatCorps( int nRoleID, const char *pCorpsName, int nCorpsID, int nCorpsColor );
	
	// 申请加入军团
	int OnEventApplyCorps( int nApplyingID, CCorps *pCorps, FamilyData &pbFamilyData );
	
	// 答复申请
	int OnEventReplyApply( int nReplyingID, int nFamilyID, int nResult, int &nApplyingID );
	
	// 邀请家族加入军团
	int OnEventInviteFamily( int nCorpsRooterID, CFamily *pFamily );
	
	// 答复军团邀请
	int OnEventReplyCorpsInvite( int nInvitedID, CCorps *pCorps, int nResult );
	
	// 解散军团
	int OnEventDisbandCorps( int nRooterID, int *pMemberList, int &nMemberNum );
	
	// 获取军团的属性信息
	int OnEventGetCorpsProperty( int nRooterID, CorpsProperty &pbCorpsInfo );
		
	// 获取某个家族的的信息
	int OnEventGetCorpsMemberInfo( int nRoleID, PBCorpsMemberList &pbCorpsMemberList );
	
	// 改变家军团的公告信息
	int OnEventChangeCoprsNotice( int nRoleID, const char *pNotice );	
	
	// 转让大将军职位
	int OnEventAppointCorpsHeader( int nHeaderID, int nMemberID );
	
	// 踢出家族
	int OnEventKickFamilyOfCorps( int nRooterID, int nFamilyID );
	
	// 离开家族
	int OnEventLeaveCorps( int nRoleID );
	
	// 获取军团列表信息
	int OnEventListCorps( int nRoleID, int nCountryID, CorpsInfoList &pbCorpsList, int nCurPage, int &rTotalPage );	
	
	// 获取军团的申请	
	int OnEventListCorpsApply( int nRoleID, PBCorpsApplyList &pbCorpsApply );
	
	// 清除军团的信息
	int OnEventClearCorpsInfo( CCorps *pCorps );
	
	// 取消解散军团的消息回复
	int OnEventCancelCorpsDisband( int nRoleID );
	
	// 清除家族的申请信息
	int OnEventClearCorpsApply( int nRoleID );

	
	


	//int OnAttachBattleCity( int nMapID, int nCorpsID );
	//int OnDeleteBattleCity( int nMapID );
	//int OnFindBattleCity( int nMapID, int& nCorpsID );
	int OnGetBattleMapList( int* npList, int* npCorpsList, int& nNum, int nServerID );

	void InsertRoom( int nCorpsID ){ m_tRoomList.push_back( nCorpsID ); }
	int OnGetRoomList( int* vpList, int& nSize );
	bool HasRoom( int nCorpsID );
	int OnDelRoom( int nCorpsID );


	//void SendCorpsNpcList( int nServerID );

	void SendCorpsNpcSingle( int nServerID, int nNpcID, int nCorpsID, bool bAdd = true );

	// 清除军团战斗状态
	int ClearCorpsBattleStatus( int nCorpsID );
	
	// 设置军团职位
	int OnEventSetCorpsMemberPost( int nManagerID, int nMemberID, int nPost, int &rOldPost );

	// 捐献金钱
	int OnEventContributeMoney( int nMemberID, int nMoney );

	// 消耗金钱
	int OnEventConsumeMoney( int nManagerID, int nMoney );

	// 家族升级
	int OnEventLevelUPCorps( int nManagerID, int &rLevel, int &rMoney );

	// 增加家族荣耀
	int OnEventAddCorpsGlory( int nCorpsID, int nGloryValue );

	// 减少家族荣耀
	int OnEventReduceCorpsGlory( int nCorpsID, int nGloryValue, bool &rLevelDown );

	// 增加家族pvp积分的接口
	int OnEventAddCorpsPVPScore( int nCorpsID, int nScoreValue, bool &rLevelUP );
	
	// 设置成员称号
	int OnEventSetCorpsMemberName( int nManagerID, int nMemberID, const char *pName );
	
	// 家族捐献金钱
	int OnEventFamilyContributeMoney( int nMemberID, int nMoney );

	// 军团结盟
	int OnEventCreateUnion( int vHeadID, int *vCorpsID, int vNumber, char *vUnionName, char *vUnionNotice, int vCharID );

	// 申请加入联军
	int OnEventApplyUnion( int nApplyingID, CUnion *pUnion, PBCorpsBasicInfo &Info );

	// 答复申请
	int OnEventReplyUnionApply( int nReplyingID, int nCorpsID, int nResult, PBCorpsBasicInfo &vCorpsInfo );

	// 邀请军团加入联军
	int OnEventInviteCorps( int nCorpsRooterID, CCorps *pInviteCorps );

	// 答复联军邀请
	int OnEventReplyUnionInvite( int nInvitedID, CUnion *pUnion, int nResult );

	// 解散联军
	int OnEventDisbandUnion( int nRooterID );	

	// 改变联军的公告信息
	int OnEventChangeUnionNotice( int nRoleID, const char *pNotice );	

	// 踢出联军
	int OnEventKickCorpsOfUnion( int nRooterID, int nCorpsID );

	// 离开联军
	int OnEventLeaveUnion( int nRoleID );	

public:
	/*********数据库相关的操作***********/
	// 创建军团的数据库信息
	int CreatCorpsDBInfo( CCorps *pCorps );

	// 创建军团联盟
	int CreatUnionDBInfo(CUnion* pUnion);

	// 更新军团的数据库信息
	int UpDateCorpsDBInfo( CCorps *pCorps );

	// 删除军团的数据库信息
	int DeletCorpsDBInfo( CCorps *pCorps );

	// 更新所有的军团信息
	void UpDateAllCorpsDBInfo( bool bDirectSave = false );
	
	// 删除解散时间已到的军团
	void DeleteCorpsOnDisbandTime();
	
	// 获取军团中拥有某项权限的在线的职位最高的成员
	CGatePlayer *GetManagerOfRight( CCorps *tpCorps, int nRight );

	// 更新联军的数据库信息
	int UpdateUnionDBInfo( CUnion *pUnion );

	// 删除联军的数据库信息
	int DeleteUnionDBInfo( CUnion *pUnion );

	// 更新所有的联军信息
	void UpDateAllUnionDBInfo( );
	
	// 根据军团ID解散军团
	void DisbandCorps( CCorps *pCorps );
	
	// 从列表中移除军团
	int RemoveCorpsFromList( int nCorpsID );
	
	// 删除军团并且发送通知
	void DeleteCorpsWithNotify( CCorps *pCorps );
	
	// 删除军盟并且发送通知
	void DeleteUnionWithNotify( CUnion *pUnion, int nRooterID );
};
