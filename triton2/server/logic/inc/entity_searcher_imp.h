#ifndef __ENTITY_SEACHER_IMP__
#define __ENTITY_SEACHER_IMP__

#include "entity_searcher.h"
#include "entity.h"
#include "sceneobj_manager.h"
#include "mapobjmanager.h"
#include "propertymodule.h"


// 搜索可见表中的玩家
template < class Check >
void SeenPlayerContainserSearcher<Check>::Visit( CEntity* pEntity  )
{
	CSeenPlayerContainer::SeenPlayerVector::iterator it = pEntity->GetSeenPlayersContainer().mPlayers.begin();

	for( ; it != pEntity->GetSeenPlayersContainer().mPlayers.end(); )
	{
		CEntity* pSeenEntity = (CEntity*) CSceneObjManager::GetSingletonPtr()->GetObject(*it);
		if( pSeenEntity == NULL )
		{
			LOG_ERROR( "default", "can't find %d in entity(%d)'s viewlist, map(%d) pos(%d,%d) ", 
				*it, pEntity->GetEntityID(), pEntity->GetMapID(), pEntity->GetPosX(), pEntity->GetPosY() );
			
			// share vector遍历中删除方法类似hash_map
			pEntity->GetSeenPlayersContainer().mPlayers.erase( it++ );

			continue ;
		}

		++it;

		if( m_Check( pSeenEntity) )
		{
			m_rEntitys.push_back( pSeenEntity );
		}
	}

}


// 搜索可见表中的NPC
template < class Check >
void SeenNpcContainserSearcher<Check>::Visit( CEntity* pEntity  )
{
	if( pEntity->IsNpc() == false )
	{
		return ;
	}

	CEntityNpc* pNpc = (CEntityNpc*)pEntity;

	CSeenCharactorNpcContainer::SeenNpcCharactorPool::iterator it = pNpc->GetSeenNpcsContainer().mNpcCharactors.begin();

	for( ; it != pNpc->GetSeenNpcsContainer().mNpcCharactors.end();)
	{
		CEntity* pSeenEntity = (CEntity*) CSceneObjManager::GetSingletonPtr()->GetObject(*it);
		if( pSeenEntity == NULL )
		{
			LOG_ERROR( "default", "can't find %d in entity(%d)'s viewlist, map(%d) pos(%d,%d) ", 
				*it, pEntity->GetEntityID(), pEntity->GetMapID(), pEntity->GetPosX(), pEntity->GetPosY() );

			// share vector遍历中删除方法类似hash_map
			pNpc->GetSeenNpcsContainer().mNpcCharactors.erase( it++ );

			continue ;
		}

		++it;

		if( m_Check( pSeenEntity) )
		{
			m_rEntitys.push_back( pSeenEntity );
		}
	}

}



// 地图区域搜索( -> 逐行遍历格子 -> 遍历格子里的实体 )
template< class Check >
void MapRegionEntitySearcher<Check>::Visit( CMapObject* pMap )
{
	int nTopLeftX = std::max( m_TopLeft.mX, 0 );
	int nTopLeftY = std::max( m_TopLeft.mY, 0 );
	int nBottomRightX = std::min( m_BottomRight.mX, (int)pMap->GetWidth()-1 );
	int nBottomRightY = std::min( m_BottomRight.mY, (int)pMap->GetHight()-1 );

	for ( int x = nTopLeftX; x <= nBottomRightX; x ++ )
	{
		for ( int y = nTopLeftY; y <= nBottomRightY; y ++ )
		{
			CMapCell* tpCell = pMap->GetCell( CWTPoint( x, y ) );
			LK_ASSERT( tpCell != NULL, continue );

			tpCell->Tick( m_Check );
			// 提前结束
			if( m_Check.GetResult() == EM_OBJFUN_ENTITY_TERMINATE)
			{
				return;
			}
		}
	}

}


// 召唤兽搜索敌人
struct MapCellSearchEnemy
{
	MapCellSearchEnemy( CEntityPet* pPet ) :
		mpSelfEntity( pPet ), mpMapObject( pPet->GetMapObject() ), mEnemys(NULL)  {}
	~MapCellSearchEnemy() {}

