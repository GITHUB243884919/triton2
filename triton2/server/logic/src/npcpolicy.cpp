#include "stdafx.hpp"
#include "npcmodule.h"
#include "protype.h"
#include "proclient_svr.h"
#include "sceneobj_manager.h"
#include "chatmessage_pb.hxx.pb.h"
#include "chattype.h"
#include "epollsocket.h"
#include "property.h"
#include "mapmodule.h"
#include "propertymodule.h"
#include "sceneobj_manager_imp.h"
#include "scenecfg_manager.h"
#include "entity_searcher_imp.h"
#include "taskmodule.h"
#include "entity.h"
#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif

// ********************************************************************** //
// CNpcPolicy
// ********************************************************************** //

CNpcPolicy::CNpcPolicy( )
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

int CNpcPolicy::Initialize()
{
	mNpcID = 0;
	mNpcType = 0;
	mRunSpeed = 0;
	mMoveTimer = 2000;
	mStepTimer = 0;
	mStandTimer = 0;
	mSneerTimer  = 0;
	mTempTimer = 0;
	mAttackTimer = 0;
	mAttackMode = CLOSE;
	mTargetPos = CWTPoint( 0, 0 );
	mPathPos = CWTPoint( 0, 0 );
	mPrevPos = CWTPoint( 0, 0 );
	mPathList.initailize( );
	mPreList.initailize( );
	mIsReturn = false;
	mNpcState = STATE_PEACE;
	mTargetEntityID = 0;
	mSneerID = 0;
	mMaxTempID = 0 ;
	mMinTempID = 0;
	mLastAlive = true;
	mLastState = STATE_PEACE;
	mLastTick = 0;
	//memset( mEnmityList, -1, sizeof(mEnmityList) );
	//memset( mEnmityMap, -1, sizeof(mEnmityMap) );
	//mEnmityCur = 0;
	//mEnmityNum = 0;
	//mpAvailable = 0;
	mpEntity = NULL;
	mEntityID = INVALID_OBJ_ID;
	mPath = false;
	mIsAttacking = false;
	mBattleStart = false;
	mBattleEnd = false;
	mIsDead = false;
	mKill = false;
	mCanUseSkill = 0;
	mSkillList.initailize();
	memset( &mTimer, 0, sizeof(CNpcTimer)*NPCTIMERNUM );
	mTimerNum = 0;
	memset( mDone, 0, sizeof( mDone ) );
	mTeleTimer = 0;
	mSleep = true;	// 初始为休眠状态
	mNoSleep = false;
	mEnmityData.initailize();
	mFollowID = 0;
	mOffsetX = 0;
	mOffsetY = 0;
	memset( mPatrolList, -1, sizeof( mPatrolList ) );
	mPatrolIndex = -1;
	mPatrolDirection = 0;
	return 0;
}

int CNpcPolicy::Resume()
{

	mpEntity = ( CEntityNpc* ) CSceneObjManager::GetSingletonPtr( )->GetObject( mEntityID );

	return 0;
}

CNpcPolicy::~CNpcPolicy( )
{

}

// 添加仇恨
void CNpcPolicy::InsertEnmity( int vEntityID, int vEnmity )
{
	if ( vEntityID == 0 )
		return;
	// 让NPC进入攻击状态
	mNpcState		= STATE_BATTLE;	

	ENMITY::iterator tFindIter = mEnmityData.find( vEntityID );

	if ( tFindIter != mEnmityData.end( ) )
	{
		tFindIter->second += vEnmity;
	}
	else
	{
		mEnmityData.insert( std::pair< int, int >( vEntityID, vEnmity ) );
	}
}

// 得到最恨的目标
int CNpcPolicy::GetEnmityEntityID( )
{
	ENMITY::iterator tBeginIter	= mEnmityData.begin( );
	ENMITY::iterator tEndIter	= mEnmityData.end( );
	int tMaxEnmity = 0;
	int tEntityID = 0;
	for ( ; tBeginIter != tEndIter; ++tBeginIter  )
	{
		CEntity* tTempEntity = CSceneLogic::GetSingleton( ).GetEntity( tBeginIter->first );
		if ( tTempEntity == NULL )
		{
			continue;
		}
		if ( tTempEntity->IsCharacter() == false )
		{
			continue;
		}
		if ( ((CEntityCharacter*)tTempEntity)->CanFly() != mpEntity->CanFly() )
		{
			continue;
		}
		if ( tMaxEnmity < tBeginIter->second )
		{
			tMaxEnmity	= tBeginIter->second;
			tEntityID	= tBeginIter->first;
		}
		// 如果仇恨相等，要判断距离
		else if ( tMaxEnmity == tBeginIter->second )
		{
			CEntity* tEntity1 = CSceneLogic::GetSingleton( ).GetEntity( tEntityID );
			CEntity* tEntity2 = CSceneLogic::GetSingleton( ).GetEntity( tBeginIter->first );
			if( tEntity1 == NULL || tEntity2 == NULL )
			{
				continue;
			}

			if ( mpEntity->mPos.Length( tEntity1->mPos ) > mpEntity->mPos.Length( tEntity2->mPos ) )
			{
				tMaxEnmity	= tBeginIter->second;
				tEntityID	= tBeginIter->first;
			}
		}	
	}
	return tEntityID;
}

// 全部清空仇恨为指定数值
void CNpcPolicy::ClearEnmity( int vEnmity )
{
	if ( vEnmity == 0 )
	{
		mEnmityData.clear( );
		return;
	}
	ENMITY::iterator tBeginIter	= mEnmityData.begin( );
	ENMITY::iterator tEndIter	= mEnmityData.end( );
	for ( ; tBeginIter != tEndIter; ++tBeginIter  )
	{
		tBeginIter->second = vEnmity;
	}
}

// 清空指定实体的仇恨
void CNpcPolicy::ClearEnmity( int vEntityID, int vEnmity )
{
	ENMITY::iterator tFindIter = mEnmityData.find( vEntityID );

	if ( tFindIter != mEnmityData.end( ) )
	{
		if ( vEnmity == 0 )
		{
			mEnmityData.erase( tFindIter );
			return;
		}

		tFindIter->second = vEnmity;
	}
}

// 删除指定实体的一部分仇恨
void CNpcPolicy::RemoveEnmity( int vEntityID, int vEnmity )
{
	ENMITY::iterator tFindIter = mEnmityData.find( vEntityID );

	if ( tFindIter != mEnmityData.end( ) )
	{
		tFindIter->second -= vEnmity;
		if ( tFindIter->second <= 0 )
		{
			mEnmityData.erase( tFindIter );
		}
	}
}

