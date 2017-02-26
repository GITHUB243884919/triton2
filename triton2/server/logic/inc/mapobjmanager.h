#pragma once

#include "sceneobj_define.h"
#include "maptplmanager.h"
#include "mapmessage_pb.hxx.pb.h"
#include "repetionmodule.h"
#include "lk_queue.h"
#include "property.h"
#include "entity_searcher.h"

#define BLOCK_MAX ( ( MAX_MAP_CELL >> 5 ) + 1 )
#define BLOCK_INDEX( tCell )		( tCell >> 5 )
#define BLOCK_CELL_INDEX( tCell )	( tCell % 32 )

// 动态创建NPC的区域
#define CREATENPC_RANGE_WIDE 20
#define CREATENPC_RANGE_HEIGHT 20

#define VIPCOOLTIME		300

// 
/** @地图单元格
** 封装了遍历函数( 不能直接用begin,end访问 )
** 需要传入自定义的函数对象对内部容器进行遍历
*/
class CMapCell
{
protected:
	typedef lk::fixed_size_memory_pool< int, SERVER_CAP_SHARE_CELLENTITY, 2> SharedCellsEntity;	
	typedef lk::shared_vector< int, SERVER_CAP_SHARE_CELLENTITY > CellsEntity;

	//注意：地图编辑器编出来的地图实际上是width+1,height+1. 
	CellsEntity mCells;

public:
	
	CMapCell( );
	~CMapCell( );

	// 将实体加入到cell
	bool AddEntityToCell( CEntity* pEntity )
	{
		mCells.push_back( pEntity->GetEntityID() );	
		return true;
	}

	// 将实体从cell中移除
	bool RemoveEntityFromCell( CEntity* pEntity )
	{
		for ( CellsEntity::iterator tBegin = mCells.begin(); tBegin != mCells.end(); ++tBegin )
		{
			// 如果找到了自己
			if ( *tBegin == (int)pEntity->GetEntityID() )
			{
				mCells.erase( tBegin );
				return true;
			}
		}

		return false;
	}

	// 遍历
	template<class Visitor>
	void Tick( Visitor& rVst )
	{
		for ( CellsEntity::iterator tBegin = mCells.begin(); tBegin != mCells.end(); )
		{
			// 函数对象返回值为false时，
			if( rVst( *tBegin ) == false )
			{
				//表示循环可以提前终止 
				if( rVst.GetResult() == EM_OBJFUN_ENTITY_TERMINATE )
				{
					return;
				}
				else if( rVst.GetResult() == EM_OBJFUN_ENTITY_NOTEXIST )
				{
					mCells.erase( tBegin++ );
				}
				else
				{
					++tBegin ;
				}
				
			}
			else
			{
				++tBegin ;
			}
		}
	}
};



// ********************************************************************** //
// CMapObject
// ********************************************************************** //
class CMapObject : public CObj
{
	friend class CMapTplManager;
public:

	typedef lk::fixed_size_memory_pool< int, SERVER_CAP_SHARE_CELLENTITY, 2> SharedCellsEntity;	
	typedef lk::shared_vector< int, SERVER_CAP_SHARE_CELLENTITY > CellsEntity;
	typedef lk::vector< int, MAX_MAP_PLAYER_PER > PLAYER_VECTOR;

protected:
	unsigned short				mWidth;				// 地图宽
	unsigned short				mHeight;			// 地图高
	unsigned short				mLineID;           
	unsigned short				mMapID; 
	unsigned short				mIndex;
	lk::string<	NAME_LENGTH >	mMapName; 			// 地图名称
	KEY_TYPE					mKey;


	static SharedCellsEntity* msMapEntityIDSharedPoolPtr; // 地图格中的对象

public:
	//注意：地图编辑器编出来的地图实际上是width+1,height+1. 
	PLAYER_VECTOR mPlayers;
	int			mBlock[ ( MAX_MAP_CELL >> 5 ) + 1 ];
	int mPlayerWaitInCount;

#ifdef _FB_
	typedef lk::vector< int, 700 > ENTITY_VECTOR;
	ENTITY_VECTOR	mEntity;
	unsigned int	mRepetionID;

	int			mData[ 10 ];
	
	int				mVirtualMapID;
#endif
	
public:
	CMapObject( unsigned short vLineID, unsigned short vMapID );
	CMapObject( ) ;
	~CMapObject( );

public:
	virtual int Initialize();
	virtual int Resume();

	bool IsCommMap( );
	bool IsBattleMap( );
	bool IsFBMap( );
	bool IsGroundMap( );
	bool IsSchoolMap( );
	
	MAP_TYPE GetMapType( );

	inline void SetMapID( unsigned short vMapID ){ mMapID = vMapID; }
	inline unsigned short GetMapID( ){ return mMapID; }

	inline unsigned short GetMapIndex( ) { return mIndex; }
	inline void SetMapIndex( unsigned short vIndex ) { mIndex = vIndex; }

