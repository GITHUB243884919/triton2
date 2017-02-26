#pragma once

#include "activitytype.h"
#include "lk_hashmap.h"
#include "lk_vector.h"
#include "marriagetype.h"
#include "servertool.h"

// 100311 MAOCY ADD-BEGIN
#define ROLE_VALID_QUESTION_FAILURE_COUNT 2
#define ROLE_VALID_QUESTION_RESOURCE_LENGTH 64*64*2

#define ROLE_VALID_QUESTION_COUNT 256
#define ROLE_VALID_QUESTION_GROUP_COUNT 4
#define ROLE_VALID_QUESTION_OPTION_COUNT 64
#define ROLE_VALID_QUESTION_DESCRIPTION_LENGTH 1024

// 应用类型
enum EAppType
{
	APPTYPE_NULL			= 0x00,			// 没有进行任何答题
	APPTYPE_NPC				= 0x01,			// 每日问答
	APPTYPE_QUESTIONPAPER	= 0x02,			// 题卷
	APPTYPE_ACTIVITY		= 0x04,			// 活动
};

// 题目的数据结构
struct CQuestion
{		
	unsigned short QuestionID;				// 题目ID
	unsigned short QuestionAnswer;			// 题目答案
	CQuestion()
	{
		if ( CObj::msCreateMode )
		{
			QuestionID = 0;
			QuestionAnswer = 0;
		}
		else
		{
			
		}
	}
};

// 题库的数据结构
struct CQuestionLib
{
	int LibID;
	int QuestionNum;
	int	FirstQuestionIndex;
	CQuestionLib()
	{
		if ( CObj::msCreateMode )
		{
			LibID = 0;
			QuestionNum = 0;
			FirstQuestionIndex = 0;
		}
		else
		{
			
		}
	}
};


// 答题具体应用的数据结构
struct CAppType
{
	int			TypeID;
	char		TypeName[XML_ELEMENT_NAME_LENGTH];
	int			QuestionLibNum;
	CQuestionLib QuestionLib[MAX_QUESTIONLIB_NUM];
	CAppType()
	{
		if ( CObj::msCreateMode )
		{
			TypeID = 0;
			memset( TypeName, 0, sizeof( TypeName ) );
			QuestionLibNum = 0;
			memset( QuestionLib, 0, sizeof( QuestionLib ) );
		}
		else
		{
		
		}
	}
};

// 物品奖励信息
struct CItemPrize
{
	int ItemPrizeID;
	int ItemPrizeNum;	
};

// 题库管理类
class  CQuestionManager
{
public:
	typedef lk::vector< CItemPrize, MAX_NPCPRIZE_NUM > NpcPrizeList;
public:
	int AddNewLib( int LibID, const char *pAppType );
	int AddQuestion( const char *QuestionID, const char *Answer );
	int CreatQuestion( int Apptype, int QuestionNum,int *Question, int *Answer );
	int CreatLibID( CAppType *pAppType,int QuestionNum, int *vLibIndex );
	int GetQuestFromLib( CAppType *pAppType,int QuestionNum,int *Question, int *Answer  );	
	int InsertNpcPrize( CItemPrize &rNpcPrize );
	int GetNpcPrize( CItemPrize *pPrizeList, int &rNum );
public:
	CQuestionManager( );
	~CQuestionManager( ){ }
	int Initialize();
	int Resume();
	
protected:
private:
	CAppType	mNpcLib;						// 每日问答npc处题库
	CAppType	mPaperLib;						// 答题卷题库
	CQuestion	mQuestion[TOTAL_QUESTION_NUM];	// 所有的题目
	int			mQuestionNum;
	int			mCurrentLibID;					// 正在读取题库ID
	NpcPrizeList mNpcPrizeList;					// npc答题的物品奖励			
};	



// 用于记录玩家答题状态
class  CAnswerStatus : public CObj
{

public:
	// 检查答案是否正确
	int CheckAnswer( int QuestionID,int QuestAnswer );
	
	// 获取新题目的ID
	int	GetNewQuestID( );
	
