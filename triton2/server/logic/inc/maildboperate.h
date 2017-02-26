#pragma once
#include "mailtype.h"
#include "dbinterface.h"

#define MAIL_TABLE_COUNT	6
#define MAIL_TABLENAME_LENGTH 40   

// 邮件基本信息,存储临时用，字符串用指针
struct MailBaseInfo
{
	int        mMailID;                   // 邮件ID
	const char *mSrc;                     // 邮件发送者
	unsigned int mSendTime;               // 邮件发送时间 
	const char *mTitle;				      // 邮件接收者	
	int mType;                            // 邮件信息
	int mLevel;							  // 邮件类型
	unsigned int mExpiredTime;            // 过期时间
	int mSrcId;							  // 邮件发送方角色ID
	int mItemNum;						  // 附件数量
	int mDestId;						  // 邮件接收方ID
	const char *mDesName;				  // 邮件接收方的名字
	MailBaseInfo()
	{
		memset( this, 0, sizeof( MailBaseInfo ) );
	}
};

// 邮件信息
struct MailInfo
{
	MailBaseInfo mMailBase;                // 基本信息
	int   mMoneySend;                      // 发送的金钱
	int   mMoneyNeedBack;                  // 需要对方支付的金钱	
	const char *  mBody;		           // 邮件正文	
    char  mExtDate[ MAIL_EXTDATE_LEN ];    // 邮件附件，包括金钱
    int   mExtLen;						   // 邮件长度
	MailInfo()
	{
		memset( (char*)this, 0, sizeof( MailInfo ) );
	}
	void ClearMailInfor()
	{
		memset( (char*)this, 0, sizeof( MailInfo )  );
	}
};


// 邮件信息的数据操作

class CMailDBOperate: public CDBReceiver
{
public:
	// 发送邮件的函数指针
	typedef void (*SendMailFun)( int MailID , int MailPtr );
	
	// 获取邮件列表的函数指针
	typedef void (*GetMailListFun)( int vParameter1, int vCount, MailBaseInfo* pMailItemArray );
	
	// 读取邮件的函数指针
	typedef void (*ReadMailFun)( int vParameter1, const MailInfo* pMailInfo);

	// 获取玩家邮件信息的
    typedef void (*GetMailBoxInforFun)( int  Nationality, int MailCount , int SrcCharId , int IfPlayerExist ) ;

	// 执行邮件操作的函数指针（ 和dataserver通信 ）
	typedef void (*ExecFun)( CMessage* pMessage, int vParameter1);
	
	// 执行获取邮件的函数
	typedef void (*GetMailFun )( int tMail, MailInfo * Mailinfor , bool IfEnd );
	
public:
	// 获取玩家的邮件数量
	static int GetMailBoxStatus( int CharID, int CountryID = 1 );
	
	// 发送邮件
	static int MailSend( int  CharID, int MailCaCheID, const MailInfo *pMailInfo, bool IfNeedCallBack = false, int CountryID = 1, int nWorldCharID = 0 );
	
	// 读取邮件
	static int MailRead( ReadMailFun pFun, int vMailID, int vCharID, int CountryID = 1 );
	
	// 删除邮件
	static int MailDel(  int vMailID, int CharID = 0, int CountryID = 1 );
	
	// 获取邮件的详细信息
	static int GetMailDetailInfo( int MailID, int CharID, int CountryID = 1 );	
	
	// 更新邮件附件
	static int UpMailAccessory( int vMailID, const MailInfo *pMailInfo, int CharID = 0, int CountryID = 1 );
	
	// 更新邮件金钱和状态到数据库中
	static int UpMailStatus( int vMailID, const MailInfo *pMailInfo, int CharID = 0, int CountryID = 1 );		
	
	// 从数据库获取邮件
	static int GetMailFormDB( int ReceKind, int Charid , int MailCount , int LastMailID, int CountryID = 1 );
	
	// 不需要特殊处理的数据库操作的回调函数
	static void MailReceNothing( CMessage* pMessage, int vParameter1, int vParameter2 );	

	// 删除邮件超时
	static void MailDelTimeout( int nParam1, int nParam2 );
    
    // 发送邮件的回调函数
    static void DbMsgRece( CMessage * pMessage, int vParameter1, int vParameter2 );
    
    // 从数据库获取邮件的回调函数
    static void DBMailRece( CMessage * pMessage, int Parameter1, int Parameter2 );
    
    // 从数据库获取玩家邮件数量的回调函数
    static void GetMailCountRece( CMessage * pMessage, int Parameter1, int Parameter2 );
    
    // 从数据库获取玩家的新邮件消息
    static void CheckNewMailRece( CMessage * pMessage, int Parameter1, int Parameter2 );
    
    // 从数据库获取邮件详细信息的回调函数
	static void GetMailDetailRece( CMessage * pMessage, int Parameter1, int Parameter2 );
	
	// 获得邮件的表名
	static const char *GetMailTableName( int CharID, int CountryID );	
	
	// 检查数据库是否还有某个邮件记录
	static void CheckIfMailInDB( int nCharID, int nContryID, int nMailID );	
	
	// 转阵营时的邮件操作
	static void ChangeRoleCampMail( int nRoleID, int nOldCampID, int nNewCampID );
	
	// 好友离线留言
	static void SaveOffLineChatMsg( int nSrcRoleID, const char *pSrcName, int nDesRoleID, const char *pMsg, int nCountryID );

	// 获取离线留言
	static void LoadOffLineChatMsg( int nRoleID, int nCountryID, int nLimitCount = 30 );

	// 删除所有留言
	static void DeleteOffLineChatMsg( int nRoleID, int nCountryID );
};


