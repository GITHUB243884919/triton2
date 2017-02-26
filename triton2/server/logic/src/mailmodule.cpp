#include "stdafx.hpp"
#include "message_pb.hxx.pb.h"
#include "mailmessage_pb.hxx.pb.h"
#include "mail.h"
#include "mailmanager.h"
#include "mailmodule.h"
#include "chatmodule.h"
#include "servermessage_pb.hxx.pb.h"
#include "scenelogic.h"
#include "propertymodule.h"
#include "marriagemodule.h"
#include "friendmodule.h"
#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif

template<> CMailModule* CSingleton< CMailModule >::spSingleton = NULL;
char CMailModule::sModuleName[ NAME_LENGTH ] = "Mail_1";

CMailManager* gpMailManager = NULL;

// ***********************************************************
//  Function:		CountSize
//  Description:	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
unsigned int CMailModule::CountSize(  )
{
	unsigned int tSize = sizeof( CMailModule );	
	tSize += CMailManager::CountSize( );
	return tSize;
}


// ***********************************************************
//  Function:		OnLaunchServer
//  Description:	启动服务器
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnLaunchServer( )
{
	CMailManager::mShmPtr = mShmPtr;
	gpMailManager = new CMailManager;
	if( gpMailManager == NULL )
	{
		LOG_ERROR( "default", "[%s:%d] Create MailManager Failed", __FUNCTION__, __LINE__ );
		printf( "Create MailManager Failed, So Exit\n" );
		exit( 1 );
	}

	MAIL_LOG* tpCfg = CSceneCfgManager::GetSingletonPtr( )->GetMail_log( );
	if( tpCfg != NULL )
	{
		INIT_ROLLINGFILE_LOG( tpCfg->mIdentifier, tpCfg->mDirectory, (LogLevel)tpCfg->mPriority, tpCfg->mMaxfilesize, tpCfg->mMaxbackupindex);
	}
	OnLoadConfig();
}

// ***********************************************************
//  Function:		OnExitServer	
//  Description:	退出服务器
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnExitServer( )
{
	if( gpMailManager != NULL )
		delete gpMailManager;	
}

// ***********************************************************
//  Function:		OnTimer
//  Description:	服务器Timer
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	
}

// ***********************************************************
//  Function:		OnCreateMap
//  Description:	建立场景
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
int CMailModule::OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName )
{
	return SUCCESS;
}

// ***********************************************************
//  Function:		OnRouterMessage
//  Description:	路由消息
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnRouterMessage( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	switch( pMessage->msghead().messageid() )
	{
		case ID_S2C_NOTIFY_NEWMAIL:
		{
			CMessageNewMailNotify *tpMsg = ( CMessageNewMailNotify * )pMessage->msgpara();
			LK_ASSERT( tpMsg != NULL, return )
			if ( tpMsg->router().sendback() == 0 &&  tpMsg->router( ).routerreason( ) == CMessageRouter::REASON_SUCCESS )
			{
				ReceiveRouterMail( pMessage );
				return;		
			}			
		}
		break;
		default:
		break;
	}
}

// ***********************************************************
//  Function:		OnClientMessage
//  Description:	客户端发送消息	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	int MsgId =  pMessage->mutable_msghead( )->messageid( );
	switch( MsgId )
	{
		case ID_C2S_REQUEST_OPENMAILBOX:
			{
			  // 打开邮箱
			  OnmessageOpenMailBoxReq( pPlayer, pMessage );
			  break;
			}
		case ID_C2S_REQUEST_OPENSENDMAILBOX:
			{
				// 发送邮件
				OnMessageOpenSendMailBox( pPlayer, pMessage );
				break;
			}
		case ID_C2S_REQUEST_OPENREADMAILBOX:
			{
				// 打开收件箱
				OnMessageOpenReadMailBox( pPlayer, pMessage );
				break;
			}
		case ID_C2S_REQUEST_MAILSEND:
			{
				// 发送邮件
				OnMessageSendMail( pPlayer, pMessage );	
				break;		
			}
		case ID_C2S_REQUEST_MAILCANSEND:
			{
				// 是否可以给指定玩家发送邮件的请求
				OnMessageCanMailSend( pPlayer , pMessage );
				break;
			}			
		case ID_C2S_REQUEST_CHANGEITEM:
			{
				// 改变邮件的附件
				//OnMessageChangeItem( pPlayer, pMessage ); 
				break;	               
			}				
		case ID_S2C_REQUEST_SHUTMAILBOX:
			{
				// 关闭邮箱
				OnMessageShutMailBox( pPlayer, pMessage );
				break;
			}
		case ID_C2S_REQUEST_SHUTREADMAILBOX:
			{
				// 关闭收件箱
				OnMessageShutReadMailBox( pPlayer, pMessage );
				break;
			}
		case ID_C2S_REQUEST_SHUTSENDMAILBOX:
			{
				// 关闭发件箱
				OnMessageShutSendMailBox( pPlayer, pMessage );
				break;
			}
	
		case ID_C2S_REQUEST_READMAIL:
			{
				// 读邮件
				OnMessageReadMail( pPlayer, pMessage );
				break;
			}
		case ID_C2S_REQUEST_DELMAIL:
			{	
				// 删除邮件			
				OnMessageDelMail( pPlayer, pMessage );
				break;
			}	
		case ID_C2S_NOTIFY_READMAILEND:
			{
				// 读取某一封信件完毕
				OnMessageReadMailEnd( pPlayer, pMessage );
				break;
			}	
		case ID_C2S_REQUEST_BACKMAIL:
			{
				// 退信
				OnMessageBackMail( pPlayer, pMessage );
				break;
			}
		case ID_C2S_REQUEST_LOCKMAIL:
			{
				OnMessageLockMail( pPlayer, pMessage );
				break;
			}
		case ID_C2S_REQUEST_UNLOCKMAIL:
			{
				OnMessageUnLockMail( pPlayer, pMessage );
				break;
			}
		case ID_C2S_REQUEST_GETMAILPAGE:
			{
				
				break;
			}
		case ID_C2S_REQUEST_GETACCESSORY:
			{
				OnMessageGetAccessory( pPlayer, pMessage );
				break;
			}
		case ID_C2S_NOTIFY_CANNCELMAILSEND:
			{
				break;
			}
		case ID_C2S_REQUEST_PAYMAILMONEY:
			{
				OnMessagePayMailMoneyRequest( pPlayer, pMessage );
				break;
			}
		default:
			break;
	}
}

// ***********************************************************
//  Function:		OnmessageOpenMailBoxReq
//  Description:	处理打开邮箱的请求
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnmessageOpenMailBoxReq( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return;
	}
	CMessageOpenMailBoxRequest *tpMsg = ( CMessageOpenMailBoxRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	
	// 检查有没有邮件服务
	if ( !CheckMailService( pPlayer, tpMsg->entityid() ) )
	{
		return;
	}
		
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}	

	OpenMailBox( pPlayer );	
}

// ***********************************************************
//  Function:		OnMessageOpenReadMailBox
//  Description:	处理打开送件箱的请求
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnMessageOpenReadMailBox( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return;
	}
	
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}
	OpenReadMailBox( pPlayer );
}

// ***********************************************************
//  Function:		OnMessageOpenSendMailBox
//  Description:	处理打开收件箱的请求
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnMessageOpenSendMailBox( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if( pPlayer == NULL || pMessage == NULL )
	{
		return;
	}
	
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}
	
	OpenSendMailBox( pPlayer );
}

// ***********************************************************
//  Function:		OnMessageSendMail
//  Description:	处理消息CMessageMailSend
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnMessageSendMail( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	if ( pPlayer == NULL || pMessage == NULL )
	{
		return;
	}
	
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}
	
	CMessageMailSend* pMessageEvent = ( CMessageMailSend* ) pMessage->msgpara( );
	if ( pMessageEvent == NULL )
	{
		return;
	}
	
	// 判断npc是否有邮件服务
	if ( !CheckMailService( pPlayer, pMessageEvent->entityid() ) )
	{
		return;
	}
	
	// 检查是否有密码保护
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();	
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tPwdCheckRet );
		return;
	}	
		
	SendMail( pPlayer, pMessageEvent );	
}

