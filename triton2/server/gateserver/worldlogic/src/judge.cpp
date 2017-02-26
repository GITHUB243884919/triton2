// CJudge
// Include Head Files
#include "judgemodule.h"
#include "judgemessage_pb.hxx.pb.h"

#include "base.h"
#include "entity.h" 

CJudge::CJudge()
{
	mCharID = 0;
	mName[0] = '\0';
	mTotalScore = JUDGE_SCORE_MAX;
	mValidScore = JUDGE_SCORE_MAX;
	mRecTotalScore = 0;
	mJudgeList.initailize();
}

CJudge::~CJudge()
{
	mJudgeList.clear();
}

int CJudge::AttachJudge( int nCharID, int nValue, int nTime, const char* npName /* = NULL */ )
{
	if( GetJudgeCnt() >= MAX_JUDGE_OTHER )
		return ERR_JUDGE_NUMFULL;

	int tChangeValue = 0;
	bool bRecordExist = false;
	JudgeList::iterator it = mJudgeList.begin();
	for ( ; it != mJudgeList.end(); ++ it )
	{
		if ( nCharID == it->mCharID )
		{
			bRecordExist = true;
			tChangeValue = nValue - it->mValue;

			if ( nValue == 0 )
			{
				mJudgeList.erase( it );
				break;
			}
			else
			{				
				it->mValue = nValue;
				it->mTimeStamp = nTime;
				break;
			}
		}
	}

	int tRet = 0;

	// if not exist, insert a new record here
	if ( !bRecordExist )
	{
		if( npName == NULL )
			return -1;
		JudgeRecord tRecord( nCharID, nValue, nTime, npName );
		mJudgeList.push_back( tRecord );
		tChangeValue = nValue;
	}
	else
		tRet = RECORD_EXIST;

	mValidScore -= tChangeValue;
	mValidScore = std::max( 0, mValidScore );
	mValidScore = std::min( mTotalScore, mValidScore );
	return tRet;
}

int CJudge::DeleteJudge( int nCharID )
{
	JudgeList::iterator it = mJudgeList.begin();
	for ( ; it != mJudgeList.end(); ++ it )
	{
		if ( nCharID == it->mCharID )
		{
			mValidScore += it->mValue;
			mJudgeList.erase( it );
			break;
		}
	}
	return 0;
}

bool CJudge::RecordExist( int nCharID )
{
	JudgeList::iterator it = mJudgeList.begin();
	for ( ; it != mJudgeList.end(); ++ it )
	{
		if ( nCharID == it->mCharID )
		{
			return true;
		}
	}
	return false;
}

// 获取对指定玩家得评价值 [1/27/2010 Macro]
int CJudge::GetJudgeValue( int nCharID )
{
	JudgeList::iterator it = mJudgeList.begin();
	for ( ; it != mJudgeList.end(); ++ it )
	{
		if ( nCharID == it->mCharID )
		{
			return it->mValue;
		}
	}
	return 0;
}


