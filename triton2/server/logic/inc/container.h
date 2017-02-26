#pragma once

#include "sceneobj_manager.h"
#include "lk_vector.h"
#include "array_list.h"
#include "lk_sharedvector.h"

//////////////////////////////////////////////////////////////////////////
/// 可见玩家容器
//////////////////////////////////////////////////////////////////////////
class CSeenPlayerContainer
{
public:
	typedef lk::fixed_size_memory_pool< unsigned int, SERVER_CAP_SHARE_SEEN_PLAYER, 2> SharedSeenPlayerPool;
	typedef lk::shared_vector< unsigned int, SERVER_CAP_SHARE_SEEN_PLAYER > SeenPlayerVector;

public:
	
	CSeenPlayerContainer();
	~CSeenPlayerContainer();

	void Initailize();
	void Resume();

	void Add(unsigned int unID);

	void Remove( unsigned int unID);

	void Show( const char* pszLogName );

public:
	SeenPlayerVector	mPlayers;
};
 
//////////////////////////////////////////////////////////////////////////
/// 可见非玩家类角色实体,可攻击( pet, ogre, funcnpc )
//////////////////////////////////////////////////////////////////////////
class CSeenCharactorNpcContainer
{
	typedef lk::fixed_size_memory_pool< unsigned int, SERVER_CAP_SHARE_SEEN_CHARNPC, 2> SharedSeenNpcCharactorPool;
	typedef lk::shared_vector< unsigned int, SERVER_CAP_SHARE_SEEN_CHARNPC > SeenNpcCharactorPool;

public:
	CSeenCharactorNpcContainer();
	~CSeenCharactorNpcContainer();

	void Initailize();
	void Resume();

	void Add(unsigned int unID);

	void Remove( unsigned int unID);

	void Show( const char* pszLogName );

public:
	SeenNpcCharactorPool	mNpcCharactors;
};


//////////////////////////////////////////////////////////////////////////
/// 可见非玩家类静态实体，不可攻击 ( money, item, mine )
//////////////////////////////////////////////////////////////////////////
class CSeenStaticNpcContainer
{
	typedef lk::fixed_size_memory_pool< unsigned int, SERVER_CAP_SHARE_SEEN_STATICNPC, 2> SharedSeenNpcStaticPool;
	typedef lk::shared_vector< unsigned int, SERVER_CAP_SHARE_SEEN_STATICNPC > SeenNpcStaticVector;
public:

	CSeenStaticNpcContainer();
	~CSeenStaticNpcContainer();

	void Initailize();
	void Resume();

	void Add(unsigned int unID);

	void Remove( unsigned int unID);

	void Show( const char* pszLogName );

public:
	SeenNpcStaticVector	mStaticNpcs;
};


