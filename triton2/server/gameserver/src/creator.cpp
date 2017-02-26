#include "stdafx.hpp"
#include "creator.h"
#include "servercore.h"
#include "proclient_svr.h"


template<> CCreatorManager* CSingleton< CCreatorManager >::spSingleton = NULL;
CSharedMem* CCreatorManager::mShmPtr = NULL;

CCreatorManager::CCreatorManager()
{
	if( mShmPtr->GetInitMode () == SHM_INIT )
	{
		memset( mOgreCreator, 0, sizeof( mOgreCreator ) ); 
		mRefreshList.initailize();
		mPedalList.initailize();
		mNotifyList.initailize();
	}
} 

CCreatorManager::~CCreatorManager()
{
	unsigned int i = 0;
	for( i = 0; i < sizeof( mOgreCreator ) / sizeof( mOgreCreator[ 0 ] ); i++ )
	{
		if( mOgreCreator[ i ] != 0 )
		{
			CSceneObjManager::GetSingletonPtr()->DestroyObject( mOgreCreator[ i ] );
			mOgreCreator[ i ] = 0;
		}
	}
}



void* CCreatorManager::operator new( size_t size )
{ 
	return mShmPtr->CreateSegment( size );
}

void CCreatorManager::operator delete( void* pointer )
{ }

void* CCreatorManager::operator new(size_t size, const char* file, int line)
{
	return operator new( size );
}
void CCreatorManager::operator delete(void* pointer, const char* file, int line)
{

}

int CCreatorManager::InsertCreatorBase( int vIndex, CCreator* vpCreator )
{
	if( vIndex > (int)ARRAY_CNT( mOgreCreator ) )
	{
		return -1;
	}
	mOgreCreator[ vIndex ] = vpCreator->get_id();
	return 0;
}

CCreator* CCreatorManager::GetCreator( int vIndex  )
{
	if( vIndex < (int)ARRAY_CNT( mOgreCreator ) )
	{
		return ( CCreator* )CSceneObjManager::GetSingletonPtr()->GetObject( mOgreCreator[ vIndex ] );
	}
	else
	{
		return NULL;
	}	
}

void CCreatorManager::RefreshPedals( )
{
	PedalRefreshList::iterator tBegin = mPedalList.begin();
	PedalRefreshList::iterator tEnd = mPedalList.end();
	for ( ; tBegin != tEnd; ++tBegin )
	{
		CPedalCreator *tpCreator = (CPedalCreator*)CSceneObjManager::GetSingleton().GetObject( *tBegin );
		if ( tpCreator == NULL )
		{
			continue;
		}
		tpCreator->RefreshActive( );
	}	
}

void CCreatorManager::RemoveCreator( CCreator *pCreator )
{
	if ( pCreator == NULL )
		return;

	TimeRefreshList::iterator tTimeBegin = mRefreshList.begin();
	TimeRefreshList::iterator tTimeEnd = mRefreshList.end();
	for ( ; tTimeBegin != tTimeEnd; ++tTimeBegin )
	{
		if ( *tTimeBegin == pCreator->get_id() )
		{
			mRefreshList.erase( tTimeBegin );
			break;
		}
	}

	PedalRefreshList::iterator tPedalBegin = mPedalList.begin();
	PedalRefreshList::iterator tPedalEnd = mPedalList.end();
	for ( ; tPedalBegin != tPedalEnd; ++tPedalBegin )
	{
		if ( *tPedalBegin == pCreator->get_id() )
		{
			mPedalList.erase( tPedalBegin );
			break;
		}
	}

	mOgreCreator[ pCreator->GetIndex( ) ] = 0;
}

/************************************************************************/
/* COgreCreator                                                                     */
/************************************************************************/
int COgreCreator::Initialize()
{
	memset(  &mInit 	   , 0, sizeof(   mInit 		 ) ); 
	memset(  &mRefresh1	   , 0, sizeof(	  mRefresh1		 ) );
	memset(  &mRCount1	   , 0, sizeof(	  mRCount1		 ) );
	memset(  &mRefresh2	   , 0, sizeof(	  mRefresh2		 ) );
	memset(  &mRCount2	   , 0, sizeof(	  mRCount2		 ) );
	memset(  &mRefresh3	   , 0, sizeof(	  mRefresh3		 ) );
	memset(  &mRCount3	   , 0, sizeof(	  mRCount3		 ) );
	memset(  &mRefresh4	   , 0, sizeof(	  mRefresh4		 ) );
	memset(  &mRCount4	   , 0, sizeof(	  mRCount4		 ) );
	memset(  &mRefresh5	   , 0, sizeof(	  mRefresh5		 ) );
	memset(  &mRCount5	   , 0, sizeof(	  mRCount5		 ) );
	memset(  &mCommonID	   , 0, sizeof(	  mCommonID		 ) );
	memset(  &mRefreshRate , 0, sizeof(	  mRefreshRate	 ) );
	memset(  &mDisappear   , 0, sizeof(	  mDisappear 	 ) ); 
	memset(  &mDirection   , 0, sizeof(	  mDirection 	 ) ); 
	memset(  &mRelife 	   , 0, sizeof(	  mRelife 		 ) );
	memset(  &mPosition    , 0, sizeof(	  mPosition 	 ) ); 
	memset(  &mCountNum    , 0, sizeof(	  mCountNum 	 ) ); 
	memset(  &mCountDelay  , 0, sizeof(	  mCountDelay	 ) ); 
	memset(  &mCurrentCount, 0, sizeof(	  mCurrentCount	 ) );
	memset(  &mRefreshTime , 0, sizeof(	  mRefreshTime	 ) );
	memset(  &mRCount6	   , 0, sizeof(	  mRCount6		 ) );
	//memset(  &mHasOgre 	   , 0, sizeof(	  mHasOgre 		 ) );
	memset(  &mPathListX   , 0, sizeof(	  mPathListX	 ) );
	memset(  &mPathListY   , 0, sizeof(	  mPathListY	 ) );
	memset(  &mPCount	   , 0, sizeof(	  mPCount		 ) ); 
	memset(  &mLineID	   , 0, sizeof(	  mLineID		 ) ); 
	memset(  &mMapID	   , 0, sizeof(	  mMapID		 ) );
	memset(  &mInitBuffID  , 0, sizeof(	  mInitBuffID	 ) ); 
	memset(  &mTimeBuffID  , 0, sizeof(	  mTimeBuffID	 ) );
	memset(  &mFuncNpc	   , 0, sizeof(	  mFuncNpc		 ) );
	memset(  &mRefresh7	   , 0, sizeof(	  mRefresh7		 ) );
	memset(  &mRCount7	   , 0, sizeof(	  mRCount7		 ) );
	memset(  &mRefresh8	   , 0, sizeof(	  mRefresh8		 ) );
	memset(  &mRCount8	   , 0, sizeof(	  mRCount8		 ) );	
	mOwnerCharID = 0;
	mIsSendNpc = 0;
	mIsShowOwner = 0;
	mCoolTime = 0;
	mMaxEnmity = 0;
//	mActivity = 0;
	mNextRefreshTime = 0;
	return SUCCESS;
}			
			
