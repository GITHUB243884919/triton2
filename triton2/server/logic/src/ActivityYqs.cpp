
#include "../inc/ActivityYqs.h"
#include "../inc/ActivityYqsCfg.h"
#include "activitytype.h"
#include "mapmodule.h"
#include "propertymodule.h"
#include "activitymodule.h"
#include "mailmodule.h"
#include "scenelogic.h"

/////////////////////////////////////////////////////////////////////////////////////////

template<>CActivityYqs*               CSingleton<CActivityYqs>::spSingleton               = new CActivityYqs;
template<>CActivityYqsPlayerRankInfo* CSingleton<CActivityYqsPlayerRankInfo>::spSingleton = new CActivityYqsPlayerRankInfo;

/////////////////////////////////////////////////////////////////////////////////////////

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
    miSettlePhase           = -1;
    mstrStartTime           = "";
    mstrEndTime             = "";
}

void CActivityYqs::CreateStoreBox( int nLevel )
{
    if ( nLevel == 0 )
    {
        nLevel = 1;
    }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start handle CreateStoreBox!", __LK_FILE__, __LINE__, __FUNCTION__ );

    CYqsCfgLevelInfoUnit* lpLevelInfo = CActivityYqsCfg::GetSingleton().GetLevelInfo( nLevel );
    if ( lpLevelInfo==NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get level info for level(%d)!", __LK_FILE__, __LINE__, __FUNCTION__,nLevel );
        return;
    }

    // 清理前一次创建的NPC宝箱
    int liStoreBoxNum = mStoreBoxCreated.size();
    for ( int i=0; i<liStoreBoxNum; i++ )
    {
        CStoreBox loStoreBox  = mStoreBoxCreated[i];
        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] destory store box(%d)!", __LK_FILE__, __LINE__, __FUNCTION__,loStoreBox.miNpcID );
        COgreCreator* pCreator = ( COgreCreator*)CCreatorManager::GetSingletonPtr()->GetCreator( loStoreBox.miNpcID );
        if ( pCreator == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get creator by id(%d)!", __LK_FILE__, __LINE__, __FUNCTION__,loStoreBox.miNpcID );
            continue;
        }
        CEntityBox *tpStoreBox = (CEntityBox*)CSceneLogic::GetSingleton().GetEntity( pCreator->GetAttachedEntityID() );
        if ( tpStoreBox == NULL )
        {
            LOG_ERROR("YaoQiansShu", "[ %s:%d ][ %s ] Can't get storebox by id(%d)", __LK_FILE__, __LINE__, __FUNCTION__, loStoreBox.miNpcID );
            continue;
        }
        CSceneLogic::GetSingleton().DestroyEntity( tpStoreBox, 0 );

        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] destory storebox(%d)!", __LK_FILE__, __LINE__, __FUNCTION__,loStoreBox.miNpcID  );
    }
    mStoreBoxCreated.clear();

    // 创建NPC宝箱
    std::vector<CStoreBox>         ltStoreBoxList = CActivityYqsCfg::GetSingleton().GetStoreBoxs( nLevel );
    int liStoreBoxSize = (int)ltStoreBoxList.size();
    for ( int i=0; i<liStoreBoxSize; i++ )
    {
        //int liRondom        = rand()%liStoreBoxSize;
        CStoreBox loStoreBox    = ltStoreBoxList[i];

        COgreCreator* pCreator = ( COgreCreator*)CCreatorManager::GetSingletonPtr()->GetCreator( loStoreBox.miNpcID );
        if ( pCreator == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get creator by id(%d)!", __LK_FILE__, __LINE__, __FUNCTION__,loStoreBox.miNpcID );
            continue;
        }

        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Created storebox(%d)! line(%d)", __LK_FILE__, __LINE__, __FUNCTION__, loStoreBox.miNpcID );

        int liline  = CSceneLogic::GetSingleton().GetLineID();
        unsigned int liEntityId  = 0;
        bool lbRetForCreateBox = CMapModule::GetSingleton().CreateNpc( liline, 
            loStoreBox.miMapID, 
            0, 
            pCreator, 
            liEntityId );
        if ( lbRetForCreateBox == false )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Create npc failed!", __LK_FILE__, __LINE__, __FUNCTION__ );
            continue;
        }

        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Created storebox(%d)!", __LK_FILE__, __LINE__, __FUNCTION__, loStoreBox.miNpcID );
        mStoreBoxCreated.push_back( loStoreBox );
    }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish handle CreateStoreBox!", __LK_FILE__, __LINE__, __FUNCTION__ );
}

void CActivityYqs::OnMessageSettleNotice( CMessage* npMessage )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start handle Settle notcie message!", 
        __LK_FILE__, __LINE__, __FUNCTION__ );
    if ( mbIsStarted == false )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] the YaoQianShu activity isn't started!", 
            __LK_FILE__, __LINE__, __FUNCTION__ );
        return;
    }

    LK_ASSERT(npMessage != NULL, return);
    CMessageG2SSettleNotice* tpResponse = ( CMessageG2SSettleNotice* ) npMessage->msgpara(); 
    if ( tpResponse == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get CMessageG2SSettleNotice message!", 
            __LK_FILE__, __LINE__, __FUNCTION__ );
        return ;
    }

    int liId                    = tpResponse->id();
    int liCampId                = tpResponse->campid();
    int liCampRankId            = tpResponse->camprankid();
    int liGrowingValue          = tpResponse->growing();
    int liLevel                 = tpResponse->level();
    int liHandinDawNum          = tpResponse->handindawnum();
    int liSettlePhase           = tpResponse->settlephase();
    int liNoticeCode            = tpResponse->noticeid();
    int liEncourage             = tpResponse->encourage();
    int liIsFinished            = tpResponse->settlefinish();
    int liParam1                = tpResponse->noticeparam1();
    int liParam2                = tpResponse->noticeparam2();
    int liParam3                = tpResponse->noticeparam3();

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] CMessageG2SSettleNotice! "
        "liId(%d) liCampId(%d) liCampRankId(%d) liGrowingValue(%d) liLevel(%d) "
        "liHandinDawNum(%d) liSettlePhase(%d) liNoticeCode(%d) "
        "param1(%d) param2(%d) param3(%d) liEncourage(%d) liIsFinished(%d)", 
        __LK_FILE__, __LINE__, __FUNCTION__,
        liId, liCampId, liCampRankId, liGrowingValue, liLevel, 
        liHandinDawNum, liSettlePhase, liNoticeCode, 
        liParam1, liParam2, liParam3, liEncourage, liIsFinished );

    if ( liId != moYaoQianShuCampInfo.miId )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] the YaoQianShu Activity id is changed old(%d) new(%d)!", 
            __LK_FILE__, __LINE__, __FUNCTION__,  moYaoQianShuCampInfo.miId, liId );
        return;
    }

    if ( liCampId != moYaoQianShuCampInfo.miCampId )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] the Campid isn't matched old(%d) new(%d)!", 
            __LK_FILE__, __LINE__, __FUNCTION__,  moYaoQianShuCampInfo.miCampId, liCampId );
        return;
    }

    // 第一次进入结算阶段向客户端发送进入结算阶段
    if ( liSettlePhase==0 )
    {
        // 发送结算通知
        SendPhaseChangeNotice( liCampId, 1, liLevel );
    }

    if ( liNoticeCode != 0 )
    {
        // 发送结算公告
        int liTemp[3]   = {0};
        liTemp[0]       = liParam1;
        liTemp[1]       = liParam2;
        liTemp[2]       = liParam3;
        CPropertyModule::GetSingletonPtr()->SendEventNotify( liNoticeCode, 
            CSceneLogic::GetSingleton().GetLineID(), 
            liTemp, 3, NULL, NULL );
    }

    if ( liEncourage >0 )
    {
        // 给与宝箱奖励
        SendEncourageNoticeBroadcast( liCampId, liEncourage );
        CreateStoreBox( liLevel );
    }

    if ( liIsFinished == 1 )
    {
        // 给与阵营奖励
        SettleFinish();
        mbIsFinished = true;
    }

    moYaoQianShuCampInfo.miId               = liId;
    moYaoQianShuCampInfo.miActivityID       = HELPID_YAOQIANSHU;
    moYaoQianShuCampInfo.miCampId           = liCampId;
    moYaoQianShuCampInfo.miSettlePhase      = liSettlePhase;
    moYaoQianShuCampInfo.miCampRank         = liCampRankId;
    moYaoQianShuCampInfo.miGrowingValue     = liGrowingValue;
    moYaoQianShuCampInfo.miLevel            = liLevel;
    moYaoQianShuCampInfo.miHandinDawNum     = liHandinDawNum;

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finished handle Settle notcie message!", 
        __LK_FILE__, __LINE__, __FUNCTION__ );
    return;
}

