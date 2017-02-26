#ifndef _FAMILY_H_
#define _FAMILY_H_
#include "base_define.h"
#include "base.h"
#include "lk_hashmap.h"
#include "object.h"	 
#include "familytype.h"
#include "familydbinfo_pb.hxx.pb.h"
#include "lk_vector.h"
#include "corpstype.h"
#include "activitytype.h"
#include "servertool.h"
#include "template.h"

// 为了避免误操作，宗正转让， 家族解散，家族建立的权利未开放	  


#define roleid_t  int  
#define labelid_t char 

//class CGoodSInfo;
//class CLeagueShopInfo;
//class CLeagueShopsManager;
//
class CLabel
{
private:
	int	 mRights;				// 权利明细
	// char mName[NAME_LENGTH];	// 称谓名称
	// labelid_t mID; 				// 称谓ID
public:

	// 设置权限
	void SetRight(EmRight emRight)
	{
		mRights |= emRight;
	}

	// 判断是否拥有权限
	bool HasRight(EmRight emRight) 
	{
		return (emRight & mRights) == emRight;
	}

	// 清除权限
	void ClearRight(EmRight emRight)
	{
		mRights &= (~emRight) ;
	}
	
	// 取得职位mID
	// labelid_t GetID() { return mID; }	
	   
	//设置职位mID
	// void SetID(labelid_t nID) { mID = nID; } 

	//取得职位名称
	// const char* GetName() const { return mName; }  

	//设置职位名称
	// void SetName(const char* szName) { strncpy(mName, szName, sizeof(mName)); mName[sizeof(mName)-1] = 0; } 
}; 

/*
 * 单个家族成员在家族中的信息, 目前只有官阶信息
 *
 */
typedef struct family_info_t
{
	// labelid_t mLabelID;				// 成员在家族中的官衔
	int		 mFamilyContribue;		// 家族贡献度
	int		 mCorpsContribue;		// 军团贡献度
	int		 mExp;					// 个人经验
	int		 mAP;					// 个人AP值
	char	 mName[NAME_LENGTH];	// 家族称号		
	labelid_t mFamilyPost;			// 家族职位
	labelid_t mCorpsPost;			// 军团职位
	int		 mOnLineTime;			// 在线时间(小时)
	family_info_t( )
	{
		if ( CObj::msCreateMode )
		{				
			memset( mName, 0, sizeof( mName ) );
			mFamilyContribue = 0;
			mCorpsContribue	 = 0;
			mExp = 0;
			mAP = 0;
			mFamilyPost = FAMILY_POST_MEMEBER;
			mCorpsPost = CORPS_POST_MEMBER; 
			mOnLineTime = 0;
		}
		else
		{
			
		}
	}
}family_info_t;

/*
 * 每个家族中官阶的组织结构树，目前使用最简单的链结构, 不使用树, 但保留树的数据结构
 *
 */
typedef struct organization
{
	CLabel	mLabel;		// 节点内容
	char	mLBrother;	// 左兄弟
	char	mRChild; 	// 右孩子
}organization_t;


// 家族军团的商品信息
class CGoodSInfo
{
public:
	int mItemID;		// 商品ID
	int mItemNum;		// 商品的已购买数量
	int	mComposeID;		// 兑换商店的话，表示复杂合成的ID
	int mPurcharseTime;	// 最后一次购买的时间
public:
	CGoodSInfo()
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
		mItemNum = 0;
		mComposeID = 0;	
		mItemID = 0;
		mPurcharseTime = 0;		
		return 0;
	}
	int Resume()
	{
		return 0;
	}
	~CGoodSInfo(){}
};

// 家族军团的商店信息
class CLeagueShopInfo:public CObj
{
public:	
	typedef lk::hash_map< int, CGoodSInfo, MAXGOODSINSHOP > GoodsList;
	enum 
	{
		SHOPTYPE_WARNPC		= 1,		//	战争商店
		SHOPTYPE_EXCHANGE	= 2,		//	兑换商店
	};
	
	enum ERefreshResult
	{
		REFRESH_ITEMNOCHANGE = 0,
		REFRESH_ITEMCHANGED	 = 1,
	};