bool COgreCreator::LoadFromConfig(TiXmlElement* pElemBlock)
{
	const char* tpOgreIndex		= pElemBlock->Attribute( "OgreIndex" );	LK_ASSERT( tpOgreIndex != NULL, return false);
	const char* tpInit			= pElemBlock->Attribute( "OgreInit" );	LK_ASSERT( tpInit != NULL, return false );
	const char*	tpDisappear		= pElemBlock->Attribute( "Disappear" );	LK_ASSERT( tpDisappear != NULL, return false );		
	const char* tpRelife		= pElemBlock->Attribute( "Relife" );	LK_ASSERT( tpRelife != NULL, return false );
	const char* tpX				= pElemBlock->Attribute( "PosX" );		LK_ASSERT( tpX != NULL, return false );
	const char* tpY				= pElemBlock->Attribute( "PosY" );		LK_ASSERT( tpY != NULL, return false );
	const char*	tpDirection		= pElemBlock->Attribute( "Dir" );		LK_ASSERT( tpDirection != NULL, return false );
	const char* tpCountNum		= pElemBlock->Attribute( "CountNum" );	LK_ASSERT( tpCountNum != NULL, return false );
	const char* tpCountDelay	= pElemBlock->Attribute( "CountDelay" );LK_ASSERT( tpCountDelay != NULL, return false );
	const char* tpRand			= pElemBlock->Attribute( "Rand" );		LK_ASSERT( tpRand != NULL, return false );
	const char* tpInitBuffID	= pElemBlock->Attribute( "InitBuffID" );//LK_ASSERT( tpInitBuffID != NULL, return false );
	const char* tpTimeBuffID	= pElemBlock->Attribute( "TimeBuffID" );//LK_ASSERT( tpTimeBuffID != NULL, return false );
	const char* tpFuncNpc		= pElemBlock->Attribute( "IsFuncNpc" );
	const char* tpMaxEnmity		= pElemBlock->Attribute( "MaxEnmity" );
	const char* tpActivity		= pElemBlock->Attribute( "Activity" );
	const char* tpNotify		= pElemBlock->Attribute( "Notify" );

	mInit			= ::atoi( tpInit );
	mDisappear		= ::atoi( tpDisappear );
	mDirection		= (EDirType)::atoi( tpDirection );
	mRelife			= ::atoi( tpRelife );
	mPosition.mX	= ::atoi( tpX );
	mPosition.mY	= ::atoi( tpY );
	mCountNum		= ::atoi( tpCountNum );
	mCountDelay		= ::atoi( tpCountDelay );
	mRefreshRate	= ::atoi( tpRand );

	if ( tpInitBuffID != NULL )
	mInitBuffID = ::atoi( tpInitBuffID );
	if ( tpTimeBuffID != NULL )
	mTimeBuffID = ::atoi( tpTimeBuffID );
	if ( tpFuncNpc != NULL )
	mFuncNpc = ::atoi( tpFuncNpc );
	if ( tpMaxEnmity != NULL )
	mMaxEnmity = ::atoi( tpMaxEnmity );
	if ( tpActivity != NULL )
	mActivity = 0-::atoi( tpActivity ); // 潜规则 先弄成负数 后面有用
	if ( tpNotify != NULL && ::atoi( tpNotify ) == 1 )
	CCreatorManager::GetSingleton().mNotifyList.push_back( this->get_id() );

	memset( mRefresh1, 0, sizeof( mRefresh1 ) );
	memset( mRefresh2, 0, sizeof( mRefresh2 ) );
	memset( mRefresh3, 0, sizeof( mRefresh3 ) );
	memset( mRefresh4, 0, sizeof( mRefresh4 ) );
	memset( mRefresh5, 0, sizeof( mRefresh5 ) );
	memset( mRefresh7, 0, sizeof( mRefresh7 ) );
	memset( mRefreshTime, 0, sizeof( mRefreshTime ) );
	mRCount1 = 0;
	mRCount2 = 0;
	mRCount3 = 0;
	mRCount4 = 0;
	mRCount5 = 0;
	mRCount6 = 0;
	mRCount7 = 0;
	mCurrentCount = 0;
	memset( mPathListX, 0, sizeof( mPathListX ) );
	memset( mPathListY, 0, sizeof( mPathListY ) );
	mPCount = 0;

	int tID = atoi( tpOgreIndex );
	if( tID >= OGREINDEX )
	{
		LOG_ERROR( "map", "[%s:%d] id %d is larger than maxindex %d ", 
			__FUNCTION__, __LINE__, tID, OGREINDEX );
		return false ;
	}

	// RefreshDate 
	TiXmlElement* tpSubEle = pElemBlock->FirstChildElement( "RefreshDate" );
	LK_ASSERT ( tpSubEle != NULL, return false );
	bool tNeedRefresh = false;
	for( TiXmlElement* tpRefreshDate = tpSubEle->FirstChildElement() ; tpRefreshDate != NULL; tpRefreshDate = tpRefreshDate->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpRefreshDate->Value( );
		if ( tEleName1 == "Refresh" )
		{
			const char* tpDate	= tpRefreshDate->Attribute( "RefreshDate" );
			LK_ASSERT( tpDate != NULL && strlen(tpDate) >= 12, return false );

			char tYear[5] = {0}, tMonth[3] = {0}, tDate[3] = {0};
			char tHour[3] = {0}, tMinute[3] = {0}, tWeek[2] = {0}, tDay[2] = {0};
			memcpy( tYear,	 tpDate,	  4 );
			memcpy( tMonth,	 tpDate + 4,  2 );
			memcpy( tDate,	 tpDate + 6,  2 );
			memcpy( tHour,	 tpDate + 8,  2 );
			memcpy( tMinute, tpDate + 10, 2 );
			memcpy( tDay,	 tpDate + 12, 1 );
			memcpy( tWeek,	 tpDate + 13, 1 );
			mRefreshTime[ mRCount6 ].mYear		= atoi( tYear );
			mRefreshTime[ mRCount6 ].mMonth		= atoi( tMonth );
			mRefreshTime[ mRCount6 ].mDate		= atoi( tDate );
			mRefreshTime[ mRCount6 ].mHour		= atoi( tHour );
			mRefreshTime[ mRCount6 ].mMinute	= atoi( tMinute );
			mRefreshTime[ mRCount6 ].mWeek		= atoi( tWeek );
			mRefreshTime[ mRCount6 ].mDay		= atoi( tDay );
			mRCount6++;	
			if ( (unsigned int)mRCount6 >= ARRAY_CNT( mRefreshTime ) )
				break;
			tNeedRefresh = true;
		}
	}
	if ( tNeedRefresh == true )
	{
		CCreatorManager::GetSingleton().mRefreshList.push_back( this->get_id() );
	}

	int nPossibilityCheck  = 0;

	// DeadRefresh
	tpSubEle = pElemBlock->FirstChildElement( "DeadRefresh" );
	LK_ASSERT ( tpSubEle != NULL, return false );			
	for( TiXmlElement* tpDeadRefresh = tpSubEle->FirstChildElement(); tpDeadRefresh != NULL; tpDeadRefresh = tpDeadRefresh->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpDeadRefresh->Value( );
		if ( tEleName1 == "NewOgre" )
		{
			const char* tpID	= tpDeadRefresh->Attribute( "ID" );
			const char*	tpRand	= tpDeadRefresh->Attribute( "Rand" );
			LK_ASSERT ( tpID != NULL && tpRand != NULL, return false );

			mRefresh1[ mRCount1 ].mOgreID	= atoi( tpID );
			mRefresh1[ mRCount1 ].mOgreRand = atoi( tpRand );

			nPossibilityCheck += mRefresh1[ mRCount1 ].mOgreRand;

			mRCount1++;
			if ( (unsigned int)mRCount1 >= ARRAY_CNT( mRefresh1 ) )
				break;
		}									
	}
	
	if( nPossibilityCheck !=0 && nPossibilityCheck != 10000 )
	{
		LOG_ERROR( "map", "ogre creator index %d deadrefreash rand total is not 10000", tID );
	}
	
	
	// OccureRefresh
	tpSubEle = pElemBlock->FirstChildElement( "OccurRefresh" );
	LK_ASSERT ( tpSubEle != NULL, return false );
	for( TiXmlElement* tpOccurRefresh = tpSubEle->FirstChildElement(); tpOccurRefresh != NULL; tpOccurRefresh = tpOccurRefresh->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpOccurRefresh->Value( );
		if ( tEleName1 == "NewOgre" )
		{
			const char* tpOldID	= tpOccurRefresh->Attribute( "OldID" );
			const char*	tpTime	= tpOccurRefresh->Attribute( "Time" );
			const char* tpNewID	= tpOccurRefresh->Attribute( "NewID" );
			LK_ASSERT ( tpOldID != NULL && tpTime != NULL && tpOldID != NULL, return false );

			mRefresh2[ mRCount2 ].mOldID	= atoi( tpOldID );
			mRefresh2[ mRCount2 ].mTime		= atoi( tpTime );
			mRefresh2[ mRCount2 ].mNewID	= atoi( tpNewID );
			mRCount2++;
			if ( (unsigned int)mRCount2 >= ARRAY_CNT( mRefresh2 ) )
				break;
		}									
	}							

	// RandRefresh
	tpSubEle = pElemBlock->FirstChildElement( "RandRefresh" );
	LK_ASSERT ( tpSubEle != NULL, return false );
	for( TiXmlElement* tpRandRefresh = tpSubEle->FirstChildElement(); tpRandRefresh != NULL; tpRandRefresh = tpRandRefresh->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpRandRefresh->Value( );
		if ( tEleName1 == "NewOgre" )
		{
			const char* tpIndex	= tpRandRefresh->Attribute( "Index" );
			LK_ASSERT ( tpIndex != NULL, return false );

			mRefresh3[ mRCount3 ]	= atoi( tpIndex );
			mRCount3++;
			if ( (unsigned int)mRCount3 >= ARRAY_CNT( mRefresh3 ) )
				break;
		}									
	}							

	// CountRefresh
	tpSubEle = pElemBlock->FirstChildElement( "CountRefresh" );
	LK_ASSERT ( tpSubEle != NULL, return false );	
	for( TiXmlElement* tpCountRefresh = tpSubEle->FirstChildElement(); tpCountRefresh != NULL; tpCountRefresh = tpCountRefresh->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpCountRefresh->Value( );
		if ( tEleName1 == "NewOgre" )
		{
			const char* tpIndex	= tpCountRefresh->Attribute( "Index" );
			LK_ASSERT( tpIndex != NULL, return false );
			mRefresh4[ mRCount4 ]	= atoi( tpIndex );
			mRCount4++;
			if ( (unsigned int)mRCount4 >= ARRAY_CNT( mRefresh4 ) )
				break;
		}									
	}
	
	// CommonRefresh
	tpSubEle = pElemBlock->FirstChildElement( "CrossRefresh" );
	LK_ASSERT ( tpSubEle != NULL, return false );
	for( TiXmlElement* tpCrossRefresh = tpSubEle->FirstChildElement(); tpCrossRefresh != NULL; tpCrossRefresh = tpCrossRefresh->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpCrossRefresh->Value( );
		if ( tEleName1 == "NewOgre" )
		{
			const char* tpIndex	= tpCrossRefresh->Attribute( "Index" );
			const char*	tpRand	= tpCrossRefresh->Attribute( "Probability" );
			LK_ASSERT ( tpIndex != NULL && tpRand != NULL, return false );

			mRefresh5[ mRCount5 ].mIndex	= atoi( tpIndex );
			mRefresh5[ mRCount5 ].mOgreRand	= atoi( tpRand );
			mRCount5++;
			if ( (unsigned int)mRCount5 >= ARRAY_CNT( mRefresh5 ) )
				break;
		}		
	}

	// Path
	tpSubEle = pElemBlock->FirstChildElement( "Path" );
	LK_ASSERT ( tpSubEle != NULL, return false );
	for( TiXmlElement* tpPath = tpSubEle->FirstChildElement(); tpPath != NULL; tpPath = tpPath->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpPath->Value( );
		if ( tEleName1 == "Point" )
		{
			const char* tpX	= tpPath->Attribute( "x" );
			const char*	tpY	= tpPath->Attribute( "y" );
			LK_ASSERT ( tpX != NULL && tpY != NULL, return false );

			mPathListX[ mPCount ]	= atoi( tpX );
			mPathListY[ mPCount ]	= atoi( tpY );
			mPCount++;
			if ( (unsigned int)mPCount >= ARRAY_CNT( mPathListX ) || (unsigned int)mPCount >= ARRAY_CNT( mPathListY ) )
				break;
		}		
	}

	// Live
	tpSubEle = pElemBlock->FirstChildElement( "RefreshByDead" );
	LK_ASSERT ( tpSubEle != NULL, return false );
	for( TiXmlElement* tpDeathRefresh = tpSubEle->FirstChildElement(); tpDeathRefresh != NULL; tpDeathRefresh = tpDeathRefresh->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpDeathRefresh->Value( );
		if ( tEleName1 == "NewOgre" )
		{
			const char* tpIndex	= tpDeathRefresh->Attribute( "Index" );
			LK_ASSERT ( tpIndex != NULL, return false );

			mRefresh7[ mRCount7 ]	= atoi( tpIndex );
			mRCount7++;
			if ( (unsigned int)mRCount7 >= ARRAY_CNT( mRefresh7 ) )
				break;
		}		
	}
	
	tpSubEle = pElemBlock->FirstChildElement( "RefreshByLive" );
	LK_ASSERT ( tpSubEle != NULL, return false );
	for( TiXmlElement* tpLiveRefresh = tpSubEle->FirstChildElement(); tpLiveRefresh != NULL; tpLiveRefresh = tpLiveRefresh->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpLiveRefresh->Value( );
		if ( tEleName1 == "NewOgre" )
		{
			const char* tpIndex	= tpLiveRefresh->Attribute( "Index" );
			LK_ASSERT ( tpIndex != NULL, return false );

			mRefresh8[ mRCount8 ]	= atoi( tpIndex );
			mRCount8++;
			if ( (unsigned int)mRCount8 >= ARRAY_CNT( mRefresh8 ) )
				break;
		}		
	}

	SetIndex( tID );	

	/*mLineID = vLineID;
	mMapID = vMapID;*/
	mHasOgre = 0;

	return true;
}