void CActivityYqs::OnMessageUpdateYQSGrowingResponse( CMessage* npMessage )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start Handle OnMessageUpdateYQSGrowingResponse message!", 
        __LK_FILE__, __LINE__, __FUNCTION__ );
        if ( mbIsStarted == false )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] the YaoQianShu activity isn't started!", 
                __LK_FILE__, __LINE__, __FUNCTION__ );
            return;
        }

        LK_ASSERT(npMessage != NULL, return);
        CMessageG2SUpdateYQSGrowingResponse* tpResponse = ( CMessageG2SUpdateYQSGrowingResponse* ) npMessage->msgpara(); 
        if ( tpResponse == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get CMessageG2SUpdateYQSGrowingResponse message!", 
                __LK_FILE__, __LINE__, __FUNCTION__ );
            return ;
        }

        CYaoQianShuCampInfoUnit loCampInfo;
        int liResult            = tpResponse->result();
        int liPlayerId          = tpResponse->playerid();
        loCampInfo.miId             = tpResponse->id();
        loCampInfo.miActivityID     = HELPID_YAOQIANSHU;
        loCampInfo.miCampId         = tpResponse->campid();
        loCampInfo.miCampRank       = tpResponse->camprankid();
        loCampInfo.miGrowingValue   = tpResponse->growing();
        loCampInfo.miLevel          = tpResponse->level();
        loCampInfo.miHandinDawNum   = tpResponse->handindawnum();
        loCampInfo.miSettlePhase    = tpResponse->settlephase();
		int liGrowing			 = tpResponse->deltagrowing();

        if ( liResult != 1 )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] the message return(%d)!", 
                __LK_FILE__, __LINE__, __FUNCTION__, liResult );
            return;
        }

        if ( mbIsLoaded == false )
        {
            mbIsLoaded  = true;
        }else if ( moYaoQianShuCampInfo.miId != loCampInfo.miId )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] the YaoQianShu id isn't match game(%d) gate(%d)", 
                __LK_FILE__, __LINE__, __FUNCTION__, moYaoQianShuCampInfo.miId, loCampInfo.miId );
        }

        // 随机给与玩家奖励
        if ( loCampInfo.miSettlePhase==-1 )
        {
            EncourageItem( liPlayerId, liGrowing );
        }

        // 检查成长度公告
        if ( loCampInfo.miSettlePhase == -1 )
        {
            CheckGrowingNotice( loCampInfo.miGrowingValue );
        }

        // 当摇钱等级上升时，向场景中的所有玩家发送晋级通知。
        if ( moYaoQianShuCampInfo.miLevel > loCampInfo.miLevel )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] the yaoqianshu level isn't match game(%d) gate(%d)", 
                __LK_FILE__, __LINE__, __FUNCTION__, moYaoQianShuCampInfo.miLevel, loCampInfo.miLevel );
        }else if ( moYaoQianShuCampInfo.miLevel< loCampInfo.miLevel )
        {
            if ( loCampInfo.miSettlePhase<=0 )
            {
                 ChangeYaoQianShuState( loCampInfo.miLevel );
            }
        }

        moYaoQianShuCampInfo = loCampInfo;

        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start OnMessageUpdateYQSGrowingResponse message!", 
            __LK_FILE__, __LINE__, __FUNCTION__ );
        return;
}

void CActivityYqs::ChangeYaoQianShuState( int nLevel )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start Handle ChangeYaoQianShuState message! nLevel(%d)", 
        __LK_FILE__, __LINE__, __FUNCTION__, nLevel  );

    CYqsCfgLevelInfoUnit* lpLevelInfo = CActivityYqsCfg::GetSingleton().GetLevelInfo( nLevel );
    if ( lpLevelInfo==NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get level info by level(%d)", 
            __LK_FILE__, __LINE__, __FUNCTION__, nLevel );
        return;
    }
    int liYqsNpcId  = CActivityYqsCfg::GetSingleton().miYaoQianShuNPCID;
    COgreCreator* pCreator = ( COgreCreator*)CCreatorManager::GetSingletonPtr()->GetCreator( liYqsNpcId );
    if ( pCreator == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get creator by id(%d)!", __LK_FILE__, __LINE__, __FUNCTION__,liYqsNpcId );
        return;
    }
    CEntityNpc *tpEntity = ( CEntityNpc * )CSceneLogic::GetSingleton().GetEntity( pCreator->GetAttachedEntityID() );
    if ( tpEntity == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get tpEntity by id(%d)!", __LK_FILE__, __LINE__, __FUNCTION__,liYqsNpcId );
        return;
    }

    CTemplateBuff* tpBuff = (CTemplateBuff*)CDataStatic::SearchTpl( lpLevelInfo->miBuff );
    if ( tpBuff == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get buff by level(%d)", 
            __LK_FILE__, __LINE__, __FUNCTION__, nLevel );
        return;
    }
	
    int liRet = tpEntity->EntityInsertBuff( tpEntity, tpBuff );
	int pTime = tpBuff->mLastTime/1000;
	if (pTime >= 60)
	{
	  if (tpEntity->IsPlayer())
	  {
		  LogEventGetBuffByYQS((CEntityPlayer* )tpEntity,tpBuff->mTempID,pTime);  // 摇钱树成长BUFF
	  }
      
	}
	

    if ( liRet != SUCCESS )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] insert buff(%d) to npc(%d) failed!", 
            __LK_FILE__, __LINE__, __FUNCTION__, lpLevelInfo->miBuff, CActivityYqsCfg::GetSingleton().miYaoQianShuNPCID );
            return;
    }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish Handle ChangeYaoQianShuState message!", 
        __LK_FILE__, __LINE__, __FUNCTION__  );
    return;
}

bool CActivityYqs::SendGrowingValueToGate( int nPlayerId, int iCampId, int iGrowingValue )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] Start handle SendGrowingValueToGate!", __LK_FILE__, __LINE__, __FUNCTION__ );
    CMessage        tTotalMessage;
    CMessageHead*   tpHead = NULL;
    tpHead = tTotalMessage.mutable_msghead();
    if ( tpHead == NULL )
    {
        LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] can't create message head", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;
    }
    tpHead->set_messageid( ID_S2G_UPDATEYQSGROWING_REQUEST );

    CMessageS2GUpdateYQSGrowingRequest tMessage;
    tMessage.set_playerid( nPlayerId );
    tMessage.set_campid( iCampId );
    tMessage.set_growingchangevalue( iGrowingValue );

    LOG_NOTICE( "YaoQianShu", 
        "[ %s:%d ] [ %s ] Send CMessageS2GUpdateYQSGrowingRequest(%d) campid(%d) growingchangevalue(%d)",
        __LK_FILE__, __LINE__, __FUNCTION__, tpHead->messageid(), iCampId, iGrowingValue );

    tTotalMessage.set_msgpara( (long)&tMessage );
    bool lbRet = CSceneLogic::GetSingletonPtr()->Send2Gate( &tTotalMessage );
    if ( lbRet == false )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ] [ %s ] Send message to gate failed!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return false;        
    }
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] Finish handle SendGrowingValueToGate!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return true;
}

void CActivityYqs::SendEncourageNoticeBroadcast( int nCampId, int nEncourageId )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] Start handle SendEncourageNoticeBroadcast!", __LK_FILE__, __LINE__, __FUNCTION__ );

    CMessage tMessage;
    CMessageYQSEncourageChangeNotice tPara;
    tMessage.mutable_msghead()->set_messageid( ID_S2C_YQSENCOURAGE_NOTICE );
    tMessage.set_msgpara( (long) &tPara );

    tPara.set_campid( nCampId );
    tPara.set_encourangeid( nEncourageId );

    LOG_NOTICE( "YaoQianShu", 
        "[ %s:%d ] [ %s ] Send CMessageYQSEncourageChangeNotice(%d) campid(%d) encourangeid(%d)",
        __LK_FILE__, __LINE__, __FUNCTION__, tMessage.mutable_msghead()->messageid(), nCampId, nEncourageId );


    CSceneLogic::GetSingleton().SendSceneMessage( &tMessage );

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] Finish handle SendEncourageNoticeBroadcast!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return; 
}

void CActivityYqs::SendPhaseChangeNotice( int nCampId, int nIsFinished, int nCurrLevel )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] Start handle SendPhaseChangeNotice!", __LK_FILE__, __LINE__, __FUNCTION__ );

    CMessage tMessage;
    CMessageYQSPhaseChangeNotice tPara;
    tMessage.mutable_msghead()->set_messageid( ID_S2C_YQSPHASECHANGE_NOTICE );
    tMessage.set_msgpara( (long) &tPara );

    tPara.set_campid( nCampId );
    tPara.set_currphase( moYaoQianShuCampInfo.miLevel );
    tPara.set_isfinished( nIsFinished );

    LOG_NOTICE( "YaoQianShu", 
        "[ %s:%d ] [ %s ] Send CMessageYQSPhaseChangeNotice(%d) campid(%d) currphase(%d) isfinished(%d)",
        __LK_FILE__, __LINE__, __FUNCTION__, tMessage.mutable_msghead()->messageid(), nCampId, moYaoQianShuCampInfo.miLevel, nIsFinished );

    CSceneLogic::GetSingleton().SendSceneMessage( &tMessage );

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] Finish handle SendPhaseChangeNotice!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return;

}

