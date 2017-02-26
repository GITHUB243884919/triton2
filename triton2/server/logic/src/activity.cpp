#include "westserver.h"
#include "activity.h"
#include "activitytype.h"
#include "protype.h"
#include "scenelogic.h"


// ***********************************************************
//  Function:		CQuestionManager
//  Description:	初始化题库管理器
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************

CQuestionManager::CQuestionManager()
{	 
	if ( CObj::msCreateMode )
	{
		Initialize();
	}
	else
	{
		Resume();
	}
}

// ***********************************************************
//  Function:		Initialize
//  Description:	初始化题库管理器
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
int CQuestionManager::Initialize()
{  
	memset( &mNpcLib, 0 , sizeof(mNpcLib) );
	memset( &mPaperLib,0, sizeof(mPaperLib) );
	memset( &mQuestion,0, sizeof(mQuestion) );	
	mQuestionNum = 0;			
	mCurrentLibID = -1;	
	mNpcPrizeList.initailize();		
	return 0;
}

// ***********************************************************
//  Function:		Resume
//  Description:	热启动
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************
int CQuestionManager::Resume()
{
	return 0;	
}

// ***********************************************************
//  Function:		AddNewLib
//  Description:	添加新题库
//  Input:			int LibID				题库ID
//  Input:			const char *pAppType	题库类型
//  Return:				
//  Others:
//  Date:		
// **********************************************************
int CQuestionManager::AddNewLib( int LibID, const char *pAppType )
{	
	
	if ( LibID < 0 || pAppType == NULL )
	{
		return -1;
	}
	
	bool HasSet[MAX_APPTYPE_NUM] = { false };
	int TypeLength = 0;	
	
	// 获取题库ID
	mCurrentLibID	= LibID;	
	
	// 获取题库的应用类型
	while( *pAppType != '\0' && TypeLength < MAX_APPTYPE_NUM )
	{
		switch( *pAppType )
		{
			case 'A':
			case 'a':
			{
				if ( mNpcLib.QuestionLibNum >= MAX_QUESTIONLIB_NUM )
				{
					return -1;
				}
				if ( HasSet[0] == true )
				{
					break;
				}			
				++mNpcLib.QuestionLibNum;				
				mNpcLib.QuestionLib[mNpcLib.QuestionLibNum-1].LibID				 = LibID;
				mNpcLib.QuestionLib[mNpcLib.QuestionLibNum-1].QuestionNum		 = 0;
				mNpcLib.QuestionLib[mNpcLib.QuestionLibNum-1].FirstQuestionIndex = mQuestionNum;
				HasSet[0] = true;					
			}
			break;
			
			case 'B':
			case 'b':
			{
			
				if ( mPaperLib.QuestionLibNum >= MAX_QUESTIONLIB_NUM )
				{
					return -1;
				}
				if ( HasSet[1] == true )
				{
					break;
				}
				++mPaperLib.QuestionLibNum;
				mPaperLib.QuestionLib[mPaperLib.QuestionLibNum-1].LibID			   = LibID;				
				mPaperLib.QuestionLib[mPaperLib.QuestionLibNum-1].QuestionNum		   = 0;
				mPaperLib.QuestionLib[mPaperLib.QuestionLibNum-1].FirstQuestionIndex = mQuestionNum;
				HasSet[1] = true;	
			}
			break;
			default:
			break;			
		}	
		++pAppType;
		++TypeLength;	
	}
	return 1;
}
// ***********************************************************
//  Function:		AddQuestion
//  Description:	添加题库中的题目
//  Input:			const char *QuestionID 题目ID
//  Input:			const char *Answer	   答案
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************

int CQuestionManager::AddQuestion( const char *QuestionID, const char *Answer )
{
	if ( QuestionID == NULL || Answer == NULL )
	{
		return -1;
	}
	
	if ( mQuestionNum >= TOTAL_QUESTION_NUM )
	{
		return -1;
	}
	int QuestID		= atoi( QuestionID );	
	mQuestion[mQuestionNum].QuestionID = QuestID;	
	int AnswerLen =  0;
	
	if (  mNpcLib.QuestionLibNum > 0 && mNpcLib.QuestionLib[mNpcLib.QuestionLibNum - 1].LibID == mCurrentLibID )
	{
		 ++mNpcLib.QuestionLib[mNpcLib.QuestionLibNum - 1].QuestionNum ;
	}
	
	if ( mPaperLib.QuestionLibNum>0 && mPaperLib.QuestionLib[mPaperLib.QuestionLibNum - 1].LibID == mCurrentLibID)
	{	
		++mPaperLib.QuestionLib [mPaperLib.QuestionLibNum - 1].QuestionNum;
	}
	
	while ( AnswerLen < MAX_RULE_NUM && *Answer != '\0' )
	{
		switch( *Answer )		
		{
			case 'a':
			case 'A':
			{
				mQuestion[mQuestionNum].QuestionAnswer |= OPTION_A;
				break;
			}
			
			case 'b':
			case 'B':
			{
				mQuestion[mQuestionNum].QuestionAnswer |= OPTION_B;
				break;
			}
			
			case 'c':
			case 'C':
			{
				mQuestion[mQuestionNum].QuestionAnswer |= OPTION_C;
				break;
			}
			
			case 'd':
			case 'D':
			{
				mQuestion[mQuestionNum].QuestionAnswer |= OPTION_D;
				break;
			}
			
			case 'e':
			case 'E':
			{
				mQuestion[mQuestionNum].QuestionAnswer |= OPTION_E;
				break;
			}
			default:
			break;
		}
		++Answer;
		++AnswerLen;
	}
	++mQuestionNum;	
	return 0;
}

// ***********************************************************
//  Function:		CreatQuestion
//  Description:	产生指定数量的题目
//  Input:			int Apptype			   应用类型
//  Input:			int QuestionNum		   题目数量
//  Input:			int *Question		   保存题目的数组
//  Input:			int *Answer			   保存答案的数组
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// **********************************************************

