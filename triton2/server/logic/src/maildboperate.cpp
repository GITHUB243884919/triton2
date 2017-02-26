#include "westserver.h"
#include "mailmessage_pb.hxx.pb.h"
#include "mail.h"
#include "mailmanager.h"
#include "scenelogic.h"

CMailDBOperate gpMailDBoperate;
static char MailTableName[][MAIL_TABLENAME_LENGTH] =
{
	"UMS_ROLE_MAIL_00",
	"UMS_ROLE_MAIL_01",
	"UMS_ROLE_MAIL_02",
	"UMS_ROLE_MAIL_03",
	"UMS_ROLE_MAIL_04",
	"UMS_ROLE_MAIL_05" 
};

// ***********************************************************
//  Function:		GetMailTableName	
//  Description:	获取玩家所在的邮件表名
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
const char * CMailDBOperate::GetMailTableName( int CharID, int CountryID )
{
	//int MailTableId = CountryID % MAXMAILTABLENUM;
	int MailTableId = 1; 
	return MailTableName[MailTableId];
}

// ***********************************************************
//  Function:	 GetMailBoxStatus   
//  Description: 获取玩家的邮件数量
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
int CMailDBOperate::GetMailBoxStatus( int CharID, int CountryID )
{	
	const char *TableName = GetMailTableName( CharID, CountryID );
	LK_ASSERT( TableName != NULL, return -1 );
	int tTimeNow = time( NULL );
	CSceneLogic::GetSingletonPtr()->ExecuteSql( getmailcountrece,CharID, 0, CharID,SQL_NEED_CALLBACK,SELECT," select count(*) from %s where recver_role_id=%d and expired_time > %d and mail_type&8=0", TableName, CharID, tTimeNow );
	CSceneLogic::GetSingletonPtr()->ExecuteSql( checknewmail,CharID, 0, CharID,SQL_NEED_CALLBACK,SELECT," select count(*) from %s where recver_role_id=%d and mail_status&1!=0 and expired_time  > %d  and mail_type&8=0 ", TableName, CharID, tTimeNow );
	return 0;
}

// ***********************************************************
//  Function:		GetMailDetailInfo
//  Description:	获取邮件的详细信息 
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
int CMailDBOperate::GetMailDetailInfo( int MailID, int CharID, int CountryID )
{
	const char *TableName = GetMailTableName( CharID, CountryID );
	LK_ASSERT( TableName != NULL, return -1 );
	CSceneLogic::GetSingletonPtr()->ExecuteSql( getmaildetailrece, CharID, MailID ,CharID,SQL_NEED_CALLBACK,SELECT," select mail_body , item_attachment  from %s where mail_id=%d ", TableName, MailID );		
	return 0;
}
  