	CEntityPet* mpSelfEntity;
	CMapObject* mpMapObject;
	CEntityCharacter* mEnemys;

	int GetResult( ) { return EM_OBJFUN_NONE; }
	void SetResult( int )  {;} 

	//返回值: false-终止遍历; true-继续
	bool operator ()( unsigned int unEntityID )
	{
		CEntity* tpDesEntity = (CEntity*) CSceneObjManager::GetSingletonPtr()->GetObject( unEntityID );
		LK_ASSERT( tpDesEntity != NULL, return true ); // 不中断遍历,任何情况都一直找下去

		// 不能攻击主人 和 自己
		if( (mpSelfEntity->GetOwnerID() == unEntityID) || ( mpSelfEntity->GetEntityID() == unEntityID ) )
			return false;

		//// 敌对 & 活的 & 非绝对安全区
		if( CPropertyModule::GetSingletonPtr()->EntityPKValid( mpSelfEntity, tpDesEntity, NULL, false ) )
		{
			mEnemys = (CEntityCharacter*)tpDesEntity;
		}
		return false;
	}

	void Visit()
	{
		int nTopLeftX = std::max( mpSelfEntity->GetPosX()-5, 0 );
		int nTopLeftY = std::max( mpSelfEntity->GetPosY()-5, 0 );
		int nBottomRightX = std::min( mpSelfEntity->GetPosX()+5, (int)mpMapObject->GetWidth()-1 );
		int nBottomRightY = std::min( mpSelfEntity->GetPosY()+5, (int)mpMapObject->GetHight()-1 );

		for ( int x = nTopLeftX; x <= nBottomRightX; x ++ )
		{
			for ( int y = nTopLeftY; y <= nBottomRightY; y ++ )
			{
				CMapCell* tpCell = mpMapObject->GetCell( CWTPoint( x, y ) );
				LK_ASSERT( tpCell != NULL, continue );

				tpCell->Tick( *this );

				// 找到一个就返回
				if( mEnemys != NULL )
				{
					return ;
				}
			}
		}
	}

};



// 范围搜索敌人(可以传入技能)
struct MapCellSearchEnemys
{
	MapCellSearchEnemys( CEntityCharacter* pSelf, int nLimit, int nRange, CTemplateSkill* pSkill ) :
		mpSelfEntity( pSelf ), mpMapObject( mpSelfEntity->GetMapObject() ),
		mEnemyLimit(nLimit), mSkillTpl( pSkill ), mRange( nRange ) {}
	~MapCellSearchEnemys() {}

	CEntityCharacter* mpSelfEntity;
	CMapObject* mpMapObject;
	std::vector<CEntityCharacter*> mEnemys;
	int mEnemyLimit;	// 查找上限
	CTemplateSkill* mSkillTpl;
	int mRange; // 作用范围

	int GetResult( ) { return EM_OBJFUN_NONE; }
	void SetResult( int )  {;} 

	//返回值: false-终止遍历; true-继续
	bool operator ()( unsigned int unEntityID )
	{
		CEntity* tpDesEntity = (CEntity*) CSceneObjManager::GetSingletonPtr()->GetObject( unEntityID );
		LK_ASSERT( tpDesEntity != NULL, return true ); 

		if ( CPropertyModule::GetSingleton().EntityPKValid( mpSelfEntity, 
			tpDesEntity, 
			mSkillTpl,
			mSkillTpl ? true : false ) == true )
		{
			mEnemys.push_back( (CEntityCharacter*)tpDesEntity );
		}

		return true;
	}

	void Visit()
	{
		int nTopLeftX = std::max( mpSelfEntity->GetPosX()-mRange, 0 );
		int nTopLeftY = std::max( mpSelfEntity->GetPosY()-mRange, 0 );
		int nBottomRightX = std::min( mpSelfEntity->GetPosX()+mRange, (int)mpMapObject->GetWidth()-1 );
		int nBottomRightY = std::min( mpSelfEntity->GetPosY()+mRange, (int)mpMapObject->GetHight()-1 );

		for ( int x = nTopLeftX; x <= nBottomRightX; x ++ )
		{
			for ( int y = nTopLeftY; y <= nBottomRightY; y ++ )
			{
				CMapCell* tpCell = mpMapObject->GetCell( CWTPoint( x, y ) );
				LK_ASSERT( tpCell != NULL, continue );

				tpCell->Tick( *this );

				if( mEnemys.size() >= (unsigned int)mEnemyLimit )
				{
					return;
				}
			}
		}
	}

};


