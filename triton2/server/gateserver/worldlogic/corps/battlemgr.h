//================================================================================
// battlemgr.h
//================================================================================
#pragma  once
#include "module_manager.h"
#include "corpstype.h"
#include "corps.h"
#include "family.h"
#include "familytype.h"
#include "shm.h"
#include "lk_hashmap.h"
#include "gateobj_define.h"
#include "corpsinfo_pb.hxx.pb.h"   
#include "entity.h"


//#include "gatelogic.h"
//#include "corpservice.h"
//using namespace GateServer;	
//战争管理类
class CBattleMgr : public CSingleton< CBattleMgr >
{
public:
	CBattleMgr();
	~CBattleMgr();
	int Initialize( );
	int Resume( );
	static CSharedMem* mShmPtr;
	void * operator new( size_t tsize );
	void   operator delete( void *pPointer );
	static unsigned int CountSize();

public:
	void OnTimerCheckState();

public:
	///数据库同步操作
	void LoadFromDB( int nStart, int nTotalCount );
	//void GetNumFromDB();
	void OnSessionLoadFromDB( CMessage* pMessage, int nStart, int nTotalCount );
	//void OnSessionGetNumFromDB();
	int SynDataToDB();

public:
	///初始化一个新的战争信息
	int InitBattle(int nAttCorpsID, int nDefCorpsID, int nType, int nNpcID, int nMapID);
	void StartBattle( KEY_TYPE nKey, int nNpcID );
	void GetBattle( KEY_TYPE nKey, int& nSrcCorpsID, int& nDesCorpsID, int& nType, int& nNpcID, int& nMapID );
	int GetBattle( int nNpcID, int &nSrcCorpsID, int& nDesCorpsID, int& nMapID );
	int DelBattle( int nNpcID );
	int DelBattle( KEY_TYPE nKey );

	bool InChallegeBattle( int nCorpsID );
	KEY_TYPE GetBattleKey( int nNpcID, int nType );
	bool IsInBattle( int nNpcID );
	bool IsInBattleByCorpsID( int nCorpsID, int nTypeTag);
	int GetBattleBeginTime( int nNpcID, int nType );

	void SendJoinNotice();
	//判断军团是否正在战斗中
	//bool CorpsIsInBattle(int nCorpsID);
protected:
private:
	class BattleInfo
	{
	public:
		KEY_TYPE	m_Key;				// 副本key
		int			m_AttCorpsID;		// 攻击方ID
		int			m_DefCorpsID;		// 防守方ID
		int			m_Type;				// 副本类型
		int			m_NpcID;			// NpcID
		int			m_MapID;			// 地图ID
		int			m_BattleTime;		// 战争开始时间
		int			m_SynTag;			// 与数据库同步0新数据  1同步 2update数据

	public:
		BattleInfo( ){ m_Key = INVITED_KEY_VALUE; m_AttCorpsID = 0; m_DefCorpsID = 0; m_Type = 0; m_NpcID = 0; m_MapID = 0; m_BattleTime=0; }
		BattleInfo( KEY_TYPE nKey, int nAttCorpsID, int nDefCorpsID, int nType, int nNpcID, int nMapID ) : m_Key( nKey ),
			m_AttCorpsID( nAttCorpsID ), m_DefCorpsID( nDefCorpsID ), m_Type( nType ), m_NpcID( nNpcID ), m_MapID( nMapID ),m_BattleTime(0){ }
		~BattleInfo( ){ }
	};
	typedef lk::vector< BattleInfo, SERVER_CAP_CORPS > BattleInfoList;	///< 战争列表from mgr
	BattleInfoList m_BattleInfoList;
	
public:
	void OnEventBattleOver(KEY_TYPE nKey);
	CCorps* GetBattleCorpsByCampID(KEY_TYPE nKey, int nCampID, int& nNpcID);

};