// ***********************************************************
//  Function:	 MailSend 
//  Description: 发送邮件
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
int CMailDBOperate::MailSend( int CharID, int MailCaCheID, const MailInfo *pMailInfo, bool IfNeedCallBack /* = false */, int CountryID /* = 1 */, int nWorldCharID /* = 0 */ )
{
	int DesID = CharID;	   
	const char *TableName = GetMailTableName( CharID, CountryID );
	LK_ASSERT( TableName != NULL, return -1 );	
	int SqlNeedCallback = SQL_NEED_CALLBACK|SQL_LATEST_INC_ID; 
	if ( IfNeedCallBack == false )
	{
		SqlNeedCallback = SQL_NONE;
		DesID = 0;
	}	
	char tMailTitleEscapeBuff[MAIL_TITLE_LEN] = { 0 };
	char tMailBodyEscapeBuff[MAIL_BODY_LEN]	 = { 0 };
	char *pTitlePtr = tMailTitleEscapeBuff;
	char *pBodyPtr = tMailBodyEscapeBuff;
	int tMailCharID = CharID;
	if ( nWorldCharID > 0 )
	{
		tMailCharID = nWorldCharID;
	}
	int tMailTileRet = EscapeDataString( &pTitlePtr, MAIL_TITLE_LEN , ( char * )pMailInfo->mMailBase.mTitle, strlen( pMailInfo->mMailBase.mTitle ) );
	if ( tMailTileRet ==-1 )
	{ 		
		LOG_ERROR( "mail","[%s,%d][%s] EscapeDataString Error Character:%d !  ",__LK_FILE__,__LINE__,__FUNCTION__, CharID );
		return -1;
	}
	
	int tMailBodyRet = EscapeDataString( &pBodyPtr, MAIL_BODY_LEN , ( char * )pMailInfo->mBody, strlen(  pMailInfo->mBody ) );
	if ( tMailBodyRet ==-1 )
	{ 		
		LOG_ERROR( "mail","[%s,%d][%s] EscapeDataString Error Character:%d !  ",__LK_FILE__,__LINE__,__FUNCTION__, CharID );
		return -1;
	}
	CSceneLogic::GetSingletonPtr( )->ExecuteSql( dbmsgrece, DesID, MailCaCheID,tMailCharID, SqlNeedCallback,INSERT, "insert into %s( recver_role_id, sender_role_name,mail_title,mail_type,mail_status,mail_body,item_attachment,send_time,expired_time, money_attachment,post_recver, sender_role_id,item_number, recver_role_name ) values( %d, '%s', '%s', %d, %d, '%s', '%s', %d, %d,%d, %d, %d, %d, '%s' ) ;",TableName,
		CharID,pMailInfo->mMailBase.mSrc, tMailTitleEscapeBuff, pMailInfo->mMailBase.mLevel, 
		pMailInfo->mMailBase.mType, tMailBodyEscapeBuff, pMailInfo->mExtDate, pMailInfo->mMailBase.mSendTime, 
		pMailInfo->mMailBase.mExpiredTime, pMailInfo->mMoneySend, pMailInfo->mMoneyNeedBack, 
		pMailInfo->mMailBase.mSrcId, pMailInfo->mMailBase.mItemNum, pMailInfo->mMailBase.mDesName );	
	return 0;
}

// ***********************************************************
//  Function:	 MailDel 
//  Description: 删除邮件
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
int CMailDBOperate::MailDel( int vMailID, int CharID /* = 0 */, int CountryID )
{
	const char *TableName = GetMailTableName( CharID, CountryID );
	LK_ASSERT( TableName != NULL, return -1 );	
	CSceneLogic::GetSingletonPtr()->ExecuteSql( mailrecenothing, CharID, vMailID, CharID, SQL_NEED_CALLBACK ,DELETE, "DELETE FROM %s WHERE mail_id=%d", TableName, vMailID );
	return 0;
}



// ***********************************************************
//  Function:	 UpMailAccessory 
//  Description: 更新邮件，带附件信息
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
int CMailDBOperate::UpMailAccessory( int vMailID, const MailInfo *pMailInfo, int CharID /* = 0 */, int CountryID /* = 1 */ )
{	
	const char *TableName = GetMailTableName( CharID, CountryID );
	LK_ASSERT( TableName != NULL, return -1 ); 	
	CSceneLogic::GetSingletonPtr()->ExecuteSql( mailrecenothing , 0, 0, CharID, SQL_NONE,UPDATE ,"update %s set item_attachment ='%s', money_attachment=%d , post_recver=%d, mail_type=%d, mail_status=%d, expired_time=%d, item_number=%d where mail_id=%d", TableName, pMailInfo->mExtDate,pMailInfo->mMoneySend,pMailInfo->mMoneyNeedBack,pMailInfo->mMailBase.mLevel,pMailInfo->mMailBase.mType, 
		pMailInfo->mMailBase.mExpiredTime , pMailInfo->mMailBase.mItemNum,vMailID );
	return 0;
}