/*
// 添加仇恨
void CNpcPolicy::InsertEnmity( int vEntityID, int vEnmity )
{
if ( vEntityID == 0 )
return; 
// 如果仇恨值为-1, 那么该目标就是怪物攻击目标
if ( vEnmity == -1 )
{
if ( mTargetEntityID == 0 )
{
mTargetEntityID = vEntityID;
}
return;
}

if ( mpAvailable >= NPCENMITY || mpAvailable == -1 )
return;
// 让NPC进入攻击状态
mNpcState		= STATE_BATTLE;	

//ResetEnmityTimer( );
int tIndex = vEntityID % NPCENMITY;
// 如果没有头节点，把头节点置为新增的
if ( mEnmityMap[ tIndex ] == -1 )
{

// 逻辑处理部分//////////////////////////////////
mEnmityList[ mpAvailable ].mEntityID = vEntityID;
mEnmityList[ mpAvailable ].mEnmity = vEnmity;
/////////////////////////////////////////////////

mEnmityList[ mpAvailable ].mpPrev = -1;
mEnmityList[ mpAvailable ].mpNext = -1;
mEnmityMap[ tIndex ] = mpAvailable;			
mpAvailable = mEnmityList[ mpAvailable ].mpNext;
if ( mpAvailable == -1 )
{
mEnmityCur++;
mEnmityNum++;
if ( mEnmityCur >= NPCENMITY )
mpAvailable = -1;
else
mpAvailable = mEnmityCur;			
}
else
{
mEnmityList[ mpAvailable ].mpPrev = -1;
}
}
// 如果已经有头节点，遍历节点，如果找到的话增加，没有的话新增加到最后
else
{
int	tFinder = mEnmityMap[ tIndex ];
while( tFinder != -1 )
{
if ( tFinder == mEnmityList[ tFinder ].mpNext )
{
break;
}
if ( mEnmityList[ tFinder ].mEntityID == 0 )
{
tFinder = mEnmityList[ tFinder ].mpNext;
continue;
}
if ( mEnmityList[ tFinder ].mEntityID == vEntityID )
{
// 逻辑处理部分//////////////////////////////////
mEnmityList[ tFinder ].mEnmity += vEnmity;
/////////////////////////////////////////////////
break;
}			
if ( mEnmityList[ tFinder ].mpNext == -1 )
{
mEnmityList[ mpAvailable ].mEntityID = vEntityID;
mEnmityList[ mpAvailable ].mEnmity = vEnmity;
mEnmityList[ tFinder ].mpNext = mpAvailable;
mEnmityList[ mpAvailable ].mpPrev = tFinder;				
mpAvailable = mEnmityList[ mpAvailable ].mpNext;
if ( mpAvailable == -1 )
{
mEnmityCur++;
mEnmityNum++;
if ( mEnmityCur >= NPCENMITY )
mpAvailable = -1;
else
mpAvailable = mEnmityCur;					
}
else
{
mEnmityList[ mpAvailable ].mpPrev = -1;
}
mEnmityList[ mEnmityList[ tFinder ].mpNext ].mpNext = -1;
break;
}
int tOldFinder = tFinder;
tFinder = mEnmityList[ tFinder ].mpNext;
// 如果死循环，退出
if ( tOldFinder == mEnmityList[ tFinder ].mpNext )
{
break;
}
}
}
}

// 得到最恨的目标
int CNpcPolicy::GetEnmityEntityID( )
{
int tMaxEnmity = 0;
int tEntityID = 0;
for ( int i = 0; i < (int)( ARRAY_CNT( mEnmityList ) ); i ++ )
{
if ( tMaxEnmity < mEnmityList[ i ].mEnmity )
{
tMaxEnmity	= mEnmityList[ i ].mEnmity;
tEntityID	= mEnmityList[ i ].mEntityID;
}
// 如果仇恨相等，要判断距离
else if ( tMaxEnmity == mEnmityList[ i ].mEnmity )
{
CEntity* tEntity1 = CSceneLogic::GetSingleton( ).GetEntity( tEntityID );
CEntity* tEntity2 = CSceneLogic::GetSingleton( ).GetEntity( mEnmityList[ i ].mEntityID );
LK_ASSERT( tEntity1 != NULL && tEntity2 != NULL, return 0 );
if ( mpEntity->mPos.Length( tEntity1->mPos ) > mpEntity->mPos.Length( tEntity2->mPos ) )
{
tMaxEnmity	= mEnmityList[ i ].mEnmity;
tEntityID	= mEnmityList[ i ].mEntityID;
}
}
}

return tEntityID;
}

// 全部清空仇恨
void CNpcPolicy::ClearEnmity( int vEnmity )
{
if ( vEnmity == 0 )
{
mTargetEntityID = 0;
memset( mEnmityMap, -1, sizeof(mEnmityMap) );
mpAvailable = 0;
}
memset( mEnmityList, vEnmity, sizeof( mEnmityList ) );
mEnmityCur = 0;
mEnmityNum = 0;
memset( mDone, 0, sizeof( mDone ) );
}

// 清空指定实体的仇恨
void CNpcPolicy::ClearEnmity( int vEntityID, int vType )
{
if ( mEnmityCur <= 0 )
return;

int tIndex = vEntityID % NPCENMITY;
int	tFinder = mEnmityMap[ tIndex ];
while ( tFinder != -1 )
{
if ( tFinder == mEnmityList[ tFinder ].mpNext )
{
break;
}
if ( mEnmityList[ tFinder ].mEntityID == vEntityID )
{
if ( vType == 0 )		// 彻底清除
{
// 逻辑处理部分//////////////////////////////////
mEnmityList[ tFinder ].mEntityID = 0;
mEnmityList[ tFinder ].mEnmity = 0;
/////////////////////////////////////////////////

// 如果是头节点
if ( mEnmityList[ tFinder ].mpPrev == -1 )
{
mEnmityMap[ tIndex ] = mEnmityList[ tFinder ].mpNext;
}
// 如果是当中的节点
else
{
mEnmityList[ mEnmityList[ tFinder ].mpPrev ].mpNext = mEnmityList[ tFinder ].mpNext;
mEnmityList[ mEnmityList[ tFinder ].mpNext ].mpPrev = mEnmityList[ tFinder ].mpPrev;
}
mEnmityList[ tFinder ].mpPrev = -1;
mEnmityList[ tFinder ].mpNext = -1;
mEnmityNum--;
// 把删除的空间加入可用列表
if ( mpAvailable != -1 )
{
int	tTemp = mpAvailable;
mpAvailable = tFinder;
mEnmityList[ mpAvailable ].mpNext = tTemp;
}
else
{
mpAvailable = tFinder;
}

if ( vEntityID == mTargetEntityID )
mTargetEntityID = 0;
}
// 逻辑处理部分//////////////////////////////////
if ( vType == 1 )		// 减到最少
{
mEnmityList[ tFinder ].mEnmity = 1;
}
if ( vType == 2 )		// 减到一半
{
//mEnmityList[ i ].mEnmity /= 2;
mEnmityList[ tFinder ].mEnmity = mEnmityList[ tFinder ].mEnmity >> 1;
}
//////////////////////////////////////////////////
break;
}
int tOldFinder = tFinder;
tFinder = mEnmityList[ tFinder ].mpNext;
// 如果死循环，退出
if ( tOldFinder == mEnmityList[ tFinder ].mpNext )
{
break;
}
}
}

// 删除指定实体的一部分仇恨
void CNpcPolicy::RemoveEnmity( int vEntityID, int vEnmity )
{
int tIndex = vEntityID % NPCENMITY;
int	tFinder = mEnmityMap[ tIndex ];
while ( tFinder != -1 )
{
if ( tFinder == mEnmityList[ tFinder ].mpNext )
{
break;
}
if ( mEnmityList[ tFinder ].mEntityID == vEntityID )
{
mEnmityList[ tFinder ].mEnmity -= vEnmity;
if ( mEnmityList[ tFinder ].mEnmity < 0 )
{
// 逻辑处理部分//////////////////////////////
mEnmityList[ tFinder ].mEntityID = 0;
mEnmityList[ tFinder ].mEnmity = 0;
/////////////////////////////////////////////

// 如果是头节点
if ( mEnmityList[ tFinder ].mpPrev == -1 )
{
mEnmityMap[ tIndex ] = mEnmityList[ tFinder ].mpNext;
}
// 如果是当中的节点
else
{
mEnmityList[ mEnmityList[ tFinder ].mpPrev ].mpNext = mEnmityList[ tFinder ].mpNext;
mEnmityList[ mEnmityList[ tFinder ].mpNext ].mpPrev = mEnmityList[ tFinder ].mpPrev;
}
mEnmityList[ tFinder ].mpPrev = -1;
mEnmityList[ tFinder ].mpNext = -1;
mEnmityNum--;
// 把删除的空间加入可用列表
if ( mpAvailable != -1 )
{
int	tTemp = mpAvailable;
mpAvailable = tFinder;
mEnmityList[ mpAvailable ].mpNext = tTemp;
}
else
{
mpAvailable = tFinder;
}
if ( vEntityID == mTargetEntityID )
mTargetEntityID = 0;
}
break;
}
tFinder = mEnmityList[ tFinder ].mpNext;
}
}
*/

void CNpcPolicy::Stand( int vTimer )
{
	mStandTimer = vTimer;	
}

// 重置目标
void CNpcPolicy::ResetTarget( )
{
	ClearEnmity( 0 );
	mTargetEntityID = 0;
	CPropertyModule::GetSingleton().ResetFirstEntity( mpEntity->GetEntityID( ) );
}

void CNpcPolicy::Return2Orgin( bool vProtected, bool vReturn )
{
	// 添加一个牛比的状态
	mpEntity->SetSuperState( vProtected );	

	mIsReturn = vReturn;
	MoveTo( GetMovePos( ) );
}

void CNpcPolicy::MoveTo( const CWTPoint& rPoint )
{
	// 如果指定的目标点和上次一样，不用再处理
	if ( mTargetPos == rPoint )
	{
		if( mTargetPos == mpEntity->GetPos( ) )
		{
			return;
		}
		if( mPreList.empty( ) == false )
		{
			return;
		} 
	}

	mTargetPos = rPoint;
	FindPath( mpEntity->GetPos( ), mTargetPos );
	mPreList.push_back( mTargetPos );
	//PathNpc( );
	mPath = true;
}

// 普通攻击
void CNpcPolicy::NormalAttack( CEntityCharacter* pTarget )
{
	CPropertyModule::GetSingleton().OnNpcNormalAttack( mpEntity, pTarget );
}

// 技能攻击
void CNpcPolicy::UseSkill( int vSkillID, CEntity* pTarget )
{
}

bool CNpcPolicy::GetAttackPosition( const CWTPoint& rSrcPos, const CWTPoint& rDesPos, int vRange, int vType )
{
	if ( ( ( rSrcPos.mX >= rDesPos.mX - vRange ) && ( rSrcPos.mX <= rDesPos.mX + vRange ) ) && ( ( rSrcPos.mY >= rDesPos.mY - vRange ) && ( rSrcPos.mY <= rDesPos.mY + vRange ) ) )
		return true;
	return false;
}

