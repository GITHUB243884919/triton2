
#pragma once

#include <string>
#include <list>
#include <map>
#include <vector>
#include "tinyxml.h"
#include "servertool.h"
#include "activitymessag_pb.hxx.pb.h"
#include "servermessage_pb.hxx.pb.h"
#include "entity.h"
#include "ActivityYqsCfg.h"

#define MAX_PLAER_RANK_NUM 50

class CYaoQianShuCampInfoUnit
{
public:
    int miId;                           // 活动标示
    int miActivityID;                   // 活动ID
    int miCampId;                       // 阵营ID
    int miCampRank;                     // 阵营排名
    int miGrowingValue;                 // 成长度
    int miLevel;                        // 成长等级
    int miHandinDawNum;                 // 上缴仙露数
    int miSettlePhase;                  // 结算阶段
    std::string mstrStartTime;          // 活动开启时间
    std::string mstrEndTime;            // 活动结束时间
public:
    CYaoQianShuCampInfoUnit();;
    ~CYaoQianShuCampInfoUnit(){};
    CYaoQianShuCampInfoUnit& operator = ( CYaoQianShuCampInfoUnit& theObj );
    void clear();
};

class CActivityYqs : public CSingleton< CActivityYqs >
{
public:
    CActivityYqs();;
    ~CActivityYqs(){};

public:
    typedef std::map<int, int>              t_map;
    typedef t_map::iterator                 t_map_itor;
    typedef t_map::value_type               t_map_value;
    typedef std::pair< t_map_itor, bool >   t_map_pair;

public:
    bool                                mbIsLoaded;
    bool                                mbIsStarted;
    CYaoQianShuCampInfoUnit             moYaoQianShuCampInfo;
    std::vector< CStoreBox >            mStoreBoxCreated;
    t_map                               mGrowingNotice;
    bool                                mbIsFinished;
    
    int GetActivtyId( void );
    int GetSettlePhase( void );

public:
    // 启动服务器
    virtual void Initialize( );

public:
    void CheckActivity( int nActivityId, int nState );
    bool StartActivity(  );
    bool EndActivity(  );
    bool IsYaoQianShuActiviy( int nActivityId );

public:
    void OnMessageSettleNotice( CMessage* npMessage );
    void OnMessageHandinRequest(CEntityPlayer *npPlayer, CMessage *npMessage);
    void OnMessageUpdateYQSGrowingResponse( CMessage* npMessage );
    void OnMessageActivityStartNotice( CMessage* npMessage );
    void OnMessageGetCampRankInfoForClient( CEntityPlayer *npPlayer, CMessage *npMessage );
    void OnMesageGetCampRankInfoForGate( CMessage* npMessage );

    bool SendGrowingValueToGate( int nPlayerId, int iCampId, int iGrowingValue );
    void SendEncourageNoticeBroadcast( int nCampId, int nEncourageId );
    void SendPhaseChangeNotice( int nCampId, int nIsFinished, int nCurrLevel );
    void SendDropItemNotice( int nPlayerId, int nItemId, int nItemNum );
    void SendHandinRequest( int nPlayerId, int nResult, int nPlayerHandinNum, int nPlayerContribute  );
    void SendActivityStart( int nActivityID, int iCampID );

private:
    void CheckGrowingNotice( int nGrowingValue );
    void EncourageItem( int nPlayerId, int nGrowing );
    int  GetLevelExp( int nLevel );
    void CreateStoreBox( int nLevel );
    void ChangeYaoQianShuState( int nLevel );
    void SettleFinish( void );
};

class CPlayerRankInfoUnit
{
public:
    int         miPlayerID;             // 角色编号
    std::string mstrPlayerName;         // 角色名称
    int         miPlayerMetie;          // 角色门派
    int         miPlayerLevel;          // 角色等级
    int         miContribute;           // 角色贡献度
public:
    CPlayerRankInfoUnit(){};
    ~CPlayerRankInfoUnit(){};
    CPlayerRankInfoUnit& operator = ( CPlayerRankInfoUnit& theObj );
    void Clear( void );
};

class CActivityYqsPlayerRankInfo : public CSingleton< CActivityYqsPlayerRankInfo >
{
public:
    typedef std::vector< CPlayerRankInfoUnit >   t_vec_rankinfo;
    typedef t_vec_rankinfo::iterator             t_vec_rankinfo_itor;
    typedef std::list< int >                     t_list_freeunit;
    typedef t_list_freeunit::iterator            t_list_freeunit_itor;
    typedef std::list< int >                     t_list_usedunit;
    typedef t_list_usedunit::iterator            t_list_usedunit_itor;
    typedef std::multimap< int, int >            t_map_rank;
    typedef t_map_rank::iterator                 t_map_rank_itor;
    typedef t_map_rank::reverse_iterator         t_map_rank_ritor;
    typedef t_map_rank::value_type               t_map_rank_value;
    typedef std::pair< t_map_rank_itor, bool >   t_map_rank_pairib;
    typedef std::map< int, int >                 t_map_player;
    typedef t_map_player::iterator               t_map_player_itor;
    typedef t_map_player::value_type             t_map_player_value;
    typedef std::pair< t_map_player_itor, bool > t_map_player_pairib;

public:
    CPlayerRankInfoUnit             mPlayerRankInfos[MAX_PLAER_RANK_NUM];
    t_list_freeunit                 mFreeRankUnit;
    t_list_usedunit                 mUsedRankUnit;
    t_map_rank                      mPlayerRank;
    t_map_player                    mPlayerInfo;
    int                             mMaxPlayerRanks;
    time_t                          mLastUpdateTime;
    int                             mTimeDelay;

public:
    void InitializeForYqsPlayerRank( int nMaxPlayerRanks );
    void OnTime(  );

    void Clear( void );

public:
    bool UpdatePlayerRankInfo( CPlayerRankInfoUnit& oPlayerInfo );
    bool InsertPlayerRankInfo( CPlayerRankInfoUnit& oPlayerInfo ); 
    void GivePlayerRankPrize( void );
    
    bool LoadPlayerRankInfoFromDB( void );
    void OnSessionLoadPlayerRankInfoDB( CMessage *npMessage, int nParam1, int nParam2 );

    void OnMessageGetPlayerRankInfoRequest( CEntityPlayer *npPlayer, CMessage *npMessage );
private:
    int                     GetPlayerRankInfoNum( void );
    CPlayerRankInfoUnit*    GetMinContribute( void );
    CPlayerRankInfoUnit*    GetPlayRankInfo( int nPlayerId );

private:
    int     GetFreeUnit ( void  );
    void    Release     ( int nPos );
    void    Sort        ( void  );
};




