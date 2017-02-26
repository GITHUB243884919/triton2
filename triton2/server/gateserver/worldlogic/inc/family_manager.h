#ifndef _FAMILY_MANAGER_H_
#define _FAMILY_MANAGER_H_

#include "family.h"
#include "familytype.h"	
#include "familydbinfo_pb.hxx.pb.h"
#include "familymessage_pb.hxx.pb.h"
#include "servertool.h"
#include "shm.h"
#include "gateobj_define.h"
#include "entity.h"
#include "mailtype.h"
enum EFamilyErrorCode
{
	EFAMILY_CREATE_DUPNAME,
	EFAMILY_ROLE_HAS_FAMILY,
	EFAMILY_MEMBER_IS_FULL,
	EFAMILY_ROLE_ISIN_FAMILY,
	EFAMILY_ROLE_NOAGREERIGHT,
	EFAMILY_ROLE_HAVE_INTHISFAMILY,
	EFAMILY_ROLE_HAVE_INOTHERFAMILY, 
};	

// 家族配置信息
struct CFamilyConfigInfo 
{
	int mLevel;					// 等级
	int mMemberNumLimit;		// 成员人数上限
	int mGloryLimit;			// 荣耀上限
	int	mPresbyterNumLimit;		// 长老人数上线
	int	mPVPScoreLimit;			// 当前pvp星级上限
	int mGloryLevelUPNeeded;	// 升级到下一等级需要的荣耀值
	int mMoneyLevelUPNeeded;	// 升级到下一级需要的金钱	
	int mInitialStability;		// 每一级初始化安定度
	int mReduceStability;		// 每一级每天应该减少的安定度
};  

// 家族基本属性的配置
struct CFamilyPropertyConfigInfo
{
	int mContributePerGold;		// 捐献一锭金子的贡献度	
	int	mScorePerFight;			// 每胜利一场获取的积分
	int	mHour;					// 刷新家族状态的小时值
	int mMinute;				// 刷新家族状态的分钟值
	int mGloryPerFight;			// 胜利一场获取的荣耀值
	int	mNpcGlory;				// npc处每天可以领取的荣耀
	int mNpcMoney;				// npc处每天可以领取的金钱
	int mExpPerGlory;			// 每点荣耀值需要的经验值
	int mExploitPerGlory;		// 每点荣耀值需要的功勋值
	int	mRepetionMoney;			// 开启家族副本需要消耗的家族金钱
	int	mRepetionGlory;			// 开启家族副本需要消耗的家族荣耀
	int	mShopRefreshTimeLength;	// 战争商店的刷新时间(小时)
	int mRefreshBasicTime;		// 刷新的基本单位(秒)
	int mWarNpcSellTableID;		// 战争npc的商店ID
	int mChallangeMoney;		// 挑战赛金锭
	int mChallangeTimesOfDay;	// 每天挑战赛次数
	int mRepetionStability;		// 每次品仙桃奖励的安定度
};

// 安定度的等级配置
class CStability
{
public:
	int mLevel;								 // 等级
	int	mTimeValue[MAX_STABILITY_TIME_NUM];	 // 在线时间长度
	int mStability[MAX_STABILITY_TIME_NUM];	 // 每一时间长度应该获取的安定度值
public:
	CStability()
	{
		mLevel = 0;
		memset( mTimeValue, 0, sizeof( mTimeValue ) );
		memset( mStability, 0, sizeof( mStability ) );
	}
public:
	int InsertStablity( int nTimeValue, int nStability );	// 插入一个安定度的配置
	int GetStablity( int nTimeValue, int &rOnLineTime );						// 获取安定度
};