int CQuestionManager::CreatQuestion( int Apptype, int QuestionNum,int *Question, int *Answer )
{	
	int tRet = -1;
	if( Apptype == APPTYPE_NPC )
	{
		tRet = GetQuestFromLib( &mNpcLib, QuestionNum, Question, Answer );
	}
	if ( Apptype == APPTYPE_QUESTIONPAPER )
	{
		tRet = GetQuestFromLib( &mPaperLib, QuestionNum, Question, Answer );
	}
	return tRet;
}


// ***********************************************************
//  Function:		GetQuestFromLib
//  Description:	产生指定数量的题目
//  Input:			CAppType *pAppType	   应用类型
//  Input:			int QuestionNum		   题目数量
//  Input:			int *Question		   保存题目的数组
//  Input:			int *Answer			   保存答案的数组
//  OutPut:			
//  Return:			int	 Num			   实际返回的题目个数，如果为-1表示错误		
//  Others:
//  Date:		
// **********************************************************

int CQuestionManager::GetQuestFromLib( CAppType *pAppType,int QuestionNum,int *Question, int *Answer )
{	

	int LibIndex[MAX_CREATED_LIBNUM] = { 0 };
	int tLibNum = CreatLibID( pAppType, QuestionNum, LibIndex );
	if( tLibNum == -1 )
	{
		return -1;
	}
		
	int QuestNum = 0;	
	int QuestIndex = 0;
	int LibPos	   = 0;
	for ( int i = 0 ; i < tLibNum; i++ )
	{
		LibPos = LibIndex[i];
		QuestIndex = pAppType->QuestionLib[LibPos].FirstQuestionIndex;
		for ( int k =0 ; k < pAppType->QuestionLib[LibPos].QuestionNum ; k ++  )
		{						
			Question[QuestNum] = mQuestion[ QuestIndex+k ].QuestionID;
			Answer[QuestNum]   = mQuestion[ QuestIndex+k ].QuestionAnswer;
			++QuestNum;		
			if ( QuestNum >= QuestionNum )
			{
				return QuestNum;
			}
		}
	}
	return QuestNum;
}

// ***********************************************************
//  Function:		InsertNpcPrize
//  Description:	插入npc奖励
//  Input:			
//  Input:			
//  Input:			
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:			01-07-2009
// **********************************************************
int CQuestionManager::InsertNpcPrize( CItemPrize &rNpcPrize )
{  
	mNpcPrizeList.push_back( rNpcPrize );
	return SUCCESS;
}

// ***********************************************************
//  Function:		GetNpcPrize
//  Description:	获取npc福利答题的物品奖励信息
//  Input:			
//  Input:			
//  Input:			
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		   01-07-2009
// **********************************************************
int CQuestionManager::GetNpcPrize( CItemPrize *pPrizeList, int &rNum )
{  
	LK_ASSERT( pPrizeList != NULL, return  -1 )	
	NpcPrizeList::iterator it = mNpcPrizeList.begin();
	int tNum = 0;
	for ( ; it != mNpcPrizeList.end(); ++it )
	{
		if ( tNum >= rNum )
		{
			rNum = tNum;
			return SUCCESS;
		}
		pPrizeList[tNum].ItemPrizeID = (*it).ItemPrizeID;
		pPrizeList[tNum].ItemPrizeNum = (*it).ItemPrizeNum;
		++tNum;
	}
	rNum = tNum;
	return SUCCESS;
}
// ***********************************************************
//  Function:		CreatLibID
//  Description:	按要求产生特定数目
//  Input:			CAppType *pAppType	   应用类型
//  Input:			int QuestionNum		   题目数量
//  OutPut:			
//  Return:			int	 Num			   实际返回的题库个数，如果为-1表示错误		
//  Others:
//  Date:		
// **********************************************************
int CQuestionManager::CreatLibID( CAppType *pAppType, int vQuestionNum, int *vLibIndex )
{
	if ( pAppType == NULL )
	{
		return -1;
	}	
	if ( pAppType->QuestionLibNum <= 0 )
	{
		return -1;
	}	
	int RandNumInstead[MAX_CREATED_LIBNUM]  = { -1 };			// 已经产生的随机数的替代数 	
	int LibIndex = 0;	
	int QuestNum = 0;
	int RandNum = 0;
		
	// 产生题库
	while ( QuestNum < vQuestionNum )
	{	
		// 产生一个不重复的随机数,换掉重复的随机数
		RandNum	= rand( ) % ( pAppType->QuestionLibNum - LibIndex );					
		for ( int i = 0; i < LibIndex ; i++  )
		{		
			if ( LibIndex == 0 )
			{
				break;
			}
			if ( RandNum == vLibIndex[i] )
			{
				RandNum				= RandNumInstead[i];
				RandNumInstead[i]	= pAppType->QuestionLibNum - LibIndex -1;
				break;
			}					
		}		
			
		// 记录产生的随机数 产生重复时的替换数
		vLibIndex[LibIndex] = RandNum;		
		RandNumInstead[LibIndex]  = pAppType->QuestionLibNum - LibIndex -1;					
		QuestNum += pAppType->QuestionLib[RandNum].QuestionNum;	
		
		// 已经没有题库了
		LibIndex ++;		
				
		// 产生的题目数量已经达到要求的数值			
		if ( QuestNum >= vQuestionNum )
		{
			break;
		}		
		
		if ( pAppType->QuestionLibNum - LibIndex <= 0 )
		{
			break;						
		}				
	}
	return LibIndex;
}


/******************************************************/
// CAnswerStatus
/******************************************************/
int CAnswerStatus::SetQuestion( int QuestNum, int *Question, int *Answer )
{
	for ( int i= 0; i < QuestNum; ++i )
	{
		mQuestionIDSet[i]  = Question[i];
		mAnswerSet[i]	   = Answer[i];
	}
	mQuestionNum = QuestNum;
	return 1;
}

