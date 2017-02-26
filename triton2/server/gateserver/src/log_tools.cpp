#include "log_tools.h"
#include "gatelogic.h"
using namespace GateServer;

CMessage* BuildFamilyLogMessage(uint32_t nFamilyID, uint32_t nOpRoleID, 
		        EOrganAction emAction, EFamilyAttr nAttrID, int nParam1, int nParam2, int nParam3 )
{
	static CMessage tMessage;
	static CMessageLogFamily proto;

	tMessage.Clear( );
	proto.Clear( );

	tMessage.set_msgpara( (uint32_t)&proto );
	tMessage.mutable_msghead( )->set_messageid(ID_S2L_FAMILY_ACTION);

	proto.set_familyid(nFamilyID);
	proto.set_oproleid(nOpRoleID);
	proto.set_optime(time(0));
	proto.set_optype(emAction);
	proto.set_attrid(nAttrID);
	proto.set_param1(nParam1);
	proto.set_param2(nParam2);
	proto.set_param3(nParam3);

	return &tMessage;
}

CMessage* BuildCorpsLogMessage(uint32_t nCorpsID, uint32_t nOpRoleID,
		        EOrganAction emAction, ECorpsAttr nAttrID, int nParam1, int nParam2, int nParam3 )
{
	static CMessage tMessage;
	static CMessageLogCorps proto;

	tMessage.Clear( );
	proto.Clear( );

	tMessage.set_msgpara( (uint32_t)&proto );
	tMessage.mutable_msghead( )->set_messageid(ID_S2L_CORPS_ACTION);

	proto.set_corpsid(nCorpsID);
	proto.set_oproleid(nOpRoleID);
	proto.set_optime(time(0));
	proto.set_optype(emAction);
	proto.set_attrid(nAttrID);
	proto.set_param1(nParam1);
	proto.set_param2(nParam2);
	proto.set_param3(nParam3);

	return &tMessage;
}

CMessage* BuildWorldLogMessage(int nWorldID, int nAction, int nParam1, int nParam2, int nParam3 )
{
	static CMessage tMessage;
	static CMessageLogWorld proto;

	tMessage.Clear( );
	proto.Clear( );

	tMessage.set_msgpara( (uint32_t)&proto );
	tMessage.mutable_msghead( )->set_messageid(ID_S2L_WORLD);

	proto.set_worldid(nWorldID);
	proto.set_optime(time(0));
	proto.set_optype(nAction);
	proto.set_param1(nParam1);
	proto.set_param2(nParam2);
	proto.set_param3(nParam3);

	return &tMessage;
}

void LogFamilyAction(uint32_t nFamilyID, uint32_t nOpRoleID, 
		EOrganAction emAction, EFamilyAttr nAttrID, int nParam1, int nParam2, int nParam3)
{
	CMessage* pMessage = BuildFamilyLogMessage( nFamilyID, nOpRoleID,
			emAction, nAttrID, nParam1, nParam2, nParam3);

	if ( pMessage != NULL)
	{
		CGateServer::GetSingletonPtr( )->Send2Log( pMessage, nFamilyID);     
	}
}

void LogCorpsAction(uint32_t nCorpsID, uint32_t nOpRoleID, 
		EOrganAction emAction, ECorpsAttr nAttrID, int nParam1, int nParam2, int nParam3)
{
	CMessage* pMessage = BuildCorpsLogMessage( nCorpsID, nOpRoleID,
			emAction, nAttrID, nParam1, nParam2, nParam3 );

	if ( pMessage != NULL)
	{
		CGateServer::GetSingletonPtr( )->Send2Log( pMessage, nCorpsID);     
	}
}

void LogWorldAction(int nWorldID, int nAction, int nParam1, int nParam2, int nParam3)
{
	CMessage* pMessage = BuildWorldLogMessage( nWorldID, nAction, nParam1, nParam2, nParam3 );

	if ( pMessage != NULL)
	{
		CGateServer::GetSingletonPtr( )->Send2Log( pMessage, nWorldID);     
	}
}


// 记录聊天消息
void LogChatMsg( int nRoleID, const char* szRoleName, const char* szMsg, int nGateWayID, int nChannel, int nIP /*= 0*/ )
{
	static CMessage tMessage;
	static CMessageLogChat proto;

	tMessage.Clear( );
	proto.Clear( );

	tMessage.set_msgpara( (uint32_t)&proto );
	tMessage.mutable_msghead( )->set_messageid(ID_G2L_ROLE_CHAT);

	proto.set_roleid( nRoleID );
	proto.set_chatmsg( szMsg != NULL ? szMsg : "N/A" );
	proto.set_gatewayid( nGateWayID );
	proto.set_channel( nChannel );
	proto.set_ip( nIP );
	proto.set_optime( time(NULL) );
	proto.set_rolename( szRoleName != NULL ? szRoleName : "N/A" );

	CGateServer::GetSingletonPtr( )->Send2Log( &tMessage, nRoleID );     
};