// ***********************************************************
//  Function:	 GetMailFormDB 
//  Description: 读取邮件
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
int CMailDBOperate::GetMailFormDB( int ReceKind, int CharID , int MailCount , int LastMailID, int CountryID )
{
	const char *TableName = GetMailTableName( CharID, CountryID );
	LK_ASSERT( TableName != NULL, return -1 );	
	int tTimeNow = time(NULL);
	CSceneLogic::GetSingletonPtr( )->ExecuteSql( dbmailrece, ReceKind, CharID, CharID, SQL_NEED_CALLBACK,SELECT,
		"Select mail_id,mail_status,mail_type,sender_role_name,mail_title,expired_time,send_time,money_attachment,post_recver, sender_role_id, item_number  from %s where recver_role_id=%d and mail_id> %d and expired_time > %d and mail_type&8=0 limit %d", TableName, CharID, LastMailID, tTimeNow, MailCount );
	return 0;
}


// ***********************************************************
//  Function:	 UpMailStatus 
//  Description: 更新邮件信息，不带附件
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
int CMailDBOperate::UpMailStatus( int vMailID,const MailInfo *pMailInfo, int CharID /* = 0 */, int CountryID /* = 2 */ )
{
    LK_ASSERT( pMailInfo != NULL, return -1 );
	const char *TableName = GetMailTableName( CharID, CountryID );
	LK_ASSERT( TableName != NULL, return -1 );	
	CSceneLogic::GetSingletonPtr()->ExecuteSql( mailrecenothing, 0, 0, CharID, SQL_NONE,UPDATE,"UPDATE %s SET mail_status=%d, mail_type=%d, money_attachment=%d, post_recver=%d, expired_time=%d WHERE mail_id = %d", 
		TableName, pMailInfo->mMailBase.mType , pMailInfo->mMailBase.mLevel, pMailInfo->mMoneySend, pMailInfo->mMoneyNeedBack, pMailInfo->mMailBase.mExpiredTime, vMailID );
	return 0;
}


// ***********************************************************
//  Function:	 MailReceNothing 
//  Description: 不需要特殊处理的回调函数
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailDBOperate::MailReceNothing( CMessage *pMessage, int vParameter1, int vParameter2 )
{
	CEntityPlayer* pPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID( vParameter1 );
	if( pPlayer == NULL )
	{
		LOG_ERROR( "default", "[%s:%d] player %d delete mail %d return , but player is logout", 
			__FUNCTION__, __LINE__, vParameter1, vParameter2 );
		return ;
	}

	CMail* pMail = (CMail*) CSceneObjManager::GetSingletonPtr()->GetObject( pPlayer->GetMailObjID() );
	LK_ASSERT( pMail != NULL, return  );

	CMessageExecuteSqlResponse3* pResMsg = (CMessageExecuteSqlResponse3*)pMessage;
	if( pResMsg->resultcode() == 0 ) // 0表示失败
	{
		LOG_ERROR( "mail", "[%s:%d] player %d delete mail %d failed", __FUNCTION__, __LINE__, vParameter1, vParameter2 );
	}

	pMail->SetActionMailID( 0 );

}

// 删除邮件超时
void CMailDBOperate::MailDelTimeout( int nParam1, int nParam2  )
{
	CEntityPlayer* pPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID( nParam1 );
	if( pPlayer == NULL )
	{
		LOG_ERROR( "default", "[%s:%d] player %d delete mail %d timeout, and logout", __FUNCTION__, __LINE__, nParam1, nParam2 );
		return ;
	}

	CMail* pMail = (CMail*)CSceneObjManager::GetSingletonPtr()->GetObject( pPlayer->GetMailObjID() );
	LK_ASSERT( pMail != NULL, return  );


	LOG_ERROR( "mail", "[%s:%d] player %d delete mail %d timeout", __FUNCTION__, __LINE__, nParam1, nParam2 );


	pMail->SetActionMailID( 0 );
}