// ***********************************************************
//  Function:		OnMessageChangeItem
//  Description:	处理消息CMessageMailChangeItem   改变附件中的物品
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnMessageChangeItem( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	//LK_ASSERT( pPlayer != NULL, return );
	//
	//// 如果在花果山，不能进行邮件操作
	//if ( pPlayer->GetNationality() == 0 )
	//{	 
	//	return;	
	//}
	//
	//CMessageMailChangeItem* tpMessage = ( CMessageMailChangeItem* )pMessage->msgpara( );  
	//LK_ASSERT( pMessage != NULL, return );
	//CMail* tpMail = CMailManager::GetSingletonPtr( )->GetMail( pPlayer );
	//if( tpMail == NULL )
	//	return;
	//int  tRet = tpMail->ChangeMailItem( tpMessage->srcindex(), tpMessage->itemnum(), tpMessage->desindex(), tpMessage->movemethod() );	
	//if ( tRet == -1 )
	//{
	//	return;
	//}	 	
	//if ( tRet == 1 )
	//{
	//	CMessage MessageHead;
	//	CMessageMailChangeItemCallBack ChangeItemResponse;
	//	ChangeItemResponse.set_srcindex( tpMessage->srcindex() );
	//	ChangeItemResponse.set_itemnum( tpMessage->itemnum() );
	//	ChangeItemResponse.set_desindex( tpMessage->desindex() );
	//	ChangeItemResponse.set_movemethod( tpMessage->movemethod() );
	//	MessageHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CHANGEITEM );
	//	MessageHead.set_msgpara( (int)&ChangeItemResponse );
	//	CSceneLogic::GetSingletonPtr()->Send2Player( pPlayer, &MessageHead );
	//}
	//else
	//{
	//	CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tRet );
	//}
}
 
// ***********************************************************
//  Function:		OnMessageShutMailBox
//  Description:	处理消息CMessageShutMailBoxRequest
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnMessageShutMailBox( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}
	
	CMail *tpMail = ( CMail * )CMailManager::GetSingletonPtr()->GetMail( pPlayer );
	LK_ASSERT( tpMail != NULL, return );
	tpMail->SetMailStatus( MAIL_STATUS_SHUTDOWN );
	//tpMail->ChangeActionMailID( 0 );
	tpMail->ClearItemList();
}

// ***********************************************************
//  Function:		OnMessageShutReadMailBox
//  Description:	处理消息CMessageShutReadMailBoxRequest
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnMessageShutReadMailBox( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}
		
	CMail *tpMail = CMailManager::GetSingletonPtr()->GetMail( pPlayer );
	LK_ASSERT( tpMail != NULL , return );
	if ( tpMail->m_nMailBoxStatus != MAIL_STATUS_READMAIL )
	{
		LOG_ERROR( "mail","[%s,%d][%s]Wrong MailBox Status of CharID:%d!  ",__LK_FILE__,__LINE__,__FUNCTION__, pPlayer->GetCharID() );
		return;
	}
	tpMail->m_nMailBoxStatus = MAIL_STATUS_OPEN;
	return;
}

// ***********************************************************
//  Function:		OnMessageShutSendMailBox
//  Description:	处理消息CMessageShutSendMailBoxRequest
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnMessageShutSendMailBox( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}
	
	CMail *tpMail = CMailManager::GetSingletonPtr()->GetMail( pPlayer );
	LK_ASSERT( tpMail != NULL, return );
	if ( tpMail->m_nMailBoxStatus != MAIL_STATUS_SENDMAIL )
	{
		LOG_ERROR( "mail","[%s,%d][%s] Wrong MailBox Status of CharID:%d ",
			__LK_FILE__,__LINE__,__FUNCTION__,pPlayer->GetCharID() );
		return;
	}
	tpMail->m_nMailBoxStatus = MAIL_STATUS_OPEN;
	return;
}

// ***********************************************************
//  Function:		OnMessageReadMail
//  Description:	处理消息CMessageMailRead
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnMessageReadMail( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}
	
	CMessageMailRead *tpMsg  = ( CMessageMailRead * )pMessage->msgpara( );
	LK_ASSERT( tpMsg != NULL, return );
	
	// 检查有没有邮件服务
	if ( !CheckMailService( pPlayer, tpMsg->entityid() ) )
	{
		return;
	}	
	
	CMessage MessageHead;
	CMessageMailReadResponse MsgMailRead;
	CMail *tpMail = CMailManager::GetSingletonPtr( )->GetMail( pPlayer );
	LK_ASSERT( tpMail != NULL, return );
	tpMail->SetMailStatus( MAIL_STATUS_GETMAILDETAIL ); 
	tpMail->GetMailDetailFromDB( tpMsg->mailid( ) );	
}

// ***********************************************************
//  Function:		OnMessageGetAccessory
//  Description:	处理消息CMessageGetAccessory
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnMessageGetAccessory( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}
	
	CMessageGetAccessory  *tpMsg = ( CMessageGetAccessory * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	
	// 检查有没有邮件服务
	if ( !CheckMailService( pPlayer, tpMsg->entityid() ) )
	{
		return;
	}
	
	// 检查是否有密码保护
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tPwdCheckRet );
		return;
	}	
	
	CMail *tpMail = CMailManager::GetSingletonPtr( )->GetMail( pPlayer );
	LK_ASSERT( tpMail != NULL, return );
	int Ret = tpMail->GetAccessory( tpMsg->itemindex(), tpMsg->mailid(), tpMsg->ismoney( ) );	
	if ( Ret == -1 )
	{
		LOG_ERROR( "mail","[%s,%d][%s] GetAccessory  Error( CharID:%d )!",__LK_FILE__,
		__LINE__,__FUNCTION__,pPlayer->GetCharID() );
		return;
	}
	if ( Ret == 1 || Ret == 2 )
	{
		CMessage					 MessageHead;
		CMessageGetAccessoryResponse MsgGetAccessory;
		MsgGetAccessory.set_itemindex( tpMsg->itemindex() );
		MsgGetAccessory.set_mailid( tpMsg->mailid() );
		MsgGetAccessory.set_ismoney( tpMsg->ismoney() );
		if ( Ret == 2 )
		{
			MsgGetAccessory.set_ifdelmail( 1 );
		}
		MessageHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETACCESSORY );
		MessageHead.set_msgpara( (int)&MsgGetAccessory );
		CSceneLogic::GetSingletonPtr()->Send2Player( pPlayer, &MessageHead ); 
		return;
	}		 
	// 发送错误消息
	CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, Ret );
}

// ***********************************************************
//  Function:		OnMessageDelMail
//  Description:	删除邮件
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnMessageDelMail( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}
	
	CMessageDelMail *tpMsg = ( CMessageDelMail * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	
	// 检查有没有邮件服务
	if ( !CheckMailService( pPlayer, tpMsg->entityid() ) )
	{
		return;
	}	
	
	// 检查是否有密码保护
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tPwdCheckRet );
		return;
	}			
	
	CMail *tpMail = CMailManager::GetSingletonPtr()->GetMail( pPlayer );
	LK_ASSERT( tpMail != NULL, return );

	// 在一个删除/更新操作未完成之前，不允许删除其他有邮件
	if( tpMail->GetActionMailID() > 0 )
	{
		LOG_ERROR( "mail", "[%s:%d] player %d delete mail %d, opration %d is not done",
			__FUNCTION__, __LINE__, pPlayer->GetCharID(), tpMsg->mailid( ), tpMail->GetActionMailID()  );
		return;
	}

	int tRet = tpMail->DelMail( tpMsg->mailid( ) );
	if ( tRet == -1 )
	{
		LOG_DEBUG("sceneserver.log","[%s,%d][%s][CharID:%d] Delete mail error! ",
		__LK_FILE__,__LINE__,__FUNCTION__, pPlayer->GetCharID() );
		return;
	}	
	if ( tRet == 1 )
	{
		CMessageDelMailResponse MsgDelMail;
		CMessage				MessageHead;
		MsgDelMail.set_mailid( tpMsg->mailid( ) );
		MessageHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_DELMAIL );
		MessageHead.set_msgpara( (int)&MsgDelMail );
		CSceneLogic::GetSingletonPtr()->Send2Player( pPlayer, &MessageHead );		
		return;
	} 	
	// 发送错误提示码
	CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tRet );
}

