#pragma once
#include "protype.h"

#define MAXMAILTABLENUM			6
#define MAILTABLEFIRNAME		"UMS_ROLE_MAIL_"
#define MAILMODULEID		    ( 8 )
#define MAIL_MAILBOX_MAXMAILNUM	    ( 40 )						 // 客户端能够显示的邮件封数的最大值
#define MAIL_SAVE_COUNT		    ( 240 )							 // 数据库能够保存的邮件封数最大值
#define MAIL_MAX_PAGENUM	    ( 40 )							 // 邮箱每一页的最大邮件个数	
#define MAIL_LOCKNUM_MAX	    ( 30 )							 // 能够锁定的邮件的最大数量
#define MAIL_TITLE_LEN		    ( 100 )   						 // 邮件标题长度
#define MAIL_BODY_LEN			( 1000 )						 // 邮件正文长度
#define MAIL_PAGE_ITEM_PER_LEN	( ITEMOBJ_BUFFLEN )				 // 附件物品字串流长度 
#define MAIL_ITEM_COUNT_MAX		( 5 )							 // 邮件中的最大附件个数
#define DAY_TIME				86400							 // ( 3600*24 )


#define MAIL_EXTDATE_LEN		( MAIL_ITEM_COUNT_MAX * MAIL_PAGE_ITEM_PER_LEN  ) // 附件缓存长度
#define MAIL_SEND_MONEY_PRO		( 10 )											  // 基本邮资
#define MAIL_ITEM_CHARGE        ( 100 )											  // 附件费用

#define MAIL_TIME_MONEYNEED     ( 2*24*3600 )									  // 付费邮件时限
#define MAIL_TIME_COMMON        ( 3*24*3600 )									  // 普通邮件未读时限
#define MAIL_TIME_PACKAGE       ( 30*24*3600 )									  // 包裹邮件时限，到期后返回
#define MAIL_TIME_SYS           ( 30*24*3600 )									  // 没有时限，直到玩家取出所有物品(-1)
#define MAIL_OVER_TIME			( 7 * 24 * 3600 )								  // 邮件有效时间


// Level表示邮件种类
#define MAIL_LEVEL_USER							( 1<<0  )                        // 用户邮件
#define MAIL_LEVEL_SYSTEM						( 0<<0  )                        // 系统邮件

#define MAIL_LEVEL_MONEYNEED                    ( 1<<1  )                        // 付费邮件
#define MAIL_LEVEL_BACKMAIL                     ( 1<<2  )                        // 退信
#define MAIL_LEVEL_CHATMSG                      ( 1<<3  )                        // 聊天留言



// 设置Level字段
#define MAIL_SETLEVEL_USER( vLevel )         ( vLevel |= ( 1<<0 )  )          
#define MAIL_SETLEVEL_SYSTEM( vLevel )       ( vLevel &= ~( 1<<0 ) )
#define MAIL_SETLEVEL_CHATMSG( vLevel )       ( vLevel |= ( 1<<3 ) )


#define MAIL_SETLEVEL_MONEYNEED( vLevel, boolvalue )     if( boolvalue ) vLevel |= MAIL_LEVEL_MONEYNEED; \
															else vLevel &= ~MAIL_LEVEL_MONEYNEED; 		\
															
#define MAIL_SETLEVEL_BACKMAIL( vLevel,	 boolvalue )     if ( boolvalue ) vLevel |= MAIL_LEVEL_BACKMAIL; \
															else vLevel	&= ~MAIL_LEVEL_BACKMAIL;		\
	

// Type字段表示邮件的状态
#define MAIL_TYPE_NEW 						( 1<<0 )                            // 未读邮件
#define MAIL_TYPE_ACCESSORY					( 1<<1 )                            // 有附件
#define MAIL_TYPE_LOCK                      ( 1<<2 )                            // 加锁
#define MAIL_TYPE_IFMONEYPAY                ( 1<<3 )                            // 是否已经付费
#define MAIL_TYPE_ACCECHAGED                ( 1<<4 )                            // 附件是否发生变化
#define MAIL_TYPE_NEEDSAVE                  ( 1<<5 )                            // 是否需要重新保存到数据库中
#define MAIL_TYPE_HASSENDTOCLIENT           ( 1<<6 )							// 邮件正文和附件信息是否已经发往客户端
#define MAIL_TYPE_HASBODY					( 1<<7 )							// 邮件是否含有正文