class CStabilityConfig
{
private:
	int	mTaskStability;									// 做任务能够获取的家族安定度
	int	mMoneyStablity;									// 每捐献一非绑定金能够获取的安定度值
	int mDisbandMailID;									// 家族解散时发给家族成员的邮件ID
	int mHeaderMailID;									// 家族解散是发给族长的邮件ID
	int mUseStability;									// 是否使用家族安定度设定(0为不使用，其他为使用),如果使用安定度降为0时家族解散
	CStability mLevelStablity[MAX_STABILITY_LEVEL_NUM];	// 玩家在线时间累积安定度的等级相关配置
public:
	// Set & Get TaskStability
	void SetTaskStablity( int nStability ) { mTaskStability = nStability; }
	int GetTaskStability(){ return mTaskStability; }
	
	// Set & Get MoneyStablity
	void SetMoneyStability( int nStablity ) { mMoneyStablity = nStablity; }
	int GetMoneyStability(){ return mMoneyStablity;  }
	
	// Set & Get DisbandMailID
	void SetDisbandMailID( int nID ){ mDisbandMailID = nID; }
	int GetDisbandMailID(){ return mDisbandMailID; }
	
	// Set & Get HeaderMailID 
	void SetHeaderMailID( int nMailID ){ mHeaderMailID = nMailID; }
	int GetHeaderMailID(){ return mHeaderMailID; }
	
	// 插入一个等级相关的配置
	void InsertLevelInfo( CStability &rStabiliy );
	
	// 获取某个等级在线时长所对应的安定度
	int GetStabilityValue( int nLevel, int nTime, int &rOnLineTime );
	
	// Set & Get UseStability
	int GetUseStability(){ return mUseStability; }
	void SetUseStability( int nStability ){ mUseStability = nStability; }
public:
	int Initialize();
	int Resume(){ return 0; }		
	CStabilityConfig()
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
	~CStabilityConfig()
	{
		
	}
};

// 每一等级安定度所对应的内胆信息
class CStabilityRedstone
{
public:
	int mStability[MAX_STABILITY_REDSTONE_NUM];
	int mRedstoneID[MAX_STABILITY_REDSTONE_NUM];
	int mEtraExp[MAX_STABILITY_REDSTONE_NUM];
public:
	CStabilityRedstone()
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
	int Initialize()
	{
		memset( mStability, 0, sizeof( mStability ) );
		memset( mRedstoneID, 0, sizeof( mRedstoneID ) );
		memset( mEtraExp, 0, sizeof( mEtraExp ) );
		return 0;
	}
	int Resume(){ return 0; }
public:
	int GetRedStoneID( int nStablity, int &rExtraExp );
	int InsertStabilityRedstone( int nStability, int nRedstone, int nExtraExp );
};

// 家族内丹的配置信息
class CFamilyRedstoneConfig
{
public:
	CFamilyRedstoneConfig()
	{
		if ( CObj::msCreateMode )
		{
			Initiallize();
		}
		else
		{
			Resume();
		}
	}
public:
	int Initiallize()
	{
		memset( mStablityRedstone, 0, sizeof( mStablityRedstone ) );
		return 0;
	}
	
	int Resume()
	{
		return 0;
	}
public:
	int GetFamilyRedstone( int nLevel, int nStablity, int &rExtraExp );
	int InsertFamilyRedstone( int nLevel, int nRedstoneID, int nStablity, int nExtraExp );
private:
	CStabilityRedstone mStablityRedstone[FAMILY_LEVEL_LIMIT];
};


// 邮件信息
struct CSysMailInfo
{
public:
	int mItemID[MAIL_ITEM_COUNT_MAX];
	int mItemNum[MAIL_ITEM_COUNT_MAX];	
public:
	CSysMailInfo()
	{		
		memset( mItemID, 0, sizeof( mItemID ) );
		memset( mItemNum, 0, sizeof( mItemNum ) );
	}
};

class CMailConfig 
{
public:
	typedef lk::hash_map< int, CSysMailInfo, MAX_MAILINFO_NUM > MailInfoList;
public:
	
	// 插入一个邮件信息
	int InsertMailInfo( int nSysID, CSysMailInfo &rMailInfo );
		
	// 获取某个邮件信息
	CSysMailInfo * GetMailInfoByID( int nSysID );
	
