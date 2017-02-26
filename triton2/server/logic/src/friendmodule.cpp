#include "stdafx.hpp"
#include "friendmodule.h"
#include "friendmanager.h"
#include "coremessage_pb.hxx.pb.h"
#include "friendmessage_pb.hxx.pb.h"
#include "friendtype.h"
#include "errcode.h"
#include "message_factory.h"
#include "scenelogic.h"
#include "entity.h"
#include "propertymodule.h"
#include "log_msg_pb.hxx.pb.h"
#include "logmodule.h"
#include "sceneobj_manager_imp.h"
#include "mailmodule.h"
#include "marriagemanager.h"
#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif

char CFriendModule::sModuleName[ NAME_LENGTH ] = "Friend";
template<> CFriendModule* CSingleton< CFriendModule >::spSingleton = NULL;


//启动服务器
void CFriendModule::OnLaunchServer()
{
	OnLoadConfig();
	mMasterList.initailize();
	mClearTimer = CWTimer(600000);
}

void CFriendModule::OnLoadConfig()
{
	FRIEND_LOG* tpCfg = CSceneCfgManager::GetSingletonPtr()->GetFriend_log();
	if( tpCfg != NULL )
	{
		INIT_ROLLINGFILE_LOG( tpCfg->mIdentifier, tpCfg->mDirectory, (LogLevel)tpCfg->mPriority , tpCfg->mMaxfilesize, tpCfg->mMaxbackupindex);
	}
	LoadFriendBlessConfig();
	const char* pcConfigPath = CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(), "friend/friend.xml");
	CSceneServer::GetSingleton().RegisterLoadConfigCallback(pcConfigPath, CFriendModule::LoadFriendBlessConfigCallback);
}

//退出服务器
void CFriendModule::OnExitServer( )
{
	
}

