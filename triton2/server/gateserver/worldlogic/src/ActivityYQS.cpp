
#include <stdio.h>
#include "tinyxml.h"
#include "gatelogic.h"
#include "protype.h"
#include "ActivityYQS.h"

template<>CActivityYQS*             CSingleton< CActivityYQS >::spSingleton         = NULL;
template<>CYaoQianShuSettleInfo*    CSingleton<CYaoQianShuSettleInfo>::spSingleton  = new CYaoQianShuSettleInfo;

#define TIME_MS_PRE_MIN     60

CYaoQianShuCampInfoUnit::CYaoQianShuCampInfoUnit()
{
    miId            = 0;
    miActivityID    = 0;
    miCampId        = 0;
    miCampRank      = 0;
    miGrowingValue  = 0;
    miLevel         = 0;
    miHandinDawNum  = 0;
    miSettlePhase   = -1;
    mtStarttimer    = 0;
    miTimeDelay     = 0;
}

CYaoQianShuCampInfoUnit& CYaoQianShuCampInfoUnit::operator=( CYaoQianShuCampInfoUnit& theObj )
{
    miId                = theObj.miId;
    miCampId            = theObj.miCampId;
    miActivityID        = theObj.miActivityID;
    miGrowingValue      = theObj.miGrowingValue;
    miLevel             = theObj.miLevel;
    miHandinDawNum      = theObj.miHandinDawNum;
    mstrStartTime       = theObj.mstrStartTime;
    mstrEndTime         = theObj.mstrEndTime;
    miCampRank          = theObj.miCampRank;
    miSettlePhase       = theObj.miSettlePhase;
    mtStarttimer        = theObj.mtStarttimer;
    miTimeDelay         = theObj.miTimeDelay;
    return *this;
}

void CYaoQianShuCampInfoUnit::clear()
{
    miId                    = 0;
    miActivityID            = 0;
    miCampId                = 0;
    miCampRank              = 0;
    miGrowingValue          = 0;
    miLevel                 = 0;
    miHandinDawNum          = 0;
    mstrStartTime           = "";
    mstrEndTime             = "";
    miSettlePhase           = -1;
    mtStarttimer            = 0;
    miTimeDelay             = 0;
}

void CYaoQianShuCampInfoUnit::UpdateCampInfo( void )
{
    char lstrBuf[1024] ={0};
    sprintf(    lstrBuf, 
        "update v_activity_yqs set CurrHandinDawNum=%d, CurrGrowing=%d, CurrLevel=%d, CampRank=%d, SettlePhase=%d where id=%d and CampId=%d;",
        miHandinDawNum,
        miGrowingValue,
        miLevel,
        miCampRank,
        miSettlePhase,
        miId,
        miCampId );

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] UpdateCampInfo(%d) sql(%s)!", __LK_FILE__, __LINE__, __FUNCTION__, miCampId, lstrBuf );
    if ( CGateLogic::GetSingleton().ExecuteSql( em_dbsession_activityyqs_update, 0, 0, 
        0, SQL_NONE, UPDATE, lstrBuf ) == false )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] exec sql %s failed", __LK_FILE__, __LINE__, __FUNCTION__, lstrBuf );
        return;
    }
}

void CYaoQianShuCampInfoUnit::StartSettle( void )
{
    if( miSettlePhase!=-1||miId==0 )
    {
        return;
    }

    mtStarttimer    = time( NULL );
    miSettlePhase   = 0;
    miTimeDelay     = CYaoQianShuSettleInfo::GetSingleton().GetSettlePhase( 0 )->miTimeDelay*TIME_MS_PRE_MIN;

    LOG_NOTICE( "YaoQianShu", 
        "[ %s:%d ][ %s ] StartSettle mtStartTime(%d) miSettlePhase(%d) miTimeDelay(%d)", 
        __LK_FILE__, __LINE__, __FUNCTION__, mtStarttimer, miSettlePhase, miTimeDelay  );
}

void CYaoQianShuCampInfoUnit::ResetTimer( time_t tTimNow, int nTimeDelay )
{
    mtStarttimer    = tTimNow;
    miTimeDelay     = nTimeDelay*TIME_MS_PRE_MIN;
    LOG_NOTICE( "YaoQianShu", 
        "[ %s:%d ][ %s ] ResetTimer mtStartTime(%d) miSettlePhase(%d) miTimeDelay(%d)", 
        __LK_FILE__, __LINE__, __FUNCTION__, mtStarttimer, miSettlePhase, miTimeDelay  );
}

