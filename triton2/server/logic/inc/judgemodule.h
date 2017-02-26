/*
 *	JudgeModule.h
 */

// Include Head Files

#pragma once
#include "servercore.h"
#include "servertool.h"

#include "errcode.h"
#include "judgetype.h"
#include "judgemessage_pb.hxx.pb.h"

class CMessage;

class CJudgeModule : public CLogicModule, public CSingleton< CJudgeModule >
{
public:
	static char		sModuleName[ NAME_LENGTH ];

public:
	CJudgeModule( );
	~CJudgeModule();
	static unsigned int CountSize();	
public:
	virtual void OnLaunchServer( );
	virtual void OnExitServer( );
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
	virtual int OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName );
	virtual void OnRouterMessage( CMessage* pMessage );
	virtual void OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage );
	virtual void OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst );
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );
	virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode );
	virtual void OnLoadConfig( );
	virtual bool OnCheckLaunch(){ return true; }
	
	void OnDBSession( CMessage* npMessage, int nParam1, int nParam2 );

	// internal interface
	int CanJudge( CEntityPlayer* pPlayer );

	// external interface

	void OnMessageClient2Gate( CEntityPlayer* pPlayer, CMessage* pMessage );

	void OnGateModifyJudgeNotice( CMessage* npMessage );

	// DB ²Ù×÷ [12/10/2009 Macro]
	void OnEventQueryJudge( int nAccountID, int nRoleID, int nType, int nParam = 0 );
	void OnDBSessionQueryJudge( CMessage* npMessage, int nParam1, int nParam2 );
};