	int Initialize()
	{
		mMailInfoList.initailize();
		return 0;
	}
	int Resume()
	{
		return 0;
	}
public:
	CMailConfig()
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
	~CMailConfig(){}
private:
	MailInfoList mMailInfoList;
};

// ncp福利的配置
struct CNpcWelfare
{
	int mNpcMoney;				// npc金钱
	int mNpcGlory;				// npc荣耀	
};

// 家族品仙桃的时间配置
struct CRepetionTimeInfo
{
public:
	int mBeginTime;
	int mEndTime;
	int mWeekDay;
	int mOpenTimes;
public:
	CRepetionTimeInfo()
	{
		mBeginTime = 0;
		mEndTime = 0;
		mWeekDay = 0;
		mOpenTimes= 0;
	}		
};
class CFamilyConfig
{
public:
	typedef lk::hash_map< int, CFamilyConfigInfo, FAMILY_LEVEL_LIMIT > FamilyConfig;
	typedef lk::hash_map< int, CNpcWelfare,	MAX_NPCNUMOFWELFARE >  NpcWelfareList;
	typedef lk::vector< CRepetionTimeInfo, MAX_REPETIONTIMEINFO_NUM > RepetionTimeInfoList;
private:
	FamilyConfig mConfigList;
	CFamilyPropertyConfigInfo mPropertyConfig;
	NpcWelfareList			  mNpcWelfare;
	RepetionTimeInfoList	  mRepetionTimeInfoList;
	CStabilityConfig		  mStabilityConfig;
	CMailConfig				  mMailConfig;
	CFamilyRedstoneConfig	  mFamilyRedstoneConfig;	
	int						  mReptionWeekNum;
public:
	CFamilyConfig( )
	{
		if ( CObj::msCreateMode )
		{	
			Initialize();
		}
		else
		{			
				
		}	
	}
	~CFamilyConfig(){ }
public:
	int Initialize( )
	{
		mConfigList.initailize();
		memset( &mPropertyConfig, 0, sizeof( mPropertyConfig ) );
		mNpcWelfare.initailize();
		mRepetionTimeInfoList.initailize();
		mStabilityConfig.Initialize();
		mMailConfig.Initialize();
		mFamilyRedstoneConfig.Initiallize();
		mReptionWeekNum = 0;
		return 0;
	}	
	int Resume( ){ return 0;  }
public:
	// 插入一条家族配置信息
	int InsertFamilyConfig( CFamilyConfigInfo& rFamilyConfig );
	
	// 设置家族的基本属性配置
	int SetFamilyPropertyConfig( CFamilyPropertyConfigInfo &rFamilyPropertyConfig );
	
	// 插入一条npc的福利信息
	int InsertNpcWelfare( int nNpcID, CNpcWelfare &rNpcWelfare );
	
	// 获取npc的福利信息
	int GetNpcWelfare( int nNpcID, CNpcWelfare &rNpcWelfare );
	
	// 获取某个等级的家族的配置信息
	int GetFamilyConfigByLevel( int nLevel, CFamilyConfigInfo& rFamilyConfig );	
	
	// 获取家族某一等级下的人数上限
	int GetMemberNumLimitByLevel( int nLevel, int &rMemberNum );
	
	// 获取家族某一等级下的荣耀上限
	int GetGloryLimitByLevel( int nLevel, int &rGlory );
	
	// 获取长老的人数上限
	int GetPresbyterNumLimit( int nLevel, int &rNumber );
	
	// 获取某一pvp星级下升级到下一pvp星级的pvp积分
	int GetPVPScoreLimit( int nLevel, int &rPvPScoreLimit );
	
	// 获取某一等级下升级到下一等级需要的荣耀值
	int GetGloryLevelUPNeeded( int nLevel, int &rGlory );
	
	// 获取某一等级下升级到下一等级需要的金钱
	int GetMoneyLevelUPNeeded( int nLevel, int &rMoney );
	
