#include "stdafx.hpp"
#include "westserver.h"
#include  "mailmessage_pb.hxx.pb.h"
#include "mail.h"
#include "mailmanager.h"
#include "mailmodule.h"
#include "property.h"
#include "propertymodule.h"
#include "property_pb.hxx.pb.h"
#include  "sceneobj_manager.h"
#include "sceneobj_define.h"
#include "errcode.h"   
#include "message_maker.h"
#include "logservertype.h"
#include "scenelogic.h"
#include "logmodule.h"

MailInfo     CMail::m_sMailInfo;						         // 用于存到数据库的数据结构	

// ***********************************************************
//  Function:		GetMailDetailInfoCallBack
//  Description:	获取邮件详细信息的回调函数	
//  Input:			int CharID						玩家角色id
//  Input:			int	MailID						邮件缓存对象id
//  Input:			MailInfo &MailDeltailInfo		邮件内容信息
//  OutPut:			
//  Return:			
//  Others:
//  Date:			03/18/2009		
// **********************************************************
void CMail::GetMailDetailInfoCallBack( int CharID, int MailID, MailInfo &MailDeltailInfo, bool bMailExist /* = true */ )
{
	CEntityPlayer *pPlayer = ( CEntityPlayer * )CSceneLogic::GetSingletonPtr( )->GetPlayerByCharID( CharID );
	if ( pPlayer == NULL )
	{
		// 玩家已经下线
		return;
	}
	CMail *tpMail = CMailManager::GetSingletonPtr( )->GetMail( pPlayer );
	if ( tpMail == NULL )
	{
		LOG_ERROR( "mail","[%s,%d][%s] Get CMail of CharID:%d Error! ",
			__LK_FILE__,__LINE__,__FUNCTION__,CharID );
		return;
	}
	
	// 如果邮件已经不存在，则删除客户端的邮件
	if ( !bMailExist )
	{
		CMessageDelMailResponse tMsgDelMail;
		CMessage				tMessageHead;
		tMsgDelMail.set_mailid( MailID );
		tMessageHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_DELMAIL );
		tMessageHead.set_msgpara( (int)&tMsgDelMail );
		CSceneLogic::GetSingletonPtr()->Send2Player( pPlayer, &tMessageHead );		
		return;		 		
	}
	
	//	检查状态
	if ( tpMail->GetMailStatus() != MAIL_STATUS_GETMAILDETAIL )
	{
		LOG_ERROR( "mail","[%s,%d][%s] Wrong MailBox Status of CharID:%d!",
			__LK_FILE__,__LINE__,__FUNCTION__,CharID );
	}
	
	// 设置邮件为打开状态
	tpMail->SetMailStatus( MAIL_STATUS_OPEN );	
	
	// 获取邮件缓存
	CMailCache *tpMailCache = tpMail->GetMailCacheByMailID( MailID );
	LK_ASSERT( tpMailCache != NULL, return );
	CMessageMailReadResponse MsgReadMail;
	CMessage				 MessageHead;
	MsgReadMail.set_mailid( MailID );
	if ( MailDeltailInfo.mBody != NULL )
	{
		MsgReadMail.set_mailbody( MailDeltailInfo.mBody );
		MAIL_SETTYPE_HASBODY( tpMailCache->mType, 1 );
	}
	else
	{
		MAIL_SETTYPE_HASBODY( tpMailCache->mType, 0 );
	}													  		 
		
	// 有附件
	if ( MailDeltailInfo.mExtLen > 0 )
	{
		// 有附件信息
		MailItemBox PbAccesoryBox;
		
		// 有附件信息
		bool Ret              =  PbAccesoryBox.ParseFromArray( MailDeltailInfo.mExtDate, MailDeltailInfo.mExtLen );
		if ( Ret == false )
		{
			LOG_ERROR( "mail","[%s,%d][%s] Parse Accessory From Array Error! ", __LK_FILE__,__LINE__,__FUNCTION__ );
			CSceneObjManager::GetSingletonPtr( )->DestroyObject( OBJTYPE_MAIL_MAILCACHE );
			return;
		}
		
		// 如果有附件则将附件解析出来
		tpMailCache->mItemNum =  PbAccesoryBox.mailitem_size( );
		if ( tpMailCache->mItemNum >= 0  )
		{
			CItemObject  *tpItemObj   = NULL;
			PBItemObject *tpPbItemObj = NULL; 
			CTplItem     *tpTplItem   = NULL;
			int           ItemIndex   = 0;
			
			// 创建新物品
			for( int i = 0 ; i<tpMailCache->mItemNum ; i++ )
			{
				tpPbItemObj = PbAccesoryBox.mutable_mailitem( i );
				ItemIndex	= PbAccesoryBox.itemindex( i );
				tpTplItem   =( CTplItem    * ) CDataStatic::SearchTpl( tpPbItemObj->itemid() );
				if ( tpTplItem == NULL )
				{				
					continue;
				}

				tpItemObj   = ( CItemObject  * )CItemRunTime::CloneFromBinary2( tpPbItemObj );
				if ( tpItemObj == 0 )
				{					
					continue;
				}
				tpPbItemObj = MsgReadMail.add_accessory();
				tpItemObj->GetItemBinaryBuffer2( tpPbItemObj, tpItemObj->GetItemNum() );
				MsgReadMail.add_mailitemindex( ItemIndex );							
				tpMailCache->mItemObjID[ItemIndex] = tpItemObj->get_id();				
			}
		}
	}
	MessageHead.mutable_msghead( )->set_messageid( ID_S2C_RESPONSE_READMAIL );
	MessageHead.set_msgpara( ( int )& MsgReadMail );
	CSceneLogic::GetSingletonPtr()->Send2Player( pPlayer, &MessageHead ); 
	
	// 记录日志
	LogEventReceiveMail( CharID, tpMailCache, tpMail->m_nTotalMailCount );
	
	// 如果是新邮件的话则需要存盘，改变邮件的状态
	if ( MAIL_IS_NEW( tpMailCache->mType ) )
	{
		MAIL_SETTYPE_NEW( tpMailCache->mType, 0 )
		MAIL_SETTYPE_NEEDSAVE( tpMailCache->mType, 1 )
	}	
	
	// 邮件正文已读
	MAIL_SETTYPE_HASSENDTOCLIENT( tpMailCache->mType, 1 )	
}

// ***********************************************************
//  Function:		SendMailCallBack
//  Description:	发送邮件的回调函数，获得邮件id后将邮件发送给客户端
//  Input:			int MailID				邮件id
//  Input:			int MailCacheID			邮件缓存对象id
//  Input:			int CharID				邮件接收方角色id		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			03/18/2009		
// **********************************************************
void CMail::SendMailCallBack( int MailID, int MailCacheID, int CharID )
{		
	CMailCache *tpMailCache = ( CMailCache * )CSceneObjManager::GetSingletonPtr()->GetObject( MailCacheID );
	LK_ASSERT( tpMailCache != NULL, return );
	
	// 邮件存储失败						  
	if( MailID == -1 )
	{
		LOG_ERROR( "mail","[%s,%d][%s] Empty CMailCache of CharID %d",__LK_FILE__,__LINE__,__FUNCTION__, CharID );
		CSceneObjManager::GetSingletonPtr()->DestroyObject( MailCacheID );
		return;
	}  
	tpMailCache->mMailID = MailID; 
	
	// 记录数据库, 先销毁物品
	int pItemIDList[MAIL_ITEM_COUNT_MAX] = { 0 };
	int pItemNumList[MAIL_ITEM_COUNT_MAX] = { 0 };	 
	int tItemNum = 0;	
	for ( int i = 0; i < MAIL_ITEM_COUNT_MAX; ++i )
	{
		if ( tpMailCache->mItemObjID[i] == 0 )
		{			
			continue;
		}
		CItemObject *tpItemObj = ( CItemObject * )CSceneObjManager::GetSingleton().GetObject( tpMailCache->mItemObjID[i] );
		if ( tpItemObj == NULL )
		{
			continue;
		}		
		pItemIDList[tItemNum]	 = tpItemObj->GetItemID();
		pItemNumList[tItemNum]   = tpItemObj->GetItemNum();
		++tItemNum;		
		CSceneObjManager::GetSingletonPtr()->DestroyObject( tpMailCache->mItemObjID[i] );			
	}	
	
	
	LogEventSendMail( tpMailCache->mSrcId, tpMailCache, pItemIDList, pItemNumList, tItemNum, CharID );	

	int tRet = SendNewMailNotice( CharID, tpMailCache );   		
	if ( tRet == 0 )
	{
		CSceneObjManager::GetSingletonPtr()->DestroyObject( MailCacheID );
	}	
}


