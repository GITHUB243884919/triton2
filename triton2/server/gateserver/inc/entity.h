#pragma once

#include "servertool.h"
#include "coretype.h"
#include "object.h"
#include "log.h"
#include "lk_string.h"
#include "lk_queue.h"
#include "lk_vector.h"
#include "lk_hashmap.h"
#include "dbinterface.h"
#include "familytype.h"
#include "client_comm_engine.h"

// 100311 MAOCY ADD-BEGIN
#define ROLE_VALID_QUESTION_FAILURE_COUNT 2
#define ROLE_VALID_QUESTION_RESOURCE_LENGTH 64*64*2

#define ROLE_VALID_QUESTION_COUNT 256
#define ROLE_VALID_QUESTION_GROUP_COUNT 4
#define ROLE_VALID_QUESTION_OPTION_COUNT 64
#define ROLE_VALID_QUESTION_DESCRIPTION_LENGTH 1024

#define GAME_LIST_SIZE 32

#define MAX_EXCEPTIP_NUM 50
#define MAX_MULTI_CLIENT_EXCEPT 1024
//************************************************************
// Class:       SRoleValidQuestion
// Description: 验证角色的问题定义
// Date:        2010-03-04
//************************************************************
typedef struct _SRoleValidQuestion{
   int id;
   int typeId;
   int resultId;
   char code[ROLE_VALID_QUESTION_DESCRIPTION_LENGTH];
   char description[ROLE_VALID_QUESTION_DESCRIPTION_LENGTH];
} SRoleValidQuestion;

//************************************************************
// Class:       SRoleValidQuestionOption
// Description: 验证角色的问题选项定义
// Date:        2010-03-04
//************************************************************
typedef struct _SRoleValidQuestionOption{
   int id;
   int groupId;
   int length;
   char filename[ROLE_VALID_QUESTION_DESCRIPTION_LENGTH];
   char data[ROLE_VALID_QUESTION_RESOURCE_LENGTH];
} SRoleValidQuestionOption;

//************************************************************
// Class:       SRoleValidQuestionGroup
// Description: 验证角色的问题选项组定义
// Date:        2010-03-04
//************************************************************
typedef struct _SRoleValidQuestionGroup{
   int id;
   int count;
   SRoleValidQuestionOption options[ROLE_VALID_QUESTION_OPTION_COUNT];
} SRoleValidQuestionGroup;
// 100311 MAOCY ADD-END

class CGatePlayer;

//#include "repetionmodule.h"

// ********************************************************************** //
// CGatePlayer
// ********************************************************************** //

// 注意同SAVE原因代码一致
/*
#define LEAVE_REDIRECT       1       // 玩家跨服务器时的标志               
#define LEAVE_RETURN2GATE    2       // 玩家返回网关时的标志                  
#define LEAVE_LEAVEGAME      3       // 玩家离开游戏时的标志                 
#define LEAVE_KICKOUT        4       // 玩家被踢下线时的标志                    
*/
enum EGroupType
{
	em_family_group = 1,	// 家族
	em_corps_group = 2,		// 军团
	em_union_group = 3,		// 军团联盟
};

class CGatePlayer:public CObj
{
public:
	int				mLoginToken;		// 登陆令牌
	int				mCommHandle;		// 通讯端口
	int				mLineID;			// 玩家所在线, 玩家上次下线的线ID
	int				mMapID;				// 玩家所在地图, 玩家上次下线的地图ID
	int				mAccountID;			// 帐号ID
	int				mCharID;			// 该帐号使用的角色ID
	char			mCharName[NAME_LENGTH];	// 该帐号使用的角色名
	unsigned short 	mLevel;				// 次角色上次下线的级别
	unsigned char 	mSexID;				// 此角色的性别
	unsigned char 	mMetierID;			// 此角色的职业
	unsigned char 	mFaceType;			// 此角色的脸型
	unsigned char 	mCountryID;			// 此角色归属的国家ID
	int				mTempCoID;			// 待修改，角色的国籍
	int				mOfficialID;		// 官职ID	
	int				mPKValue;
	int 			mGameID;			// 玩家从哪里来
	unsigned int 	mCpuID;				// CPUID
	unsigned int 	mDiskID;			// DISKID



	CWTimer			mCheckTimer;		// 定时检查等待玩家
	CWTPoint		mPoint;				// Return Point From Repetion
	int mClientAddr;					// 客户端IP
	time_t mSockTime;					// sock的创建时间
	char m_szMac[12+1];					// 客户端的Mac地址
	int mClientPort;					// 客户端端口号, 玩家登录游戏后则无效
	time_t m_tLastActiveTime; 			// 最后的活跃时间, 玩家登录游戏后则无效

	unsigned char	mCSKey[ KEYLEN ];	// 客户端与服务器通讯使用的密钥

	unsigned char	mAdultFlag;			// 成人标志
	int mValidSeconds;					// 防沉迷允许的正常游戏时间
	int mUserStatus;					// 用户状态 比如是否GM等 和下面这个状态是两码事！	
	int	mRoleWorldID;					// 玩家所在区组的世界ID

   // 100309 MAOCY ADD-BEGIN
   bool mRoleValid;
   int mRoleValidRetry;
   int mRoleOptionDataLength;
   char mRoleOptionData[ROLE_VALID_QUESTION_RESOURCE_LENGTH];
   // 100309 MAOCY ADD-END

	enum EmPlayerStatus
	{
		em_status_none = 0x00000000,
		em_status_ingame_idle = 0x00000001,	//ingame
		em_status_ingate_idle = 0x00000002,	//ingate_idle
		em_status_ingate_v4erating_createrole = 0x00000004,
		em_status_ingate_v4erating_entergame = 0x00000008,
		em_status_ingate_v4kickoff = 0x00000010,
		em_status_ingate_v4leavemap = 0x00000020,
		em_status_ingate_v4loginqueue = 0x00000040,
		em_status_ingate_v4mysql_createrole = 0x00000080,
		em_status_ingate_v4mysql_obtainroles = 0x00000100,
		em_status_ingate_v4scene_entergame = 0x00000200,
		em_status_ongame_v4scene_logingame = 0x00000400,
		em_status_inwait_wait = 0x00000800,
		em_status_ingate_erating_createdrole = 0x00001000,
		em_status_ingate_erating_enteredgame = 0x00002000,
		em_status_ingame_v4scene_redirectscene = 0x00004000,
		em_status_ingate_v4erating_passwdverify = 0x00008000,
		em_status_ingate_erating_passwdverify = 0x00010000,
		em_status_ingate_v4mysql_removerole = 0x00020000,		
	} mStatus;

	// 与模块相关的内容
	unsigned char mIMStatus;

	// 把每个人物的charid,lineid,mapid存起来备用