//服务器Timer
void CFriendModule::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
	if ( mClearTimer.Elapse( vTickOffset ) == true )
	{
		// 删除过期的邀请
		int tNow = time( NULL );
		for ( MasterList::iterator it = mMasterList.begin(); it != mMasterList.end(); )
		{
			if ( tNow - (*it).mTime > 60000 )
			{
				it = mMasterList.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

// 建立场景
int CFriendModule::OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName )              
{                                                                                                                 
	return SUCCESS;	                                                                                                                  
}

//  路由消息
void CFriendModule::OnRouterMessage( CMessage* pMessage )
{ 
	LK_ASSERT(pMessage, return);
	const CMessageHead&  tHead = pMessage->msghead();

	switch(tHead.messageid())
	{
	case ID_S2S_FRIEND_INVITE_RESPONSE_R: // 邀请
		{
			CMessageInviteFriendResponseMsg* pMsg = (CMessageInviteFriendResponseMsg*) pMessage->msgpara();
			LK_ASSERT( pMsg, return );
			CMessageRouter* pRouter = pMsg->mutable_router();
			LK_ASSERT( pRouter != NULL , return );

			if ( pRouter->sendback() == 1 ) // 打回消息
			{
			}
			else
			{
				// 在邀请者的服务器
				PlayerReplyAddFriend( pRouter->descharid(), pRouter->srccharid(), pMsg->beinvitingplayername().c_str()
					, pMsg->beinvitingplayerhead(), pMsg->beinvitingplayersex(), pMsg->agreecode(), pMsg->isfriendlstfull(), pMsg->isdesinmylst(), pMsg->beinvitingplayernation() );
			}

			break;
		}

	case ID_S2C_FRIEND_INVITE_NOTICE_R: // 处理要添加的好友是否在线
		{
			CMessageInviteFriendNotice* pMsg = (CMessageInviteFriendNotice*) pMessage->msgpara();

			// 该人下线后那个请求就无效了
			CMessageRouter* pRouter = pMsg->mutable_router();
			LK_ASSERT( pRouter != NULL , return );
			if ( pRouter->sendback() == 1 ) // 打回消息
			{
				if ( pRouter->routerreason() == CMessageRouter_EMRouterReason_REASON_OFFLINE 
					|| pRouter->routerreason() == CMessageRouter_EMRouterReason_REASON_REDIRECT )
					OnMessageAddFriendOffLine( pMsg->srccharid(), pRouter->desname().c_str() );
			}
			else
			{
				// 以下内容在被加者的服务器进行(成功时) 发给玩家
				OnMessageAddFriendInDesServer(pMessage);
			}
	
			break;
		}

	case ID_S2S_FRIEND_CHANGESTATUS_NOTICE_R:  // 处理好友状态发生改变的消息
		{
			OnMessageFriendChangeStatusNotice( pMessage );
			break;
		}
	case ID_S2S_FRIEND_ADDFRIEND_NOTICE: // 收到其他服务器通知，记录好友
		{
			OnReplayerAddRequesterInServer( pMessage );
			break;
		}
	case ID_S2C_FRIEND_MESSAGE_NOTICE_R: // 玩家给好友发消息 回应消息
		{
			OnPlayerSendMessageRet( pMessage );
			break;
		}
	case ID_S2C_FRIEND_ERRCODE_NOTICE: // 错误码<纯发给客户端>
		{
			SendRouterMsg2Client( pMessage );
			break;
		}
	case ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE: // 好友改变消息<纯发给客户端>
		{
			SendRouterMsg2Client( pMessage );
			break;
		}
	case ID_S2C_NOTIFY_FRIENDLEVELUP:
		{
			OnRouterMsgFriendLevelUpNotify( pMessage );
			break;
		}
	case ID_S2S_NOTIFY_FRIENDBLESS:
		{
			OnRouterFriendBlessNotify( pMessage );
			break;
		}
	case ID_S2C_NOTIFY_LASTCONTACTER:
		{
			OnRouterMsgLastContacter(pMessage);
		}
		break;
	}
} 

// ***********************************************************
//  Function:		SendRouterMsg2Client
//  Description:	发给客户端路由消息
//  Input:			消息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2009.4.1
// **********************************************************
void CFriendModule::SendRouterMsg2Client( CMessage* pMessage )
{
	LK_ASSERT(pMessage, return);
	Message* pUnknownMessage = (Message*) pMessage->msgpara();
	const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessage->GetDescriptor();
	const ::google::protobuf::Reflection*  pReflection = pUnknownMessage->GetReflection();
	if (pDescriptor == NULL || pReflection == NULL) return; 
	const ::google::protobuf::FieldDescriptor*  pFieldDescriptor = pDescriptor->FindFieldByNumber(1);
	if (pFieldDescriptor == NULL) return;
	LK_ASSERT(pFieldDescriptor->type() == ::google::protobuf::FieldDescriptor::TYPE_MESSAGE, return );
	const Message& tRouter = pReflection->GetMessage(*pUnknownMessage, pFieldDescriptor); 
	CMessageRouter* pRouter = (CMessageRouter*) &tRouter;

	if ( pRouter->sendback() == 1 )
		return;

	CEntityPlayer* pPlayer = NULL;
	if ( pRouter->routertype() == CMessageRouter::ROUTER_NAME )
	{
		pPlayer = CSceneLogic::GetSingleton().GetPlayerByName( pRouter->desname().c_str() );
		CSceneLogic::GetSingleton( ).Send2Player( pPlayer, pMessage );
	}
	else if ( pRouter->routertype() == CMessageRouter::ROUTER_CHAR )
	{
		pPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( pRouter->descharid() );
		CSceneLogic::GetSingleton( ).Send2Player( pPlayer, pMessage );
	}
}


// ***********************************************************
//  Function:		OnGateMessage
//  Description:	网关消息
//  Input:			消息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2009.3.17
// **********************************************************
void CFriendModule::OnGateMessage( CMessage* pMessage )
{ 
	LK_ASSERT(pMessage, return);
	const CMessageHead&  tHead = pMessage->msghead();

	switch(tHead.messageid())
	{
	case ID_G2S_GETCHARID_RESPONSE: // 新版添加好友 去网关查ID 返回的消息 
		{
			OnMessageGetCharIDFromGate( pMessage );
			break;
		}
	case ID_G2S_ADDFRIEND_RESPONSE: // 新版添加好友 
		{
			OnMessageAddFriend( pMessage );
			break;
		}
	case ID_G2S_PLAYER_ONLINE_FIREND_RESPONSE : // 处理gate传回来的在线好友列表【好友模块】
		{
			OnMessageRecvOnlineFriends( pMessage );
			break;
		}

	case ID_S2C_RESPONSE_CHANGEMASTER:
		{
			OnMessageChangeMasterResponse( pMessage );
			break;
		}
	case ID_S2C_NOTIFY_CHANGEMASTER:
		{
			OnMessageChangeMasterNotify( pMessage );
			break;
		}
	case ID_S2C_RESPONSE_GETSLAVE:
		{
			OnMessageGetSlaveResponse( pMessage );
			break;
		}
	case ID_S2C_NOTIFY_SLAVELEVELUP:
		{
			OnMessageSlaveLevelUpNotify( pMessage );
			break;
		}
	case ID_S2C_NOTIFY_SLAVEONOFF:
		{
			OnMessageSlaveOnOffNotify( pMessage );
			break;
		}
	case ID_S2C_RESPONSE_QUERYPLAYER:
		{
			OnMessageQueryPlayerResponse( pMessage );
			break;
		}
	case ID_S2C_NOTIFY_REMOVESLAVE:
		{
			OnMessageRemoveSlaveNotify( pMessage );
			break;
		}
	case ID_S2C_RESPONSE_REMOVESLAVE:
		{
			OnMessageRemoveSlaveResponse( pMessage );
			break;
		}
	case ID_G2S_ADDMASTERPOINT_NOTIFY:
		{
			OnMessageAddMasterPointNotify( pMessage );
			break;
		}
	case ID_S2C_NOTIFY_ANSWERCHANGE:
		{
			OnMessageAnswerChangeNotify( pMessage );
			break;
		}
	case ID_S2C_RESPONSE_REMOVEMASTER:
		{
			OnMessageRemoveMasterResponse( pMessage );
			break;
		}
	case ID_S2C_NOTIFY_REMOVEMASTER:
		{
			OnMessageRemoveMasterNotify( pMessage );
			break;
		}
	case ID_G2S_NOTIFY_CONTACTERSTATUS:
		{
			OnMessageContacterStatusNotice(pMessage);
		}
		break;
	case ID_S2C_NOTIFY_SWORNINFO:
		{
			OnRouterMsgSwornInfo( pMessage );
			break;
		}
	case ID_S2C_NOTIFY_LEAVESWORNTEAM:
		{
			OnRouterMsgLeaveSwornTeamNotify( pMessage );
			break;
		}
	case ID_S2C_NOTIFY_KICKSWORNMEMBER:
		{
			OnRouterMsgKickSwornMember( pMessage );
			break;
		}	
	case ID_S2C_NOTIFY_DISBANDSWORNTEAM:
		{
			OnRouterMsgDisbandSwornTeam( pMessage );
			break;
		}
	case ID_S2S_NOTIFY_MODIFYINTIMACY:
		{
			OnRouterMsgModifyIntimacy( pMessage );
			break;
		}	
	case ID_S2C_NOTIFY_CHANGESWORNNAME:
		{
			OnRouterMsgChangeSwornName( pMessage );
			break;
		}
	case ID_S2S_NOTIFY_MODIFYLASTTIMEALLONLINE:
		{
			OnRouterMsgModifyLastTimeAllOnLine( pMessage );
			break;
		}

	default:
		{
			LOG_ERROR( "friend", "[%s:%d] message id %d", __FUNCTION__, __LINE__, tHead.messageid() );
			break;
		}
	}
}

// ***********************************************************
//  Function:		OnMessageAddFriendInDesServer
//  Description:	在被加的人的服务器上处理
//  Input:			NULL
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.12.17
// **********************************************************
void CFriendModule::OnMessageAddFriendInDesServer( CMessage* pMessage )
{
	LK_ASSERT(pMessage, return);
	CMessageInviteFriendNotice* pMsg = (CMessageInviteFriendNotice*)pMessage->msgpara();
	LK_ASSERT(pMsg, return);
	CMessageRouter* pRouter = pMsg->mutable_router();
	LK_ASSERT( pRouter != NULL , return );

	FriendType::iterator it = mRels.find( pMsg->descharid() );	

	if (it != mRels.end())
	{
		if ( it->second.IsBlack(pMsg->srccharid()) )
		{
			SendErrorCode(pMsg->srccharid(),ERROR_YOU_ARE_BLACK_ADD);
			return;
		}
	}

	CEntityPlayer* pPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( pMsg->descharid() );
	if ( pPlayer )
		CSceneLogic::GetSingleton( ).Send2Player( pPlayer, pMessage );
}



// ***********************************************************
//  Function:		CountSize
//  Description:	计算模块的大小
//  Input:			NULL
//  OutPut:			NULL
//  Return:			size
//  Others:
//  Date:			2008.10.28
// **********************************************************
unsigned int CFriendModule::CountSize()
{
	unsigned int tSize = sizeof( CFriendModule );

	return tSize;
}



// ***********************************************************
//  Function:		OnClientMessage
//  Description:	客户端发送消息
//  Input:			玩家, 消息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.10.28
// **********************************************************
void CFriendModule::OnClientMessage(CEntityPlayer* pPlayer, CMessage* pMessage)
{
	LK_ASSERT(pMessage && pPlayer, return);
	const CMessageHead&  tHead = pMessage->msghead();

	switch(tHead.messageid())
	{
		case ID_C2S_FRIEND_ADDFRIENDBYNAME_REQUEST:  
		{
			// 增加好友第一步, 响应客户端向场景提交请求 
			CMessageInviteFriendRequestByName* req = (CMessageInviteFriendRequestByName*) pMessage->msgpara();
			PlayerAddFriendByName( pPlayer, req->invitedplayername().c_str() );
			break;
		}

		case ID_C2S_FRIEND_INVITE_REQUEST:  
		{
			// 增加好友第一步, 响应客户端向场景提交请求 (根据ID加好友)
			CMessageInviteFriendRequest* req = (CMessageInviteFriendRequest*) pMessage->msgpara();

			PlayerAddFriendByID( pPlayer, req->playerid() );
			break;
		}

		case ID_C2S_FRIEND_INVITE_OPTION:
		{
			// 玩家应答是否同意被加好友
			CMessageInviteFriendOptionRequest* req = (CMessageInviteFriendOptionRequest*) pMessage->msgpara();

			OnPlayerReplyAddFriend( pPlayer, req->invitingplayerid(), req->agreecode() );
			break;
		}

		case ID_C2S_FRIEND_CHANGESTATUS_REQUEST:
		{
			// 玩家请求改变状态
			OnMessagePlayerChangeStatus( pPlayer, pMessage );
			break;
		}

		case ID_C2S_FRIEND_DEL_REQUEST:
		{
			// 玩家删除好友的信息
			OnMessagePlayerDelFriends( pPlayer, pMessage );
			break;
		}

		case ID_C2S_FRIEND_CHGGROUPNAME_REQUEST:
		{
			// 玩家修改组名
			OnMessagePlayerChangeGroupName( pPlayer, pMessage );
			break;
		}

		case ID_C2S_FRIEND_CHGFRIENDGROUP_REQUEST:
		{
			// 玩家移动好友到其他分组
			OnMessagePlayerChangeFriendGroup( pPlayer, pMessage );
			break;
		}

		case ID_C2S_FRIEND_SENDMESSAGE_REQUEST:
		{
			// 发私聊消息
			OnMessagePlayerSendMsg( pPlayer, pMessage );
			break;
		}

		case ID_C2S_FRIEND_MOVETOBLACK_REQUEST:
		{
			// 移动好友到黑名单
			OnMessagePlayerMoveBlack( pPlayer, pMessage );
			break;
		}
	
		
		case ID_C2S_FRIEND_ADDBLACK_REQUEST:
		{
			// 添加黑名单
			OnMessagePlayerAddBlack( pPlayer, pMessage );
			break;
		}
		
		case ID_C2S_FRIEND_LOCKENEMY_REQUEST:
		{
			// 锁定仇敌请求
			OnMessagePlayerLockEnemy( pPlayer, pMessage );
			break;
		}
		case ID_C2S_FRIEND_GETFRIENDSSTATE_REQUEST:
		{
			// 客户端打开好友界面 刷新好友的状态
			OnMessagePlayerRereshFriendState( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_CHANGEMASTER:
		{
			OnMessageChangeMasterRequest( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_GETSLAVE:
		{
			OnMessageGetSlaveRequest( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_QUERYPLAYER:
		{
			OnMessageQueryPlayerRequest( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_REMOVESLAVE:
		{
			OnMessageRemoveSlaveRequest( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_REMOVEMASTER:
		{
			OnMessageRemoveMasterRequest( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_ALTERMASTERPERCENT:
		{
			OnMessageAlterMasterPercentRequest( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_GETLOWPLAYER:
		{
			OnMessageGetLowPlayerRequest( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_ANSWERCHANGE:
		{
			OnMessageAnswerChangeRequest( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_FRIENDBLESS:
		{
			OnMessageFriendBlessRequest( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_CREATESWORNTEAM :
		{
			OnMessageCreateSwornTeam( pPlayer, pMessage );
			break;
		}		
		
		case ID_C2S_REQUEST_LEAVESWORNTEAM :
		{
			OnMessageLeaveSwornTeam( pPlayer, pMessage );
			break;
		}

		case ID_C2S_REQUEST_KICKSWORNMEMBER :
		{
			OnMessageKickSwornMember( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_DISBANDSWORNTEAM :
		{
			OnMessageDisbandSwornTeam( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_CHANGESWORNNAME :
		{
			OnMessageChangeSwornName( pPlayer, pMessage );
			break;
		}
		case ID_C2S_REQUEST_REBUILDSWORN:
		{
			OnMessageRebuildSworn( pPlayer, pMessage );
			break;
		}
		default:
			break;
	}
}



// ***********************************************************
//  Function:		OnSaveData
//  Description:	存储数据
//  Input:			玩家
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2009.2.10
// **********************************************************
bool CFriendModule::OnSaveData(CEntityPlayer* pPlayer, int vSaveCode)
{
	LK_ASSERT(pPlayer, return false);
	FriendType::iterator it = mRels.find( pPlayer->GetCharID() );	

	if (it == mRels.end())
	{
		LOG_ERROR("FRIEND", "can't get rels, in saving，player char id=%d", pPlayer->GetCharID() );
		return false;
	}

	// 如果是在特殊地点，不存盘
	if ( IS_GLOBAL_SCENESERVER( pPlayer->GetLineID() ) || IS_DUNGEONSERVER_LINE( pPlayer->GetLineID() ) )
	{
		return true;
	}

	//if ( !it->second.IstoSave() )
	//{
	//	// 没有修改，不用保存
	//	return;
	//}

	// 更新好友的同时在线时间
	if ( vSaveCode == SAVE_LEAVEGAME || vSaveCode == SAVE_RETURN2GATE || vSaveCode == SAVE_KICKOUT )
	{
		UpdateLastTimeAllOnLine( pPlayer );
	}

	char tBuffer[ PLAYER_DATALEN ] = { 0 }; 
	int  tBufferSize = sizeof(tBuffer);

	PBFriend tPBFriend;

	it->second.GetTlvBuffer2( &tPBFriend );
	tPBFriend.SerializePartialToArray( tBuffer, tBufferSize );

	char tEscapStr[ PLAYER_DATALEN ] = {0};
	char* tPointer = tEscapStr;

	int tRealLen = EscapeDataString( &tPointer, sizeof(tEscapStr)-1, tBuffer, tPBFriend.ByteSize() );

	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return false);

	int tIsCallBack = SQL_NEED_CALLBACK;
	if ( vSaveCode == SAVE_TIMETRIGER )
		tIsCallBack = SQL_NONE;

	CMessagePlayerProperty* tpSave = CSceneLogic::GetSingletonPtr( )->GetSavePlayerProperty( );
	
	tpSave->mutable_role_friend( )->CopyFrom( tPBFriend );
	return true;

//	return CSceneLogic::GetSingletonPtr()->ExecuteSql( saveplayer_friend, vSaveCode, pPlayer->get_id(), pPlayer->GetCharID(), tIsCallBack, 
//		UPDATE, "update UMS_ROLE_DETAIL_%02d set role_friend = '%s', save_bitflag = save_bitflag + '%d' where role_id = %d",
//		pPlayer->GetAccountID()%16, tEscapStr, LSSTATE_FRIEND, pPlayer->GetCharID() );

}

CFriendItem* CFriendModule::GetPlayerFriendItem(int nRoleID)
{
	FriendType::iterator it = mRels.find( nRoleID );	

	return it != mRels.end() ? &(it->second) : NULL;
}

// ***********************************************************
//  Function:		CommonSqlCallBack
//  Description:	通用回调函数
//  Input:			SQL执行结果
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.10.28
// **********************************************************
void CFriendModule::CommonSqlCallBack( CMessage* pMsg, int vparam1, int vparam2 )
{
	LK_ASSERT( pMsg != NULL, return );

	CMessageExecuteSqlResponse3* pMessage = (CMessageExecuteSqlResponse3*)pMsg->msgpara();
	LK_ASSERT( pMessage != NULL, return );

	if ( pMessage->resultcode() != 1 )
	{
		LOG_ERROR( "FRIEND", "error in execute sql, file=%s, line=%d vparam2=%d", __FILE__, __LINE__, vparam2 );
	}
}



// ***********************************************************
//  Function:		OnCreateEntity
//  Description:	建立实体
//  Input:			****
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.10.28
// **********************************************************
void CFriendModule::OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst )
{
	LK_ASSERT(pEntity, return);
	if ( pEntity->GetEntityType( ) != ENTITYTYPE_PLAYER )
		return;

	CEntityPlayer* tpPlayer = (CEntityPlayer*) pEntity;	
	LK_ASSERT( tpPlayer != NULL, return );

	// 获取玩家的结拜信息
	if ( tpPlayer->GetSwornID() > 0 )
	{
		CSwornTeam *tpSwornTeam = GetSwornTeam( tpPlayer->GetSwornID() );
		if ( tpSwornTeam != NULL )
		{
			// 查看是否被踢掉
			if ( tpSwornTeam->HasMember( tpPlayer->GetCharID() ) )
			{
				tpSwornTeam->ChangeRoleStatus( tpPlayer->GetCharID(), CSwornTeam::EROLE_STATUS_ONLINE );
				SendNotifySwornInfo( tpPlayer, tpSwornTeam );
			}								
			else
			{
				CPropertyModule::GetSingleton().OnPlayerDelTitleEvent( tpPlayer, mFriendBlessConfig.GetIntimacyConfig()->mSwornTitleID );
				tpPlayer->SetSwornID( 0 );
			}
		}			
		else
		{
			GetSwornDBInfo( tpPlayer );
		}
	}		

	FriendType::iterator it = mRels.find( tpPlayer->GetCharID() );
	if ( it == mRels.end() )
	{
		LOG_ERROR("FRIEND", "can't find rel charid=%d", tpPlayer->GetCharID() );
		return;
	}

	// 去网关服务器取在线的人,flag = 1  在收到网关发回来的信息后，通知所有的在线好友，我上线了！
	AskGateFriendInfo( tpPlayer->GetCharID(), it->second, !vFirst );
	if (vFirst)
	{
		GetContacterListInfo(tpPlayer->GetCharID(), it->second);
		UploadContacterListToGate(tpPlayer->GetCharID());
		SendMyStatus2Gate(tpPlayer->GetCharID(), AT_ONLINE);
	}
	
	
	// 通知前后辈玩家
	if ( vFirst == true )
	{
		SendSlaveOnOffNotify( tpPlayer, 1 );

		// 更新亲密度的数值
		UpdateIntimacyDegree( tpPlayer );	
		
	}
}

//销毁实体
void CFriendModule::OnDestroyEntity( CEntity* pEntity, int vCode )
{
	LK_ASSERT( pEntity, return );
	if ( pEntity->GetEntityType( ) == ENTITYTYPE_PLAYER )
	{

		CEntityPlayer* pPlayer = ( CEntityPlayer* )pEntity;

		if ( pPlayer->GetSwornID() > 0 )
		{
			CSwornTeam *tpSwornTeam = GetSwornTeam( pPlayer->GetSwornID() );
			if ( tpSwornTeam != NULL )
			{
				tpSwornTeam->ChangeRoleStatus( pPlayer->GetCharID(), CSwornTeam::EROLE_STATUS_OFFLINE );
				if ( tpSwornTeam->GetOnLineNum() <= 0 )
				{
					DeleteSwornTeam( pPlayer->GetSwornID() );
				}				
			}		
		}		
		FriendType::iterator it = mRels.find( pPlayer->GetCharID() );
		if ( it == mRels.end() )
		{
			// 没有好友列表
			return;
		}

		std::vector<int> friend_list;
		// 取得在线列表
		if ( vCode != SAVE_REDIRECT ) // 如果不是跨场景 就通知好友、仇敌自己下线了
		{
			it->second.GetFriendCharIDList( friend_list, true );
			SendMyStatus2Friends( pPlayer->GetCharID(), friend_list, AT_OFFLINE, YOUR_FRIEND );
			friend_list.clear();
			it->second.GetEnemyCharIDList( friend_list, true );
			SendMyStatus2Friends( pPlayer->GetCharID(), friend_list, AT_OFFLINE, YOUR_ENEMY );

			// 通知前后辈玩家
			SendSlaveOnOffNotify( pPlayer, 0 );

			SendMyStatus2Gate(pPlayer->GetCharID(), AT_OFFLINE);
		}
		DeleteLevelUpFriendList( pPlayer->GetCharID() );
		mRels.erase( it );				
	}
}


// ***********************************************************
//  Function:		PlayerAddFriendByName	
//  Description:	玩家添加好友
//  Input:			玩家，要加的人的名字
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.10.25
// **********************************************************
void CFriendModule::PlayerAddFriendByName ( CEntityPlayer* pPlayer, const  char* strDesName )
{
	LK_ASSERT( pPlayer, return );
	LK_ASSERT( strDesName, return );
	CMessage tMessage;
	if ( strcmp( strDesName, pPlayer->GetCharName() ) ==0 ) // 看看是不是在加自己
	{
		CMessageFriendErrorNotice msg;
		msg.set_errorcode( ERROR_CANT_ADD_SELF );
		MakeRouterMessageByName<CMessageFriendErrorNotice>( strDesName, strDesName, 0, ID_S2C_FRIEND_ERRCODE_NOTICE
			, msg, &tMessage );
		CSceneLogic::GetSingleton( ).Send2Player( strDesName, &tMessage );
		return;
	}

	if ( IS_GLOBAL_SCENESERVER( pPlayer->GetLineID() ) || IS_DUNGEONSERVER_LINE( pPlayer->GetLineID() ) )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, EDUNGEON_ERROR_NOPLACEACTION );
		return ;
	}

	LOG_DEBUG( "FRIEND", "player %s add player %s", pPlayer->GetCharNameShow(), strDesName );

	// 发给网关
	CMessageGetCharIDRequest msg;
	msg.set_srcentityid( pPlayer->GetEntityID() );
	msg.set_name( strDesName );
	msg.set_typeid_( TYPE_ADD_FRIEND );
	tMessage.set_msgpara((uint32_t) &msg);
	tMessage.mutable_msghead()->set_messageid( ID_S2G_GETCHARID_REQUEST );
	CSceneLogic::GetSingleton( ).Send2Gate( &tMessage );
}



// ***********************************************************
//  Function:		PlayerAddFriendByID	
//  Description:	玩家添加好友 根据ID
//  Input:			玩家，要加的人的ID
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2009.4.13
// **********************************************************
void CFriendModule::PlayerAddFriendByID( CEntityPlayer* pPlayer, int nDesCharID ) 
{
	LK_ASSERT( pPlayer, return );
	CMessage tMessage;
	if ( (int)pPlayer->GetCharID() == nDesCharID || nDesCharID == 0 ) // 看看是不是在加自己
	{
		CMessageFriendErrorNotice msg;
		msg.set_errorcode( ERROR_CANT_ADD_SELF );
		MakeRouterMessageByID<CMessageFriendErrorNotice>( nDesCharID, nDesCharID, 0, ID_S2C_FRIEND_ERRCODE_NOTICE
			, msg, &tMessage );
		CSceneLogic::GetSingleton( ).Send2Player( nDesCharID, &tMessage );
		return;
	}

	if ( IS_GLOBAL_SCENESERVER( pPlayer->GetLineID() ) || IS_DUNGEONSERVER_LINE( pPlayer->GetLineID() ) )
	{
		CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, EDUNGEON_ERROR_NOPLACEACTION );
		return ;
	}

	DisposeAddFriend( pPlayer, nDesCharID );
}



// ***********************************************************
//  Function:		OnPlayerAddFriendCallBack	
//  Description:	玩家添加好友 去数据库查人和charid
//  Input:			回调信息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.12.17
// **********************************************************
void CFriendModule::OnPlayerAddFriendCallBack( CMessage* pMsg, int vparam1, int vparam2 )
{
}

// ***********************************************************
//  Function:		InsertFriendData	
//  Description:	在加载好友数据成功后插入
//  Input:			角色ID， 好友项
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.2.10
// **********************************************************
void CFriendModule::InsertFriendData( int nCharID, const CFriendItem& rItem )
{
	FriendType::iterator it = mRels.find( nCharID );
	if ( it != mRels.end() )
	{
		LOG_ERROR( "FRIEND", "friend_data exsited!! player ID=%d", nCharID );
		mRels.erase( it );
	}

	mRels.insert(std::make_pair(nCharID, rItem));
}

// ***********************************************************
//  Function:		PlayerReplyAddFriend	
//  Description:	玩家应答添加好友的请求(肯定在邀请方所在的服务器)
//  Input:			邀请者ID，回应者ID，回应者名字，回应者性别，态度码, 被邀请者的列表满了没，邀请者是不是已经在被邀请者的列表中了
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.10.25
// **********************************************************
void CFriendModule::PlayerReplyAddFriend(  int nSrcPlayerID, int nReplyCharID,  const char* szReplyName, int nReplyHead
										 , int nReplySex, int nApplyCode, unsigned char nIsFullList, unsigned char nIsReqInReplyLst, int nReplyNationID )
{
	CEntityPlayer* pPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( nSrcPlayerID );
	if ( pPlayer == NULL ) // 估计已经下线了
	{
		return;
	}

	// 把结果发给邀请者
	CMessageInviteFriendResponseNotice msg; // 发到邀请者玩家

	CMessage tMessage;

	msg.set_beinvitingplayername( szReplyName );
	msg.set_agreecode( nApplyCode );

	tMessage.set_msgpara((uint32_t) &msg);
	tMessage.mutable_msghead()->set_messageid( ID_S2C_FRIEND_INVITE_RESPONSE_NOTICE );
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );

	if ( nApplyCode == 0 ) 
		return;

	//CEntityPlayer* tpReplyPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( nReplyCharID );
	//if ( tpReplyPlayer == NULL ) // 估计已经下线了
	//{
	//	return;
	//}

	CPropertyPlayer* tpSrcProperty = ( CPropertyPlayer* ) pPlayer->GetProperty();
	//CPropertyPlayer* tpReplyProperty = ( CPropertyPlayer* ) tpReplyPlayer->GetProperty(); 

	// 判断被邀请者的列表是否已满
	if ( nIsFullList == 1 )
	{
		// 发给邀请者，对方列表满
		CMessageFriendErrorNotice msg1;
		msg1.set_errorcode( ERROR_DES_LIST_FULL );

		MakeRouterMessageByID<CMessageFriendErrorNotice>( nReplyCharID, pPlayer->GetCharID(), 0
			, ID_S2C_FRIEND_ERRCODE_NOTICE, msg1, &tMessage );

		CSceneLogic::GetSingleton( ).Send2Player( pPlayer->GetCharID(), &tMessage );

		// 发给被邀请者，你的列表满
		CMessageFriendErrorNotice msg2;
		msg2.set_errorcode( ERROR_YOUR_LIST_FULL );

		MakeRouterMessageByID<CMessageFriendErrorNotice>( pPlayer->GetCharID(), nReplyCharID, 0
			, ID_S2C_FRIEND_ERRCODE_NOTICE, msg2, &tMessage );

		CSceneLogic::GetSingleton( ).Send2Player( nReplyCharID, &tMessage );

		// 发给被邀请者，添加陌生人
		CMessageChangeFriendNotice msg3;
		msg3.set_charid( nReplyCharID );
		msg3.set_name( szReplyName );
		msg3.set_oprcode( ADD_TO_STRANGER );
		msg3.set_reltome( YOUR_STRANGER );
		//msg3.set_vipflag( tpReplyProperty->GetVipFlag() );

		MakeRouterMessageByID<CMessageChangeFriendNotice>( pPlayer->GetCharID(), nReplyCharID, 1
			, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg3, &tMessage );

		CSceneLogic::GetSingleton( ).Send2Player( nReplyCharID, &tMessage );
		return;
	}

	// 判断邀请者的列表是否已满
	FriendType::iterator it = mRels.find( pPlayer->GetCharID() );
	if ( it == mRels.end() )
	{
		LOG_ERROR("FRIEND", "can't find rels, charid=%d", pPlayer->GetCharID() );
		return;
	}
	if ( it->second.GetFriendNum( ) >= FRIENDS_LIMIT )
	{
		// 发给邀请者，你的列表满
		CMessageFriendErrorNotice msg1;
		msg1.set_errorcode( ERROR_YOUR_LIST_FULL );

		MakeRouterMessageByID<CMessageFriendErrorNotice>( nReplyCharID, pPlayer->GetCharID(), 0
			, ID_S2C_FRIEND_ERRCODE_NOTICE, msg1, &tMessage );

		CSceneLogic::GetSingleton( ).Send2Player( pPlayer->GetCharID(), &tMessage );

		// 发给被邀请者，对方列表满
		CMessageFriendErrorNotice msg2;
		msg2.set_errorcode( ERROR_DES_LIST_FULL );

		MakeRouterMessageByID<CMessageFriendErrorNotice>( pPlayer->GetCharID(), nReplyCharID, 0
			, ID_S2C_FRIEND_ERRCODE_NOTICE, msg2, &tMessage );

		CSceneLogic::GetSingleton( ).Send2Player( nReplyCharID, &tMessage );

		// 发给被邀请者，添加陌生人
		CMessageChangeFriendNotice msg3;
		msg3.set_charid( pPlayer->GetCharID() );
		msg3.set_name( pPlayer->GetCharName() );
		msg3.set_oprcode( ADD_TO_STRANGER );
		msg3.set_reltome( YOUR_STRANGER );
		msg3.set_vipflag( tpSrcProperty->GetVipFlag() );

		MakeRouterMessageByID<CMessageChangeFriendNotice>( pPlayer->GetCharID(), nReplyCharID, 1
			, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg3, &tMessage );

		CSceneLogic::GetSingleton( ).Send2Player( nReplyCharID, &tMessage );
		return;
	}

	EPlayerRels nRel = GetRel2Me( pPlayer->GetCharID(), nReplyCharID );
	// 到这里列表满的情况判断完毕

	// 判断A,B是不是已经在对方的好友列表中了
	if ( nIsReqInReplyLst == 0 )
	{
		// 如果邀请者没有在被邀请者的好友列表中
		// 给被邀请者发添加好友的通知
		CMessageChangeFriendNotice msg;
		msg.set_charid( pPlayer->GetCharID() );
		msg.set_name( pPlayer->GetCharName() );
		msg.set_status( it->second.GetMyStatus() );
		msg.set_groupid( 0 );
		msg.set_reltome( nRel );
		msg.set_sex( pPlayer->GetSex() );
		msg.set_head( pPlayer->GetFaceType() );
		msg.set_nationid( pPlayer->GetNationality() );
		msg.set_oprcode( ADD_TO_FRIEND );
		msg.set_vipflag( tpSrcProperty->GetVipFlag() );

		MakeRouterMessageByID<CMessageChangeFriendNotice>( pPlayer->GetCharID(), nReplyCharID, 1
			, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg, &tMessage );
		
		CSceneLogic::GetSingleton( ).Send2Player( nReplyCharID, &tMessage );

		// 通知被邀请者所在的服务器，记录好友列表
		CMessageAddFriendNotice msgadd;
		msgadd.set_charid( pPlayer->GetCharID() );
		msgadd.set_status( it->second.GetMyStatus() );
		msgadd.set_groupid( 0 );
		msgadd.set_reltome( nRel );

		MakeRouterMessageByID<CMessageAddFriendNotice>( pPlayer->GetCharID(), nReplyCharID, 0
			, ID_S2S_FRIEND_ADDFRIEND_NOTICE, msgadd, &tMessage );

		if ( CSceneLogic::GetSingleton().GetPlayerByCharID( nReplyCharID ) == NULL )
			CSceneLogic::GetSingleton( ).Send2Player( nReplyCharID, &tMessage );
		else
			OnRouterMessage( &tMessage );

		// 存入数据库
		//SaveFriend( nReplyCharID , pPlayer->mCharProfile.mCharID, nRel, nReplyCharID, true );
		LOG_DEBUG("FRIEND", "%s add %s successful", szReplyName, pPlayer->GetCharNameShow() );
	}

	// 邀请者rels是空，添加好友 (邀请者好友列表里肯定没有被邀请者，在他发出邀请的时候已经判断过)

	//if ( it == mRels.end() )【前面判断过了】
	//{
	//	LOG_ERROR("FRIEND", "%d can't find friend Item", pPlayer->GetCharID());
	//}
	//else
	//{
	it->second.AddFriend( nReplyCharID );
	//}

	// 发给邀请者的客户端
	CMessageChangeFriendNotice msg5;
	msg5.set_charid( nReplyCharID );
	msg5.set_name( szReplyName );
	msg5.set_status( AT_ONLINE );
	msg5.set_groupid( 0 );
	msg5.set_reltome( nRel );
	msg5.set_head( nReplyHead );
	msg5.set_sex( nReplySex );
	msg5.set_nationid( nReplyNationID );
	msg5.set_oprcode( ADD_TO_FRIEND );
	//msg5.set_vipflag( tpReplyProperty->GetVipFlag() );

	MakeRouterMessageByID<CMessageChangeFriendNotice>( nReplyCharID, pPlayer->GetCharID(), 1
		, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg5, &tMessage );

	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );

	// 看看新加的好友是不是自己的仇敌 是就干掉
	if (0 == it->second.DelEnemy(nReplyCharID))
	{
		// 通知客户端 干掉仇敌
		CMessageChangeFriendNotice msg2;
		msg2.set_charid( nReplyCharID );
		msg2.set_oprcode( DEL_ENEMY );
		msg2.set_reltome( YOUR_ENEMY );
		//msg2.set_vipflag( tpReplyProperty->GetVipFlag() );
		CMessage tMessage2;
		MakeRouterMessageByID<CMessageChangeFriendNotice>( nReplyCharID, pPlayer->GetCharID()
			, 1, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg2, &tMessage2 );
		CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage2 );
	}

	// 看看新加的好友是不是自己的黑名单 是就干掉
	if (0 == it->second.DelBlack(nReplyCharID))
	{
		// 通知客户端 干掉黑名单
		CMessageChangeFriendNotice msg2;
		msg2.set_charid( nReplyCharID );
		msg2.set_oprcode( DEL_BLACK );
		//msg2.set_vipflag( tpReplyProperty->GetVipFlag() );
		CMessage tMessage2;
		MakeRouterMessageByID<CMessageChangeFriendNotice>( nReplyCharID, pPlayer->GetCharID()
			, 1, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg2, &tMessage2 );
		CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage2 );
	}

	LOG_DEBUG("FRIEND", "%s add %s successful", pPlayer->GetCharNameShow(), szReplyName );
}


// ***********************************************************
//  Function:		GetRel2Me	
//  Description:	取得该人和我的关系，要去其他模块处理 (暂时认为2个人彼此的关系是一样的)
//  Input:			我的char id,对方的char id
//  OutPut:			NULL
//  Return:			关系枚举
//  Others:
//  Date:			2008.10.31
// **********************************************************
EPlayerRels CFriendModule::GetRel2Me( int nCallerCharID, int nFriendID )
{
	/*CFriendItem *tpFriendItem = GetPlayerFriendItem( nCallerCharID );
	if ( tpFriendItem == NULL )
	{
		return YOUR_STRANGER;
	}
	st_friend *tpFriend = tpFriendItem->GetFriendByRoleID( nFriendID );
	if ( tpFriend == NULL )
	{
		return YOUR_STRANGER;
	}	
	return tpFriend->enRel2Me;*/
	return YOUR_FRIEND;
}

// ***********************************************************
//  Function:		SetRel2Me	
//  Description:	设置好友关系(如果关系没有当前的亲密则不变),不包括仇,黑名单等关系
//  Input:			我的char id,对方的char id
//  OutPut:			NULL
//  Return:			关系枚举
//  Others:
//  Date:			2008.10.31
// **********************************************************
int CFriendModule::SetRel2Me( CEntityPlayer *pPlayer, int nFriendID, EPlayerRels eRels, EOprFriendCode eOprCode )
{
	LK_ASSERT( pPlayer != NULL, return -1 )
	CFriendItem *tpFriendItem = GetPlayerFriendItem( pPlayer->GetCharID() );
	if ( tpFriendItem == NULL )
	{
		return 0;
	}
	st_friend *tpFriend = tpFriendItem->GetFriendByRoleID( nFriendID );
	if ( tpFriend == NULL )
	{
		return 0;
	}	

	if ( eOprCode == ADD_TO_FRIEND )
	{
		if ( eRels < tpFriend->enRel2Me )
		{
			tpFriend->enRel2Me = eRels;		
		}	
	}

	if ( eOprCode == DEL_FRIEND )
	{
		if ( tpFriend->enRel2Me == eRels )
		{
			tpFriend->enRel2Me = YOUR_FRIEND;		
		}	
	}	

	CMessage tMsgHead;
	CMessageChangeFriendNotice msg3;
	msg3.set_charid( nFriendID );		
	msg3.set_oprcode( eOprCode );
	msg3.set_reltome( eRels );
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMsgHead );
	return 0;
}

// ***********************************************************
//  Function:		OnPlayerReplyAddFriend	
//  Description:	玩家应答别人加他为好友的请求
//  Input:			玩家，邀请者的ID，同意不同意
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.10.27
// **********************************************************
void CFriendModule::OnPlayerReplyAddFriend( CEntityPlayer* pPlayer, int nSrcPlayerID, int nArgCode )
{
	LK_ASSERT( pPlayer, return );
	unsigned char nIsLstFull    = 0; // 被邀请者的好友列表满了没，因为要到邀请者所在的服务器进行处理(合法性)，所以要把这个值传走
	unsigned char nIsDesInMyLst = 0; // 邀请者是不是已经在被邀请者的好友列表中了，因为要到邀请者所在的服务器进行处理，所以要把这个值传走

	FriendType::iterator it = mRels.find( pPlayer->GetCharID() );
	if ( it != mRels.end() )
	{
		if ( it->second.GetFriendNum( ) >= FRIENDS_LIMIT )
			nIsLstFull = 1;

		if ( it->second.IsFriend( nSrcPlayerID ) )
			nIsDesInMyLst = 1;

		if ( it->second.IsBlack( nSrcPlayerID ) )
		{
			if ( it->second.DelBlack( nSrcPlayerID ) == 0 )
			{
				CMessage tMessage;
				CMessageChangeFriendNotice msg5;
				msg5.set_charid( nSrcPlayerID );
				msg5.set_reltome( YOUR_BLACKLIST );
				msg5.set_oprcode( DEL_BLACK );

				CEntityPlayer* tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( nSrcPlayerID );
				if ( tpPlayer != NULL )
				{
					msg5.set_vipflag( ((CPropertyPlayer*)tpPlayer->GetProperty())->GetVipFlag() );
				}				

				MakeRouterMessageByID<CMessageChangeFriendNotice>( pPlayer->GetCharID(), pPlayer->GetCharID(), 1
					, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg5, &tMessage );

				CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
			}
			else
			{
				LOG_ERROR( "FRIEND", "remove blacklist failded (%d del %d)", pPlayer->GetCharID(), nSrcPlayerID );
			}
		}
	}	

	CEntityPlayer* pInvitingPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( nSrcPlayerID );
	if ( pInvitingPlayer != NULL ) // 在本场景服务器上
	{
		PlayerReplyAddFriend( nSrcPlayerID, pPlayer->GetCharID(), pPlayer->GetCharName()
			, pPlayer->GetFaceType(), pPlayer->GetSex(), nArgCode, nIsLstFull, nIsDesInMyLst, pPlayer->GetNationality() );

		LOG_DEBUG("FRIEND", "be invited player %d(%s), invite %d", pPlayer->GetCharID(), pPlayer->GetCharNameShow()
			, pInvitingPlayer->GetCharID() );
	}
	else
	{
		// 发送路由消息到邀请者所在的服务器处理
		CMessageInviteFriendResponseMsg msg; // 发到邀请者玩家
		CMessage tMessage;

		msg.set_beinvitingplayername( pPlayer->GetCharName() );
		msg.set_agreecode( nArgCode );
		msg.set_isfriendlstfull( nIsLstFull );
		msg.set_isdesinmylst( nIsDesInMyLst );
		msg.set_beinvitingplayersex( pPlayer->GetSex() );
		msg.set_beinvitingplayerhead( pPlayer->GetFaceType() );
		msg.set_beinvitingplayernation( pPlayer->GetNationality() );

		MakeRouterMessageByID<CMessageInviteFriendResponseMsg>( pPlayer->GetCharID(), nSrcPlayerID, 0
			, ID_S2S_FRIEND_INVITE_RESPONSE_R, msg, &tMessage );
		// 已经判断过 肯定不在本服务器
		CSceneLogic::GetSingleton( ).Send2Player( nSrcPlayerID, &tMessage );
	}
}



// 
// ***********************************************************
//  Function:		SendErrorCode	
//  Description:	发错误码 (目标玩家必须在本服务器)
//  Input:			实体ID，错误码
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.10.27
// **********************************************************
void CFriendModule::SendErrorCode ( int nCharID, EFriendModuleErrorCode ecode )
{
	CMessageFriendErrorNotice msg; // 发到邀请者玩家
	CMessage tMessage;
	msg.set_errorcode( ecode );
	MakeRouterMessageByID<CMessageFriendErrorNotice>( nCharID, nCharID, 0, ID_S2C_FRIEND_ERRCODE_NOTICE, msg, &tMessage );
	CSceneLogic::GetSingleton( ).Send2Player( nCharID, &tMessage );
}



// 
// ***********************************************************
//  Function:		OnMessageAddFriendOffLine	
//  Description:	要添加的好友不在线(用于回复同意不同意后)
//  Input:			发起者ID，好友的名字
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.10.28
// **********************************************************
void CFriendModule::OnMessageAddFriendOffLine( int nRequesterID, const char* szDesName )
{
	SendErrorCode( nRequesterID, ERROR_DES_PLAYER_IS_NOT_ONLINE );
}



// 
// ***********************************************************
//  Function:		AskGateFriendInfo	
//  Description:	问gate要好友和仇敌的状态
//  Input:			发起者ID，好友列表项
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.10.28
// **********************************************************
void CFriendModule::AskGateFriendInfo( int nRequesterID, const CFriendItem& rItem, bool bIsRefresh )
{
	int nTotal = 0;
	CMessagePlayerOnLineFriendRequest msg;
	msg.set_requester( nRequesterID );

	// 取好友
	if ( bIsRefresh ) // 是刷新情况下
		msg.set_flag( FLAG_GETFRIEND_STATE );
	else
		msg.set_flag( FLAG_GETFRIEND_ALL );

	std::vector<int> friend_list;
	int nRet = rItem.GetFriendCharIDList( friend_list );
	if ( nRet != 0 )
	{
		LOG_ERROR( "FRIEND", "GetFriendCharIDList ERROR ,ret=%d", nRet );
		return;
	}

	CMessage tMessage;
	std::vector<int>::iterator it = friend_list.begin() ;
	int n = 0;
	nTotal += friend_list.size();
	for ( ; it != friend_list.end(); ++it, ++n )
	{
		if ( n >= MAX_ONCE_DISPOS )//(int)CSceneCfgManager::GetSingletonPtr()->GetFriend_cfg()->mMaxoncefriends )
		{
			tMessage.set_msgpara((uint32_t) &msg);
			tMessage.mutable_msghead()->set_messageid( ID_S2G_PLAYER_ONLINE_FRIEND_REQUEST );
			CSceneLogic::GetSingleton( ).Send2Gate( &tMessage );

			msg.clear_charids();
			n = 0;
		}
		msg.add_charids( *it );
	}

	if ( n > 0 )
	{
		tMessage.set_msgpara((uint32_t) &msg);
		tMessage.mutable_msghead()->set_messageid( ID_S2G_PLAYER_ONLINE_FRIEND_REQUEST );
		CSceneLogic::GetSingleton( ).Send2Gate( &tMessage );
	}

	// 取仇敌

	if ( bIsRefresh ) // 是刷新情况下
		msg.set_flag( FLAG_GETENEMY_STATE );
	else
		msg.set_flag( FLAG_GETENEMY_ALL );

	friend_list.clear();
	rItem.GetEnemyCharIDList( friend_list );

	msg.clear_charids();
	it = friend_list.begin() ;
	n = 0;
	nTotal += friend_list.size();

	for ( ; it != friend_list.end(); ++it, ++n )
	{
		if ( n >= MAX_ONCE_DISPOS )//(int)CSceneCfgManager::GetSingletonPtr()->GetFriend_cfg()->mMaxoncefriends )
		{
			tMessage.set_msgpara((uint32_t) &msg);
			tMessage.mutable_msghead()->set_messageid( ID_S2G_PLAYER_ONLINE_FRIEND_REQUEST );
			CSceneLogic::GetSingleton( ).Send2Gate( &tMessage );

			msg.clear_charids();
			n = 0;
		}
		msg.add_charids( *it );
	}

	if ( n > 0 )
	{
		tMessage.set_msgpara((uint32_t) &msg);
		tMessage.mutable_msghead()->set_messageid( ID_S2G_PLAYER_ONLINE_FRIEND_REQUEST );
		CSceneLogic::GetSingleton( ).Send2Gate( &tMessage );
	}

	if ( !bIsRefresh )
	{
		// 取黑名单

		msg.set_flag( FLAG_GETBLACK );
		// int nRead = 0; // 已经处理了的数量
		int nBlack = 0;
		int arrBlacks[BLACKLIST_LIMIT] = {0};
		rItem.GetBlackCharIDList( (int*)arrBlacks, nBlack );

		msg.clear_charids();
		n = 0;
		nTotal += nBlack;

		for ( int i = 0; i < nBlack; ++i, ++n )
		{
			if ( n >= MAX_ONCE_DISPOS )// (int)CSceneCfgManager::GetSingletonPtr()->GetFriend_cfg()->mMaxoncefriends )
			{
				tMessage.set_msgpara((uint32_t) &msg);
				tMessage.mutable_msghead()->set_messageid( ID_S2G_PLAYER_ONLINE_FRIEND_REQUEST );
				CSceneLogic::GetSingleton( ).Send2Gate( &tMessage );

				msg.clear_charids();
				n = 0;
			}
			msg.add_charids( arrBlacks[i] );
		}

		if ( n > 0 )
		{
			tMessage.set_msgpara((uint32_t) &msg);
			tMessage.mutable_msghead()->set_messageid( ID_S2G_PLAYER_ONLINE_FRIEND_REQUEST );
			CSceneLogic::GetSingleton( ).Send2Gate( &tMessage );
		}
	}

	// 如果以上全是空 则不给网关发消息 但是要把分组名字发给客户端 【或者其他全局信息】
	if (nTotal == 0 && !bIsRefresh)
	{
		CMessageFriendGetListNotice msg;

		for ( int i = 1; i <= PLAYER_GROUP_COUNT; ++i )
		{
			msg.add_groupname( rItem.GetGroupName(i) );
		}
		CMessage tMessage;
		tMessage.set_msgpara((uint32_t) &msg);
		tMessage.mutable_msghead()->set_messageid( ID_S2C_FRIEND_GETLIST_NOTICE );
		CSceneLogic::GetSingleton( ).Send2Player( nRequesterID, &tMessage );
	}

}

// 
// ***********************************************************
//  Function:		OnMessageRecvOnlineFriends	
//  Description:	处理gate发回来的在线好友的信息
//  Input:			消息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.10.26
// **********************************************************
void CFriendModule::OnMessageRecvOnlineFriends( CMessage* pMsg )
{
	LK_ASSERT( pMsg, return );
	CMessagePlayerOnLineFriendResponse* pMessage = (CMessagePlayerOnLineFriendResponse*) pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	if ( pMessage->friends_size() > MAX_ONCE_DISPOS )//(int)CSceneCfgManager::GetSingletonPtr()->GetFriend_cfg()->mMaxoncefriends )
	{
		LOG_ERROR( "FRIEND", "recv friends from gate error ,get %d lines", pMessage->friends_size() );
		return;
	}

	FriendType::iterator it = mRels.find( pMessage->requester() );
	if ( it == mRels.end() )
	{
		LOG_ERROR( "FRIEND", "error in OnMessageRecvOnlineFriends, id=%d", pMessage->requester() );
		return;
	}


	std::vector<int> tList;


	if ( pMessage->flag() == FLAG_GETFRIEND_STATE ) // 如果是好友 状态 说明是强制刷新调用的
	{
		CMessageFriendGetFriendsStateNotice msg;
		for ( int i = 0 ; i < pMessage->friends_size() ; ++i )
		{
			const PBSinglePlayerInfo friend_info = pMessage->friends(i);
			if (friend_info.noexist() == 1)
			{
					LOG_DEBUG( "FRIEND", "好友已经删除了账号" );
					it->second.DelFriend(friend_info.charid());
					continue;
			}
			
			msg.add_charids(friend_info.charid());
			msg.add_rels(YOUR_FRIEND);
			msg.add_states(friend_info.status());
			msg.add_vipflag( friend_info.vipflag() );
			it->second.SetFriendStatus( friend_info.charid(), ( EPlayerShow )friend_info.status() );
		}
		CMessage tMessage;
		tMessage.set_msgpara((uint32_t) &msg);
		tMessage.mutable_msghead()->set_messageid( ID_S2C_FRIEND_GETFRIENDSSTATE_NOTICE );
		CSceneLogic::GetSingleton( ).Send2Player( pMessage->requester(), &tMessage );
	}
	else if ( pMessage->flag() == FLAG_GETENEMY_STATE ) 
	{
		CMessageFriendGetFriendsStateNotice msg;
		for ( int i = 0 ; i < pMessage->friends_size() ; ++i )
		{
			const PBSinglePlayerInfo friend_info = pMessage->friends(i);
			if (friend_info.noexist() ==1)
			{
				LOG_DEBUG( "FRIEND", "好友已经删除了账号 id=%d" ,friend_info.charid());
				it->second.DelEnemy(friend_info.charid());
				continue;
			}
			msg.add_charids(friend_info.charid());
			msg.add_rels(YOUR_ENEMY);
			msg.add_states(friend_info.status());
			msg.add_vipflag( friend_info.vipflag() );
			it->second.SetEnemyState( friend_info.charid(), ( EPlayerShow )friend_info.status() );
		}
		CMessage tMessage;
		tMessage.set_msgpara((uint32_t) &msg);
		tMessage.mutable_msghead()->set_messageid( ID_S2C_FRIEND_GETFRIENDSSTATE_NOTICE );
		CSceneLogic::GetSingleton( ).Send2Player( pMessage->requester(), &tMessage );
	}
	else if ( pMessage->flag() == FLAG_GETFRIEND_ALL ) // 好友 全部 说明是登陆时候取的
	{
		CMessageFriendGetListNotice msg;
		for ( int i = 0 ; i < pMessage->friends_size() ; ++i )
		{
			st_friend tmp;
			const PBSinglePlayerInfo friend_info = pMessage->friends(i);
			if (friend_info.noexist() == 1)
			{
				LOG_DEBUG( "FRIEND", "好友已经删除了账号 id=%d" ,friend_info.charid());
				it->second.DelFriend(friend_info.charid());
				continue;
			}
			else if ( it->second.GetFriend( friend_info.charid(), tmp) == 0 ) 
			{
				SingleFriendData* pData = msg.add_friends();
				LK_ASSERT(pData, continue);
				
				pData->set_charid( friend_info.charid() );
				pData->set_name( friend_info.name().c_str() );
				pData->set_head( friend_info.face() );
				pData->set_sex( friend_info.sex() );
				pData->set_relation( tmp.enRel2Me );
				pData->set_status( friend_info.status());
				pData->set_groupid( tmp.nGroupId );
				pData->set_nationid( friend_info.nation() );
				pData->set_vipflag( friend_info.vipflag() ); 
				pData->set_intimacydegree( tmp.nIntimacyDegree );
				it->second.SetFriendStatus( friend_info.charid(), ( EPlayerShow )friend_info.status() );
				if ( friend_info.status() != AT_OFFLINE )
					tList.push_back( friend_info.charid() );
			}
			else
			{
				LOG_ERROR( "FRIEND", "no this friend! id=%d src=%d", friend_info.charid(), pMessage->requester() );
				continue;
			}
		}
		for ( int i = 1; i <= PLAYER_GROUP_COUNT; ++i )
		{
			msg.add_groupname( it->second.GetGroupName(i) );
		}
		CMessage tMessage;
		tMessage.set_msgpara((uint32_t) &msg);
		tMessage.mutable_msghead()->set_messageid( ID_S2C_FRIEND_GETLIST_NOTICE );
		CSceneLogic::GetSingleton( ).Send2Player( pMessage->requester(), &tMessage );
	}
	else if( pMessage->flag() == FLAG_GETENEMY_ALL )
	{
		CMessageFriendGetListNotice msg;
		for ( int i = 0 ; i < pMessage->friends_size() ; ++i )
		{
			st_enemy tmp;
			const PBSinglePlayerInfo friend_info = pMessage->friends(i);
			if (friend_info.noexist() == 1)
			{
				LOG_DEBUG( "FRIEND", "好友已经删除了账号" );
				it->second.DelEnemy(friend_info.charid());
				continue;
			}
			else if ( it->second.GetEnemy( friend_info.charid(), tmp ) == 0 )
			{
				SingleFriendData* pData = msg.add_friends();
				LK_ASSERT(pData, continue);
				pData->set_charid( friend_info.charid() );
				pData->set_name( friend_info.name().c_str() );
				pData->set_head( friend_info.face() );
				pData->set_sex( friend_info.sex() );
				pData->set_relation( YOUR_ENEMY );
				pData->set_status( friend_info.status() );
				pData->set_groupid( PLAYER_GROUP_COUNT + 2 );
				pData->set_islock( tmp.nIsLocked );
				pData->set_killed( tmp.nKilled );
				pData->set_bekilled( tmp.nBeKilled );
				pData->set_timeorder( tmp.nKillTime );
				pData->set_nationid( friend_info.nation() );
				pData->set_vipflag( friend_info.vipflag() ); 
				it->second.SetEnemyState( friend_info.charid(), ( EPlayerShow )friend_info.status() );
				if ( friend_info.status() != AT_OFFLINE )
					tList.push_back( friend_info.charid() );
			}
			else
			{
				LOG_ERROR( "FRIEND", "no this enemy! id=%d src=%d", friend_info.charid(), pMessage->requester() );
				continue;
			}
		}
		for ( int i = 1; i <= PLAYER_GROUP_COUNT; ++i )
		{
			msg.add_groupname( it->second.GetGroupName(i) );
		}
		CMessage tMessage;
		tMessage.set_msgpara((uint32_t) &msg);
		tMessage.mutable_msghead()->set_messageid( ID_S2C_FRIEND_GETLIST_NOTICE );
		CSceneLogic::GetSingleton( ).Send2Player( pMessage->requester(), &tMessage );
	}
	else if( pMessage->flag() == FLAG_GETBLACK )  // 黑名单
	{
		CMessageFriendGetListNotice msg;
		for ( int i = 0 ; i < pMessage->friends_size() ; ++i )
		{
			SingleFriendData* pData = msg.add_friends();
			const PBSinglePlayerInfo friend_info = pMessage->friends(i);
			LK_ASSERT(pData, continue);
			pData->set_charid( friend_info.charid() );
			pData->set_name( friend_info.name().c_str() );
			pData->set_head( friend_info.face() );
			pData->set_sex( friend_info.sex() );
			pData->set_relation( YOUR_BLACKLIST );
			pData->set_status( AT_OFFLINE );
			pData->set_groupid( PLAYER_GROUP_COUNT + 1 );
			pData->set_nationid( friend_info.nation() );
			pData->set_vipflag( friend_info.vipflag() ); 			
		}

		for ( int i = 1; i <= PLAYER_GROUP_COUNT; ++i )
		{
			msg.add_groupname( it->second.GetGroupName(i) );
		}
		CMessage tMessage;
		tMessage.set_msgpara((uint32_t) &msg);
		tMessage.mutable_msghead()->set_messageid( ID_S2C_FRIEND_GETLIST_NOTICE );
		CSceneLogic::GetSingleton( ).Send2Player( pMessage->requester(), &tMessage );
	}
	else if( pMessage->flag() == FLAG_GETCONTACTER )
	{
		CMessageFriendGetListNotice msg;
		for ( int i = 0 ; i < pMessage->friends_size() ; ++i )
		{
			SingleFriendData* pData = msg.add_friends();
			const PBSinglePlayerInfo friend_info = pMessage->friends(i);
			LK_ASSERT(pData, continue);
			pData->set_charid( friend_info.charid() );
			pData->set_name( friend_info.name().c_str() );
			pData->set_head( friend_info.face() );
			pData->set_sex( friend_info.sex() );
			pData->set_relation(YOUR_LATEST_CONTACTER);
			pData->set_status(friend_info.status());
			//pData->set_groupid( PLAYER_GROUP_COUNT + 1 );
			pData->set_nationid( friend_info.nation() );
			pData->set_vipflag( friend_info.vipflag() ); 			
		}

		CMessage tMessage;
		tMessage.set_msgpara((uint32_t) &msg);
		tMessage.mutable_msghead()->set_messageid( ID_S2C_FRIEND_GETLIST_NOTICE );
		CSceneLogic::GetSingleton( ).Send2Player( pMessage->requester(), &tMessage );
	}
	
	if ( pMessage->flag() == FLAG_GETFRIEND_ALL ) // 通知好友我的状态
		SendMyStatus2Friends( pMessage->requester(), tList, AT_ONLINE, YOUR_FRIEND );
	else if ( pMessage->flag() == FLAG_GETENEMY_ALL)
		SendMyStatus2Friends( pMessage->requester(), tList, AT_ONLINE, YOUR_ENEMY );
}


// 
// ***********************************************************
//  Function:		SendMyStatus2Friends	
//  Description:	给我的好友列表里在线的人发送我的状态(用于上线和改变状态之后)
//  Input:			ID,列表，状态
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.10.27
// **********************************************************
void CFriendModule::SendMyStatus2Friends( int nMyCharID, const std::vector<int>& rList, unsigned char nStat, int nRels )
{
	CMessageFriendChangeStatusNotice_R msg; 
	CMessage tMessage;
	msg.set_status( nStat );
	msg.set_rels(nRels);

	CEntityPlayer* tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( nMyCharID );
	if ( tpPlayer != NULL )
	{
		msg.set_vipflag( ((CPropertyPlayer*)tpPlayer->GetProperty())->GetVipFlag() );
	}	

	msg.mutable_router()->set_srccharid( nMyCharID  );
	msg.mutable_router()->set_send2player( 0 );
	msg.mutable_router()->set_routertype( CMessageRouter::ROUTER_CHAR );
	tMessage.mutable_msghead()->set_messageid( ID_S2S_FRIEND_CHANGESTATUS_NOTICE_R );

	for ( std::vector<int>::const_iterator it = rList.begin(); it != rList.end(); ++it )
	{
		msg.mutable_router()->set_descharid( *it ); // 目标ID
		tMessage.set_msgpara((uint32_t) &msg);
		if ( CSceneLogic::GetSingleton( ).GetPlayerByCharID(*it) == NULL ) // 不在本场景
			CSceneLogic::GetSingleton( ).Send2Player( *it, &tMessage );
		else
			OnRouterMessage( &tMessage );
	}

}


// 
// ***********************************************************
//  Function:		SendMyStatus2Gate	
//  Description:	通知网关， 我改状态了
//  Input:			ID,状态
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.10.27
// **********************************************************
void CFriendModule::SendMyStatus2Gate( int nMyCharID, unsigned char nStat )
{
	CMessagePlayerChangeStatusNotice msg; 

	CMessage tMessage;
	
	msg.set_charid( nMyCharID );
	msg.set_status( nStat );

	tMessage.mutable_msghead()->set_messageid( ID_S2G_PLAYER_CHANGE_STATUS_NOTICE );

	tMessage.set_msgpara((uint32_t) &msg);
	CSceneLogic::GetSingleton( ).Send2Gate( &tMessage );
}

// 
// ***********************************************************
//  Function:		OnMessagePlayerChangeStatus	
//  Description:	客户端请求改变状态
//  Input:			消息,玩家
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.10.28
// **********************************************************
void CFriendModule::OnMessagePlayerChangeStatus( CEntityPlayer* pPlayer, CMessage* pMsg )
{
	LK_ASSERT( pPlayer, return );
	LK_ASSERT( pMsg, return );
	CMessageFriendChangeStatusRequest* pMessage = ( CMessageFriendChangeStatusRequest* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );
	unsigned char nStatus = pMessage->status();

	FriendType::iterator it = mRels.find( pPlayer->GetCharID() );
	if ( it == mRels.end() )
	{
		return;
	}

	if ( nStatus == it->second.GetMyStatus() )
		return;

	if ( nStatus != AT_OFFLINE )
		it->second.SetMyStatus( (EPlayerShow) nStatus );

	SendMyStatus2Gate( pPlayer->GetCharID(), nStatus );
	std::vector<int> friend_list;
	// 取得在线列表
	it->second.GetFriendCharIDList( friend_list, true );
	friend_list.push_back( pPlayer->GetCharID() );
	SendMyStatus2Friends( pPlayer->GetCharID(), friend_list, nStatus, YOUR_FRIEND );

	friend_list.clear();
	it->second.GetEnemyCharIDList( friend_list, true );
	SendMyStatus2Friends( pPlayer->GetCharID(), friend_list, nStatus, YOUR_ENEMY );
}


// ***********************************************************
//  Function:		OnMessageFriendChangeStatusNotice	
//  Description:	处理经过路由转发的改变好友状态的通知
//  Input:			路由消息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.10.29
// **********************************************************
void CFriendModule::OnMessageFriendChangeStatusNotice( CMessage* pMsg )
{
	LK_ASSERT( pMsg, return );
	CMessageFriendChangeStatusNotice_R* pMessage = ( CMessageFriendChangeStatusNotice_R* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	FriendType::iterator it = mRels.find( pMessage->router().descharid() );
	if ( it == mRels.end() )
	{
		return;
	}
	else
	{
		if ( pMessage->rels() == YOUR_ENEMY )
			it->second.SetEnemyState( pMessage->router().srccharid(), (EPlayerShow)pMessage->status() ); 
		else
			it->second.SetFriendStatus( pMessage->router().srccharid(), (EPlayerShow)pMessage->status() ); 
	}

	// 发给客户端
	CMessageFriendChangeStatusNotice msg;
	CMessage tMessage;

	msg.set_charid( pMessage->router().srccharid() );
	msg.set_status( pMessage->status() );
	msg.set_rels( pMessage->rels() );
	msg.set_vipflag( pMessage->vipflag() );

	tMessage.mutable_msghead()->set_messageid( ID_S2C_FRIEND_CHANGESTATUS_NOTICE );
	tMessage.set_msgpara((uint32_t) &msg);

	CEntityPlayer* pPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( pMessage->router().descharid() );
	if (pPlayer != NULL)
		CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
}

// ***********************************************************
//  Function:		OnReplayerAddRequesterInServer	
//  Description:	收到服务器发来的通知，把好友邀请者的信息记录到被邀请者的数据结构中 还要处理仇敌
//  Input:			消息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.11.5
// **********************************************************
void CFriendModule::OnReplayerAddRequesterInServer( CMessage* pMsg )
{
	LK_ASSERT( pMsg, return );
	CMessageAddFriendNotice* pMessage = (CMessageAddFriendNotice*)pMsg->msgpara();
	LK_ASSERT( pMessage != NULL, return );

	CMessageRouter* pRouter = pMessage->mutable_router();
	LK_ASSERT (pRouter != NULL , return );

	if ( pRouter->send2player() != 0 ) // 如果不发给客户端的话才会处理 TODO:以后可以去掉
	{
		return;
	}

	FriendType::iterator it = mRels.find( pRouter->descharid() );
	if ( it == mRels.end() )
	{
		//CFriendItem tmp;
		//tmp.AddFriend( pMessage->charid(), (EPlayerShow)pMessage->status(), (EPlayerRels)pMessage->reltome()
		//	, pMessage->groupid() );
		////AddNewFriendItem( pRouter->descharid(), tmp ) ;
		LOG_ERROR("FRIEND", "can't find rels @1133 charid=%d", pRouter->descharid() );
		return;
	}
	else
	{
		it->second.AddFriend( pMessage->charid(), (EPlayerShow)pMessage->status(), (EPlayerRels)pMessage->reltome()
			, pMessage->groupid() );
		// 看看新加的好友是不是自己的仇敌 是就干掉
		if (0 == it->second.DelEnemy(pMessage->charid()))
		{
			// 通知客户端 干掉仇敌
			CMessageChangeFriendNotice msg2;
			msg2.set_charid( pMessage->charid() );
			msg2.set_oprcode( DEL_ENEMY );
			msg2.set_reltome( YOUR_ENEMY );
			CMessage tMessage2;
			MakeRouterMessageByID<CMessageChangeFriendNotice>( pMessage->charid() , pRouter->descharid() 
				, 1, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg2, &tMessage2 );
			CSceneLogic::GetSingleton( ).Send2Player( pRouter->descharid(), &tMessage2 );
		}
	}
}

// ***********************************************************
//  Function:		OnMessagePlayerDelFriends	
//  Description:	删除好友，删除完毕后不给客户端回应
//  Input:			玩家 ， 消息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.11.7
// **********************************************************
void CFriendModule::OnMessagePlayerDelFriends( CEntityPlayer* pPlayer, CMessage* pMsg )
{
	LK_ASSERT( pPlayer, return );
	LK_ASSERT( pMsg, return );
	CMessageDelFrientRequest* pMessage = (CMessageDelFrientRequest*)pMsg->msgpara();
	LK_ASSERT( pMessage, return );
	DelMember( pPlayer, pMessage->charid(), pMessage->type() );
}

// ***********************************************************
//  Function:		DelMember	
//  Description:	玩家删除好友 黑名单等
//  Input:			玩家 ，目标角色ID， 操作类型
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2009.4.8
// **********************************************************
void CFriendModule::DelMember( CEntityPlayer* pPlayer, int nDesCharID, int oprType )
{
	LK_ASSERT( pPlayer, return );
	FriendType::iterator it = mRels.find( pPlayer->GetCharID() );
	LK_ASSERT( it != mRels.end(), return );

	if ( oprType == TYPE_FRIEND )
	{	
		// 判断是否是结婚关系
		if ( pPlayer->GetMarriageID() != 0 )
		{
			CMarriageInfo *tpInfo = CMarriageManager::GetSingleton().GetMarriageInfoByID( pPlayer->GetNationality(), pPlayer->GetMarriageID() );
			if ( tpInfo != NULL )
			{
				if ( tpInfo->GetBrideID() == nDesCharID || tpInfo->GetGroomID() == nDesCharID )
				{
					CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_DELFRIEND_SPOURSE );
					return;
				}				
			}			
		}
		
		if ( pPlayer->GetSwornID() != 0 )
		{
			CSwornTeam *tpSwornTeam = GetSwornTeam( pPlayer->GetSwornID() );
			if ( tpSwornTeam != NULL )
			{
				if ( tpSwornTeam->HasMember( nDesCharID ) )
				{
					CPropertyModule::GetSingleton().SendProErrorNotice( pPlayer, ERROR_DELFRIEND_INSAMESWORNTEAM );					
					return;
				}					  
			}			
		}
		
		st_friend tmpfriend;
		it->second.GetFriend( nDesCharID, tmpfriend );
		if ( it->second.DelFriend( nDesCharID ) == 0 )
		{
			CMessage tMessage;
			CMessageChangeFriendNotice msg5;
			msg5.set_charid( nDesCharID );
			msg5.set_reltome( tmpfriend.enRel2Me );
			msg5.set_oprcode( DEL_FRIEND );

			MakeRouterMessageByID<CMessageChangeFriendNotice>( pPlayer->GetCharID(), pPlayer->GetCharID(), 1
				, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg5, &tMessage );

			CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
		}
	}
	else if( oprType == TYPE_BLACK )
	{
		if ( it->second.DelBlack( nDesCharID ) == 0 )
		{
			CMessage tMessage;
			CMessageChangeFriendNotice msg5;
			msg5.set_charid( nDesCharID );
			msg5.set_reltome( YOUR_BLACKLIST );
			msg5.set_oprcode( DEL_BLACK );

			MakeRouterMessageByID<CMessageChangeFriendNotice>( pPlayer->GetCharID(), pPlayer->GetCharID(), 1
				, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg5, &tMessage );

			CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
		}
	}
	else if( oprType == TYPE_EMEMY )
	{
		if ( it->second.DelEnemy( nDesCharID ) == 0 )
		{
			CMessage tMessage;
			CMessageChangeFriendNotice msg5;
			msg5.set_charid( nDesCharID );
			msg5.set_reltome( YOUR_ENEMY );
			msg5.set_oprcode( DEL_ENEMY );

			MakeRouterMessageByID<CMessageChangeFriendNotice>( pPlayer->GetCharID(), pPlayer->GetCharID(), 1
				, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg5, &tMessage );

			CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
		}
	}
}

// ***********************************************************
//  Function:		OnMessagePlayerChangeGroupName	
//  Description:	玩家修改组名
//  Input:			玩家 ， 消息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.11.19
// **********************************************************
void CFriendModule::OnMessagePlayerChangeGroupName( CEntityPlayer* pPlayer, CMessage* pMsg )
{
	LK_ASSERT( pPlayer, return );
	LK_ASSERT( pMsg, return );
	CMessageFriendChgGroupNameRequest* pMessage = (CMessageFriendChgGroupNameRequest*)pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	FriendType::iterator it = mRels.find( pPlayer->GetCharID() );
	CFriendItem tmp;
	LK_ASSERT( it != mRels.end(), return );

	CMessage tMessage;
	if ( pMessage->has_newname() != true || pMessage->newname().c_str() == NULL )
	{
		return;
	}
	if ( pMessage->newname().length() > GROUP_NAME_LENGTH - 1 )
	{
		CMessageFriendErrorNotice msg2;
		msg2.set_errorcode( ERROR_GROUP_NAME_TOO_LONG );

		MakeRouterMessageByID<CMessageFriendErrorNotice>( pPlayer->GetCharID(), pPlayer->GetCharID(), 0
			, ID_S2C_FRIEND_ERRCODE_NOTICE, msg2, &tMessage );

		CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
		return ;
	}

	CDirtyWord::GetSingleton().OI_Dirty_Check( 1, ( char * )pMessage->newname().c_str() );
	if ( it->second.SetGroupName( pMessage->groupid(), pMessage->newname().c_str() ) != 0 )
		return;

	//SaveGlobalInfo( pPlayer->mCharProfile.mCharID, *pItem );

	CMessageFriendGroupNameNotice msg3;
	msg3.add_groupid( pMessage->groupid() );
	msg3.add_groupname( pMessage->newname().c_str() );
	tMessage.set_msgpara((uint32_t) &msg3);
	tMessage.mutable_msghead()->set_messageid( ID_S2C_FRIEND_GROUPNAME_NOTICE );
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
}


// ***********************************************************
//  Function:		OnMessagePlayerChangeFriendGroup	
//  Description:	玩家移动好友到其他分组
//  Input:			玩家 ， 消息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.11.20
// **********************************************************

void	CFriendModule::OnMessagePlayerChangeFriendGroup( CEntityPlayer* pPlayer, CMessage* pMsg )
{
	LK_ASSERT( pPlayer, return );
	CMessageFriendChgFriendGroupRequest* pMessage = ( CMessageFriendChgFriendGroupRequest* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	FriendType::iterator it = mRels.find( pPlayer->GetCharID() );
	if ( it == mRels.end() )
	{
		LOG_ERROR( "FRIEND", "error in load friend rel in line=%d", __LINE__ );
		return;
	}
	
	if ( it->second.MoveFriend( pMessage->charid(), pMessage->desgroupid() ) != 0 )
	{
		SendErrorCode( pPlayer->GetCharID(), ERROR_MOVE_FRIEND_FAIL );
		return;
	}

	st_friend tmp;
	it->second.GetFriend( pMessage->charid(), tmp );

	//SaveFriend( pPlayer->mCharProfile.mCharID, tmp, false );

	// 发给客户端
	CMessage tMessage;
	CMessageFriendChgFriendGroupNotice msg4;
	msg4.set_charid( pMessage->charid() );
	msg4.set_desgroupid( pMessage->desgroupid() );
	tMessage.set_msgpara((uint32_t) &msg4);
	tMessage.mutable_msghead()->set_messageid( ID_S2C_FRIEND_CHGFRIENDGROUP_NOTICE );
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );

}


// ***********************************************************
//  Function:		OnMessagePlayerChangeFriendGroup	
//  Description:	发私聊消息
//  Input:			玩家 ， 消息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.11.20
// **********************************************************

void	CFriendModule::OnMessagePlayerSendMsg( CEntityPlayer* pPlayer, CMessage* pMsg )
{
	LK_ASSERT( pPlayer, return );
	LK_ASSERT( pMsg, return );
	CMessageFriendSendMessageRequest* pMessage = ( CMessageFriendSendMessageRequest* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );
	if ( !pMessage->has_msg() || pMessage->msg().c_str() == NULL )
		return;

	if ( pMessage->msg().length() > MAX_FRIEND_MSGBUF_LENGTH )
	{
		SendErrorCode( pPlayer->GetCharID(), ERROR_MESSAGE_TOO_LONG );
		return;
	}

	if ( pMessage->charid() == pPlayer->GetCharID() )
	{
		return;
	}

	FriendType::iterator it = mRels.find( pPlayer->GetCharID() );
	LK_ASSERT ( it != mRels.end(), return );

	if ( it->second.IsBlack(pMessage->charid()) )
	{
		SendErrorCode( pPlayer->GetCharID(), ERROR_DES_IN_YOUR_BLACK );
		return;
	}

	CDirtyWord::GetSingleton().OI_Dirty_Check( 1, ( char * )pMessage->msg().c_str() );

	CMessage tMessage;
	CMessageFriendMessageNotice msg;
	msg.set_charid( pPlayer->GetCharID() );
	msg.set_name( pPlayer->GetCharName() );
	msg.set_sex( pPlayer->GetSex() );
	msg.set_msg( pMessage->msg().c_str() );
	msg.set_faceid( pPlayer->GetFaceType() );
	if ( pMessage->has_autoflag() )
		msg.set_autoflag( pMessage->autoflag() );

//	msg.mutable_router()->set_sendback( 1 );
	MakeRouterMessageByID<CMessageFriendMessageNotice>( pPlayer->GetCharID(), pMessage->charid(), 0
		, ID_S2C_FRIEND_MESSAGE_NOTICE_R, msg, &tMessage );

	if ( CSceneLogic::GetSingleton().GetPlayerByCharID( pMessage->charid() ) == NULL )
		CSceneLogic::GetSingleton( ).Send2Player( pMessage->charid(), &tMessage );
	else
		OnRouterMessage( &tMessage );

	// 更新最近联系人列表
	UpdateLatestContact(pPlayer->GetCharID(), pMessage->charid());
	//NotifyLastContacterToClient(pMessage->charid(), pPlayer->GetCharID());
}


// ***********************************************************
//  Function:		OnMessagePlayerMoveBlack	
//  Description:	移动好友到黑名单
//  Input:			玩家 ， 消息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.11.20
// **********************************************************
void	CFriendModule::OnMessagePlayerMoveBlack( CEntityPlayer* pPlayer, CMessage* pMsg )
{
	LK_ASSERT( pPlayer, return );
	CMessageFriendMoveToBlackRequest* pMessage = (CMessageFriendMoveToBlackRequest*)pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	FriendType::iterator it = mRels.find( pPlayer->GetCharID() );
	if ( it == mRels.end() )
	{
		LOG_ERROR( "FRIEND", "error in load friend rel in line=%d", __LINE__ );
		return;
	}	
	if ( pMessage->charid() == pPlayer->GetCharID() )
	{
		SendErrorCode( pPlayer->GetCharID(), ERROR_MOVE_BLACK_FAIL );
		return;
	}

	// 不能是结拜兄弟
	if ( pPlayer->GetSwornID() != 0 )
	{	
		CSwornTeam *tpSwornTeam = GetSwornTeam( pPlayer->GetSwornID() );
		if ( tpSwornTeam != NULL )
		{
			if ( tpSwornTeam->HasMember( pMessage->charid() ) )
			{
				SendErrorCode( pPlayer->GetCharID(), ERROR_MOVEBLACK_INSAMESWORNTEAM );
				return;
			}
		}		
	}	
	// 不能是夫妻
	if ( pPlayer->GetMarriageID() != 0 )
	{
		CMarriageInfo *tpMarriage = CMarriageManager::GetSingleton().GetMarriageInfoByID( pPlayer->GetNationality(), pPlayer->GetMarriageID() );
		if ( tpMarriage != NULL )
		{
			if ( tpMarriage->GetGroomID() == pMessage->charid() || tpMarriage->GetBrideID() == pMessage->charid() )
			{
				SendErrorCode( pPlayer->GetCharID(), ERROR_MOVEBLACK_MARRIED );
				return;
			}			
		}		
	}
	
	if ( it->second.AddBlack( pMessage->charid() ) != 0 )
	{
		SendErrorCode( pPlayer->GetCharID(), ERROR_MOVE_BLACK_FAIL );
		return;
	}
	CMessage tMessage;
	CMessageFriendMoveToBlackNotice msg;
	msg.set_charid( pMessage->charid() );
	tMessage.set_msgpara((uint32_t) &msg);
	tMessage.mutable_msghead()->set_messageid( ID_S2C_FRIEND_MOVETOBLACK_NOTICE );
	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );

	if (0 == it->second.DelFriend( pMessage->charid() ) )
	{
		CMessageChangeFriendNotice msg;
		CMessage tMessage;
		msg.set_charid( pMessage->charid() );
		msg.set_oprcode( DEL_FRIEND);

		MakeRouterMessageByID<CMessageChangeFriendNotice>( pPlayer->GetCharID(), pPlayer->GetCharID(), 0
			, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg, &tMessage );
		CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
	}

	//SaveFriend( pPlayer->GetCharID(), pMessage->charid(), YOUR_BLACKLIST, PLAYER_GROUP_COUNT + 1, false );
}


// ***********************************************************
//  Function:		OnMessagePlayerAddBlack	
//  Description:	添加黑名单
//  Input:			玩家 ， 消息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.11.21
// **********************************************************
void	CFriendModule::OnMessagePlayerAddBlack( CEntityPlayer* pPlayer, CMessage* pMsg )
{
	LK_ASSERT( pPlayer, return );
	LK_ASSERT( pMsg, return );
	CMessageFriendAddBlackRequest* pMessage = ( CMessageFriendAddBlackRequest* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );
	// 去数据库查资料
	if ( !pMessage->has_desname() )
	{
		return;
	}
	CMessage tMessage;
	if ( strcmp( pMessage->desname().c_str(), pPlayer->GetCharName() ) ==0 ) // 看看是不是在加自己
	{
		CMessageFriendErrorNotice msg;
		msg.set_errorcode( ERROR_ADDSELFTOBLACK );
		MakeRouterMessageByID<CMessageFriendErrorNotice>( pPlayer->GetCharID(), pPlayer->GetCharID(), 0, ID_S2C_FRIEND_ERRCODE_NOTICE
			, msg, &tMessage );
		CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
		return;
	}

	// 发给网关
	CMessageGetCharIDRequest msg;
	msg.set_srcentityid( pPlayer->GetEntityID() );
	msg.set_name( pMessage->desname().c_str() );
	msg.set_typeid_( TYPE_ADD_BLACK );
	tMessage.set_msgpara((uint32_t) &msg);
	tMessage.mutable_msghead()->set_messageid( ID_S2G_GETCHARID_REQUEST );
	CSceneLogic::GetSingleton( ).Send2Gate( &tMessage );
}

// ***********************************************************
//  Function:		AddBlackCallBack	
//  Description:	添加黑名单 数据库回调
//  Input:			SQL执行消息 请求者charid
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.11.21
// **********************************************************
void CFriendModule::AddBlackCallBack( CMessage* pMsg, int vparam1, int vparam2 )
{
}

// ***********************************************************
//  Function:		OnPlayerSendMessageRet	
//  Description:	玩家私聊返回消息
//  Input:			消息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.11.20
// **********************************************************
void CFriendModule::OnPlayerSendMessageRet( CMessage* pMsg )
{
	LK_ASSERT( pMsg, return );
	CMessageFriendMessageNotice* pMessage = (CMessageFriendMessageNotice*) pMsg->msgpara();
	LK_ASSERT( pMessage, return );
	CMessageRouter* pRouter = pMessage->mutable_router();
	LK_ASSERT( pRouter != NULL , return );

	if ( pRouter->sendback() == 1 ) // 是打回消息
	{
		if ( pRouter->routerreason() == CMessageRouter_EMRouterReason_REASON_OFFLINE 
			|| pRouter->routerreason() == CMessageRouter_EMRouterReason_REASON_REDIRECT )
		{	
			// 改为离线留言			
			CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( pRouter->srccharid() );
			if ( tpPlayer != NULL )
			{
				CMailModule::GetSingleton().SendOffLineChatMsg( tpPlayer, pRouter->descharid(), pMessage->mutable_msg()->c_str() );
			}		
			// SendErrorCode( pRouter->srccharid(), ERROR_DES_OFFLINE );
		}		
		return;
	}

	// 以下是非打回 在目标玩家的服务器上进行
	FriendType::iterator it = mRels.find( pRouter->descharid() );
	LK_ASSERT ( it != mRels.end(), return );

	if ( it->second.IsBlack( pRouter->srccharid() ) )
	{
		// SendErrorCode( pRouter->srccharid(), ERROR_YOU_ARE_BLACK );
		return;
	}
	else
	{
		// pRouter->set_send2player( 1 );
		CMessage tMessage = *pMsg;
		CSceneLogic::GetSingleton( ).Send2Player( pRouter->descharid(), &tMessage );

		// 更新最近联系人列表
		UpdateLatestContact(pRouter->descharid(), pRouter->srccharid());

		// 通知客户端更新最后联系人
		//NotifyLastContacterToClient(pRouter->srccharid(), pRouter->descharid());
		NotifyLastContacterToGate(pRouter->srccharid(), pRouter->descharid());
	}
}



// ***********************************************************
//  Function:		IsInBlack	
//  Description:	A在不在B的黑名单里？(为聊天模块做)(确保在B所在的服务器调用)
//  Input:			A,B的charid
//  OutPut:			NULL
//  Return:			bool 在的话返回true
//  Others:
//  Date:			2008.11.21
// **********************************************************

bool CFriendModule::IsInBlack( int nAcharid, int nBcharid )
{
	FriendType::iterator it = mRels.find( nBcharid );
	if ( it == mRels.end() )
	{
		return false;
	}

	return it->second.IsBlack( nAcharid );
}



// ***********************************************************
//  Function:		OnPlayerBeKilled	
//  Description:	玩家被杀死了
//  Input:			源 目标
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2009.7.14
// **********************************************************

void CFriendModule::OnPlayerBeKilled( CEntityPlayer* pSrcPlayer, CEntityPlayer* pDesPlayer )
{
	LK_ASSERT( pSrcPlayer, return );
	LK_ASSERT( pDesPlayer, return );

	// 先处理被杀者
	FriendType::iterator it = mRels.find( pDesPlayer->GetCharID() );
	LK_ASSERT( it != mRels.end(), return );

	if ( !it->second.IsFriend( pSrcPlayer->GetCharID() ) )
	{
		int nBeKilled = 0;
		int nKilled   = 0;
		int nDelCharID = 0;
		int nRet = it->second.OnBeKilled( pSrcPlayer->GetCharID(), nBeKilled, nKilled, nDelCharID );
		if ( nRet == 1 ) // 通知客户端移动到最前
		{
			CMessageChangeFriendNotice msg;
			msg.set_charid( pSrcPlayer->GetCharID() );
			msg.set_killed( nKilled );
			msg.set_bekilled( nBeKilled );
			msg.set_oprcode( ENEMY_MOVEFIRST );
			msg.set_timeorder( time(0) );
			msg.set_vipflag( ((CPropertyPlayer*)pSrcPlayer->GetProperty())->GetVipFlag() );
			CMessage tMessage;
			MakeRouterMessageByID<CMessageChangeFriendNotice>( pDesPlayer->GetCharID(), pDesPlayer->GetCharID()
				, 1, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg, &tMessage );
			CSceneLogic::GetSingleton( ).Send2Player( pDesPlayer, &tMessage );
		}
		else if ( nRet == 2 ) // 新加的
		{
			// 看是是不是要先删除老的仇敌
			if ( nDelCharID != 0 )
			{
				CMessageChangeFriendNotice msg2;
				msg2.set_charid( nDelCharID );
				msg2.set_oprcode( DEL_ENEMY );
				msg2.set_reltome( YOUR_ENEMY );
				CMessage tMessage2;
				MakeRouterMessageByID<CMessageChangeFriendNotice>( pDesPlayer->GetCharID(), pDesPlayer->GetCharID()
					, 1, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg2, &tMessage2 );
				CSceneLogic::GetSingleton( ).Send2Player( pDesPlayer, &tMessage2 );
			}

			// 通知客户端(添加)
			CMessageChangeFriendNotice msg;
			msg.set_charid( pSrcPlayer->GetCharID() );
			msg.set_name( pSrcPlayer->GetCharName() );
			msg.set_status( AT_ONLINE );
			msg.set_reltome( YOUR_ENEMY );
			msg.set_sex( pSrcPlayer->GetSex() );
			msg.set_head( pSrcPlayer->GetFaceType() );
			msg.set_oprcode( ADD_TO_ENEMY );
			msg.set_islock( 0 );
			msg.set_killed( 0 );
			msg.set_bekilled( nBeKilled );
			msg.set_timeorder( time(0) );
			msg.set_nationid( pSrcPlayer->GetNationality() );
			msg.set_vipflag( ((CPropertyPlayer*)pSrcPlayer->GetProperty())->GetVipFlag() );
			CMessage tMessage;
			MakeRouterMessageByID<CMessageChangeFriendNotice>( pDesPlayer->GetCharID(), pDesPlayer->GetCharID()
				, 1, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg, &tMessage );
			CSceneLogic::GetSingleton( ).Send2Player( pDesPlayer, &tMessage );
		}
	}
	// 被杀者处理完毕
	// 处理杀人者 先判断is仇敌
	it = mRels.find( pSrcPlayer->GetCharID() );
	LK_ASSERT ( it != mRels.end(), return );

	int nKillHim = 0;
	int nBeKilled= 0;
	if ( it->second.OnKilled( pDesPlayer->GetCharID(), nKillHim, nBeKilled ) )
	{
		CMessageChangeFriendNotice msg3;
		msg3.set_charid( pDesPlayer->GetCharID() );
		msg3.set_killed( nKillHim );
		msg3.set_bekilled( nBeKilled );
		msg3.set_oprcode( ENEMY_MOVEFIRST );
		msg3.set_timeorder( time(0) );
		msg3.set_vipflag( ((CPropertyPlayer*)pDesPlayer->GetProperty())->GetVipFlag() );
		CMessage tMessage3;
		MakeRouterMessageByID<CMessageChangeFriendNotice>( pSrcPlayer->GetCharID(), pSrcPlayer->GetCharID()
			, 1, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg3, &tMessage3 );
		CSceneLogic::GetSingleton( ).Send2Player( pSrcPlayer, &tMessage3 );
	}

	// 改变亲密度	
	ChangeIntimacy( pSrcPlayer->GetCharID(), pDesPlayer->GetCharID(), ENTIMACY_CHANGE_TYPE_KILL,
		mFriendBlessConfig.GetIntimacyConfig()->mKillReduced );
}


// ***********************************************************
//  Function:		OnMessagePlayerLockEnemy	
//  Description:	处理玩家锁定仇敌请求
//  Input:			玩家，消息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2008.12.8
// **********************************************************

void CFriendModule::OnMessagePlayerLockEnemy( CEntityPlayer* pPlayer, CMessage* pMsg )
{
	LK_ASSERT( pPlayer, return );
	LK_ASSERT( pMsg, return );
	CMessageFriendLockEnemyRequest* pMessage = (CMessageFriendLockEnemyRequest*)pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	FriendType::iterator it = mRels.find( pPlayer->GetCharID() );
	if ( it == mRels.end() )
	{
		LOG_ERROR( "FRIEND", "error in load friend rel in line=%d", __LINE__ );
		return;
	}

	int nRet = it->second.LockEnemy( pMessage->charid(), pMessage->locktype() );
	if ( nRet >= 0 )
	{
		CMessageChangeFriendNotice msg;
		msg.set_charid( pMessage->charid() );
		if ( pMessage->locktype() == LOCK_TYPE_LOCK )
			msg.set_oprcode( LOCK_ENEMY );
		else if ( pMessage->locktype() == LOCK_TYPE_FREE )
			msg.set_oprcode( UNLOCK_ENEMY );
		msg.set_islock( nRet );
		CMessage tMessage;
		MakeRouterMessageByID<CMessageChangeFriendNotice>( pPlayer->GetCharID(), pPlayer->GetCharID()
			, 1, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg, &tMessage );
		CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
	}
	else
	{
		SendErrorCode( pPlayer->GetCharID(), ERROR_LOCKENEMYFAIL );
	}

}


// ***********************************************************
//  Function:		OnMessagePlayerRereshFriendState	
//  Description:	玩家打开好友界面的时候，刷新好友的状态(最多X分钟允许刷新一次,X在配置里配)
//  Input:			玩家，消息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2009.1.4
// **********************************************************

void CFriendModule::OnMessagePlayerRereshFriendState( CEntityPlayer* pPlayer, CMessage* pMsg )
{
	LK_ASSERT( pPlayer, return );
	LK_ASSERT( pMsg, return );
	// CMessageFriendGetFriendsStateRequest* pMessage = (CMessageFriendGetFriendsStateRequest*)pMsg->msgpara();

	FriendType::iterator it = mRels.find( pPlayer->GetCharID() );
	
	if ( it == mRels.end() )
		return;

	if ( time(0) - it->second.GetLastRefresh() < CSceneCfgManager::GetSingleton().GetFriend_cfg()->mMaxrefreshtime )
		return;

	AskGateFriendInfo( pPlayer->GetCharID(), it->second, true );
	it->second.SetLastRefresh( time(0) );
}

// ***********************************************************
//  Function:		DisposeAddFriend	
//  Description:	处理加好友 
//  Input:			玩家，消息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2009.1.4
// **********************************************************
void CFriendModule::DisposeAddFriend( CEntityPlayer* pPlayer, int nDesID )
{
	LK_ASSERT( pPlayer, return );
	// 检查是不是已经是好友了
	FriendType::iterator it = mRels.find( pPlayer->GetCharID() );
	if ( it != mRels.end() )
	{
		if ( it->second.IsFriend( nDesID ) ) // 已经是好友了
		{
			SendErrorCode( pPlayer->GetCharID(), ERROR_ISFRIEND );
			return;
		}
		else if( it->second.IsBlack( nDesID ) ) // 是黑名单
		{
			DelMember( pPlayer, nDesID, TYPE_BLACK ); // 先干掉
		}
	}
	else
		return;

	// 发给网关 加好友请求
	CMessageAddFriendRequest msg;
	msg.set_srcid( pPlayer->GetCharID() );
	msg.set_desid( nDesID );
	msg.set_srclevel( pPlayer->GetLevel() );

	CMessage tMessage;
	tMessage.set_msgpara((uint32_t) &msg);
	tMessage.mutable_msghead()->set_messageid( ID_S2G_ADDFRIEND_REQUEST );
	CSceneLogic::GetSingleton( ).Send2Gate( &tMessage );
}

// ***********************************************************
//  Function:		OnMessageGetCharIDFromGate	
//  Description:	去网关查玩家ID 返回的消息
//  Input:			消息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2009.3.17
// **********************************************************

void CFriendModule::OnMessageGetCharIDFromGate( CMessage* pMessage )
{
	LK_ASSERT( pMessage, return );
	CMessageGetCharIDResponse* pMsg = (CMessageGetCharIDResponse*) pMessage->msgpara();
	LK_ASSERT( pMsg, return );
	CEntityPlayer* pPlayer = (CEntityPlayer*)CSceneLogic::GetSingleton().GetEntity(pMsg->srcentityid());
	if( pPlayer == NULL )
		return ;

	switch( pMsg->typeid_() )
	{
	case TYPE_ADD_FRIEND:
		{
			if ( pMsg->charid() == 0 ) // 没找到
			{
				SendErrorCode( pPlayer->GetCharID(), ERROR_INVALID_PLAYER );
				return;
			}

			DisposeAddFriend( pPlayer,pMsg->charid() );
			break;
		}
	case TYPE_ADD_BLACK :
		{
			if ( pMsg->charid() == 0 ) // 没找到
			{
				SendErrorCode( pPlayer->GetCharID(), ERROR_INVALID_PLAYER );
				return;
			}

			int nDesCharID = pMsg->charid();
			int nNoticeFlag = 0; // 通知客户端的方式：0=添加新的，1=从好友移动
			FriendType::iterator it = mRels.find( pPlayer->GetCharID() );
			LK_ASSERT ( it != mRels.end(), return );

			int nRet = it->second.AddBlack( nDesCharID );
			if ( nRet != 0 )
			{
				SendErrorCode( pPlayer->GetCharID(), (EFriendModuleErrorCode)nRet );
				return;
			}
			// 如果在好友里
			if ( it->second.IsFriend( nDesCharID ) )
			{
				it->second.DelFriend( nDesCharID );
				nNoticeFlag = 1;
			}
				// TODO:如果在仇敌里(下次上线就不在仇敌里了)
				//else if ( it->second.IsEnemy( nDesCharID ) )
				//{
				//	//SaveFriend( vparam1, nDesCharID, YOUR_BLACKLIST, PLAYER_GROUP_COUNT + 1, false );
				//}
				//else // 确实是新的黑名单
				//{
				//	//SaveFriend( vparam1, nDesCharID, YOUR_BLACKLIST, PLAYER_GROUP_COUNT + 1, true );
				//}
			//}

			//CMessage tMessage;
			//CMessageFriendMoveToBlackNotice msg;
			//msg.set_charid( nDesCharID );
			//tMessage.set_msgpara((uint32_t) &msg);
			//tMessage.mutable_msghead()->set_messageid( ID_S2C_FRIEND_MOVETOBLACK_NOTICE );
			//CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );

			CMessageChangeFriendNotice msg;
			CMessage tMessage;
			msg.set_charid( nDesCharID );
			msg.set_name( pMsg->name().c_str() );
			msg.set_head( pMsg->head());
			msg.set_sex( pMsg->sex() );
			msg.set_reltome( YOUR_BLACKLIST );
			msg.set_status( AT_OFFLINE );
			msg.set_groupid( PLAYER_GROUP_COUNT + 1 );
			msg.set_oprcode( ADD_TO_BLACK);

			CEntityPlayer* tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( nDesCharID );
			if ( tpPlayer  != NULL )
			{
				msg.set_vipflag( ((CPropertyPlayer*)tpPlayer->GetProperty())->GetVipFlag() );
			}
			

			MakeRouterMessageByID<CMessageChangeFriendNotice>( pPlayer->GetCharID(), pPlayer->GetCharID(), 0
				, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg, &tMessage );
			CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );

			if( nNoticeFlag == 1 )
			{
				CMessageChangeFriendNotice msg;
				CMessage tMessage;
				msg.set_charid( nDesCharID );
				msg.set_oprcode( DEL_FRIEND);

				MakeRouterMessageByID<CMessageChangeFriendNotice>( pPlayer->GetCharID(), pPlayer->GetCharID(), 0
					, ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE, msg, &tMessage );
				CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
			}
			break;
		}
	}

}


// ***********************************************************
//  Function:		OnMessageAddFriend	
//  Description:	玩家加好友 返回消息(肯定在被邀请方的服务器 没必要发路由消息)
//  Input:			消息
//  OutPut:			NULL
//  Return:			NULL
//  Others:
//  Date:			2009.3.17
// **********************************************************
void CFriendModule::OnMessageAddFriend( CMessage* pMessage )
{
	LK_ASSERT( pMessage, return );
	CMessageAddFriendResponse* pMsg = (CMessageAddFriendResponse*) pMessage->msgpara();
	LK_ASSERT( pMsg, return );

	if (pMsg->sendback() == 1 )
	{
		if (pMsg->retcode() == RET_OFFLINE)
		{
			SendErrorCode( pMsg->srccharid(), ERROR_DES_PLAYER_IS_NOT_ONLINE );
		}
		else if (pMsg->retcode() == RET_INVALID_PLAYER )
		{
			SendErrorCode( pMsg->srccharid(), ERROR_INVALID_PLAYER );
		}
		return;
	}

	// 以下内容肯定在被邀请人的服务器 先判断黑名单
	CEntityPlayer* pPlayer = CSceneLogic::GetSingleton().GetPlayerByName( pMsg->desname().c_str() );
	if (pPlayer == NULL )
		return;

	FriendType::iterator it = mRels.find( pPlayer->GetCharID() );
	LK_ASSERT ( it != mRels.end(), return );
	if (it->second.IsBlack( pMsg->srccharid() ) )
	{
		return;
	}

	CMessageInviteFriendNotice msg; // 发到被邀请玩家
	CMessage tMessage;

	msg.set_srccharid( pMsg->srccharid() );
	msg.set_srcmetier( pMsg->srcmetier() );
	msg.set_srclevel( pMsg->srclevel() );
	msg.set_srchead( pMsg->srchead() );
	msg.set_srcsex( pMsg->srcsex() );
	msg.set_descharid( pMsg->descharid() ) ;

	MakeRouterMessageByName<CMessageInviteFriendNotice>( pMsg->srcname().c_str(), pMsg->desname().c_str(), 0
		, ID_S2C_FRIEND_INVITE_NOTICE_R, msg, &tMessage );

	//msg.mutable_router()->set_sendback( 0 );
	CSceneLogic::GetSingleton( ).Send2Player( pMsg->desname().c_str(), &tMessage );
}

void CFriendModule::OnMessageChangeMasterRequest( CEntityPlayer* pPlayer, CMessage* pMsg )
{
	LK_ASSERT( pPlayer, return );
	CMessageChangeMasterRequest* pMessage = ( CMessageChangeMasterRequest* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	CTemplateMatrixTable *tpTable = (CTemplateMatrixTable*)CDataStatic::GetTemp( TEMP_MATRIXTABLE );
	if ( tpTable == NULL )
	{
		return;
	}
	
	CMessage tMessage;
	CMessageChangeMasterResponse tResponse;	
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CHANGEMASTER );
	tMessage.set_msgpara((uint32_t) &tResponse);

	if ( IS_GLOBAL_SCENESERVER( pPlayer->GetLineID() ) || IS_DUNGEONSERVER_LINE( pPlayer->GetLineID() ) )
	{
		tResponse.set_result( EDUNGEON_ERROR_NOPLACEACTION );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );
		return;
	}
	
	if ( pPlayer->GetLevel() > tpTable->mMaxLimit )
	{
		tResponse.set_result( ERROR_MASTER_HIGHLEVEL );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );
		return;
	}	
	CPropertyPlayer *tpProperty = (CPropertyPlayer*)pPlayer->GetProperty();

	if ( tpProperty->GetMasterTimes( ) >= tpTable->mEnterLimit )
	{
		tResponse.set_result( ERROR_MASTER_NOTIMES );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );
		return;
	}

	// 把自己的MasterID加到消息里给网关
	bool tEmpty = false;
	pMessage->clear_masterid();
	for ( int i = 0; i < MAX_MASTER_NUM; i++ )
	{
		if ( tpProperty->GetMasterID( i ) == 0 )
		{
			tEmpty = true;
		}
		else
		{
			pMessage->add_masterid( tpProperty->GetMasterID( i ) );
		}
	}

	if ( tEmpty == false )
	{
		tResponse.set_result( ERROR_MASTER_NOMOREMASTER );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );
		return;
	}

	pMessage->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMsg );
}

void CFriendModule::OnMessageGetSlaveRequest( CEntityPlayer* pPlayer, CMessage* pMsg )
{
	LK_ASSERT( pPlayer, return );
	CMessageGetSlaveRequest* pMessage = ( CMessageGetSlaveRequest* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	CPropertyPlayer *tpProperty = (CPropertyPlayer*)pPlayer->GetProperty();
	pMessage->clear_slaveid();
	for ( CPropertyPlayer::MatrixList::iterator it = tpProperty->mMatrixList.begin(); it != tpProperty->mMatrixList.end(); ++it )
	{
		pMessage->add_slaveid( *it );
	}	
	pMessage->clear_masterid();
	for ( int i = 0; i < MAX_MASTER_NUM; i++ )
	{
		pMessage->add_masterid( tpProperty->GetMasterID( i ) );
	}

	pMessage->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMsg );
}

void CFriendModule::OnMessageGetSlaveResponse( CMessage* pMsg )
{
	CMessageGetSlaveResponse* pMessage = ( CMessageGetSlaveResponse* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( pMessage->charid() );

	if ( tpPlayer == NULL )
	{
		return;
	}

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );
}

void CFriendModule::OnMessageChangeMasterResponse( CMessage* pMsg )
{
	CMessageChangeMasterResponse* pMessage = ( CMessageChangeMasterResponse* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( pMessage->charid() );

	if ( tpPlayer == NULL )
	{
		return;
	}

	if ( pMessage->result() == SUCCESS )
	{
		CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();		
		int tMasterCount = 0;
		bool tSet = false;

		int tMasterPercent = 0;
		for ( int i = 0; i < MAX_MASTER_NUM; i++ )
		{
			if ( tpProperty->GetMasterID( i ) == 0 )
			{
				continue;
			}
			tMasterPercent += tpProperty->GetMasterPercent( i );
		}

		for ( int i = 0; i < MAX_MASTER_NUM; i++ )
		{
			if ( tpProperty->GetMasterID( i ) == 0 && tSet == false )
			{
				tpProperty->SetMasterID( pMessage->masterid(), i );
				CTemplateMatrixTable *tpTable = (CTemplateMatrixTable*)CDataStatic::GetTemp( TEMP_MATRIXTABLE );
				if ( tpTable != NULL )
				{
					int tPercent = std::min( std::max( tpTable->mAlterAll - tMasterPercent, 0 ), tpTable->mAlterDefault );
					tpProperty->SetMasterPercent( tPercent, i );
				}
				tSet = true;
			}
			if ( tpProperty->GetMasterID( i ) != 0 )
			{
				tMasterCount++;
			}
		}

		if ( tSet == true )
		{		
			tpProperty->SetMasterTimes( tpProperty->GetMasterTimes( ) + 1 );
			LogMatrixAddMaster( tpPlayer->GetCharID(), pMessage->masterid(), tpPlayer->GetLevel(), tMasterCount );

			CMessageAcquireMasterNotify tNotify;
			CMessage tMessage;
			tNotify.set_slaveid( tpPlayer->GetCharID() );
			tNotify.set_masterid( pMessage->masterid() );
			tMessage.mutable_msghead()->set_messageid( ID_S2G_ACQUIREMASTER_NOTIFY );
			tMessage.set_msgpara((uint32_t) &tNotify);
			CSceneLogic::GetSingleton().Send2Gate( &tMessage );
		}
	}

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );	
}

void CFriendModule::OnMessageChangeMasterNotify( CMessage* pMsg )
{
	CMessageChangeMasterNotify* pMessage = ( CMessageChangeMasterNotify* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( pMessage->charid() );

	if ( tpPlayer == NULL )
	{
		return;
	}
	CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();		

	tpProperty->AddSlaveID( pMessage->mutable_slave()->slaveid() );	
	tpProperty->SetBeginnerNum( tpProperty->GetBeginnerNum( ) + 1 );
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );

	LogMatrixAddSlave( pMessage->mutable_slave()->slaveid(), tpPlayer->GetCharID(), tpPlayer->GetLevel(), 
		tpProperty->mMatrixList.size(), tpProperty->GetSlaveNum() );
}

void CFriendModule::OnMessageSlaveLevelUpNotify( CMessage* pMsg )
{
	CMessageSlaveLevelUpNotify* pMessage = ( CMessageSlaveLevelUpNotify* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( pMessage->charid() );

	if ( tpPlayer == NULL )
	{
		return;
	}
	CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();

	int tLevel = pMessage->mutable_slave()->level();

	CTemplateMatrixTable *tpTable = (CTemplateMatrixTable*)CDataStatic::GetTemp( TEMP_MATRIXTABLE );
	if ( tpTable == NULL )
	{
		return;
	}

	if ( tLevel == tpTable->mFormalLevel )
	{
		tpProperty->SetFormalNum( tpProperty->GetFormalNum() + 1 );
		tpProperty->SetBeginnerNum( tpProperty->GetBeginnerNum() - 1 );
	}

	if ( tLevel == tpTable->mEducatedLevel )
	{
		tpProperty->SetFormalNum( tpProperty->GetFormalNum() - 1 );
		tpProperty->SetSlaveNum( tpProperty->GetSlaveNum() + 1 );
		for ( CPropertyPlayer::MatrixList::iterator it = tpProperty->mMatrixList.begin(); it != tpProperty->mMatrixList.end(); ++it )
		{
			if ( (unsigned int)(*it) == pMessage->mutable_slave()->slaveid() )
			{
				tpProperty->mMatrixList.erase( it );
				break;
			}
		}
	}

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );
}

void CFriendModule::OnMessageSlaveOnOffNotify( CMessage* pMsg )
{
	CMessageSlaveOnOffNotify* pMessage = ( CMessageSlaveOnOffNotify* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	for ( int i = 0; i < pMessage->descharid_size(); i++ )
	{
		CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( pMessage->descharid( i ) );

		if ( tpPlayer == NULL )
		{
			continue;
		}

		CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );
	}
}

void CFriendModule::SendSlaveOnOffNotify( CEntityPlayer *pSrcPlayer, int vOnline )
{
	if ( pSrcPlayer == NULL )
	{
		return;
	}
	CPropertyPlayer *tpProperty = (CPropertyPlayer*)pSrcPlayer->GetProperty();
	CMessageSlaveOnOffNotify tNotify;
	CMessage tMessage;
	tNotify.set_srccharid( pSrcPlayer->GetCharID() );
	tNotify.set_srccharname( pSrcPlayer->GetCharName() );
	tNotify.set_online( vOnline );
	tNotify.set_level( pSrcPlayer->GetLevel() );
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_SLAVEONOFF );
	tMessage.set_msgpara((uint32_t) &tNotify);

	for ( int i = 0; i < MAX_MASTER_NUM; i++ )
	{
		tNotify.add_descharid( tpProperty->GetMasterID( i ) );
	}

	for ( CPropertyPlayer::MatrixList::iterator it = tpProperty->mMatrixList.begin(); it != tpProperty->mMatrixList.end(); ++it )
	{
		tNotify.add_descharid( *it );		
	}	
	CSceneLogic::GetSingleton().Send2Gate( &tMessage );
}

void CFriendModule::OnMessageQueryPlayerRequest( CEntityPlayer* pPlayer, CMessage* pMsg )
{
	LK_ASSERT( pPlayer, return );
	CMessageQueryPlayerRequest* pMessage = ( CMessageQueryPlayerRequest* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	pMessage->set_charid( pPlayer->GetCharID() );

	CSceneLogic::GetSingleton().Send2Gate( pMsg );
}

void CFriendModule::OnMessageQueryPlayerResponse( CMessage* pMsg )
{
	CMessageQueryPlayerResponse* pMessage = ( CMessageQueryPlayerResponse* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( pMessage->charid() );

	if ( tpPlayer == NULL )
	{
		return;
	}

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );
}

void CFriendModule::SendSlaveValueNotify( CEntityPlayer *pSrcPlayer )
{
	if ( pSrcPlayer == NULL )
	{
		return;
	}
	CPropertyPlayer *tpProperty = (CPropertyPlayer*)pSrcPlayer->GetProperty();
	CMessageSlaveNumNotify tNotify;
	CMessage tMessage;
	for ( int i = 0; i < MAX_MATRIX_LEVEL; i++ )
	{
		tNotify.add_slavevalue( tpProperty->GetSlaveValue( i ) );
	}
	tNotify.set_slavepoint( tpProperty->GetSlavePoint( ) );
	tNotify.set_masterpoint( tpProperty->GetMasterPoint( ) );
	tNotify.set_slavetotal( tpProperty->GetSlaveTotal( ) );
	tNotify.set_mastertotal( tpProperty->GetMasterTotal( ) );
	
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_SLAVENUM );
	tMessage.set_msgpara((uint32_t) &tNotify);

	CSceneLogic::GetSingleton().Send2Player( pSrcPlayer, &tMessage );
}

void CFriendModule::OnMessageRemoveSlaveRequest( CEntityPlayer* pPlayer, CMessage* pMsg )
{
	LK_ASSERT( pPlayer, return );
	CMessageRemoveSlaveRequest* pMessage = ( CMessageRemoveSlaveRequest* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	pMessage->set_charid( pPlayer->GetCharID() );

	CMessage tMessage;
	CMessageRemoveSlaveResponse tResponse;	
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_REMOVESLAVE );
	tMessage.set_msgpara((uint32_t) &tResponse);

	CPropertyPlayer *tpProperty = (CPropertyPlayer*)pPlayer->GetProperty();
	CTemplateMatrixTable *tpTable = (CTemplateMatrixTable*)CDataStatic::GetTemp( TEMP_MATRIXTABLE );
	if ( tpTable == NULL )
	{
		return;
	}

	int HasItem = 0;
	int vItemIndex =  CPropertyModule::GetSingletonPtr()->HasItem(  pPlayer->GetEntityID() ,tpTable->mRemoveItem, 1 , HasItem, 0 );
	if ( vItemIndex == -1 )
	{
		tResponse.set_result( ERROR_MASTER_NOITEM );
		tResponse.set_slaveid( pMessage->slaveid() );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );	
		return;
	}

	for ( CPropertyPlayer::MatrixList::iterator it = tpProperty->mMatrixList.begin(); it != tpProperty->mMatrixList.end(); ++it )
	{
		if ( (unsigned int)(*it) == pMessage->slaveid() )
		{
			//tpProperty->mMatrixList.erase( it );
			CSceneLogic::GetSingleton().Send2Gate( pMsg );
			return;
		}
	}	

	tResponse.set_result( ERROR_MASTER_NOSLAVE );
	tResponse.set_slaveid( pMessage->slaveid() );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );	
}

void CFriendModule::OnMessageRemoveSlaveResponse( CMessage* pMsg )
{
	CMessageRemoveSlaveResponse* pMessage = ( CMessageRemoveSlaveResponse* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( pMessage->charid() );
	if ( tpPlayer == NULL )
	{
		return;
	}
	CTemplateMatrixTable *tpTable = (CTemplateMatrixTable*)CDataStatic::GetTemp( TEMP_MATRIXTABLE );
	if ( tpTable == NULL )
	{
		return;
	}

	int HasItem = 0;
	int vItemIndex =  CPropertyModule::GetSingletonPtr()->HasItem( tpPlayer->GetEntityID() ,tpTable->mRemoveItem, 1 , HasItem, 0 );
	if ( vItemIndex != -1 )
	{
		CPropertyModule::GetSingletonPtr()->PlayerRemoveItem( tpPlayer, vItemIndex, 1 );
		LogEventLostItemByMaster( tpPlayer, 0, tpTable->mRemoveItem, 1);
	}

	CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();
	for ( CPropertyPlayer::MatrixList::iterator it = tpProperty->mMatrixList.begin(); it != tpProperty->mMatrixList.end(); ++it )
	{
		if ( (unsigned int)(*it) == pMessage->slaveid() )
		{
			tpProperty->mMatrixList.erase( it );
			if ( pMessage->param() == BeginnerNum )
			{
				tpProperty->SetBeginnerNum( tpProperty->GetBeginnerNum() - 1 );
			}
			else if ( pMessage->param() == FormalNum )
			{
				tpProperty->SetFormalNum( tpProperty->GetFormalNum() - 1 );
			}
			else if ( pMessage->param() == EducatedNum )
			{
				tpProperty->SetSlaveNum( tpProperty->GetSlaveNum() - 1 );
			}
			break;
		}
	}

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );

	LogMatrixDelSlave( pMessage->slaveid(), tpPlayer->GetCharID(), tpPlayer->GetLevel(), tpProperty->mMatrixList.size(), tpProperty->GetSlaveNum() );
}

void CFriendModule::OnMessageRemoveSlaveNotify( CMessage* pMsg )
{
	CMessageRemoveSlaveNotify* pMessage = ( CMessageRemoveSlaveNotify* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( pMessage->charid() );

	if ( tpPlayer == NULL )
	{
		return;
	}
	CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();
	int tMasterCount = 0;
	for ( int i = 0; i < MAX_MASTER_NUM; i++ )
	{
		if ( (unsigned int)(tpProperty->GetMasterID( i )) == pMessage->masterid() )
		{
			tpProperty->SetMasterID( 0, i );
			tpProperty->SetMasterPercent( 0, i );
			continue;
		}
		tMasterCount++;
	}

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );

	LogMatrixDelMaster( tpPlayer->GetCharID(), pMessage->masterid(), tpPlayer->GetLevel(), tMasterCount );
}

void CFriendModule::SendAllFriendNotice( CEntityPlayer* npPlayer )
{
	if ( npPlayer == NULL )
	{
		return ;
	}
	
	FriendType::iterator it = mRels.find( npPlayer->GetCharID() );
	if ( it == mRels.end() )
	{
		// 没有好友列表
		return;
	}

	std::vector<int> friend_list;
	// 取得在线列表
	it->second.GetFriendCharIDList( friend_list, true );
	SendMyStatus2Friends( npPlayer->GetCharID(), friend_list, it->second.GetMyStatus(), YOUR_FRIEND );
	friend_list.clear();
	it->second.GetEnemyCharIDList( friend_list, true );
	SendMyStatus2Friends( npPlayer->GetCharID(), friend_list, it->second.GetMyStatus(), YOUR_ENEMY );

	return ;
}

void CFriendModule::OnMessageRemoveMasterRequest( CEntityPlayer* pPlayer, CMessage* pMsg )
{
	LK_ASSERT( pPlayer, return );
	CMessageRemoveMasterRequest* pMessage = ( CMessageRemoveMasterRequest* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	pMessage->set_charid( pPlayer->GetCharID() );

	CMessage tMessage;
	CMessageRemoveMasterResponse tResponse;	
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_REMOVEMASTER );
	tMessage.set_msgpara((uint32_t) &tResponse);

	CPropertyPlayer *tpProperty = (CPropertyPlayer*)pPlayer->GetProperty();
	CTemplateMatrixTable *tpTable = (CTemplateMatrixTable*)CDataStatic::GetTemp( TEMP_MATRIXTABLE );
	if ( tpTable == NULL )
	{
		return;
	}
	if ( tpProperty->GetBindMoney() + tpProperty->GetMoney() < tpTable->mRemoveMoney )
	{
		tResponse.set_result( ERROR_MASTER_NOMONEY );
		tResponse.set_masterid( pMessage->masterid() );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );
		return;
	}

	for ( int i = 0; i < MAX_MASTER_NUM; i++ )
	{
		if ( (unsigned int)(tpProperty->GetMasterID( i )) == pMessage->masterid() )
		{
			tpProperty->SetMasterID( 0, i );
			tpProperty->SetMasterPercent( 0, i );
			CSceneLogic::GetSingleton().Send2Gate( pMsg );
			return;
		}
	}

	tResponse.set_result( ERROR_MASTER_NOMASTER );
	tResponse.set_masterid( pMessage->masterid() );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );	
}

void CFriendModule::OnMessageRemoveMasterResponse( CMessage* pMsg )
{
	CMessageRemoveMasterResponse* pMessage = ( CMessageRemoveMasterResponse* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( pMessage->charid() );
	if ( tpPlayer == NULL )
	{
		return;
	}
	CTemplateMatrixTable *tpTable = (CTemplateMatrixTable*)CDataStatic::GetTemp( TEMP_MATRIXTABLE );
	if ( tpTable == NULL )
	{
		return;
	}
	CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();
	CPropertyModule::GetSingleton().PlayerChangeMoney( tpPlayer, tpTable->mRemoveMoney, true, true );
	LogEventLostMoneyByMaster( tpPlayer, tpTable->mRemoveMoney );

	int tMasterCount = 0;
	for ( int i = 0; i < MAX_MASTER_NUM; i++ )
	{
		if ( (unsigned int)(tpProperty->GetMasterID( i )) == pMessage->masterid() )
		{
			tpProperty->SetMasterID( 0, i );
			tpProperty->SetMasterPercent( 0, i );
			continue;
		}
		tMasterCount++;
	}

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );

	LogMatrixDelMaster( tpPlayer->GetCharID(), pMessage->masterid(), tpPlayer->GetLevel(), tMasterCount );
}

void CFriendModule::OnMessageRemoveMasterNotify( CMessage* pMsg )
{
	CMessageRemoveMasterNotify* pMessage = ( CMessageRemoveMasterNotify* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( pMessage->charid() );

	if ( tpPlayer == NULL )
	{
		return;
	}
	CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();
	for ( CPropertyPlayer::MatrixList::iterator it = tpProperty->mMatrixList.begin(); it != tpProperty->mMatrixList.end(); ++it )
	{
		if ( (unsigned int)(*it) == pMessage->slaveid() )
		{
			tpProperty->mMatrixList.erase( it );
			if ( pMessage->param() == BeginnerNum )
			{
				tpProperty->SetBeginnerNum( tpProperty->GetBeginnerNum() - 1 );
			}
			else if ( pMessage->param() == FormalNum )
			{
				tpProperty->SetFormalNum( tpProperty->GetFormalNum() - 1 );
			}
			else if ( pMessage->param() == EducatedNum )
			{
				tpProperty->SetSlaveNum( tpProperty->GetSlaveNum() - 1 );
			}
			break;
		}
	}

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );

	LogMatrixDelSlave( pMessage->slaveid(), tpPlayer->GetCharID(), tpPlayer->GetLevel(), tpProperty->mMatrixList.size(), tpProperty->GetSlaveNum() );
}

void CFriendModule::OnMessageAlterMasterPercentRequest( CEntityPlayer* pPlayer, CMessage* pMsg )
{
	LK_ASSERT( pPlayer, return );
	CMessageAlterMasterPercentRequest* pMessage = ( CMessageAlterMasterPercentRequest* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	CTemplateMatrixTable *tpTable = (CTemplateMatrixTable*)CDataStatic::GetTemp( TEMP_MATRIXTABLE );
	if ( tpTable == NULL )
	{
		return;
	}
	
	CMessage tMessage;
	CMessageAlterMasterPercentResponse tResponse;	
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_ALTERMASTERPERCENT );
	tMessage.set_msgpara((uint32_t) &tResponse);
	int tPercent = pMessage->percent();
	tResponse.set_masterid( pMessage->masterid() );
	tResponse.set_percent( tPercent );

	if ( tPercent < tpTable->mAlterMin || tPercent > tpTable->mAlterMax )
	{
		tResponse.set_result( ERROR_MASTER_CHANGEOVERFLOW );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );
		return;
	}

	int tIndex = -1;
	int tMasterCount = 0;
	int tOldPercent = 0;
	CPropertyPlayer *tpProperty = (CPropertyPlayer*)pPlayer->GetProperty();
	for ( int i = 0; i < MAX_MASTER_NUM; i++ )
	{
		if ( (unsigned int)(tpProperty->GetMasterID( i )) == pMessage->masterid() && tpProperty->GetMasterID( i ) != 0 )
		{
			tIndex = i;			
			tOldPercent = tpProperty->GetMasterPercent( i );
			continue;
		}
		tMasterCount += tpProperty->GetMasterPercent( i );
	}

	if ( tMasterCount + tPercent > tpTable->mAlterAll )
	{
		tResponse.set_result( ERROR_MASTER_CHANGEOVERFLOW );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );
		return;
	}

	if ( tIndex != -1 )
	{
		tpProperty->SetMasterPercent( tPercent, tIndex );
	}
	else
	{
		tResponse.set_result( ERROR_MASTER_CHANGENOMASTER );
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );
		return;
	}

	tResponse.set_result( SUCCESS );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );

	LogMatrixMasterPercent( pPlayer->GetCharID(), pMessage->masterid(), tOldPercent, tPercent );
}