	int LoadTpl( CMapTpl* pMapTpl );
	int CreateAllEntity( );	

	unsigned short GetLineID( ) { return mLineID; }
	void SetLineID( unsigned short usLineID ) { mLineID = usLineID; }

	int SetKey( KEY_TYPE vKey ) { mKey = vKey; return SUCCESS; }
	KEY_TYPE GetKey( ) { return mKey; }

	short GetMinLevelLmt( ) ;
	short GetMaxLevellmt( ) ;

	CGridBlock* GetGridBlock( unsigned int vWidth, unsigned int vHeight );

	// 地图宽
	inline unsigned short GetWidth( ) { return mWidth; }

	// 地图高
	inline unsigned short GetHight( ) { return mHeight; }

	// 建立NPC
	int CreateNpc( CCreator* pNpcTpl, bool vInit = false, int vNpcID = 0, CWTPoint vPoint = CWTPoint( 0, 0 ), bool vDelay = false );

	// 得到地图格
	virtual CMapCell* GetCell( const CWTPoint& rPoint ) = 0;

	// 玩家是否可以通过
	bool CanPlayerPass( const CWTPoint& rPoint );

	// NPC是否可以通过
	bool CanNpcPass( const CWTPoint& rPoint );

	// 是否是有效坐标
	inline bool IsValidPos( const CWTPoint& rPoint ) { return rPoint.mX <= GetWidth() && rPoint.mY <= GetHight(); }
	
	// 是否是相对安全区域(本国不能厮杀)
	bool IsCtrSafeRegion( const CWTPoint& rPoint );

	// 是否是绝对安全区域(敌国不能厮杀)
	bool IsAbsSafeRegion( const CWTPoint& rPoint );

	// 是否静态阻挡
	bool IsStaticBlock( const CWTPoint& rPoint );

	// 是否是摆摊安全区
	bool IsStallSaftyArea( const CWTPoint& rPoint );

	// 格子是否有玩家在摆摊
	bool HasInStall( const CWTPoint& rPoint );

	// 是否被移动加锁
	unsigned int IsLocked( const CWTPoint& rPoint );

	// 是否高级阻挡
	bool IsSuperBlock( const CWTPoint& rPoint );

	// 移动实体
	void MoveEntity( CEntityCharacter* pEntity, const CWTPoint& rPoint );

	// 移动实体
	void PathEntity( CEntityCharacter* pEntity, const CWTPoint& rSrcPos, const CWTPoint& rDesPos, int vDirection );

	// 查找路径
	void FindPath( const CWTPoint& rSrcPos, const CWTPoint& rDesPos, std::vector< CWTPoint >& rPathList );

	// 建立实体
	int CreateEntity( CEntity* pSrcEntity, CCreator* pCreator ,bool vFirst = false);


	// 发送创建npc消息给单个玩家
	int SendCreateNpc2Player( CEntityPlayer* pReceiver, std::vector<CEntity*> *pNpcVec, int nListID = 0 );

	// 发送创建单个npc消息给多个玩家
	int SendCreateNpc2Players( std::vector<CEntityPlayer*>* pReceiver, CEntity* pSingleNpc, int nListID = 0 );

	// 发送创建玩家消息给单个玩家
	int SendCreatePlayer2Player( CEntityPlayer* pReceiver, std::vector<CEntityPlayer*> *pPlayerVec );

	// 发送创建单个玩家消息给多个玩家
	int SendCreatePlayer2Players( std::vector<CEntityPlayer*>* pReceiver, CEntityPlayer* pSinglePlayer );

	// 发送销毁多个entity消息给单个玩家
	int SendDestroyEntity2Player( CEntityPlayer* pReceiver, std::vector<CEntityPlayer*>* pPlayerToBeDestroy, std::vector<CEntity*>* pNpcToBeDestroy );
	
	// 发送销毁单个entity消息给多个玩家
	int SendDestroyEntity2Players( std::vector<CEntityPlayer*>* pReceiver, CEntity* pEntityToBeDestroy );

	// 删除实体
	void DestroyEntity( CEntity* pEntity );

	// 定时器
	void OnTimer( unsigned int vTickOffset );

	void OnEntityMove( CEntityCharacter* pEntity, const CWTPoint& rPos, EDirType vDirection );

	// 将实体加入到cell
	bool AddEntityToCell( CEntity* pEntity );

	// 将实体从cell中移除
	bool RemoveEntityFromCell( CEntity* pEntity );

	// 得到掉落点
	bool GetDropPos( CWTPoint &vOrginPos );

	// 得到NPC创建点
	bool GetNpcCreatePos( CWTPoint &vOrginPos, EEntityType nType );

	bool IsFull( );

	int AddPlayerIntoMap( CEntityPlayer* vpPlayer ,bool vFirst);
	int RemovePlayerFromMap( CEntityPlayer* vpPlayer );
	int GetPlayerCount( );

