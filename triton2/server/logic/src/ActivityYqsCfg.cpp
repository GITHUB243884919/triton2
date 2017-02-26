
#include "base.h"
#include "log.h"
#include "template.h"
#include "protype.h"
#include "creator.h"
#include "ActivityYqsCfg.h"
#include "ActivityYqs.h"
#include "scenelogic.h"

/////////////////////////////////////////////////////////////////////////////////////////

template<>CActivityYqsCfg*            CSingleton<CActivityYqsCfg>::spSingleton            = new CActivityYqsCfg;

/////////////////////////////////////////////////////////////////////////////////////////

CYqsCfgLevelInfoUnit& CYqsCfgLevelInfoUnit::operator=( CYqsCfgLevelInfoUnit& theObj )
{
    miBuff              = theObj.miBuff;
    miStoreBoxType      = theObj.miStoreBoxType;
    miNum               = theObj.miNum;
    return *this;
}

CYqsCfgPlayerRankPrizeUnit& CYqsCfgPlayerRankPrizeUnit::operator=( CYqsCfgPlayerRankPrizeUnit& theObj )
{
    miRank              = theObj.miRank;
    miMailId            = theObj.miMailId;
    miPrizeItem         = theObj.miPrizeItem;
    miPrizeItemNum      = theObj.miPrizeItemNum;
    return *this;
}

CYqsCfgPrizeInfoUnit& CYqsCfgPrizeInfoUnit::operator=( CYqsCfgPrizeInfoUnit& theObj )
{
    miGrowing     = theObj.miGrowing;
    miRandom      = theObj.miRandom;
    miPrizeItemId   = theObj.miPrizeItemId;
    miPrizeItemNum  = theObj.miPrizeItemNum;
    return *this;
}

CYqsCfgSettllUnit& CYqsCfgSettllUnit::operator=( CYqsCfgSettllUnit& theObj )
{
    miTime              = theObj.miTime;
    miCampRankLimite    = theObj.miCampRankLimite;
    miNoticeId          = theObj.miNoticeId;
    miEncourage         = theObj.miEncourage;
    return *this;
}

CHandInEncourageUnit& CHandInEncourageUnit::operator=( CHandInEncourageUnit& theObj )
{
    miItemType      = theObj.miItemType;
    miItemId        = theObj.miItemId;
    miItemNum       = theObj.miItemNum;
    miEncourageType = theObj.miEncourageType;
    miEncourageId   = theObj.miEncourageId;
    miEncourageNum  = theObj.miEncourageNum;
    return *this;
}

/////////////////////////////////////////////////////////////////////////////////////////

bool CActivityYqsCfg::LoadConfigure()
{
  /*  bool lbRetForLoadFromXml = LoadConfigureFromXml( "../config/data/activity/ActivityYQS.xml" );*/
	  bool lbRetForLoadFromXml = LoadConfigureFromXml( CSceneLogic::GetSingleton().GetConfigDataFileNamePath(CSceneLogic::GetSingletonPtr()->GetGameID(),const_cast<char*>("activity/ActivityYQS.xml")) );
    if ( lbRetForLoadFromXml == false )
    {
        return false;
    }
    bool lbRetForLoadEncourageFromTemplate = LoadHandinEncourageFromTemple( );
    if ( lbRetForLoadEncourageFromTemplate == false )
    {
        return false;
    }

    return true;
}