// ***********************************************************
//  Function:		OnMessageReadMailEnd
//  Description:	 处理消息CMessageReadMailEnd:读取邮件完毕
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnMessageReadMailEnd( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}
	
	CMessageReadMailEnd *tpMsg = ( CMessageReadMailEnd * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	CMail *tpMail = CMailManager::GetSingletonPtr( )->GetMail( pPlayer );
	LK_ASSERT( tpMail != NULL, return );
	tpMail->UpdateMailInfo( tpMsg->mailid( ) );
}

// ***********************************************************
//  Function:		SendMail
//  Description:	发送邮件
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::SendMail( CEntityPlayer* pPlayer, CMessageMailSend* pMessage )
{
	LK_ASSERT( pPlayer != NULL, return )
	
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}
	
	// 检查是否有密码保护
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tPwdCheckRet );
		return;
	}

	// 检查是否超过携带上限
	CTemplateLevelMoneyLmt* tpMoneyTable = (CTemplateLevelMoneyLmt*)CDataStatic::GetTemp(TEMP_MONEYLMT_CFG);
	LK_ASSERT( tpMoneyTable!=NULL, return );
	LK_ASSERT( pPlayer->GetLevel() >= 0 && pPlayer->GetLevel() < TMP_MAX_GRADE, return );
	if (pMessage->moneyneedback() > tpMoneyTable->mLmtTable[pPlayer->GetLevel()].mBagMoneyLmt )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERR_MAIL_BACKMONEY_OVER );
		return;
	}
	
	CMail* tpMail = CMailManager::GetSingletonPtr( )->GetMail( pPlayer );
	LK_ASSERT( tpMail != NULL, return );
	if ( tpMail->m_nMailBoxStatus != MAIL_STATUS_SENDMAIL )
	{
		LOG_ERROR( "mail","[%s,%d][%s] Wrong MailStatus! ",
		 __LK_FILE__,__LINE__,__FUNCTION__ );
		return;
	}
	
	if (  tpMail->GetMailReceiverID() != (int)pMessage ->descharid() )
	{
		LOG_ERROR( "mail","[%s,%d][%s] Wrong  Character Id  while sending mail!!!  " ,
		 __LK_FILE__,__LINE__,__FUNCTION__ );
		return;
	}
	if( pPlayer->GetCharID() == pMessage->descharid() )
	{		
		// 不能给给自己发送邮件
		return;
	}
	else
	{	
		// 添加邮件附件
		for ( int i = 0; i < pMessage->accessory_size() ; ++i )
		{
			int tRet = tpMail->ChangeMailItem( pMessage->mutable_accessory( i )->itemindex(), pMessage->mutable_accessory( i )->itemnum(), pMessage->mutable_accessory( i )->itempos() );
			if ( tRet == SUCCESS )
			{				
				continue;
			}
			if ( tRet != -1 )
			{
				CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tRet );
			}			
		}		
			
		// 标题长度检查
		if ( pMessage->mutable_title()->length( ) >= MAIL_TITLE_LEN )
		{
			LOG_ERROR( "mail","[%s,%d][%s] Mailbody  is too long !!!  ",
			__LK_FILE__,__LINE__,__FUNCTION__ );
			return;
		}
		
		// 正文长度检查
		if (  pMessage->mutable_body()->length() >= MAIL_BODY_LEN )
		{
			LOG_ERROR( "mail","[%s,%d][%s] Mailtitle is too long !!! ",
			__LK_FILE__,__LINE__,__FUNCTION__ );
			return;
		}
		
		// 邮件接收方长度检查
		if ( pMessage->mutable_desname()->length() >= NAME_LENGTH )
		{
			LOG_ERROR( "mail","[%s,%d][%s]  The name of mailreceiver is too long!!! " );
			return;			
		}
		
		// 屏蔽字检查
		CDirtyWord::GetSingletonPtr()->OI_Dirty_Check( 1, ( char * )pMessage->mutable_title()->c_str( ) );
		CDirtyWord::GetSingletonPtr()->OI_Dirty_Check( 1, ( char * ) pMessage->mutable_body()->c_str( ) );
		
		// 发送邮件
		int tRet = tpMail->Send(  pMessage->money(),pMessage->moneyneedback(), pMessage->mutable_title()->c_str(), 
		pMessage->mutable_body()->c_str() , pMessage->descharid(), pMessage->mutable_desname()->c_str() );	
		
		// 设置邮箱状态为打开状态
		tpMail->SetMailStatus( MAIL_STATUS_OPEN );			
		
		// 发生错误
		if ( tRet == -1 )
		{
			LOG_ERROR( "mail","[%s,%d][%s][charid:%d] Send mail error! ",
			__LK_FILE__,__LINE__,__FUNCTION__,pPlayer->GetCharID()  );
			return;
		}
		
		// 发送错误通知				
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tRet );
	}	
}

// ***********************************************************
//  Function:		ReceiveRouterMail
//  Description:	接收来自其他国家的邮件
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::ReceiveRouterMail( CMessage *pMessage )
{
	CMessageNewMailNotify *pMsg = ( CMessageNewMailNotify * )pMessage->msgpara();
	LK_ASSERT( pMsg != NULL, return );
	int DesCharID = pMsg->router().descharid();
	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( DesCharID );
	if ( tpPlayer == NULL )
	{
		return;
	}
	// 如果在花果山，不能进行邮件操作
	if ( tpPlayer->GetNationality() == 0 )
	{	 
		return;	
	}	
	
	CMail *tpMail = CMailManager::GetSingleton().GetMail( tpPlayer );
	LK_ASSERT( tpMail != NULL, return ); 
	int tRet = tpMail->ReceiveRouterMail( pMsg );
	if( tRet == 1 )
	{
		CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMessage );
	}
}

// ***********************************************************
//  Function:		CheckInvitationReceiver
//  Description:	查询结婚接受人的角色ID信息
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::CheckInvitationReceiver( CEntityPlayer *pPlayer, CMessageGetPlayerMailInfoResponse *pMessage )
{
	LK_ASSERT( pMessage != NULL && pMessage != NULL, return )	
	if ( pMessage->errorcode(  ) == 0 )
	{			
		CMarriageModule::GetSingleton().SendUseSysInvitationResponse( pPlayer, EM_MARRIAGE_INVITATION_RECEIVERNOEXIST );
		return;
	}	

	// 邮件接收方的国籍判断	
	if ( pMessage->roleofcountry() != pPlayer->GetNationality() )
	{		
		CMarriageModule::GetSingleton().SendUseSysInvitationResponse( pPlayer, EM_MARRIAGE_INVITATION_INDIFFCOUNTRY );
		return;
	}	
	
	if ( pMessage->querydata_size() < 1 )
	{									
		return;
	}
	
	CMarriageModule::GetSingleton().SendInvitationByMail( pPlayer, pMessage->querydata( 0 ), pMessage->roleid(), pMessage->mutable_rolename()->c_str() );
}

// ***********************************************************
//  Function:		OnCreateEntity
//  Description:	建立实体
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst )
{	
	LK_ASSERT( pEntity != NULL, return )
	if( pEntity->GetEntityType() == ENTITYTYPE_PLAYER )
	{
		CEntityPlayer* tpPlayer = (CEntityPlayer*)pEntity;		

		// 加载离线留言
		CMailDBOperate::LoadOffLineChatMsg( tpPlayer->GetCharID(), tpPlayer->GetNationality() );

		CMail* tpMail = CMailManager::GetSingletonPtr()->AddMail( tpPlayer );		
		if ( tpMail == NULL )
		{
			LOG_ERROR( "mail","[ %s : %d ][ %s ]Launch Error", __LK_FILE__, __LINE__, __FUNCTION__  );
			return;
		}
		if ( tpPlayer->GetNationality() == 0 )
		{
			tpMail->SetMailStatus( MAIL_STATUS_SHUTDOWN );
			return;
		}
		tpMail->SetMailStatus( MAIL_STATUS_INITIALIZE );
		CMailDBOperate::GetMailBoxStatus( tpPlayer->GetCharID(), tpPlayer->GetNationality() );			
	}
}