/************************************************************************/
/* CPedalCreator                                                                     */
/************************************************************************/
bool CPedalCreator::LoadFromConfig(TiXmlElement* pElemBlock)
{
	const char* tpNpcID		= pElemBlock->Attribute( "NpcID" );		LK_ASSERT( tpNpcID != NULL, return false );
	const char* tpLineID	= pElemBlock->Attribute( "LineID" );	LK_ASSERT( tpLineID != NULL, return false );
	const char*	tpMapID		= pElemBlock->Attribute( "MapID" );		LK_ASSERT( tpMapID != NULL, return false );
	const char*	tpMinLevel	= pElemBlock->Attribute( "MinLevel" );	LK_ASSERT( tpMinLevel != NULL, return false );	
	//const char*	tpNation	= pElemBlock->Attribute( "Nation" );	LK_ASSERT( tpNation != NULL, return false );
	const char* tpPedalIndex= pElemBlock->Attribute( "TeleportIndex" ); LK_ASSERT( tpPedalIndex != NULL, return false );
	const char* tpInit 		= pElemBlock->Attribute( "Init" ); 
	const char* tpMaxActive	= pElemBlock->Attribute( "MaxActive" );
	const char* tpExistTime	= pElemBlock->Attribute( "ExistTime" );

	for( TiXmlElement* tpPinEle	= pElemBlock->FirstChildElement( ); 
		tpPinEle != NULL;
		tpPinEle = tpPinEle->NextSiblingElement( ) )
	{
		std::string tEleName = tpPinEle->Value( );
		if ( tEleName == "SrcRect" )
		{
			const char* tpX		= tpPinEle->Attribute( "x" );
			const char* tpY		= tpPinEle->Attribute( "y" );
			const char* tpCX	= tpPinEle->Attribute( "cx" );
			const char* tpCY	= tpPinEle->Attribute( "cy" );
			LK_ASSERT ( tpX != NULL && tpY != NULL && tpCX != NULL && tpCY != NULL, return false );
			mSrcRect = CWTRect( CWTPoint( ::atoi( tpX ), ::atoi( tpY ) ), CWTSize( ::atoi( tpCX ), ::atoi( tpCY ) ) );
		}

		if ( tEleName == "DesPos" )
		{
			const char* tpX		= tpPinEle->Attribute( "x" );
			const char* tpY		= tpPinEle->Attribute( "y" );
			LK_ASSERT( tpX != NULL && tpY != NULL, return false );
			mDesPos = CWTPoint( ::atoi( tpX ), ::atoi( tpY ) );
		}

		if ( tEleName == "Time" )
		{
			if ( mRandCount >= (int) ARRAY_CNT(mRandList) )
			{
				continue;
			}
			const char* tpHour		= tpPinEle->Attribute( "hour" );
			const char* tpRand		= tpPinEle->Attribute( "rand" );
			LK_ASSERT ( tpHour != NULL && tpRand != NULL, return false );
			mRandList[ mRandCount ].mHour = ::atoi( tpHour );
			mRandList[ mRandCount ].mRand = ::atoi( tpRand );
			mRandList[ mRandCount ].mActive = false;
			mRandCount++;
		}
		
	}

	if ( mRandCount > 0 )
	{
		CCreatorManager::GetSingleton().mPedalList.push_back( this->get_id() );
	}	

	mLineID		= ::atoi( tpLineID );
	mMapID		= ::atoi( tpMapID );
	mMinLevel	= ::atoi( tpMinLevel );
	mInit		= ( tpInit != NULL ) ? atoi( tpInit ) : 1;		//初始默认创建传送点
	mPedalType	= PEDAL_TELEPORT;
	if ( tpMaxActive != NULL )
	{
		mMaxActive = ::atoi( tpMaxActive );
	}
	if ( tpExistTime != NULL )
	{
		mExistTime = ::atoi( tpExistTime );
	}	

	unsigned int tPedalIndex	= atoi( tpPedalIndex );
	LK_ASSERT( tPedalIndex < OGREINDEX, return false );

	SetIndex( tPedalIndex );

	return true;
}