	class SaveData
	{
	public:
		unsigned int mCharID;
		unsigned short mLineID;
		unsigned short  mMapID;
		unsigned short mLevel;

		unsigned char 	mSexID;				// 此角色的性别
		unsigned char 	mMetierID;			// 此角色的职业
		unsigned char 	mFaceType;			// 此角色的脸型
		unsigned char 	mCountryID;			// 此角色归属的国家ID

		unsigned short mBackLineID;
		unsigned short mBackMapID;
		unsigned short mPosX;
		unsigned short mPosY;
		KEY_TYPE	   mKey;			//当前副本地图Key
		int			  mTempID;			// 此角色的国籍
		int			  mRoleDeleteTime;	// 角色删除时间	
		int			  mProtectTime;		// 密码的安全保护时间	
	public:
		SaveData()
		{

		}
		SaveData(	unsigned int vCharID, 
					unsigned short vLineID, 
					unsigned short vMapID, 
					unsigned char  vCountryID,
					unsigned short vLevel,
					unsigned char  vSexID,
					unsigned char  vMetierID,
					unsigned short vBackLineID = 0,
					unsigned short vBackMapID = 0,
					unsigned short vPosX = -1,
					unsigned short vPosY = -1,
					unsigned char vFaceType = 1,
					KEY_TYPE	   vKey = INVITED_KEY_VALUE,
					int			 vTempID = -1,
					int			 vRoleDeleteTime = -1									
				)
		{
			mCharID = vCharID;
			mLineID = vLineID;
			mMapID = vMapID;
			mCountryID = vCountryID;
			mLevel = vLevel;
			mSexID = vSexID;
			mMetierID = vMetierID;

			mBackLineID = vBackLineID;
			mBackMapID = vBackMapID;
			mPosX = vPosX;
			mPosY = vPosY;
			mFaceType = vFaceType;

			mKey = vKey;
			mTempID = vTempID;	
			mRoleDeleteTime = vRoleDeleteTime;				
		}
	};
	
	class CPasswordData
	{
		public:
			int mProtectTime;
			int mPasswordStatus;
			int mProtectTimeLength;			
		public:
			CPasswordData()
			{
				mProtectTime = 0;
				mPasswordStatus = EM_PASSWORD_NOTSET;
			}		
	};
	
	// 此帐号拥有的角色列表
	typedef lk::vector< SaveData, PLAYERNUM > SaveList;
	SaveList mSave;		  
		
	typedef lk::hash_map< int, lk::CString64, PLAYERNUM  > RolePwdList;
	RolePwdList mRolePwd; 
	KEY_TYPE mRepetionMap[ MAP_INDEX_MAX ];	  	
	
	typedef lk::vector< int, INVITEDLISTNUM >  FamilyInviteList;		
	FamilyInviteList mFamilyInviteList;					// 玩家收到的家族邀请
	
	typedef lk::hash_map< int, CPasswordData, PLAYERNUM > PasswordDataList;
	PasswordDataList mPasswordData;	
public:
	struct BattleJoinInfo
	{
		KEY_TYPE		mKey;
		time_t			mTime;
		int				mCamp;
		bool			mTimeLimit;

		BattleJoinInfo( )
		{
			if( CObj::msCreateMode == true )
			{
				mKey = INVITED_KEY_VALUE;
				mTime = 0;
				mCamp = 0;
				mTimeLimit = false;
			}
		}
	};
public:
	BattleJoinInfo	mJoinBattle[ MAP_INDEX_MAX ];
public:

	CGatePlayer()
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

	virtual int Initialize( )
	{
		mLoginToken = -1;
		mCharName[0] = 0;
		mCharID = -1;
		mAccountID = -1;
		mCommHandle = 0;
		mCheckTimer = 10000;
		mLineID = -1;
		mMapID = -1;
		mFaceType = 0;
		mMetierID = 0;
		mLevel = 1;
		mSexID = 0;
		mCountryID = 0;
		mTempCoID = 0;
		mOfficialID = 0;
		mPKValue = 0;

		mClientAddr = 0;
		mClientPort = 0;
		mSockTime = 0;
		mStatus = em_status_none;
		mIMStatus = 1;

		mAdultFlag = 1;
		mValidSeconds = 5 * 3600;

		mUserStatus = 0;

		mPoint.mX = -1;
		mPoint.mY = -1;

		for( size_t i = 0; i < sizeof( mRepetionMap ) / sizeof( mRepetionMap[ 0 ] ); i++ )
		{
			mRepetionMap[ i ] = INVITED_KEY_VALUE;
		}
		
      // 100309 MAOCY ADD-BEGIN
      mRoleValid = false;
      mRoleValidRetry = 0;
      mRoleOptionDataLength = 0;
      memset(mRoleOptionData, 0, ROLE_VALID_QUESTION_RESOURCE_LENGTH);
      // 100309 MAOCY ADD-END

      mSave.initailize();
		mRolePwd.initailize();
		mFamilyInviteList.initailize();
		mPasswordData.initailize();
		
		memcpy(mCSKey, ClientCommEngine::tpKey, KEYLEN);  	

		mDiskID = 0;
		mCpuID = 0;
		mRoleWorldID = 0;
		memset( m_szMac, '0',sizeof( m_szMac ));
		m_szMac[ sizeof( m_szMac ) - 1] = 0;

		return 0;
	}

	virtual int Resume() 
	{
		return 0;
	}

	~CGatePlayer()
	{

	}
public:

	// ------------------Getter & Setter--------------------------

	inline int LoginToken() const { return mLoginToken; }
	inline void LoginToken(int val) { mLoginToken = val; }

	inline int CommHandle() const { return mCommHandle; }
	inline void CommHandle(int val) { mCommHandle = val; }

	inline int LineID() const { return mLineID; }
	inline void LineID(int val) { mLineID = val; }

	inline int MapID() const { return mMapID; }
	inline void MapID(int val) { mMapID = val; }

	inline int AccountID() const { return mAccountID; }
	inline void AccountID(int val) { mAccountID = val; }

	inline int CharID() const { return mCharID; }
	inline void CharID(int val) { mCharID = val; }

	inline const char* CharName() const { return mCharName; }
	inline void CharName(const char* val) { strncpy( mCharName, val, sizeof(mCharName)-1); }

	inline unsigned short Level() const { return mLevel; }
	inline void Level(unsigned short val) { mLevel = val; }

	inline unsigned char SexID() const { return mSexID; }
	inline void SexID(unsigned char val) { mSexID = val; }

	inline unsigned char MetierID() const { return mMetierID; }
	inline void MetierID(unsigned char val) { mMetierID = val; }

	inline unsigned char FaceType() const { return mFaceType; }
	inline void FaceType(unsigned char val) { mFaceType = val; }

	inline unsigned char CountryID() const { return mCountryID; }
	inline void CountryID(unsigned char val) { mCountryID = val; }