// ***********************************************************
//  Function:		OnDestroyEntity
//  Description:	销毁实体
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnDestroyEntity( CEntity* pEntity, int vCode )
{
	LK_ASSERT( pEntity != NULL, return );
	if( pEntity->GetEntityType() == ENTITYTYPE_PLAYER )
	{
		CEntityPlayer* tpPlayer = ( CEntityPlayer* )pEntity;
		
		// 如果在花果山，不能进行邮件操作
		CMail *tpMail = CMailManager::GetSingletonPtr()->GetMail( tpPlayer );		
		if ( tpMail	== NULL )
		{
			LOG_ERROR( "mail", "can not find mail object of player ( roleid:%d ) by ID:%d when destroy player", tpPlayer->GetCharID(), tpPlayer->GetMailObjID() );			
			return;
		} 		
		
		tpMail->DestroyMailCache();						
		CMailManager::GetSingletonPtr()->Erase( tpPlayer );
	}
}

// ***********************************************************
//  Function:		OnSaveData
//  Description:	保存玩家数据
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
bool CMailModule::OnSaveData( CEntityPlayer *pPlayer, int vSaveCode )
{
	return true;
}

// ***********************************************************
//  Function:		OnLoadConfig
//  Description:	加载配置
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnLoadConfig( ) 
{
	//InitializeSysMail( "../config/data/mail/SysMail.xml" );
	InitializeSysMail(CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),"mail/SysMail.xml" ));
	const char* pcConfigPath = CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),"mail/SysMail.xml" );
	CSceneServer::GetSingleton().RegisterLoadConfigCallback(pcConfigPath, CMailModule::LoadSysMailConfigCallback);
}

// ***********************************************************
//  Function:		OnMessageCanMailSend
//  Description:	查看邮件接收方是否存在
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnMessageCanMailSend( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	CMessageMailCanSend       *tpMessage  = ( CMessageMailCanSend * )pMessage->msgpara();
	LK_ASSERT( tpMessage != NULL, return );
	
	// 检查有没有邮件服务
	if ( !CheckMailService( pPlayer, tpMessage->entityid() ) )
	{
		return;
	}
	
	// 检查是否有密码保护
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();	
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tPwdCheckRet );
		return;
	}
	
	if ( !tpMessage->has_name() )
	{
		return;	
	}
	if ( tpMessage->mutable_name( )->length( ) >= NAME_LENGTH )
	{
		return;
	}
	
	// 在花果山时不能发送邮件
	if ( pPlayer->GetNationality() == 0 )
	{			
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERR_MAIL_SENDMAIL_SENDNOTALLOWED );
		return;
	}

	const char                *PlayerName = tpMessage->mutable_name()->c_str();	
	if ( strncmp( PlayerName, pPlayer->GetCharName() , NAME_LENGTH ) == 0 )
	{
		// 不能给自己发送邮件
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERR_MAIL_SENDMAIL_NOTINSELFCONTRY );
		return;
	}
	
	CMessage                  HeadMessage;	
    CMessageMailCanSendCallBack MailCanSendCallBack;
    if ( tpMessage->mutable_name()->size( ) > NAME_LENGTH || tpMessage->mutable_name()->size( ) == 0  )
    {
		LOG_ERROR( "mail","[%s,%d][%s] Name of Mail Receiver is too long sended by CharID:! ",
			__LK_FILE__,__LINE__,__FUNCTION__, pPlayer->GetCharID() );
		return;
    }   
    CMail *tpMail = ( CMail * )CMailManager::GetSingletonPtr( )->GetMail( pPlayer );
    LK_ASSERT( tpMail != NULL, return );
    CEntityPlayer *tpPlayer = CSceneLogic::GetSingletonPtr( )->GetPlayerByName( PlayerName );
	if ( tpPlayer == NULL )
	{		
		// 玩家不在线向网关查询玩家状态
		if ( tpMail->GetMailStatus( ) != MAIL_STATUS_OPEN  )
		{	
			// 邮箱状态错误
			return;
		}
		tpMail->SetMailStatus( MAIL_STATUS_CHECROLEINFO );		
		CMessageGetPlayerMailiInfoRequest MsgRoleInfo;	
		MsgRoleInfo.set_charid( pPlayer->GetCharID() );	
		MsgRoleInfo.set_rolename( PlayerName );
		MsgRoleInfo.set_queryway( 0 );
		HeadMessage.mutable_msghead( )->set_messageid( ID_C2G_REQUEST_GETMAILINFO );
		HeadMessage.set_msgpara( (int)&MsgRoleInfo );
		CSceneLogic::GetSingletonPtr( )->Send2Gate( &HeadMessage );		
	}
	else
	{
		// 设置邮箱状态发送状态
		tpMail->SetMailStatus( MAIL_STATUS_SENDMAIL );	
		
		// 设置邮件发送方的角色ID
		tpMail->SetMailReceiverID( tpPlayer->GetCharID() );
			
		// 是否是本国玩家
		if ( pPlayer->GetNationality() != tpPlayer->GetNationality() )
		{
			CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERR_MAIL_SENDMAIL_DESOINOTHERCOUNTRY );
			return;
		}					
				
		// 玩家在线可以发送邮件
		MailCanSendCallBack.set_cansendmail( 1 );
		MailCanSendCallBack.set_playercharid( tpPlayer->GetCharID() );
		HeadMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_MAILCANSEND );
		HeadMessage.set_msgpara( ( int )&MailCanSendCallBack );
		CSceneLogic::GetSingletonPtr()->Send2Player( pPlayer , &HeadMessage );		
	}
}

// ***********************************************************
//  Function:		OpenMailBox
//  Description:	打开玩家邮箱
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OpenMailBox( CEntityPlayer*pPlayer )
{
	LK_ASSERT( pPlayer != NULL, return );

	
	CMessage                    MessageHead;
	CMessageOpenMailBoxResponse MsgOpenMail;
	CMessageMailListNotify MsgMailList; 
	/*
		判断玩家是否在自己国家和中立区
	*/
	CMail *tpMail = ( CMail * )CMailManager::GetSingletonPtr( )->GetMail( pPlayer );
	if ( tpMail == NULL )
	{
		SendMailErrorCode( pPlayer, ERROR_MAILBOX_CANNOTOPEN );
		return;
	}
	if ( tpMail->GetMailStatus() ==  MAIL_STATUS_INITIALIZE )
	{	
		// 邮箱还没有初始化完毕	
		SendMailErrorCode( pPlayer, ERROR_INITIAL_NOTCOMPLETE );
		LOG_ERROR( "mail","[%s,%d][%s]Mail Box of player charID:%d is not initialized!",
			__LK_FILE__,__LINE__,__FUNCTION__, pPlayer->GetCharID() );
		return;		
	}
	
	// 获取第一页邮件
	int tRet = tpMail->GetMailPage( );
	if( tRet == -1 )
	{
		//SendMailErrorCode( pPlayer, ERROR_CHANGEPAGE_NOMAIL );		
	} 
	
	// 设置邮件状态 
	tpMail->SetMailStatus( MAIL_STATUS_OPEN );					
	MsgOpenMail.set_result( 1 );
	MessageHead.mutable_msghead( )->set_messageid( ID_S2C_RESPONSE_OPENMAILBOX );
	MessageHead.set_msgpara( ( int )&MsgOpenMail );	
	CSceneLogic::GetSingletonPtr( )->Send2Player( pPlayer, &MessageHead );	
}

// ***********************************************************
//  Function:		OpenSendMailBox
//  Description:	打开玩家送件箱
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OpenSendMailBox( CEntityPlayer *pPlayer )
{
	LK_ASSERT( pPlayer != NULL, return );
	
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}
	
	CMessage                        MessageHead;
	CMessageOpenSendMailBoxResponse OpenSendMailBox;
	/*
		判读玩家所在区域
	*/
	CMail *tpMail = CMailManager::GetSingletonPtr( )->GetMail( pPlayer );
	LK_ASSERT( tpMail != NULL, return );
	if ( tpMail->m_nMailBoxStatus != MAIL_STATUS_OPEN )
	{
		LOG_ERROR( "mail","[%s,%d][%s] Wrong MailBox Status !",__LK_FILE__,__LINE__,__FUNCTION__ );
		return;
	}
	tpMail->m_nMailBoxStatus = MAIL_STATUS_SENDMAIL;	
	OpenSendMailBox.set_result( 1 );
	MessageHead.mutable_msghead( )->set_messageid( ID_S2C_RESPONSE_OPENSENDMAILBOX );
	MessageHead.set_msgpara( (int)&OpenSendMailBox );
	CSceneLogic::GetSingletonPtr()->Send2Player( pPlayer, &MessageHead );		  
}