// ***********************************************************
//  Function:		RecMailFromDBCallBack
//  Description:	将从数据库里面返回的邮件插入玩家邮箱中
//  Input:			int pMail								玩家CMail对象指针
//  Input:			MailInfo *tpMailInfor					邮件内容信息
//  Input:			bool IfEnd								是否是收取最后一封邮件
//  Input:			CMessageMailListNotify & MsgMailList	邮件通知列表信息	
//  Input:			int ReceKind							从数据库取邮件的类型（获取邮件列表，取新的未读邮件）
//  OutPut:			
//  Return:				
//  Others:
//  Date:			2009-02-18		
// **********************************************************
void CMail::RecMailFromDBCallBack( CEntityPlayer *pPlayer, CMail *tpMail, MailInfo *tpMailInfor, bool IfEnd, CMessageMailListNotify & MsgMailList, int ReceKind /* = FETCHMAIL_PAGE */ )
{

	LK_ASSERT( pPlayer != NULL && tpMail != NULL, return );	 

	// 发送一页邮件信息
	if ( IfEnd == true )
	{
		CMessage                   MessageHead;			
		MessageHead.mutable_msghead( )->set_messageid( ID_S2C_NOTIFY_MAILLISTINFO );
		MessageHead.set_msgpara( (int)&MsgMailList );
		CSceneLogic::GetSingletonPtr( )->Send2Player( pPlayer, &MessageHead );			
		return ;
	}
	if ( tpMailInfor == NULL )
	{
		return;
	}		
	CMailCache *tpMailCache = ( CMailCache * )CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_MAIL_MAILCACHE );
	if ( tpMailCache == NULL  )
	{
		LOG_ERROR( "mail","[%s,%d][%s] Creat CMailCache Failed! ", __LK_FILE__, __LINE__, __FUNCTION__ );
		return;
	}
	tpMailCache->mMailID           =  tpMailInfor->mMailBase.mMailID; 
	tpMailCache->mSendTime         =  tpMailInfor->mMailBase.mSendTime;
	tpMailCache->mSrcId			   =  tpMailInfor->mMailBase.mSrcId;
	if ( tpMailInfor->mMailBase.mSrc != NULL )
	{
		strncpy(  tpMailCache->mSrcName,tpMailInfor->mMailBase.mSrc ,NAME_LENGTH-1 );
		tpMailCache->mSrcName[NAME_LENGTH -1] = '\0';
	}
	tpMailCache->mLevel            =  tpMailInfor->mMailBase.mLevel;
	tpMailCache->mType             =  tpMailInfor->mMailBase.mType;
	if ( tpMailInfor->mMailBase.mTitle != NULL )
	{
		strncpy( tpMailCache->mMailTitle , tpMailInfor->mMailBase.mTitle , MAIL_TITLE_LEN - 1 );
		tpMailCache->mMailTitle[MAIL_TITLE_LEN-1] = '\0';
	}
	tpMailCache->mMoney            =  tpMailInfor->mMoneySend;
	tpMailCache->mMoneyNeedPayBack =  tpMailInfor->mMoneyNeedBack;
	tpMailCache->mExpiredTime	   =  tpMailInfor->mMailBase.mExpiredTime;	
	
	// 邮件正文未读
	MAIL_SETTYPE_HASSENDTOCLIENT( tpMailCache->mType, 0  );
	
	// 取新邮件
	if ( ReceKind == FETCHMAIL_NEW )
	{
		// 如果是新邮件插入到邮箱头		
		int nInsertRet = tpMail->InsertMail( tpMailCache );
	
		if( ( nInsertRet == 0) || (nInsertRet == 1) )
		{
			CMessageNewMailNotify MsgNewMail;
			CMessage			   MessageHead;	
			if ( tpMailInfor->mMailBase.mTitle != NULL )
			{
				MsgNewMail.mutable_maillist(  )->set_title( tpMailInfor->mMailBase.mTitle );
			}	
			MsgNewMail.mutable_maillist(  )->set_sendtime( tpMailInfor->mMailBase.mSendTime );
			MsgNewMail.mutable_maillist(  )->set_maillevel( tpMailInfor->mMailBase.mLevel );
			if ( tpMailInfor->mMailBase.mSrc != NULL )
			{
				MsgNewMail.mutable_maillist(  )->set_srcname( tpMailInfor->mMailBase.mSrc );
			}		
			MsgNewMail.mutable_maillist(  )->set_money( tpMailInfor->mMoneySend );
			MsgNewMail.mutable_maillist(  )->set_itemnum( tpMailInfor->mMailBase.mItemNum );
			MsgNewMail.mutable_maillist(  )->set_mailid( tpMailInfor->mMailBase.mMailID );	
			MsgNewMail.mutable_maillist(  )->set_mailtype( tpMailInfor->mMailBase.mType );	
			MsgNewMail.mutable_maillist(  )->set_expiredtime( tpMailInfor->mMailBase.mExpiredTime/DAY_TIME );
			MsgNewMail.mutable_maillist(  )->set_moneyneedback( tpMailInfor->mMoneyNeedBack );
			
			MsgNewMail.set_haslist( (nInsertRet == 0) ? 0:  1 );
			
			MessageHead.mutable_msghead(  )->set_messageid( ID_S2C_NOTIFY_NEWMAIL );
			MessageHead.set_msgpara( (int)&MsgNewMail );
			CSceneLogic::GetSingletonPtr()->Send2Player( pPlayer, &MessageHead );
		}

		if( nInsertRet != 1 )
		{
			CSceneObjManager::GetSingletonPtr()->DestroyObject( tpMailCache->get_id() );
		}

	}
	// 取邮件列表		
	else if ( ReceKind == FETCHMAIL_PAGE )
	{	 	
		// 如果是锁定的邮件，则锁定邮件数量++
		if ( MAIL_IS_LOCK( tpMailCache->mType ) )
		{
			++ tpMail->m_nLockMailNum;
		}
		
		// 读取一页的文件，插入到邮箱尾		
		if( tpMail->InsertMail( tpMailCache ) != 1 )
		{
			CSceneObjManager::GetSingletonPtr()->DestroyObject( tpMailCache->get_id() );
			return ;
		}

		CMailInfo *pMailInfo = MsgMailList.add_maillist( );
		if ( tpMailInfor->mMailBase.mTitle != NULL )
		{	 
			pMailInfo->set_title( tpMailInfor->mMailBase.mTitle );
		}					 			
		pMailInfo->set_sendtime( tpMailInfor->mMailBase.mSendTime );
		pMailInfo->set_maillevel( tpMailInfor->mMailBase.mLevel );
		if ( tpMailInfor->mMailBase.mSrc != NULL )
		{
			pMailInfo->set_srcname( tpMailInfor->mMailBase.mSrc );
		}		
		pMailInfo->set_money( tpMailInfor->mMoneySend );
		pMailInfo->set_itemnum( tpMailInfor->mMailBase.mItemNum );
		pMailInfo->set_mailid( tpMailInfor->mMailBase.mMailID );	
		pMailInfo->set_mailtype( tpMailInfor->mMailBase.mType );
		pMailInfo->set_expiredtime( tpMailInfor->mMailBase.mExpiredTime/DAY_TIME );	
		pMailInfo->set_moneyneedback( tpMailInfor->mMoneyNeedBack );
	}	
	return;
}	

// ***********************************************************
//  Function:		
//  Description:	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::GetCharID( )
{	
	CEntityPlayer *pPlayer = GetEntityPlayer();
	LK_ASSERT( pPlayer != NULL, return 0 );
	return pPlayer->GetCharID();
}

// ***********************************************************
//  Function:		
//  Description:	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2010-01-01		
// **********************************************************
bool CMail::CanSendNewMailNotice( )
{ 
	// 客户端能够显示的邮件缓存已满，不能插入
	if ( m_nCurCacheMailCount >= MAIL_MAILBOX_MAXMAILNUM || m_nTotalMailCount > MAIL_MAILBOX_MAXMAILNUM )
	{			
		return false;
	}		
	return 	true;
}


// ***********************************************************
//  Function:		GetMailStatus
//  Description:	获取邮件状态
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int  CMail::GetMailStatus(  )
{
	return m_nMailBoxStatus;
}

// ***********************************************************
//  Function:		vMailStatus
//  Description:	设置邮件状态，异步事件保持状态一致
//  Input:			int  vMailStatus 邮件状态
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
void CMail::SetMailStatus( int  vMailStatus )
{
	m_nMailBoxStatus = vMailStatus;
}


// ***********************************************************
//  Function:		IfMailBoxInitial
//  Description:	邮箱是否已经初始化完毕,是否获取数据库邮件数量完毕
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
bool CMail::IfMailBoxInitial(  )
{
	if ( m_nMailBoxStatus != MAIL_STATUS_INITIALIZE )
	{
		return true;
	}
	return false;
}


// ***********************************************************
//  Function:		Send	
//  Description:	发送撰写的邮件
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::Send( int vMoney, int MoneyNeedPayBack, const char *pMailTitle, const char *pMailBody, int DesCharID, const char *pDesname )
{	
	CEntityPlayer *pPlayer = GetEntityPlayer();
	if( pPlayer == NULL )
		return -1;
		
    // 计算发送需要的金钱  
    int MoneyNeedToPay = MAIL_SEND_MONEY_PRO;
	for ( int i=0; i <  MAIL_ITEM_COUNT_MAX ; i++ )
	{
		if ( m_nList[i].ItemNum > 0 )
		{
			MoneyNeedToPay = MoneyNeedToPay + MAIL_ITEM_CHARGE;
		}
	}		
	CPropertyPlayer* pProperty = ( CPropertyPlayer* ) pPlayer->GetProperty( );
		
	// 金钱不够
    if ( ( unsigned int  )( pProperty->GetMoney( ) + pProperty->GetBindMoney() ) < ( unsigned int )(  MoneyNeedToPay + vMoney )
		 || ( unsigned int )pProperty->GetMoney() < ( unsigned int )vMoney 
	   )
    {
		// 发送邮件完毕，清空附件列表
		ClearItemList( );	
		return ERR_MAIL_SENDMAIL_MONEYNOTENOUGH;
    }
    
    // 计算需要消耗的绑定金钱
    int tBindMoney = MoneyNeedToPay< pProperty->GetBindMoney()?MoneyNeedToPay:pProperty->GetBindMoney();
    int tMoneyCharge = MoneyNeedToPay - tBindMoney + vMoney; 
    	
	// 创建一份邮件缓存
	CMailCache *tpMailCache = CreatNewMail( vMoney , MoneyNeedPayBack , pMailTitle , DesCharID, pDesname);
	if ( tpMailCache == NULL )
	{
		// 创建邮件失败
		LOG_ERROR( "mail","[%s,%d][%s] Creat CMailCache Failed! ",
		 __LK_FILE__,__LINE__,__FUNCTION__ );
		return -1;
	}
	
	//// 如果收件方在线，则将邮件发送给对方
	//bool IfNeedCallBack = SendMailToPlayer( DesCharID, tpMailCache  );	
	
	// 把邮件保存到数据库	
	SaveMail( tpMailCache, pMailBody, DesCharID, true, pDesname, pPlayer->GetNationality() );
	
	// 扣本发送邮件的费用  	
	if ( tBindMoney > 0 && CPropertyModule::GetSingletonPtr( )->PlayerChangeMoney( pPlayer, tBindMoney , true, true ) == SUCCESS)
	{
		LogEventLostMoneyByPostalFee(pPlayer, CYuanBao::em_bind_money, tBindMoney, DesCharID, tpMailCache->get_id() );
	}

	if ( tMoneyCharge > 0 && CPropertyModule::GetSingletonPtr( )->PlayerChangeMoney( pPlayer, tMoneyCharge , true, false ) == SUCCESS)
	{
		LogEventLostMoneyBySendMail(pPlayer, CYuanBao::em_unbind_money, tMoneyCharge, DesCharID, tpMailCache->get_id());
	}

	// 发送邮件完毕，清空附件列表( 等收到数据库消息返回后再清空 )
	ClearItemList( );	
	return ERR_MAIL_SENDMAIL_SUCESS;	
}


// ***********************************************************
//  Function:		SendMailToPlayer	
//  Description:	发送邮件给客户端
//  Input:			int DesCharID				对方角色id
//  Input:			CMailCache *tpMailCache		邮件对象指针
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
//bool CMail::SendMailToPlayer( int DesCharID, CMailCache *tpMailCache )
//{
//	if ( tpMailCache == NULL )
//	{
//		return false;
//	}	
//	bool IfNeedCallBack = true;
//	
//	// 获取收件人的邮件信息
//	CEntityPlayer *pPlayer = ( CEntityPlayer * )CSceneLogic::GetSingleton().GetPlayerByCharID( DesCharID );
//	if ( pPlayer != NULL )
//	{
//		CMail *tpDesMail    = CMailManager::GetSingletonPtr( )->GetMail( pPlayer );
//		LK_ASSERT( tpDesMail != NULL, return  false );
//		
//		// 插入到收件方的邮件缓存中
//		bool tRet =  tpDesMail->CanSendNewMailNotice(); /*tpDesMail->InsertMail( tpMailCache, 1 );	*/	
//		if ( tRet != true )
//		{
//			IfNeedCallBack = false;	 		
//		}
//		else
//		{
//			CMessage MsgHead;
//			CMessageNewMailNotify MsgNewMail;
//			MsgNewMail.set_haslist( 0 );
//			MsgHead.set_msgpara( (int)&MsgNewMail );
//			MsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_NEWMAIL );
//			CSceneLogic::GetSingleton().Send2Player( pPlayer, &MsgHead );
//		}	  								
//	}
//	return IfNeedCallBack;
//}

// ***********************************************************
//  Function:		SendNewMailNotice	
//  Description:	发送新邮件的消息通知
//  Input:			int DesCharID				对方角色id
//  Input:			CMailCache *tpMailCache		邮件对象指针
//  OutPut:			
//  Return:			int		1表示不需要删除邮件缓存， 0表示要删除邮件缓存
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::SendNewMailNotice( int DesCharID, CMailCache *tpMailCache ) 
{  	
	LK_ASSERT( tpMailCache != NULL, return 0 );
	CMessageNewMailNotify MsgNewMail;
	CMessage			   MessageHead;	 	
	MsgNewMail.mutable_router()->set_send2player( 0 );
	MsgNewMail.mutable_router()->set_srccharid( tpMailCache->mSrcId );
	MsgNewMail.mutable_router()->set_descharid( DesCharID );
	MsgNewMail.mutable_router()->set_routertype( CMessageRouter::ROUTER_CHAR );	
	if ( tpMailCache->mMailTitle != NULL )
	{
		MsgNewMail.mutable_maillist( )->set_title( tpMailCache->mMailTitle );
	}	
	MsgNewMail.mutable_maillist( )->set_sendtime( tpMailCache->mSendTime );
	MsgNewMail.mutable_maillist( )->set_maillevel( tpMailCache->mLevel );
	if ( tpMailCache->mSrcName != NULL )
	{
		MsgNewMail.mutable_maillist(  )->set_srcname( tpMailCache->mSrcName );
	}		
	MsgNewMail.mutable_maillist(  )->set_money( tpMailCache->mMoney );
	MsgNewMail.mutable_maillist(  )->set_itemnum( tpMailCache->mItemNum );
	MsgNewMail.mutable_maillist(  )->set_mailid( tpMailCache->mMailID );	
	MsgNewMail.mutable_maillist(  )->set_mailtype( tpMailCache->mType );	
	MsgNewMail.mutable_maillist(  )->set_expiredtime( tpMailCache->mExpiredTime/DAY_TIME );
	MsgNewMail.mutable_maillist(  )->set_moneyneedback( tpMailCache->mMoneyNeedPayBack );
	MessageHead.mutable_msghead(  )->set_messageid( ID_S2C_NOTIFY_NEWMAIL );
	MessageHead.set_msgpara( (int)&MsgNewMail );
	
	CEntityPlayer *pPlayer = CSceneLogic::GetSingletonPtr( )->GetPlayerByCharID( DesCharID );
	
	// 玩家在本场景服务器
	if ( pPlayer != NULL )
	{			  
		CMail *tpMail = CMailManager::GetSingleton().GetMail( pPlayer );
		LK_ASSERT( tpMail != NULL, return 0 );
	
		int nInsertRet = tpMail->InsertMail( tpMailCache, 1 );
		if ( (nInsertRet == 0 ) || (nInsertRet == 1 ) )
		{
			MsgNewMail.set_haslist( (nInsertRet == 0) ? 0 : 1 );	

			CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &MessageHead );
		}

		if( nInsertRet != 1 )
		{
			return 0;
		}

		return 1;
	}  		
	
	// 玩家不在线或者在其他服务器
	CSceneLogic::GetSingletonPtr( )->Send2Gate( &MessageHead );
	return 0;			
}	