// 踏板(陷阱,传送,神符)搜索
struct MapCellPedalSearcher
{
	MapCellPedalSearcher( CEntityPedal* pPedal ) :
		mpSelfEntity( pPedal ), mpMapObject( pPedal->GetMapObject() ) {}
	~MapCellPedalSearcher() {}

	CEntityPedal* mpSelfEntity;
	CMapObject* mpMapObject;
	std::vector<CEntityCharacter*> mEntitysOnPedal;

	int GetResult( ) { return EM_OBJFUN_NONE; }
	void SetResult( int )  {;} 

	//返回值: false-终止遍历; true-继续遍历
	bool operator ()( unsigned int unEntityID )
	{
		CEntity* tpDesEntity = (CEntity*) CSceneObjManager::GetSingletonPtr()->GetObject( unEntityID );
		if( tpDesEntity == NULL )
		{
			LOG_ERROR( "map", "[%s:%d] map[id:%d index:%d] can't find entity %d", __FUNCTION__, __LINE__,
				mpMapObject->GetMapID(), mpMapObject->GetMapIndex(), unEntityID );
			return true; // 不中断遍历,任何情况都一直找下去
		}

		switch ( mpSelfEntity->GetPedalType() )
		{
		case PEDAL_TRAP:
			{
				// 敌对 & 活的 & 非绝对安全区
				if( tpDesEntity->IsCharacter() &&  mpSelfEntity->IsHostileTo( tpDesEntity ) && tpDesEntity->IsAlive() )
				{
					if( tpDesEntity->IsPlayer() || tpDesEntity->IsPet() )
					{
						if( mpMapObject->IsAbsSafe() == false )
						{
							mEntitysOnPedal.push_back( (CEntityCharacter*)tpDesEntity );
						}
					}
					else
					{
						mEntitysOnPedal.push_back( (CEntityCharacter*)tpDesEntity );
					}
				}

				break;
			}
		case PEDAL_TELEPORT:
		case PEDAL_HIEROGRAM:
			{
				if( tpDesEntity->IsPlayer() && tpDesEntity->IsAlive() )
				{
					mEntitysOnPedal.push_back( (CEntityCharacter*)tpDesEntity );
				}
				break;
			}
		case PEDAL_NEEDFIRE:
			{
				if( tpDesEntity->IsPlayer() && tpDesEntity->IsAlive() )
				{
					mEntitysOnPedal.push_back( (CEntityCharacter*)tpDesEntity );
				}
				break;
			}
		default:
			{
				break;
			}
		}

		return true;
	}

	// 搜索触发范围
	void Visit()
	{
		int nTopLeftX = std::max( mpSelfEntity->GetSrcRect().TopLeft().mX , 0 );
		int nTopLeftY = std::max(mpSelfEntity->GetSrcRect().TopLeft().mY, 0 );
		if ( mpMapObject == NULL )
			return;
		int nBottomRightX = std::min( mpSelfEntity->GetSrcRect().BottomRight().mX, (int)mpMapObject->GetWidth()-1 );
		int nBottomRightY = std::min(  mpSelfEntity->GetSrcRect().BottomRight().mY, (int)mpMapObject->GetHight()-1 );

		for ( int x = nTopLeftX; x <= nBottomRightX; x ++ )
		{
			for ( int y = nTopLeftY; y <= nBottomRightY; y ++ )
			{
				CMapCell* tpCell = mpMapObject->GetCell( CWTPoint( x, y ) );
				LK_ASSERT( tpCell != NULL, continue );

				tpCell->Tick( *this );

				// 找到一个触发实体就可以
				if( mEntitysOnPedal.size() > 0 )
					return ;

			}
		}
	}

