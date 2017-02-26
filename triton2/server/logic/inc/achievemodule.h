/*
 *	Achieve Module
 *  Created By: Gao Hong
 */

// Include Head Files
#pragma once

#include "servercore.h"
#include "servertool.h"
#include "scenelogic.h"

#include "errcode.h"
#include "achieve.h"
#include "achievetype.h"
#include "achievemessage_pb.hxx.pb.h"

// 成就逻辑处理
class CAchieveModule : public CLogicModule, public CSingleton< CAchieveModule >
{
public:	
	typedef lk::hash_map< int, CAchieveCfgItem, MAX_ACHIEVE_NUM > ACHIEVE_CFG_LIST;
	// 场景服务的玩家成就数据列表
	typedef lk::hash_map< int, int, SERVER_CAP_PLAYER > ACHIEVE_UNIT_TYPE;
	ACHIEVE_UNIT_TYPE	mAchieveUnitList;
	ACHIEVE_CFG_LIST	mAchieveCfgList;
	
public:
	CAchieveModule() : CLogicModule( sModuleName )	
	{
		mAchieveUnitList.initailize();
		mAchieveCfgList.initailize();
	}
	~CAchieveModule()
	{ 
		mAchieveUnitList.clear();
		mAchieveCfgList.clear();
	}
	static char		sModuleName[ NAME_LENGTH ];
	static unsigned int CountSize();

	virtual void OnLaunchServer( );
	virtual void OnExitServer( );
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
	virtual int	 OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName );
	virtual void OnRouterMessage( CMessage* pMessage );
	virtual void OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage );
	virtual void OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst );
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );
	virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode );
	virtual void OnLoadConfig( );
	virtual bool OnCheckLaunch( ) { return true; }

	// Internal Interface
public:
	int	LoadAchieveCfg();
	int LoadAchieveDBData( int nAccountID, int nCharID );

	int GetAchieveDataUnit( int nCharID );
	int DelAchieveDataUnit( int nCharID );
	CAchieveCfgItem*  GetAchieveCfgItem( int nAchieveID );


	void OnDBSession( CMessage* npMessage, int nParam1, int nParam2 );
	void OnSessionQueryAchieveDB( CMessage* npMessage, int nParam1, int nParam2 );
	void OnSessionSaveAchieveDB( CMessage* npMessage, int nParam1, int nParam2 );

	// 加载成就配置的回调函数
	static void LoadAchieveConfigCallback(const char* pcConfigPath);
	// External Interface
public:
	// 成就上报处理
	void OnMessageReportAchieveRequest( CEntityPlayer* npPlayer, CMessage* npMessage );
	// 获取单个成就数据请求
	void OnMessageObtainSingleAchieveRequest( CEntityPlayer* npPlayer, CMessage* npMessage );
	// 获取所有成就数据请求
	void OnMessageObtainAllAchieveRequest( CEntityPlayer* npPlayer, CMessage* npMessage );
	// 获取功德数据请求
	void OnMessageObtainAllHonorRequest( CEntityPlayer* npPlayer, CMessage* npMessage );

	void SendAchieveFinishNotice( CEntityPlayer* npPlayer, int nAchieveID );

	void OnEventSaveAchieveSession( CEntityPlayer* npPlayer );
	void OnEventInitAchieveDB( CEntityPlayer* npPlayer );
	void OnEventSendAward( CEntityPlayer* npPlayer, int nAwardID, int nID );
	void OnEventHonor( CEntityPlayer* npPlayer, CAchieveDataUnit* npUnit, CAchieveCfgItem* npCfgItem );
	void OnEventClearAchieveMemory( CEntityPlayer* npPlayer );

	// 其他模块触发成就 [12/10/2009 Macro]
	void OnEventFireAchieve( CEntityPlayer* npPlayer, int nValue, int nParam );

	void OnEventInsertHonor( CEntityPlayer* npPlayer, int nMapID );
	void OnMessageGetStageBonusRequest( CEntityPlayer* npPlayer, CMessage* npMessage );
};