	enum EShopError
	{
		SHOPERROR_BUYGOODS_WRONGTEMP				= 1,		// 购买的npc不是战争npc
		SHOPERROR_BUYGOODS_ITEMNOTENOUGH			= 2,		// 商品数量不足
		SHOPERROR_EXCHANGEGOODS_WRONGTEMP			= 3,		// 兑换npc不正确
		SHOPERROR_EXCHANGEGOODS_NPCHASNOCOMPOSEID	= 4,		// 兑换npc没有该配方
		SHOPEEROR_EXCHANGEGOODS_NPCHASNOITEM		= 5,		// 不能兑换改物品
	};
public:	
	CLeagueShopInfo()
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
	~CLeagueShopInfo(){ }	
public:
	int Initialize()
	{ 
		mShopType = 0;		
		mGoodsList.initailize();
		return 0;
	}
	int Resume(){ return 0; }
public:
	int mShopType;			// 商店类型(兑换还是占领商店)
	int	mShopID;			// 商店的ID，战争商店为出售表ID，兑换商店为兑换表ID
	GoodsList mGoodsList;	// 商品列表
public:
	// 设置&获取商店类型
	void SetShopType( int nShopType ){ mShopType = nShopType; }
	int	 GetShopType(){ return mShopType; }

	// 设置&获取商店npc的ID
	void SetShopID( int nSellTableID ){ mShopID = nSellTableID; }
	int	 GetShopID( ){ return mShopID; }		
	

	// 插入一个商品信息
	int InsertGoodInfo( int nItemID, CGoodSInfo &rGoodInfo );

	// 删除一个商品信息
	int DeleteGoodInfo( int nItemID );

	// 获取某个商品的信息
	CGoodSInfo *GetGoodInfo( int nItemID );

	// 获取商品的列表
	int GetGoodsList( PBLeagueShopInfo &rLeagueShop ); 

	// 设置商品列表
	int SetGoodsList( PBLeagueShopInfo &rLeagueShop ); 		

	// 刷新商品信息(只限于战争商店)
	int RefreshShopGoodsInfo( );
};

// 家族军团的所有商店信息
class CLeagueShopsManager
{
public:
	typedef lk::hash_map< int, int, MAXLEAGUESHOPSNUM > LeagueShopsList;	
public:
	CLeagueShopsManager()
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
	~CLeagueShopsManager(){ }
public:
	int Initialize()
	{
		mLeagueShopsList.initailize();
		mSaveStatus = false;
		return 0;

	}
	int Resume()
	{
		return 0;
	}
public:
	LeagueShopsList mLeagueShopsList;
	bool			mSaveStatus;	
public:

	// 购买某个物品
	int BuyItem( int nSellTableID, int nItemID, int nNum );

	// 兑换某个商品
	int ExchangeItem( int nExchangeID, int nItemID, int nNum, int nComposeID );	

	// 获取商店列表
	int GetShopListInfo( PBLeagueShopsListInfo &rShopList );

	// 设置商店列表
	int SetShopListInfo( PBLeagueShopsListInfo &rShopList );

	// 获取某个战争商店物品的信息
	int GetItemInfoInSellTable( int nSellTableID,  int nItemID, CGoodSInfo &rGoodInfo, int &rGoldPrice );	 		

	// 获取兑换商店物品的信息
	int GetExchangeShopItemInfo( int nExchangeID, int nItemID, CGoodSInfo &rGoodInfo );

	//	获取&设置保存状态
	void SetSaveStatus( bool bSaveStatus ){ mSaveStatus = bSaveStatus; }
	bool GetSaveStatus(  ){ return mSaveStatus; }	

	// 插入一个可兑换的物品
	int InsertExchangeItem( int nExchangeID, int nItemID, int nItemNum );						 

	// 获取某个商店的信息
	int GetShopInfo( int nShopID, PBLeagueShopInfo &tShopInfo ); 

	// 获取某个商店的信息
	CLeagueShopInfo *GetLeagueShopInfo( int nShopID ); 

	// 获取战争商店中某个物品的现有信息
	int GetWarShopItemInfo( int nSellTableID, int nItemID, CGoodSInfo &rGoodInfo, int &rGoldPrice );	
};

/*
 * 单个家族的数据结构
 *
 */

class CFamily : public CObj
{

public:
	enum E_SAVESTATUS
	{
		FAMILY_INFONOCHANGE = 0 ,			// 数据信息没有改变
		FAMILY_PROPERYCHANGED,				// 属性改变
		FAMILY_MEMBERCHANGED,				// 成员信息改变
		FAMILY_ALLINFOCHANGED,				// 所有信息都改变
	};
	enum
	{
		em_family_label_null = -1,
		em_family_officer_limit = 10,
		em_family_member_limit = 15,
	};
	enum
	{
		EM_FAMILYACTION =	1,					// 家族相关的操作	
		EM_CORPSACTION  =	2,	  				// 军团相关的操作
	};
	enum
	{
		EM_FAMILY_STATUS_ALIVE		= 0,	// 正常的状态
		EM_FAMILY_STATUS_DISBAND	,		// 解散状态
	};
	
