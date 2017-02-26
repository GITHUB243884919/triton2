/************************************************************************/
//	Class Name	:	CGateTeam	
//	Description	:	To maintain team data in GateServer;
//					Sustain to redirect scene function of team module;
//	Modifier	:	GaoHong
//	Found Time	:	2008-10-31
/************************************************************************/

#pragma once
#include "base.h"
#include "object.h"
#include "coretype.h"
#include "servertool.h"
#include "entity.h"
#include "lk_vector.h"
#include "teamtype.h"
#include "servermessage_pb.hxx.pb.h"
using namespace std;

#define TEAM_NUMBER_MAX		6				// 队伍人数上限

// 队员头衔
enum MemberTitle
{
	emHeader			= 0,				// 队长
	emStaffer			= 1,				// 队员
};



// 队员BuffList
class CBuffList
{
public:

	enum {MAX_BUFF_NUM = 24};
	
	class CBuffObject
	{
	public:
		unsigned int	mBuffID;		// Buff模版ID
		int				mPersistTime;	// Buff持续时间（毫秒为单位）
	};

	typedef lk::vector< CBuffObject, MAX_BUFF_NUM > BuffList;

	CBuffList();
	~CBuffList();

private:

	BuffList mBuffList;

public:

	void GetTlvBuff( PBBuffList* pBuffList )
	{
		for( BuffList::iterator it = mBuffList.begin(); it != mBuffList.end(); ++it )
		{
			PBBuffObject* pBuffObj = pBuffList->add_buffobjects();
			pBuffObj->set_buffid( (*it).mBuffID );
			pBuffObj->set_persisttime( (*it).mPersistTime );
		}

		pBuffList->set_buffnumber( mBuffList.size() );

	}

	void CreatFromTlv( PBBuffList* pBuffList )
	{
		CBuffObject tBuff;
		mBuffList.clear();

		for( int i = 0; i < pBuffList->buffobjects_size() && i < MAX_BUFF_NUM; i++ )
		{
			PBBuffObject* pPBBuffObj = pBuffList->mutable_buffobjects( i );
			tBuff.mBuffID = pPBBuffObj->buffid();
			tBuff.mPersistTime = pPBBuffObj->persisttime();

			mBuffList.push_back( tBuff );
		}
	}
};


class CGateTeamMember : public CObj
{
private:
	int		mCharID;
	char	mName[ NAME_LENGTH ];
	int		mFace;
	int		mSex;
	int		mMetier;
	int		mLevel;
	int		mHP;
	int		mMP;
	int		mStatus;
	int		mLineID;
	int		mMapID;
	int		mPosX;
	int		mPosY;
	int		mNationality;
	CBuffList mBuffList;
	int		mRepetionMapIndex;
	int		m_nOrder; // 在队伍中的序号
public:
	CGateTeamMember();
	~CGateTeamMember();

	virtual int Initialize();
	virtual int Resume();

	// Set & Get
	inline void SetCharID( int nCharID ) { mCharID = nCharID; }
	inline int	GetCharID() { return mCharID; }

	inline void SetFace( int nFace ) { mFace = nFace; }
	inline int	GetFace() { return mFace; }

	inline void SetSex( int nSex ) { mSex = nSex; }
	inline int	GetSex() { return mSex; }

	inline void SetMetier( int nMetier ) { mMetier = nMetier; }
	inline int	GetMetier() { return mMetier; }

	inline void SetLevel( int nLevel ) { mLevel = nLevel; }
	inline int	GetLevel() { return mLevel; }

	inline void SetHP( int nHP ) { mHP = nHP; }
	inline int	GetHP() { return mHP; }

	inline void SetMP( int nMP ) { mMP = nMP; }
	inline int	GetMP() { return mMP; }

	inline void SetLineID( int nLineID ) { mLineID = nLineID; }
	inline int	GetLineID() { return mLineID; }

	inline void SetMapID( int nMapID ) { mMapID = nMapID; }
	inline int	GetMapID() { return mMapID; }

	inline void SetPosX( int nPosX ) { mPosX = nPosX; }
	inline int	GetPosX() { return mPosX; }

	inline void SetPosY( int nPosY ) { mPosY = nPosY; }
	inline int	GetPosY() { return mPosY; }

	inline void	SetStatus( int nStatus ) { mStatus = nStatus; }
	inline int	GetStatus() { return mStatus; }

	inline void SetNationality( int nNationality ) { mNationality = nNationality; }
	inline int	GetNationality() { return mNationality; }

	inline CBuffList& GetBuffList( ) { return mBuffList; }
	
	void SetName( char* pName ) 
	{
		LK_ASSERT( pName != NULL, return );
		strncpy( mName, pName, sizeof( mName ) - 1 );
	}
	inline char* GetName() { return mName; }

	inline int GetRepetionMapIndex( ) { return mRepetionMapIndex; }
	void SetRepetionMapIndex( int vRepetionMapIndex ) { mRepetionMapIndex = vRepetionMapIndex; }

	inline int GetOrderInTeam() {return m_nOrder;}
	void SetOrderInTeam(int nOrder)
	{
		if (nOrder <= TEAM_NUMBER_MAX)
		{
			m_nOrder = nOrder;
		}
	}
};

class CGateTeam : public CObj
{
public:
	typedef lk::hash_map< int, int, TEAM_NUMBER_MAX > TEAM_TYPE;
	//typedef lk::vector< int, TEAM_NUMBER_MAX > TEAM_TYPE;
	TEAM_TYPE				mGateTeam;

	int		mHeadID;	// 队长
	char	mHeadName[ NAME_LENGTH ];
	TEAM_DISTRIBUTE mMode;	// 队伍分配模式

	typedef lk::vector< int, MAX_INVITE_NUM > REQUEST_LIST;
	REQUEST_LIST mReqList;
public:
	CGateTeam( );
	~CGateTeam( );

	// initial
	virtual int Initialize();
	// resume
	virtual int Resume(); 

public:
	// Set & Get HeadID
	void SetHeadID( int vHeadID ) { mHeadID = vHeadID; }
	int GetHeadID() { return mHeadID; }

	void SetHeadName( char* pName ) 
	{
		LK_ASSERT( pName != NULL, return );
		strncpy( mHeadName, pName, sizeof( mHeadName ) - 1 );
	}
	char* GetHeadName(){ return mHeadName; }

	int InsertReq( int nCharID );
	int RemoveReq( int nCharID );
	inline void ClearReq() { mReqList.clear(); }
	inline int GetReqNum(){ return mReqList.size(); }

	// Set & Get Mode
	void SetMode( TEAM_DISTRIBUTE vMode ) { mMode = vMode; }
	TEAM_DISTRIBUTE GetMode() { return mMode; }

	// Add Member
	int AddMember( CGatePlayer* pPlayer );
	// Del Member
	int DelMember( int vCharID );
	// del team
	int DelTeam();

	// is header
	inline bool IsHeader( int vCharID ){ return ( vCharID == mHeadID ) ? true : false; }

	// Get team count
	inline int GetCount() { return mGateTeam.size(); }

	// get member list
	int GetMemberList( int* pList, int vNum, int& vRtnNum );

	// pack team members' PB
	int MakeTeamPB( PBTeamInfo* pInfo );

	// refresh member value
	int RefreshMemberData( PBTeamInfo* pInfo );
	
	// make member pb
	int MakeMemberPB( int vCharID, PBTeamItem* pItem );


	int MakePbTeamMember( int vCharID, PBMember* pMember );

	// print team info
	void PrintTeam();

	int GetNewOrderInTeam();
};
