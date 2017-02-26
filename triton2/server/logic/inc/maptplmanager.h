#pragma once
// ********************************************************************** //
// CGridBlock
// ********************************************************************** //
#include "property.h"
#include "errcode.h"
#include "base_define.h"
#include "sceneobj_define.h"

// 阻挡定义
// 所有人可以通过
#define FLAG_ALLCANPASS			0xFFFFFFFF
// NPC不能通过
#define FLAG_NPCCANNOTPASS		0x00000001
// 绝对安全
#define FLAG_ABSOLUTESAFE		0x00000002
// 相对安全
#define FLAG_CONTRARYSAFE		0x00000004
// 静态阻挡
#define FLAG_STATICBLOCK		0x00000008
// 可以使用传送道具
#define FLAG_USETELEITEM		0x00000010
// 摆摊安全区
#define FLAG_STALLAREA			0x00000020
// PK区
#define FLAG_PKAREA				0x00000040
// 高级阻挡区
#define FLAG_SUPERBLOCK			0x00000080

enum MAP_TYPE
{
	COMM_MAP		= 0,
	GROUND_MAP		= 1,
	FB_MAP			= 2,
	BATTLE_MAP		= 3,
	SCHOOL_MAP		= 4,
};


class CMapObject;
class TiXmlElement;
class  CSharedMem;

// 地图静态阻挡信息
class CGridBlock
{
	public:
		int		mBlockInfo[ 1 ];

	public:
		CGridBlock( )
		{
		}

	public:
		int Initial( )
		{
			mBlockInfo[ 0 ] = 0;
			return SUCCESS;
		}

		// 玩家是否可以通过
		bool CanPlayerPass( )	{ return ( ( mBlockInfo[ 0 ] & FLAG_ALLCANPASS ) == FLAG_ALLCANPASS ); }

		// NPC是否可以通过
		bool CanNpcPass( )		{ return ( ( mBlockInfo[ 0 ] & FLAG_NPCCANNOTPASS ) != FLAG_NPCCANNOTPASS ); }

		// 格子是否是绝对安全区域
		bool IsAbsSafeRegion( )		{ return ( ( mBlockInfo[ 0 ] & FLAG_ABSOLUTESAFE ) == FLAG_ABSOLUTESAFE ); }

		// 格子是否是相对安全区域
		bool IsCtrSafeRegion( )	{ return ( ( mBlockInfo[ 0 ] & FLAG_CONTRARYSAFE ) == FLAG_CONTRARYSAFE ); }

		// 是否是静态阻挡
		bool IsStaticBlock( )	{ return ( ( mBlockInfo[ 0 ] & FLAG_STATICBLOCK ) == FLAG_STATICBLOCK ); }

		// 是否是摆摊安全区
		bool IsStallSafty( ) { return ( ( mBlockInfo[ 0 ] & FLAG_STALLAREA ) == FLAG_STALLAREA ); }

		// 是否是高级阻挡
		bool IsSuperBlock( )	{ return ( ( mBlockInfo[ 0 ] & FLAG_SUPERBLOCK ) == FLAG_SUPERBLOCK ); }

};

// ********************************************************************** //
// CMapTpl
// ********************************************************************** //

class CMapTpl
{
private:
	typedef lk::vector< int, 2000 > CREATOR_LIST_TYPE;
public:
	char					mMapName[ NAME_LENGTH ];
	int						mMapID;
	int						mWidth;
	int						mHeight;
	CREATOR_LIST_TYPE	mCreators;	
	int						mPlayerCountMax;
	MAP_TYPE				mMapType;

	// 新增的地图配置
	bool				mPKPunish;			// PK惩罚
	bool				mBasRide;			// 允许低级坐骑
	bool				mAdvRide;			// 允许高级坐骑
	bool				mTeleItem;			// 允许使用传送物品
	bool				mNoNation;			// 是否没有国籍限制
	bool				mAbsSafe;			// 是否绝对安全
	bool				mConSafe;			// 是否相对安全
	bool				mFlyRide;
	short				mMaxLevelLmt;	// 最高等级限制
	short				mMinLevelLmt;	// 最低等级限制
	short				mProtectLevel;
	EPKMode				mPKMode;			// PK模式
	CGridBlock			mGrids[ MAX_MAP_CELL ];

public:
	CMapTpl( )
	{
	}

public:
	~CMapTpl( )
	{
		Final( );
	}

	CGridBlock* GetGridBlock( unsigned int vWidth, unsigned int vHeight );

	CMapObject* CreateMapObject( );

	// 宽和高
	inline unsigned short GetWidth( ) { return mWidth; }
	inline unsigned short GetHeight( ) {return mHeight; }

	short GetMinLevelLmt( ) { return mMinLevelLmt; }
	void SetMinLevelLmt( int nMinLevel ) { mMinLevelLmt = nMinLevel; }

	short GetMaxLevellmt( ) { return mMaxLevelLmt; }
	void SetMaxLevelLmt( int nMaxLevel ) { mMaxLevelLmt = nMaxLevel; }

	int LoadMap( TiXmlElement* vpElement, FILE* vpBlock, const char* pTplName, unsigned short vLineID, 
			unsigned short vMapID );
	int Final( );
	int Initial( )
	{
		memset( mMapName, 0, sizeof( mMapName ) );
		mMapID = INVALID_MAP_ID;
		mWidth = 0;
		mHeight = 0;
		mCreators.initailize( );
		mPlayerCountMax = 0;
		mPKPunish = true;
		mBasRide = true;
		mAbsSafe = true;
		mTeleItem = true;
		mNoNation = true;
		mAbsSafe = true;
		mConSafe = true;
		mPKMode = PKMODE_TRAINING;
		mFlyRide = true;

		unsigned int i = 0;
		for( i = 0; i < ARRAY_CNT( mGrids ); i++ )
		{
			mGrids[ i ].Initial( );
		}

		mMinLevelLmt = 0;
		mMaxLevelLmt = 0;
		mProtectLevel = 0;

		return SUCCESS;
	}

	bool IsUnLoad( ) { return ( mWidth == 0 ) || ( mHeight == 0 ); }
	bool IsCreatorIn( int vCreatorIndex );
};

// ********************************************************************** //
// CMapTplManager
// ********************************************************************** //

class CMapTplManager : public CSingleton< CMapTplManager >
{

public:
	CMapTpl mMapTpls[ SERVER_CAP_MAP_TPL ];
	unsigned int mMapTplCount;
public:
	CMapTplManager();
	~CMapTplManager( );

	void* operator new( size_t tSize );
	void operator delete( void* pointer );

	void* operator new(size_t size, const char* file, int line);
	void operator delete(void* pointer, const char* file, int line);

	static unsigned int CountSize( ){ return sizeof( CMapTplManager ); }
	static CSharedMem* mShmPtr;

public:
	// 通过地图ID得到地图模版
	CMapTpl* GetMapTplByID( unsigned short vMapID );
	CMapTpl* CreateMapTpl( const char* pTplName, unsigned short vMapID, unsigned short vLineID, bool bReload = false );
};



