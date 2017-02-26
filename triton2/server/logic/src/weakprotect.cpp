//=========================================================================
// weakprotect.cpp
//=========================================================================
#include "weakprotect.h"
#include "stdafx.hpp"
#include "entity.h"
#include "worldmodule.h"
#include "logservertype.h"
#include "message_maker.h"
#include "scenelogic.h"
#include "chatmodule.h"
#include "taskmodule.h"
#include "tasktype.h"
#include "mapobjmanager.h"
#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif

//-------------------------------------------------------------------------
CWeakProtect::CWeakProtect()
{

}
CWeakProtect::~CWeakProtect()
{

}

void CWeakProtect::CWeakProtect::Init()
{
	memset(m_nCampWeakLevel, 0, sizeof(m_nCampWeakLevel));
	for (int i = 0; i < MAX_CAMP_NUM; i++)
	{
		m_PBWeakInfo[i].Clear();
	}
}

//-------------------------------------------------------------------------
// 排行查询
void CWeakProtect::OnMessageCampWeakListReq( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessage tMessage;
	CMessageHead* tpHead = tMessage.mutable_msghead();
	
	if ( tpHead == NULL )
	{
		LOG_ERROR("default", "Message null[ %s : %d ] [ %s ]", __LK_FILE__, __LINE__, __FUNCTION__);
		return;
	}

	CMessageCampWeakListResponse tMsgPara;
	for (int i = 0; i < MAX_CAMP_NUM; i ++)
	{
		PBCampWeakForceInfo *tpInfo = tMsgPara.add_camp_info();
		tpInfo->CopyFrom(m_PBWeakInfo[i]);
	}
	tMsgPara.set_errcode(SUCCESS);
	tpHead->set_messageid( ID_S2C_CAMPWEAKLIST_RESPONSE );
	tMessage.set_msgpara( (long)&tMsgPara );

	CSceneLogic::GetSingleton( ).Send2Player( pPlayer, &tMessage );
}


//-------------------------------------------------------------------------
// 奖励获取
void CWeakProtect::OnMessageCampGetAward( CEntityPlayer* pPlayer, CMessage* pMessage )
{
	CMessageCampGetAwardRequest* pMsgPara = (CMessageCampGetAwardRequest *)pMessage->msgpara();
	
	int nNpcEntityID = pMsgPara->npc_entity_id();
	
	int nAwardType = pMsgPara->award_type();
	int nCampID = pPlayer->GetNationality();
	int nWeekLevel = m_nCampWeakLevel[CAMP_INDEX(nCampID)];
	int nRetCode = SUCCESS;

	//PBCampAwardInfo* pAwardInfo = 
	if (EWL_Normal == nWeekLevel)
	{
		nRetCode = ERROR_CAMP_NotWeakCamp;
	}
	else if (EAT_Buff == nAwardType)
	{
		nRetCode = GetWeakCampAward(pPlayer, nNpcEntityID, nAwardType);
		if (-1 == nRetCode)
		{
			LOG_ERROR("default", "[%s] GetWeakCampAward err npc_ett_id:%d award_type:%d", __FUNCTION__, nNpcEntityID, nAwardType);
			return;
		}
	}
	else if (EAT_GiftPackage == nAwardType)
	{
		if (EWL_Level_2 != nWeekLevel && EWL_Level_3 != nWeekLevel)
		{
			nRetCode = ERROR_CAMP_NotWeakCampL2;
		}
		else
		{
			nRetCode = GetWeakCampAward(pPlayer, nNpcEntityID, nAwardType);
			if (-1 == nRetCode)
			{
				LOG_ERROR("default", "[%s] GetWeakCampAward err npc_ett_id:%d award_type:%d", __FUNCTION__, nNpcEntityID, nAwardType);
				return;
			}
		}
		
	}
	else
	{
		LOG_ERROR("default", "[%s] Err AwardType %d", __FUNCTION__, nAwardType);
		return;
	}

	CMessage tRetMsg;
	CMessageCampGetAwardResponse tMsgPara;
	tMsgPara.set_errcode(nRetCode);
	tRetMsg.mutable_msghead()->set_messageid( ID_S2C_CAMPGETAWARD_RESPONSE );
	tRetMsg.set_msgpara( (long)&tMsgPara );
	CSceneLogic::GetSingleton().Send2Player( pPlayer, &tRetMsg );
}