	//  检查是本道题是否已经作废
	int CheckIfQuestionInvalidation( int CheckType );
	
	// 结束答题
	int EndAnswerQuesetion(  );
	
	// 检查题目打错的次数
	int CheckCountAnswered( int QuestionID = 0 );
	
	// 清除答题状态信息
	void ClearStatus(  );

public:
	  void SetAppType( int status ){	mAppType = status; }
	  int  GetAppType(  ) { return mAppType;	 } 	  
	  
	  void SetCurrentQuestIndex( int index  ) {  mCurrentQuestIndex = index; } 
	  int  GetCurrentQuestIndex( ){  return mCurrentQuestIndex; }	
	  
	  void SetCountAnswered( int count ){  mCountAnswered = count; }
	  int  GetCountAnswered(){  return mCountAnswered; }
	  
	  int  SetQuestion( int QuestNum, int *Question, int *Answer );	
	  
	  void SetQuestionNum( int QuestionNum ){ mQuestionNum = QuestionNum; } 
	  int  GetQuestionNum( ){  return mQuestionNum; }		
	  
	  void SetPaperIndex( int Index ){ mQuestPaperIndex = Index; }
	  int  GetPaperIndex(  ){ return mQuestPaperIndex; }
	  
	  void SetRightAnswerNum( int Num ){ mRightAnswerNum = Num; } 
	  int  GetRightAnswerNum(  ) { return mRightAnswerNum; }
	  
	  void SetSingleTimerID( int TimerID ) { mSingleTimerID = TimerID; }
	  int  GetSingleTimerID( ){  return mSingleTimerID; }
	  
	  void SetTotalTimerID( int TimerID ){ mTotalTimerID = TimerID; }
	  int GetTotalTimerID(  ){  return mTotalTimerID; }
	  
	  void SetInvestLibID( int vLibID ) { mInvestLibID = vLibID; } 
	  int  GetInvestLibID( ){ return mInvestLibID; } 
	  
	  void SetExpRate( int nExpRate ){ mExpRate = nExpRate; }
	  int  GetExpRate(  ){ return mExpRate; }
 
	  void SetQuestionVerifyDataLength( int nLength ) { mQuestionVerifyDataLength = nLength; }
	  int GetQuestionVerifyDataLength() { return mQuestionVerifyDataLength; }
	  
	  void SetQuestionVerifyData( const char *pData, int nLength )
	  {
		if ( pData != NULL )
		{
			int tSize = nLength > (int)( sizeof( mQuestionVerifyData ) ) ?   (int)(sizeof( mQuestionVerifyData )) : nLength; 
			memcpy( mQuestionVerifyData, pData, tSize );
			mQuestionVerifyDataLength = tSize;
		}
	  } 
	  
	  bool CheckQuestionVerifyData( const char *pData, int nLength )
	  {
		  if ( pData == NULL )
		  {
			return false;
		  }
		  
		  if ( nLength != mQuestionVerifyDataLength )		  
		  {
			return false;
		  }
		  
		  if ( memcmp( mQuestionVerifyData, pData, mQuestionVerifyDataLength ) == 0 )
		  {
			return true;
		  }
		  
		  return false;
	  }
	  
	  void SetRoleVerifyKind( int nKind ){ mRoleVerifyKind = nKind; }
	  int GetRoleVerifyKind(){ return mRoleVerifyKind; }
	  
	  void SetRoleVerifyType( int nType ){ mRoleVerifyType = nType; }
	  int GetRoleverifyType(  ) { return mRoleVerifyType; }
	  