// ***********************************************************
//  Function:		ChangeMailItem	
//  Description:	改变发送邮件的附件
//  Input:			int SrcIndex		源索引	 
//  Input:			int ItemNum			移动的物品数量
//  Input:			int MovePos			移动方式：0表示在包裹和附件列表位置
//	之间的移动（ItemNum>0 表示从包裹到附件列表， ItemNum<=0 表示从附件列表
//	到包裹 ），1表示在附件列表位置上的移动
//  OutPut:			
//  Return:			int					1表示移动成功否则表示移动失败
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::ChangeMailItem( int SrcIndex, int ItemNum, int DesIndex, int MovePos )
{

	// 参数正确性检查
	if ( ItemNum < 0 || SrcIndex < 0  || DesIndex < 0 )
	{
		LOG_ERROR( "mail","[%s,%d][%s] WrongItemIndex，source index:%d, destination index :%d !",
			 __LK_FILE__,__LINE__,__FUNCTION__, SrcIndex, DesIndex );
		return -1;
	}	
	// 获取玩家属性
	CEntityPlayer *pPlayer = GetEntityPlayer();
	if( pPlayer == NULL )
	{
		LOG_ERROR( "mail","[%s,%d][%s] CEntityPlayer is null, entity id:%d !", 
			__LK_FILE__,__LINE__,__FUNCTION__, m_nEntityID );
		return -1;
	}
	CPropertyPlayer* tpProperty = ( CPropertyPlayer* )pPlayer->GetProperty( );		
	
	// 在附件列表之间移动物品
	if ( MovePos == ITEM_MOVE )
	{
		if ( SrcIndex >= MAIL_ITEM_COUNT_MAX || DesIndex >= MAIL_ITEM_COUNT_MAX )
		{
			LOG_ERROR( "mail","[%s,%d][%s] Wrong Item Index, source index:%d, destination index:%d!",
				 __LK_FILE__,__LINE__,__FUNCTION__, SrcIndex, DesIndex );
			return -1;
		}	
		
		// 源和目标相同		
		if( SrcIndex == DesIndex )
		{
			LOG_DEBUG( "mail","[%s,%d][%s]  same Src index and des Index, source index:%d !", 
				__LK_FILE__,__LINE__,__FUNCTION__, SrcIndex );
			return -1;
		}
		if ( m_nList[SrcIndex].ItemNum > 0  )
		{
			int MoveItemIndex =  m_nList[DesIndex].ItemNum;
			int MoveItemNum   =	 m_nList[DesIndex].ItemIndex;
			m_nList[DesIndex].ItemNum	= m_nList[SrcIndex].ItemNum;
			m_nList[DesIndex].ItemIndex = m_nList[SrcIndex].ItemIndex;
			m_nList[SrcIndex].ItemNum	= MoveItemIndex;
			m_nList[SrcIndex].ItemIndex = MoveItemNum;			
			return 1;
		}
		LOG_ERROR( "mail","[%s,%d][%s] Item num is wrong, item number:%d, source index:%d  !",
			 __LK_FILE__,__LINE__,__FUNCTION__, m_nList[SrcIndex].ItemNum, SrcIndex );
		return -1;
	}	
		
	// 在附件列表和包裹之间移动物品		
	int PackageIndex = 0;		// 移动的物品在包裹里面的索引
	if ( ItemNum > 0 )
	{
		PackageIndex = SrcIndex;
	}
	else
	{
		PackageIndex = DesIndex;
	}

	// 获取待移动的物品
	CItemObject *tpItemObj =( CItemObject * ) tpProperty->GetBaggage( )->GetItemObjPtr( PackageIndex );
	if ( tpItemObj == NULL )
	{
		// error
		LOG_ERROR( "mail","[%s,%d][%s] Item is null while sending mail, item index:%d ",
			__LK_FILE__,__LINE__,__FUNCTION__, SrcIndex );
		// 物品不存在
		return -1;
	}	
	
	// 判断物品的绑定状态
	if ( tpItemObj->GetBindStatus() != ITEM_BINDSTATUS_FREE )
	{
		// 物品已经绑定
		return ITEM_MAIL_BINDED;
	}  
		
	// 取下附件物品
	if( MovePos == ITEM_TAKE )
	{
	  
		// 索引位置合法性检查
	   if ( SrcIndex >= MAIL_ITEM_COUNT_MAX || ( !tpProperty->GetBaggage( )->CheckIndexValid( DesIndex ) ) )
	   {
			// error
			LOG_ERROR( "mail","[%s,%d][%s] wrong item index, ，source index:%d , destination index: %d !", 
				__LK_FILE__,__LINE__,__FUNCTION__, SrcIndex, DesIndex );
			return -1;
	   }
	   // 没有物品不需移动   	
	   if ( m_nList[ SrcIndex ].ItemNum <= 0 )
	   {
			// error 
			LOG_ERROR( "mail","[%s,%d][%s] wrong item index，source index:%d , destination index: %d !  !",
				 __LK_FILE__,__LINE__,__FUNCTION__, SrcIndex, DesIndex );
			return -1;
	   }
	   if (  m_nList[ SrcIndex ].ItemIndex != DesIndex  )
	   {
			LOG_ERROR( "mail","[%s,%d][%s] wrong item index，source index:%d , destination index: %d !", __LK_FILE__,__LINE__,__FUNCTION__, SrcIndex, DesIndex );
		   return -1;
	   }
	   
	   // 给物品解锁
	   if ( tpItemObj->GetLockStatus() == LOCKTYPE_MAIL )
	   {
		   CMessage HeadMessage;
		   CMessageLockItemNotice LockItemNotice ;   
		   tpProperty->GetBaggage( )->Lock( m_nList[ SrcIndex ].ItemIndex ,LOCKTYPE_NONE );	
		   HeadMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_LOCKITEM );
		   HeadMessage.set_msgpara( (int )&LockItemNotice );
		   LockItemNotice.set_index( m_nList[ SrcIndex ].ItemIndex );
		   LockItemNotice.set_locked( LOCKTYPE_NONE );
		   CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &HeadMessage ); 
	   }	 
	   m_nList[SrcIndex].ItemNum = 0;
	   m_nList[SrcIndex].ItemIndex = -1;
	}
	// 放置附件物品
	else if( MovePos == ITEM_ADD )
	{			
		// 判断物品的锁定状态
		if ( tpItemObj->GetLockStatus() != LOCKTYPE_NONE )
		{
			return -1;
		}		
	
		// 索引位置合法性检查
		if( ( !tpProperty->GetBaggage( )->CheckIndexValid( SrcIndex ) ) || DesIndex >= MAIL_ITEM_COUNT_MAX )
		{
			LOG_ERROR( "mail","[%s,%d][%s] wrong item index ，source index:%d , destination index: %d !", __LK_FILE__,__LINE__,__FUNCTION__, SrcIndex, DesIndex );
			return -1;
		}
		
		// 已经有物品不能放置物品
		if ( m_nList[ DesIndex ].ItemNum > 0 )
		{
			LOG_ERROR( "mail","[%s,%d][%s] Wrong destination item index，source index:%d , destination index: %d ",
				 __LK_FILE__,__LINE__,__FUNCTION__, SrcIndex, DesIndex);
			return -1;	
		}			
		
		// 给物品加锁
		CMessage HeadMessage;
		CMessageLockItemNotice LockItemNotice ;   
		m_nList[DesIndex].ItemNum = ItemNum;			// 记录数量
		m_nList[DesIndex].ItemIndex = SrcIndex;			// 记录物品在包裹中的索引
		tpProperty->GetBaggage( )->Lock( SrcIndex ,LOCKTYPE_MAIL );
		HeadMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_LOCKITEM );
		HeadMessage.set_msgpara( (int )&LockItemNotice );
		LockItemNotice.set_index( SrcIndex );
		LockItemNotice.set_locked( LOCKTYPE_MAIL );
		CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &HeadMessage ); 
	}	
	return SUCCESS;
}

// ***********************************************************
//  Function:		SetMailReceiverID	
//  Description:	设置邮件接收方角色ID
//  Input:			int DestCharID		玩家角色id	 
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
void CMail::SetMailReceiverID( int DestCharID )
{
	m_nReceiverID = DestCharID;
}


// ***********************************************************
//  Function:		GetMailReceiverID	
//  Description:	获取邮件接收方角色ID
//  OutPut:			
//  Return:			int				邮件接收方角色id	
//  Others:
//  Date:			2009-02-18		
// **********************************************************

int CMail::GetMailReceiverID()
{
	return m_nReceiverID;
}


// ***********************************************************
//  Function:	  GetAccessory	
//  Description:  获取收费邮件的附件	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::GetAccessory( int vMailID )
{
	
	CMailCache *tpMailCache = GetMailCacheByMailID( vMailID );
	if ( tpMailCache == NULL )
	{
		return -1;
	}	
	CEntityPlayer *pPlayer = GetEntityPlayer();
	if ( pPlayer == NULL )
	{
		return -1;
	}
	CPropertyPlayer* tpProperty = (CPropertyPlayer*)pPlayer->GetProperty();
	CTemplateLevelMoneyLmt* tpMoneyTable = (CTemplateLevelMoneyLmt*)CDataStatic::GetTemp(TEMP_MONEYLMT_CFG);
	LK_ASSERT( tpMoneyTable!=NULL, return -1 );
	LK_ASSERT( pPlayer->GetLevel() >= 0 && pPlayer->GetLevel() < TMP_MAX_GRADE, return -1 );

	// 判断金钱上限
	if(  tpProperty->GetMoney() + tpMailCache->mMoney > MONEY_LIMIT 
		|| (unsigned int)(tpProperty->GetMoney() + tpMailCache->mMoney) > tpMoneyTable->mLmtTable[pPlayer->GetLevel()].mBagMoneyLmt ) 
	{
		return ERR_BAG_MONEY_UPLMT;
	}

	// 获取金钱
	if (CPropertyModule::GetSingletonPtr( )->PlayerChangeMoney( pPlayer, tpMailCache->mMoney, false, false ) == SUCCESS)
	{
		LogEventGetMoneyByRecvMail(pPlayer, 
				CYuanBao::em_unbind_money, tpMailCache->mMoney, tpMailCache->mSrcId, vMailID, tpMailCache->mSendTime );
	}
	
	/*
		把金钱发送给邮件发送方
	*/
	
	tpMailCache->mMoney = 0;	
	/*
		判断包裹格子数量
	*/ 
	// 获取附件
	for ( int i = 0; i<MAIL_ITEM_COUNT_MAX; i ++ )
	{
		if ( tpMailCache->mItemObjID[i] == 0 )
		{
			continue;
		}
		CItemObject *tpObj = ( CItemObject * )CSceneObjManager::GetSingletonPtr()->GetObject( tpMailCache->mItemObjID[i] );
		if ( tpObj == NULL )
		{
			continue;
		}

		int nItemNum = tpObj->GetItemNum();
		uint64_t ullGUID = tpObj->GetGUID();

		if (CPropertyModule::GetSingletonPtr( )->PlayerInsertItem( pPlayer, tpObj ));
		{
			LogEventGetItemByRecvMail(pPlayer, ullGUID, tpObj->GetItemID(), nItemNum, 
					tpMailCache->mSrcId, vMailID, tpMailCache->mSendTime );
		}

		tpMailCache->mItemObjID[i]  = 0;
	}  
	return 0;
}