void CFriendModule::OnMessageGetLowPlayerRequest( CEntityPlayer* pPlayer, CMessage* pMsg )
{
	CMessage tMessage;
	CMessageGetLowPlayerResponse tResponse;	
	tMessage.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_GETLOWPLAYER );
	tMessage.set_msgpara((uint32_t) &tResponse);

	int tRand = RAND(10) + 1;
	int tCount = 0;
	int tReqCount = 0;
	CTemplateMatrixTable *tpTable = (CTemplateMatrixTable*)CDataStatic::GetTemp( TEMP_MATRIXTABLE );
	if ( tpTable == NULL )
		return;
	CSceneObjManagerImp* pSceneneObjManagerImp = CSceneObjManager::GetSingletonPtr()->mSceneObjManagerImp;
	for( CSceneObjManagerImp::ObjMng_EntityPlayer::iterator tPlayerIt = 
			pSceneneObjManagerImp->mEntityPlayerPool.begin( );          
			tPlayerIt != pSceneneObjManagerImp->mEntityPlayerPool.end( );
			tPlayerIt++
	   )
	{
		CEntityPlayer* tpSrcEntity = &( *tPlayerIt );
		CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpSrcEntity->GetProperty();
		if ( tpSrcEntity->GetLevel() > tpTable->mMaxLimit )
			continue;
		bool tHasMaster = true;
		for ( int i = 0; i < MAX_MASTER_NUM; i++ )
		{
			if ( tpProperty->GetMasterID( i ) == 0 )
			{
				tHasMaster = false;
				break;
			}
		}
		if ( tHasMaster == true )
			continue;
		if ( tReqCount < 10 )
		{
			SlaveInfo* pInfo = tResponse.add_required();
			pInfo->set_slaveid( tpSrcEntity->GetCharID() );
			pInfo->set_slavename( tpSrcEntity->GetCharName() );
			pInfo->set_level( tpSrcEntity->GetLevel() );
			pInfo->set_metier( tpSrcEntity->GetMetier() );
			pInfo->set_face( tpSrcEntity->GetFaceType() );
			pInfo->set_sex( tpSrcEntity->GetSex() );
			tReqCount++;
		}
		if ( (int)(tpSrcEntity->GetEntityID() % tRand) == RAND(10) )
		{
			SlaveInfo* pInfo = tResponse.add_player();
			pInfo->set_slaveid( tpSrcEntity->GetCharID() );
			pInfo->set_slavename( tpSrcEntity->GetCharName() );
			pInfo->set_level( tpSrcEntity->GetLevel() );
			pInfo->set_metier( tpSrcEntity->GetMetier() );
			pInfo->set_face( tpSrcEntity->GetFaceType() );
			pInfo->set_sex( tpSrcEntity->GetSex() );
			tCount++;
			if ( tCount >= 10 )
			{
				break;
			}
		}
	}
	tResponse.set_result( SUCCESS );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMessage );
}