	inline unsigned char GetNationalityID( ) const  { return mCountryID; }
	inline void SetNationalityID( unsigned char val ) { mCountryID = val; }

	inline int TempID() const { return mTempCoID; }
	inline void TempID(int val) { mTempCoID = val; }

	inline int GetGameID() const { return mGameID; }
	inline void SetGameID(int val) { mGameID = val; }

	inline unsigned int GetCpuID() const { return mCpuID; }
	inline void SetCpuID(unsigned int val) { mCpuID = val; }

	inline unsigned int GetDiskID() const { return mDiskID; }
	inline void SetDiskID(unsigned int val) { mDiskID = val; }

	inline int OfficialID() const { return mOfficialID; }
	inline void OfficialID(int val) { mOfficialID = val; }

	inline int PKValue() const { return mPKValue; }
	inline void PKValue(int val) { mPKValue = val; }

	inline unsigned char* GetCSKey() { return mCSKey; }
	inline void SetCSKey(const char* sKey) { memcpy(mCSKey, (unsigned char*) sKey, sizeof(mCSKey)); }
	inline void ResetCSKey() { memcpy(mCSKey, ClientCommEngine::tpKey, KEYLEN); }

	inline void SetMac(const char* szMac) { memcpy(m_szMac, szMac, sizeof(m_szMac)); m_szMac[sizeof(m_szMac)-1] = 0; }
	inline const char* GetMac() { return m_szMac; }
	inline void SetRoleWorldID( int nRoleWorldID ){ mRoleWorldID = nRoleWorldID; }
	inline int GetRoleWorldID(){ return mRoleWorldID; }
	void InsertRolePwd( int RoleID, const char *pPwd )
	{
		if( pPwd != NULL )
		 {
			mRolePwd.insert( RolePwdList::value_type( RoleID, lk::CString64( pPwd ) ) ); 
		 } 		 
	}
	void DeletRolePwd( int RoldeID ) 
	{			
		RolePwdList::iterator it = mRolePwd.find( RoldeID );   
		if( it != mRolePwd.end() )
		{
			mRolePwd.erase( it );
		}		
	 }
	 
	 void ModifyRolePwd( int nRoleID, const char *pPwd )
	 {			
		if ( pPwd != NULL )
		{	
			DeletRolePwd( nRoleID );
			InsertRolePwd( nRoleID, pPwd );
		}		
	 }
	 const char *GetRolePwd( int nRoleID )
	 {
		RolePwdList::iterator it = mRolePwd.find( nRoleID );
		if ( it != mRolePwd.end() )
		{
			return (it->second).c_str();
		}
		return NULL;
	 }
	int CheckRolePwd( int RoleID, const char *pPwd )
	{  
		RolePwdList::iterator it = mRolePwd.find( RoleID );	
		if( it == mRolePwd.end() )			
		{
			return -1;
		}
		
		const char *pPassword = (it->second).c_str();
		
		// 没有密码不需要验证
		if ( pPassword[0] == '\0')
		{
			return 0;
		}
		
		if ( pPwd == NULL )
		{
			return -1;
		}
		
		if ( strncmp( pPwd, (it->second).c_str(), MAX_PASSWORD_LENGTH ) == 0 )
		{
			return 0;
		}
		return -1;
	}
	
	bool CheckPwdUsed( int nRoleID )
	{
		RolePwdList::iterator it = mRolePwd.find( nRoleID );	
		if( it == mRolePwd.end() )			
		{
			return false;
		}
		
		if ( (it->second).c_str()[0] == '\0' )
		{
			return false;
		}
		return true;
	}	
	
	void DeleteFamilyInvite( int nFamilyID )
	{
		FamilyInviteList::iterator it = mFamilyInviteList.begin();
		if ( it == mFamilyInviteList.end() )
		{
			return;
		}	 
		for ( ; it != mFamilyInviteList.end(); ++it )
		{
			if ( *it == nFamilyID )
			{
				mFamilyInviteList.erase( it );
				break;
			}
		}
	}
	
	bool CheckFamilyInvite( int nFamilyID )
	{
		FamilyInviteList::iterator it = mFamilyInviteList.begin();
		if ( it == mFamilyInviteList.end() )
		{
			return false;
		}	 
		for ( ; it != mFamilyInviteList.end(); ++it )
		{
			if ( *it == nFamilyID )
			{
				return true;
				break;
			}
		}
		return false;
	}
	
	int InsertFamilyInvite( int nFamilyID )
	{	  
		bool  tRet = CheckFamilyInvite( nFamilyID );
		if ( tRet == true )
		{
			return INVITELIST_EXIST;
		}
		if ( mFamilyInviteList.size() >= INVITEDLISTNUM )
		{
			return INVITELIST_FULL;
		}
		mFamilyInviteList.push_back( nFamilyID );
		return SUCCESS;						
	}
	
	bool CheckInviteListIfFull()
	{
		return mFamilyInviteList.size() >= INVITEDLISTNUM;
	}

	void ClearFamilyInvite( ){ return mFamilyInviteList.clear(); }	

	const char* GetCharNameShow( )
	{
		#ifdef CHARSET_SHOW_GBK
		static char cCharName[ NAME_LENGTH ] = {0};
		int nNameLen = sizeof(cCharName)-1;
		charset_convert( "utf8", "gbk", mCharName, strlen(mCharName ), cCharName, (size_t*)&nNameLen );
		return cCharName;

		#else
		return mCharName;
		#endif
	}

	bool IsDealedFromQueue()
	{
		switch (mStatus) 
		{
			case em_status_ingate_v4erating_entergame:
			case em_status_ingate_v4scene_entergame:
			case em_status_ingate_erating_enteredgame:
			{

				return true;
			}

			default:
			{
				return false;
			}
		}
	}
public:
	// 角色删除状态信息
	int GetRoleDeleteStatus( int nRoleID, int &rTimeLeft, int nProtectTime, int nAllowedTime, int nCoolTime, int nProtectLevel )
	{
		rTimeLeft = 0;
		for ( SaveList::iterator it = mSave.begin(); it != mSave.end(); ++it )
		{
			if ( (*it).mCharID == (unsigned int)nRoleID )
			{
				// 先判断等级
				if ( (*it).mLevel < nProtectLevel )
				{
					return ROLEDELETESTATUS_LEVELLOW;
				}
				int tTimeNow = time( NULL );
				int tDeleteTimeLength = tTimeNow - (*it).mRoleDeleteTime;
				
				if ( (*it).mRoleDeleteTime == 0 )
				{				
					return ROLEDELETESTATUS_LIVE;
				}				
				
				if ( tDeleteTimeLength > 0 && tDeleteTimeLength < nProtectTime )
				{
					rTimeLeft = nProtectTime - tDeleteTimeLength;
					return ROLEDELETESTATUS_DELETEPROTECT;
				}				
				
				if ( tDeleteTimeLength >= nProtectTime &&  tDeleteTimeLength < nAllowedTime )
				{
					rTimeLeft = nAllowedTime - tDeleteTimeLength;  
					return	ROLEDELETESTATUS_DELETEDALLOWED;
				}				
				
				if ( tDeleteTimeLength >= nAllowedTime && tDeleteTimeLength < nCoolTime )
				{
					rTimeLeft =	 nCoolTime - tDeleteTimeLength;
					return	ROLEDELETESTATUS_DELETECOOLTIME;
				}
				
				// 取消删除等待删除状态后重置的删除时间比当前时间大
				if ( (*it).mRoleDeleteTime - tTimeNow > DELETECOOLTIMESPACE )
				{						
					rTimeLeft = (*it).mRoleDeleteTime - tTimeNow - DELETECOOLTIMESPACE;
					return 	ROLEDELETESTATUS_DELETECOOLTIME;
				}
				
				// 已经过了操作冷却期清空删除角色的状态信息
				(*it).mRoleDeleteTime = 0; 				
				break;
			}
		}
		return ROLEDELETESTATUS_LIVE;
	}
	