// ***********************************************************
//  Function:		GetAccessory
//  Description:	获取普通邮件附件	
//  Input:			int vItemIndex			附件物品索引
//  Input:			int vMailID				邮件ID
//  Input:			bool vIsMoney			是否金钱
//  OutPut:			
//  Return:			1 表示获取成功，2表示成功并且需要从客户端列表里面删除邮件，
//					-1表示失败, 其他表示错误提示码
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::GetAccessory( int vItemIndex, int vMailID, bool vIsMoney )
{
	// 获取邮件
	CMailCache *tpMailCache = GetMailCacheByMailID( vMailID );
	if( tpMailCache == NULL )
	{	
		return -1;
	}
	
	// 索引合法性检查
	if ( vItemIndex < 0 || vItemIndex >= MAIL_ITEM_COUNT_MAX )
	{
		return -1;
	}
	
	// 判断是否付费邮件
	if ( tpMailCache->mMoneyNeedPayBack > 0 )
	{
		return ERR_MAIL_GETACCESSORY_MONEYNOTPAY;
	}
	
	// 获取人物属性
	CEntityPlayer *pPlayer = GetEntityPlayer();
	if ( pPlayer == NULL )
	{
		return -1;
	}
	CPropertyPlayer *tpProperty = ( CPropertyPlayer *)pPlayer->GetProperty();  
	
	// 如果邮件已经过期，不能取附件
	if ( tpMailCache->mExpiredTime <= time( NULL ) )
	{
		return ERR_MAIL_GETACCESSORY_EXPIRED;
	}

	// 提取附件中的金钱
	if ( vIsMoney == true )
	{
		CTemplateLevelMoneyLmt* tpMoneyTable = (CTemplateLevelMoneyLmt*)CDataStatic::GetTemp(TEMP_MONEYLMT_CFG);
		LK_ASSERT( tpMoneyTable!=NULL, return -1 );
		LK_ASSERT( pPlayer->GetLevel() >= 0 && pPlayer->GetLevel() < TMP_MAX_GRADE, return -1 );

		// 判断金钱上限
		if(  tpProperty->GetMoney() + tpMailCache->mMoney > MONEY_LIMIT 
			|| (unsigned int)(tpProperty->GetMoney() + tpMailCache->mMoney) > tpMoneyTable->mLmtTable[pPlayer->GetLevel()].mBagMoneyLmt ) 
		{
			return ERR_BAG_MONEY_UPLMT;
		}
		
		if ( tpMailCache->mMoney > 0 )
		{
			if (CPropertyModule::GetSingletonPtr()->PlayerChangeMoney( pPlayer, tpMailCache->mMoney, false , false ) == SUCCESS)
			{
				LogEventGetMoneyByRecvMail( pPlayer,
						CYuanBao::em_unbind_money, tpMailCache->mMoney, vMailID, tpMailCache->mSrcId, tpMailCache->mSendTime);
			}

			tpMailCache->mMoney = 0;
								
			// 邮件已经改变需要重新存盘
			MAIL_SETTYPE_NEEDSAVE( tpMailCache->mType, 1 )
			
			// 改变一下当前被操作的邮件ID
			// ChangeActionMailID( vMailID );	
		}			
		
	}
	// 提取附件物品
	else
	{
		int ItemID = tpMailCache->mItemObjID[ vItemIndex ];
		if ( ItemID == 0 )
		{
			return -1;
		}
		CItemObject *tpItemObj = ( CItemObject * )CSceneObjManager::GetSingletonPtr()->GetObject( ItemID );
		if ( tpItemObj == NULL )
		{
			return -1;
		}

		// 判断包裹是否已满
		if ( !tpProperty->GetBaggage( )->Space( tpItemObj->GetItemID(), tpItemObj->GetItemNum() ) )
		{
			return ERR_MAIL_GETACCESSORY_NOPACKAGESPACE;
		}


		int64_t ullGUID = tpItemObj->GetGUID();
		int nItemNum = tpItemObj->GetItemNum();

		// 把附件物品插入到玩家包裹中
		if ( CPropertyModule::GetSingletonPtr( )->PlayerInsertItem( pPlayer, tpItemObj))
		{
			LogEventGetItemByRecvMail( pPlayer, ullGUID, tpItemObj->GetItemID(), nItemNum,
						tpMailCache->mSrcId, tpMailCache->mMailID,  tpMailCache->mSendTime);
		}
		else
		{
			LOG_ERROR( "mail", "Player(RoleID:%d) get item( ItemID:%d, ItemNum:%d ) accessory error !", pPlayer->GetCharID(),
					tpItemObj->GetItemID(), tpItemObj->GetItemNum() );
		}

		tpMailCache->mItemObjID[ vItemIndex ] = 0;	

		// 邮件已经改变需要重新存盘
		MAIL_SETTYPE_NEEDSAVE( tpMailCache->mType, 1 )

		// 附件发生改变
		MAIL_SETTYPE_ACCECHAGED( tpMailCache->mType, 1 )
		--tpMailCache->mItemNum;		
	}

	// 改变一下当前被操作的邮件ID
	// ChangeActionMailID( vMailID );
	
	if ( tpMailCache->mItemNum == 0 && tpMailCache->mMoney == 0 )
	{
		// 没有附件，邮件状态变为普通邮件
		MAIL_SETTYPE_ACCESSORY( tpMailCache->mType, 0 )

		// 邮件已经改变需要重新存盘
		MAIL_SETTYPE_NEEDSAVE( tpMailCache->mType, 1 )
		
		// 改变到期时间
		int tExpiredTime =	(time(NULL)+MAIL_TIME_COMMON);///DAY_TIME;
		tpMailCache->mExpiredTime = tExpiredTime>tpMailCache->mExpiredTime?tpMailCache->mExpiredTime:tExpiredTime; 
		
		// 如果是退信或者是系统邮件或者是没有正文的邮件的话立即删除邮件			
		if ( !MAIL_IS_HASBODY( tpMailCache->mType ) || MAIL_IS_BACKMAIL( tpMailCache->mLevel) || MAIL_IS_SYSTEM( tpMailCache->mLevel ) )
		{	
			// 需要从客户端列表里面删除邮件		
			DelMail( vMailID );
			return 2;
		}
	}	

	// 立即更新到数据库
	UpdateMail( tpMailCache );	
	return 1;	  
}

// ***********************************************************
//  Function:		GetMailFromDB
//  Description:	从数据库读取邮件	
//  Input:			int count			获取邮件的数量
//  Input:			int LastMailID		从数据库获取的最后一封邮件的id
//  Input:			int ReceKind		获取类型（ 获取邮件列表还是获取新邮件 ）
//  OutPut:							   
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::GetMailFromDB( int count, int LastMailID, int ReceKind ) 
{			
	CEntityPlayer *pPlayer = GetEntityPlayer();
	LK_ASSERT( pPlayer != NULL, return -1 );
	CMailDBOperate::GetMailFormDB( ReceKind , GetCharID(), count , LastMailID, pPlayer->GetNationality() );
	return 0;
}

// ***********************************************************
//  Function:		MailCacheClear
//  Description:	清空邮件缓存	
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::MailCacheClear( )
{
	m_vMailCache.clear();
	return 0;
}

// ***********************************************************
//  Function:		SysSend
//  Description:	发送系统邮件	
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18			
// **********************************************************
int CMail::SysSend( const char *pDesName, int DesCharid, const char *pTitle, const char *pSrcName, const char *pBody, CItemObject ** tpItemObj,int ItemObjNum, int CountryID /* = 1 */, int nMoney /* = 0 */, int nCharID /* = 0 */ )
{		
	CMailCache *tpMailCache = ( CMailCache * )CSceneObjManager::GetSingletonPtr()->CreateObject( OBJTYPE_MAIL_MAILCACHE );
	if ( tpMailCache == NULL )
	{
		return -1;
	}
	
	// 设置邮件状态
	MAIL_SETLEVEL_SYSTEM( tpMailCache->mLevel );
	int ItemNum = 0;
	
	// 获取附件信息
	if ( tpItemObj != NULL && ItemObjNum > 0 )
	{
		while ( tpItemObj[ItemNum] != NULL )
		{
			tpMailCache->mItemObjID[ItemNum] = tpItemObj[ItemNum]->get_id( );
			++ItemNum;
			if( ItemNum >= ItemObjNum )
			{
				break;
			}
		}
	} 
		
 	// 判断是否含有附件
    if ( ItemNum >= 0 )
	{
		MAIL_SETTYPE_ACCESSORY( tpMailCache->mLevel,1 );
	}
	
	MAIL_SETLEVEL_MONEYNEED( tpMailCache->mLevel, false )
	
	MAIL_SETTYPE_NEW( tpMailCache->mType, true )
	
	// 设置邮件时间
	tpMailCache->mSendTime		= time( NULL );
	tpMailCache->mExpiredTime	= ( tpMailCache->mSendTime + MAIL_TIME_SYS );///DAY_TIME;
	
	tpMailCache->mMoney = nMoney;
	tpMailCache->mItemNum = ItemNum;
	
	// 邮件发送人
	if ( pSrcName != NULL )
	{
		strncpy( tpMailCache->mSrcName, pSrcName, NAME_LENGTH - 1 );
	}	
	
	// 邮件标题
	if ( pTitle != NULL )
	{
		strncpy( tpMailCache->mMailTitle, pTitle, MAIL_TITLE_LEN-1 );
	}
	
	//// 如果对方在线发送给对方玩家
	//bool IfNeedCallBack =  SendMailToPlayer( DesCharid, tpMailCache );
	
	// 把邮件保存到数据库
	SaveMail( tpMailCache, pBody, DesCharid, true, pDesName, CountryID, nCharID );	
	
	//// 如果玩家不在线或者邮箱已满，删除邮件
	//if ( IfNeedCallBack == false )
	//{
	//	CSceneObjManager::GetSingletonPtr()->DestroyObject( tpMailCache->get_id() );
	//}		
	return 0;
}

// ***********************************************************
//  Function:		DelMail	
//  Description:	删除邮件
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::DelMail( int MailId )
{
	CEntityPlayer *pPlayer = GetEntityPlayer();
	LK_ASSERT( pPlayer != NULL, return  -1 );
	
	// 获取邮件并从列表中移除
	CMailCache *tpMailCache = GetMailCacheByMailID( MailId  );
	if ( tpMailCache == NULL )
	{
		// 缓存中删除
		EraseMailCache( MailId  );

		return -1;
	}

	// 缓存中删除
	EraseMailCache( MailId  );

	if ( tpMailCache->mItemNum != 0  && tpMailCache->mMoney > 0 )
	{	
		// 附件不为空，不能删除
		return -1;
	}
	
	LogEventDeleteMail( GetCharID(), tpMailCache, m_nTotalMailCount );
	
	// 删除邮件对象
    for ( int i = 0 ; i < MAIL_ITEM_COUNT_MAX; i++ )
    {
		if ( tpMailCache->mItemObjID[i] != 0 )
		{
			CSceneObjManager::GetSingletonPtr( )->DestroyObject( tpMailCache->mItemObjID[i] );
		}
    }
    CSceneObjManager::GetSingletonPtr( )->DestroyObject( tpMailCache->get_id( ) );
    	
    // 从数据库中删除     
    CMailDBOperate::MailDel( MailId,GetCharID(), pPlayer->GetNationality() );	 

	SetActionMailID( MailId );
           
    --m_nCurCacheMailCount;
    --m_nTotalMailCount;   	
    if ( m_nTotalMailCount >= MAIL_MAILBOX_MAXMAILNUM )
    {	
		// 如果邮箱有空闲地方而且数据库中还有邮件(邮件总数大于邮箱中的邮件)		
		GetMailFromDB( 1, GetMaxMailID(), FETCHMAIL_NEW );
    }    
	return 1;
}