	enum									  // 家族操作的一些错误码
	{
		EM_MONEYNOTENOUGH =	1,				// 金钱不足
	};
	
	// 获取角色列表时的返回结果
	enum
	{
		EM_MEMBERINFO_SUCCESS = 0,
		EM_MEMBERINFO_NEEDDISBAD = 1
	};
	typedef lk::hash_map<roleid_t, family_info_t, MAX_FAMILYMEMBER_NUM > members_t;
	typedef members_t::const_iterator const_iterator;
	typedef members_t::iterator iterator; 
	typedef lk::vector< roleid_t, MAX_APPLYLIST_APPLIESNUM > FamilyApplyList;
	typedef lk::vector< roleid_t, MAX_CORPSINVITE_NUM >	CorpsInviteList;
private:
	unsigned int m_nFamilyID;												// 家族ID
	char 		 m_sFamilyName[MAX_FAMILYNAME_LENGTH*MAX_UTF8_BYTES];		// 家族名称
	roleid_t	 m_nRooter;													// 最高权利角色
	char		 m_sRooterName[NAME_LENGTH];								// 族长的角色名称
	members_t	 m_tMembers;												// 成员在家族中的属性表
	char		 m_szBulletin[MAX_FAMILYNOTICE_LENGTH*MAX_UTF8_BYTES];		// 公告
	unsigned int m_nCountryID;												// 家族的国家ID
	organization_t 	m_tOfficers[em_family_officer_limit];					// 官职体系
	int			 m_nFamilyDBID;												// 数据库中的家族ID	
	FamilyApplyList	m_tApplyList;											// 申请列表
	int			m_nCorpsID;													// 家族所加入的军团ID
	int			m_nSaveStatus;												// 存储状态
	CorpsInviteList	m_tInviteList;											// 军团的邀请信息
	int			m_nFamilyStatus;											// 家族的状态正常状态，或者解散状态
	int			m_nDisbandTime;												// 族长解散家族的时间
	int			m_nLastTimeApplyCorps;										// 申请加入军团成功的最后一次时间
	int			m_nLastTimeLeaveCorps;										// 家族离开军团的最后时间
	int			m_nNpcID;													// 家族占领/竞价的NPCID
	int			m_nBidMoney;												// 家族竞价的钱数
	int			m_nBidTime;													// 家族竞价的时间
	bool		m_bJoinBattle;												// 是否有参战资格
	int			m_nBattleState;												// 家族战状态
	int			m_nWarRank;													// 综合排名
	char		m_sPVPName[NAME_LENGTH];									// 对战家族名称
	int			m_nFamilyActivityStatus;									// 家族的状态
	unsigned short m_nBossChallengeTimes;									// 每星期boss的挑战次数	
	
	// 以下内容暂时不用
	int m_nStar;			// 星级
	int m_nLevel;			// 等级
	int m_nGlory;			// 荣耀
	int m_nMoney;			// 金库
	int m_nFreezeMoney;		// 冻结钱
	int m_nContribution;	// 贡献
	int m_nHot;				// 人气
	int m_nProd; 			// 生产度
	int m_nFun;  			// 娱乐度
	int m_nTech;			// 科技度
	int m_nCul;				// 文化度
	
	
	int m_nPVPWinNum;		// pvp胜利的场数
	int m_nPVPFailedNum;	// pvp失败的场数
	int	m_nPVPScore;		// pvp积分
	int	m_nPVPStarLevel;	// pvp星级 	
	
	int		m_nChallengeStatus;										// 挑战家族的状况
	int		m_nChallengeMoney;										// 挑战金额
	char	m_sChallengeStarted[NAME_LENGTH];						// 发起的挑战
	char	m_sChallengeReceived[NAME_LENGTH];						// 收到的挑战
	
	int		m_nFamilyNpcMoney;										// 家族npc处可以领取的金钱
	int		m_nFamilyNpcGlory;										// 家族npc处可以领取的荣耀
	int		m_nNpcWelfareGetTime;									// 家族npc处领取福利的时间
	