void CAnswerStatus::ClearStatus( )
{	
	mAppType			= 0;
	mQuestionNum		= 0;
	mCurrentQuestIndex	= -1;
	mCountAnswered		= 0;	
	mRightAnswerNum		= 0;
}


// ***********************************************************
//  Function:		InitialQuestionFile
//  Description:	检查答案是否正确
//  Input:			int QuestID      题目ID
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// *********************************************************

int CAnswerStatus::CheckAnswer( int QuestionID,int QuestAnswer )
{
	if ( mQuestionIDSet[mCurrentQuestIndex] != QuestionID  )
	{
		// error 
		return -1;
	}
	if( mAnswerSet[mCurrentQuestIndex] != QuestAnswer )
	{
		++mCountAnswered;
		return 0;
	}
	
	// TODO : 检查答错的次数
	++mRightAnswerNum;
	return 1;
}

// ***********************************************************
//  Function:		CheckCountAnswered
//  Description:	检查已经打错的次数(打错之后进行检查)
//  Input:			
//  OutPut:			int	 1表示已经超过允许的次数	0 表示没有超过允许的次数 -1表示错误
//  Return:			
//  Others:
//  Date:		
// *********************************************************
int CAnswerStatus::CheckCountAnswered( int QuestionID  )
{
	if ( mAppType == APPTYPE_NULL )
	{
		return -1;
	}
	if ( mAppType == APPTYPE_NPC )
	{
		// TODO: 检查是否已经超过允许打错的次数
		return 0;		
	}
	if ( mAppType == APPTYPE_QUESTIONPAPER )
	{
		// TODO: 检查是否已经超过允许打错的次数
		return 0;
	}
	if ( mAppType == APPTYPE_ACTIVITY )
	{
		// TODO: 检查是否已经超过允许打错的次数
		return 0; 
	}
	return -1;
}


// ***********************************************************
//  Function:		GetNewQuestID
//  Description:	获得一道新的题目
//  Input:			int QuestID      题目ID
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// *********************************************************

int CAnswerStatus::GetNewQuestID( )
{	
	++mCurrentQuestIndex;	
		
	// 答题结束
	if ( mCurrentQuestIndex >= mQuestionNum )
	{	
		return -1;
	}
	
	// 随机题目顺序
	int Random = rand() % ( mQuestionNum - mCurrentQuestIndex );	
	if( Random > 0 )
	{
		int SwapValue = mQuestionIDSet[mCurrentQuestIndex];
		mQuestionIDSet[mCurrentQuestIndex] = mQuestionIDSet[mCurrentQuestIndex+Random];
		mQuestionIDSet[mCurrentQuestIndex+Random] = SwapValue;
		
		SwapValue = mAnswerSet[mCurrentQuestIndex];
		mAnswerSet[mCurrentQuestIndex]  = mAnswerSet[mCurrentQuestIndex+Random];
		mAnswerSet[mCurrentQuestIndex+Random] = SwapValue;
	}		
	
	mCountAnswered  = 0;	
	return mQuestionIDSet[mCurrentQuestIndex];			
}


// ***********************************************************
//  Function:		CheckIfQuestionInvalidation
//  Description:	检查是本道题是否已经作废
//  Input:			int CheckType	答题次数，每题时间，或者总共时间
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// *********************************************************
int CAnswerStatus::CheckIfQuestionInvalidation( int CheckType )
{
	return 1;
}


// ***********************************************************
//  Function:		EndAnswerQuesetion
//  Description:	结束答题
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		
// *********************************************************
int CAnswerStatus::EndAnswerQuesetion(  )
{
	ClearStatus();
	mAppType = APPTYPE_NULL;
	return 1;
}		


// ***********************************************************
//  Function:		CInvestQuestionLib
//  Description:	调查问卷题库的构造函数
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 05-11-2009
// *********************************************************
CInvestQuestionLib::CInvestQuestionLib()
{
	if ( CObj::msCreateMode )
	{
		Initialize();
	}
	else
	{
		Resume();
	}
}

// ***********************************************************
//  Function:		InsertQuestion
//  Description:	插入一道题目
//  Input:			int IfSelected 是否必选题目
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 05-11-2009
// *********************************************************
int CInvestQuestionLib::InsertQuestion( int QuetionID, int IfSelected )
{
	if ( m_nQuestionNum >= MAX_QUESTIONNUM_EACHLIB )
	{
		return -1;
	}
	if ( IfSelected != 1 )
	{	 
		m_sQuestionList[m_nQuestionNum] = QuetionID;
		++m_nQuestionNum;
		return 0;	
	}
	
	// 把必选题目放到最前面	  	
	for ( int i = 0; i < m_nQuestionNum ; ++i )
	{
		m_sQuestionList[m_nQuestionNum-i] = m_sQuestionList[m_nQuestionNum-i-1];
	}
	m_sQuestionList[0]=QuetionID;
	++m_nQuestionNum;
	++m_nSelectedQuestionNum;	
	return 0;
}

// ***********************************************************
//  Function:		CreatInvestQuestionID
//  Description:	产生问卷调查的题目，数量不会超过 m_nMaxQuestionNum
//  Input:			
//  OutPut:			
//  Return:			实际产生的题目数量
//  Others:
//  Date:		 05-11-2009
// *********************************************************
int CInvestQuestionLib::CreatInvestQuestionID( )
{  
	// 无需排序
	if ( m_nMaxQuestionNum >= m_nQuestionNum )
	{
		return	m_nQuestionNum;
	}
	
	// 全是必选题目，无需排序 
	if ( m_nSelectedQuestionNum >= m_nMaxQuestionNum )
	{
		return	m_nMaxQuestionNum;
	}
	
	int tRandNum = m_nMaxQuestionNum -	 m_nSelectedQuestionNum;     // 需要随机产生的题目数量
	for ( int i = 0 ; i < tRandNum; i++ )
	{
		int tRand = rand()%( m_nQuestionNum - m_nSelectedQuestionNum - i );
		if( tRand == 0 )
		{
			continue;
		}
		int tQuestionID =   m_sQuestionList[m_nSelectedQuestionNum + i + tRand];
		m_sQuestionList[m_nSelectedQuestionNum + i + tRand] = m_sQuestionList[m_nSelectedQuestionNum+i];
		m_sQuestionList[m_nSelectedQuestionNum] = tQuestionID;
	}	
	return 	m_nMaxQuestionNum;							  				
}