// ***********************************************************
//  Function:		GetMailCacheByMailID
//  Description:	通过邮件id获取邮件
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
CMailCache * CMail::GetMailCacheByMailID( int vMailID )
{
/*	_MAIL_CACHE_LIST_TYPE_::iterator it;
	for ( it=m_vMailCache.begin(); it != m_vMailCache.end() ; ++it  )
	{
		CMailCache *tpMailCache	 = ( CMailCache * )CSceneObjManager::GetSingletonPtr( )->GetObject( *it );	
		if ( tpMailCache == NULL )
		{
			continue;
		}
		if ( tpMailCache->mMailID == vMailID )
		{
			if ( IfRemoveFromCache == true )
			{
				m_vMailCache.erase( it );
			}
			return tpMailCache;
		}			
	}	
	return NULL;*/	

	_MAIL_CACHE_LIST_TYPE_::iterator it = m_vMailCache.find( vMailID );
	if( it != m_vMailCache.end() )
	{
		CMailCache *tpMailCache	 = ( CMailCache * )CSceneObjManager::GetSingletonPtr( )->GetObject( it->second );
		if ( tpMailCache == NULL )
		{
			LOG_ERROR( "mail", "[%s:%d] can't find mailcache obj by id %d", __FUNCTION__, __LINE__, it->second );
			return NULL;
		}

		return tpMailCache;
	}

	return NULL;
}


// ***********************************************************
//  Function:	 EraseMailCache	
//  Description: 从邮件缓存中删除邮件	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::EraseMailCache( int vMailID )
{
/*	_MAIL_CACHE_LIST_TYPE_::iterator it;
	for ( it=m_vMailCache.begin(); it != m_vMailCache.end() ; ++it  )
	{
		CMailCache *tpMailCache	 = ( CMailCache * )CSceneObjManager::GetSingletonPtr( )->GetObject( *it );	
		if ( tpMailCache == NULL )
		{
			continue;
		}
		if ( tpMailCache->mMailID == vMailID )
		{
			m_vMailCache.erase( it );
			return 1;
		}			
	}	
	return 0;	*/		

	_MAIL_CACHE_LIST_TYPE_::iterator it = m_vMailCache.find( vMailID );
	if( it != m_vMailCache.end() )
	{
		m_vMailCache.erase( it );
		return 1;
	}

	return 0;

}
//
//// ***********************************************************
////  Function:	GetMailCache	
////  Description: 获取待处理的邮件	
////  Input:		
////  OutPut:			
////  Return:			
////  Others:
////  Date:			2009-02-18		
//// **********************************************************
//CMailCache * CMail::GetMailCache( int vIndex, int vMailID )
//{
//	if ( vIndex < 0  || vIndex >= m_nCurCacheMailCount )
//	{
//		return NULL;		
//	}
//	CMailCache *tpMail = ( CMailCache * )CSceneObjManager::GetSingletonPtr()->GetObject( m_vMailCache[ vIndex ] );
//	if ( tpMail == NULL )
//	{
//		return NULL;
//	}
//	if ( tpMail->mMailID != vMailID )
//	{
//		return NULL;
//	}
//	return tpMail;  
//}


// ***********************************************************
//  Function:	ClearItemList	
//  Description: 清空附件列表	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
void CMail::ClearItemList( )
{		
	CEntityPlayer *pPlayer = GetEntityPlayer();
	if( pPlayer == NULL )
	{
		return;
	}
	CPropertyPlayer* tpProperty = ( CPropertyPlayer* )pPlayer->GetProperty( );	
	CItemObject *tpItemObj = NULL;		
	CMessage				HeadMessage;
	CMessageLockItemNotice  LockItemNotice;
	for ( int i=0; i<MAIL_ITEM_COUNT_MAX; i++ )
	{	
		if (  m_nList[i].ItemNum == 0  )
		{
			continue;
		}
				
		tpItemObj = tpProperty->GetBaggage( )->GetItemObjPtr( m_nList[i].ItemIndex );
		
		if ( tpItemObj == NULL )
		{
			m_nList[i].ItemNum   = 0;			
			continue;
		}
		
		if ( tpItemObj->GetLockStatus() == LOCKTYPE_MAIL )
		{
			tpProperty->GetBaggage( )->Lock( m_nList[i].ItemIndex ,LOCKTYPE_NONE );	
			HeadMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_LOCKITEM );
			HeadMessage.set_msgpara( (int )&LockItemNotice );
			LockItemNotice.set_index(  m_nList[i].ItemIndex );
			LockItemNotice.set_locked( LOCKTYPE_NONE );
			CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &HeadMessage ); 			
		}		
		m_nList[i].ItemNum   = 0;
	}
	return ;
}

// ***********************************************************
//  Function:	SetMaxMailID	
//  Description: 设置客户端显示的邮件ID的最大值	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-04-21
// **********************************************************
int CMail::SetMaxMailID( int vMailID )
{  
	if( vMailID > m_nLastMailID )
	{	
		m_nLastMailID = vMailID;	 
		return 1;	
	}

	return 0;
} 

// ***********************************************************
//  Function:	SetMaxMailID	
//  Description: 设置客户端显示的邮件ID的最大值	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-04-21
// **********************************************************
int CMail::GetMaxMailID(  )
{
	//if ( m_nMailIDFetched == m_nLastMailID  )
	//{
	//	++m_nMailFetcheCount;
	//	return 	m_nMailIDFetched + m_nMailFetcheCount;	
	//}
	//m_nMailIDFetched = m_nLastMailID;
	//m_nMailFetcheCount = 0;
	return m_nLastMailID;
}

// ***********************************************************
//  Function:	ClearItemList	
//  Description: 清空附件列表	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
CEntityPlayer* CMail::GetEntityPlayer()
{
	CEntityPlayer *pPlayer = ( CEntityPlayer * )CSceneLogic::GetSingleton().GetEntity( m_nEntityID );
	return pPlayer;
}

// ***********************************************************
//  Function:		CreatNewMail
//  Description:	用附件列表创建新邮件
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
CMailCache * CMail::CreatNewMail( int vMoney, int MoneyNeedPayBack, const char *pMailTitle , int nDesCharID, const char* pDesName)
{
	CMailCache *tpMailCache =(CMailCache *)CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_MAIL_MAILCACHE );
	LK_ASSERT( tpMailCache != NULL, return NULL );
	CEntityPlayer *pPlayer = GetEntityPlayer();	
	LK_ASSERT( pPlayer != NULL, return NULL );
	CPropertyPlayer *tpProperty =( CPropertyPlayer *) pPlayer->GetProperty( );	
   
	// 发件人
	strncpy( tpMailCache->mSrcName , pPlayer->GetCharName(), NAME_LENGTH-1 );
	tpMailCache->mSrcName[ NAME_LENGTH-1 ] = '\0' ; 
	
	//发件人id
	tpMailCache->mSrcId = GetCharID();
	 
	// 标题
	strncpy( tpMailCache->mMailTitle , pMailTitle , MAIL_TITLE_LEN - 1 );
	tpMailCache->mMailTitle[ MAIL_TITLE_LEN - 1 ] = '\0';	
			
	// 初始化附件数量
	tpMailCache->mItemNum = 0;
	for( int i = 0 ; i < MAIL_ITEM_COUNT_MAX ; i++ )
	{
		if( m_nList[i].ItemNum > 0 )
		{
			CItemObject *tpObject = ( CItemObject * )tpProperty->GetBaggage( )->GetItemObjPtr( m_nList[i].ItemIndex );
			if ( tpObject == NULL )
			{
				// error : 包裹栏该格子类没有物品
				LOG_ERROR( "mail","[%s,%d][%s] There is no item in index:%d of player:%d ",
					__LK_FILE__,__LINE__,__FUNCTION__, m_nList[i].ItemIndex, pPlayer->GetCharID() );
				continue;
			}
			if ( (int)tpObject->GetItemNum() < m_nList[ i ].ItemNum )
			{
				// error : 包裹栏格子里面没有这么多该物品
				LOG_ERROR( "mail","[%s,%d][%s] There is not enough item in index:%d of player:%d ",
					__LK_FILE__,__LINE__,__FUNCTION__, m_nList[i].ItemIndex, pPlayer->GetCharID() );
				 continue;
			}
			tpObject = tpProperty->GetBaggage( )->EraseItem( m_nList[i].ItemIndex, m_nList[i].ItemNum );			
			if ( tpObject == NULL )
			{
				continue;
			}

			// 记录插入附件日志
			LogEventLostItemBySendMail(pPlayer, tpObject->GetGUID(), tpObject->GetItemID(), m_nList[i].ItemNum, 
					nDesCharID, tpMailCache->get_id());


			CPropertyModule::GetSingletonPtr()->SendRemoveItemNotice( pPlayer, POS_BAGGAGE, m_nList[i].ItemIndex, m_nList[i].ItemNum );
			tpMailCache->mItemObjID[i] = tpObject->get_id();
									
			++tpMailCache->mItemNum;
		}
	}		

	//if( tpMailCache->mItemNum <= 0 && MoneyNeedPayBack > 0 )
	//{
	//	// error : 没有发送物品却需要对方支付金钱
	//	// return NULL;
	//}
	
	// 携带的金钱
	tpMailCache->mMoney            = vMoney;
	// 需要支付的金钱
	tpMailCache->mMoneyNeedPayBack = MoneyNeedPayBack;
	
	// 设置邮件种类和状态
	MAIL_SETLEVEL_USER( tpMailCache->mLevel );
	
	// 设置邮件的状态							  
	tpMailCache->mSendTime    = time( NULL );
	if ( MoneyNeedPayBack > 0 )
	{
		MAIL_SETLEVEL_MONEYNEED( tpMailCache->mLevel , 1 );		
	    tpMailCache->mExpiredTime = ( tpMailCache->mSendTime + MAIL_TIME_MONEYNEED );// /DAY_TIME; 
		tpMailCache->mMailTimer   = CWTimer( MAIL_TIME_MONEYNEED );
	}
	else
	{
		if ( tpMailCache->mItemNum > 0 || vMoney > 0 )
		{			
			MAIL_SETTYPE_ACCESSORY( tpMailCache->mType , 1 );
			tpMailCache->mExpiredTime = ( tpMailCache->mSendTime + MAIL_TIME_PACKAGE ); // /DAY_TIME;
		}
		else
		{		
			tpMailCache->mExpiredTime = ( tpMailCache->mSendTime + MAIL_TIME_COMMON );// /DAY_TIME;
		}
	} 
	
	// 设置为新邮件
	MAIL_SETTYPE_NEW( tpMailCache->mType , 1 );
	return tpMailCache;	
}