void CNpcPolicy::FindPath( const CWTPoint& rSrcPos, const CWTPoint& rDesPos )
{
	//mPathList.clear( );
	mPreList.clear( );
	if ( rSrcPos.mX == rDesPos.mX )
	{
		mPreList.push_back( rDesPos );
		return;
	}

	if ( rSrcPos.mY == rDesPos.mY )
	{
		mPreList.push_back( rDesPos );
		return;
	}

	if ( rDesPos.mX > rSrcPos.mX && rDesPos.mY < rSrcPos.mY )
	{
		CWTPoint tMidPoint = rSrcPos;
		while( 1 )
		{
			tMidPoint.mX ++;
			tMidPoint.mY --;
			if ( tMidPoint.mX == rDesPos.mX )
			{
				mPreList.push_back( tMidPoint );
				mPreList.push_back( rDesPos );
				return;
			}

			if ( tMidPoint.mY == rDesPos.mY )
			{
				mPreList.push_back( tMidPoint );
				mPreList.push_back( rDesPos );
				return;
			}
		}
	}

	if ( rDesPos.mX > rSrcPos.mX && rDesPos.mY > rSrcPos.mY )
	{
		CWTPoint tMidPoint = rSrcPos;
		while( 1 )
		{
			tMidPoint.mX ++;
			tMidPoint.mY ++;
			if ( tMidPoint.mX == rDesPos.mX )
			{
				mPreList.push_back( tMidPoint );
				mPreList.push_back( rDesPos );
				return;
			}

			if ( tMidPoint.mY == rDesPos.mY )
			{
				mPreList.push_back( tMidPoint );
				mPreList.push_back( rDesPos );
				return;
			}
		}
	}

	if ( rDesPos.mX < rSrcPos.mX && rDesPos.mY < rSrcPos.mY )
	{
		CWTPoint tMidPoint = rSrcPos;
		while( 1 )
		{
			tMidPoint.mX --;
			tMidPoint.mY --;
			if ( tMidPoint.mX == rDesPos.mX )
			{
				mPreList.push_back( tMidPoint );
				mPreList.push_back( rDesPos );
				return;
			}

			if ( tMidPoint.mY == rDesPos.mY )
			{
				mPreList.push_back( tMidPoint );
				mPreList.push_back( rDesPos );
				return;
			}
		}
	}

	if ( rDesPos.mX < rSrcPos.mX && rDesPos.mY > rSrcPos.mY )
	{
		CWTPoint tMidPoint = rSrcPos;
		while( 1 )
		{
			tMidPoint.mX --;
			tMidPoint.mY ++;
			if ( tMidPoint.mX == rDesPos.mX )
			{
				mPreList.push_back( tMidPoint );
				mPreList.push_back( rDesPos );
				return;
			}

			if ( tMidPoint.mY == rDesPos.mY )
			{
				mPreList.push_back( tMidPoint );
				mPreList.push_back( rDesPos );
				return;
			}
		}
	}	
}

CWTPoint CNpcPolicy::GetRandPos( )
{
	CWTPoint tPos;
	while( 1 )
	{
		tPos.mX = mpEntity->GetPosX( ) + rand( ) % 17 - 8;
		tPos.mY = mpEntity->GetPosY( ) + rand( ) % 17 - 8;

		if ( tPos == mpEntity->GetPos( ) )
			continue;

		bool tResult = CMapModule::GetSingleton().IsCellValid( mpEntity, tPos.mX, tPos.mY );

		if ( tResult == true )
		{
			break;
		}
		else
		{
			LOG_WARN( "map", "[%s:%d] pos(%d,%d) out of map %d",
				__FUNCTION__, __LINE__, tPos.mX, tPos.mY, mpEntity->GetMapID() );
		}
	}

	return tPos;
}

void CNpcPolicy::GetTarget( int vCode, std::vector< int > &vTargetList )
{
	int tMaxEnmity = 0;
	int tMaxID = 0;
	int tSecondEnmity = 0;
	int tSecondID = 0;
	int tMinEnmity = 1000000;
	int tMinID = 0;
	unsigned int tMaxHP = 0;
	int tMaxHPID = 0;
	unsigned int tMaxMP = 0;
	int tMaxMPID = 0;
	unsigned int tMinHP = 1000000;
	int tMinHPID = 0;

	ENMITY::iterator tBeginIter	= mEnmityData.begin( );
	ENMITY::iterator tEndIter	= mEnmityData.end( );
	for ( ; tBeginIter != tEndIter; tBeginIter ++ )
	{
		CPropertyNPC *tProperty = ( CPropertyNPC* )CProperty::SearchProByEntityID( tBeginIter->first );
		if ( tProperty == NULL )
			continue;
		if ( tMaxEnmity < tBeginIter->second )
		{
			tSecondEnmity = tMaxEnmity;
			tSecondID = tMaxID;
			tMaxEnmity	= tBeginIter->second;
			tMaxID	= tBeginIter->first;
		}
		else
		{
			if ( tSecondEnmity < tBeginIter->second )
			{
				tSecondEnmity = tBeginIter->second;
				tSecondID = tBeginIter->first;
			}
		}
		if ( tMinEnmity > tBeginIter->second )
		{
			tMinEnmity = tBeginIter->second;
			tMinID	= tBeginIter->first;
		}

		if ( tMaxHP < tProperty->GetCurHP( ) )
		{
			tMaxHP = tProperty->GetCurHP( );
			tMaxHPID = tBeginIter->first;
		}
		if ( tMaxMP < tProperty->GetCurMP( ) )
		{
			tMaxMP = tProperty->GetCurMP( );
			tMaxMPID = tBeginIter->first;
		}
		if ( tMinHP > tProperty->GetCurHP( ) )
		{
			tMinHP = tProperty->GetCurHP( );
			tMinHPID = tBeginIter->first;
		}
		if ( ( vCode & PROF1 ) == PROF1 && tProperty->GetPropertyTempID( ) == METIER_WARRIOR )
			vTargetList.push_back( tBeginIter->first );
		if ( ( vCode & PROF2 ) == PROF2 && tProperty->GetPropertyTempID( ) == METIER_ROGUE )
			vTargetList.push_back( tBeginIter->first );
		if ( ( vCode & PROF3 ) == PROF3 && tProperty->GetPropertyTempID( ) == METIER_HUNTER )
			vTargetList.push_back( tBeginIter->first );
		if ( ( vCode & PROF4 ) == PROF4 && tProperty->GetPropertyTempID( ) == METIER_SORCERER )
			vTargetList.push_back( tBeginIter->first );
		if ( ( vCode & PROF5 ) == PROF5 && tProperty->GetPropertyTempID( ) == METIER_PRIEST )
			vTargetList.push_back( tBeginIter->first );
		if ( ( vCode & PROF6 ) == PROF6 && tProperty->GetPropertyTempID( ) == METIER_DRUID )
			vTargetList.push_back( tBeginIter->first );
		if ( ( vCode & PROF7 ) == PROF7 && tProperty->GetPropertyTempID( ) == METIER_NOVICE )
			vTargetList.push_back( tBeginIter->first );
	}

	if ( ( vCode & FIRSTENMITY ) == FIRSTENMITY )
		vTargetList.push_back( tMaxID );
	if ( ( vCode & SECONDENMITY ) == SECONDENMITY )
		vTargetList.push_back( tSecondID );
	if ( ( vCode & RANDOMENMITY ) == RANDOMENMITY )
	{
		/*int tTempID = 0;
		do
		{
		tTempID = mEnmityList[ RAND(511) ].mEntityID;
		}while ( tTempID == tMaxID );
		vTargetList.push_back( tTempID );*/
	}
	if ( ( vCode & MAXHP ) == MAXHP )
		vTargetList.push_back( tMaxHPID );
	if ( ( vCode & MAXMP ) == MAXMP )
		vTargetList.push_back( tMaxMPID );
	if ( ( vCode & MINHP ) == MINHP )
		vTargetList.push_back( tMinHPID );
	if ( ( vCode & SELF ) == SELF )
		vTargetList.push_back( mpEntity->GetEntityID( ) );

}