void CYaoQianShuCampInfoUnit::OnSettleTimer( void )
{
    if ( miSettlePhase == -1 || miId==0 )
    {
        return;
    }

    int liTimeNow = time(0);
    if ( mtStarttimer!=0 && liTimeNow-mtStarttimer>miTimeDelay )
    {
        CYaoQianShuSettleInfo* lpYaoQianShuSettleInfo = CYaoQianShuSettleInfo::GetSingletonPtr();
        if ( lpYaoQianShuSettleInfo == NULL )
        {
            LOG_ERROR( "YaoQianShu", 
                "[ %s:%d ][ %s ] Can't get CYaoQianShuSettleInfo!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return;
        }
        int liLastSettlePhase = lpYaoQianShuSettleInfo->GetLastPhase();
        if ( liLastSettlePhase < miSettlePhase )
        {
            mtStarttimer = 0;
            SendSettleNoticeToSence();
            return;
        }

        CYaoQianShuSettleInfoUnit* lpSettleUnit = CYaoQianShuSettleInfo::GetSingleton().GetSettlePhase( miSettlePhase );
        if ( lpSettleUnit == NULL )
        {
            mtStarttimer = 0;
            return;
        }
        if ( lpSettleUnit->miCampRankLimit > miCampRank && miCampRank != 0 )
        {
            SendSettleNoticeToSence();
            miSettlePhase++;
            UpdateCampInfo();

            if ( liLastSettlePhase <= miSettlePhase )
            {
                ResetTimer( liTimeNow, lpYaoQianShuSettleInfo->miTimeDelayForEnd );
            }else
            {
                CYaoQianShuSettleInfoUnit* lpYaoQianShuSettleInfoUnit = lpYaoQianShuSettleInfo->GetSettlePhase( miSettlePhase );
                int liTimeDelay         = lpYaoQianShuSettleInfoUnit->miTimeDelay;
                ResetTimer( liTimeNow, liTimeDelay );
            }
        }else
        {
            miSettlePhase           =liLastSettlePhase+1;
            //SendSettleNoticeToSence();
            UpdateCampInfo();
            ResetTimer( liTimeNow, lpYaoQianShuSettleInfo->miTimeDelayForEnd );
        }

    }
}

bool CYaoQianShuCampInfoUnit::SendSettleNoticeToSence( void )
{
    CMessage tMessage;
    CMessageG2SSettleNotice tPara;
    tMessage.mutable_msghead()->set_messageid( ID_G2S_SETTLE_NOTICE );
    tMessage.set_msgpara( (long) &tPara );

    tPara.set_id( miId );
    tPara.set_campid( miCampId );
    tPara.set_camprankid( miCampRank );
    tPara.set_growing( miGrowingValue );
    tPara.set_handindawnum( miHandinDawNum );
    tPara.set_level( miLevel );
    tPara.set_settlephase( miSettlePhase );
    CYaoQianShuSettleInfo* lpSettleInfo = CYaoQianShuSettleInfo::GetSingletonPtr();
    if ( lpSettleInfo->GetLastPhase( ) < miSettlePhase )
    {
        tPara.set_settlefinish( 1 );
        tPara.set_noticeid( 0 );
        tPara.set_encourage( 0 );
    }else
    {
        tPara.set_settlefinish( 0 );
        CYaoQianShuSettleInfoUnit* lpSettleInfoUnit = lpSettleInfo->GetSettlePhase( miSettlePhase );
        if ( lpSettleInfoUnit==NULL )
        {
            LOG_ERROR("YaoQianshu", 
                "Can't get CYaoQianShuSettleInfoUnit by SettlePhas(%d)", 
                __LK_FILE__, __LINE__, __FUNCTION__, miSettlePhase );
            return false;
        }
        tPara.set_noticeid( lpSettleInfoUnit->miNoticeCode );
        tPara.set_encourage( lpSettleInfoUnit->miEncourageId );
        tPara.set_noticeparam1( lpSettleInfoUnit->miNoticeParam1 );
        tPara.set_noticeparam2( lpSettleInfoUnit->miNoticeParam2 );
        tPara.set_noticeparam3( lpSettleInfoUnit->miNoticeParam3 );
    }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Send CMessageG2SSettleNotice(%d) to Scenc message! "
        "id(%d) campid(%d) camprankid(%d) growing(%d) level(%d) handindawnum(%d) settlephase(%d) "
        "settlefinish(%d) noticeid(%d) noticeparam1(%d) noticeparam2(%d) noticeparam3(%d) encourage(%d)",
        __LK_FILE__, __LINE__, __FUNCTION__, tMessage.mutable_msghead()->messageid(),
        tPara.id(), tPara.campid(), tPara.camprankid(), tPara.growing(), tPara.level(), tPara.handindawnum(), 
        tPara.settlephase(), tPara.settlefinish(), tPara.noticeid(), tPara.noticeparam1(), tPara.noticeparam2(), 
        tPara.noticeparam3(), tPara.encourage() );

    CSceneServer* lpSceneServer = CGateServer::GetSingleton().GetServerByID( miCampId );
    if ( lpSceneServer == NULL )
    {
        LOG_ERROR( "YaoQianShu","[ %s:%d ][ %s ] Can't get scene server by campid(%d)", __LK_FILE__, __LINE__, __FUNCTION__, miCampId );
        return false;
    }

    CGateServer::GetSingleton().Send2Scene( lpSceneServer, &tMessage);
    return true;
}
CActivityYQS::CActivityYQS()
{
    mbIsUsingYaoQianShu = false;
    mbIsLauched         = false;
    memset( &mLevel, 0x00, sizeof(mLevel) );
    mCheckTimer         = CWTimer( 1000 );
}

void CActivityYQS::OnLaunchServer()
{
    if ( mShmPtr->GetInitMode() == SHM_INIT )
    {
       /* bool lbRet = LoadActivityYQSConfigFromXml( "../config/data/activity/ActivityYQS.xml" );*/
		  bool lbRet = LoadActivityYQSConfigFromXml( CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"activity/ActivityYQS.xml" ));
        if ( lbRet==false )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Load configure from xml failed!!", __LK_FILE__, __LINE__, __FUNCTION__ );
            exit(0);
        }

		const char* pcConfigPath = CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"activity/ActivityYQS.xml");
		CGateServer::GetSingleton().RegisterLoadConfigCallback(pcConfigPath, CActivityYQS::LoadActivityYQSConfigCallback);
      
    }

	mbIsLauched = true;
}

bool CActivityYQS::IsAllStart( void )
{
    for ( int i=1; i<MAX_CAMP_NUM; i++ )
    {
        if ( moCampYaoQianShuInfo[i].miId == 0 )
        {
            return false;
        }
    }

    return true;
}

bool CActivityYQS::IsAllEnd( void )
{
    for ( int i=1; i<MAX_CAMP_NUM; i++ )
    {
        if ( moCampYaoQianShuInfo[i].miId != 0 )
        {
            return false;
        }
    }

    return true;
}