	  void SetVerifyTimeID( int nID ){ mVerifyTimeID = nID; }
	  int GetVerifyTimeID(){ return mVerifyTimeID; }
	 
	  
	  // 清空验证信息
	  void ClearQuestionVerifyInfo()
	  {
		  mQuestionVerifyDataLength = 0;
		  memset( mQuestionVerifyData, 0, sizeof( mQuestionVerifyData ) );	
		  mRoleVerifyKind = 0;
		  mRoleVerifyType = 0;	  
	  }  
public:  
	  CAnswerStatus( )
	  {		
		  if( CObj::msCreateMode )
		  {
			  Initialize();
		  }
		  else
		  {
			  Resume();
		  }	
	  }
public:
	// 初始化
	virtual int Initialize( ) 
	{ 		
		mAppType = 0;
		mQuestionNum  = 0;
		mCurrentQuestIndex = -1;
		mCountAnswered    = 0;	
		mRightAnswerNum = 0;
		mQuestPaperIndex = 0;
		mTotalTimerID = 0;
		mSingleTimerID = 0 ;
		memset( mQuestionIDSet, 0, sizeof( mQuestionIDSet ) );
		memset( mAnswerSet,	0, sizeof( mAnswerSet ) );
		mInvestLibID = 0;
		mExpRate = 1;	
		
		mQuestionVerifyDataLength = 0;
		memset( mQuestionVerifyData, 0, sizeof( mQuestionVerifyData ) );	
		mRoleVerifyKind = 0;
		mRoleVerifyType = 0;
		mVerifyTimeID=0;	
		return 0;
	} 
	// 恢复
	virtual int Resume( ) { return 0; } 
protected:
public:
	int		  mQuestionIDSet[MAX_PLAYER_QUESTIONNUM];	// 题目
	int		  mAnswerSet[MAX_PLAYER_QUESTIONNUM];		// 题目答案
	short	  mAppType;									// 目前的答题状态
	short 	  mQuestionNum;								// 目前所回答题目的数量
	short     mCurrentQuestIndex;						// 目前玩家所答题目的索引
	short	  mCountAnswered;							// 本道题目已经回答的次数		
	short     mRightAnswerNum;							// 已经回答正确的题目数量
	short     mQuestPaperIndex;							// 答题卷的索引
	int		  mTotalTimerID;							// 所有题目的计时器
	int		  mSingleTimerID;							// 每道题目的计时器
	int		  mInvestLibID;							    // 调查问卷的题目ID
	int		  mExpRate;									// 答题卷的经验加成
	
	int		  mQuestionVerifyDataLength;							// 答题验证数据长度
	char	  mQuestionVerifyData[ROLE_VALID_QUESTION_RESOURCE_LENGTH]; // 答题验证的数据			
	short	  mRoleVerifyKind;										// 答题验证类型，副本验证或者普通验证
	short	  mRoleVerifyType;										// 验证类型，什么情况下算错(超时或者下线等)	
	int		  mVerifyTimeID;										// 验证时计时器的ID
};



/*****************问卷调查的数据结构*************/ 
struct CInvestAnswer
{
public:
	short mAnswerCount[MAX_OPTION_NUM];
	int	  mQuestionID;
public:
	CInvestAnswer()
	{
		if ( CObj::msCreateMode )
		{
			memset( mAnswerCount, 0, sizeof( mAnswerCount ) );	
			mQuestionID = 0;
		}
		else
		{	
					
		}		
	}
};

class  CInvestQuestionLib
{	
public:
	int Initialize();
	int Resume();
	CInvestQuestionLib();
	~CInvestQuestionLib(){ }	
public:
	void SetLibID( int vLibID ){ m_nLibID = vLibID; }
	int	 GetLibID( ){ return m_nLibID; }
	
	void SetQuestionNum( int vQuestionNum ){ m_nQuestionNum = vQuestionNum; }
	int  GetQuestionNum(  ){ return m_nQuestionNum; }
	
	int InsertQuestion( int QuetionID, int IfSelected );
	int GetQuestion( int QuestionIndex );
	
	void SetExpPrize( int vExp ){ m_nExpPrize = vExp; } 	
	int  GetExpPrize(  ){ return m_nExpPrize; }	
	
	int InsertItemPrize( int ItemID, int ItemNum );
	int GetItemPrize( int Index, CItemPrize &vItemPrize );
	int GetItemPrizeNum(){ return m_nItemPrizeNum; }	
	