void CFriendModule::SendAddMasterPointRequest( CEntityPlayer *pPlayer, int vRawPoint )
{
	if ( pPlayer == NULL )
		return;
	CMessage tMessage;
	CMessageAddMasterPointRequest tRequest;	
	tMessage.mutable_msghead()->set_messageid( ID_S2G_ADDMASTERPOINT_REQUEST );
	tMessage.set_msgpara((uint32_t) &tRequest);

	CTemplateMatrixTable *tpTable = (CTemplateMatrixTable*)CDataStatic::GetTemp( TEMP_MATRIXTABLE );
	if ( tpTable == NULL )
	{
		return;
	}

	CPropertyPlayer *tpProperty = (CPropertyPlayer*)pPlayer->GetProperty();
	int tPercent = 0;
	if ( tpProperty->GetMasterGive() >= tpTable->mReturnLimit )
	{
		return;
	}
	if ( pPlayer->GetLevel() < tpTable->mFormalLevel )
	{
		return;
	}
	else if ( pPlayer->GetLevel() >= tpTable->mFormalLevel && pPlayer->GetLevel() < tpTable->mEducatedLevel )
	{
		tPercent = tpTable->mFormalPercent;
	}
	else if ( pPlayer->GetLevel() >= tpTable->mEducatedLevel )
	{
		tPercent = tpTable->mFormalPercent;
	}
	for ( int i = 0; i < MAX_MASTER_NUM; i++ )
	{
		if ( tpProperty->GetMasterID( i ) == 0 )
		{
			continue;
		}
		
		int tPoint = tpProperty->GetMasterPercent( i ) / (float)tpTable->mAlterAll * tPercent / SERVER_PERCENT_INT * vRawPoint;
		tRequest.set_point( tPoint );
		tRequest.set_masterid( tpProperty->GetMasterID( i ) );
		CSceneLogic::GetSingleton().Send2Gate( &tMessage );
		LogMatrixAddPoint( pPlayer->GetCharID(), tpProperty->GetMasterID( i ), vRawPoint, tPoint );
		tpProperty->SetMasterGive( tpProperty->GetMasterGive() + tPoint );
		int tNum = tPoint;
		char pNum[10] = { 0 };
		sprintf( pNum, "%d", tNum );
		char *pParam[2]	= { NULL };
		int pParamPos[2] = { 0 };
		pParam[0] = (char*)pPlayer->GetCharName();
		pParamPos[0] = 1;
		pParam[1] = pNum;
		pParamPos[1] = 1;
		CMailModule::GetSingleton().SendSysMailBySysID( tpTable->mReturnMailID, NULL, tpProperty->GetMasterID( i ), NULL,
							NULL, 0, pPlayer->GetNationality(), 0, pParam, pParamPos, 2 );
	}
}