	// 搜索作用范围
	void Visit2()
	{
		// 清空
		mEntitysOnPedal.clear();

		int nTopLeftX = std::max( mpSelfEntity->GetActRect().TopLeft().mX , 0 );
		int nTopLeftY = std::max(mpSelfEntity->GetActRect().TopLeft().mY, 0 );
		if ( mpMapObject == NULL )
			return;
		int nBottomRightX = std::min( mpSelfEntity->GetActRect().BottomRight().mX, (int)mpMapObject->GetWidth()-1 );
		int nBottomRightY = std::min(  mpSelfEntity->GetActRect().BottomRight().mY, (int)mpMapObject->GetHight()-1 );

		for ( int x = nTopLeftX; x <= nBottomRightX; x ++ )
		{
			for ( int y = nTopLeftY; y <= nBottomRightY; y ++ )
			{
				CMapCell* tpCell = mpMapObject->GetCell( CWTPoint( x, y ) );
				LK_ASSERT( tpCell != NULL, continue );

				tpCell->Tick( *this );

			}
		}
	}
	
};


// 搜索一个物品掉落点
struct MapCellDropPosSearcher
{
	MapCellDropPosSearcher( CWTPoint& rPos, CMapObject* pMapObject ) 
		: mPos( rPos ), mpMapObject( pMapObject ), mHasItem( false ) {}
	~MapCellDropPosSearcher() {}

	CWTPoint& mPos;
	CMapObject* mpMapObject;
	bool mHasItem; 

	int GetResult( ) { return EM_OBJFUN_NONE; }
	void SetResult( int )  {;} 

	//返回值: false-终止遍历; true-继续遍历
	bool operator ()( unsigned int unEntityID )
	{
		CEntity* tpDesEntity = (CEntity*) CSceneObjManager::GetSingletonPtr()->GetObject( unEntityID );
		LK_ASSERT( tpDesEntity != NULL, return true); 

		if( tpDesEntity->IsItem() || tpDesEntity->IsRedStone() )
		{
			mHasItem = true;
			return false; // 找到，终止
		}
		return true;
	}

	void Visit()
	{
		CMapCell* tpCell = mpMapObject->GetCell( mPos );
		LK_ASSERT( tpCell != NULL, return );

		tpCell->Tick( *this );
	}
};


// 搜索一个NPC创建点(动态创建)
struct MapCellNpcCreatePosSearcher
{
	MapCellNpcCreatePosSearcher( CWTPoint& rPos, CMapObject* pMapObject, EEntityType nSrcType ) 
		: mPos( rPos ), mpMapObject( pMapObject ), mHasNpc( false ), mSrcType(nSrcType) {}
	~MapCellNpcCreatePosSearcher() {}

	CWTPoint& mPos;
	CMapObject* mpMapObject;
	bool mHasNpc; 
	EEntityType mSrcType;

	int GetResult( ) { return EM_OBJFUN_NONE; }
	void SetResult( int )  {;} 

	//返回值: false-终止遍历; true-继续遍历
	bool operator ()( unsigned int unEntityID )
	{
		CEntity* tpDesEntity = (CEntity*) CSceneObjManager::GetSingletonPtr()->GetObject( unEntityID );
		LK_ASSERT( tpDesEntity != NULL, return true); 

		if( tpDesEntity->GetEntityType() == mSrcType ) // 同类不共存
		{
			mHasNpc = true;
			return false; // 找到，终止
		}
		return true;
	}

	void Visit()
	{
		CMapCell* tpCell = mpMapObject->GetCell( mPos );
		LK_ASSERT( tpCell != NULL, return );

		tpCell->Tick( *this );
	}
};


// 搜索一个坐标格子是否有摆摊
struct MapCellStallSearcher
{
	MapCellStallSearcher( const CWTPoint& rPos, CMapObject* pMapObject ) 
		: mPos( rPos ), mpMapObject( pMapObject ), mHasStall( false ) {}
	~MapCellStallSearcher() {}

	const CWTPoint& mPos;
	CMapObject* mpMapObject;
	bool mHasStall;

	int GetResult( ) { return EM_OBJFUN_NONE; }
	void SetResult( int )  {;} 

