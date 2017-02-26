#include "message_factory.h"

#include "coremessage_pb.hxx.pb.h"

#ifdef LINUX
#include "log.h"
#endif

#ifdef LINUX
	#include "servermessage_pb.hxx.pb.h"
	#include "gmmessage_pb.hxx.pb.h"
	#include "db_msg_pb.hxx.pb.h"
	#include "log_msg_pb.hxx.pb.h"
	#include "ibstore_pb.hxx.pb.h"
	#include "globalserver_pb.hxx.pb.h"
    #include "taskmessage_pb.hxx.pb.h"
#endif

#ifndef OTHERSERVER
	#include "chatmessage_pb.hxx.pb.h"
	#include "mapmessage_pb.hxx.pb.h"
	#include "message_pb.hxx.pb.h"
	#include "promessage_pb.hxx.pb.h"
	#include "taskmessage_pb.hxx.pb.h"
	#include "teammessage_pb.hxx.pb.h"
  	#include "mailmessage_pb.hxx.pb.h"
	#include "trademessage_pb.hxx.pb.h"
	#include "trademessage_pb.hxx.pb.h"
	#include "friendmessage_pb.hxx.pb.h"
	#include "stallmessage_pb.hxx.pb.h"
	#include "repetionmessage_pb.hxx.pb.h"
	#include "activitymessag_pb.hxx.pb.h"
	#include "familymessage_pb.hxx.pb.h"
	#include "ibstore_pb.hxx.pb.h"
	#include "corpsmessage_pb.hxx.pb.h"
	#include "judgemessage_pb.hxx.pb.h"
	#include "achievemessage_pb.hxx.pb.h"
	#include "countrymessage_pb.hxx.pb.h"
	#include "privilege_pb.hxx.pb.h"
	#include "boursemessage_pb.hxx.pb.h"
	#include "marriagemessage_pb.hxx.pb.h"
#endif
#include "rankmessage_pb.hxx.pb.h"
// 这里之所以用宏，是因为对消息工厂将要做的事情还不太确定，以后改起来容易一些,只改这一处。
#ifndef _USE_MESSAGE_BUFF_ 
	// 多线程下使用
	#define CASE_NEW_MSG(msgname,msgid)	\
		case msgid:						\
		{								\
			tpMessage = new msgname;	\
			break;						\
		}							
#else
	// 单线程下，使用placement new , 减少内存碎片和内存分配时间
	#define CASE_NEW_MSG(msgname,msgid)					\
		case msgid:										\
		{												\
			tpMessage = new(msMessageBuff) msgname;		\
			break;										\
		}

#endif // !_MT_THREAD_



// 单件定义
template <> CMessageFactory* CSingleton< CMessageFactory >::spSingleton = NULL;

// 静态缓冲区初始化
char CMessageFactory::msMessageBuff[ MAX_MESSAGE_LEN ] = {0};

// 根据 消息id 创建消息
Message* CMessageFactory::CreateMessage( unsigned int unMessageID  )
{
	Message* tpMessage = CreateClientMessage( unMessageID );

#ifdef LINUX
	if( tpMessage == NULL )
	{
		tpMessage = CreateServerMessage( unMessageID );
	}
#endif

	return tpMessage;
}

