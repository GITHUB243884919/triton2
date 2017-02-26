#ifndef _LOGIN_ENTITY_H_
#define _LOGIN_ENTITY_H_

#include "servertool.h"
#include "coretype.h"
#include "object.h"
#include "log.h"
#include "lk_string.h"
#include "dbinterface.h"

#ifdef USING_ERATING
#include "SysProtocol.h"

enum em_connect_erating_flag
{
	em_connect = 0,
	em_reconnect = 1
};

#endif

// ********************************************************************** //
// CLoginPlayer
// ********************************************************************** //

class CLoginPlayer: public CObj
{
public:
#ifdef YUENAN
	enum 
	{
		USER_NAME_LEN = 50, 
		USER_PASSWD_LEN = 32,
		USER_PASSWD_DBLEN = 32,
	};
#else
	enum 
	{
		USER_NAME_LEN = 32, 
		USER_PASSWD_LEN = 128,
		USER_PASSWD_DBLEN = 32,
	};
#endif

	int		mLoginToken;	// 登陆令牌
	int		mCommHandle;	// 通讯端口
	int		mAccountID;		// 账号信息
	int 	mClientAddr; 	// 客户端IP
	time_t 	mSockTime;   	// sock的创建时间
	int 	mClientPort; 	// 客户端端口号
	time_t	mActiveTime;	// 玩家最后激活时间
	bool 	mVerifiedByERating; // 是否经过了ERating认证
	
	char	mUserName[USER_NAME_LEN+1]; 	//玩家帐号名称
	char 	mUserPasswd[USER_PASSWD_LEN+1];	//玩家密码	

	int 	m_nMatrixType; 	// 密保类型
	char 	m_szMatrixCode[USER_PASSWD_LEN+1]; // 密保
	char	m_szMatrixCoord[USER_PASSWD_LEN+1];// 密保坐标
	char 	m_szMac[USER_NAME_LEN];	// MAC地址
	
	uint8_t mAdultFlag; 	// 成人标志
	uint8_t mUserClass; 	// VIP 等级
	int32_t mUserPoint;		// 玩家积分
	uint32_t mUserFlag; 	// 跨区标志
	uint32_t mUrlID; 		// 从哪里下载的客户端

	int		mStatus;		// 用户状态 比如GM等

	int    m_nGameID;		// 哪里来的用户

	unsigned int 	m_nDiskID;		// 磁盘序列号
	unsigned int 	m_nCpuID;		// CPU 序列

	enum
	{
		STATE_INWAIT  = 1,  // 帐号验证之前
		STATE_INLOGIN = 2,	// 帐号验证之后 ---- 玩家断开链接之前
		STATE_INGATE  = 3,  // 玩家断开连接之后 ---- 玩家完成登录之前
		STATE_INGAME  = 4,  // 玩家游戏中   ---- 玩家对象销毁
	} mWhere;
				    
public:
	CLoginPlayer( ) 
	{
		if ( CObj::msCreateMode ) Initialize(); else Resume();
	}

	int Initialize()
	{
		//TODO: 
		mLoginToken  = 0;
		mVerifiedByERating = false;


		memset(mUserName, 0, sizeof(mUserName));
		memset(mUserPasswd, 0, sizeof(mUserPasswd));

		m_nMatrixType = 1;
		memset(m_szMatrixCode, 0, sizeof(m_szMatrixCode));

		mAccountID = 0;

		mUrlID = 0;
		mStatus = 0;
		memset( m_szMatrixCoord, 0, sizeof( m_szMatrixCoord ) );

		m_nGameID = 4;

		strcpy(m_szMac, "000000000000"); 

		m_nDiskID = 0;
		m_nCpuID = 0;

		return 0;
	}

	bool IsPartner()
	{
		return (mUserName[0] == '$');
	}

	int GetGameID()
	{
		return m_nGameID;
	}

	int  Resume(){ return 0; }

	~CLoginPlayer() {}
};

// ********************************************************************** //
// CLoginPlayer
// ********************************************************************** //
class CWorldServer: public CObj
{
public:
	enum 
	{
		IP_ADDRESS_LENGTH= 32,
		HOST_NAME_LENGTH = 32,
		SCENE_NUMBER = 10
	};

	char mAddress[IP_ADDRESS_LENGTH];		// 主机服务地址
	char mHostName[HOST_NAME_LENGTH];		// 主机名称
	int  mPlayerNumber[SCENE_NUMBER];		// 玩家数量
	int  mGameVersion;						// 世界版本

#ifdef USING_ERATING
	char mGateCode[AGIP_GATEWAY_CODE_LEN+1];
	char mGatePasswd[AGIP_PASSWORD_LEN+1];
	char mGateMac[AGIP_MAC_LEN+1];
#endif

	int  mRegistID;							// 注册到ERATING之后返回的GateID
	int  mWorldID; 							// 游戏自己用来标识的, 因为erating 规定得是xxx
	int  mGameID;							// 游戏ID
public:
	CWorldServer()
	{
		if ( CObj::msCreateMode ) Initialize(); else Resume();
	}

	~CWorldServer() {}