// ***********************************************************
//  Function:	 DBMailRece 
//  Description: 读取邮件的回调函数
//  Input:							
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailDBOperate::DBMailRece( CMessage * pMessage, int Parameter1, int Parameter2 )
{
	if( pMessage == NULL )
	{
		return;
	}
	CMessageExecuteSqlResponse3 *tpMsg = ( CMessageExecuteSqlResponse3 * )pMessage->msgpara();
	if( tpMsg == NULL )
	{
		return;
	}
	if( tpMsg->resultcode() == 0 )
	{
		// 执行数据库错误
		LOG_ERROR( "mail","[ %s : %d ][ %s ]ExecuteSql Error!", __LK_FILE__, __LINE__, __FUNCTION__  );
		return;
	}
	CEntityPlayer *pPlayer = ( CEntityPlayer * )CSceneLogic::GetSingleton().GetPlayerByCharID( Parameter2 );	
	if( pPlayer == NULL )
	{
		// 获取玩家邮件信息错误
		LOG_ERROR( "mail","[ %s : %d ][ %s ] Player Of Charid: %d has been off line when mail recieved from db ! ", __LK_FILE__, __LINE__, __FUNCTION__,Parameter2 );
		return;
	}
	CMail *tpMail = CMailManager::GetSingleton().GetMail( pPlayer );
	if ( tpMail == NULL )
	{
		// 获取玩家邮件信息错误
		LOG_ERROR( "mail","[ %s : %d ][ %s ] Get CMail Of Charid: %d Error! ", __LK_FILE__, __LINE__, __FUNCTION__,Parameter2 );
		return;
	}		
	int RowCount    = tpMsg->rowcount();
	int ColumnCount = tpMsg->colcount();
	if (  ColumnCount != 11 || RowCount == 0 )
	{
		// 数据库里面没有新邮件
		LOG_DEBUG( "mail","[ %s : %d ][ %s ] Player Of Charid: %d has no new mail when get mail from database! ", 
			__LK_FILE__, __LINE__, __FUNCTION__,Parameter2 );		
		return;
	}
	
	int i = 0;
	MailInfo    MailDbInfor;
	CMessageMailListNotify  MsgMailList;
	for(  i=0; i<RowCount; i++ )
	{			
		if ( tpMsg->fieldvaluelen( i*ColumnCount) > 0 )
		{
		  MailDbInfor.mMailBase.mMailID   = atoi( tpMsg->fieldvalue( i*ColumnCount ).c_str() );
		}
		if ( tpMsg->fieldvaluelen( i*ColumnCount+1 ) > 0 )
		{
			MailDbInfor.mMailBase.mType     = atoi( tpMsg->fieldvalue( i*ColumnCount + 1 ).c_str() );
		}
		
		if ( tpMsg->fieldvaluelen( i*ColumnCount+2 ) > 0 )
		{
			MailDbInfor.mMailBase.mLevel    = atoi( tpMsg->fieldvalue( i*ColumnCount + 2 ).c_str() );
			
		}
		if ( tpMsg->fieldvaluelen( i*ColumnCount+3 ) > 0  )
		{
			 MailDbInfor.mMailBase.mSrc = tpMsg->fieldvalue( i*ColumnCount + 3 ).c_str();			
		}
		
		if ( tpMsg->fieldvaluelen( i*ColumnCount+4 ) > 0  )
		{
			MailDbInfor.mMailBase.mTitle = tpMsg->fieldvalue( i*ColumnCount + 4 ).c_str( );		
		}
		if ( tpMsg->fieldvaluelen( i*ColumnCount+5 ) > 0  )
		{
			MailDbInfor.mMailBase.mExpiredTime = atoi(tpMsg->fieldvalue( i*ColumnCount + 5 ).c_str());			

		}
		if ( tpMsg->fieldvaluelen( i*ColumnCount+6 ) > 0  )
		{
			MailDbInfor.mMailBase.mSendTime = atoi(tpMsg->fieldvalue( i*ColumnCount + 6 ).c_str());	
		}
		
		if ( tpMsg->fieldvaluelen( i*ColumnCount+7 ) > 0  )
		{
			MailDbInfor.mMoneySend     = atoi(tpMsg->fieldvalue( i*ColumnCount + 7  ).c_str());			
		}	
		
		if ( tpMsg->fieldvaluelen( i*ColumnCount+8 ) > 0 )
		{ 
		   MailDbInfor.mMoneyNeedBack = atoi( tpMsg->fieldvalue( i*ColumnCount + 8 ).c_str() );
		}	
		if ( tpMsg->fieldvaluelen( i*ColumnCount+9 ) > 0 )
		{ 
			MailDbInfor.mMailBase.mSrcId = atoi( tpMsg->fieldvalue( i*ColumnCount + 9 ).c_str() );
		}		
		if ( tpMsg->fieldvaluelen( i*ColumnCount+10 ) > 0 )
		{
			MailDbInfor.mMailBase.mItemNum = atoi(tpMsg->fieldvalue( i*ColumnCount + 10 ).c_str() );
		}			
		CMail::RecMailFromDBCallBack( pPlayer, tpMail, &MailDbInfor, false,  MsgMailList, Parameter1);			
		MailDbInfor.ClearMailInfor( );
	}	
	if ( Parameter1 == FETCHMAIL_PAGE )
	{
		CMail::RecMailFromDBCallBack( pPlayer, tpMail, NULL, true, MsgMailList );
	}
}