// ***********************************************************
//  Function:		RecordAnswer
//  Description:	记录题目答案
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 04-21-2009
// *********************************************************
int CInvestQuestionLib::RecordAnswer( int nQuestionID, int nAnswerOption )
{
	if ( HasQuestion( nQuestionID ) == false )
	{
		return -1;
	}
	int tQuestionIndex = 0;
	for ( ; tQuestionIndex < (int)ARRAY_CNT( m_nAnswerList ); ++tQuestionIndex )
	{	
		if ( m_nAnswerList[tQuestionIndex].mQuestionID == nQuestionID )
		{
			break;
		}
				
		if ( m_nAnswerList[tQuestionIndex].mQuestionID == 0 )
		{
			m_nAnswerList[tQuestionIndex].mQuestionID = nQuestionID;
			break;
		}
	}
	for ( int i = 0; i < (int)ARRAY_CNT(m_nAnswerList[tQuestionIndex].mAnswerCount); ++i  )
	{	   
		if ( nAnswerOption & ( 1 << i ) )
		{
			++m_nAnswerList[tQuestionIndex].mAnswerCount[i];
		}
	}
	return SUCCESS;
}


// ***********************************************************
//  Function:		HasQuestion
//  Description:	是否有某道题目
//  Date:		 04-21-2009
// *********************************************************
bool CInvestQuestionLib::HasQuestion( int nQuestionID )
{ 
	for ( int i = 0;i < (int)ARRAY_CNT( m_sQuestionList ); ++i )
	{
		if ( nQuestionID == m_sQuestionList[i] )
		{
			return true;
		}
	}
	return false;		
}

// ***********************************************************
//  Function:		SetOptionCount
//  Description:	设置某道题目某个选项的数量
//  Date:		 04-21-2009
// *********************************************************
void CInvestQuestionLib::SetOptionCount( int nQuestionID, int nOptionIndex, int nCount )
{  
	if ( HasQuestion( nQuestionID ) != true )
	{
		return;
	}
	
	int tQuestionIndex = 0;
	for ( ; tQuestionIndex < (int)ARRAY_CNT( m_nAnswerList ); ++tQuestionIndex )
	{
		if ( m_nAnswerList[tQuestionIndex].mQuestionID == nQuestionID )
		{
			break;
		}
		if ( m_nAnswerList[tQuestionIndex].mQuestionID == 0 )
		{
			m_nAnswerList[tQuestionIndex].mQuestionID = nQuestionID;
			break;
		}  		 		
	}		
	
	if ( nCount < 0 || nCount >= (int)ARRAY_CNT( m_nAnswerList[tQuestionIndex].mAnswerCount ) )
	{
		return;
	}
	m_nAnswerList[tQuestionIndex].mAnswerCount[nOptionIndex]  = nCount;
}

// ***********************************************************
//  Function:		GetQuestion
//  Description:	获得指定位置的题目
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 05-11-2009
// *********************************************************
int CInvestQuestionLib::GetQuestion( int QuestionIndex )
{
	if ( QuestionIndex < 0 || QuestionIndex >= MAX_QUESTIONNUM_EACHLIB )
	{
		return -1;
	}
	return m_sQuestionList[QuestionIndex];
}

// ***********************************************************
//  Function:		InsertItemPrize
//  Description:	插入一个奖励物品
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 05-11-2009
// *********************************************************
int CInvestQuestionLib::InsertItemPrize( int ItemID, int ItemNum )
{
	if ( m_nItemPrizeNum >= MAX_INVEST_PRIZENUM || m_nItemPrizeNum < 0 )
	{
		return -1;
	}
	m_nItemPrizeList[m_nItemPrizeNum].ItemPrizeID = ItemID;
	m_nItemPrizeList[m_nItemPrizeNum].ItemPrizeNum = ItemNum;
	++m_nItemPrizeNum;
	return 0;
}

// ***********************************************************
//  Function:		GetItemPrize
//  Description:	获取指定位置的奖励物品
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 05-11-2009
// *********************************************************
int CInvestQuestionLib::GetItemPrize( int Index, CItemPrize &vItemPrize )
{	 
	if ( Index >= m_nItemPrizeNum || Index < 0 )
	{
		return -1;
	}
	vItemPrize.ItemPrizeID = m_nItemPrizeList[Index].ItemPrizeID;
	vItemPrize.ItemPrizeNum = m_nItemPrizeList[Index].ItemPrizeNum;	
	return 0;		
}

// ***********************************************************
//  Function:		Initialize
//  Description:	调查问卷题库的初始话函数
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 05-11-2009
// *********************************************************
int CInvestQuestionLib::Initialize()
{
	m_nLibID = 0;
	memset( m_sQuestionList, 0, sizeof( m_sQuestionList ) );
	m_nQuestionNum = 0;
	m_nExpPrize = 0;
	memset( m_nItemPrizeList, 0, sizeof( m_nItemPrizeList ) );
	m_nItemPrizeNum = 0;
	m_nMaxQuestionNum = 0;
	m_nSelectedQuestionNum = 0; 
	m_nMoney = 0;
	m_nBindMoney = 0;
	m_nCowry = 0;
	memset( m_nAnswerList, 0, sizeof( m_nAnswerList ) );	
	return 0;
}  