// 设置Type 字段
#define  MAIL_SETTYPE_NEW( vType , boolvalue )              if ( boolvalue) vType |= MAIL_TYPE_NEW ; else vType &= ~MAIL_TYPE_NEW ; 
#define  MAIL_SETTYPE_ACCESSORY( vType , boolvalue )        if ( boolvalue) vType |= MAIL_TYPE_ACCESSORY ; else vType &= ~MAIL_TYPE_ACCESSORY ;
#define  MAIL_SETTYPE_LOCK( vType , boolvalue )             if ( boolvalue) vType |= MAIL_TYPE_LOCK ; else vType &= ~MAIL_TYPE_LOCK ; 
#define  MAIL_SETTYPE_IFMONEYPAY( vType , boolvalue )       if ( boolvalue) vType |= MAIL_TYPE_IFMONEYPAY ; else vType &= ~MAIL_TYPE_IFMONEYPAY ;
#define  MAIL_SETTYPE_ACCECHAGED( vType , boolvalue )        if ( boolvalue) vType |= MAIL_TYPE_ACCECHAGED ; else vType &= ~MAIL_TYPE_ACCECHAGED; 
#define  MAIL_SETTYPE_NEEDSAVE( vType , boolvalue )         if ( boolvalue) vType |= MAIL_TYPE_NEEDSAVE ; else vType &= ~MAIL_TYPE_NEEDSAVE; 
#define  MAIL_SETTYPE_HASSENDTOCLIENT( vType , boolvalue )  if ( boolvalue) vType |= MAIL_TYPE_HASSENDTOCLIENT ; else vType &= ~MAIL_TYPE_HASSENDTOCLIENT; 
#define	 MAIL_SETTYPE_HASBODY( vType , boolvalue  )			if ( boolvalue) vType |= MAIL_TYPE_HASBODY ; else vType &= ~MAIL_TYPE_HASBODY; 

#define MAIL_IS_USER( vLevel )                  ( ( vLevel &  MAIL_LEVEL_USER )== MAIL_LEVEL_USER )        // 是否用户邮件
#define MAIL_IS_SYSTEM( vLevel )                ( ( vLevel &  MAIL_LEVEL_USER )== MAIL_LEVEL_SYSTEM )      // 是否系统邮件                            

#define MAIL_IS_MONEYNEED( vLevel )             ( (vLevel & MAIL_LEVEL_MONEYNEED) == MAIL_LEVEL_MONEYNEED ) // 是否付费邮件
#define MAIL_IS_BACKMAIL( vLevel )              ( (vLevel & MAIL_LEVEL_BACKMAIL) == MAIL_LEVEL_BACKMAIL )  // 是否系统退信


// 获得邮件状态
#define MAIL_IS_NEW( vType )              ( ( vType & MAIL_TYPE_NEW ) )        // 是否未读
#define MAIL_IS_ACCESSORY( vType )        ( ( vType & MAIL_TYPE_ACCESSORY ) )  // 是否带有附件
#define MAIL_IS_LOCK( vType )             ( ( vType & MAIL_TYPE_LOCK ) )       // 是否锁定
#define MAIL_IS_ACCECHAG( vType )         ( ( vType & MAIL_TYPE_ACCECHAGED ) ) // 附件是否发生变化
#define MAIL_IS_NEEDSAVE( vType )         ( ( vType & MAIL_TYPE_NEEDSAVE ) )   // 是否需要存盘
#define MAIL_IS_IFMONEYPAY( vType )       ( ( vType & MAIL_TYPE_IFMONEYPAY ) ) // 是否已付费
#define MAIL_IS_HASSENDTOCLIENT( vType )  ( ( vType & MAIL_TYPE_HASSENDTOCLIENT ) )	// 邮件附件是否已经发送给客户端
#define MAIL_IS_HASBODY( vType )		  ( ( vType & MAIL_TYPE_HASBODY ) ) // 邮件是否含有正文

