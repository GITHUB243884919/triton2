//=========================================================================
// union.h
//=========================================================================
#pragma once

#include "entity.h"
#include "corpstype.h"
#include "corpsinfo_pb.hxx.pb.h"   


	
//-------------------------------------------------------------------------
class MsgList
{
public:
	typedef lk::vector< int, MAX_CORPSAPPLY_NUM > ApplyVector;

public:
	ApplyVector mList;

public:
	void Initialize( ){ mList.initailize(); }

	inline bool IsFull( ){ return mList.size() == mList.max_size(); }

	inline bool InList( int vID )
	{ 
		for ( ApplyVector::iterator it = mList.begin(); it != mList.end(); ++it )
		{
			if ( *it == vID )
			{
				return true;
			}
		}
		return false;
	}

	inline void Insert( int vID ){ mList.push_back( vID ); }

	inline void Remove( int vID )
	{
		for ( ApplyVector::iterator it = mList.begin(); it != mList.end(); ++it )
		{
			if ( *it == vID )
			{
				mList.erase( it );
				return;
			}
		}
	}
}; 

/**
** 军盟
** 军盟中子军团个数一共有9个，加上主军团自身，总共是10个军团的联盟
** 因为程序里很多地方都会用到军盟列表或者军盟参战列表，我把MAX_CORPS_UNION暂时放到10个，
** 意味着是所有军团都在列表里，不再区分主，子军团。
** 但是军盟加军团的时候要注意区分，实际上只能接受9个军团。
**
*/
class CUnion : public CObj
{
public:
	typedef lk::vector< int, MAX_CORPS_UNION-1 > CorpsVector;	
	enum ECorpsList
	{
		CORPSLISTFULL	 = 1,
		CORPSHASEXIST	 = 2,
	};

	typedef lk::vector< int, MAX_BATTLESIZE > UnionBattleList;

public:
	int		mUnionID;												// 联军ID
	char	mUnionName[MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES];		// 联军名称
	char	mHeadName[NAME_LENGTH];									// 团长名称
	char	mCoreName[MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES];			// 主军团名称
	int		mCoreID;												// 主军团ID
	int		mCorpNum;												// 军团数量
	int		mCity;													// 主城地图
	int		mMapNum;												// 地图数量
	char	mBulletin[CORPS_BULLETIN_LENGTH*MAX_UTF8_BYTES];		// 公告
	int		mMemberNum;												// 人口数量

	CorpsVector mCorpsList;
	MsgList mApplyList;
	MsgList mInviteList;

	UnionBattleList mUnionBattleList;
public:
	CUnion()
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

	~CUnion() {}

	int Initialize()
	{
		mUnionID = 0;
		mCoreID = 0;
		mCorpNum = 0;
		mCity = 0;
		mMapNum = 0;
		mMemberNum = 0;
		memset( mUnionName, 0, ARRAY_CNT(mUnionName) );
		memset( mHeadName, 0, ARRAY_CNT(mHeadName) );
		memset( mCoreName, 0, ARRAY_CNT(mCoreName) );
		memset( mBulletin, 0, ARRAY_CNT(mBulletin) );
		mCorpsList.initailize();
		mApplyList.Initialize();
		mInviteList.Initialize();
		mUnionBattleList.initailize();
		return 0;
	}

	int Resume() {return 0;}

	void SetUnionID( const int vID )
	{ 
		mUnionID = vID;
	}
	int GetUnionID(){ return mUnionID; }

	void SetUnionName( const char* vpName )
	{ 
		if ( vpName == NULL ) return ;
		strncpy( mUnionName, vpName, sizeof( mUnionName ) - 1 );
	}
	char* GetUnionName(){ return mUnionName; }

	void SetHeadName( const char* vpName )
	{ 
		if ( vpName == NULL ) return ;
		strncpy( mHeadName, vpName, sizeof( mHeadName ) - 1 );
	}
	char* GetHeadName(){ return mHeadName; }

	void SetCoreName( const char* vpName )
	{ 
		if ( vpName == NULL ) return ;
		strncpy( mCoreName, vpName, sizeof( mCoreName ) - 1 );
	}
	char* GetCoreName(){ return mCoreName; }

	// 主军团
	void SetCoreID( const int vID )
	{ 
		mCoreID = vID;
	}
	int GetCoreID(){ return mCoreID; }

	void SetCorpNum( const int vNum )
	{ 
		mCorpNum = vNum;
	}
	int GetCorpNum(){ return mCorpNum; }

	void SetCity( const int vCity )
	{ 
		mCity = vCity;
	}
	int GetCity(){ return mCity; }

	void SetMapNum( const int vNum )
	{ 
		mMapNum = vNum;
	}
	int GetMapNum(){ return mMapNum; }

	void SetBulletin( const char* vpBulletin )
	{ 
		if ( vpBulletin == NULL ) return ;
		strncpy( mBulletin, vpBulletin, sizeof( mBulletin ) - 1 );
	}
	char* GetBulletin(){ return mBulletin; }

	void SetMemberNum( const int vNum )
	{ 
		mMemberNum = vNum;
	}
	int GetMemberNum(){ return mMemberNum; }

	int InsertCorps( int vCorpsID )
	{
		if ( mCorpsList.size() == mCorpsList.max_size() )
			return CORPSLISTFULL;
		for ( CorpsVector::iterator it = mCorpsList.begin(); it != mCorpsList.end(); ++it )
		{
			if ( *it == vCorpsID )
				return CORPSHASEXIST;
		}
		mCorpsList.push_back( vCorpsID );
		return 0;
	}

	void RemoveCorps( int vCorpsID )
	{
		for ( CorpsVector::iterator it = mCorpsList.begin(); it != mCorpsList.end(); ++it )
		{
			if ( *it == vCorpsID )
			{
				// 参战列表处理
				this->DeleteBattleMemberByCorpsID( vCorpsID );

				mCorpsList.erase( it );
				return;
			}
		}
	}

	bool HasCorps( int vCorpsID )
	{
		// 主军团也算军盟内的军团
		if( vCorpsID == mCoreID )
			return true;

		for ( CorpsVector::iterator it = mCorpsList.begin(); it != mCorpsList.end(); ++it )
		{
			if ( *it == vCorpsID )
				return true;
		}
		return false;
	}

	void GetCorpsList( int *vCorpsID, int &vNumber )
	{
		int tNumber = 1;
		vCorpsID[ 0 ] = mCoreID;
		for ( CorpsVector::iterator it = mCorpsList.begin(); it != mCorpsList.end(); ++it )
		{
			if ( tNumber >= vNumber )
				break;
			vCorpsID[ tNumber++ ] = *it;
		}
		vNumber = tNumber;
	}

	void GetUnionBasicInfo( PBUnionBasicInfo *vInfo );	

	int InsertBattleMember( int nRoleID );
	int ClearBattleList();
	int GetBattleMemberList( int* vpList, int& nSize );
	bool IsInBattleList( int nRoleID );

	// 删除参战人员 [2010-03-14 GaoHong]
	int DeleteBattleMemberByCorpsID( int nCorpsID );
	int DeleteBattleMemberByRoleID( int nRoleID );

	// 联盟势力值
	int GetUnionPower();


};