void CNpcPolicy::OnTactics( int vTacticsID )
{
	if ( mpEntity->GetEntityType() == ENTITYTYPE_FUNCNPC && mNpcState == STATE_PEACE )
		return;
	// 遍历触发器
	if ( vTacticsID <= 0 || vTacticsID >= (int)sizeof(CNpcModule::mTactics) )
		return;
	CNpcTactics *tTac = &( CNpcModule::mTactics[ vTacticsID ] );

	/*CDispParams tParams;
	CVariant tResult;*/
	int tHP = 0;
	int tMP = 0;
	int tAP = 0;
	int tLevel = 0;

	CPropertyModule::GetSingleton().GetPropertyPercent( ( CEntityCharacter* )mpEntity, tHP, tMP, tAP, tLevel );

	for ( unsigned int i = 0;i < tTac->mTrigger.size();i++ )
	{
		if (  ( mNpcState != STATE_BATTLE ) && ( tTac->mTrigger[ i ].mInbattle == true ) )
			continue;
		if ( tTac->mTrigger[ i ].mActive == false )
			continue;
		// 判断是否可以进行操作
		bool act = false;
		for ( unsigned int j = 0;j < tTac->mTrigger[ i ].mCondition.size() 
			&& i < ARRAY_CNT( mDone )
			&& j < ARRAY_CNT( mDone[ 0 ] )
			;j++ )
		{
			if ( tTac->mTrigger[ i ].mCondition[ j ].mTypeID == 0 )
				continue;
			if ( ( tTac->mTrigger[ i ].mCondition[ j ].mTypeID & ONTIMER ) == ONTIMER )	// 如果是定时器到时
			{
				int tID = tTac->mTrigger[ i ].mCondition[ j ].mParam1;
				if (tID >= ARRAY_CNT(mTimer))
				{
					continue;
				}

				if ( mTimer[ tID ].mEnabled == false )
				{
					continue;
				}
			}
			if ( ( tTac->mTrigger[ i ].mCondition[ j ].mTypeID & KILLPLAYER ) == KILLPLAYER )	// 如果是杀死玩家
			{
				if ( mKill == false )
				{
					continue;
				}
			}
			if ( ( tTac->mTrigger[ i ].mCondition[ j ].mTypeID & HPLESSTHAN ) == HPLESSTHAN ) // 如果是HP<%
			{
				if ( ( tHP >= tTac->mTrigger[ i ].mCondition[ j ].mParam1 ) || ( mDone[ i ][ j ] == true ) )
				{
					continue;
				}
			}
			if ( ( tTac->mTrigger[ i ].mCondition[ j ].mTypeID & HPMORETHAN ) == HPMORETHAN ) // 如果是HP>%
			{
				if ( ( tHP <= tTac->mTrigger[ i ].mCondition[ j ].mParam1 ) || ( mDone[ i ][ j ] == true ) )
				{
					continue;
				}
			}
			if ( ( tTac->mTrigger[ i ].mCondition[ j ].mTypeID & BATTLESTART ) == BATTLESTART )	// 如果是战斗开始
			{
				if ( ( mBattleStart == false ) || ( mDone[ i ][ j ] == true ) )
				{
					continue;
				}
			}
			if ( ( tTac->mTrigger[ i ].mCondition[ j ].mTypeID & BATTLEEND ) == BATTLEEND )	// 如果是战斗结束
			{
				if ( ( mBattleEnd == false ) || ( mDone[ i ][ j ] == true ) )
				{
					continue;
				}
			}
			if ( ( tTac->mTrigger[ i ].mCondition[ j ].mTypeID & RANDOMT ) == RANDOMT )			// 如果是随机判断
			{
				int tTemp = RAND(100);
				if ( ( tTemp >= tTac->mTrigger[ i ].mCondition[ j ].mParam1 ) || ( mDone[ i ][ j ] == true ) )
				{
					continue;
				}
			}			
			if ( ( tTac->mTrigger[ i ].mCondition[ j ].mTypeID & KILLOGRE ) == KILLOGRE )	// 如果是怪物死亡
			{
				if ( ( mIsDead == false ) || ( mDone[ i ][ j ] == true ) )
				{
					continue;
				}
			}
			act = true;
			mDone[ i ][ j ] = true;
			break;
		}
		if ( act == false )
			continue;
		for ( unsigned int k = 0;k < tTac->mTrigger[ i ].mAction.size();k++ )
		{
			std::vector< int > tTargetList;
			GetTarget( tTac->mTrigger[ i ].mAction[ k ].mTarget, tTargetList );

			switch( tTac->mTrigger[ i ].mAction[ k ].mTypeID )
			{
			case ATTACK:
				{
					mAttackMode = tTac->mTrigger[ i ].mAction[ k ].mParam1;
					break;
				}
			case USESKILL :
				{

					mCanUseSkill++;
					CNpcSkill tSkill = CNpcSkill( 0, 0, 0 );
					tSkill.mSkillID = tTac->mTrigger[ i ].mAction[ k ].mParam1;
					tSkill.mSkillLevel = tTac->mTrigger[ i ].mAction[ k ].mParam2;
					if ( tTargetList.empty() == true )
						tSkill.mSkillTarID = 0;
					else
						tSkill.mSkillTarID = tTargetList[ 0 ];

					if( mSkillList.size() == mSkillList.max_size() )
					{
						LOG_ERROR( "npc", "npc %d templgateid %d use skill %d, but skill list is full",
							mpEntity->GetEntityID(), mNpcID, tSkill.mSkillID );
					}
					else
					{
						mSkillList.push_back( tSkill );
					}

					break;
				}
			case SPEAK:
				{
					// 城门特殊处理。。
					CTemplateOgre* tplNpc = (CTemplateNpc*)CDataStatic::SearchTpl( mNpcID );
					if ( tplNpc != NULL && tplNpc->mOgreEvent == CTemplateOgre::EVENT_GATE )
					{
						int tIntParam[ 3 ] = { 0 };
						tIntParam[ 0 ] = mpEntity->GetNpcID( );
						tIntParam[ 1 ] = 0;
						tIntParam[ 2 ] = mpEntity->GetMapID( );
						CPropertyModule::GetSingleton().SendEventNotify( NOTICE_ACCUSE_GATEATTACKED, mpEntity->GetLineID( ), tIntParam, ARRAY_CNT(tIntParam), NULL, NULL );
						break;
					}
					CMessage tMessage;
					CMessageChatSendCallBack tChatSendMsg;
					tChatSendMsg.set_channel( CHANNEL_COMMON );
					tChatSendMsg.set_msg( tTac->mTrigger[ i ].mAction[ k ].mSpeak );					
					tChatSendMsg.mutable_router()->set_srcname( tplNpc == NULL ? "" : tplNpc->mName );
					tChatSendMsg.mutable_router()->set_srccharid( mpEntity->GetEntityID( ) );
					tChatSendMsg.set_npcshoutflag(1);

					tMessage.mutable_msghead()->set_messageid( ID_S2C_CHATSENDCALLBACK );
					tMessage.set_msgpara( (uint32)&tChatSendMsg );

					CMapModule::GetSingletonPtr()->MessageBroadCast( mpEntity, &tMessage );

					break;
				}
			case RESETENMITY:
				{
					ClearEnmity( 1 );
					break;
				}
			case RUNTRIGGER:
				{
					int tTriggerID = tTac->mTrigger[ i ].mAction[ k ].mParam1;
					if ( tTriggerID >= (int)tTac->mTrigger.size() )
					{
						continue;
					}
					tTac->mTrigger[ tTriggerID ].mActive = true;
					break;
				}
			case STOPTRIGGER:
				{
					int tTriggerID = tTac->mTrigger[ i ].mAction[ k ].mParam1;
					if ( tTriggerID >= (int)tTac->mTrigger.size() )
					{
						continue;
					}
					tTac->mTrigger[ tTriggerID ].mActive = false;
					break;
				}
			case ACTIVETRIGGER:
				{
					int tTriggerID = tTac->mTrigger[ i ].mAction[ k ].mParam1;
					if ( tTriggerID >= (int)tTac->mTrigger.size() )
					{
						continue;
					}
					tTac->mTrigger[ tTriggerID ].mActive = true;
					break;
				}
			case CREATETIMER:
				{
					if ( mTimerNum >= NPCTIMERNUM )
						continue;
					int tID = tTac->mTrigger[ i ].mAction[ k ].mParam1;	
					if ( tID >= NPCTIMERNUM )
					{
						continue;
					}
					mTimer[ tID ].mID = tID;
					mTimer[ tID ].mInterval = tTac->mTrigger[ i ].mAction[ k ].mParam2;
					mTimer[ tID ].mNum = tTac->mTrigger[ i ].mAction[ k ].mParam3;
					mTimer[ tID ].mTimer = mTimer[ tID ].mInterval * 1000;
					mTimer[ tID ].mEnabled = false;
					mTimerNum++;
					break;
				}
			case STOPTIMER:
				{
					int tID = tTac->mTrigger[ i ].mAction[ k ].mParam1;
					if ( tID >= NPCTIMERNUM )
					{
						continue;
					}
					mTimer[ tID ].mID = 0;
					mTimer[ tID ].mInterval = 0;
					mTimer[ tID ].mNum = 0;
					mTimer[ tID ].mTimer = 0;
					mTimer[ tID ].mEnabled = false;
					mTimerNum--;

					break;
				}
			case FLEE:
				{
					break;
				}
			case TARFIRSTENMITY:
				{
					mMaxTempID = tTargetList[ 0 ];
					break;
				}
			case TARLASTENMITY:
				{
					mMinTempID = tTargetList[ 0 ];
					break;
				}
			case REDUCEHALFENMITY:
				{
					for ( unsigned int n = 0; n < tTargetList.size(); n++ )
					{
						ClearEnmity( tTargetList[ n ], 2 );
					}
					break;
				}
			case BREAK:
				{
					break;
				}
			default:
				{
					break;
				}
			}

		}
	}
	mBattleStart = false;
	mBattleEnd = false;
	mIsDead = false;
	mKill = false;
}

