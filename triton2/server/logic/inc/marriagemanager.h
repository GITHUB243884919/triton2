#pragma once
#include "servertool.h"
#include "object.h"
#include "protype.h"
#include <string.h>
#include "shm.h"
#include "lk_hashmap.h"
#include "lk_vector.h"
#include "marriagetype.h"
#include "sceneobj_define.h"


class CMarriageInfo : public CObj
{
public:
	char mGroomName[NAME_LENGTH];	// 新郎名称
	char mBrideName[NAME_LENGTH];			// 新娘名称
	unsigned int  mGroomID;					// 新郎角色ID	
	unsigned int  mBrideID;					// 新娘角色ID		
	int  mMarriageStatus;					// 婚姻状态
	int  mWeddingTime;						// 婚礼举行的时间
	int  mWeddingTimeLength;				// 婚礼举行的时间长度
	int  mCampSequence;						// 阵营第几对
	int  mServerSequence;					// 全服第几对
	int	 mWeddingModule;					// 婚礼规模	
	int	 mCampID;							// 阵营ID
	int  mWeddinNpcID[MAX_WEDDING_NPCNUM];	// 婚礼npc的实体ID
	int	 mWeddingNpcPosIndex[MAX_WEDDING_NPCNUM];	// 婚礼npc移动到位置索引
	int	 mSynchMark[MAX_WEDDING_NPCNUM];	// 到达的同步点标志
	int	 mGroomKeepsakeIndex;				// 新郎的结婚信物的索引
	int  mGroomKeepsakeStatus;				// 新郎的结婚信物的状态
	int	 mBrideKeepsakeIndex;				// 新娘的结婚信物的索引
	int  mBrideKeepsakeStatus;				// 新娘的结婚信物的状态
	int  mWeddingEventStatus;				// 婚礼的事件状态
	int  mWaitSynMark;						// 正在等待的同步标志ID
	
public:
	CMarriageInfo()
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
	
	virtual int Initialize();
	virtual int Resume();
public:

	// 设置&获取新郎名称
	void SetGroomName( const char *pName )
	{
		strncpy( mGroomName, pName, NAME_LENGTH - 1 );
		mGroomName[NAME_LENGTH-1] = '\0';
	}
	const char *GetGroomName( ){ return mGroomName; } 
	
	// 设置&获取新郎名称
	void SetBrideName( const char *pName )
	{
		strncpy( mBrideName, pName, NAME_LENGTH - 1 );
		mBrideName[NAME_LENGTH - 1] = '\0';
	}
	
	const char *GetBrideName(){ return mBrideName; }
	
	// 设置&获取新郎的角色ID
	void SetGroomID( int nGroomID ){ mGroomID = nGroomID; }
	unsigned int GetGroomID(){ return mGroomID; }
	
	// 设置&获取新娘的角色ID
	void SetBrideID( int nBrideID ){ mBrideID = nBrideID; }
	unsigned int	GetBrideID( ){ return mBrideID; }
	
	// 设置&获取婚姻状态
	void SetMarriageStatus( int nStatus ){ mMarriageStatus = nStatus; }
	int GetMarriageStatus(){ return mMarriageStatus; }
	
	// 设置&获取婚礼的举行时间
	void SetWeddingTime( int nTime ){ mWeddingTime = nTime; }
	int GetWeddingTime(){ return mWeddingTime; }
	
	// 设置&获取婚礼举行的时间长度
	void SetWeddingTimeLength( int nTimeLength ){ mWeddingTimeLength = nTimeLength; }
	int GetWeddingTimeLength(  ){ return mWeddingTimeLength; }
	
	// 设置&获取婚礼规模
	void SetWeddingModule( int nWeddingModule ){ mWeddingModule = nWeddingModule; }
	int	GetWeddingModule(){ return mWeddingModule; }
	
	// 设置阵营序列
	void SetCampSequence( int nSequence ){ mCampSequence = nSequence; }
	int	GetCampSequence( ){ return mCampSequence; }
	
	// 设置服务序列
	void SetServerSequence( int nSequence ){ mServerSequence = nSequence; }
	int GetServerSequence( ){ return mServerSequence; }
	
	// 设置&获取阵营ID
	void SetCampID( int nCampID ){ mCampID = nCampID; }
	int	GetCampID( ){ return mCampID; }
	
	// 设置&获取新郎的信物索引
	void SetGroomKeepsakeIndex( int nIndex ){ mGroomKeepsakeIndex = nIndex; }
	int GetGroomKeepsakeIndex(){ return mGroomKeepsakeIndex; }
	
	// 设置&获取新娘的信物索引
	void SetBrideKeepsakeIndex( int nIndex ){ mBrideKeepsakeIndex = nIndex; }
	int GetBrideKeepsakeIndex(){ return mBrideKeepsakeIndex; } 
	