// ***********************************************************
//  Function:		OpenReadMailBox
//  Description:	打开玩家收件箱
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OpenReadMailBox( CEntityPlayer *pPlayer )
{
	LK_ASSERT( pPlayer != NULL, return );
	
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}
	
	CMessage                        MessageHead;
	CMessageOpenReadMailBoxResponse OpenReadMailBox;
	/*
	判读玩家所在区域
	*/
	CMail *tpMail = CMailManager::GetSingletonPtr()->GetMail( pPlayer );
	LK_ASSERT( tpMail != NULL, return );
	tpMail->m_nMailBoxStatus = MAIL_STATUS_READMAIL;		
	OpenReadMailBox.set_result( 1 );	
	MessageHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_OPENREADMAILBOX );	
	MessageHead.set_msgpara( ( int )&OpenReadMailBox );
	CSceneLogic::GetSingletonPtr()->Send2Player( pPlayer, &MessageHead  );	 
}

// ***********************************************************
//  Function:		OnMessageBackMail
//  Description:	处理消息CMessageBackMail:玩家退信
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnMessageBackMail( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}
	
	// 检查是否有密码保护
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tPwdCheckRet );
		return;
	}
	
	CMessageBackMail *tpMsg = ( CMessageBackMail * )pMessage->msgpara();
	LK_ASSERT( pMessage != NULL, return );
	
	// 检查有没有邮件服务
	if ( !CheckMailService( pPlayer, tpMsg->entityid() ) )
	{
		return;
	}
	
	CMail *tpMail = ( CMail * )CMailManager::GetSingletonPtr()->GetMail( pPlayer );
	LK_ASSERT( tpMail != NULL, return );
	
	// 正文长度检查
	if (  tpMsg->mutable_mailbody( )->length() >= MAIL_BODY_LEN )
	{
		LOG_ERROR( "mail","[%s,%d][%s] Mailtitle  is too long  sended by charID:%d!!! ",
			__LK_FILE__,__LINE__,__FUNCTION__, pPlayer->GetCharID() );
		return;
	}		
	int tRet = tpMail->BackMail( tpMsg->mailid( ), tpMsg->mutable_mailbody( )->c_str( ) );
	
	// 退信错误
	if ( tRet == -1 )
	{
		LOG_ERROR( "mail","[%s,%d][%s][charid：%d] Backmail Error! ",
			__LK_FILE__,__LINE__,__FUNCTION__, pPlayer->GetCharID() );
		return;
	}
	
	// 退信成功
	if ( tRet == 1 )
	{
		CMessage					MessageHead;
		CMessageBackMailResponse 	MsgBackMail;
		MsgBackMail.set_mailid( tpMsg->mailid() );
		MessageHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_BACKMAIL );		
		MessageHead.set_msgpara( (int)&MsgBackMail );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &MessageHead );
		return;
	}
	
	// 发送错误提示	
	CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tRet );
}

// ***********************************************************
//  Function:		OnMessageLockMail	
//  Description:	给邮件加锁
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnMessageLockMail( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}
	
	// 检查是否有密码保护
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tPwdCheckRet );
		return;
	}		
	
	CMessageLockMail *tpMsg = ( CMessageLockMail * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	CMail *tpMail = CMailManager::GetSingletonPtr()->GetMail( pPlayer );
	LK_ASSERT( tpMail != NULL, return );
	int tRet = tpMail->LockMail( tpMsg->mailid( ) );	
	if ( tRet == -1 )
	{
		LOG_ERROR("mail","[%s,%d][%s][charid:%d] Lock mail error!!!",
		__LK_FILE__,__LINE__,__FUNCTION__, pPlayer->GetCharID() );
	}
	if ( tRet == 1 )
	{
		return;	
	}
}

// ***********************************************************
//  Function:	 OnMessageUnLockMail	
//  Description: 解锁邮件	
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnMessageUnLockMail( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL, return );
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}	
	CMessageUnLockMail *tpmsg = ( CMessageUnLockMail * )pMessage->msgpara();
	LK_ASSERT( tpmsg != NULL, return );	
	CMail *tpMail = CMailManager::GetSingletonPtr( )->GetMail( pPlayer );	
	LK_ASSERT( tpMail != NULL, return );
	int tRet = tpMail->UnLockMail(  tpmsg->mailid( ) );
	if ( tRet == 1 )
	{
		// 发送成功消息
	}
} 

// ***********************************************************
//  Function:		OnMessageGetMailPageRequest
//  Description:	获取一页邮件信息
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnMessageGetMailPageRequest( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}
	CMessageGetMailPageRequest *tpMsg = ( CMessageGetMailPageRequest * )pMessage->msgpara( );
	LK_ASSERT( tpMsg != NULL, return );
	CMail *tpMail = ( CMail * )CMailManager::GetSingletonPtr()->GetMail( pPlayer );
	if ( tpMail == NULL )
	{
		LOG_ERROR( "mail","[%s,%d][%d] Get CMail of  Player ( CharID： )%d Error, null pointer!",__LK_FILE__,
		__LINE__,__FUNCTION__, pPlayer->GetCharID());
		return;
	}
	int Ret =  tpMail->GetMailPage( );
	if ( Ret == -1 )
	{
		SendMailErrorCode( pPlayer, ERROR_CHANGEPAGE_NOMAIL );
	}
}

// ***********************************************************
//  Function:		OnMessageCannelMailSend
//  Description:	取消邮件的发送
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnMessageCannelMailSend( CEntityPlayer *pPlayer, CMessage *pMessage )
{
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return );
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}
	CMessageCannelMailSend *tpMsg = ( CMessageCannelMailSend * )pMessage->msgpara();
	if(  tpMsg == NULL )
	{
		LOG_ERROR( "mail","[%s,%d][%d] Empty Message  CMessageCannelMailSend!",__LK_FILE__,__LINE__,__FUNCTION__);
		return;
	}
	CMail *tpMail = ( CMail * )CMailManager::GetSingletonPtr()->GetMail( pPlayer );
	if ( tpMail == NULL )
	{
		LOG_ERROR( "mail","[%s,%d][%d] Get CMail of  Player ( CharID： )%d Error, null pointer!",__LK_FILE__,
			__LINE__,__FUNCTION__, pPlayer->GetCharID());
		return;
	}
	tpMail->ClearItemList();	
}

// ***********************************************************
//  Function:		OnMessagePayMailMoneyRequest
//  Description:	付费邮件付费的请求
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnMessagePayMailMoneyRequest( CEntityPlayer *pPlayer, CMessage *pMessage )
{  
	LK_ASSERT( pPlayer != NULL && pMessage != NULL, return )
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}	
	
	// 检查是否有密码保护
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	int tPwdCheckRet = tpProperty->GetRolePassword()->CheckActionAllowed( EM_PASSWORD_ACTION_GAMEACTION );
	if ( tPwdCheckRet != SUCCESS )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, tPwdCheckRet );
		return;
	}	
	
	CMessagePayMailMoneyRequest *tpMsg = ( CMessagePayMailMoneyRequest * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CMail *tpMail = CMailManager::GetSingletonPtr( )->GetMail( pPlayer );		
	LK_ASSERT( tpMail != NULL, return );
	if ( tpMail->GetMailCacheByMailID( tpMsg->mailid() ) == NULL )
	{
		return;
	}
	
	// 检查邮件的有效性
	CMailDBOperate::CheckIfMailInDB( pPlayer->GetCharID(), pPlayer->GetNationality(), tpMsg->mailid() );
}