// 创建 client - server 消息
Message* CMessageFactory::CreateClientMessage( unsigned int unMessageID  )
{	
	Message* tpMessage = NULL;

	switch( unMessageID )
	{
		// core message
		CASE_NEW_MSG( CMessageLoginErrorNotice,					ID_L2C_NOTIFY_LOGINERROR )
		CASE_NEW_MSG( CMessageGateErrorNotice,					ID_G2C_NOTIFY_GATEERROR )
		CASE_NEW_MSG( CMessageLoginServerRequest,				ID_C2L_REQUEST_LOGINSERVER )
		CASE_NEW_MSG( CMessageWaitForLoginNotice,				ID_L2C_NOTIFY_WAITFORQUEUE )
		CASE_NEW_MSG( CMessageRedirectGateNotice,				ID_L2C_NOTIFY_REDIRECTGATE )
		CASE_NEW_MSG( CMessageLoginGateRequest,					ID_C2G_REQUEST_LOGINGATE )
		CASE_NEW_MSG( CMessageLoginGateResponse,				ID_G2C_RESPONSE_LOGINGATE )
		CASE_NEW_MSG( CMessageObtainCharRequest,				ID_C2G_REQUEST_OBTAINCHAR )
		CASE_NEW_MSG( CMessageObtainCharNotice,					ID_G2C_NOTIFY_OBTAINCHAR )
		CASE_NEW_MSG( CMessageObtainCharCompleteNotice,			ID_G2C_NOTIFY_OBTAINCHARCOMPLTE )
		CASE_NEW_MSG( CMessageEnterGameRequest,					ID_C2G_REQUEST_ENTERGAME )
		CASE_NEW_MSG( CMessageEnterGameResponse,				ID_G2C_RESPONSE_ENTERGAME )
		CASE_NEW_MSG( CMessageLoginSceneRequest,				ID_C2S_REQUEST_LOGINSCENE )
		CASE_NEW_MSG( CMessageLoginSceneResponse,				ID_S2C_RESPONSE_LOINGSCENE )
		CASE_NEW_MSG( CMessageReturn2GateRequest,				ID_C2S_REQUEST_RETURN2GATE )
		CASE_NEW_MSG( CMessageReturn2GateResponse,				ID_S2C_RESPONSE_RETURN2GATE )
		CASE_NEW_MSG( CMessageCreateCharRequest,				ID_C2G_REQUEST_CREATECHAR )
		CASE_NEW_MSG( CMessageCreateCharResponse,				ID_G2C_RESPONSE_CREATECHAR )
		CASE_NEW_MSG( CMessageDeleteCharRequest,				ID_C2G_REQUEST_DELETECHAR )
		CASE_NEW_MSG( CMessageDeleteCharResponse,				ID_G2C_RESPONSE_DELETECHAR )
		CASE_NEW_MSG( CMessageReconnectSceneNotice,				ID_S2C_NOTIFY_RECONNECTSCENE )
		CASE_NEW_MSG( CMessageRedirectSceneRequest,				ID_C2S_REQUEST_REDIRECTSCENE )
		CASE_NEW_MSG( CMessageRedirectSceneResponse,			ID_S2C_RESPONSE_REDIRECTSCENE )
		CASE_NEW_MSG( CMessageLeaveGameRequest,					ID_C2S_REQUEST_LEAVEGAME )
		CASE_NEW_MSG( CMessageLeaveGameResponse,				ID_S2C_RESPONSE_LEAVEGAME )
		CASE_NEW_MSG( CMessageKickoutNotice,					ID_S2C_NOTIFY_KICK )
		CASE_NEW_MSG( CMessageClientPingServerRequest,			ID_C2S_REQUEST_PING)
		CASE_NEW_MSG( CMessageClientPingServerResponse,			ID_S2C_RESPONSE_PING)
		CASE_NEW_MSG( CMessageGateKickPlayerNotify,				ID_G2S_NOTIFY_KICKPLAYER )
		CASE_NEW_MSG( CMessageRedirectErr,						ID_S2C_REDIRECT_ERR_CODE )
		CASE_NEW_MSG( CMessageQueuePlayerNotify,				ID_G2S_NOTIFY_LOGINQUEUE )
		CASE_NEW_MSG( CMessageCheckRolePwdRequest,				ID_C2G_REQUEST_CHECKROLEPWD )	 	
		CASE_NEW_MSG( CMessageCheckRolePwdResponse,				ID_G2C_RESPONSE_CHECKROLEPWD )
		CASE_NEW_MSG( CMessageSendClientInfoRequest,			ID_C2S_REQUEST_CLIENTINFO )
		CASE_NEW_MSG( CMessagePlayerCancelLoginQueue,			ID_S2G_CANCEL_LOGINQUEUE )
		CASE_NEW_MSG( CMessageSendAntiBotDataToServerNotify,	ID_C2S_NOTIFY_CLIENTANTIBOTDATA )
		CASE_NEW_MSG( CMessageSendAntiBotDataToClientNotify,	ID_S2C_NOTIFY_SERVERANTIBOTDATA )
		CASE_NEW_MSG( CMessageClientAntiBotStartRetNotify,		ID_C2S_NOTIFY_CLIENTANTIBOTSTARTRET )		
		CASE_NEW_MSG( CMessageDisconnectNotify,					ID_S2C_NOTIFY_DISCONNECT )
		CASE_NEW_MSG( CMessageCancelDeleteRoleRequest,			ID_C2G_REQUEST_CANCELROLEDELETE )
		CASE_NEW_MSG( CMessageCancelDeleteRoleResponse,			ID_G2C_RESPONSE_CANCELROLEDELETE )
// 100304 MAOCY ADD-BEGIN
		CASE_NEW_MSG( CMessageQuestionRequireRequest,			ID_C2G_REQUEST_QUESTIONREQUIRE )
		CASE_NEW_MSG( CMessageQuestionRequireResponse,			ID_G2C_RESPONSE_QUESTIONREQUIRE )
		CASE_NEW_MSG( CMessageQuestionAnswerRequest,			ID_C2G_REQUEST_QUESTIONANSWER )
		CASE_NEW_MSG( CMessageQuestionAnswerResponse,			ID_G2C_RESPONSE_QUESTIONANSWER )
// 100304 MAOCY ADD-END
// 100329 MAOCY ADD-BEGIN
		// 用户认证
		CASE_NEW_MSG( CMessageCertifyRequest,					ID_C2A_REQUEST_CERTIFY )
		CASE_NEW_MSG( CMessageCertifyResponse,					ID_A2C_RESPONSE_CERTIFY )
		CASE_NEW_MSG( CMessageCertifyQueryRequest,				ID_A2L_REQUEST_CERTIFYQUERY )
		CASE_NEW_MSG( CMessageCertifyQueryResponse,				ID_L2A_RESPONSE_CERTIFYQUERY )
		CASE_NEW_MSG( CMessageCertifyTokenRequest,				ID_L2A_REQUEST_CERTIFYTOKEN )
		CASE_NEW_MSG( CMessageCertifyTokenResponse,				ID_A2L_RESPONSE_CERTIFYTOKEN )
		// 角色数目查询
		CASE_NEW_MSG( CMessageRoleQueryRequest,					ID_L2G_REQUEST_ROLEQUERY )
		CASE_NEW_MSG( CMessageRoleQueryResponse,				ID_G2L_RESPONSE_ROLEQUERY )
		CASE_NEW_MSG( CMessageRoleQueryNotify,					ID_L2A_NOTIFY_ROLEQUERY )
		// 服务器信息通知
		CASE_NEW_MSG( CMessageOnlineStatusNotify,				ID_L2A_NOTIFY_ONLINESTATUS )
// 100329 MAOCY ADD-END


		///////////////////////////////////
		// 消息：转阵营（转服）消息定义	
		// author： qiufufu
		// date : 10-08-23
		//////////////////////////////////////

		CASE_NEW_MSG( CMessageInviteCampRequest, ID_C2S_REQUEST_INVITE_CAMP)		// 客户端申请转阵营
		CASE_NEW_MSG( CMessageInviteCampResponse,ID_S2C_RSPONSE_INVITE_CAMP)			// 服务端回复转阵营 把可转的阵营列表通知给客户端

		CASE_NEW_MSG( CMessageChangeSingleCampRequest,ID_C2S_REQUEST_CHANGE_CAMP)		 // 客户端选择一个阵营，通知服务端转阵营
		CASE_NEW_MSG( CMessageChangeSingleCampResponse,ID_S2C_RSPONSE_CHANGE_CAMP)	// 服务端回复客户端 转阵营是否成功

		CASE_NEW_MSG( CMessageChangeCampSuccesNotify, ID_S2C_NOTIFY_CHANGE_CAMP_SUCCESED) // 世界频道通知玩家转阵营成功

		CASE_NEW_MSG( CMessageChangeCamp2GateNotice, ID_S2G_CHANGE_CAMP_NOTICE)  //scenserver 转换成功后通知 gateserver
		
		CASE_NEW_MSG( CMessageChangeCampConditionRequest,ID_C2S_REQUEST_CHANGE_CAMP_CONDITION) // 客户端发送查询阵营转换条件
		CASE_NEW_MSG( CMessageChangeCampConditionResponse,ID_S2C_RSPONSE_CHANGE_CAMP_CONDITION) // 服务端回复查询阵营转换条件

#ifndef OTHERSERVER
		// pro message
        CASE_NEW_MSG( CMessagePlayerChangeFashionShowRequest,    ID_C2S_REQUEST_PLAYERCHANGEFASHIONSHOW )
        CASE_NEW_MSG( CMessagePlayerChangeFashionShowResponse,   ID_S2C_RESPONSE_PLAYERCHANGEFASHIONSHOW )
        CASE_NEW_MSG( CMessagePlayerChangeFashionNotice,         ID_S2C_NOTIFY_PLAYERCHANGEFASHIONSHOW )
        CASE_NEW_MSG( CMessagePlayerChangeEquipShowRequest,      ID_C2S_REQUEST_PLAYERCHANGEEQUIPSHOW )
        CASE_NEW_MSG( CMessagePlayerChangeEquipShowResponse,     ID_S2C_RESPONSE_PLAYERCHANGEEQUIPTSHOW )
		CASE_NEW_MSG( CMessageYourProDataNotice,				ID_S2C_NOTIFY_YOURPRODATA )
		CASE_NEW_MSG( CMessageCreatePlayerProNotice,			ID_S2C_NOTIFY_CREATEPLAYER )
		CASE_NEW_MSG( CMessageCreateNpcProNotice,				ID_S2C_NOTIFY_CREATENPC )
		CASE_NEW_MSG( CMessageUseSkillRequest,					ID_C2S_REQUEST_USESKILL )
		CASE_NEW_MSG( CMessageNormalAttackRequest,				ID_C2S_REQUEST_NORMALATTACK )
		CASE_NEW_MSG( CMessagePickItemRequest,					ID_C2S_REQUEST_PICKITEM )
		CASE_NEW_MSG( CMessageClickNpcRequest,					ID_C2S_REQUEST_CLICKNPC )
		CASE_NEW_MSG( CMessageClickNpcResponse,					ID_S2C_RESPONSE_CLICKNPC )
		CASE_NEW_MSG( CMessageCloseDialogNotice,				ID_S2C_NOTIFY_CLOSEDIALOG )
		CASE_NEW_MSG( CMessageChangeAppertainings,				ID_S2C_NOTIFY_CHANGEAPPERTAININGS)
		CASE_NEW_MSG( CMessageUpdateMoneyNotice,				ID_S2C_NOTIFY_UPDATEMONEY )
		CASE_NEW_MSG( CMessageUpdateBindMoneyNotice,			ID_S2C_RESPONSE_BINDMONEYNOTICE )
		CASE_NEW_MSG( CMessagePurchaseItemRequest,				ID_C2S_REQUEST_PURCHASEITEM )
		CASE_NEW_MSG( CMessageSellItemRequest,					ID_C2S_REQUEST_SELLITEM )
		CASE_NEW_MSG( CMessageSellItemResponse,					ID_S2C_RESPONSE_SELLITEM )
		CASE_NEW_MSG( CMessageRemoveItemNotify,					ID_S2C_NOTIFY_REMOVEITEM )
		CASE_NEW_MSG( CMessageRemoveTaskItemNotice,				ID_S2C_NOTIFY_REMOVETASKITEM )
		CASE_NEW_MSG( CMessageThrowItemRequest,					ID_C2S_REQUEST_THROWITEM )
		CASE_NEW_MSG( CMessageThrowItemNotice,					ID_S2C_NOTIFY_THROWITEM )
		CASE_NEW_MSG( CMessageThrowMoneyRequest,				ID_C2S_REQUEST_THROWMONEY )
		CASE_NEW_MSG( CMessageSetShortcutRequest,				ID_C2S_REQUEST_SETSHORTCUT )
		CASE_NEW_MSG( CMessageEntityRelifeNotice,				ID_S2C_NOTIFY_ENTITYRELIFE )
		CASE_NEW_MSG( CMessageEntityDisappearNotice,			ID_S2C_NOTIFY_ENTITYDISAPPEAR )
		CASE_NEW_MSG( CMessagePlayerReliveRequest,				ID_C2S_REQUEST_PLAYERRELIVE )
		CASE_NEW_MSG( CMessagePlayerReliveNotice,				ID_S2C_NOTIFY_PLAYERRELIVE )
		CASE_NEW_MSG( CMessageRemoveBuffNotice,					ID_S2C_NOTIFY_REMOVEBUFF )
		CASE_NEW_MSG( CMessageChangeStateNotice,				ID_S2C_NOTIFY_CHANGESTATE )
		CASE_NEW_MSG( CMessageUseSkillNotice,					ID_S2C_NOTIFY_USESKILL )
		CASE_NEW_MSG( CMessageFuncResultNotice,					ID_S2C_NOTIFY_FUNCRESULT )
		CASE_NEW_MSG( CMessageChangeMetierNotice,				ID_S2C_NOTIFY_CHANGEMETIER )
		CASE_NEW_MSG( CMessageLockItemNotice,					ID_S2C_NOTIFY_LOCKITEM )
		CASE_NEW_MSG( CMessageObtainExpNotice,					ID_S2C_NOTIFY_OBTAINEXP )
		CASE_NEW_MSG( CMessageMoveItemRequest,					ID_C2S_REQUEST_MOVEITEM )
		CASE_NEW_MSG( CMessageMoveItemNotice,					ID_S2C_NOTIFY_MOVEITEM )
		CASE_NEW_MSG( CMessageInsertItemNotify,					ID_S2C_NOTIFY_INSERTITEM )
		CASE_NEW_MSG( CMessageProDebugNotice,					ID_S2C_NOTIFY_PRODEBUG )
		CASE_NEW_MSG( CMessageProErrorNotice,					ID_S2C_NOTIFY_PROERROR )
		CASE_NEW_MSG( CMessageEquipItemRequest,					ID_C2S_REQUEST_EQUIPITEM )
		CASE_NEW_MSG( CMessageUnEquipItemRequest,				ID_C2S_REQUEST_UNEQUIPITEM )
		CASE_NEW_MSG( CMessageEquipItemNotice,					ID_S2C_NOTIFY_EQUIPITEM )
		CASE_NEW_MSG( CMessageUnEquipItemNotice,				ID_S2C_NOTIFY_UNEQUIPITEM )
		CASE_NEW_MSG( CMessagePlayerChangeEquipNotice,			ID_S2C_NOTIFY_PLAYERCHANGEEQUIQ )
		CASE_NEW_MSG( CMessageEntityRefreshNotice,				ID_S2C_NOTIFY_ENTITYREFRESH )
		CASE_NEW_MSG( CMessageClickSkillPointRequest,			ID_C2S_REQUEST_CLICKSKILLPOINT )
		CASE_NEW_MSG( CMessageClickSkillPointResponse,			ID_S2C_RESPONSE_CLICKSKILLPOINT )
		CASE_NEW_MSG( CMessageResetAPPRequest,					ID_C2S_REQUEST_RESETAPP )
		CASE_NEW_MSG( CMessageResetAPPResponse,					ID_S2C_RESPONSE_RESETAPP )
		CASE_NEW_MSG( CMessageResetSSPRequest,					ID_C2S_REQUEST_RESETSSP )
		CASE_NEW_MSG( CMessageResetASPRequest,					ID_C2S_REQUEST_RESETASP )
		CASE_NEW_MSG( CMessageResetSSPResponse,					ID_S2C_RESPONSE_RESETSSP )
		CASE_NEW_MSG( CMessageResetASPResponse,					ID_S2C_RESPONSE_RESETASP )
		CASE_NEW_MSG( CMessageEntityDieNotice,					ID_S2C_NOTIFY_ENTITYDIE )
		CASE_NEW_MSG( GetRolePersonalInfoByIDMsg,				ID_C2S_REQUEST_GETROLEPERSONALINFOBYID )
		CASE_NEW_MSG( GetRolePersonalInfoByNameMsg,				ID_C2S_REQUEST_GETROLEPERSONALINFOBYNAME )
		CASE_NEW_MSG( SendRolePersonalInfoMsg,					ID_S2C_RESPONSE_SENDROLEPERSONALINFO )
		CASE_NEW_MSG( SetRolePersonalInfoMsg,					ID_C2S_REQUEST_SETROLEPERSONALINFO )
		CASE_NEW_MSG( RolePersonalInfoBackMsg,					ID_S2C_RESPONSE_ROLEPERSONALINFOBACK )
		CASE_NEW_MSG( CMessageUseItemRequest,					ID_C2S_REQUEST_USEITEM )
		CASE_NEW_MSG( CMessageUseItemResponse,					ID_S2C_RESPONSE_USEITEM )
		CASE_NEW_MSG( CMessagePlayerUseItemNotice,				ID_S2C_NOTIFY_PLAYERUSEITEM )
		CASE_NEW_MSG( CMessageTelePortResponse,					ID_S2C_RESPONSE_TELEPORT )
		CASE_NEW_MSG( CMessageProduceEquipRequest,				ID_C2S_REQUEST_PRODUCEEQUIP )
		CASE_NEW_MSG( CMessageUpgAddItemRequest,				ID_C2S_REQUEST_UPGADDITEM )
		CASE_NEW_MSG( CMessageUpgAddItemResponse,				ID_S2C_RESPONSE_UPGADDITEM )
		CASE_NEW_MSG( CMessageUpgDelItemRequest,				ID_C2S_REQUEST_UPGDELITEM )
		CASE_NEW_MSG( CMessageUpgDelItemResponse,				ID_S2C_RESPONSE_UPGDELITEM )
		CASE_NEW_MSG( CMessageUpgEquipRequest,					ID_C2S_REQUEST_UPGEQUIP )
		CASE_NEW_MSG( CMessageUpgEquipNotice,					ID_S2C_NOTIFY_UPGEQUIP )
		CASE_NEW_MSG( CMessageProducePhysRequest,				ID_C2S_REQUEST_PRODUCEPHY )
		CASE_NEW_MSG( CMessageProFinNotice,						ID_S2C_NOTIFY_PROFIN )
		CASE_NEW_MSG( CMessageProBreakNotice,					ID_S2C_NOTIFY_PROBREAK )
		CASE_NEW_MSG( CMessagePushRuneRequest,					ID_C2S_REQUEST_PUSHRUNE )
		CASE_NEW_MSG( CMessagePushRuneResponse,					ID_S2C_RESPONSE_PUSHRUNE )
		CASE_NEW_MSG( CMessagePopRuneRequest,					ID_C2S_REQUEST_POPRUNE )
		CASE_NEW_MSG( CMessagePopRuneResponse,					ID_S2C_RESPONSE_POPRUNE )
		CASE_NEW_MSG( CMessageInsertRuneRequest,				ID_C2S_REQUEST_INSERTRUNE )
		CASE_NEW_MSG( CMessageInsertRuneNotice,					ID_S2C_RESPONSE_INSERTRUNE )
		CASE_NEW_MSG( CMessageInsertPushJewelRequest,			ID_C2S_REQUEST_PUSHJEWEL )
		CASE_NEW_MSG( CMessageInsertPushJewelResponse,			ID_S2C_RESPONSE_PUSHJEWEL )
		CASE_NEW_MSG( CMessageInsertPopJewelRequest,			ID_C2S_REQUEST_POPJEWEL )
		CASE_NEW_MSG( CMessageInsertPopJewelResponse,			ID_S2C_RESPONSE_POPJEWEL )
		CASE_NEW_MSG( CMessageInsertJewelRequest,				ID_C2S_REQUEST_INSERTJEWEL )
		CASE_NEW_MSG( CMessageInsertJewelNotice,				ID_S2C_RESPONSE_INSERTJEWEL )
		CASE_NEW_MSG( CMessageEatPushEquipRequest,				ID_C2S_REQUEST_PUSHEAT )
		CASE_NEW_MSG( CMessageEatPushEquipResponse,				ID_S2C_RESPONSE_PUSHEAT )
		CASE_NEW_MSG( CMessageEatPopEquipRequest,				ID_C2S_REQUEST_POPEAT )
		CASE_NEW_MSG( CMessageEatPopEquipResponse,				ID_S2C_RESPONSE_POPEAT )
		CASE_NEW_MSG( CMessageEatEquipRequest,					ID_C2S_REQUEST_EATEQUIP )
		CASE_NEW_MSG( CMessageEatEquipNotice,					ID_S2C_RESPONSE_EATEQUIP )
		CASE_NEW_MSG( CMessageComposePushRequest,				ID_C2S_REQUEST_PUSHCOMPOSE )
		CASE_NEW_MSG( CMessageComposePushResponse,				ID_S2C_RESPONSE_PUSHCOMPOSE )
		CASE_NEW_MSG( CMessageComposePopRequest,				ID_C2S_REQUEST_POPCOMPOSE )
		CASE_NEW_MSG( CMessageComposePopResponse,				ID_S2C_RESPONSE_POPCOMPOSE )
		CASE_NEW_MSG( CMessageComposeRequest,					ID_C2S_REQUEST_COMPOSE )
		CASE_NEW_MSG( CMessageComposeNotice,					ID_S2C_RESPONSE_COMPOSE )
		CASE_NEW_MSG( CMessageOpenPushRequest,					ID_C2S_REQUEST_PUSHSLOT )
		CASE_NEW_MSG( CMessageOpenPushResponse,					ID_S2C_RESPONSE_PUSHSLOT )
		CASE_NEW_MSG( CMessageOpenPopRequest,					ID_C2S_REQUEST_POPSLOT )
		CASE_NEW_MSG( CMessageOpenPopResponse,					ID_S2C_RESPONSE_POPSLOT )
		CASE_NEW_MSG( CMessageOpenSlotRequest,					ID_C2S_REQUEST_OPENSLOT )
		CASE_NEW_MSG( CMessageOpenSlotNotice,					ID_S2C_RESPONSE_OPENSLOT )
		CASE_NEW_MSG( CMessagePlayerTeleRequest,				ID_C2S_REQUEST_PLAYERTELE )
		CASE_NEW_MSG( CMessagePlayerTeleResponse,				ID_S2C_RESPONSE_PLAYERTELE )
		CASE_NEW_MSG( CMessageRandMoveDirectCallBack,			ID_S2C_RANDMOVEDIRECTION )
		CASE_NEW_MSG( CMessageProDebugCommand,					ID_C2S_DEBUGCOMMAND )
		CASE_NEW_MSG( CMessageNotifyDirection,					ID_C2S_NOTIFY_DIRECTION )
		CASE_NEW_MSG( CMessageNotifyDirectionCallBack,			ID_S2C_NOTIFY_DIRECTION )
		CASE_NEW_MSG( CMessageChangePetModeRequest,				ID_C2S_REQUEST_CHANGEPETMODE )
		CASE_NEW_MSG( CMessageChangePetModeResponse,			ID_S2C_RESPONSE_CHANGEPETMODE )
		CASE_NEW_MSG( CMessagePetLevelUpNotice,					ID_S2C_NOTIFY_PETLEVELUP )
		CASE_NEW_MSG( CMessageSyncPosNotify,					ID_S2C_NOTIFY_SYNCPOS )
		CASE_NEW_MSG( CMessageSpeedChangedNotify,				ID_S2C_NOTIFY_SPEEDCHANGED )
		CASE_NEW_MSG( CMessageProExpNotify,						ID_S2C_NOTIFY_PROEXP )
		CASE_NEW_MSG( CMessageClickTeleSymbolRequest,			ID_C2S_REQUEST_CLICKSYMBOL )
		CASE_NEW_MSG( CMessageClickTeleSymbolResponse,			ID_S2C_RESPONSE_CLICKSYMBOL )
		CASE_NEW_MSG( CMessageBaggageLockedRequest,				ID_C2S_NOTIFY_BAGGAGELOCKEN )
		CASE_NEW_MSG( CMessageBaggageLockedResponse,			ID_S2C_NOTIFY_BAGGAGELOCKEN )
	
		CASE_NEW_MSG( CMessageNotifyEqptDurability,             ID_S2C_NOTIFY_CHANGEHARDINESS )
		CASE_NEW_MSG( CMessageMendEquipment ,					ID_C2S_REQUEST_MENDEQUIPMENT  )
		CASE_NEW_MSG( CMessaegMendEquipmentCallBack,			ID_S2C_RESPONSE_MENDEQUIPMENT )
		
		CASE_NEW_MSG( CMessageCommonBindRequest,			ID_C2S_REQUEST_COMMONBIND )		
		CASE_NEW_MSG( CMessageCommonBindResponse,			ID_S2C_RESPONSE_COMMONBIND )	

		CASE_NEW_MSG( CMessageRemoteMendAllEquipRequest,		ID_C2S_REQUEST_REMOTEMEND )
		CASE_NEW_MSG( CMessageRemoteMendAllEquipResponse,		ID_S2C_RESPONSE_REMOTEMEND )
		
		CASE_NEW_MSG( CMessageStartProgressRequest,			ID_C2S_REQUEST_STARTPROGRESS )
		CASE_NEW_MSG( CMessageStartProgressResponse,		ID_S2C_RESPONSE_STARTPROGRESS )
		

		CASE_NEW_MSG( CMessageObtainSaleItem,					ID_C2S_REQUEST_SALEITEM )
		CASE_NEW_MSG( CMessageObtainSaleItemCallBack,			ID_S2C_RESPONSE_SALEITEM )
		CASE_NEW_MSG( CMessageBuyBackItem,						ID_C2S_REQUEST_BUYBACK )
		CASE_NEW_MSG( CMessageBuyBackItemCallBack,				ID_S2C_RESPONSE_BUYBACK )
		CASE_NEW_MSG( CMessageCloseWindowRequest,				ID_C2S_REQUEST_CLOSEWINDOW )
		CASE_NEW_MSG( CMessageCloseWindowResponse,				ID_S2C_RESPONSE_CLOSEWINDOW )
		CASE_NEW_MSG( CMessageBindItemRequest,			        ID_C2S_REQUEST_BINDITEMREQ )
		CASE_NEW_MSG( CMessageBindItemResponse,			        ID_S2C_RESPONSE_BINDITEM )
		CASE_NEW_MSG( CMessageErrorActOnBindItemNotify,			ID_S2C_NOTIFY_ERRORACTONBINDITEM )

		CASE_NEW_MSG( CMessageOperateAutoHookRequest,			ID_C2S_REQUEST_AUTOHOOK )
		CASE_NEW_MSG( CMessageOperateAutoHookResponse,			ID_S2C_RESPONSE_AUTOHOOK )
		CASE_NEW_MSG( CMessageBuyPhysicRequest,					ID_C2S_REQUEST_BUYPHYSIC )
		CASE_NEW_MSG( CMessageBuyPhysicResponse,				ID_S2C_RESPONSE_BUYPHYSIC )

		CASE_NEW_MSG( CMessageAddBindItemRequest,				ID_C2S_REQUEST_ADDITEMREQUEST )
		CASE_NEW_MSG( CMessageAddBindItemResponse,				ID_S2C_RESPONSE_ADDITEMREPONSE )
		CASE_NEW_MSG( CMessageTakeBindItemRequest,				ID_C2S_REQUEST_TAKEITEMREQUEST )
		CASE_NEW_MSG( CMessageTakeBindItemResponse,				ID_S2C_RESPONSE_TAKEITEMRESPONSE )
		CASE_NEW_MSG( CMessageAddItemForFreeRequest,			ID_C2S_REQUEST_ADDITEMFORFREE )
		CASE_NEW_MSG( CMessageAddItemForFreeResponse,			ID_S2C_RESPONSE_ADDITEMFORFREE )		
		CASE_NEW_MSG( CMessageTakeItemForFreeRequest,			ID_C2S_REQUEST_TAKEITEMFORFREE )
		CASE_NEW_MSG( CMessageTakeItemForFreeResponse,			ID_S2C_RESPONSE_TAKEITEMFORFREE)
		CASE_NEW_MSG( CMessageFreeItem,							ID_C2S_REQUEST_FREEITEM )
		CASE_NEW_MSG( CMessageFreeItemResponse,					ID_S2C_RESPONSE_FREEITEM )
		CASE_NEW_MSG( CMessageRideHorseNotice,					ID_S2C_NOTIFY_RIDEHORSE )
		CASE_NEW_MSG( CMessageUseTaskItemRequest,				ID_C2S_REQUEST_USETASKITEM )
		CASE_NEW_MSG( CMessageUseTaskItemNotice,				ID_S2C_NOTICE_USETASKITEM )
		CASE_NEW_MSG( CMessageStudySkillRequest,				ID_C2S_REQUEST_STUDYSKILL )
		CASE_NEW_MSG( CMessageStudySkillResponse,				ID_S2C_RESPONSE_STUDYSKILL )
		CASE_NEW_MSG( CMessageAddPropertyPointRequest,			ID_C2S_REQUEST_ADDPROPERTYPOINT )
		CASE_NEW_MSG( CMessageAddPropertyPointResponse,			ID_S2C_RESPONSE_ADDPROPERTYPOINT )
		CASE_NEW_MSG( CMessageChangePKModeRequest,				ID_C2S_REQUEST_CHANGEPKMODE )
		CASE_NEW_MSG( CMessageChangePKModeResponse,				ID_S2C_RESPONSE_CHANGEPKMODE )
		CASE_NEW_MSG( CMessageSetTownRequest,					ID_C2S_REQUEST_SETTOWN )
		CASE_NEW_MSG( CMessageSetTownResponse,					ID_S2C_RESPONSE_SETTOWN )
		CASE_NEW_MSG( CMessageFuncResultCallBack,				ID_S2C_FUNCRESULT_CALLBACK )	
		CASE_NEW_MSG( CMessageLookPropertyRequest,				ID_C2S_REQUEST_LOOKPROPERTY )
		CASE_NEW_MSG( CMessageLookPropertyResponse,				ID_S2C_RESPONSE_LOOKPROPERTY )
		CASE_NEW_MSG( CMessageLookPropertyRouterRequest,		ID_S2S_REQUEST_LOOKPROPERTY )
		CASE_NEW_MSG( CMessageLookPropertyRouterResponse,		ID_S2S_RESPONSE_LOOKPROPERTY )
		CASE_NEW_MSG( CMessageThrowStorItemRequest,             ID_C2S_REQUEST_THROWSTORITEM )
		CASE_NEW_MSG( CMessageThrowStorItemResponse,			ID_C2S_RESPONSE_THROWSTORITEM )
		CASE_NEW_MSG( CMessageNotifyItemBinded,					ID_C2S_NOTIFY_ITEMHASBINDED )
		CASE_NEW_MSG( CMessageNotifyStorItem,					ID_S2C_NOTIFY_STORAGEITEM )
		CASE_NEW_MSG( CMessageTaskCollectRequest,				ID_C2S_REQUEST_TASKCOLLECT )
		CASE_NEW_MSG( CMessageChangeHonorNotify,				ID_S2C_NOTIFY_CHANGEHONOR )
		CASE_NEW_MSG( CMessageHonorToExpRequest,				ID_C2S_REQUEST_HONORTOEXP )
		CASE_NEW_MSG( CMessageBrushOgreNotify,					ID_S2C_NOTIFY_BRUSHOGRE )

		CASE_NEW_MSG( CMessageObtainDiploidTimeRequest,			ID_C2S_REQUEST_OBTAINDIPLOIDTIME );	
		CASE_NEW_MSG( CMessageObtainDiploidResponse,			ID_S2C_RESPONSE_OBTAINDIPLOID );
		CASE_NEW_MSG( CMessageFreezeDiploidRequest,				ID_C2S_REQUEST_FREEZEDIPLOID );
		CASE_NEW_MSG( CMessageFreezeDiploidResponse,			ID_S2C_RESPONSE_FREEZEDIPLOID );
		CASE_NEW_MSG( CMessageQueryRemainDiploid,				ID_C2S_REQUEST_QUERYREMAINTIME );
		CASE_NEW_MSG( CMessageRemainDiploidNotice,				ID_S2C_RESPONSE_REMAINTIMENOTICE );

		CASE_NEW_MSG( CMessageNotifyModifyPKValue,				ID_S2C_NOTIFY_MODIFYPKVALUE );
		CASE_NEW_MSG( CMessageYellowStateNotify,				ID_S2C_NOTIFY_YELLOWSTATE );
		
		CASE_NEW_MSG( CMessageAddEqtForResumeBindRequest,		ID_C2S_REQUEST_ADDEQTFORRESUMEBIND );
		CASE_NEW_MSG( CMessageAddEqtForResumeBindResponse,		ID_S2C_RESPONSE_ADDEQTFORRESUMEBIND );
		CASE_NEW_MSG( CMessageTakeEqtForResumeBindRequest,		ID_C2S_REQUEST_TAKEEQTFORRUMEBIND );
		CASE_NEW_MSG( CMessageTakeEqtForResumeBindResponse,		ID_S2C_RESPONSE_TAKEEQTFORRUMEBIND );
		CASE_NEW_MSG( CMessageResumeBindRequest,				ID_C2S_REQUEST_RESUMEBIND  );
		CASE_NEW_MSG( CMessageResumeBindResponse,				ID_S2C_RESPONSE_RESUMEBIND );

		CASE_NEW_MSG( CMessageUseContinuedSkillRequest,			ID_C2S_REQUEST_USECONTINUEDSKILL );

		CASE_NEW_MSG( CMessageChangeHelpStaus,					ID_C2S_REQUEST_CHANGEHELPSTATUS )
		CASE_NEW_MSG( CMessageServerRefreshNotify,				ID_S2C_NOTIFY_SERVERREFRESH );		
		
		CASE_NEW_MSG( CMessageBeginRedstoneMagicNotify,			ID_S2C_NOTIFY_BEGINREDSTONEMAGIC );
		CASE_NEW_MSG( CMessageRedstoneStarted,					ID_S2C_NOTIFY_REDSTONESTATRED );
		CASE_NEW_MSG( CMessageClickRedStoneRequest,			    ID_C2S_REQUEST_CLICKREDSTONDE )
		CASE_NEW_MSG( CMessageClickRedStoneResponse,			ID_S2C_RESPONSE_CLICKREDSTONDE )
		CASE_NEW_MSG( CMessageStopRedStoneTimer,				ID_S2C_NOTIFY_STOPRENDSTONETIMEER )
		CASE_NEW_MSG( CMessageRedStoneTimerEnd,					ID_C2S_NOTIFY_REDSTONETIMEREND )
		CASE_NEW_MSG( CMessageChangeRestoneStaus,				ID_S2C_NOTIFY_CHANGEREDSTONESTATUS )
		CASE_NEW_MSG( CMessageExpMagicEnd,						ID_S2C_NOTIFY_EXPMAGICEND )
		CASE_NEW_MSG( CMessageClickWelfareRequest,				ID_C2S_REQUEST_CLICKWELFARE )
		CASE_NEW_MSG( CMessageClickWelfareResponse,				ID_S2C_RESPONSE_CLICKWELFARE)
		CASE_NEW_MSG( CMessageChangeCampRequest,				ID_C2S_CHANGE_CAMP_REQUEST )
		CASE_NEW_MSG( CMessageChangeCampResponse,				ID_S2C_CHANGE_CAMP_RESPONSE )
		CASE_NEW_MSG( CMessageInsertEquipNotify,				ID_S2C_NOTIFY_INSERTEQUIP )
		CASE_NEW_MSG( CMessageOpenServiceRequest,				ID_C2S_REQUEST_OPEN_SERVICE )
		CASE_NEW_MSG( CMessageOpenServiceResponse,				ID_S2C_RESPONSE_OPEN_SERVICE )
		CASE_NEW_MSG( CMessageRefreshPropertyNotify,			ID_S2C_NOTIFY_REFRESHPROPERTY )
		CASE_NEW_MSG( CMessageChangePKTypeNotify,				ID_S2C_NOTIFY_CHANGEPKTYPE )
		CASE_NEW_MSG( CMessageActiveRemoveBuffRequest,			ID_C2S_REQUEST_ACTIVEREMOVEBUFF ) 
		CASE_NEW_MSG( CMessageChangeEntityPKTypeNotify,			ID_S2C_NOTIFY_CHANGEENTITYPKTYPE )
		
		CASE_NEW_MSG( CMessageSetPasswordRequest,				ID_C2S_REQUEST_SETPASSWORD )
		CASE_NEW_MSG( CMessageSetPasswordResponse,				ID_S2C_RESPONSE_SETPASSWORD )
		CASE_NEW_MSG( CMessageCheckPasswordRequest,				ID_C2S_REQUEST_CHECKPASSWORD )
		CASE_NEW_MSG( CMessageCheckPasswordResponse,			ID_S2C_RESPONSE_CHECKPASSWORD )
		CASE_NEW_MSG( CMessageModifyPasswordRequest,			ID_C2S_REQUEST_MODIFYPASSWORD )
		CASE_NEW_MSG( CMessageModifyPasswordResponse,			ID_S2C_RESPONSE_MODIFYPASSWORD )
		CASE_NEW_MSG( CMessageSetPasswordOptionNotify,			ID_C2S_NOTIFY_SETPASSWORDOPTION )		
		
		

		CASE_NEW_MSG( CMessageIbItemRemainNotice,				ID_S2C_NOTIFY_IBSTOREPROMOTION)		
		CASE_NEW_MSG( CMessageListIbItemsRequest,				ID_C2S_REQUEST_LISTIBITEMS)		
		CASE_NEW_MSG( CMessageListIbItemsResponse,				ID_S2C_RESPONSE_LISTIBITEMS)		
		CASE_NEW_MSG( CMessagePurchaseIbItemRequest,			ID_C2S_REQUEST_PURCHASEIBITEM)		
		CASE_NEW_MSG( CMessagePurchaseIbItemResponse,			ID_S2C_RESPONSE_PURCHASEIBITEM)	
		CASE_NEW_MSG( CMessageUserChargeNotice,					ID_S2C_NOTIFY_USERCHARGE)		
		CASE_NEW_MSG( CMessageQueryGoldenYBRequeset,			ID_C2S_REQUEST_QUERY_GOLDENYB)		
		CASE_NEW_MSG( CMessageQueryGoldenYBResponse,			ID_S2C_RESPONSE_QUERY_GOLDENYB)		
		CASE_NEW_MSG( CMessageDrawGoldenYBRequest,				ID_C2S_REQUEST_DRAW_GOLDENYB)		
		CASE_NEW_MSG( CMessageDrawGoldenYBResponse,				ID_S2C_RESPONSE_DRAW_GLODENYB)		
		CASE_NEW_MSG( CMessagePlayerUseItemNotify,				ID_S2C_NOTIFY_USEITEM)		
		CASE_NEW_MSG( CMessageActivityChange2CliNotify,			ID_S2C_NOTIFY_ACTIVITYCHANGED)

		CASE_NEW_MSG( CMessageGetFuncConsumesRequest,			ID_C2S_GET_FUNCCONSUMES_REQUEST)
		CASE_NEW_MSG( CMessageGetFuncConsumesResponse,			ID_S2C_GET_FUNCCONSUMES_RESPONSE)
		CASE_NEW_MSG( CMessagePurchaseFuncRequest,				ID_C2S_PURCHASE_FUNC_REQUEST)
		CASE_NEW_MSG( CMessagePurchaseFuncResponse,				ID_S2C_PURCHASE_FUNC_RESPONSE)


		CASE_NEW_MSG( CMessageUseFlowerRequest,				ID_C2S_REQUEST_USEFLOWER)		
		CASE_NEW_MSG( CMessageUseFlowerResponse,			ID_S2C_RESPONSE_USEFLOWER)		
		CASE_NEW_MSG( CMessageObtainFlowerNotify,			ID_S2C_NOTIFY_OBTAINFLOWER)		
		CASE_NEW_MSG( CMessageS2SPlayerUseFlowerRequest,			ID_S2S_REQUEST_USEFLOWER)		
		CASE_NEW_MSG( CMessageS2SPlayerUseFlowerResponse,			ID_S2S_RESPONSE_USEFLOWER)		

		
		CASE_NEW_MSG( CMessageStorMoneyRequest,					ID_C2S_REQUEST_STORMONEY )	
		CASE_NEW_MSG( CMessageStorMoneyResponse,				ID_S2C_RESPONSE_STORMONEY)
		CASE_NEW_MSG( CMessageTakeStorMoneyRequest,				ID_C2S_REQUEST_TAKESTORMONEY )
		CASE_NEW_MSG( CMessageTakeStorMoneyResponse,			ID_S2C_RESPONSE_TAKESTORMONEY )


		CASE_NEW_MSG( CMessageInterruptContiuneSkillRequest,	ID_C2S_REQUEST_INTERRUPTSKILL )
		CASE_NEW_MSG( CMessageInterruptContiuneSkillNotify,		ID_S2C_NOTIFY_INTERRUPTSKILL )
		CASE_NEW_MSG( CMessageUseContinuedSkillNotify,			ID_S2C_NOTIFY_USECONTINUESKILL )
		CASE_NEW_MSG( CMessageEventNotify,						ID_S2C_NOTIFY_EVENT )
		CASE_NEW_MSG( CMessageActiveCardNotify,					ID_S2C_NOTIFY_ACTIVECARD )
		CASE_NEW_MSG( CMessagePushCardRequest,					ID_C2S_REQUEST_PUSHCARD )
		CASE_NEW_MSG( CMessagePushCardResponse,					ID_S2C_RESPONSE_PUSHCARD )
		CASE_NEW_MSG( CMessageTalentChangeNotify,				ID_S2C_NOTIYFY_TALENTCHANGE )
		CASE_NEW_MSG( CMessageClickTalentPointRequest,			ID_C2S_REQUEST_CLICKTALENTPOINT )
		CASE_NEW_MSG( CMessageClickTalentPointResponse,			ID_S2C_RESPONSE_CLICKTALENTPOINT )
		CASE_NEW_MSG( CMessageIncreaseTalentRequest,			ID_C2S_REQUEST_INCREASESTAGE )
		CASE_NEW_MSG( CMessageIncreaseTalentResponse,			ID_S2C_RESPONSE_INCREASESTAGE )
		CASE_NEW_MSG( CMessageUpdateTitleNotice,				ID_S2C_NOTICE_UPDATETITLE )
		CASE_NEW_MSG( CMessageSelectTitleRequest,				ID_C2S_REQUEST_SELECTTITLE )
		CASE_NEW_MSG( CMessageUpdateCurrentTitleNotice,			ID_S2C_NOTICE_UPDATECURRENTTITLE )
		CASE_NEW_MSG( CMessageActivityDiploidNotice,			ID_S2C_NOTICE_ACTIVITYDIPLOID )
		
		CASE_NEW_MSG( CMessageInitialLifeSkillRequest,			ID_C2S_REQUEST_INITIALLIFESKILL )
		CASE_NEW_MSG( CMessageInitialLifeSkillResponse,			ID_S2C_RESPONSE_INITIALLIFESKILL )	
		
		CASE_NEW_MSG( CMessageLevelUPLifeSkillRequest,			ID_C2S_REQUEST_LEVELUPLIFESKILL )
		CASE_NEW_MSG( CMessageLevelUPLifeSkillResponse,			ID_S2C_RESPONSE_LEVELUPLIFESKILL )
		
		CASE_NEW_MSG( CMessageStudyComposeRequest,				ID_C2S_REQUEST_STUDYCOMPOSE )
		CASE_NEW_MSG( CMessageStudyComposeResponse,				ID_S2C_RESPONSE_STUDYCOMPOSE )		
		
		CASE_NEW_MSG( CMessageSetMagicStoneRequest,				ID_C2S_REQUEST_SETMAGICSTONE )
		CASE_NEW_MSG( CMessageSetMagicStoneResponse,			ID_S2C_RESPONSE_SETMAGICSTONE )		

		CASE_NEW_MSG( CMessageDecompoundEquipRequest,			ID_C2S_REQUEST_DECOMPOUNDEQUIP )
		CASE_NEW_MSG( CMessageDecompoundEquipResponse,			ID_S2C_RESPONSE_DECOMPOUNDEQUIP )  
		
		CASE_NEW_MSG( CMessageAddMaterialRequest,				ID_C2S_REQUEST_ADDMATERIAL )
		CASE_NEW_MSG( CMessageAddMaterialResponse,				ID_S2C_RESPONSE_ADDMATERIAL )
		
		CASE_NEW_MSG( CMessageForgetSkillRequest,				ID_C2S_REQUEST_FORGETLIFESKILL )
		CASE_NEW_MSG( CMessageForgetSkillResponse,				ID_S2C_RESPONSE_FORGETLIFESKILL )
		
		CASE_NEW_MSG( CMessageLifeSkillAttributeNotify,			ID_S2C_NOTIFY_LIFESKILLATTRIBUTE )					

		CASE_NEW_MSG( CMessageTakeMaterialRequest,				ID_C2S_REQUEST_TAKEMATERIAL )
		CASE_NEW_MSG( CMessageTakeMaterialResponse,				ID_S2C_RESPONSE_TAKEMATERIAL )

		CASE_NEW_MSG( CMessageMultiBuffRequest,					ID_C2S_REQUEST_MULTIBUFF )
		CASE_NEW_MSG( CMessageChangeCamp,						ID_S2C_CHANGE_CAMP )
		CASE_NEW_MSG( CMessageChangePKDropMode,					ID_C2S_CHANGEPKDROP )
		
		CASE_NEW_MSG( CMessageGetEquipExchangeListRequest,		ID_C2S_REQUEST_GETEUIPEXCHANGELIST )
		CASE_NEW_MSG( CMessageGetEquipExchangeListResponse,		ID_S2C_RESPONSE_GETEUIPEXCHANGELIST )

		CASE_NEW_MSG( CMessagePushMultiRequest,					ID_C2S_REQUEST_PUSHMULTI )
		CASE_NEW_MSG( CMessagePushMultiResponse,				ID_S2C_RESPONSE_PUSHMULTI )
		CASE_NEW_MSG( CMessagePopMultiRequest,					ID_C2S_REQUEST_POPMULTI )
		CASE_NEW_MSG( CMessagePopMultiResponse,					ID_S2C_RESPONSE_POPMULTI )
		
		CASE_NEW_MSG( CMessageAddEcompoundEquipRequest,			ID_C2S_REQUEST_ADDDECOMPOUNDEQUIP )
		CASE_NEW_MSG( CMessageAddEcompoundEquipResponse,		ID_S2C_RESPONSE_ADDDECOMPOUNDEQUIP )
		
		CASE_NEW_MSG( CMessageTakeDecompoundEquipRequest,		ID_C2S_REQUEST_TAKERDECOMPOUNDEQUIP )
		CASE_NEW_MSG( CMessageTakeDecompoundEquipResponse,		ID_S2C_RESPONSE_TAKERDECOMPOUNDEQUIP )

		CASE_NEW_MSG( CMessageProPrisonTimeNotice,				ID_S2C_NOTIFY_PRISONTIME )
		
		CASE_NEW_MSG( CMessageTakeKitBagRequest,				ID_C2S_REQUEST_TAKEKITBAG )		
		CASE_NEW_MSG( CMessageTakeKitBagResponse,				ID_S2C_RESPONSE_TAKEKITBAG )			
		CASE_NEW_MSG( CMessagePickItemNotify,					ID_S2C_NOTIFY_PICKITEM )

		CASE_NEW_MSG( CMessageGetTeleListRequest,				ID_C2S_REQUEST_GETTELELIST	 )
		CASE_NEW_MSG( CMessageGetTeleListResponse,				ID_S2C_RESPONSE_GETTELELIST )
		CASE_NEW_MSG( CMessageAddTelePointRequest,				ID_C2S_REQUEST_ADDTELEPOINT )
		CASE_NEW_MSG( CMessageAddTelePointResponse,				ID_S2C_RESPONSE_ADDTELEPOINT )
		CASE_NEW_MSG( CMessageDelTelePointRequest,				ID_C2S_REQUEST_DELTELEPOINT	 )
		CASE_NEW_MSG( CMessageDelTelePointResponse,				ID_S2C_RESPONSE_DELTELEPOINT )
		CASE_NEW_MSG( CMessageUseTeleItemRequest,				ID_C2S_REQUEST_USETELEITEM	 )


		CASE_NEW_MSG( CMessageProVipNotice,						ID_S2C_NOTIFY_VIPNOTICE )
		
		CASE_NEW_MSG( CMessageSetPwdProtectTimeRequest,			ID_C2S_REQUEST_SETPWDPROTECTTIME )
		CASE_NEW_MSG( CMessageSetPwdProtectTimeResponse, ID_S2C_RESPONSE_SETPWDPROTECTTIME )

		CASE_NEW_MSG( CMessageChangeMasterRequest,				ID_C2S_REQUEST_CHANGEMASTER )
		CASE_NEW_MSG( CMessageChangeMasterResponse,				ID_S2C_RESPONSE_CHANGEMASTER )
		CASE_NEW_MSG( CMessageChangeMasterNotify,				ID_S2C_NOTIFY_CHANGEMASTER )
		CASE_NEW_MSG( CMessageGetSlaveRequest,					ID_C2S_REQUEST_GETSLAVE	 )
		CASE_NEW_MSG( CMessageGetSlaveResponse,					ID_S2C_RESPONSE_GETSLAVE )
		CASE_NEW_MSG( CMessageSlaveLevelUpNotify,				ID_S2C_NOTIFY_SLAVELEVELUP	 )
		CASE_NEW_MSG( CMessageAnswerChangeNotify,				ID_S2C_NOTIFY_ANSWERCHANGE	 )
		CASE_NEW_MSG( CMessageQueryPlayerRequest,				ID_C2S_REQUEST_QUERYPLAYER )
		CASE_NEW_MSG( CMessageQueryPlayerResponse,				ID_S2C_RESPONSE_QUERYPLAYER )
		CASE_NEW_MSG( CMessageSlaveOnOffNotify,					ID_S2C_NOTIFY_SLAVEONOFF )
		CASE_NEW_MSG( CMessageSlaveNumNotify,					ID_S2C_NOTIFY_SLAVENUM )
		CASE_NEW_MSG( CMessageRemoveSlaveRequest,				ID_C2S_REQUEST_REMOVESLAVE )
		CASE_NEW_MSG( CMessageRemoveSlaveResponse,				ID_S2C_RESPONSE_REMOVESLAVE )
		CASE_NEW_MSG( CMessageRemoveSlaveNotify,				ID_S2C_NOTIFY_REMOVESLAVE )
		CASE_NEW_MSG( CMessageResetTalentRequest,				ID_C2S_REQUEST_RESETTALENT )
		CASE_NEW_MSG( CMessageResetTalentResponse,				ID_S2C_RESPONSE_RESETTALENT )
		CASE_NEW_MSG( CMessageRemoveMasterRequest,				ID_C2S_REQUEST_REMOVEMASTER )
		CASE_NEW_MSG( CMessageRemoveMasterResponse,				ID_S2C_RESPONSE_REMOVEMASTER )
		CASE_NEW_MSG( CMessageRemoveMasterNotify,				ID_S2C_NOTIFY_REMOVEMASTER )
		CASE_NEW_MSG( CMessageAlterMasterPercentRequest,		ID_C2S_REQUEST_ALTERMASTERPERCENT )
		CASE_NEW_MSG( CMessageAlterMasterPercentResponse,		ID_S2C_RESPONSE_ALTERMASTERPERCENT )
		CASE_NEW_MSG( CMessageGetLowPlayerRequest,				ID_C2S_REQUEST_GETLOWPLAYER	 )
		CASE_NEW_MSG( CMessageGetLowPlayerResponse,				ID_S2C_RESPONSE_GETLOWPLAYER )
		CASE_NEW_MSG( CMessageAnswerChangeRequest,				ID_C2S_REQUEST_ANSWERCHANGE	 )

		CASE_NEW_MSG( CMessageDecomposeMWRequest,				ID_C2S_REQUEST_DECOMPOSEMW		 )
		CASE_NEW_MSG( CMessageDecomposeMWResponse,				ID_S2C_RESPONSE_DECOMPOSEMW	 )
		CASE_NEW_MSG( CMessageMWQLPrintRequest,					ID_C2S_REQUEST_MWQLPNT	 )
		CASE_NEW_MSG( CMessageMWQLPrintResponse,				ID_S2C_RESPONSE_MWQLPNT	 )
		CASE_NEW_MSG( CMessageQuickUpRequest,					ID_C2S_REQUEST_QUICKUP		 )
		CASE_NEW_MSG( CMessageQuickUpResponse,					ID_S2C_RESPONSE_QUICKUP		 )
		CASE_NEW_MSG( CMessageQilinComposeRequest,				ID_C2S_REQUEST_QLCOMPOSE	 )
		CASE_NEW_MSG( CMessageQilinComposeResponse,				ID_S2C_RESPONSE_QLCOMPOSE	 )
		CASE_NEW_MSG( CMessageMWObtainExpNotify,				ID_S2C_NOTIFY_MWOBTAINEXP	 )

		CASE_NEW_MSG( CMessageGetActivityInfoRequest,			ID_C2S_REQUEST_GETACTVITYINFO )
		CASE_NEW_MSG( CMessageGetActivityTimesResponse,			ID_S2C_RESPONSE_GETACTVITYINFO )
		CASE_NEW_MSG( CMessageChangeEquipProRequest,				ID_C2S_REQUEST_CHANGEEQUIPPRO )
		CASE_NEW_MSG( CMessageChangeEquipProResponse,				ID_S2C_RESPONSE_CHANGEEQUIPPRO )
		CASE_NEW_MSG( CMessageOpenStoneRequest,					ID_C2S_REQUEST_OPENSTONE )
		CASE_NEW_MSG( CMessageOpenStoneResponse,				ID_S2C_RESPONSE_OPENSTONE )
		CASE_NEW_MSG( CMessagePlayerPickItemNotify,					ID_S2C_NOTIFY_PLAYERPICKITEM )
		CASE_NEW_MSG( CMessageQueryPedalRequest,					ID_C2S_REQUEST_QUERYPEDAL )
		CASE_NEW_MSG( CMessageQueryPedalResponse,				ID_S2C_RESPONSE_QUERYPEDAL )
		CASE_NEW_MSG( CMessageAutoBuyItemRequest,					ID_C2S_REQUEST_AUTOBUYITEM )
		CASE_NEW_MSG( CMessageAutoBuyItemResponse,				ID_S2C_RESPONSE_AUTOBUYITEM )
		CASE_NEW_MSG( CMessageUseSummonItemRequest,				ID_C2S_REQUEST_USESUMMONITEM )
		CASE_NEW_MSG( CMessageUseSummonItemResponse,				ID_S2C_RESPONSE_USESUMMONITEM )
		CASE_NEW_MSG( CMessageSplitEquipRequest,				ID_C2S_REQUEST_SPLITEQUIP )
		CASE_NEW_MSG( CMessageSplitEquipResponse,				ID_S2C_RESPONSE_SPLITEQUIP )
		CASE_NEW_MSG( CMessageGetStarRequest,					ID_C2S_REQUEST_GETSTAR )
		CASE_NEW_MSG( CMessageGetStarResponse,					ID_S2C_RESPONSE_GETSTAR )
		CASE_NEW_MSG( CMessageAddStarRequest,					ID_C2S_REQUEST_ADDSTAR )
		CASE_NEW_MSG( CMessageAddStarResponse,					ID_S2C_RESPONSE_ADDSTAR )
		CASE_NEW_MSG( CMessageEquipmentOverdueNotify,			ID_S2C_NOTIFY_EQUIPOVERDUE )
		CASE_NEW_MSG( CMessageItemStartTimerNotify,				ID_S2C_NOTIFY_STARTTIMER )
		CASE_NEW_MSG( CMessageAddExtraRequest,					ID_C2S_REQUEST_ADDEXTRA )
		CASE_NEW_MSG( CMessageAddExtraResponse,					ID_S2C_RESPONSE_ADDEXTRA )
		CASE_NEW_MSG( CMessageCanUseAutoCollectItemRequest, ID_C2S_REQUEST_CANUSEAUTOCOLLECTITEM )
		CASE_NEW_MSG( CMessageCanUseAutoCollectItemResponse, ID_S2C_RESPONSE_CANUSEAUTOCOLLECTITEM )
		CASE_NEW_MSG( CMessageLearnFangCunSkillRequest,				ID_C2S_REQUEST_LEARNFANGCUNSKILL )
		CASE_NEW_MSG( CMessageLearnFangCunSkillResponse,			ID_S2C_RESPONSE_LEARNFANGCUNSKILL )
		CASE_NEW_MSG( CMessagePlayerTransformNotify,			ID_S2C_NOTIFY_PLAYERTRANSFORM )
		CASE_NEW_MSG( CMessageHonorToItemRequest,			ID_C2S_REQUEST_HONORTOITEM )

		CASE_NEW_MSG( CMessageCollectByAutoItemRequest, ID_C2S_REQUEST_COLLECTBYAUTOITEM )
		CASE_NEW_MSG( CMessageCollectByAutoItemResponse, ID_S2C_RESPONSE_COLLECTBYAUTOITEM  )		
		CASE_NEW_MSG( CMessageUpdateYuanBaoNotice,				ID_S2C_NOTICE_YUANBAOUPDATE )
		CASE_NEW_MSG( CMessageUpgAndJewNotify,				ID_S2C_NOTICE_UPGANDJEW )

		CASE_NEW_MSG( CMessageJewelCarveRequest,				ID_C2S_REQUEST_JEWELCARVE )
		CASE_NEW_MSG( CMessageJewelCarveResponse,				ID_S2C_RESPONSE_JEWELCARVE )
		
		//符文第二期
		CASE_NEW_MSG( CMessageRequestChangeRune,				ID_C2S_REQUEST_CHANGERUNE )
		CASE_NEW_MSG( CMessageResponseChangeRune,				ID_S2C_RESPONSE_CHANGERUNE )
		CASE_NEW_MSG( CMessageRequestRuneCom,					ID_C2S_REQUEST_RUNECOM)
		CASE_NEW_MSG( CMessageResponseRuneCom,					ID_S2C_RESPONSE_RUNECOM)

		CASE_NEW_MSG( CMessageQueryBossRequest,				ID_C2S_REQUEST_QUERYBOSS )
		CASE_NEW_MSG( CMessageQueryBossResponse,				ID_S2C_RESPONSE_QUERYBOSS )

		CASE_NEW_MSG( CMessageQueryAroundPlayerRequest,			ID_C2S_QUERY_AROUND_PLAYER )
		CASE_NEW_MSG( CMessageQueryAroundPlayerResponse,		ID_S2C_QUERY_AROUND_PLAYER )

		CASE_NEW_MSG( CMessagePlayerRepetData,					ID_S2C_PLAYER_REPETION_DATA )
		
		// 数字礼花
		CASE_NEW_MSG( CMessageStartDigitalFireworksRequest,				ID_C2S_REQUEST_STARTDIGITALFIREWORKS )
		CASE_NEW_MSG( CMessageStartDigitalFireworksResponse,				ID_S2C_RESPONSE_STARTDIGITALFIREWORKS )
		CASE_NEW_MSG( CMessageStartDigitalFireworksNotify,				ID_S2C_NOTIFY_STARTDIGITALFIREWORKS )

        // 防沉迷通知
        CASE_NEW_MSG( CMessagePlayerFcmStatusNotify,            ID_S2C_NOTIFY_PLAYERFCMSTATUS )
		
		// 法宝的
		CASE_NEW_MSG(CMessageAddPotentialRequest,				ID_C2S_REQUEST_ADDPOTENTIAL		)
		CASE_NEW_MSG(CMessageAddPotentialResponse,				ID_S2C_RESPONSE_ADDPOTENTIAL	)
		CASE_NEW_MSG(CMessageChangeExpScaleRequest,				ID_C2S_REQUEST_CHANGEEXPSCALE	)
		CASE_NEW_MSG(CMessageChangeExpScaleResponse,			ID_S2C_RESPONSE_CHANGEEXPSCALE	)
		CASE_NEW_MSG(CMessageMagicWeaponRefreshValueNotify,		ID_S2C_NOTIFY_REFRESHVALUE		)

		CASE_NEW_MSG(CMessageActiveSkillBoxNotify,				ID_S2C_NOTIFY_ACTIVESKILLBOX	)
		CASE_NEW_MSG(CMessageInsertSkillNotify,					ID_S2C_NOTIFY_INSERTSKILL		)
		CASE_NEW_MSG(CMessageMWAddSkillPointsRequest,			ID_C2S_REQUEST_MWADDSKILLPOINTS	)
		CASE_NEW_MSG(CMessageMWAddSkillPointsResponse,			ID_S2C_RESPONSE_MWADDSKILLPOINTS)
		CASE_NEW_MSG(CMessageMWJudgeSkillRequest,				ID_C2S_REQUEST_MWJUDGESKILL		)

		CASE_NEW_MSG(CMessageLockMWInBagRequest,				ID_C2S_REQUEST_MWLOCKBAG	)
		CASE_NEW_MSG(CMessageLockMWInBagResponse,				ID_S2C_RESPONSE_MWLOCKBAG	)
		CASE_NEW_MSG(CMessageMWTakeOffItemRequest,				ID_C2S_REQUEST_MWTAKEOFF	)
		CASE_NEW_MSG(CMessageMWJudgePropertyRequest,			ID_C2S_REQUEST_JUDGEPROPERTY	)
		CASE_NEW_MSG(CMessageMWJudgePropertyResponse,			ID_S2C_RESPONSE_JUDGEPROPERTY	)
		CASE_NEW_MSG(CMessageMWUseBookRequest,					ID_C2S_REQUEST_USESKILLBOOK	)


		CASE_NEW_MSG(CMessageAddWizardPointRequest,				ID_C2S_REQUEST_ADDWIZARDPOINT	)
		CASE_NEW_MSG(CMessageAddWizardPointResponse,			ID_S2C_RESPONSE_ADDWIZARDPOINT	)
		CASE_NEW_MSG(CMessageHonorToWizardRequest,				ID_C2S_REQUEST_HONORTOWIZARD	)
		CASE_NEW_MSG(CMessageGetTrendRequest,					ID_C2S_REQUEST_GETTREND	)
		CASE_NEW_MSG(CMessageGetTrendResponse,					ID_S2C_RESPONSE_GETTREND )
		CASE_NEW_MSG(CMessageLearnOgreSkillRequest,				ID_C2S_REQUEST_LEARNOGRESKILL	)
		CASE_NEW_MSG(CMessageLearnOgreSkillResponse,			ID_S2C_RESPONSE_LEARNOGRESKILL )

		CASE_NEW_MSG(CMessageMagicWeaponClearPointRequest,		ID_C2S_MAGICWEAPONCLEARPOINTREQUEST			)
		CASE_NEW_MSG(CMessageMagicWeaponClearPointResponse,		ID_S2C_MAGICWEAPONCLEARPOINTRESPONSE		)																
		CASE_NEW_MSG(CMessageMagicWeaponClearPointAddMWRequest,	ID_C2S_MAGICWEAPONCLEARPOINT_ADDMWREQUEST	)
		CASE_NEW_MSG(CMessageMagicWeaponClearPointAddMWResponse,ID_S2C_MAGICWEAPONCLEARPOINT_ADDMWRESPONSE	)
		CASE_NEW_MSG(CMessageClientRunTimeParamNotify,			ID_S2C_NOTIFY_RUNTIMEPARAM )
		CASE_NEW_MSG(CMessageGetHelpDataRequest,				ID_C2S_GETHELPDATAREQUEST )
		CASE_NEW_MSG(CMessageGetHelpDataResponse,				ID_S2C_GETHELPDATARESPONSE )

		CASE_NEW_MSG(CMessageSpiritOpenRequest,					ID_C2S_REQUEST_SPIRIT_OPEN )
		CASE_NEW_MSG(CMessageSpiritOpenResponse,				ID_S2C_RESPONSE_SPIRIT_OPEN )
		CASE_NEW_MSG(CMessageSpiritCastingRequest,				ID_C2S_REQUEST_SPIRIT_CASTING )
		CASE_NEW_MSG(CMessageSpiritCastingResponse,				ID_S2C_RESPONSE_SPIRIT_CASTING )
		CASE_NEW_MSG(CMessageSpiritUpgradeRequest,				ID_C2S_REQUEST_SPIRIT_UPGRADE )
		CASE_NEW_MSG(CMessageSpiritUpgradeResponse,				ID_S2C_RESPONSE_SPIRIT_UPGRADE )
		CASE_NEW_MSG(CMessageSpiritDowngradeRequest,			ID_C2S_REQUEST_SPIRIT_DOWNGRADE )
		CASE_NEW_MSG(CMessageSpiritDowngradeResponse,			ID_S2C_RESPONSE_SPIRIT_DOWNGRADE )
		CASE_NEW_MSG(CMessageSpiritInfoRequest,					ID_C2S_REQUEST_SPIRIT_INFO )
		CASE_NEW_MSG(CMessageSpiritInfoNotify,					ID_S2C_NOTIFY_SPIRIT_INFO )

		// task message 
		CASE_NEW_MSG( CMessageObtainTaskRequest,	            ID_C2S_REQUEST_OBTAINTASK ) 
		CASE_NEW_MSG( CMessageTaskEndRequest,                   ID_C2S_REQUEST_ENDTASK )    
		CASE_NEW_MSG( CMessageObtainTaskNotice,                 ID_S2C_NOTICE_OBTAINTASK )  
		CASE_NEW_MSG( CMessageTaskCompleteNotice,               ID_S2C_NOTICE_COMPLETE )    
		CASE_NEW_MSG( CMessageTaskEndNotice,                    ID_S2C_NOTICE_TASKEND )     
		CASE_NEW_MSG( CMessageUpdateTaskNotice,                 ID_S2C_NOTICE_UPDATE )      
		CASE_NEW_MSG( CMessageResetTaskNotice,                  ID_S2C_NOTICE_RESET ) 
		CASE_NEW_MSG( CMessageAbortTaskRequest,                 ID_C2S_REQUEST_ABORTTASK )  
		CASE_NEW_MSG( CMessageTaskListNotice,                   ID_S2C_NOTICE_TASKLIST )    
		CASE_NEW_MSG( CMessageNpcTaskListNotice,                ID_S2C_NOTICE_NPCTASKLIST ) 
		CASE_NEW_MSG( CMessageNpcTaskTipNotice,                 ID_S2C_NOTICE_NPCTASKTIP )  
		CASE_NEW_MSG( CMessagePlayerSearchRequest,              ID_C2S_REQUEST_TASK_SEARCH )  
		CASE_NEW_MSG( CMessagePlayerSearchResponse,             ID_S2C_NOTICE_TASK_SEARCH )  
		CASE_NEW_MSG( CMessagePlayerInviteTaskRequest,          ID_C2S_INVITE_TASK_REQUEST	)		
		CASE_NEW_MSG( CMessagePlayerInviteTaskNotice,           ID_S2C_INVITE_TASK_NOTICE	)		
		CASE_NEW_MSG( CMessagePlayerInviteTaskResponseRequest,  ID_C2S_INVITE_TASK_RESPONSE_REQUEST)
		CASE_NEW_MSG( CMessagePlayerInviteTaskResponseNotice,   ID_S2C_INVITE_TASK_RESPONSE_NOTICE )
		CASE_NEW_MSG( CMessageDoPlayerInviteTaskMsg,            ID_S2S_DOPLAYER_INVITE_TASK_MSG		)
		CASE_NEW_MSG( CMessagePlayerInviteTaskErrorMsg,         ID_S2C_PLAYER_INVITE_TASK_ERROR_MSG )
		CASE_NEW_MSG( CMessageAbortTaskNotice,					ID_S2C_NOTICE_ABORT_TASK )
		CASE_NEW_MSG( CMessageTaskFailNotice,					ID_S2C_NOTICE_TASKFAIL )
		CASE_NEW_MSG( CMessageTaskOprFailNotice,				ID_S2C_NOTICE_TASKOPRFAIL )
		CASE_NEW_MSG( CMessageNpcStorageTaskListNotice,			ID_S2C_NOTICE_NPCSTORAGETASKLIST )
		CASE_NEW_MSG( CMessageObtainScrollTaskRequest,			ID_C2S_REQUEST_OBTAINSCROLLTASK )
		CASE_NEW_MSG( CMessageTragedyTaskUpdateNotice,			ID_S2C_NOTICE_TRAGEDYTASKUPDATE )
		CASE_NEW_MSG( CMessageGetTipRequest,					ID_C2S_REQUEST_GETTIP )
		CASE_NEW_MSG( CMessageEndTalkRequest,					ID_C2S_REQUEST_ENDTALK )
		CASE_NEW_MSG( CMessageNpcTalkStateChangeNotice,			ID_S2C_NOTICE_NPCTALKCHANGE )
		CASE_NEW_MSG( CMessageLockBaggageRequest,				ID_C2S_REQUEST_LOCKBAG )
		CASE_NEW_MSG( CMessageLockBaggageResponse,				ID_S2C_LOCKBAG_RESPONSE )
		CASE_NEW_MSG( CMessageUpdateTaskConditions,				ID_S2C_UPDATE_CONDITIONS )
		CASE_NEW_MSG( CMessageClearTaskNotify,					ID_S2C_CLEARTASKNOTIFY )
		CASE_NEW_MSG( CMessageTakeOffItemRequest,				ID_C2S_REQUEST_TAKEOFFITEM )
		CASE_NEW_MSG( CMessageGetFamilyTaskRequest,				ID_C2S_REQUEST_GETFAMILYTASK	 )
		CASE_NEW_MSG( CMessageGetFamilyTaskResponse,			ID_S2C_RESPONSE_GETFAMILYTASK )
		CASE_NEW_MSG( CMessageGetCorpsTaskRequest,				ID_C2S_REQUEST_GETCORPSTASK	 )
		CASE_NEW_MSG( CMessageGetCorpsTaskResponse,				ID_S2C_RESPONSE_GETCORPSTASK )
		CASE_NEW_MSG( CMessageGetCountryTaskRequest,			ID_C2S_REQUEST_GETCOUNTRYTASK )
		CASE_NEW_MSG( CMessageGetCountryTaskResponse,			ID_S2C_RESPONSE_GETCOUNTRYTASK )
		CASE_NEW_MSG( CMessageUnFinishTaskTime,					ID_S2C_NOTIFY_UNFINISHTIMEERR )
		CASE_NEW_MSG( CMessageTaskRefreshRequest,				ID_C2S_REQUEST_REFRESHTASK )
		CASE_NEW_MSG( CMessageTaskRefreshFailNotice,			ID_S2C_NOTICE_TASKREFRESHFAIL )
		CASE_NEW_MSG( CMessageTaskUnRefreshNotice,				ID_S2C_NOTICE_TASKUNREFRESHED )
		CASE_NEW_MSG( CMessageTaskRefreshResult,				ID_S2C_NOTICE_TASKREFRESHRESULT )
		CASE_NEW_MSG( CMessageRmoveTaskColor,					ID_S2C_NOTICE_REMOVETASKCOLOR )
		CASE_NEW_MSG( CMessageUnReaccpetTask,					ID_S2C_NOTICE_TASKUNREACCEPT )
					
		
		CASE_NEW_MSG( CMessageCorpsLoginBattleRequest,			ID_C2S_REQUEST_LOGINBATTLE )
		CASE_NEW_MSG( CMessageCorpsLoginBattleResponse,			ID_S2C_RESPONSE_LOGINBATTLE )

		CASE_NEW_MSG( CMessageGetCampTaskRequest,				ID_C2S_REQUEST_GETCAMPTASK )
		CASE_NEW_MSG( CMessageGetCampTaskResponse,				ID_S2C_RESPONSE_GETCAMPTASK )

		CASE_NEW_MSG( CMessageGetChairManTaskRequest,			ID_C2S_REQUEST_GETCHAIRMANTASK	)
		CASE_NEW_MSG( CMessageGetChairManTaskResponse,			ID_S2C_RESPONSE_GETCHAIRMANTASK )
		CASE_NEW_MSG( CMessageGetTeamLoopErrNotify,				ID_S2C_NOTIFY_GETTEAMLOOPERR )
		

		// friend message
		CASE_NEW_MSG( CMessageInviteFriendRequest,	            ID_C2S_FRIEND_INVITE_REQUEST             ) 
		CASE_NEW_MSG( CMessageInviteFriendNotice,               ID_S2C_FRIEND_INVITE_NOTICE_R            )    
		CASE_NEW_MSG( CMessageInviteFriendOptionRequest,        ID_C2S_FRIEND_INVITE_OPTION              )  
		CASE_NEW_MSG( CMessageInviteFriendResponseMsg,          ID_S2S_FRIEND_INVITE_RESPONSE_R          )    
		CASE_NEW_MSG( CMessageInviteFriendResponseNotice,       ID_S2C_FRIEND_INVITE_RESPONSE_NOTICE     )     
		CASE_NEW_MSG( CMessageDelFrientRequest,                 ID_C2S_FRIEND_DEL_REQUEST                )      
		CASE_NEW_MSG( CMessageFriendErrorNotice,                ID_S2C_FRIEND_ERRCODE_NOTICE			 ) 
		CASE_NEW_MSG( CMessageChangeFriendNotice,               ID_S2C_FRIEND_CHANGE_FRIEND_NOTICE		 )  
		CASE_NEW_MSG( CMessageFriendGetListNotice,              ID_S2C_FRIEND_GETLIST_NOTICE			 )    
		CASE_NEW_MSG( CMessageFriendGetListRequest,             ID_C2S_FRIEND_GETLIST_REQUEST			 ) 
		CASE_NEW_MSG( CMessageFriendGroupNameNotice,            ID_S2C_FRIEND_GROUPNAME_NOTICE			 )  
		CASE_NEW_MSG( CMessageFriendChangeStatusNotice_R,       ID_S2S_FRIEND_CHANGESTATUS_NOTICE_R		 )  
		CASE_NEW_MSG( CMessageFriendChangeStatusRequest,        ID_C2S_FRIEND_CHANGESTATUS_REQUEST		 )  
		CASE_NEW_MSG( CMessageFriendChangeStatusNotice,         ID_S2C_FRIEND_CHANGESTATUS_NOTICE		 )		

		CASE_NEW_MSG( CMessageAddFriendNotice,		            ID_S2S_FRIEND_ADDFRIEND_NOTICE			 )		
		CASE_NEW_MSG( CMessageFriendChgGroupNameRequest,        ID_C2S_FRIEND_CHGGROUPNAME_REQUEST		 )		
		CASE_NEW_MSG( CMessageFriendChgFriendGroupRequest,      ID_C2S_FRIEND_CHGFRIENDGROUP_REQUEST	 )		
		CASE_NEW_MSG( CMessageFriendSendMessageRequest,         ID_C2S_FRIEND_SENDMESSAGE_REQUEST		 )		
		CASE_NEW_MSG( CMessageFriendMessageNotice,			    ID_S2C_FRIEND_MESSAGE_NOTICE_R			 )		
		CASE_NEW_MSG( CMessageFriendMoveToBlackRequest,         ID_C2S_FRIEND_MOVETOBLACK_REQUEST		 )		
		CASE_NEW_MSG( CMessageFriendAddBlackRequest,            ID_C2S_FRIEND_ADDBLACK_REQUEST			 )		
		CASE_NEW_MSG( CMessageFriendChgFriendGroupNotice,       ID_S2C_FRIEND_CHGFRIENDGROUP_NOTICE		 )		
		CASE_NEW_MSG( CMessageFriendMoveToBlackNotice,          ID_S2C_FRIEND_MOVETOBLACK_NOTICE		 )		
		CASE_NEW_MSG( CMessageFriendLockEnemyRequest,           ID_C2S_FRIEND_LOCKENEMY_REQUEST		     )		
		CASE_NEW_MSG( CMessageFriendGetFriendsStateRequest,     ID_C2S_FRIEND_GETFRIENDSSTATE_REQUEST	 )		
		CASE_NEW_MSG( CMessageFriendGetFriendsStateNotice,      ID_S2C_FRIEND_GETFRIENDSSTATE_NOTICE	 )		
		CASE_NEW_MSG( CMessageFriendUpdateEnemyRequest,         ID_C2S_FRIEND_UPDATEENEMY_REQUEST		 )		
		CASE_NEW_MSG( CMessageInviteFriendRequestByName,        ID_C2S_FRIEND_ADDFRIENDBYNAME_REQUEST	 )		
		CASE_NEW_MSG( CMessageFriendLevelUpNotify,				ID_S2C_NOTIFY_FRIENDLEVELUP )
		CASE_NEW_MSG( CMessageFriendBlessRequest,				ID_C2S_REQUEST_FRIENDBLESS )
		CASE_NEW_MSG( CMessageFriendBlessResponse,				ID_S2C_RESPONSE_FRIENDBLESS )
		CASE_NEW_MSG( CMessageFriendBlessNotify,				ID_S2S_NOTIFY_FRIENDBLESS )
		CASE_NEW_MSG( CMessageModifyIntimacyNogify,				ID_S2S_NOTIFY_MODIFYINTIMACY )		
		CASE_NEW_MSG( CMessageModifyLastTimeAllOnLineNotify,	ID_S2S_NOTIFY_MODIFYLASTTIMEALLONLINE )		

		CASE_NEW_MSG( CMessageCreateSwornTeamRequest,			ID_C2S_REQUEST_CREATESWORNTEAM )
		CASE_NEW_MSG( CMessageCreateSwornTeamResponse,			ID_S2C_RESPONSE_CREATESWORNTEAM )
		CASE_NEW_MSG( CMessageLeaveSwornTeamRequest,			ID_C2S_REQUEST_LEAVESWORNTEAM )
		CASE_NEW_MSG( CMessageLeaveSwornTeamResponse,			ID_S2C_RESPONSE_LEAVESWORNTEAM )
		CASE_NEW_MSG( CMessageKickSwornMemberRequest,			ID_C2S_REQUEST_KICKSWORNMEMBER )
		CASE_NEW_MSG( CMessageKickSwornMemberResponse,			ID_S2C_RESPONSE_KICKSWORNMEMBER )
		CASE_NEW_MSG( CMessageDisbandSwornTeamRequest,			ID_C2S_REQUEST_DISBANDSWORNTEAM )
		CASE_NEW_MSG( CMessageDisbandSwornTeamResponse,			ID_S2C_RESPONSE_DISBANDSWORNTEAM )
		CASE_NEW_MSG( CMessageSwornInfoNotify,					ID_S2C_NOTIFY_SWORNINFO )
		CASE_NEW_MSG( CMessageCreateSwornTeamNotify,			ID_S2C_NOTIFY_CREATESWORNTEAM )
		CASE_NEW_MSG( CMessageLeaveSwornTeamNotify,				ID_S2C_NOTIFY_LEAVESWORNTEAM )
		CASE_NEW_MSG( CMessageKickSwornMemberNotify,			ID_S2C_NOTIFY_KICKSWORNMEMBER )	
		CASE_NEW_MSG( CMessageChangeSwornNameRequest,			ID_C2S_REQUEST_CHANGESWORNNAME )
		CASE_NEW_MSG( CMessageChangeSwornNameResponse,			ID_S2C_RESPONSE_CHANGESWORNNAME )
		CASE_NEW_MSG( CMessageChangeSwornNameNotify,			ID_S2C_NOTIFY_CHANGESWORNNAME )
		CASE_NEW_MSG( CMessageDisbandSwornTeamNotify,			ID_S2C_NOTIFY_DISBANDSWORNTEAM )
		CASE_NEW_MSG( CMessageLastContacterNotice,				ID_S2C_NOTIFY_LASTCONTACTER)
		CASE_NEW_MSG( CMessageUpdateLastContacterNotice,		ID_S2G_NOTIFY_UPDATELASTCONTACTER)
		CASE_NEW_MSG( CMessageLatestContactChangeStatusNotice,	ID_S2C_NOTIFY_CONTACTERSTATUS)
		CASE_NEW_MSG( CMessageUploadContacterList,				ID_S2G_NOTIFY_UPLOADCONTACTERLIST)
		CASE_NEW_MSG( CMessageContacterStatusNotifyG2S,			ID_G2S_NOTIFY_CONTACTERSTATUS)
		CASE_NEW_MSG( CMessageRebuildSwornRequest,				ID_C2S_REQUEST_REBUILDSWORN	)
		CASE_NEW_MSG( CMessageRebuildSwornResponse,				ID_S2C_RESPONSE_REBUILDSWORN )					
		CASE_NEW_MSG( CMessageRebuildSwornNotify,				ID_S2C_NOTIFY_REBUILDSWORN )

		// chat message
	    CASE_NEW_MSG( CMessageChatSend ,						ID_C2S_CMESSCHATSEND    )
		CASE_NEW_MSG( CMessageChatSendCallBack,					ID_S2C_CHATSENDCALLBACK )
		CASE_NEW_MSG( CMessageRouterChatMsg ,					ID_S2C_ROUTERCHATMESSAGE)
		CASE_NEW_MSG( CMessageChatResult,						ID_S2C_CHATRES2CLIENT   )
		CASE_NEW_MSG( CAutoChatMessage,                         ID_C2S_NOTIFY_AUTOCHATMESSAGE)
		CASE_NEW_MSG( CStopAutoChatMessage,                     ID_C2S_NOTIFY_STOPAUTOCHAT)
		CASE_NEW_MSG( CMessageUseItem,                          ID_C2S_NOTIFY_USECHATITEM)
		CASE_NEW_MSG( CMesssageClickBugleItem,                  ID_C2S_NOTIFY_CLICKBUGLEITEM)
        CASE_NEW_MSG( CMessageCheckDirtyWord,                   ID_C2S_REQUEST_CHECKDIRTYWORD)
        CASE_NEW_MSG( CMessageHasDirtyWord,                     ID_S2C_NOTIFY_HASDIRTYWORD)
		// team message
		CASE_NEW_MSG( CMessageCreateTeamSelf,					ID_C2S_REQUEST_CREATETEAM )
		CASE_NEW_MSG( CMessageTeamInvite,						ID_C2S_REQUEST_INVITE )
		CASE_NEW_MSG( CMessageTeamInviteRtn,					ID_C2S_REQUEST_INVITERTN )
		CASE_NEW_MSG( CMessageTeamInvitedRequestCallBack,		ID_C2S_REQUEST_INVITEDREQUEST )
		CASE_NEW_MSG( CMessageTeamSearch,						ID_C2S_REQUEST_SEARCH )
		CASE_NEW_MSG( CMessageTeamRequest,						ID_C2S_REQUEST_ASKFORJOIN )
		CASE_NEW_MSG( CMessageTeamRequestRtn,					ID_C2S_REQUEST_APPLYRTN )
		CASE_NEW_MSG( CMessageTeamLeave,						ID_C2S_REQUEST_LEAVE )
		CASE_NEW_MSG( CMessageTeamNewHead,						ID_C2S_REQUEST_NEWHEAD )
		CASE_NEW_MSG( CMessageTeamTick,							ID_C2S_REQUEST_TICK )
		CASE_NEW_MSG( CMessageTeamEnd,							ID_C2S_REQUEST_ENDTEAM )
		CASE_NEW_MSG( CMessageTeamRequestPlayerCallBack,		ID_S2C_REQUEST_PROPOSERRTN )
		CASE_NEW_MSG( CMessageTeamNotifyCallBack,				ID_S2C_NOTIFY_SYNITEMLIST )
		CASE_NEW_MSG( CMessageTeamCallBack,						ID_S2C_NOTIFY_ITEMLIST )
		CASE_NEW_MSG( CMessageTeamEndCallBack,					ID_S2C_NOTIFY_ENDTEAM )
		CASE_NEW_MSG( CMessageTeamInviteAskCallBack,			ID_S2C_NOTIFY_ASKJOINRTN )
		CASE_NEW_MSG( CMessageTeamTickCallBack,					ID_S2C_NOTIFY_TICKRTN )
		CASE_NEW_MSG( CMessageTeamErrMsgCallBack,				ID_S2C_NOTIFY_ERRORMSG )
		CASE_NEW_MSG( CMessageTeamAskTeamMsg,					ID_C2S_NOTIFY_ASKTEAMMSG )
		CASE_NEW_MSG( CMessageTeamAskTeamCallBack,				ID_S2C_NOTIFY_ASKTEAMTRN )
		CASE_NEW_MSG( CMessageTeamConscribeMsg,					ID_C2S_NOTIFY_CONSCRIBEMSG )
		CASE_NEW_MSG( CMessageTeamConscribeCallBack,			ID_S2C_NOTIFY_CONSCRIBERTN )
		CASE_NEW_MSG( CMessageTeamPropertyNotifyCallBack,		ID_S2C_NOTIFY_SYNPROPERTY )
		CASE_NEW_MSG( CMessageTeamChangeDistribute,				ID_C2S_REQUEST_CHANGEDISTRIBUTE )
		CASE_NEW_MSG( CMessageTeamChangeDistributeCallBack,		ID_S2C_NOTIFY_CHANGEDISTRIBUTERTN )
		CASE_NEW_MSG( CMessageTeamNotifyBuffCallBack,			ID_S2C_NOTIFY_SYNBUFF )
		CASE_NEW_MSG( CMessageTeamBuffCallBack,					ID_C2S_REQUEST_BUFFRTN )
		CASE_NEW_MSG( CMessageTeamClearRequestList,				ID_C2S_REQUEST_CLEARREQUESTLIST )
		CASE_NEW_MSG( CMessageTeamClearInviteList,				ID_C2S_REQUEST_CLEARINVITELIST )
		CASE_NEW_MSG( CMessageTeamClearRequestCallBack,			ID_S2C_NOTIFY_CLEARREQUESTCALLBACK )
		CASE_NEW_MSG( CMessageTeamClearInviteCallBack,			ID_S2C_NOTIFY_CLEARINVITECALLBACK )
		CASE_NEW_MSG( CMessageTeamSearchMsg,					ID_C2S_REQUEST_SEARCHMSG )
		CASE_NEW_MSG( CMessageTeamSearchMsgCallBack,			ID_S2C_NOTIFY_SEARCHMSGCALLBACK )
		CASE_NEW_MSG( CMessageTeamSearchConscribeMsg,			ID_C2S_REQUEST_SRHCONSCRIBEMSG )
		CASE_NEW_MSG( CMessageTeamSearchConscribeCallBack,		ID_S2C_NOTIFY_SRHCONSCRIBECALLBACK )
		CASE_NEW_MSG( CMessageTeamDeleteMsg,					ID_C2S_REQUEST_DELMSG )
		CASE_NEW_MSG( CMessageTeamDeleteMsgCallBack,			ID_S2C_NOTIFY_DELMSGCALLBACK )
		CASE_NEW_MSG( CMessageTeamOrderFollow,					ID_C2S_REQUEST_ORDERFOLLOW )
		CASE_NEW_MSG( CMessageTeamOrderFollowCallBack,			ID_S2C_NOTIFY_ORDERFOLLOWRTN )
		CASE_NEW_MSG( CMessageTeamRequestFollow,				ID_C2S_REQUEST_REQUESTFOLLOW )
		CASE_NEW_MSG( CMessageTeamRequestFollowCallBack,		ID_S2C_NOTIFY_REQUESTFOLLOWRTN )
		CASE_NEW_MSG( CMessageTeamTwoParamCallBack,				ID_S2C_NOTIFY_2PARAMNOTIFY )
		CASE_NEW_MSG( CMessageTeamStateChange,					ID_S2C_NOTIFY_CHANGETEAMSTATE )
		CASE_NEW_MSG( CMessageRedirectJoinTeamResponse,			ID_S2S_RESPONSE_REDIRECTJOINTEAM )
		CASE_NEW_MSG( CMessageRedirectTeamRelationNotice,		ID_S2S_NOTIFY_REDIRECTRELATION )
		CASE_NEW_MSG( CMessageRedirectCreateTeamNotice,			ID_S2S_NOTIFY_REDIRECTCREATETEAM )
		CASE_NEW_MSG( CMessageTeamNotice,						ID_S2C_NOTIFY_TEAMNOTICE )

		// map message

		CASE_NEW_MSG( CMessageDestroyEntityNotice,				ID_S2C_NOTICE_DESTROYENTITY )
		CASE_NEW_MSG( CMessageYourMapDataNotice,				ID_S2C_NOTICE_YOURMAPDATA )
		CASE_NEW_MSG( CMessageTeleportNotice,					ID_S2C_NOTICE_TELEPORT )
		CASE_NEW_MSG( CMessagePlayerPathRequest,				ID_C2S_REQUEST_PLAYERPATH )
		CASE_NEW_MSG( CMessagePlayerStopRequest,				ID_C2S_REQUEST_PLAYERSTOP )
		CASE_NEW_MSG( CMessageEntityPathNotice,					ID_C2S_NOTICE_ENTITYPATH )
		CASE_NEW_MSG( CMessageEntityPosReSetCallBack,			ID_S2C_NOTICE_ENTITYPOSRESET )
		CASE_NEW_MSG( CMessagePlayerCanntMoveCallBack,			ID_S2C_NOTICE_CANNTMOVE )
		CASE_NEW_MSG( CMessageEntityStopNotice,					ID_S2C_NOTICE_ENTITYSTOP )
		CASE_NEW_MSG( CMessagePetPathRequest,					ID_C2S_REQUEST_PETPATH )
		CASE_NEW_MSG( CMessagePetStopRequest,					ID_C2S_REQUEST_PETSTOP )


	 // mail message 
        CASE_NEW_MSG( CMessageOpenMailBoxRequest,				ID_C2S_REQUEST_OPENMAILBOX )
		CASE_NEW_MSG( CMessageOpenMailBoxResponse,				ID_S2C_RESPONSE_OPENMAILBOX )
		CASE_NEW_MSG( CMessageOpenSendMailBox,					ID_C2S_REQUEST_OPENSENDMAILBOX	)			
		CASE_NEW_MSG( CMessageOpenReadMailBox,					ID_C2S_REQUEST_OPENREADMAILBOX)		
		CASE_NEW_MSG( CMessageMailErrorCode,	                ID_S2C_NOTIFY_MAILERRORCODE)		
		CASE_NEW_MSG( CMessageMailChangeItem,					ID_C2S_REQUEST_CHANGEITEM)
		CASE_NEW_MSG( CMessageMailChangeItemCallBack,			ID_S2C_RESPONSE_CHANGEITEM )
		CASE_NEW_MSG( CMessageMailCanSend,						ID_C2S_REQUEST_MAILCANSEND)		
		CASE_NEW_MSG( CMessageMailCanSendCallBack,				ID_S2C_RESPONSE_MAILCANSEND)
		CASE_NEW_MSG( CMessageMailSend,							ID_C2S_REQUEST_MAILSEND)
		CASE_NEW_MSG( CMessageMailListNotify,					ID_S2C_NOTIFY_MAILLISTINFO)
		CASE_NEW_MSG( CMessageShutReadMailBoxRequest,			ID_C2S_REQUEST_SHUTREADMAILBOX )
		CASE_NEW_MSG( CMessageShutSendMailBoxRequest,			ID_C2S_REQUEST_SHUTSENDMAILBOX )
		CASE_NEW_MSG( CMessageNewMailNotify	,					ID_S2C_NOTIFY_NEWMAIL )
		CASE_NEW_MSG( CMessageMailRead	  ,						ID_C2S_REQUEST_READMAIL )
		CASE_NEW_MSG( CMessageMailReadResponse,					ID_S2C_RESPONSE_READMAIL )
		CASE_NEW_MSG( CMessageGetAccessory	  ,					ID_C2S_REQUEST_GETACCESSORY )
		CASE_NEW_MSG( CMessageDelMail	  , 					ID_C2S_REQUEST_DELMAIL )
		CASE_NEW_MSG( CMessageReadMailEnd  ,					ID_C2S_NOTIFY_READMAILEND )
		CASE_NEW_MSG( CMessageBackMail 	  , 					ID_C2S_REQUEST_BACKMAIL )
		CASE_NEW_MSG( CMessageLockMail	  ,						ID_C2S_REQUEST_LOCKMAIL )
		CASE_NEW_MSG( CMessageUnLockMail	  ,					ID_C2S_REQUEST_UNLOCKMAIL  )
		CASE_NEW_MSG( CMessageGetMailPageRequest,				ID_C2S_REQUEST_GETMAILPAGE )
		CASE_NEW_MSG( CMessageDelMailResponse,					ID_S2C_RESPONSE_DELMAIL )
		CASE_NEW_MSG( CMessageGetAccessoryResponse,				ID_S2C_RESPONSE_GETACCESSORY )
		CASE_NEW_MSG( CMessageBackMailResponse,					ID_S2C_RESPONSE_BACKMAIL )
		CASE_NEW_MSG( CMessageShutDownMailBoxRequest,			ID_S2C_REQUEST_SHUTMAILBOX )
		CASE_NEW_MSG( CMessageMailBoxStatusNotify,				ID_S2C_NOTIFY_MAILBOXSTATUS )
		
		CASE_NEW_MSG( CMessageAddKitBagRequest,					ID_C2S_REQUEST_ADDKITBAG )
		CASE_NEW_MSG( CMessageAddKitBagResponse,				ID_S2C_RESPONSE_ADDKITBAG )
		
		CASE_NEW_MSG( CMessageOpenKitBagRequest,				ID_C2S_REQUEST_OPENKITBAG )
		CASE_NEW_MSG( CMessageOpenKitBagResponse,				ID_S2C_RESPONSE_OPENKITBAG )
		
		CASE_NEW_MSG( CMessagePayMailMoneyRequest,				ID_C2S_REQUEST_PAYMAILMONEY )
		CASE_NEW_MSG( CMessagePayMailMoneyResponse,				ID_S2C_RESPONSE_PAYMAILMONEY )
					   
		// trade message
		CASE_NEW_MSG( CMessageTradeInvite,						ID_C2S_REQUEST_TRADEINVITE )
		CASE_NEW_MSG( CMessageTradeInviteCallBack,				ID_S2C_NOTIFY_INVITERTN )
		CASE_NEW_MSG( CMessageTradeInviteReply,					ID_C2S_REQUEST_INVITEREPLY )
		CASE_NEW_MSG( CMessageNotifyTradeBegin,					ID_S2C_NOTIFY_TRADEBEGIN )
		CASE_NEW_MSG( CMessageSendGoodsInfo,					ID_C2S_REQUEST_SENDGOODSINFO )
		CASE_NEW_MSG( CMessageNotifyGoodsInfo,					ID_S2C_NOTIFY_GOODSINFO )
		CASE_NEW_MSG( CMessageSendTradeMoney,					ID_C2S_REQUEST_SENDTRADEMONEY )
		CASE_NEW_MSG( CMessageNotifyTradeMoney,					ID_S2C_NOTIFY_TRADEMONEY )
		CASE_NEW_MSG( CMessageLockTradeInfo,					ID_C2S_REQUEST_LOCKTRADEINFO )
		CASE_NEW_MSG( CMessageNotifyTradeInfoLock,				ID_S2C_NOTIFY_TRADEINFOLOCK )
		CASE_NEW_MSG( CMessageCloseTrade,						ID_C2S_REQUEST_CLOSETRADE )
		CASE_NEW_MSG( CMessageNotifyTradeClose,					ID_S2C_NOTIFY_TRADECLOSE )
		CASE_NEW_MSG( CMessageNotifyResultCode,					ID_S2C_NOTIFY_RESULTCODE )
		CASE_NEW_MSG( CMessageSubmitTrade,						ID_C2S_REQUEST_SUBMITTRADE )

		// stall message
		CASE_NEW_MSG( CMessageOpenStallRequest,					ID_C2S_REQUEST_OPENSTALL )
		CASE_NEW_MSG( CMessageOpenStallResponse,				ID_S2C_RESPONSE_OPENSTALL )
		CASE_NEW_MSG( CMessageProcessReadResult,				ID_C2S_REQUEST_READPROCESS )
		CASE_NEW_MSG( CMessageStartStallNotify,					ID_S2C_NOTIFY_STARTSTALL )
		CASE_NEW_MSG( CMessageUpdateStallRequest,				ID_C2S_REQUEST_UPDATEGOODS )
		CASE_NEW_MSG( CMessageUpdateStallResponse,				ID_S2C_RESPONSE_UPDATEGOODS )
		CASE_NEW_MSG( CMessageUpdatePriceRequest,				ID_C2S_REQUEST_UPDATEPRICE )
		CASE_NEW_MSG( CMessageUpdatePriceResponse,				ID_S2C_RESPONSE_UPDATEPRICE )
		CASE_NEW_MSG( CMessageClickStallRequest,				ID_C2S_REQUEST_CLICKSTALL )
		CASE_NEW_MSG( CMessageClickStallResponse,				ID_S2C_RESPONSE_CLICKSTALL )
		CASE_NEW_MSG( CMessageBuyGoodsRequest,					ID_C2S_REQUEST_BUYGOODS )
		CASE_NEW_MSG( CMessageBuyGoodsResponse,					ID_S2C_RESPONSE_BUYGOODS )
		CASE_NEW_MSG( CMessageCloseStallRequest,				ID_C2S_REQUEST_CLOSESTALL )
		CASE_NEW_MSG( CMessageCloseStallResponse,				ID_S2C_RESPONSE_CLOSESTALL )
		CASE_NEW_MSG( CMessageCloseStallDlgRequest,				ID_C2S_REQUEST_CLOSEDIALOG )
		CASE_NEW_MSG( CMessageUpdateTextRequest,				ID_C2S_REQUEST_UPDATETEXT )
		CASE_NEW_MSG( CMessageUpdateTextResponse,				ID_S2C_RESPONSE_UPDATETEXT )
		CASE_NEW_MSG( CMessageStallGetRecordRequest,			ID_C2S_REQUEST_STALLRECORD )
		CASE_NEW_MSG( CMessageStallGetRecordResponse,			ID_S2C_RESPONSE_STALLRECORD )
		CASE_NEW_MSG( CMessageStallCleanRecordRequest,			ID_C2S_REQUEST_CLEANRECORD )
		CASE_NEW_MSG( CMessageStallCleanRecordResponse,			ID_S2C_RESPONSE_CLEANRECORD )
		CASE_NEW_MSG( CMessageLockPasswordRequest,				ID_C2S_REQUEST_LOCKPASSWORD )		
		CASE_NEW_MSG( CMessageLockPasswordResponse,				ID_S2C_RESPONSE_LOCKPASSWORD )	
		
		// repetion message
		CASE_NEW_MSG( CMessageRepetionTeamIn,					ID_S2S_REPETION_TEAM_IN )
		CASE_NEW_MSG( CMessageRepetionErrCode,					ID_S2C_REPETION_ERR )
		CASE_NEW_MSG( CMessageRepetionEventNotic,				ID_S2C_REPETION_EVENT )	 
		CASE_NEW_MSG( CMessageS2CBattleJoinNotice,				ID_S2C_BATTLE_JOIN_NOTICE )
		CASE_NEW_MSG( CMessageC2SBattleJoin,					ID_C2S_BATTLE_JOIN )
		CASE_NEW_MSG( CMessageBattleWaitResponse,				ID_S2C_BATTLE_WAIT_RESPONSE )
		CASE_NEW_MSG( CMessageBattleTeamWaitRequest,			ID_C2S_BATTLE_TEAM_WAIT_REQUEST )
		CASE_NEW_MSG( CMessageC2SBattleWaitLeaveRequest,		ID_C2S_BATTLE_WAIT_LEAVE_REQUEST )
		CASE_NEW_MSG( CMessageS2CBattleWaitLeaveResponse,		ID_S2C_BATTLE_WAIT_LEAVE_RESPONSE )
		CASE_NEW_MSG( CMessageC2SRepetionLeave,					ID_C2S_REPETION_LEAVE )
		CASE_NEW_MSG( CMessageRepetionIndexNotice,				ID_S2C_REPETIONINDEX_NOTICE )
		CASE_NEW_MSG( CMessageBourseAccountNotify,				ID_S2C_BOURSE_NOTIFY_ACCOUNT )
		CASE_NEW_MSG( CMessageBourseIncAccount,					ID_C2S_BOURSE_INC_ACCOUNT )
		CASE_NEW_MSG( CMessageBourseDecAccount,					ID_C2S_BOURSE_DEC_ACCOUNT )
		CASE_NEW_MSG( CMessageBourseAccountLoad,				ID_C2S_BOURSE_LOAD_ACCOUNT )
		CASE_NEW_MSG( CMessageBourseAccountAddBuy,				ID_C2S_BOURSE_ADD_BUY )
		CASE_NEW_MSG( CMessageBourseAccountAddSell,				ID_C2S_BOURSE_ADD_SELL )
		CASE_NEW_MSG( CMessageBourseLoadSelfList,				ID_C2S_BOURSE_LOAD_SELF_LIST )
		CASE_NEW_MSG( CMessageBourseSelfListNotice,				ID_S2C_BOURSE_NOTIFY_SELF_LIST )
		CASE_NEW_MSG( CMessageBourseListNotice,					ID_S2C_BOURSE_LIST_NOTICE )
		CASE_NEW_MSG( CMessageBourseLoadList,					ID_C2S_BOURSE_LOAD_LIST )
		CASE_NEW_MSG( CMessageBourseCode,						ID_S2C_BOURSE_CODE )
		CASE_NEW_MSG( CMessageBourseTradeBuyNotice,				ID_S2C_BOURSE_TRADE_BUY )
		CASE_NEW_MSG( CMessageBourseTradeSellNotice,			ID_S2C_BOURSE_TRADE_SELL )
		CASE_NEW_MSG( CMessageBourseDecBuyDoc,					ID_C2S_BOURSE_BUY_DOC )
		CASE_NEW_MSG( CMessageBourseDecSellDoc,					ID_C2S_BOURSE_SELL_DOC )
		CASE_NEW_MSG( CMessageBourseTradeLogQuery,				ID_C2S_BOURSE_TRADE_LOG_QUERY )
		CASE_NEW_MSG( CMessageBourseTradeLog,					ID_S2C_BOURSE_TRADE_LOG )
		CASE_NEW_MSG( CMessageC2SBattleJoinRandomPlace,			 ID_C2S_BATTLE_JOIN_RANDOM_PLACE)

		CASE_NEW_MSG( CMessageRepetionUseItemIntoNotice,		ID_S2C_USEITEM_INTO_NOTICE )
		CASE_NEW_MSG( CMessageRepetionUseItemIntoResponse,		ID_C2S_USEITEM_INTO_RESPINSE )
		CASE_NEW_MSG( CMessageRepetionFuncPlayerClick,			ID_C2S_REPETION_FUNC )

		CASE_NEW_MSG( CMessageBattleTeamFixWaitRequest,		ID_C2S_BATTLE_TEAM_FIX_WAIT_REQUEST)
		CASE_NEW_MSG( CMessageBattleSingleFixWaitRequest,		ID_C2S_BATTLE_SINGLE_FIX_WAIT_REQUEST)
		CASE_NEW_MSG( CMessageHaveSignUpBattle,		ID_S2C_HAVE_SIGN_UP_BATTLE)
		CASE_NEW_MSG( CMessageBattleOnTimeOpenNotice,			ID_S2C_BATTLE_ON_TIME_OPEN )
		CASE_NEW_MSG( CMessageC2SJoinOnTimeOpenBattleRequest,	ID_C2S_JOIN_ON_TIME_OPEN_BATTLE_REQUEST)
		CASE_NEW_MSG( CMessageC2SGetRandomTeleChargeRequest,	ID_C2S_GET_RANDOM_TELE_CHARGE_REQUEST)
		CASE_NEW_MSG( CMessageS2CGetRandomTeleChargeResponse,	ID_S2C_GET_RANDOM_TELE_CHARGE_RESPONSE)
		CASE_NEW_MSG( CMessageC2SRrandomTeleRequest,			ID_C2S_RANDOM_TELE_REQUEST)

		CASE_NEW_MSG( CMessageApplyGlobalWarRequest, ID_C2S_REQUEST_APPLYGLOBALWAR )
		CASE_NEW_MSG( CMessageApplyGlobalWarResponse,ID_S2C_RESPONSE_APPLYGLOBALWAR )
		CASE_NEW_MSG( CMessageEnterGlobalWarRequest, ID_C2S_REQUEST_ENTERGLOBALWAR )
		CASE_NEW_MSG( CMessageEnterGlobalWarResponse, ID_S2C_RESPONSE_ENTERGLOBALWAR )
		CASE_NEW_MSG( CMessageCanEnterGlobalWarNotify, ID_S2C_NOTIFY_CANENTERGLOBALWAR )
		CASE_NEW_MSG( CMessagePlayerLeaveGlobalWarRequest, ID_C2S_REQUEST_LEAVEGLOBALWAR )
		CASE_NEW_MSG( CMessagePlayerLeaveGlobalWarResponse, ID_S2C_RESPONSE_LEAVEGLOBALWAR )

		CASE_NEW_MSG( CMessageKickCampPlayerRequest, ID_C2S_REQUEST_KICKCAMPPLAYER )
		CASE_NEW_MSG( CMessageKickCampPlayerResponse,  ID_S2C_RESPONSE_KICKCAMPPLAYER )
		CASE_NEW_MSG( CMessageKickCampPlayerNotify, ID_S2C_NOTIFY_KICKCAMPPLAYER )
		CASE_NEW_MSG( CMessageVoteToKickPlayerRequest, ID_C2S_REQUEST_VOTETOKICKPLAYER )
		CASE_NEW_MSG( CMessageVoteToKickPlayerResponse, ID_S2C_RESPONSE_VOTETOKICKPLAYER )
		CASE_NEW_MSG( CMesssageKickResultNoitfy,  ID_S2C_NOTIFY_KICKRESULT )
		CASE_NEW_MSG( CMessageIntoCorpsBossRepetionRequest, ID_C2S_INTO_CORPSBOSSREPETION_REQUEST)
		CASE_NEW_MSG( CMessageEnterLastReptionRequest, ID_C2S_REQUEST_ENTERLASTREPETION )
		CASE_NEW_MSG( CMessageEnterLastReptionResponse, ID_S2C_RESPONSE_ENTERLASTREPETION )
		// activity message 
		CASE_NEW_MSG( CMessageCheckAnswerRequest,				ID_C2S_REQUEST_CHECKANSWER	 )
		CASE_NEW_MSG( CMessageCheckAnswerResponse,				ID_S2C_RESPONSE_CHECKANSER )
		CASE_NEW_MSG( CMessageQuestionTimeEnd,					ID_C2S_NOTIFY_QUESTIONTIMEREND )
		CASE_NEW_MSG( CMessageQuestionNew,						ID_S2C_NOTIFY_QUESTIONNEW )
		CASE_NEW_MSG( CMessageAnswerEnd,						ID_S2C_NOTIFY_ENDANSWER )
		CASE_NEW_MSG( CMessageUseQuestionPaper,					ID_C2S_NOTIFY_USEQUESTIONPAPER  )
		CASE_NEW_MSG( CMessageIfAnswerNow,						ID_C2S_NOTIFY_IFANSWERNOW )
		CASE_NEW_MSG( CMessageIfBeginQuest,						ID_C2S_NOTIFY_IFBEGINQUEST	 )
		CASE_NEW_MSG( CMessageStopAnser,						ID_C2S_NOTIFY_STOPANSWER )	
		CASE_NEW_MSG( CMessageUsePaperResponse,					ID_S2C_RESPONSE_USEQUESIONPAPER )
		CASE_NEW_MSG( CMessageInvestQuestionNotify,				ID_S2C_NOIIFY_INVESTQUESTION )	
		CASE_NEW_MSG( CMessageInvestAnswerNotify,				ID_C2S_NOTIFY_INVESTANSWER )
		
		// family message
		CASE_NEW_MSG( CMessageCreatFamilyRequest,				ID_C2S_REQUEST_CREATFAMILY )
		CASE_NEW_MSG( CMessageCreatFamilyResponse,				ID_S2C_RESPONSE_CREATFAMILY )
		CASE_NEW_MSG( CMessageInviteMemberRequest,				ID_C2S_REQUEST_INVITEMEMBER )
		CASE_NEW_MSG( CMessageInviteMemberResponse,				ID_S2C_RESPONSE_INVITEMEMBER )
		CASE_NEW_MSG( CMessageFamilyInvitedNotify,				ID_S2C_NOTIY_FAMILYINVITED )
		CASE_NEW_MSG( CMessageReplyFamilyInviteRequest,			ID_C2S_REQUEST_REPLYFAMILYINVITE )
		CASE_NEW_MSG( CMessageReplyFamilyInviteResponse,		ID_S2C_RESPONSE_REPLYFAMILYINVITE )				
		CASE_NEW_MSG( CMessageInviteFamilyResultNotify,			ID_S2C_NOTIFY_INVITERESULT )
		
		CASE_NEW_MSG( CMessageApplyFamilyRequest,				ID_C2S_REQUEST_FAMILYAPPLY )
		CASE_NEW_MSG( CMessageApplyFamilyResponse,				ID_S2C_RESPONSE_FAMILYAPPLY )
		CASE_NEW_MSG( CMessageMemberApplyNotify,				ID_S2C_NOTIFY_MEMBERAPPLY )
		CASE_NEW_MSG( CMessageReplyMemberApplyRequest,		    ID_C2S_REQUEST_REPLYMEMBERAPPLY )
		CASE_NEW_MSG( CMessageReplyMemberApplyResponse,			ID_S2C_RESPONSE_REPLYMEMBERAPPLY )
		CASE_NEW_MSG( CMessageApplyFamilySuccessNotify,			ID_S2C_NOTIFY_FAMILYAPPLYSUCESS )
		
		CASE_NEW_MSG( CMessageFamilyMemberAddedNotify,			ID_S2C_NOTIFY_FAMILYMEMBERADDED )
		
		CASE_NEW_MSG( CMessageGetFamilyGoodsListRequest,		ID_C2S_REQUEST_GETFAMILYGOODSLIST )
		CASE_NEW_MSG( CMessageGetFamilyGoodsListResponse,		ID_S2C_RESPONSE_GETFAMILYGOODSLIST )
		CASE_NEW_MSG( CMessageGetFamilyExchangeGoodsListRequest,ID_C2S_REQUEST_GETFAMILYEXCHANGEGOODSLIST )
		CASE_NEW_MSG( CMessageGetFamilyExchangeGoodsListResponse,ID_S2C_RESPONSE_GETFAMILYEXCHANGEGOODSLIST )
		CASE_NEW_MSG( CMessageChangeFamilyGoodsListNotify,		ID_S2C_NOTIFY_CHANGEFAMILYGOODSLIST )
		CASE_NEW_MSG( CMessageChangeFamilyExchangeGoodsListNotify, ID_S2C_NOTIFY_CHANGEFAMILYEXCHANGEGOODSLIST )
		
		CASE_NEW_MSG( CMessagePostAppointRequest,				ID_C2S_REQUEST_POSTAPPOINT )
		CASE_NEW_MSG( CMessagePostAppointResponse,				ID_S2C_RESPONSE_POSTAPPOINT )
		CASE_NEW_MSG( CMessagePostAppointNoify,					ID_S2C_NOTIFY_POSTAPPOINT )
		CASE_NEW_MSG( CMessageLeaveFamilyRequest,				ID_C2S_REQUEST_LEAVEFAMILY )
		CASE_NEW_MSG( CMessageLeaveFamilyResponse,				ID_S2C_RESPONSE_LEAVEFAMILY )
		CASE_NEW_MSG( CMessageMemberLeaveFamilyNotify,			ID_S2C_NOTIFY_MEMBERLEAVEFAMILY )
		CASE_NEW_MSG( CMessageGetFamilyMemberInfoRequest,		ID_C2S_REQUEST_GETFAMILYMEMBERINFO )
		CASE_NEW_MSG( CMessageGetFamilyMemberInfoResponse,		ID_S2C_RESPONSE_GETFAMILYMEMBERINFO )
		CASE_NEW_MSG( CMessageGetFamilyPropertyRequest,			ID_C2S_REQUEST_GETFAMILYPROPERTY )
		CASE_NEW_MSG( CMessageGetFamilyPropertyResponse,		ID_S2C_RESPONSE_FAMILYPROPERTY )
		CASE_NEW_MSG( CMessageGetAllFanmilyInfoRequest,			ID_C2S_REQUEST_GETALLFAMILYINFO )
		CASE_NEW_MSG( CMessageGetAllFamilyInfoResponse,			ID_S2C_RESPONSE_GETALLFAMILYINFO )		
		CASE_NEW_MSG( CMessageGetApplyListRequest,				ID_C2S_REQUEST_GETAPPLYLIST )
		CASE_NEW_MSG( CMessageGetApplyListResponse,				ID_S2C_RESPONSE_GETAPPLYLIST ) 
		CASE_NEW_MSG( CMessageKickPlayerRequest,				ID_C2S_REQUEST_KICKMEMBER )
		CASE_NEW_MSG( CMessageKickPlayerResponse,				ID_S2C_RESPONSE_KICKMEMBER )
		CASE_NEW_MSG( CMessageKickPlayerNotify,					ID_S2C_NOTIFY_KICKPLAYER )
		CASE_NEW_MSG( CMessageDisbandFamilyRequest,				ID_C2S_REQUEST_DISBANDFAMILY )
		CASE_NEW_MSG( CMessageDisbandFamilyResponse,			ID_S2C_RESPONSE_DISBANDFAMILY )
		CASE_NEW_MSG( CMessageDisbandFamilyNotify,				ID_S2C_NOTIFY_DISBANDFAMILY )
		CASE_NEW_MSG( CMessageChangeFamilyNoticeRequest,		ID_C2S_REQUEST_CHANGENOTICE )
		CASE_NEW_MSG( CMessageChangeFamilyNoticeResponse,		ID_S2C_RESPONSE_CHANGENOTICE )
		CASE_NEW_MSG( CMessageAppointHeaderRequest,				ID_C2S_REQUEST_APPOINTHEADER )
		CASE_NEW_MSG( CMessageAppointHeaderResponse,			ID_S2C_RESPONSE_APPOINTHEADER )
		CASE_NEW_MSG( CMessageChangeHeaderNotify,				ID_S2C_NOTIFY_CHANGERHEADER )
		CASE_NEW_MSG( CMessageFamilyPropertyNotify,				ID_S2C_NOTIFY_FAMILYPEROPERTY )	
		CASE_NEW_MSG( CMessageClearFamilyApplyRequest,			ID_C2S_REQUEST_CLEARFAMILYAPPLYLIST )		
		CASE_NEW_MSG( CMessageClearFamilyApplyResponse,			ID_S2C_RESPONSE_CLEARFAMILYAPPLYLIST )
		CASE_NEW_MSG( CMessageJoinCorpsNotify,					ID_S2C_NOTIFY_JOINCORPS )
		CASE_NEW_MSG( CMessageCancelFamilyDisbandRequest,		ID_C2S_REQUEST_CANCELFAMILYDISBAND )
		CASE_NEW_MSG( CMessageCancelFamilyDisbandResponse,		ID_S2C_RESPONSE_CANCELFAMILYDISBAND )
		CASE_NEW_MSG( CMessageChangeFamilyNoticeNotify,			ID_S2C_NOTIFY_CHANGEFAMILYNOTICE )
		CASE_NEW_MSG( CMessageMendShengHuoTaiRequest,			ID_C2S_REQUEST_MENDSHENGHUOTAI )
		CASE_NEW_MSG( CMessageCorpsPropertyNotice,				ID_S2C_NOTIFY_CORPSPROPERTY )
		CASE_NEW_MSG( CMessageCorpsChallegeRequest,				ID_C2S_REQUEST_CHALLEGE )
		CASE_NEW_MSG( CMessageCorpsChallegeResponse,			ID_S2C_RESPONSE_CHALLEGE )
		CASE_NEW_MSG( CMessageCorpsOperateRoomRequest,			ID_C2S_REQUEST_ROOMOPERATION )
		CASE_NEW_MSG( CMessageCorpsOperateRoomResponse,			ID_S2C_RESPONSE_ROOMOPERATION )
		CASE_NEW_MSG( CMessageCorpsRoomNotice,					ID_S2C_NOTIFY_ROOM )
		CASE_NEW_MSG( CMessageCorpsChallegeNotice,				ID_S2C_NOTIFY_CHALLEGE )
		CASE_NEW_MSG( CMessageCorpsReplyChallegeRequest,		ID_C2S_REQUEST_REPLYCHALLEGE )
		CASE_NEW_MSG( CMessageCorpsReplyChallegeResponse,		ID_S2C_RESPONSE_REPLYCHALLEGE )
		CASE_NEW_MSG( CMessageCorpsQueryRoomListRequest,		ID_C2S_REQUEST_QUERYROOM)
		CASE_NEW_MSG( CMessageCorpsQueryRoomListResponse,		ID_S2C_RESPONSE_QUERYROOM )
		CASE_NEW_MSG( CMessageCorpsQueryChallegeListRequest,	ID_C2S_REQUEST_QUERYCHALLEGELIST )
		CASE_NEW_MSG( CMessageCorpsQueryChallegeListResponse,	ID_S2C_RESPONSE_QUERYCHALLEGELIST )
		CASE_NEW_MSG( CMessageCorpsQueryRecChallegeListRequest,	ID_C2S_REQUEST_QUERYRECCHALLEGE )
		CASE_NEW_MSG( CMessageCorpsQueryRecChallegeListResponse,ID_S2C_RESPONSE_QUERYRECCHALLEGE )
		CASE_NEW_MSG( CMessageCorpsAbortBattleCityRequest,		ID_C2S_REQUEST_ABORTBATTLECITY )
		CASE_NEW_MSG( CMessageCorpsAbortBattleCityResponse,		ID_S2C_RESPONSE_ABORTBATTLECITY )
		CASE_NEW_MSG( CMessageCorpsAbortBattleCityNotice,		ID_S2C_NOTIFY_ABORTBATTLECITY )
		CASE_NEW_MSG( CMessageCorpsNpcOwnerNotice,				ID_S2C_NOTIFY_NPCOWNERUPDATE )
		CASE_NEW_MSG( CMessageGetCorpsGoodsListRequest,			ID_C2S_REQUEST_GETCORPSGOODSLIST )
		CASE_NEW_MSG( CMessageGetCorpsGoodsListResponse,		ID_S2C_RESPONSE_GETCORPSGOODSLIST )
		CASE_NEW_MSG( CMessageGetCorpsExchangeGoodsListRequest,	ID_C2S_REQUEST_GETCORPSEXCHANGEGOODSLIST )
		CASE_NEW_MSG( CMessageGetCorpsExchangeGoodsListResponse,ID_S2C_RESPONSE_GETCORPSEXCHANGEGOODSLIST )				
		CASE_NEW_MSG( CMessageChangeCorpsGoodsListNotify,		ID_S2C_NOTIFY_CHANGECORPSGOODSLIST )
		CASE_NEW_MSG( CMessageChangeCorpsExchangeGoodsListNotify,	ID_S2C_NOTIFY_CHANGEXCHANGEECORPSGOODSLIST )
		CASE_NEW_MSG( CMessageFamilyStabilityChangedNotify,		ID_S2C_NOTIFY_FAMILYSTABILITYCHANGED )

		CASE_NEW_MSG( CMessageAddSlotRequest,					ID_C2S_REQUEST_ADDSLOT )
		CASE_NEW_MSG( CMessageAddSlotResponse,					ID_S2C_RESPONSE_ADDSLOT )
		CASE_NEW_MSG( CMessageRemoveJewelRequest,				ID_C2S_REQUEST_REMOVEJEWEL )
		CASE_NEW_MSG( CMessageRemoveJewelResponse,				ID_S2C_RESPONSE_REMOVEJEWEL )
		CASE_NEW_MSG( CMessageJudgeRequest,						ID_C2S_REQUEST_JUDGE )
		CASE_NEW_MSG( CMessageJudgeResponse,					ID_S2C_RESPONSE_JUDGE )
		CASE_NEW_MSG( CMessagePushAddSlotRequest,				ID_C2S_REQUEST_PUSHADDSLOT )
		CASE_NEW_MSG( CMessagePushAddSlotResponse,				ID_S2C_RESPONSE_PUSHADDSLOT )
		CASE_NEW_MSG( CMessagePopAddSlotRequest,				ID_C2S_REQUEST_POPADDSLOT )
		CASE_NEW_MSG( CMessagePopAddSlotResponse,				ID_S2C_RESPONSE_POPADDSLOT )
		CASE_NEW_MSG( CMessagePushRemoveJewelRequest,			ID_C2S_REQUEST_PUSHREMOVEJEWEL )
		CASE_NEW_MSG( CMessagePushRemoveJewelResponse,			ID_S2C_RESPONSE_PUSHREMOVEJEWEL )
		CASE_NEW_MSG( CMessagePopRemoveJewelRequest,			ID_C2S_REQUEST_POPREMOVEJEWEL )
		CASE_NEW_MSG( CMessagePopRemoveJewelResponse,			ID_S2C_RESPONSE_POPREMOVEJEWEL )
		CASE_NEW_MSG( CMessagePushJudgeRequest,					ID_C2S_REQUEST_PUSHJUDGE )
		CASE_NEW_MSG( CMessagePushJudgeResponse,				ID_S2C_RESPONSE_PUSHJUDGE )
		CASE_NEW_MSG( CMessagePopJudgeRequest,					ID_C2S_REQUEST_POPJUDGE )
		CASE_NEW_MSG( CMessagePopJudgeResponse,					ID_S2C_RESPONSE_POPJUDGE )
		CASE_NEW_MSG( CMessagePushChangeRequest,				ID_C2S_REQUEST_PUSHCHANGE )
		CASE_NEW_MSG( CMessagePushChangeResponse,				ID_S2C_RESPONSE_PUSHCHANGE )
		CASE_NEW_MSG( CMessagePopChangeRequest,					ID_C2S_REQUEST_POPCHANGE )
		CASE_NEW_MSG( CMessagePopChangeResponse,				ID_S2C_RESPONSE_POPCHANGE )
		CASE_NEW_MSG( CMessageChangeRequest,					ID_C2S_REQUEST_CHANGE )
		CASE_NEW_MSG( CMessageChangeResponse,					ID_S2C_RESPONSE_CHANGE )
		CASE_NEW_MSG( CMessagePushJewComRequest,				ID_C2S_REQUEST_PUSHJEWCOM )
		CASE_NEW_MSG( CMessagePushJewComResponse,				ID_S2C_RESPONSE_PUSHJEWCOM )
		CASE_NEW_MSG( CMessagePopJewComRequest,					ID_C2S_REQUEST_POPJEWCOM )
		CASE_NEW_MSG( CMessagePopJewComResponse,				ID_S2C_RESPONSE_POPJEWCOM )
		CASE_NEW_MSG( CMessageJewComRequest,					ID_C2S_REQUEST_JEWCOM )
		CASE_NEW_MSG( CMessageJewComResponse,					ID_S2C_RESPONSE_JEWCOM )
		CASE_NEW_MSG( CMessageMultiComRequest,					ID_C2S_REQUEST_MULTICOM )
		CASE_NEW_MSG( CMessageMultiComResponse,					ID_S2C_RESPONSE_MULTICOM )
		CASE_NEW_MSG( CMessageSyncFameNotify,					ID_S2C_NOTIFY_SYNCFAME )

		CASE_NEW_MSG( CMessageListActivityMenuRequest,			ID_C2S_REQUEST_LISTACTIVITYMENU )
		CASE_NEW_MSG( CMessageListActivityItemRequest,			ID_C2S_REQUEST_LISTACTIVITYITEM )
		CASE_NEW_MSG( CMessageMinusActivityItemRequest,			ID_C2S_REQUEST_MINUSACTIVITYITEM )
		CASE_NEW_MSG( CMessageMinusActivityItemResponse,		ID_S2C_RESPONSE_MINUSACTIVITYITEM )

		CASE_NEW_MSG( CMessageListActivityMenuResponse,			ID_S2C_RESPONSE_LISTACTIVITYMENU )
		CASE_NEW_MSG( CMessageListActivityItemResponse,			ID_S2C_RESPONSE_LISTACTIVITYITEM )

		CASE_NEW_MSG( CMessageRefreshFamilyInfoRequest,			ID_C2S_REQUEST_REFRESHFAMILYINFO )
		CASE_NEW_MSG( CMessageRefreshFamilyInfoResponse,		ID_S2C_RESPONSE_REFRESHFAMILYINFO )
		
		CASE_NEW_MSG( CMessageSetFamilyPostRequest,				ID_C2S_REQUEST_SETFAMILYMEMBERPOST ) 
		CASE_NEW_MSG( CMessageSetFamilyPostResponse,			ID_S2C_RESPONSE_SETFAMILYMEMBERPOST )

		CASE_NEW_MSG( CMessageLevelUPFamilyRequest,				ID_C2S_REQUEST_LEVELUPFAMILY )
		CASE_NEW_MSG( CMessageLevelUPFamilyResponse,			ID_S2C_RESPONSE_LEVELUPFAMILY )

		CASE_NEW_MSG( CMessageContributeFamilyMoneyRequest,		ID_C2S_REQUEST_CONTRIBUTEFAMILYMONEY )
		CASE_NEW_MSG( CMessageContributeFamilyMoneyResponse,	ID_S2C_RESPONSE_CONTRIBUTEFAMILYMONEY )

		CASE_NEW_MSG( CMessageObtainFamilyMoneyRequest,			ID_C2S_REQUEST_OBTAINFAMILYMONEY )
		CASE_NEW_MSG( CMessageObtainFamilyMoneyResponse,		ID_S2C_RESPONSE_OBTAINFAMILYMONEY )	

		CASE_NEW_MSG( CMessageFamilyBidNpcRequest,				ID_C2S_REQUEST_BIDNPC )
		CASE_NEW_MSG( CMessageFamilyBidNpcResponse,				ID_S2C_RESPONSE_BIDNPC )
		CASE_NEW_MSG( CMessageMemberSubmitNotify,				ID_S2C_NOTIFY_MEMBERSUBMIT )
		CASE_NEW_MSG( CMessageMemberSubmitRequest,				ID_C2S_REQUEST_MEMBERSUBMIT )
		CASE_NEW_MSG( CMessageMemberSubmitResponse,				ID_S2C_RESPONSE_MEMBERSUBMIT )
		CASE_NEW_MSG( CMessageAbortNpcRequest,					ID_C2S_REQUEST_ABORTNPC )
		CASE_NEW_MSG( CMessageAbortNpcResponse,					ID_S2C_RESPONSE_ABORTNPC )
		CASE_NEW_MSG( CMessageBidInfoRequest,					ID_C2S_REQUEST_BIDINFO )
		CASE_NEW_MSG( CMessageBidInfoResponse,					ID_S2C_RESPONSE_BIDINFO )
		CASE_NEW_MSG( CMessageFamilySubmitNotify,				ID_S2C_NOTIFY_FAMILYSUBMIT )
		CASE_NEW_MSG( CMessageBattleResultNotify,				ID_S2C_NOTIFY_BATTLERESULT )		

		CASE_NEW_MSG( CMessageMakeRoomRequest,					ID_C2S_REQUEST_MAKEROOM )
		CASE_NEW_MSG( CMessageMakeRoomResponse,					ID_S2C_RESPONSE_MAKEROOM )
		CASE_NEW_MSG( CMessageChaRoomRequest,					ID_C2S_REQUEST_CHAROOM )
		CASE_NEW_MSG( CMessageChaRoomResponse,					ID_S2C_RESPONSE_CHAROOM )
		CASE_NEW_MSG( CMessageAgreeBattleRequest,				ID_C2S_REQUEST_AGREEBATTLE )
		CASE_NEW_MSG( CMessageAgreeBattleResponse,				ID_S2C_RESPONSE_AGREEBATTLE )
		CASE_NEW_MSG( CMessageQueryAllRoomRequest,				ID_C2S_REQUEST_QUERYALLROOM )
		CASE_NEW_MSG( CMessageQueryAllRoomResponse,				ID_S2C_RESPONSE_QUERYALLROOM )
		CASE_NEW_MSG( CMessageQueryMyRoomRequest,				ID_C2S_REQUEST_QUERYMYROOM )
		CASE_NEW_MSG( CMessageQueryMyRoomResponse,				ID_S2C_RESPONSE_QUERYMYROOM )
		CASE_NEW_MSG( CMessageQueryMyChaRequest,				ID_C2S_REQUEST_QUERYMYCHA )
		CASE_NEW_MSG( CMessageQueryMyChaResponse,				ID_S2C_RESPONSE_QUERYMYCHA )
		CASE_NEW_MSG( CMessageAbortRoomRequest,					ID_C2S_REQUEST_ABORTROOM )
		CASE_NEW_MSG( CMessageAbortRoomResponse,				ID_S2C_RESPONSE_ABORTROOM )
		CASE_NEW_MSG( CMessageAbortChaRequest,					ID_C2S_REQUEST_ABORTCHA )
		CASE_NEW_MSG( CMessageAbortChaResponse,					ID_S2C_RESPONSE_ABORTCHA )

		CASE_NEW_MSG( CMessageMakeRoomNotify,					ID_S2C_NOTIFY_MAKEROOM )
		CASE_NEW_MSG( CMessageChaRoomNotify,					ID_S2C_NOTIFY_CHAROOM )
		CASE_NEW_MSG( CMessageAbortRoomNotify,					ID_S2C_NOTIFY_ABORTROOM )
		CASE_NEW_MSG( CMessageAbortChaNotify,					ID_S2C_NOTIFY_ABORTCHA ) 		
	
		CASE_NEW_MSG( CMessageContributeFamilyMoneyNotify,		ID_S2C_NOTIFY_CONTRIBUTEFAMILYMONEY )
		
		CASE_NEW_MSG( CMessageFamilyMoneyChangedNotify,			ID_S2C_NOTIFY_FAMILYMONEYCHANGED )	

		CASE_NEW_MSG( CMessageUseFamilyExpCardNotify,			ID_S2C_NOTIFY_USEFAMILYEXPCARD )		
		CASE_NEW_MSG( CMessageFamilyGloryChangedNotify,			ID_S2C_NOTIFY_FAMILYGLORYCHANGED )
		
		CASE_NEW_MSG( CMessageStartFamilyBossRequest,			ID_C2S_REQUEST_STARTFAMILYBOSS )
		CASE_NEW_MSG( CMessageStartFamilyBossResponse,			ID_S2C_RESPONSE_STARTFAMILYBOSS )
		CASE_NEW_MSG( CMessageEnterBossReptionRequest,			ID_C2S_REQUEST_ENTERBOSSREPTION )
		CASE_NEW_MSG( CMessageEnterBossReptionResponse,			ID_S2C_RESPONSE_ENTERBOSSREPTION )
				
		CASE_NEW_MSG( CMessageMemberContributeChangedNotify,	ID_S2C_NOTIFY_MEMBERCONTRIBUTECHANGED )
		CASE_NEW_MSG( CMessageFamilyPVPStarLevelUPNotify,		ID_S2C_NOTIFY_FAMILYPVPSTARLEVELUP )
		
		CASE_NEW_MSG( CMessageQueryNpcWelfareRequest,			ID_C2S_REQUEST_QUERYNPCWELFARE )	
		CASE_NEW_MSG( CMessageQueryNpcWelfareResponse,			ID_S2C_RESPONSE_QUERYNPCWELFARE )
		
		CASE_NEW_MSG( CMessageGetFamilyMoneyRequest,			ID_C2S_REQUEST_GETFAMILYMONEY )
		CASE_NEW_MSG( CMessageGetFamilyMoneyResponse,			ID_S2C_RESPONSE_GETFAMILYMONEY )

		CASE_NEW_MSG( CMessageAbortNpcNotify,					ID_S2C_NOTIFY_ABORTNPC )
		CASE_NEW_MSG( CMessagePreBattleNotify,					ID_S2C_NOTIFY_PREBATTLE )
		CASE_NEW_MSG( CMessageLevelUPFamilyNotify,				ID_S2C_NOTIFY_LEVELUPFAMILY )	
		CASE_NEW_MSG( CMessageFamilyRepetionScoreNotify,		ID_S2C_NOTIFY_FAMILYREPETIONSCORE )		
		
		CASE_NEW_MSG( CMessageStartCountryRedstoneNotify,		ID_S2C_NOTIFY_STARTCOUNTRYREDSTONE )				
		CASE_NEW_MSG( CMessageUseFamilyLeagueItemNotify,			ID_S2C_NOTIFY_USEFAMILYLEAGUEITEM )

		CASE_NEW_MSG( CMessageGetHomeInfoRequest,			ID_C2S_REQUEST_GETHOMEINFO )
		CASE_NEW_MSG( CMessageGetHomeInfoResponse,			ID_S2C_RESPONSE_GETHOMEINFO )
		CASE_NEW_MSG( CMessageUpgradeHomeRequest,			ID_C2S_REQUEST_UPGRADEHOME )
		CASE_NEW_MSG( CMessageUpgradeHomeResponse,			ID_S2C_RESPONSE_UPGRADEHOME )
		
		// corps
		CASE_NEW_MSG( CMessageCreatCorpsRequest ,				ID_C2S_REQUEST_CREATCORPS )
		CASE_NEW_MSG( CMessageCreatCorpsResponse,				ID_S2C_RESPONSE_CREATCORPS )

		CASE_NEW_MSG( CMessageApplyCorpsRequest,				ID_C2S_REQUEST_APPLYCORPS )
		CASE_NEW_MSG( CMessageApplyCorpsResponse,				ID_S2C_RESPONSE_APPLYCORPS )
		CASE_NEW_MSG( CMessageCorpsMemberApplyNotify,			ID_S2C_NOTIFY_CORPSMEMBERAPPLY )
		CASE_NEW_MSG( CMessageReplyCorpsMemberApplyRequest,		ID_C2S_REQUEST_REPLYCORPSMEMBERAPPLY )
		CASE_NEW_MSG( CMessageReplyCorpsMemberApplyResponse,	ID_S2C_RESPONSE_REPLYCORPSMEMBERAPPLY )
		CASE_NEW_MSG( CMessageApplyCorpsResultNotify,			ID_S2C_NOTIFY_APPLYCORPSRESULT )

		CASE_NEW_MSG( CMessageInviteCorpsMemberRequest,			ID_C2S_REQUEST_INVITECORPSMEMBER )
		CASE_NEW_MSG( CMessageInviteCorpsMemberResponse,		ID_S2C_RESPONSE_INVITECORPSMEMBER )
		CASE_NEW_MSG( CMessageCorpsInviteNotify,				ID_S2C_NOTIFY_CORPSINVITE )
		CASE_NEW_MSG( CMessageReplyCorpsInviteRequest,			ID_C2S_REQUEST_REPLYCORPSINVITE	)
		CASE_NEW_MSG( CMessageReplyCorpsInviteResponse,			ID_S2C_RESPONSE_REPLYCORPSINVITE )
		CASE_NEW_MSG( CMessageCorpsInviteResultNotify,			ID_S2C_NOTIFY_CORPSINVITERESULT	)

		CASE_NEW_MSG( CMessageFamilyJoinCorpsNotify,			ID_S2C_NOTIFY_FAMILYJOINCORPS )

		CASE_NEW_MSG( CMessageFamilyLeaveCorpsRequest,			ID_C2S_REQUEST_LEAVECORPS )
		CASE_NEW_MSG( CMessageFamilyLeaveCorpsResponse,			ID_S2C_RESPONSE_LEAVECORPS )

		CASE_NEW_MSG( CMessageFamilyLeaveCorpsNotify,			ID_S2C_NOTIFY_FAMILYLEAVED )

		CASE_NEW_MSG( CMessageKickFamilyRequest,				ID_C2S_REQUEST_KICKFAMILY )
		CASE_NEW_MSG( CMessageKickFamilyResponse,				ID_S2C_RESPONSE_KICKFAMILY )

		CASE_NEW_MSG( CMessageKickFamilyNotify,					ID_S2C_NOTIFY_FAMILYKICKED )


		CASE_NEW_MSG( CMessageGetCorpsFamilyInfoRequest,		ID_C2S_REQUEST_GETCORPSFAMILYINFO )
		CASE_NEW_MSG( CMessageGetCorpsFamilyInfoResponse,		ID_S2C_RESPONSE_GETCORPSFAMILYINFO )

		CASE_NEW_MSG( CMessageGetCorpsPropertyRequest,			ID_C2S_REQUEST_GETCORPSPROPERTYINFO )
		CASE_NEW_MSG( CMessageGetCorpsPropertyResponse,			ID_S2C_RESPONSE_GETCORPSPROPERTYINFO )

		CASE_NEW_MSG( CMessageGetCorpsFamilyMemberRequest,		ID_C2S_REQUEST_GETCOPRSFAMILYMEMBER	)
		CASE_NEW_MSG( CMessageGetCorpsFamilyMemberResponse,		ID_S2C_RESPONE_GETCOPRSFAMILYMEMBER	)

		CASE_NEW_MSG( CMessageChangeCorpsBulletinRequest,		ID_C2S_REQUEST_CHANGECORPSBULLETIN )
		CASE_NEW_MSG( CMessageChangeCorpsBulletinResponse,		ID_S2C_RESPONSE_CHANGECORPSBULLETIN	)

		CASE_NEW_MSG( CMessageChangeCorpsBulletinNotify,		ID_S2C_NOTIFY_CHANGECORPSBULLETIN )

		CASE_NEW_MSG( CMessageChangeCorpsHeaderRequest,			ID_C2S_REQUEST_CHANGECORPSHEADER )
		CASE_NEW_MSG( CMessageChangeCorpsHeaderResponse,		ID_S2C_RESPONSE_CHANGECORPSHEADER )

		CASE_NEW_MSG( CMessageChangeCorpsHeaderNotify,			ID_S2C_NOTIFY_CHANGECORPSHEADER	)
		
		CASE_NEW_MSG( CMessageDisbandCorpsRequest,				ID_C2S_REQUEST_DISBANDCORPS )
		CASE_NEW_MSG( CMessageDisbandCorpsResponse,				ID_S2C_RESPONSE_DISBANDCORPS)
		CASE_NEW_MSG( CMessageDisbandCorpsNotify,				ID_S2C_NOTIFY_DISBANDCORPS )
		
		CASE_NEW_MSG( CMessageGetCorpsApplyListRequest,			ID_C2S_REQUEST_GETCORPSAPPLYLIST )
		CASE_NEW_MSG( CMessageGetCorpsApplyListResponse,		ID_S2C_RESPONSE_GETCORPSAPPLYLIST )
		
		CASE_NEW_MSG( CMessageCancelCorpsDisbandRequest,		ID_C2S_REQUEST_CANCELCORPSDISBAND )
		CASE_NEW_MSG( CMessageCancelCorpsDisbandResponse,		ID_S2C_RESPONSE_CANCELCORPSDISBAND )
				
		CASE_NEW_MSG( CMessageRefreshCorpsInfoRequest,			ID_C2S_REQUEST_REFRESHCORPSINFO )
		CASE_NEW_MSG( CMessageRefreshCorpsInfoResponse,			ID_S2C_RESPONSE_REFRESHCORPSINFO )

		CASE_NEW_MSG( CMessageClearCorpsApplyRequest,			ID_C2S_REQUEST_CLEARCORPSAPPLY )
		CASE_NEW_MSG( CMessageClearCorpsApplyResponse,			ID_S2C_RESPONSE_CLEARCORPSAPPLY )
		
		CASE_NEW_MSG( CMessageGetCorpsListRequest,				ID_C2S_REQUEST_GETCOPRSLIST )
		CASE_NEW_MSG( CMessageGetCorpsListResponse,				ID_S2C_RESPONSE_GETCORPSLIST )		

		CASE_NEW_MSG( CMessageSetCorpsPostRequest,				ID_C2S_REQUEST_SETCORPSPOST )
		CASE_NEW_MSG( CMessageSetCorpsPostResponse,				ID_S2C_RESPONSE_SETCORPSPOST ) 

		CASE_NEW_MSG( CMessageLevelUPCorpsRequest,				ID_C2S_REQUEST_LEVELUPCORPS )
		CASE_NEW_MSG( CMessageLevelUPCorpsResponse,			ID_S2C_RESPONSE_LEVELUPCORPS )

		CASE_NEW_MSG( CMessageContributeCorpsMoneyRequest,		ID_C2S_REQUEST_CONTRIBUTECORPSMONEY )
		CASE_NEW_MSG( CMessageContributeCorpsMoneyResponse,		ID_S2C_RESPONSE_CONTRIBUTECORPSMONEY )

		CASE_NEW_MSG( CMessageFamilyContributeMoneyRequest,		ID_C2S_REQUEST_FAMILYCONTRIBUTEMONEY )
		CASE_NEW_MSG( CMessageFamilyContributeMoneyResponse,	ID_S2C_RESPONSE_FAMILYCONTRIBUTEMONEY )

		CASE_NEW_MSG( CMessageSetMemberNameRequest,				ID_C2S_REQUEST_SETMEMBERNAME )
		CASE_NEW_MSG( CMessageSetMemberNameResponse,			ID_S2C_RESPONSE_SETMEMBERNAME )

		CASE_NEW_MSG( CMessageSetCorpsPostNotify,				ID_S2C_NOTIFY_SETCORPSPOST )

		CASE_NEW_MSG( CMessageCorpsLevelUPNotify,				ID_S2C_NOTIFY_CORPSLEVELUP )	

		CASE_NEW_MSG( CMessageCorpsBattleBidRequest,					ID_C2S_REQUEST_BATTLEBID )
		CASE_NEW_MSG( CMessageCorpsBattleBidResponse,				ID_S2C_RESPONSE_BATTLEBID )

		CASE_NEW_MSG( CMessageCorpsQueryBidRankRequest,				ID_C2S_REQUEST_QUERYBIDRANK )
		CASE_NEW_MSG( CMessageCorpsQueryBidRankResponse,				ID_S2C_RESPONSE_QUERYBIDRANK )

		CASE_NEW_MSG( CMessageCorpsSelectFamilyBattleRequest,	ID_C2S_REQUEST_SELECTFAMILY )
		CASE_NEW_MSG( CMessageCorpsSelectFamilyBattleResponse,	ID_S2C_RESPONSE_SELECTFAMILY )

		CASE_NEW_MSG( CMessageCorpsSelectMemberBattleRequest,	ID_C2S_REQUEST_SELECTBATTLE )
		CASE_NEW_MSG( CMessageCorpsSelectMemberBattleResponse,	ID_S2C_RESPONSE_SELECTBATTLE )

		CASE_NEW_MSG( CMessageCorpsBattleRightsNotify,			ID_S2C_NOTIFY_CORPSBATTLERIGHTS )
		CASE_NEW_MSG( CMessageCorpsRecBattleRightsNotify,		ID_S2C_NOTIFY_RECBATTLERIGHTS )
		CASE_NEW_MSG( CMessageCorpsMemberBattleRightsNotify,	ID_S2C_NOTIFY_MEMBERBATTLERIGHTS )
		
		CASE_NEW_MSG( CMessageCorpsMoneyChangedNotify,			ID_S2C_NOTIFY_CORPSMONEYCHANGED )
		CASE_NEW_MSG( CMessageCorpsGloryChangedNotify,			ID_S2C_NOTIFY_CORPSGLORYCHANGED )		
		CASE_NEW_MSG( CMessageCorpsContributeChangedNotify,		ID_S2C_NOTIFY_CORPSCONTRIBUTECHANGED )

		CASE_NEW_MSG( CMessageCorpsBattleResultNotice,			ID_S2C_NOTIFY_CORPSBATTLERESULT )
		
		
		CASE_NEW_MSG( CMessageQueryCorpsWelfareRequest,			ID_C2S_REQUEST_QUERYCORPSWELFARE )
		CASE_NEW_MSG( CMessageQueryCorpsWelfareResponse,		ID_S2C_RESPONSE_QUERYCORPSWELFARE )

		CASE_NEW_MSG( CMessageObtainCorpsWelfareRequest,		ID_C2S_REQUEST_OBTAINCORPSWELFARE )
		CASE_NEW_MSG( CMessageObtainCorpsWelfareResponse,		ID_S2C_RESPONSE_OBTAINCORPSWELFARE )

		CASE_NEW_MSG( CMessageGetCorpsMoneyRequest,				ID_C2S_REQUEST_GETCORPSMONEY )
		CASE_NEW_MSG( CMessageGetCorpsMoneyResponse,			ID_S2C_RESPONSE_GETCORPSMONEY )
		
		CASE_NEW_MSG( CMessageGetExpCardRequest,				ID_C2S_REQUEST_GETEXPCARD )
		CASE_NEW_MSG( CMessageGetExpCardResponse,				ID_S2C_RESPONSE_GETEXPCARD )
		
		CASE_NEW_MSG( CMessageFamilyCanLevelUPNotify,			ID_S2C_NOTIFY_FAMILYCANLEVELUP )
		CASE_NEW_MSG( CMessageCorpsCanLevelUPNotify,			ID_S2C_NOTIFY_CORPSCANLEVELUP )
		CASE_NEW_MSG( CMessageCorpsPublishTaskNotify,			ID_S2C_NOTIFY_CORPSPUBLISHTASK )
			
		CASE_NEW_MSG( CMessageCorpsNameChangedNotify,			ID_S2C_NOTIFY_CORPSNAMECHANGED )

		CASE_NEW_MSG( CMessageCreateUnionRequest,				ID_C2S_REQUEST_CREATEUNION )
		CASE_NEW_MSG( CMessageCreateUnionResponse,				ID_S2C_RESPONSE_CREATEUNION )
		CASE_NEW_MSG( CMessageCreateUnionNotify,				ID_S2C_NOTIFY_CREATEUNION )

		CASE_NEW_MSG( CMessageApplyUnionRequest,				ID_C2S_REQUEST_APPLYUNION )
		CASE_NEW_MSG( CMessageApplyUnionResponse,				ID_S2C_RESPONSE_APPLYUNION )
		CASE_NEW_MSG( CMessageUnionMemberApplyNotify,			ID_S2C_NOTIFY_UNIONMEMBERAPPLY )
		CASE_NEW_MSG( CMessageReplyUnionMemberApplyRequest,		ID_C2S_REQUEST_REPLYUNIONMEMBERAPPLY )
		CASE_NEW_MSG( CMessageReplyUnionMemberApplyResponse,	ID_S2C_RESPONSE_REPLYUNIONMEMBERAPPLY )
		CASE_NEW_MSG( CMessageApplyUnionResultNotify,			ID_S2C_NOTIFY_APPLYUNIONRESULT )

		CASE_NEW_MSG( CMessageInviteUnionMemberRequest,			ID_C2S_REQUEST_INVITEUNIONMEMBER )
		CASE_NEW_MSG( CMessageInviteUnionMemberResponse,		ID_S2C_RESPONSE_INVITEUNIONMEMBER )
		CASE_NEW_MSG( CMessageUnionInviteNotify,				ID_S2C_NOTIFY_UNIONINVITE )
		CASE_NEW_MSG( CMessageReplyUnionInviteRequest,			ID_C2S_REQUEST_REPLYUNIONINVITE	)
		CASE_NEW_MSG( CMessageReplyUnionInviteResponse,			ID_S2C_RESPONSE_REPLYUNIONINVITE )
		CASE_NEW_MSG( CMessageUnionInviteResultNotify,			ID_S2C_NOTIFY_UNIONINVITERESULT	)

		CASE_NEW_MSG( CMessageCorpsJoinUnionNotify,				ID_S2C_NOTIFY_CORPSJOINUNION )

		CASE_NEW_MSG( CMessageCorpsLeaveUnionRequest,			ID_C2S_REQUEST_LEAVEUNION )
		CASE_NEW_MSG( CMessageCorpsLeaveUnionResponse,			ID_S2C_RESPONSE_LEAVEUNION )

		CASE_NEW_MSG( CMessageCorpsLeaveUnionNotify,			ID_S2C_NOTIFY_CORPSLEAVED )

		CASE_NEW_MSG( CMessageKickCorpsRequest,					ID_C2S_REQUEST_KICKCORPS )
		CASE_NEW_MSG( CMessageKickCorpsResponse,				ID_S2C_RESPONSE_KICKCORPS )

		CASE_NEW_MSG( CMessageKickCorpsNotify,					ID_S2C_NOTIFY_CORPSKICKED )

		CASE_NEW_MSG( CMessageMakeUnionRequest,					ID_C2S_REQUEST_MAKEUNION )
		CASE_NEW_MSG( CMessageMakeUnionResponse,				ID_S2C_RESPONSE_MAKEUNION )
		CASE_NEW_MSG( CMessageMakeUnionNotify,					ID_S2C_NOTIFY_MAKEUNION )

		CASE_NEW_MSG( CMessageGetUnionInfoRequest,				ID_C2S_REQUEST_UNIONINFO )
		CASE_NEW_MSG( CMessageGetUnionInfoResponse,				ID_S2C_RESPONSE_UNIONINFO )

		CASE_NEW_MSG( CMessageDismissUnionRequest,					ID_C2S_REQUEST_DISMISSUNION )
		CASE_NEW_MSG( CMessageDismissUnionResponse,					ID_S2C_RESPONSE_DISMISSUNION )
		CASE_NEW_MSG( CMessageDismissUnionNotify,					ID_S2C_NOTIFY_DISMISSUNION )
		CASE_NEW_MSG( CMessageUnionApplyListRequest,					ID_C2S_REQUEST_UNIONAPPLYLIST )
		CASE_NEW_MSG( CMessageUnionApplyListResponse,					ID_S2C_RESPONSE_UNIONAPPLYLIST )
		CASE_NEW_MSG( CMessageAllUnionListRequest,					ID_C2S_REQUEST_ALLUNIONLIST )
		CASE_NEW_MSG( CMessageAllUnionListResponse,					ID_S2C_RESPONSE_ALLUNIONLIST )

		CASE_NEW_MSG( CMessageCorpsGetOtherMemberRequest,		ID_C2S_REQUEST_GETOTHERMEMBER )
		CASE_NEW_MSG( CMessageCorpsGetOtherMemberResponse,		ID_S2C_RESPONSE_GETOTHERMEMBER )

		CASE_NEW_MSG( CMessageUnionJoinBattleListRequest,			ID_C2S_REQUEST_UNIONSUBMIT )
		CASE_NEW_MSG( CMessageUnionJoinBattleListResponse,		ID_S2C_RESPONSE_UNIONSUBMIT )

		CASE_NEW_MSG( CMessageCorpsGetAllMapRequest,			ID_C2S_REQUEST_GETALLMAP )
		CASE_NEW_MSG( CMessageCorpsGetAllMapResponse,			ID_S2C_RESPONSE_GETALLMAP )

		CASE_NEW_MSG( CMessageCorpsAppointPrinceRequest,			ID_C2S_REQUEST_APPOINTPRINCE )
		CASE_NEW_MSG( CMessageCorspAppointPrinceResponse,			ID_S2C_RESPONSE_APPOINTPRINCE )
		CASE_NEW_MSG( CMessageCorpsAppointPrinceNotice,				ID_S2C_NOTIFY_APPOINTPRINCE )
		CASE_NEW_MSG( CMessageCorpsMoveCapitalRequest,				ID_C2S_REQUEST_MOVECAPITAL )
		CASE_NEW_MSG( CMesssageCorpsMoveCapitalResponse,			ID_S2C_RESPONSE_MOVECAPITAL )
		CASE_NEW_MSG( CMessageCorpsSelectColorRequest,				ID_C2S_REQUEST_SELECTCOLOR )
		CASE_NEW_MSG( CMessageCorpsSelectColorResponse,				ID_S2C_RESPONSE_SELECTCOLOR )

		CASE_NEW_MSG( CMessageChangeUnionNameRequest,			ID_C2S_REQUEST_CHANGEUNIONNAME )
		CASE_NEW_MSG( CMessageChangeUnionNameResponse,			ID_S2C_RESPONSE_CHANGEUNIONNAME )
		CASE_NEW_MSG( CMessageChangeUnionNameNotify,			ID_S2C_NOTIFY_CHANGEUNIONNAME )
		
		CASE_NEW_MSG( CMessageUseCorpsLeagueItemNotify,			ID_S2C_NOTIFY_USECORPSLEAGUEITEM )

		CASE_NEW_MSG( CMessageCorpsBossTimesChangedNotify,		ID_S2C_CORPSBOSSTIMES_CHANGED_NOTIFY )

		CASE_NEW_MSG( CMessageCorpsBossRefineValueNotify,		ID_S2C_CORPSBOSSREFINEVALUE_CHANGED_NOTIFY)
		CASE_NEW_MSG( CMessageSummonCorpsBossRequest ,			ID_C2S_SUMMON_CORPSBOSS_REQUEST)
		CASE_NEW_MSG( CMessageSummonCorpsBossResponse,			ID_S2C_SUMMON_CORPSBOSS_RESPONSE)
		CASE_NEW_MSG( CMessageCorpsBossRepetionOpenNotify,		ID_S2C_CORPSBOSSREPETION_OPEN_NOTIFY)
		CASE_NEW_MSG( CMessageIntoCorpsBossRepetionResponse,	ID_S2C_INTO_CORPSBOSS_REPETION_RESPONSE)
		CASE_NEW_MSG( CMessageCorpsBossRepetionEndNotify,		ID_S2C_CORPSBOSSREPETION_END_NOTIFY)
		
		// judge message
		CASE_NEW_MSG( CMessageOpenJudgeNotice,					ID_S2C_NOTIFY_OPENJUDGE)
		CASE_NEW_MSG( CMessageCloseJudgeNotice,					ID_S2C_NOTIFY_CLOSEJUDGE)

		CASE_NEW_MSG( CMessageModifySingleJudgeRequest,			ID_C2S_REQUEST_MODIFYJUDGESINGLE)
		CASE_NEW_MSG( CMessageModifySingleJudgeResponse,		ID_S2C_RESPONSE_MODIFYJUDGESINGLE)
		CASE_NEW_MSG( CMessageModifyJudgeListRequest,			ID_C2S_REQUEST_MODIFYJUDGELIST)
		CASE_NEW_MSG( CMessageModifyJudgeListResponse,			ID_S2C_RESPONSE_MODIFYJUDGELIST)
		CASE_NEW_MSG( CMessageModifyJudgeNotice,				ID_S2C_NOTIFY_MODIFYJUDGE)

		CASE_NEW_MSG( CMessageQueryJudgeListRequest,			ID_C2S_REQUEST_QUERYJUDGELIST)
		CASE_NEW_MSG( CMessageQueryJudgeListResponse,			ID_S2C_RESPONSE_QUERYJUDGELIST)

		CASE_NEW_MSG( CMessageUpdateMaxJudgeValueNotice,		ID_S2C_NOTIFY_JUDGEMAX)

		CASE_NEW_MSG( CMessageQuerySingleRecordRequest,			ID_C2S_REQUEST_QUERYJUDGE )
		CASE_NEW_MSG( CMessageQuerySingleRecordResponse,		ID_S2C_RESPONSE_QUERYJUDGE )

		// achieve message
		CASE_NEW_MSG( CMessageAchieveReportRequest,				ID_C2S_REQUEST_REPORTACHIEVE )
		CASE_NEW_MSG( CMessageAchieveReportResponse,			ID_S2C_RESPONSE_REPORTACHIEVE )

		CASE_NEW_MSG( CMessageAchieveGetSingleRequest,			ID_C2S_REQUEST_GETSINGLEACHIEVE )
		CASE_NEW_MSG( CMessageAchieveGetSingleResponse,			ID_S2C_RESPONSE_GETSINGLEACHIEVE )

		CASE_NEW_MSG( CMessageAchieveGetAllRequest,				ID_C2S_REQUEST_GETALLACHIEVE )
		CASE_NEW_MSG( CMessageAchieveGetAllResponse,			ID_S2C_RESPONSE_GETALLACHIEVE )

		CASE_NEW_MSG( CMessageAchieveFinishNotice,				ID_S2C_NOTIFY_ACHIEVENOTICE )

		CASE_NEW_MSG( CMessageGetAllHonorRequest,				ID_C2S_REQUEST_GETALLHONOR )
		CASE_NEW_MSG( CMessageGetAllHonorResponse,				ID_S2C_RESPONSE_GETALLHONOR )
		CASE_NEW_MSG( CMessageHonorFinishNotice,				ID_S2C_NOTIFY_HONORFINSH )
		CASE_NEW_MSG( CMessageGetStageBonusRequest,				ID_C2S_REQUEST_GETSTAGEBONUS )
		CASE_NEW_MSG( CMessageGetStageBonusResponse,			ID_S2C_RESPONSE_GETSTAGEBONUS )	

		// countrymessage
		CASE_NEW_MSG( CMessageCountryInfoRequest,				ID_C2S_REQUEST_COUNTRYINFO )
		CASE_NEW_MSG( CMessageCountryInfoResponse,				ID_S2C_RESPONSE_COUNTRYINFO )
		CASE_NEW_MSG( CMessageJoinCountryRequest,				ID_C2S_REQUEST_JOINCOUNTRY )
		CASE_NEW_MSG( CMessageJoinCountryResponse,				ID_S2C_RESPONSE_JOINCOUNTRY )
		CASE_NEW_MSG( CMessageLeaveCountryRequest,				ID_C2S_REQUEST_LEAVECOUNTRY )
		CASE_NEW_MSG( CMessageLeaveCountryResponse,				ID_S2C_RESPONSE_LEAVECOUNTRY )

		CASE_NEW_MSG( CMessageSetOfficialRequest,				ID_C2S_REQUEST_SETOFFICIAL )
		CASE_NEW_MSG( CMessageSetOfficialResponse,				ID_S2C_RESPONSE_SETOFFICIAL )
		CASE_NEW_MSG( CMessageUnSetOfficialRequest,				ID_C2S_REQUEST_UNSETOFFICIAL )
		CASE_NEW_MSG( CMessageUnSetOfficialResponse,			ID_S2C_RESPONSE_UNSETOFFICIAL )
		CASE_NEW_MSG( CMessageUseFunctionRequest,				ID_C2S_REQUEST_USEFUNCTION )
		CASE_NEW_MSG( CMessageUseFunctionResponse,				ID_S2C_RESPONSE_USEFUNCTION )
		CASE_NEW_MSG( CMessageOfficialInfoRequest,				ID_C2S_REQUEST_OFFICIALINFO )
		CASE_NEW_MSG( CMessageOfficialInfoResponse,				ID_S2C_RESPONSE_OFFICIALINFO )
		CASE_NEW_MSG( CMessageUseTimeRequest,					ID_C2S_REQUEST_USETIME )
		CASE_NEW_MSG( CMessageUseTimeResponse,					ID_S2C_RESPONSE_USETIME )
		CASE_NEW_MSG( CMessageWorldInfoRequest,				ID_C2S_REQUEST_WORLDINFO )
		CASE_NEW_MSG( CMessageWorldInfoResponse,			ID_S2C_RESPONSE_WORLDINFO )
		CASE_NEW_MSG( CMessageWorldQuestRequest,			ID_C2S_REQUEST_WORLDQUEST )
		CASE_NEW_MSG( CMessageWorldQuestResponse,			ID_S2C_RESPONSE_WORLDQUEST )
		CASE_NEW_MSG( CMessageWorldAnswerRequest,			ID_C2S_REQUEST_WORLDANSWER )
		CASE_NEW_MSG( CMessageWorldAnswerResponse,			ID_S2C_RESPONSE_WORLDANSWER )
		CASE_NEW_MSG( CMessageWorldQuestNotify,				ID_S2C_NOTIFY_WORLDQUEST )
		CASE_NEW_MSG( CMessageWorldResultNotify,			ID_S2C_NOTIFY_WORLDRESULT )		
		CASE_NEW_MSG( CMessageJoinCountryNotify,			ID_S2C_NOTIFY_JOINCOUNTRY )
		CASE_NEW_MSG( CMessageOfficialChangeNotify,			ID_S2C_NOTIFY_OFFICIALCHANGE )
		CASE_NEW_MSG( CMessageWorldAnswerNotify,			ID_S2C_NOTIFY_WORLDANSWER )

		CASE_NEW_MSG( CMessagePlayerGSInfoRequest,				ID_C2S_REQUEST_PLAYERGSINFO )
		CASE_NEW_MSG( CMessagePlayerGSInfoResponse,				ID_S2C_RESPONSE_PLAYERGSINFO )
		CASE_NEW_MSG( CMessagePlayerGSInfoNotify,				ID_S2C_NOTIFY_PLAYERGSINFO )
		CASE_NEW_MSG( CMessageKingChangeNotify,					ID_S2C_NOTIFY_KINGCHANGE )		
		CASE_NEW_MSG( CMessageGetAwardRequest,					ID_C2S_REQUEST_GETAWARD )
		CASE_NEW_MSG( CMessageGetAwardResponse,					ID_S2C_RESPONSE_GETAWARD )
		CASE_NEW_MSG( CMessageSummonPlayerNotify,				ID_S2C_NOTIFY_SUMMONPLAYER )
	
		CASE_NEW_MSG( CMessageUserPrivilegeNotify,				ID_G2C_NOTIFY_PRIVILEGE)
		CASE_NEW_MSG( CMessageExecPrivilegeRequest,				ID_C2G_REQUEST_PRIVILEGE)
		CASE_NEW_MSG( CMessageExecPrivilegeResponse,			ID_G2C_RESPONSE_PRIVILEGE)

		CASE_NEW_MSG( CMessageSubmitWarRequest,				ID_C2S_REQUEST_SUBMITWAR )
		CASE_NEW_MSG( CMessageSubmitWarResponse,			ID_S2C_RESPONSE_SUBMITWAR )
		CASE_NEW_MSG( CMessageEnterWarRequest,			ID_C2S_REQUEST_ENTERWAR )
		CASE_NEW_MSG( CMessageEnterWarResponse,			ID_S2C_RESPONSE_ENTERWAR )
		CASE_NEW_MSG( CMessageQueryWarCampRequest,			ID_C2S_REQUEST_QUERYWARCAMP )
		CASE_NEW_MSG( CMessageQueryWarCampResponse,			ID_S2C_RESPONSE_QUERYWARCAMP )
		CASE_NEW_MSG( CMessageQueryWarNumRequest,				ID_C2S_REQUEST_QUERYWARNUM )
		CASE_NEW_MSG( CMessageQueryWarNumResponse,			ID_S2C_RESPONSE_QUERYWARNUM )		
		CASE_NEW_MSG( CMessageQueryWarInfoRequest,			ID_C2S_REQUEST_QUERYWARINFO )
		CASE_NEW_MSG( CMessageQueryWarInfoResponse,			ID_S2C_RESPONSE_QUERYWARINFO )
		CASE_NEW_MSG( CMessageQueryWarRankRequest,			ID_C2S_REQUEST_QUERYRANK )
		CASE_NEW_MSG( CMessageQueryWarRankResponse,			ID_S2C_RESPONSE_QUERYRANK )
		CASE_NEW_MSG( CMessageBidCampRequest,				ID_C2S_REQUEST_BIDCAMP )
		CASE_NEW_MSG( CMessageBidCampResponse,				ID_S2C_RESPONSE_BIDCAMP )

		CASE_NEW_MSG( CMessageWarSourceNotify,				ID_S2C_NOTIFY_WARSOURCE )
		CASE_NEW_MSG( CMessageWarOperateRequest,			ID_C2S_REQUEST_WAROPERATE )
		CASE_NEW_MSG( CMessageWarOperateResponse,			ID_S2C_RESPONSE_WAROPERATE )
		CASE_NEW_MSG( CMessageCampBidInfoRequest,			ID_C2S_REQUEST_CAMPBIDINFO )
		CASE_NEW_MSG( CMessageCampBidInfoResponse,			ID_S2C_RESPONSE_CAMPBIDINFO )
		CASE_NEW_MSG( CMessageCampResInfoRequest,			ID_C2S_REQUEST_CAMPRESINFO )
		CASE_NEW_MSG( CMessageCampResInfoResponse,			ID_S2C_RESPONSE_CAMPRESINFO )
		CASE_NEW_MSG( CMessageGetWarAwardRequest,			ID_C2S_REQUEST_GETWARAWARD )
		CASE_NEW_MSG( CMessageGetWarAwardResponse,			ID_S2C_RESPONSE_GETWARAWARD )
		CASE_NEW_MSG( CMessageWarInfoNotify,				ID_S2C_NOTIFY_WARINFO )
		CASE_NEW_MSG( CMessageChangeOwnCampNotify,			ID_S2C_NOTIFY_CHANGEOWNCAMP )
		CASE_NEW_MSG( CMessageQueryWarAwardRequest,			ID_C2S_REQUEST_QUERYWARAWARD )
		CASE_NEW_MSG( CMessageQueryWarAwardResponse,		ID_S2C_RESPONSE_QUERYWARAWARD )
		CASE_NEW_MSG( CMessageWarHonorNotify,				ID_S2C_NOTIFY_WARHONOR )
		CASE_NEW_MSG( CMessageWarCampNotify,				ID_S2C_NOTIFY_WARCAMP )
		CASE_NEW_MSG( CMessageModifyRateRequest,			ID_C2S_REQUEST_MODIFYRATE )
		CASE_NEW_MSG( CMessageModifyRateResponse,			ID_S2C_RESPONSE_MODIFYRATE )
		CASE_NEW_MSG( CMessageModifyRateNotify,				ID_S2C_NOTIFY_MODIFYRATE )
		CASE_NEW_MSG( CMessageGetTaxRequest,				ID_C2S_REQUEST_GETTAX )
		CASE_NEW_MSG( CMessageGetTaxResponse,				ID_S2C_RESPONSE_GETTAX )
		CASE_NEW_MSG( CMessageQueryStoneNotify,				ID_S2C_NOTIFY_QUERYSTONE )
		CASE_NEW_MSG( CMessageRefuseSummonRequest,			ID_C2S_REQUEST_REFUSESUMMON )
		CASE_NEW_MSG( CMessageRefuseSummonNotify,			ID_S2C_NOTIFY_REFUSESUMMON )
		CASE_NEW_MSG( CMessageAccuseKingRequest,			ID_C2S_REQUEST_ACCUSEKING )
		CASE_NEW_MSG( CMessageAccuseKingResponse,			ID_S2C_RESPONSE_ACCUSEKING )
		CASE_NEW_MSG( CMessageAccuseKingNotify,				ID_S2C_NOTIFY_ACCUSEKING )
		CASE_NEW_MSG( CMessageAccuseStatusNotify,			ID_S2C_NOTIFY_ACCUSESTATUS )
		CASE_NEW_MSG( CMessagePowerInfoRequest,				ID_C2S_REQUEST_POWERINFO )
		CASE_NEW_MSG( CMessagePowerInfoResponse,			ID_S2C_RESPONSE_POWERINFO )

		CASE_NEW_MSG( CMessageMakeGuoTanNotify,					ID_S2C_NOTIFY_MAKEGUOTAN )
		CASE_NEW_MSG( CMessageMakeLuanWuNotify,					ID_S2C_NOTIFY_MAKELUANWU )

		// weak camp protection
		CASE_NEW_MSG( CMessageCampWeakListRequest,			ID_C2S_CAMPWEAKLIST_REQUEST)
		CASE_NEW_MSG( CMessageCampWeakListResponse,				ID_S2C_CAMPWEAKLIST_RESPONSE)
		CASE_NEW_MSG( CMessageCampGetAwardRequest,			ID_C2S_CAMPGETAWARD_REQUEST)
		CASE_NEW_MSG( CMessageCampGetAwardResponse,			ID_S2C_CAMPGETAWARD_RESPONSE)
				
		// marriage
		CASE_NEW_MSG( CMessageApplyMarryRequest ,ID_C2S_REQUEST_APPLYMARRY )
		CASE_NEW_MSG( CMessageApplyMarryResponse,ID_S2C_RESPONSE_APPLYMARRY )

		CASE_NEW_MSG( CMessageMarryRequestNotify, ID_S2C_NOTIFY_MARRYREQUEST )
		CASE_NEW_MSG( CMessageReplyMarryRequest, ID_C2S_REQUEST_REPLYMARRY )
		CASE_NEW_MSG( CMessageReplyMarryResponse, ID_S2C_RESPONSE_REPLYMARRY )

		CASE_NEW_MSG( CMessageMarriageTipsNotify, ID_S2C_NOTIFY_MARRIAGETIPS )

		CASE_NEW_MSG ( CMessageBeginWeddingRequest, ID_C2S_REQUEST_BEGINWEDDING )
		CASE_NEW_MSG( CMessageBeginWeddingResponse, ID_S2C_RESPONSE_BEGINWEDDING )

		CASE_NEW_MSG( CMessageBeginWeddingNotify, ID_S2C_NOTIFY_BEGINWEDDING )

		CASE_NEW_MSG( CMessageReplyBeginWeddingRequest, ID_C2S_REQUEST_REPLYBEGINWEDDING )
		CASE_NEW_MSG( CMessageReplyBeginWeddingResponse, ID_S2C_RESPONSE_REPLYBEGINWEDDING )

		CASE_NEW_MSG( CMessageWeddingStartNotify, ID_S2C_NOTIFY_WEDDINGSTART )

		CASE_NEW_MSG( CMessageEnterMarriageHallNotify, ID_S2C_NOTIFY_ENTERMARRIAGEHALL )
		CASE_NEW_MSG( CMessageReplyEnterMarriageHallRequest,  ID_C2S_REQUEST_REPLYENTERMARRIAGEHALL	)
		CASE_NEW_MSG( CMessageReplyEnterMarriageHallResponse, ID_S2C_RESPONSE_REPLYENTERMARRIAGEHALL )

		CASE_NEW_MSG( CMessageNewlyEnterHallRequest, ID_C2S_REQUEST_NEWLYWEDENTERHALL )
		CASE_NEW_MSG( CMessageNewlyEnterHallResponse, ID_S2C_RESPONSE_NEWLYWEDENTERHALL )

		CASE_NEW_MSG( CMessageGuestEnterHallRequest, ID_C2S_REQUEST_GUESTENTERHALL )
		CASE_NEW_MSG( CMessageGuestEnterHallResponse, ID_S2C_RESPONSE_GUESTENTERHALL )

		CASE_NEW_MSG( CMessageBaitangRequest, ID_C2S_REQUEST_BAITANG )
		CASE_NEW_MSG( CMessageBaitangResponse, ID_S2C_RESPONSE_BAITANG )

		CASE_NEW_MSG( CMessageBaitangNotify, ID_S2C_NOTIFY_BAITANG )

		CASE_NEW_MSG( CMessagePutKeepSakeRequest, ID_C2S_REQUEST_PUTKEEPSAKE )
		CASE_NEW_MSG( CMessagePutKeepSakeResponse, ID_S2C_RESPONSE_PUTKEEPSAKE )

		CASE_NEW_MSG( CMessageExchangeKeepsakeRequest, ID_C2S_REQUEST_EXCHANGKEEPSAKE )
		CASE_NEW_MSG( CMessageExchangeKeepsakeResponse, ID_S2C_RESPONSE_EXCHANGKEEPSAKE )

		CASE_NEW_MSG( CMessageExchangeKeepsakeNotify, ID_S2C_NOTIFY_EXCHANGEKEEPSAKE )

		CASE_NEW_MSG( CMessageDivorceRequest, ID_C2S_REQUEST_DIVORCE )
		CASE_NEW_MSG( CMessageDivorceResponse, ID_S2C_RESPONSE_DIVORCE )

		CASE_NEW_MSG( CMessageDivorceNotify, ID_S2C_NOTIFY_DIVORCE )

		CASE_NEW_MSG( CMessageMarryResultNotify, ID_S2C_NOTIFY_MARRYRESULT )

		CASE_NEW_MSG( CMessageBeginWeddingResultNofiy, ID_S2C_NOTIFY_BEGINWEDINGRESULT )

		CASE_NEW_MSG( CMessageGetCampMarriageInfoRequest, ID_C2S_REQUEST_GETCAMPMARRAIGEINFO )
		CASE_NEW_MSG( CMessageGetCampMarriageInfoResponse, ID_S2C_RESPONSE_GETCAMPMARRIAGEINFO )

		CASE_NEW_MSG( CMessagePersonMarriedNotify, ID_S2C_NOTIFY_PERSONMARRIED )
		CASE_NEW_MSG( CMessagePersonMarriageInfoNotify, ID_S2C_NOTIFY_PERSONMARRIAGEINFO )	
		CASE_NEW_MSG( CMessageSpouseDivorceNotify,	ID_S2C_NOTIFY_SPOUSEDIVORCE )
		
		CASE_NEW_MSG( CMessageUseSysInvitationRequest,	ID_C2S_REQUEST_USESYSINVITATION )
		CASE_NEW_MSG( CMessageUseSysInvitationResponse, ID_S2C_RESPONSE_USESYSINVITATION )
	
		CASE_NEW_MSG( CMessageTimeToPutKeepsakeNotify, ID_S2C_NOTIFY_TIMETOPUTKEEPSAKE )
		
		CASE_NEW_MSG( CMessageUseKitbagNeedleRequest, ID_C2S_REQUEST_USEKITBAGNEEDLE )						
		CASE_NEW_MSG( CMessageUseKitbagNeedleResponse, ID_S2C_RESPONSE_USEKITBAGNEEDLE )
		
		// activity
		CASE_NEW_MSG( CMessageThrowHydrangeaNotify, ID_S2C_NOTIFY_THROWHYDRANGEA )
		CASE_NEW_MSG( CMessageGrabHydrangeaEnableNotify, ID_S2C_NOTIFY_GRABHYDRANGEAENABLE )

		CASE_NEW_MSG( CMessageGrabHydrangeaRequest, ID_C2S_REQUEST_GRABHYDRANGEA )
		CASE_NEW_MSG( CMessageGrabHydrangeaResponse, ID_S2C_RESPONSE_GRABHYDRANGEA )
		CASE_NEW_MSG( CMessageGrabHydrangeaResultNotify, ID_S2C_NOTIFY_GRABHYDRANGEARESULT )

		CASE_NEW_MSG( CMessageHydrangeaResultNotify, ID_S2C_NOTIFY_HYDRANGEAREASULT )
		CASE_NEW_MSG( CMessageStartHydrangeaNotify, ID_S2C_NOTIFY_STARTHYDRANGEA )

		CASE_NEW_MSG( CMessageThrowHydrangeaRequest, ID_C2S_REQUEST_THROWHYDRANGEA )
		CASE_NEW_MSG( CMessageThrowHydrangeaResponse, ID_S2C_RESPONSE_THROWHYDRANGEA )
		
		CASE_NEW_MSG( CMessageThrowSugarNotify,	ID_S2C_NOTIFY_NPCTHROWSUGAR )
		
		CASE_NEW_MSG( CMessageSetHydrangeaStatusNotify, ID_S2C_NOTIFY_SETHYDRANGEASTATUS )
		
		CASE_NEW_MSG( CMessageCancelWeddingRequest, ID_C2S_REQUEST_CANCELWEDDING )
		CASE_NEW_MSG( CMessageCancelWeddingResponse, ID_S2C_RESPONSE_CANCELWEDDING )
		
		CASE_NEW_MSG( CMessageNewlyMarriedNotify,	ID_S2C_NOTIFY_NEWLYMARRIED )
		
		CASE_NEW_MSG( CMessageWeddingEndNotify,	ID_S2C_NOTIFY_WEDDINGEND )
		
		CASE_NEW_MSG( CMessageStartKeepsakeExchangeRequest, ID_C2S_REQUEST_STARTKEEPSAKEEXCHANGE )	
		CASE_NEW_MSG( CMessageStartKeepsakeExchangeResponse, ID_S2C_RESPONSE_STARTKEEPSAKEEXCHANGE )
		CASE_NEW_MSG( CMessageStopKeepsakeExchangeRequest, ID_C2S_REQUEST_STOPKEEPSAKEEXCHANGE )
		CASE_NEW_MSG( CMessageStopKeepsakeExchangeResponse, ID_S2C_RESPONSE_STOPKEEPSAKEEXCHANGE )
		CASE_NEW_MSG( CMessageStopKeepsakeExchangeNotify, ID_S2C_NOTIFY_STOPKEEPSAKEEXCHANGE )
		CASE_NEW_MSG( CMessageInvitationExchangeRequest, ID_C2S_REQUEST_INVITATIONEXCHANGE )
		CASE_NEW_MSG( CMessageInvitationExchangeResponse,	ID_S2C_RESPONSE_INVITATIONEXCHANGE )
		CASE_NEW_MSG( CMessageExchangeResultNotify,	ID_S2C_NOTIFY_EXCHANGERESULT )
		CASE_NEW_MSG( CMessageTeleToWeddingNpcRequest, ID_C2S_REQUEST_TELETOWEDDINGNPC )
		CASE_NEW_MSG( CMessageTeleToWeddingNpcResponse, ID_S2C_RESPONSE_TELETOWEDDINGNPC )
		
		CASE_NEW_MSG( CMessageReviewWeddingRequest, ID_C2S_REQUEST_REVIEWWEDDING )		
		CASE_NEW_MSG( CMessageReviewWeddingResponse, ID_S2C_RESPONSE_REVIEWWEDDING )
		CASE_NEW_MSG( CMessageReviewWeddingNotify, ID_S2C_NOTIFY_REVIEWWEDDING )		
		
		
		CASE_NEW_MSG( CMessageStartPreliminaryNotify, ID_S2C_NOTIFY_STARTPRELIMINARY )
		CASE_NEW_MSG( CMessagePreliminaryQuestionNotify, ID_S2C_NOTIFY_PRELIMINARYQUESTION )		
		CASE_NEW_MSG( CMessagePreliminaryAnswerNotify, ID_S2C_NOTIFY_PRELIMINARYANSWER	)
		CASE_NEW_MSG( CMessageEndPreliminaryNotify, ID_S2C_NOTIFY_ENDPRELIMINARY )
		CASE_NEW_MSG( CMessageSubmitPreliminaryAnswerRequest, ID_C2S_REQUEST_SUBMITPRELIMINARYANSWER )
		CASE_NEW_MSG( CMessageSubmitPreliminaryAnswerResponse, ID_S2C_RESPONSE_SUBMITPRELIMINARYANSWER )
		CASE_NEW_MSG( CMessageViewPreliminaryListRequest, ID_C2S_REQUEST_VIEWPREMILINARYLIST )
		CASE_NEW_MSG( CMessageViewPreliminaryResponse, ID_S2C_RESPONSE_IEWPREMILINARYLIST )
		CASE_NEW_MSG( CMessageUseBlackPillsNotify, ID_S2C_NOTIFY_USEBLACKPILLS )
		CASE_NEW_MSG( CMessageExpContainerNotify, ID_S2C_NOTIFY_EXPCONTAINER )
		CASE_NEW_MSG( CMessageQuestionVerifyNotify, ID_S2C_NOTIFY_QUESTIONVERIFY )		
		CASE_NEW_MSG( CMessageAnswerVerifyRequest, ID_C2S_REQUEST_ANSWERVERIFY )
		CASE_NEW_MSG( CMessageAnswerVerifyResponse, ID_S2C_RESPONSE_ANSWERVERIFY )	

		CASE_NEW_MSG( CMessageStartGiftTimerNotify, ID_S2C_GIFTTIMER_NOTIFY )				
		CASE_NEW_MSG( CMessageGetOnlineGiftRequest, ID_C2S_GETGIFT_REQUEST )				
		CASE_NEW_MSG( CMessageGetOnlineGiftResponse, ID_S2C_GETGIFT_RESPONSE )				
		
		CASE_NEW_MSG( CMessageGetTreasureBowlInfoRequest, ID_C2S_REQUEST_GETTREASUREBOWLINFO )
		CASE_NEW_MSG( CMessageGetTreasureBowlInfoResponse, ID_S2C_RESPONSE_GETTREASUREBOWLINFO )
		CASE_NEW_MSG( CMessageStartTreasureBowlRequest, ID_C2S_REQUEST_STARTTREASUREBOWL )
		CASE_NEW_MSG( CMessageStartTreasureBowlResponse, ID_S2C_RESPONSE_STARTTREASUREBOWL )
		CASE_NEW_MSG( CMessageTreasureBowlSuccessNotify, ID_S2C_NOTIFY_TREASUREBOWLSUCCESS )
		
        // 摇钱树活动
        CASE_NEW_MSG( CMessageHandInRequest,                ID_C2S_HANDIN_REQUEST                   )
        CASE_NEW_MSG( CMessageHandInResponse,               ID_S2C_HANDIN_RESPONSE                  )
        CASE_NEW_MSG( CMessageGetPlayerRankForYQSRequest,   ID_C2S_GETPLAYERRANKFORYQS_REQUEST      )
        CASE_NEW_MSG( CMessageGetPlayerRankForYQSResponse,  ID_S2C_GETPLAYERRANKFORYQS_RESPONSE     )
        CASE_NEW_MSG( CMessageC2SGetCampRankForYQSRequest,  ID_C2S_GETCAMPRANKFORYQS_REQUEST        )
        CASE_NEW_MSG( CMessageS2CGetCampRankForYQSResponse, ID_S2C_GETCAMPRANKFORYQS_RESPONSE       )
        CASE_NEW_MSG( CMessageYQSPhaseChangeNotice,         ID_S2C_YQSPHASECHANGE_NOTICE            )
        CASE_NEW_MSG( CMessageYQSEncourageChangeNotice,     ID_S2C_YQSENCOURAGE_NOTICE              )
        CASE_NEW_MSG( CMessageCheckAnswerRequest,           ID_S2C_YQSDROPITEM_NOTICE               )
        CASE_NEW_MSG( CMessageActivityYqsStart,             ID_S2C_YQSSTART_NOTICE )


		// 发布任务消息 game -client
		CASE_NEW_MSG( CMessageCheckGroupPublishRequest,             ID_C2S_REQUEST_CHECK_GROUPPUBLISH )
		CASE_NEW_MSG( CMessagePublishGroupTaskRequest,				ID_C2S_REQUEST_PLAYERPUBLISHTASK )
		CASE_NEW_MSG( CMessageCheckGroupPublishResponse,			ID_S2C_RESPONSE_CHECK_GROUPPUBLISH )
		CASE_NEW_MSG( CMessagePublishGroupTaskResponse,				ID_S2C_RESPONSE_PLAYERPUBLISHTASK )

		// single publish request

		CASE_NEW_MSG( CMessagePlayerCheckSinglePublish,             ID_C2S_REQUEST_CHECK_SINGLEPUBLISH )
		CASE_NEW_MSG( CMessagePlayerSinglePublishTask,				ID_C2S_REQUEST_SINGLEPUBLISHTASK )
		CASE_NEW_MSG( CMessageRefreshSingleTaskRequest,				ID_C2S_REQUEST_REFRESHSINGLETASK )
		CASE_NEW_MSG( CMessagePlayerGetPublishTaskRequest,			ID_C2S_REQUEST_OBTAINSINGLETASK )
		CASE_NEW_MSG( CMessagePlayerGetAwardsRequest,				ID_C2S_REQUEST_RECVSINGLEAWARDS )

		// single publish response

		CASE_NEW_MSG( CMessageCheckSinglePublishResponse,			ID_S2C_RESPONSE_CHECK_SINGLEPUBLISH )
		CASE_NEW_MSG( CMessagePlayerSinglePublishResponse,			ID_S2C_RESPONSE_SINGLEPUBLISHTASK )
		CASE_NEW_MSG( CMessageRefreshSingleTaskResponse,			ID_S2C_RESPONSE_REFRESHRESULT )
		
#endif				   
		// rank module
		CASE_NEW_MSG( CMessageGetPlayerRankRequest,				ID_C2S_REQUEST_PLAYERRANK )
		CASE_NEW_MSG( CMessageGetPlayerRankResponse,			ID_S2C_RESPONSE_PLAYERRANK )
	default :
		{
			break;
		}

	}

	return tpMessage;
}