	// 设置&获取婚礼的事件
	void SetWeddingEventStatus( int nStatus ){ mWeddingEventStatus = nStatus; }
	int	GetWeddingEventStatus( ){ return mWeddingEventStatus; }
	
	// 设置&获取信物索引
	void SetKeepsakeIndex( int nIndex, int nSex )
	{	
		if ( nSex == 2 )
		{
			mGroomKeepsakeIndex = nIndex;
		}
		else
		{
			mBrideKeepsakeIndex = nIndex;
		}
	}
	
	int GetKeepsakeIndex( int nSex )
	{	
		if ( nSex == 2 )
		{
			return mGroomKeepsakeIndex;		
		}
		else
		{
			return mBrideKeepsakeIndex;
		}
	}
	
	// 设置&获取新郎结婚信物的状态
	void SetGroomKeepsakeStatus( int nSatus ){ mGroomKeepsakeStatus = nSatus; } 
	int GetGroomKeepsakeStatus(){ return mGroomKeepsakeStatus; } 
	
	// 设置&获取新郎结婚信物的状态
	void SetBrideKeepsakeStatus( int nStatus ){ mBrideKeepsakeStatus = nStatus; } 
	int GetBrideKeepsakeStatus(){ return mBrideKeepsakeStatus; } 
	
		
	// 设置&获取结婚信物的状态
	void SetKeepsakeStatus( int nStatus, int nSex )
	{	
		if ( nSex == 2 )
		{
			mGroomKeepsakeStatus = nStatus;
		}
		else
		{
			mBrideKeepsakeStatus = nStatus;
		}
	}
	int GetKeepsakeSatus( int nSex )
	{
		if ( nSex == 2 )
		{
			return mGroomKeepsakeStatus;
		}
		else
		{
			return mBrideKeepsakeStatus;
		}			
	}
	
	// 设置&获取Npc的实体ID
	void SetWeddingNpcID( int nPos, int nNpcID )
	{
		if ( nPos >= 0 && nPos < (int)ARRAY_CNT( mWeddinNpcID ) )
		{
			mWeddinNpcID[nPos] = nNpcID;
		}
	}
	int GetWeddingNpcID( int nPos )
	{
		if ( nPos >= 0 && nPos < (int)ARRAY_CNT( mWeddinNpcID ) )
		{
			return mWeddinNpcID[nPos];
		}
		return 0;
	}
	
	// 初始化婚礼npc的ID
	int InitializeNpcID()
	{
		memset( mWeddinNpcID, 0, sizeof( mWeddinNpcID ) );
		return SUCCESS;
	}
	
	// 设置&获取npc的移动位置索引
	void SetWeddingNpcPosIndex( int nPos, int nIndex )
	{
		if ( nPos >= 0 && nPos < (int)ARRAY_CNT( mWeddingNpcPosIndex ) )
		{
			mWeddingNpcPosIndex[nPos] = nIndex;
		}
	}
	
	int GetWeddingNpcPosIndex( int nPos )
	{
		if ( nPos >= 0 && nPos < (int)ARRAY_CNT( mWeddingNpcPosIndex ) )
		{
			return mWeddingNpcPosIndex[nPos];
		}
		return -1;
	}

	
	// 初始化婚礼npc的移动位置索引
	int InitializeNpcPosIndex()
	{
		memset( mWeddingNpcPosIndex, 0, sizeof( mWeddingNpcPosIndex ) );
		return SUCCESS;
	}
	
	// 设置同步标志
	void SetSynchMark( int nPos, int nMark )
	{
		if ( nPos >= 0 && nPos < (int)ARRAY_CNT( mWeddingNpcPosIndex ) )
		{
			mSynchMark[nPos] = nMark;
		}
	}
	
	// 获取同步标志
	int GetSynchMark( int nPos )
	{
		if ( nPos >= 0 && nPos < (int)ARRAY_CNT( mWeddingNpcPosIndex ) )
		{
			return mWeddingNpcPosIndex[nPos];
		}
		return -1;	
	}
	
	// 初始化同步标志
	void InitializeSyncMark()
	{
		memset( mSynchMark, 0, sizeof( mSynchMark ) );	
	}
	
	// 检查是否所有点都到达了需要同步的位置
	bool CheckSynchMark( );
	
	// 设置&获取同步标志
	void SetWaitSynMark( int nMark ){ mWaitSynMark = nMark;  }
	int GetWaitSynMark( ){ return mWaitSynMark; }
		
public:
	
	// 判断某个结婚时间是否和现在的时间相互冲突
	bool JudgeTimeValide( int nTime, int nTimeLength );	
	
	// 获取婚礼的举行状态	
	int GetWeddingStatus();
	
	// 初始化婚礼信息
	void InitialWeddingInfo();
};	