	KEY_TYPE m_nFamilyBossKey;										// 家族副本key
	int		m_nFamilyRepetionLevel;									// 家族副本等级
	int		m_nReptionScore;
	int		m_nRepetionDate;										// 家族副本的上次进入时间(天)
	int		m_nRepetionOpenTimes;									// 当天家族副本开启的次数
	int		m_nWeekNum;												// 家族副本本周进入次数
	CLeagueShopsManager m_tFamilyShop;								// 家族商店信息
	int		m_nStabilityDegree;										// 家族安定度	
public:
	CFamily();
	virtual int Initialize();
	virtual int Resume() ;
	~CFamily(){}
public:
	void SetID(int nFamilyID) { m_nFamilyID = nFamilyID; }
	int  GetID() { return m_nFamilyID; }

	void SetRooter(roleid_t nRoleID) 
	{
		m_nRooter = nRoleID;
		SetSaveStatus( FAMILY_PROPERYCHANGED );	
	}
	roleid_t GetRooter() { return m_nRooter; }
	
	void SetName(const char* szName) { strncpy(m_sFamilyName, szName, sizeof(m_sFamilyName)); m_sFamilyName[sizeof(m_sFamilyName)-1] = 0; }
	const char* GetName() { return m_sFamilyName; }
	const char *GetFamilyNameShow()
	{			
		// 字符集用于显示
#ifdef CHARSET_SHOW_GBK
		static char tFamilyName[ MAX_FAMILYNAME_LENGTH ] = {0};
		int nNameLen = sizeof(tFamilyName)-1;
		charset_convert( "utf8", "gbk", (char*)m_sFamilyName, strlen( m_sFamilyName ),
			tFamilyName, (size_t*)&nNameLen );
		return tFamilyName;
#else
		return m_sFamilyName;
#endif			
	} 
	void SetBulletin(const char* szBulletin) 
	{ 
		SetSaveStatus( FAMILY_PROPERYCHANGED );	
		strncpy(m_szBulletin, szBulletin, sizeof(m_szBulletin)); 
		m_szBulletin[sizeof(m_szBulletin)-1] = 0; 
	}

	const char* GetBulletin() { return m_szBulletin; }

	// set labelid's id, name, mRights, and his lbrother, rchild.
	void SetOrgan(labelid_t nLableID, organization_t organ)
	{
		SetSaveStatus( FAMILY_MEMBERCHANGED );	
		if (nLableID >= em_family_officer_limit || nLableID < 0) return;
		m_tOfficers[(int)nLableID] = organ;
	}

	void SetCanJoinBattle( bool bFlag ){ m_bJoinBattle = bFlag; }
	bool GetCanJoinBattle(){ return m_bJoinBattle; }

	// 取得成员列表
	members_t& GetMembers( ) { return m_tMembers; }
	
	// 获取家族成员数量
	int GetMemberNum(){ return m_tMembers.size(); }
	
	// 获取官职情况
	organization_t *GetOfficers() { return m_tOfficers; }

	// 以下暂时用不到
	void SetStar(int nStar) { m_nStar = nStar; }
	const int  GetStar() const { return m_nStar; }

	void SetLevel(int nLevel) { m_nLevel = nLevel; }
	int GetLevel() { return m_nLevel; }

	void SetGlory(int nGlory) { m_nGlory = nGlory; }
	int GetGlory() { return m_nGlory; }

	void SetMoney(int nMoney) { m_nMoney = nMoney; }
	int GetMoney() { return m_nMoney; }
	void SetFreezeMoney(int nMoney) { m_nFreezeMoney = nMoney; }
	int GetFreezeMoney() { return m_nFreezeMoney; }

	int GetFamilyMoney() { return m_nMoney - m_nFreezeMoney; }
	void RemoveFreezeMoney(){ m_nMoney -= m_nFreezeMoney; m_nFreezeMoney = 0; }

	void SetContribution(int nContribution) { m_nContribution = nContribution; }
	int GetContribution() { return m_nContribution; }

	void SetHot(int nHot) { m_nHot = nHot; }
	int  GetHot() { return m_nHot; }

	void SetProd(int nProd) { m_nProd = nProd; }
	int GetProd() { return m_nProd; }

	void SetFun(int nFun) { m_nFun = nFun; }
	int GetFun() { return m_nFun; }

	void SetTech(int nTech) { m_nTech = nTech; }
	int GetTech() { return m_nTech; }

	void SetCul(int nCul) { m_nCul = nCul; }
	int GetCul() { return m_nCul; }
	
	void SetCountryID( int nCountryID ){ m_nCountryID = nCountryID; }
	int  GetCountryID( ){ return m_nCountryID; }
	