	// 获取某一等级的初始安定度
	int GetInitialStability( int nLevel, int &rStability );
	
	// 获取某一个等级每天应该减少的家族安定度
	int GetReduceStability( int nLevel, int &rStability );
	
	// 获取一锭金子的贡献度
	int GetContributePerGold(  );
	
	// 获取 每胜利一场获取的积分
	int GetScorePerFight(  );
	
	// 获取每日刷新的时钟值
	int GetRefreshHour( ) { return mPropertyConfig.mHour; }	
	
	// 获取每日刷新的分钟值
	int GetRefreshMinute( ){ return mPropertyConfig.mMinute; }
	
	// 获取胜利一场的荣耀值
	int GetGloryPerFight() { return mPropertyConfig.mGloryPerFight; }
	
	// 获取npc处每天可以领取的金钱
	int	GetNpcMoney(){ return mPropertyConfig.mNpcMoney; }
	
	// 获取npc处媒体可以领取的荣耀
	int GetNpcGlory(){ return mPropertyConfig.mNpcGlory; }
	
	// 获取一点荣耀值的需要的经验值
	int GetExpPerGlory(){ return mPropertyConfig.mExpPerGlory; }
	
	// 获取一点荣耀值的需要的功勋值
	int GetExploitPerGlory(){ return mPropertyConfig.mExploitPerGlory; }
	
	// 获取进入家族副本需要消耗的家族金钱
	int GetRepetionMoney( ){ return mPropertyConfig.mRepetionMoney; }
	
	// 获取进入家族副本需要消耗的家族经验
	int	GetRepetionGlory( ){ return mPropertyConfig.mRepetionGlory; }	
	
	// 获取商店的刷新时间
	int GetShopRefreshTime(){ return mPropertyConfig.mShopRefreshTimeLength*mPropertyConfig.mRefreshBasicTime; }
	
	// 获取家族战争npc的出售表ID
	int GetWarNpcSellTableID(){ return mPropertyConfig.mWarNpcSellTableID; }

	// 获取家族挑战赛金钱次数
	int GetChallangeMoney() { return mPropertyConfig.mChallangeMoney;}

	// 获取家族挑战赛每日次数
	int GetChallangeTimesOfDay() { return mPropertyConfig.mChallangeTimesOfDay; }
	
	// 插入一个副本的时间配置信息
	int InsertRepetionTimeInfo( CRepetionTimeInfo &rRepetionTimeInfo );	
		
	// 检查是否可以进入家族副本
	int CheckRepetionOpenTime(  );
	
	// 获取今天的副本时间信息	
	CRepetionTimeInfo *GetCurrentReptionTimeInfo();
	
	// 获取安定度配置
	CStabilityConfig *GetStabilityConfig(){ return &mStabilityConfig; }
	
	// 获取邮件配置
	CMailConfig *GetMailConfig(){ return &mMailConfig; }

	// 获取家族内丹配置
	CFamilyRedstoneConfig *GetFamilyRedstoneConfig() { return &mFamilyRedstoneConfig; }

	// 获取品仙桃的安定度
	int GetRepetionStability(){ return mPropertyConfig.mRepetionStability; }

	// 设置副本周次数
	void SetRepetionWeekNum( int nNum ){ mReptionWeekNum = nNum; }
	// 获取副本周次数
	int GetRepetionWeekNum(){ return mReptionWeekNum; }
};						    

// 与逻辑相关, 与接收的消息无关; 但可以发消息
class CFamilyManager : public CSingleton< CFamilyManager >
{
public:
	enum 
	{
		// 非成员事件
		em_event_family_establish,		// 家族建立
		em_event_clear_apply_by_timer,	// 申请定期清除

		// 宗正事件
		em_event_alienate_by_rooter, 	// 宗正转让职位
		em_event_dismiss_by_rooter,		// 家族解散

