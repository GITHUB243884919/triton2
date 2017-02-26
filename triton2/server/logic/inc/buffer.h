#pragma once
#include "coretype.h"
#include "protype.h"
#include "sceneobj_manager.h"
#include "promessage_pb.hxx.pb.h"
#include "share_define.h"
#include "property_pb.hxx.pb.h"
#include "lk_vector.h"
#include "lk_hashmap.h"
#include "template.h"
#include "timer_define.h"
#include "skill_list.h"

class CProperty;
class CPropertyCharacter;
class CPropertyPlayer;
class CPropertyNPC;
class CPropertyPet;
class CEntityCharacter;
class CEntityResultList;
class CEntityPlayer;
class CTransform
{
public:
	CTransform(){ Clear(); }
	~CTransform() {}

	void Clear()
	{
		mHasTransform = 0;
		mSexID = 0;
		mStateID = 0;
		mWeaponID = 0;
		mClothID = 0;
		mFashionID = 0;
		mNpcID = 0;
	}	

public:
	int				mHasTransform;		// 是否存在变身状态，0不存在；>0存在( buffid )
	int				mSexID;			// 性别
	int				mStateID;		// 变身状态
	int				mWeaponID;		// 武器ID
	int				mClothID;		// 衣服ID
	int				mFashionID;		// 时装ID
	int				mNpcID;			// 怪物ID
};

class CBuffObject 
{
public:
	unsigned int	mBuffID;			// Buff模版ID
	int				mPersistTime;		// Buff持续时间（毫秒为单位）
	unsigned int	mSrcEntityID;		// 释放者ID
	int				mLastInterval;		// 上次时间间隔
	int				mTimerType;			// 时间类型( 用于服务器存储,减少模板遍历 )
	int				mParames[ MAX_BUFFFUN_COUNT ];		// 扩展数据
	int				mParames2[ MAX_BUFFFUN_COUNT ];		// 扩展数据2
	int				mBackDamage;		// 返回伤害总量
	int				mAttack;			// 伤害值
	int				mBuffTimerID;		// buff定时器ID
	int				mDstEntityID;		// buff作用实体
	int				mBuffSuperCount;	// buff叠加层数
	int				mListID;	

public:
	CBuffObject( ) 
	{
		if ( CObj::msCreateMode )
		{
			Initialize( );
		} 
		else
		{
			Resume( );
		}
	}

	~CBuffObject( ) {}

	int Initialize( )
	{
		mBuffID = 0;
		mPersistTime = 0;
		mSrcEntityID = 0;
		mLastInterval = 0;
		mTimerType = 0;
		mBackDamage = 0;
		mAttack = 0;
		memset( mParames, 0, sizeof( mParames ) );
		memset( mParames2, 0, sizeof( mParames2 ) );
		mBuffTimerID = INVALID_OBJ_ID;
		mDstEntityID = 0;
		mBuffSuperCount = 0;

		mListID = 0;
		return 0;
	}

	int Resume( )
	{
		return 0;
	}

	// Buff对数值影响数值(固定或百分比)
	unsigned int GetAffectNumber( CTemplateBuff::BUFF_FUN_TYPE vBuffType );

	// Get & Set BuffTimerID
	int GetBuffTimerID( ) { return mBuffTimerID; }
	void SetBuffTimerID( int nTimeriD ) { mBuffTimerID = nTimeriD ;}

	// Get & Set SrcEntityID
	int GetBuffSrcEntityID() { return mSrcEntityID; }
	void SetBuffSrcEntityID( int nSrcEntityiD ) { mSrcEntityID = nSrcEntityiD; }

	void ChangeParames( int vBuffID, int vIndex, int vParames  );
	void ChangeParames2( int vBuffID, int vIndex, int vParames2 );

	void SetPersistTime( int nTime ) { mPersistTime = nTime; }
	int GetPersistTime( ) { return mPersistTime; }

	// 得到buff的作用间隔; -1表示失败；0表示是持续性作用buff; >0表示是间隔性作用buff
	int GetBuffActionTime( ) ;

};


/************************************************************************/
/*  Buff列表 ( 个数可以调整 )                                                                     */
/************************************************************************/
class CBuffList
{
public:
	enum { MAX_BUFF_NUM = 24, };
public:
	unsigned int	mBuffNumber;
	CBuffObject		mBuffList[ MAX_BUFF_NUM ]; // BUFF列表
	//	static int 		sBuffListID;
	int				mBuffListID;
	CTransform		mTransform;

public:
	CBuffList( ) 
	{
		if ( CObj::msCreateMode )
		{
			Initialize( );
		} 
		else
		{
			Resume( );
		}
	}

	~CBuffList( )
	{

	}

	int Initialize( )
	{
		Clean( );
		return 0;
	}

	int Resume( )
	{
		return 0;
	}

public:
	void Clean( )
	{
		mBuffNumber = 0;
		mBuffListID = 0;
		::memset( mBuffList, 0, sizeof( mBuffList ) );
		::memset( &mTransform, 0, sizeof( mTransform ) );
	}

	CBuffObject* InsertBuff( unsigned int vBuffID, int vPersistTime, CEntityCharacter* pSrcEntity, unsigned int& vLastBuffID, int vAttack, CEntityCharacter* pDstEntity, int* vListID, int* vResultCode );


	int GetBuffNum( ) { return mBuffNumber; }

	int RemoveBuff( unsigned int vBuffID, CEntityCharacter* pEntity, int* vListID = 0 );

	// 清除所有buff, 包括定时器
	int RemoveAllBuff( CEntityCharacter* pEntity );

	void CreateFromTlv2( CPropertyCharacter* vpCharacter, PBBuffList* pBuffer );	

	void GetTlvBuffer2( PBBuffList* pBuffer );

	// 得到仅供小队显示的buff信息
	void GetTlvBufferTeamShow( PBBuffList* pBuffer );

	// 得到用于玩家自身显示的buff信息
	void GetTlvBufferShow( PBBuffList* pBuffer );

	unsigned int GetFixNumber( CTemplateBuff::BUFF_FUN_TYPE vBuffType );

	// 得到BuffList对数值影响的百分比
	int GetPerNumber( CTemplateBuff::BUFF_FUN_TYPE vBuffType );

	CBuffObject* GetBuffObjectByID( int nBuffID );

	// 通过索引取得buff obj
	CBuffObject& GetBuffObjectByIndex( int nIdx ) { return mBuffList[nIdx]; }

	// 是否有buff
	bool HasBuff( int nBuffID );

	// 刷新Buff剩余时间
	void RefreshBuffLeftTime( );

	// 复制变身
	void SetTransform( CEntityCharacter *pEntity, int nBuffID );

	// 获得变身
	void GetTransform( PBTransform *pTran );

	// 如果有方寸山变身; >0表示BuffID
	int HasTransform( ) { return mTransform.mHasTransform; }
};