// ***********************************************************
//  Function:		Resume
//  Description:	
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 05-11-2009
// *********************************************************
int CInvestQuestionLib::Resume()
{
	 return 0;
}

// ***********************************************************
//  Function:		CInvestQuestionManage
//  Description:	调查问卷题库管理类的构造函数
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 05-11-2009
// *********************************************************
CInvestQuestionManage::CInvestQuestionManage()
{	 
	if ( CObj::msCreateMode )
	{
		Initialize();
	}
	else
	{
		Resume();	
	}		
}

// ***********************************************************
//  Function:		Initialize
//  Description:	调查问卷题库管理类的初始化函数
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 05-11-2009
// *********************************************************
int CInvestQuestionManage::Initialize()
{
	m_tInvestLibList.initailize();
	m_bInvestOutput = false;
	return 0;
}

// ***********************************************************
//  Function:		Resume
//  Description:	调查问卷题库管理类的初始化函数
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 05-11-2009
// *********************************************************
int CInvestQuestionManage::Resume()
{
	return 0;
}	

// ***********************************************************
//  Function:		InsertInvestQuestLib
//  Description:	添加一个调查问卷的题库
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 05-11-2009
// *********************************************************
int CInvestQuestionManage::InsertInvestQuestLib( CInvestQuestionLib *pQuestionLib )
{	
	if( m_tInvestLibList.size() >= MAX_INVEST_QUESTLIBNUM )	
	{
		return -1;
	}			  	
	m_tInvestLibList.insert( INVESTLIBLIST::value_type( pQuestionLib->GetLibID(), *pQuestionLib ) );	 
	return 0;		
}

// ***********************************************************
//  Function:		RecordLibAnswer
//  Description:	记录题目答案
//  Date:		 04-21-2009
// *********************************************************
int CInvestQuestionManage::RecordLibAnswer( int nLibID, int nQuestionID, int nOption )
{
	CInvestQuestionLib *tpQuestionLib = GetInvestQuestLib( nLibID );
	if ( tpQuestionLib != NULL )
	{
		tpQuestionLib->RecordAnswer( nQuestionID, nOption );
	}
	return SUCCESS;
}

// ***********************************************************
//  Function:		ClearLibAnswer
//  Description:	清除某个题库的答案
//  Date:		 04-21-2009
// *********************************************************	
int CInvestQuestionManage::ClearLibAnswer( int nLibID )
{
	CInvestQuestionLib *tpQuestionLib = GetInvestQuestLib( nLibID );
	if ( tpQuestionLib != NULL )
	{
		tpQuestionLib->ClearAnswer( );
	}
	return SUCCESS;
}

// ***********************************************************
//  Function:		ClearAllLibAnswer
//  Description:	清除所有题目的答案
//  Date:		 04-21-2009
// *********************************************************	
int CInvestQuestionManage::ClearAllLibAnswer()
{
	INVESTLIBLIST::iterator it = m_tInvestLibList.begin();
	for ( ; it != m_tInvestLibList.end(); ++it )
	{
		(it->second).ClearAnswer( );				
	}
	return SUCCESS;
}

// ***********************************************************
//  Function:		GetInvestQuestLib
//  Description:	通过题库ID获取一个调查问卷的题库
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 05-11-2009
// *********************************************************
CInvestQuestionLib * CInvestQuestionManage::GetInvestQuestLib( int vLibID )
{  
	INVESTLIBLIST::iterator it = m_tInvestLibList.find( vLibID );
	if ( it == m_tInvestLibList.end() )
	{
		return NULL;
	}
	return  (CInvestQuestionLib *)&it->second;	
}	 

// ***********************************************************
//  Function:		InsertNpcPath
//  Description:	插入一个npc的路径信息
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 03-30-2010
// *********************************************************
int CHydrangeaNpcManager::InsertNpcPath( CHydrangeaNpcPath &rNpcPath, int nActivityID )
{
	CHydrangeaNpcInfo *tpHydrangeaInfo = GetHydrangeaNpcInfo( nActivityID );
	if ( tpHydrangeaInfo == NULL )
	{	
		for ( int i = 0; i <(int) ARRAY_CNT( mNpcInfo ); ++i )
		{
			if ( mNpcInfo[i].mActivityID == 0 )
			{
				tpHydrangeaInfo = &mNpcInfo[i];
				mNpcInfo[i].mActivityID = nActivityID;
				break;
			}
		}
	}	

	if ( tpHydrangeaInfo == NULL )
	{
		return -1;
	}
	for ( int i = 0; i < (int)ARRAY_CNT( tpHydrangeaInfo->mNpcPath ); ++i )
	{
		if ( tpHydrangeaInfo->mNpcPath[i].mNpcTmpID == 0 )
		{
			memcpy( &tpHydrangeaInfo->mNpcPath[i], &rNpcPath, sizeof( rNpcPath ) );
			break;	
		}		
	}		
	return SUCCESS;
}

// ***********************************************************
//  Function:		Initialize
//  Description:	初始化npc信息
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 03-30-2010
// *********************************************************
int CHydrangeaNpcManager::Initialize()
{
	//memset( mHydrangeaPath, 0, sizeof( mHydrangeaPath ) );
	memset( mHydrangeaPosIndex, 0, sizeof( mHydrangeaPosIndex ) );
	memset( mNpcEntityID, 0, sizeof( mNpcEntityID ) );	
	mTimeInfo.initailize();
	memset( mNpcInfo, 0, sizeof( mNpcInfo ) );
	mActivityID = 0; 
	return SUCCESS;
}