bool CActivityYQS::StartActivity( std::string strStartTime )
{
    if ( mbIsUsingYaoQianShu == false )
    {
        LOG_ERROR( "YaoQianShu","[ %s:%d ][ %s ] the yqs activity is't been using !", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    LOG_NOTICE( "YaoQianShu","[ %s:%d ][ %s ] send start activity to db !", __LK_FILE__, __LINE__, __FUNCTION__ );
    char lstrBuf[1024] ={0};
    sprintf(    lstrBuf, 
        "call ProcYaoQianShuStartActivity( %d, \"%s\" );",
        HELPID_YAOQIANSHU,
        strStartTime.c_str()  );

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] ProcYaoQianShuStartActivity sql(%s)!", __LK_FILE__, __LINE__, __FUNCTION__,lstrBuf );
    if ( CGateLogic::GetSingleton().ExecuteSql( 
        em_dbsession_activityyqs_start, 0, 0, 
        0, SQL_NEED_CALLBACK, SELECT, lstrBuf) == false )
    {
        LOG_ERROR( "YaoQianShu", "[%s:%d] exec sql %s failed", __FUNCTION__, __LINE__, lstrBuf );
        return false;
    }

    for ( int i=1;i<MAX_CAMP_NUM;i++ )
    {
        moCampYaoQianShuInfo[i].clear();
    }

    return true;
}

bool CActivityYQS::EndActivity( std::string strEndTime )
{
    if ( mbIsUsingYaoQianShu==false )
    {
        LOG_ERROR( "YaoQianShu","[ %s:%d ][ %s ] the yqs activity is't been using !", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    if ( mbIsStarted == false )
    {
        LOG_ERROR( "YaoQianShu","[ %s:%d ][ %s ] the yqs activity is't stated !", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    LOG_NOTICE( "YaoQianShu","[ %s:%d ][ %s ] send end activity to db !", __LK_FILE__, __LINE__, __FUNCTION__ );
    char lstrBuf[1024] ={0};
    sprintf(    lstrBuf, 
            "call ProcYaoQianShuEndActivity( %d,\"%s\" );",
            moCampYaoQianShuInfo[1].miId, 
            strEndTime.c_str()  ) ;

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] ProcYaoQianShuEndActivity sql(%s)!", __LK_FILE__, __LINE__, __FUNCTION__, lstrBuf );
    if ( CGateLogic::GetSingleton().ExecuteSql( em_dbsession_activityyqs_end, 0, 0, 
        0, SQL_NEED_CALLBACK, SELECT, lstrBuf) == false )
    {
        LOG_ERROR( "YaoQianShuActivity", "[%s:%d] exec sql %s failed", __FUNCTION__, __LINE__, lstrBuf );
        return false;
    }

    mbIsStarted = false;
    return true;
}

bool CActivityYQS::OnSessionActivityEnd( CMessage* tpMessage, int Param1, int nParam2 )
{
    LOG_NOTICE( "YaoQianShu","[ %s:%d ][ %s ] Start handle  OnSessionActivityEnd !", __LK_FILE__, __LINE__, __FUNCTION__ );

    LK_ASSERT(tpMessage != NULL, return false);
    CMessageExecuteSqlResponse3* pMessage = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

    if ( pMessage == NULL )
    {
        LOG_ERROR( "YaoQianShu","[ %s:%d ][ %s ] Can't get CMessageExecuteSqlResponse3!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    if ( pMessage->resultcode() != 1 )
    {
        LOG_ERROR( "YaoQianShu",
            "[ %s:%d ][ %s ] CMessageExecuteSqlResponse3 result(%d)!", 
            __LK_FILE__, __LINE__, __FUNCTION__, pMessage->resultcode() );
        return false;
    }
    std::string lstrResult = pMessage->fieldvalue( 1 );
    if ( lstrResult == "1" )
    {
        for ( int i=1;i<MAX_CAMP_NUM; i++ )
        {
            moCampYaoQianShuInfo[i].clear();
        }
    }else
    {
        LOG_ERROR( "YaoQianShu",
            "[ %s:%d ][ %s ] Can't get resultvalue(%s)!", 
            __LK_FILE__, __LINE__, __FUNCTION__, lstrResult.c_str() );
        return false;
    }

    mbIsStarted = false;

    LOG_NOTICE( "YaoQianShu","[ %s:%d ][ %s ] Finish handle  OnSessionActivityEnd !", __LK_FILE__, __LINE__, __FUNCTION__ );

    return true;
}

bool CActivityYQS::OnSessionActivityStart( CMessage* tpMessage, int nParam1, int nParam2 )
{
    LOG_NOTICE( "YaoQianShu","[ %s:%d ][ %s ] Start handle  OnSessionActivityStart !", __LK_FILE__, __LINE__, __FUNCTION__ );

    LK_ASSERT(tpMessage != NULL, return false);
    CMessageExecuteSqlResponse3* pMessage = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

    if ( pMessage == NULL )
    {
        LOG_ERROR( "YaoQianShu","[ %s:%d ][ %s ] Can;t get message!", __LK_FILE__,__LINE__, __FUNCTION__ );
        return false;
    }

    int colnum = pMessage->colcount();
    int rownum = pMessage->rowcount();

    if ( pMessage->resultcode() != 1 )
    {
        LOG_ERROR( "YaoQianShu","[ %s:%d ][ %s ] resultcode(%d)!", __LK_FILE__,__LINE__, __FUNCTION__, pMessage->resultcode() );
        return false;
    }

    for(int tRowIndex = 0; tRowIndex<rownum ; ++tRowIndex)
    {
        std::string lstrId              = pMessage->fieldvalue( 0+tRowIndex*colnum );
        std::string lstrActivityID      = pMessage->fieldvalue( 1+tRowIndex*colnum );
        std::string lstrStartTime       = pMessage->fieldvalue( 2+tRowIndex*colnum );
        std::string lstrEndTime         = pMessage->fieldvalue( 3+tRowIndex*colnum );
        std::string lstrCampID          = pMessage->fieldvalue( 4+tRowIndex*colnum );
        std::string lstrCampRank        = pMessage->fieldvalue( 5+tRowIndex*colnum );
        std::string lstrCurrHandinDawNum = pMessage->fieldvalue( 6+tRowIndex*colnum );
        std::string lstrCurrGrowing     = pMessage->fieldvalue( 7+tRowIndex*colnum );
        std::string lstrCurrLevel       = pMessage->fieldvalue( 8+tRowIndex*colnum );
        std::string lstrSettlePhase     = pMessage->fieldvalue( 9+tRowIndex*colnum );

        int liCampiD                                    = atoi( lstrCampID.c_str() );
        moCampYaoQianShuInfo[liCampiD].miId             = atoi( lstrId.c_str() );
        moCampYaoQianShuInfo[liCampiD].miActivityID     = atoi( lstrActivityID.c_str() );
        moCampYaoQianShuInfo[liCampiD].miCampId         = liCampiD;
        moCampYaoQianShuInfo[liCampiD].miHandinDawNum   = atoi( lstrCurrHandinDawNum.c_str() );
        moCampYaoQianShuInfo[liCampiD].miGrowingValue   = atoi( lstrCurrGrowing.c_str() );
        moCampYaoQianShuInfo[liCampiD].miLevel          = atoi( lstrCurrLevel.c_str() );
        moCampYaoQianShuInfo[liCampiD].miCampRank       = atoi( lstrCampRank.c_str() );
        moCampYaoQianShuInfo[liCampiD].mstrStartTime    = lstrStartTime;
        moCampYaoQianShuInfo[liCampiD].mstrEndTime      = lstrEndTime;
        moCampYaoQianShuInfo[liCampiD].miSettlePhase    = atoi( lstrSettlePhase.c_str() );

        LOG_NOTICE( "YaoQianShu","[ %s:%d ][ %s ] result(%d) liCampiD(%d)"
            "miId(%d) miActivityID(%d) miCampId(%d) miHandinDawNum(%d) "
            "miGrowingValue(%d) miLevel(%d) miCampRank(%d) "
            "mstrStartTime(%s) mstrEndTime(%s) miSettlePhase(%d)", 
            __LK_FILE__,__LINE__, __FUNCTION__, pMessage->resultcode(),liCampiD,
            moCampYaoQianShuInfo[liCampiD].miId, moCampYaoQianShuInfo[liCampiD].miActivityID,
            moCampYaoQianShuInfo[liCampiD].miCampId,moCampYaoQianShuInfo[liCampiD].miHandinDawNum,
            moCampYaoQianShuInfo[liCampiD].miGrowingValue,moCampYaoQianShuInfo[liCampiD].miLevel,
            moCampYaoQianShuInfo[liCampiD].miCampRank, moCampYaoQianShuInfo[liCampiD].mstrStartTime.c_str(),
            moCampYaoQianShuInfo[liCampiD].mstrEndTime.c_str(),moCampYaoQianShuInfo[liCampiD].miSettlePhase );

        //发送活动开始通知
        SendYaoQianShuAvtivityStartNotice( liCampiD );

        if ( moCampYaoQianShuInfo[liCampiD].miSettlePhase!=-1 )
        {
            time_t liTimeNow = time(NULL);
            int liCampSettlePhase = moCampYaoQianShuInfo[liCampiD].miSettlePhase;
            CYaoQianShuSettleInfo* lpYaoQianShuSettleInfo = CYaoQianShuSettleInfo::GetSingletonPtr();
            if ( lpYaoQianShuSettleInfo == NULL )
            {
                LOG_ERROR( "YaoQianShu", 
                    "[ %s:%d ][ %s ] Can't get CYaoQianShuSettleInfo!", __LK_FILE__, __LINE__, __FUNCTION__ );
                continue;
            }
            int liLastSettlePhase = lpYaoQianShuSettleInfo->GetLastPhase();
            if ( liLastSettlePhase < liCampSettlePhase )
            {
                LOG_ERROR( "YaoQianShu", 
                    "[ %s:%d ][ %s ] the Yqs settle phase is invalied! last(%d) now(%d)", 
                    __LK_FILE__, __LINE__, __FUNCTION__, liLastSettlePhase, liCampSettlePhase );
                continue;
            }
            if ( liLastSettlePhase == liCampSettlePhase )
            {
                moCampYaoQianShuInfo[liCampiD].ResetTimer( liTimeNow, lpYaoQianShuSettleInfo->miTimeDelayForEnd );
            }else
            {
                CYaoQianShuSettleInfoUnit* lpYaoQianShuSettleInfoUnit = lpYaoQianShuSettleInfo->GetSettlePhase( liCampSettlePhase );
                int liTimeDelay         = lpYaoQianShuSettleInfoUnit->miTimeDelay;
                moCampYaoQianShuInfo[liCampiD].ResetTimer( liTimeNow, liTimeDelay );
            }
        }


    }

    mbIsStarted = true;

    LOG_NOTICE( "YaoQianShu","[ %s:%d ][ %s ] Finish handle  OnSessionActivityStart !", __LK_FILE__, __LINE__, __FUNCTION__ );

    return true;
}

void CActivityYQS::SendYaoQianShuAvtivityStartNotice( int nCampId )
{
    CMessage tMessage;
    CMessageYQSActivtyStartNotice tPara;
    tMessage.mutable_msghead()->set_messageid( ID_G2S_YAOQIANSHUSTART_NOTICE );
    tMessage.set_msgpara( (long) &tPara );

    tPara.set_id( moCampYaoQianShuInfo[nCampId].miId );
    tPara.set_campid(moCampYaoQianShuInfo[nCampId].miCampId );
    tPara.set_camprankid(moCampYaoQianShuInfo[nCampId].miCampRank);
    tPara.set_growing( moCampYaoQianShuInfo[nCampId].miGrowingValue );
    tPara.set_handindawnum( moCampYaoQianShuInfo[nCampId].miHandinDawNum );
    tPara.set_level( moCampYaoQianShuInfo[nCampId].miLevel );
    tPara.set_settlephase( moCampYaoQianShuInfo[nCampId].miSettlePhase );

    LOG_NOTICE( "YaoQianShu",
        "[ %s:%d ][ %s ] Send CMessageYQSActivtyStartNotice to scence "
        " miId(%d), miCampId(%d), miCampRank(%d), miGrowingValue(%d), "
        "miHandinDawNum(%d), miLevel(%d), miSettlePhase(%d)", 
        __LK_FILE__, __LINE__, __FUNCTION__,
        moCampYaoQianShuInfo[nCampId].miId, 
        moCampYaoQianShuInfo[nCampId].miCampId,
        moCampYaoQianShuInfo[nCampId].miCampRank, 
        moCampYaoQianShuInfo[nCampId].miGrowingValue, 
        moCampYaoQianShuInfo[nCampId].miHandinDawNum, 
        moCampYaoQianShuInfo[nCampId].miLevel, 
        moCampYaoQianShuInfo[nCampId].miSettlePhase  );

    CSceneServer* lpSceneServer = CGateServer::GetSingleton().GetServerByID( nCampId );
    if ( lpSceneServer == NULL )
    {
        LOG_ERROR( "YaoQianShu","[ %s:%d ][ %s ] Can't tet scene server by campid(%d)", __LK_FILE__, __LINE__, __FUNCTION__, nCampId );
        return;
    }

    CGateServer::GetSingleton().Send2Scene( lpSceneServer, &tMessage);
}

bool CActivityYQS::IsYaoQianShu( int iActivityId )
{
    return iActivityId==HELPID_YAOQIANSHU?true:false;
}

bool CActivityYQS::LoadActivityYQSConfigFromXml( const char* pFileName )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start Load configure from xml!", __LK_FILE__, __LINE__, __FUNCTION__ );

    TiXmlDocument xmldoc;
    xmldoc.LoadFile( pFileName );

    TiXmlElement* tpRoot= xmldoc.FirstChildElement();
    if (tpRoot == NULL)
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't Get root path(%s)!", __LK_FILE__, __LINE__, __FUNCTION__, pFileName );
        return false;
    }

    TiXmlElement* lpUsing   = tpRoot->FirstChildElement( "IsUsingActivityYQS" );
    if ( lpUsing == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get using flag!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    const char* lszpUsing           = lpUsing->GetText();
    mbIsUsingYaoQianShu      = ::atoi( lszpUsing )==1?true:false;

    TiXmlElement* lpLevels  = tpRoot->FirstChildElement( "Levels" );
    if ( lpLevels == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get levels!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    TiXmlElement* lpLevel = lpLevels->FirstChildElement( "Level" );
    for ( ;lpLevel!=NULL; lpLevel=lpLevel->NextSiblingElement() )
    {
        const char* lszpId      = lpLevel->Attribute( "id" );
        int liId        = ::atoi( lszpId );
        if ( liId<=0 || liId>MAX_ACTIVIT_YAOQIANSHU_LEVEL )
        {
            return false;
        }

        const char* lszpMaxValue = lpLevel->GetText();
        int liMaxValue  = ::atoi( lszpMaxValue );

        mLevel[liId]     = liMaxValue;

        LOG_INFO( "YaoQianShu", "[ %s:%d ][ %s ] level(%d) max value(%d)!", __LK_FILE__, __LINE__, __FUNCTION__, liId, liMaxValue );
    }
    
    bool lbRetForLoadSettle = CYaoQianShuSettleInfo::GetSingleton().LoadSettlePhaseFromXML( tpRoot );
    if ( lbRetForLoadSettle == false )
    {
        LOG_INFO( "YaoQianShu", "[ %s:%d ][ %s ] Load Settle phase failed!", __LK_FILE__, __LINE__, __FUNCTION__);
        return false;
    }


    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish Load configure from xml!", __LK_FILE__, __LINE__, __FUNCTION__ );

    return true;
}

void CActivityYQS::OnExitServer()
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] is exit!", __LK_FILE__, __LINE__, __FUNCTION__ );
}

void CActivityYQS::OnTimer( unsigned int vTickCount, unsigned int vTickOffset )
{
    if ( mbIsUsingYaoQianShu != true || mbIsStarted != true )
    {
        return;
    }

    if ( mCheckTimer.Elapse( vTickOffset ) )
    {
        int liSettleTime = CYaoQianShuSettleInfo::GetSingleton().miSettleTime;
        if ( IsPassed( liSettleTime ) )
        {
            for ( int i=1; i<MAX_CAMP_NUM; i++ )
            {
                moCampYaoQianShuInfo[i].StartSettle();
            }
        }

        for ( int i=1; i<MAX_CAMP_NUM;i++ )
        {
            moCampYaoQianShuInfo[i].OnSettleTimer();
        }
    }
}

bool CActivityYQS::IsPassed( int nTime )
{
    time_t t = time( 0 );
    tm tmp;
    localtime_r( &t, &tmp);

    int tHour = nTime/100;
    int tMini = nTime%100;

    if ( tmp.tm_hour > tHour ) // 小时超过了
    {
        return true;
    }
    else if ( tmp.tm_hour == tHour ) // 小时相等
    {
        if ( tmp.tm_min >= tMini ) // 分钟超过了
        {
            return true;
        }
    }
    return false;
}

void CActivityYQS::OnMessage( int nServerID, CMessage* pMessage )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Get message!", __LK_FILE__, __LINE__, __FUNCTION__ );
    LK_ASSERT(pMessage != NULL && pMessage->msgpara() != 0, return);	
    CMessageHead* tpHead = pMessage->mutable_msghead();
    unsigned short nMessageID = (unsigned short) tpHead->messageid();

    if ( mbIsUsingYaoQianShu != true || mbIsStarted != true )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] the activity has't been used!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return;
    }

    switch (nMessageID)
    {	
    case ID_S2G_UPDATEYQSGROWING_REQUEST:
        {
            OnMessageGrowing( nServerID, pMessage );
            break;
        }
    case ID_S2G_GETCAMPRANKFORYQS_REQUEST:
        {
            OnMessageGetCampRank( nServerID, pMessage );
            break;
        }
    default:
        {
            LOG_ERROR("YaoQianShu", "[ %s:%d ][ %s ] there is no action for the message_id(%d)", 
               __LK_FILE__, __LINE__, __FUNCTION__, nMessageID);
            return;
        }
    }
}

void CActivityYQS::OnMessageGetCampRank( int nServerID, CMessage* pMessage )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start Handle OnMessageGetCampRank", __LK_FILE__, __LINE__, __FUNCTION__ );
    LK_ASSERT(pMessage != NULL && pMessage->msgpara() != 0, return);
    CMessageS2GGetCampRankForYQSRequest *tpMsg = (CMessageS2GGetCampRankForYQSRequest *)pMessage->msgpara();
    
    int liPlayerId = tpMsg->playerid();

    if ( mbIsUsingYaoQianShu == false || mbIsStarted == false )
    {
        SendResultForGetCampRankForYQS( nServerID, -1, liPlayerId, tpMsg->param1() );
    }

    SendResultForGetCampRankForYQS( nServerID, 1, liPlayerId, (int)tpMsg->param1() );
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] finish Handle OnMessageGetCampRank", __LK_FILE__, __LINE__, __FUNCTION__ );

}