	void SetRooterName( const char *pName )
	{ 
		strncpy( m_sRooterName, pName, sizeof( m_sRooterName ) - 1 ); 
		m_sRooterName[NAME_LENGTH -1]='\0';
		SetSaveStatus( FAMILY_PROPERYCHANGED ); 
	}	
	const char *GetRooterName(  ){ return m_sRooterName; }
	
	void SetFamilyDBID( int vDbID ){ m_nFamilyDBID = vDbID;  }
	int  GetFamilyDBID( ){ return m_nFamilyDBID; }
	
	void SetCorpsID( int nCorpsID )
	{
		 m_nCorpsID = nCorpsID;
		 SetSaveStatus( FAMILY_PROPERYCHANGED );	
	}
	int	 GetCorpsID( ){ return m_nCorpsID; }
	
	void SetSaveStatus( int vSaveStatus );
	int  GetSaveStatus( ){ return m_nSaveStatus; };
	void ClearSaveStatus( ) { m_nSaveStatus = FAMILY_INFONOCHANGE; }
	
	// 设置&获取家族状态
	void SetFamilyStatus( int nSaveSatus )
	{
		m_nFamilyStatus = nSaveSatus; 
		SetSaveStatus( FAMILY_PROPERYCHANGED );	
	}
	int GetFamilyStatus( ){ return m_nFamilyStatus; }
	
	// 设置&获取家族解散时间
	void SetFamilyDisbandTime( int nDisbandTime ){ m_nDisbandTime = nDisbandTime;  }
	int	 GetFamilyDisbandTime(){ return m_nDisbandTime; } 
	
	// 家族npc处领取福利的时间
	int GetNpcWelfareGetTime( ){ return m_nNpcWelfareGetTime; }
	void  SetNpcWelfareGetTime( int nTime )
	{ 
		SetSaveStatus( FAMILY_PROPERYCHANGED );	
		m_nNpcWelfareGetTime = nTime;
	}
	
	
	// 设置和获取申请加入军团成功的最后时间
	void SetLastTimeApplyCorps( int nLastTime ){ m_nLastTimeApplyCorps = nLastTime; }
	int	 GetLastTimeApplyCorps(){ return m_nLastTimeApplyCorps ; }
	
	// 设置和获取离开军团的最后时间
	void SetLastTimeLeaveCorps( int nLastTime ){  m_nLastTimeLeaveCorps = nLastTime; }
	int	 GetLastTimeLeaveCorps( ){ return m_nLastTimeLeaveCorps; }	 
	
	// 设置&获取挑战家族的状态
	void SetChallengeStatus( int nStatus ){ m_nChallengeStatus = nStatus; }
	int	 GetChallengeStatus( ){ return m_nChallengeStatus; }
	
	// 设置&获取挑战金额
	void SetChallengeMoney( int nMoney ){ m_nChallengeMoney = nMoney; }
	int	 GetChallengeMoney( ){ return m_nChallengeMoney; }
	
	// 设置&获取最近发起的挑战
	void SetChallengeStarted( const char *pChallenge )
	{
		strncpy( m_sChallengeStarted, pChallenge, NAME_LENGTH - 1 );
		m_sChallengeStarted[NAME_LENGTH-1]='\0';
	}
	const char *GetChallengeStarted(){ return m_sChallengeStarted; }
	
	// 设置&获取最近收到的挑战
	void SetChallengeReceived( const char *pChallenge )
	{
		strncpy( m_sChallengeReceived, pChallenge, NAME_LENGTH - 1 );
		m_sChallengeReceived[NAME_LENGTH-1] = '\0';
	}
	
	const char *GetChallengeReceived(){ return m_sChallengeReceived; }	
	
	// 设置&获取副本积分
	void SetRepetionScore( int nScore )
	{
		 m_nReptionScore = nScore; 
		 SetSaveStatus( FAMILY_PROPERYCHANGED );
	}
	int	 GetRepetionScore( ){ return m_nReptionScore; }	
	

	void SetNpcID( int nNpcID ){  m_nNpcID = nNpcID; SetSaveStatus( FAMILY_PROPERYCHANGED );  }
	int	 GetNpcID( ){ return m_nNpcID; }
	void SetBidMoney( int nBidMoney ){  m_nBidMoney = nBidMoney;  SetSaveStatus( FAMILY_PROPERYCHANGED );  }
	int	 GetBidMoney( ){ return m_nBidMoney; }
	void SetBidTime( int nBidTime ){  m_nBidTime = nBidTime;  SetSaveStatus( FAMILY_PROPERYCHANGED );  }
	int	 GetBidTime( ){ return m_nBidTime; }
	void SetBattleState( int vState ){ m_nBattleState = vState; SetSaveStatus( FAMILY_PROPERYCHANGED );  }
	int	 GetBattleState( ){ return m_nBattleState; }