void CFriendModule::OnMessageAddMasterPointNotify( CMessage* pMsg )
{
	CMessageAddMasterPointNotify* pMessage = ( CMessageAddMasterPointNotify* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( pMessage->charid() );

	if ( tpPlayer == NULL )
	{
		return;
	}
	CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();

	tpProperty->SetMasterPoint( tpProperty->GetMasterPoint( ) + pMessage->point() );

	SendSlaveValueNotify( tpPlayer );
}

void CFriendModule::OnMessageAnswerChangeNotify( CMessage* pMsg )
{
	CMessageAnswerChangeNotify* pMessage = ( CMessageAnswerChangeNotify* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( pMessage->charid() );

	if ( tpPlayer == NULL )
	{
		return;
	}

	CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();

	if ( tpProperty->IsSlaveFull() )
	{
		CMessageAnswerChangeRequest tRequest;
		CMessage tMessage;
		tRequest.set_slaveid( pMessage->mutable_slave()->slaveid() );
		tRequest.set_charid( tpPlayer->GetCharID() );
		tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_ANSWERCHANGE );
		tMessage.set_msgpara((uint32_t) &tRequest);
		tRequest.set_result( ERROR_MASTER_FULL );
		CSceneLogic::GetSingleton().Send2Gate( &tMessage );
		return;
	}

	mMasterList.push_back( MasterInfo( tpPlayer->GetCharID(), pMessage->mutable_slave()->slaveid() ) );

	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );
}

void CFriendModule::OnMessageAnswerChangeRequest( CEntityPlayer* pPlayer, CMessage* pMsg )
{
	CMessageAnswerChangeRequest* pMessage = ( CMessageAnswerChangeRequest* )pMsg->msgpara();
	LK_ASSERT( pMessage, return );

	if ( pPlayer == NULL )
	{
		return;
	}

	bool tFind = false;

	for ( MasterList::iterator it = mMasterList.begin(); it != mMasterList.end(); ++it )
	{
		if ( (unsigned int)((*it).mMasterID) == pPlayer->GetCharID() && (unsigned int)((*it).mSlaveID) == pMessage->slaveid() )
		{
			mMasterList.erase(it);
			tFind = true;
			break;
		}
	}

	if ( tFind == false )
	{
		return;
	}

	pMessage->set_charid( pPlayer->GetCharID() );
	CSceneLogic::GetSingleton().Send2Gate( pMsg );
}

// 加载好友祝福的配置信息
void CFriendModule::LoadFriendBlessConfig()
{
	TiXmlDocument tDoc;	
	LK_ASSERT( tDoc.LoadFile( CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(), const_cast<char*>("/friend/friend.xml" ) ) ) == true, return );	
	mFriendBlessConfig.Initialize();
	TiXmlElement *tpRoot = tDoc.RootElement();
	LK_ASSERT( tpRoot != NULL, return )
	TiXmlElement *tpFriendBlessConfig = tpRoot->FirstChildElement( "FriendBlessConfig" );
	LK_ASSERT( tpFriendBlessConfig != NULL, return  )

	if ( tpFriendBlessConfig->Attribute( "LevelInterval" ) != NULL )
	{
		mFriendBlessConfig.SetLevelInterval( atoi(  tpFriendBlessConfig->Attribute( "LevelInterval" ) ) );
	}
	
	if ( tpFriendBlessConfig->Attribute( "OgreNum" ) != NULL )
	{
		mFriendBlessConfig.SetOgreNum( atoi(  tpFriendBlessConfig->Attribute( "OgreNum" ) ) );
	}
	
	if ( tpFriendBlessConfig->Attribute( "OgreExpTableID" ) != NULL )
	{
		mFriendBlessConfig.SetOgreExpTableID( atoi( tpFriendBlessConfig->Attribute( "OgreExpTableID" ) ) );
	}	

	if ( tpFriendBlessConfig->Attribute( "LevelLimit" ) != NULL )
	{
		mFriendBlessConfig.SetLevel( atoi(  tpFriendBlessConfig->Attribute( "LevelLimit" ) ) );
	}
	

	TiXmlElement *tpBlessInfo = tpFriendBlessConfig->FirstChildElement( "BlessInfo" );
	for ( ; tpBlessInfo != NULL; tpBlessInfo = tpBlessInfo->NextSiblingElement() )
	{
		int tTimes = 0;
		if ( tpBlessInfo->Attribute( "Times" ) != NULL )
		{
			tTimes = atoi( tpBlessInfo->Attribute( "Times" ) );
		}
		
		int tOgreNum = 0;
		if ( tpBlessInfo->Attribute( "OgreNum" ) != NULL )
		{
			tOgreNum = atoi( tpBlessInfo->Attribute( "OgreNum" ) );
		}
		
		mFriendBlessConfig.InsertBlessExpInfo( tTimes, tOgreNum );
	}	

	// 加载亲密度相关内容
	TiXmlElement *tpIntimacyConfig = tpRoot->FirstChildElement( "IntimacyConfig" );
	LK_ASSERT( tpIntimacyConfig != NULL, return )
	TiXmlElement *tpIntimacyElement = tpIntimacyConfig->FirstChildElement( "OffLineDays" );
	if ( tpIntimacyElement != NULL )
	{
		if ( tpIntimacyElement->Attribute( "DataValue" ) != NULL )
		{
			mFriendBlessConfig.GetIntimacyConfig()->mOffLineDays = atoi( tpIntimacyElement->Attribute( "DataValue" ) );
		}		
	}
	tpIntimacyElement = tpIntimacyConfig->FirstChildElement( "DayReduced" );
	if ( tpIntimacyElement != NULL )
	{
		if ( tpIntimacyElement->Attribute( "DataValue" ) != NULL )
		{
			mFriendBlessConfig.GetIntimacyConfig()->mDayReduced = atoi( tpIntimacyElement->Attribute( "DataValue" ) );
		}		
	}
	tpIntimacyElement = tpIntimacyConfig->FirstChildElement( "KillReduced" );
	if ( tpIntimacyElement != NULL )
	{
		if ( tpIntimacyElement->Attribute( "DataValue" ) != NULL )
		{
			mFriendBlessConfig.GetIntimacyConfig()->mKillReduced = atoi( tpIntimacyElement->Attribute( "DataValue" ) );
		}		
	}
	tpIntimacyElement = tpIntimacyConfig->FirstChildElement( "DisbandReduced" );
	if ( tpIntimacyElement != NULL )
	{
		if ( tpIntimacyElement->Attribute( "DataValue" ) != NULL )
		{
			mFriendBlessConfig.GetIntimacyConfig()->mDisbandReduced = atoi( tpIntimacyElement->Attribute( "DataValue" ) );
		}		
	}
	tpIntimacyElement = tpIntimacyConfig->FirstChildElement( "LeaveReduced" );
	if ( tpIntimacyElement != NULL )
	{
		if ( tpIntimacyElement->Attribute( "DataValue" ) != NULL )
		{
			mFriendBlessConfig.GetIntimacyConfig()->mLeaveReduced = atoi( tpIntimacyElement->Attribute( "DataValue" ) );
		}		
	}
	tpIntimacyElement = tpIntimacyConfig->FirstChildElement( "KickReduced" );
	if ( tpIntimacyElement != NULL )
	{
		if ( tpIntimacyElement->Attribute( "DataValue" ) != NULL )
		{
			mFriendBlessConfig.GetIntimacyConfig()->mKickReduced = atoi( tpIntimacyElement->Attribute( "DataValue" ) );
		}		
	}
	tpIntimacyElement = tpIntimacyConfig->FirstChildElement( "SwornIntimacy" );
	if ( tpIntimacyElement != NULL )
	{
		if ( tpIntimacyElement->Attribute( "DataValue" ) != NULL )
		{
			mFriendBlessConfig.GetIntimacyConfig()->mSwornIntimacy = atoi( tpIntimacyElement->Attribute( "DataValue" ) );
		}		
	}
	tpIntimacyElement = tpIntimacyConfig->FirstChildElement( "TeamIntimacyTime" );
	if ( tpIntimacyElement != NULL )
	{
		if ( tpIntimacyElement->Attribute( "DataValue" ) != NULL )
		{
			mFriendBlessConfig.GetIntimacyConfig()->mTeamIntimacyTime = atoi( tpIntimacyElement->Attribute( "DataValue" ) );
		}		
	}

	tpIntimacyElement = tpIntimacyConfig->FirstChildElement( "MaxIntimacy" );
	if ( tpIntimacyElement != NULL )
	{
		if ( tpIntimacyElement->Attribute( "DataValue" ) != NULL )
		{
			mFriendBlessConfig.GetIntimacyConfig()->mMaxIntimacy = atoi( tpIntimacyElement->Attribute( "DataValue" ) );
		}		
	}

	tpIntimacyElement = tpIntimacyConfig->FirstChildElement( "DivorceReduced" );
	if ( tpIntimacyElement != NULL )
	{
		if ( tpIntimacyElement->Attribute( "DataValue" ) != NULL )
		{
			mFriendBlessConfig.GetIntimacyConfig()->mDivorceReduced = atoi( tpIntimacyElement->Attribute( "DataValue" ) );
		}		
	}

	tpIntimacyElement = tpIntimacyConfig->FirstChildElement( "SwornTitleID" );
	if ( tpIntimacyElement != NULL )
	{
		if ( tpIntimacyElement->Attribute( "DataValue" ) != NULL )
		{
			mFriendBlessConfig.GetIntimacyConfig()->mSwornTitleID = atoi( tpIntimacyElement->Attribute( "DataValue" ) );
		}		
	}

	tpIntimacyElement = tpIntimacyConfig->FirstChildElement( "MarriageIntimacy" );
	if ( tpIntimacyElement != NULL )
	{
		mFriendBlessConfig.GetIntimacyConfig()->InitializeMarriageIntimacy();
		TiXmlElement *tpData = tpIntimacyElement->FirstChildElement( "DataValue" );
		for ( ; tpData != NULL; tpData = tpData->NextSiblingElement()  )
		{
			int tIntimacy = 0;
			int tNum = 0;
			if ( tpData->Attribute( "Intimacy" ) != NULL )
			{
				tIntimacy = atoi( tpData->Attribute( "Intimacy" ) );
			}
			if ( tpData->Attribute( "ItemReduced" ) != NULL )
			{
				tNum = atoi( tpData->Attribute( "ItemReduced" ) );
			}			
			mFriendBlessConfig.GetIntimacyConfig()->InsertMarriageIntimacy( tIntimacy, tNum );
		}		
	}
	
	tpIntimacyElement = tpIntimacyConfig->FirstChildElement( "ItemCreateSworn" );
	if ( tpIntimacyElement != NULL )
	{
		if ( tpIntimacyElement->Attribute( "DataValue" ) != NULL )
		{
			mFriendBlessConfig.GetIntimacyConfig()->mItemCreateSworn = atoi( tpIntimacyElement->Attribute( "DataValue" ) );
		}		
	}

	tpIntimacyElement = tpIntimacyConfig->FirstChildElement( "ItemDisbandSworn" );
	if ( tpIntimacyElement != NULL )
	{
		if ( tpIntimacyElement->Attribute( "DataValue" ) != NULL )
		{
			mFriendBlessConfig.GetIntimacyConfig()->mItemDisbandSworn = atoi( tpIntimacyElement->Attribute( "DataValue" ) );
		}		
	}

	tpIntimacyElement = tpIntimacyConfig->FirstChildElement( "ItemKickMember" );
	if ( tpIntimacyElement != NULL )
	{
		if ( tpIntimacyElement->Attribute( "DataValue" ) != NULL )
		{
			mFriendBlessConfig.GetIntimacyConfig()->mItemKickMember = atoi( tpIntimacyElement->Attribute( "DataValue" ) );
		}		
	}

	tpIntimacyElement = tpIntimacyConfig->FirstChildElement( "ItemChangeName" );
	if ( tpIntimacyElement != NULL )
	{
		if ( tpIntimacyElement->Attribute( "DataValue" ) != NULL )
		{
			mFriendBlessConfig.GetIntimacyConfig()->mItemChangeName = atoi( tpIntimacyElement->Attribute( "DataValue" ) );
		}		
	}

	tpIntimacyElement = tpIntimacyConfig->FirstChildElement( "MaxDailyDegree" );
	if ( tpIntimacyElement != NULL )
	{
		if ( tpIntimacyElement->Attribute( "DataValue" ) != NULL )
		{
			mFriendBlessConfig.GetIntimacyConfig()->mMaxDailyDegree = atoi( tpIntimacyElement->Attribute( "DataValue" ) );
		}		
	}

	tpIntimacyElement = tpIntimacyConfig->FirstChildElement( "RebuildItemID" );
	if ( tpIntimacyElement != NULL )
	{
		if ( tpIntimacyElement->Attribute( "DataValue" ) != NULL )
		{
			mFriendBlessConfig.GetIntimacyConfig()->mRebuildItemID = atoi( tpIntimacyElement->Attribute( "DataValue" ) );
		}		
	}
}

void CFriendModule::FriendLevelUp( CEntityPlayer *pPlayer, int nUpLevel )
{
	LK_ASSERT( pPlayer != NULL, return )
	if ( mFriendBlessConfig.GetLevelInterval() <= 0 )
	{
		return;
	}
	if ( mFriendBlessConfig.GetLevel() > pPlayer->GetLevel() )
	{
		return;
	}
	
	
	bool tCanSendBless = false;	
	for ( int i = 0; i < nUpLevel; ++i )
	{	
		int tLevel = pPlayer->GetLevel() - i;
		if ( ( tLevel % mFriendBlessConfig.GetLevelInterval() ) == 0 )
		{
			tCanSendBless = true;			
			break;
		}				
	}
	
	if ( tCanSendBless == false )
	{
		return;
	}	

	// 给好友发送消息
	CFriendItem *tpFriendItem = GetPlayerFriendItem( pPlayer->GetCharID() );
	if ( tpFriendItem == NULL )
	{
		return;
	}
	tpFriendItem->SetBlessTimes( 0 );  
	std::vector<int> tFriendList;
	tpFriendItem->GetFriendCharIDList( tFriendList, true );
	for ( int i = 0; i < (int)tFriendList.size(); ++i )
	{												
		CEntityPlayer *tpDesplayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tFriendList[i] );		
		if ( tpDesplayer != NULL )
		{			
			CFriendBlessData tBlessData;
			tBlessData.mLevel = pPlayer->GetLevel() > tpDesplayer->GetLevel() ? tpDesplayer->GetLevel() : pPlayer->GetLevel();
			tBlessData.mRoleID = pPlayer->GetCharID();			
			SetLevelUpFriendID( tpDesplayer->GetCharID(), tBlessData );
		}
		
		SendFriendLevelUpNotify( pPlayer, tFriendList[i], tpDesplayer );
	}	
}

void CFriendModule::SendFriendLevelUpNotify( CEntityPlayer *pPlayer, int pDesCharID, CEntityPlayer *pDesPlayer /* = NULL */ )
{
	LK_ASSERT( pPlayer != NULL , return )
	CMessage tMsgHead;
	CMessageFriendLevelUpNotify tMsg;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_FRIENDLEVELUP );
	tMsgHead.set_msgpara( (int)&tMsg );
	tMsg.mutable_router()->set_descharid( pDesCharID );	
	tMsg.mutable_router()->set_routertype( CMessageRouter::ROUTER_CHAR );

	tMsg.set_level( pPlayer->GetLevel() );
	tMsg.set_name( pPlayer->GetCharName() );
	tMsg.set_roleid( pPlayer->GetCharID() );
	if ( pDesPlayer != NULL )
	{
		CSceneLogic::GetSingleton().Send2Player( pDesPlayer, &tMsgHead );
	}
	else
	{
		CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
	}
}

void CFriendModule::OnMessageFriendBlessRequest( CEntityPlayer *pPlayer, CMessage *pMsg )
{
	LK_ASSERT( pPlayer != NULL && pMsg != NULL, return )
	CMessageFriendBlessRequest *tpFriendBless = ( CMessageFriendBlessRequest * )pMsg->msgpara();
	LK_ASSERT( tpFriendBless != NULL, return )
		
	CFriendBlessData *tpData = GetFriendBlessData( pPlayer->GetCharID(), tpFriendBless->roleid() );
	if ( tpData == NULL )
	{
		return;
	}
	CFriendBlessData tFriendData = *tpData;
	DeleteLevelUpFriendID( pPlayer->GetCharID(), tpFriendBless->roleid() );
	
	//  等级经验表
	CTemplateRedStoneExpTable *tpTable = ( CTemplateRedStoneExpTable * )CDataStatic::SearchTpl( mFriendBlessConfig.GetOgreExpTableID() );
	LK_ASSERT( tpTable != NULL, return )

	// 受祝福的好友获得经验	
	CEntityPlayer *tpFriend = ( CEntityPlayer * )CSceneLogic::GetSingleton().GetPlayerByCharID( tpFriendBless->roleid() );
	if ( tpFriend != NULL )
	{			

		CFriendItem *tpFriendItem = GetPlayerFriendItem( tpFriend->GetCharID() );
		if ( tpFriendItem == NULL )
		{
			return;
		}	

		int tOgreNum = mFriendBlessConfig.GetBlessExpInfo( tpFriendItem->GetBlessTimes() + 1 );
		int tExp = tpTable->mExp[tpFriend->GetLevel()]*tOgreNum;
		if ( tExp > 0 )
		{
			int tCharID = tpFriend->GetCharID();
			CPropertyModule::GetSingleton().PlayerObtainExp2( tpFriend, tExp, 0, EXPREASON_ACTIVITY, &tCharID, 1 );			
		}	

		// 发送祝福通知
		SendNotifyFriendBless( tpFriend, tpFriendBless->roleid(), pPlayer->GetCharName(), tExp );

		// 受祝福的次数加1
		tpFriendItem->SetBlessTimes( tpFriendItem->GetBlessTimes() + 1 );
		
	}	
	else
	{
		// 发送路由消息		
		SendNotifyFriendBless( NULL, tpFriendBless->roleid(), pPlayer->GetCharName(), 0 );
	}

	// 自己获得经验
	int tExp = tpTable->mExp[tFriendData.mLevel]*mFriendBlessConfig.GetOgreNum();
	if ( tExp > 0 )
	{
		int tCharID = pPlayer->GetCharID();
		CPropertyModule::GetSingleton().PlayerObtainExp2( pPlayer, tExp, 0, EXPREASON_ACTIVITY, &tCharID, 1 );	
	}	
}

void CFriendModule::SendFriendBlessResponse( CEntityPlayer *pPlayer, int nErrcode )
{
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageFriendBlessResponse tMsgFriendBless;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_FRIENDBLESS );
	tMsgHead.set_msgpara( (int)&tMsgFriendBless );
	tMsgFriendBless.set_errcode( nErrcode );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
}

void CFriendModule::OnRouterMsgFriendLevelUpNotify( CMessage *pMsg )
{
	LK_ASSERT( pMsg != NULL, return )
	CMessageFriendLevelUpNotify *tpMsg = ( CMessageFriendLevelUpNotify * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	if ( tpMsg->mutable_router()->sendback() == 1 )			
	{
		return;
	}	
	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->mutable_router()->descharid() );
	if ( tpPlayer == NULL )
	{
		return;
	}
	//tpPlayer->SetFriendID( tpMsg->roleid() );
	CFriendBlessData tData;
	tData.mRoleID = tpMsg->roleid();
	tData.mLevel = tpMsg->level() > tpPlayer->GetLevel()? tpPlayer->GetLevel() : tpMsg->level();
	SetLevelUpFriendID( tpPlayer->GetCharID(), tData );
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );
}

void CFriendModule::SendNotifyFriendBless( CEntityPlayer *pPlayer, int nRoleID, const char *pFriendName, int nExp )
{
	CMessage tMsgHead;
	CMessageFriendBlessNotify tMsgFriendBless;

	tMsgHead.mutable_msghead()->set_messageid( ID_S2S_NOTIFY_FRIENDBLESS );
	tMsgHead.set_msgpara( (int)&tMsgFriendBless );

	tMsgFriendBless.mutable_router()->set_descharid( nRoleID );
	tMsgFriendBless.mutable_router()->set_routertype( CMessageRouter::ROUTER_CHAR );
	if ( pFriendName != NULL )
	{
		tMsgFriendBless.set_friendname( pFriendName );
	}
	tMsgFriendBless.set_exp( nExp );
	
	if ( pPlayer != NULL )
	{
		CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
	}	
	else
	{
		CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );	
	}
}

