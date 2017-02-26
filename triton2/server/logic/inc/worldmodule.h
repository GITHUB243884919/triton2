#pragma once

#include "countrymessage_pb.hxx.pb.h"
#include "sceneobj_define.h"
#include "sceneobj_manager.h"
#include "propertymodule.h"
#include "servercore.h"
#include "servertool.h"
#include "weakprotect.h"

class StoneInfo
{
public:
	int	mStoneID;

	int mStoneX;

	int mStoneY;

	int mStoneEntityID;

	StoneInfo() 
	{
		mStoneID = 0;
		mStoneX = 0;
		mStoneY = 0;
		mStoneEntityID = 0;
	}

	~StoneInfo() {}
};

class GTLWInfo
{
public:
	int	mSourceWorld;
	int	mTargetWorld;
	int	mEndTime;

	GTLWInfo()
	{
		mSourceWorld = 0;
		mTargetWorld = 0;
		mEndTime = 0;
	}

	GTLWInfo( int nSrcWorld, int nDesWorld, int nTime )
	{
		mSourceWorld = nSrcWorld;
		mTargetWorld = nDesWorld;
		mEndTime = nTime;
	}

	~GTLWInfo() {}
};

class CWorldModule : public CLogicModule, public CSingleton< CWorldModule >
{
public:
	static char	sModuleName[ NAME_LENGTH ];

	int mNationStatus[ WORLD_NUM ][ WORLD_NUM ];

	ComplexPos mSummonPos[ WORLD_NUM ];

	int mKingID[ WORLD_NUM ][ COUNTRY_NUM ];

	CWTimer mSaveTimer;

	bool mSaveFin;

	int	mWarSerial;

	typedef lk::vector< int , 100 > TeleList;

	TeleList mTeleList;

	// 新加灵石位置
	StoneInfo mStone[ MAX_WAR_CAMP ];

	int mRateOrder;

	CWTimer mStoneTimer;

	// 获胜方
	int mWinWorld;

	typedef lk::vector< GTLWInfo, 16 > GTAndLWList;

	GTAndLWList	mGTList;

	GTAndLWList	mLWList;
public:
	// 弱国保护功能
	CWeakProtect m_WeakProtecter;

public:
	CWorldModule() : CLogicModule( sModuleName )
	{
		if ( mShmPtr->GetInitMode() == SHM_INIT )
		{
			memset( mNationStatus, 0, sizeof(mNationStatus));
			memset( mKingID, 0, sizeof( mKingID ));
			mSaveTimer = CWTimer(0);
			mSaveFin = false;;
			mWarSerial = 0;
			mTeleList.initailize();
			mRateOrder = 0;
			mStoneTimer = CWTimer(0);
			mWinWorld = 0;
			m_WeakProtecter.Init();
			mGTList.initailize();
			mLWList.initailize();
		}	
	}

	static unsigned int CountSize();

public:
	virtual void OnLaunchServer( );
	virtual void OnExitServer();
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
	virtual int OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName );
	virtual void OnRouterMessage( CMessage* pMessage );
	virtual void OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage );
	virtual void OnCreateEntity( CEntity* pEntity , CCreator* pCreator, bool vFirst );
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );
	virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode );

	// 加载配置
	virtual void OnLoadConfig( ) ;

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( ) { return true; }

	void OnGateMessage( CMessage *pMsg );

	inline int GetNationStatus( int vSrcWorld, int vDesWorld )
	{
		if ( vSrcWorld <= WORLD_START || vSrcWorld >= WORLD_NUM || vDesWorld <= WORLD_START || vDesWorld >= WORLD_NUM )
		{
			return WORLD_STATUS_NONE;
		}
		return mNationStatus[ vSrcWorld ][ vDesWorld ];
	}

	inline bool IsKing( int vCharID, int vWorldID, int vCountryID )
	{
		if ( vWorldID <= WORLD_START || vWorldID >= WORLD_NUM || vCountryID <= COUNTRY_NONE || vCountryID >= COUNTRY_NUM )
		{
			return false;
		}
		if ( vCharID == mKingID[ vWorldID ][ vCountryID ] )
		{
			return true;
		}
		return false;
	}