// ***********************************************************
//  Function:	 DbMsgRece 
//  Description: 发送邮件的回调函数
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailDBOperate::DbMsgRece( CMessage * pMessage, int vParameter1, int vParameter2 )
{

	if ( pMessage == NULL )
	{
		return;
	}
	CMessageExecuteSqlResponse3 *tpResponseMsg =( CMessageExecuteSqlResponse3 * )pMessage->msgpara(  );
	if( tpResponseMsg == NULL )
	{
		return;
	}
	
	if( tpResponseMsg->resultcode( ) == 1 )
	{
		int MailID = tpResponseMsg->latestautoincid();	
		CMail::SendMailCallBack(  MailID, vParameter2, vParameter1 );	
	}
	else
	{
		CMail::SendMailCallBack(  -1, vParameter2, vParameter1 );
	}	
}



// ***********************************************************
//  Function:	 GetMailCountRece 
//  Description: 从数据库获取玩家邮件数量的回调函数
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailDBOperate::GetMailCountRece( CMessage * pMessage, int Parameter1, int Parameter2  )
{
	if ( pMessage == NULL )
	{
		return;
	}
	CMessageExecuteSqlResponse3 *tpResponseMsg = ( CMessageExecuteSqlResponse3 * )pMessage->msgpara();
	if ( tpResponseMsg == NULL )
	{
		return;
	}
	if( tpResponseMsg->rowcount() == 0 || tpResponseMsg->colcount( ) == 0 )
	{
		return;
	}
	
	LK_ASSERT( tpResponseMsg->resultcode( ) == 1, return );
	
	if ( tpResponseMsg->fieldvalue( 0 ).length( ) >= 1 )
	{
		int  MailCount = atoi( tpResponseMsg->fieldvalue( 0 ).c_str( ) );
		CMail::GetMailCountCallBack( Parameter1, MailCount );		
	}		
}

// ***********************************************************
//  Function:	 CheckNewMailRece 
//  Description: 从数据库获取玩家新邮件数量的回调函数
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 2009-04-15
// **********************************************************
void CMailDBOperate::CheckNewMailRece( CMessage * pMessage, int Parameter1, int Parameter2 )
{
	LK_ASSERT( pMessage != NULL, return );
	if ( pMessage == NULL )
	{
		return;
	}
	CMessageExecuteSqlResponse3 *tpResponseMsg = ( CMessageExecuteSqlResponse3 * )pMessage->msgpara();
	if ( tpResponseMsg == NULL )
	{
		return;
	}
	if( tpResponseMsg->rowcount() == 0 || tpResponseMsg->colcount( ) == 0 )
	{
		return;
	}
	LK_ASSERT( tpResponseMsg->resultcode( ) == 1, return );
	if ( tpResponseMsg->fieldvalue( 0 ).length( ) >= 1 )
	{
		int  MailCount = atoi( tpResponseMsg->fieldvalue( 0 ).c_str( ) );
		CMail::CheckNewMailCallBack( Parameter1, MailCount  );		
	}	
}