		//需要控制成员权限的事件
		em_event_post_appoint_by_player,	// 职务任免
		em_event_change_notice_by_player,// 改变家族公告
		em_event_fired_by_player,	// 开除族员
		em_event_list_apply_by_player, 	// 查看申请加入表
		em_event_agree_apply_by_player,	// 批准加入家族申请 
		em_event_invite_by_player,		// 邀请玩家加入

		// 控制成员权限管理类事件
		em_event_oprate_officer, 		// 成员建立官职, 修改官职，删除官职
		em_event_oprate_rights,  		// 成员为官职分配权利/回收

		// 普通成员拥有的权利
		em_event_family_outline_watch,	// 家族概要信息查看
		em_event_family_detail_watch,	// 家族详细信息查看
		em_event_retire_by_norooter,	// 非宗正成员退出家族
		em_event_refused_inviting,		// 玩家拒绝邀请
		em_event_agreed_inviting,		// 玩家同意邀请
		em_event_listfamily_by_player,	// 玩家获取本国已有家族表
		em_event_applyto_by_player,		// 玩家申请加入家族
	};

	enum EFamilyInfo
	{
		em_apply_limit = 5,
		em_family_num  = FAMILY_CAP,
		em_npc_num = 100,
	};	
public:

	typedef lk::hash_map< int, int, em_family_num > TFamilies;
	typedef lk::hash_map< roleid_t, int, em_apply_limit> TApplyList;

	class BidInfo
	{
	public:
		int mBidMoney;		// 竞价金额
		int mBidTime;		// 竞价时间

	public:
		BidInfo( ){ mBidMoney = 0; mBidTime = 0; }
		~BidInfo( ){ }
	};

	typedef lk::hash_map< int, BidInfo, em_family_num > BidList;

	class NPCInfo
	{
	public:
		int		mOwnFamily;		// 所属家族
		BidList mBidList;		// 竞价列表
		int		mMaxMoney;		// 最高竞价
		int		mDefID;			// 防守方ID
		int		mAttID;			// 进攻方ID
	public:
		NPCInfo( ){ }
		NPCInfo( const NPCInfo& vInfo ){ }
		~NPCInfo( ){ }
	};
	
	typedef lk::hash_map< int, NPCInfo, em_npc_num > NPCList;
	typedef lk::vector< int, SUBMIT_LIMIT > MemberList;
	typedef lk::hash_map< int, MemberList, em_family_num > SubmitList; 

	class BattleInfo
	{
	public:
		KEY_TYPE	mKey;				// 副本key
		int			mAttFamilyID;		// 攻击方ID
		int			mDefFamilyID;		// 防守方ID
		int			mType;				// 副本类型

	public:
		BattleInfo( ){ mKey = INVITED_KEY_VALUE; mAttFamilyID = 0; mDefFamilyID = 0; mType = TYPE_BATTLE_NPC; }
		BattleInfo( KEY_TYPE vKey, int vAttFamilyID, int vDefFamilyID, int vType ) : mKey( vKey ), mAttFamilyID( vAttFamilyID ), mDefFamilyID( vDefFamilyID ), mType( vType ){ }
		~BattleInfo( ){ }
	};
	typedef lk::vector< BattleInfo, em_family_num > BattleList;

	typedef lk::vector< int, em_family_num > ChaList;
	class RoomInfo
	{
	public:
		char	mName[ NAME_LENGTH ];	// 房间名
		int		mMoney;					// 房间价格
		ChaList	mChList;				// 挑战列表
		int		mState;					// 房间状态
		bool	mLock;					// 锁定状态
	public:
		RoomInfo( ){ mMoney = 0; mState = STATE_IDLE; mLock = false; }
		RoomInfo( int vMoney, int vState ) : mMoney( vMoney ), mState( vState ){ }
		~RoomInfo( ){ }
	};
	typedef lk::hash_map< int, RoomInfo, em_family_num > RoomList;
	