bool CActivityYqsCfg::LoadConfigureFromXml( const char* szFileName )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start Load configure from xml!", __LK_FILE__, __LINE__, __FUNCTION__ );

    TiXmlDocument xmldoc;
    xmldoc.LoadFile( szFileName );

    TiXmlElement* tpRoot= xmldoc.FirstChildElement();
    if (tpRoot == NULL)
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't Get root!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    bool lbRetForLoadBaseInfo = LoadBaseYaoQianShuInfo( tpRoot );
    if ( lbRetForLoadBaseInfo == false )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] LoadBaseYaoQianShuInfo failed!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    bool lbRetForLevelInfo  = LoadLevelInfoFromXml( tpRoot );
    if ( lbRetForLevelInfo == false )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] LoadLevelInfoFromXml failed!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    bool lbRetForLoadDawGrowing = LoadDawGrowingInfoFromXml( tpRoot );
    if ( lbRetForLoadDawGrowing == false )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] LoadDawGrowingInfoFromXml failed!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    bool lbRetForLoadGrowingNotice = LoadGrowingNoticeFromXml( tpRoot );
    if ( lbRetForLoadGrowingNotice == false )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] LoadGrowingNoticeFromXml failed!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    bool lbRetForLoadPrizeInfo  = LoadPrizeInfoFromXml( tpRoot );
    if (  lbRetForLoadPrizeInfo == false)
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] LoadPrizeInfoFromXml failed!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    bool lbRetForLoadStoreBox = LoadStoreBoxFromXml( tpRoot );
    if ( lbRetForLoadStoreBox == false )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] LoadStoreBoxFromXml failed!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    bool lbRetForLoadPlayerRankInfo = LoadCfgForPlayerRankPrizeFromXml( tpRoot );
    if ( lbRetForLoadPlayerRankInfo == false )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] LoadCfgForPlayerRankInfoFromXml failed!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
   }

    bool lbRetForLoadCampPrize = LoadCfgForCampPrize( tpRoot );
    if ( lbRetForLoadCampPrize == false )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] LoadCfgForCampPrize failed!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
   }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish Load configure from xml!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return true;
}

