

#include "log.h"
#include "propertymodule.h"
#include "playerinfo_yqs.h"
#include "ActivityYqs.h"

bool CPlayerInfoForYaoQianShu::Handin( int nCharId, int nContribute )
{
    LOG_DEBUG( "YaoQianShu","[ %s:%d ][ %s ] Start handle Handin!", __LK_FILE__, __LINE__, __FUNCTION__ );
    int liActiviyId = CActivityYqs::GetSingleton().GetActivtyId();

    if ( liActiviyId == 0 )
    {
        LOG_ERROR( "YaoQianShu", 
            "[ %s:%d ][ %s ] the Yqs Activity isn't staart(%d)", 
            __LK_FILE__, __LINE__, __FUNCTION__, liActiviyId );
        return false;
    }

    miActivityID        = liActiviyId;
    miContribute        += nContribute;
    miHandinDawNum++;

    LOG_DEBUG( "YaoQianShu","[ %s:%d ][ %s ] UpdatePlayerInfoToDB nCharId(%d) miActivityID(%d) miContribute(%d) miHandinDawNum(%d)!", 
        __LK_FILE__, __LINE__, __FUNCTION__, nCharId, miActivityID, miContribute, miHandinDawNum );

    UpdatePlayerInfoToDB( nCharId, miActivityID );

    LOG_DEBUG( "YaoQianShu","[ %s:%d ][ %s ] Finish handle Handin!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return true;
}

int CPlayerInfoForYaoQianShu::PickStoreBox( int nCharId, int nStoreBoxId )
{
    LOG_DEBUG( "YaoQianShu","[ %s:%d ][ %s ] Start handle PickStoreBox!", __LK_FILE__, __LINE__, __FUNCTION__ );

    int liActiviyId = CActivityYqs::GetSingleton().GetActivtyId();
    int liSettlePhase = CActivityYqs::GetSingleton().GetSettlePhase();

    if ( miActivityID!=0 && liActiviyId != miActivityID )
    {
        LOG_DEBUG( "YaoQianShu","[ %s:%d ][ %s ] the activity changed! old(%d) new(%d)",
            __LK_FILE__, __LINE__, __FUNCTION__, miActivityID, liActiviyId );
        miContribute    = 0;
        miHandinDawNum  = 0;
        miActivityID    = liActiviyId;
        UpdatePlayerInfoToDB( nCharId, liActiviyId );
    }else if ( miSettlePhase!= -1 && miSettlePhase != liSettlePhase )
    {
        LOG_NOTICE( "YaoQianShu","[ %s:%d ][ %s ] the SettlePhase changed! old(%d) new(%d)",
            __LK_FILE__, __LINE__, __FUNCTION__, miSettlePhase, liSettlePhase );
        mPickStoreBoxIndex.clear();
        miSettlePhase = liSettlePhase;
    }

    miActivityID    = liActiviyId;
    miSettlePhase   = liSettlePhase;

    CPickStoreBox loStoreBox;
    loStoreBox.miActivityIndex  = liActiviyId;
    loStoreBox.miSettlePhase    = liSettlePhase;
    loStoreBox.miStoreBoxId     = nStoreBoxId;
    t_map_pairib lPairib = mPickStoreBoxIndex.insert( t_map_value( loStoreBox.miStoreBoxId, loStoreBox ) );
    if ( lPairib.second == false )
    {
        LOG_WARN( "YaoQianShu","[ %s:%d ][ %s ] Finish handle PickStoreBox!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return -1;
    }

    InsertStoreBoxToDB( nCharId, loStoreBox );

    LOG_DEBUG( "YaoQianShu","[ %s:%d ][ %s ] Finish handle PickStoreBox!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return 1;
}

void CPlayerInfoForYaoQianShu::InsertStoreBoxToDB( int nCharID, CPickStoreBox& oStoreBoxInfo )
{
    LOG_DEBUG( "YaoQianShu","[ %s:%d ][ %s ] Start handle InsertStoreBoxToDB!", __LK_FILE__, __LINE__, __FUNCTION__ );

    // 将拾取的宝箱插入到数据库中
    bool lbRet = CSceneLogic::GetSingletonPtr()->ExecuteSql( 
        session_yaoqianshu_updateplayerinfo, 
        0, 0, nCharID, SQL_NONE, INSERT, 
        "insert UMS_ACTIVITY_YQS_STOREBOXINFO( PlayerId, ActivityIndex, SettlePhase, StoreBoxId ) values( %d, %d, %d, %d );", 
        nCharID, oStoreBoxInfo.miActivityIndex, oStoreBoxInfo.miSettlePhase, oStoreBoxInfo.miStoreBoxId );
    LOG_DEBUG( "YaoQianShu",
        "[ %s:%d ][ %s ] Sql(insert UMS_ACTIVITY_YQS_STOREBOXINFO( PlayerId, ActivityIndex, SettlePhase, StoreBoxId ) "
        "values( %d, %d, %d, %d );)!", 
        __LK_FILE__, __LINE__, __FUNCTION__,
        nCharID, oStoreBoxInfo.miActivityIndex, oStoreBoxInfo.miSettlePhase, oStoreBoxInfo.miStoreBoxId );
    if ( lbRet == false )
    {
        LOG_ERROR( "YaoQianShu","[ %s:%d ][ %s ] insert UMS_ACTIVITY_YQS_STOREBOXINFO failed!", __LK_FILE__, __LINE__, __FUNCTION__ );
    }

    LOG_DEBUG( "YaoQianShu","[ %s:%d ][ %s ] Finish handle InsertStoreBoxToDB!", __LK_FILE__, __LINE__, __FUNCTION__ );

    return;
}

void CPlayerInfoForYaoQianShu::UpdatePlayerInfoToDB( int nCharID, int nActivityId )
{
    LOG_DEBUG( "YaoQianShu","[ %s:%d ][ %s ] Start handle UpdatePlayerInfoToDB!", __LK_FILE__, __LINE__, __FUNCTION__ );
    // 从数据库中查询用户
    bool lbRet= CSceneLogic::GetSingletonPtr()->ExecuteSql( 
        session_yaoqianshu_updateplayerinfo, 
        0, 0, nCharID, SQL_NEED_CALLBACK, SELECT, 
        "call ProcYaoQianShuUpdateUserInfo( %d, %d, %d, %d );", 
        nCharID, nActivityId, miContribute, miHandinDawNum );

    LOG_DEBUG( "YaoQianShu","[ %s:%d ][ %s ] call ProcYaoQianShuUpdateUserInfo( %d, %d, %d, %d );!", 
        __LK_FILE__, __LINE__, __FUNCTION__,nCharID, nActivityId, miContribute, miHandinDawNum  );

    if ( lbRet == false )
    {
        LOG_ERROR( "YaoQianShu","[ %s:%d ][ %s ] call ProcYaoQianShuUpdateUserInfo failed!", __LK_FILE__, __LINE__, __FUNCTION__ );

    }
    LOG_DEBUG( "YaoQianShu","[ %s:%d ][ %s ] Finish handle UpdatePlayerInfoToDB!", __LK_FILE__, __LINE__, __FUNCTION__ );
    return;
}

void CPlayerInfoForYaoQianShu::OnMessageLoadPlayerInfo( CMessage *pMessage, int nParam1, int nParam2 )
{
    LOG_DEBUG( "YaoQianShu","[ %s:%d ][ %s ] Start handle OnMessageLoadPlayerInfo!", __LK_FILE__, __LINE__, __FUNCTION__ );

    LK_ASSERT( pMessage != NULL, return );	
    CMessageExecuteSqlResponse3 *tpResponseMsg = ( CMessageExecuteSqlResponse3 * )pMessage->msgpara();
    if ( tpResponseMsg == NULL )
    {
        LOG_ERROR( "YaoQianShu","[ %s:%d ][ %s ]Can't get CMessageExecuteSqlResponse3!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return;
    }

    if ( tpResponseMsg->resultcode() == 0 )
    {
        LOG_WARN( "YaoQianShu",
            "[ %s:%d ][ %s ] Can't load OnMessageLoadPlayerInfo from database!(%d)", 
            __LK_FILE__, __LINE__, __FUNCTION__, tpResponseMsg->resultcode() );
        return;
    }

    int tRowNum = tpResponseMsg->rowcount();
    int	tColumn	= tpResponseMsg->colcount();	

    for(int tRowIndex = 0; tRowIndex<tRowNum ; ++tRowIndex)
    {
        std::string lstrPlayerId            = tpResponseMsg->fieldvalue( 0+tRowIndex*tColumn );
        std::string lstrAcitivityIndex      = tpResponseMsg->fieldvalue( 1+tRowIndex*tColumn );
        std::string lstrContribute          = tpResponseMsg->fieldvalue( 2+tRowIndex*tColumn );
        std::string lstrHandInNum           = tpResponseMsg->fieldvalue( 3+tRowIndex*tColumn );

        miContribute        = atoi( lstrContribute.c_str() );
        miHandinDawNum      = atoi( lstrHandInNum.c_str() );

        miActivityID        = CActivityYqs::GetSingleton().GetActivtyId();
        miSettlePhase       = CActivityYqs::GetSingleton().GetSettlePhase();

        LOG_DEBUG( "YaoQianShu","[ %s:%d ][ %s ] PlayerInfo playerid(%s), Acitivyid(%s) miContribute(%d) miHandinDawNum(%d)!",
            __LK_FILE__, __LINE__, __FUNCTION__, lstrPlayerId.c_str(), lstrAcitivityIndex.c_str(), miContribute, miHandinDawNum );
    }

    LOG_DEBUG( "YaoQianShu","[ %s:%d ][ %s ] finish handle OnMessageLoadPlayerInfo!", __LK_FILE__, __LINE__, __FUNCTION__ );
}

void CPlayerInfoForYaoQianShu::OnMessageLoadPlayerPickStoreBoxInfo( CMessage *pMessage, int nParam1, int nParam2 )
{
    LOG_DEBUG( "YaoQianShu","[ %s:%d ][ %s ] Start handle OnMessageLoadPlayerPickStoreBoxInfo!", __LK_FILE__, __LINE__, __FUNCTION__ );
    LK_ASSERT( pMessage != NULL, return; );	
    CMessageExecuteSqlResponse3 *tpResponseMsg = ( CMessageExecuteSqlResponse3 * )pMessage->msgpara();
    if ( tpResponseMsg == NULL )
    {
        LOG_ERROR( "YaoQianShu","[ %s:%d ][ %s ]Can't get CMessageExecuteSqlResponse3!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return;
    }

    if ( tpResponseMsg->resultcode() != 1 )
    {
        LOG_WARN( "YaoQianShu","[ %s:%d ][ %s ] Can'tload LoadPlayerPickStoreBoxInfo from database!", __LK_FILE__, __LINE__, __FUNCTION__ );
        return;
    }

    int tRowNum = tpResponseMsg->rowcount();
    int	tColumn	= tpResponseMsg->colcount();	

    for(int tRowIndex = 0; tRowIndex<tRowNum ; ++tRowIndex)
    {
        std::string lstrPlayerId                = tpResponseMsg->fieldvalue( 0+tRowIndex*tColumn );
        std::string lstrActivityIndex           = tpResponseMsg->fieldvalue( 1+tRowIndex*tColumn );
        std::string lstrSettlePhase             = tpResponseMsg->fieldvalue( 2+tRowIndex*tColumn );
        std::string lstrStoreBoxId              = tpResponseMsg->fieldvalue( 3+tRowIndex*tColumn );

        CPickStoreBox   loPickStoreBox;
        int liPlayerID                  = atoi( lstrPlayerId.c_str() );
        loPickStoreBox.miActivityIndex  = atoi( lstrActivityIndex.c_str() );
        loPickStoreBox.miSettlePhase    = atoi( lstrSettlePhase.c_str() );
        loPickStoreBox.miStoreBoxId     = atoi( lstrStoreBoxId.c_str() );

        t_map_pairib lPairib = mPickStoreBoxIndex.insert( t_map_value( loPickStoreBox.miStoreBoxId, loPickStoreBox ) );
        if ( lPairib.second == false )
        {
            LOG_ERROR( "YaoQianShu",
                "[ %s:%d ][ %s ] insert storebox(%d) to map failed! for player(%d)",
                __LK_FILE__, __LINE__, __FUNCTION__, 
                loPickStoreBox.miStoreBoxId, nParam1 );
            continue;
        }

        miActivityID        = CActivityYqs::GetSingleton().GetActivtyId();
        miSettlePhase       = CActivityYqs::GetSingleton().GetSettlePhase();

        LOG_DEBUG( "YaoQianShu",
            "[ %s:%d ][ %s ] StoreBox liPlayerID(%d) miActivityIndex(%d) miSettlePhase(%d) miStoreBoxId(%d)",
            __LK_FILE__, __LINE__, __FUNCTION__, 
            liPlayerID, loPickStoreBox.miActivityIndex, loPickStoreBox.miSettlePhase, loPickStoreBox.miStoreBoxId );
    }

    LOG_DEBUG( "YaoQianShu","[ %s:%d ][ %s ] Finish handle OnMessageLoadPlayerPickStoreBoxInfo!", __LK_FILE__, __LINE__, __FUNCTION__ );
}

CPlayerInfoForYaoQianShu::CPlayerInfoForYaoQianShu()
{
    miSettlePhase = -1;
    miActivityID = 0;
    miHandinDawNum = 0;
    miContribute = 0;
}

CPickStoreBox& CPickStoreBox::operator=( CPickStoreBox& theObj )
{
    miActivityIndex     = theObj.miActivityIndex;
    miSettlePhase       = theObj.miSettlePhase;
    miStoreBoxId        = theObj.miStoreBoxId;
    return *this;
}

CPickStoreBox::CPickStoreBox()
{
    miActivityIndex     = 0;
    miSettlePhase       = 0;
    miStoreBoxId        = 0;
}