	typedef lk::hash_map< int, KEY_TYPE, em_family_num > FamilyRepitonKeyList;
private:		
	// 家族
	TFamilies		m_tFamilies; 
	// 申请表
	TApplyList		m_tApplies;
	// 配置信息表
	CFamilyConfig	m_tFamilyConfig;
	// 争夺的NPC列表
	NPCList		mNpcList;
	// 争夺战报名列表
	SubmitList  mNPCSubmitList;
	// 战场列表
	BattleList	mBattleList;
	// 房间列表
	RoomList	mRoomList;
	
	// 家族副本的key列表
	FamilyRepitonKeyList mFamilyReptionKeyList;
public:
	TFamilies		*GetFamilyList() { return &m_tFamilies; }
	CFamilyConfig	*GetFamilyConfig(){ return &m_tFamilyConfig; }
public:
	static CSharedMem* mShmPtr; // 共享内存地址
	void * operator new( size_t tsize );
	void   operator delete( void *pPointer ); 
	static unsigned int CountSize();
	CFamilyManager() ;
	~CFamilyManager(){}
	int Initailize( );
	int Resume();
public:
	/* 通用操作函数 */
	// 通过家族名称获取家族
	CFamily * GetFamilyByName( const char* szFamilyName );
	
	// 通过家族ID获取家族
	CFamily* GetFamilyByID( int nFamilyID );	
	
	// 获取玩家家族
	CFamily * GetFamilyOfPlayer( roleid_t nRoleID );
	
	// 把玩家的申请加入申请列表
	int	Insert2ApplyList( int nRoleID, int nFamilyID );
	
	// 获取某个家族的申请列表
	int GetApplyList( int nFamilyID, PBApplyList & pbApplyList ); 
	 
	// 通过ID获取某个家族的名字
	const char * GetFamilyNameByID( int nRoleID );	
	
	// 把家族插入到列表里面
	int InsertFamily( CFamily *pFamily );
	
	// 获取家族申请成员的数量
	int GetFamilyApplyNum( int vFamilyID );	

public:		
	/* 客户端请求相关函数 */
	
	// 创建家族		
	int  OnEventEstablishFamily( roleid_t nRoleID, const char* szFamilyName, int &FamilyID );
	
	// 邀请玩家加入家族
	int	 OnEventInvite2Family( roleid_t nInvitingRoleID, roleid_t nInvitedRoleID );

	// 邀请被人拒绝
	int OnEventRefusedByPlayer( roleid_t nInvitingRoleID, roleid_t nInvitedRoleID, int nFamilyID ); 

	// 邀请被人同意
	int OnEventAgreeedByPlayer( roleid_t nInvitingRoleID, roleid_t nInvitedRoleID, int nFamilyID ); 
		
	// 获取家族成员列表
	int OnEventListFamilies( roleid_t nRoleID, int* pMember, int &nNum, int ExceptRoleID = 0 );
	int OnEventListFamilies( CFamily *pFamily, int* pMember, int &nNum, int ExceptRoleID = 0 );  
	
	// 申请加入家族	
	int OnEventSubmitApply( roleid_t nApplyingRoleID, CFamily *pFamily );
	
	// 通过家族名称申请加入家族
	int OnEventApplyByName( roleid_t nApplyingRoleID, const char *pFamilyName );		
	
	// 通过家族ID申请加入家族
	int OnEventApplyByID( roleid_t nApplyingRoleID, int nFamilyID );
	
	// 获取所有的申请列表
	int OnEventListAllApplies( roleid_t nRoleID, PBApplyList &pbApply );

	// 同意申请
	int OnEventReplayApply( roleid_t nApprovalRoleID, int nApplyingRoleID, int tRet );  	
	
	//	清除玩家的申请
	bool OnEventRemoveApply( roleid_t nRoleID );	
		
	// 清除所有的申请
	bool OnEventClearAllApply( );  
	
	// 宗主转让职位给其它玩家
	int OnEventARoleAlienate( roleid_t nARoleID, roleid_t nDestRoleID ); 
	
	// 开除玩家
	int OnEventFirePlayer( roleid_t nABCRoleID, roleid_t nDestRoleID );	
	