// 婚礼举行时npc的行走路线
struct CNpcPos
{
public:
	int mPosX;
	int mPosY;
	int mSynchMark;		// 同步标志	
public:
	CNpcPos()
	{
		if ( CObj::msCreateMode )
		{
			mPosX = 0;
			mPosY = 0;
		}
	} 	
};
struct CWeddingNpcPath
{
public:
	CNpcPos mNpcPath[MAX_WEDDINGNPCPATH_STEPS];
	int		mNpcTmpID;
	int		mMapID;
	int		mMapIndex;
public:
	CWeddingNpcPath( )
	{
		if ( CObj::msCreateMode )
		{
			memset( mNpcPath, 0, sizeof( mNpcPath ) );
			mNpcTmpID = 0;
			mMapID = 0;
			mMapIndex = 0;
		}
		else
		{
	
		}
	}
};

struct CWeddingNpcInfo
{
public:
	  CWeddingNpcPath mNpcPathInfo[MAX_WEDDING_NPCNUM];
public:
	CWeddingNpcInfo( )
	{
		if ( CObj::msCreateMode )
		{
			memset( mNpcPathInfo, 0, sizeof( mNpcPathInfo ) );
		}
		else
		{
			
		}
	}
};

class CCampInfo
{
public:
	int mCampID;
	int mMarriageNum;
public:
	CCampInfo()
	{
		if ( CObj::msCreateMode )
		{
			mCampID = 0;
			mMarriageNum = 0;
		}
		else
		{
			
		}
	}
	~CCampInfo() { }
public:
	void SetMarriageNum( int nMarriageNum ){ mMarriageNum = nMarriageNum; }
	int GetMarriageNum( ){ return mMarriageNum; }
	
	// 设置&获取阵营ID
	void SetCampID( int nCampID ){ mCampID = nCampID; }
	int	GetCampID( ){ return mCampID; }
};		


// 婚礼的系统可选时间段
struct CWeddingSysTime
{
public:	
	int mBeginTime;				// 开始时间
	int mEndTime;			// 时间段的长度		
	int	mCommonAllowed;			// 普通婚礼是否可用
	
public:
	CWeddingSysTime()
	{
		mBeginTime		= 0;
		mEndTime		= 0;
		mCommonAllowed	= 0;
	}
};					 

class CMarriageManager : public CSingleton< CMarriageManager >
{
public:
	CMarriageManager()
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
	~CMarriageManager(){ }
public:
	static CSharedMem* mShmPtr; // 共享内存指针
public:
	int Initialize();		
	int Resume();	
public:	
	void *operator new( size_t nSize );
	void operator delete( void *pPointer );
	static unsigned int CountSize();	
public:
	typedef lk::hash_map< int, int, SERVER_CAP_MARRIAGEINFO >	MarriageInfoList;				// 婚姻信息列表	
	typedef lk::vector< int, SERVER_CAP_MARRIAGEINFO >		MarriageApplyInfo;					// 所有待申请的婚姻信息
	typedef lk::hash_map< int, CWeddingSysTime, MAX_WEDDNGTIMEAREA_NUM > SysWeddingTimeList;	// 系统婚礼时间段
	typedef lk::hash_map< int, int, MAX_WEDDING_TIMERNUM >	WeddingTimeList;					// 婚礼定时器列表
private:
	MarriageInfoList mMarriageInfoList;															// 本阵营所有的婚姻信息
	MarriageInfoList mWeddingList;																// 本阵营所有待举办婚礼的婚姻信息
	CCampInfo		 mCampInfo;																	// 阵营信息
	int				 mCurretnWeddingID;															// 目前正在举行婚礼的婚姻信息ID
	MarriageApplyInfo mMarriageApplyInfo;	
	MarriageInfoList mOtherCampMarriageInfo;													// 在本服的其他阵营的玩家的婚姻信息	
	CWeddingNpcInfo	mNpcPath[MAX_WEDDING_MODULENUM];											// npc的路径信息	
	SysWeddingTimeList mWeddingTimeInfo;														// 系统的婚礼时间段信息
	WeddingTimeList		mWeddingTimer;															// 婚礼定时器
public:	

	// 获取&设置正在举行婚礼的婚姻信息ID
	int GetCurrentWeddingID(){ return mCurretnWeddingID; }
	void SetCurrentWeddingID( int nWeddingID ){ mCurretnWeddingID = nWeddingID; }
	
	// 获取正在举行婚礼的婚姻信息
	CMarriageInfo * GetCurrentWeddingInfo()
	{
		return GetMarriageInfo( mCurretnWeddingID );
	}
	
	// 获取阵营信息
	CCampInfo *GetCampInfo( ){ return &mCampInfo; }
	
