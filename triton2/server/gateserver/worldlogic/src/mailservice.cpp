#include "mailservice.h"
#include "gateobj_manager.h"
#include "gatelogic.h"
#include "servermessage_in.h"
#include "nameservice.h"
#include "core_service.h"

using namespace GateServer;

template<> CServiceGateMail* CSingleton< CServiceGateMail >::spSingleton = NULL;

void CServiceGateMail::OnLaunchServer()
{

}

void CServiceGateMail::OnExitServer()
{
	
}

void CServiceGateMail::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{

}

void CServiceGateMail::OnMessage( int nServerID, CMessage* tpMessage )
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);	

	CMessageHead* tpHead = tpMessage->mutable_msghead();
	unsigned short nMessageID = (unsigned short) tpHead->messageid();

	switch (nMessageID)
	{	
		case ID_C2G_REQUEST_GETMAILINFO:
		{
			OnMessageGetMailInfoRequest(nServerID,  tpMessage);
			break;
		}
        case ID_G2S_NOTIFY_SENDSYSMAILTOPLAYER:
        {
            OnMessageSendSysMail( tpMessage );
            break;
        }

		default:
		{
			LOG_ERROR("default", "[%s:%d]there is no action for the message_id(%d)",
				__FUNCTION__, __LINE__, nMessageID);
			return;
		}
	}
}

unsigned int CServiceGateMail::CountSize()
{
	return sizeof(CServiceGateMail);
}


CServiceGateMail::CServiceGateMail()
{
	if( mShmPtr->GetInitMode() == SHM_INIT )		
	{
		Initialize();
	}else
	{
		Resume();
	}
}

CServiceGateMail::~CServiceGateMail()
{

}

int CServiceGateMail::Initialize()
{
	return 0;
}

int CServiceGateMail::Resume()
{
	return 0;
}

void* CServiceGateMail::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CServiceGateMail::operator delete( void* pointer )
{

}

void CServiceGateMail::OnMessageGetMailInfoRequest(int vServerID, CMessage* tpMessage )
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);	

	CMessage  tMessage;
	CMessageGetPlayerMailInfoResponse tMsgMailInfo;
	CMessageGetPlayerMailiInfoRequest* tpMsg = (CMessageGetPlayerMailiInfoRequest*) tpMessage->msgpara();

	CRoleGlobalInfo* pRoleInfo = NULL;
	CGatePlayer* tpPlayer = NULL;

	if ( tpMsg->queryway() == 1 )
	{
		pRoleInfo = CServiceRoleIDName::GetSingletonPtr()->GetRoleInfoByRoleID( tpMsg->roleid( ) );
	}else if( tpMsg->queryway() == 0  )
	{
		if ( !tpMsg->has_rolename()  )
		{
			return;
		}
		pRoleInfo = CServiceRoleIDName::GetSingletonPtr()->GetRoleInfoByRoleName( tpMsg->rolename().c_str());
	}

	tMsgMailInfo.set_charid( tpMsg->charid() );
	if ( pRoleInfo == NULL )
	{
		tMsgMailInfo.set_errorcode( 0 );
	}else
	{
		tMsgMailInfo.set_errorcode( 1 );
		tMsgMailInfo.set_roleid( pRoleInfo->RoleID( )  );
		tMsgMailInfo.set_rolename( pRoleInfo->RoleName( ) );
		tMsgMailInfo.set_roleofcountry( pRoleInfo->CountryID() );

		tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( pRoleInfo->RoleID() );
		if ( tpPlayer != NULL )
		{
			tMsgMailInfo.set_roleonline( 1 );
			tMsgMailInfo.set_roleincountry( tpPlayer->mLineID );
		}else
		{
			tMsgMailInfo.set_roleonline( 0 );
		}
	}
	tMsgMailInfo.set_querytype( tpMsg->querytype() );	
	for ( unsigned int i = 0; i < tpMsg->querydata_size(); ++i )
	{
		tMsgMailInfo.add_querydata( tpMsg->querydata( i ) );
	}
	
	pbmsg_sethead(&tMessage, ID_G2S_RESPONSE_GETMAIINFO, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tMsgMailInfo);
		    
	CGateServer::GetSingleton().Send2Scene( vServerID, &tMessage );
}

void CServiceGateMail::OnMessageSendSysMail( CMessage* tpMessage )
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);	

	CMessageSendSysMailToPlayer* tpMsg = (CMessageSendSysMailToPlayer*) tpMessage->msgpara();
	LK_ASSERT(tpMsg != NULL, return);	

	LOG_DEBUG( "default", "[%s] handle begine! "
		"receiver_name(%s) receiver_id(%d) sender_name(%s) "
		"mail_title(%s), mail_body(%s) receiver_countryid(%d)",
	    __FUNCTION__,
        tpMsg->role_name().c_str(), tpMsg->role_id(), tpMsg->sender_name().c_str(), 
		tpMsg->title().c_str(), tpMsg->body().c_str(), tpMsg->receive_countryid() );
	
	// 获取玩家信息
	int liCharId	= tpMsg->role_id();
    CRoleGlobalInfo *pPlayerInfo = CServiceRoleIDName::GetSingleton( ).GetRoleInfoByRoleID( liCharId ) ;
	if ( pPlayerInfo == NULL )
	{
		LOG_WARN( "mail", "[%s] Can't get role info by charid(%d)!", __FUNCTION__, liCharId );
		return ;
	}

	// 设置玩家名称，阵营
    int liCountryID     = pPlayerInfo->CountryID();
	tpMsg->set_role_name( pPlayerInfo->RoleName() );
	tpMsg->set_receive_countryid( liCountryID );

	// 获取玩家所在的场景服务器
	CSceneServer* pSceneServer = CGateServer::GetSingleton().GetServerByID( liCountryID );
	if( pSceneServer == NULL )
	{
		LOG_ERROR( "default", "[%s] Can't get scene server by charid(%d) CountryID(%d)", __FUNCTION__, liCharId, liCountryID );
		return ;
	}

	LOG_DEBUG( "default", "[%s] handle finished! "
		"receiver_name(%s) receiver_id(%d) sender_name(%s) "
		"mail_title(%s), mail_body(%s) receiver_countryid(%d) "
		"SceneServer(%d) CountryID(%d) ",
        __FUNCTION__,
		tpMsg->role_name().c_str(), tpMsg->role_id(), tpMsg->sender_name().c_str(), 
		tpMsg->title().c_str(), tpMsg->body().c_str(), tpMsg->receive_countryid(),
		(int)pSceneServer->mServerID, liCountryID );
	// 向玩家发送邮件消息
	
    CGateServer::GetSingleton().Send2Scene( pSceneServer, tpMessage );


}