	int Initialize()
	{
		mAddress[0] = 0;
		mHostName[0] = 0;
		mWorldID = 0;
#ifdef USING_ERATING
		mGateCode[0] = 0;
		mGateMac[0] = 0;
		mGatePasswd[0] = 0;
#endif
		mGameID = 504;

		for (int i = 0; i< SCENE_NUMBER; ++i) mPlayerNumber[i] = 0;
		
		return 0;
	}

	int Resume(){ return 0; }
};

/*
class CSession: public CObj
{
public:
	int         mSessionID;
	int 		mSessionClass;
	int         mParameter1;
	int         mParameter2;
	CWTimer     mTimer;

public:
	CSession()
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

	~CSession() {}

	int Initialize()
	{
		mSessionID = 0;
		return 0;
	}

	int Resume() {return 0;}

	void assign(int session_id, int session_class, int param1, int param2, int timeout)
	{
		mSessionID = session_id;
		mSessionClass = session_class;
		mParameter1 = param1;
		mParameter2 = param2;
		mTimer = timeout;
	}
};
*/

enum 
{
	em_dbsession_null = 0x00,
	em_agipsession_handset = 0x01,
	em_agipsession_unbind, 
	em_agipsession_bind,
	em_agipsession_userlogin,
	em_agipsession_ibpayaudit,
	em_agipsession_ibitemused,
	em_agipsession_ibexchange1,
	em_agipsession_ibexchange2,
	em_agipsession_ibexchange3,
	em_agipsession_ibexchange4,
	em_dbsession_verifyacctbymysql,
	em_dbsession_roledetail,
	em_dbsession_roleproto,
	em_dbsession_ipbind,
	em_dbsession_chargeamount,
	em_dbsession_chargeaudit,
	em_dbsession_createaccount,
	em_dbsession_ibpayres,
	em_dbsession_ibpayaudit,
	em_dbsession_currentibpay,
	em_dbsession_ibtranserror,
	em_dbsession_loadibused,
	em_dbsession_goods2npc,
	em_dbsession_goodsfromnpc, 
	em_dbsession_goodsbetweenusers,
	em_dbsession_goodshistory,
	em_dbsession_operationdetail,
	em_dbsession_roleitemoperate,
	em_dbsession_rolemoneyoperate,
	em_dbsession_roleexpoperate,
	em_dbsession_itemupgrade,
	em_dbsession_corpslist,
	em_dbsession_corpsdetail,
	em_dbsession_paylistquery,
	em_dbsession_paydetaildemand, 
	em_dbsession_familylog,
	em_dbsession_corpslog,
	em_dbsession_querylogin,
	em_dbsession_querydeath,
	em_dbsession_recoverequip,

	em_dbsession_loadbourselog,
	em_agipsession_exchange,
	em_dbsession_querytask,
	em_dbsession_userlevel,
	em_dbsession_queryiplog,
	em_dbsession_query_stock1,
	em_dbsession_query_stock2,
	em_dbsession_query_stock3,
	em_dbsession_query_ybstall,
	em_dbsession_query_rolepickitemfromboss,
};

class CLoginLogic;
class SysProtocol;
class CMessage;
#ifdef USING_ERATING
typedef CSession CAGIPSession;
#endif

typedef CSession CServerSession;

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

		int Resume() { return 0; }

		enum  { CRYPT_KEY_LENGTH = 32 };

		unsigned int			mUin;					//帐号ID
		unsigned int 			mClientAddr;			//客户端的网络地址
		unsigned short 			mClientPort;			//客户端的端口号
		unsigned int 			mSockTime;				//客户端的网络地址
		unsigned short 			mSockIndex;				//客户端的端口号
		int 					mNetSpeed;				//玩家网速
		short 					mVer;					//C-S协议版本号
		unsigned char 			mKey[CRYPT_KEY_LENGTH];	//加密使用的Key
		char 					mFlag;					//客户端状态：0 - 未连接，1 - 在线， 2 - 断线
		time_t					m_tLastActiveTime;		// 最后活跃时间
};

class CIbPayAudit: public CObj
{
  public:
  	enum { em_audit_limit = 64 };

	uint32_t m_nAccountID;
	uint32_t m_nCount;
	uint8_t  m_nSubjectID;
	uint64_t m_ullEndIbTransID;
	uint64_t m_ullMinIbTransID;

	struct 
	{
		uint64_t ullIbTransID;
		uint32_t nRoleID;
		uint8_t  nMetierID;
		uint8_t  nGenderID;
		uint8_t  nRoleLevel;
		uint8_t  nRatingID;
		uint32_t nIbCode;
		uint8_t  nPackageFlag;
		uint32_t nItemCount;
		time_t   nPayTime;
		uint32_t nUserIP;
		uint32_t nPrice;
		uint32_t nDiscountPrice;       
	}m_Records[em_audit_limit];

  public:
	CIbPayAudit()
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

	~CIbPayAudit() {};

	int Initialize()
	{
		m_nAccountID = 0;
		m_nCount = 0;
		m_ullMinIbTransID = 0;
		m_ullEndIbTransID = 0;

		memset(m_Records, 0, sizeof(m_Records));

		return 0;
	}

	int Resume() { return 0; }
};

#endif



