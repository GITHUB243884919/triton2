#pragma once

#include "servercore.h"
#include "servertool.h"
#include "shm.h"
#include "scenelogic.h"
#include "lk_hashmap.h"

// ********************************************************************** //
// CMapModule
// ********************************************************************** //

// 宏定义部分
#define MAP_PBDATA_LEN	128 // 地图模块数据序列化后的最大长度

class CEntityCharacter;
class CEntity;
class CEntityNpc;
class CSharedMem;
class CMessage;


// 异步传送
class CAsynTeleport
{
public:
	CAsynTeleport();
	~CAsynTeleport();
	
	int Initialize();


	int mTokenID;	// 令牌ID
	int mCharID;	 // 发起人
	char mCharName[ NAME_LENGTH ];	// 发起人角色姓名
	int mTimeout;	 // 超时时间，秒
	int mLevelLmt;	 // 等级限制

	struct DstChar
	{
		int mNationID;		// 阵营ID
		int mCountryID;		// 国家ID
		int mCorpsID;		// 军团ID
		int mFamilyID;		// 家族ID
		int mCharID;		// 角色ID 
		int mKing;			// 是否国王
	};	// 被传送人

	DstChar	mDstChar;
	ComplexPos mDestination; // 目的地
};



// 地图模块
class CMapModule : public CLogicModule, public CSingleton< CMapModule >
{
public:
	static char	sModuleName[ NAME_LENGTH ];
	CWTimer		mTimer;	
	int			mMoveCheck;

	typedef lk::hash_map< int, CAsynTeleport, 300 >  AsynTeleport;
	AsynTeleport	mAsynTeleportList;

public:
	CMapModule( ) : CLogicModule( sModuleName ), mTimer( CWTimer( 1000 ) )
	{
		if ( mShmPtr->GetInitMode() == SHM_INIT )
		{
			mMoveCheck = 0;
			mAsynTeleportList.initailize( );
		} 
		else
		{
			;
		}
	}
		
	~CMapModule() {}

	static unsigned int CountSize();

public:
	// 启动服务器
	virtual void OnLaunchServer( );

	// 退出服务器
	virtual void OnExitServer( );

	// 服务器Timer
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );

	// 建立场景
	virtual int OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName );

	// 路由消息
	virtual void OnRouterMessage( CMessage* pMessage );

	// 客户端发送消息
	virtual void OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 建立实体
	virtual void OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst );

	// 销毁实体
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );

	// 存储数据
	virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode );

	// 加载配置
	virtual void OnLoadConfig( );

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( ) { return true; }
private:

	// 处理玩家寻路消息
	int OnMessagePlayerPathRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理玩家停止移动消息
	int OnMessagePlayerStopRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理Pet寻路消息
	int OnMessagePetPathRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理Pet停止移动消息
	int OnMessagePetStopRequest( CEntityPlayer* pPlayer, CMessage* pMessage );


