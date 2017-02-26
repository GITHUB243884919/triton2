#pragma once

#include "sceneobj_define.h"  
#include "mailtype.h"
#include "maildboperate.h"
#include "mailmessage_pb.hxx.pb.h"
#include "servertool.h"
#include "lk_hashmap.h"


class CItemObject;
class  CEntityPlayer;


// 存储玩家每封邮件的信息
class CMailCache : public CObj
{
public:
	int			   mMailID;								                      // 邮件ID
	char           mSrcName[ NAME_LENGTH ];				                      // 发信人
	unsigned char  mLevel;                                                    // 邮件类型	
    unsigned char  mType;										              // 邮件状态	
	char           mMailTitle[MAIL_TITLE_LEN];				                  // 标题
	int			   mItemObjID[MAIL_ITEM_COUNT_MAX];							  // 邮件附件，存放附件ID
	int	           mItemNum;	       										  // 附件的数量	
	int            mExpiredTime;								              // 过期时间
	int            mSendTime;                                                 // 邮件发送时间
	int            mMoney;                                                    // 附件中的金钱	
	int            mMoneyNeedPayBack;                                         // 需要支付的金钱	
	int			   mSrcId;													  // 发送方的角色ID
	CWTimer        mMailTimer;                                                // 付费邮件计时器10分钟
public:
 	CMailCache( )
	{
		if ( CObj::msCreateMode )
		{
			Initialize();
		}				
		else
		{
			Resume( );
		}				
	}
	~CMailCache( )
	{		
	
	}
	// 初始化
	virtual int Initialize() 
	{ 	
		mMailID = 0;
		mLevel = 0;
		mType = 0;
		mExpiredTime = 0;
		mItemNum = 0;
		mMoney = 0;
		mMoneyNeedPayBack = 0;
		mSrcId = 0;
		mSendTime = 0;
		memset( mItemObjID, 0, sizeof( mItemObjID ) );
		memset( mSrcName, 0, sizeof( mSrcName ) );
		memset( mMailTitle, 0, sizeof( mMailTitle ) );		
		return 0;
	}
	// 恢复
	virtual int Resume( ) { return 0; } 
}; 

// 玩家的邮箱信息
class CMail : public CObj
{
public:
	CMail( )
	{	
		if( CObj::msCreateMode )		
		{
			Initialize();
		}
		else
		{
			Resume();
		}
	}

	virtual ~CMail( )
	{ 
		
	} 
	
	// 初始化
	virtual int Initialize(  ) 
	{
		m_nEntityID = 0;
		m_nTotalMailCount = 0;
		m_nCurCacheMailCount = 0;
		m_nMailBoxStatus = MAIL_STATUS_INITIALIZE; 
		memset( m_nList, 0, sizeof( m_nList ) );  		
		m_nActionMailID = 0;
		m_nLockMailNum = 0;
		m_nLastPageMailID = 0;
		m_nLeftPageMailNum = 0;	
		m_nLastMailID = 0;
		m_nReceiverID = 0;	
		m_vMailCache.initailize(); 
		m_nMailIDFetched = 0;	
		m_nMailFetcheCount = 0;	
		return 0; 
	}

	// 恢复
	virtual int Resume(  ) { return 0; } 
private:	 
	typedef lk::hash_map< int, int, MAIL_MAILBOX_MAXMAILNUM >  _MAIL_CACHE_LIST_TYPE_;
	struct MailItemList  
	{
		int ItemIndex;
		int ItemNum;
		MailItemList( )
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
			ItemNum   = 0;
			ItemIndex = 0;	
			return 0;
		}
		int Resume()
		{
			return 0;
		}
		
	};	
	