void CActivityYQS::SendResultForGetCampRankForYQS( int nServerId, int nResult, int nPlayerId, int nParam )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start Handle SendResultForGetCampRankForYQS result(%d)", 
        __LK_FILE__, __LINE__, __FUNCTION__, nResult );

    CMessage tMessage;
    CMessageG2SGetCampRankForYQSResponse tPara;
    tMessage.mutable_msghead()->set_messageid( ID_G2S_GETCAMPRANKFORYQS_RESPONSE );
    tMessage.set_msgpara( (long) &tPara );

    tPara.set_playerid( nPlayerId );
    tPara.set_param1( nParam );
    tPara.set_result( nResult );
    if ( nResult == 1 )
    {
        for ( int i=1;i<MAX_ACTIVIT_YAOQIANSHU_LEVEL;i++ )
        {
            if ( moCampYaoQianShuInfo[i].miId == 0 )
            {
                tPara.set_result( -1 );
                break;
            }
            CampRankForYQSForSever* lpCampRankInfo = tPara.add_camprankinfo();
            lpCampRankInfo->set_campid( moCampYaoQianShuInfo[i].miCampId );
            lpCampRankInfo->set_rank( moCampYaoQianShuInfo[i].miCampRank );
            lpCampRankInfo->set_growinggrade( moCampYaoQianShuInfo[i].miGrowingValue );
            lpCampRankInfo->set_growingphase( moCampYaoQianShuInfo[i].miLevel );

            LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] camp rank info! campid(%d) rank(%d) growinggrade(%d) growingphase(%d)", 
                __LK_FILE__, __LINE__, __FUNCTION__, 
                moCampYaoQianShuInfo[i].miCampId, moCampYaoQianShuInfo[i].miCampRank, 
                moCampYaoQianShuInfo[i].miGrowingValue, moCampYaoQianShuInfo[i].miLevel );
        }
    }

    CGateServer::GetSingleton().Send2Scene(nServerId, &tMessage);
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish Handle SendResultForGetCampRankForYQS result(%d)", 
        __LK_FILE__, __LINE__, __FUNCTION__, tPara.result() );

}