bool CActivityYqsCfg::LoadCfgForPlayerRankPrizeFromXml( TiXmlElement* pRoot )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] LoadCfgForPlayerRankInfoFromXml Started!", __LK_FILE__, __LINE__, __FUNCTION__ );

    if ( pRoot == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get root!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    mPlayerRankInfoPrizes.clear();

    TiXmlElement* lpPlayerRankInfo = pRoot->FirstChildElement( "PlayerRankInfo" );
    if ( lpPlayerRankInfo==NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get PlayerRankInfo!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    const char* lpszMaxLimite   = lpPlayerRankInfo->Attribute( "MaxLimite" );
    if ( lpszMaxLimite == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get MaxLimite!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    miPlayerRankInfoMaxLimite   = atoi( lpszMaxLimite );
    const char* lpszUpdateDelay = lpPlayerRankInfo->Attribute( "UpdateDelay" );
    if ( lpszUpdateDelay == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get UpdateDelay!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    miPlayerRankInfoUpdateDelay = atoi( lpszUpdateDelay );

    TiXmlElement* lpPrize = lpPlayerRankInfo->FirstChildElement( "Prize" );
    if ( lpPrize==NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get prize!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    for ( ; lpPrize!=NULL; lpPrize=lpPrize->NextSiblingElement() )
    {
        const char* lpRank = lpPrize->Attribute( "Rank" );
        if ( lpRank == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get rank!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        const char* lpMailId = lpPrize->Attribute( "mailid" );
        if ( lpMailId==NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get TitleReel!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        const char* lpPrizeItem = lpPrize->Attribute( "prizeitem" );
        if ( lpMailId==NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get TitleReel!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        const char* lpPrizeItemNum= lpPrize->Attribute( "prizeitemnum" );
        if ( lpMailId==NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get TitleReel!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }

        CYqsCfgPlayerRankPrizeUnit          loPlayerRankInfo;
        loPlayerRankInfo.miRank             = atoi( lpRank );
        loPlayerRankInfo.miMailId           = atoi( lpMailId );
        loPlayerRankInfo.miPrizeItem        = atoi( lpPrizeItem );
        loPlayerRankInfo.miPrizeItemNum     = atoi( lpPrizeItemNum );

        LOG_NOTICE( "YaoQianShu", 
            "[ %s:%d ][ %s ] Player rank prize info! "
            "miRank(%d) miMailId(%d) miPrizeItem(%d) miPrizeItemNum(%d)", 
            __LK_FILE__, __LINE__, __FUNCTION__, 
            loPlayerRankInfo.miRank, loPlayerRankInfo.miMailId, 
            loPlayerRankInfo.miPrizeItem, loPlayerRankInfo.miPrizeItemNum );

        t_map_playrankinfo_pairib lPair = mPlayerRankInfoPrizes.insert( t_map_playrankinfo_value( loPlayerRankInfo.miRank, loPlayerRankInfo ) );
        if ( lPair.second == false )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] insert player rankinfo cfg failed!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
    }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] LoadCfgForPlayerRankInfoFromXml finished!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return true;
}

bool CActivityYqsCfg::LoadStoreBoxFromXml( TiXmlElement* pRoot )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] LoadStoreBoxFromXml Started!", __LK_FILE__, __LINE__, __FUNCTION__ );
    if ( pRoot == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get root!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    for ( int i=0;i<MAX_STOREBOX_LEVEL; i++ )
    {
        mStoreBoxList[i].clear();
    }

    TiXmlElement* lpStoreBoxs = pRoot->FirstChildElement( "StoreBoxs" );
    if ( lpStoreBoxs == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get StoreBoxs!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    TiXmlElement* lpStoreBox    = lpStoreBoxs->FirstChildElement( "StoreBox" );
    if ( lpStoreBox == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get StoreBox!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    for ( ;lpStoreBox!=NULL; lpStoreBox= lpStoreBox->NextSiblingElement() )
    {
        const char* lpStoreBoxType = lpStoreBox->Attribute( "Level" );
        if ( lpStoreBoxType==NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get Level!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        const char* lpNpcId         = lpStoreBox->Attribute( "id" );
        if ( lpNpcId==NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get NpcId!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        const char* lpMapID         = lpStoreBox->Attribute( "map" );
        if ( lpMapID==NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get map!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }

        CStoreBox                   loStoreBox;
        int liStoreBoxType          = atoi( lpStoreBoxType );
        loStoreBox.miNpcID          = atoi( lpNpcId );
        loStoreBox.miMapID          = atoi( lpMapID );

        if ( liStoreBoxType<=0 || liStoreBoxType>=MAX_STOREBOX_LEVEL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] the storebox level is invalied!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }

        std::vector< CStoreBox >& lStoreBoxList    = mStoreBoxList[ liStoreBoxType ];
        lStoreBoxList.push_back( loStoreBox );
    }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] LoadStoreBoxFromXml Finished!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return true;
}

bool CActivityYqsCfg::LoadBaseYaoQianShuInfo( TiXmlElement* pRoot )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] LoadBaseYaoQianShuInfo started!", __LK_FILE__, __LINE__, __FUNCTION__ );

    if ( pRoot == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get root!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    TiXmlElement* lpIsUsing = pRoot->FirstChildElement( "IsUsingYQSActivity" );
    if ( lpIsUsing == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get IsUsingYQSActivity!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    const char* lpszUsingFlag   = lpIsUsing->GetText();
    mbIsUsingYaoQingShu         = ::atoi( lpszUsingFlag )==1?true:false;

    TiXmlElement* lpLevelExpTemp = pRoot->FirstChildElement( "LevelExpTpl" );
    if ( lpLevelExpTemp == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get IsUsingYQSActivity!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    const char* lpszLevelExpTpl = lpLevelExpTemp->GetText();
    if ( lpszLevelExpTpl == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get lpszLevelExpTpl!", __LK_FILE__, __LINE__, __FUNCTION__ );
    }
    miLevelExpTemplete          = ::atoi( lpszLevelExpTpl );

    TiXmlElement* lpNPCS    = pRoot->FirstChildElement( "NPCS" );
    if ( lpNPCS == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get NPCS!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    TiXmlElement* lpNPC     = lpNPCS->FirstChildElement( "NPC" );
    if ( lpNPC == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get NPC!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    for ( ;lpNPC!=NULL; lpNPC = lpNPC->NextSiblingElement() )
    {
        const char* lpNpcName = lpNPC->Attribute( "name" );
        if ( lpNpcName==NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get Name!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        const char* lpNpcId   = lpNPC->Attribute( "id" );
        if ( lpNpcId==NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get id!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        const char* lpmap    = lpNPC->Attribute( "map" );
        if ( lpmap == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get id!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }

        std::string lstrNpcName = lpNpcName;
        if ( lstrNpcName == "YaoQianShu" )
        {
            miYaoQianShuNPCID   = atoi( lpNpcId );
            miMapForYqs         = atoi( lpmap );
        }
        if ( lstrNpcName == "ShanCaiTongZhi" )
        {
            miShanCaiTongZhiNPCID   = atoi(  lpNpcId );
            miMapForSctz            = atoi( lpmap );
        }
    }

    TiXmlElement* lpExpPrize = pRoot->FirstChildElement( "ExpPrize" );
    if ( lpExpPrize == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get ExpPrize!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    const char* lpszHandinLimite = lpExpPrize->Attribute( "limite" );
    if ( lpszHandinLimite == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get HandinLimite!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    miHandinNumLimitePerUser    = atoi( lpszHandinLimite );

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] LoadBaseYaoQianShuInfo finished!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return true;
}

bool CActivityYqsCfg::LoadLevelInfoFromXml( TiXmlElement* pRoot )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] LoadLevelInfoFromXml Started!", __LK_FILE__, __LINE__, __FUNCTION__ );

    if ( pRoot == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get root!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    mLevelInfo.clear();
    CYqsCfgLevelInfoUnit loLevelInfo;
    mLevelInfo.push_back( loLevelInfo );

    TiXmlElement* lpLevels = pRoot->FirstChildElement( "Levels" );
    if ( lpLevels == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get Levels!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    TiXmlElement* lpLevel   = lpLevels->FirstChildElement( "Level" );
    if ( lpLevel == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get Levels!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    for ( int i=1; lpLevel!=NULL; lpLevel=lpLevel->NextSiblingElement(), i++ )
    {
        const char* lpLevelId   = lpLevel->Attribute( "id" );
        if ( lpLevelId == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get id!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        const char* lpBuff      = lpLevel->Attribute( "buff" );
        if ( lpBuff == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get Buff!", __LK_FILE__, __LINE__, __FUNCTION__  );
            return false;
        }
        const char* lpSbType    = lpLevel->Attribute( "storeboxtype" );
        if ( lpSbType == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get storeboxtype!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        const char* lpNum       = lpLevel->Attribute( "num" );
        if ( lpNum == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get num!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }

        int liLevelId = atoi( lpLevelId );
        if ( liLevelId!=i )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] the level id(%d:%d) is invalied!", __LK_FILE__, __LINE__, __FUNCTION__, liLevelId,i );
            return false;
        }
        CYqsCfgLevelInfoUnit loLevelInfo;
        loLevelInfo.miBuff            = atoi( lpBuff );
        loLevelInfo.miStoreBoxType    = atoi( lpSbType );
        loLevelInfo.miNum             = atoi( lpNum );

        mLevelInfo.push_back( loLevelInfo );  
    }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] LoadLevelInfoFromXml Finished!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return true;
}

CYqsCfgLevelInfoUnit* CActivityYqsCfg::GetLevelInfo( int nLevel )
{
    if ( nLevel<0 || nLevel>(int)mLevelInfo.size() )
    {
        return NULL;
    }
    return &mLevelInfo[nLevel];
}

bool CActivityYqsCfg::LoadDawGrowingInfoFromXml( TiXmlElement* pRoot )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] LoadDawGrowingInfoFromXml Started!", __LK_FILE__, __LINE__, __FUNCTION__ );
    if ( pRoot == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get root!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    mdawGrowing.clear();

    TiXmlElement* lpGrowing = pRoot->FirstChildElement( "Growing" );
    if ( lpGrowing == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get Growing!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    const char* lpszBase = lpGrowing->Attribute( "Base" );
    if ( lpszBase == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get Base!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    miBaseGrowing   = atoi( lpszBase );
    TiXmlElement* lpDaw = lpGrowing->FirstChildElement( "Daw" );
    if ( lpDaw == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get Daw!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    for ( ; lpDaw!=NULL; lpDaw= lpDaw->NextSiblingElement() )
    {
        const char* lpId    = lpDaw->Attribute( "id" );
        if ( lpId == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get id!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        std::string lpGrowingValue = lpDaw->GetText();
        int liId            = atoi( lpId );
        int liGrowingValue  = atoi( lpGrowingValue.c_str() );

        t_growing_map_pairib lPairlb = mdawGrowing.insert( t_growing_map_value( liId, liGrowingValue ) );
        if ( lPairlb.second == false )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] insert GrowingValue failed!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
    }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] LoadDawGrowingInfoFromXml finished!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return true;
}

int CActivityYqsCfg::GetGrowingValue( int iDawItemid )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] get daw(%d)'s growing value!", __LK_FILE__, __LINE__, __FUNCTION__, iDawItemid );
    t_growing_map_itor loItor = mdawGrowing.find( iDawItemid );
    if ( loItor == mdawGrowing.end() )
    {
        LOG_ERROR(  "YaoQianShu", "[ %s:%d ][ %s ] Can't daw(%s)'s growing value!", __LK_FILE__, __LINE__, __FUNCTION__, iDawItemid );
        return 0;
    }

    return (loItor->second*miBaseGrowing);
}

bool CActivityYqsCfg::LoadGrowingNoticeFromXml( TiXmlElement* pRoot )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] LoadGrowingNoticeFromXml Started!", __LK_FILE__, __LINE__, __FUNCTION__ );

    if ( pRoot == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get root!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    mGrowingNotice.clear();

    TiXmlElement* lpGrowingNotice = pRoot->FirstChildElement( "GrowingNotice" );
    if ( lpGrowingNotice==NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get GrowingNotice!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    TiXmlElement* lpNotice = lpGrowingNotice->FirstChildElement( "Notice" );
    if ( lpNotice==NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get Notice!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    for ( ; lpNotice!=NULL; lpNotice=lpNotice->NextSiblingElement() )
    {
        const char* lpValue = lpNotice->Attribute( "Value" );
        if ( lpValue == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get Value!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        std::string lpNoticeCode = lpNotice->GetText();

        int liValue         = atoi( lpValue );
        int liNoticeCode    = atoi( lpNoticeCode.c_str() );

        t_notice_map_pairib lPair = mGrowingNotice.insert( t_notice_map_value( liValue, liNoticeCode ) );
        if ( lPair.second == false )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] insert growing notice failed!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
    }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] LoadGrowingNoticeFromXml finished!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return true;
}

bool CActivityYqsCfg::LoadPrizeInfoFromXml( TiXmlElement* pRoot )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] LoadPrizeInfoFromXml Started!", __LK_FILE__, __LINE__, __FUNCTION__ );
    if ( pRoot == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get root!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    mPrizeInfo.clear();

    TiXmlElement* lpPrizes = pRoot->FirstChildElement( "Prizes" );
    if ( lpPrizes == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get Prizes!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    const char* lpszPrizeCondition =  lpPrizes->Attribute( "condition" );
    if ( lpszPrizeCondition == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get condition!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    miPrizeCondition = atoi( lpszPrizeCondition );

    TiXmlElement* lpPrize = lpPrizes->FirstChildElement( "Prize" );
    if ( lpPrize == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get Prize!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    for ( ; lpPrize!=NULL; lpPrize=lpPrize->NextSiblingElement() )
    {
        const char* lpGrowing = lpPrize->Attribute( "Growing" );
        if ( lpGrowing == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get Growing!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        const char* lpRandom = lpPrize->Attribute( "Random" );
        if ( lpRandom == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get Random!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        const char* lpPrizeId           = lpPrize->Attribute( "id" );
        if ( lpPrizeId == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get id!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        const char* lpNum               = lpPrize->Attribute( "num" );
        if ( lpNum == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get num!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }

        CYqsCfgPrizeInfoUnit        loPrizeInfo;
        loPrizeInfo.miGrowing       = atoi( lpGrowing );
        loPrizeInfo.miRandom        = atoi( lpRandom );
        loPrizeInfo.miPrizeItemId   = atoi( lpPrizeId );
        loPrizeInfo.miPrizeItemNum  = atoi( lpNum );

        mPrizeInfo.push_back( loPrizeInfo );
    }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] LoadPrizeInfoFromXml Finished!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return true;
}

bool CActivityYqsCfg::LoadHandinEncourageFromTemple()
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start Load HandinEncourage from template!", __LK_FILE__, __LINE__, __FUNCTION__ );
    CTplHandinTable* lpHandinTable  = static_cast<CTplHandinTable*>(CDataStatic::GetHandinTable( HELPID_YAOQIANSHU ));
    if ( lpHandinTable==NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get handin table from template!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    mYQSHandin.clear();

    miTemplateid    = lpHandinTable->mTempID;
    for ( int i=0; i<MAX_HANDINENCOURAGE_NUM; i++ )
    {
        int liHandinEncourageTplId = lpHandinTable->mEncourageTplId[i];
        if ( liHandinEncourageTplId == 0 )
        {
            continue;
        }

        CTplHandinEncourage* lpHandinEncourage = static_cast< CTplHandinEncourage* >(CDataStatic::GetHandinEncourage( liHandinEncourageTplId ));
        if ( lpHandinEncourage == NULL )
        {
            LOG_ERROR( "YaoQianShu", 
                "[ %s:%d ][ %s ] Can't get HandinEncourage(%d) from template!", 
                __LK_FILE__, __LINE__, __FUNCTION__, liHandinEncourageTplId );
            return false;
        }

        CHandInEncourageUnit loHandInEncourageUnit;
        loHandInEncourageUnit.miItemType            = lpHandinEncourage->mHandinItemType;
        loHandInEncourageUnit.miItemId              = lpHandinEncourage->mHandinItemID;
        loHandInEncourageUnit.miItemNum             = lpHandinEncourage->mHandinItemNum;
        loHandInEncourageUnit.miEncourageType       = lpHandinEncourage->mEncourageType;
        loHandInEncourageUnit.miEncourageId         = lpHandinEncourage->mEncourageId;
        loHandInEncourageUnit.miEncourageNum        = lpHandinEncourage->mEncourageNum;

        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Get handin encourage info from templete(%d)! "
            "miItemType(%d) miItemId(%d) miItemNum(%d) miEncourageType(%d) miEncourageId(%d) miEncourageNum(%d)", 
            __LK_FILE__, __LINE__, __FUNCTION__, liHandinEncourageTplId,
            loHandInEncourageUnit.miItemType, loHandInEncourageUnit.miItemId, loHandInEncourageUnit.miItemNum,
            loHandInEncourageUnit.miEncourageType, loHandInEncourageUnit.miEncourageId, loHandInEncourageUnit.miEncourageNum );

        t_handin_encourage_pairib loRet = mYQSHandin.insert( t_handin_encourage_value( loHandInEncourageUnit.miItemId, loHandInEncourageUnit ) );
        if ( loRet.second == false )
        {
            LOG_ERROR( "YaoQianShu", 
                "[ %s:%d ][ %s ] insert HandinEncourage(%d) failed!", 
                __LK_FILE__, __LINE__, __FUNCTION__, liHandinEncourageTplId );
            return false;
        }
    }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish Load HandinEncourage from template!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return true;
}

CHandInEncourageUnit* CActivityYqsCfg::GetHandInEncourage( int iItemId )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start get item(%d)'s encourage!", __LK_FILE__, __LINE__, __FUNCTION__, iItemId );
    t_handin_encourage_itor loItor = mYQSHandin.find( iItemId );
    if ( loItor == mYQSHandin.end() )
    {
        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Can't get item(%d)'s encourage!", __LK_FILE__, __LINE__, __FUNCTION__, iItemId );
        return NULL;
    }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish get item(%d)'s encourage!", __LK_FILE__, __LINE__, __FUNCTION__, iItemId );
    return &loItor->second;
}

std::vector<CStoreBox> CActivityYqsCfg::GetStoreBoxs( int nType )
{
    if ( nType<0 || nType>MAX_STOREBOX_LEVEL )
    {
        std::vector<CStoreBox> lTemp;
        return lTemp;
    }
    return mStoreBoxList[nType];
}

bool CActivityYqsCfg::LoadCfgForCampPrize( TiXmlElement* pRoot )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] LoadCfgForCampPrize Started!", __LK_FILE__, __LINE__, __FUNCTION__ );

    if ( pRoot == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get root!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    mCampPrizes.clear();

    TiXmlElement* lpCampPrizes = pRoot->FirstChildElement( "CampPrizes" );
    if ( lpCampPrizes==NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get lpCampPrizes!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    TiXmlElement* lpPrize = lpCampPrizes->FirstChildElement( "CampPrize" );
    if ( lpPrize==NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get prize!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    for ( ; lpPrize!=NULL; lpPrize=lpPrize->NextSiblingElement() )
    {
        const char* lpRank = lpPrize->Attribute( "Rank" );
        if ( lpRank == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get rank!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        const char* lpDoubltExpTime = lpPrize->Attribute( "doubleexptime" );
        if ( lpDoubltExpTime==NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get lpDoubltExpTime!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        const char* lpDoubltExpRate = lpPrize->Attribute( "doubleexprate" );
        if ( lpDoubltExpRate==NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get lpDoubltExpRate!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }

        CCampPrize loCampPrize;
        loCampPrize.miRank          = atoi( lpRank );
        loCampPrize.miDoubleExpTime = atoi( lpDoubltExpTime );
        loCampPrize.miDoubleExpRate = atoi( lpDoubltExpRate );

        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Get Camp prize! "
            "miRank(%d) miDoubleExpTime(%d) miDoubleExpRate(%d)", 
            __LK_FILE__, __LINE__, __FUNCTION__,
            loCampPrize.miRank, loCampPrize.miDoubleExpTime, loCampPrize.miDoubleExpRate );

        t_map_campprize_pairib lPair = mCampPrizes.insert( t_map_campprize_value( loCampPrize.miRank, loCampPrize ) );
        if ( lPair.second == false )
        {
            LOG_ERROR( "YaoQianShu", 
                "[ %s:%d ][ %s ] insert player campprize(%d) cfg failed!", 
                __LK_FILE__, __LINE__, __FUNCTION__, loCampPrize.miRank );
            return false;
        }
    }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] LoadCfgForCampPrize finished!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return true;
}

CCampPrize* CActivityYqsCfg::GetCampPrize( int nRank )
{
    t_map_campprize_itor loItor = mCampPrizes.find( nRank );
    if ( loItor == mCampPrizes.end() )
    {
        return NULL;
    }
    return &loItor->second;
}

bool CActivityYqsCfg::IsStoreBox( int nLevel, int nStoreBoxIndex )
{
    std::vector<CStoreBox>& loStoreBoxs        = mStoreBoxList[nLevel];
    std::vector<CStoreBox>::iterator        loItor = loStoreBoxs.begin();
    for ( int i=0; loItor!= loStoreBoxs.end(); loItor++, i++ )
    {
        CStoreBox loStoreBox = loStoreBoxs[i];

        if ( loStoreBox.miNpcID == nStoreBoxIndex )
        {
            return true;
        }
    }
    return false;
}

bool CActivityYqsCfg::GetStoreBox( int nStoreBoxID, CStoreBox& theStoreBox )
{
    for ( int i=1; i<MAX_STOREBOX_LEVEL; i++ )
    {
        std::vector<CStoreBox>& loStoreBoxs        = mStoreBoxList[i];
        std::vector<CStoreBox>::iterator        loItor = loStoreBoxs.begin();
        for ( int i=0; loItor!= loStoreBoxs.end(); loItor++, i++ )
        {
            CStoreBox loStoreBox = loStoreBoxs[i];
            
            if ( loStoreBox.miNpcID == nStoreBoxID )
            {
                theStoreBox = loStoreBox;
                return true;
            }
        }
    }
    return false;
}