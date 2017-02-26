#pragma once
class CMapObject;
class CObj;
class CEntity;
// ********************************************************************** //
// CMapEntity
// ********************************************************************** //

#include "lk_sharedvector.h"
#include "sceneobj_define.h"
#include "lk_vector.h"


class CMapEntity : public CObj 
{
public:

	typedef lk::fixed_size_memory_pool< int, SERVER_CAP_DATA_MAPENTIY, 2> SharedSeenEntityPool;	
	typedef lk::fixed_size_memory_pool< int, SERVER_CAP_DATA_MAPPEDAL, 2> SharedSeenPedalPool;	
	typedef lk::shared_vector< int, SERVER_CAP_DATA_MAPENTIY > SeenEntityVector;
	typedef lk::shared_vector< int, SERVER_CAP_DATA_MAPPEDAL > SeenPedalVector;


	//TODO
	int							mEntityID;	// 自己的实体
	SeenEntityVector			mSeenEntitys;// 看到的实体
	SeenPedalVector				mSeenPedals; // 看到的踏板

	// 静态共享容器
	static SharedSeenEntityPool*	msSharedSeenEntityPoolPtr;
	static SharedSeenPedalPool*		msSharedSeenPedalPoolPtr;
public:
	
	CMapEntity( CEntity* pEntity )	{	}
	
public:
	//TODO
	CMapEntity( );
	
	virtual ~CMapEntity( );

	virtual int Initialize();
	virtual int Resume();

public:
	inline int GetSelfEntityID( ) {return mEntityID;}

	CEntity* GetSelfEntity( ); 

	// 添加看到的实体
	void AddViewEntity( CMapEntity* pEntity );

	// 删除看到的实体
	void DeleteViewEntity( int nMapEntityObjID );

	// 传送到目标场景
//	void Teleport( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const CWTPoint& rDesPos );
	//void Teleport( int vLineID, int vMapID, const CWTPoint& rDesPos );


	// 发送消息给看到自己的实体 
	int SendMessageToSeenEntitys( CMessage* tpMessage );

	// 得到视野里的NPC 实体ID  任务模块调用
	int GetSeenNpcs( vector<int>& vNpcs );

};

// ********************************************************************** //
// CMapEntity
// ********************************************************************** //

class CMapNpc : public CMapEntity
{
public:
	
	CMapNpc( ) {}
	CMapNpc( CEntity* pEntity ) : CMapEntity( pEntity )	{	}

public:

	int GetNpcID();

};

// ********************************************************************** //
// CMapPedal
// ********************************************************************** //

class CMapPedal : public CMapNpc
{
public:

	//TODO: 这些都不要了，放在了CEntityPedal

	//int				mPedalType;			// 踏板类型
	//CWTRect			mSrcRect;			// 踏板的触发源矩形
	//CWTPoint		mDesPos;			// 如果是传送踏板，那么传送的目标坐标
	//unsigned short	mLineID;			// 如果是传送踏板，那么传送的目标线ID
	//unsigned short	mMapID;				// 如果是传送踏板，那么传送的目标地图ID
	//int				mSrcEntityID;		// 如果是陷阱踏板，释放者体ID
	//int				mPresistTime;		// 如果是陷阱踏板，持续时间
	//unsigned short	mSkillID;			// 如果是陷阱踏板，对应技能ID
	//unsigned short	mSkillLevel;		// 如果是陷阱踏板，对应技能等级
	//unsigned int	mMinLevel;			// 限制传送的等级
	//unsigned int	mNation;			// 传送限制的国家

public:
	//TODO
	CMapPedal( ) {}
	virtual ~CMapPedal( ) {}

public:
	//bool OnTouch( CMapObject* pMap, CMapEntity* pEntity, bool& rErase );		// 踏板触发逻辑
};