	// 设置角色删除时间
	void SetRoleDleteTime( int nRoleID, int nTime )
	{
		for ( SaveList::iterator it = mSave.begin(); it != mSave.end(); ++it )
		{
			if ( (*it).mCharID == (unsigned int)nRoleID )
			{
				(*it).mRoleDeleteTime = nTime;
			}
		}
	}
	
	// 获取角色删除时间
	int GetRoleDeleteTime( int nRoleID )
	{
		for ( SaveList::iterator it = mSave.begin(); it != mSave.end(); ++it )
		{
			if ( (*it).mCharID == (unsigned int)nRoleID )
			{
				return (*it).mRoleDeleteTime;
			}
		}
		return 0;
	}
	
	// 获取安全保护时间
	int GetPwdProtectTime( int nRoleID )
	{	
		PasswordDataList::iterator it = mPasswordData.find( nRoleID );
		if ( it != mPasswordData.end() )
		{
			return (it->second).mProtectTime;
		}
		return 0;
	}		
	
	// 获取安全保护时间总长
	int GetPwdProtectTimeLength( int nRoleID )
	{
		PasswordDataList::iterator it = mPasswordData.find( nRoleID );
		if ( it != mPasswordData.end() )
		{
			return (it->second).mProtectTimeLength;
		}
		return 0;		
	}
	
	// 设置密码信息
	void SetPassWordData( int nRoleID, CPasswordData &rPwdData )
	{
		PasswordDataList::iterator it = mPasswordData.find( nRoleID );
		if ( it == mPasswordData.end() )
		{
			rPwdData.mProtectTimeLength = rPwdData.mProtectTime;
			rPwdData.mProtectTime += time(NULL);			
			mPasswordData.insert( PasswordDataList::value_type( nRoleID, rPwdData ) );
		}
		else
		{
			(it->second).mProtectTimeLength = rPwdData.mProtectTime;
		}
	}
	
	// 获取密码状态
	int GetPasswordStatus( int nRoleID )
	{
		PasswordDataList::iterator it = mPasswordData.find( nRoleID );
		if ( it != mPasswordData.end() )
		{
			return (it->second).mPasswordStatus;
		}
		return	EM_PASSWORD_NOTSET;
	}	
	
	//  设置密码状态
	void SetPasswordStatus( int nRoleID, int nStatus )
	{	
		PasswordDataList::iterator it = mPasswordData.find( nRoleID );		
		if ( it != mPasswordData.end() )
		{
			(it->second).mPasswordStatus = nStatus;
		}
	}
	
	// 清除密码信息
	void ClearPasswordInfo()
	{
		mPasswordData.clear();
		mRolePwd.clear();
	}
	
	// 获取当前角色的进入副本的信息
	KEY_TYPE GetRepetionKey(unsigned int nRoleID )
	{
		for ( SaveList::iterator it = mSave.begin(); it != mSave.end(); ++it )
		{
			if ( it->mCharID == nRoleID )
			{
				return it->mKey;
			}
		}
		return 0;
	}

	// 设置当前角色的副本key
	void SetRepetionKey( unsigned int nRoleID, KEY_TYPE nKey )
	{
		for ( SaveList::iterator it = mSave.begin(); it != mSave.end(); ++it )
		{
			if ( it->mCharID == nRoleID )
			{
				 it->mKey = nKey;
			}
		}		
	}

};

// ********************************************************************** //
// CQueuePlayerInfo
// ********************************************************************** //

class CQueuePlayerInfo
{
private:
	int m_nPlayerObjID;  //对象ID

	unsigned int  m_nQueueTick; 	//排队时刻
	unsigned int  m_nReportTick;	//最近一次报告时刻
	bool 		  m_bDealed; 		//是否entergame

public:
	CQueuePlayerInfo(){}
	CQueuePlayerInfo(int nObjID, unsigned int unTick): m_nPlayerObjID(nObjID), m_nQueueTick(unTick) { m_bDealed = false; }

	int PlayerID() { return m_nPlayerObjID; }
	unsigned int QueueTick() { return m_nQueueTick; }
	unsigned int ReportTick() { return m_nReportTick; }
	bool Dealed() { return m_bDealed; }

	void SetQueueTick(unsigned int unTick) { m_nQueueTick = unTick; }	
	void SetPlerObjID(int nObjID) { m_nPlayerObjID = nObjID; }
	void SetReportTick(unsigned int unTick) { m_nReportTick = unTick; }
	void SetDealed(bool bDealed ) { m_bDealed = bDealed; }

};

// ********************************************************************** //
// CSceneServer
// ********************************************************************** //

class CSceneServer:public CObj
{
public:
	typedef lk::hash_map< int, int, GAME_LIST_SIZE > GAMEIDPLAYER;

	enum 
	{
		em_scene_is_offline = 0x00,		//离线状态
		em_scene_is_online = 0x01,		//在线状态
		em_scene_is_shutdown = 0x02,	//关闭过程
		em_scene_is_startup = 0x03, 	//启动过程
	};

	enum 
	{
		PLAYER_WAIT4LOGIN_CAP = 2000,
	};