public:
	int						   m_nMailBoxStatus;
	_MAIL_CACHE_LIST_TYPE_     m_vMailCache;                                        // 邮件缓存          
	MailItemList               m_nList[ MAIL_ITEM_COUNT_MAX ];                      // 物品id列表（用于保存发送邮件附件信息）	
	int						   m_nReceiverID;										// 邮件接收方的角色id（发送邮件用）
    unsigned short             m_nCurCacheMailCount;                                // 当前收件箱邮件数量
	unsigned short			   m_nTotalMailCount;									// 当前数据库的邮件数量
	int                        m_nLastMailID;										// 邮件缓存里面最新一封邮件的发送日期
    CWTimer                    m_sTimer;											// 邮件计时器10分钟，每5分钟把需要存盘的邮件保存一遍
    int						   m_nLeftPageMailNum;									// 还未读取的邮件数量  
	int						   m_nLastPageMailID;									// 当前读取最后一页最后一封邮件的ID  
	int						   m_nLockMailNum;										// 锁定的邮件数量
	int						   m_nActionMailID;										// 现在正在被操作的邮件ID
	int		  			       m_nEntityID;											// 玩家实体ID
	int						   m_nMailIDFetched;									// 记录正在从数据获取邮件ID			
	unsigned short			   m_nMailFetcheCount;									// 记录正在从数据获取的新邮件ID
public:
	static MailInfo  m_sMailInfo;												    // 用于存到数据库的数据结构
public:
	/***************** 设置数据项****************/
	// 获取邮件状态
	int  GetMailStatus(  );	  
	
	// 设置邮件状态
	void SetMailStatus( int vMailStatus );	
	
	// 邮箱是否已经初始化完毕
	bool IfMailBoxInitial(  );	   	
	
public:
	/************整个邮箱的缓存*****************/
	// 玩家下线时销毁邮件缓存
	void DestroyMailCache(  );
	
	// 获取邮箱中的邮件标题信息	
	int GetMAilListInfo( CMessageMailListNotify &pMessage );	
	
	// 获取邮件内容
	int GetMailDetailInfo( int vMailID, CMessageMailReadResponse &MsgMailRead );
public:
    /************发送邮件*****************/
    
    // 设置邮件接收方角色id
	void SetMailReceiverID( int DestCharID );
	
	// 获取邮件接收方角色id
	int  GetMailReceiverID(  );
    
	// 改变发送邮件的附件
	int ChangeMailItem( int SrcIndex, int ItemNum, int DesIndex, int MovePos = ITEM_ADD );

	// 发送邮件
	int Send( int vMoney, int MoneyNeedPayBack, const char *pMailTitle, const char *pMailBody , int DesCharID, const char *pDesname );

	// 发送系统邮件
	static int SysSend( const char *pDesName, int DesCharid, const char *pTitle, const char *pSrcName, const char *pBody, CItemObject ** tpItemObj,int  ItemObjNum, int CountryID = 1, int nMoney = 0, int nCharID = 0 );

	// 把邮件存入数据库中
	static void SaveMail( CMailCache * tpMail , const char * pMailBody, int DesCharID = 0, bool IfNeedCallBack = false, const char *pDesname = NULL, int CountryID = 1, int nCharID = 0 );

	// 用物品id列表创建带发送的邮件
	CMailCache * CreatNewMail( int vMoney, int MoneyNeedPayBack, const char *pMailTitle , int nDestCharID, const char* pDesName);	
	
	// 发送邮件给本场景服务器玩家
	//static bool SendMailToPlayer( int DesCharID, CMailCache *tpMailCache );
	
	// 发送新邮件邮件通知
	static int SendNewMailNotice( int DesCharID, CMailCache *tpMailCache );
	
	// 设置/获取玩家实体ID
	void SetEntityID( int vEntityID ){ m_nEntityID = vEntityID; }
	int	 GetEntityID( )	{ return m_nEntityID; }	  
	
	// 获取玩家实体指针
	CEntityPlayer *GetEntityPlayer( );
			
	// 清空发送邮件的附件列表
	void ClearItemList( );	
	
	// 设置客户端显示的邮件ID的最大值
	int SetMaxMailID( int vMailID );
	
	// 获取客户端显示的邮件ID的最大值
	int GetMaxMailID();
	
	//	为付费邮件支付金钱
	int PayMoneyForMail( int nMailID, int &rMoney, int &rTime );

	// 返回是否有付费邮件 
	bool IsHavePayMail();