void CActivityYQS::OnMessageGrowing( int nServerID, CMessage* pMessage )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start Handle OnMessageGrowing", __LK_FILE__, __LINE__, __FUNCTION__ );
    LK_ASSERT(pMessage != NULL && pMessage->msgpara() != 0, return);
    CMessageS2GUpdateYQSGrowingRequest *tpMsg = (CMessageS2GUpdateYQSGrowingRequest *)pMessage->msgpara();

    int liPlayerId      = tpMsg->playerid();
    int liCampId        = tpMsg->campid();
    int liGrowingValue  = tpMsg->growingchangevalue();
    if ( liCampId<1 || liCampId>=MAX_CAMP_NUM )
    {
        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] the campid(%d) is invalid!", __LK_FILE__, __LINE__, __FUNCTION__, liCampId );
        return;
    }

    if ( moCampYaoQianShuInfo[liCampId].miSettlePhase!=-1 )
    {    
        SendResultForUpdateYQSGrowing( nServerID, -1, liPlayerId, liCampId, 0 );
        return;
    }

    int liCampLevel     = moCampYaoQianShuInfo[liCampId].miLevel;
    if ( liCampLevel<0 || liCampLevel>=MAX_ACTIVIT_YAOQIANSHU_LEVEL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] the campid(%d)'s level(%d) is invalid!", 
            __LK_FILE__, __LINE__, __FUNCTION__, liCampId, liCampLevel );
        return;
    }
    int liLevelValue            = mLevel[liCampLevel];
    int liMaxLevelValue         = mLevel[MAX_ACTIVIT_YAOQIANSHU_LEVEL-1];
    int liCampGrowingValue      = moCampYaoQianShuInfo[liCampId].miGrowingValue;
    int liNewCampGrowingValue   = liCampGrowingValue+liGrowingValue;
    if ( liNewCampGrowingValue >= liMaxLevelValue )
    {
            liNewCampGrowingValue = liMaxLevelValue;
            moCampYaoQianShuInfo[liCampId].miLevel  = (MAX_ACTIVIT_YAOQIANSHU_LEVEL-1);
            moCampYaoQianShuInfo[liCampId].StartSettle();
    }else if ( liNewCampGrowingValue>liLevelValue )
    {
        for ( int i=1; i<MAX_ACTIVIT_YAOQIANSHU_LEVEL-liCampLevel; i++ )
        {
            int liNextLevel         = liCampLevel+i;
            int liNextLevelValue    = mLevel[liNextLevel];

            if ( liNewCampGrowingValue<liNextLevelValue )
            {
                moCampYaoQianShuInfo[liCampId].miLevel = liNextLevel;
                break;
            }

            if ( liNextLevel == MAX_ACTIVIT_YAOQIANSHU_LEVEL-1)
            {
                liNewCampGrowingValue = liMaxLevelValue;
                moCampYaoQianShuInfo[liCampId].miLevel  = (MAX_ACTIVIT_YAOQIANSHU_LEVEL-1);
                moCampYaoQianShuInfo[liCampId].StartSettle();
                break;
            }
        }
    }

    moCampYaoQianShuInfo[liCampId].miGrowingValue = liNewCampGrowingValue;
    moCampYaoQianShuInfo[liCampId].miHandinDawNum++;
   
    Sort();
    moCampYaoQianShuInfo[liCampId].UpdateCampInfo( );
    

    SendResultForUpdateYQSGrowing( nServerID, 1, liPlayerId, liCampId, liGrowingValue );

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish Handle OnMessageGrowing", __LK_FILE__, __LINE__, __FUNCTION__ );
}