	char 			mServerName[NAME_LENGTH];		// 服务器唯一标示
	char 			mAddress[NAME_LENGTH];			// 服务器地址
	int				mPlayerNumber;		// 服务器当前的在线人数, 网关计数统计的
	int 			mSizePermited;		// 网关配置的服务器允许的最大数
	int 			mPlayerCapacity;	// 服务器允许的容量人数
	int				mTemplateVersion;	// 模板文件版本号
	int				mTaskVersion;		// 任务文件版本号
	int				mServerVersion;		// 场景服务器版本号
	char 			mServerID;			// 只能根据ServerID取得Scene啦
	char			mLaunchMode;		// 服务器启动模式
	int 			mLaunchTimeStampt; 	// 服务器的最近启动时间
	CWTimer			mActiveTimer;		// 服务器的超时时间
	GAMEIDPLAYER	mRptPlayerNumber;	// 游戏服务器上报的有效玩家数
	int 			mPlayerPerTimer;	// 每个周期内已经登录的玩家数目
	int 			mPermitPerTimer;	// 每个周期内允许登录的玩家数目
	bool			mbIsFull;			// 服务器是否已满
	int				mLineID;			// 服务器所属的lineid

	typedef lk::vector< CQueuePlayerInfo, PLAYER_WAIT4LOGIN_CAP > CLoginQueue; 
	CLoginQueue mLoginQueue;

public:
	CSceneServer()
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

	virtual int Resume() { return 0; }

	virtual int Initialize()
	{
		mServerName[0] = 0;
		mAddress[0] = 0;

		mPlayerNumber = 0;
		mTemplateVersion = 0;
		mTaskVersion = 0;
		mServerVersion = 0;
		mServerID = 0;
		mPlayerCapacity = 100;

		mActiveTimer = CWTimer(240000);

		mRptPlayerNumber.initailize();

		mLoginQueue.initailize();

		mbIsFull = false;
		mLineID = 0;

		return 0;
	}

	void RemoveQueuePlayer(CGatePlayer* tpPlayer)
	{
		CLoginQueue :: iterator iter = mLoginQueue.begin();
		for (; iter != mLoginQueue.end(); ++iter)
		{
			if (tpPlayer->get_id() ==  iter->PlayerID()) 
			{
				mLoginQueue.erase(iter++);
				break;
			}
		}
	}

	void ResetQueuePlayer(CGatePlayer* tpPlayer)
	{
		CLoginQueue :: iterator iter = mLoginQueue.begin();
		for (; iter != mLoginQueue.end(); ++iter)
		{
			if (tpPlayer->get_id() ==  iter->PlayerID()) 
			{
				tpPlayer->mStatus = CGatePlayer::em_status_ingate_v4loginqueue ; 
				break;
			}
		}
	}

	int PlayerCount() 
	{
		GAMEIDPLAYER::iterator iter = mRptPlayerNumber.begin();
		int n = 0;
		for (; iter != mRptPlayerNumber.end(); ++iter)
		{
			n += iter->second;
		}

		return n;
	}

	bool IsFull()
	{
		return mbIsFull || (PlayerCount() >= mSizePermited);
	}
};


// ********************************************************************** //
// CSceneMap
// ********************************************************************** //

class CSceneMap:public CObj
{
public:
	unsigned short	mLineID;
	unsigned short	mMapID;
	int mServerObjID;
		
public:
	CSceneMap()
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

	virtual int Initialize()
	{
		mLineID = 0;
		mMapID = 0;

		mServerObjID = -1;
		return 0;
	}

	virtual int Resume()
	{
		return 0;
	}

public:
	static int MakeKey( unsigned short vLineID, unsigned short vMapID )	{ return ( ( vLineID << 16 ) | vMapID ); }
	static unsigned short GetLineID( int vMapKey )	{ return (unsigned short) ( vMapKey >> 16 ); }
	static unsigned short GetMapID( int vMapKey )	{ return (unsigned short) vMapKey & 0xFFFF; }
};

// ********************************************************************** //
// CClientInfo
// ********************************************************************** //
//
// 客户端连接信息

class CClientInfo
{
	public:
		CClientInfo()
		{
			if ( CObj::msCreateMode )
			{
				Initialize( );
			} 
			else
			{
				Resume( );
			}
		};

		~CClientInfo() {};

		int Initialize()
		{
			mUin = 0;
			mClientAddr = 0;
			mClientPort = 0;
			mSockTime = 0;
			mSockIndex = 0;
			mNetSpeed = 0;
			mVer = 0;
			memset( mKey, 0, sizeof(mKey) );
			mFlag = 0;
			m_tLastActiveTime = 0;

			return 0;
		}

		int Resume() { return 0; };

		unsigned int			mUin;					//帐号ID
		unsigned int 			mClientAddr;			//客户端的网络地址
		unsigned short 			mClientPort;			//客户端的端口号
		unsigned int 			mSockTime;				//客户端的网络地址
		unsigned short 			mSockIndex;				//客户端的端口号
		int 					mNetSpeed;				//玩家网速
		short 					mVer;					//C-S协议版本号
		unsigned char 			mKey[KEYLEN];			//加密使用的Key
		char 					mFlag;					//客户端状态：0 - 未连接，1 - 在线， 2 - 断线
		time_t					m_tLastActiveTime;		// 最后活跃时间
};

enum 
{
	em_dbsession_null = 0x00,
	em_dbsession_createchar = 0x01,
	em_dbsession_obtainchars = 0x02,
	em_agipsession_obtainroles = 0x03,
	em_agipsession_createrole = 0x04,
	em_agipsession_deleterole = 0x05,
	em_agipsession_gwdatareport = 0x06,
	em_agipsession_userlogout = 0x07,
	em_agipsession_entergame = 0x08,
	em_agipsession_verifypasswd = 0x09,
	em_dbsession_allroles = 0x0A,
	em_dbsession_rolelocation = 0x0B,
	em_dbsession_taskinfo = 0x0C,
	em_dbsession_freezeaccount = 0x0D,
	em_dbsession_unfreezeaccount = 0x0E,
	em_dbsession_removechar = 0x0F,
	em_dbsession_recoverrole = 0x10,
	em_agipsession_useritemlist = 0x11,
	em_agipsession_useritemminus = 0x12,
	em_dbsession_userstorageinfo = 0x13,

	em_dbsession_roledetail = 0x14,
	em_dbsession_roleequip = 0x15,
	em_dbsession_rolepackage = 0x16,
	em_dbsession_iteminpack = 0x17,
	em_dbsession_roleskill			= 0x18,
	em_dbsession_updatedeltetime	= 0x19,	
	em_dbsession_updateaccount		= 0x20,	
	/*
	em_dbsession_purchase_ibitem = 0x19,
	em_dbsession_commit_purchase = 0x1A,
	em_dbsession_rollback_purchase = 0x1B,
	*/

	em_dbsession_socialinfo = 0x1C,
	em_dbsession_activityitem = 0x1D,
	em_dbsession_list_player_money = 0x1F,