public:
	/*************读取邮件***************/ 
	
	//// 得到指定位置的邮件
	//CMailCache * GetMailCache( int vIndex, int vMailID );
	
	// 得到指定邮件id的邮件
	CMailCache *GetMailCacheByMailID( int vMailID );

	// 得到附件
	int GetAccessory( int vMailID );
	
	// 获取付费邮件的附件
	int GetAccessory( int vItemIndex, int vMailID, bool vIsMoney );
	
	// 从数据库中获得邮件放入自己的邮箱
	int GetMailFromDB( int count, int LastMailID, int ReceKind = FETCHMAIL_PAGE );

	// 给邮箱插入一封新邮件
	int InsertMail( CMailCache *tpMailCache, int InIfNewMail = 0 );	
	
	//	接收来自其他服务器的邮件
	int ReceiveRouterMail( CMessageNewMailNotify *tpMsg );
	
	// 将邮件信息更新到服务器当中
	int UpdateAccesory( CMailCache *tpMailCache );
	
	// 将邮件状态和金钱更新到数据库中
	int UpdateMailStatus( CMailCache *tpMailCache );
	
	// 更新邮件信息
	int UpdateMailInfo( int MailID );	
	
	// 更新邮件信息到数据库中
	int UpdateMail(  CMailCache *tpMailCache );

	// 读取邮件的附件和正文信息
	int GetMailDetailFromDB( int MailID );
	
	// 获取一页邮件信息
	int GetMailPage(  );
	
	// 销毁邮件附件物品
	int DestroyMailAccessory( CMailCache *tpMailCache ); 
	
	// 设置正在操作的邮件ID
	void SetActionMailID( int MailID );
	int GetActionMailID( ) { return m_nActionMailID; }	


	// 发送邮件的状态信息，新邮件个数和邮件总数
	void SendMsgMailBoxStatus( int MailCount, int NewMailCount );
	
public:
	/*************删除邮件***************/ 	
	// 删除一封邮件
	int DelMail( int MailId );
	
	// 从邮件缓存中删除邮件	
	int EraseMailCache( int MailId );
public:
	// 退信
	int BackMail(  int vMailID , const char *pMailBody );
public:	
	/*************邮件加减锁***************/ 
	// 给邮件加锁
	int LockMail( int MailId );
	
	// 给邮件解锁
	int UnLockMail( int MailId );
public:
	// 获取
    static int TestGetMail( int CharId, int count, int m_nLastMailID );
    
    // 	发送邮件的回调函数
	static void SendMailCallBack( int MailID, int MailCacheID, int CharId );
	
	// 从数据库读取邮件的回调函数
	static void RecMailFromDBCallBack( CEntityPlayer *pPlayer, CMail *tpMail, MailInfo *tpMailInfor, bool IfEnd,  CMessageMailListNotify & MsgMailList, int ReceKind = FETCHMAIL_PAGE  );
	
	// 从数据库读取邮件个数的回调函数
	static void GetMailCountCallBack( int CharID, int MailCount );
	
	// 从数据库获取新邮件个数的回调函数
	static void CheckNewMailCallBack( int CharID, int NewMailCount );
		
	// 从数据库获取一页邮件信息的回调函数
	static void GetMailPageCallBack( int CharID, int MailCount  );
	
	// 从数据库获取邮件详细信息的回调函数
	static void GetMailDetailInfoCallBack( int CharID, int MailID, MailInfo &MailDeltailInfo, bool bMailExist = true ); 
	
	// 清空邮箱
	int MailCacheClear( );		

public:
	inline int GetCharID( );
public:

	// 发送邮件时是否发送新邮件通知
	bool CanSendNewMailNotice();		
};