CWTPoint CNpcPolicy::GetMovePos( )
{
	CWTPoint tMovePos;
	if ( mFollowID != 0 )
	{
		CEntityCharacter* tpTarget = ( CEntityCharacter* )CSceneLogic::GetSingleton( ).GetEntity( mFollowID );
		if ( tpTarget != NULL )
		{
			tMovePos = tpTarget->GetPos( );
			tMovePos.mX += mOffsetX;
			tMovePos.mY += mOffsetY;
		}
	}
	else if ( mPatrolIndex >= 0 )
	{
		tMovePos = mPatrolList[ mPatrolIndex ];
	}
	else
	{
		tMovePos = mpEntity->GetOrginPos( );
	}
	return tMovePos;
}

void CNpcPolicy::StepNpc( )
{
	if ( mpEntity->GetMoveable() == false ) 
	{
		return;
	}
	//LOG_DEBUG("TASK", "StepNpc---%d,%d", mPathPos.mX, mPathPos.mY);
	CMapModule::GetSingleton().MoveEntity( mpEntity, mPathPos.mX, mPathPos.mY, false );		
}

void CNpcPolicy::PathNpc( )
{
	if ( mpEntity->GetMoveable() == false )
	{
		return;
	}

	if ( mPreList.empty( ) == true )
		return;

	mPathList.clear( );
	for ( unsigned int i = 0; i < mPreList.size( ); i++ )
	{
		mPathList.push_back( mPreList[ i ] );
	}

	// 如果NPC已经到达第一个路径点, 那么清除第一个路径点, 处理第二个路径点
	if ( mPathList[ 0 ] == mpEntity->GetPos( ) )
	{
		mPathList.erase( mPathList.begin( ) );
		mPreList.erase( mPreList.begin( ) );
	}

	if ( mPathList.empty() == true )
		return;

	// 如果在战斗状态下能打到目标就不移动了
	if ( mNpcState == CNpcPolicy::STATE_BATTLE )
	{
		CEntity* tpTarget = CSceneLogic::GetSingleton( ).GetEntity( mTargetEntityID );
		if ( tpTarget != NULL )
		{

			int tRange = 1;
			if ( mpEntity->IsOgre( ) )
			{
				CTemplateOgre *tpOgre = (CTemplateOgre*)CDataStatic::SearchTpl( mNpcID );
				if ( tpOgre != NULL )
				{
					tRange = ( tpOgre->mOgreType == ATTACKTYPE_PHYSICS ) ? tpOgre->mPAttackRange : tpOgre->mMAttackRange;
				}
			}			
			CWTPoint tDesPos = tpTarget->GetPos( );
			if ( GetAttackPosition( mpEntity->GetPos( ), tDesPos, tRange, 0 ) == true )
			{
				mTargetPos = mpEntity->GetPos( );
				return;
			}
		}
	}

	CWTPoint tTempStep[ 8 ] = {
		CWTPoint( -1, -1 ),CWTPoint( -1, 0 ),CWTPoint( -1, 1 ),CWTPoint( 0, 1 ),
		CWTPoint( 1, 1 ),CWTPoint( 1, 0 ),CWTPoint( 1, -1 ),CWTPoint( 0, -1 ) };
		unsigned short tIndex = 0;

		CWTPoint tNextStep = mpEntity->GetPos( );

		if ( mPathList[ 0 ].mX < mpEntity->GetPosX( ) && mPathList[ 0 ].mY < mpEntity->GetPosY( ) )
			tIndex = 0;
		if ( mPathList[ 0 ].mX < mpEntity->GetPosX( ) && mPathList[ 0 ].mY == mpEntity->GetPosY( ) )
			tIndex = 1;
		if ( mPathList[ 0 ].mX < mpEntity->GetPosX( ) && mPathList[ 0 ].mY > mpEntity->GetPosY( ) )
			tIndex = 2;
		if ( mPathList[ 0 ].mX == mpEntity->GetPosX( ) && mPathList[ 0 ].mY > mpEntity->GetPosY( ) )
			tIndex = 3;
		if ( mPathList[ 0 ].mX > mpEntity->GetPosX( ) && mPathList[ 0 ].mY > mpEntity->GetPosY( ) )
			tIndex = 4;
		if ( mPathList[ 0 ].mX > mpEntity->GetPosX( ) && mPathList[ 0 ].mY == mpEntity->GetPosY( ) )
			tIndex = 5;
		if ( mPathList[ 0 ].mX > mpEntity->GetPosX( ) && mPathList[ 0 ].mY < mpEntity->GetPosY( ) )
			tIndex = 6;
		if ( mPathList[ 0 ].mX == mpEntity->GetPosX( ) && mPathList[ 0 ].mY < mpEntity->GetPosY( ) )
			tIndex = 7;
		bool tCanmove = false;

		// 遍历周围的8个点
		int i;
		for ( i = 0; i < 8; i++ )
		{
			unsigned short tIndex1 = 0;		

			switch ( i )
			{
			case 0:
				tIndex1 = tIndex;
				break;
			case 1:
				tIndex1 = tIndex - 1 >= 0 ? tIndex - 1 : tIndex + 7;
				break;
			case 2:
				tIndex1 = tIndex + 1 > 7 ? tIndex - 7 : tIndex + 1; 
				break;
			case 3:
				tIndex1 = tIndex - 2 >= 0 ? tIndex - 2 : tIndex + 6;
				break;
			case 4:
				tIndex1 = tIndex + 2 > 7 ? tIndex - 6 : tIndex + 2;
				break;
			case 5:
				tIndex1 = tIndex - 3 >= 0 ? tIndex - 3 : tIndex + 5;
				break;
			case 6:
				tIndex1 = tIndex + 3 > 7 ? tIndex - 5 : tIndex + 3;
				break;
			case 7:
				tIndex1 = tIndex - 4 >= 0 ? tIndex - 4 : tIndex + 4;
				break;
			default:
				break;
			}

			tNextStep = mpEntity->GetPos( ) + tTempStep[ tIndex1 ];

			if ( i == 1 )
			{
				CWTPoint tPoint = mpEntity->GetPos( ) + tTempStep[ tIndex + 1 > 7 ? tIndex - 7 : tIndex + 1 ];
				if ( tNextStep.Length( mPathList[ 0 ] ) > tPoint.Length( mPathList[ 0 ] ) )
					continue;
			}

			if ( i == 3 )
			{
				CWTPoint tPoint = mpEntity->GetPos( ) + tTempStep[ tIndex + 2 > 7 ? tIndex - 6 : tIndex + 2 ];
				if ( tNextStep.Length( mPathList[ 0 ] ) > tPoint.Length( mPathList[ 0 ] ) )
					continue;
			}

			if ( i == 5 )
			{
				CWTPoint tPoint = mpEntity->GetPos( ) + tTempStep[ tIndex + 3 > 7 ? tIndex - 5 : tIndex + 3 ];
				if ( tNextStep.Length( mPathList[ 0 ] ) > tPoint.Length( mPathList[ 0 ] ) )
					continue;
			}

			// 如果选到刚走过的点，暂不考虑
			if ( tNextStep == mPrevPos )
				continue;

			// 搜索该坐标点，npc是否能进入(里面已经判断了任务npc)
			MapCellNpcOccupiedSearcher tNpcSearcher( tNextStep, mpEntity );
			tNpcSearcher.Visit();

			bool vIsOccupied  = tNpcSearcher.mIsOccupied;

			bool vCanPass = CMapModule::GetSingleton().CanNpcPass( mpEntity, tNextStep.mX, tNextStep.mY );

			if ( vIsOccupied == false && vCanPass == true )
			{
				tCanmove = true;
				break;
			}
		}	
		if ( tCanmove == false )
		{
			if ( mNpcState == CNpcPolicy::STATE_PEACE )
				Stand( 5000 );
			return;
		}
		// 如果不是预期的点或者已经走到预定点，要重新寻路
		if ( ( i > 0  && mNpcState == CNpcPolicy::STATE_BATTLE ) || ( tNextStep == mPathList[ 0 ] ) )
		{
			CEntity* tpTarget = CSceneLogic::GetSingleton( ).GetEntity( mTargetEntityID );
			if ( tpTarget != NULL )
			{
				FindPath( tNextStep, tpTarget->GetPos( ) );
			}
		}

		mPathPos = tNextStep;	
		mPrevPos = mpEntity->GetPos( );	
		//if (mpEntity->GetNpcChangeState() != 0)
		//	printf("PATH NPC 到 %d,%d\n", tNextStep.mX, tNextStep.mY );
		CMapModule::GetSingleton().PathEntity( mpEntity, mpEntity->GetPosX( ), mpEntity->GetPosY( ), tNextStep.mX, tNextStep.mY, 0 );
}

