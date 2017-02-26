#include "fcmservice.h"
#include "gateobj_manager.h"
#include "gatelogic.h"
#include "servermessage_in.h"
#include "nameservice.h"
#include "chatmodule.h"
#include "chatmessage_pb.hxx.pb.h"
#include "chattype.h"
#include "core_service.h"
#include "family_manager.h"
#include "corpsmanager.h"
#include "protype.h"
#include "gateteammanager.h"
#include "log_tools.h"
#include "dirtyword.h"

void CChatmodule::OnSeverMessage( int vServerID, CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return );	      	
	switch ( pMessage->mutable_msghead()->messageid() )
	{	
	case ID_S2C_CHATSENDCALLBACK:
		{
			CMessageChatSendCallBack* pChatMessage = (CMessageChatSendCallBack*)pMessage->msgpara();

			switch( pChatMessage->channel() )
			{
			case CHANNEL_COUNTRY:
				{
					CPlayerManager::GetSingleton().SendCountryMessageFromGate( pMessage, pChatMessage->countryid() );
					SendChatMsg2Log( pMessage );			
				}
				break;
			case CHANNEL_P2P:
				{
					OnP2PChatMsg( pMessage );
				}
				break;
			case CHANNEL_BUGLE:
			case CHANNEL_SYS:
			case CHANNEL_WORLD:
				{
					OnWorldChatMsg( pMessage );
					SendChatMsg2Log( pMessage );
				}	
				break;
			case CHANNEL_FAMILY:
				{
					OnFamilyChatMsg( pMessage );
				}	
				break;
			case CHANNEL_CORPS:
				{
					OnCorpsChatMsg( pMessage );
				}	
				break;
			case CHANNEL_TEAM:
				{
					OnTeamMsg( pMessage );
				}
				break;
			}

			break;
		}
	default:
		break;
	}	
}
// 军团频道
void CChatmodule::OnCorpsChatMsg( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return )
		CMessageChatSendCallBack* pChatMessage = (CMessageChatSendCallBack*)pMessage->msgpara();
	LK_ASSERT( pChatMessage != NULL, return ) 	

		CCorps *tpCorps = CCorpsManager::GetSingleton().GetCorpsByID( pChatMessage->flagid() );
	if ( tpCorps != NULL )
	{			
		int tRoleIDList[CORPS_MAXMEMBERNUM] = { 0 };
		int tNum = CORPS_MAXMEMBERNUM;
		CCorpsManager::GetSingleton().GetCorpsMemberList( tpCorps, tRoleIDList, tNum );

		std::vector<CGatePlayer* > vPlayerList;
		for ( int i = 0; i < tNum; ++i )
		{
			CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().ScenePlayer( tRoleIDList[i] );
			if ( tpPlayer != NULL )
			{					
				vPlayerList.push_back(tpPlayer);
			}
		}		
		CGateServer::GetSingletonPtr()->Send2Player( vPlayerList, pMessage );
	}		
}

// 家族频道
void CChatmodule::OnFamilyChatMsg( CMessage* pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )
		CMessageChatSendCallBack* pChatMessage = (CMessageChatSendCallBack*)pMessage->msgpara();
	LK_ASSERT( pChatMessage != NULL, return ) 	

		CFamily *tpFamily = CFamilyManager::GetSingleton().GetFamilyByID( pChatMessage->flagid() );
	if ( tpFamily != NULL )
	{
		int tRoleIDList[MAX_FAMILYMEMBER_NUM] = { 0 };
		int tNum = MAX_FAMILYMEMBER_NUM;
		tpFamily->GetFamilyMemberIDList( tRoleIDList, tNum );

		std::vector<CGatePlayer* > vPlayerList;
		for ( int i = 0; i < tNum; ++i )
		{
			CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().CPlayerManager::GetSingleton().ScenePlayer( tRoleIDList[i] );
			if ( tpPlayer != NULL )
			{
				//CGateServer::GetSingletonPtr()->Send2Player( tpPlayer, pMessage );
				vPlayerList.push_back(tpPlayer);
			}
		}		
		CGateServer::GetSingletonPtr()->Send2Player( vPlayerList, pMessage );
	}	
}


void CChatmodule::OnMessage( int nServerID, CMessage* tpMessage )
{
	LK_ASSERT(tpMessage != NULL , return);	
}