	int AddPlayerWaitInCount( );
	int DelPlayerWaitInCount( );
	int GetPlayerWaitInCount( );
	
	int GetPlayerCountMax( );

	// 坐标是否被阻挡NPC占用
	bool IsOccupiedByBlock( CWTPoint& rPos );

	// 允许使用低级坐骑
	bool CanRideBaseHorse( CWTPoint& rPos ) { return true; }

	// 允许使用高级坐骑
	bool CanRideAdvanceHorse( CWTPoint& rPos ) { return true; }

	// 得到地图名
	const char* GetMapName( ) { return mMapName.c_str(); }

	// 发送Message给地图上所有player
	void SendMsg2Player( CMessage* vpMsg );

	// 是否是绝对安全区
	bool IsAbsSafe( );

	// Point Is Block
	bool IsBlock( int mX, int mY );

	// Set Block
	int AddBlock( int mX, int mY );

	// Remove Block
	int RemoveBlock( int mX, int mY );

	// 得带可见范围
	int GetViewRect( CEntity* pEntity, CWTPoint& rLeftTop, CWTPoint& rRightBottom );
	
	bool IsCreatorIn( int vCreatorIndex );

#ifdef _FB_
	int SetRepetionID( unsigned int vObjID ) { mRepetionID = vObjID; return SUCCESS; }
	CRepetion* GetRepetion( ) { return ( CRepetion* )CSceneObjManager::GetSingletonPtr( )->GetObject( mRepetionID ); }
	int GetRepetionID( ) { return mRepetionID; }
	int DestroyAliveTempOgreEntity( unsigned int vTemplateID, unsigned int vCount );

	int SetVirtualMapID( unsigned int vMapID );
	int GetVirtualMapID( );

	int SetMapData( int vIndex, int vData );
	int GetMapData( int vIndex );
#endif

};


// 大型地图
class CMapObjectL : public CMapObject
{
public:
#ifndef _FB_
	static const int msWidth = 600 ;	
	static const int msHeight = 800 ; 
#else
	static const int msWidth = 300 ;
	static const int msHeight = 320;
#endif
	virtual CMapCell* GetCell( const CWTPoint& rPoint );

private:
	CMapCell mCells[msWidth*msHeight];

};

// 中型地图
class CMapObjectM : public CMapObject
{
public:
#ifndef _FB_
	static const int msWidth = 240 ;
	static const int msHeight = 320 ;
#else
	static const int msWidth = 200;
	static const int msHeight = 350;
#endif
	virtual CMapCell* GetCell( const CWTPoint& rPoint );

private:
	CMapCell mCells[msWidth*msHeight];
};

// 小型地图
class CMapObjectS : public CMapObject
{
public:
#ifndef _FB_
	static const int msWidth = 188 ;
	static const int msHeight = 172 ;
#else
	static const int msWidth = 90;
	static const int msHeight = 95;
#endif
	virtual CMapCell* GetCell( const CWTPoint& rPoint );

private:
	CMapCell mCells[msWidth*msHeight];
};

// ********************************************************************** //
// CMapObjManager
// ********************************************************************** //

class CMapObjManager : public CSingleton< CMapObjManager >
{
public:
	typedef lk::hash_map< int, int, SERVER_CAP_L_MAP_OBJECT+SERVER_CAP_M_MAP_OBJECT+SERVER_CAP_S_MAP_OBJECT > 	MapObjMap;
	
	static CSharedMem* mShmPtr; // 共享内存地址

#ifdef _FB_
	typedef lk::queue< int, SERVER_CAP_REPETION_OBJECT >	FreeMapObjQueue;
	int mMapObjID[ SERVER_CAP_REPETION_OBJECT ];
	FreeMapObjQueue mFreeMapObjQueue;
#else
	MapObjMap mMapObjects;
#endif	
	typedef lk::vector< int, 200 >		DENY_LIST_TYPE;
	DENY_LIST_TYPE	mDenyList;

public:
	CMapObjManager();
	~CMapObjManager( );

	void* operator new( size_t size );
	void  operator delete( void* pointer );

	void* operator new(size_t size, const char* file, int line);
	void operator delete(void* pointer, const char* file, int line);

	static unsigned int CountSize();

public:
	// 建立地图
	CMapObject* CreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName,
		KEY_TYPE vKey = 0 );

	// 得到地图对象
	CMapObject* GetMapObject( unsigned short vLineID, unsigned short vMapID, unsigned short vMapIndex, 
		KEY_TYPE vKey = INVITED_KEY_VALUE  );

	CMapObject* GetMapObject( unsigned int vObjID );

	int DestroyMapObject( CMapObject* vpMapObj );

	int ReloadDeny( );
	bool IsDeny( int vTempID );

public:
	static int MakeKey( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex = 0 );
};