#ifdef LINUX
// 创建 server 专用消息
Message* CMessageFactory::CreateServerMessage( unsigned int unMessageID  )
{
	Message* tpMessage = NULL;

	switch( unMessageID )
	{
		// 全局服务器消息
		CASE_NEW_MSG( CMessageGlobalTeleportRequest, ID_S2D_REQUEST_GLOBALTELEPORT )
		CASE_NEW_MSG( CMessageGlobalTeleportResponse, ID_D2S_RESPONSE_GLOBALTELEPORT )
		CASE_NEW_MSG( CMessageChangeServerRequest, ID_D2G_REQUEST_CHANGESERVER )
		CASE_NEW_MSG( CMessageChangeServerResponse, ID_G2D_RESPONSE_CHANGESERVER )
		CASE_NEW_MSG( CMessagePlayerApplyWarRequest, ID_S2D_REQUEST_PLAYERAPPLYWAR )
		CASE_NEW_MSG( CMessagePlayerApplyWarResponse, ID_D2S_REQPONSE_PLAYERAPPLYWAR )
		CASE_NEW_MSG( CMessageWarEnableNotify,	ID_D2S_NOTIFY_WARENABLE	)
		CASE_NEW_MSG( CMessagePlayerLeaveWarNotify, ID_S2D_NOTIFY_PLAYERLEAVEWAR )
		CASE_NEW_MSG( CMessageWarEndNotify, ID_S2D_NOTIFY_WAREND )
		CASE_NEW_MSG( CMessageCreateRepetionRequest, ID_D2G_REQUEST_CREATEREPETION )
		CASE_NEW_MSG( CMessageCreateRepetionResponse, ID_G2D_RESPONSE_CREATEREPETION )
		CASE_NEW_MSG( CMessageKickGlobalPlayerNotify, ID_S2S_NOTIFY_KICKGLOBALPLAYER )
		CASE_NEW_MSG( CMessageKickGlobalPlayerACK, ID_S2S_NOTIFY_KICKGLOBALPLAYERACK )
		CASE_NEW_MSG( CMessageSyncGlobalPlayeNotify, ID_S2S_NOTIFY_SYNCGLOBALPLAYE )

        // 获取摇钱树活动消息
        CASE_NEW_MSG( CMessageS2GGetCampRankForYQSRequest,     ID_S2G_GETCAMPRANKFORYQS_REQUEST					)
        CASE_NEW_MSG( CMessageG2SGetCampRankForYQSResponse,    ID_G2S_GETCAMPRANKFORYQS_RESPONSE				)
        CASE_NEW_MSG( CMessageS2GUpdateYQSGrowingRequest,      ID_S2G_UPDATEYQSGROWING_REQUEST					)
        CASE_NEW_MSG( CMessageG2SUpdateYQSGrowingResponse,     ID_G2S_UPDATEYQSGROWING_RESPONSE					)
        CASE_NEW_MSG( CMessageG2SSettleNotice,                 ID_G2S_SETTLE_NOTICE					            )
        CASE_NEW_MSG( CMessageYQSActivtyStartNotice,           ID_G2S_YAOQIANSHUSTART_NOTICE					)

		CASE_NEW_MSG( CMessageInternalTimeOut,                 ID_INTER_TIMEOUT					)
		CASE_NEW_MSG( CMessageGateInfoNotice,                  ID_G2L_GATE_INFO_NOTICE            )
		CASE_NEW_MSG( CMessageGateInfoResponse,                ID_L2G_GATE_INFO_RESPONSE          )
		CASE_NEW_MSG( CMessagePlayerEnterGateNotice,           ID_L2G_PLAYER_ENTER_GATE_NOTICE    )
		CASE_NEW_MSG( CMessagePlayerEnterGateResponse,         ID_G2L_PLAYER_ENTER_GATE_RESPONSE  )
		CASE_NEW_MSG( CMessagePlayerLeaveGateNotice,           ID_G2L_PLAYER_LEAVE_GATE_NOTICE    )
		CASE_NEW_MSG( CMessageServerInfoNotice,                ID_S2G_SERVER_INFO_NOTICE          )
		CASE_NEW_MSG( CMessageServerInfoResponse,              ID_G2S_SERVER_INFO_RESPONSE        )
		CASE_NEW_MSG( CMessagePlayerEnterMapNotice,            ID_G2S_PLAYER_ENTER_MAP_NOTICE     )
		CASE_NEW_MSG( CMessagePlayerEnterMapResponse,          ID_S2G_PLAYER_ENTER_MAP_RESPONSE   )
		CASE_NEW_MSG( CMessagePlayerLoginCompleteNotice,       ID_G2S_PLAYER_LOGIN_COMPLETE_NOTICE)
		CASE_NEW_MSG( CMessagePlayerLeaveMapNotice,            ID_S2G_PLAYER_LEAVE_MAP_NOTICE     )
		CASE_NEW_MSG( CMessagePlayerLeaveMapResponse,          ID_G2S_PLAYER_LEAVE_MAP_RESPONSE   )
		CASE_NEW_MSG( CMessagePlayerReturn2GateNotice,         ID_S2G_PLAYER_RETURN2GATE_NOTICE   )
		CASE_NEW_MSG( CMessagePlayerReturn2GateResponse,       ID_G2S_PLAYER_RETURN2GATE_RESPONSE )
		CASE_NEW_MSG( CMessageCreateMapNotice,                 ID_G2S_CREATE_MAP_NOTICE           )
		CASE_NEW_MSG( CMessageCreateMapResponse,               ID_S2G_CREATE_MAP_RESPONSE         )
		CASE_NEW_MSG( CMessageRegistRepetionInfo,			   ID_S2G_REGIST_REPETION_INFO		  )
		CASE_NEW_MSG( CMessagePlayerRedirectRequest,           ID_S2G_PLAYER_REDIRECT_REQUEST     )
		CASE_NEW_MSG( CMessagePlayerRedirectResponse,          ID_G2S_PLAYER_REDIRECT_RESPONSE    )
		CASE_NEW_MSG( CMessagePlayerEnterConfirmNotice,        ID_S2G_PLAYER_ENTER_CONFIRM_NOTICE )
		CASE_NEW_MSG( CMessageSceneNotOpenNotice,              ID_G2S_SCENE_NOT_OPEN_NOTICE       )
		CASE_NEW_MSG( CMessageUpdateVersionNotice,             ID_G2L_UPDATE_VERSION_NOTICE       )
		CASE_NEW_MSG( CMessagePlayerRedirectMapNotice,		   ID_G2S_REDIRECT_MAP_NOTICE		  )
		CASE_NEW_MSG( CMessagePlayerRedirectEnterMapResponse,  ID_S2G_REDIRECT_ENTER_MAP_RESPONSE )
		CASE_NEW_MSG( CMessageRedirectGErrorCode,			   ID_G2S_REDIRECT_G_ERROR_CODE		  )
		CASE_NEW_MSG( CMessageRedirectSErrorCode,			   ID_S2G_REDIRECT_S_ERROR_CODE		  )
		CASE_NEW_MSG( CMessageCountryInfo,					   ID_G2S_COUNTRY_INFO				  )
		CASE_NEW_MSG( CMessageChangePasswordStatusNotify,	   ID_S2G_NOTIFY_CHANGEPASSWORDSTATUS )
		CASE_NEW_MSG( CMessageResetRolePasswordNotify,		   ID_G2S_NOTIFY_RESETROLEPASSWORD )
		CASE_NEW_MSG( CMessageSGProVipFlagNotice,			ID_S2G_NOTIFY_VIPFLAG )

		// db
		CASE_NEW_MSG( CMessageExecuteSqlRequest,			   ID_S2S_EXECUTESQL_REQUEST		  )
		CASE_NEW_MSG( CMessageExecuteSqlResponse3,             ID_S2S_EXECUTESQL_RESPONSE		  )
		CASE_NEW_MSG( CMessageLoadPlayerItemInfoRequest,		ID_S2D_LOADPLAYERITEMINFO_REQUEST)
		CASE_NEW_MSG( CMessageLoadPlayerItemInfoResponse,		ID_D2S_LOADPLAYERITEMINFO_RESPONSE)
		CASE_NEW_MSG( CMessageSavePlayerItemInfoRequest,		ID_S2D_SAVEPLAYERITEMINFO_REQUEST)
		CASE_NEW_MSG( CMessageSavePlayerItemInfoResponse,		ID_D2S_SAVEPLAYERITEMINFO_RESPONSE)
		CASE_NEW_MSG( CMessageLoadPlayerInfoRequest,			ID_S2D_LOADPLAYERINFO_REQUEST )
		CASE_NEW_MSG( CMessageLoadPlayerInfoResponse,			ID_D2S_LOADPLAYERINFO_RESPONSE )
		CASE_NEW_MSG( CMessageSavePlayerRequest, 				ID_S2D_SAVEPLAYER_REQUEST )
		CASE_NEW_MSG( CMessageSavePlayerResponse,				ID_D2S_SAVEPLAYER_RESPONSE )
		CASE_NEW_MSG( CMessageLoadPlayerPropertyRequest,         ID_S2D_LOADPLAYERPROPERTY_REQUEST )
		CASE_NEW_MSG( CMessageLoadPlayerPropertyResponse,       ID_D2S_LOADPLAYERPROPERTY_RESPONSE )

		CASE_NEW_MSG( CMessageGetBourseInfo, 					ID_G2D_GETBOURSE_INFO )
		CASE_NEW_MSG( CMessageBourseInfoNotice,					ID_D2G_BOURSELIST_NOTICE )	
		CASE_NEW_MSG( CMessageBourseS2DIncAccount,				ID_S2D_BOURSE_INC_ACCOUNT )
		CASE_NEW_MSG( CMessageBourseS2DDecAccount,				ID_S2D_BOURSE_DEC_ACCOUNT )
		CASE_NEW_MSG( CMessageBourseS2DGetAccount,				ID_S2D_BOURSE_GET_ACCOUNT )
		CASE_NEW_MSG( CMessageBourseD2SAccountNotice,			ID_D2S_BOURSE_ACCOUNT_NOTICE )
		CASE_NEW_MSG( CMessageBourseAddDoc,						ID_S2D_BOURSE_ADD_DOC )
		CASE_NEW_MSG( CMessageBoursePlayerListGet,				ID_S2D_BOURSE_PLAYER_LIST_GET )
		CASE_NEW_MSG( CMessageBoursePlayerListNotice,			ID_D2S_BOURSE_PLAYER_NOTICE );
		CASE_NEW_MSG( CMessageBourseTrade,						ID_G2D_BOURSE_TRADE )
		CASE_NEW_MSG( CMessageBourseS2GLoadList,				ID_S2G_BOURSELIST_LOAD )
		CASE_NEW_MSG( CMessageBourseG2SLoadList,				ID_G2S_BOURSELIST_LOAD )
		CASE_NEW_MSG( CMessageBourseTradeNotice,				ID_D2G_BOURSE_TRADE_NOTICE )
		CASE_NEW_MSG( CMessageBoursePlayerTradeBuyNotice,		ID_G2S_BOURSE_TRADE_BUY )
		CASE_NEW_MSG( CMessageBoursePlayerTradeSellNotice,		ID_G2S_BOURSE_TRADE_SELL )
		CASE_NEW_MSG( CMessageS2DBourseDecBuyDoc,					ID_S2D_BOURSE_DEC_BUY_DOC )
		CASE_NEW_MSG( CMessageS2DBourseDecSellDoc,					ID_S2D_BOURSE_DEC_SELL_DOC )
		CASE_NEW_MSG( CMessageExchangeLog,						ID_D22_BOURSE_LOG )
		CASE_NEW_MSG( CMessageBourseLogQuery,					ID_S2D_BOURSE_LOG_QUERY )
		CASE_NEW_MSG( CMessageD2SBourseLog,						ID_D2S_BOURSE_LOG )
		

		CASE_NEW_MSG( CMessagePlayerOnLineFriendRequest,       ID_S2G_PLAYER_ONLINE_FRIEND_REQUEST)
		CASE_NEW_MSG( CMessagePlayerOnLineFriendResponse,      ID_G2S_PLAYER_ONLINE_FIREND_RESPONSE)
		CASE_NEW_MSG( CMessagePlayerChangeStatusNotice,        ID_S2G_PLAYER_CHANGE_STATUS_NOTICE )
		CASE_NEW_MSG( CMessageAddFriendRequest,				   ID_S2G_ADDFRIEND_REQUEST				 )		
		CASE_NEW_MSG( CMessageAddFriendResponse,			   ID_G2S_ADDFRIEND_RESPONSE			 )		
		CASE_NEW_MSG( CMessageGetCharIDRequest,					ID_S2G_GETCHARID_REQUEST			 )		
		CASE_NEW_MSG( CMessageGetCharIDResponse,			   ID_G2S_GETCHARID_RESPONSE			 )	
		CASE_NEW_MSG( CMessageAddMasterPointRequest,			ID_S2G_ADDMASTERPOINT_REQUEST			 )		
		CASE_NEW_MSG( CMessageAddMasterPointNotify,			   ID_G2S_ADDMASTERPOINT_NOTIFY			 )	
		CASE_NEW_MSG( CMessageAcquireMasterNotify,			ID_S2G_ACQUIREMASTER_NOTIFY )

		CASE_NEW_MSG( CMessageGameHeartBeatNotice,				ID_S2G_GAME_HEART_BEAT_NOTICE      )
		CASE_NEW_MSG( CMessageERatingProtoStream,				ID_X2X_ERATING_PROTO_STREAM        )
		CASE_NEW_MSG( CMessageKickOffPlayerRequest,				ID_G2S_KICKOFF_PLAYER_REQUEST      )
		CASE_NEW_MSG( CMessageKickOffPlayerResponse,			ID_S2G_KICKOFF_PLAYER_RESPONSE     )
		CASE_NEW_MSG( CMessageGateShutDownNotice,				ID_G2L_GATE_SHUTDOWN_NOTICE        )
		CASE_NEW_MSG( CMessageERatingStateNotice,				ID_L2G_ERATING_STATUS_NOTICE		  )
		CASE_NEW_MSG( CMessageRoleGameInfoReportToGateNotify,	ID_S2G_NOTIFY_ROLEGAMEINFO2GATE	)
		CASE_NEW_MSG( CMessageUpdatePlayerInfoRequest,		ID_S2D_UPDATEPLAYERINFO_REQUEST)
		CASE_NEW_MSG( CMessageUpdatePlayerInfoResponse,		ID_D2S_UPDATEPLAYERINFO_RESPONSE)

		// team msg between scene and gate 
		CASE_NEW_MSG( CMessageCreateGateTeamRequest,		   ID_S2G_REQUEST_CREATETEAMATGATE	  )
		CASE_NEW_MSG( CMessageJoinGateTeamRequest,				   ID_S2G_REQUEST_JOINREQUEST	  )
		CASE_NEW_MSG( CMessageJoinGateTeamInvite,				   ID_S2G_REQUEST_JOININVITE	  )
		CASE_NEW_MSG( CMessageRequestJoinReply,				   ID_S2G_REQUEST_REPLYJOINREQUEST	  )
		CASE_NEW_MSG( CMessageInviteJoinReply,				   ID_S2G_REQUEST_REPLYJOININVITE	  )
		CASE_NEW_MSG( CMessageKickoutGateTeamRequest,			   ID_S2G_REQUEST_KICKOUTTEAM )
		CASE_NEW_MSG( CMessageChangeHeaderRequest,			   ID_S2G_REQUEST_CHANGEHEADER )
		CASE_NEW_MSG( CMessageLeaveGateTeamRequest,				   ID_S2G_REQUEST_LEAVETEAM )
		CASE_NEW_MSG( CMessageEndGateTeamRequest,				   ID_S2G_REQUEST_ENDTEAM )
		CASE_NEW_MSG( CMessageNotifyErrToScene,						ID_G2S_NOTIFY_ERRORNOTICE )
		CASE_NEW_MSG( CMessageCreateGateTeamResponse,			ID_G2S_RESPONSE_CREATETEAM )
		CASE_NEW_MSG( CMessageJoinTeamInviteResponse,			ID_G2S_RESPONSE_JOININVITE )
		CASE_NEW_MSG( CMessageJoinGateTeamResponse,				ID_G2S_RESPONSE_JOINREQUEST )
		CASE_NEW_MSG( CMessageNotifySceneToUpdateTeam,			ID_G2S_NOTIFY_UPDATETEAM )
		CASE_NEW_MSG( CMessageNotifySceneToSendTeamList,		ID_G2S_NOTIFY_SENDLIST )
		CASE_NEW_MSG( CMessageInvite2Request,					ID_G2S_RESPONSE_INVITEREQUEST )
		CASE_NEW_MSG( CMessageUpdateTeamPropertyRequest,		ID_S2G_REQUEST_UPDATETEAMPRO )
		CASE_NEW_MSG( CMessageUpdateTeamPropertyResponse,		ID_G2S_RESPONSE_UPDATETEAMPRO )
		CASE_NEW_MSG( CMessageSendClaimRecruitRequest,			ID_S2G_REQUEST_CLAIMRECRUIT )
		CASE_NEW_MSG( CMessageSendClaimRecruitResponse,			ID_G2S_RESPONSE_CLAIMRECRUIT )
		CASE_NEW_MSG( CMessageQueryClaimRecruitRequest,			ID_S2G_REQUEST_QUERYCLAIMRECRUIT )
		CASE_NEW_MSG( CMessageQueryClaimRecruitResponse,		ID_G2S_RESPONSE_QUERYCLAIMRECRUIT )
		CASE_NEW_MSG( CMessageDeleteClaimRecruitRequest,		ID_S2G_REQUEST_DELETECLAIMRECRUIT )
		CASE_NEW_MSG( CMessageDeleteClaimRecruitResponse,		ID_G2S_RESPONSE_DELETECLAIMRECRUIT )
		CASE_NEW_MSG( CMessageFollowRequest,					ID_S2G_REQUEST_FOLLOW )
		CASE_NEW_MSG( CMessageFollowResponse,					ID_G2S_RESPONSE_FOLLOW )
		CASE_NEW_MSG( CMessageChangeDistributeRequest,			ID_S2G_REQUEST_DISTRIBUTE )
		CASE_NEW_MSG( CMessageChangeDistributeResponse,			ID_G2S_RESPONSE_DISTRIBUTE )
		CASE_NEW_MSG( CMessageObtainGateTeamRequest,		    ID_S2G_REQUEST_OBTAINTEAMATGATE	  )
		CASE_NEW_MSG( CMessageClearInvReqRecordRequest,			ID_S2G_REQUEST_CLEARRECORD )
	
		CASE_NEW_MSG( CMessageGetPlayerMailiInfoRequest, ID_C2G_REQUEST_GETMAILINFO )
		CASE_NEW_MSG( CMessageGetPlayerMailInfoResponse, ID_G2S_RESPONSE_GETMAIINFO )
        CASE_NEW_MSG( CMessageSendSysMailToPlayer, ID_G2S_NOTIFY_SENDSYSMAILTOPLAYER )


		CASE_NEW_MSG( CMessageOfflineSysMailNotify,				ID_S2G_NOTIFY_OFFLINEMAIL)
		CASE_NEW_MSG( CGMMessageFreezeAccountCmd,				ID_M2G_FROZENACCOUNT_CMD)
		CASE_NEW_MSG( CGMMessageUnFreezeAccountCmd,				ID_M2G_UNFROZENACCOUNT_CMD)
		CASE_NEW_MSG( CGMMessageShutUpCmd,						ID_M2G_SHUTUP_CMD)
		CASE_NEW_MSG( CGMMessageCancelShutUpCmd,				ID_M2G_CANCLESHUTUP_CMD)
		CASE_NEW_MSG( CGMMessageDeleteRoleCmd,					ID_M2G_DELETEROLE_CMD)
		CASE_NEW_MSG( CGMMessageRecoverRoleCmd,					ID_M2G_RECOVERROLE_CMD)
		CASE_NEW_MSG( CGMMessagePostBulletinCmd,				ID_M2G_POSTBULLETIN_CMD)

		CASE_NEW_MSG( CGMMessageFreezeMacCmd,					ID_M2G_FROZENMAC_CMD)
		CASE_NEW_MSG( CGMMessageFreezeMacCmd,					ID_M2G_UNFROZENMAC_CMD)

		CASE_NEW_MSG( CGMMessageSetIbPriceCmd,					ID_M2G_SETIBPRICE_CMD)
		CASE_NEW_MSG( CGMMessageIbOnSaleCmd,					ID_M2G_IBONSALE_CMD)
		CASE_NEW_MSG( CGMMessageIbNotSaleCmd,					ID_M2G_IBNOTSALE_CMD)
		CASE_NEW_MSG( CGMMessageSetIbLocationCmd,				ID_M2G_SETIBLOCATION_CMD)

		CASE_NEW_MSG( CGMMessageKickRoleCmd,				   ID_M2G_KICKROLE_CMD)
		CASE_NEW_MSG( CGMMessageMoveRoleCmd,	               ID_M2G_MOVEROLE_CMD)
		CASE_NEW_MSG( CGMMessageCmdNormalResult,			   ID_G2M_CMDNORMAL_RESULT)
		CASE_NEW_MSG( CGMMessageUserStorageInfoCmd,			   ID_M2G_USERSTORAGEINFO_CMD)
		CASE_NEW_MSG( CGMMessageUserStorageInfoResult,		   ID_G2M_USERSTORAGEINFO_RESULT)
		CASE_NEW_MSG( CGMMessageRoleLocationCmd,			   ID_M2G_ROLELOCATION_CMD)
		CASE_NEW_MSG( CGMMessageRoleLocationResult,			   ID_G2M_ROLELOCATION_RESULT)
		CASE_NEW_MSG( CGMMessageNPCDetailCmd,			   	   ID_M2G_NPCDETAIL_CMD)
		CASE_NEW_MSG( CGMMessageNPCDetailResult,			   ID_G2M_NPCDETAIL_RESULT)
		CASE_NEW_MSG( CGMMessageTaskInfoCmd,			   	   ID_M2G_TASKINFO_CMD)
		CASE_NEW_MSG( CGMMessageEndTaskCmd,			   	   	   ID_M2G_ENDTASK_CMD)
		CASE_NEW_MSG( CGMMessageTaskInfoCmd,			   	   ID_M2G_FINISHED_TASKINFO_CMD)
		CASE_NEW_MSG( CGMMessageTaskInfoCmd,			   	   ID_M2G_QUERY_ROLETITLE_CMD)
		CASE_NEW_MSG( CGMMessageTaskInfoResult,			   	   ID_G2M_TASKINFO_RESULT)
		CASE_NEW_MSG( CGMMessageRoleProtoCmd,			   	   ID_M2G_ROLEPROTO_CMD)
		CASE_NEW_MSG( CMessageResetRolePasswordRequest,		   ID_S2G_REQUEST_RESETROLEPASSWORD )
		CASE_NEW_MSG( CMessageResetRolePasswordResponse,	   ID_G2S_RESPONSE_RESETROLEPASSWORD )

		CASE_NEW_MSG( CMessageFCMPlayerListNotify,			   ID_G2S_FCMPLAYERLIST_NOTIFY)
		CASE_NEW_MSG( CMessageFCMPlayerListNotify,			   ID_G2S_FCMPLAYERRESET_NOTIFY)
		CASE_NEW_MSG( CMessageResetRolePasswordRequest,		   ID_S2G_FCMPLAYERRESET_REQUEST)
		CASE_NEW_MSG( CMessageFangChenMiRequest,			   ID_S2G_FANGCHENMI_REQUEST)
		CASE_NEW_MSG( CMessageFangChenMiResponse,			   ID_G2S_FANGCHENMI_RESPONSE)
		CASE_NEW_MSG( CMessageDestroyRepetionMap,			   ID_S2G_DESTROY_REPETION_MAP )
		CASE_NEW_MSG( CMessageBattleTeamWait,					ID_S2G_BATTLE_TEAM_WAIT )
		CASE_NEW_MSG( CMessageBattleWaitLeaveResponse,			ID_G2S_BATTLE_WAIT_LEAVE_RESPONSE )
		CASE_NEW_MSG( CMessageBattleJoinNotice, 				ID_G2S_BATTLE_JOIN_NOTICE )
		CASE_NEW_MSG( CMessageBattlePlayerJoin,					ID_S2G_BATTLE_PLAYER_JOIN )
		CASE_NEW_MSG( CMessageBattleWaitNotice,					ID_G2S_BATTLE_WAIT_NOTICE )
		CASE_NEW_MSG( CMessageBattleWaitLeaveRequest,			ID_S2G_BATTLE_WAIT_LEAVE_REQUEST )
		CASE_NEW_MSG( CMessageBattleCreateRequest,				ID_S2G_BATTLE_CREATE_REQUEST )
		CASE_NEW_MSG( CMessageBattleCreateResponse,				ID_G2S_BATTLE_CREATE_RESPONSE )
		CASE_NEW_MSG( CMessageTeamIntoRepetion,					ID_S2G_INTO_REPETION_TEAM )
		CASE_NEW_MSG( CMessageSingleIntoRepetion,				ID_S2G_INTO_REPETION_SINGLE )
		CASE_NEW_MSG( CMessageIntoRepetionNotify,				ID_G2S_INTO_REPETION_NOTIFY )
		CASE_NEW_MSG( CMessageG2SRepetionEventCode, 			ID_G2S_REPETION_EVENT_CODE )
		CASE_NEW_MSG( CMessageBattleWinnerNotify, 				ID_S2G_REPETION_BATTLE_WINNER_NOTIFY )
		CASE_NEW_MSG( CMessageS2GJoinOnTimeOpenBattleRequest, 	ID_S2G_JOIN_ONTIME_OPEN_BATTLE_REQUEST )
		CASE_NEW_MSG( CMessageS2GBattleClose, 					ID_S2G_BATTLE_CLOSE )
		CASE_NEW_MSG( CMessageS2GIntoCorpsBossRepetion,			ID_S2G_INTO_CORPSBOSS_REPETION )
		CASE_NEW_MSG( CMessgeS2GEndCorpsBossRepetion,			ID_S2G_END_CORPSBOSS_REPETION )

		CASE_NEW_MSG( CMessageBattleTeamFixWait , 				ID_S2G_BATTLE_TEAM_FIX_WAIT )
		CASE_NEW_MSG( CMessageBattleSingleFixWait , 			ID_S2G_BATTLE_SINGLE_FIX_WAIT)
		CASE_NEW_MSG( CMessageBattleWaitLeaveRequest , 			ID_S2G_BATTLE_FIX_WAIT_LEAVE_REQUEST)

		CASE_NEW_MSG( CGMMessageRoleDetailCmd,					ID_M2G_ROLEDETAIL_CMD)
		CASE_NEW_MSG( CGMMessageRoleEquipmentCmd,				ID_M2G_ROLEEQUIPMENT_CMD )
		CASE_NEW_MSG( CGMMessageRolePackInfoCmd,				ID_M2G_ROLEPACKINFO_CMD )
		CASE_NEW_MSG( CGMMessageRoleItemInPackCmd,				ID_M2G_ROLEITEMINPACK_CMD )
		CASE_NEW_MSG( CGMMessageRoleSkillInfoCmd,				ID_M2G_ROLESKILLINFO_CMD )
		CASE_NEW_MSG( CGMMessageRoleSkillDetailCmd,				ID_M2G_ROLESKILLDETAIL_CMD )

		CASE_NEW_MSG( CGMMessageRoleEquipmentDetailCmd,			ID_M2G_ROLEEQUIPMENT_DETAIL_CMD )
		CASE_NEW_MSG( CGMMessageRoleStorageDetailCmd,			ID_M2G_ROLESTORAGE_DETAIL_CMD )
		CASE_NEW_MSG( CGMMessageRoleEquipmentDetailResult,		ID_G2M_ROLEEQUIPMENT_DETAIL_RESULT  )
		CASE_NEW_MSG( CGMMessageRoleStorageDetailResult,		ID_G2M_ROLESTORAGE_DETAIL_RESULT  )

		CASE_NEW_MSG( CGMMessageRoleDetailResult,				ID_G2M_ROLEDETAIL_RESULT)
		CASE_NEW_MSG( CGMMessageRoleEquipmentResult,			ID_G2M_ROLEEQUIPMENT_RESULT )
		CASE_NEW_MSG( CGMMessageRolePackInfoResult,				ID_G2M_ROLEPACKINFO_RESULT )
		CASE_NEW_MSG( CGMMessageRoleItemInPackResult,			ID_G2M_ROLEITEMINPACK_RESULT )
		CASE_NEW_MSG( CGMMessageRoleSkillInfoResult,			ID_G2M_ROLESKILLINFO_RESULT )
		CASE_NEW_MSG( CGMMessageRoleSkillDetailResult,			ID_G2M_ROLESKILLDETAIL_RESULT )
		CASE_NEW_MSG( CMessageRoleChangeCampNotice,				ID_S2G_ROLECHANGECAMP_NOTICE )
		CASE_NEW_MSG( CMessageLoginSvrStartUpNotice,			ID_L2G_LOGINSVRSTARTUP_NOTICE )
		CASE_NEW_MSG( CMessageReSavePlayerRequest,				ID_G2S_REQUEST_RESAVEPLAYER )
		CASE_NEW_MSG( CMessageReSavePlayerResponse,				ID_S2G_RESPONSE_RESAVEPLAYER )

		CASE_NEW_MSG( CMessageGate2LoginHeartBeatNotice,		ID_G2L_HEARTBEAT_NOTICE )
		CASE_NEW_MSG( CMessgeSafeShutdownAck,					ID_S2G_ACK_SAFE_SHUTDOWN)
		CASE_NEW_MSG( CMessgeSafeShutdownNotify,				ID_G2S_NOTIFY_SAFE_SHUTDOWN )

		CASE_NEW_MSG( CGMMessageGoodsHistoryToNPCCmd,			ID_M2G_GOODSHISTORYTONPC_CMD )
		CASE_NEW_MSG( CGMMessageGoodsHistoryFromNPCCmd,			ID_M2G_GOODSHISTORYFROMNPC_CMD )
		CASE_NEW_MSG( CGMMessageGoodsHistoryBetweenUsersCmd,	ID_M2G_GOODSHISTORYBETWEENUSERS_CMD )
		CASE_NEW_MSG( CGMMessageGoodsHistoryCmd,				ID_M2G_GOODSHISTORY_CMD )
		CASE_NEW_MSG( CGMMessageOperationDetailCmd,				ID_M2G_OPERATIONDETAIL_CMD )
		CASE_NEW_MSG( CGMMessageUserSocialInfoCmd,				ID_M2G_USERSOCIALINFO_CMD )
		CASE_NEW_MSG( CGMMessageUserSocialInfoResult,			ID_G2M_USERSOCIALINFO_RESULT )
		CASE_NEW_MSG( CGMMsgRoleItemRecoveryCmd,			 	ID_M2G_ROLE_ITEM_RECOVERY_CMD)
		CASE_NEW_MSG( CGMMessageSqlQueryResult,			 		ID_G2M_SQL_QUERY_RESULT )
		CASE_NEW_MSG( CGMMessageQueryRoleMailCmd,			 	ID_M2G_QUERY_ROLE_MAIL_CMD)



		CASE_NEW_MSG( CGMMessageMultiExpCmd,					ID_M2G_MULTIEXP_CMD )
		CASE_NEW_MSG( CGMMessageCancelMultiExpCmd,				ID_M2G_CANCELMULTIEXP_CMD )
		CASE_NEW_MSG( CGMMessageUserLevelInfoCmd,				ID_M2G_USERLEVELINFO_CMD)
		CASE_NEW_MSG( CGMMessageUserInfoByIPCmd,				ID_M2G_USERINFOBYIP_CMD)
		CASE_NEW_MSG( CGMMsgUserInfoByIPResult,					ID_G2M_USERINFOBYIP_RESULT)

		CASE_NEW_MSG( CGMMessageTrusteeUserCmd,					ID_M2G_TRUSTEEUSER_CMD)
		CASE_NEW_MSG( CGMMessageUnTrusteeUserCmd,				ID_M2G_UNTRUSTEEUSER_CMD)

		CASE_NEW_MSG( CGMMessageRoleItemOperatedCmd,			ID_M2G_REQUEST_ROLEITEMOPERATE)
		CASE_NEW_MSG( CGMMessageRoleMoneyOperateCmd,			ID_M2G_ROLEMONEYOPERATE)
		CASE_NEW_MSG( CGMMessageRoleExpCmd,						ID_M2G_REQUEST_ROLEEXP)
		CASE_NEW_MSG( CGMMessageRoleItemLevelUpCmd,				ID_M2G_REQUEST_ROLEITEMLEVELUP)

		CASE_NEW_MSG( CMessageUserItemListRequest,				ID_S2E_USERITEMLIST_REQUEST )
		CASE_NEW_MSG( CMessageUserItemListResponse,				ID_E2S_USERITEMLIST_RESPONSE )
		CASE_NEW_MSG( CMessageUserItemMinusRequest,				ID_S2E_USERITEMMINUS_REQUEST )
		CASE_NEW_MSG( CMessageUserItemMinusResponse,			ID_E2S_USERITEMMINUS_RESPONSE )
		CASE_NEW_MSG( CMessageActivityListRequest,				ID_S2E_ACTIVITYLIST_REQUEST )
		CASE_NEW_MSG( CMessageActivityListResponse,				ID_E2S_ACTIVITYLIST_RESPONSE )

		CASE_NEW_MSG( CGMMessageFreezeAccountBatCmd,			ID_M2G_REQUEST_FASTFREEZE_CMD )
		CASE_NEW_MSG( CGMMessageFreezeAccountBatCmd,			ID_M2G_REQUEST_UNFASTFREEZE_CMD )
		CASE_NEW_MSG( CGMMessageIpBindCmd,						ID_M2G_IPBIND_CMD )
		CASE_NEW_MSG( CGMMessageCorpsTotalCmd,					ID_M2G_REQUEST_CORPSTOTAL_CMD )
		CASE_NEW_MSG( CGMMessageCorpsDetailCmd, 				ID_M2G_REQUEST_CORPSDETAIL_CMD )
		CASE_NEW_MSG( CGMMessageCorpsInfoCmd, 					ID_M2G_REQUEST_CORPSINFO_CMD )
		CASE_NEW_MSG( CGMMessageRoleInfoCmd, 					ID_M2G_REQUEST_ROLEINFO_CMD )

		
		CASE_NEW_MSG( CMessageNotifyTracedPlayer,				ID_G2S_TRACEDPLAYER_NOTIFY )

		CASE_NEW_MSG( CMessageFamilyIDNameNotify,				ID_G2S_NOTIFY_FAMILYIDNAME )
		CASE_NEW_MSG( CMessageNpcListNotify,					ID_G2S_NOTIFY_NPCLIST )
		CASE_NEW_MSG( CMessageBidTimeOutNotify,					ID_S2G_NOTIFY_BIDTIMEOUT )
		CASE_NEW_MSG( CMessageBattleStartRequest,				ID_S2G_REQUEST_BATTLESTART )
		CASE_NEW_MSG( CMessageBattleStartResponse,				ID_G2S_RESPONSE_BATTLESTART )
		CASE_NEW_MSG( CMessageBattleEndNotify,					ID_S2G_NOTIFY_BATTLEEND )
		CASE_NEW_MSG( CMessageBidRestartNotify,					ID_G2S_NOTIFY_BIDRESTART )
		CASE_NEW_MSG( CMessageCreateRepetionNotify,				ID_G2S_NOTIFY_CREATE_REPETION )
		CASE_NEW_MSG( CMessageCreateRepetionErr,				ID_S2G_CREATE_REPETION_ERR );

		CASE_NEW_MSG( CMessageCorpsBidTimerStopRequest,			ID_S2G_REQUEST_BIDTIMERSTOP )
		CASE_NEW_MSG( CMessageCorpsJoinBattleNotice,			ID_G2S_NOTIFY_JOINBATTLE )
		CASE_NEW_MSG( CMessageNotifyBattleResult2Gate,			ID_S2G_REQUEST_BATTLERESULT )
		CASE_NEW_MSG( CMessageCorpsBattleFlagRequest,			ID_S2G_REQUEST_BATTLEFLAG )
		CASE_NEW_MSG( CMessageCorpsBattleFlagResponse,			ID_G2S_RESPONSE_BATTLEFLAG )
		CASE_NEW_MSG( CMessageCorpsStartBattleRequest,			ID_S2G_REQUEST_STARTCOPRSBATTLE )
		CASE_NEW_MSG( CMessageCorpsBattleMapNotice,				ID_S2G_REQUEST_BATTLEMAP)
		
		CASE_NEW_MSG( CMessageChangeFamilyContributeNotify,		ID_S2G_NOTIFY_CHANGEFAMILYCONTRIBUTE )
		CASE_NEW_MSG( CMessageChangeFamilyGloryNotify,			ID_S2G_NOTIFY_CHANGEFAMILYGLORY	)
		CASE_NEW_MSG( CMessageChangeFamilyMoneyNotify,			ID_S2G_NOTIFY_CHANGEFAMILYMONEY	)
		
		CASE_NEW_MSG( CMessageChangeCorpsContributeNotify,		ID_S2G_NOTIFY_CHANGECORPSCONTRIBUTE )
		CASE_NEW_MSG( CMessageChangeCorpsGloryNotify,			ID_S2G_NOTIFY_CHANGECORPSGLORY )
		CASE_NEW_MSG( CMessageChangeCorpsMoneyNotify,			ID_S2G_NOTIFY_CHANGECORPSMONEY )
		
		CASE_NEW_MSG( CMessageFamilyApExpChangedNotify,			ID_S2G_NOTIFY_FAMILYAPANDEXPCHANGED )
		CASE_NEW_MSG( CMessageCorpsApExpChangedNotify,			ID_S2G_NOTIFY_CORPSAPEXPCHANGED )
		
		CASE_NEW_MSG( CMessageChangeFamilyStabilityNotify,		ID_S2G_NOTIFY_CHANGEFAMILYSTABILITY )
		CASE_NEW_MSG( CMessageChangeCorpsBossTimes,				ID_S2G_CHANGE_CORPSBOSSTIMES )
		CASE_NEW_MSG( CMessageChangeCorpsBossRefineValue,		ID_S2G_CHANGE_CORPSBOSSREFINEVALUE )
		CASE_NEW_MSG( CMessageChangeCorpsCollectActive,			ID_S2G_CHANGE_CORPSCOLLECTACTIVE )
					  

		// 日志服务器
		CASE_NEW_MSG( CMessageHandleTimeOut,					ID_L2L_HANDLETIMEOUT )
		CASE_NEW_MSG( CMessageLogRoleOpTask,					ID_S2L_ROLE_OPTASK )
		CASE_NEW_MSG( CMessageLogRoleBeKilled,					ID_S2L_ROLE_KILLED )
		CASE_NEW_MSG( CMessageLogRoleLogout,					ID_S2L_ROLE_LOGOUT )
		CASE_NEW_MSG( CMessageLogRoleOpExp,						ID_S2L_ROLE_OPEXP )
		CASE_NEW_MSG( CMessageLogRoleOpMoney,					ID_S2L_ROLE_OPMONEY )
		CASE_NEW_MSG( CMessageLogRoleOpItem,					ID_S2L_ROLE_OPITEM )
		CASE_NEW_MSG( CMessageLogRoleGhost,						ID_S2L_ROLE_GHOST )
		CASE_NEW_MSG( CMessageLogRoleUpgrade,					ID_S2L_ROLE_UPGRADE )
		CASE_NEW_MSG( CMessageLogItemUpgrade,					ID_S2L_ITEM_UPGRADE )
		CASE_NEW_MSG( CMessageLogOrgeDropItem,					ID_S2L_ORGE_DROPED )
		CASE_NEW_MSG( CMessageLogItemDestroy,					ID_S2L_ITEM_DISPEAR )
		CASE_NEW_MSG( CMessageLogFamily,						ID_S2L_FAMILY_ACTION )
		CASE_NEW_MSG( CMessageLogCorps,							ID_S2L_CORPS_ACTION )
		CASE_NEW_MSG( CMessageLogMail,							ID_S2L_MAIL )
		CASE_NEW_MSG( CMessageLogChat,							ID_G2L_ROLE_CHAT )
		CASE_NEW_MSG( CMessageLogRepetion,						ID_G2L_REPETION )
		CASE_NEW_MSG( CMessageLogValuableEquip,					ID_S2L_VALUABLE_EQUIP )
		CASE_NEW_MSG( CMessageLogBossOgreDie,					ID_S2L_BOSSORGEDIE )
		CASE_NEW_MSG( CMessageLifeSkill,						ID_S2L_LIFESKILL )
		CASE_NEW_MSG( CMessageExchangeLog,						ID_S2L_EXCHANGELOG_LOG )
		CASE_NEW_MSG( CMessageLogMatrix,						ID_S2L_MATRIX )
		CASE_NEW_MSG( CMessageLogWorld,							ID_S2L_WORLD )
		CASE_NEW_MSG( CMessageLogRoleDamage,					ID_S2L_ROLE_DAMAGE )
		CASE_NEW_MSG( CMessageLogRoleBuffer,					ID_S2L_ROLE_BUFFER )
		CASE_NEW_MSG( CMessageLogRoleRepeScore,					ID_S2L_ROLE_REPE_SCORE )



		CASE_NEW_MSG( CMessageLockGoodsRequest,					ID_S2G_LOCKGOODS_REQUEST)
		CASE_NEW_MSG( CMessageLockGoodsResponse,				ID_G2S_LOCKGOODS_RESPONSE)
		CASE_NEW_MSG( CMessageIbItemRemainNotice,				ID_G2S_NOTIFY_IBSTOREPROMOTION)

		CASE_NEW_MSG( CMessageSynIbStoreRequest,				ID_S2G_SYNIBSTORE_REQUEST)
		CASE_NEW_MSG( CMessageSynIbStoreResponse,				ID_G2S_SYNIBSTORE_RESPONSE)
		CASE_NEW_MSG( CMessageUserChargeNotice,					ID_M2G_USER_CHARGE_NOTICE)
		CASE_NEW_MSG( CMessage4EQueryYuanBaoRequest,			ID_S2G_QUERYYUANBAO_REQUEST)
		CASE_NEW_MSG( CMessage4EQueryYuanBaoResponse,			ID_G2S_QUERYYUANBAO_RESPONSE)
		CASE_NEW_MSG( CMessage4EDrawYuanBaoRequest,				ID_S2G_DRAWYUANBAO_REQUEST)
		CASE_NEW_MSG( CMessage4EDrawYuanBaoResponse,			ID_G2S_DRAWYUANBAO_RESPONSE)
		CASE_NEW_MSG( CMessageGateEntityGMMove,					ID_S2G_ENTITY_GM_MOVE )
		CASE_NEW_MSG( CMessageSceneEntityGMMove,				ID_G2S_ENTITY_GM_MOVE )
		CASE_NEW_MSG( CMessageChangeGateNotify,					ID_S2G_CHANGEGATE_NOTIFY )
		CASE_NEW_MSG( CMessageS2GRoleMove,						ID_S2G_ROLE_MOVE )
		CASE_NEW_MSG( CMessageG2SRoleMove,						ID_G2S_ROLE_MOVE )

		//同步功能付费
		CASE_NEW_MSG( CMessageSynFuncStoreRequest,				ID_S2G_SYNFUNCSTORE_REQUEST)
		CASE_NEW_MSG( CMessageSynFuncStoreResponse,				ID_G2S_SYNFUNCSTORE_RESPONSE)

		CASE_NEW_MSG( CMessagePurchaseIbItemRequest,			ID_S2G_PURCHASEIBITEM_REQUEST)
		CASE_NEW_MSG( CMessagePurchaseIbItemResponse,			ID_G2S_PURCHASEIBITEM_RESPONSE)
		CASE_NEW_MSG( CMessageRoleIbTransNotify,				ID_S2G_ROLEIBTRANS_NOTIFY)
		CASE_NEW_MSG( CMessageUserIbExchangeNotice,				ID_G2E_IBEXCHANGE_NOTICE)
		

		// 排行榜
		CASE_NEW_MSG( CMessagePersonalRankInfoNotify,			ID_S2G_NOTIFY_PERSONALRANKINFO )
		CASE_NEW_MSG( CMessagePersonalRankInfoRequest,			ID_G2S_REQUEST_PERSONALRANKINFO )
		CASE_NEW_MSG( CMessageNewRankTopTenNotify,				ID_G2S_NOTIFY_NEWTOPTEN )
		CASE_NEW_MSG( CMessageObtainTopTenRequest,				ID_S2G_REQUEST_TOPTEN )
		CASE_NEW_MSG( CMessageObtainTopTenResponse,				ID_G2S_RESPONSE_TOPTEN )
		CASE_NEW_MSG( CMessageRankRemove,						ID_S2G_RANK_REMOVE )
		CASE_NEW_MSG( CMessageActivityEventNotify,				ID_G2S_ACTIVITY_EVENT_NOTIFY )

		// 验证服务器
		CASE_NEW_MSG( CMessageServerValidateRequest,			ID_S2V_SERVERVALIDATE_REQUEST )
		CASE_NEW_MSG( CMessageServerValidateResponse,			ID_V2S_SERVERVALIDATE_RESPONSE )

		CASE_NEW_MSG( CMessageChangeJudgeScoreReuqest,			ID_S2G_REQUEST_CHANGESCORE )
		CASE_NEW_MSG( CMessageJudgeModifySuccessNotice,			ID_G2S_NOTIFY_JUDGEMODIFY )
		
		// 家族军团
		CASE_NEW_MSG( CMessageBuyFamilyItemRequest,				ID_S2G_REQUEST_BUYFAMILYITEM )
		CASE_NEW_MSG( CMessageBuyFamilyItemResponse,			ID_G2S_RESPONSE_BUYFAMILYITEM )
		CASE_NEW_MSG( CMessageClearFamilyRepetionInfoNotify,	ID_G2S_NOTIFY_CLEARFAMILYREPETIONIFO )
		
		// CASE_NEW_MSG( CMessageBuyCorpsRedPaperRequest,			ID_C2S_REQUEST_BUYCORPSREDPAPER )
		// CASE_NEW_MSG( CMessageBuyCorpsRedPaperResponse,			ID_S2C_RESPONSE_BUYCORPSREDPAPER )
		CASE_NEW_MSG( CMessageBuyCorpsItemRequest,	ID_S2G_REQUEST_BUYCORPSITEM )
		CASE_NEW_MSG( CMessageBuyCorpsItemResponse,  ID_G2S_RESPONSE_BUYCORPSITEM )

		CASE_NEW_MSG( CMessageFamilyExchangeRequest,			ID_S2G_REQUEST_FAMILYEXCHANGE )
		CASE_NEW_MSG( CMessageFamilyExchangeResponse,			ID_G2S_RESPONSE_FAMILYEXCHANGE ) 
		
		CASE_NEW_MSG( CMessageCorpsExchangeRequest,				ID_S2G_REQUEST_CORPSEXCHANGE ) 
		CASE_NEW_MSG( CMessageCorpsExchangeResponse,			ID_G2S_RESPONSE_CORPSEXCHANGE )
		
		CASE_NEW_MSG( CMessageAddFamilyExchangeItemNotify,		ID_S2G_NOTIFY_ADDFAMILYEXCHANGEITEM )
		CASE_NEW_MSG( CMessageAddCorpsExchangeItemNotify,		ID_S2G_REQUEST_ADDCORPSEXCHANGEITEM )
		CASE_NEW_MSG( CMessageRollCorpsMoneyRequest,			ID_S2G_REQUEST_CALLBACK_CORPSMONEY )

		// 国家
		CASE_NEW_MSG( CMessageChangeWizardRequest,			ID_S2G_REQUEST_CHANGEWIZARD )
		CASE_NEW_MSG( CMessageChangeWizardResponse,			ID_G2S_RESPONSE_CHANGEWIZARD )
		CASE_NEW_MSG( CMessageChangeConstructRequest,		ID_S2G_REQUEST_CHANGECONSTRUCT )
		CASE_NEW_MSG( CMessageChangeConstructResponse,		ID_G2S_REQPONSE_CHANGECONSTRUCT )
		CASE_NEW_MSG( CMessageChangePowerRequest,			ID_S2G_REQUEST_CHANGEPOWER )
		CASE_NEW_MSG( CMessageChangePowerResponse,			ID_G2S_RESPONSE_CHANGEPOWER )
		CASE_NEW_MSG( CMessageUseFunctionNotify,			ID_G2S_NOTIFY_USEFUNCTION )
		CASE_NEW_MSG( CMessageSetOfficialNotify,			ID_G2S_NOTIFY_SETOFFICIAL )
		CASE_NEW_MSG( CMessageChangeFriendNotify,			ID_S2G_NOTIFY_CHANGEFRIEND )
		CASE_NEW_MSG( CMessageKillPersonRequest,			ID_S2G_REQUEST_KILLPERSON )
		CASE_NEW_MSG( CMessageKillPersonResponse,			ID_G2S_RESPONSE_KILLPERSON )		
		CASE_NEW_MSG( CMessageSyncNationNotify,				ID_G2S_NOTIFY_SYNCNATION )
		CASE_NEW_MSG( CMessageUpdateNationTitleNotice,		ID_G2S_NOTIFY_NATIONTITLE)
		CASE_NEW_MSG( CMessageGetNationTitleListRequest,	ID_S2G_REQUEST_NATIONTITLELIST )
		CASE_NEW_MSG( CMessageGetNationTitleListResponse,	ID_G2S_RESPONSE_NATIONTITLELIST )
		CASE_NEW_MSG( CMessageTrySummonRequest,				ID_S2G_REQUEST_TRYSUMMON )
		CASE_NEW_MSG( CMessageTrySummonResponse,			ID_G2S_RESPONSE_TRYSUMMON )
		CASE_NEW_MSG( CMessageRealSummonRequest,			ID_S2G_REQUEST_REALSUMMON )
		CASE_NEW_MSG( CMessageRealSummonNotify,				ID_G2S_NOTIFY_REALSUMMON )
		CASE_NEW_MSG( CMessageWarChangeNotify,				ID_S2G_NOTIFY_WARCHANGE )
		CASE_NEW_MSG( CMessageReliveChangeNotify,			ID_G2S_NOTIFY_RELIVECHANGE )
		CASE_NEW_MSG( CMessageObtainWarHonorNotify,			ID_S2G_NOTIFY_OBTAINWARHONOR )
		CASE_NEW_MSG( CMessageSyncWarStatusNotify,			ID_G2S_NOTIFY_SYNCWARSTATUS )
		CASE_NEW_MSG( CMessageWarControlNotify,				ID_S2G_NOTIFY_WARCONTROL )
		CASE_NEW_MSG( CMessageWarStoneOverNotify,			ID_S2G_NOTIFY_WARSTONEOVER )
		CASE_NEW_MSG( CMessageCreateWarStoneNotify,			ID_G2S_NOTIFY_CREATEWARSTONE )
		CASE_NEW_MSG( CMessageAddTaxNotify,					ID_S2G_NOTIFY_ADDTAX )
		CASE_NEW_MSG( CMessageSpecialEventNotify,			ID_S2G_NOTIFY_SPECIALEVENT )
		CASE_NEW_MSG( CMessageSyncGTAndLWNotify,			ID_G2S_NOTIFY_SYNCGTANDLW )

		// 阵营
		CASE_NEW_MSG( CMessageCampWeakListRequest,			ID_S2G_CAMPWEAKLIST_REQUEST )
		CASE_NEW_MSG( CMessageCampWeakListResponse,			ID_G2S_CAMPWEAKLIST_RESPONSE )
		CASE_NEW_MSG( CMessageCampWeakListResponse,			ID_G2S_CAMPWAEKLIST_NOTICE )
		CASE_NEW_MSG( CMessageCampAddHonerScore,			ID_S2G_ADDHONER_NOTICE )

		// 活动
		CASE_NEW_MSG( CMessageTodayActivitysNotify,			ID_G2S_TODAYACTIVITYS_LIST_NOTIFY	 )
		CASE_NEW_MSG( CMessageActivityChangeNotify,			ID_G2S_ACTIVITY_CHANGE_NOTIFY		 )


		// 发布任务消息  game - gate

		CASE_NEW_MSG( CMessageGatePublishGroupRequest,				ID_S2G_REQUEST_PLAYERPBULISHTASK )
		CASE_NEW_MSG( CMessageGatePublishGroupResponse,				ID_G2S_RESPONSE_PBULISHTASKRESULT )
		CASE_NEW_MSG( CMessageGateCheckGroupPublishRequest,			ID_S2G_REQUEST_CHECKPUBLISHBOARD )
		CASE_NEW_MSG( CMessageGateCheckGroupPublishResponse,		ID_G2S_RESPONSE_CHECKPUBLISHBOARD )

		default:
		{
			LOG_ERROR( "default", "invalid msg id %u", unMessageID );
			break;
		}
	}

	return tpMessage;
}
#endif // !LINUX