void CActivityYqs::OnMessageHandinRequest( CEntityPlayer *npPlayer, CMessage *npMessage )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start Handle OnMessageHandinRequest", __LK_FILE__, __LINE__, __FUNCTION__ );
    LK_ASSERT(npPlayer != NULL && npMessage != NULL, return);
    CMessageHandInRequest* tpReq = ( CMessageHandInRequest* ) npMessage->msgpara(); 
    if ( tpReq == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get message CMessageHandInRequest", __LK_FILE__, __LINE__, __FUNCTION__ );
        return ;
    }

    int liPlayer                = tpReq->playerid();
    int liActivityId            = tpReq->activityid();

    // 验证活动是否开始
    if ( mbIsStarted == false )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] the Activity isn't started", __LK_FILE__, __LINE__, __FUNCTION__ );
        SendHandinRequest( liPlayer, EM_YQS_ACTIVITYNOTSTART, 0, 0 );
        return ;
    }

    const ItemForHandin loItem       = tpReq->items( 0 );
    int liItemIndex             = loItem.itemid();

    if ( npPlayer->GetCharID() != (unsigned int)liPlayer )
    {
        LOG_ERROR( "YaoQianShu", 
            "[ %s:%d ][ %s ] the player id isn't matched! old(%d) new(%d)", 
            __LK_FILE__, __LINE__, __FUNCTION__, npPlayer->GetCharID(), liPlayer );
        SendHandinRequest( liPlayer, EM_YQS_USERNOTMATCH, 0, 0 );
        return;
    }

    if ( HELPID_YAOQIANSHU != liActivityId )
    {
        LOG_ERROR( "YaoQianShu", 
            "[ %s:%d ][ %s ] the Activity id(%d) invalied!", 
            __LK_FILE__, __LINE__, __FUNCTION__, liActivityId );
        SendHandinRequest( liPlayer, EM_YQS_ACTIVITYIDNOTMATCH, 0, 0 );
        return ;
    }

    // 获取奖励
    CItemObject* lpItemObj  = npPlayer->GetItemObject( liItemIndex );
    if ( lpItemObj == NULL )
    {
        LOG_ERROR( "YaoQianShu", 
            "[ %s:%d ][ %s ] the item id(%d) invalied!",  
            __LK_FILE__, __LINE__, __FUNCTION__, liItemIndex );
        SendHandinRequest( liPlayer, EM_YQS_HANDINITEMINVALIED, 0, 0 );
        return ;
    }
    int liItemID            = lpItemObj->GetItemID();
    int liItemNum           = lpItemObj->GetItemNum();
    CHandInEncourageUnit* lpEncourage = CActivityYqsCfg::GetSingleton().GetHandInEncourage( liItemID );
    if ( lpEncourage == NULL || lpEncourage->miItemId != liItemID )
    {
        LOG_ERROR( "YaoQianShu", 
            "[ %s:%d ][ %s ] Can't get encourage unit for item(%d)!", 
            __LK_FILE__, __LINE__, __FUNCTION__, liItemID );
        SendHandinRequest( liPlayer, EM_YQS_HANDINITEMINVALIED, 0, 0 );
        return;
    }

    if ( liItemNum <= 0 )
    {
        LOG_ERROR( "YaoQianShu", 
            "[ %s:%d ][ %s ] the encourage item(%d) num(%d) isn't matched!, need(%d)", 
            __LK_FILE__, __LINE__, __FUNCTION__, liItemID, liItemNum, lpEncourage->miItemNum );
        SendHandinRequest( liPlayer, EM_YQS_HANDINITEMINVALIED, 0, 0 );
        return;
    }

    int liRet = CPropertyModule::GetSingleton().PlayerRemoveItem( npPlayer, liItemIndex, 1 );
    if ( liRet != 0  )
    {
        LOG_ERROR( "YaoQianShu", 
            "[ %s:%d ][ %s ] player(%d:%s)remove item(%d) num(%d) failed", 
            __LK_FILE__, __LINE__, __FUNCTION__, npPlayer->GetCharID(),npPlayer->GetCharName(), 
            liItemIndex, 1 );
        SendHandinRequest( liPlayer, EM_YQS_HANDINITEMINVALIED, 0, 0 );
        return;
    }

    int liCharId        = npPlayer->GetCharID();
    int liCampId        = npPlayer->GetNationality();

    // 更新摇钱树成长度
    int liGrowingValue = CActivityYqsCfg::GetSingleton().GetGrowingValue( liItemID );
    if ( liGrowingValue==0 )
    {
        LOG_ERROR( "YaoQianShu", 
            "[ %s:%d ][ %s ] Can't get growing value for item(%d)", 
            __LK_FILE__, __LINE__, __FUNCTION__, liItemIndex );
        SendHandinRequest( liPlayer, EM_YQS_GETGROWINGVALUEFAILED, 0, 0 );
        return;
    }
    bool lbRetForSendGrowingValue = SendGrowingValueToGate(  liCharId, liCampId, liGrowingValue );
    if ( lbRetForSendGrowingValue == false )
    {
        LOG_ERROR( "YaoQianShu", 
            "[ %s:%d ][ %s ] SendGrowingValueToGate failed!", 
            __LK_FILE__, __LINE__, __FUNCTION__ );
        SendHandinRequest( liPlayer, EM_YQS_SENDUPDATEMSGTOGATEFAILED, 0, 0 );
        return;
    }

    // 计算用户所得到的经验值，贡献度，上缴仙露数
    int liPlayerHandinDawNum = npPlayer->moYaoQianShu.GetHandInDawNum();
    if ( liPlayerHandinDawNum < CActivityYqsCfg::GetSingleton().miHandinNumLimitePerUser  )
    {
        // 添加用户检验值奖励
        int liPlayerLevel   = npPlayer->GetLevel();
        int liBaseLevelExp  = GetLevelExp( liPlayerLevel );
        int liExpChange     = liBaseLevelExp * lpEncourage->miEncourageNum;
        int nFinalExp = CPropertyModule::GetSingleton().PlayerObtainExp2( npPlayer, liExpChange, 0,EXPREASON_HANDINITEM,&liCharId,1 );
        if ( nFinalExp>0 )
        {
            LogEventGetExpByHandin( npPlayer, liExpChange, nFinalExp, liItemID );
        }

        // 添加用户贡献度
        npPlayer->moYaoQianShu.Handin( liCharId, liGrowingValue );

        // 更新用户贡献度排名
        CPlayerRankInfoUnit loPlayerRankUnit;
        loPlayerRankUnit.miPlayerID         = npPlayer->GetCharID();
        loPlayerRankUnit.mstrPlayerName     = npPlayer->GetCharName();
        loPlayerRankUnit.miPlayerMetie      = npPlayer->GetMetier();
        loPlayerRankUnit.miPlayerLevel      = npPlayer->GetLevel();
        loPlayerRankUnit.miContribute       = npPlayer->moYaoQianShu.miContribute;

        bool lbRet = CActivityYqsPlayerRankInfo::GetSingleton().UpdatePlayerRankInfo( loPlayerRankUnit );
        if ( lbRet == false )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] UpdatePlayerRankInfo failed", __LK_FILE__, __LINE__, __FUNCTION__ );
        }

        // 发送用户经验值与贡献度公告
        //int     liParam[2]   = {0};
        //liParam[0]          = liBaseLevelExp;
        //liParam[1]          = liGrowingValue;
        //CPropertyModule::GetSingletonPtr()->SendEventNotify( NOTICE_YQS_IRRIGATE1, 
        //    CSceneLogic::GetSingleton().GetLineID(), 
        //    liParam, ARRAY_CNT(liParam), NULL, NULL );

        LOG_NOTICE( "YaoQianShu", 
            "[ %s:%d ][ %s ] Give player exp and growing!", 
            __LK_FILE__, __LINE__, __FUNCTION__ );

    }else
    {
        // 增加用户贡献度
        npPlayer->moYaoQianShu.Handin( liCharId, liGrowingValue );

        // 更新用户贡献度排名
        CPlayerRankInfoUnit loPlayerRankUnit;
        loPlayerRankUnit.miPlayerID         = npPlayer->GetCharID();
        loPlayerRankUnit.mstrPlayerName     = npPlayer->GetCharName();
        loPlayerRankUnit.miPlayerMetie      = npPlayer->GetMetier();
        loPlayerRankUnit.miPlayerLevel      = npPlayer->GetLevel();
        loPlayerRankUnit.miContribute       = npPlayer->moYaoQianShu.miContribute;

        bool lbRet = CActivityYqsPlayerRankInfo::GetSingleton().UpdatePlayerRankInfo( loPlayerRankUnit );
        if ( lbRet == false )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] UpdatePlayerRankInfo failed", __LK_FILE__, __LINE__, __FUNCTION__ );
        }

        // 发送用户经验值与贡献度公告
        //int     liParam     = 0;
        //CPropertyModule::GetSingletonPtr()->SendEventNotify( NOTICE_YQS_IRRIGATE2, 
        //    CSceneLogic::GetSingleton().GetLineID(), 
        //    &liParam, 1, NULL, NULL );

        LOG_NOTICE( "YaoQianShu", 
            "[ %s:%d ][ %s ] Give player exp and growing!", 
            __LK_FILE__, __LINE__, __FUNCTION__ );
    }

    SendHandinRequest( liPlayer, 1, npPlayer->moYaoQianShu.GetHandInDawNum(), liGrowingValue );

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish Handle OnMessageHandinRequest", __LK_FILE__, __LINE__, __FUNCTION__ );
    return;
}

void CActivityYqs::CheckActivity( int nActivityId, int nState )
{
    if ( IsYaoQianShuActiviy( nActivityId ) )
    {
        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] YaoQianShu Activiy(%d) state(%d) changed!", 
            __LK_FILE__, __LINE__, __FUNCTION__, nActivityId, nState );
        switch ( nState )
        {
        case ACTIVITYSTATE_STARTED:
            {
                StartActivity();
                break;
            }
        case ACTIVITYSTATE_NOT_START:
            {
                EndActivity();
                break;
            }
        default:
            break;
        }
    }
}

bool CActivityYqs::StartActivity()
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] YaoQianShu Activiy started!", __LK_FILE__, __LINE__, __FUNCTION__ );
    mbIsStarted = true;
    mbIsLoaded  = false;
    mbIsFinished   = false;
    moYaoQianShuCampInfo.clear();
    CActivityYqsPlayerRankInfo::GetSingleton().Clear();

    int liYqsNpc    = CActivityYqsCfg::GetSingleton().miYaoQianShuNPCID;
    int liSctzNpc   = CActivityYqsCfg::GetSingleton().miShanCaiTongZhiNPCID;
    int liLine      = CSceneLogic::GetSingleton().GetLineID();

    // 创建摇钱树NPC
    COgreCreator* pCreator = ( COgreCreator*)CCreatorManager::GetSingletonPtr()->GetCreator( liYqsNpc );
    if ( pCreator == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get creator by id(%d)!", __LK_FILE__, __LINE__, __FUNCTION__, liYqsNpc);
        return false;
    }
    CEntity* lpEYqs = CSceneLogic::GetSingleton().GetEntity( pCreator->GetAttachedEntityID() );
    if ( lpEYqs != NULL )
    {
        CSceneLogic::GetSingleton().DestroyEntity( lpEYqs, 0 );
    }
    unsigned int liEntityId1  = 0;
    bool lbRetForCreateYqs = CMapModule::GetSingleton().CreateNpc( liLine, 
        pCreator->mMapID, 
        0, 
        pCreator, 
        liEntityId1 );
    if ( lbRetForCreateYqs == false )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Create npc(%d) failed!", __LK_FILE__, __LINE__, __FUNCTION__, liYqsNpc );
        return false;
    }

    // 创建散财童子NPC
    COgreCreator* pCreator2 = ( COgreCreator*)CCreatorManager::GetSingletonPtr()->GetCreator( liSctzNpc );
    if ( pCreator2 == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get creator by id(%d)!", __LK_FILE__, __LINE__, __FUNCTION__,liSctzNpc );
        return false;
    }
    CEntity* lpESctz = CSceneLogic::GetSingleton().GetEntity( pCreator2->GetAttachedEntityID() );
    if ( lpESctz != NULL )
    {
        CSceneLogic::GetSingleton().DestroyEntity( lpESctz, 0 );
    }
    unsigned int liEntityId2  = 0;
    bool lbRetForCreateSctz = CMapModule::GetSingleton().CreateNpc( liLine, 
        pCreator2->mMapID, 
        0, 
        pCreator2, 
        liEntityId2 );
    if ( lbRetForCreateSctz == false )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Create npc(%d) failed!", __LK_FILE__, __LINE__, __FUNCTION__, liSctzNpc );
        return false;
    }

    return true;
}