	void SetMaxQuestionNum( int vNum ){ m_nMaxQuestionNum = vNum; }
	int GetMaxQuestionNum(){ return m_nMaxQuestionNum; }
	
	void SetMoney( int vMoney ){ m_nMoney = vMoney; }
	int  GetMoney(  ){ return m_nMoney; }
	
	void SetBindMoney( int vBindMoney ){ m_nBindMoney = vBindMoney; }
	int  GetBindMoney( ){ return m_nBindMoney; } 
	
	void SetCowry( int vCowry ){ m_nCowry = vCowry; }
	int  GetCowry(  ){ return m_nCowry; }
	
	int CreatInvestQuestionID( );
	
	// 记录答案
	int RecordAnswer( int nQuestionID, int nAnswerOption );
	
	// 清除答案
	void ClearAnswer(){  memset( m_nAnswerList, 0, sizeof( m_nAnswerList ) ); }
	
	// 是否有某道题目
	bool HasQuestion( int nQuestionID );	
	
	// 设置某道题目某个选项的数量
	void SetOptionCount( int nQuestionID, int nOptionIndex, int nCount );

public:
	CInvestAnswer *GetAnswerInfo( int nIndex )
	{ 
		if ( nIndex < 0 || nIndex >= (int)ARRAY_CNT( m_nAnswerList ) )
		{
			return NULL;
		}
		return &m_nAnswerList[nIndex];
	}
private:						  
	int m_nLibID;										// 题库ID
	int m_sQuestionList[MAX_QUESTIONNUM_EACHLIB];		// 题目列表
	int m_nQuestionNum;									// 题目数量
	int m_nExpPrize;									// 经验奖励
	CItemPrize m_nItemPrizeList[MAX_INVEST_PRIZENUM];	// 物品奖励
	int m_nItemPrizeNum;								// 奖励物品的数量
	int m_nMaxQuestionNum;							    // 题库中的调查问卷的最大题目数量小于题目数量
	int m_nSelectedQuestionNum;							// 必选题目数量
	int m_nMoney;										// 奖励的金钱
	int m_nBindMoney;									// 奖励的绑定金钱
	int m_nCowry;										// 奖励的元宝	
	CInvestAnswer m_nAnswerList[MAX_QUESTIONNUM_EACHLIB];// 答案列表
};


class CInvestQuestionManage
{
public:
	typedef lk::hash_map< int, CInvestQuestionLib, MAX_INVEST_QUESTLIBNUM > INVESTLIBLIST; 
public:
	CInvestQuestionManage();
	~CInvestQuestionManage(){}
	int Initialize();
	int Resume();
public:
	CInvestQuestionLib *GetInvestQuestLib( int vLibID );
	int InsertInvestQuestLib( CInvestQuestionLib *pQuestionLib );
	INVESTLIBLIST *GetQuestionLibList(){ return &m_tInvestLibList; }
	
	// 记录题目答案
	int RecordLibAnswer( int nLibID, int nQuestionID, int nOption );
	
	// 清除某个题库的答案	
	int ClearLibAnswer( int nLibID );
	
	// 清除所有题目的答案
	int ClearAllLibAnswer();
	
	// 设置&获取调查问卷是否输出过
	void SetInvestOutput( bool bOutput ){ m_bInvestOutput = bOutput; }
	bool GetInvestOutput(){ return m_bInvestOutput; }

private:
	INVESTLIBLIST m_tInvestLibList;	
	bool m_bInvestOutput;								// 调查结果是否输出过		
};



// 抛绣球绣球活动相关
struct CHydrangeaNpcPath
{
public:
	CWTPoint mNpcPath[MAX_WEDDINGNPCPATH_STEPS];
	int		mNpcTmpID;
	int		mMapID;	
public:
	CHydrangeaNpcPath( )
	{
		if ( CObj::msCreateMode )
		{
			memset( mNpcPath, 0, sizeof( mNpcPath ) );
			mNpcTmpID = 0;
			mMapID = 0;			
		}
		else
		{

		}
	}
};