void CFriendModule::OnRouterFriendBlessNotify( CMessage *pMsg )
{
	LK_ASSERT( pMsg != NULL, return )
	CMessageFriendBlessNotify *tpMsg = ( CMessageFriendBlessNotify * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )	

	if ( tpMsg->mutable_router()->sendback() == 1 )			
	{
		return;
	}		

	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->mutable_router()->descharid() );
	if ( tpPlayer == NULL )
	{
		return;
	}
	
	CFriendItem *tpFriendItem = GetPlayerFriendItem( tpPlayer->GetCharID() );
	LK_ASSERT( tpFriendItem != NULL, return )

	//  等级经验表
	CTemplateRedStoneExpTable *tpTable = ( CTemplateRedStoneExpTable * )CDataStatic::SearchTpl( mFriendBlessConfig.GetOgreExpTableID() );
	LK_ASSERT( tpTable != NULL, return )	

	int tExp  =  tpTable->mExp[tpPlayer->GetLevel()]*mFriendBlessConfig.GetBlessExpInfo( tpFriendItem->GetBlessTimes() + 1 );
	if ( tExp > 0 )
	{
		int tCharID = tpPlayer->GetCharID();
		CPropertyModule::GetSingleton().PlayerObtainExp2( tpPlayer, tExp, 0, EXPREASON_ACTIVITY, &tCharID, 1 );	
	}			
	tpFriendItem->SetBlessTimes( tpFriendItem->GetBlessTimes() + 1 );

	// 发送祝福通知
	tpMsg->set_exp( tExp );
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );
}

void CFriendModule::SetLevelUpFriendID( int nRoleID, CFriendBlessData &rData )
{
	LevelUpPlayerList::iterator it = mLevelUpPlayerList.find( nRoleID );
	if ( it != mLevelUpPlayerList.end() )
	{
		if ( (it->second).HasFriend( rData.mRoleID ) == false )
		{
			(it->second).push_back( rData );
		}		
	}
	else
	{
		LevelUpFriendList tLevelUpFriend;
		tLevelUpFriend.push_back( rData );
		mLevelUpPlayerList.insert( LevelUpPlayerList::value_type( nRoleID, tLevelUpFriend ) );
	}
}

void CFriendModule::DeleteLevelUpFriendID( int nRoleID, int nFriendRoleID )
{
	LevelUpPlayerList::iterator it = mLevelUpPlayerList.find( nRoleID );
	if ( it != mLevelUpPlayerList.end() )
	{
		(it->second).DeleteFriend( nFriendRoleID ); 				
		if ( (it->second).size() == 0 )
		{
			mLevelUpPlayerList.erase( it );
		}		
	}	
}

bool CFriendModule::HasLevelUpFriendID( int nRoleID, int nFriendRoleID )
{
	LevelUpPlayerList::iterator it = mLevelUpPlayerList.find( nRoleID );
	if ( it != mLevelUpPlayerList.end() )
	{
		return (it->second).HasFriend( nFriendRoleID );
	}	
	return false;
}

void CFriendModule::DeleteLevelUpFriendList( int nRoleID )
{
	LevelUpPlayerList::iterator it = mLevelUpPlayerList.find( nRoleID );
	if ( it != mLevelUpPlayerList.end() )
	{
		mLevelUpPlayerList.erase( it );
	}	
}

CFriendBlessData *CFriendModule::GetFriendBlessData( int nRoleID, int nFriendRoleID )
{
	LevelUpPlayerList::iterator it = mLevelUpPlayerList.find( nRoleID );
	if ( it != mLevelUpPlayerList.end() )
	{
		return (it->second).GetData( nFriendRoleID ); 				
				
	}	
	return NULL;
}

void CFriendModule::LoadFriendBlessConfigCallback(const char* pcConfigPath)
{
	CFriendModule::GetSingleton().LoadFriendBlessConfig();
}

int CFriendModule::UpdateLatestContact(int nCharID, int nContacterID)
{
	CFriendItem* pFriendItem = GetPlayerFriendItem(nCharID);
	if (pFriendItem != NULL)
	{
		pFriendItem->AddLatestContacter(nContacterID);
	}
	
	return 0;
}

int CFriendModule::NotifyLastContacterToClient(int nCharID, int nContacterID)
{
	CEntityPlayer* pPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID(nContacterID);
	if (pPlayer == NULL)
	{
		return -1;
	}
	
	CMessage Msg;
	CMessageLastContacterNotice MsgPara;
	MsgPara.set_charid(nContacterID);
	MsgPara.set_faceid(pPlayer->GetFaceType());
	MsgPara.set_sex(pPlayer->GetSex());
	MsgPara.set_nationid(pPlayer->GetNationality());
	MsgPara.set_name(pPlayer->GetCharName());
	Msg.mutable_msghead()->set_messageid(ID_S2C_NOTIFY_LASTCONTACTER);
	
	MsgPara.mutable_router()->set_srccharid(nContacterID);
	MsgPara.mutable_router()->set_descharid(nCharID);
	MsgPara.mutable_router()->set_send2player(1);
	MsgPara.mutable_router()->set_routertype(CMessageRouter::ROUTER_CHAR);
	Msg.set_msgpara((int)&MsgPara);
	CSceneLogic::GetSingleton().Send2Player(nCharID, &Msg);

	return 0;
}

int CFriendModule::NotifyLastContacterToGate(int nSenderID, int nReceiverID)
{
	CMessage Msg;
	CMessageUpdateLastContacterNotice MsgPara;
	MsgPara.set_senderid(nSenderID);
	MsgPara.set_receiverid(nReceiverID);
	Msg.mutable_msghead()->set_messageid(ID_S2G_NOTIFY_UPDATELASTCONTACTER);
	Msg.set_msgpara((int)&MsgPara);
	CSceneLogic::GetSingleton().Send2Gate(&Msg);

	return 0;
}

int CFriendModule::UploadContacterListToGate(int nCharID)
{
	FriendType::iterator iter = mRels.find(nCharID);
	if (iter == mRels.end())
	{
		LOG_ERROR("FRIEND", "can't find rel charid=%d when upload contacter list to gate", nCharID);
		return -1;
	}
	
	CMessage Msg;
	CMessageUploadContacterList MsgPara;
	MsgPara.set_playerid(nCharID);
	int nCount = iter->second.GetContacterCount();
	int nContacterID = 0;
	for (int i = 0; i < nCount; i++)
	{
		nContacterID = iter->second.GetContacter(i);
		MsgPara.add_contacterid(nContacterID);
	}
	Msg.mutable_msghead()->set_messageid(ID_S2G_NOTIFY_UPLOADCONTACTERLIST);
	Msg.set_msgpara((int)&MsgPara);
	CSceneLogic::GetSingleton().Send2Gate(&Msg);

	return 0;
}

void CFriendModule::OnMessageContacterStatusNotice( CMessage* pMsg )
{
	if (pMsg == NULL)
	{
		return;
	}
	
	CMessageContacterStatusNotifyG2S* pMsgPara = (CMessageContacterStatusNotifyG2S*)pMsg->msgpara();
	if (pMsgPara == NULL)
	{
		return;
	}
	
	int nCharID = pMsgPara->playerid();
	int nStatus = pMsgPara->playerstatus();
	int nContacterID = pMsgPara->contacterid();
	CMessage Msg;
	CMessageLatestContactChangeStatusNotice MsgPara;
	MsgPara.set_charid(nCharID);
	MsgPara.set_status(nStatus);
	Msg.mutable_msghead()->set_messageid(ID_S2C_NOTIFY_CONTACTERSTATUS);
	Msg.set_msgpara((int)&MsgPara);
	CSceneLogic::GetSingleton().Send2Player(nContacterID, &Msg);
}

// 判断列表中所有人之间的亲密度是否超过某个值
bool CFriendModule::JudgeIntimacyDegree( int *pRoleIDList, int nNum, int nDegree )
{
	LK_ASSERT( pRoleIDList != NULL, return false )
	for ( int i = 0; i < nNum; ++i )
	{	
		for ( int j=i+1; j < nNum; ++j )
		{			
			int tIntimacyDegree = 0;
			int tRet = GetIntimacyDegree( pRoleIDList[i], pRoleIDList[j],tIntimacyDegree );
			if ( tRet != 0 )
			{
				return false;
			}			
			if ( tIntimacyDegree < nDegree )
			{
				return false;
			}			
		}		
	}	
	return true;
}

// ***********************************************************
//  Function:		ChangeGroupIntimacyDegree	
//  Description:	改变一组玩家相互之间的亲密度
//  Date:			2011.06.30
// **********************************************************
void CFriendModule::ChangeGroupIntimacyDegree( int *pRoleIDList, int nNum, EM_EntimacyChangeType nType, int nValue /* = 0 */ )
{
	LK_ASSERT( pRoleIDList != NULL, return )
	for ( int i = 0; i < nNum; ++i )
	{	
		for ( int j=i+1; j < nNum; ++j )
		{							
			ChangeIntimacy( pRoleIDList[i], pRoleIDList[j], nType, nValue );
		}		
	}	
}

// ***********************************************************
//  Function:		UpdateIntimacyDegree	
//  Description:	当玩家上线时更新某个玩家的亲密度
//  Date:			2011.06.30
// **********************************************************
void CFriendModule::UpdateIntimacyDegree( CEntityPlayer *pPlayer )
{
	LK_ASSERT( pPlayer != NULL, return )
	CFriendItem *tpFriendItem = GetPlayerFriendItem( pPlayer->GetCharID() );
	if ( tpFriendItem == NULL )
	{
		return;
	}	
	std::vector<int> tFriendList;
	tpFriendItem->GetFriendCharIDList( tFriendList );
	for ( std::vector<int>::iterator it = tFriendList.begin(); it != tFriendList.end(); ++it )
	{
		st_friend *stpFriend = tpFriendItem->GetFriendByRoleID( *it );
		if ( stpFriend == NULL )
		{
			continue;
		}
		if ( stpFriend->nLastTimeAllOnline <= 0 )
		{
			continue;
		}
		int nReducedValue = 0;
		int tDays = ( time( NULL ) - stpFriend->nLastTimeAllOnline )/DAY_TIME;
		if ( tDays > mFriendBlessConfig.GetIntimacyConfig()->mDayReduced )
		{
			nReducedValue = ( tDays - mFriendBlessConfig.GetIntimacyConfig()->mOffLineDays )*mFriendBlessConfig.GetIntimacyConfig()->mDayReduced;			
		}

		stpFriend->nIntimacyDegree -= nReducedValue;
		if ( stpFriend->nIntimacyDegree < 0 )
		{
			stpFriend->nIntimacyDegree = 0;
		}				

		CEntityPlayer *tpOtherPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( *it );
		if ( tpOtherPlayer == NULL )
		{
			// 通知另外一个玩家亲密度的改动(并且通知到客户端)(等返回后再通知客户端)
			SendModifyIntimacyNotify( *it, nReducedValue, stpFriend->nIntimacyDegree, ENTIMACY_CHANGE_TYPE_MODIFY, pPlayer->GetCharID(), stpFriend->nLastUpdateTime, stpFriend->nDailyIntimacy );
			continue;
		}
		
		CFriendItem *tpOtherFriendItem = GetPlayerFriendItem( *it );
		if ( tpOtherFriendItem == NULL )
		{
			continue;
		}
		st_friend *stpOtherFriend = tpOtherFriendItem->GetFriendByRoleID( pPlayer->GetCharID() );
		if ( stpOtherFriend == NULL )
		{
			// 已经被删除
			tpFriendItem->DelFriend( *it );
			continue;
		}
		stpOtherFriend->nIntimacyDegree -= nReducedValue;
		if ( stpOtherFriend->nIntimacyDegree < 0 )
		{
			stpOtherFriend->nIntimacyDegree = 0;
		}
		
		if ( stpOtherFriend->nIntimacyDegree < stpFriend->nIntimacyDegree )
		{
			stpFriend->nIntimacyDegree = stpOtherFriend->nIntimacyDegree;
		}
		else
		{
			stpOtherFriend->nIntimacyDegree = stpFriend->nIntimacyDegree;
		}

		// 通知到客户端
		SendModifyIntimacyNotify( *it, nReducedValue, stpFriend->nIntimacyDegree, ENTIMACY_CHANGE_TYPE_UPDATE, pPlayer->GetCharID() );
		SendModifyIntimacyNotify( pPlayer->GetCharID(), nReducedValue, stpFriend->nIntimacyDegree, ENTIMACY_CHANGE_TYPE_UPDATE, *it );
	}	
}


// 获取某两个玩家的亲密度
int CFriendModule::GetIntimacyDegree( int nRole1, int nRoleID2, int &rIntimacyDegree )
{
	CFriendItem  *tpFriendItem1 = GetPlayerFriendItem( nRole1 );
	if ( tpFriendItem1 == NULL )
	{
		return -1;
	}
	CFriendItem *tpFriendItem2 = GetPlayerFriendItem( nRoleID2 );
	if ( tpFriendItem2 == NULL )
	{
		return -1;
	}
	st_friend *tpFriendInfo1 = tpFriendItem1->GetFriendByRoleID( nRoleID2 );
	if ( tpFriendInfo1 == NULL )
	{
		return -1;
	}

	st_friend *tpFriendInfo2 = tpFriendItem2->GetFriendByRoleID( nRole1 );
	if ( tpFriendInfo2 == NULL )
	{
		return -1;
	}
	rIntimacyDegree = tpFriendInfo1->nIntimacyDegree < tpFriendInfo2->nIntimacyDegree ? tpFriendInfo1->nIntimacyDegree : tpFriendInfo2->nIntimacyDegree;
	return 0;
}

// 发送修改亲密度的通知
void CFriendModule::SendModifyIntimacyNotify( int nRoleID, int nChangedValue, int nIntimacy, EM_EntimacyChangeType nType, int nFriendID, int nUpdateTime /* = 0 */, int nDailyIntimacy /* = 0 */ )
{
	CMessage tMsgHead;
	CMessageModifyIntimacyNogify tMsgModifyIntimacy;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2S_NOTIFY_MODIFYINTIMACY );
	tMsgHead.set_msgpara( (int)&tMsgModifyIntimacy );
	tMsgModifyIntimacy.set_changevalue( nChangedValue );
	tMsgModifyIntimacy.set_intimacydegree( nIntimacy );
	tMsgModifyIntimacy.set_type( nType );
	tMsgModifyIntimacy.set_friendid( nFriendID );
	tMsgModifyIntimacy.set_updatetime( nUpdateTime );
	tMsgModifyIntimacy.set_dailyintimacy( nDailyIntimacy );
	tMsgModifyIntimacy.mutable_router()->set_descharid( nRoleID );
	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( nRoleID );
	if ( tpPlayer != NULL )
	{
		CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMsgHead );
	}
	else
	{
		CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
	}
}

// 发送更新同时在线时间的通知
void CFriendModule::SendModifyLastTimeAllOnLineNotify( CEntityPlayer *pPlayer, int nRoleID, int nTime )
{
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageModifyLastTimeAllOnLineNotify tMsgModifyIntimacy;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2S_NOTIFY_MODIFYLASTTIMEALLONLINE );
	tMsgHead.set_msgpara( (int)&tMsgModifyIntimacy );
	tMsgModifyIntimacy.set_time( nTime );
	tMsgModifyIntimacy.set_friendid( pPlayer->GetCharID() );
	tMsgModifyIntimacy.mutable_router()->set_descharid( nRoleID );
	CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
}

// 玩家下线的时候同步最后同时在线时间
void CFriendModule::UpdateLastTimeAllOnLine( CEntityPlayer *pPlayer )
{
	LK_ASSERT( pPlayer != NULL, return )	
	CFriendItem *tpFriendItem = GetPlayerFriendItem( pPlayer->GetCharID() );
	if ( tpFriendItem == NULL )
	{
		return;
	}	
	std::vector<int> tFriendList;
	tpFriendItem->GetFriendCharIDList( tFriendList, true );
	for ( std::vector<int>::iterator it = tFriendList.begin(); it != tFriendList.end(); ++it )
	{
		st_friend *tpFriendInfo = tpFriendItem->GetFriendByRoleID( *it );
		if ( tpFriendInfo == NULL )
		{
			continue;
		}
		tpFriendInfo->nLastTimeAllOnline = time( NULL );
		CEntityPlayer *tpFriend = CSceneLogic::GetSingleton().GetPlayerByCharID( *it );
		if ( tpFriend == NULL )
		{
			SendModifyLastTimeAllOnLineNotify(  pPlayer, *it, tpFriendInfo->nLastTimeAllOnline );
			continue;
		}
		CFriendItem *tpOtherFriendItem = GetPlayerFriendItem( *it );
		if ( tpOtherFriendItem == NULL )
		{
			continue;
		}
		st_friend *tpOtherFriendInfo = tpOtherFriendItem->GetFriendByRoleID( pPlayer->GetCharID() );
		// 已经被删除
		if ( tpOtherFriendInfo == NULL )
		{
			tpFriendItem->DelFriend( *it );
			continue;
		}
		tpOtherFriendInfo->nLastTimeAllOnline = tpFriendInfo->nLastTimeAllOnline;		
	}
}

// ***********************************************************
//  Function:		ChangeIntimacy	
//  Description:	减少两个玩家的亲密度
//  Date:			2011.06.28
// **********************************************************
void CFriendModule::ChangeIntimacy( int nRoleID1, int nRoleID2, EM_EntimacyChangeType eType, int nValue /* = 0 */ )
{
	int tOtherRoleID = nRoleID2;
	int tSelfRoleID = nRoleID1;
	CFriendItem *tpFriendItem = GetPlayerFriendItem( nRoleID1 );
	if ( tpFriendItem == NULL )
	{
		tpFriendItem = GetPlayerFriendItem( nRoleID2 );
		tOtherRoleID = nRoleID1;
		tSelfRoleID = nRoleID2;
	}

	// 都不在线
	if ( tpFriendItem == NULL )
	{
		return;
	}
	
	st_friend *tpFriend = tpFriendItem->GetFriendByRoleID( tOtherRoleID );
	if ( tpFriend == NULL )
	{
		return;
	}		
	switch ( eType )  
	{
		// 解散时按照百分比降低
		case ENTIMACY_CHANGE_TYPE_LEAVESWORN:
		case ENTIMACY_CHANGE_TYPE_DISBANDSWORN:
			{
				tpFriend->nIntimacyDegree -= tpFriend->nIntimacyDegree*nValue/100;
				break;
			}
		case ENTIMACY_CHANGE_TYPE_DIVORCE:
		case ENTIMACY_CHANGE_TYPE_KILL:					
			{
				tpFriend->nIntimacyDegree -= nValue;
				if ( tpFriend->nIntimacyDegree < 0 )
				{
					tpFriend->nIntimacyDegree = 0;
				}				
				break;				
			}
	
		case ENTIMACY_CHANGE_TYPE_TEAM:
			{
				nValue = tpFriend->AddIntimacy( nValue, GetFriendBlessConfig()->GetIntimacyConfig()->mMaxDailyDegree );
				break;
			}
		case ENTIMACY_CHANGE_TYPE_FLOWER:
			{	
				tpFriend->nIntimacyDegree += nValue;
				break;
			}	
		default:
			break;
	}

	// 判断是否超上限
	if ( tpFriend->nIntimacyDegree > mFriendBlessConfig.GetIntimacyConfig()->mMaxIntimacy )
	{
		tpFriend->nIntimacyDegree = mFriendBlessConfig.GetIntimacyConfig()->mMaxIntimacy;
	}	

	SendModifyIntimacyNotify( tSelfRoleID, nValue, tpFriend->nIntimacyDegree, eType, tOtherRoleID );
	CFriendItem *tpOtherFriendItem = GetPlayerFriendItem( tOtherRoleID );
	if ( tpOtherFriendItem == NULL )
	{
		SendModifyIntimacyNotify( tOtherRoleID, nValue, tpFriend->nIntimacyDegree, eType, tSelfRoleID, tpFriend->nLastUpdateTime, tpFriend->nDailyIntimacy );
		return;
	}
	st_friend *tpOtherFriend = tpOtherFriendItem->GetFriendByRoleID( tSelfRoleID );
	if ( tpOtherFriend == NULL )
	{
		return;
	}
	tpOtherFriend->nIntimacyDegree = tpFriend->nIntimacyDegree;
	tpOtherFriend->nLastUpdateTime = tpFriend->nLastUpdateTime;
	tpOtherFriend->nDailyIntimacy = tpFriend->nDailyIntimacy;
	SendModifyIntimacyNotify( tOtherRoleID, nValue, tpOtherFriend->nIntimacyDegree, eType, tSelfRoleID );
}

// ***********************************************************
//  Function:		UpDateTeamIntimacy	
//  Description:	更新组队玩家之间的亲密度
//  Date:			2011.06.28
// **********************************************************
void CFriendModule::UpDateTeamIntimacy( CTeam* tpTeam )
{
	LK_ASSERT( tpTeam != NULL, return )
	if ( time( NULL ) - tpTeam->mUpdateTime >= mFriendBlessConfig.GetIntimacyConfig()->mTeamIntimacyTime )
	{
		// 判断队伍人数
		CEntityPlayer *tTeamList[TEAMNUM] = { NULL };	
		int tTeamCount = 0;
		int tRtn = CTeamModule::GetSingletonPtr()->OnGetMemberList( tpTeam->get_id(),tTeamList, ARRAY_CNT( tTeamList ), tTeamCount );
		if ( tRtn < 0 || tTeamCount <= 1 )
		{			
			tpTeam->mUpdateTime = time( NULL );
			return;
		}  	 		
		for ( int i = 0; i < tTeamCount; ++i )
		{
			if ( tTeamList[i] == NULL )
			{
				continue;
			}			
			for ( int j = i+1; j < tTeamCount; ++j )
			{
				if ( tTeamList[j]  == NULL )
				{
					continue;
				}		
				if ( tTeamList[i]->IsWithinDistance( tTeamList[j], VIEW_RADIUS_Y ) == false )
				{
					continue;
				}							
				ChangeIntimacy( tTeamList[i]->GetCharID(), tTeamList[j]->GetCharID(), ENTIMACY_CHANGE_TYPE_TEAM, 1 );
			}			
		}				
		tpTeam->mUpdateTime = time( NULL );
	}	
}

// ***********************************************************
//  Function:		OnMessageCreateSwornTeam	
//  Description:	创建结拜的消息请求
//  Date:			2011.06.28
// **********************************************************
void CFriendModule::OnMessageCreateSwornTeam( CEntityPlayer *pPlayer, CMessage *pMsg )
{
	LK_ASSERT( pPlayer != NULL && pMsg != NULL, return )
	CMessageCreateSwornTeamRequest *tpMsg = ( CMessageCreateSwornTeamRequest * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )	
	
	// npc处结拜
	if ( pPlayer->GetNationality() != CSceneLogic::GetSingleton().GetLineID() )
	{
		return;
	}	

	// 是否有队伍,没有队伍不能结拜
	int tTeamID  = CTeamModule::GetSingleton().InTeam( pPlayer->GetCharID() );
	if ( tTeamID == INVALID_OBJ_ID )
	{
		SendResponseCreateSwornTeam( pPlayer, ERROR_SWORN_CREATE_HAVENOTEAM );
		return;				
	}		

	// 判断队伍人数
	CEntityPlayer *tTeamList[TEAMNUM] = { NULL };	
	int tTeamCount = 0;
	int tRtn = CTeamModule::GetSingletonPtr()->OnGetMemberList( tTeamID,tTeamList, ARRAY_CNT( tTeamList ), tTeamCount );
	if ( tRtn < 0 || tTeamCount <= 1 )
	{
		SendResponseCreateSwornTeam( pPlayer, ERROR_SWORN_CREATE_MEMBERNOTENOUGH );
		return;
	}  	 

	// 判断是不是队长
	CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( tTeamID );
	LK_ASSERT( tpTeam != NULL, return )
	if ( ( unsigned int )tpTeam->GetHeaderID() != pPlayer->GetCharID() )
	{
		SendResponseCreateSwornTeam( pPlayer, ERROR_SWORN_CREATE_NOTHEADER );		
		return;
	}	

	// 检查是否已经结拜
	if ( pPlayer->GetSwornID() > 0 )
	{
		SendResponseCreateSwornTeam( pPlayer, ERROR_SWORN_CREATE_INSWORNTEAM );		
		return;
	}	
	
	// 检查有没有结拜		
	CSwornTeam tSwornTeam;
	int tNum = 1;
	int tTeamRoleID[TEAM_NUMBER] = { pPlayer->GetCharID() };
	CSwornMember tSwornMember;
	tSwornMember.mRoleID = pPlayer->GetCharID();
	tSwornMember.mTeamPos = tNum;	
	tSwornMember.mRoleStatus = CSwornTeam::EROLE_STATUS_ONLINE;
	tSwornTeam.InsertMember( tSwornMember );
	for ( int i = 0; i < tTeamCount; ++i )
	{	
		if ( tTeamList[i] == NULL )
		{
			continue;
		}		
		if ( tTeamList[i]->GetCharID() == pPlayer->GetCharID() )
		{
			continue;
		}	
		// 判断距离
		if ( tTeamList[i]->IsWithinDistance( pPlayer, VIEW_RADIUS_Y ) == false )
		{
			SendResponseCreateSwornTeam( pPlayer, ERROR_SWORN_CREATE_MEMBERTOOFAR );		
			return;
		}
		
		if ( tTeamList[i]->GetSwornID() > 0 )
		{
			SendResponseCreateSwornTeam( pPlayer, ERROR_SWORN_CREATE_INSWORNTEAM );		
			return;
		}		
		tSwornMember.mRoleID = tTeamList[i]->GetCharID();
		tSwornMember.mTeamPos = tNum+1;
		tSwornMember.mRoleStatus = CSwornTeam::EROLE_STATUS_ONLINE;
		tSwornTeam.InsertMember( tSwornMember );	
		tTeamRoleID[tNum] = tTeamList[i]->GetCharID();
		++tNum;
	}
	bool bRet = JudgeIntimacyDegree( tTeamRoleID, tNum, mFriendBlessConfig.GetIntimacyConfig()->mSwornIntimacy );
	if ( bRet == false )
	{
		SendResponseCreateSwornTeam( pPlayer, ERROR_SWORN_CREATE_INTIMACYNOTENOUGH );		
		return;
	}	

	// 查看结拜名称是否有脏词
	if ( CDirtyWord::GetSingleton().OI_Dirty_Check( 3, ( char * )tpMsg->mutable_swornname()->c_str() ) != 0 )
	{
		SendResponseCreateSwornTeam( pPlayer, ERROR_SWORN_CREATE_HASDITYWORD );		
		return;
	}

	// 判断有没有物品
	int tItemID = mFriendBlessConfig.GetIntimacyConfig()->mItemCreateSworn;
	if ( ( (CPropertyPlayer *)pPlayer->GetProperty() )->GetBaggage()->HasItem( tItemID, 1, BIND_ALL ) == false )
	{
		SendResponseCreateSwornTeam( pPlayer, ERROR_SWORN_CREATE_HASNOTIME );
		return;
	}	
	
	// 设置结拜信息
	tSwornTeam.SetSwornID( pPlayer->GetCharID() );
	tSwornTeam.SetSwornName( tpMsg->mutable_swornname()->c_str() );	
	tSwornTeam.SetCampID( pPlayer->GetNationality() );
	InsertSwornTeam( pPlayer->GetCharID(), tSwornTeam );	

	// 发送结拜成功的回应消息
	SendResponseCreateSwornTeam( pPlayer, SUCCESS );

	// 扣除物品
	bool tBind = false;
	uint64_t ullGUID = 0;
	CPropertyModule::GetSingleton().PlayerRemoveItemByID( pPlayer, tItemID, 1, tBind , &ullGUID );
	LogEventLostItemBySworn(pPlayer, ullGUID, tItemID, 1, pPlayer->GetSwornID());


	// 发送结拜通知
	SendNotifyCreateSwornTeam( tTeamList, tTeamCount, &tSwornTeam );

	// 设置每个人的结拜ID和称号
	for ( int i = 0; i < tTeamCount; ++i )
	{
		if ( tTeamList[i] != NULL )
		{
			tTeamList[i]->SetSwornID( pPlayer->GetCharID() );
			CPropertyModule::GetSingleton().OnPlayerAddTitleEvent( tTeamList[i], 
				mFriendBlessConfig.GetIntimacyConfig()->mSwornTitleID );
			LOG_INFO( "FRIEND", "player %d join sworn of %d", tTeamList[i]->GetCharID(), pPlayer->GetCharID() );
		}		
	}
	
	// 创建DB信息
	CreateSwornDBInfo( tSwornTeam );
}

// ***********************************************************
//  Function:		SendResponseCreateSwornTeam	
//  Description:	创建结拜的回应消息
//  Date:			2011.06.28
// **********************************************************
void CFriendModule::SendResponseCreateSwornTeam( CEntityPlayer *pPlayer, int nErrcode )
{
	LK_ASSERT( pPlayer != NULL, return )	
	CMessage tMsgHead;
	CMessageCreateSwornTeamResponse tMsgCreateSworn;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_CREATESWORNTEAM );				
	tMsgHead.set_msgpara( (int)&tMsgCreateSworn );
	tMsgCreateSworn.set_errcode( nErrcode );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
}