/************************************************************************/
/*  CCollectCreator                                                                    */
/************************************************************************/
bool CCollectCreator::LoadFromConfig(TiXmlElement* pElemBlock)
{
	const char* tpNpcID	= pElemBlock->Attribute( "NpcID" );		LK_ASSERT( tpNpcID != NULL, return false );
	const char*	tpDirection = pElemBlock->Attribute( "Direction" );	LK_ASSERT( tpDirection != NULL, return false );
	const char* tpRelife	= pElemBlock->Attribute( "Relife" );	LK_ASSERT( tpRelife != NULL, return false );
	const char*	tpDisappear	= pElemBlock->Attribute( "Disappear" );	LK_ASSERT( tpDisappear != NULL, return false );
	const char* tpNpcInit	= pElemBlock->Attribute( "NpcInit" );	LK_ASSERT( tpNpcInit != NULL, return false );
	const char* tpActivity	= pElemBlock->Attribute( "Activity" );

	mPCount = 0;
	TiXmlElement* tpPosEle = pElemBlock->FirstChildElement( );
	for( ; tpPosEle != NULL; tpPosEle = tpPosEle->NextSiblingElement( ) )
	{
		std::string tEleName = tpPosEle->Value( );
		if ( tEleName == "Pos" )
		{
			const char* tpX		= tpPosEle->Attribute( "x" );
			const char* tpY		= tpPosEle->Attribute( "y" );
			LK_ASSERT ( tpX != NULL && tpY != NULL, return false );
			mPosition[ mPCount++ ] = CWTPoint( ::atoi( tpX ), ::atoi( tpY ) );
			if ( mPCount >= ( int )ARRAY_CNT(mPosition) )
				break;			
		}
	}

	mNpcID  = ::atoi( tpNpcID );
	mRelife = ::atoi( tpRelife );
	mDisappear = ::atoi( tpDisappear );
	
	mDirection = (EDirType)::atoi( tpDirection );
	mInit = ::atoi( tpNpcInit );
	if ( tpActivity != NULL )
	{
		mActivity = 0 - ::atoi( tpActivity );		
	}
	
	const char* tpCreatorIndex = pElemBlock->Attribute( "NpcIndex" );
	LK_ASSERT( tpCreatorIndex != NULL, return false );

	unsigned int tCreatorIndex = atoi( tpCreatorIndex );
	LK_ASSERT( tCreatorIndex < OGREINDEX, return false );

	SetIndex( tCreatorIndex );

	return true;
}