// ***********************************************************
//  Function:		CheckRoleNameInfo
//  Description:	处理邮件接收方查询结果
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::CheckRoleNameInfo( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );
	CMessageGetPlayerMailInfoResponse *pRoleInfoMsg = ( CMessageGetPlayerMailInfoResponse*  )pMessage->msgpara( );
	LK_ASSERT( pRoleInfoMsg != NULL, return );
	CEntityPlayer *pPlayer = ( CEntityPlayer * )CSceneLogic::GetSingletonPtr( )->GetPlayerByCharID( pRoleInfoMsg->charid( ) );		
	
	// 邮件发送方已经下线	
	if ( pPlayer == NULL )
	{	
		return;
	}
	// 如果在花果山，不能进行邮件操作
	if ( pPlayer->GetNationality() == 0 )
	{	 
		return;	
	}
	
	// 判断是否是对请帖接收人的查询
	if ( pRoleInfoMsg->querytype() == ROLEQUERYTYPE_MARRIAGEINVITATION )
	{
		CheckInvitationReceiver( pPlayer, pRoleInfoMsg );
		return;
	}
	
	// 获取邮箱对象	
	CMail *tpMail = ( CMail * )CMailManager::GetSingletonPtr( )->GetMail( pPlayer );
	LK_ASSERT( tpMail != NULL, return ) ;
	
	// 邮件接收方不存在
	if ( pRoleInfoMsg->errorcode(  ) == 0 )
	{	
		tpMail->SetMailStatus( MAIL_STATUS_OPEN );
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERR_MAIL_SENDMAIL_DESNOTEXIST );
		return;
	}	
	
	// 邮件接收方的国籍判断	
	if ( pRoleInfoMsg->roleofcountry() != pPlayer->GetNationality() )
	{
		tpMail->SetMailStatus( MAIL_STATUS_OPEN );
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERR_MAIL_SENDMAIL_DESOINOTHERCOUNTRY );
		return;
	}	
	
	// 发送回应消息   
	CMessage		 			MessageHead;
	CMessageMailCanSendCallBack MailCanSendCallBack;	
	if ( tpMail->m_nMailBoxStatus != MAIL_STATUS_CHECROLEINFO )
	{	
		LOG_ERROR( "mail","[%s,%d][%s] Wrong MailStatus! ",
		__LK_FILE__,__LINE__,__FUNCTION__ );
		return;
	}	
	tpMail->SetMailStatus( MAIL_STATUS_SENDMAIL );
	tpMail->SetMailReceiverID( pRoleInfoMsg->roleid(  ) );
	MailCanSendCallBack.set_playercharid(  pRoleInfoMsg->roleid(  ) );
	MailCanSendCallBack.set_cansendmail( 1 );
	MessageHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_MAILCANSEND );
	MessageHead.set_msgpara( (int)&MailCanSendCallBack );
	CSceneLogic::GetSingletonPtr()->Send2Player( pPlayer, &MessageHead );
}

// ***********************************************************
//  Function:		SendMailErrorCode
//  Description:	发送邮件的错误通知
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::SendMailErrorCode( CEntityPlayer *pPlayer, int ErrorCode )
{
	LK_ASSERT( pPlayer != NULL, return );
	CMessage			  HeadMessage;
	CMessageMailErrorCode MsgErrorCode;	
	MsgErrorCode.set_errorcode( ErrorCode );
	HeadMessage.mutable_msghead( )->set_messageid( ID_S2C_NOTIFY_MAILERRORCODE );
	HeadMessage.set_msgpara( ( int )&MsgErrorCode );
	CSceneLogic::GetSingletonPtr()->Send2Player( pPlayer, &HeadMessage );
}

// ***********************************************************
//  Function:		SendSysMail
//  Description:	给玩家发送系统邮件的接口
//  Input:			int DesCharid			目标玩家角色ID
//  Input:			const char *pTitle		邮件标题
//  Input:			const char *pSrcName	邮件发送方姓名
//  Input:			const char *pBody		邮件正文
//  Input:			CItemObject **tpItemObj 附件物品指针数组
//　Input:			 int ItemObjNum			附件物品数量
//  Input:			int	nCharID				角色ID(和邮件接收方同服务器的在线玩家的角色ID，只为执行
//											sql语句时能获取接收方的world ID,大雷音寺或者跨服战场用)
//　Input:			int ItemObjNum			附件物品数量
//  Others:
//  Date:	02/04/2009
// **********************************************************
int CMailModule::SendSysMail( const char *pDesName, int DesCharid, const char *pTitle, const char *pSrcName, const char *pBody, CItemObject **tpItemObj,int ItemObjNum, int CountryID, int nMoney /* = 0 */, int nCharID )
{	
	int tRet =  CMail::SysSend( pDesName, DesCharid, pTitle, pSrcName, pBody, tpItemObj, ItemObjNum, CountryID, nMoney, nCharID );
 	return tRet;
}

// ***********************************************************
//  Function:		SendSysMail
//  Description:	给玩家发送系统邮件的接口
//  Input:			int DesCharid			目标玩家角色ID
//  Input:			const char *pTitle		邮件标题
//  Input:			const char *pSrcName	邮件发送方姓名
//  Input:			const char *pBody		邮件正文
//  Input:			int *pItemIDList		附件物品ID数组
//  Input:			int *pItemNumList		附件物品数量数组
//　Input:			int ItemObjNum			附件物品数量
//  Input:			int	nCharID				角色ID(和邮件接收方同服务器的在线玩家的角色ID，只为执行
//											sql语句时能获取接收方的world ID,大雷音寺或者跨服战场用)
//  Return:			int						等于-1表示发送失败
//  Others:
//  Date:	02/04/2009
// **********************************************************
int CMailModule::SendSysMail( const char *pDesName, int DesCharID, const char *pTitle, const char *pSrcName, const char *pBody, int *pItemIDList, int *pItemNumList, int nItemNum, int nCountryID, int nMoney, int nCharID ) 
{
	CItemObject *tpItemObj[MAIL_ITEM_COUNT_MAX] = { NULL } ;

	int tNum = 0;
	int tTempMoney = nMoney;
	int tRet = 0;
	
	// 如果是纯文本文件直接发送
	if ( nItemNum <= 0 && tTempMoney <= 0 )
	{
		tRet =  CMail::SysSend( pDesName, DesCharID, pTitle, pSrcName, pBody, tpItemObj, tNum, nCountryID, tTempMoney, nCharID );
		if( tRet < 0 )
		{
			LOG_ERROR( "mail", "[%s:%d] send mail to %d failed", __FUNCTION__, __LINE__, DesCharID );
		}
		return SUCCESS;
	}
	
	for( int i = 0; i < nItemNum; ++i  )
	{
		if ( pItemIDList == NULL )
		{				
			break;
		}
		if( pItemIDList[i] == 0 )
		{
			continue;
		}

		CTemplate* tpTemplate = CDataStatic::SearchTpl( pItemIDList[i] );
		if ( tpTemplate == NULL )
		{
			LOG_ERROR( "mail","[%s,%d]can not find template by id:%d",__FUNCTION__,__LINE__,pItemIDList[i] );
			continue;
		}	

		// 如果物品类型不是道具
		if( tpTemplate->mTemplateType != EM_TEMPTYPE_ITEM )
		{
			LOG_ERROR( "mail","[%s,%d] %d is not itemtype ",__FUNCTION__,__LINE__,pItemIDList[i] );
			continue;
		}		

		CTplItem * tpTplItem = ( CTplItem * )tpTemplate;
		
	
		int nResult = CPropertyModule::GetSingleton().CreateItem( tpTplItem, pItemNumList[i], 0, &tpItemObj[tNum] );
		if ( nResult != SUCCESS || tpItemObj[tNum] == NULL )
		{
			LOG_ERROR( "mail","[%s,%d] creat item wrong , item template id:%d ",__FUNCTION__,__LINE__, pItemIDList[i] );
			continue;
		}		
		
		// 道具个数太多，一封邮件发不下，分N次发, 
		if( ++tNum >= MAIL_ITEM_COUNT_MAX )
		{
			tRet =  CMail::SysSend( pDesName, DesCharID, pTitle, pSrcName, pBody, tpItemObj, tNum, nCountryID, tTempMoney );
			if( tRet < 0 )
			{
				LOG_ERROR( "mail", "[%s:%d] send mail to %d failed", __FUNCTION__, __LINE__, DesCharID );
			}
			tNum = 0;

			// 金钱置0，否则多次发送会出错
			tTempMoney = 0;
		}
	}		 	

	// 发送剩余的道具部分
	if( tNum > 0 || tTempMoney > 0 )
	{
		tRet =  CMail::SysSend( pDesName, DesCharID, pTitle, pSrcName, pBody, tpItemObj, tNum, nCountryID, tTempMoney, nCharID );
		if( tRet < 0 )
		{
			LOG_ERROR( "mail", "[%s:%d] send mail to %d failed", __FUNCTION__, __LINE__, DesCharID );
		}
	}
	return tRet;
}