void CNpcPolicy::OnPet( unsigned int vTickCount )
{
	if ( mpEntity->IsAlive() == false )
		return;

	CEntityPet *pPet = (CEntityPet*)mpEntity;
	CEntity* pOwner =  CSceneLogic::GetSingleton( ).GetEntity( pPet->GetOwnerID( ) );	

	//unsigned int tTickOffset = ( mLastTick == 0 ) ? 0 : vTickCount - mLastTick ;
	mLastTick = vTickCount;	

	if ( pOwner == NULL )
	{
		//LOG_ERROR( "npc", "[ %s : %d ][ %s ] Pet %d Cannt Find Owner %d", __LK_FILE__, __LINE__, __FUNCTION__,
		//	pPet->GetEntityID( ), pPet->GetOwnerID( ) );
		return;
	}	

	//// 主动攻击模式，并且无目标
	//if ( pPet->GetCurMode( ) == EM_PET_MODE_ASSIST && pPet->GetCurTargetID() == 0 )	
	//{
	//	// 召唤兽搜索敌人
	//	MapCellSearchEnemy tPetEnemySearcher( pPet );
	//	tPetEnemySearcher.Visit();
	//	
	//	if( CPropertyModule::GetSingleton().EntityPKValid( pPet,tPetEnemySearcher.mEnemys,NULL,false ) )
	//	{
	//		// 能执行到这里 tPetEnemySearcher.mEnemys 一定不是NULL
	//		mNpcState = CNpcPolicy::STATE_BATTLE;
	//		pPet->SetCurTargetID( tPetEnemySearcher.mEnemys->GetEntityID() );
	//		mTargetEntityID = pPet->GetCurTargetID( ); 
	//		mBattleStart = true;
	//	}
	//}

	//CEntity* tpTarget = CSceneLogic::GetSingleton( ).GetEntity( pPet->GetCurTargetID() );	
	//if ( tpTarget != NULL )
	//{
	//	if ( tpTarget->IsAlive( ) == false )
	//	{
	//		pPet->SetCurTargetID( 0 );
	//		tpTarget = NULL;
	//		mNpcState = CNpcPolicy::STATE_PEACE;
	//	}
	//}
	//else
	//{
	//	pPet->SetCurTargetID( 0 );
	//}
	//
	//if ( mAttackTimer > 0 )
	//{
	//	mAttackTimer -= tTickOffset;
	//	if ( mAttackTimer < 0 )
	//		mAttackTimer = 0;
	//}	
	//

	if ( pOwner != NULL )
	{
		// 如果超过召唤者一定距离，直接传送 
		if( mpEntity->IsWithinDistance( pOwner, VIEW_RADIUS_Y ) == false )
		{
#ifndef _FB_
			CMapModule::GetSingleton().Teleport( pOwner->GetLineID( ), pOwner->GetMapID( ), pOwner->GetPosX( ), pOwner->GetPosY( ), mpEntity );
#else
			CMapModule::GetSingleton( ).Teleport( pOwner->GetLineID( ), pOwner->GetMapID( ), pOwner->GetPosX( ), 
				pOwner->GetPosY( ), mpEntity, pOwner->GetMapIndex( ) ); 
#endif

			// 传送清空目标
			pPet->SetCurTargetID( 0 );
			//tpTarget = NULL;
			mNpcState = CNpcPolicy::STATE_PEACE;

			//PathNpc( );

			return;
		}
	}	

}

void CNpcPolicy::DoAttack( )
{
	CEntity* tpTarget = CSceneLogic::GetSingleton( ).GetEntity( mTargetEntityID );
	if ( tpTarget != NULL )
	{
		CTemplateOgre* tplNpc = (CTemplateOgre*)CDataStatic::SearchTpl( mNpcID );
		LK_ASSERT( tplNpc != NULL, return );

		CWTPoint tDesPos = tpTarget->GetPos( );		
		if( mAttackTimer > 0 )
		{
			MoveTo( tDesPos );
			return;
		}
		//tDesPos = mpEntity->GetPos( );
		MoveTo( tDesPos );
		if (  mCanUseSkill > 0 && mSkillList.empty( ) == false )
		{
			CEntity* tSkillTarget;
			if ( mSkillList[ 0 ].mSkillTarID > 0 )
				tSkillTarget = CSceneLogic::GetSingleton( ).GetEntity( mSkillList[ 0 ].mSkillTarID );
			else
				tSkillTarget = mpEntity;

			CPropertyModule::GetSingleton().OnNpcUseSkill( mpEntity, ( CEntityCharacter* )tSkillTarget, mSkillList[ 0 ].mSkillID, mSkillList[ 0 ].mSkillLevel );
			mCanUseSkill--;
			mSkillList.erase( mSkillList.begin( ) );
			Stand(tplNpc->mAttackTime );
			mAttackTimer =tplNpc->mAttackRate * 1000;
			mIsAttacking = true;		
			return;
		}

		int	tRange = ( tplNpc->mOgreType == ATTACKTYPE_PHYSICS ) ? tplNpc->mPAttackRange : tplNpc->mMAttackRange;
		if ( GetAttackPosition( mpEntity->GetPos( ), tDesPos, tRange, 0 ) == true )
		{
			if ( mAttackTimer <= 0 )
			{
				if( tpTarget->IsCharacter() == false )
				{
					return;
				}
				NormalAttack( ( CEntityCharacter* )tpTarget );
				Stand( tplNpc->mAttackTime );
				mAttackTimer = tplNpc->mAttackRate * 1000;
				mIsAttacking = true;
				return;
			}
		}	
		else
		{
			if ( mAttackTimer <= 0 )
				mAttackTimer = 1;
		}
	}
}

void CNpcPolicy::OnTask( unsigned int vTickCount )
{
	unsigned int tTickOffset = ( mLastTick == 0 ) ? 0 : vTickCount - mLastTick;
	mLastTick = vTickCount;
	mStepTimer -= tTickOffset;
	if ( mStepTimer <= 0 )
	{
		//LOG_DEBUG("TASK", "mStepTimer=%d  sleep=%d state=%d", mStepTimer, mSleep, mNpcState );
		mStepTimer	= mRunSpeed + mStepTimer;
		StepNpc( );
		PathNpc( );
	}
	// 如果是任务护送NPC 仇恨列表里的实体都死了 则继续往前走
	if ( mNpcState == CNpcPolicy::STATE_BATTLE )
	{
		if ( mEnmityData.empty() ) // 继续走
		{
			mNpcState = CNpcPolicy::STATE_PEACE;
			CWTPoint pt;
			if ( CTaskModule::GetSingleton().GetSendNpcCurTarget( mpEntity->GetEntityID(), pt ) )
			{
				LOG_DEBUG("TASK", "send NPC goto:%d %d", pt.mX, pt.mY );
				MoveTo( pt );
			}
		}
		else // 单独处理 效率会比GetEnmityEntityID略高
		{
			ENMITY::iterator tBeginIter	= mEnmityData.begin( );
			ENMITY::iterator tEndIter	= mEnmityData.end( );
			std::vector<int> _todel;
			for ( ; tBeginIter != tEndIter; ++tBeginIter  )
			{
				CEntityCharacter* tpEnemy = ( CEntityCharacter* )CSceneLogic::GetSingleton( ).GetEntity( tBeginIter->first );
				if ( tpEnemy == NULL || tpEnemy->IsAlive( ) == false || 
					tpEnemy->GetLineID() != mpEntity->GetLineID() ||
					tpEnemy->GetMapID() != mpEntity->GetMapID() )
				{
					_todel.push_back(tBeginIter->first);
				}
			}

			for ( std::vector<int>::iterator it = _todel.begin(); it != _todel.end(); ++it )
			{
				ClearEnmity( *it, 0 );
			}
		}
	}
}