void CChatmodule::OnMessageFangChenMiRequest( int vServerID, CMessage* tpMessage )
{
	//LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	//CMessageFangChenMiRequest *tpMsg = ( CMessageFangChenMiRequest * )tpMessage->msgpara();

}
// 密语频道
void CChatmodule::OnP2PChatMsg( CMessage* pMessage )
{
	LK_ASSERT( pMessage != NULL, return )
		CMessageChatSendCallBack* pChatMessage = (CMessageChatSendCallBack*)pMessage->msgpara();
	LK_ASSERT( pChatMessage != NULL, return ) 
		CGatePlayer *tpPlayer = CPlayerManager::GetSingleton().CPlayerManager::GetSingleton().ScenePlayer( pChatMessage->mutable_router()->mutable_desname()->c_str() );	
	if ( tpPlayer != NULL )
	{
		// 如果目标用户是GM 则消息里填充目标用户字段
		if ( (tpPlayer->mUserStatus & GM_STATE) == GM_STATE )  
		{
			pChatMessage->set_desusertype( USERTYPE_GM );
		}
		CGateServer::GetSingletonPtr()->Send2Player( tpPlayer, pMessage );
		CGatePlayer *tpSrcPlayer = CPlayerManager::GetSingleton().CPlayerManager::GetSingleton().ScenePlayer( pChatMessage->mutable_router()->srccharid() );
		if ( tpSrcPlayer != NULL )
		{
			CGateServer::GetSingletonPtr()->Send2Player( tpSrcPlayer, pMessage );				
		}
	}
	else
	{
		CGatePlayer *tpSrcPlayer = CPlayerManager::GetSingleton().CPlayerManager::GetSingleton().ScenePlayer( pChatMessage->mutable_router()->srccharid() );
		if ( tpSrcPlayer != NULL )
		{
			CMessage tMsgHead;
			CMessageChatResult tChatResult;
			tMsgHead.mutable_msghead()->set_messageid( ID_S2C_CHATRES2CLIENT );
			tMsgHead.set_msgpara( (int)&tChatResult );
			tChatResult.set_channel( CHANNEL_P2P );
			tChatResult.set_chatres( ERROR_CHAT_OFFLINE );				
			tChatResult.set_name( pChatMessage->mutable_router()->mutable_desname()->c_str() );
			CGateServer::GetSingletonPtr()->Send2Player( tpSrcPlayer, &tMsgHead );				
		}
	}	
}




// 世界频道
void CChatmodule::OnWorldChatMsg( CMessage* pMessage )
{ 
	LK_ASSERT( pMessage != NULL, return )
		CPlayerManager::GetSingleton().SendWorldMessageFromGate( pMessage );	
}


// 发送队伍消息
void CChatmodule::OnTeamMsg( CMessage* pMessage )
{
	if ( pMessage == NULL )
	{
		return;
	}
	CMessageChatSendCallBack* pChatMessage = (CMessageChatSendCallBack*)pMessage->msgpara();		
	LK_ASSERT( pChatMessage != NULL, return )
		if ( !pChatMessage->has_router() )
		{		
			return;
		}

		int tTeamMember[TEAMNUM] = { 0 };
		int tNum = CServiceGateTeam::GetSingleton().GetTeamList( pChatMessage->mutable_router()->srccharid(), tTeamMember, TEAMNUM );

		if ( tNum > 0 )
		{
			for ( int i = 0; i < tNum && i < (int) ARRAY_CNT( tTeamMember ) ; ++i )
			{
				CGatePlayer *tpDesPlayer = CPlayerManager::GetSingleton().ScenePlayer( tTeamMember[i] );
				if ( tpDesPlayer != NULL )
				{
					CGateServer::GetSingletonPtr()->Send2Player( tpDesPlayer, pMessage );
				}						
			}		
		}				
}


// 把聊天内容保存到日志服务器
void CChatmodule::SendChatMsg2Log( CMessage* pMessage )
{
	CMessageChatSendCallBack* pChatMessage = (CMessageChatSendCallBack*)pMessage->msgpara();
	// 系统消息不记录
	if( pChatMessage->channel() == CHANNEL_SYS )
	{
		return;
	}

	// 阵营聊天， 需要过滤关键字
	if( pChatMessage->channel() == CHANNEL_COUNTRY )
	{
		// 判断是否有需要记录的关键字(第8等级的字是关键字)
		if ( CDirtyWord::GetSingleton().OI_Dirty_Check( 8, ( char * )pChatMessage->msg().c_str(), 0 ) == 0 )
		{
			return;
		}
	}

	int nCharID = pChatMessage->mutable_router()->srccharid();
	int nIP = 0;
	CGatePlayer* pPlayer = CPlayerManager::GetSingletonPtr()->ScenePlayer( nCharID );
	if( pPlayer != NULL )
	{
		nIP = pPlayer->mClientAddr;
		LogChatMsg( nCharID,
			pPlayer->CharName(),
			pChatMessage->msg().c_str(),
			CGateServer::GetSingletonPtr()->GetWorldID(),
			pChatMessage->channel(),
			nIP);
	}
}


// 发送系统消息
void CChatmodule::SendSysNotice( const char *pMsg, int RepeatedNum )
{
	if ( pMsg == NULL || RepeatedNum <= 0 )
	{
		return;
	}
	CMessage		tMessage;
	CMessageChatSendCallBack tChatCallBack;
	tChatCallBack.mutable_router( )->set_srccharid( 0 );
	tChatCallBack.mutable_router( )->set_descharid( 0 );
	tChatCallBack.mutable_router( )->set_routertype( CMessageRouter::ROUTER_CHAR );
	if ( pMsg != NULL )
	{
		tChatCallBack.set_msg( pMsg );
	}	
	tChatCallBack.set_repeatednum( RepeatedNum );
	tChatCallBack.set_channel( CHANNEL_SYS );  	
	tMessage.mutable_msghead()->set_messageid( ID_S2C_CHATSENDCALLBACK );	
	tMessage.set_msgpara( (int)&tChatCallBack );


	// 直接走网关发送
	OnSeverMessage( 0, &tMessage );
}

