
#pragma once

#define MAX_STOREBOX_LEVEL 5

#include <vector>
#include <list>
#include <map>
#include "tinyxml.h"
#include "servertool.h"

class CYqsCfgPlayerRankPrizeUnit
{
public:
    int miRank;
    int miMailId;
    int miPrizeItem;
    int miPrizeItemNum;
public:
    CYqsCfgPlayerRankPrizeUnit(){}
    ~CYqsCfgPlayerRankPrizeUnit(){}
    CYqsCfgPlayerRankPrizeUnit& operator = ( CYqsCfgPlayerRankPrizeUnit& theObj );
};

class CYqsCfgLevelInfoUnit
{
public:
    int miBuff;
    int miStoreBoxType;
    int miNum;
public:
    CYqsCfgLevelInfoUnit(){};
    ~CYqsCfgLevelInfoUnit(){};
    CYqsCfgLevelInfoUnit& operator=( CYqsCfgLevelInfoUnit& theObj );
};

class CYqsCfgPrizeInfoUnit
{
public:
    int miGrowing;
    int miRandom;
    int miPrizeItemId;
    int miPrizeItemNum;
public:
    CYqsCfgPrizeInfoUnit(){};
    ~CYqsCfgPrizeInfoUnit(){};
    CYqsCfgPrizeInfoUnit& operator=( CYqsCfgPrizeInfoUnit& theObj );
};

class CYqsCfgSettllUnit
{
public:
    int     miTime;
    int     miCampRankLimite;
    int     miNoticeId;
    int     miEncourage;
public:
    CYqsCfgSettllUnit(){};
    ~CYqsCfgSettllUnit(){};
    CYqsCfgSettllUnit& operator = ( CYqsCfgSettllUnit& theObj );
};

class CHandInEncourageUnit
{
public:
    int miItemType;
    int miItemId;
    int miItemNum;
    int miEncourageType;
    int miEncourageId;
    int miEncourageNum;
public:
    CHandInEncourageUnit(){};
    ~CHandInEncourageUnit(){};
    CHandInEncourageUnit& operator = ( CHandInEncourageUnit& theObj );
};

class CCampPrize
{
public:
    int miRank;
    int miDoubleExpTime;
    int miDoubleExpRate;
public:
    CCampPrize(){};
    ~CCampPrize(){};
    CCampPrize& operator = ( CCampPrize& theObj )
    {
        miRank = theObj.miRank;
        miDoubleExpTime = theObj.miDoubleExpRate;
        miDoubleExpRate = theObj.miDoubleExpRate;
        return *this;
    }
};

class CStoreBox
{
public:
    int         miNpcID;
    int         miLineID;
    int         miMapID;
    CStoreBox(){};
    ~CStoreBox(){};
    CStoreBox& operator= (CStoreBox& theObj)
    {
        miNpcID     = theObj.miNpcID;
        miLineID    = theObj.miLineID;
        miMapID     = theObj.miMapID;
        return *this;
    }
};

class CActivityYqsCfg : public CSingleton< CActivityYqsCfg >
{
public:
    bool LoadConfigure(  );
    bool LoadConfigureFromXml( const char* szFileName );

    // 摇钱树上缴奖励配置信息
public:
    typedef std::map< int, CHandInEncourageUnit >                              t_handin_encourage_type;
    typedef t_handin_encourage_type::iterator                                  t_handin_encourage_itor;
    typedef t_handin_encourage_type::value_type                                t_handin_encourage_value;
    typedef std::pair<t_handin_encourage_itor, bool>                           t_handin_encourage_pairib;
    t_handin_encourage_type                                                    mYQSHandin;
    int                                                                        miTemplateid;

    bool LoadHandinEncourageFromTemple(  );
    CHandInEncourageUnit* GetHandInEncourage( int iItemId );

// 摇钱树基本配置信息
public:
    bool                                mbIsUsingYaoQingShu;
    int                                 miYaoQianShuNPCID;
    int                                 miLineForYqs;
    int                                 miMapForYqs;
    int                                 miShanCaiTongZhiNPCID;
    int                                 miLineForSctz;
    int                                 miMapForSctz;
    int                                 miHandinNumLimitePerUser;
    int                                 miLevelExpTemplete;

