#pragma once
#include "entity.h"
#include "protype.h"
#include "servertool.h"


// 实体作用结果集合
class CEntityResultList
{
	struct CEntityResult
	{
		unsigned int mResultCount;
		CFuncResult mResultList[ FUNC_LIMIT ];
		CEntity* mEntity;
	};

protected:

	CEntityResult mEntityResult[ 2000 ];
	unsigned int mEntityCount;
	unsigned int mCurIndex;

	int mListID;

public:
	CEntityResultList() 
	{
		mEntityCount = 0;
		mCurIndex = 0;
		mListID = 0;
	}
	~CEntityResultList() {}
public:

	bool push_back( unsigned int vSrcEntityID, CEntity* pEntity, unsigned short vFunc, unsigned short vResult, bool vBroadCast = true, unsigned int vValue1 = 0, unsigned int vValue2 = 0, unsigned int vValue3 = 0, unsigned int vValue4 = 0, unsigned int vValue5 = 0 )
	{
		LK_ASSERT( pEntity != NULL && "Parames Error, pEntity Is NULL", return false );

		CEntityResult* tpEntityResult = NULL;
		if( mCurIndex >= ARRAY_CNT( mEntityResult ) )
		{
			LOG_ERROR( "pro", "[ %s : %d ][ %s ] EntityResultList push_back mCurIndex more than Max EntityCount",
				__LK_FILE__, __LINE__, __FUNCTION__ );
			return false;
		} 	

		if( GetEntityCount( ) > 0 && mEntityResult[ mCurIndex ].mEntity == pEntity )
		{
			tpEntityResult = &mEntityResult[ mCurIndex ];
		}
		else
		{
			for( unsigned int i = 0; i < GetEntityCount( ); i++ )
			{
				if( mEntityResult[ i ].mEntity == pEntity )
				{
					tpEntityResult = &mEntityResult[ i ];
					mCurIndex = i;
					break;
				}
			}
		}

		if( tpEntityResult == NULL )
		{
			if( GetEntityCount( ) < ARRAY_CNT( mEntityResult ) )
			{ 
				tpEntityResult = &mEntityResult[ mEntityCount ];
				tpEntityResult->mEntity = pEntity;
				tpEntityResult->mResultCount = 0;
				mCurIndex = mEntityCount;
				mEntityCount++;
			}
			else
			{
				LOG_ERROR( "pro", "[ %s : %d ][ %s ]EntityResult Is Full, overload Entity( %d )",
					__LK_FILE__, __LINE__, __FUNCTION__, pEntity->GetEntityID( ) );
				return false;
			}
		}
		else if( tpEntityResult->mResultCount >= ARRAY_CNT( tpEntityResult->mResultList ) )
		{
			LOG_ERROR( "pro", "[ %s : %d ][ %s ]EntityResultList Is Full, overload Entity( %d )",
				__LK_FILE__, __LINE__, __FUNCTION__, pEntity->GetEntityID( ) );
			return false;
		}

		tpEntityResult->mResultList[ tpEntityResult->mResultCount ].mEntityID = pEntity->GetEntityID();
		tpEntityResult->mResultList[ tpEntityResult->mResultCount ].mResult = vResult;
		tpEntityResult->mResultList[ tpEntityResult->mResultCount ].mValue1 = vValue1;
		tpEntityResult->mResultList[ tpEntityResult->mResultCount ].mValue2 = vValue2;
		tpEntityResult->mResultList[ tpEntityResult->mResultCount ].mValue3 = vValue3;
		tpEntityResult->mResultList[ tpEntityResult->mResultCount ].mValue4 = vValue4;
		tpEntityResult->mResultList[ tpEntityResult->mResultCount ].mValue5 = vValue5;
		tpEntityResult->mResultList[ tpEntityResult->mResultCount ].mListID = mListID;
		tpEntityResult->mResultList[ tpEntityResult->mResultCount ].mFunc = vFunc;
		tpEntityResult->mResultList[ tpEntityResult->mResultCount ].mSrcEntityID = vSrcEntityID;
		tpEntityResult->mResultList[ tpEntityResult->mResultCount ].mBroadCast = vBroadCast;
		tpEntityResult->mResultCount++;

		return true;
	}


	inline unsigned int GetEntityCount( ) { return mEntityCount; }	
	inline unsigned int GetResultCount( unsigned int vIndex )
	{
		if( vIndex >= GetEntityCount( ) )
		{
			return 0;
		}

		return mEntityResult[ vIndex ].mResultCount;
	}

	inline CFuncResult* GetFuncResult( unsigned int vIndex, unsigned int vResultIndex )
	{
		if( vIndex >= GetEntityCount( ) )
		{
			return NULL;
		}

		if( vResultIndex >= GetResultCount( vIndex ) )
		{
			return NULL;
		}

		return &( mEntityResult[ vIndex ].mResultList[ vResultIndex ] );
	}

	inline CEntity* GetEntityByIndex( unsigned int vIndex )
	{
		if( vIndex >= GetEntityCount( ) )
		{
			return NULL;
		}

		return mEntityResult[ vIndex ].mEntity;
	}	

	inline void Initialize( )
	{
		mEntityCount = 0;
		mEntityResult[ 0 ].mEntity = NULL;
		mCurIndex = 0;
		//mListID = 0;
	}

	inline void SetListID( int nListID ) { mListID = nListID; }
	inline int GetListID() { return mListID; }
};