	void SetPVPName( const char *pName )
	{ 
		strncpy( m_sPVPName, pName, sizeof( m_sPVPName ) - 1 ); 
		m_sPVPName[NAME_LENGTH -1]='\0';
	}	
	const char *GetPVPName(  ){ return m_sPVPName; }

	// 设置&获取pvp成功的场数
	void SetPVPWinNum( int nPVPWinNum ){ m_nPVPWinNum = nPVPWinNum; SetSaveStatus( FAMILY_PROPERYCHANGED );  }
	int	 GetPVPWinNum( ){ return m_nPVPWinNum; }
	
	// 设置&获取pvp失败的场数
	void SetPVPFaildNum( int nPVPFailedNum ){ m_nPVPFailedNum = nPVPFailedNum; SetSaveStatus( FAMILY_PROPERYCHANGED );  }
	int	 GetPVPFailedNum( ){ return m_nPVPFailedNum; }	 	

	
	// 设置&获取家族的pvp积分
	void SetPVPScore( int nPVPScore ){ m_nPVPScore = nPVPScore; SetSaveStatus( FAMILY_PROPERYCHANGED );  }
	int	 GetPVPScore( ){ return m_nPVPScore; } 
	
	// 设置&获取家族的pvp星级
	void SetPVPStarLevel( int nPVPLevel ){ m_nPVPStarLevel = nPVPLevel; SetSaveStatus( FAMILY_PROPERYCHANGED ); }
	int	 GetPVPStarLevel( ){ return m_nPVPStarLevel; }
	
	// 设置&获取综合排名
	void SetWarRank( int nWarRank ){ m_nWarRank = nWarRank; SetSaveStatus( FAMILY_PROPERYCHANGED );  }
	int	 GetWarRank( ){ return m_nWarRank; }
	
	// 设置&获取家族状态
	void SetFamilyActivityStatus( int nStatus ){ m_nFamilyActivityStatus = nStatus; }
	int	 GetFamilyActivityStatus(){ return m_nFamilyActivityStatus; }
	
	// 设置&获取家族副本key
	void SetFamilyBossKey( KEY_TYPE nKey ){ m_nFamilyBossKey = nKey; }
	KEY_TYPE  GetFamilyBossKey(){ return m_nFamilyBossKey; }
	
	// 设置家族副本等级
	void SetFamilyRepetionLevel( int nLevel ){ m_nFamilyRepetionLevel = nLevel; }
	int	 GetFamilyRepetionLevel( ){ return m_nFamilyRepetionLevel; }
	
	// 设置&获取npc处可领的金钱
	void SetFamilyNpcMoney( int nMoney )
	{ 
		m_nFamilyNpcMoney = nMoney;
		SetSaveStatus( FAMILY_PROPERYCHANGED ); 
	}
	int	 GetFamilyNpcMoney( bool bRefresh = true );
	
	// 设置&获取npc可以领取的荣耀值
	void SetFamilyNpcGlory( int nGlory )
	{
		m_nFamilyNpcGlory = nGlory;
		SetSaveStatus( FAMILY_PROPERYCHANGED );
	}
	int	 GetFamilyNpcGlory( bool bRefresh = true );
	
	// 获取家族npc福利
	int GetFamilyWelfare( int nNpcTmpID, int &rGlory, int &rMoney, bool bRefresh = true );
		
	// 查询申请冷却时间是否结束
	bool CheckLastTimeApplyCorps( );
	
	// 检查离开家族的冷却时间是否已经结束
	bool CheckLastTimeLeaveCorps( );
	
	// 获取家族的基本属性(家族成员可见)
	int  GetFamilyProperty( PBFamilyPropery &rPBProperty, bool bIfIncludeBasicInfo = true );
	
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
	
	// 设置&获取家族副本的进入日期
	void SetRepetionDate( int nDate )
	{ 
		m_nRepetionDate = nDate;
		SetSaveStatus( FAMILY_PROPERYCHANGED );
	}
	int	 GetRepetionDate(){ return m_nRepetionDate; }
	
	// 设置&获取家族副本当天的开启次数
	void SetRepetionOpenTimes( int nTimes )
	{ 
		m_nRepetionOpenTimes = nTimes;
		SetSaveStatus( FAMILY_PROPERYCHANGED );
	}
	int	 GetRepetionOpenTimes(){ return m_nRepetionOpenTimes; }
	