struct CTimeInfo
{
public:
	int mWeekDay;
	int mHour;
	int mMinute;
	int mActivityID;
};

struct CHydrangeaNpcInfo
{
public:
	CHydrangeaNpcPath mNpcPath[MAX_HYDRANGEANPC_NUM];
	int				  mActivityID;
public:
	CHydrangeaNpcInfo()
	{
		if ( CObj::msCreateMode )
		{
			memset( mNpcPath, 0, sizeof( mNpcPath ) );
			mActivityID = 0;
		}
		else
		{
			
		}
	}
};
class CHydrangeaNpcManager
{
public:
	CHydrangeaNpcManager()
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
	~CHydrangeaNpcManager(){ }
	
	int Initialize();
	int Resume( ){ return SUCCESS; };
public:
	typedef lk::vector<CTimeInfo, HYDRANGEA_TIMENUM> TimeInfoList;	
public:

	// 插入一个npc的路径信息
	int InsertNpcPath( CHydrangeaNpcPath &rNpcPath, int nActivityID );
	
	// 设置某个npc的实体ID
	int SetNpcEntityID( int nNpcIndex, int nEntityID );
	
	// 获取某个npc的实体ID
	int GetNpcEntityID( int nNpcIndex );
	
	// 获得npc实体的数量
	int GetNpcNum();
	
	// 检查某个npc是否到达了指定坐标
	bool CheckNpcPos( int nNpcIndex, int nPosX, int nPosY, bool &rReachEnd );
	
	// 设置某个npc的移动位置索引
	int SetNpcPosIndex( int nNpcIndex, int nPosIndex );
	
	// 获取npc某个位置索引的坐标
	int GetNpcPos( int nNpcIndex, int nPosIndex, CWTPoint &rPoint );
	
	// 获取某个npc的路径信息
	CHydrangeaNpcPath *GetHydrangeaNpcPath( int nIndex, int nActivityID );
	
	// 获取某个npc的位置索引
	int GetNpcPosIndex( int nIndex );
	
	// 插入一个时间信息
	int InsertTimeInfo( CTimeInfo &rTimeInfo );	 
	
	// 检查是否可以开始活动
	bool CheckActivityTime( CTimeInfo &rTimeInfo, int &rActivityID );	
	
	// 初始化配置信息
	void InitializeConfig();
	
	// 设置&获取活动ID
	void SetActivityID( int nActivityID ){ mActivityID = nActivityID; }
	int GetActivityID(){ return mActivityID; }
	
	// 根据活动ID获取npc的婚礼信息
	CHydrangeaNpcInfo *GetHydrangeaNpcInfo( int nActivityID );
	
private:
//	CHydrangeaNpcPath mHydrangeaPath[MAX_HYDRANGEANPC_NUM];		// 绣球npc的路径
	int		  mHydrangeaPosIndex[MAX_HYDRANGEANPC_NUM];		    // npc的当前位置索引
	int		  mNpcEntityID[MAX_HYDRANGEANPC_NUM];				// npc的实体ID
	TimeInfoList mTimeInfo;
	CHydrangeaNpcInfo mNpcInfo[HYDRANGEA_TIMENUM];				// npc的路径信息
	int		 mActivityID;										// 当前举行的活动ID
};

// 绣球活动
class CHydrangeaActivityManager
{
public:
	CHydrangeaActivityManager()
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
	int Initialize();
	int Resume(){ return SUCCESS; }
public:	
	int mHydrangeaGraberID[MAX_HYDRANGEARABER_NUM]; // 抢绣球的人的角色ID
	int mHydrangeaGraberNum;						// 抢绣球的人数
	int mHydrangeaRollValue;						// 抢绣球的随机值
	int mHydrangeaHolderID;							// 绣球持有者的ID
	int mActivityStatus;							// 绣球活动状态
	int mHydrangeaHoldTimerID;						// 绣球持有定时器的ID
	int	mActivityTimerID;							// 活动结束计时器的ID
public:
	// 设置&获取抢绣球的人数
	void SetHydrangeaGraberNum( int nNum ){ mHydrangeaGraberNum = nNum; }
	int GetHydrangeaGraberNum( ){ return mHydrangeaGraberNum; }
	