// ***********************************************************
//  Function:		SetNpcEntityID
//  Description:	设置某个npc的实体ID
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 03-30-2010
// *********************************************************
int CHydrangeaNpcManager::SetNpcEntityID( int nNpcIndex, int nEntityID )
{	
	if ( nNpcIndex < 0 || nNpcIndex >= (int)ARRAY_CNT( mNpcEntityID ) )
	{	
		return -1;
	}
	mNpcEntityID[nNpcIndex] = nEntityID;
	return SUCCESS;		
} 

// ***********************************************************
//  Function:		GetNpcEntityID
//  Description:	获取某个npc的实体ID
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 03-30-2010
// *********************************************************
int CHydrangeaNpcManager::GetNpcEntityID( int nNpcIndex )
{
	if ( nNpcIndex < 0 || nNpcIndex >= (int)ARRAY_CNT( mNpcEntityID ) )
	{
		return 0;
	}
	return mNpcEntityID[nNpcIndex];
}

// ***********************************************************
//  Function:		GetNpcNum
//  Description:	获得npc实体的数量
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 03-30-2010
// *********************************************************
int CHydrangeaNpcManager::GetNpcNum()
{
	int tNpcNum = 0;
	CHydrangeaNpcInfo *tpNpcInfo = GetHydrangeaNpcInfo( mActivityID );
	if ( tpNpcInfo == NULL )
	{
		return tNpcNum;
	}
	for ( int i = 0; i < (int)ARRAY_CNT( tpNpcInfo->mNpcPath ); ++i )
	{
		if ( tpNpcInfo->mNpcPath[i].mNpcTmpID == 0 )
		{
			break;
		}
		++tNpcNum;
	}
	return tNpcNum;
		
}

// ***********************************************************
//  Function:		CheckNpcPos
//  Description:	检查某个npc是否到达了指定坐标
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 03-30-2010
// *********************************************************
bool CHydrangeaNpcManager::CheckNpcPos( int nNpcIndex, int nPosX, int nPosY, bool &rReachEnd )
{	
	rReachEnd = false;
	CHydrangeaNpcInfo *tpNpcInfo = ( CHydrangeaNpcInfo * )GetHydrangeaNpcInfo( mActivityID );
	if ( tpNpcInfo == NULL )
	{
		rReachEnd = true;
		return true;
	}
	if ( nNpcIndex < 0 || nNpcIndex >= (int)ARRAY_CNT( mHydrangeaPosIndex ) || nNpcIndex >= (int)ARRAY_CNT( tpNpcInfo->mNpcPath ) )
	{
		rReachEnd = true;
		return true;
	}
	
	int tPosIndex = mHydrangeaPosIndex[nNpcIndex];
	if ( tPosIndex < 0 || tPosIndex >= (int)ARRAY_CNT( tpNpcInfo->mNpcPath[nNpcIndex].mNpcPath ) )
	{	
		rReachEnd = true;
		return true;
	}
	if ( tpNpcInfo->mNpcPath[nNpcIndex].mNpcPath[tPosIndex].mX == nPosX && tpNpcInfo->mNpcPath[nNpcIndex].mNpcPath[tPosIndex].mY == nPosY )
	{
		if ( tPosIndex == (int)ARRAY_CNT( tpNpcInfo->mNpcPath[nNpcIndex].mNpcPath ) -1 )
		{
			rReachEnd = true;
		}
		else
		{
			if ( tpNpcInfo->mNpcPath[nNpcIndex].mNpcPath[tPosIndex+1].mX == 0 && tpNpcInfo->mNpcPath[nNpcIndex].mNpcPath[tPosIndex+1].mY == 0 )
			{
				rReachEnd = true;
			}
		}
		return true;
	}						
					
	return false;			
}

// ***********************************************************
//  Function:		SetNpcPosIndex
//  Description:	设置某个npc的移动位置索引
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 03-30-2010
// *********************************************************
int CHydrangeaNpcManager::SetNpcPosIndex( int nNpcIndex, int nPosIndex )
{
	CHydrangeaNpcInfo *tpNpcInfo = GetHydrangeaNpcInfo( mActivityID );
	if ( tpNpcInfo == NULL )
	{						
		return -1;
	}
	if ( nNpcIndex < 0 || nNpcIndex >= (int)ARRAY_CNT( mHydrangeaPosIndex ) || nNpcIndex >= (int)ARRAY_CNT( tpNpcInfo->mNpcPath ) )
	{
		return -1;
	}
	
	if ( nPosIndex < 0 || nPosIndex >= (int)ARRAY_CNT( tpNpcInfo->mNpcPath[nNpcIndex].mNpcPath ) )
	{
		return -1;
		
	}
	
	mHydrangeaPosIndex[nNpcIndex] = nPosIndex;
	return SUCCESS;
}

// ***********************************************************
//  Function:		GetNpcPos
//  Description:	获取npc某个位置索引的坐标
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 03-30-2010
// *********************************************************
int CHydrangeaNpcManager::GetNpcPos( int nNpcIndex, int nPosIndex, CWTPoint &rPoint )
{		
	CHydrangeaNpcInfo *tpNpcInfo = GetHydrangeaNpcInfo( mActivityID );
	if ( tpNpcInfo == NULL )
	{
		return -1;
	}
	if ( nNpcIndex < 0 || nNpcIndex >= (int)ARRAY_CNT( tpNpcInfo->mNpcPath ) )
	{
		return -1;
	}
	
	if ( nPosIndex < 0 || nPosIndex >= (int)ARRAY_CNT( tpNpcInfo->mNpcPath[nNpcIndex].mNpcPath ) )
	{
		return -1;
	}
	
	rPoint.mX = tpNpcInfo->mNpcPath[nNpcIndex].mNpcPath[nPosIndex].mX;
	rPoint.mY = tpNpcInfo->mNpcPath[nNpcIndex].mNpcPath[nPosIndex].mY;
	return SUCCESS;
}