// ***********************************************************
//  Function:		SaveMail
//  Description:	把发送的邮件插入到数据库中
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
void CMail::SaveMail( CMailCache * tpMail , const char * pMailBody, int DesCharID /* = 0 */, bool IfNeedCallBack /* = false */, const char *pDesname /* = NULL */, int CountryID /* = 1 */, int nCharID /* = 0 */ )
{	
	MailItemBox  PbAccesoryBox;
	int          ItemNum = 0;
	char         AccesoryBuf[MAIL_EXTDATE_LEN];	
    if ( tpMail == NULL )
    {			
		return;
    }
	m_sMailInfo.ClearMailInfor( );
	
	// 设置邮件基本信息和正文信息	
	m_sMailInfo.mMailBase.mMailID    	= tpMail->mMailID;	
	m_sMailInfo.mMailBase.mSrc			= tpMail->mSrcName;
	m_sMailInfo.mMailBase.mTitle		= tpMail->mMailTitle;
	m_sMailInfo.mMailBase.mSendTime		= tpMail->mSendTime;
	m_sMailInfo.mMailBase.mExpiredTime  = tpMail->mExpiredTime;
	m_sMailInfo.mMailBase.mLevel        = tpMail->mLevel;
	m_sMailInfo.mMailBase.mType         = tpMail->mType ;	
	m_sMailInfo.mMailBase.mSrcId		= tpMail->mSrcId;
	m_sMailInfo.mMailBase.mItemNum		= tpMail->mItemNum;	
	m_sMailInfo.mMailBase.mDesName		= pDesname;
	
	// 获取邮件正文
	m_sMailInfo.mBody					= pMailBody;
	
	// 获取附件信息
	for ( int i=0 ; i< MAIL_ITEM_COUNT_MAX ; i++ )
	{
		if (  tpMail->mItemObjID[i] == 0 )
		{
			continue;
		}
		CItemObject *tpObj = ( CItemObject * )CSceneObjManager::GetSingletonPtr( )->GetObject( tpMail->mItemObjID[i] );
		if ( tpObj == NULL )
		{
		  // error		  
		   continue;
		}		

	    PBItemObject *tPbitem = PbAccesoryBox.add_mailitem( );
		tpObj->GetItemBinaryBuffer2( tPbitem , tpObj->GetItemNum( ) );
		PbAccesoryBox.add_itemindex( i );		
		// 删除物品信息( 等收到数据库返回消息后再删除 )
		//CSceneObjManager::GetSingletonPtr()->DestroyObject( tpMail->mItemObjID[i] );	
		//tpMail->mItemObjID[i] = 0;
		ItemNum++;
	}	
	
	// 获取金钱信息
	m_sMailInfo.mMoneySend         = tpMail->mMoney;
	m_sMailInfo.mMoneyNeedBack     = tpMail->mMoneyNeedPayBack;

	// 序列化附件信息	
	bool Ret = PbAccesoryBox.SerializeToArray( AccesoryBuf, MAIL_EXTDATE_LEN );
	if( Ret ==  false )
	{
		// 序列化失败
		LOG_ERROR( "mail","[%s,%d][%s] Accessory SerializeToArray Error!  ",__LK_FILE__,__LINE__,__FUNCTION__ );
		return;
	}	
	int Byesize = PbAccesoryBox.ByteSize();
	
	// 过滤附件序列化buff中的转义字符 
	char *DesBuf = m_sMailInfo.mExtDate;
	int CapRet = EscapeDataString( &DesBuf, MAIL_EXTDATE_LEN , AccesoryBuf , Byesize );
    if ( CapRet ==-1 )
    { 		
		LOG_ERROR( "mail","[%s,%d][%s] EscapeDataString Error!  ",__LK_FILE__,__LINE__,__FUNCTION__ );
		return;
    }
        
	// 把邮件存入数据库中
  	CMailDBOperate::MailSend( DesCharID , tpMail->get_id( ), &m_sMailInfo, IfNeedCallBack, CountryID, nCharID );
 }

// ***********************************************************
//  Function:		InsertMail
//  Description:	往邮件缓存中插入一封新邮件，InsertPos = 0 表示插入队列头，否则插入队列尾部
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int  CMail::InsertMail( CMailCache *tpMailCache, int IfNewMail )
{	 	
	if ( tpMailCache == NULL )
	{		
		return -1;
	}
	
	if ( IfNewMail == 1 )
	{
		// 邮件总数加1
		++m_nTotalMailCount ;	
	}
	
	// 客户端能够显示的邮件缓存已满，不能插入
	if ( m_nCurCacheMailCount >= MAIL_MAILBOX_MAXMAILNUM )
	{			
		return -1;
	}	
	if ( m_nTotalMailCount > MAIL_MAILBOX_MAXMAILNUM && IfNewMail == 1 )
	{
		return -1;
	}
	
	// 现在不发送邮件信息，等到打开邮箱时再发送
	if ( m_nLeftPageMailNum != -1  )
	{
		++m_nLeftPageMailNum;
		return 0;
	}
	   	
	// 把获取的邮件放到邮件列表的头部	
	_MAIL_CACHE_LIST_TYPE_::_Pairib tInsertRet = m_vMailCache.insert( std::pair<int,int>(tpMailCache->mMailID, tpMailCache->get_id()) );
	if( tInsertRet.second == true )
	{
		++m_nCurCacheMailCount;	

		SetMaxMailID( tpMailCache->mMailID );

		return 1;
	}
	else
	{
		LOG_ERROR( "mail", "[%s:%d] insert mail %d failed, cache mailnum:%d, total mailnum:%d ", __FUNCTION__, __LINE__,
			 tpMailCache->mMailID, m_nCurCacheMailCount, m_nTotalMailCount );
		if ( IfNewMail == 1 )
		{
			// 邮件总数减1
			--m_nTotalMailCount ;	
		}
		return -1;
	}	
}

// ***********************************************************
//  Function:	 ReceiverRouterMail	
//  Description: 接收来自其他服务器的邮件	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 2009-03-20		
// **********************************************************
int CMail::ReceiveRouterMail( CMessageNewMailNotify *tpMsg )
{	
	LK_ASSERT( tpMsg != NULL, return -1 );	
	if ( !tpMsg->has_maillist() )
	{						  
		return -1;			
	}
	
	// 如果客户端邮箱已经满了则不发送新邮件通知
	if( m_nCurCacheMailCount >= MAIL_MAILBOX_MAXMAILNUM )
	{
		tpMsg->set_haslist( 0 );
		++m_nTotalMailCount;
		return -1;
	}
	if ( m_nLeftPageMailNum != -1 )
	{
		++m_nLeftPageMailNum;
		tpMsg->set_haslist( 0 );
		return 1;
	}
	CMailCache *tpMailCache = ( CMailCache * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_MAIL_MAILCACHE );
	LK_ASSERT( tpMailCache != NULL, return  -1 );
	tpMailCache->mSrcId		  = tpMsg->router().srccharid();
	tpMailCache->mMailID	  =	tpMsg->maillist().mailid();	
	tpMailCache->mExpiredTime = tpMsg->maillist().expiredtime()*DAY_TIME;
	tpMailCache->mSendTime    = tpMsg->maillist().sendtime();
	tpMailCache->mType		  = tpMsg->maillist().mailtype();
	tpMailCache->mLevel		  = tpMsg->maillist().maillevel();
	tpMailCache->mMoney		  = tpMsg->maillist().money();
	tpMailCache->mMoneyNeedPayBack	= tpMsg->maillist().moneyneedback();
	if ( tpMsg->maillist().has_srcname() )
	{
		strncpy( tpMailCache->mSrcName, tpMsg->maillist().srcname().c_str(), NAME_LENGTH-1 );
		tpMailCache->mSrcName[NAME_LENGTH-1] = '\0';
	}
	if ( tpMsg->maillist().has_title() )
	{
		strncpy( tpMailCache->mMailTitle, tpMsg->maillist().title().c_str(), MAIL_TITLE_LEN -1 );
		tpMailCache->mMailTitle[MAIL_TITLE_LEN-1] = '\0';
	}
	tpMsg->set_haslist( 1 );

	int nInsertRet = InsertMail( tpMailCache, 1 );
	if( nInsertRet != 1 )
	{
		CSceneObjManager::GetSingletonPtr()->DestroyObject( tpMailCache->get_id() );
		return -1;
	}

	return 1;

}

// ***********************************************************
//  Function:	 UpdateMail	
//  Description: 更新邮件信息到数据库中	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::UpdateMail(  CMailCache *tpMailCache )
{
	if (  tpMailCache == NULL )
	{
		return -1;
	}
	if ( MAIL_IS_NEEDSAVE( tpMailCache->mType ) )
	{
		MAIL_SETTYPE_NEEDSAVE( tpMailCache->mType, 0 )
		if ( MAIL_IS_ACCECHAG( tpMailCache->mType ) )
		{
			MAIL_SETTYPE_ACCECHAGED( tpMailCache->mType, 0 )
			UpdateAccesory( tpMailCache );
		}
		else
		{
			UpdateMailStatus( tpMailCache );	
		}				
	}	
	return 1;
}


// ***********************************************************
//  Function:	 UpdateAccesory	
//  Description: 更新附件信息到数据库中	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::UpdateAccesory( CMailCache *tpMail )
{					
	CEntityPlayer *pPlayer = GetEntityPlayer() ;
	LK_ASSERT( tpMail != NULL, return -1 );
	MailItemBox  PbAccesoryBox;
	int          ItemNum = 0;
	char         AccesoryBuf[MAIL_EXTDATE_LEN];		
	m_sMailInfo.ClearMailInfor();
	
	// 获取附件信息
	for ( int i=0 ; i< MAIL_ITEM_COUNT_MAX ; i++ )
	{
		if ( tpMail->mItemObjID[i] == 0 )
		{
			continue;
		}
		CItemObject *tpObj = ( CItemObject * )CSceneObjManager::GetSingletonPtr()->GetObject( tpMail->mItemObjID[i] );
		if ( tpObj == NULL )
		{
			// error		  
			continue;
		}		
		PbAccesoryBox.add_mailitem();
		PBItemObject *tPbitem = PbAccesoryBox.mutable_mailitem( ItemNum );
		PbAccesoryBox.add_itemindex( i );
		tpObj->GetItemBinaryBuffer2( tPbitem , tpObj->GetItemNum() ); 		
		ItemNum++;
	}	
	
	// 邮件状态
	m_sMailInfo.mMailBase.mLevel		= tpMail->mLevel;
	m_sMailInfo.mMailBase.mExpiredTime	= tpMail->mExpiredTime;
	m_sMailInfo.mMailBase.mType			= tpMail->mType;
	m_sMailInfo.mMoneyNeedBack			= tpMail->mMoneyNeedPayBack;
	m_sMailInfo.mMoneySend				= tpMail->mMoney;
	m_sMailInfo.mMailBase.mItemNum      = ItemNum;
	
	// 序列化附件信息	
	bool Ret = PbAccesoryBox.SerializeToArray( AccesoryBuf, MAIL_EXTDATE_LEN );
	if( Ret ==  false )
	{
		// 序列化失败
		//return;
		LOG_ERROR( "mail","[%s,%d][%s] SerializeToArray Error!",__LK_FILE__,__LINE__,__FUNCTION__ );
		return -1;
	}
	int Byesize = PbAccesoryBox.ByteSize( );

	// 过滤附件序列化buff中的转义字符 
	char *DesBuf = m_sMailInfo.mExtDate;
	EscapeDataString( &DesBuf, MAIL_EXTDATE_LEN , AccesoryBuf , Byesize );

	// 把邮件更新到数据库中			
	CMailDBOperate::UpMailAccessory( tpMail->mMailID, &m_sMailInfo,GetCharID(), pPlayer->GetNationality() );
	return 0;
}


// ***********************************************************
//  Function:		UpdateMailStatus	
//  Description:	更新邮件的金钱信息和状态信息到数据库中
//  Input:			CMailCache *tpMail		待更新的邮件
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::UpdateMailStatus( CMailCache *tpMail )
{
	CEntityPlayer *pPlayer = GetEntityPlayer();
	if ( tpMail == NULL )
	{
		return -1;
	}	

	// 邮件状态
	m_sMailInfo.mMailBase.mLevel		= tpMail->mLevel;
	m_sMailInfo.mMailBase.mExpiredTime	= tpMail->mExpiredTime;
	m_sMailInfo.mMailBase.mType			= tpMail->mType;
	m_sMailInfo.mMoneyNeedBack			= tpMail->mMoneyNeedPayBack;
	m_sMailInfo.mMoneySend				= tpMail->mMoney;
	CMailDBOperate::UpMailStatus( tpMail->mMailID, &m_sMailInfo, GetCharID(), pPlayer->GetNationality() );
	return 0;
}