	// 设置&获取抢绣球的随机值
	void SetHydrangeaRollValue( int nValue ){ mHydrangeaRollValue = nValue; }
	int GetHydrangeaRollValue(){ return mHydrangeaRollValue; }
	
	// 设置&获取绣球持有者的ID
	void SetHydrangeaHolderID( int nRoleID ){ mHydrangeaHolderID = nRoleID; }
	int GetHydrangeaHolderID( ){ return mHydrangeaHolderID; }
	
	// 设置&获取绣球活动的状态
	void SetActivityStatus( int nStatus ){ mActivityStatus = nStatus; }
	int GetActivityStatus(){ return mActivityStatus; }
		
	// 获取原始绣球持有者
	int GetOriginalHolder(){ return mHydrangeaGraberID[0]; }
	
	// 设置&获取绣球持有定时器的ID
	void SetHydrangeaHolderTimerID( int nTimerID ){ mHydrangeaHoldTimerID = nTimerID; }
	int GetHydrangeaHolderTimerID(){ return mHydrangeaHoldTimerID; }
	
	// 设置&获取绣球活动定时器ID
	void SetActivityTimerID( int nTimerID ){ mActivityTimerID = nTimerID; }
	int GetActivityTimerID( ){ return mActivityTimerID; }
public:
	int InitializeGrab( );					// 抢绣球的初始化
	int GrabHydrangea( int nRoleID );		// 抢夺绣球
	bool CheckGraber( int nRoleID );		// 检查是否已经抢过绣球		
};

// 怪物攻城活动的怪物信息
class COgreInfo
{
public:
	int mTempID;							// 怪物的模板ID
	int mActivityType;						// 怪物的活动类型
	int mLiveTime;							// 显示怪物的剩余时间
	int mKillerID;							// 杀死怪物的另外一个怪的ID
	int mErrcode;							// 显示怪物的剩余时间时应该提示的错误码
	int mBossOgre;							// 是否boss怪
	int mEntityID[MAX_TEMPENTITY_NUM];		// 模板刷处的怪物	
	int mDeathErrcode;						// 死亡时错误码
public:
	void ClearEntityID(){ memset( mEntityID, 0, sizeof( mEntityID ) ); }
	void SetEntityID( int nEntityID )
	{
		for ( int i = 0; i < (int)ARRAY_CNT( mEntityID ); ++i )
		{
			if ( mEntityID[i] == 0 )
			{
				mEntityID[i] = nEntityID;
				break;
			}
		}
	}
	bool IsDead()
	{
		if ( mEntityID[0] == 0 )
		{
			return true;
		}
		return false;
	}
public:
	COgreInfo()
	{
		mTempID = 0;
		mActivityType = 0;
		mLiveTime = 0;
		mKillerID = 0;
		mErrcode = 0;
		mBossOgre = 0;
		memset( mEntityID, 0, sizeof( mEntityID ) );
		mDeathErrcode = 0;
	}
	~COgreInfo(){}
};

// 怪物攻城活动的管理器
class COgreActivityManager
{
public:
	typedef lk::hash_map< int, COgreInfo, MAX_ACTIVITYOGRE_NUM > OgreInfoList;
public:
	int Initialize()
	{
		mOgreInfoList.initailize();
		return 0;
	}
	int Resume()
	{
		return 0;
	}
public:
	COgreActivityManager()
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
	~COgreActivityManager(){ }
public:
	int InsertOgreInfo( COgreInfo &rOgreInfo );
	COgreInfo* GetOgreInfo( int nTempID );		
	int EndOgreActivity( int nExceptTempID );
private:
	OgreInfoList mOgreInfoList;
};