	// 解散家族
	int OnEventDismissFamily( roleid_t nARoleID, int *pMemberList, int &rMemberNum );	

	// 离开家族
	int OnEventLeaveFromFamily( roleid_t nRoleID );
			
	// 设置官职
	int OnEventOperateOfficer(roleid_t nRoleID, int nFamilyID, labelid_t nLabelID, const char* szLabelName);
	
	// 为职位设置权限
	int OnEventOperateRights(roleid_t nRoleID, int nFamilyID, labelid_t nLabelID, int rights);
	
	// 列出所有家族的信息
	int OnEventListFamily( roleid_t nRoleID, int nCountryID, PBFamilyList &pFamilyData, int nCurPage, int &rTotalPage );
	
	// 获取申请列表
	bool OnEventListApply(roleid_t nRoleID, int nFamilyID);	 
	
	// 职位任免
	int  OnEventPostAppoint( roleid_t RooterID, roleid_t MemberID, int LabelID );
			
	// 清除申请列表
	void OnEventClearApplyByTimer( );
	
	// 同意家族邀请
	int OnEventAgreeInvite( roleid_t nInvitingRoleID, roleid_t nInvitedRoleID );
	
	// 获取家族的信息
	int OnEventGetFamiyProperty( roleid_t nRoleID, PBFamilyPropery &pFamilyData );
	
	// 改变家族的公告信息
	int OnEventChangeFamilyBulletin( roleid_t nRoleID, const char *pBulletion );
	
	// 清除某个家族的申请列表
	int ClearApplyListOfFamily( int FamilyID );
	
	// 清除家族申请列表
	int OnEventClearApplyList( roleid_t nRoleID );	 
	
	// 解散家族,清除家族信息
	int OnEventClearFamilyInfo( CFamily *pFamily ); 
	
	// 取消家族解散
	int OnEventCancelFamilyDisband( int nRoleID );
	
	// 删除玩家的所有申请信息
	void DeleteAllApplyOfPlayer( int nRoleID );
	
	
	
	// 设置职位
	int OnEventSetFamilyPost( int nManagerID, int nMemberID, int nPost, int &rOldPost );
	
	// 捐献金钱
	int OnEventContributeMoney( int nMemberID, int nMoney );
	
	// 消耗金钱
	int OnEventConsumeMoney( int nManagerID, int nMoney, bool bFreeze );
	
	// 家族升级
	int OnEventLevelUPFamily( int nManagerID, int &rLevel, int &rMoney, int &rStability );
	
	// 增加家族荣耀
	int OnEventAddFamilyGlory( int nFamilyID, int nGloryValue );
	
	// 减少家族荣耀
	int OnEventReduceFamilyGlory( int nFamilyID, int nGloryValue, bool &rLevelDown );
	
	// 增加家族pvp积分的接口
	int OnEventAddFamilyPVPScore( int nFamilyID, int nScoreValue, bool &rLevelUP );		
	
public:
	/*********数据库相关的操作***********/
	// 创建家族的数据库信息
	int CreatFamilyDBInfo( CFamily *pFamily );

	// 更新家族的数据库信息
	int UpDateFamilyDBInfo( CFamily *pFamily );

	// 删除家族的数据库信息
	int DeletFamilyDBInfo( CFamily *pFamliy );
	
	// 更所有的家族信息
	void UpDateAllFamilyDBInfo( );
	
	// 删除所有处于解散状态并且已经到期的家族
	void DeleteFamilyOnTimeDisband( );	
	