bool CActivityYqs::EndActivity()
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] YaoQianShu Activiy End!", __LK_FILE__, __LINE__, __FUNCTION__ );
    mbIsStarted = false;
    mbIsLoaded  = false;
    moYaoQianShuCampInfo.clear();

    int liYqsNpc    = CActivityYqsCfg::GetSingleton().miYaoQianShuNPCID;
    int liSctzNpc   = CActivityYqsCfg::GetSingleton().miShanCaiTongZhiNPCID;

    //  销毁摇钱树NPC
    COgreCreator* pYqsCreator = ( COgreCreator*)CCreatorManager::GetSingletonPtr()->GetCreator( liYqsNpc );
    if ( pYqsCreator == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get creator by id(%d)!", __LK_FILE__, __LINE__, __FUNCTION__, liYqsNpc);
        return false;
    }
    CEntity *tpYqsNpc = CSceneLogic::GetSingleton().GetEntity( pYqsCreator->GetAttachedEntityID() );
    if ( tpYqsNpc == NULL )
    {
        LOG_ERROR("YaoQianShu", "[ %s:%d ][ %s ] Can't get npc by id(%d)", __LK_FILE__, __LINE__, __FUNCTION__, liYqsNpc );
        return false;
    }
    CSceneLogic::GetSingleton().DestroyEntity( tpYqsNpc, 0 );

    //  销毁散财童子NPC
    COgreCreator* pSctzCreator = ( COgreCreator*)CCreatorManager::GetSingletonPtr()->GetCreator( liSctzNpc );
    if ( pSctzCreator == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get creator by id(%d)!", __LK_FILE__, __LINE__, __FUNCTION__, liSctzNpc);
        return false;
    }
    CEntity *tpSctz = CSceneLogic::GetSingleton().GetEntity( pSctzCreator->GetAttachedEntityID() );
    if ( tpSctz == NULL )
    {
        LOG_ERROR("YaoQianShu", "[ %s:%d ][ %s ] Can't get npc by id(%d)", __LK_FILE__, __LINE__, __FUNCTION__, liSctzNpc );
        return false;
    }
    CSceneLogic::GetSingleton().DestroyEntity( tpSctz, 0 );

    // 重新清理宝箱
    for ( int i=0; i<(int)mStoreBoxCreated.size();i++ )
    {
        int liStoreBoxNpc    = mStoreBoxCreated[i].miNpcID;
        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] destory store box(%d)!", __LK_FILE__, __LINE__, __FUNCTION__,liStoreBoxNpc );
        COgreCreator* pCreator = ( COgreCreator*)CCreatorManager::GetSingletonPtr()->GetCreator( liStoreBoxNpc );
        if ( pCreator == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get creator by id(%d)!", __LK_FILE__, __LINE__, __FUNCTION__,liStoreBoxNpc );
            continue;
        }
        CEntityBox *tpStoreBoxNpc = (CEntityBox*)CSceneLogic::GetSingleton().GetEntity( pCreator->GetAttachedEntityID() );
        if ( tpStoreBoxNpc == NULL )
        {
            LOG_ERROR("YaoQianShu", "[ %s:%d ][ %s ] Can't get npc by id(%d)", __LK_FILE__, __LINE__, __FUNCTION__, liStoreBoxNpc );
            continue;
        }
        CSceneLogic::GetSingleton().DestroyEntity( tpStoreBoxNpc, 0 );
    }

    return true;
}

bool CActivityYqs::IsYaoQianShuActiviy( int nActivityId )
{
    return nActivityId==HELPID_YAOQIANSHU?true:false;
}

void CActivityYqs::SendDropItemNotice( int nPlayerId, int nItemId, int nItemNum )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] Start handle SendDropItemNoticeBroadcast!", __LK_FILE__, __LINE__, __FUNCTION__ );

    CMessage tMessage;
    CMessageYQSDropItemNotice tPara;
    tMessage.mutable_msghead()->set_messageid( ID_S2C_YQSDROPITEM_NOTICE );
    tMessage.set_msgpara( (long) &tPara );

    tPara.set_playerid( nPlayerId );
    tPara.set_itemid( nItemId );
    tPara.set_itemnum( nItemNum );

    LOG_NOTICE( "YaoQianShu", 
        "[ %s:%d ] [ %s ] Send SendDropItemNoticeBroadcast(%d) playerid(%d) item(%d) itemnum(%d)",
        __LK_FILE__, __LINE__, __FUNCTION__, tMessage.mutable_msghead()->messageid(), nPlayerId, nItemId, nItemNum );


    int liCampID    = CSceneLogic::GetSingleton().GetLineID();
    CSceneLogic::GetSingleton().SendSceneMessage( &tMessage, liCampID );

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] Finish handle SendDropItemNoticeBroadcast!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return;
}

void CActivityYqs::OnMessageActivityStartNotice( CMessage* npMessage )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start handle OnMessageActivityStartNotice message!", 
        __LK_FILE__, __LINE__, __FUNCTION__ );

    LK_ASSERT(npMessage != NULL, return);
    CMessageYQSActivtyStartNotice* tpResponse = ( CMessageYQSActivtyStartNotice* ) npMessage->msgpara(); 
    if ( tpResponse == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get CMessageYQSActivtyStartNotice message!", 
            __LK_FILE__, __LINE__, __FUNCTION__ );
        return ;
    }

    int liId                    = tpResponse->id();
    int liCampId                = tpResponse->campid();
    int liCampRankId            = tpResponse->camprankid();
    int liGrowingValue          = tpResponse->growing();
    int liLevel                 = tpResponse->level();
    int liHandinDawNum          = tpResponse->handindawnum();
    int liSettlePhase           = tpResponse->settlephase();


    moYaoQianShuCampInfo.miId               = liId;
    moYaoQianShuCampInfo.miActivityID       = HELPID_YAOQIANSHU;
    moYaoQianShuCampInfo.miCampId           = liCampId;
    moYaoQianShuCampInfo.miSettlePhase      = liSettlePhase;
    moYaoQianShuCampInfo.miCampRank         = liCampRankId;
    moYaoQianShuCampInfo.miGrowingValue     = liGrowingValue;
    moYaoQianShuCampInfo.miLevel            = liLevel;
    moYaoQianShuCampInfo.miHandinDawNum     = liHandinDawNum;

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] OnMessageActivityStartNotice message! "
        "miId(%d) miActivityID(%d) miCampId(%d) miSettlePhase(%d) "
        "miCampRank(%d) miGrowingValue(%d) miLevel(%d) miHandinDawNum(%d)", 
        __LK_FILE__, __LINE__, __FUNCTION__,
        moYaoQianShuCampInfo.miId, moYaoQianShuCampInfo.miActivityID, 
        moYaoQianShuCampInfo.miCampId, moYaoQianShuCampInfo.miSettlePhase,
        moYaoQianShuCampInfo.miCampRank, moYaoQianShuCampInfo.miGrowingValue,
        moYaoQianShuCampInfo.miLevel, moYaoQianShuCampInfo.miHandinDawNum );

    CActivityYqsPlayerRankInfo::GetSingleton().Clear();

    // 清理过期的成长通知
    mGrowingNotice.clear();
    CActivityYqsCfg::t_notice_map_itor loItor = CActivityYqsCfg::GetSingleton().mGrowingNotice.begin();
    for ( ; loItor!=CActivityYqsCfg::GetSingleton().mGrowingNotice.end(); loItor++  )
    {
        int liGrowingValueForNotice  = loItor->first;
        int liNoticeID               = loItor->second;
        if ( liGrowingValueForNotice > liGrowingValue )
        {
            LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] liGrowingValueForNotice(%d) liGrowingValue(%d) (%d)!", 
               __LK_FILE__, __LINE__, __FUNCTION__, liGrowingValueForNotice, liGrowingValue, liNoticeID );
            t_map_pair lPair = mGrowingNotice.insert( t_map_value( liGrowingValueForNotice, liNoticeID ) );
            if ( lPair.second == false )
            {
                LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] insert notice failed!", 
                    __LK_FILE__, __LINE__, __FUNCTION__, liGrowingValueForNotice, liGrowingValue, liNoticeID );
            }
        }
    }

    // 向服务器发送摇钱树活动开始消息
    // SendActivityStart( HELPID_YAOQIANSHU, liCampId );

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finished handle OnMessageActivityStartNotice message!", 
        __LK_FILE__, __LINE__, __FUNCTION__ );
    return;
}