// ***********************************************************
//  Function:		SendNotifyCreateSwornTeam	
//  Description:	创建结拜的通知
//  Date:			2011.06.28
// **********************************************************
void CFriendModule::SendNotifyCreateSwornTeam( CEntityPlayer **pPlayer, int nTeamCount, CSwornTeam *pSwornTeam )
{
	LK_ASSERT( pSwornTeam != NULL && pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageCreateSwornTeamNotify tMsgCreateSwornTeam;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CREATESWORNTEAM );
	tMsgHead.set_msgpara( (int)&tMsgCreateSwornTeam );

	for ( int i = 0; i < nTeamCount; ++i )
	{
		if ( pPlayer[i] == NULL )
		{
			continue;
		}

		CMsgSwornMember *pbMember = tMsgCreateSwornTeam.add_swornmember();
		pbMember->set_roleid( pPlayer[i]->GetCharID() );
		pbMember->set_rolename( pPlayer[i]->GetCharName() );		
		pbMember->set_rolepos( pSwornTeam->GetTeamPos( pPlayer[i]->GetCharID() ) );
		pbMember->set_faceid(  pPlayer[i]->GetFaceType() );
		pbMember->set_genderid( pPlayer[i]->GetSex() );
		pbMember->set_metierid( pPlayer[i]->GetMetier() );
		pbMember->set_rolelevel( pPlayer[i]->GetLevel() );
	}
	tMsgCreateSwornTeam.set_swornname( pSwornTeam->GetSwornName() );
	for ( int i = 0; i < nTeamCount; ++i )
	{
		if ( pPlayer[i] == NULL )
		{
			continue;
		}
		CSceneLogic::GetSingleton().Send2Player( pPlayer[i], &tMsgHead );
	}
}

// ***********************************************************
//  Function:		OnMessageLeaveSwornTeam	
//  Description:	离开结拜团体的请求
//  Date:			2011.06.28
// **********************************************************
void CFriendModule::OnMessageLeaveSwornTeam( CEntityPlayer *pPlayer, CMessage *pMsg )
{
	LK_ASSERT( pPlayer != NULL && pMsg != NULL, return )
	CMessageLeaveSwornTeamRequest *tpMsg = ( CMessageLeaveSwornTeamRequest * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )		
	if ( pPlayer->GetNationality() != CSceneLogic::GetSingleton().GetLineID() )
	{
		return;
	}	
	CSwornTeam *tpSwornTeam = GetSwornTeam( pPlayer->GetSwornID() );
	if ( tpSwornTeam == NULL )
	{
		return;
	}
	if ( tpSwornTeam->HasMember( pPlayer->GetCharID() ) == false )
	{
		return;
	}

	LOG_INFO( "FRIEND", "player %d leave sworn of %d", pPlayer->GetCharID(), tpSwornTeam->GetSwornID() );

	// 如果只剩两个人就直接解散	
	if ( tpSwornTeam->GetSwornMember()->size() <= 2 )
	{
		CSwornTeam::SwornMemberList::iterator it = tpSwornTeam->GetSwornMember()->begin();
		for ( ; it != tpSwornTeam->GetSwornMember()->end(); ++it )
		{
			CEntityPlayer *tpMember = CSceneLogic::GetSingleton().GetPlayerByCharID( it->mRoleID );
			if ( tpMember != NULL )
			{
				tpMember->SetSwornID( 0 );
				CPropertyModule::GetSingleton().OnPlayerDelTitleEvent( tpMember, mFriendBlessConfig.GetIntimacyConfig()->mSwornTitleID );
			}		
			if ( it->mRoleID != pPlayer->GetCharID() )
			{
				ChangeIntimacy( pPlayer->GetCharID(), it->mRoleID, ENTIMACY_CHANGE_TYPE_DISBANDSWORN, mFriendBlessConfig.GetIntimacyConfig()->mDisbandReduced );
			}		
			/*else
			{
				UpDateRoleSwornID( it->mRoleID, 0 );
			}*/
		}
		SendNotifyDisbandSwornTeam( tpSwornTeam );
		DeleteSwornDBInfo( tpSwornTeam->GetSwornID() );
		DeleteSwornTeam( tpSwornTeam->GetSwornID() );
		return;
	}
	CPropertyModule::GetSingleton().OnPlayerDelTitleEvent( pPlayer, mFriendBlessConfig.GetIntimacyConfig()->mSwornTitleID );
	tpSwornTeam->DeleteMember( pPlayer->GetCharID() );	
	pPlayer->SetSwornID( 0 );
	int tOldSwornID = tpSwornTeam->GetSwornID();
	int tNewSwornID = tpSwornTeam->GetSwornID();

	// 发送回应消息
	SendResponseLeaveSwornTeam( pPlayer, SUCCESS );

	// 如果是老大的话要做特殊操作
	if ( tpSwornTeam->GetSwornID() == pPlayer->GetCharID() )
	{
		CSwornTeam tSwornTeam = *tpSwornTeam;
		tSwornTeam.SetSwornID( tSwornTeam.FindFirstPos() );			
		DeleteSwornTeam( tOldSwornID );
		InsertSwornTeam( tSwornTeam.GetSwornID(), tSwornTeam );
		tpSwornTeam = GetSwornTeam( tSwornTeam.GetSwornID() );
	}

	// 改变所有在线成员的结拜ID,和减少亲密度	
	CFriendItem *tpItem = GetPlayerFriendItem( pPlayer->GetCharID() );	
	CSwornTeam::SwornMemberList::iterator it = tpSwornTeam->GetSwornMember()->begin();
	for ( ; it != tpSwornTeam->GetSwornMember()->end(); ++it )
	{
		CEntityPlayer *tpMember = CSceneLogic::GetSingleton().GetPlayerByCharID( it->mRoleID );
		if ( tpMember != NULL )
		{	
			tpMember->SetSwornID( tpSwornTeam->GetSwornID() );
			// 重新发送结拜信息
			SendNotifySwornInfo( tpMember, tpSwornTeam );
		}	
		else
		{
			UpDateRoleSwornID( it->mRoleID, tpSwornTeam->GetSwornID() );
		}
		ChangeIntimacy( pPlayer->GetCharID(), it->mRoleID, ENTIMACY_CHANGE_TYPE_LEAVESWORN, mFriendBlessConfig.GetIntimacyConfig()->mLeaveReduced );
	}	

	// 发送通知
	SendNotifyLeaveSwornTeam( pPlayer, tpSwornTeam );
	UpdateSwornDBInfo( tOldSwornID, *tpSwornTeam );
}

// ***********************************************************
//  Function:		SendResponseLeaveSwornTeam	
//  Description:	离开结拜团体的消息回应
//  Date:			2011.06.28
// **********************************************************
void CFriendModule::SendResponseLeaveSwornTeam( CEntityPlayer *pPlayer, int nErrcode )
{
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageLeaveSwornTeamResponse tMsgLeaveSwornTeam;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_LEAVESWORNTEAM );
	tMsgHead.set_msgpara( (int)&tMsgLeaveSwornTeam );
	tMsgLeaveSwornTeam.set_errcode( nErrcode );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
}

// ***********************************************************
//  Function:		SendNotifyLeaveSwornTeam	
//  Description:	离开结拜团体的消息通知
//  Date:			2011.06.28
// **********************************************************
void CFriendModule::SendNotifyLeaveSwornTeam( CEntityPlayer *pPlayer, CSwornTeam *pSwornTeam )
{
	LK_ASSERT( pPlayer != NULL && pSwornTeam != NULL, return )
	CMessage tMsgHead;
	CMessageLeaveSwornTeamNotify tMsgLeaveSwornTeam;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_LEAVESWORNTEAM );
	tMsgHead.set_msgpara( (int)&tMsgLeaveSwornTeam );
	tMsgLeaveSwornTeam.set_roleid( pPlayer->GetCharID() );
	tMsgLeaveSwornTeam.set_masterid( pSwornTeam->GetSwornID() );
	CSwornTeam::SwornMemberList::iterator it = pSwornTeam->GetSwornMember()->begin();
	for ( ; it != pSwornTeam->GetSwornMember()->end(); ++it )
	{
		CEntityPlayer *tpMember = CSceneLogic::GetSingleton().GetPlayerByCharID( it->mRoleID );
		if ( tpMember != NULL )
		{
			CSceneLogic::GetSingleton().Send2Player( tpMember, &tMsgHead );
		}
		else
		{
			tMsgLeaveSwornTeam.mutable_router()->set_descharid( it->mRoleID );
			CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
		}
	}	
}

// ***********************************************************
//  Function:		OnMessageKickSwornMember	
//  Description:	踢人请求
//  Date:			2011.06.28
// **********************************************************
void CFriendModule::OnMessageKickSwornMember( CEntityPlayer *pPlayer, CMessage *pMsg )
{
	LK_ASSERT( pPlayer != NULL && pMsg != NULL, return  )
	if ( pPlayer->GetNationality() != CSceneLogic::GetSingleton().GetLineID() )
	{
		return;
	}
	if ( pPlayer->GetSwornID() <= 0 )
	{
		return;
	}
	CSwornTeam *tpSwornTeam = GetSwornTeam( pPlayer->GetSwornID() );
	if ( tpSwornTeam == NULL )
	{
		return;
	}
	CMessageKickSwornMemberRequest *tpMsg = ( CMessageKickSwornMemberRequest * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	if ( tpSwornTeam->GetSwornID() != pPlayer->GetCharID() )
	{
		SendResponseKickSwornMember( pPlayer, ERROR_SWORN_KICK_HASNORIGHT );
		return;
	}	
	if ( pPlayer->GetCharID() == tpMsg->roleid() )
	{
		return;
	}

	// 判断有没有物品
	int tItemID = mFriendBlessConfig.GetIntimacyConfig()->mItemKickMember;
	if ( ( (CPropertyPlayer *)pPlayer->GetProperty() )->GetBaggage()->HasItem( tItemID, 1, BIND_ALL ) == false )
	{
		SendResponseCreateSwornTeam( pPlayer, ERROR_SWORN_KICKMEMBER_HASNOTIME );
		return;
	}	

	// 扣除物品
	bool tBind = false;
	uint64_t ullGUID = 0;
	CPropertyModule::GetSingleton().PlayerRemoveItemByID( pPlayer, tItemID, 1, tBind , &ullGUID );
	LogEventLostItemBySworn(pPlayer, ullGUID, tItemID, 1, pPlayer->GetSwornID());

	LOG_INFO( "FRIEND", "player %d kicked sworn of %d", pPlayer->GetCharID(), tpSwornTeam->GetSwornID() );

	// 如果只剩两个人就直接解散
	if ( tpSwornTeam->GetSwornMember()->size() <= 2 )
	{
		CSwornTeam::SwornMemberList::iterator it = tpSwornTeam->GetSwornMember()->begin();
		for ( ; it != tpSwornTeam->GetSwornMember()->end(); ++it )
		{
			CEntityPlayer *tpMember = CSceneLogic::GetSingleton().GetPlayerByCharID( it->mRoleID );
			if ( tpMember != NULL )
			{
				tpMember->SetSwornID( 0 );
				CPropertyModule::GetSingleton().OnPlayerDelTitleEvent( tpMember, mFriendBlessConfig.GetIntimacyConfig()->mSwornTitleID );
			}		
			if ( it->mRoleID != pPlayer->GetCharID() )
			{
				ChangeIntimacy( pPlayer->GetCharID(), it->mRoleID, ENTIMACY_CHANGE_TYPE_DISBANDSWORN, mFriendBlessConfig.GetIntimacyConfig()->mDisbandReduced );
			}		
			/*else
			{
				UpDateRoleSwornID( it->mRoleID, 0 );
			}*/
		}
		SendNotifyDisbandSwornTeam( tpSwornTeam );
		DeleteSwornDBInfo( tpSwornTeam->GetSwornID() );
		DeleteSwornTeam( tpSwornTeam->GetSwornID() );
		return;
	}
	// 存储数据
	tpSwornTeam->DeleteMember( tpMsg->roleid() );
	UpdateSwornDBInfo( tpSwornTeam->GetSwornID(), *tpSwornTeam );	
	ChangeIntimacy( pPlayer->GetCharID(), tpMsg->roleid(), ENTIMACY_CHANGE_TYPE_LEAVESWORN, mFriendBlessConfig.GetIntimacyConfig()->mLeaveReduced );	
	SendNotifyKickSwornMember( pPlayer, tpSwornTeam, tpMsg->roleid() );	
	SendResponseKickSwornMember( pPlayer, SUCCESS );
	
}

// ***********************************************************
//  Function:		SendResponseKickSwornMember	
//  Description:	踢人回应
//  Date:			2011.06.28
// **********************************************************
void CFriendModule::SendResponseKickSwornMember( CEntityPlayer *pPlayer, int nErrcode )
{
	CMessage tMsgHead;
	CMessageKickSwornMemberResponse tMsgKickSwornMember;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_KICKSWORNMEMBER );
	tMsgHead.set_msgpara( (int)&tMsgKickSwornMember );
	tMsgKickSwornMember.set_errcode( nErrcode );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
}

// ***********************************************************
//  Function:		SendNotifyKickSwornMember	
//  Description:	踢人通知
//  Date:			2011.06.28
// **********************************************************
void CFriendModule::SendNotifyKickSwornMember( CEntityPlayer *pPlayer, CSwornTeam *pSwornTeam, int nRoleID )
{
	LK_ASSERT( pPlayer != NULL && pSwornTeam != NULL, return )
	CMessage tMsgHead;
	CMessageKickSwornMemberNotify tMsgKickSwornMember;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_KICKSWORNMEMBER );
	tMsgHead.set_msgpara( (int)&tMsgKickSwornMember );
	tMsgKickSwornMember.set_roleid( nRoleID );  	
	CEntityPlayer *tpKickedPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( nRoleID );
	if ( tpKickedPlayer != NULL )
	{
		tpKickedPlayer->SetSwornID( 0 );
		CPropertyModule::GetSingleton().OnPlayerDelTitleEvent( tpKickedPlayer, mFriendBlessConfig.GetIntimacyConfig()->mSwornTitleID );
		CSceneLogic::GetSingleton().Send2Player( tpKickedPlayer, &tMsgHead );
	}
	else
	{
		tMsgKickSwornMember.mutable_router()->set_descharid( nRoleID );
		CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
	}
	CSwornTeam::SwornMemberList::iterator it = pSwornTeam->GetSwornMember()->begin();
	for ( ; it != pSwornTeam->GetSwornMember()->end(); ++it )
	{
		CEntityPlayer *tpMember = CSceneLogic::GetSingleton().GetPlayerByCharID( it->mRoleID );
		if ( tpMember == NULL )
		{
			tMsgKickSwornMember.mutable_router()->set_descharid( it->mRoleID );
			CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
			continue;
		}
		SendNotifySwornInfo( tpMember, pSwornTeam );
		CSceneLogic::GetSingleton().Send2Player( tpMember, &tMsgHead );
	}
}

// ***********************************************************
//  Function:		OnMessageDisbandSwornTeam	
//  Description:	踢人通知
//  Date:			2011.06.28
// **********************************************************
void CFriendModule::OnMessageDisbandSwornTeam( CEntityPlayer *pPlayer, CMessage *pMsg )
{
	LK_ASSERT( pPlayer != NULL && pMsg != NULL, return )
	CMessageDisbandSwornTeamRequest *tpMsg = ( CMessageDisbandSwornTeamRequest * )pMsg->msgpara(); 
	LK_ASSERT( tpMsg != NULL, return )	
	CSwornTeam *tpSwornTeam = GetSwornTeam( pPlayer->GetSwornID() );
	if ( tpSwornTeam == NULL )
	{
		return;
	}
	if ( tpSwornTeam->GetSwornID() != pPlayer->GetCharID() )
	{
		SendResponseDisbandSwornTeam( pPlayer, ERROR_SWORN_DISBAND_HASNORIGHT );
		return;
	}
	// 判断有没有物品
	int tItemID = mFriendBlessConfig.GetIntimacyConfig()->mItemDisbandSworn;
	if ( ( (CPropertyPlayer *)pPlayer->GetProperty() )->GetBaggage()->HasItem( tItemID, 1, BIND_ALL ) == false )
	{
		SendResponseCreateSwornTeam( pPlayer, ERROR_SWORN_DISBAND_HASNOTIME );
		return;
	}	
	// 扣除物品
	bool tBind = false;
	uint64_t ullGUID = 0;
	CPropertyModule::GetSingleton().PlayerRemoveItemByID( pPlayer, tItemID, 1, tBind , &ullGUID );
	LogEventLostItemBySworn(pPlayer, ullGUID, tItemID, 1, pPlayer->GetSwornID());
	// 降低所有人之间的亲密度
	int tpRoleIDList[TEAM_NUMBER] ={ 0 };
	int tNum = ARRAY_CNT( tpRoleIDList );
	tpSwornTeam->GetRoleIDList( tpRoleIDList, tNum );
	int tValue = mFriendBlessConfig.GetIntimacyConfig()->mDisbandReduced;
	ChangeGroupIntimacyDegree( tpRoleIDList, tNum, ENTIMACY_CHANGE_TYPE_DISBANDSWORN, tValue );
	CSwornTeam::SwornMemberList::iterator it = tpSwornTeam->GetSwornMember()->begin();
	for ( ; it != tpSwornTeam->GetSwornMember()->end(); ++it )
	{
		CEntityPlayer *tpMember = CSceneLogic::GetSingleton().GetPlayerByCharID( it->mRoleID );
		if ( tpMember != NULL )
		{
			tpMember->SetSwornID( 0 );
			CPropertyModule::GetSingleton().OnPlayerDelTitleEvent( tpMember, mFriendBlessConfig.GetIntimacyConfig()->mSwornTitleID );
			LOG_INFO( "FRIEND", "player %d dismissed sworn of %d", tpMember->GetCharID(), tpSwornTeam->GetSwornID() );
		}		
		/*else
		{
			UpDateRoleSwornID( it->mRoleID, 0 );
		}*/
	}
	SendResponseDisbandSwornTeam( pPlayer, SUCCESS );
	SendNotifyDisbandSwornTeam( tpSwornTeam );
	DeleteSwornDBInfo( tpSwornTeam->GetSwornID() );
	DeleteSwornTeam( tpSwornTeam->GetSwornID() );
}

// ***********************************************************
//  Function:		SendResponseDisbandSwornTeam	
//  Description:	解散结拜团体的回应
//  Date:			2011.06.28
// **********************************************************
void CFriendModule::SendResponseDisbandSwornTeam( CEntityPlayer *pPlayer, int nErrcode )
{
	CMessage tMsgHead;
	CMessageDisbandSwornTeamResponse tMsgDisbandSwornTeam;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_DISBANDSWORNTEAM );
	tMsgHead.set_msgpara( (int)&tMsgDisbandSwornTeam );
	tMsgDisbandSwornTeam.set_errcode( nErrcode );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
}

// ***********************************************************
//  Function:		SendNotifyDisbandSwornTeam	
//  Description:	解散结拜团体的通知
//  Date:			2011.06.28
// **********************************************************
void CFriendModule::SendNotifyDisbandSwornTeam( CSwornTeam *pSwornTeam )
{
	LK_ASSERT( pSwornTeam != NULL, return )
	CMessage tMsgHead;
	CMessageDisbandSwornTeamNotify tMsgDisbandSwornTeam;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_DISBANDSWORNTEAM );
	tMsgHead.set_msgpara( (int)&tMsgDisbandSwornTeam );
	tMsgDisbandSwornTeam.set_swornid( pSwornTeam->GetSwornID() );
	CSwornTeam::SwornMemberList::iterator it = pSwornTeam->GetSwornMember()->begin();
	for ( ; it != pSwornTeam->GetSwornMember()->end(); ++it )
	{
		CEntityPlayer *tpMember = CSceneLogic::GetSingleton().GetPlayerByCharID( it->mRoleID );
		if ( tpMember != NULL )
		{
			CSceneLogic::GetSingleton().Send2Player( tpMember, &tMsgHead );
		}		
		else
		{
			tMsgDisbandSwornTeam.mutable_router()->set_descharid( it->mRoleID );
			CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
		}
	}
}

// ***********************************************************
//  Function:		SendNotifySwornInfo	
//  Description:	发送结拜信息
//  Date:			2011.06.28
// **********************************************************
void CFriendModule::SendNotifySwornInfo( CEntityPlayer *pPlayer, CSwornTeam *pSwornTeam )
{
	LK_ASSERT( pPlayer != NULL && pSwornTeam != NULL, return )
	CMessage tMsgHead;
	CMessageSwornInfoNotify tSwornInfo;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_SWORNINFO );
	tMsgHead.set_msgpara( (int)&tSwornInfo );
	tSwornInfo.set_swornname( pSwornTeam->GetSwornName() );
	CSwornTeam::SwornMemberList::iterator it = pSwornTeam->GetSwornMember()->begin();
	for ( ; it != pSwornTeam->GetSwornMember()->end(); ++it )
	{
		CMsgSwornMember *pbSwornMember = tSwornInfo.add_swornmember();
		pbSwornMember->set_roleid( it->mRoleID );
		pbSwornMember->set_rolepos( it->mTeamPos );
	}
	tSwornInfo.set_roleid( pPlayer->GetCharID() );
	CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
}

// ***********************************************************
//  Function:		OnMessageChangeSwornName	
//  Description:	改变结拜团体名称的请求
//  Date:			2011.06.28
// **********************************************************
void CFriendModule::OnMessageChangeSwornName( CEntityPlayer *pPlayer, CMessage *pMsg )
{
	LK_ASSERT( pPlayer != NULL && pMsg != NULL, return )  
	CMessageChangeSwornNameRequest *tpMsg = ( CMessageChangeSwornNameRequest * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	if ( pPlayer->GetNationality() != CSceneLogic::GetSingleton().GetLineID() )
	{
		return;
	}
	CSwornTeam *tpSwornTeam = GetSwornTeam( pPlayer->GetSwornID() );
	if ( tpSwornTeam == NULL )
	{
		return;
	}
	if ( tpSwornTeam->GetSwornID() != pPlayer->GetCharID() )
	{
		SendResponseChangeSwornName( pPlayer, ERROR_SWORN_CHANGENAME_HASNOWRIGHT );
		return;
	}

	// 检查是否有脏词
	if ( CDirtyWord::GetSingleton().OI_Dirty_Check( 3, (char *)tpMsg->mutable_swornname()->c_str() ) != 0 )
	{
		SendResponseChangeSwornName( pPlayer, ERROR_SWORN_CHANGENAME_HASDIRTYWORD );
		return;
	}
	// 判断有没有物品
	int tItemID = mFriendBlessConfig.GetIntimacyConfig()->mItemChangeName;
	if ( ( (CPropertyPlayer *)pPlayer->GetProperty() )->GetBaggage()->HasItem( tItemID, 1, BIND_ALL ) == false )
	{
		SendResponseCreateSwornTeam( pPlayer, ERROR_SWORN_CHANGENAME_HASNOTIME );
		return;
	}	
	// 扣除物品
	bool tBind = false;
	uint64_t ullGUID = 0;
	CPropertyModule::GetSingleton().PlayerRemoveItemByID( pPlayer, tItemID, 1, tBind , &ullGUID );
	LogEventLostItemBySworn(pPlayer, ullGUID, tItemID, 1, pPlayer->GetSwornID());
	SendResponseChangeSwornName( pPlayer, SUCCESS );
	tpSwornTeam->SetSwornName( tpMsg->mutable_swornname()->c_str() );
	UpdateSwornDBInfo( tpSwornTeam->GetSwornID(), *tpSwornTeam ) ;
	SendNotifyChangeSwornName( tpSwornTeam );
}

// ***********************************************************
//  Function:		SendResponseChangeSwornName	
//  Description:	改变结拜团体名称的请求
//  Date:			2011.06.28
// **********************************************************
void CFriendModule::SendResponseChangeSwornName( CEntityPlayer *pPlayer, int nErrcode )
{
	CMessage tMsgHead;
	CMessageChangeSwornNameResponse tChangeSwornName;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CHANGESWORNNAME );
	tMsgHead.set_msgpara( (int)&tChangeSwornName );
	tChangeSwornName.set_errcode( nErrcode );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
}

// ***********************************************************
//  Function:		SendNotifyChangeSwornName	
//  Description:	改变结拜团体名称的通知
//  Date:			2011.06.28
// **********************************************************
void CFriendModule::SendNotifyChangeSwornName( CSwornTeam *pSwornTeam )
{
	LK_ASSERT( pSwornTeam != NULL , return )
	CMessage tMsgHead;
	CMessageChangeSwornNameNotify tMsgChangeSwornName;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CHANGESWORNNAME );
	tMsgHead.set_msgpara( (int)&tMsgChangeSwornName );
	tMsgChangeSwornName.set_swornname( pSwornTeam->GetSwornName() );
	CSwornTeam::SwornMemberList::iterator it = pSwornTeam->GetSwornMember()->begin();
	for ( ; it != pSwornTeam->GetSwornMember()->end(); ++it )
	{
		CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( it->mRoleID );
		if ( tpPlayer != NULL )
		{	
			CPropertyPlayer *tpProperty = ( CPropertyPlayer * )tpPlayer->GetProperty();
			if ( tpProperty->GetCurrentTitle() == GetFriendBlessConfig()->GetIntimacyConfig()->mSwornTitleID )
			{
				CMessage tMessage;
				CMessageUpdateCurrentTitleNotice tPara;
				tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTICE_UPDATECURRENTTITLE );
				tMessage.set_msgpara( (long) &tPara );
				tPara.set_charid( tpPlayer->GetCharID() );
				tPara.set_titleid( tpProperty->GetCurrentTitle() );
				tPara.set_titlename( pSwornTeam->GetSwornName() );
				CMapModule::GetSingleton().MessageBroadCast( tpPlayer, &tMessage, false, true );
			}			
			CSceneLogic::GetSingleton().Send2Player( tpPlayer, &tMsgHead );			
		}
		else
		{
			tMsgChangeSwornName.mutable_router()->set_descharid( it->mRoleID );
			CSceneLogic::GetSingleton().Send2Gate( &tMsgHead );
		}
	}
}

// ***********************************************************
//  Function:		OnMessageRebuildSworn	
//  Description:	重建结拜的消息请求
//  Date:			2011.08.30
// **********************************************************
void CFriendModule::OnMessageRebuildSworn( CEntityPlayer *pPlayer, CMessage *pMsg )
{
	LK_ASSERT( pPlayer != NULL && pMsg != NULL, return )
	CMessageRebuildSwornRequest *tpMsg = ( CMessageRebuildSwornRequest * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )

	// npc处结拜
	if ( pPlayer->GetNationality() != CSceneLogic::GetSingleton().GetLineID() )
	{
		return;
	}	

	// 没有结拜不能加人
	CSwornTeam *tpSwornTeam = GetSwornTeam( pPlayer->GetSwornID() );
	if ( tpSwornTeam == NULL )
	{
		SendResponseRebuildSworn( pPlayer, ERROR_SWORN_REBUILD_HASNOSWORN );
		return;
	}	

	if ( pPlayer->GetCharID() != tpSwornTeam->GetSwornID() )
	{
		SendResponseRebuildSworn( pPlayer, ERROR_SWORN_REBUILD_NOTHEADER );		
		return;
	}
	

	// 是否有队伍,没有队伍不能重建结拜
	int tTeamID  = CTeamModule::GetSingleton().InTeam( pPlayer->GetCharID() );
	if ( tTeamID == INVALID_OBJ_ID )
	{
		SendResponseRebuildSworn( pPlayer, ERROR_SWORN_REBUILD_HASNOTEAM );
		return;				
	}		

	// 判断队伍人数
	CEntityPlayer *tTeamList[TEAMNUM] = { NULL };	
	int tTeamCount = 0;
	int tRtn = CTeamModule::GetSingletonPtr()->OnGetMemberList( tTeamID,tTeamList, ARRAY_CNT( tTeamList ), tTeamCount );
	if ( tRtn < 0 || tTeamCount <= 1 )
	{
		SendResponseRebuildSworn( pPlayer, ERROR_SWORN_REBUILD_HASNOTEAM );
		return;
	}  	 

	// 判断是不是队长
	CTeam* tpTeam = ( CTeam* ) CSceneObjManager::GetSingleton().GetObject( tTeamID );
	LK_ASSERT( tpTeam != NULL, return )
	if ( ( unsigned int )tpTeam->GetHeaderID() != pPlayer->GetCharID() )
	{
		SendResponseRebuildSworn( pPlayer, ERROR_SWORN_REBUILD_NOTHEADER );		
		return;
	}

	// 判断是否所有结拜兄弟都在队伍中
	CEntityPlayer *tNewMember[TEAMNUM] = { NULL };
	int tNewNum = 0;	
	int tTeamSwornNum = 0;
	int tTeamRoleID[TEAM_NUMBER] = { 0 };
	int tRoleNum = 0;
	for ( int i = 0; i < tTeamCount; ++i )
	{
		if ( tTeamList[i] == NULL )
		{
			continue;
		}		
		if ( tTeamList[i]->IsWithinDistance( pPlayer, VIEW_RADIUS_Y ) == false )
		{
			SendResponseRebuildSworn( pPlayer, ERROR_SWORN_REBUILD_MEBERTOOFAR );	
			return;
		}		
		tTeamRoleID[tRoleNum] = tTeamList[i]->GetCharID();
		++tRoleNum;
		if ( tTeamList[i]->GetSwornID() == pPlayer->GetSwornID() )
		{
			++tTeamSwornNum;
		}
		else
		{
			if ( tTeamList[i]->GetSwornID() != 0 )
			{
				SendResponseRebuildSworn( pPlayer, ERROR_SWORN_REBUILD_NEWMEMBERHASSWORN );	
				return;
			}		
			if ( tTeamList[i]->GetNationality() != pPlayer->GetNationality() )
			{				
				return;
			}			
			// 判断新成员有没有道具
			if ( mFriendBlessConfig.GetIntimacyConfig()->mRebuildItemID > 0 )
			{
				if ( ( (CPropertyPlayer *)tTeamList[i]->GetProperty() )->GetBaggage()->HasItem( mFriendBlessConfig.GetIntimacyConfig()->mRebuildItemID , 1, BIND_ALL ) == false )
				{
					SendResponseRebuildSworn( pPlayer, ERROR_SWORN_REBUILD_HASNOREBUILDITEM );
					return;
				}				
			}	
			tNewMember[tNewNum] = tTeamList[i];
			++tNewNum;
		}
	}
	
	// 判断是否所有结拜成员都在队伍中
	if ( tTeamSwornNum < tpSwornTeam->GetNumber() )
	{
		SendResponseRebuildSworn( pPlayer, ERROR_SWORN_REBUILD_MEMBEROFFLIEN );	
		return;
	}

	// 判断是否有新成员
	if ( tNewNum <= 0 )
	{
		SendResponseRebuildSworn( pPlayer, ERROR_SWORN_REBUILD_HASNONEWMEMBER );	
		return;
	}
	
	// 判断亲密度
	bool bRet = JudgeIntimacyDegree( tTeamRoleID, tRoleNum, mFriendBlessConfig.GetIntimacyConfig()->mSwornIntimacy );
	if ( bRet == false )
	{
		SendResponseRebuildSworn( pPlayer, ERROR_SWORN_REBUILD_INTIMACYNOTENOUGH );		
		return;
	}	

	// 操作成功
	SendResponseRebuildSworn( pPlayer, SUCCESS );
	// 加入操作
	int tMaxPos = tpSwornTeam->GetMaxPos()+1;
	for ( int i = 0; i < tNewNum; ++i )
	{			
		// 扣除物品
		if ( mFriendBlessConfig.GetIntimacyConfig()->mRebuildItemID > 0 )
		{
			bool tBind = false;
			uint64_t ullGUID = 0;
			CPropertyModule::GetSingleton().PlayerRemoveItemByID( tNewMember[i], mFriendBlessConfig.GetIntimacyConfig()->mRebuildItemID, 1, tBind ,&ullGUID);
			LogEventLostItemBySworn(tNewMember[i], ullGUID, mFriendBlessConfig.GetIntimacyConfig()->mRebuildItemID, 1, pPlayer->GetSwornID());
		}	
		CSwornMember tMember;
		tMember.mRoleID = tNewMember[i]->GetCharID();
		tMember.mRoleStatus = CSwornTeam::EROLE_STATUS_ONLINE;
		tMember.mTeamPos = tMaxPos++;
		tpSwornTeam->InsertMember( tMember );
		tNewMember[i]->SetSwornID( tpSwornTeam->GetSwornID() );				
	}	
	// 结拜信息
	SendNotifyRebuildSworn( tTeamList, tTeamCount, tNewMember, tNewNum, tpSwornTeam );
	// 称号信息
	for ( int i = 0; i < tNewNum; ++i )
	{		
		CPropertyModule::GetSingleton().OnPlayerAddTitleEvent( tNewMember[i], 
		mFriendBlessConfig.GetIntimacyConfig()->mSwornTitleID );
	}
	UpdateSwornDBInfo( tpSwornTeam->GetSwornID(), *tpSwornTeam );
}