	// 刷新所有家族的状态
	void RefreshFamilyStatus();
public:
	// 加入新建的争夺NPC
	int InsertNPC( int vNpcID, int vCountryID );
	// 查找NPC的所属家族
	int GetNpcOwner( int vNpcID, int vCountryID );
	// 修改NPC的所属家族
	int SetNpcOwner( int vNpcID, int vOwnerFamily, int vCountryID, bool vInit = false );
	// 增加NPC竞价
	int AddBid( int vNpcID, int vBidFamily, int vBidMoney, int vBidTime, int vCountryID, bool vInit = false );
	// 删除NPC竞价
	int DelBid( int vNpcID, int vBidFamily, int vCountryID );
	// 清除某NPC所有竞价
	int ClearBid( int vNpcID, int vCountryID );
	// 获得竞价成功的家族
	int GetWinner( int vNpcID, int &vAttFamily, int &vDefFamily, int vCountryID );
	// 获得所有竞价家族
	int GetAllFamily( int vNpcID, int *FamilyList, int *MoneyList, int &Num, int vCountryID );
	// 填充查询信息
	void GetBid( int vNpcID, int vCountryID, CMessageBidInfoResponse &vResponse );

	// 添加报名
	void AddSubmit( int vFamilyID, int vCharID );
	// 获得报名
	int GetSubmit( int vCharID );
	// 清空报名
	void ClearSubmit( int vFamilyID );
	// 通知进入战斗
	void NotifyBattle( int vAttFamilyID, int vDefFamilyID, int vType, int vNpcID = 0 );
	// 战斗结束
	void EndBattle( KEY_TYPE vKey, int vWinSide );
	// 判断是否在报名列表中
	bool InList( int vFamilyID, int vCharID );
	// 通知进入战场
	void NotifyEnter( int vFamilyID, int vCharID );

	// 新增房间
	int AddRoom( int vFamilyID, int vMoney, char *vName );
	// 新增挑战
	int AddCha( int vRoomFamilyID, int vChaFamilyID );
	// 删除房间
	int	DelRoom( int vFamilyID, int &vMoney );
	// 删除挑战
	int DelCha( int vFamilyID );
	// 清空挑战
	int ClearCha( int vFamilyID );
	// 设置房间状态
	int SetRoom( int vFamilyID, int vState );
	// 设置房间锁定
	int LockRoom( int vFamilyID, bool vLock );
	// 查询挑战匹配
	int HasCha( int vRoomFamilyID, int vChaFamilyID, int &vMoney );
	// 查询全部房间信息
	int QueryAllRoom( CMessageQueryAllRoomResponse &vResponse );
	// 查询自己房间信息
	int QueryMyRoom( int vFamilyID, int &vMoney, CMessageQueryMyRoomResponse &vResponse );
	// 查询自己挑战信息
	int QueryMyCha( int vFamilyID, CMessageQueryMyChaResponse &vResponse );

	// 向场景发送新家族ID名字通知
	void SendFamilyIDName( int vServerID );
	// 向场景发送NPC占领通知
	void SendNpcList( int vServerID );

	void SendSingleIDName( int vServerID, int vFamilyID, char *vFamilyName );
	void SendSingleNpc( int vServerID, int vNpcID, int vFamilyID );
	
	// 获取家族中有某项权限的职位最高并且在线的成员信息
	CGatePlayer *GetManagerOfRight( CFamily *pFamily, int nRight );
	
	// 插入一个副本Key
	int InsertFamilyReptionKey( int nFamilyID, KEY_TYPE nReptionKey );
	
	// 删除一个副本Key
	int DeleteFamilyReptionKey( int nFamilyID );
	
	// 使用副本Key结束副本
	int EndFamilyReptionByKey( KEY_TYPE nReptionKey, bool bNormalEnd );
	
	// 根据家族ID解散家族
	int DisbandFamily( CFamily *pFamily );
	
	// 因为角色不存在的原因删除一个不存在的家族
	int ClearNoExistFamily( CFamily *pFamily );
	
	// 从列表中移除家族
	int RemoveFromList( int nFamilyID );
	
	// 检查家族清理不存在的信息
	int CheckFamily();
	
	// 检查家族安定度
	int CheckFamilyStability();
	
	// 解散家族的上层组织
	bool DisbandedFamilyOrg( CFamily *pFamily );
	
	// 发送家族解散的系统邮件	
	int SendDisbandNoticeByMail( CFamily *pFamily );
	
}; 
#endif