	//返回值: false-终止遍历; true-继续遍历
	bool operator ()( unsigned int unEntityID )
	{
		CEntity* tpDesEntity = (CEntity*) CSceneObjManager::GetSingletonPtr()->GetObject( unEntityID );
		LK_ASSERT( tpDesEntity != NULL, return true);

		if ( tpDesEntity->IsPlayer() && tpDesEntity->IsAlive() == true )
		{
			if (  ((CEntityPlayer*) tpDesEntity )->HasStall() == true )
			{
				mHasStall = true;
				return false; // 找到，终止
			}
		}

		return true;
	}

	void Visit()
	{
		CMapCell* tpCell = mpMapObject->GetCell( mPos );
		LK_ASSERT( tpCell != NULL, return );

		tpCell->Tick( *this );
	}

};


// 对于npc来讲,格子是否被占用
//(各种类型npc可能互斥需求不一样，如，怪物，护送npc等)
struct MapCellNpcOccupiedSearcher
{
	MapCellNpcOccupiedSearcher( CWTPoint& rPos, CEntityNpc* pNpc ) 
		: mPos( rPos ), mpNpc( pNpc ), mIsOccupied( false ) {}
	~MapCellNpcOccupiedSearcher() {}

	CWTPoint& mPos;		//坐标点一定是和npc在一个地图上，否则没意义
	CEntityNpc* mpNpc;	//npc类型
	bool mIsOccupied;	//占用

	int GetResult( ) { return EM_OBJFUN_NONE; }
	void SetResult( int )  {;} 

	//返回值: false-终止遍历; true-继续遍历
	bool operator ()( unsigned int unEntityID )
	{
		CEntity* tpDesEntity = (CEntity*) CSceneObjManager::GetSingletonPtr()->GetObject( unEntityID );
		LK_ASSERT( tpDesEntity != NULL, return true); 

		// 任务(功能)npc畅通无阻
		if( mpNpc->IsFunNpc() )
		{
			mIsOccupied = false;
			return false; // 终止遍历
		}
		// 怪物
		if( mpNpc->IsOgre() && tpDesEntity->IsNpc() )
		{
			mIsOccupied = true;
			return false; // 终止遍历
		}
		
		return true;
	}

	void Visit()
	{
		CMapObject* pMapObj = mpNpc->GetMapObject();
		LK_ASSERT( pMapObj != NULL, return );

		CMapCell* tpCell = pMapObj->GetCell( mPos );
		if( tpCell == NULL )
			return;

		tpCell->Tick( *this );
	}
};



// 内丹搜索阵营
struct MapCellRedStoneSearcher
{
	MapCellRedStoneSearcher( CEntity* pNpc, int nRadius, int nCampID, CEntityPlayer **pIDList, int nSize, int nMetierID ) 
		:  mpNpc( pNpc ), mRadius( nRadius ), mCampID( nCampID ), mPlayerIDList( pIDList ), mArraySize( nSize ), mPlayerCount( 0 )
		,mMetierID( nMetierID )		
	{ }
	~MapCellRedStoneSearcher() {}					   
	
	CEntity* mpNpc;					// 内丹实体	
	int		 mRadius;				// 内丹的作用半径
	int		 mCampID;				// 待搜索的阵营ID
	CEntityPlayer **mPlayerIDList;	// 存储玩家的数组
	int			   mArraySize;	
	int			   mPlayerCount;	// 玩家的数量
	int			   mMetierID;		// 职业ID

	int GetResult( ) { return EM_OBJFUN_NONE; }
	void SetResult( int )  {;} 

	bool operator ()( unsigned int unEntityID )
	{
		CEntity* tpDesEntity = (CEntity*) CSceneObjManager::GetSingletonPtr()->GetObject( unEntityID );
		LK_ASSERT( tpDesEntity != NULL, return true); 

		// 判断是不是玩家不是玩家
		if( !tpDesEntity->IsPlayer() )
		{
			return true;
		}
		
		// 判断国家ID
		if ( ( ( CEntityPlayer * )tpDesEntity )->GetNationality() != mCampID && mCampID != 0 )
		{
			return true;
		}
		
		// 判断门派
		if ( ( ( CEntityPlayer * )tpDesEntity )->GetMetier() != mMetierID && mMetierID != 0 )
		{
			return true;
		}
		
		// 数组大小判断
		if ( mPlayerCount >= mArraySize )
		{
			return false;
		}				
		
		mPlayerIDList[ mPlayerCount ]  = ( CEntityPlayer * )tpDesEntity;
		++mPlayerCount;
		return true;
	}
	