	em_dbsession_creatcorps			= 0x20,
	em_dbsession_updatecorpsinfo	= 0x21,
	em_dbsession_delcorps			= 0x22,	
	em_dbsession_loadcorps			= 0x23,	
	em_dbsession_creatfamily		= 0x24,
	em_dbsession_allfamilies		= 0x25,
	em_dbsession_getfamilynum		= 0x26,
	em_dbsession_getcorpsnum		= 0x27,
	em_dbsession_load_corpsbid		= 0x28, // 加载军团竞价信息DLC@2010-06-07
	em_dbsession_load_corpsbattle	= 0x29,
	em_dbsession_update_corpsbid	= 0x2A,	
	em_dbsession_update_corpsbattle	= 0x2B,
	em_dbsession_getcorpsbidnum		= 0x2C,
	em_dbsession_getcorpsbattlenum	= 0x2D,
	// rank 
	em_dbsession_query_rank_player_outdated	= 0x2E, // 查询玩家历史排行表
	em_dbsession_query_rank_player			= 0x2F, // 查询玩家实时排行表

	em_dbsession_save_rank		= 0x30,	// 保存历史排行表
	em_dbsession_truncate_intime_rank		= 0x31,	// 清空实时表

	em_dbsession_rolecount = 0x32,

	// judge
	em_dbsession_query_judge	= 0x41,	// 查询评价列表

	em_dbsession_query_recjudge	= 0x42,	// 查询收到评价列表

	em_dbsession_save_judge_favor			= 0x43,	// 保存评价记录 好感
	em_dbsession_save_judge_bad				= 0x44,	// 保存评价记录 恶感

	em_dbsession_delete_judge_favor		= 0x45,
	em_dbsession_delete_judge_bad		= 0x46,

	em_dbsession_create_union			= 0x47,
	em_dbsession_load_union				= 0x48,
	em_dbsession_update_union			= 0x49,
	em_dbsession_delete_union			= 0x4A,

	em_dbsession_load_country			= 0x4B,
	em_dbsession_update_country			= 0x4C,
	em_dbsession_load_countrystatus		= 0x4D,
	em_dbsession_update_countrystatus	= 0x4E,

	// create group
	em_agipsession_creategroup			= 0x4F,

	// delete achieve
	em_dbsession_del_achieve			= 0x50,	// 删除成就

	// 查询收到的评价总值
	em_dbsession_query_rec_judge_sum	= 0x51,

	em_dbsession_load_official			= 0x52,
	em_dbsession_update_official		= 0x53,

	em_agipsession_queryyuanbao			= 0x54,
	em_agipsession_drawyuanbao			= 0x55,

	em_dbsession_ibstoreinfo			= 0x56,
	em_dbsession_sysmail				= 0x57,
	em_dbsession_marriageinfo			= 0x58,	

	em_dbsession_rolemail				= 0x59,

	em_dbsession_check_bourse			= 0x5A,

	em_dbsession_load_worldwar			= 0x5B,
	em_dbsession_update_worldwar		= 0x5C,
	em_dbsession_load_worldglobal		= 0x5D,
	em_dbsession_update_worldglobal		= 0x5E,
	em_dbsession_load_worldwarchar		= 0x60,
	em_dbsession_update_worldwarchar	= 0x61,

	em_dbsession_getnpcstatusnum		= 0x70, // NPC状态表
	em_dbsession_load_npcstatus			= 0x71,
	em_dbsession_update_npcstatus		= 0x72,
	em_dbsession_roletitle				= 0x73,

    em_dbsession_activityyqs_start      = 0x74, // 摇钱树活动开始
    em_dbsession_activityyqs_end        = 0x75, // 摇钱说活动终止
    em_dbsession_activityyqs_update     = 0x76, // 更新阵营摇钱树信息

	em_dnsession_proc_active_role_info	= 0x80,	// 调用存储过程统计活跃玩家级别
	em_dbsession_matixinfo				= 0x81,	// 获取仙缘信息
	em_globalsession_kickglobalplayer   = 0x82,	// 踢玩家信息	
	em_dbsession_getglobalroledata	    = 0x83,	// 踢玩家信息	
};

#ifdef USING_ERATING                                                                                        
// ********************************************************************** //
// CAGIPSession 
//
// ********************************************************************** //

class CAGIPSession : public CSession
{
	public:
		int mParameter3;
		int mParameter4;
		char mParameter5[32+1];
};

#endif

typedef CSession CServerSession;



// ********************************************************************** //
// CRoleGlobalInfo
//
// 网关上保存一份所有玩家的必要数据信息，为了一些全局模块(主要是社会关系系统)
// 直接从网关取到玩家游戏数据，不必再去游戏服务器查.
// 各模块要根据实际情况来取数据
//
// ********************************************************************** //

class CRoleGlobalInfo: public CObj
{
	private:
		int mRoleID;					// 角色ID
		unsigned int    mAccountID; 	// 帐号ID
		char mCountryID;				// 阵营
		unsigned char	mFaceType;		// 脸型
		unsigned char	mGenderID;		// 性别
		unsigned char	mMetier;		// 职业
		unsigned short	mLevel;			// 等级
		unsigned short	mMapID;			// 地图ID
		unsigned short	mLineID;		// 线ID	
		short			mVipFlag;		// 普通会员
		unsigned int	mFamilyID;		// 家族
		int				mLastTimeLeavefamily; // 最后一次离开家族的时间
		lk::string<NAME_LENGTH> mRoleName;//角色名

	public:
		CRoleGlobalInfo()
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

		~CRoleGlobalInfo() {}

		int Initialize()
		{
			mRoleID = 0;
			mAccountID = 0;
			mCountryID = 0;
			mFaceType = 0;
			mGenderID = 0;
			mMetier = 0;
			mLevel = 1;
			mMapID = 0;
			mLineID = 0;
			mVipFlag = 0;
			mFamilyID = 0;
			mLastTimeLeavefamily = 0;
			mRoleName.clear();
			return 0; 
		}

		int Resume () 
		{ 
			return 0;
		} 


		void SetRoleID(int nRoleID) { mRoleID = nRoleID; }
		void SetRoleName( const char* szRoleName) { mRoleName = szRoleName; }
		void SetAccountID(int nAccountID) { mAccountID = nAccountID; }
		void SetCountryID(const char nCountryID) { mCountryID = nCountryID; }
		void SetGenderID(const unsigned char nGenderID) { mGenderID = nGenderID; }
		void SetFaceType(const unsigned char nFaceType) { mFaceType = nFaceType; }
		void SetLevel( unsigned short nLevel ){ mLevel = nLevel; }
		void SetMetier( unsigned short nMetier ){ mMetier = nMetier; }
		void SetFamilyID( unsigned int nFamilyID ){ mFamilyID = nFamilyID; }
		void SetMapID( int nMapID )	{ mMapID = nMapID; }
		void SetLineID( int nLineID ){ mLineID = nLineID; }									    
									  
