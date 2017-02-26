#pragma once

#include "object.h"
#include "coretype.h"
#include "lk_vector.h"
#include "servertool.h"

class TiXmlElement;
class CSharedMem;


// ********************************************************************** //
// CCreator
// ********************************************************************** //

class CCreator : public CObj
{
protected:
	int	mCreatorType;		// 建立的NPC类型
	unsigned int mIndex;
	bool mShow;
public:
	int				mHasOgre;			// 记录当前是否有怪/npc
	int				mActivity;			// 记录是否有活动
	unsigned short mMapIndex;

public:
	CCreator() 
	{
		if( CObj::msCreateMode )
		{
			Initialize();
		}
		else
		{
			Resume();
		}
	}
	CCreator( int vType ) : mCreatorType( vType )	{ Initialize(); }
	
	inline void SetIndex( unsigned int vIndex ){ mIndex = vIndex; }
	inline unsigned int GetIndex() {return mIndex;}

	inline void SetShow( unsigned short vShow ){ mShow = vShow; }
	inline unsigned short GetShow() {return mShow;}

	inline int GetAttachedEntityID( ) { return mHasOgre; }
	inline void SetAttachedEntityID( int nID ) { mHasOgre = nID; }

	virtual int GetCreatorType( ) = 0;
	virtual ~CCreator() {}

	virtual int Initialize() {mShow = true; mHasOgre = 0; mActivity = 0; mMapIndex = 0; mIndex = 0; return 0;}

	// 恢复
	virtual int Resume() { return 0;} 

	// 从配置文件加载
	virtual bool LoadFromConfig(TiXmlElement* pElemBlock) = 0;
};

// ********************************************************************** //
// COgreCreator
// ********************************************************************** //

struct DeadRefresh
{
	int		mOgreID;			// 怪物的模版ID
	short	mOgreRand;			// 怪物产生的概率
};

struct OccurRefresh
{
	int		mOldID;				// 存活的怪物ID
	int		mTime;				// 存活的时间
	int		mNewID;				// 刷新的
};

struct CommonRefresh
{
	int		mIndex;				// 触发的索引
	short	mOgreRand;			// 触发的概率
};

struct RefreshDate
{
	int		mYear;
	char	mMonth;
	char	mDate;
	char	mHour;
	char	mMinute;
	char	mWeek;
	char	mDay;


	// 双倍里面用的含义与字段命名不一致
	time_t  timet()
	{
		struct tm tm;
		tm.tm_sec = 0;
		tm.tm_min = mMinute;
		tm.tm_hour = mHour;
		tm.tm_mday = mDate;
		tm.tm_mon  = mMonth - 1;
		tm.tm_year = mYear - 1900;

		tm.tm_isdst = 0;

		return mktime(&tm);
	}
};

class COgreCreator : public CCreator
{
public:
	short			mRCount1;
	short			mRCount2;
	short			mRCount3;
	short			mRCount4;
	short			mRCount5;
	short			mRefreshRate;		// 触发其它点刷新的概率
	short			mRCount6;
	short			mPCount;

	// 在这里记录这个生成器的LineID和MapID
	unsigned short	mLineID;
	unsigned short	mMapID;
	short			mRCount7;		
	short			mRCount8;				
	int				mInit;				// 起始是否刷怪
	DeadRefresh		mRefresh1[ 20 ];	// 死亡刷新
	OccurRefresh	mRefresh2[ 10 ];	// 存在刷新
	int				mRefresh3[ 25 ];	// 概率刷新
	int				mRefresh4[ 10 ];	// 计数刷新
	CommonRefresh	mRefresh5[ 10 ];	// 公共刷新
	int				mCommonID;			// 管理它的生成器ID
	int				mDisappear;			// NPC消失时间
	EDirType		mDirection;			// 初始方向
	int				mRelife;			// NPC重生时间
	CWTPoint		mPosition;			// NPC坐标
	int				mCountNum;			// 杀死计数
	int				mCountDelay;		// 杀死触发的延时
	int				mCurrentCount;		// 当前杀死的计数
	RefreshDate		mRefreshTime[ 100 ];		// 指定的刷新时间
	//int				mHasOgre;			// 记录当前是否有怪
	int				mPathListX[ 20 ];
	int				mPathListY[ 20 ];
	int				mInitBuffID;
	int				mTimeBuffID;
	int				mFuncNpc;
//	int				mActivity;


	// 从NPC Creater中
	int				mOwnerCharID; // 所有者ID
	int				mIsSendNpc;	  // 是否护送NPC
	int				mIsShowOwner;  // 是否在客户端显示所有者名称

	// 新增冷却时间
	int				mCoolTime;

	int				mMaxEnmity;			// 最大仇恨索引
	int				mRefresh7[ 10 ];	// 死刷新
	