#define  MAX_SYSMAILTYPE_NUM		100		// 系统邮件的类型数量

enum Method_ChangeItem
{
	ITEM_ADD,							// 发送邮件时添加附件
	ITEM_TAKE,							// 发送邮件时取下附件
	ITEM_MOVE,							// 发送时在附件列表上移动物品
};

enum FetchMailKind
{
	FETCHMAIL_PAGE,						// 从数据库获取一页的邮件信息
	FETCHMAIL_NEW,						// 从数据库获取一封新邮件
};

enum MailBoxStatus
{	 
	MAIL_STATUS_INITIALIZE   =  ( 0 ),				// 初始化状态
	MAIL_STATUS_GETCOUNT	 =  ( 1 << 1 ),			// 获取邮件数量
	MAIL_STATUS_CHECKNEWMAIL =	( 1 << 2 ),			// 检查是否含有新邮件
	MAIL_STATUS_SHUTDOWN	 =	( 1 << 3 ),			// 邮箱关闭状态
	MAIL_STATUS_OPEN    	 =	( 1 << 4 ),			// 邮箱打开状态
	MAIL_STATUS_CHECROLEINFO =	( 1 << 5 ),			// 检查邮件接收方信息
	MAIL_STATUS_GETMAILPAGE	 =	( 1 << 6 ),			// 获取一页邮件
	MAIL_STATUS_GETMAILDETAIL=	( 1 << 7 ),			// 获取邮件详细信息
	MAIL_STATUS_BACKMAIL	 =	( 1 << 8 ),			// 退信状态
	MAIL_STATUS_SENDMAIL     =	( 1 << 9 ),			// 送件箱状态
	MAIL_STATUS_READMAIL	 =	( 1 << 10 ),		// 收件箱状态		
};


enum EMail_ERROR
{
	ERROR_MAILACTION_SUCCESS = 1,		// 邮件操作成功	
	ERROR_MAILBOX_NOEXIST ,				// 玩家不存在
	ERROR_MAILBOX_CANNOTSEND,           // 邮件不能发送
	ERROR_MAILBOX_CANNOTOPEN,			// 邮箱不能打开
	ERROR_GETACCESSORY_SUCESS,			// 获取附件成功	
	ERROR_SENDMAIL_SUCESS,				// 发送邮件成功
	ERROR_CHANGEPAGE_NOMAIL,			// 客户端要求换页时已经没有邮件可以发送
	ERROR_INITIAL_NOTCOMPLETE,			// 打开邮箱时，还没有初始化完毕		
	ERROR_SENDMAIL_NOTENGHMONEY,		// 发送邮件时，邮资不够	
};

enum Email_CheckType
{
	CHECKTYPE_PAYMONEYFORMAIL	= 1,		// 因为付费邮件而检查数据库
};

enum EMail_RoleQueryType
{
	ROLEQUERYTYPE_MAILSEND				=	0,		// 普通发送邮件查询
	ROLEQUERYTYPE_MARRIAGEINVITATION	=	1,		// 婚姻的请帖查询
};

// 系统邮件类型
enum en_sys_mail_type
{
	SYSMAIL_QUESTION_AWARD = 33, // 调查问卷
	SYSMAIL_FB_AWARD = 34,  // 副本
};
enum en_mail_prize_type
{
	PRIZE_CHANGE_CAMP = 37,//转阵营奖励
};

// 系统邮件ID为成就ID+ACHIEVE_SYSMAIL_BASEID
#define  ACHIEVE_SYSMAIL_BASEID 10000

// 从网关发送到场景的邮件的发送类型
enum Em_MailSendType
{
	MAILSENDTYPE_SYS	=	0,	// 系统直接发送
	MAILSENDTYPE_BYID	=	1,	// 需要通过系统邮件ID转换邮件内容
};