void CActivityYQS::SendResultForUpdateYQSGrowing( int nServerId, int nResult, int nPlayerId, int nCampId, int nGrowing )
{
    CMessage tMessage;
    CMessageG2SUpdateYQSGrowingResponse tPara;
    tMessage.mutable_msghead()->set_messageid( ID_G2S_UPDATEYQSGROWING_RESPONSE );
    tMessage.set_msgpara( (long) &tPara );

    tPara.set_result        ( nResult );
    tPara.set_playerid      ( nPlayerId );
    tPara.set_campid        ( nCampId );
    tPara.set_id            ( moCampYaoQianShuInfo[nCampId].miId );
    tPara.set_growing       ( moCampYaoQianShuInfo[nCampId].miGrowingValue );
    tPara.set_camprankid    ( moCampYaoQianShuInfo[nCampId].miCampRank );
    tPara.set_level         ( moCampYaoQianShuInfo[nCampId].miLevel );
    tPara.set_handindawnum  ( moCampYaoQianShuInfo[nCampId].miHandinDawNum );
    tPara.set_settlephase   ( moCampYaoQianShuInfo[nCampId].miSettlePhase );
	tPara.set_deltagrowing	( nGrowing );
    LOG_NOTICE( "YaoQianshu",
        "[ %s:%d ][ %s ] Send CMessageG2SUpdateYQSGrowingResponse "
        "result(%d) playerid(%d) id(%d) campid(%d) growing(%d) camprankid(%d) level(%d) handindawnum(%d) settlephase(%d) deltagrowing(%d)",
        nResult,nPlayerId, moCampYaoQianShuInfo[nCampId].miId, nCampId, 
        moCampYaoQianShuInfo[nCampId].miGrowingValue,moCampYaoQianShuInfo[nCampId].miCampRank,
        moCampYaoQianShuInfo[nCampId].miLevel, moCampYaoQianShuInfo[nCampId].miHandinDawNum, 
        moCampYaoQianShuInfo[nCampId].miSettlePhase, nGrowing );

    CGateServer::GetSingleton().Send2Scene( nServerId, &tMessage);
}