//-------------------------------------------------------------------------
// syn camp list from gate
void CWeakProtect::OnMessageGSCampWeakList( CMessage* pMessage )
{
	CMessageCampWeakListResponse* tpMsgPara = (CMessageCampWeakListResponse *)pMessage->msgpara();

	if (MAX_CAMP_NUM != tpMsgPara->camp_info_size())
	{
		LOG_DEBUG("default", "[%s] get error CampWeakListResponse", __FUNCTION__);
		return;
	}
	
	for ( int i = 0; i < tpMsgPara->camp_info_size(); i ++ )
	{
		m_PBWeakInfo[i].CopyFrom(tpMsgPara->camp_info(i));
		int nCampID = m_PBWeakInfo[i].camp_id();
		int nWeakLevel = m_PBWeakInfo[i].weak_level();
		int nLevelScore = m_PBWeakInfo[i].level_score();
		int nHonerScore = m_PBWeakInfo[i].honer_score();	
		LOG_INFO("default", "[%s] CID:%d\tWL:%d\tLS:%d\tHS:%d", __FUNCTION__, nCampID, nWeakLevel, nLevelScore, nHonerScore);
		m_nCampWeakLevel[CAMP_INDEX(nCampID)] = nWeakLevel;
	}
}

//-------------------------------------------------------------------------
// send honer score to gate
void CWeakProtect::Send2GatePlayerGetHonerScoreNotify( int nCampID, int nRoleID, int nScore )
{
	CMessage tMessage;
	CMessageCampAddHonerScore tMsgPara;
	tMsgPara.set_camp_id(nCampID);
	tMsgPara.set_role_id(nRoleID);
	tMsgPara.set_honer_value(nScore);
	tMessage.mutable_msghead()->set_messageid( ID_S2G_ADDHONER_NOTICE );
	tMessage.set_msgpara((long)&tMsgPara);
	CSceneLogic::GetSingleton().Send2Gate(&tMessage);
	LOG_DEBUG("default", "[%s] CampID:%d\tRoleID:%d\tHoner:%d", __FUNCTION__, nCampID, nRoleID, nScore);
}

//-------------------------------------------------------------------------
// request gate for list
void CWeakProtect::Send2GateCampWeakListRequest()
{
	CMessage tMessage;
	CMessageCampWeakListRequest tMsgPara;
	tMessage.mutable_msghead()->set_messageid( ID_S2G_CAMPWEAKLIST_REQUEST );
	tMessage.set_msgpara((long)&tMsgPara);
	CSceneLogic::GetSingleton().Send2Gate(&tMessage);
	LOG_INFO("default", "[%s]", __FUNCTION__);
}


//-------------------------------------------------------------------------
bool CWeakProtect::CouldObtainTask( CEntityPlayer* pPlayer )
{
	return IsWeakCamp(pPlayer->GetNationality());
}

//-------------------------------------------------------------------------
bool CWeakProtect::IsWeakCamp(int nCampID)
{
	if (!IS_WORLDID(nCampID))
	{
		LOG_ERROR("default", "[%s] get error camp id %d", __FUNCTION__, nCampID);
		return false;
	}
	int nWeekLevel = m_nCampWeakLevel[CAMP_INDEX(nCampID)];
	if (EWL_Normal == nWeekLevel)
	{
		return false;
	}
	else
	{
		return true;
	}
}


int  CWeakProtect::GetWeekLevel( int nCampID )
{
	return m_nCampWeakLevel[CAMP_INDEX(nCampID)];
}