/************************************************************************/
/*   CNpcCreator                                                                   */
/************************************************************************/
bool CNpcCreator::LoadFromConfig(TiXmlElement* pElemBlock)
{
	std::string tEleName = pElemBlock->Value( );
	LK_ASSERT ( tEleName == "Npc", return false );

	const char* tpNpcID	= pElemBlock->Attribute( "NpcID" );
	const char*	tpDirection = pElemBlock->Attribute( "Direction" );
	const char* tpInit = pElemBlock->Attribute( "NpcInit" );
	const char* tpDisappear = pElemBlock->Attribute( "Disappear" );
	const char* tpRelife = pElemBlock->Attribute( "Relife" );
	LK_ASSERT( tpNpcID != NULL && tpDirection != NULL, return false );

	TiXmlElement* tpPosEle = pElemBlock->FirstChildElement( );
	for( ; tpPosEle != NULL; tpPosEle = tpPosEle->NextSiblingElement( ) )
	{
		std::string tEleName = tpPosEle->Value( );
		if ( tEleName == "Pos" )
		{
			const char* tpX		= tpPosEle->Attribute( "x" );
			const char* tpY		= tpPosEle->Attribute( "y" );
			LK_ASSERT ( tpX != NULL && tpY != NULL, return false );
			mPosition = CWTPoint( ::atoi( tpX ), ::atoi( tpY ) );
		}
	}

	// Path
	TiXmlElement* tpSubEle = pElemBlock->FirstChildElement( "Path" );
	if( tpSubEle != NULL )
	{
		for( TiXmlElement* tpPath = tpSubEle->FirstChildElement(); tpPath != NULL; tpPath = tpPath->NextSiblingElement( ) )
		{
			std::string tEleName1 = tpPath->Value( );
			if ( tEleName1 == "Point" )
			{
				const char* tpX	= tpPath->Attribute( "x" );
				const char*	tpY	= tpPath->Attribute( "y" );
				LK_ASSERT ( tpX != NULL && tpY != NULL, return false );

				mPathListX[ mPCount ]	= atoi( tpX );
				mPathListY[ mPCount ]	= atoi( tpY );
				mPCount++;
				if ( (unsigned int)mPCount >= ARRAY_CNT( mPathListX ) || (unsigned int)mPCount >= ARRAY_CNT( mPathListY ) )
					break;
			}
		}
	}

	mNpcID = ::atoi( tpNpcID );
	mDirection = (EDirType)::atoi( tpDirection );
	mInit = ( tpInit != NULL ) ? atoi( tpInit ) : 1;
	if ( tpDisappear != NULL )
		mDisappear = ::atoi( tpDisappear );
	if ( tpRelife != NULL )
		mRelife = ::atoi( tpRelife );
	const char* tpCreatorIndex = pElemBlock->Attribute( "NpcIndex" );
	LK_ASSERT( tpCreatorIndex != NULL, return false );

	unsigned int tCreatorIndex = atoi( tpCreatorIndex );
	LK_ASSERT( tCreatorIndex < OGREINDEX, return false );

	SetIndex( tCreatorIndex );

	return true;
}

int CNpcCreator::Initialize() 
{
	COgreCreator::Initialize();

	//memset(  &mPathListX   , 0, sizeof(	  mPathListX	 ) );
	//memset(  &mPathListY   , 0, sizeof(	  mPathListY	 ) );
	//memset(  &mPCount	   , 0, sizeof(	  mPCount		 ) ); 
	return 0;
}