void CActivityYQS::ReloadCfg()
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Reload configure!", __LK_FILE__, __LINE__, __FUNCTION__ );
   /* bool lbRet = LoadActivityYQSConfigFromXml( "../config/data/activity/ActivityYQS.xml" );*/
	 bool lbRet = LoadActivityYQSConfigFromXml( CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"activity/ActivityYQS.xml" ));
    if ( lbRet==false )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Load configure from xml failed!!", __LK_FILE__, __LINE__, __FUNCTION__ );
        exit(0);
    }
}

bool CActivityYQS::IsLaunched()
{
    return mbIsLauched;
}

std::string CActivityYQS::ConverTime( int nTime )
{
    time_t t = time( 0 );
    tm tmp;
    localtime_r( &t, &tmp);

    int tHour = nTime/100;
    int tMini = nTime%100;

    tmp.tm_hour = tHour;
    tmp.tm_min  = tMini;
    tmp.tm_sec  = 0;

    char lszbuf[128] = {0};
    strftime( lszbuf, sizeof(lszbuf), "%F %T", &tmp );
    return lszbuf;
}

void CActivityYQS::Sort( void )
{
    t_map       loMapForCamp;
    for ( int i=1;i<MAX_CAMP_NUM;i++ )
    {
        loMapForCamp.insert( t_map_value( moCampYaoQianShuInfo[i].miGrowingValue, moCampYaoQianShuInfo[i].miCampId ) );
    }

    t_map_ritor loItor = loMapForCamp.rbegin();
    for ( int i=1; loItor!=loMapForCamp.rend(); loItor++, i++ )
    {
        int liCampId    = loItor->second;

        // 解决在两个阵营同时达到最大阵营排名时会被修改的问题
        if ( moCampYaoQianShuInfo[liCampId].miGrowingValue == mLevel[MAX_ACTIVIT_YAOQIANSHU_LEVEL-1] )
        {
            continue;
        }
        moCampYaoQianShuInfo[liCampId].miCampRank=i;
    }
}

unsigned int CActivityYQS::CountSize()
{
    return sizeof( CActivityYQS );
}