// ***********************************************************
//  Function:		SendSysMail
//  Description:	给玩家发送系统邮件的接口
//  Input:			int DesCharid			目标玩家角色ID
//  Input:			int nSysMailID			系统邮件信息在配置中的ID	
//  Input:			int *pItemIDList		附件物品ID数组
//  Input:			int *pItemNumList		附件物品数量数组
//　Input:			int ItemObjNum			附件物品数量
//	Input:			const char **pParam	字符串参数
//	Input:			int *pParamPos	每个字符串参数的位置，0表示标题1表示正文
//	Input:			int nNum		字符串参数的个数
//  Input:			int	nCharID				角色ID(和邮件接收方同服务器的在线玩家的角色ID，只为执行
//											sql语句时能获取接收方的world ID,大雷音寺或者跨服战场用)
//  Return:			int						等于-1表示发送失败
//  Others:
//  Date:	02/04/2009
// **********************************************************
int CMailModule::SendSysMailBySysID( int nSysMailID, const char *pDesName, int DesCharID, int *pItemIDList, int *pItemNumList, int nItemNum, int nCountryID, int nMoney /* = 0 */, char **pParam /* = NULL */, int *pParamPos /* = NULL */, int nNum /* = 0 */, int nCharID )
{
	
	char tName[NAME_LENGTH] = { 0 };
	char tTitle[MAIL_TITLE_LEN] = { 0 };
	char tBody[MAIL_BODY_LEN] = { 0 };	
	sprintf( tName, "%d", nSysMailID );	   	
	if ( pParam != NULL && pParamPos != NULL && nNum > 0 )
	{
		char *pTitlePos = tTitle;
		char *pBodyPos  = tBody;
		for ( int i = 0; i < nNum; ++i )
		{
			if ( pParamPos[i] == 0 )
			{
				sprintf( pTitlePos, "%s \t", pParam[i] );
				pTitlePos = strchr( pTitlePos, '\t' );	
			}
			
			if ( pParamPos[i] == 1 )
			{
				sprintf( pBodyPos, "%s \t", pParam[i] );
				pBodyPos = strchr( pBodyPos, '\t' );
			}
		}
	}
	return SendSysMail( pDesName, DesCharID, tTitle, tName, tBody, pItemIDList, pItemNumList, nItemNum, nCountryID, nMoney, nCharID );
}

// ***********************************************************
//  Function:		CheckMailService
//  Description:	检查npc的服务类型
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
bool CMailModule::CheckMailService( CEntityPlayer *pPlayer, int nEntityID )
{
	
	LK_ASSERT( pPlayer != NULL, return  false )

	//TODO: nNpcEntityID可能是客户端消息上报的，必须检查合法性。
	// 家族npc的判断
	CEntityNpc* tpEntity = dynamic_cast< CEntityNpc* >( CSceneObjManager::GetSingletonPtr( )->GetObject( nEntityID ) );
	LK_ASSERT( tpEntity != NULL, return false  )
	if ( tpEntity->GetEntityType() != ENTITYTYPE_FUNCNPC )
	{
		return false;
	}

	// 服务判断
	CPropertyNPC * tpNpcProperty = ( CPropertyNPC * )tpEntity->GetProperty(); 
	CTemplateNpc *tpTmpNpc = ( CTemplateNpc * )CDataStatic::SearchTpl( tpNpcProperty->GetPropertyTempID() );
	LK_ASSERT( tpTmpNpc != NULL, return false )
	if ( tpTmpNpc->mMailService == 0 )
	{
		return false;
	}

	// 距离判断
	return pPlayer->IsWithinDistance( tpEntity, MAX_NPC_TALK_GIRD );			
}

// ***********************************************************
//  Function:		CheckIfMailInDB
//  Description:	检查数据库是否还存在邮件
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::CheckIfMailInDB( CEntityPlayer *pPlayer, int nMailID, int nCheckType )
{
	LK_ASSERT( pPlayer != NULL, return )		
	CMailDBOperate::CheckIfMailInDB( pPlayer->GetCharID(), (int)pPlayer->GetNationality(), nMailID );
}

// ***********************************************************
//  Function:		OnSessionCheckIfMailInDB
//  Description:	检查数据库是否还存在邮件
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::OnSessionCheckIfMailInDB( CMessage * pMessage, int nCharID, int nMailID )
{
	LK_ASSERT( pMessage != NULL, return )	
	CMessageExecuteSqlResponse3 *tpMsg = ( CMessageExecuteSqlResponse3 * )pMessage->msgpara();
	if( tpMsg == NULL )
	{
		return;
	}
	
	// 成功
	if ( tpMsg->resultcode() == 1 )
	{			
		// 一行两列数据
		if( tpMsg->rowcount() != 1 || tpMsg->colcount() != 1 )		
		{
			LOG_ERROR( "mail","[%s,%d] check mail Error!( roleid:%d, mailid:%d ) ",__FUNCTION__,__LINE__, nCharID, nMailID );
			return; 
		}
		int tCount = 0;
		if ( tpMsg->fieldvalue_size() > 0 )
		{
			tCount = atoi( tpMsg->fieldvalue( 0 ).c_str() );
		}		
		
		CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( nCharID );
		if ( tpPlayer == NULL )
		{
			return;
		}
		
		// 数据库中的邮件已经被退回
		if ( tCount <= 0 )
		{
			PayMoneyForMail( tpPlayer, nMailID, true );
		}
		
		// 数据库中的邮件还在，邮件还有效
		else
		{
			PayMoneyForMail( tpPlayer, nMailID, false );
		}
	}
	else
	{
		LOG_ERROR( "mail","[%s,%d] check mail Error!( roleid:%d, mailid:%d ) ",__FUNCTION__,__LINE__, nCharID, nMailID );				
	}
}

// ***********************************************************
//  Function:		PayMoneyForMail
//  Description:	为付费邮件付费
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::PayMoneyForMail( CEntityPlayer *pPlayer, int nMailID, bool bMailDelted )
{
	LK_ASSERT( pPlayer != NULL, return )	
	CMail *tpMail = CMailManager::GetSingletonPtr( )->GetMail( pPlayer );	
	LK_ASSERT( tpMail != NULL, return );		  
	
	CMessage tMsgHead;
	CMessagePayMailMoneyResponse tMsgPayMailMoney;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_PAYMAILMONEY ); 
	tMsgHead.set_msgpara( (int)&tMsgPayMailMoney );
	
	if ( bMailDelted )
	{
		tpMail->DelMail( nMailID );
		tMsgPayMailMoney.set_errcode( ERR_MAIL_PAYMONEYFORMAIL_INVALIDE );	 		
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );	
		
		CMessageDelMailResponse tMsgDelMail;
		CMessage				tMessageHead;
		tMsgDelMail.set_mailid( nMailID );
		tMessageHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_DELMAIL );
		tMessageHead.set_msgpara( (int)&tMsgDelMail );
		CSceneLogic::GetSingletonPtr()->Send2Player( pPlayer, &tMessageHead );		
		return;
	}			
	
	// 邮件有效可以付费
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();			
	int tMoney = tpProperty->GetMoney();
	int tExpiredTime = 0;
	int tRet = tpMail->PayMoneyForMail( nMailID, tMoney, tExpiredTime );
	if ( tRet == SUCCESS )
	{ 			
		CPropertyModule::GetSingleton().PlayerChangeMoney( pPlayer, tMoney, true, false );			
	} 
	tMsgPayMailMoney.set_errcode( tRet );
	tMsgPayMailMoney.set_expiredtime( tExpiredTime );
	tMsgPayMailMoney.set_mailid( nMailID );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );	
}