void CActivityYqs::EncourageItem( int nPlayerId, int nGrowing )
{
    CEntityPlayer* lpPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID( nPlayerId );
    if ( lpPlayer == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get player(%d) entity", 
            __LK_FILE__, __LINE__, __FUNCTION__, nPlayerId );
        return;
    }

    int liYqsNpcId  = CActivityYqsCfg::GetSingleton().miYaoQianShuNPCID;
    COgreCreator* pYqsCreator = ( COgreCreator*)CCreatorManager::GetSingletonPtr()->GetCreator( liYqsNpcId );
    if ( pYqsCreator == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get sztz's creator by id(%d)!", __LK_FILE__, __LINE__, __FUNCTION__, liYqsNpcId );
        return;
    }
    CEntityOgre *tpSctz = (CEntityOgre*)CSceneLogic::GetSingleton().GetEntity( pYqsCreator->GetAttachedEntityID() );
    if ( tpSctz == NULL )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get sztz's NPC(%d)'s entity", 
            __LK_FILE__, __LINE__, __FUNCTION__, liYqsNpcId );
        return;
    }
    int liRandom                = RAND(SERVER_PERCENT_INT);
    CYqsCfgPrizeInfoUnit        loPrize;
    bool                        lbIsPrize= false;
    for( int i=0; i<(int)CActivityYqsCfg::GetSingleton().mPrizeInfo.size(); i++ )
    {
        loPrize = CActivityYqsCfg::GetSingleton().mPrizeInfo[i];
        if ( nGrowing == loPrize.miGrowing )
        {
			if ( liRandom < loPrize.miRandom )
			{
				lbIsPrize = true;
			}
            break;
        }
    }

    if ( lbIsPrize == false )
    {
        return;
    }

    // 通知全阵营物品掉落
    char lszBuf[1024] = {0};
    sprintf( lszBuf, "%s", lpPlayer->GetCharName() );
    char* lpTemp = lszBuf; 
    CPropertyModule::GetSingletonPtr()->SendEventNotify( NOTICE_YQS_IRRIGATEPRIZE, 
        CSceneLogic::GetSingleton().GetLineID(), 
        &loPrize.miPrizeItemNum, 1, lpTemp, NULL );

    // 将物品插入用户包裹
    CItemObject *pInsItemPile = CPropertyModule::GetSingleton().PlayerInsertItem( ( CEntityPlayer* )lpPlayer, loPrize.miPrizeItemId, loPrize.miPrizeItemNum );
    if ( pInsItemPile != NULL)
    {
        // 将奖励物品插入背包后进行记录
        LogEventGetItemByDirectIns(( CEntityPlayer* )lpPlayer, 
            pInsItemPile->GetGUID(), loPrize.miPrizeItemId, loPrize.miPrizeItemNum, 
            CDropFrom::em_drop_from_npc, 
            0, 
            tpSctz->GetEntityID() );
    }else
    {
        // 发送掉落物品提示
        SendDropItemNotice( lpPlayer->GetCharID(), loPrize.miPrizeItemId, loPrize.miPrizeItemNum );

        // 掉落物品
        for ( int i=1; i<=loPrize.miPrizeItemNum; i++ )
        {
            // 若不能插入物品这将物品作为NPC掉落
            int liPlayerChatID  = lpPlayer->GetCharID();
            CItemCreator tCreator( loPrize.miPrizeItemId, 5, lpPlayer->GetPosX( ), lpPlayer->GetPosY( ), &liPlayerChatID, NULL );
            unsigned int tEntityID = 0;
            CMapModule::GetSingleton().CreateNpc( lpPlayer->GetLineID(),
                lpPlayer->GetMapID(),
                lpPlayer->GetMapIndex(),
                &tCreator,
                tEntityID );
        }
    }
}

void CActivityYqs::CheckGrowingNotice( int nGrowingValue )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start Handle CheckGrowingNotice!", __LK_FILE__, __LINE__, __FUNCTION__ );

    t_map_itor loItor = mGrowingNotice.begin();
    for ( ; loItor!=mGrowingNotice.end(); loItor++ )
    {
        int liGrowingValue  = loItor->first;
        int liNoticeCode    = loItor->second;
        if ( liGrowingValue < nGrowingValue )
        {
            LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Send growing notice! liGrowingValue(%d), liNoticeCode(%d)", 
                __LK_FILE__, __LINE__, __FUNCTION__, liGrowingValue, liNoticeCode );
            CPropertyModule::GetSingletonPtr()->SendEventNotify( liNoticeCode, 
                CSceneLogic::GetSingleton().GetLineID(), 
                NULL, 0, NULL, NULL );
            mGrowingNotice.erase( loItor );
            return;
        }
    }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish Handle CheckGrowingNotice!", __LK_FILE__, __LINE__, __FUNCTION__ );
}

int CActivityYqs::GetLevelExp( int nLevel )
{
    if ( nLevel<0 || nLevel>TMP_MAX_GRADE )
    {
        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] the level(%d) is invalid!", __LK_FILE__, __LINE__, __FUNCTION__, nLevel );
        return 0;
    }

    int liLevelExpTpl = CActivityYqsCfg::GetSingleton().miLevelExpTemplete;
    CTemplateRedStoneExpTable* lpLevelExp = (CTemplateRedStoneExpTable*)CDataStatic::SearchTpl( liLevelExpTpl );
    if ( lpLevelExp == NULL )
    {
        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Can't get lpLevelExp(%d)!", __LK_FILE__, __LINE__, __FUNCTION__, liLevelExpTpl );
        return 0;
    }
    return lpLevelExp->mExp[ nLevel ];
}

int CActivityYqs::GetActivtyId( void )
{
    return moYaoQianShuCampInfo.miId;
}

int CActivityYqs::GetSettlePhase( void )
{
    return moYaoQianShuCampInfo.miSettlePhase;
}

void CActivityYqs::Initialize()
{
    YQSACTIVITY_LOG* tpLog = CSceneCfgManager::GetSingletonPtr()->GetYqsactivity_log();
    if( tpLog != NULL )
    {
        INIT_ROLLINGFILE_LOG( tpLog->mIdentifier, 
            tpLog->mDirectory, 
            ( LogLevel ) tpLog->mPriority , 
            tpLog->mMaxfilesize, 
            tpLog->mMaxbackupindex);
    }

    CActivityYqsPlayerRankInfo::GetSingleton().InitializeForYqsPlayerRank( MAX_PLAER_RANK_NUM );
}

void CActivityYqs::OnMessageGetCampRankInfoForClient( CEntityPlayer *npPlayer, CMessage *npMessage )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start Handle OnMessageGetCampRankInfoForClient!", __LK_FILE__, __LINE__, __FUNCTION__ );
    LK_ASSERT(npPlayer != NULL && npMessage != NULL, return);
    CMessageC2SGetCampRankForYQSRequest* tpReq = ( CMessageC2SGetCampRankForYQSRequest* ) npMessage->msgpara(); 
    if ( tpReq == NULL )
    {
        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Can't get CMessageC2SGetCampRankForYQSRequest!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return ;
    }

    int liPlayerId                = tpReq->playerid();
    int liParam1                  = tpReq->param1();
    if ( npPlayer->GetCharID() != (unsigned int)liPlayerId )
    {
        LOG_NOTICE( "YaoQianShu", 
            "[ %s:%d ][ %s ] the player id isn't match! old(%d) new(%d)", 
            __LK_FILE__, __LINE__, __FUNCTION__, npPlayer->GetCharID(), liPlayerId );
        return;
    }

    CMessage        tTotolMessage;
    CMessageHead*   tpHead = NULL;
    tpHead = tTotolMessage.mutable_msghead();
    if ( tpHead == NULL )
    {
        LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] can't create message head", __LK_FILE__, __LINE__, __FUNCTION__ );
        return ;
    }
    tpHead->set_messageid( ID_S2G_GETCAMPRANKFORYQS_REQUEST );
    CMessageS2GGetCampRankForYQSRequest tMessage;
    tMessage.set_playerid( liPlayerId );
    tMessage.set_param1( liParam1 );

    LOG_NOTICE( "YaoQianShu", 
        "[ %s:%d ] [ %s ] Send CMessageS2GGetCampRankForYQSRequest(%d) to player(%d) param1(%d)", 
        __LK_FILE__, __LINE__, __FUNCTION__, tpHead->messageid(), liPlayerId, liParam1 );

    tTotolMessage.set_msgpara( (long) &tMessage );
    bool lbRet = CSceneLogic::GetSingleton().Send2Gate( &tTotolMessage );
    if ( lbRet == false )
    {
        LOG_NOTICE( "YaoQianShu", 
            "[ %s:%d ] [ %s ] Send CMessageS2GGetCampRankForYQSRequest(%d) to Gate failed! player(%d) param1(%d)", 
            __LK_FILE__, __LINE__, __FUNCTION__, tpHead->messageid(), liPlayerId, liParam1 );
        return;
    }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish Handle OnMessageGetCampRankInfoForClient!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return ;
}