    bool LoadBaseYaoQianShuInfo( TiXmlElement* pRoot );

// 摇钱树等级配置信息
public:
    std::vector< CYqsCfgLevelInfoUnit >            mLevelInfo;

    bool        LoadLevelInfoFromXml( TiXmlElement* pRoot );
    CYqsCfgLevelInfoUnit* GetLevelInfo( int nLevel );

// 摇钱树浇灌成长度配置信息
public:
    typedef std::map< int, int >                t_growing_map;
    typedef t_growing_map::iterator             t_growing_map_itor;
    typedef t_growing_map::value_type           t_growing_map_value;
    typedef std::pair<t_growing_map_itor, bool> t_growing_map_pairib;
    t_growing_map                       mdawGrowing;
    int                                 miBaseGrowing;

    bool    LoadDawGrowingInfoFromXml( TiXmlElement* pRoot );
    int     GetGrowingValue( int iDawItemid );

// 摇钱树成长消息
public:
    typedef std::map< int, int >            t_notice_map;
    typedef t_notice_map::iterator          t_notice_map_itor;
    typedef t_notice_map::value_type        t_notice_map_value;
    typedef std::pair<t_notice_map_itor, bool> t_notice_map_pairib;
    t_notice_map                            mGrowingNotice;

    bool    LoadGrowingNoticeFromXml( TiXmlElement* pRoot );

// 摇钱树中奖奖励信息
public:
    std::vector< CYqsCfgPrizeInfoUnit >                mPrizeInfo;
    int                                     miPrizeCondition;

    bool    LoadPrizeInfoFromXml( TiXmlElement* pRoot );

// 摇钱树宝箱配置信息
public:
    std::vector< CStoreBox >                       mStoreBoxList[MAX_STOREBOX_LEVEL];

    bool                                            LoadStoreBoxFromXml( TiXmlElement* pRoot );
    std::vector<CStoreBox>                          GetStoreBoxs( int nType );
    bool                                            IsStoreBox( int nLevel, int nStoreBoxEntityId );
    bool GetStoreBox( int nStoreBoxID, CStoreBox& theStoreBox );

// 摇钱树用户贡献度排名配置信息
public:
    int                                     miPlayerRankInfoMaxLimite;
    int                                     miPlayerRankInfoUpdateDelay;
    typedef std::map< int, CYqsCfgPlayerRankPrizeUnit >       t_map_playrankinfo;
    typedef t_map_playrankinfo::iterator                     t_map_playrankinfo_itor;
    typedef t_map_playrankinfo::value_type                   t_map_playrankinfo_value;
    typedef std::pair<t_map_playrankinfo_itor, bool>         t_map_playrankinfo_pairib;
    t_map_playrankinfo                       mPlayerRankInfoPrizes;
public:
    bool LoadCfgForPlayerRankPrizeFromXml( TiXmlElement* pRoot );
    CYqsCfgPlayerRankPrizeUnit*          GetPlayerPrize( int nRank )
    {
        t_map_playrankinfo_itor loItor = mPlayerRankInfoPrizes.find( nRank );
        if ( loItor == mPlayerRankInfoPrizes.end() )
        {
            return NULL;
        }
        return &loItor->second;
    }

// 摇钱树阵营奖励
public:
    typedef std::map< int, CCampPrize >                   t_map_campprize;
    typedef t_map_campprize::iterator                     t_map_campprize_itor;
    typedef t_map_campprize::value_type                   t_map_campprize_value;
    typedef std::pair<t_map_campprize_itor, bool>         t_map_campprize_pairib;
    t_map_campprize                                       mCampPrizes;
public:
    bool LoadCfgForCampPrize( TiXmlElement* pRoot );
    CCampPrize* GetCampPrize( int nRank );
};