// ***********************************************************
//  Function:		InitializeSysMail
//  Description:	初始化系统邮件信息
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
int CMailModule::InitializeSysMail( const char *pFileName )
{
	LK_ASSERT( pFileName != NULL, return -1 )
	TiXmlDocument tDoc;
	if ( tDoc.LoadFile( pFileName ) == false )
	{
		return -1;
	}
	
	TiXmlElement *tpRoot = tDoc.FirstChildElement();
	LK_ASSERT( tpRoot != NULL, return -1 )
	TiXmlElement *tpMailElement = tpRoot->FirstChildElement( "SysMail" );
	for ( ; tpMailElement != NULL; tpMailElement = tpMailElement->NextSiblingElement() )
	{
		CSysMailInfo tMailInfo;
		tMailInfo.mSysMailID = atoi( tpMailElement->Attribute( "ID" ) );
		strncpy( tMailInfo.mMailTitle, tpMailElement->Attribute( "Title" ), sizeof( tMailInfo.mMailTitle ) - 1 );
		strncpy( tMailInfo.mName, tpMailElement->Attribute( "Name" ), sizeof( tMailInfo.mName ) - 1 );
		strncpy( tMailInfo.mMailBody, tpMailElement->Attribute( "Body" ), sizeof( tMailInfo.mMailBody ) - 1 );
		CMailManager::GetSingleton().InsertSysMail( tMailInfo.mSysMailID, tMailInfo );
	}
	
	return SUCCESS;
}

// ***********************************************************
//  Function:		ChangeCampMail
//  Description:	邮件转阵营
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailModule::ChangeCampMail( CEntityPlayer *pPlayer,int nOldCampID, int nNewCampID )
{
	LK_ASSERT( pPlayer != NULL, return )
	//CMail *tpMail = CMailManager::GetSingletonPtr( )->GetMail( pPlayer );	
	//LK_ASSERT( tpMail != NULL, return )	
	//tpMail->DestroyMailCache();	
	//CMailDBOperate::ChangeRoleCampMail( pPlayer->GetCharID(), nOldCampID, nNewCampID );
}


void CMailModule::OnSendSysMailToPlayer( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );

	CMessageSendSysMailToPlayer *tpMsg = ( CMessageSendSysMailToPlayer * )pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );

	LOG_DEBUG( "mail", "[%s] handle begine! "
            "receiver_name(%s) receiver_id(%d) sender_name(%s) "
            "mail_title(%s) mail_body(%s) receiver_countryid(%d)", 
            __FUNCTION__, 
            tpMsg->role_name().c_str(), tpMsg->role_id(), tpMsg->sender_name().c_str(), 
            tpMsg->title().c_str(), tpMsg->body().c_str(), tpMsg->receive_countryid() );


	std::string lstrDesName = tpMsg->role_name();
	int			liDesCharid	= tpMsg->role_id();
	std::string lstrTitle	= tpMsg->title();
	std::string	lstrSrcName	= tpMsg->sender_name();
	std::string lstrBody	= tpMsg->body();
	int			liCountry	= tpMsg->receive_countryid();
	CItemObject* lpItemObject = NULL;
	int tItemID[MAIL_ITEM_COUNT_MAX] = { 0 };
	int tItemNum[MAIL_ITEM_COUNT_MAX] = { 0 };
	int tAccessoryNum = 0;
	for ( ;  tAccessoryNum < tpMsg->itemid_size() && tAccessoryNum < tpMsg->itemnum_size() && tAccessoryNum < MAIL_ITEM_COUNT_MAX ; ++tAccessoryNum ) 
	{
		tItemID[tAccessoryNum] = tpMsg->itemid( tAccessoryNum );
		tItemNum[tAccessoryNum]	= tpMsg->itemnum( tAccessoryNum );
	}
										  
	if ( tpMsg->mailsendtype() == MAILSENDTYPE_SYS )
	{
		int liRetForSendSysMail = SendSysMail(	lstrDesName.c_str(),
			liDesCharid,
			lstrTitle.c_str(),
			lstrSrcName.c_str(),
			lstrBody.c_str(),
			&lpItemObject,
			0,
			liCountry,
			0 );
		if ( liRetForSendSysMail != 0 )
		{
			LOG_ERROR( "mail", "[%s] Send sys mail to player failed! "
				"lstrDesName(%s) liDesCharid(%d) lstrTitle(%s) lstrSrcName(%s), liCountry(%d)",
				__FUNCTION__,
				lstrDesName.c_str(),
				liDesCharid,
				lstrTitle.c_str(),
				lstrSrcName.c_str(),
				liCountry );
		}
	}
	else if ( tpMsg->mailsendtype() == MAILSENDTYPE_BYID )
	{
		int tSysMailID = atoi( lstrSrcName.c_str() );		
		SendSysMailBySysID( tSysMailID, lstrDesName.c_str(), liDesCharid, tItemID, tItemNum, tAccessoryNum, liCountry );
	}
	return;
}

void CMailModule::OnGateMessage( CMessage *pMsg )
{  
	LK_ASSERT( pMsg != NULL, return );

	// 处理公共消息
	switch ( pMsg->mutable_msghead()->messageid() )
	{
	case ID_G2S_RESPONSE_GETMAIINFO:
		{
			CheckRoleNameInfo( pMsg );
			break ;
		}
	case ID_G2S_NOTIFY_SENDSYSMAILTOPLAYER:
		{
			OnSendSysMailToPlayer( pMsg );
			break;
		}
	default:
		break;
	}
}

void CMailModule::LoadSysMailConfigCallback(const char *pcConfigPath)
{
	CMailModule::GetSingleton().InitializeSysMail(pcConfigPath);
}

// 发送离线留言(只能给好友留言)
void CMailModule::SendOffLineChatMsg( CEntityPlayer *pPlayer, int nDesCharID, const char *pMsg )
{
	LK_ASSERT( pPlayer != NULL, return )
	CFriendItem *tpItem = CFriendModule::GetSingleton().GetPlayerFriendItem( pPlayer->GetCharID() );
	if ( tpItem == NULL )
	{
		return;
	}
	/*if ( tpItem->GetFriendByRoleID( nDesCharID ) == NULL )
	{
		return;
	}*/
	CMailDBOperate::SaveOffLineChatMsg( pPlayer->GetCharID(), pPlayer->GetCharName(), nDesCharID, pMsg, pPlayer->GetNationality() );
}


// 获取离线留言
void CMailModule::OnSessionLoadOffLineChatMsg( CMessage *pMsg, int nParam1, int nParam2 )
{	
	LK_ASSERT( pMsg != NULL, return )	
	CEntityPlayer *tpPlayer =CSceneLogic::GetSingleton().GetPlayerByCharID( nParam1 );
	if ( tpPlayer == NULL )
	{
		return;
	}
	CMessageExecuteSqlResponse3 *tpMsg = ( CMessageExecuteSqlResponse3 * )pMsg->msgpara();
	if ( tpMsg == NULL )
	{		
		LOG_ERROR( "mail","load chatmsg error, roleid:%d", nParam1 );
		return ;
	}
	if ( tpMsg->resultcode() != 1 )
	{
		LOG_ERROR( "mail","load chatmsg error, roleid:%d", nParam1 );
		return ;		
	}

	int tRowNum = tpMsg->rowcount();
	int	tColumn	= tpMsg->colcount();	

	if ( tColumn != 5 )
	{
		LOG_ERROR( "mail","load chatmsg error, roleid:%d", nParam1 );
		return ;		
	}	
	for ( int i = tRowNum - 1; i >= 0; --i )
	{			
		const char *tpSendName = tpMsg->mutable_fieldvalue( 0+i*tColumn )->c_str();
		int tSendTime = atoi( tpMsg->mutable_fieldvalue( 1+i*tColumn )->c_str() );
		int tSendRoleID = atoi( tpMsg->mutable_fieldvalue( 2+i*tColumn )->c_str() );
		const char *pBody = tpMsg->mutable_fieldvalue( 3+i*tColumn )->c_str();
		int tCampID = atoi( tpMsg->mutable_fieldvalue( 4+i*tColumn )->c_str() );
		CMessage tMessage;
		CMessageFriendMessageNotice msg;
		tMessage.mutable_msghead()->set_messageid(ID_S2C_FRIEND_MESSAGE_NOTICE_R);
		tMessage.set_msgpara( (int)&msg );
		msg.set_charid( tSendRoleID );
		if ( tpSendName != NULL )
		{
			msg.set_name( tpSendName );
		}			
		if ( pBody != NULL )
		{
			msg.set_msg( pBody );
		}					
		msg.set_sendtime( tSendTime );
		msg.set_campid( tCampID );
		CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMessage );
	}	
	CMailDBOperate::DeleteOffLineChatMsg( tpPlayer->GetCharID(), tpPlayer->GetNationality() );
}