// ***********************************************************
//  Function:		BackMail				
//  Description:	退信
//  Input:			int vMailID				 邮件ID
//  Input:			const char *pMailBody	 邮件正文
//  OutPut:			
//  Return:			1 表示退信成功 否则表示退信失败
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::BackMail(  int vMailID, const char *pMailBody )
{ 
	CEntityPlayer *pPlayer = GetEntityPlayer();
	if ( pPlayer == NULL )
	{
		return -1;
	}	

	// 获取邮件
	CMailCache *tpMailCache = GetMailCacheByMailID( vMailID );
	if ( tpMailCache == NULL )
	{	
		// error
		return -1;
	}  
		
	// 纯文本不能退信
	if ( tpMailCache->mItemNum == 0  && tpMailCache->mMoney <= 0 )
	{
		// 纯文本不能退信
		return ERR_MAIL_BACKMAIL_TEXT;
	}
		
	// 被退回的邮件不能再次退信
	if ( MAIL_IS_BACKMAIL( tpMailCache->mLevel ) )
	{
		return ERR_MAIL_BACKMAIL_BACKED;
	}
	
	// 系统邮件不能退信
	if ( MAIL_IS_SYSTEM( tpMailCache->mLevel ) )
	{
		return -1;
	}
	
	// 邮资不够不能退信
	CPropertyPlayer *tpProperty = ( CPropertyPlayer  *)pPlayer->GetProperty();
	if ( MAIL_SEND_MONEY_PRO > tpProperty->GetBindMoney() + tpProperty->GetMoney() )
	{
		return  ERR_MAIL_SENDMAIL_MONEYNOTENOUGH;  
	}
	int tBindMoney = MAIL_SEND_MONEY_PRO > tpProperty->GetBindMoney()? tpProperty->GetBindMoney():MAIL_SEND_MONEY_PRO;
	int tMoney	   = MAIL_SEND_MONEY_PRO - tBindMoney;
		
	// 从邮件列表中删除
	EraseMailCache( vMailID );
			   	
	// 删除数据库中的信件
	CMailDBOperate::MailDel( vMailID, GetCharID(), pPlayer->GetNationality() );
	
	// 清空原来的状态
	tpMailCache->mType  = 0;
	tpMailCache->mLevel = 0;
	
	// 设置邮件类型为用户退信
	MAIL_SETLEVEL_BACKMAIL( tpMailCache->mLevel, 1 );
	MAIL_SETLEVEL_USER( tpMailCache->mLevel );	
	MAIL_SETTYPE_NEW( tpMailCache->mType, 1 )
	
	
	tpMailCache->mMoneyNeedPayBack = 0;
	MAIL_SETLEVEL_MONEYNEED( tpMailCache->mLevel, false )
	
	// 设置退信的发送方	 	
	int DestCharID = tpMailCache->mSrcId;
	tpMailCache->mSrcId = pPlayer->GetCharID();
	const char *pSrcName = pPlayer->GetCharName( ); 	
	char DesName[ NAME_LENGTH ];
	
	// 保存邮件接收方名字
	memset( DesName, 0, sizeof( DesName ) );
	strncpy( DesName, tpMailCache->mSrcName, NAME_LENGTH -1 );
	
	// 重置邮件发送方名字
	memset( tpMailCache->mSrcName, 0 , sizeof( tpMailCache->mSrcName ) );	
	strncpy( tpMailCache->mSrcName, pSrcName, sizeof( tpMailCache->mSrcName ) - 1 );
	
	// 设置退信时间和邮件种类
	tpMailCache->mSendTime		= time( NULL );
	tpMailCache->mExpiredTime	= ( tpMailCache->mSendTime + MAIL_TIME_PACKAGE );// /DAY_TIME;	   
	
	//  如果对方在线的话则发给对方	
	/*bool IfNeedCallBack = SendMailToPlayer( DestCharID, tpMailCache );		*/
		
	// 保存邮件到数据库
	SaveMail( tpMailCache, pMailBody, DestCharID, true, DesName, pPlayer->GetNationality() );
	
	//// 如果邮箱满，删除邮件
	//if (  IfNeedCallBack == false )
	//{
	//	CSceneObjManager::GetSingletonPtr()->DestroyObject( tpMailCache->get_id() );
	//}
	//
	// 扣除基本邮资
	if (CPropertyModule::GetSingletonPtr( )->PlayerChangeMoney( pPlayer, tBindMoney, true, true ) == SUCCESS)
	{
		LogEventLostMoneyByPostalFee(pPlayer, 
				CYuanBao::em_bind_money, tBindMoney, DestCharID, tpMailCache->get_id() );
	}

	if(CPropertyModule::GetSingletonPtr( )->PlayerChangeMoney( pPlayer, tMoney, true, false ) == SUCCESS)
	{
		LogEventLostMoneyBySendMail(pPlayer, 
				CYuanBao::em_unbind_money, tMoney, DestCharID, tpMailCache->get_id() );
	}
	
	// 改变自己的邮件数量
	--m_nCurCacheMailCount;
	--m_nTotalMailCount;
	
	// 如果还有新邮件在数据库中的话则将其取出
	if ( m_nTotalMailCount >= MAIL_MAILBOX_MAXMAILNUM  )
	{	
		// 如果邮箱有空闲地方而且数据库中还有邮件(邮件总数大于邮箱中的邮件)		
		GetMailFromDB( 1, GetMaxMailID(), FETCHMAIL_NEW );
	}    
	return 1;
}

// ***********************************************************
//  Function:	 GetMAilListInfo	
//  Description: 获取邮件列表	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 2009-02-18		
// **********************************************************
int CMail::GetMAilListInfo( CMessageMailListNotify &pMessage )
{	
	_MAIL_CACHE_LIST_TYPE_::iterator it;
	CMailCache *tpMailCache = NULL ;
	CMailInfo *MailInfo = NULL;
	int MailNum = 0;
	for ( it = m_vMailCache.begin( ); it != m_vMailCache.end( ); it++ )
	{
		tpMailCache = ( CMailCache * )CSceneObjManager::GetSingletonPtr( )->GetObject( it->second );
		if ( tpMailCache == NULL )
		{	
			LOG_ERROR( "mail","[%s,%d][%s] Can't get CMailCache Object",__LK_FILE__,__LINE__,__FUNCTION__ );
			return -1;
		}
		if ( tpMailCache->mMailID == 0 )
		{
			LOG_ERROR( "mail","[%s,%d][%s] CMailCache Obj %d dosn't have mailid",
				__LK_FILE__,__LINE__,__FUNCTION__ , tpMailCache->get_id() );
			continue;
		}
		pMessage.add_maillist( );
		MailInfo = pMessage.mutable_maillist( MailNum );	
		MailInfo->set_title( tpMailCache->mMailTitle );
		MailInfo->set_sendtime( tpMailCache->mSendTime );
		MailInfo->set_maillevel( tpMailCache->mLevel );
		MailInfo->set_srcname( tpMailCache->mSrcName );
		MailInfo->set_money( tpMailCache->mMoney );
		MailInfo->set_itemnum( tpMailCache->mItemNum );
		MailInfo->set_mailid( tpMailCache->mMailID );
		MailInfo->set_mailtype( tpMailCache->mType );
		MailInfo->set_expiredtime( tpMailCache->mExpiredTime/DAY_TIME );
		MailInfo->set_moneyneedback( tpMailCache->mMoneyNeedPayBack );
		MailNum ++;
	}
	return 1;	
}

// ***********************************************************
//  Function:		DestroyMailCache
//  Description:	销毁邮件缓存
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
void CMail::DestroyMailCache( )
{
    _MAIL_CACHE_LIST_TYPE_ ::iterator it;
    CMailCache *tpCache = NULL;
	for ( it = m_vMailCache.begin() ; it != m_vMailCache.end(); )
	{
		tpCache = ( CMailCache * )CSceneObjManager::GetSingletonPtr( )->GetObject( it->second );
		if ( tpCache == NULL )
		{
			// error
			LOG_ERROR( "mail","[%s,%d][%s] Empty MailCache! ",__LK_FILE__,__LINE__,__FUNCTION__  );
			m_vMailCache.erase( it++ );
			continue;
		}

		UpdateMail( tpCache );

		for ( int i = 0; i < MAIL_ITEM_COUNT_MAX ; i++ )
		{
			if ( tpCache->mItemObjID[ i ] != 0 )
			{
				CSceneObjManager::GetSingletonPtr( )->DestroyObject( tpCache->mItemObjID[ i ] );
			}	
		}

		CSceneObjManager::GetSingletonPtr( )->DestroyObject( tpCache->get_id() );	

		m_vMailCache.erase( it++ );
	}
}

// ***********************************************************
//  Function:	 DestroyMailAccessory	
//  Description: 销毁邮件附件物品	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::DestroyMailAccessory( CMailCache *tpMailCache )
{
	if ( tpMailCache == NULL )
	{
		return -1;
	}
	for ( int i  = 0; i < MAIL_ITEM_COUNT_MAX; ++i  )
	{
		if ( tpMailCache->mItemObjID[i] == 0 )
		{	
			continue;
		}
		CSceneObjManager::GetSingletonPtr()->DestroyObject( tpMailCache->mItemObjID[i] );	
	}
	return 0;
}

// ***********************************************************
//  Function:		SetActionMailID
//  Description:	获取邮件的详细信息
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
void CMail::SetActionMailID( int MailID )
{
/*	if ( m_nActionMailID <= 0 )
	{
		m_nActionMailID = MailID;
		return;
	}
	if ( m_nActionMailID != MailID )
	{		
		UpdateMailInfo( m_nActionMailID );
		m_nActionMailID = MailID;
	}	*/	

	m_nActionMailID = MailID;

}

// ***********************************************************
//  Function:		SendMsgMailBoxStatus
//  Description:	发送玩家信箱的状态(登录之后就会发送)
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-04-15		
// **********************************************************
void CMail::SendMsgMailBoxStatus( int MailCount, int NewMailCount )
{
	CEntityPlayer *pPlayer = GetEntityPlayer( );
	LK_ASSERT( pPlayer != NULL, return );
	CMessage MsgHead;
	CMessageMailBoxStatusNotify MsgMailBoxStatus;	
	MsgMailBoxStatus.set_newmailcount( NewMailCount );
	MsgMailBoxStatus.set_mailcount( MailCount>MAIL_MAILBOX_MAXMAILNUM?MAIL_MAILBOX_MAXMAILNUM:MailCount );
	MsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_MAILBOXSTATUS );
	MsgHead.set_msgpara( (int)&MsgMailBoxStatus );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &MsgHead );
}

// ***********************************************************
//  Function:		GetMailDetailInfo
//  Description:	获取邮件的详细信息
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::GetMailDetailInfo( int vMailID, CMessageMailReadResponse &MsgMailRead )
{
	CMailCache *tpMailCache = GetMailCacheByMailID( vMailID );
	LK_ASSERT( tpMailCache != NULL, return -1 );
	MsgMailRead.set_mailid( vMailID );
	
	// 状态设置为已读
	if ( MAIL_IS_NEW( tpMailCache->mType ) )
	{
		MAIL_SETTYPE_NEW( tpMailCache->mType, 0 )
		MAIL_SETTYPE_NEEDSAVE( tpMailCache->mType, 1 )
	}	
	CItemObject *tpItemObj = NULL;
	int Index = 0;
	for ( int i = 0; i < tpMailCache->mItemNum; i++ )
	{
		if ( tpMailCache->mItemObjID[ i ] == 0 )
		{
			continue;
		}
		tpItemObj = ( CItemObject * )CSceneObjManager::GetSingletonPtr( )->GetObject( tpMailCache->mItemObjID[ i ] );
		if ( tpItemObj == NULL )
		{
			continue;
		}
		MsgMailRead.add_accessory( );
		tpItemObj->GetItemBinaryBuffer2( MsgMailRead.mutable_accessory( Index ) );				
		++Index;
	}
	
	// 改变一下被操作的邮件ID
	// ChangeActionMailID( vMailID );
	UpdateMail( tpMailCache );
	return 1;
}