void CActivityYqs::OnMesageGetCampRankInfoForGate( CMessage* npMessage )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start Handle OnMesageGetCampRankInfoForGate!", __LK_FILE__, __LINE__, __FUNCTION__ );
    LK_ASSERT( npMessage != NULL, return);
    CMessageG2SGetCampRankForYQSResponse* tpReq = ( CMessageG2SGetCampRankForYQSResponse* ) npMessage->msgpara(); 
    if ( tpReq == NULL )
    {
        LOG_NOTICE( "YaoQianShu", 
            "[ %s:%d ][ %s ] Can't get CMessageG2SGetCampRankForYQSResponse message!", 
            __LK_FILE__, __LINE__, __FUNCTION__ );
        return ;
    }

    int liPlayerId                = tpReq->playerid();
    int liParam1                  = tpReq->param1();


    CEntityPlayer* pPlayer = CSceneLogic::GetSingletonPtr()->GetPlayerByCharID( liPlayerId );
    if ( pPlayer == NULL )
    {
        LOG_NOTICE( "YaoQianShu", 
            "[ %s:%d ][ %s ] Can't get CEntityPlayer(%d)!", 
            __LK_FILE__, __LINE__, __FUNCTION__, liPlayerId );
        return ;
    }
    int liPlayerHandinNum   = pPlayer->moYaoQianShu.miHandinDawNum;
    int liPlayerContribute  = pPlayer->moYaoQianShu.miContribute;

    CMessage        tTotolMessage;
    CMessageHead*   tpHead = NULL;
    tpHead = tTotolMessage.mutable_msghead();
    if ( tpHead == NULL )
    {
        LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] can't create message head", __LK_FILE__, __LINE__, __FUNCTION__ );
        return ;
    }
    tpHead->set_messageid( ID_S2C_GETCAMPRANKFORYQS_RESPONSE );
    CMessageS2CGetCampRankForYQSResponse tMessage;
    tMessage.set_playerid( liPlayerId );
    tMessage.set_param1( liParam1 );
    tMessage.set_playerhandindawnum( liPlayerHandinNum );
    tMessage.set_playercontribute( liPlayerContribute );

    LOG_NOTICE( "YaoQianShu", 
        "[ %s:%d ] [ %s ] Send CMessageS2CGetCampRankForYQSResponse(%d) to player(%d) param1(%d) HandinNum(%d) Contribute(%d)", 
        __LK_FILE__, __LINE__, __FUNCTION__, tpHead->messageid(), liPlayerId, liParam1, liPlayerHandinNum, liPlayerContribute );

    for ( int i=0; i<tpReq->camprankinfo_size(); i++ )
    {
        const CampRankForYQSForSever loCampRank = tpReq->camprankinfo( i );
        int liRank              =  loCampRank.rank();
        int liCampId            = loCampRank.campid();
        int liGrowingGrade      = loCampRank.growinggrade();
        int liGrowingPhase      = loCampRank.growingphase();

        CampRankForYQS* lpCampRankInfo = tMessage.add_camprankinfo();
        lpCampRankInfo->set_rank( liRank );
        lpCampRankInfo->set_campid( liCampId );
        lpCampRankInfo->set_growinggrade( liGrowingGrade );
        lpCampRankInfo->set_growingphase( liGrowingPhase );

        LOG_NOTICE( "YaoQianShu", 
            "[ %s:%d ] [ %s ] \t rank(%d) campid(%d) growinggrade(%d) growingphase(%d)", 
            __LK_FILE__, __LINE__, __FUNCTION__, liRank, liCampId, liGrowingGrade, liGrowingPhase );
    }

    tTotolMessage.set_msgpara( (long) &tMessage );
    CSceneLogic::GetSingleton().Send2Player( liPlayerId, &tTotolMessage );

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish Handle OnMesageGetCampRankInfoForGate!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return ;
}

void CActivityYqs::SendHandinRequest( int nPlayerId, int nResult, int nPlayerHandinNum, int nPlayerContribute )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] Start handle SendHandinRequest!",  __LK_FILE__, __LINE__, __FUNCTION__ );

    CMessage tMessage;
    CMessageHandInResponse tPara;
    tMessage.mutable_msghead()->set_messageid( ID_S2C_HANDIN_RESPONSE );
    tMessage.set_msgpara( (long) &tPara );

    tPara.set_playerid( nPlayerId );
    tPara.set_result( nResult );
    tPara.set_activityid( HELPID_YAOQIANSHU );
    if ( nResult == 1 )
    {
        tPara.set_encouragenum( 1 );
        EncourageForHandin* lpEncourage = tPara.add_encourages();
        lpEncourage->set_encourageid( nPlayerHandinNum );
        lpEncourage->set_encouragenum( nPlayerContribute );
    }

    LOG_NOTICE( "YaoQianShu", 
        "[ %s:%d ] [ %s ] Send CMessageHandInResponse(%d) playerid(%d) result(%d) nPlayerHandinNum(%d) nPlayerContribute(%d) ",
        __LK_FILE__, __LINE__, __FUNCTION__, tMessage.mutable_msghead()->messageid(), nPlayerId, nResult, nPlayerHandinNum,nPlayerContribute );

    CSceneLogic::GetSingleton().Send2Player( nPlayerId, &tMessage );

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] Finish handle SendHandinRequest!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return;
}

void CActivityYqs::SettleFinish( void )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] Start handle SettleFinish!", __LK_FILE__, __LINE__, __FUNCTION__ );

    // 清理宝箱
    for ( int i=0; i<(int)mStoreBoxCreated.size();i++ )
    {
        int liStoreBoxNpc    = mStoreBoxCreated[i].miNpcID;
        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] destory store box(%d)!", __LK_FILE__, __LINE__, __FUNCTION__,liStoreBoxNpc );
        COgreCreator* pCreator = ( COgreCreator*)CCreatorManager::GetSingletonPtr()->GetCreator( liStoreBoxNpc );
        if ( pCreator == NULL )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get creator by id(%d)!", __LK_FILE__, __LINE__, __FUNCTION__,liStoreBoxNpc );
            continue;
        }
        CEntityBox *tpStoreBoxNpc = (CEntityBox*)CSceneLogic::GetSingleton().GetEntity( pCreator->GetAttachedEntityID() );
        if ( tpStoreBoxNpc == NULL )
        {
            LOG_ERROR("YaoQianShu", "[ %s:%d ][ %s ] Can't get npc by id(%d)", __LK_FILE__, __LINE__, __FUNCTION__, liStoreBoxNpc );
            continue;
        }
        CSceneLogic::GetSingleton().DestroyEntity( tpStoreBoxNpc, 0 );
    }

    // 添加双倍时间
    CCampPrize* lpCampPrize = CActivityYqsCfg::GetSingleton().GetCampPrize( moYaoQianShuCampInfo.miCampRank );
    if ( lpCampPrize == NULL )
    {
        LOG_WARN( "YaoQianShu", "[ %s:%d ][ %s ] Can't get camp prize! miCampRank(%d)", 
            __LK_FILE__, __LINE__, __FUNCTION__, moYaoQianShuCampInfo.miCampRank );
    }else{
        time_t seconds = time( NULL );
        struct tm* pnow = localtime(&seconds);

        char tpDate[32] = {0}; 
        strftime(tpDate,  sizeof(tpDate) - 1, "%Y%m%d%H%M00", pnow); 

        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Give camp prize! "
            "tpDate(%s) CampRank(%d) miDoubleExpTime(%d) miDoubleExpRate(%d)", 
            __LK_FILE__, __LINE__, __FUNCTION__,
            tpDate, lpCampPrize->miRank, lpCampPrize->miDoubleExpTime, lpCampPrize->miDoubleExpRate );

        bool lbRet = CActivityModule::GetSingleton().AppendDoubleTime( tpDate, lpCampPrize->miDoubleExpTime, lpCampPrize->miDoubleExpRate );
        if ( lbRet == false )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Append Double time failed!", __LK_FILE__, __LINE__, __FUNCTION__ );
        }
    }

    // 给与称号奖励
    CActivityYqsPlayerRankInfo::GetSingleton().GivePlayerRankPrize();

    mbIsStarted = false;

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] Finish handle SettleFinish!", __LK_FILE__, __LINE__, __FUNCTION__ );

}

CActivityYqs::CActivityYqs()
{
    mbIsLoaded = false;
    mbIsStarted = false;
    mbIsFinished    = false;
}

void CActivityYqs::SendActivityStart( int nActivityID, int iCampID )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] Start handle SendActivityStart!", __LK_FILE__, __LINE__, __FUNCTION__ );
    CMessage        tTotalMessage;
    CMessageHead*   tpHead = NULL;
    tpHead = tTotalMessage.mutable_msghead();
    if ( tpHead == NULL )
    {
        LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] can't create message head", __LK_FILE__, __LINE__, __FUNCTION__ );
        return;
    }
    tpHead->set_messageid( ID_S2C_YQSSTART_NOTICE );

    CMessageActivityYqsStart tMessage;
    tMessage.set_activityid( nActivityID );
    tMessage.set_campid( iCampID );

    LOG_NOTICE( "YaoQianShu", 
        "[ %s:%d ] [ %s ] Send CMessageActivityYqsStart(%d) activityid(%d) campid(%d)",
        __LK_FILE__, __LINE__, __FUNCTION__, tpHead->messageid(), nActivityID, iCampID );

    tTotalMessage.set_msgpara( (long)&tMessage );
    CSceneLogic::GetSingletonPtr()->SendSceneMessage( &tTotalMessage );
    
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] Finish handle SendActivityStart!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return ;
}
void CActivityYqsPlayerRankInfo::InitializeForYqsPlayerRank( int nMaxPlayerRanks )
{
    mMaxPlayerRanks    = nMaxPlayerRanks;
    mLastUpdateTime    = 0;
    Clear( );
}

void CActivityYqsPlayerRankInfo::OnTime()
{
    time_t ltTimeNow = time(NULL);
    if ( mLastUpdateTime !=0 && 
        ltTimeNow - mLastUpdateTime > mTimeDelay )
    {
        bool lbRet= LoadPlayerRankInfoFromDB();
        if ( lbRet == false )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] LoadPlayerRankInfoFromDB failed", __LK_FILE__, __LINE__, __FUNCTION__ );
            return;
        }
        mLastUpdateTime = 0;
    }
}

void CActivityYqsPlayerRankInfo::Clear( void )
{
    for( int i=0; i<MAX_PLAER_RANK_NUM; i++ ) 
    {
        mPlayerRankInfos[i].Clear();

    }
    mFreeRankUnit.clear();
    mUsedRankUnit.clear();
    mPlayerRank.clear();
    mPlayerInfo.clear();

    for ( int i=0; i<MAX_PLAER_RANK_NUM; i++ )
    {
        mFreeRankUnit.push_back( i );
    }
}