		int RoleID() { return mRoleID;}
		const char*	 RoleName() { return mRoleName.c_str(); }
		char CountryID() { return mCountryID; }
		unsigned char FaceType() { return mFaceType; }
		unsigned char GenderID() { return mGenderID; }
		unsigned short Level(){ return mLevel; }
		unsigned char Metier(){ return mMetier; }
		unsigned int   AccountID() { return mAccountID; }
		unsigned int GetFamilyID(){ return mFamilyID; }	
		int	GetMapID(){ return mMapID; }
		int GetLineID(){ return mLineID; }		

		inline short	GetVipFlag() { return mVipFlag; }
		inline void SetVipFlag( short nVipFlag ) { mVipFlag = nVipFlag; }
		
		inline void SetLastTimeLeaveFamily( int nTime ){ mLastTimeLeavefamily = nTime; }
		inline int  GetLastTimeLeaveFamily(){ return mLastTimeLeavefamily; }
};

// ********************************************************************** //
// CIbTrans
//
// 正在进行中的交易, 必须放在共享内存中
//
// ********************************************************************** //

class CIbTrans: public CObj
{
	enum 
	{
		em_invalid_purchase = 0,
	};

  private:
	unsigned long long m_ullTransID;  // 商城交易的唯一ID
	unsigned int m_nRoleID;	          // 玩家角色ID

	int m_nGoodsID;    // 商品ID
	int m_nGoodsCount;  // 商品的数量

	int m_nItemID;     // 物品ID
	int m_nItemCount;  // 单商品包括的物品的数量

	int m_nPriceUnit;  // 商品计价单位
	int m_nPriceValue; // 单价
	int m_nDiscount;   // 折扣后价格

	int  m_nPileLimit; // 物品允许堆叠的上限
	int  m_iStatus;    // 当前交易的状态
  public:
	CIbTrans()
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

	~CIbTrans()
	{
	}

	int Initialize()
	{
		m_ullTransID = em_invalid_purchase;

		m_nRoleID = 0;

		m_nGoodsID = 0;
		m_nGoodsCount = 0;

		m_nItemID = 0;
		m_nItemCount = 0;

		m_nPriceUnit = 0;
		m_nPriceValue = 0;
		m_nDiscount = 0;

		m_nPileLimit = 0;

		return 0;
	}

	int Resume()
	{
		return 0;
	}

	// TransID
	void SetTransID(unsigned long long ullTransID) { m_ullTransID = ullTransID; }
	unsigned long long  GetTransID() { return m_ullTransID; }

	// RoleID
	void SetRoleID (unsigned int nRoleID) { m_nRoleID = nRoleID; }
	unsigned int GetRoleID() { return m_nRoleID; }

	// GoodsID
	void SetGoodsID(int nGoodsID) { m_nGoodsID = nGoodsID; }
	int GetGoodsID() { return m_nGoodsID; }

	// GoodsCount
	void SetGoodsCount(int nCount) { m_nGoodsCount = nCount; }
	int GetGoodsCount() { return m_nGoodsCount; }

	// ItemID
	void SetItemID(int nIbItemID) { m_nItemID = nIbItemID; }
	int GetItemID() { return m_nItemID; }

	// ItemCount
	void SetItemCount(int nCount) { m_nItemCount = nCount; }
	int GetItemCount() { return m_nItemCount; }

	// PriceUnit
	void SetPriceUnit(unsigned int nPriceUnit) { m_nPriceUnit = nPriceUnit; }
	int GetPriceUnit() { return m_nPriceUnit; }

	// PriceValue
	void SetPriceValue(unsigned int nPriceValue) { m_nPriceValue = nPriceValue; }
	int GetPriceValue() { return m_nPriceValue; } 

	// Discount
	void SetDiscount(unsigned int nPriceValue) { m_nDiscount = nPriceValue; }
	int GetDiscount() { return m_nDiscount; } 

	// PileLimit
	void SetPileLimit(int nPileLimit) { m_nPileLimit = nPileLimit; }
	int GetPileLimit() { return m_nPileLimit; }

};

// ********************************************************************** //
// CActivityItem
//
// 活动物品, 可以放在程序内存中，支持动态刷新
//
// ********************************************************************** //

class CActivityItem : public CObj
{
  public:
	enum 
	{
		em_property_limit = 8,
		em_jewel_limit = 4,
		em_judge_limit = 4,
	};

	CActivityItem()	
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

	~CActivityItem()
	{

	}

	int Initialize()
	{
		m_nItemID = 0;
		m_nNumber = 0;
		m_nTemplateID = 0;

		for(size_t i=0; i < sizeof(m_nProperty)/sizeof(m_nProperty[0]); ++i) 
		{
			m_nProperty[i] = 0;
		}

		m_szProducerName[0] = 0;
		m_nLevel = 0;

		for (size_t i=0; i < sizeof(m_nJudge)/sizeof(m_nJudge[0]); ++i)
		{
			m_nJudge[i] = 0;
		}

		for (size_t i = 0; i < sizeof(m_nJewelID)/sizeof(m_nJewelID[0]);  ++i)
		{
			// 默认是不可开启的槽位
			m_nJewelID[i] = -2;
		}

		m_nInitSlot = 0;
		m_nRuneID = 0;

		m_nResistLevel = 0;
		m_nResistType = 0;
		m_nAttackLevel = 0;
		m_nAttackType = 0;

		return 0;
	}

	int Resume() 
	{
		return 0;
	}

	void SetItemID(int nItemID) { m_nItemID = nItemID; }
	int  GetItemID() { return m_nItemID; }

	void SetNumber(int nNumber) { m_nNumber= nNumber; }
	int  GetNumber() { return m_nNumber; }

	void SetTemplateID(int nTemplateID) { m_nTemplateID = nTemplateID; }
	int  GetTemplateID() { return m_nTemplateID; }

	// 设置属性
	void SetProperty(size_t nIndex, int nPropertyID) 
	{
		if (nIndex >= 0 && nIndex < sizeof(m_nProperty)/sizeof(m_nProperty[0]))
		{
			m_nProperty[nIndex] = nPropertyID;
		}
	}

	int GetProperty(size_t nIndex)
	{
		if (nIndex >= 0 && nIndex < sizeof(m_nProperty)/sizeof(m_nProperty[0]))
		{
			return m_nProperty[nIndex];
		}

		return 0;
	}

	// 设置生产者
	void SetProduceName(const char* szProducerName)
	{
		strncpy(m_szProducerName, szProducerName, sizeof(m_szProducerName));
		m_szProducerName[sizeof(m_szProducerName)-1] = 0;
	}
	const char* GetProduceName() { return m_szProducerName; }

	// 设置等级
	void SetLevel(int nLevel) { m_nLevel = nLevel; }
	int GetLevel() { return m_nLevel; }

	// 设置生效标志位
	void SetValid(int nValid) { m_nValid = nValid; } 
	bool IsValid() { return m_nValid > 0; }
	void SetSpecial(int nSpecial) { m_nSpecial = nSpecial; }
	bool IsSpecial() { return m_nSpecial > 0; }

