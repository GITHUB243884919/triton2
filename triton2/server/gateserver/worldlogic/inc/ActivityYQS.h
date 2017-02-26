
#pragma once

#define MAX_ACTIVIT_YAOQIANSHU_LEVEL 5
#define MAX_CAMP_NUM        5

#include <string>
#include <vector>
#include "module_manager.h"
#include "entity.h"
#include "lk_vector.h"
#include "lk_hashmap.h"
#include "lk_string.h"
#include "gateobj_define.h"
#include "lk_set.h"
#include "SysProtocol.h"
#include "property_pb.hxx.pb.h"
#include "gmmessage_pb.hxx.pb.h"
#include "activitytype.h"
#include "servermessage_pb.hxx.pb.h"
#include "servertool.h"

using namespace GateServer;

#define MAX_SETTLE_PHASE 10

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
    time_t      mtStarttimer;
    int         miTimeDelay;
public:
    CYaoQianShuCampInfoUnit();;
    ~CYaoQianShuCampInfoUnit(){};
    CYaoQianShuCampInfoUnit& operator = ( CYaoQianShuCampInfoUnit& theObj );
    void clear();

    void UpdateCampInfo( void );

    void StartSettle( void );
    void ResetTimer( time_t tTimNow, int nTimeDelay );
    void OnSettleTimer( void );

    bool SendSettleNoticeToSence( void );
};

class CYaoQianShuSettleInfoUnit
{
public:
    int miPhase;                        // 结算阶段
    int miTimeDelay;                    // 到达此阶段的时间言辞
    int miCampRankLimit;                // 阵营排名限制
    int miNoticeCode;                   // 发送消息码
    int miNoticeParam1;                 // 消息参数
    int miNoticeParam2;                 // 消息参数
    int miNoticeParam3;                 // 消息参数
    int miEncourageId;                  // 第几次奖励

public:
    CYaoQianShuSettleInfoUnit();
    ~CYaoQianShuSettleInfoUnit(){};
    CYaoQianShuSettleInfoUnit& operator=( CYaoQianShuSettleInfoUnit& theObj );
};

class CYaoQianShuSettleInfo : public CSingleton< CYaoQianShuSettleInfo >
{
public:
    int                                                         miSettleTime;
    int                                                         miTimeDelayForEnd;          // 结束时所需的时间延迟
    std::vector< CYaoQianShuSettleInfoUnit >                    mSettlePhase;

    CYaoQianShuSettleInfoUnit* GetSettlePhase( int nSettlePhase );

    int GetLastPhase( );
    bool CheckSettlePhase( void );
    bool LoadSettlePhaseFromXML( TiXmlElement* tpRoot );
};

class CActivityYQS : public CDataModule, public CSingleton< CActivityYQS >
{
public:
    CYaoQianShuCampInfoUnit   moCampYaoQianShuInfo[MAX_CAMP_NUM];

public:
     bool   mbIsUsingYaoQianShu;
     bool   mbIsStarted;
     bool   mbIsLauched;
     int    mLevel[MAX_ACTIVIT_YAOQIANSHU_LEVEL];

     static unsigned int CountSize();


public:
    CActivityYQS();
    ~CActivityYQS(){};

    typedef std::multimap<int, int>         t_map;
    typedef t_map::iterator                 t_map_itor;
    typedef t_map::reverse_iterator         t_map_ritor;
    typedef t_map::value_type               t_map_value;

private:
    CWTimer mCheckTimer; // 检查间隔


public:
    // 启动服务器
    virtual void OnLaunchServer( );

    bool IsAllStart( void );

    bool IsAllEnd( void );

    void Sort( void );

    bool StartActivity( std::string strStartTime );
    bool EndActivity( std::string strEndTime );

    bool OnSessionActivityEnd( CMessage* tpMessage, int Param1, int nParam2 );
    bool OnSessionActivityStart( CMessage* tpMessage, int nParam1, int nParam2 );

    bool IsYaoQianShu( int iActivityId );

    bool LoadActivityYQSConfigFromXml( const char* pFileName );

    // 退出服务器
    virtual void OnExitServer( );

    // 服务器Timer
    virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );

    // 客户端发送消息
    virtual void OnMessage( int nServerID, CMessage* pMessage );;

    // 重新加载配置文件
    virtual void ReloadCfg();

    // 启动是否完成
    virtual bool IsLaunched();

    std::string ConverTime( int nTime );
    bool SendSettleNoticeToSence( int nCampId );
    bool IsPassed( int nTime );
    void OnMessageGrowing( int nServerID, CMessage* pMessage );
    void OnMessageGetCampRank( int nServerID, CMessage* pMessage );
    void SendResultForUpdateYQSGrowing( int nServerId, int nResult, int nPlayerId, int nCampId, int nGrowing );
    void SendYaoQianShuAvtivityStartNotice( int nCampId );
    void SendResultForGetCampRankForYQS( int nServerId, int nResult, int nPlayerId, int nParam );

	// 加载摇钱树配置的回调函数
	static void LoadActivityYQSConfigCallback(const char* pcConfigPath);
};