void CActivityYqsPlayerRankInfo::OnSessionLoadPlayerRankInfoDB( CMessage *npMessage, int nParam1, int nParam2 )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start OnSessionLoadPlayerRankInfoDB ",
        __LK_FILE__, __LINE__, __FUNCTION__ );

    LK_ASSERT( npMessage!=NULL,  return );
    CMessageExecuteSqlResponse3* tpSqlRes = ( CMessageExecuteSqlResponse3* ) npMessage->msgpara();

    if ( tpSqlRes->resultcode() == 0 )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Load PlayerRankInfo failed", __LK_FILE__, __LINE__, __FUNCTION__ );
        return ;
    }

    // 清理现有用户贡献度排名榜
    Clear();

    int tRowNum = tpSqlRes->rowcount();
    int	tColumn	= tpSqlRes->colcount();	

    for(int tRowIndex = 0; tRowIndex<tRowNum ; ++tRowIndex)
    {
        std::string lstrAcitivityIndex      = tpSqlRes->fieldvalue( 0+tRowIndex*tColumn );
        std::string lstrPlayerId            = tpSqlRes->fieldvalue( 1+tRowIndex*tColumn );
        std::string lstrPlayerName          = tpSqlRes->fieldvalue( 2+tRowIndex*tColumn );
        std::string lstrPlayerMetier        = tpSqlRes->fieldvalue( 3+tRowIndex*tColumn );
        std::string lstrPlayerLevel         = tpSqlRes->fieldvalue( 4+tRowIndex*tColumn );
        std::string lstrContribute          = tpSqlRes->fieldvalue( 5+tRowIndex*tColumn );

        CPlayerRankInfoUnit loPlayerRankUnit;
        loPlayerRankUnit.miPlayerID         = atoi( lstrPlayerId.c_str() );
        loPlayerRankUnit.mstrPlayerName     = lstrPlayerName;
        loPlayerRankUnit.miPlayerMetie      = atoi( lstrPlayerMetier.c_str() );
        loPlayerRankUnit.miPlayerLevel      = atoi( lstrPlayerLevel.c_str() );
        loPlayerRankUnit.miContribute       = atoi( lstrContribute.c_str() );

        bool lbRet = UpdatePlayerRankInfo( loPlayerRankUnit );
        if ( lbRet == false )
        {
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] UpdatePlayerRankInfo failed", __LK_FILE__, __LINE__, __FUNCTION__ );
        }
    }

    mLastUpdateTime = time( NULL );

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish OnSessionLoadPlayerRankInfoDB ",
        __LK_FILE__, __LINE__, __FUNCTION__ );
}

bool CActivityYqsPlayerRankInfo::LoadPlayerRankInfoFromDB( void )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start LoadPlayerRankInfoFromDB ",
        __LK_FILE__, __LINE__, __FUNCTION__ );

    // 从数据库中查询用户贡献度排行信息
    bool lbRet = CSceneLogic::GetSingletonPtr()->ExecuteSql( 
        session_yaoqianshu_loadplayerrankinfo, 
        0, 0,0, SQL_NEED_CALLBACK, SELECT, 
        "call ProcYaoQianShuLoadPlayerRankInfo;" );

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish LoadPlayerRankInfoFromDB ",
        __LK_FILE__, __LINE__, __FUNCTION__);

    return lbRet;
}

bool CActivityYqsPlayerRankInfo::UpdatePlayerRankInfo( CPlayerRankInfoUnit& oPlayerInfo )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start UpdatePlayerRankInfo ",
        __LK_FILE__, __LINE__, __FUNCTION__);
    CPlayerRankInfoUnit* lpPlayerRankInfo = GetPlayRankInfo( oPlayerInfo.miPlayerID );
    if ( lpPlayerRankInfo != NULL )
    {
        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] update 1 player(%d) contribute(%d) ",
            __LK_FILE__, __LINE__, __FUNCTION__, lpPlayerRankInfo->miPlayerID, lpPlayerRankInfo->miContribute );

        (*lpPlayerRankInfo)   = oPlayerInfo;
        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] update 2 player(%d) contribute(%d) ",
            __LK_FILE__, __LINE__, __FUNCTION__, oPlayerInfo.miPlayerID, oPlayerInfo.miContribute );

        Sort();

        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish UpdatePlayerRankInfo ",
            __LK_FILE__, __LINE__, __FUNCTION__);
        return true;
    }

    int liNum   = GetPlayerRankInfoNum();
    if ( liNum>mMaxPlayerRanks )
    {
        LOG_ERROR( "YaoQianShu", 
            "[ %s:%d ][ %s ] the player rankinfo num is invalied now(%d) max(%d) ",
            __LK_FILE__, __LINE__, __FUNCTION__, liNum, mMaxPlayerRanks );
        return false;
    }
    CPlayerRankInfoUnit* lpMinContributePlayer =  GetMinContribute();
    if ( liNum==mMaxPlayerRanks && 
        lpMinContributePlayer!=NULL &&
        lpMinContributePlayer->miContribute<oPlayerInfo.miContribute )
    {
        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] update 3 player(%d) contribute(%d) ",
            __LK_FILE__, __LINE__, __FUNCTION__, lpMinContributePlayer->miPlayerID, lpMinContributePlayer->miContribute );

        ( *lpMinContributePlayer ) = oPlayerInfo;

        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] update 4 player(%d) contribute(%d) ",
            __LK_FILE__, __LINE__, __FUNCTION__, oPlayerInfo.miPlayerID, oPlayerInfo.miContribute );

        Sort();

        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish UpdatePlayerRankInfo ",
            __LK_FILE__, __LINE__, __FUNCTION__);
        return true;
    }

    bool lbRetForInsertPlayerRankInfo = InsertPlayerRankInfo( oPlayerInfo );
    if ( lbRetForInsertPlayerRankInfo == false )
    {
        return false;
    }

    Sort();

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish UpdatePlayerRankInfo ",
        __LK_FILE__, __LINE__, __FUNCTION__);
    return true;
}

int CActivityYqsPlayerRankInfo::GetPlayerRankInfoNum( void )
{
    return mUsedRankUnit.size();
}

CPlayerRankInfoUnit* CActivityYqsPlayerRankInfo::GetMinContribute( void )
{
    if( mPlayerRank.size() == 0 )
    {
        return NULL;
    }
    t_map_rank_itor loItor = mPlayerRank.begin();
    int liPos = loItor->second;
    return &mPlayerRankInfos[liPos];
}

int CActivityYqsPlayerRankInfo::GetFreeUnit( void )
{
    if ( mFreeRankUnit.size()==0 )
    {
        return -1;
    }

    int liPos = (*mFreeRankUnit.begin());
    mFreeRankUnit.pop_front();
    mUsedRankUnit.push_back( liPos );

    return liPos;
}

void CActivityYqsPlayerRankInfo::Release( int nPos )
{
    mPlayerRankInfos[nPos].Clear();
    mFreeRankUnit.push_front( nPos );
    mUsedRankUnit.pop_back();
}

void CActivityYqsPlayerRankInfo::Sort( void )
{
    mPlayerRank.clear();
    mPlayerInfo.clear();
    t_list_usedunit_itor loItor = mUsedRankUnit.begin();

    for( ; loItor!=mUsedRankUnit.end(); loItor++ ) 
    {
        int liPos = (*loItor);
        CPlayerRankInfoUnit& loPlayerRankUnit = mPlayerRankInfos[liPos];
        t_map_player_pairib lPairForPlayerInfo = mPlayerInfo.insert( t_map_player_value( loPlayerRankUnit.miPlayerID, liPos ) );
        if ( lPairForPlayerInfo.second == false )
        {
            Release( liPos );
            LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] insert player(%d) rank failed!", __LK_FILE__, __LINE__,__FUNCTION__, loPlayerRankUnit.miPlayerID );
            return;
        }
        mPlayerRank.insert( t_map_rank_value( loPlayerRankUnit.miContribute, liPos ) );
    }
}

CPlayerRankInfoUnit* CActivityYqsPlayerRankInfo::GetPlayRankInfo( int nPlayerId )
{
    t_map_player_itor loItor = mPlayerInfo.find( nPlayerId );
    if ( loItor == mPlayerInfo.end() )
    {
        return NULL;
    }
    int liPos = loItor->second;
    return &mPlayerRankInfos[liPos];
}

bool CActivityYqsPlayerRankInfo::InsertPlayerRankInfo( CPlayerRankInfoUnit& oPlayerInfo )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start InsertPlayerRankInfo player(%d) cou(%d)",
        __LK_FILE__, __LINE__, __FUNCTION__, oPlayerInfo.miPlayerID, oPlayerInfo.miContribute );

    int liPos = GetFreeUnit();
    if ( liPos == -1 )
    {
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] Can't get a freeunit", __LK_FILE__, __LINE__,__FUNCTION__ );
        return false;
    }
    mPlayerRankInfos[liPos]     = oPlayerInfo;

    t_map_player_pairib lPairForPlayerInfo = mPlayerInfo.insert( t_map_player_value( oPlayerInfo.miPlayerID, liPos ) );
    if ( lPairForPlayerInfo.second == false )
    {
        Release( liPos );
        LOG_ERROR( "YaoQianShu", "[ %s:%d ][ %s ] insert player(%d) rank failed!", __LK_FILE__, __LINE__,__FUNCTION__, oPlayerInfo.miPlayerID );
        return false;
    }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish InsertPlayerRankInfo player(%d) liPos(%d)",
        __LK_FILE__, __LINE__, __FUNCTION__, oPlayerInfo.miPlayerID, liPos );

    return true;
}