	// 便利寻找内丹附近本阵营的玩家
	void Visit( )
	{
		
		CMapObject* pMapObj = mpNpc->GetMapObject();
		LK_ASSERT( pMapObj != NULL, return );
					
		int nTopLeftX = std::max( mpNpc->GetPosX() - mRadius , 0 );
		int nTopLeftY = std::max( mpNpc->GetPosY() - mRadius, 0 );
		int nBottomRightX = std::min( mpNpc->GetPosX() + mRadius, (int)pMapObj->GetWidth()-1 );
		int nBottomRightY = std::min( mpNpc->GetPosY() + mRadius, (int)pMapObj->GetHight()-1 );

		for ( int x = nTopLeftX; x <= nBottomRightX; x ++ )
		{
			for ( int y = nTopLeftY; y <= nBottomRightY; y ++ )
			{
				CMapCell* tpCell = pMapObj->GetCell( CWTPoint( x, y ) );
				LK_ASSERT( tpCell != NULL, continue );
				tpCell->Tick( *this );
			}
		}				
	}
};


// 内丹搜索国家
struct MapCellCountryRedStoneSearcher
{
	MapCellCountryRedStoneSearcher( CEntity* pNpc, int nRadius, int nCountryID, CEntityPlayer **pIDList, int nSize ) 
		:  mpNpc( pNpc ), mRadius( nRadius ), mCountryID( nCountryID ), mPlayerIDList( pIDList ), mArraySize( nSize ), mPlayerCount( 0 )		
	{ }
	~MapCellCountryRedStoneSearcher() {}					   

	CEntity* mpNpc;					// 内丹实体	
	int		 mRadius;				// 内丹的作用半径
	int		 mCountryID;				// 待搜索的国家ID
	CEntityPlayer **mPlayerIDList;	// 存储玩家的数组
	int			   mArraySize;	
	int			   mPlayerCount;	// 玩家的数量

	int GetResult( ) { return EM_OBJFUN_NONE; }
	void SetResult( int )  {;} 

	bool operator ()( unsigned int unEntityID )
	{
		CEntity* tpDesEntity = (CEntity*) CSceneObjManager::GetSingletonPtr()->GetObject( unEntityID );
		LK_ASSERT( tpDesEntity != NULL, return true); 

		// 判断是不是玩家不是玩家
		if( !tpDesEntity->IsPlayer() )
		{
			return true;
		}

		// 判断国家ID
		if ( ( ( CEntityPlayer * )tpDesEntity )->GetCountryID() != mCountryID )
		{
			return true;
		}

		// 数组大小判断
		if ( mPlayerCount >= mArraySize )
		{
			return false;
		}				

		mPlayerIDList[ mPlayerCount ]  = ( CEntityPlayer * )tpDesEntity;
		++mPlayerCount;
		return true;
	}

	// 便利寻找内丹附近本阵营的玩家
	void Visit( )
	{

		CMapObject* pMapObj = mpNpc->GetMapObject();
		LK_ASSERT( pMapObj != NULL, return );

		int nTopLeftX = std::max( mpNpc->GetPosX() - mRadius , 0 );
		int nTopLeftY = std::max( mpNpc->GetPosY() - mRadius, 0 );
		int nBottomRightX = std::min( mpNpc->GetPosX() + mRadius, (int)pMapObj->GetWidth()-1 );
		int nBottomRightY = std::min( mpNpc->GetPosY() + mRadius, (int)pMapObj->GetHight()-1 );

		for ( int x = nTopLeftX; x <= nBottomRightX; x ++ )
		{
			for ( int y = nTopLeftY; y <= nBottomRightY; y ++ )
			{
				CMapCell* tpCell = pMapObj->GetCell( CWTPoint( x, y ) );
				LK_ASSERT( tpCell != NULL, continue );
				tpCell->Tick( *this );
			}
		}				
	}
};

#endif // __ENTITY_SEACHER_IMP__