CYaoQianShuSettleInfoUnit::CYaoQianShuSettleInfoUnit()
{
    miPhase=-1; 
    miCampRankLimit=0;
    miNoticeCode=0;
    miEncourageId=0; 
    miNoticeParam1=0; 
    miNoticeParam2=0; 
    miNoticeParam3=0;
}

CYaoQianShuSettleInfoUnit& CYaoQianShuSettleInfoUnit::operator=( CYaoQianShuSettleInfoUnit& theObj )
{
    miPhase             = theObj.miPhase;
    miCampRankLimit     = theObj.miCampRankLimit;
    miNoticeCode        = theObj.miNoticeCode;
    miEncourageId       = theObj.miEncourageId;
    miNoticeParam1      = theObj.miNoticeParam1;
    miNoticeParam2      = theObj.miNoticeParam2;
    miNoticeParam3      = theObj.miNoticeParam3;
    return *this;
}

CYaoQianShuSettleInfoUnit* CYaoQianShuSettleInfo::GetSettlePhase( int nSettlePhase )
{
    if ( nSettlePhase<0 || nSettlePhase>(int)mSettlePhase.size() )
    {
        return NULL;
    }

    return &mSettlePhase[nSettlePhase];
}

int CYaoQianShuSettleInfo::GetLastPhase()
{
    int liLastPhasePos = (int)mSettlePhase.size()-1;
    return mSettlePhase[liLastPhasePos].miPhase;
}

bool CYaoQianShuSettleInfo::CheckSettlePhase( void )
{
    for ( int i=0; i<(int)mSettlePhase.size();i++ )
    {
        if ( mSettlePhase[i].miPhase != i )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't SettlePhase(%d) is invalied!", __LK_FILE__, __LINE__, __FUNCTION__,i );
            return false;
        }
    }

    return true;
}

bool CYaoQianShuSettleInfo::LoadSettlePhaseFromXML( TiXmlElement* tpRoot )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start handle LoadSettlePhaseFromXML!", __LK_FILE__, __LINE__, __FUNCTION__ );
    mSettlePhase.clear();

    // 获取结算段
    TiXmlElement* lpSettle  = tpRoot->FirstChildElement( "Settle" );
    if ( lpSettle == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get Settle!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    // 获取结算开始时间
    const char* lpszStartTime     = lpSettle->Attribute( "starttime" );
    if ( lpszStartTime==NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get starttime!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    miSettleTime            = atoi( lpszStartTime );
    // 获取结算在结束后多久销毁所有NPC包箱
    const char* lpszTimeDelayForEnd     = lpSettle->Attribute( "end" );
    if ( lpszTimeDelayForEnd== NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get TimeDelayForEnd!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    miTimeDelayForEnd       = atoi( lpszTimeDelayForEnd );
    // 获取结算阶段
    TiXmlElement* lpPhase   = lpSettle->FirstChildElement( "phase" );
    if ( lpPhase==NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get phase!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    for ( ; lpPhase!=NULL; lpPhase=lpPhase->NextSiblingElement() )
    {
        // 获取结算阶段
        CYaoQianShuSettleInfoUnit loSettleInfo;
        const char* lpszId      = lpPhase->Attribute( "id" );
        if ( lpszId == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get id for phase!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        loSettleInfo.miPhase    = atoi( lpszId );

        // 获取时间延迟
        const char* lpszTime      = lpPhase->Attribute( "time" );
        if ( lpszId == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get id for phase!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        loSettleInfo.miTimeDelay    = atoi( lpszTime );
        // 获取阵营排名限制
        const char* lpszCampRankLimite = lpPhase->Attribute( "campranklimit" );
        if ( lpszCampRankLimite==NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get CampRankLimite for phase!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;

        }
        loSettleInfo.miCampRankLimit    = atoi( lpszCampRankLimite );
        // 获取需要发送的通知
        const char* lpszNotice  = lpPhase->Attribute( "notice" );
        if ( lpszNotice==NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get notice for phase!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        loSettleInfo.miNoticeCode       = atoi( lpszNotice );
        // 获取是否发送奖励
        const char* lpszEncourage   = lpPhase->Attribute( "encourage" );
        if ( lpszEncourage==NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get encourage for phase!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        loSettleInfo.miEncourageId      = atoi( lpszEncourage );

        // 获取消息参数
        const char* lpszParam1       = lpPhase->Attribute( "param1" );
        if ( lpszParam1==NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get param1 for phase!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        loSettleInfo.miNoticeParam1     = atoi( lpszParam1 );
        // 获取消息参数
        const char* lpszParam2       = lpPhase->Attribute( "param2" );
        if ( lpszParam2==NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get param2 for phase!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        loSettleInfo.miNoticeParam2     = atoi( lpszParam2 );
        // 获取消息参数
        const char* lpszParam3       = lpPhase->Attribute( "param3" );
        if ( lpszParam3==NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get param3 for phase!", __LK_FILE__, __LINE__, __FUNCTION__ );
            return false;
        }
        loSettleInfo.miNoticeParam3     = atoi( lpszParam3 );

        mSettlePhase.push_back( loSettleInfo );
    }

    bool lbRetForCheckSettlePhase = CheckSettlePhase();
    if ( lbRetForCheckSettlePhase == false )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] CheckSettlePhase failed!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish handle LoadSettlePhaseFromXML!", __LK_FILE__, __LINE__, __FUNCTION__ );

    return true;
}

void CActivityYQS::LoadActivityYQSConfigCallback(const char* pcConfigPath)
{
	if (pcConfigPath == NULL)
	{
		return;
	}

	LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Reload configure!", __LK_FILE__, __LINE__, __FUNCTION__ );
	bool lbRet = CActivityYQS::GetSingleton().LoadActivityYQSConfigFromXml(pcConfigPath);
	if ( lbRet==false )
	{
		LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Load configure from xml failed!!", __LK_FILE__, __LINE__, __FUNCTION__ );
		exit(0);
	}
}