	// 检查副本进入时间
	bool CheckRepetionDate( int nDate, int nMaxTimes, int nMaxWeekNum );
	
	// 获取家族商店信息
	CLeagueShopsManager *GetFamilyShop( ){ return &m_tFamilyShop; } 		
	
	// 获取家族商店的pb信息
	int GetFamilyShopPBInfo( PBLeagueShopsListInfo &rPbInfo );
	
	// 通过家族商店的pb结构信息设置家族商店
	int SetFamilyShopFromPBInfo( PBLeagueShopsListInfo &rPbInfo );
	
	// set & get 
	void SetStabilityDegree( int nStability )
	{
		m_nStabilityDegree = nStability;
		SetSaveStatus( FAMILY_PROPERYCHANGED );
	}
	int GetStabilityDegree(){ return m_nStabilityDegree; }
	
public:
	// 取得官衔
	CLabel* GetLabel(labelid_t nLabelID); 

	// 取得当前成员数量
	int GetMemberCount();		

	// 判断家庭成员是否已达上限
	bool IsFull();

	// 添加新成员
	bool Ins2Member( roleid_t nRoleID, family_info_t &stFamilyInfo );

	// 删除成员
	bool Del4Member( roleid_t nRoleID );
	
	// 取得成员在家族中的信息
	family_info_t* GetMemberFamilyInfo( roleid_t nRoleID );

	// 判断家族成员是否拥有权利emright
	bool MemberHasRight( roleid_t nRoleID, EmRight emright );	
	
	// 判断中是否存在某个成员
	bool HasMember( roleid_t nRoleID ){ return m_tMembers.find( nRoleID )!= m_tMembers.end(); }
	
	// 获取成员信息列表
	void GetMemberInfoList( PBFamilyMemberInfoList *pbMembList  );
	
	// 通过PBFamilyMemberInfoList创建成员信息
	void CreatMemberInfo( PBFamilyMemberInfoList *pbMembList );
	
	// 加入一个玩家的申请
	int	InsertApply( int nApplyingID );
	
	// 删除一个玩家的申请
	int DeletApply( int nApplyingID );
	
	// 检查玩家的申请信息是否存在
	bool CheckApply( int nApplyingID );
	
	// 检查是否家族的申请列表已满
	bool CheckIfApplyListFull(){ return m_tApplyList.size() >= MAX_APPLYLIST_APPLIESNUM; }
	
	// 清除申请列表
	void ClearApplyList(){ m_tApplyList.clear(); }	
	
	// 获取申请列表的信息
	int GetApplyList( PBApplyList &pbApply );
	
	// 获取申请列表的玩家个数
	int GetAppplyNum( ){ return m_tApplyList.size(); }
	
	// 获取家族的成员信息列表
	int GetFamilyMemberInfoList( PBFamilyMemberList &pbFamilyMemberList, bool bCorpsInfo = false );
	
	// 获取家族的基本属性信息( 获取家族列表用 )
	int	GetFamilyBasicInfo( FamilyData &pbFamilyData );	
	
	// 获取家族的成员ID列表
	int GetFamilyMemberIDList( int *pMemberList, int &nNum, int nExceptRoleID = 0 );
	
	// 检查家族是否含有某个军团的邀请信息
	bool CheckCorpsInvite( int nCorpsID );
	
	// 插入一个军团的邀请信息
	int InsertCorpsInvite( int nCorpsID );
	
	// 删除一个军团的邀请信息
	int DeleteCorpsInvite( int nCorpsID );
	
	// 检查军团邀请信息是否已经满了
	bool CheckInviteListIfFull(  ){ return m_tInviteList.size() >=  MAX_CORPSINVITE_NUM; }
	
	// 检查是否应该解散家族
	bool CheckDisbandTime( );
	
	// 设置玩家的职位
	void SetPost( int nRoleID, labelid_t nPost, int &rOldPost, int nPostKind = EM_FAMILYACTION );
	
	// 获取玩家的职位
	int GetPost( int nRoleID, int nType, int &rPost );
public:
	// 获取家族当前等级下的成员数量上限
	int GetMemberLimitNum( );
	
	// 获取家族当前等级下的荣耀值上限
	int GetGloryLimit( );
	
	// 设置家族成员的职位
	int SetFamilyPost( int nCharID, int nPost, int &rOldPost );
	