public:
	void OnMessageCountryInfo( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageJoinCountry( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageLeaveCountry( CEntityPlayer* pPlayer, CMessage* pMessage );

	void OnMessageSetOfficial( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageUnSetOfficial( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageUseFunction( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageOfficialInfo( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageUseTimes( CEntityPlayer* pPlayer, CMessage* pMessage );

	void OnMessageWorldQuest( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageWorldAnswer( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageWorldInfo( CEntityPlayer* pPlayer, CMessage* pMessage );

	void OnMessageGetAward( CEntityPlayer* pPlayer, CMessage* pMessage );

	void OnMessageSubmitWar( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageEnterWar( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageQueryWarCamp( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageQueryWarNum( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageQueryWarInfo( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageQueryWarRank( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageBidCamp( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageCampBidInfo( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageCampResInfo( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageGetWarAward( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageQueryWarAward( CEntityPlayer* pPlayer, CMessage* pMessage );

	void OnMessageModifyRate( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageGetTax( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageRefuseSummon( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageAccuseKing( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessagePowerInfo( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageOfficialChange( CMessage *pMessage );	 // 场景处理职位改变的消息

	void OnMessageMakeGuoTan( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnMessageMakeLuanWu( CEntityPlayer* pPlayer, CMessage* pMessage );
public:
	void OnMessageGSCountryInfoResponse( CMessage* pMessage );
	void OnMessageGSJoinCountryResponse( CMessage* pMessage );
	void OnMessageGSLeaveCountryResponse( CMessage* pMessage );

	void OnMessageGSSetOfficialResponse( CMessage* pMessage );
	void OnMessageGSUnSetOfficialResponse( CMessage* pMessage );
	void OnMessageGSUseFunctionResponse( CMessage* pMessage );
	void OnMessageGSOfficialInfoResponse( CMessage* pMessage );
	void OnMessageGSUseTimesResponse( CMessage* pMessage );
	void OnMessageGSUseFunctionNotify( CMessage* pMessage );
	void OnMessageGSSetOfficialNotify( CMessage* pMessage );
	void OnMessageGSKillPersonResponse( CMessage* pMessage );

	void OnMessageGSWorldQuestResponse( CMessage* pMessage );
	void OnMessageGSWorldAnswerResponse( CMessage* pMessage );
	void OnMessageGSWorldQuestNotify( CMessage* pMessage );
	void OnMessageGSSyncNationNotify( CMessage* pMessage );
	void OnMessageGSWorldInfoResponse( CMessage* pMessage );
	void OnMessageGSJoinCountryNotify( CMessage* pMessage );
	void OnMessageGSChangeOfficialNotify( CMessage* pMessage );
	void OnMessageGSPlayerInfoNotify( CMessage* pMessage );
	void OnMessageGSWorldAnswerNotify( CMessage* pMessage );	
	void OnMessageGSKingChangeNotify( CMessage* pMessage );
	void OnMessageGSRealSummonNotify( CMessage* pMessage );
	void OnMessageGSTrySummonResponse( CMessage* pMessage );	

	void OnMessageGSSubmitWarResponse( CMessage* pMessage );
	void OnMessageGSEnterWarResponse( CMessage* pMessage );
	void OnMessageGSQueryWarCampResponse( CMessage* pMessage );
	void OnMessageGSQueryWarNumResponse( CMessage* pMessage );
	void OnMessageGSQueryWarInfoResponse( CMessage* pMessage );
	void OnMessageGSQueryWarRankResponse( CMessage* pMessage );
	void OnMessageGSBidCampResponse( CMessage* pMessage );
	void OnMessageGSWarSourceNotify( CMessage* pMessage );
	void OnMessageGSReliveChangeNotify( CMessage* pMessage );
	void OnMessageGSCampBidInfoResponse( CMessage* pMessage );
	void OnMessageGSCampResInfoResponse( CMessage* pMessage );
	void OnMessageGSGetWarAwardResponse( CMessage* pMessage );
	void OnMessageGSQueryWarAwardResponse( CMessage* pMessage );
	void OnMessageGSSyncWarStatusNotify( CMessage* pMessage );
	void OnMessageGSCreateWarStoneNotify( CMessage* pMessage );

	void OnMessageGSModifyRateResponse( CMessage* pMessage );
	void OnMessageGSGetTaxResponse( CMessage* pMessage );
	void OnMessageGSModifyRateNotify( CMessage* pMessage );	
	void OnMessageGSRefuseSummonNotify( CMessage* pMessage );
	void OnMessageGSAccuseKingResponse( CMessage* pMessage );
	void OnMessageGSAccuseKingNotify( CMessage* pMessage );
	void OnMessageGSPowerInfoResponse( CMessage* pMessage );
	void OnMessageGSAccuseStatusNotify( CMessage* pMessage );
	void OnMessageGSMakeGuoTanNotify( CMessage* pMessage );
	void OnMessageGSMakeLuanWuNotify( CMessage* pMessage );
	void OnMessageGSSyncGTAndLWNotify( CMessage* pMessage );
	
	// 国家类称号接口
	void OnMessageGSUpdateNationTitleNotify( CMessage* npMessage );
	void OnMessageGSNationTitleList( CMessage* npMessage );
	void OnEventNationTitle( CEntityPlayer* npPlayer, int nCountryID, bool bIsAdd );

	int ConvertCountryToMapID( int nCountryID );
	int ConvertMapIDToCountry( int nMapID );
public:
	void SendChangeWizardRequest( int vLineID, int vCountryID, int vChangeType, CEntityPlayer *pPlayer );
	void SendChangeConstructRequest( int vLineID, int vCountryID, int *vChangeType, int *vChangeValue, int vLen );
	void SendChangePowerRequest( int vLineID, int vCountryID, int vChangeType, int vPower );
	void SendChangeFriendNotify( int vChangeType, int vSrcWorld, int vDesWorld, int vChangeValue );
	void SendKillPersonRequest( int vSrcCharID, int vDesCharID );
	void SendPlayInfoM2SRequest( CEntityPlayer *pPlayer, std::vector<CEntityPlayer*>* pPlayerSet );
	void SendPlayInfoS2MRequest( std::vector<CEntityPlayer*>* pPlayerSet, CEntityPlayer *pPlayer );
	void SendGetAwardResponse( CEntityPlayer* pPlayer, int vResult, int vAwardID, int vTempID = 0 );
	void SendWarControlNotify( int vState, int vDay, int vHour, int vMinute, int vSerial );
	void SendWarStoneOverNotify( int vMapID );
	void SendAddTaxNotify( int vTax );
	void SendStoneInfoNotify( );
	void SendSpecialEventNotify( int vSrcWorldID, int vDesWorldID, int vEventType, int vCharID );
public:
	bool HasCountryService( CEntityPlayer *pPlayer, int vNpcEntityID, int vCountryID );
	bool HasFriendService( CEntityPlayer *pPlayer, int vNpcEntityID );
	void SetStoneInfo( int vMapID, int vStoneID, int vStoneX, int vStoneY, int vEntityID );
	int GetStoneID( int vMapID );
	int GetStoneX( int vMapID );
	int GetStoneY( int vMapID );
	int GetStoneEntity( int vMapID );
	bool HasTaxService( CEntityPlayer *pPlayer, int vNpcEntityID );
	int ChangeGT( int nSrcWorldID, int nDesWorldID, int nTime );
	int ChangeLW( int nSrcWorldID, int nDesWorldID, int nTime );
	bool IsInGT( int nSrcWorldID, int nDesWorldID );
	bool IsInLW( int nSrcWorldID, int nDesWorldID );
};