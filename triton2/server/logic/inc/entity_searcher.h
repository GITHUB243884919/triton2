#pragma once

class CMessage;
class CEntity;
class CMapObject;


// 搜索可见表中的玩家
template< class Check >
struct SeenPlayerContainserSearcher
{
	SeenPlayerContainserSearcher( Check& rCheck, std::vector<CEntity*>& rEntitySet) 
		: m_Check( rCheck ), m_rEntitys(rEntitySet) {}


	void Visit( CEntity* pEntity );


	Check& m_Check;							// 检查器(函数对象)
	std::vector<CEntity*>& m_rEntitys;		
};

// 搜索可见表中的NPC
template< class Check >
struct SeenNpcContainserSearcher
{
	SeenNpcContainserSearcher( Check& rCheck, std::vector<CEntity*>& rEntitySet) 
		: m_Check( rCheck ), m_rEntitys(rEntitySet) {}


	void Visit( CEntity* pEntity );


	Check& m_Check;							// 检查器(函数对象)
	std::vector<CEntity*>& m_rEntitys;		
};




// 搜索地图区域内的所有类型实体,由Check来做判断是否加入
template< class Check >
struct MapRegionEntitySearcher
{
	MapRegionEntitySearcher(Check& rCheck) : m_Check( rCheck ) {}
	MapRegionEntitySearcher(Check& rCheck, CWTPoint& rTopLeft, CWTPoint& rBottomRight) 
		: m_Check( rCheck ), m_TopLeft( rTopLeft ), m_BottomRight( rBottomRight ) 
	{}

	Check& m_Check;							// 检查器(函数对象)
	CWTPoint& m_TopLeft;					// 左上角
	CWTPoint& m_BottomRight;				// 右下角

	void Visit( CMapObject* pMap );			// 访问器
};



// 函数对象结果
enum emObjectFunRsult 
{
	EM_OBJFUN_NONE				= 0, // 没有结果
	EM_OBJFUN_ENTITY_NOTEXIST	= 1, // 对象ID不存在
	EM_OBJFUN_ENTITY_TERMINATE	= 2, // 提前结束
};


// 可供地图查询用的
class ICheckerForMapSearch
{
public:

	virtual bool operator() ( int ) = 0;
	virtual int GetResult( ) = 0;
	virtual void SetResult( int  ) = 0; 
};


/**
	1.玩家向可见表中的实体，同时被添加的实体也要向可见表中添加该玩家
	2.如果是向可见表中添加玩家，那么把该玩家放到容器中，以便后面发创建实体消息给玩家
	3.如果是向可见表中添加NPC,那么把该NPC放到NPC容器中，以便后面给玩家发送创建NPC的消息
	4.陷阱特殊处理
*/
class VisibleViewEntityChecker : public ICheckerForMapSearch
{
public:

	VisibleViewEntityChecker(CEntityPlayer* pSelf) : m_pSelf( pSelf ), mResult( 0 )  {}

	bool operator() ( int nEntityID )
	{
		CEntity* pTarget = (CEntity*)CSceneObjManager::GetSingletonPtr()->GetObject( nEntityID );
		if( pTarget == NULL )
		{
			mResult = EM_OBJFUN_ENTITY_NOTEXIST;
			LOG_ERROR( "default", "[%s:%d] entity %d can't find entity %d", __FUNCTION__, __LINE__,
				m_pSelf->GetEntityID(),	nEntityID );
			return false;
		}

		if( pTarget->IsVisibleFor( m_pSelf ) )
		{	
			m_pSelf->AddViewEntity( pTarget );
			pTarget->AddViewEntity( m_pSelf );

			if( pTarget->IsPlayer() )
				m_Players.push_back( (CEntityPlayer*)pTarget );
			else
				m_Npcs.push_back( pTarget );
		}
		else
		{
			// 这里主要是处理陷阱，尽管玩家看不见陷阱，但是陷阱的可见表里要有玩家，在陷阱
			// 销毁时为了简单起见，会广播销毁消息
			if( pTarget->IsPedal() )
			{
				pTarget->AddViewEntity( m_pSelf );
			}
		}

		return true;
	}

	inline int GetResult( ) { return mResult; }
	inline void SetResult( int nRet ) { mResult = nRet; }

	CEntityPlayer* m_pSelf;
	std::vector<CEntity*>		m_Npcs;		// 实体容器
	std::vector<CEntityPlayer*> m_Players;	// 玩家容器
	int mResult; // 用于保存中间结果
};