void CActivityYqsPlayerRankInfo::OnMessageGetPlayerRankInfoRequest( CEntityPlayer *npPlayer, CMessage *npMessage )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start Handle OnMessageGetPlayerRankInfoRequest!", __LK_FILE__, __LINE__, __FUNCTION__ );
    LK_ASSERT(npPlayer != NULL && npMessage != NULL, return);
    CMessageGetPlayerRankForYQSRequest* tpReq = ( CMessageGetPlayerRankForYQSRequest* ) npMessage->msgpara(); 
    if ( tpReq == NULL )
    {
        LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Can't get CMessageGetPlayerRankForYQSRequest!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return ;
    }

    int liPlayerId                = tpReq->playerid();
    if ( npPlayer->GetCharID() != (unsigned int)liPlayerId )
    {
        LOG_NOTICE( "YaoQianShu", 
            "[ %s:%d ][ %s ] the player id isn't match! old(%d) new(%d)", 
            __LK_FILE__, __LINE__, __FUNCTION__, npPlayer->GetCharID(), liPlayerId );
        return;
    }

    int liPlayerHandInDawNum    = npPlayer->moYaoQianShu.GetHandInDawNum();
    int liPlayerContribute      = npPlayer->moYaoQianShu.GetContribute();

    CMessage        tTotolMessage;
    CMessageHead*   tpHead = NULL;
    tpHead = tTotolMessage.mutable_msghead();
    if ( tpHead == NULL )
    {
        LOG_NOTICE( "YaoQianShu", "[ %s:%d ] [ %s ] can't create message head", __LK_FILE__, __LINE__, __FUNCTION__ );
        return ;
    }
    tpHead->set_messageid( ID_S2C_GETPLAYERRANKFORYQS_RESPONSE );
    CMessageGetPlayerRankForYQSResponse tMessage;
    tMessage.set_playerid( liPlayerId );
    tMessage.set_playerhandindawnum( liPlayerHandInDawNum );
    tMessage.set_playercontribute( liPlayerContribute );

    LOG_NOTICE( "YaoQianShu", 
        "[ %s:%d ] [ %s ] Send CMessageGetPlayerRankForYQSResponse(%d) to player(%d) handindawnum(%d) contribute(%d)", 
        __LK_FILE__, __LINE__, __FUNCTION__, tpHead->messageid(), liPlayerId, liPlayerHandInDawNum, liPlayerContribute  );

    t_map_rank_ritor loItor = mPlayerRank.rbegin();
    for ( int i=1; loItor!=mPlayerRank.rend(); loItor++, i++ )
    {
        int liPos = loItor->second;
        CPlayerRankInfoUnit& lPlayerRankInfo = mPlayerRankInfos[liPos];
        PlayerRankForYQS* lpPlayerRankInfo = tMessage.add_playerrankinfo();
        lpPlayerRankInfo->set_rank( i );
        lpPlayerRankInfo->set_userid( lPlayerRankInfo.miPlayerID );
        lpPlayerRankInfo->set_username( lPlayerRankInfo.mstrPlayerName );
        lpPlayerRankInfo->set_userlevel( lPlayerRankInfo.miPlayerLevel );
        lpPlayerRankInfo->set_usermetie( lPlayerRankInfo.miPlayerMetie );
        lpPlayerRankInfo->set_contribute( lPlayerRankInfo.miContribute );

        LOG_NOTICE( "YaoQianShu", 
            "[ %s:%d ] [ %s ] \t rank(%d) userid(%d) name(%s) level(%d) metie(%d) contribute(%d)", 
            __LK_FILE__, __LINE__, __FUNCTION__, 
            i, lPlayerRankInfo.miPlayerID, lPlayerRankInfo.mstrPlayerName.c_str(),
            lPlayerRankInfo.miPlayerLevel, lPlayerRankInfo.miPlayerMetie, lPlayerRankInfo.miContribute );

    }

    tTotolMessage.set_msgpara( (long) &tMessage );
    CSceneLogic::GetSingleton().Send2Player( npPlayer, &tTotolMessage );

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish Handle OnMessageGetPlayerRankInfoRequest!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return ;

}

void CActivityYqsPlayerRankInfo::GivePlayerRankPrize( void )
{
    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Start Handle GivePrize!", __LK_FILE__, __LINE__, __FUNCTION__ );

    t_map_rank_ritor loItor = mPlayerRank.rbegin();
    int liLastPlayerContribute  = 0;
    for ( int i=1; loItor!=mPlayerRank.rend(); loItor++,i++ )
    {
        CYqsCfgPlayerRankPrizeUnit* lpPlayerRankPrize = CActivityYqsCfg::GetSingleton().GetPlayerPrize( i );
        if ( lpPlayerRankPrize == NULL )
        {
            LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Can't get player prize for playerrank(%d)!", __LK_FILE__, __LINE__, __FUNCTION__,i );
            for ( int j=0; loItor!=mPlayerRank.rend(); loItor++, j++ )
            {
                lpPlayerRankPrize = CActivityYqsCfg::GetSingleton().GetPlayerPrize( i-1 );
                if ( lpPlayerRankPrize == NULL )
                {
                    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Can't get player prize for playerrank(%d)!", __LK_FILE__, __LINE__, __FUNCTION__,i-1 );
                    return;
                }
                int liPos = loItor->second;
                CPlayerRankInfoUnit& loPlayerRankInfo = mPlayerRankInfos[liPos];
                if ( liLastPlayerContribute != loPlayerRankInfo.miContribute )
                {
                    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish Handle GivePrize!", __LK_FILE__, __LINE__, __FUNCTION__ );
                    return;
                }

                int liMailId   = lpPlayerRankPrize->miMailId;
                int liPlayerId = loPlayerRankInfo.miPlayerID;
                std::string lstrPlayerName = loPlayerRankInfo.mstrPlayerName;
                int liPlayerCountry = CSceneLogic::GetSingleton().GetLineID();
                int liPrizeItem = lpPlayerRankPrize->miPrizeItem;
                int liPrizeItemNum = lpPlayerRankPrize->miPrizeItemNum;
                char lszBuf[128] = {0};
                sprintf( lszBuf, "%d", i-1 );
                char *pTemp = lszBuf;
                int liMailPos = 1;

                LOG_NOTICE( "YaoQianShu", 
                    "[ %s:%d ][ %s ] GivePrize! "
                    "liMailID(%d) liPlayerId(%d) lstrPlayerName(%s) liPlayerCountry(%d) "
                    "liPrizeItem(%d) liPrizeItemNum(%d) param(%s) parampos(%d) ", 
                    __LK_FILE__, __LINE__, __FUNCTION__, liMailId, liPlayerId, lstrPlayerName.c_str(), liPlayerCountry,
                    liPrizeItem, liPrizeItemNum, pTemp, liMailPos );

                int liRet = CMailModule::GetSingleton().SendSysMailBySysID( liMailId, 
                    const_cast<char*>(lstrPlayerName.c_str()), 
                    liPlayerId,
                    &liPrizeItem, 
                    &liPrizeItemNum, 
                    1, 
                    liPlayerCountry,
                    0,
                    &pTemp,
                    &liMailPos,
                    1 );
                if ( liRet != 0 )
                {
                    LOG_NOTICE( "YaoQianShu", 
                        "[ %s:%d ][ %s ] send mail failed! ret(%d) ", 
                        __LK_FILE__, __LINE__, __FUNCTION__, liRet );

                }
            }

            break;
        }
        int liPos = loItor->second;
        CPlayerRankInfoUnit& loPlayerRankInfo = mPlayerRankInfos[liPos];
        liLastPlayerContribute                = loPlayerRankInfo.miContribute;

        int liMailId   = lpPlayerRankPrize->miMailId;
        int liPlayerId = loPlayerRankInfo.miPlayerID;
        std::string lstrPlayerName = loPlayerRankInfo.mstrPlayerName;
        int liPlayerCountry = CSceneLogic::GetSingleton().GetLineID();
        int liPrizeItem = lpPlayerRankPrize->miPrizeItem;
        int liPrizeItemNum = lpPlayerRankPrize->miPrizeItemNum;
        char lszBuf[128] = {0};
        sprintf( lszBuf, "%d", i );
        char *pTemp = lszBuf;
        int liMailPos = 1;

        LOG_NOTICE( "YaoQianShu", 
            "[ %s:%d ][ %s ] GivePrize! "
            "liMailID(%d) liPlayerId(%d) lstrPlayerName(%s) liPlayerCountry(%d) "
            "liPrizeItem(%d) liPrizeItemNum(%d) param(%s) parampos(%d) ", 
            __LK_FILE__, __LINE__, __FUNCTION__, liMailId, liPlayerId, lstrPlayerName.c_str(), liPlayerCountry,
            liPrizeItem, liPrizeItemNum, pTemp, liMailPos );

        int liRet = CMailModule::GetSingleton().SendSysMailBySysID( liMailId, 
            const_cast<char*>(lstrPlayerName.c_str()), 
            liPlayerId,
            &liPrizeItem, 
            &liPrizeItemNum, 
            1, 
            liPlayerCountry,
            0,
            &pTemp,
            &liMailPos,
            1 );
        if ( liRet != 0 )
        {
            LOG_NOTICE( "YaoQianShu", 
                "[ %s:%d ][ %s ] send mail failed! ret(%d) ", 
                __LK_FILE__, __LINE__, __FUNCTION__, liRet );

        }
    }

    LOG_NOTICE( "YaoQianShu", "[ %s:%d ][ %s ] Finish Handle GivePrize!", __LK_FILE__, __LINE__, __FUNCTION__ );

    return;
}

CPlayerRankInfoUnit& CPlayerRankInfoUnit::operator=( CPlayerRankInfoUnit& theObj )
{
    miPlayerID      = theObj.miPlayerID;
    mstrPlayerName  = theObj.mstrPlayerName;
    miPlayerMetie   = theObj.miPlayerMetie;
    miPlayerLevel   = theObj.miPlayerLevel;
    miContribute    = theObj.miContribute;
    return *this;
}

void CPlayerRankInfoUnit::Clear( void )
{
    miPlayerID      = 0;
    mstrPlayerName  = "";
    miPlayerMetie   = 0;
    miPlayerLevel   = 0;
    miContribute    = 0;
}