// ***********************************************************
//  Function:		GetHydrangeaNpcPath
//  Description:	获取某个npc的路径信息
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 03-30-2010
// *********************************************************
CHydrangeaNpcPath *CHydrangeaNpcManager::GetHydrangeaNpcPath( int nIndex, int nActivityID )
{
	CHydrangeaNpcInfo *tpNpcInfo = GetHydrangeaNpcInfo( nActivityID );
	if ( tpNpcInfo == NULL )
	{
		return NULL;
	}
	
	if ( nIndex < 0 || nIndex >= (int)ARRAY_CNT( tpNpcInfo->mNpcPath ) )
	{
		return NULL;
	}	
	
	return &tpNpcInfo->mNpcPath[nIndex];
}

// ***********************************************************
//  Function:		GetNpcPosIndex
//  Description:	获取某个npc的位置索引
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 03-30-2010
// *********************************************************
int CHydrangeaNpcManager::GetNpcPosIndex( int nIndex )
{ 
	if ( nIndex < 0 || nIndex >= (int)ARRAY_CNT( mHydrangeaPosIndex ) )
	{
		return 0;
	}	
	
	return mHydrangeaPosIndex[nIndex];
}

// ***********************************************************
//  Function:		InsertTimeInfo
//  Description:	插入一个时间信息
//  Date:		 04-02-2010
// *********************************************************
int CHydrangeaNpcManager::InsertTimeInfo( CTimeInfo &rTimeInfo )
{
	mTimeInfo.push_back( TimeInfoList::value_type( rTimeInfo ) );
	return SUCCESS;	
}

// ***********************************************************
//  Function:		CheckActivityTime
//  Description:	检查是否可以开始活动
//  Date:		 04-02-2010
// *********************************************************
bool CHydrangeaNpcManager::CheckActivityTime( CTimeInfo &rTimeInfo, int &rActivityID )
{
	for ( TimeInfoList::iterator it = mTimeInfo.begin(); it != mTimeInfo.end(); ++it )
	{
		if ( rTimeInfo.mHour == (*it).mHour && rTimeInfo.mMinute == (*it).mMinute && rTimeInfo.mWeekDay == (*it).mWeekDay )
		{
			rActivityID = (*it).mActivityID;
			return true;
		}
			
	}
	return false;
}

// ***********************************************************
//  Function:		InitializeConfig
//  Description:	初始化配置信息
//  Date:		 04-22-2010
// *********************************************************
void CHydrangeaNpcManager::InitializeConfig()
{
	 //memset( mHydrangeaPath, 0, sizeof( mHydrangeaPath ) );
	 mTimeInfo.initailize();
	 memset( mNpcInfo, 0, sizeof( mNpcInfo ) ); 
}

// ***********************************************************
//  Function:		GetHydrangeaNpcInfo
//  Description:	根据活动ID获取npc的婚礼信息
//  Date:		 04-26-2010
// *********************************************************
CHydrangeaNpcInfo *CHydrangeaNpcManager::GetHydrangeaNpcInfo( int nActivityID )
{	
	for ( int i = 0; i < (int)ARRAY_CNT( mNpcInfo ); ++i )
	{
		if ( mNpcInfo[i].mActivityID == nActivityID )
		{
			return &mNpcInfo[i];
		}
	}
	return NULL;		
}

// ***********************************************************
//  Function:		Initialize
//  Description:	活动初始化
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 03-31-2010
// *********************************************************
int CHydrangeaActivityManager::Initialize()
{
	memset( mHydrangeaGraberID, 0, sizeof( mHydrangeaGraberID ) );
	mHydrangeaGraberNum = 0;
	mHydrangeaRollValue = 0;
	mHydrangeaHolderID = 0;
	mActivityStatus = 0;	
	mHydrangeaHoldTimerID = 0;
	mActivityTimerID = 0;	
	return SUCCESS;
}

// ***********************************************************
//  Function:		InitializeGrab
//  Description:	抢绣球的初始化
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 03-31-2010
// *********************************************************
int CHydrangeaActivityManager::InitializeGrab( )		
{
	mHydrangeaGraberID[0] = mHydrangeaHolderID;
	mHydrangeaGraberNum = 1;
	mHydrangeaRollValue = rand()%SERVER_PERCENT_INT;
	mActivityStatus = HYDRANGEASTATUS_GRAB;	
	mHydrangeaHoldTimerID = 0;
	return SUCCESS;
}

// ***********************************************************
//  Function:		GrabHydrangea
//  Description:	抢夺绣球
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 03-31-2010
// *********************************************************
int CHydrangeaActivityManager::GrabHydrangea( int nRoleID )
{		
	if ( mActivityStatus != HYDRANGEASTATUS_GRAB )
	{
		return ERR_HYDRANGEA_ACTIVITYEND;
	}
	
	if ( CheckGraber( nRoleID ) == true )
	{
		return ERR_HYDRANGEA_GRABED;
	}
	
	if ( mHydrangeaGraberNum >= (int)ARRAY_CNT( mHydrangeaGraberID ) )
	{
		return ERR_HYDRANGEA_GRABERFULL;
	}
	
	mHydrangeaGraberID[mHydrangeaGraberNum] = nRoleID;
	++mHydrangeaGraberNum;
	
	int tRandValue = rand()%SERVER_PERCENT_INT;
	if ( tRandValue > mHydrangeaRollValue )
	{
		mHydrangeaHolderID = nRoleID;
		mHydrangeaRollValue = tRandValue;
	}
	
	return SUCCESS;	
}

// ***********************************************************
//  Function:		GrabHydrangea
//  Description:	抢夺绣球
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 03-31-2010
// *********************************************************
bool CHydrangeaActivityManager::CheckGraber( int nRoleID )
{
	for ( int i = 0; i < mHydrangeaGraberNum; ++i )
	{
		if ( nRoleID == mHydrangeaGraberID[i] )
		{
			return true;
		}
	}
	return false;	
}