	int				mRefresh8[ 10 ];	// 生刷新

	int				mNextRefreshTime;

public:
	COgreCreator( ) : CCreator( ENTITYTYPE_OGRE )	{ Initialize();	}
	COgreCreator( int vType ) : CCreator( vType ) { Initialize(); } 
	virtual int Initialize() ;
	int GetCreatorType( ) { return ENTITYTYPE_OGRE; }	

	virtual ~COgreCreator() {}

	// 从配置文件加载
	virtual bool LoadFromConfig(TiXmlElement* pElemBlock);

	// 获得最近刷新时间
	int GetRightTime( struct tm *now );
};

/************************************************************************/
/* CBoxCreator                                                                     */
/************************************************************************/
class CBoxCreator : public COgreCreator
{
public:
	CBoxCreator( ) : COgreCreator( ENTITYTYPE_BOX ) { }
	int GetCreatorType( ) { return ENTITYTYPE_BOX; }

	virtual ~CBoxCreator( ) { }

	// 从配置文件加载
	virtual bool LoadFromConfig(TiXmlElement* pElemBlock) ;
};

/************************************************************************/
/* CBlockCreator                                                                     */
/************************************************************************/
class CBlockCreator : public COgreCreator
{
public:
	CBlockCreator( ) : COgreCreator( ENTITYTYPE_BLOCK ) { }
	int GetCreatorType( ) { return ENTITYTYPE_BLOCK; }

	virtual ~CBlockCreator( ) { }

	// 从配置文件加载
	virtual bool LoadFromConfig(TiXmlElement* pElemBlock) ;
};


// ********************************************************************** //
// CNpcCreator
// ********************************************************************** //

class CNpcCreator : public COgreCreator
{
public:
	int				mNpcID;			// NPC的模版ID
	EDirType		mDirection;		// 初始方向
	CWTPoint		mPosition;		// NPC坐标
	//int				mInit;

public:
	CNpcCreator( EEntityType enType = ENTITYTYPE_FUNCNPC ) : COgreCreator( enType ) {Initialize();}
	int GetCreatorType( ) { return mCreatorType; }

	virtual ~CNpcCreator() {}

	virtual int Initialize() ;

	// 从配置文件加载
	virtual bool LoadFromConfig(TiXmlElement* pElemBlock);
};

// ********************************************************************** //
// CDropCreator
// ********************************************************************** //

class CDropCreator : public CCreator
{
public:
	int				mRange;			// 掉落范围
	int				mCenterX;		// 掉落中心X
	int				mCenterY;		// 掉落中心Y
	int				mDropType;		// 掉落类型，来自怪物或者扔
public:
	CDropCreator( int vCreatorType ) : CCreator( vCreatorType ) {}
	CDropCreator( int vRange, int vCenterX, int vCenterY, int vDropType )
		: CCreator( ENTITYTYPE_ITEM ), mRange( vRange ), mCenterX( vCenterX ), mCenterY( vCenterY ), mDropType( vDropType )
	{
	}
	virtual ~CDropCreator() {}

	// 从配置文件加载
	virtual bool LoadFromConfig(TiXmlElement* pElemBlock) { return true;}
};

// ********************************************************************** //
// CRedStoneCreator
// ********************************************************************** //

class  CRedstondeCreator : public CDropCreator
{
public:
	int mItemID;
	int	mOwnerCharID[ TEAMNUM ];			// 物品所有者,需要一个唯一的标示,所以改用CHARID
	CRedstondeCreator( ) : CDropCreator(  ENTITYTYPE_REDSTONE )	{ }
	
	CRedstondeCreator(  int vItemID,int vRange, int vCenterX, int vCenterY,int *vOwnerCharID, int vDropType = DROP_OBJECTIVE   )
		:CDropCreator( vRange, vCenterX, vCenterY, vDropType ),mItemID( vItemID  )
		{			
			memcpy( mOwnerCharID, vOwnerCharID, sizeof( mOwnerCharID ) );
		}
public:
	virtual int GetCreatorType( ) { return ENTITYTYPE_REDSTONE; }
	int *GetOwnerID( ){  return mOwnerCharID; }
	virtual ~CRedstondeCreator(){}

	// 从配置文件加载
	virtual bool LoadFromConfig(TiXmlElement* pElemBlock) { return true;}
};



// ********************************************************************** //
// CItemCreator
// ********************************************************************** //

class CItemCreator : public CDropCreator
{
public:
	int				mItemID;					// 生成的道具ID	
	int				mOwnerCharID[ TEAMNUM ];	// 物品所有者,需要一个唯一的标示,所以改用CHARID
	void*			mpItem;						// 一个具体道具，如果该值等于0，那么通过mItemID建立一个新道具
	int				mListID;					// 掉落物品时客户端显示用
	int				mLevel;						// 掉落装备时查询等级
	bool			mProtect;					// 是否需要保护

public:

	CItemCreator() : CDropCreator( ENTITYTYPE_ITEM ) {}

	CItemCreator( int vItemID, int vRange, int vCenterX, int vCenterY, int *vOwnerCharID, void* pItem, int vListID = 0, int vLevel = 0, int vDropType = DROP_OBJECTIVE, bool vProtect = true ) 
		: CDropCreator( vRange, vCenterX, vCenterY, vDropType ), mItemID( vItemID ), mpItem( pItem ), mListID( mListID ), mLevel( vLevel ), mProtect( vProtect ) 
	{
		memcpy( mOwnerCharID, vOwnerCharID, sizeof( mOwnerCharID ) );
	}
	int GetCreatorType( ) { return ENTITYTYPE_ITEM; }

	virtual ~CItemCreator() {}

	// 从配置文件加载
	virtual bool LoadFromConfig(TiXmlElement* pElemBlock) { return true;}
};

// ********************************************************************** //
// CMoneyCreator
// ********************************************************************** //

class CMoneyCreator : public CDropCreator
{
public:
	unsigned int	mMoney;			// 生成的道具ID

public:

	CMoneyCreator() : CDropCreator( ENTITYTYPE_MONEY ) {}

	CMoneyCreator( unsigned int vMoney, int vRange, int vCenterX, int vCenterY, int vCreateType, int vDieEntityID, int vDropType = DROP_OBJECTIVE ) 
		: CDropCreator( vRange, vCenterX, vCenterY, vDropType ), mMoney( vMoney )
	{
	}
	
	virtual ~CMoneyCreator() {}
	int GetCreatorType( ) { return ENTITYTYPE_MONEY; }

	// 从配置文件加载
	virtual bool LoadFromConfig(TiXmlElement* pElemBlock) { return true;}

};

// ********************************************************************** //
// CPedalCreator
// ********************************************************************** //

class CPedalCreator : public CCreator
{
public:
//	enum
//	{
//		PEDAL_TELEPORT,		// 传送点踏板
//		PEDAL_TRAP,			// 陷阱踏板
//		PEDAL_HIEROGRAM,	// 神符踏板
//	};
	struct ActiveData 
	{
		int  mHour;
		int  mRand;
		bool mActive;
	};	

public:
	int				mPedalType;		// 如果是踏板，踏板类型
	CWTRect			mSrcRect;		// 踏板的源区域
	CWTRect			mActRect;		// 作用区域
	CWTPoint		mDesPos;		// 如果是传送点踏板，目标坐标
	unsigned short	mLineID;		// 如果是传送点踏板，目标线ID, 如果等于0表示本线地图
	unsigned short	mMapID;			// 如果是传送点踏板，目标地图ID
	unsigned short	mSkillID;		// 如果是陷阱踏板，对应的技能ID
	unsigned short	mSkillLevel;	// 如果是陷阱踏板，对应的技能等级
	int				mSrcEntityID;	// 释放者实体ID
	unsigned int	mTeamID;
	unsigned int	mMinLevel;		// 最低等级
	unsigned int	mNation;		// 限制国家
	int				mTempID;
	int				mInit;
	ActiveData		mRandList[ 24 ];	// 激活时间列表
	int				mRandCount;		// 激活个数
	int				mMaxActive;		// 最大激活数量
	int				mCurrentActive;	// 当前激活数量
	int				mPedalID;		// 对应的实体ID
	int				mExistTime;		// 存活时间

	int				mDefaultLineID;
	int				mDefaultMapID;
public:

	CPedalCreator( ) : CCreator( ENTITYTYPE_PEDAL )	
	{	
		if( CObj::msCreateMode )
		{
			mPedalType = 0;
			mLineID = 0;
			mMapID = 0;
			mSkillID = 0;
			mSkillLevel = 0;
			mSrcEntityID = 0;
			mTeamID = 0;
			mMinLevel = 0;
			mNation = 0;
			mTempID = 0;
			memset( mRandList, 0, sizeof(mRandList) );
			mRandCount = 0;
			mMaxActive = 0;
			mCurrentActive = 0;
			mPedalID = 0;
			mExistTime = 0;

			mDefaultLineID = 0;
			mDefaultMapID = 0;
		}
	}
	virtual ~CPedalCreator() {}
	int GetCreatorType( ) { return ENTITYTYPE_PEDAL; }

	// 从配置文件加载
	virtual bool LoadFromConfig(TiXmlElement* pElemBlock) ;

	int RefreshActive( );
};