	// 设置初始装备槽个数
	void SetInitSlot(int nSlot) { m_nInitSlot = nSlot; }
	int GetInitSlot() { return m_nInitSlot; }

	// 设置宝石ID
	void SetJewelID(size_t nIndex, int nJewelID) 
	{
		if (nIndex >= 0 && nIndex < sizeof(m_nJewelID)/sizeof(m_nJewelID[0]))
		{
			m_nJewelID[nIndex] = nJewelID;
		}
	}

	int GetJewelID(size_t nIndex)
	{
		if (nIndex >= 0 && nIndex < sizeof(m_nJewelID)/sizeof(m_nJewelID[0]))
		{
			return m_nJewelID[nIndex];
		}

		return 0;
	}

	// 设置本体属性加成
	void SetJudge(size_t nIndex, int nJudge) 
	{
		if (nIndex >= 0 && nIndex < sizeof(m_nJudge)/sizeof(m_nJudge[0]))
		{
			m_nJudge[nIndex] = nJudge;
		}
	}

	int GetJudge(size_t nIndex)
	{
		if (nIndex >= 0 && nIndex < sizeof(m_nJudge)/sizeof(m_nJudge[0]))
		{
			return m_nJudge[nIndex];
		}

		return 0;
	}

	// 设置抗性
	void SetResistType(int nResistType) { m_nResistType = nResistType; }
	int GetResistType() { return m_nResistType; }

	void SetResistLevel(int nResistLevel) { m_nResistLevel = nResistLevel; }
	int GetResistLevel() { return m_nResistLevel; }

	// 设置攻击属性
	void SetAttackType(int nAttackType) { m_nAttackType = nAttackType; }
	int GetAttackType() { return m_nAttackType; }

	void SetAttackLevel(int nAttackLevel) { m_nAttackLevel = nAttackLevel; }
	int GetAttackLevel() { return m_nAttackLevel; }

	// 设置神符ID
	void SetRuneID(int nRuneID) { m_nRuneID = nRuneID; }
	int GetRuneID() { return m_nRuneID; }

	// 设置PkgCode
	void SetPkgCode(const char* szPkgCode)
	{
		strncpy(m_szPkgCode, szPkgCode, sizeof(m_szPkgCode));
		m_szPkgCode[sizeof(m_szPkgCode)-1] = 0;
	}
	const char* GetPkgCode() { return m_szPkgCode; }

	
  private:
	char m_szPkgCode[32+1];

	int	m_nItemID;          // 补偿道具的ID
	int m_nNumber;			// 道具的数量
	int m_nTemplateID;		// 道具的模板ID
	int m_nValid;			// 是否生效
	int m_nSpecial;			// 是否是设定属性值的物品
	int m_nProperty[em_property_limit];		// 每个槽的附加属性
	int m_nInitSlot;						// 装备初始槽位个数
	int m_nJewelID[em_jewel_limit];			// 镶嵌的宝石
	int m_nJudge[em_judge_limit];			// 鉴定的本体属性加成
	int m_nResistType;						// 抗性类别
	int m_nResistLevel;						// 抗性级别
	int m_nAttackType;						// 攻击类别
	int m_nAttackLevel;						// 攻击级别
	int m_nRuneID;							// 神符ID
	int m_nLevel;			// 物品的星级
	char m_szProducerName[32+1]; 	// 生产者名称
};

// ********************************************************************** //
// CActivity
//
// 活动说明, 可以放在程序内存中，支持动态刷新
//
// ********************************************************************** //
class CActivity: public CObj
{
  private:
	int  m_nMenuID;			//活动ID
	char m_szCode[32+1];	// activity_code
	char m_szDesc[256+1];	// 活动描述
	time_t m_tmBegin;		// 活动开始时间
	time_t m_tmEnd;			// 活动结束时间

  public:
	
	CActivity()	
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

	~CActivity()
	{

	}

	int Initialize()
	{
		m_nMenuID = -1;
		m_szCode[0] = 0;
		m_szDesc[0] = 0;
		m_tmBegin = 0;
		m_tmEnd = 0;

		return 0;
	}

	int Resume()
	{
		return 0;
	}

	void SetMenuID(int nMenuID) 
	{
		m_nMenuID = nMenuID;
	}

	int GetMenuID() 
	{
		return m_nMenuID;
	}

	void SetCode(const char* szCode)
	{
		strncpy(m_szCode, szCode, sizeof(m_szCode));
		m_szCode[sizeof(m_szCode)-1] = 0;
	}

	const char* GetCode() const 
	{
		return m_szCode;
	}

	void SetDesc(const char* szDesc)
	{
		strncpy(m_szDesc, szDesc, sizeof(m_szDesc));
		m_szDesc[sizeof(m_szDesc)-1] = 0;
	}

	const char* GetDesc() const
	{
		return m_szDesc;
	}

	void SetBeginTime(time_t tmBegin)
	{
		m_tmBegin = tmBegin;
	}

	time_t GetBeginTime()
	{
		return m_tmBegin;
	}

	void SetEndTime(time_t tmEnd)
	{
		m_tmEnd = tmEnd;
	}

	time_t GetEndTime()
	{
		return m_tmEnd;
	}
};

class CYuanBao
{
	private:
		int m_nValue;

		typedef lk::vector<int, 32> LOCKEDYB;
		LOCKEDYB m_lstLocked;

	public:
		CYuanBao(int value);
		~CYuanBao();

		void initialize()
		{
			m_nValue = 0;
			m_lstLocked.initailize();
		}

		// 锁定nValue
		inline bool lock(int nValue)
		{
			if ( m_nValue > nValue )
			{
				if ( m_lstLocked.push_back(nValue) != m_lstLocked.end())
				{
					m_nValue = m_nValue - nValue;
					return true;
				}
			}

			return false;
		}

		// 确认nValue
		inline bool commit(int nValue)
		{
			LOCKEDYB::iterator iter = m_lstLocked.begin();
			for ( ;iter != m_lstLocked.end(); ++iter)
			{
				if ( *iter == nValue)
				{
					m_lstLocked.erase(iter);
					return true;
				}
			}
			
			return false;
		}

		// 回滚 nValue 
		inline bool rollback(int nValue)
		{
			if ( commit(nValue) )
			{
				m_nValue += nValue;
				return true;
			}

			return false;
		}

		// 返回锁定的金额
		inline int locken()
		{
			int nlocked = 0;
			LOCKEDYB::iterator iter = m_lstLocked.begin();

			for ( ;iter != m_lstLocked.end(); ++iter) nlocked += *iter;


			return nlocked;
		}

		// 返回当前剩余的金额
		inline int remain()
		{
			return m_nValue;
		}

		// 充值
		inline void add( int nValue)
		{
			m_nValue += nValue;
		}

};