// ***********************************************************
//  Function:		InsertOgreInfo
//  Description:	插入一个怪物信息
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 07-19-2010
// *********************************************************
int COgreActivityManager::InsertOgreInfo( COgreInfo &rOgreInfo )
{
	mOgreInfoList.insert( OgreInfoList::value_type( rOgreInfo.mTempID, rOgreInfo ) );
	return SUCCESS;
}
// ***********************************************************
//  Function:		GetOgreInfo
//  Description:	根据模板ID获取一个怪物信息
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 07-19-2010
// *********************************************************
COgreInfo* COgreActivityManager::GetOgreInfo( int nTempID )
{
	OgreInfoList::iterator it = mOgreInfoList.find( nTempID );
	if ( it != mOgreInfoList.end() )
	{
		return &it->second;
	}
	return NULL;
}

// ***********************************************************
//  Function:		EndOgreActivity
//  Description:	结束怪物活动
//  Input:			
//  OutPut:			
//  Return:			
//  Others:
//  Date:		 07-20-2010
// *********************************************************
int COgreActivityManager::EndOgreActivity( int nExceptTempID )
{
	OgreInfoList::iterator it = mOgreInfoList.begin();
	for ( ; it != mOgreInfoList.end(); ++it )
	{
		COgreInfo *tpOgreInfo = &it->second;
		if ( tpOgreInfo == NULL )
		{
			continue;
		}
		
		if ( tpOgreInfo->mTempID == nExceptTempID )
		{
			continue;
		}
				
		for ( int i = 0; i < (int)ARRAY_CNT( tpOgreInfo->mEntityID ); ++i )
		{							
			if ( tpOgreInfo->mEntityID == 0 )
			{	
				break;
			}
			CEntity *tpEntity = CSceneLogic::GetSingleton().GetEntity( tpOgreInfo->mEntityID[i] );
			if ( tpEntity == NULL )
			{
				continue;
			}
			if ( tpEntity->IsCharacter() )
			{
				CEntityCharacter  *tpCharacter = ( CEntityCharacter  * )tpEntity;
				tpCharacter->EntityDie( NULL, false, false );				
			}
			else
			{
				CSceneLogic::GetSingleton().DestroyEntity( tpEntity, 0 );

			}
		}
		
		tpOgreInfo->ClearEntityID();
	}	
	return 0;	
}

// ***********************************************************
//  Function:		InsertTreasureBowlProbality
//  Description:	插入一个聚宝盆的概率信息
//  Date:		 11-02-2010
// *********************************************************
int CTreasureBowlManager::InsertTreasureBowlProbality( int nProbalityID, CTreasureBowlProbality &rProbality )
{
	mTreasureBowlProbalityList.insert( CTreasureBowlProbalityList::value_type( nProbalityID, rProbality ) );
	return SUCCESS;
}

// ***********************************************************
//  Function:		InsertTreasureBowlExchangeInfo
//  Description:	插入一个举报彭的兑换信息
//  Date:		 11-02-2010
// *********************************************************
int	CTreasureBowlManager::InsertTreasureBowlExchangeInfo( int nTemID, CTreasureItemExchangeInfo &rExchangeInfo )
{
	mTreasureBowlItemExchangeList.insert( CTreasureBowlItemExchangeList::value_type( nTemID, rExchangeInfo ) );
	return SUCCESS;
}

// ***********************************************************
//  Function:		InsertTreasureBowlProbality
//  Description:	获取聚宝盆的一个概率信息
//  Date:		 11-02-2010
// *********************************************************
CTreasureBowlProbality* CTreasureBowlManager::GetTreasureBowlProbality( int nProbalityID )
{
	CTreasureBowlProbalityList::iterator it = mTreasureBowlProbalityList.find( nProbalityID );
	if ( it != mTreasureBowlProbalityList.end() )
	{
		return &(it->second);
	}
	return NULL;
}

// ***********************************************************
//  Function:		InsertTreasureBowlProbality
//  Description:	获取聚宝盆的一个交换信息
//  Date:		 11-02-2010
// *********************************************************
CTreasureItemExchangeInfo*  CTreasureBowlManager::GetTreasureBowlExchangeInfo( int nItemID )
{
	CTreasureBowlItemExchangeList::iterator it = mTreasureBowlItemExchangeList.find( nItemID );
	if ( it != mTreasureBowlItemExchangeList.end() )
	{
		return &(it->second);
	}
	return NULL;
}

int CTreasureBowlManager::StartTreasureBowl( int nItemID, int &rNum, int &rNewItemID )
{
	CTreasureItemExchangeInfo *tpItemInfo = GetTreasureBowlExchangeInfo( nItemID );	
	if ( tpItemInfo == NULL )
	{
		return -1;
	}
	
	CTreasureBowlProbality *tpProbality = GetTreasureBowlProbality( tpItemInfo->mProbilityID );
	if ( tpProbality == NULL )
	{
		return -1;
	}
	
	int tTotalCount = 0;
	for (unsigned int i = 0; i < ARRAY_CNT( tpProbality->mNum ) && tpProbality->mNum[i] > 0; ++i )
	{
		tTotalCount += tpProbality->mProbality[i];
	}		
	
	if ( tTotalCount <= 0 )
	{
		return -1;
	}
	
	rNewItemID = tpItemInfo->mExchangeItemID;
	int tProbalityValue = RAND( tTotalCount );
	int tOldValue = 0;
	rNum = 1;
	for ( unsigned int i = 0; i < ARRAY_CNT( tpProbality->mNum ) && tpProbality->mNum[i] > 0; ++i )
	{
		if ( tProbalityValue >= tOldValue && tProbalityValue <= ( tOldValue + tpProbality->mProbality[i] ) )
		{
			rNum = tpProbality->mNum[i];
			break;
		}		
		
		tOldValue =	  tOldValue + tpProbality->mProbality[i];
	}		
	return SUCCESS;
}