// ********************************************************************** //
// CPetCreator
// ********************************************************************** //

class CPetCreator : public CCreator
{
public:
	CWTPoint		mPos;			// 召唤兽创建的位置
	EDirType		mDirection;		// 初始方向
	int				mPetID;			// 召唤兽模板ID
	int				mSrcEntityID;	// 召唤者的ID
	int				mOnlineTime;	// 在线时间
	int				mPetMode;		// pk模式
public:

	int Initialize()
	{
		mPos = CWTPoint(0,0);
		mDirection = DIR_DOWN;
		mPetID = 0;
		mSrcEntityID = 0;
		mOnlineTime = 0;
		mPetMode = 0;
		return 0;
	}

	CPetCreator( ) : CCreator( ENTITYTYPE_PET )
	{	
		if( CObj::msCreateMode )
		{
			Initialize();
		}
	}

	int GetCreatorType( ) { return ENTITYTYPE_PET; }
	virtual ~CPetCreator() {}

	// 从配置文件加载
	virtual bool LoadFromConfig(TiXmlElement* pElemBlock) { return true;}
};



// ********************************************************************** //
// CCollectCreator
// ********************************************************************** //

class CCollectCreator : public CCreator
{
public:
	CWTPoint		mPosition[ 200 ];	// 创建的位置
	EDirType		mDirection;		// 初始方向
	int				mNpcID;			// 模板ID
	int				mRelife;		// NPC重生时间
	int				mDisappear;		// 消失时间 
	int				mPCount;		// 位置数量
	int				mInit;

public:

	CCollectCreator( ) : CCreator( ENTITYTYPE_COLLECT )	
	{
		if( CObj::msCreateMode )
		{
			memset( mPosition, 0, sizeof( mPosition ) );
			mDirection = DIR_DOWN;
			mNpcID = 0;
			mRelife = 0;
			mDisappear = 0;
			mPCount	= 0;
			mInit = 0;
		}
	}
	int GetCreatorType( ) { return ENTITYTYPE_COLLECT; }
	virtual ~CCollectCreator() {}

	// 从配置文件加载
	virtual bool LoadFromConfig(TiXmlElement* pElemBlock) ;
};

// ********************************************************************** //
// CGhostCreator
// ********************************************************************** //

class CGhostCreator : public CCreator
{
//public:
//	CWTPoint		mPos;	// 创建的位置
//	EDirType		mDirection;		// 初始方向
//	int				mNpcID;			// 模板ID
//public:
//
//	CGhostCreator( ) : CCreator( ENTITYTYPE_GHOST )	{ }
//	virtual int GetCreatorType( ){ return ENTITYTYPE_GHOST; }
//	CGhostCreator( int vType ) : CCreator( vType ) {  } 
//
//	virtual ~CGhostCreator() {}
//
//
//	// 从配置文件加载
//	virtual bool LoadFromConfig(TiXmlElement* pElemBlock) { return true;};
//	/////////////////
public:
	CWTPoint		mPos;	// 创建的位置
	EDirType		mDirection;		// 初始方向
	int				mNpcID;			// 模板ID

public:
	CGhostCreator( ) : CCreator( ENTITYTYPE_GHOST )	{ Initialize();	}
	CGhostCreator( int vType ) : CCreator( vType ) { Initialize(); } 
	virtual int Initialize() ;
	int GetCreatorType( ) { return ENTITYTYPE_GHOST; }	

	virtual ~CGhostCreator() {}

	// 从配置文件加载
	virtual bool LoadFromConfig(TiXmlElement* pElemBlock){ return true;};
	//////////////////
};



/************************************************************************/
/* CCreatorManager                                                                     */
/************************************************************************/
class CCreatorManager : public CSingleton< CCreatorManager >
{
public:
	typedef lk::vector< int, 3000 > TimeRefreshList;
	typedef lk::vector< int, 10 > PedalRefreshList;
	typedef lk::vector< int, 100 > NotifyList;
	int mOgreCreator[ OGREINDEX ];
	TimeRefreshList mRefreshList;
	PedalRefreshList mPedalList;
	NotifyList mNotifyList;

public:

	void* operator new( size_t size );
	void operator delete( void* pointer );

	void* operator new(size_t size, const char* file, int line);
	void operator delete(void* pointer, const char* file, int line);

	int InsertCreatorBase( int vIndex, CCreator* vpCreatorBase );
	CCreator* GetCreator( int vIndex  );
	unsigned int GetCreateIndex( CCreator* vpCreate );
	void RefreshPedals( );
	void RemoveCreator( CCreator *pCreator );

	CCreatorManager( );
	~CCreatorManager( );

	static CSharedMem* mShmPtr;
	static unsigned int CountSize(){ return sizeof( CCreatorManager ); }
};