//************************************************************
// Class:       SRoleValidQuestion
// Description: 验证角色的问题定义
// Date:        2010-03-04
//************************************************************
typedef struct _SRoleValidQuestion{
	int id;
	int typeId;
	int resultId;
	char code[ROLE_VALID_QUESTION_DESCRIPTION_LENGTH];
	char description[ROLE_VALID_QUESTION_DESCRIPTION_LENGTH];
} SRoleValidQuestion;

//************************************************************
// Class:       SRoleValidQuestionOption
// Description: 验证角色的问题选项定义
// Date:        2010-03-04
//************************************************************
typedef struct _SRoleValidQuestionOption{
	int id;
	int groupId;
	int length;
	char filename[ROLE_VALID_QUESTION_DESCRIPTION_LENGTH];
	char data[ROLE_VALID_QUESTION_RESOURCE_LENGTH];
} SRoleValidQuestionOption;

//************************************************************
// Class:       SRoleValidQuestionGroup
// Description: 验证角色的问题选项组定义
// Date:        2010-03-04
//************************************************************
typedef struct _SRoleValidQuestionGroup{
	int id;
	int count;
	SRoleValidQuestionOption options[ROLE_VALID_QUESTION_OPTION_COUNT];
} SRoleValidQuestionGroup;


// 聚宝盆的概率信息
class CTreasureBowlProbality
{
public:
	int mNum[MAX_TREASUREBOWL_EXCHANGENUM];
	int mProbality[MAX_TREASUREBOWL_EXCHANGENUM];
public:
	CTreasureBowlProbality()
	{
		memset( mNum, 0, sizeof( mNum ) );
		memset( mProbality, 0, sizeof( mProbality ) );
	}
	~CTreasureBowlProbality(){}
};
// 聚宝盆的物品兑换信息
class CTreasureItemExchangeInfo
{
public:	
	int mExchangeItemID;	// 兑换成的物品ID
	int mProbilityID;		// 选用的概率ID
	int mTipsNum;			// 数量多少时阵营提示
	int mUseTime;			// 每天可以兑换的次数
	int mTaxMoney;			// 聚宝需要的金钱
	int mTipsType;			// 提示类型0表示阵营提示1表示全服提示
public:
	CTreasureItemExchangeInfo()
	{
		mExchangeItemID = 0;			
		mProbilityID = 0;
		mTipsNum = 0;
		mUseTime = 0;
		mTipsType = 0;
	}
	~CTreasureItemExchangeInfo(){}
};

//
class CTreasureBowlManager
{
public:
	typedef lk::hash_map< int, CTreasureBowlProbality, MAX_TREASUREBOWL_PROBALITY_NUM >	CTreasureBowlProbalityList;
	typedef lk::hash_map< int, CTreasureItemExchangeInfo, MAX_TREASUREBOWL_ITEM_NUM  >	 CTreasureBowlItemExchangeList;
public:
	CTreasureBowlManager()
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
	
	~CTreasureBowlManager(){}	
public:
	int Initialize()
	{
		mTreasureBowlProbalityList.initailize();
		mTreasureBowlItemExchangeList.initailize();
		mTotalNum = 0;		
		return 0;
	}
	int Resume()
	{
		return 0;
	}
public:
	int InsertTreasureBowlProbality( int nProbalityID, CTreasureBowlProbality &rProbality );
	int	InsertTreasureBowlExchangeInfo( int  nTemID, CTreasureItemExchangeInfo &rExchangeInfo );
	CTreasureBowlProbality* GetTreasureBowlProbality( int nProbalityID );
	CTreasureItemExchangeInfo* GetTreasureBowlExchangeInfo( int nItemID );		
	int GetTotalNum() { return mTotalNum; }	
	void SetTotalNum( int nTotalNum ) { mTotalNum = nTotalNum; }	
	int StartTreasureBowl( int nItemID, int &rNum, int &rNewItemID );	
public:
	CTreasureBowlProbalityList mTreasureBowlProbalityList;
	CTreasureBowlItemExchangeList mTreasureBowlItemExchangeList;	
	int mTotalNum;			// 聚宝盆每天可以使用的次数	
};