/**
	1.NPC向可见表中添加实体，同时被添加的实体也要向可见表中添加该玩家
	2.如果是向可见表中添加NPC,那么把该NPC放到NPC容器中，以便后面给玩家发送创建NPC的消息
	3.陷阱特殊处理
*/
class VisibleViewPlayerChecker : public ICheckerForMapSearch
{
public:

	VisibleViewPlayerChecker(CEntity* pSelf) : m_pNPCSelf( pSelf ),mResult( 0 )  {}

	bool operator() (int nEntityID)
	{
		CEntity* pTarget = (CEntity*)CSceneObjManager::GetSingletonPtr()->GetObject( nEntityID );
		if( pTarget == NULL )
		{
			mResult = EM_OBJFUN_ENTITY_NOTEXIST;
			LOG_ERROR( "default", "[%s:%d] entity %d can't find entity %d", __FUNCTION__, __LINE__,
				m_pNPCSelf->GetEntityID(),	nEntityID );
			return false;
		}

		if( m_pNPCSelf->IsVisibleFor( pTarget ) )
		{	
			m_pNPCSelf->AddViewEntity( pTarget );
			pTarget->AddViewEntity( m_pNPCSelf );

			if( pTarget->IsPlayer() )
				m_Players.push_back( (CEntityPlayer*)pTarget );

		}
		else
		{
			// 对陷阱特殊处理
			if( m_pNPCSelf->IsPedal() )
			{
				m_pNPCSelf->AddViewEntity( pTarget );
			}
		}

		return true;
	}

	inline int GetResult( ) { return mResult; }
	inline void SetResult( int nRet ) { mResult = nRet; }

	CEntity* m_pNPCSelf; // 自己是非玩家
	std::vector<CEntityPlayer*> m_Players;	// 玩家容器
	int mResult; // 用于保存中间结果
};


/**
	1.玩家删除可见表中的实体，同时被删除的实体也要从可见表中删除该玩家
	2.如果是从可见表中删除玩家，那么把该玩家放到容器中，以便后面发销毁实体消息给玩家
	3.如果是从可见表中删除NPC,那么把该NPC放到NPC容器中，以便后面给玩家发送销毁NPC的消息
	4.陷阱特殊处理
*/
class VisibleViewEntityDeleter : public ICheckerForMapSearch
{
public:

	VisibleViewEntityDeleter(CEntityPlayer* pSelf) : m_pSelf( pSelf ),mResult( 0 )  {}

	bool operator() (int nEntityID)
	{
		CEntity* pTarget = (CEntity*)CSceneObjManager::GetSingletonPtr()->GetObject( nEntityID );
		if( pTarget == NULL )
		{
			mResult = EM_OBJFUN_ENTITY_NOTEXIST;
			LOG_ERROR( "default", "[%s:%d] entity %d can't find entity %d", __FUNCTION__, __LINE__,
				m_pSelf->GetEntityID(),	nEntityID );
			return false;
		}


		if( pTarget->IsVisibleFor( m_pSelf ) )
		{	
			pTarget->DeleteViewEntity( m_pSelf );
			m_pSelf->DeleteViewEntity( pTarget );

			if( pTarget->IsPlayer() )
				m_Players.push_back( (CEntityPlayer*)pTarget );
			else
				m_Npcs.push_back( pTarget );

		}
		else
		{
			// 玩家对陷阱不可见的话，也要从陷阱的可见表里把玩家删除
			if( pTarget->IsPedal() )
			{
				pTarget->DeleteViewEntity( m_pSelf );
			}
		}

		return true;
	}


	inline int GetResult( ) { return mResult; }
	inline void SetResult( int nRet ) { mResult = nRet; }


	CEntityPlayer* m_pSelf;
	std::vector<CEntity*>		m_Npcs;		// 实体容器
	std::vector<CEntityPlayer*> m_Players;	// 玩家容器
	int mResult; // 用于保存中间结果
};


/**
	1.NPC删除可见表中的实体，同时被删除的实体也要从可见表中删除该NPC
	2.如果是从可见表中删除玩家，那么把该玩家放到容器中，以便后面发销毁实体消息给玩家
	3.陷阱特殊处理
*/
class VisibleViewPlayerDeleter : public ICheckerForMapSearch
{
public:

	VisibleViewPlayerDeleter(CEntity* pSelf) : m_pNpcSelf( pSelf ),mResult(0)  {}
	