void CNpcPolicy::OnPeace( unsigned int vTickCount )
{	
	unsigned int tTickOffset = (mLastTick == 0) ? 0 : vTickCount - mLastTick;
	mLastTick = vTickCount;

	if ( mpEntity->IsAlive( ) == false )
	{
		return;
	}

	CTemplateOgre* tplNpc = (CTemplateOgre*)CDataStatic::SearchTpl( mNpcID );
	LK_ASSERT( tplNpc != NULL, return  );	

	if ( tplNpc->mIsInitiative == 1 && GetEnmityEntityID( ) == 0 )
		SearchEnemy( );

	// 如果是npc变成怪物 被玩家打死后重生 而且已经跑回了出生点
	if (mpEntity->GetNpcChangeState()!= 0 && mpEntity->GetOwnercharID() == 0 && mpEntity->GetTaskID() == 0
		&& mpEntity->GetPos() == mpEntity->GetOrginPos() )
	{
		mpEntity->SetSuperState( false );	// 关闭牛逼状态
		mIsReturn = false;// 关闭返回状态
		CPropertyModule::GetSingleton().ResetFuncNpc( mpEntity );
	}

	// 如果NPC处于返回状态，并且到达了出生点，开启罚站计时器，关闭返回状态
	if ( ( mIsReturn == true ) && ( mpEntity->GetPos( ) == mTargetPos ) )
	{
		Stand( 2000 );
		CMapModule::GetSingleton().PathEntity( mpEntity, mpEntity->GetPosX( ), mpEntity->GetPosY( ), -1, -1, 0 );
		OnBackLogic( );
	}

	if ( mTeleTimer > 0 )
	{
		mTeleTimer -= tTickOffset;
		// 如果时间到还没走回,传送
		if ( mTeleTimer <= 0 )
		{
			OnBackLogic( );
			CWTPoint tMovePos = GetMovePos( );
			CMapModule::GetSingleton().Teleport( mpEntity->GetLineID( ), mpEntity->GetMapID( ), tMovePos.mX, tMovePos.mY, mpEntity );			
			// 如果是功能NPC 清除目标点(回来就不能再走了)
			if ( mpEntity->GetEntityType() == ENTITYTYPE_FUNCNPC )
			{
				ResetTarget( );
				mPreList.clear();
				mPathList.clear();
			}
			Stand( 2000 );
			CMapModule::GetSingleton().PathEntity( mpEntity, mpEntity->GetPosX( ), mpEntity->GetPosY( ), -1, -1, 0 );
		}
	}

	// 处理复杂策略
	int tID =  tplNpc->mStrategyID ;
	if ( tID != 0 )
	{
		// 处理NPC策略的计时器
		OnTacticTimer( tTickOffset );
		OnTactics( tID );
	}	

	// 如果跟随了目标,向目标的方向前进
	if ( mFollowID != 0 )
	{
		CEntityCharacter* tpTarget = ( CEntityCharacter* )CSceneLogic::GetSingleton( ).GetEntity( mFollowID );
		if ( tpTarget != NULL )
		{
			CWTPoint tMovePos = tpTarget->GetPos( );
			tMovePos.mX += mOffsetX;
			tMovePos.mY += mOffsetY;
			MoveTo( tMovePos );
		}
	}

	// 如果NPC配了巡逻模式，按照配的路线行走
	if ( mPatrolIndex >= 0 && mPatrolIndex < (int)ARRAY_CNT(mPatrolList) )
	{
		// 如果走到了配置的目标点，寻找下一个点
		if ( mpEntity->GetPos( ) == mPatrolList[ mPatrolIndex ] )
		{

			if ( mPatrolDirection == 0 )
			{
				mPatrolIndex++;
				if ( mPatrolIndex >= (int)ARRAY_CNT(mPatrolList) || mPatrolList[ mPatrolIndex ] == CWTPoint( -1, -1 ) )
				{
					mPatrolIndex = mPatrolIndex - 2;
					mPatrolDirection = 1;
					mpEntity->MoveStop( mpEntity );
				}
			}
			else if( mPatrolDirection == 1 )
			{
				mPatrolIndex--;
				if ( mPatrolIndex < 0 )
				{
					mPatrolIndex = 1;
					mPatrolDirection = 0;
					mpEntity->MoveStop( mpEntity );
				}
			}
		}
		if( mPatrolDirection >= 0 && mPatrolIndex >= 0 )
		{
			MoveTo( mPatrolList[ mPatrolIndex ] );
		}
	}

	// 如果罚站计时器开启了
	if ( mStandTimer > 0 )
	{
		mStandTimer = std::max( 0, int( mStandTimer - tTickOffset ) );
		if ( mStandTimer == 0 )
		{			
			if ( rand( ) % 2 != 0 )
			{
				Stand( 7000 );
				return;
			}
			mStepTimer = mRunSpeed;	

			OnIdle( );

			PathNpc( );
		}
		else
		{			
			return;
		}
	}	

	if ( mpEntity->GetPos( ) == mpEntity->GetOrginPos( ) )
	{
		OnIdle( );
	}
	// 如果达到了目标点，触发OnIdle事件
	if ( mpEntity->GetPos( ) == mTargetPos )
	{
		OnIdle( );		
	}
	else
	{
		// 如果移动计时器到时,触发一次移动,并触发移动一步结束的事件
		mStepTimer -= tTickOffset;
		if ( mStepTimer <= 0 && mRunSpeed != 0 )
		{
			mStepTimer	= mRunSpeed + mStepTimer;		

			StepNpc( );		

			OnStep( );		

			if ( rand( ) % 2 != 0 )
			{
				if ( tplNpc->mPatrolType == CTemplateOgre::PATROL_RAND )
				{
					Stand( 7000 );
					CMapModule::GetSingleton().PathEntity( mpEntity, mpEntity->GetPosX( ), mpEntity->GetPosY( ), -1, -1, 0 );
					return;
				}				
			}

			PathNpc( );
		}
	}
}

void CNpcPolicy::OnBattle( unsigned int vTickCount )
{
	unsigned int tTickOffset = (mLastTick == 0) ? 0 : vTickCount - mLastTick;
	mLastTick = vTickCount;

	CTemplateOgre* tplNpc = (CTemplateOgre*)CDataStatic::SearchTpl( mNpcID );
	LK_ASSERT( tplNpc != NULL, return  );	

	// 如果处于混乱状态，什么也不做
	if ( mpEntity->GetContorlable( ) == false )
	{
		return;
	}

	if ( tplNpc->mIsInitiative == 1 && GetEnmityEntityID( ) == 0 && mpEntity->GetSuperState( ) == false )
		SearchEnemy( );

	// 如果NPC处于返回状态，并且到达了出生点，开启罚站计时器，关闭返回状态
	if ( ( mIsReturn == true ) && ( mpEntity->GetPos( ) == mTargetPos ) )
	{
		Stand( 2000 );
		// 通知客户端停止移动
		CMapModule::GetSingleton().PathEntity( mpEntity, mpEntity->GetPosX( ), mpEntity->GetPosY( ), -1, -1, 0 );
		OnBackLogic( );
	}

	if ( mAttackTimer > 0 )
		mAttackTimer -= tTickOffset;

	if ( mTeleTimer > 0 )
	{
		mTeleTimer -= tTickOffset;
		// 如果时间到还没走回,传送
		if ( mTeleTimer <= 0 )
		{
			OnBackLogic( );
			CWTPoint tMovePos = GetMovePos( );
			CMapModule::GetSingleton().Teleport( mpEntity->GetLineID( ), mpEntity->GetMapID( ), tMovePos.mX, tMovePos.mY, mpEntity );
			// 如果是功能NPC 清除目标点(回来就不能再走了)
			if ( mpEntity->IsFunNpc() )
			{
				ResetTarget( );
				mPreList.clear();
				mPathList.clear();
			}
			Stand( 2000 );
			CMapModule::GetSingleton().PathEntity( mpEntity, mpEntity->GetPosX( ), mpEntity->GetPosY( ), -1, -1, 0 );
		}
	}

	// 处理复杂策略
	int tID = tplNpc->mStrategyID;
	if ( tID != 0 )
	{
		// 处理NPC策略的计时器
		OnTacticTimer( tTickOffset );
		OnTactics( tID );
	}

	// 如果死了,清空目标和仇恨列表
	if ( mpEntity->IsAlive( ) == false )
	{
		OnDieLogic( );
		return;
	}		

	// 如果罚站计时器开启了
	if ( mStandTimer > 0 )
	{
		mStandTimer = std::max( 0, int( mStandTimer - tTickOffset ) );
		if ( mStandTimer == 0 )
		{
			mStepTimer = mRunSpeed;
			// 如果已经到达攻击目标点		
			CEntity* tpTarget = CSceneLogic::GetSingleton( ).GetEntity( mTargetEntityID );
			if ( tpTarget != NULL )
			{
				CWTPoint tDesPos = tpTarget->GetPos( );
				MoveTo( tDesPos );
			}
			if ( mIsAttacking == true )
			{
				mIsAttacking = false;
				PathNpc( );
			}
		}
		else
		{			
			return;
		}
	}	

	// 如果达到了目标点，触发OnIdle事件
	if ( mpEntity->GetPos( ) == mTargetPos )
	{
		OnStep( );
		DoAttack( );
	}
	else
	{
		// 如果移动计时器到时,触发一次移动,并触发移动一步结束的事件
		mStepTimer -= tTickOffset;
		if ( mStepTimer <= 0 && mRunSpeed != 0 )
		{	
			mStepTimer	= mRunSpeed + mStepTimer;	

			StepNpc( );

			OnStep( );		// 是否攻击的判断	

			const int &tPolicyID = get_id( );
			const int &tOgreID = mpEntity->get_id( );
			DoAttack( );
			if( ( tPolicyID == INVALID_OBJ_ID ) || tOgreID == INVALID_OBJ_ID )
			{
				LOG_NOTICE( "default", "[ %s : %d ][ %s ] Pet UseSkill Ogre Disppear", 
					__LK_FILE__, __LINE__, __FUNCTION__ );
				return;
			}							

			PathNpc( );
		}
	}
}