// ***********************************************************
//  Function:	 GetMailDetailRece 
//  Description: 获取邮件详细信息的回调函数
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailDBOperate::GetMailDetailRece( CMessage * pMessage, int Parameter1, int Parameter2 )
{
	if ( pMessage == NULL )
	{
		return;
	}
	
	CMessageExecuteSqlResponse3 *tpResponseMsg = ( CMessageExecuteSqlResponse3 * )pMessage->msgpara();
		
	if ( tpResponseMsg == NULL )
	{
		return;
	}
	
	// 一行两列数据, 如果是空值的话删掉邮件
	MailInfo MailDetailInfo;
	if( tpResponseMsg->rowcount() != 1 || tpResponseMsg->colcount() != 2 )		
	{
		CMail::GetMailDetailInfoCallBack( Parameter1 , Parameter2 , MailDetailInfo, false );
		return;
	}
	
	MailDetailInfo.mBody = NULL;
	//MailDetailInfo.mExtDate = NULL;
	if ( tpResponseMsg->fieldvaluelen( 0 ) > 0  )
	{
		MailDetailInfo.mBody = tpResponseMsg->fieldvalue( 0 ).c_str(  );
	}
	if ( tpResponseMsg->fieldvaluelen( 1 ) > 0 )
	{
		memcpy( MailDetailInfo.mExtDate, tpResponseMsg->fieldvalue( 1 ).c_str( ), tpResponseMsg->fieldvaluelen( 1 ) );	
		MailDetailInfo.mExtLen = tpResponseMsg->fieldvaluelen( 1 );
	}
	CMail::GetMailDetailInfoCallBack( Parameter1 , Parameter2 , MailDetailInfo );
}

// ***********************************************************
//  Function:	 CheckIfMailInDB 
//  Description: 检查数据库是否还有某个邮件记录
//  Input:		
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
void CMailDBOperate::CheckIfMailInDB( int nCharID, int nContryID, int nMailID )
{		
	const char *TableName = GetMailTableName( nCharID, nContryID );
	LK_ASSERT( TableName != NULL, return  );	
	CSceneLogic::GetSingletonPtr()->ExecuteSql( checkmailindb, nCharID, nMailID, nCharID, SQL_NEED_CALLBACK,SELECT," select count(*) from %s  WHERE mail_id = %d", 
		TableName, nMailID );
}

// ***********************************************************
//  Function:	 ChangeRoleCampMail 
//  Description: 转阵营时的邮件操作
//  Input:		
// **********************************************************
void CMailDBOperate::ChangeRoleCampMail( int nRoleID, int nOldCampID, int nNewCampID )
{
	const char *tpOldTableName	= GetMailTableName( nRoleID, nOldCampID );
	const char *tpNewTableName	= GetMailTableName( nRoleID, nNewCampID );
	const char *tpSql = "insert into %s( mail_id,mail_type,mail_status,sender_role_id,sender_role_name,recver_role_id, \
						mail_title,mail_body,send_time,money_attachment,item_number,item_attachment,post_recver,expired_time,recver_role_name \
						 )  select 0,mail_type,mail_status,sender_role_id,sender_role_name,recver_role_id, \
	mail_title,mail_body,send_time,money_attachment,item_number,item_attachment,post_recver,expired_time,recver_role_name \
	from %s where recver_role_id=%d;" ;		
	CSceneLogic::GetSingletonPtr()->ExecuteSql( mail_changecamp, nRoleID, 0, nRoleID, SQL_NONE,INSERT, tpSql, 
		tpNewTableName, tpOldTableName, nRoleID );
	CSceneLogic::GetSingletonPtr()->ExecuteSql( mail_changecamp, nRoleID, 0, nRoleID, SQL_NONE, DELETE, "delete from %s where recver_role_id=%d" , 
		tpOldTableName, nRoleID );			
}