//-------------------------------------------------------------------------
// 领取弱国任务
int CWeakProtect::GetWeakCampAward(CEntityPlayer* pPlayer, int nNpcEntityID, int nAwardType)
{
	CEntityNpc* tpNpc = dynamic_cast<CEntityNpc *>(CSceneObjManager::GetSingletonPtr()->GetObject(nNpcEntityID));
	if (NULL == tpNpc)
	{
		return -1;
	}

	CTemplateOgre *tplOgre = (CTemplateOgre*)CDataStatic::SearchTpl(tpNpc->GetNpcID());
	if (NULL == tplOgre)
	{
		return -1;
	}

	if(!pPlayer->IsWithinDistance(tpNpc, MAX_NPC_TALK_GIRD)) // 在合法距离之内才判断
	{
		LOG_ERROR("default", "[%s] player %s not near npc %d!", __FUNCTION__, pPlayer->GetCharNameShow(), nNpcEntityID);
		return -1;
	}

	CTemplateAwardInfo* tpInfo = (CTemplateAwardInfo*)CDataStatic::SearchTpl( tplOgre->mAwardID );
	if (NULL == tpInfo)
	{
		return -1;
	}
	if ( (unsigned int)tpInfo->mNpcID != tpNpc->GetNpcID() )
	{
		return -1;
	}

	CPropertyPlayer *tpProperty = (CPropertyPlayer*)pPlayer->GetProperty();

	if (EAT_GiftPackage == nAwardType)
	{
		int nCampID = pPlayer->GetNationality();
		int nWeekLevel = m_nCampWeakLevel[CAMP_INDEX(nCampID)];
		int nAwardIdx;
		if (EWL_Level_2 == nWeekLevel)
		{
			// 弱势二级领取奖励0
			nAwardIdx = 0;
		}
		else if (EWL_Level_3 == nWeekLevel)
		{
			// 弱势三级领取奖励1
			nAwardIdx = 1;
		}
		else
		{
			return -1;
		}
		
		// 当前要领取物品数量
		int tnItemNum = 0;
		
		if ( tpInfo->mItemID[ nAwardIdx ] != 0 && tpInfo->mItemNum[ nAwardIdx ] != 0 )
		{	
			tnItemNum++;
		}
		else
		{
			LOG_ERROR("default", "[%s] Error award config,  nAwardIdx %d", __FUNCTION__, nAwardIdx);
			return -1;
		}

		// 判断包裹是否满了
		CItemBoxToolkit tItemBoxTooKit( tpProperty->GetBaggage() );
		if ( tItemBoxTooKit.GetLeftPileNum( -1 ) < tnItemNum )
		{
			return ERROR_WORLD_AWARD_FULLBAGGAGE;		
		}

		// 每周领取次数判断

		if (tpInfo->mItemID[nAwardIdx] != 0 && tpInfo->mItemNum[nAwardIdx] != 0 &&
			tpProperty->GetAwardTimes( tpInfo->mItemID[nAwardIdx], EALT_Weekly ) >= tpInfo->mTimes)
		{
			return ERROR_CAMP_AlreadyGetAward_Week;
			//return ERROR_WORLD_AWARD_FULLTIMES;
		}

		// 向玩家添加物品奖励

		if ( tpInfo->mItemID[nAwardIdx] != 0 && tpInfo->mItemNum[nAwardIdx] != 0 )
		{
			CItemObject* pInsItemPile = CPropertyModule::GetSingleton().PlayerInsertItem( pPlayer, tpInfo->mItemID[ nAwardIdx ], tpInfo->mItemNum[ nAwardIdx ] ); 
			if ( pInsItemPile != NULL)
			{
				tpProperty->AddAwardTimes( tpInfo->mItemID[nAwardIdx], EALT_Weekly );
				// Log
				LogEventGetItemByAward( pPlayer, pInsItemPile->GetGUID(), tpInfo->mItemID[ nAwardIdx ], tpInfo->mItemNum[ nAwardIdx ], tpInfo->mTempID );
			}
		}
	}
	else if (EAT_Buff == nAwardType)
	{
		if ( tpInfo->mBuffID != 0 )
		{
			CTemplateBuff *tplBuff = (CTemplateBuff*)CDataStatic::SearchTpl( tpInfo->mBuffID );
			if (NULL == tplBuff)
			{
				return -1;
			}
			// 当天次数判断
			if (tpProperty->GetAwardTimes( tpInfo->mBuffID ) >= tpInfo->mTimes)
			{
				return ERROR_CAMP_AlreadyGetAward_Day;
				//return ERROR_WORLD_AWARD_FULLTIMES;
			}
			pPlayer->EntityInsertBuff(pPlayer, tplBuff);
			tpProperty->AddAwardTimes(tpInfo->mBuffID); 
			int pTime = tplBuff->mLastTime/1000;
			if (pTime >= 60)
			{
				LogEventGetBuffBySystem(pPlayer,tpInfo->mBuffID,pTime);
			}

			LogEventGetItemByAward( pPlayer, 0, tpInfo->mBuffID, 1, tpInfo->mTempID );

		}
		else
		{
			return -1;
		}
	}
	return SUCCESS;
}

bool  CWeakProtect:: IsTopTotalscoreCampID(int  maxCampID)
{
	int nmaxscore =  0;
	 int nmaxCampID  = 0;
	for (int i = 0 ;i < MAX_CAMP_NUM; i++)
	{
		int nCampID = m_PBWeakInfo[i].camp_id();
		int ntotalscore = m_PBWeakInfo[i].total_score();
		if (ntotalscore > nmaxscore)
		{
			nmaxscore = ntotalscore;
			nmaxCampID = nCampID;
		}
		
	}
	return  maxCampID == nmaxCampID ? true:false;
	
}