/************************************************************************/
/*  CBoxCreator                                                                     */
/************************************************************************/
bool CBoxCreator::LoadFromConfig(TiXmlElement* pElemBlock)
{
	const char* tpBoxIndex		= pElemBlock->Attribute( "BoxIndex" );	LK_ASSERT( tpBoxIndex != NULL, return false);
	const char* tpInit			= pElemBlock->Attribute( "BoxInit" );	LK_ASSERT( tpInit != NULL, return false );
	const char*	tpDisappear		= pElemBlock->Attribute( "Disappear" );	LK_ASSERT( tpDisappear != NULL, return false );		
	const char* tpRelife		= pElemBlock->Attribute( "Relife" );	LK_ASSERT( tpRelife != NULL, return false );
	const char* tpX				= pElemBlock->Attribute( "PosX" );		LK_ASSERT( tpX != NULL, return false );
	const char* tpY				= pElemBlock->Attribute( "PosY" );		LK_ASSERT( tpY != NULL, return false );
	const char*	tpDirection		= pElemBlock->Attribute( "Dir" );		LK_ASSERT( tpDirection != NULL, return false );
	const char* tpCountNum		= pElemBlock->Attribute( "CountNum" );	LK_ASSERT( tpCountNum != NULL, return false );
	const char* tpCountDelay	= pElemBlock->Attribute( "CountDelay" );LK_ASSERT( tpCountDelay != NULL, return false );
	const char* tpRand			= pElemBlock->Attribute( "Rand" );		LK_ASSERT( tpRand != NULL, return false );

	mInit			= ::atoi( tpInit );
	mDisappear		= ::atoi( tpDisappear );
	mDirection		= (EDirType)::atoi( tpDirection );
	mRelife			= ::atoi( tpRelife );
	mPosition.mX	= ::atoi( tpX );
	mPosition.mY	= ::atoi( tpY );
	mCountNum		= ::atoi( tpCountNum );
	mCountDelay		= ::atoi( tpCountDelay );
	mRefreshRate	= ::atoi( tpRand );

	memset( mRefresh1, 0, sizeof( mRefresh1 ) );
	memset( mRefresh2, 0, sizeof( mRefresh2 ) );
	memset( mRefresh3, 0, sizeof( mRefresh3 ) );
	memset( mRefresh4, 0, sizeof( mRefresh4 ) );
	memset( mRefresh5, 0, sizeof( mRefresh5 ) );
	memset( mRefreshTime, 0, sizeof( mRefreshTime ) );
	mRCount1 = 0;
	mRCount2 = 0;
	mRCount3 = 0;
	mRCount4 = 0;
	mRCount5 = 0;
	mRCount6 = 0;
	mCurrentCount = 0;

	// RefreshDate 
	TiXmlElement* tpSubEle = pElemBlock->FirstChildElement( "RefreshDate" );
	LK_ASSERT ( tpSubEle != NULL, return false );
	for( TiXmlElement* tpRefreshDate = tpSubEle->FirstChildElement() ; tpRefreshDate != NULL; tpRefreshDate = tpRefreshDate->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpRefreshDate->Value( );
		if ( tEleName1 == "Refresh" )
		{
			const char* tpDate	= tpRefreshDate->Attribute( "RefreshDate" );
			LK_ASSERT( tpDate != NULL && strlen(tpDate) >= 12, return false );

			char tYear[5] = {0}, tMonth[3] = {0}, tDate[3] = {0};
			char tHour[3] = {0}, tMinute[3] = {0}, tWeek[2] = {0}, tDay[2] = {0};
			memcpy( tYear,	 tpDate,	  4 );
			memcpy( tMonth,	 tpDate + 4,  2 );
			memcpy( tDate,	 tpDate + 6,  2 );
			memcpy( tHour,	 tpDate + 8,  2 );
			memcpy( tMinute, tpDate + 10, 2 );
			memcpy( tDay,	 tpDate + 12, 1 );
			memcpy( tWeek,	 tpDate + 13, 1 );
			mRefreshTime[ mRCount6 ].mYear		= atoi( tYear );
			mRefreshTime[ mRCount6 ].mMonth		= atoi( tMonth );
			mRefreshTime[ mRCount6 ].mDate		= atoi( tDate );
			mRefreshTime[ mRCount6 ].mHour		= atoi( tHour );
			mRefreshTime[ mRCount6 ].mMinute	= atoi( tMinute );
			mRefreshTime[ mRCount6 ].mWeek		= atoi( tWeek );
			mRefreshTime[ mRCount6 ].mDay		= atoi( tDay );
			mRCount6++;	
		}
	}							

	// DeadRefresh
	tpSubEle = pElemBlock->FirstChildElement( "DeadRefresh" );
	LK_ASSERT ( tpSubEle != NULL, return false );			
	for( TiXmlElement* tpDeadRefresh = tpSubEle->FirstChildElement(); tpDeadRefresh != NULL; tpDeadRefresh = tpDeadRefresh->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpDeadRefresh->Value( );
		if ( tEleName1 == "NewBox" )
		{
			const char* tpID	= tpDeadRefresh->Attribute( "ID" );
			const char*	tpRand	= tpDeadRefresh->Attribute( "Rand" );
			LK_ASSERT ( tpID != NULL && tpRand != NULL, return false );

			mRefresh1[ mRCount1 ].mOgreID	= atoi( tpID );
			mRefresh1[ mRCount1 ].mOgreRand = atoi( tpRand );
			mRCount1++;
		}									
	}							
	
	// OccureRefresh
	tpSubEle = pElemBlock->FirstChildElement( "OccurRefresh" );
	LK_ASSERT ( tpSubEle != NULL, return false );
	for( TiXmlElement* tpOccurRefresh = tpSubEle->FirstChildElement(); tpOccurRefresh != NULL; tpOccurRefresh = tpOccurRefresh->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpOccurRefresh->Value( );
		if ( tEleName1 == "NewBox" )
		{
			const char* tpOldID	= tpOccurRefresh->Attribute( "OldID" );
			const char*	tpTime	= tpOccurRefresh->Attribute( "Time" );
			const char* tpNewID	= tpOccurRefresh->Attribute( "NewID" );
			LK_ASSERT ( tpOldID != NULL && tpTime != NULL && tpOldID != NULL, return false );

			mRefresh2[ mRCount2 ].mOldID	= atoi( tpOldID );
			mRefresh2[ mRCount2 ].mTime		= atoi( tpTime );
			mRefresh2[ mRCount2 ].mNewID	= atoi( tpNewID );
			mRCount2++;
		}									
	}							

	// RandRefresh
	tpSubEle = pElemBlock->FirstChildElement( "RandRefresh" );
	LK_ASSERT ( tpSubEle != NULL, return false );
	for( TiXmlElement* tpRandRefresh = tpSubEle->FirstChildElement(); tpRandRefresh != NULL; tpRandRefresh = tpRandRefresh->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpRandRefresh->Value( );
		if ( tEleName1 == "NewBox" )
		{
			const char* tpIndex	= tpRandRefresh->Attribute( "Index" );
			LK_ASSERT ( tpIndex != NULL, return false );

			mRefresh3[ mRCount3 ]	= atoi( tpIndex );
			mRCount3++;
		}									
	}							

	// CountRefresh
	tpSubEle = pElemBlock->FirstChildElement( "CountRefresh" );
	LK_ASSERT ( tpSubEle != NULL, return false );	
	for( TiXmlElement* tpCountRefresh = tpSubEle->FirstChildElement(); tpCountRefresh != NULL; tpCountRefresh = tpCountRefresh->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpCountRefresh->Value( );
		if ( tEleName1 == "NewBox" )
		{
			const char* tpIndex	= tpCountRefresh->Attribute( "Index" );
			LK_ASSERT( tpIndex != NULL, return false );
			mRefresh4[ mRCount4 ]	= atoi( tpIndex );
			mRCount4++;
		}									
	}
	
	// CommonRefresh
	tpSubEle = pElemBlock->FirstChildElement( "CrossRefresh" );
	LK_ASSERT ( tpSubEle != NULL, return false );
	for( TiXmlElement* tpCrossRefresh = tpSubEle->FirstChildElement(); tpCrossRefresh != NULL; tpCrossRefresh = tpCrossRefresh->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpCrossRefresh->Value( );
		if ( tEleName1 == "NewBox" )
		{
			const char* tpIndex	= tpCrossRefresh->Attribute( "Index" );
			const char*	tpRand	= tpCrossRefresh->Attribute( "Rand" );
			LK_ASSERT ( tpIndex != NULL && tpRand != NULL, return false );

			mRefresh5[ mRCount5 ].mIndex	= atoi( tpIndex );
			mRefresh5[ mRCount5 ].mOgreRand	= atoi( tpRand );
			mRCount5++;
		}		
	}							
	int tID = atoi( tpBoxIndex );
	LK_ASSERT( tID < OGREINDEX, return false );
	SetIndex( tID );

	/*mLineID = vLineID;
	mMapID = vMapID;*/
	mHasOgre = 0;

	return true;
}