	// 创建一个婚姻信息
	CMarriageInfo * CreatMarriageInfo( const char *pGroomName, int nGroomID, const char *pBrideName, int nBrideID, int nWeddingTime, int nWeddingTimeLength, int nWeddingModule, int nCampID );					
	
	// 插入一个玩家的婚姻信息
	int InsertMarriageInfo( int nServerSequence, CMarriageInfo *tpMarriageInfo );
	
	// 获取玩家的婚姻信息
	CMarriageInfo * GetMarriageInfo( int nServerSequenceID );
	
	// 获取某个玩家的信息，通过角色ID
	CMarriageInfo * GetMarriageByRoleID( unsigned int nRoleID );	
	
	// 删除某个婚姻信息
	int DeleteMarriageInfo( int nServerSequenceID );
	
	// 获取当前可以进行婚礼的家族
	CMarriageInfo *GetWeddingMarriageInfo();
	
	// 获取本阵营的所有婚姻信息的列表
	MarriageInfoList &GetMarriageInfoList(){ return mMarriageInfoList; }
	
	// 获取本阵营所有待举行婚礼的信息列表
	MarriageInfoList &GetWeddingList(){ return mWeddingList; }
	
	// 检查婚礼时间是否符合要求
	bool CheckWeddingTime( int nWeddingTime, int nWeddingTimeLength );
	
	// 检查是否和已经结婚的玩家的婚礼时间冲突
	bool CheckMarriageWeddingTime( int nWeddingTime, int nWeddingTimeLength );
	
	// 获取和当前时间点冲突的婚姻信息
	CMarriageInfo *GetMarriageInfoConfictedWithTime( int nWeddingTime, int nWeddingTimeLength );
	
	// 检查是否和申请结婚的玩家的婚礼时间冲突
	bool CheckMarriageApplyWeddingTime( int nWeddingTime, int nWeddingTimeLength );
	
	// 获取某个待申请的婚姻信息
	CMarriageInfo *GetMarriageApplyInfo( unsigned int nRoleID );
	
	// 获取玩家的婚姻信息
	CMarriageInfo *GetMarriageInfoByMarriageID( int nMarriageID );
	
	// 创建一个待申请的婚姻信息
	CMarriageInfo * CreatMarriageApplyInfo( int nGroomID, int nBrideID, int nWeddingTime, int nWeddingTimeLength, int nWeddingModule );			
	
	// 删除一个待申请的婚姻信息
	int DeleteMarriageApplyInfo( unsigned int nRoleID );	
	
	// 记录一个其他国家玩家的婚姻信息
	int InsertOtherCampMarriageInfo( CMarriageInfo *pMarriageInfo );  	
	
	// 记录一个其他国家玩家的婚姻信息
	void DeleteOtherCampMarriageInfo( int nMarriageID ); 
	
	// 获取一个在本阵营的其他阵营玩家的婚姻信息
	CMarriageInfo *GetMarriageInfoOfOtherCamp( int nMarriageID ); 
	
	// 获取玩家的婚姻信息
	CMarriageInfo* GetMarriageInfoByID( int nCampID, int nMarriageID );	
	
	// 获取npc是否到达目标点
	bool CheckNpcPos( int nNpcPosIndex, int nWeddingModule, int nNpcIndex, int nNpcPosX, int nNpcPosY, bool &rReachEnd );
	
	// 获取某种规模的npc路径信息	
	CWeddingNpcInfo *GetWeddingNpcInfo( int nWeddingModule );	
	
	// 获取某种规模的npc的路径信息
	
	// 获取npc的移动坐标点
	int GetNpcMovePos( int nWeddingModule,  int nNpcIndex, int nNpcPosIndex, CNpcPos &rNpcPos );
	
	// 插入一个npc的路径信息
	int InsertNpcPath( int nWeddingModule, CWeddingNpcPath &rNpcPath );
	
	
	// 插入一个系统婚礼时间段
	int InsertSysWeddingTimeInfo( int nTimeID, CWeddingSysTime &rWeddingTime );	
	
	// 获取指定ID的婚礼时间段
	CWeddingSysTime* GetWeddingTimeInfo( int nTimeID );
	
	// 删除一个系统时间段
	void DeleteWeddingTimeInfoByID( int nTimeID );
	
	// 获取时间列表
	SysWeddingTimeList *GetWeddingTimeInfoList(){ return &mWeddingTimeInfo; }
	
	// 初始化婚礼时间配置
	int InitialWeddingTimeInfo();
	
	// 初始化npc的路径信息
	int InitialNpcPathInfo( );
	
	// 设置婚礼定时器
	int SetWeddingTimer( int nTimeMark,	int nTimerID );
	
	// 获取婚礼定时器
	int GetWeddingTimerID( int nTimerMark );
	
	// 清除婚礼定时器
	int ClearWeddingTimer( int nTimerMark );
	
	// 清除所有定时器
	int ClearAllWeddingTime();
};