// ***********************************************************
//  Function:		UpdateMailInfo
//  Description:	更新邮件信息到数据库
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::UpdateMailInfo( int MailID )
{
	CMailCache *tpMailCache = GetMailCacheByMailID( MailID );
	if ( tpMailCache == NULL )
	{
		return -1;
	}
	return UpdateMail( tpMailCache );
}

// ***********************************************************
//  Function:	 GetMailPage	
//  Description: 获取一页邮件信息	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::GetMailPage(  )
{
	if ( m_nMailBoxStatus == MAIL_STATUS_INITIALIZE )
	{
		// 发送错误提示
		return -1;
	}
	
	// 邮件已经取完了
	if ( m_nLeftPageMailNum == -1  )
	{
		// 发送错误提示
		return -1;
	}
	if ( m_nLeftPageMailNum > MAIL_MAX_PAGENUM )
	{
		GetMailFromDB( MAIL_MAX_PAGENUM, m_nLastPageMailID, FETCHMAIL_PAGE );
		m_nLeftPageMailNum -= MAIL_MAX_PAGENUM;
		return 1;
	}	
	GetMailFromDB( m_nLeftPageMailNum, m_nLastPageMailID, FETCHMAIL_PAGE );
	m_nLeftPageMailNum = -1;	
	return 1;
}

// ***********************************************************
//  Function:	 LockMail	
//  Description: 给邮件加锁	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::LockMail( int MailId )
{
	CMailCache *tpMailCache = GetMailCacheByMailID( MailId );
	if ( tpMailCache == NULL )
	{
		return -1;
	}
	/*
	TODO: 判断是否是已经付费的付费邮件
	*/
	if ( m_nLockMailNum >= MAIL_LOCKNUM_MAX )
	{
		return -2;
	}

	MAIL_SETTYPE_LOCK( tpMailCache->mType, 1  )
	MAIL_SETTYPE_NEEDSAVE( tpMailCache->mType, 1 )
	MAIL_SETTYPE_ACCECHAGED( tpMailCache->mType, 0 )
	++m_nLockMailNum;
	return 1;
}

// ***********************************************************
//  Function:	 UnLockMail	
//  Description: 给邮件解锁	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
int CMail::UnLockMail( int MailId )
{
	CMailCache *tpMailCache = GetMailCacheByMailID( MailId );
	if ( tpMailCache == NULL )
	{
		// error
		return -1;		
	}
	if ( !MAIL_IS_LOCK( tpMailCache->mType ) )
	{	
		// error
		return -1;			
	}
	MAIL_SETTYPE_LOCK( tpMailCache->mType, 0 )
	MAIL_SETTYPE_NEEDSAVE( tpMailCache->mType, 1 )
	MAIL_SETTYPE_ACCECHAGED( tpMailCache->mType, 0 )
	--m_nLockMailNum;
	return 1;
}

// ***********************************************************
//  Function:		GetMailCountCallBack
//  Description:	从数据库读取邮件个数的回调函数
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-02-18		
// **********************************************************
void CMail::GetMailCountCallBack( int CharID, int MailCount )
{
	CEntityPlayer *pPlayer = ( CEntityPlayer * )CSceneLogic::GetSingletonPtr( )->GetPlayerByCharID( CharID );
	if ( pPlayer == NULL )
	{
		// 刚上线又下线了
		return;
	}
	CMail *tpMail = CMailManager::GetSingletonPtr()->GetMail( pPlayer );
	if ( tpMail == NULL )
	{
		// 错误
		LOG_ERROR( "mail","[%s,%d][%s] Get CMail Error! ", __LK_FILE__, __LINE__, __FUNCTION__ );
		return;
	}
	if ( MailCount < 0 )
	{
		MailCount = 0;		
	}
	
	// 邮件总数
	tpMail->m_nTotalMailCount  =  MailCount;  	
	
	// 还未从数据库取出的邮件数量
	tpMail->m_nLeftPageMailNum = std::min( MailCount, MAIL_MAX_PAGENUM );
	
	// 锁定的邮件数量初始化为0
	tpMail->m_nLockMailNum	   = 0;	 
	
	// 如果已经获取新邮件数量	
	if ( tpMail->m_nMailBoxStatus & MAIL_STATUS_CHECKNEWMAIL )
	{
		int NewMailCount = tpMail->m_nCurCacheMailCount; 
		if ( MailCount > MAIL_MAILBOX_MAXMAILNUM )
		{
			NewMailCount -= MailCount - MAIL_MAILBOX_MAXMAILNUM;
		}						
		tpMail->SendMsgMailBoxStatus( MailCount, NewMailCount );
		
		// 邮件初始化完毕
		tpMail->m_nMailBoxStatus = MAIL_STATUS_SHUTDOWN;
		tpMail->m_nCurCacheMailCount =  0;	 				
		return;
	} 	
	tpMail->m_nMailBoxStatus |= MAIL_STATUS_GETCOUNT;  	
}

// ***********************************************************
//  Function:		CheckNewMailCallBack
//  Description:	从数据库读取新邮件个数的回调函数
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:			2009-04-14		
// **********************************************************
void CMail::CheckNewMailCallBack( int CharID, int NewMailCount )
{
	CEntityPlayer *pPlayer = ( CEntityPlayer * )CSceneLogic::GetSingletonPtr( )->GetPlayerByCharID( CharID );
	if ( pPlayer == NULL )
	{
		// 刚上线又下线了
		return;
	}
	CMail *tpMail = CMailManager::GetSingletonPtr()->GetMail( pPlayer );
	if ( tpMail == NULL )
	{
		// 错误
		LOG_ERROR( "mail","[%s,%d][%s] Get CMail Error! ", __LK_FILE__, __LINE__, __FUNCTION__ );
		return;
	}
	if ( NewMailCount < 0 )
	{
		NewMailCount = 0;		
	}  
	
	// 邮件数量已经获取完毕
	if ( tpMail->m_nMailBoxStatus & MAIL_STATUS_GETCOUNT )
	{	   				
		if ( tpMail->m_nTotalMailCount > MAIL_MAILBOX_MAXMAILNUM )
		{
			NewMailCount -= tpMail->m_nTotalMailCount - MAIL_MAILBOX_MAXMAILNUM;
		}
		tpMail->SendMsgMailBoxStatus( tpMail->m_nTotalMailCount, NewMailCount );
		tpMail->m_nMailBoxStatus = MAIL_STATUS_SHUTDOWN;
		return;		
	} 
	
	// 设置邮箱状态
	tpMail->m_nCurCacheMailCount = NewMailCount;
	tpMail->m_nMailBoxStatus |= MAIL_STATUS_CHECKNEWMAIL;	
}		 

// ***********************************************************
//  Function:		GetMailDetailFromDB
//  Description:	从数据库读取邮件的附件信息和正文信息	
//  Input:			
//  OutPut:			
//  Return:			
//  Others:			
//  Date:			2009-02-18			
// **********************************************************
int CMail::GetMailDetailFromDB( int MailID )
{
	CEntityPlayer *pPlayer = GetEntityPlayer();
	LK_ASSERT( pPlayer != NULL, return -1 );
	CMailCache *tpMailCache = GetMailCacheByMailID( MailID );
	if ( tpMailCache == NULL )
	{
		return -1;
	}
	if( MAIL_IS_HASSENDTOCLIENT( tpMailCache->mType ) ) 
	{
		return -2;
	}
	CMailDBOperate::GetMailDetailInfo( MailID, GetCharID(), pPlayer->GetNationality() );	
	return 1;
}

// ***********************************************************
//  Function:		PayMoneyForMail
//  Description:	为付费邮件支付金钱	
//  Input:			
//  OutPut:			
//  Return:			
//  Others:			
//  Date:			2009-02-18			
// **********************************************************
int CMail::PayMoneyForMail( int nMailID, int &rMoney, int &rTime )
{  
	CMailCache *tpCache = GetMailCacheByMailID( nMailID );		
	if ( tpCache == NULL )
	{
		return -1;
	}
	
	if ( tpCache->mMoneyNeedPayBack <= 0 && !MAIL_IS_MONEYNEED( tpCache->mLevel ) )
	{
		return ERR_MAIL_PAYMAIMONEY_NONEED;
	}
	
	if ( rMoney < tpCache->mMoneyNeedPayBack )
	{
		return ERR_MAIL_PAYMAIMONEY_MONEYNOTENOUGH;
	}
	
	// 金钱
	rMoney = tpCache->mMoneyNeedPayBack;
		
	// 重新设置状态
	MAIL_SETLEVEL_BACKMAIL( tpCache->mLevel, false )
	MAIL_SETLEVEL_MONEYNEED( tpCache->mLevel, false ) 
	tpCache->mExpiredTime = ( time(NULL) + MAIL_TIME_PACKAGE );// /DAY_TIME;
	rTime = tpCache->mExpiredTime/DAY_TIME;
	
	// 邮件已经改变需要重新存盘
	MAIL_SETTYPE_NEEDSAVE( tpCache->mType, 1 )
	tpCache->mMoneyNeedPayBack = 0;
	// ChangeActionMailID( nMailID );
	UpdateMail( tpCache );
	
	// 发送邮件
	CMailCache *tpReplyCache = ( CMailCache * )CSceneObjManager::GetSingleton().CreateObject( OBJTYPE_MAIL_MAILCACHE ); 
	if ( tpReplyCache == NULL )
	{
		return -1;
	}
	
	CEntityPlayer *pPlayer = GetEntityPlayer();
	LK_ASSERT( pPlayer != NULL, return -1 );
	tpReplyCache->mMoney = rMoney;
	
	// 保存邮件接收方名字	
	strncpy( tpReplyCache->mSrcName, pPlayer->GetCharName(), NAME_LENGTH );
	tpReplyCache->mSrcName[NAME_LENGTH-1] = '\0';	
	tpReplyCache->mSrcId = pPlayer->GetCharID();

	// 设置退信时间和邮件种类
	tpReplyCache->mSendTime		= time( NULL );
	tpReplyCache->mExpiredTime	= ( tpReplyCache->mSendTime + MAIL_TIME_PACKAGE );// /DAY_TIME;	   
	
	// 设置邮件主题
	strncpy(  tpReplyCache->mMailTitle, tpCache->mMailTitle, sizeof(tpCache->mMailTitle) - 1 );
	tpReplyCache->mMailTitle[sizeof(tpCache->mMailTitle) - 1]='\0';
	
	// 设置邮件为新邮件
	MAIL_SETTYPE_NEW( tpReplyCache->mType, true )

	//  如果对方在线的话则发给对方	
	//bool IfNeedCallBack = SendMailToPlayer( tpCache->mSrcId, tpReplyCache );
			
	m_sMailInfo.mMailBase.mTitle		= tpCache->mMailTitle;
	const char *pBody = "";
	
	// 保存邮件到数据库
	SaveMail( tpReplyCache, pBody, tpCache->mSrcId, true, tpCache->mSrcName, pPlayer->GetNationality() );

	//// 如果邮箱满，删除邮件
	//if (  IfNeedCallBack == false )
	//{
	//	CSceneObjManager::GetSingletonPtr()->DestroyObject( tpReplyCache->get_id() );
	//}  		
	LogEventLostMoneyBySendMail( pPlayer, CYuanBao::em_unbind_money, rMoney, tpCache->mSrcId, nMailID );
	
	return SUCCESS;
}	

bool CMail::IsHavePayMail()
{
	_MAIL_CACHE_LIST_TYPE_::iterator it;
	CMailCache *tpMailCache = NULL ;
	for ( it = m_vMailCache.begin( ); it != m_vMailCache.end( ); it++ )
	{
		tpMailCache = ( CMailCache * )CSceneObjManager::GetSingletonPtr( )->GetObject( it->second );
		if(tpMailCache->mMoneyNeedPayBack > 0)
			return true;
	}
	return false;

}