/************************************************************************/
/*  CBlockCreator                                                                     */
/************************************************************************/
bool CBlockCreator::LoadFromConfig(TiXmlElement* pElemBlock)
{
	const char* tpBlockIndex		= pElemBlock->Attribute( "BlockIndex" );	LK_ASSERT( tpBlockIndex != NULL, return false);
	const char* tpInit			= pElemBlock->Attribute( "BlockInit" );	LK_ASSERT( tpInit != NULL, return false );
	const char*	tpDisappear		= pElemBlock->Attribute( "Disappear" );	LK_ASSERT( tpDisappear != NULL, return false );		
	const char* tpRelife		= pElemBlock->Attribute( "Relife" );	LK_ASSERT( tpRelife != NULL, return false );
	const char* tpX				= pElemBlock->Attribute( "PosX" );		LK_ASSERT( tpX != NULL, return false );
	const char* tpY				= pElemBlock->Attribute( "PosY" );		LK_ASSERT( tpY != NULL, return false );
	const char*	tpDirection		= pElemBlock->Attribute( "Dir" );		LK_ASSERT( tpDirection != NULL, return false );
	const char* tpCountNum		= pElemBlock->Attribute( "CountNum" );	LK_ASSERT( tpCountNum != NULL, return false );
	const char* tpCountDelay	= pElemBlock->Attribute( "CountDelay" );LK_ASSERT( tpCountDelay != NULL, return false );
	const char* tpRand			= pElemBlock->Attribute( "Rand" );		LK_ASSERT( tpRand != NULL, return false );

	mInit			= ::atoi( tpInit );
	mDisappear		= ::atoi( tpDisappear );
	mDirection		= (EDirType)::atoi( tpDirection );
	mRelife			= ::atoi( tpRelife );
	mPosition.mX	= ::atoi( tpX );
	mPosition.mY	= ::atoi( tpY );
	mCountNum		= ::atoi( tpCountNum );
	mCountDelay		= ::atoi( tpCountDelay );
	mRefreshRate	= ::atoi( tpRand );

	memset( mRefresh1, 0, sizeof( mRefresh1 ) );
	memset( mRefresh2, 0, sizeof( mRefresh2 ) );
	memset( mRefresh3, 0, sizeof( mRefresh3 ) );
	memset( mRefresh4, 0, sizeof( mRefresh4 ) );
	memset( mRefresh5, 0, sizeof( mRefresh5 ) );
	memset( mRefreshTime, 0, sizeof( mRefreshTime ) );
	mRCount1 = 0;
	mRCount2 = 0;
	mRCount3 = 0;
	mRCount4 = 0;
	mRCount5 = 0;
	mRCount6 = 0;
	mCurrentCount = 0;

	// RefreshDate 
	TiXmlElement* tpSubEle = pElemBlock->FirstChildElement( "RefreshDate" );
	LK_ASSERT ( tpSubEle != NULL, return false );
	for( TiXmlElement* tpRefreshDate = tpSubEle->FirstChildElement() ; tpRefreshDate != NULL; tpRefreshDate = tpRefreshDate->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpRefreshDate->Value( );
		if ( tEleName1 == "Refresh" )
		{
			const char* tpDate	= tpRefreshDate->Attribute( "RefreshDate" );
			LK_ASSERT( tpDate != NULL && strlen(tpDate) >= 12, return false );

			char tYear[5] = {0}, tMonth[3] = {0}, tDate[3] = {0};
			char tHour[3] = {0}, tMinute[3] = {0}, tWeek[2] = {0}, tDay[2] = {0};
			memcpy( tYear,	 tpDate,	  4 );
			memcpy( tMonth,	 tpDate + 4,  2 );
			memcpy( tDate,	 tpDate + 6,  2 );
			memcpy( tHour,	 tpDate + 8,  2 );
			memcpy( tMinute, tpDate + 10, 2 );
			memcpy( tDay,	 tpDate + 12, 1 );
			memcpy( tWeek,	 tpDate + 13, 1 );
			mRefreshTime[ mRCount6 ].mYear		= atoi( tYear );
			mRefreshTime[ mRCount6 ].mMonth		= atoi( tMonth );
			mRefreshTime[ mRCount6 ].mDate		= atoi( tDate );
			mRefreshTime[ mRCount6 ].mHour		= atoi( tHour );
			mRefreshTime[ mRCount6 ].mMinute	= atoi( tMinute );
			mRefreshTime[ mRCount6 ].mWeek		= atoi( tWeek );
			mRefreshTime[ mRCount6 ].mDay		= atoi( tDay );
			mRCount6++;	
		}
	}							

	// DeadRefresh
	tpSubEle = pElemBlock->FirstChildElement( "DeadRefresh" );
	LK_ASSERT ( tpSubEle != NULL, return false );			
	for( TiXmlElement* tpDeadRefresh = tpSubEle->FirstChildElement(); tpDeadRefresh != NULL; tpDeadRefresh = tpDeadRefresh->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpDeadRefresh->Value( );
		if ( tEleName1 == "NewBlock" )
		{
			const char* tpID	= tpDeadRefresh->Attribute( "ID" );
			const char*	tpRand	= tpDeadRefresh->Attribute( "Rand" );
			LK_ASSERT ( tpID != NULL && tpRand != NULL, return false );

			mRefresh1[ mRCount1 ].mOgreID	= atoi( tpID );
			mRefresh1[ mRCount1 ].mOgreRand = atoi( tpRand );
			mRCount1++;
		}									
	}							
	
	// OccureRefresh
	tpSubEle = pElemBlock->FirstChildElement( "OccurRefresh" );
	LK_ASSERT ( tpSubEle != NULL, return false );
	for( TiXmlElement* tpOccurRefresh = tpSubEle->FirstChildElement(); tpOccurRefresh != NULL; tpOccurRefresh = tpOccurRefresh->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpOccurRefresh->Value( );
		if ( tEleName1 == "NewBlock" )
		{
			const char* tpOldID	= tpOccurRefresh->Attribute( "OldID" );
			const char*	tpTime	= tpOccurRefresh->Attribute( "Time" );
			const char* tpNewID	= tpOccurRefresh->Attribute( "NewID" );
			LK_ASSERT ( tpOldID != NULL && tpTime != NULL && tpOldID != NULL, return false );

			mRefresh2[ mRCount2 ].mOldID	= atoi( tpOldID );
			mRefresh2[ mRCount2 ].mTime		= atoi( tpTime );
			mRefresh2[ mRCount2 ].mNewID	= atoi( tpNewID );
			mRCount2++;
		}									
	}							

	// RandRefresh
	tpSubEle = pElemBlock->FirstChildElement( "RandRefresh" );
	LK_ASSERT ( tpSubEle != NULL, return false );
	for( TiXmlElement* tpRandRefresh = tpSubEle->FirstChildElement(); tpRandRefresh != NULL; tpRandRefresh = tpRandRefresh->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpRandRefresh->Value( );
		if ( tEleName1 == "NewBlock" )
		{
			const char* tpIndex	= tpRandRefresh->Attribute( "Index" );
			LK_ASSERT ( tpIndex != NULL, return false );

			mRefresh3[ mRCount3 ]	= atoi( tpIndex );
			mRCount3++;
		}									
	}							

	// CountRefresh
	tpSubEle = pElemBlock->FirstChildElement( "CountRefresh" );
	LK_ASSERT ( tpSubEle != NULL, return false );	
	for( TiXmlElement* tpCountRefresh = tpSubEle->FirstChildElement(); tpCountRefresh != NULL; tpCountRefresh = tpCountRefresh->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpCountRefresh->Value( );
		if ( tEleName1 == "NewBlock" )
		{
			const char* tpIndex	= tpCountRefresh->Attribute( "Index" );
			LK_ASSERT( tpIndex != NULL, return false );
			mRefresh4[ mRCount4 ]	= atoi( tpIndex );
			mRCount4++;
		}									
	}
	
	// CommonRefresh
	tpSubEle = pElemBlock->FirstChildElement( "CrossRefresh" );
	LK_ASSERT ( tpSubEle != NULL, return false );
	for( TiXmlElement* tpCrossRefresh = tpSubEle->FirstChildElement(); tpCrossRefresh != NULL; tpCrossRefresh = tpCrossRefresh->NextSiblingElement( ) )
	{
		std::string tEleName1 = tpCrossRefresh->Value( );
		if ( tEleName1 == "NewBlock" )
		{
			const char* tpIndex	= tpCrossRefresh->Attribute( "Index" );
			const char*	tpRand	= tpCrossRefresh->Attribute( "Rand" );
			LK_ASSERT ( tpIndex != NULL && tpRand != NULL, return false );

			mRefresh5[ mRCount5 ].mIndex	= atoi( tpIndex );
			mRefresh5[ mRCount5 ].mOgreRand	= atoi( tpRand );
			mRCount5++;
		}		
	}							
	int tID = atoi( tpBlockIndex );
	LK_ASSERT( tID < OGREINDEX, return false );
	SetIndex( tID );

	/*mLineID = vLineID;
	mMapID = vMapID;*/
	mHasOgre = 0;

	return true;
}