// ***********************************************************
//  Function:	 SaveOffLineChatMsg 
//  Description: 好友离线留言
//  Input:		
// **********************************************************
void CMailDBOperate::SaveOffLineChatMsg( int nSrcRoleID, const char *pSrcName, int nDesRoleID, const char *pMsg, int nCountryID )
{
	LK_ASSERT( pSrcName != NULL && pMsg != NULL, return )
	const char *tpTable = GetMailTableName( nSrcRoleID, nCountryID );
	if ( tpTable == NULL )
	{
		return;
	}
	char tMailBodyEscapeBuff[MAIL_BODY_LEN]	 = { 0 };	
	char *pBodyPtr = tMailBodyEscapeBuff;		
	int tMailBodyRet = EscapeDataString( &pBodyPtr, MAIL_BODY_LEN-1, ( char * )pMsg, strlen( pMsg ) );
	if ( tMailBodyRet ==-1 )
	{ 		
		LOG_ERROR( "mail","[%s,%d][%s] EscapeDataString Error Character:%d !  ",__LK_FILE__,__LINE__,__FUNCTION__, nSrcRoleID );
		return;
	}
	int tMailType = 0;
	MAIL_SETLEVEL_USER( tMailType );
	MAIL_SETLEVEL_CHATMSG( tMailType );
	int tMailStatus = 0;
	MAIL_SETTYPE_NEW( tMailStatus, 1 )
	MAIL_SETTYPE_HASBODY( tMailStatus, 1 )		
	const char *tpSql = "insert into %s( recver_role_id, sender_role_name, mail_type,mail_status,mail_body, send_time, expired_time, sender_role_id, mail_title ) values( %d, '%s',  %d, %d, '%s', %d, %d, %d, '%d' ) ";
	CSceneLogic::GetSingletonPtr( )->ExecuteSql( em_dbsession_savechatmsg, nSrcRoleID, nSrcRoleID, nSrcRoleID, SQL_NONE, INSERT, tpSql, 
		tpTable, nDesRoleID, pSrcName, tMailType, tMailStatus, pBodyPtr, time(NULL), time(NULL)+MAIL_TIME_COMMON, nSrcRoleID, nCountryID );
}

// ***********************************************************
//  Function:	 LoadOffLineChatMsg 
//  Description: 获取离线留言
//  Input:		
// **********************************************************
void CMailDBOperate::LoadOffLineChatMsg( int nRoleID, int nCountryID, int nLimitCount /* = 30 */ )
{
	const char *TableName = GetMailTableName( nRoleID, nCountryID );
	LK_ASSERT( TableName != NULL, return );	
	int tTimeNow = time(NULL);
	CSceneLogic::GetSingletonPtr( )->ExecuteSql( em_dbsession_loadchatmsg, nRoleID, nRoleID, nRoleID, SQL_NEED_CALLBACK,SELECT,
		"Select sender_role_name, send_time, sender_role_id, mail_body, mail_title from %s where recver_role_id=%d and expired_time > %d and mail_type&8!=0 order by send_time desc limit %d", TableName, nRoleID, tTimeNow, nLimitCount );	
}

// ***********************************************************
//  Function:	 LoadOffLineChatMsg 
//  Description: 删除所有留言
//  Input:		
// **********************************************************
void CMailDBOperate::DeleteOffLineChatMsg( int nRoleID, int nCountryID )
{
	const char *TableName = GetMailTableName( nRoleID, nCountryID );
	LK_ASSERT( TableName != NULL, return );	
	CSceneLogic::GetSingletonPtr( )->ExecuteSql( em_dbsession_deletechatmsg, nRoleID, nRoleID, nRoleID, SQL_NONE,DELETE,
		"delete from %s where recver_role_id=%d and mail_type&8!=0", TableName, nRoleID );	
}