	bool operator() (int nEntityID)
	{
		CEntity* pTarget = (CEntity*)CSceneObjManager::GetSingletonPtr()->GetObject( nEntityID );
		if( pTarget == NULL )
		{
			mResult = EM_OBJFUN_ENTITY_NOTEXIST;
			LOG_ERROR( "default", "[%s:%d] entity %d can't find entity %d", __FUNCTION__, __LINE__,
				m_pNpcSelf->GetEntityID(),	nEntityID );
			return false;
		}


		if( pTarget->IsVisibleFor( m_pNpcSelf ) )
		{	
			m_pNpcSelf->DeleteViewEntity( pTarget );
			pTarget->DeleteViewEntity( m_pNpcSelf );
	
			if( pTarget->IsPlayer() )
				m_Players.push_back( (CEntityPlayer*)pTarget );	
		}
		else
		{
			// 陷阱特殊处理
			if( m_pNpcSelf->IsPedal() )
			{
				m_pNpcSelf->DeleteViewEntity( pTarget );
			}
		}

		return true;
	}


	inline int GetResult( ) { return mResult; }
	inline void SetResult( int nRet ) { mResult = nRet; }

	CEntity* m_pNpcSelf;
	std::vector<CEntityPlayer*> m_Players;	// 玩家容器
	int mResult; // 用于保存中间结果
};


// 可攻击实体检查器, 主要用于怪物或召唤兽搜索攻击目标
class AttackableEntityChecker
{
public:

	AttackableEntityChecker( CEntityCharacter* pSrcEntity ) : m_pSrcEntity( pSrcEntity ) { }

	bool operator() (CEntity* pTarget);

	CEntityCharacter* m_pSrcEntity;
};


// 地图所有玩家实体
class MapPlayerChecker : public ICheckerForMapSearch
{
public:

	MapPlayerChecker(  ):mNum(0), mDistance(10000), mLineID(-1), mEntityPtr(NULL),mResult(0) { }
	CEntityPlayer* mPlayerList[MAX_WEDDINGHALL_PLAYENUM];
	int			   mNum;
	int			   mDistance;
	int			   mLineID;
	CEntity		   *mEntityPtr;		
	int				mResult;
	inline int GetResult( ) { return mResult; }
	inline void SetResult( int nRet ) { mResult = nRet; }

	bool operator()( int nEntityID )
	{		
		if ( mNum >= MAX_WEDDINGHALL_PLAYENUM )
		{
			mResult = EM_OBJFUN_ENTITY_TERMINATE;
			return false;
		}
			
		CEntity* tpEntity = (CEntity*)CSceneObjManager::GetSingletonPtr()->GetObject( nEntityID );
		if ( tpEntity->IsPlayer() == true )
		{
			mPlayerList[mNum] = ( CEntityPlayer * )tpEntity;
			++mNum;		
		}			
		return true;
	}		
			
	bool operator()( CEntity *pEntity )			
	{
		if ( pEntity->IsPlayer() == true )
		{
			if ( mNum >= MAX_WEDDINGHALL_PLAYENUM )
			{
				mResult = EM_OBJFUN_ENTITY_TERMINATE;
				return false;
			}
			
			bool bAddPlayer = false;
			if ( mEntityPtr == NULL )
			{
				bAddPlayer = true;
			}
			else
			{
				if ( mEntityPtr->IsWithinDistance( pEntity, mDistance ) == true )
				{
					if ( mLineID < 0 )
					{
						bAddPlayer = true;
					}
					else
					{
						if ( ( ( CEntityPlayer * )pEntity )->GetNationality() == mLineID )
						{
							bAddPlayer = true;
						}
					}
				}				
			}
			
			if ( bAddPlayer == true )
			{
				mPlayerList[mNum] = ( CEntityPlayer * )pEntity;
				++mNum;					
			}		
		}			
		return true;
	}

};

// 寻找一个功能npc
class MapFuncNpcChecker : public ICheckerForMapSearch 
{
public:

	MapFuncNpcChecker():mEntityID(0){ }
	int  mEntityID;
	
	bool operator()( int nEntityID ) 
	{
		CEntity* tpEntity = (CEntity*)CSceneObjManager::GetSingletonPtr()->GetObject( nEntityID );
		if ( tpEntity != NULL & tpEntity->IsFunNpc() == true )
		{
			mEntityID = nEntityID;
			return false;
		}				
		return true;
	}

	int GetResult( ) { return EM_OBJFUN_NONE; }
	void SetResult( int )  {;} 
};