void CNpcPolicy::SearchEnemy( )
{
	// 如果是主动怪，在这里确定攻击目标
	std::vector<CEntity*> tEnemys;
	AttackableEntityChecker  tChecker( mpEntity );
	SeenPlayerContainserSearcher<AttackableEntityChecker> tSearcher( tChecker, tEnemys );
	tSearcher.Visit( mpEntity );

	// 如果没找到目标，继续找
	if( tEnemys.empty() )
	{
		SeenNpcContainserSearcher<AttackableEntityChecker> tSearcher( tChecker, tEnemys );
		tSearcher.Visit( mpEntity );
	}

	CTemplateOgre *tpOgre = (CTemplateOgre*)CDataStatic::SearchTpl( mNpcID );
	if ( tpOgre == NULL )
		return;
	for ( std::vector<CEntity*>::iterator it = tEnemys.begin(); it != tEnemys.end(); ++it )
	{	
		CEntity *tEntity = *it;
		if ( tEntity == NULL )
			continue;
		if( mpEntity->IsWithinDistance( tEntity, tpOgre->mSearchDistance ) == false )
			continue;

		if ( tEntity->IsPlayer() )
		{
			CEntityPlayer *tpPlayer = (CEntityPlayer*)tEntity;
			CPropertyPlayer *tpProperty = (CPropertyPlayer*)tpPlayer->GetProperty();
			CPropertyCharacter *tpNpcProperty = (CPropertyCharacter*)mpEntity->GetProperty();
			if ( tpProperty->GetStateParam( STATE_HIDE ).mInState == true 
				&& (int)(tpNpcProperty->GetLevel()) <= tpProperty->GetStateParam( STATE_HIDE ).mParames1 )
			{
				continue;
			}
		}

		if ( mNpcState == CNpcPolicy::STATE_PEACE )
			mBattleStart = true;
		mNpcState = CNpcPolicy::STATE_BATTLE;
		mAttackTimer = 300;
		mStandTimer = 0;
		if ( tEntity->IsTaskNpc() )
		{
			InsertEnmity( tEntity->GetEntityID( ), 1000000 );
		}
		else
			InsertEnmity( tEntity->GetEntityID( ), 1 );
		mTargetEntityID = tEntity->GetEntityID( );
		mIsReturn = false;
		mpEntity->SetSuperState( false );
		mTeleTimer = 0;
	}
}

void CNpcPolicy::OnTacticTimer( unsigned int vTickOffSet )
{
	for ( unsigned int i = 0; i < NPCTIMERNUM && i < sizeof( mTimer ) / sizeof( mTimer[ 0 ] ); i++ )
	{
		if ( mTimer[ i ].mID > 0  && mTimer[ i ].mTimer > 0 )
		{
			if ( mTimer[ i ].mNum == 0 )
			{
				mTimer[ i ].mTimer = 0;
				mTimer[ i ].mEnabled = false;
				continue;
			}
			mTimer[ i ].mTimer -= vTickOffSet;
			if ( mTimer[ i ].mTimer <= 0 )
			{
				mTimer[ i ].mEnabled = true;
				mTimer[ i ].mTimer = mTimer[ i ].mInterval * 1000;
				if ( mTimer[ i ].mNum > 0 )
					mTimer[ i ].mNum--;
			}
			else
			{
				mTimer[ i ].mEnabled = false;
			}			
		}
	}
}

void CNpcPolicy::OnBackLogic( )
{
	mIsReturn = false;					// 关闭返回状态
	mTargetEntityID = 0;
	mNpcState	= STATE_PEACE;			// 恢复和平状态	
	mpEntity->SetSuperState( false );	// 关闭牛比状态
	memset( mDone, 0, sizeof( mDone ) );
	mCanUseSkill = 0;
	mSkillList.clear();
	mTeleTimer = 0;
	CTemplateOgre* tpOgreTpl = (CTemplateOgre*) CDataStatic::SearchTpl( mNpcID );
	if( tpOgreTpl == NULL )
	{
		LOG_ERROR( "default", "Can't Find NpcID %d", mNpcID );	
		return ;
	}
	if ( tpOgreTpl->mRecover == 0 )
	{
		CPropertyModule::GetSingleton().RecoverPer( mpEntity, 100 );
	}	
	if (mpEntity->GetNpcChangeState() != 0) // NPC已经变身
		CPropertyModule::GetSingleton().RecoverNpcState( mpEntity );
}

void CNpcPolicy::OnDieLogic( )
{
	mTargetEntityID = 0;
	mAttackTimer = 0;
	ClearEnmity( 0 );
	mTargetPos = mpEntity->GetOrginPos( );
	mPathPos = mpEntity->GetOrginPos( );
	mNpcState = STATE_PEACE;
	memset( mDone, 0, sizeof( mDone ) );
	mCanUseSkill = 0;
	mSkillList.clear();	
}

void CNpcPolicy::OnIdle( )
{
	CEntityNpc *pNpc = (CEntityNpc*)mpEntity;
	if ( pNpc == NULL )
		return;
	if ( pNpc->IsPet() )
		return;
	CTemplateOgre* tpOgreTpl = (CTemplateOgre*) CDataStatic::SearchTpl( mNpcID );
	if( tpOgreTpl == NULL )
	{
		LOG_ERROR( "default", "Can't Find NpcID %d", mNpcID );	
		return ;
	}

	// 如果普通怪处于和平状态
	if ( ( mNpcState == CNpcPolicy::STATE_PEACE ) && ( mIsReturn == false ) )
	{
		// 如果怪物随机走动
		if ( tpOgreTpl->mPatrolType == CTemplateOgre::PATROL_RAND )
		{

			CWTPoint tNextTarget = GetRandPos( );
			if ( tNextTarget == CWTPoint( ) )
				return;

			MoveTo( tNextTarget );
		}
		// 如果怪物站立不动
		else if ( tpOgreTpl->mOgreType == CTemplateOgre::PATROL_STAND )
		{
			return;
		}
		else if ( tpOgreTpl->mOgreType == CTemplateOgre::PATROL_FIX )
		{
			if ( mpEntity->IsOgre() )
			{
				/*COgreCreator *pCreator = (COgreCreator *)CSceneLogic::GetSingletonPtr()->GetEntity( mpEntity->GetCreatorID() );
				LK_ASSERT( pCreator != NULL, return );

				if ( mpEntity->mPos == pCreator->mPathList[ 0 ] )
				{
				MoveTo( mpEntity->GetOrginPos( ) );
				}
				else if ( mpEntity->mPos == mpEntity->GetOrginPos( ) && pCreator->mPCount > 0 )
				{
				MoveTo( pCreator->mPathList[ 0 ] );
				}*/
			}
		}
	}
}

void CNpcPolicy::OnStep( )
{
	CTemplateOgre* tpOgre = (CTemplateOgre*) CDataStatic::SearchTpl( mNpcID );
	if ( tpOgre == NULL )
		return;
	// 判断怪物是否超过休闲距离 (非变怪状态)
	if ( mpEntity->GetPos( ).LimitDistance( mpEntity->GetOrginPos( ), tpOgre->mWalkDistance ) == false && mNpcState == CNpcPolicy::STATE_PEACE && mpEntity->GetNpcChangeState() == 0 )
	{
		Return2Orgin( false, false );		
		return;
	}
	// 判断怪物是否超过仇恨距离
	if ( mpEntity->GetPos( ).LimitDistance( GetMovePos( ), tpOgre->mEnmityDistance ) == false 
		&& mNpcState == CNpcPolicy::STATE_BATTLE && mIsReturn == false )
	{
		mAttackTimer = 0;
		if ( mFollowID != 0 || mPatrolIndex >= 0 )
			Return2Orgin( false, false );
		else
			Return2Orgin( true, true );
		// 通知属性模块重置第一刀
		ResetTarget( );
		mBattleEnd = true;
		mTeleTimer = tpOgre->mTeleTime;
		return;
	}

	// 如果处于战斗状态( 不是属性模块的战斗状态 )
	if ( mNpcState == CNpcPolicy::STATE_BATTLE )
	{
		CEntityCharacter* tpTarget = ( CEntityCharacter* )CSceneLogic::GetSingleton( ).GetEntity( mTargetEntityID );
		// 如果目标找不到了，或是目标已经死亡立刻跑回出生点
		if ( tpTarget == NULL || tpTarget->IsAlive( ) == false || 
			tpTarget->GetLineID() != mpEntity->GetLineID() ||
			tpTarget->GetMapID() != mpEntity->GetMapID() ||
			tpTarget->CanFly() != mpEntity->CanFly() )
		{
			if ( mTargetEntityID != 0 )
			{
				// 清空这个目标的仇恨
				ClearEnmity( mTargetEntityID, 0 );
				int SecondID = GetEnmityEntityID( );
				if( SecondID != 0 )
				{
					mTargetEntityID = SecondID;
				}
				// 如果仇恨列表里没有目标，跑回出发点
				else
				{
					// 如果跟随了目标或者有巡逻路线,不返回出生点,也不回血
					if ( mFollowID != 0 || mPatrolIndex >= 0 )
						Return2Orgin( false, false );
					else
						Return2Orgin( true, true );
					//pPolicy->mNpcState = CNpcPolicy::STATE_PEACE;
					mAttackTimer = 0;
					mTargetEntityID = 0;
					if ( mBattleEnd == false )
						mBattleEnd = true;
					mTeleTimer = tpOgre->mTeleTime;
					return;
				}
			}
		}
	}
}