public:
	// 玩家是否可以通过
	bool CanPlayerPass( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, unsigned int vPosX, unsigned int vPosY, bool vFlyState );

	// NPC是否可以通过
	bool CanNpcPass( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, unsigned int vPosX, unsigned int vPosY, bool vFlyState );
	bool CanNpcPass( CEntityCharacter* tpEntity, unsigned int vPosX, unsigned int vPosY );

	// 是否是摆摊安全区域
	bool IsStallSafeArea( unsigned short usLineID, unsigned short usMapID, unsigned short vIndex, unsigned int vPosX, unsigned int vPosY );
	
	// 通过区域得到实体列表
	void GetRangeEntity(	unsigned short vLineID,
                           	unsigned short vMapID,
							unsigned short vIndex,
                            unsigned short vRangeType,
                            unsigned short vLength,
                            unsigned short vWidth,
                            CEntity* vpSrcEntity,
                            CEntityCharacter** vpEntityList,
							int* vpEntityCount 
						);


	//////////////////////////////////////////////////////////////////////////
	// 模块间调用函数
	//////////////////////////////////////////////////////////////////////////
	
	// 得到地图名称
	const char* GetMapName( unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex );

	// 玩家是否可以通过
	bool CanPlayerPass( unsigned short vLineID, unsigned short vMapID, unsigned int vPosX, unsigned int vPosY, bool vFlyState );

	// NPC是否可以通过
	bool CanNpcPass( unsigned short vLineID, unsigned short vMapID, unsigned int vPosX, unsigned int vPosY, bool vFlyState );

	//// 是否安全区域
	//bool IsSafeRegion( unsigned short usLineID, unsigned short usMapID, unsigned short usIndex, int nPosX, int nPoxY  );

	// 验证坐标是否有效
	bool IsCellValid( CEntity* pEntity, int nPosX, int nPoxY );

	// 实体沿路径移动
	bool PathEntity(CEntityCharacter* pEntity, int nSrcPosX, int nSrcPosY, int nDesPosX, int nDesPosY, unsigned short usDir );

	// 移动实体
	bool MoveEntity( CEntityCharacter* pEntity, int nPosX, int nPosY, bool bStop );

	// 传送实体
	bool Teleport( unsigned short usLineID, unsigned short usMapID, int nPosX, int nPosY, CEntity* pEntity, unsigned short usMapIndex = 0, KEY_TYPE vKey = INVITED_KEY_VALUE );


	//////////////////////////////////////////////////////////////////////////

	// 建立一个NPC
	bool CreateNpc( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, CCreator* vpCreator, unsigned int& vEntityID, int vNpcID = 0, CWTPoint vPoint = CWTPoint( 0, 0 ), bool vDelay = false );

	// 发送消息到指定矩形区域
	void SendRectMessage(  unsigned short usLineID, unsigned usMapID, unsigned short usMapIndex, const CWTRect* pRect, CMessage* pMessage );

	// 验证格子上是否已经有玩家在摆摊
	bool HasInStall( unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CWTPoint& rPos );

	// 验证格子是否是静态阻挡
	bool IsStaticBlock( unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex , CWTPoint& rPos );

	// 验证格子是否是高级阻挡
	bool IsSuperBlock( unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex , CWTPoint& rPos );

	// 得到掉落点坐标
	bool GetDropPos( unsigned short usLineID, unsigned short usMapID, unsigned short usMapIndex, CWTPoint &vOrginPos );

	// 判断指定的区域是否有玩家或NPC
	void HasEntity( CEntity* pSrcEntity, int nSrcPosX, int nSrcPosY,  unsigned short usRound, unsigned short usType );

	// 使用技能时,当前坐标与服务器坐标不同,以技能当前坐标为主,处理坐标(暂时只对主角有效)
	void OnEntityMove( CEntityCharacter* pEntity, const CWTPoint& vDesPoint );

	//// 玩家向周围可见玩家广播
	//void MessageBroadCast( CEntityPlayer* pPlayer, CMessage* pMessage, bool bToSelf = false, bool bTeamOnly = false );

	//// NPC向周围可见玩家广播
	//void MessageBroadCast( CEntityNpc* pNpc, CMessage* pMessage );

	//// 玩家附属物(只限pet,pedal,redstone)向周围可见队伍玩家广播
	//void MessageBroadCast( CEntity* pPlayerDependency, CMessage* pMessage, bool bTeamOnly  );

	void MessageBroadCast( CEntity* pEntity, CMessage* pMessage, bool bTeamOnly = false, bool bToSelf = false  );

	CWTPoint GetRandomPos( CEntityCharacter* pEntity );
	
	// 增加异步传送
	int AddAsynTeleport( CAsynTeleport& rAsynTele );

	// 查询异步传送
	CAsynTeleport* GetAsyncTeleport( int nTeleTokenID );

	// 删除异步传送
	int DeleteAsynTeleport( int nTeleTokenID );

	// 玩家等级是否满足地图限制等级
	bool IsLevelMatched( CEntityPlayer* pPlayer, int nMapID );

	virtual void OnLaunchComplete( );

	// 加载屏蔽NPC配置的回调函数
	static void LoadDenyTempConfigCallback(const char* pcConfigPath);
};



