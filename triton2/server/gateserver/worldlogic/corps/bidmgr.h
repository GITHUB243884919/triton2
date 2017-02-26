//================================================================================
// bidmgr.h
//================================================================================
#pragma  once
#include "corpstype.h"
//#include "module_manager.h"
#include "family.h"
#include "familytype.h"
#include "shm.h"
#include "lk_hashmap.h"
#include "lk_vector.h"
#include "gateobj_define.h"
#include "corpsinfo_pb.hxx.pb.h"   
#include "entity.h"

#define MAX_BID_FINISH_DELAY 30 //竞价结束触发的最大延迟时间，单位分钟
//#define ID_SWAP 10000000
#define MAPID_SWAP					1000000000	//ID_SWAP*100
#define OCP_MAINCITY				1	// 主城标识
#define OCP_SUBCITY					2	// 非主城标识
#define GET_DBMAPID(type, mapid)	(type*MAPID_SWAP + mapid)

class CCorps;

/// 军团竞价信息
struct CorpsBidInfo
{
	int m_NpcID;
	int m_CorpsID;
	int m_BidMoney;
	int m_Status;
	int m_BidTime;
	int	m_SynTag;			// 与数据库同步0新数据  1同步 2update数据
};

enum E_DATASYNTAG
{
	DataTag_New = 0,	//新数据
	DataTag_Syn,		//已经同步的数据
	DataTag_Upd,		//有更新的数据
	DataTag_Del		//删除的数据
};


struct NpcStatus
{
	int m_OwnerID;
	enum E_BIDSTATE
	{
		STATE_BidOpen,
		STATE_BidClose,
		STATE_WaitBattle,
		STATE_Inbattle,
		STATE_MaxValue
	};
	int m_Status;
	int m_BidMoneyRank1;	//排名第一的竞价价格
	int m_BidMoneyRank2;	//排名第二的竞价价格
	int m_CorpsIDRank1;
	int m_CorpsIDRank2;
	int m_MapID;
	int m_WelfareTime;
	int m_OccupyTime;
	int m_SynTag;

	bool m_TmpTag;
	int m_TmpDesCorpID;
	int m_TmpBidMoney;
	int m_TmpNpcStat;
	int m_TmpWarStat;
	int m_NpcID;
	
	int m_nMapOccupyType;		//城池的占领类型
};



/// 竞价管理类
class CBidMgr : public CSingleton< CBidMgr >
{
public:
	CBidMgr();
	~CBidMgr();
	int Initialize( );
	int Resume( );
	static CSharedMem* mShmPtr;
	void * operator new( size_t tsize );
	void   operator delete( void *pPointer );
	static unsigned int CountSize();

	int m_nDataState; // 0,正常状态 1,迁移状态 
	
public:
	void LoadCorpsBidFromDB( int nStart, int nTotalCount );
	void LoadNpcStatusFromDB( int nStart, int nTotalCount );
	void OnSessionLoadCorpsBid( CMessage* pMessage, int nStart, int nTotalCount );
	void OnSessionLoadNpcStatus( CMessage* pMessage, int nStart, int nTotalCount );
	int SynDataToDB();

	void OnGMChangeNpcState(int nServeID, int nSceneNpcID, int nStateCode);

	/// 添加NPC状态，只在冷启动时会被外部调用
	int AddNpcStateInfo(int nNpcID, NpcStatus *pNpc);
public:
	void OnTimerCheckState();
public:
	//int OnEventClearBidList( int nNpcID, int* vpCorpsList, int* vpBidMoney, int& vSize );

	int OnEventRollAllBidList();
	int OnEventRollCorpsNpc( int nCorpsID, int nNpcID, int nServerID );


	void SendCorpsNpcList( int nServerID );
private:
	/// 所有NPC的竞价信息
	typedef lk::vector<CorpsBidInfo, SERVER_CAP_CORPS> BidInfoList;
	BidInfoList m_BidInfoList;

	/// NPC状态表
	typedef lk::hash_map<int, NpcStatus, CORPS_NPC_CAP> NpcStatusMap;
	NpcStatusMap m_NpcStatusMap;

private:

	NpcStatusMap::iterator m_itNpcTmp;

public:
	int OnGetBattleMapList( int* npList, int* npCorspList, int& nNum, int nServerID );
	int SetNpcOwner(int nGateNpcID, int nCorpsID);
	int SetNpcOwnerByMapID(int nGateMapID, int nCorpsID);
	bool SetMainCity(int nCorpsID, int nSceneMapID);
	int GetMainCity(int nCorpsID);
public:
	int GetMapListByCorps(CCorps* pCorps, int* pnMapList, int& nNum);
	//int GetAllBidMoneyByCorpsID(int nCorpsID);
	///返回NPC状态
	NpcStatus* GetNpcStatus(int nGateNpcID);
	///拼装军团的争夺战状态属性
	void MakeCorpsBattleState(int nCorpsID, CorpsProperty* pProperty);
	
	void SetNpcStatusBegin();
	NpcStatus* GetOneNpcTmpState();

	/// 返回指定Npc的竞价列表
	int GetBidListByNpcID(int nNpcID, int* pnCorpsList, int* pnBidMoney, int& nNum);
	
	//开放指定NPC的竞价
	void SetBidOpen(int nGateNpcID, bool bOpen = true);

	//-------------------------------------------------------------------------
	/// 查找指定NPC的占领军团ID
	///	@param nMapID
	/// @return 军团ID，0表示未被占领
	int FindNpcOwnerByNpcID(int nGateNpcID);
	//-------------------------------------------------------------------------
	/// 查找指定NPC的占领军团ID
	///	@param nMapID
	/// @ret 军团ID，0表示未被占领
	int FindCityOwnerByMapID(int nGateMapID, int &nNpcID);
	//-------------------------------------------------------------------------
	/// 查找指定的MapID对应的NpcID
	///	@param nSceneMapID
	/// @ret NpcID，0表示没找到
	int GetNpcIDByMapID(int nSceneMapID);

	//-------------------------------------------------------------------------
	/// 对争霸战竞价
	/// @return 0表示竞价成功，小于0为错误码。
	int BidForBattle(CCorps* pCorps, int nRoleID, int nNpcID, int nMapID, int nBidMoney);

	int OnBidFinishTimesUp(int nNpcID, CCorps* &pAttacker, CCorps* &pDefender, int& nMoneyAtt, int& nMoneyDef);

	// 判断当前军团是否参与竞价或占领了城池在被竞价中
	bool IsInBid(int nCorpsID, int nType);
private:
	int ChangeNpcBidRank(NpcStatus* pNpcStat, int nCorpsID, int nMoney);

	int DeleteNpcBidList(int nNpcID, int nCorpsID1 = 0, int nCorpsID2 = 0);

	//如果Npc不在状态表中，预置该Npc信息
	int PreSetNpcState(int nGateNpcID);

	//根据竞价列表重新计算NPC的竞价前两名
	void ReCalculeteBidRank(int nNpcID, int nServerID);

	// 判断指定时间是否在某个指定时间范围内
	inline bool InTimeZone(tm* pLocal, int nEventDay, int nEventHour, int nEventMin, int nDelayMin);
};