// ***********************************************************
//  Function:		SendResponseRebuildSworn	
//  Description:	发送创建结拜的消息回应
//  Date:			2011.08.30
// **********************************************************
void CFriendModule::SendResponseRebuildSworn( CEntityPlayer *pPlayer, int nErrcode )
{
	LK_ASSERT( pPlayer != NULL, return )
	CMessage tMsgHead;
	CMessageRebuildSwornResponse tMsgRebuildSworn;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_RESPONSE_REBUILDSWORN );
	tMsgHead.set_msgpara( (int)&tMsgRebuildSworn );
	tMsgRebuildSworn.set_errcode( nErrcode );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tMsgHead );
}

// ***********************************************************
//  Function:		SendNotifyRebuildSworn	
//  Description:	发送重新结拜的消息通知
//  Date:			2011.08.30
// **********************************************************
void CFriendModule::SendNotifyRebuildSworn( CEntityPlayer **pPlayer, int nTeamCount, CEntityPlayer **pNewMember, int nNewMemberNum, CSwornTeam *pSwornTeam )
{
	LK_ASSERT( pSwornTeam != NULL && pPlayer != NULL && pNewMember != NULL, return )
	CMessage tMsgHead;
	CMessageRebuildSwornNotify tMsgRebuildSworn;
	tMsgHead.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_REBUILDSWORN );
	tMsgHead.set_msgpara( (int)&tMsgRebuildSworn );

	for ( int i = 0; i < nTeamCount; ++i )
	{
		if ( pPlayer[i] == NULL )
		{
			continue;
		}

		CMsgSwornMember *pbMember = tMsgRebuildSworn.add_swornmember();
		pbMember->set_roleid( pPlayer[i]->GetCharID() );
		pbMember->set_rolename( pPlayer[i]->GetCharName() );		
		pbMember->set_rolepos( pSwornTeam->GetTeamPos( pPlayer[i]->GetCharID() ) );
		pbMember->set_faceid(  pPlayer[i]->GetFaceType() );
		pbMember->set_genderid( pPlayer[i]->GetSex() );
		pbMember->set_metierid( pPlayer[i]->GetMetier() );
		pbMember->set_rolelevel( pPlayer[i]->GetLevel() );
	}	
	tMsgRebuildSworn.set_swornname( pSwornTeam->GetSwornName() );	
	for ( int i = 0; i < nNewMemberNum; ++ i )
	{
		if ( pNewMember[i] != NULL  )
		{
			tMsgRebuildSworn.add_newmember( pNewMember[i]->GetCharID() );
		}		
	}
	for ( int i = 0; i < nTeamCount; ++i )
	{
		if ( pPlayer[i] == NULL )
		{
			continue;
		}
		CSceneLogic::GetSingleton().Send2Player( pPlayer[i], &tMsgHead );
	}
}


// ***********************************************************
//  Function:		InsertSwornTeam	
//  Description:	插入一个结拜团体信息
//  Date:			2011.06.28
// **********************************************************
int CFriendModule::InsertSwornTeam( int nSwornID, CSwornTeam &rSwornTeam )
{
	mSwornTeamList.insert( SwornTeamList::value_type( nSwornID, rSwornTeam ) );
	return SUCCESS;
}

// ***********************************************************
//  Function:		GetSwornTeam	
//  Description:	获取一个结拜团体信息
//  Date:			2011.06.28
// **********************************************************
CSwornTeam *CFriendModule::GetSwornTeam( int nSwornID )
{
	SwornTeamList::iterator it = mSwornTeamList.find( nSwornID );
	if ( it != mSwornTeamList.end() )
	{
		return &(it->second);
	}	
	return NULL;
}

// ***********************************************************
//  Function:		DeleteSwornTeam	
//  Description:	删除一个结拜团体信息
//  Date:			2011.06.29
// **********************************************************
int CFriendModule::DeleteSwornTeam( int nSwornID )
{
	SwornTeamList::iterator it = mSwornTeamList.find( nSwornID );
	if ( it != mSwornTeamList.end() )
	{
		mSwornTeamList.erase( it );
	}
	return 0;
}

// ***********************************************************
//  Function:		CreateSwornDBInfo	
//  Description:	创建结拜的DB信息
//  Date:			2011.06.28
// **********************************************************
int CFriendModule::CreateSwornDBInfo( CSwornTeam &rSwornTeam )
{
	PBSwornTeam tSwornTeam;
	rSwornTeam.CreateTlvPB( tSwornTeam );	
	char tBuff[MAX_CODE_LEN] ={ 0 };
	char tEscapeBuff[MAX_CODE_LEN] = { 0 };		
	int tRet = tSwornTeam.SerializeToArray( tBuff, MAX_CODE_LEN );
	LK_ASSERT( tRet == true, return  -1 )
	int ByteSize = tSwornTeam.ByteSize();
	char  *pAddress = tEscapeBuff;
	int CapRet = EscapeDataString( &pAddress, MAX_CODE_LEN, tBuff, ByteSize );
	LK_ASSERT( CapRet != -1, return -1 )		

	const char *tpSql = "insert UMS_ROLE_SWORN( sworn_id, sworn_name, camp_id, sworn_member ) values( %d,'%s',%d,'%s' )";
	CSceneLogic::GetSingleton().ExecuteSql( em_dbsession_createswornteam, rSwornTeam.GetSwornID(), rSwornTeam.GetSwornID(), rSwornTeam.GetSwornID(), SQL_NONE,
		INSERT, tpSql, rSwornTeam.GetSwornID(), rSwornTeam.GetSwornName(), rSwornTeam.GetCampID(), tEscapeBuff );
}

// ***********************************************************
//  Function:		DeleteSwornDBInfo	
//  Description:	删除结拜的DB信息
//  Date:			2011.06.28
// **********************************************************
int CFriendModule::DeleteSwornDBInfo( int nSwornID )
{
	const char *tpSql = "delete from UMS_ROLE_SWORN where sworn_id=%d";
	CSceneLogic::GetSingleton().ExecuteSql( em_dbsession_deleteswornteam, 0, 0, 0, SQL_NONE, DELETE, tpSql, nSwornID );
}

// ***********************************************************
//  Function:		GetSwornDBInfo	
//  Description:	获取结拜的DB信息
//  Date:			2011.06.29
// **********************************************************
int CFriendModule::GetSwornDBInfo( CEntityPlayer *pPlayer )
{
	LK_ASSERT( pPlayer != NULL, return -1 )
	const char *tpSql = "select sworn_id, sworn_name, camp_id, sworn_member from UMS_ROLE_SWORN where sworn_id=%d";
	CSceneLogic::GetSingleton().ExecuteSql( em_dbsession_getswornteam, pPlayer->GetCharID(), pPlayer->GetCharID(), pPlayer->GetCharID(), SQL_NEED_CALLBACK,
		SELECT, tpSql, pPlayer->GetSwornID() );
	return SUCCESS;
}	

// ***********************************************************
//  Function:		UpdateSwornDBInfo	
//  Description:	更新结拜信息
//  Date:			2011.06.29
// **********************************************************
int CFriendModule::UpdateSwornDBInfo( int nSwornID, CSwornTeam &rSwornTeam )
{
	PBSwornTeam tSwornTeam;
	rSwornTeam.CreateTlvPB( tSwornTeam );	
	char tBuff[MAX_CODE_LEN] ={ 0 };
	char tEscapeBuff[MAX_CODE_LEN] = { 0 };		
	int tRet = tSwornTeam.SerializeToArray( tBuff, MAX_CODE_LEN );
	LK_ASSERT( tRet == true, return  -1 )
	int ByteSize = tSwornTeam.ByteSize();
	char  *pAddress = tEscapeBuff;
	int CapRet = EscapeDataString( &pAddress, MAX_CODE_LEN, tBuff, ByteSize );
	LK_ASSERT( CapRet != -1, return -1 )	
	const char *tpSql = "update UMS_ROLE_SWORN set sworn_id=%d, sworn_name='%s', camp_id=%d, sworn_member='%s' \
						where sworn_id=%d";
	CSceneLogic::GetSingleton().ExecuteSql( em_dbsession_updateswornteam, rSwornTeam.GetSwornID(), rSwornTeam.GetSwornID(), rSwornTeam.GetSwornID(), SQL_NONE,
		INSERT, tpSql, rSwornTeam.GetSwornID(), rSwornTeam.GetSwornName(), rSwornTeam.GetCampID(), tEscapeBuff, nSwornID );
}

// ***********************************************************
//  Function:		UpDateRoleSwornID	
//  Description:	更新玩家结拜ID
//  Date:			2011.06.29
// **********************************************************
int CFriendModule::UpDateRoleSwornID( int nRoleID, int nSwornID )
{
	const char *tpSql = "update UMS_ROLE set role_swornid=%d where role_id=%d";
	CSceneLogic::GetSingleton().ExecuteSql( em_dbsession_updateroleswornid, 0, 0, nRoleID, SQL_NONE,
		UPDATE, tpSql, nSwornID, nRoleID );
}

// ***********************************************************
//  Function:		OnSessionGetSwornDBInfo	
//  Description:	获取db信息
//  Date:			2011.06.29
// **********************************************************
int CFriendModule::OnSessionGetSwornDBInfo( CMessage *pMsg, int nParam1, int nParam2 )
{
	LK_ASSERT( pMsg != NULL, return -1 )	
	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( nParam1 );
	if ( tpPlayer == NULL )
	{
		return -1;
	}
	CMessageExecuteSqlResponse3 *tpResponseMsg = ( CMessageExecuteSqlResponse3 * )pMsg->msgpara();
	if ( tpResponseMsg == NULL )
	{
		printf( "load SwornTeam info from database error, roleid:%d", nParam1 );
		LOG_ERROR( "friend","load SwornTeam info from database error, roleid:%d", nParam1 );
		return 0;
	}
	if ( tpResponseMsg->resultcode() != 1 )
	{		
		printf( "load SwornTeam sql info from database error,executive error， roleid:%d", nParam1 );
		LOG_ERROR( "friend","load SwornTeam sql info from database error, executive error，roleid:%d", nParam1 );
		return 0;		
	}

	int tRowNum = tpResponseMsg->rowcount();
	int	tColumn	= tpResponseMsg->colcount();	

	if ( tColumn != 4 )
	{
		printf( "load SwornTeam sql info from database error,wrong Column roleid:%d", nParam1 );
		LOG_ERROR( "friend","load SwornTeam sql info from database error,wrong Column roleid:%d", nParam1 );
		return 0;		
	}

	if ( tRowNum <= 0 )
	{
		tpPlayer->SetSwornID( 0 );
		CPropertyModule::GetSingleton().OnPlayerDelTitleEvent( tpPlayer, mFriendBlessConfig.GetIntimacyConfig()->mSwornTitleID );
		return 0;		
	}	

	CSwornTeam tSwornTeam;
	tSwornTeam.SetSwornID( atoi( tpResponseMsg->mutable_fieldvalue( 0 )->c_str() ) );
	tSwornTeam.SetSwornName( tpResponseMsg->mutable_fieldvalue( 1 )->c_str() );
	tSwornTeam.SetCampID( atoi( tpResponseMsg->mutable_fieldvalue( 2 )->c_str() ) );
	PBSwornTeam pbSwornTeam;	
	if ( pbSwornTeam.ParseFromArray( tpResponseMsg->mutable_fieldvalue( 3 )->c_str(), tpResponseMsg->mutable_fieldvalue( 3 )->length() ) == true )
	{
		tSwornTeam.SetFromPB( pbSwornTeam );
	}
	else
	{
		
	}	
	if ( tSwornTeam.HasMember( tpPlayer->GetCharID() ) == false )
	{		
		tpPlayer->SetSwornID( 0 );
		CPropertyModule::GetSingleton().OnPlayerDelTitleEvent( tpPlayer, mFriendBlessConfig.GetIntimacyConfig()->mSwornTitleID );
		return 0;
	}

	CSwornTeam *tpSwornTeam = GetSwornTeam( tSwornTeam.GetSwornID( ) );
	if ( tpSwornTeam != NULL )
	{
		tpSwornTeam->ChangeRoleStatus( tpPlayer->GetCharID(), CSwornTeam::EROLE_STATUS_ONLINE );											
	}		
	else
	{	tSwornTeam.ChangeRoleStatus( tpPlayer->GetCharID(), CSwornTeam::EROLE_STATUS_ONLINE );											
		InsertSwornTeam( tSwornTeam.GetSwornID(), tSwornTeam );
	}
	SendNotifySwornInfo( tpPlayer, &tSwornTeam );
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )tpPlayer->GetProperty();
	if ( tpProperty->GetCurrentTitle() == GetFriendBlessConfig()->GetIntimacyConfig()->mSwornTitleID )
	{
		CMessage tMessage;
		CMessageUpdateCurrentTitleNotice tPara;
		tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTICE_UPDATECURRENTTITLE );
		tMessage.set_msgpara( (int) &tPara );
		tPara.set_charid( tpPlayer->GetCharID() );
		tPara.set_titleid( tpProperty->GetCurrentTitle() );
		tPara.set_titlename( tSwornTeam.GetSwornName() );
		CMapModule::GetSingleton().MessageBroadCast( tpPlayer, &tMessage, false, true );
	}			
	return 0;		
}

// ***********************************************************
//  Function:		OnTimeOutGetSwornDBInfo	
//  Description:	获取结拜信息超时
//  Date:			2011.06.29
// **********************************************************
int CFriendModule::OnTimeOutGetSwornDBInfo( int nParam1, int nParam2 )
{	
	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( nParam1 );
	if ( tpPlayer != NULL )
	{
		LOG_ERROR( "friend","get SwornDBInfo Error, RoleID:%d, SwornID:%d", nParam1, tpPlayer->GetCharID() );
	}
	return 0;
}

// ***********************************************************
//  Function:		OnRouterMsgSwornInfo	
//  Description:	获取结拜信息超时
//  Date:			2011.06.29
// **********************************************************
void CFriendModule::OnRouterMsgSwornInfo( CMessage *pMsg )
{
	LK_ASSERT( pMsg != NULL, return )
	CMessageSwornInfoNotify *tpMsg = ( CMessageSwornInfoNotify * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->roleid() );
	if ( tpPlayer != NULL )
	{
		CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );
	}	
}

// ***********************************************************
//  Function:		OnRouetMsgLeaveSwornTeamNotify	
//  Description:	网关返回的离开结拜的消息
//  Date:			2011.06.29
// **********************************************************
void CFriendModule::OnRouterMsgLeaveSwornTeamNotify( CMessage *pMsg )
{
	LK_ASSERT( pMsg != NULL, return )
	CMessageLeaveSwornTeamNotify *tpMsg = ( CMessageLeaveSwornTeamNotify * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->mutable_router()->descharid() );
	if ( tpPlayer == NULL )
	{		
		return;
	}		
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );
	CSwornTeam *tpSwornTeam = GetSwornTeam( tpPlayer->GetSwornID() );
	if ( tpSwornTeam != NULL )
	{
		tpSwornTeam->DeleteMember( tpMsg->roleid() );	
		if ( tpSwornTeam->GetSwornID() != tpMsg->masterid() && tpMsg->masterid() > 0 )
		{
			CSwornTeam tSwornTeam = *tpSwornTeam;
			tSwornTeam.SetSwornID( tpMsg->masterid() );
			DeleteSwornTeam( tpSwornTeam->GetSwornID() );
			InsertSwornTeam( tSwornTeam.GetSwornID(), tSwornTeam );		
			tpPlayer->SetSwornID( tpMsg->masterid() );
		}
	}		
	else
	{	
		tpPlayer->SetSwornID( tpMsg->masterid() );	
		tpSwornTeam = GetSwornTeam( tpPlayer->GetSwornID() );
	}	

	// 重新发送结拜信息
	if ( tpSwornTeam != NULL )
	{
		SendNotifySwornInfo( tpPlayer, tpSwornTeam );
	}	
}

// ***********************************************************
//  Function:		OnRouterMsgModifyIntimacy	
//  Description:	修正好友亲密度的消息
//  Date:			2011.06.29
// **********************************************************
void CFriendModule::OnRouterMsgModifyIntimacy( CMessage *pMsg )
{
	LK_ASSERT( pMsg != NULL, return )
	CMessageModifyIntimacyNogify *tpMsg = ( CMessageModifyIntimacyNogify * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->mutable_router()->descharid() );
	if ( tpPlayer == NULL )
	{
		return;
	}
	CFriendItem *tpFriendItem = GetPlayerFriendItem( tpPlayer->GetCharID() );
	if ( tpFriendItem == NULL )
	{
		return;
	}	
	st_friend *tpFriend = tpFriendItem->GetFriendByRoleID( tpMsg->friendid() );
	if ( tpFriend == NULL )
	{
		SendModifyIntimacyNotify( tpMsg->friendid(), 0, 0, ENTIMACY_CHANGE_TYPE_UPDATE, tpPlayer->GetCharID() );
		return;
	}
	switch ( tpMsg->type() )
	{
		case  ENTIMACY_CHANGE_TYPE_MODIFY :
			{
				tpFriend->nIntimacyDegree -= tpMsg->changevalue();
				if ( tpFriend->nIntimacyDegree < 0 )
				{
					tpFriend->nIntimacyDegree = 0;
				}
				
				if ( tpFriend->nIntimacyDegree >= 0 && tpFriend->nIntimacyDegree < tpMsg->intimacydegree() )
				{	
					SendModifyIntimacyNotify( tpMsg->friendid(), 0, tpFriend->nIntimacyDegree, ENTIMACY_CHANGE_TYPE_UPDATE, tpPlayer->GetCharID(), tpMsg->updatetime(), tpMsg->dailyintimacy() );
				}		
				else
				{
					tpFriend->nIntimacyDegree = tpMsg->intimacydegree();
				}	
				break;
			}
		case ENTIMACY_CHANGE_TYPE_UPDATE :
			{
				tpFriend->nIntimacyDegree = tpMsg->intimacydegree();		
				break;
			}
		case ENTIMACY_CHANGE_TYPE_FLOWER:
		case ENTIMACY_CHANGE_TYPE_TEAM:
			{
				tpFriend->nIntimacyDegree += tpMsg->changevalue();		
				break;
			}
		case ENTIMACY_CHANGE_TYPE_DISBANDSWORN:
			{
				tpFriend->nIntimacyDegree -= tpFriend->nIntimacyDegree*tpMsg->changevalue()/100;
				break;
			}
		case ENTIMACY_CHANGE_TYPE_DIVORCE:
		case ENTIMACY_CHANGE_TYPE_KILL:		
		case ENTIMACY_CHANGE_TYPE_LEAVESWORN:
			{
				tpFriend->nIntimacyDegree -= tpMsg->changevalue();	
				break;
			}
	}
	tpFriend->nLastUpdateTime = tpMsg->updatetime();
	tpFriend->nDailyIntimacy = tpMsg->dailyintimacy();
	if ( tpFriend->nIntimacyDegree < 0 )
	{
		tpFriend->nIntimacyDegree = 0;
	}
	// 判断是否超上限
	if ( tpFriend->nIntimacyDegree > mFriendBlessConfig.GetIntimacyConfig()->mMaxIntimacy )
	{
		tpFriend->nIntimacyDegree = mFriendBlessConfig.GetIntimacyConfig()->mMaxIntimacy;
	}	
	tpMsg->set_intimacydegree( tpFriend->nIntimacyDegree );	
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );
}

// ***********************************************************
//  Function:		OnRouterMsgKickSwornMember	
//  Description:	踢人通知
//  Date:			2011.06.29
// **********************************************************
void CFriendModule::OnRouterMsgKickSwornMember( CMessage *pMsg )
{
	LK_ASSERT( pMsg != NULL, return ) 
	CMessageKickSwornMemberNotify *tpMsg = ( CMessageKickSwornMemberNotify * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->mutable_router()->descharid() );
	if ( tpPlayer == NULL )
	{
		return;
	}	

	if ( tpPlayer->GetCharID() == tpMsg->roleid() )
	{
		CPropertyModule::GetSingleton().OnPlayerDelTitleEvent( tpPlayer, mFriendBlessConfig.GetIntimacyConfig()->mSwornTitleID );
	}
	
	CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );
	CSwornTeam *tpSwornTeam = GetSwornTeam( tpPlayer->GetSwornID() );
	if ( tpSwornTeam != NULL )
	{
		tpSwornTeam->DeleteMember( tpMsg->roleid() );		
	}
	if ( tpSwornTeam != NULL )
	{
		SendNotifySwornInfo( tpPlayer, tpSwornTeam );
	}	
}

// ***********************************************************
//  Function:		OnRouterMsgDisbandSwornTeam	
//  Description:	解散结拜的消息通知
//  Date:			2011.06.29
// **********************************************************
void CFriendModule::OnRouterMsgDisbandSwornTeam( CMessage *pMsg )
{
	LK_ASSERT( pMsg != NULL, return )
	CMessageDisbandSwornTeamNotify *tpMsg = ( CMessageDisbandSwornTeamNotify * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->mutable_router()->descharid() );
	if ( tpPlayer != NULL )
	{
		CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );
		CPropertyModule::GetSingleton().OnPlayerDelTitleEvent( tpPlayer, mFriendBlessConfig.GetIntimacyConfig()->mSwornTitleID );
	}	
	DeleteSwornTeam( tpMsg->swornid() );
}

// ***********************************************************
//  Function:		OnRouterMsgChangeSwornName	
//  Description:	结拜改名的通知消息
//  Date:			2011.06.29
// **********************************************************
void CFriendModule::OnRouterMsgChangeSwornName( CMessage *pMsg )
{
	LK_ASSERT( pMsg != NULL, return )
	CMessageChangeSwornNameNotify *tpMsg = ( CMessageChangeSwornNameNotify * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->mutable_router()->descharid() );
	if ( tpPlayer != NULL )
	{
		CSceneLogic::GetSingleton().Send2Player( tpPlayer, pMsg );
		CSwornTeam *tpSwornTem = GetSwornTeam( tpPlayer->GetSwornID() );
		if ( tpSwornTem != NULL )
		{
			tpSwornTem->SetSwornName( tpMsg->mutable_swornname()->c_str( ) );
		}		
		CPropertyPlayer *tpProperty = ( CPropertyPlayer * )tpPlayer->GetProperty();
		if ( tpProperty->GetCurrentTitle() == GetFriendBlessConfig()->GetIntimacyConfig()->mSwornTitleID )
		{
			CMessage tMessage;
			CMessageUpdateCurrentTitleNotice tPara;
			tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTICE_UPDATECURRENTTITLE );
			tMessage.set_msgpara( (long) &tPara );
			tPara.set_charid( tpPlayer->GetCharID() );
			tPara.set_titleid( tpProperty->GetCurrentTitle() );
			tPara.set_titlename( tpSwornTem->GetSwornName() );
			CMapModule::GetSingleton().MessageBroadCast( tpPlayer, &tMessage, false, true );
		}
	}	
}

// ***********************************************************
//  Function:		OnRouterMsgModifyLastTimeAllOnLine	
//  Description:	设置最后的同时登录时间
//  Date:			2011.06.30
// **********************************************************
void CFriendModule::OnRouterMsgModifyLastTimeAllOnLine( CMessage *pMsg )
{
	LK_ASSERT( pMsg != NULL, return )
	CMessageModifyLastTimeAllOnLineNotify *tpMsg = ( CMessageModifyLastTimeAllOnLineNotify * )pMsg->msgpara();
	LK_ASSERT( tpMsg != NULL, return )
	CEntityPlayer *tpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( tpMsg->mutable_router()->descharid() );
	if ( tpPlayer == NULL )
	{
		return;
	}
	CFriendItem *tpFriendItem = GetPlayerFriendItem( tpPlayer->GetCharID() );
	if ( tpFriendItem == NULL )
	{
		return;
	}
	st_friend *tpFriend = tpFriendItem->GetFriendByRoleID( tpMsg->friendid() );
	if ( tpFriend != NULL )
	{
		tpFriend->nLastTimeAllOnline = tpMsg->time();
	}	
}

// ***********************************************************
//  Function:		CheckItemNumAffectedByIntimacy	
//  Description:	检查因为亲密度结婚可以减少的石头数量
//  Date:			2011.07.18
// **********************************************************
int CFriendModule::CheckItemNumAffectedByIntimacy( int nSrcCharID, int nDesCharID, int &rNum )
{
	int tCharID[2] = { nSrcCharID, nDesCharID };
	int tRoleNum = ARRAY_CNT( tCharID );
	rNum = 0;
	int tIntimacyDegree = 0;
	int tRet = GetIntimacyDegree( nSrcCharID, nDesCharID, tIntimacyDegree );
	if ( tRet < 0 )
	{
		return 0;
	}	
	rNum = mFriendBlessConfig.GetIntimacyConfig()->GetReducedNumOfMarriageItem( tIntimacyDegree );	
	return 0;
}

// ***********************************************************
//  Function:		CheckTeamSwornIntimacy	
//  Description:	检查队伍中结拜成员的结拜亲密度
//  Date:			2011.07.18
// **********************************************************
int CFriendModule::CheckTeamSwornIntimacy( CEntityPlayer *pPlayer, int nValue )
{
	LK_ASSERT( pPlayer != NULL, return ERROR_NOTINSWORNTEAM )
	if ( pPlayer->GetSwornID() <= 0 )
	{
		return ERROR_NOTINSWORNTEAM;
	}	
	int tSwornID = pPlayer->GetSwornID();

	// 是否有队伍,没有队伍不能结婚
	int tTeamID  = CTeamModule::GetSingleton().InTeam( pPlayer->GetCharID() );
	if ( tTeamID == INVALID_OBJ_ID )
	{		
		return ERROR_SWORNCOMPOSE_NUMBERNOTENOUGH;				
	}		

	// 判断队伍人数
	CEntityPlayer *tTeamList[TEAMNUM] = { NULL };	
	int tTeamCount = 0;
	int tRtn = CTeamModule::GetSingletonPtr()->OnGetMemberList( tTeamID,tTeamList, ARRAY_CNT( tTeamList ), tTeamCount );
	if ( tRtn < 0 || tTeamCount <= 1 )
	{		
		return ERROR_SWORNCOMPOSE_NUMBERNOTENOUGH;
	}  	 
	
	// 检查亲密度
	int tTeamMemberRoleID[TEAMNUM] = { 0 };
	int tCount = 0;
	for ( int i = 0; i < tTeamCount; ++i )
	{
		if ( tTeamList[i] == NULL )
		{
			return ERROR_SWORNCOMPOSE_NUMBERNOTENOUGH;;
		}
		if ( tTeamList[i]->GetSwornID() != tSwornID )
		{
			return ERROR_SWORNCOMPOSE_NOTINSAMESWORNTEAM;
		}		
		tTeamMemberRoleID[tCount] = tTeamList[i]->GetCharID();
		++tCount;
	}
	bool tRet = JudgeIntimacyDegree( tTeamMemberRoleID, tCount, nValue );
	if ( tRet == false )
	{
		return ERROR_SWORNCOMPOSE_INTIMACYNOTENOUGH;
	}
	return SUCCESS;
}

void CFriendModule::GetContacterListInfo(int nRequesterID, CFriendItem &rItem)
{
	CMessagePlayerOnLineFriendRequest msg;
	msg.set_requester( nRequesterID );
	msg.set_flag(FLAG_GETCONTACTER);

	CMessage Message;
	int nSize = rItem.GetContacterCount();
	if ( nSize == 0 )
	{
		return;
	}

	for (int i = 0; i < nSize; i++)
	{
		int nCharID = rItem.GetContacter(i);
		msg.add_charids(nCharID);
	}

	Message.set_msgpara((uint32_t) &msg);
	Message.mutable_msghead()->set_messageid( ID_S2G_PLAYER_ONLINE_FRIEND_REQUEST );
	CSceneLogic::GetSingleton( ).Send2Gate(&Message);
}

void CFriendModule::OnRouterMsgLastContacter(CMessage* pMessage)
{
	if (pMessage == NULL)
	{
		return;
	}

	CMessageLastContacterNotice* pMsgPara = (CMessageLastContacterNotice*)pMessage->msgpara();
	if (pMsgPara == NULL)
	{
		return;
	}
	
	int nCharID = pMsgPara->router().descharid();
	CSceneLogic::GetSingleton( ).Send2Player(nCharID, pMessage );
}