int CPedalCreator::RefreshActive( )
{
	int tRand = 0;
	mCurrentActive = 0;
	// 先按概率随机刷
	for ( int i = 0; i < mRandCount; i++ )
	{
		tRand = RAND(SERVER_PERCENT_INT);
		mRandList[ i ].mActive = false;
		if ( tRand <= mRandList[ i ].mRand )
		{
			mRandList[ i ].mActive = true;
			mCurrentActive++;
		}
		if ( mCurrentActive >= mMaxActive )
		{
			break;
		}
	}	
	// 如果不够要补够
	int tCount = 0;
	while ( mCurrentActive < mMaxActive && tCount < mRandCount )
	{
		int tIndex = RAND(mRandCount);
		if ( mRandList[ tIndex ].mActive == false )
		{
			mRandList[ tIndex ].mActive = true;
			mCurrentActive++;
		}
		tCount++;
	}
	
	return mCurrentActive;
}

int COgreCreator::GetRightTime( struct tm *now )
{
	int tMinDeltaTime = 24 * 60;
	int tRightTime = 0;
	int tNowTime = now->tm_hour * 60 + now->tm_min;
	for ( int j = 0; j < mRCount6; j++ )
	{
		RefreshDate tTime = mRefreshTime[ j ];
		int tConfigTime = tTime.mHour * 60 + tTime.mMinute;
		// 保证初始差值时间大于初始最小间隔时间
		int tFirstDeltaTime = 48 * 60;
		if ( tConfigTime > tNowTime )
			tFirstDeltaTime = tConfigTime - tNowTime;
		int tNextConfigTime = tConfigTime + 24 * 60;
		int tSecondDeltaTime = tNextConfigTime- tNowTime;
		if ( tTime.mDay > 0 )
		{
			if ( now->tm_wday == 0 )
			{
				if ( tTime.mDay == 7 )
				{
					tRightTime = tMinDeltaTime >= tFirstDeltaTime ? tConfigTime : tRightTime;
					tMinDeltaTime = tMinDeltaTime >= tFirstDeltaTime ? tFirstDeltaTime : tMinDeltaTime;					
				}
				else if ( tTime.mDay == 1 )
				{
					tRightTime = tMinDeltaTime >= tSecondDeltaTime ? tNextConfigTime : tRightTime;
					tMinDeltaTime = tMinDeltaTime >= tSecondDeltaTime ? tSecondDeltaTime : tMinDeltaTime;
				}
			}
			else
			{
				if ( tTime.mDay == now->tm_wday )
				{
					tRightTime = tMinDeltaTime >= tFirstDeltaTime ? tConfigTime : tRightTime;
					tMinDeltaTime = tMinDeltaTime >= tFirstDeltaTime ? tFirstDeltaTime : tMinDeltaTime;
				}
				else if ( tTime.mDay == now->tm_wday + 1 )
				{
					tRightTime = tMinDeltaTime >= tSecondDeltaTime ? tNextConfigTime : tRightTime;
					tMinDeltaTime = tMinDeltaTime >= tSecondDeltaTime ? tSecondDeltaTime : tMinDeltaTime;
				}
			}
		}
		else
		{
			tRightTime = tMinDeltaTime >= tFirstDeltaTime ? tConfigTime : tRightTime;
			tMinDeltaTime = tMinDeltaTime >= tFirstDeltaTime ? tFirstDeltaTime : tMinDeltaTime;
			tRightTime = tMinDeltaTime >= tSecondDeltaTime ? tNextConfigTime : tRightTime;
			tMinDeltaTime = tMinDeltaTime >= tSecondDeltaTime ? tSecondDeltaTime : tMinDeltaTime;
		}
	}
	return tRightTime;
}

int CGhostCreator::Initialize()
{
	memset(  &mPos 	   , 0, sizeof(   mPos 		 ) ); 
	memset(  &mDirection 	   , 0, sizeof(   mDirection 		 ) ); 
	mNpcID = 0;

	return SUCCESS;
}