	// 设置军团成员的职位
	int SetCorpsPost( int nCharID, int nPost, int &rOldPost );
	
	// 获取家族或者军团中某个职位的数量
	int GetNumOfCertainPost( int nPost, int nType );
	
	// 检查成员是否有某个职位 
	bool HasPost( int nRoleID, int nPost, int nPostKind );
	
	// 某种家族职位当前等级下的人数上限
	int GetPostLimitNum( int nPost );
	
	// 设置家族成员的名称
	void SetMemberName( int nRoleID, const char *pName );	   
	
	// 增加玩家的贡献度
	void AddMemberContribute( int nMemberID, int nValue, int nType = EM_FAMILYACTION );	
	
	// 减少玩家的贡献度
	void ReduceMemberContribute( int nMemberID, int nValue, int nType = EM_FAMILYACTION );	
	
	// 增加玩家的贡献度
	int GetMemberContribute( int nMemberID, int nType = EM_FAMILYACTION );	
	
	// 设置玩家的贡献度
	int SetMemberContirubte( int nMemberID, int nContribute, int nType = EM_FAMILYACTION );	
			
public:
	// 升级家族
	int LevelUPFamily( int &rLevel, int &rMoney, int &rStability );
	
	// 家族成员贡献金钱
	int ContributeMoney( int nMemberID, int nFamilyMoney );
	
	// 增加荣耀
	int AddGlory( int nGloryValue, int nGloryType = 0 );
	
	// 减少荣耀
	int ReduceGlory( int nGloryValue, int nGloryType = 0 );	
	
	// 获取家族对军团的总贡献度
	int GetTotalCorpsContribute();		
	
	// 增加pvp积分
	bool AddPVPScore( int nValue, int nFailedFamilyLevel, int nType = 0 );
	
	//  减少pvp积分
	bool ReducePVPScore( int nValue, int nWinFamilyLevel, int nType = 0 );
	
	// 获取某个职位成员列表
	int GetMemberListOfPost( int *pList, int& rNum, int nPost, int nType );
	
	// 增加家族的pvp胜利场数
	int AddPVPWinNum( int nNum );	
	
	// 增加家族的pvp失败场数
	int AddPVPFailedNum( int nNum );
	
	// 消耗家族金钱
	int ConsumeFamilyMoney( int nMoney, bool bFreeze = false );
	
	// 增加家族金钱
	int AddFamilyMoney( int nMoney, bool bFreeze = false );

	// 按天刷新一些属性
	int RefreshStatus();
	
	// 家族是否可以升级
	bool CanFamilyLevelUp();
	
	// 获取升级需要的金钱和荣耀
	int GetLevelCondition( int &rMoney, int &rGlory );
	
	// 初始化职位系统
	int InitialOrganization();
	
	// 判断职位和权限的对应关系
	bool PostHasRight( int nPost, int nRight );
	
	// 获取有某项权限的玩家列表
	int ListMemberOfRight( int *pIDList, int *pPost, int *pContributeList, int &rNum, int nRight ); 
	
	// 设置家族副本信息
	int SetFamilyRepetionInfo( KEY_TYPE nRepetionKey, int nLevel );
		
	// 清空家族副本信息
	int ClearFamilyRepetionInfo(  );		
	
	inline int GetNPCState( )
	{
		if ( m_nNpcID == 0 )
		{
			return STATE_NOBID;
		}
		else
		{
			if ( m_nBidMoney == 0 )
			{
				return STATE_OWN;
			}
			else
			{
				return STATE_CHAZING;
			}
		}
	}
	
	// 获取家族商店的错误码
	int GetShopErrcode( int nErrcode, int &rShopErrcode );
	
	// 判断家族金钱是否足够
	bool CheckFamilyMoneyEnough( int nMoney );	
	
	// 检查家族的成员信息
	int CheckFamilyRoleInfo();
	
	// 减少家族安定度
	int ReduceStabilityDegree( int nValue );
	
	// 增加家族安定度
	int AddStabilityDegree( int nValue );

	// 计算玩家累积的安定度
	int AddStabilityByMemberOnTime( int nRoleID, int nLevel, int nTime, int &rOnLineTime, int &rAddStability );

	// 清空所有玩家的在线累积时间
	int ClearMemberOnLineTime();

	// m_nWeekNumber
	void SetWeekNum( int nNum )
	{
		SetSaveStatus( FAMILY_PROPERYCHANGED );
		m_nWeekNum = nNum;
	}
	int GetWeekNum(){ return m_nWeekNum; }
}; 
  
#endif

