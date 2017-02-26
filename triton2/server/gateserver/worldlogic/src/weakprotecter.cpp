//-------------------------------------------------------------------------
// weakprotecter.cpp
//-------------------------------------------------------------------------
#include "weakprotecter.h"
#include "world.h"
#include "gatelogic.h"

#define CALC_WAIT_ZONE	300 //300 seconds

CWeakProtecter::CWeakProtecter()
{
	m_State = EWP_Normal;
}

CWeakProtecter::~CWeakProtecter()
{

}

void CWeakProtecter::OnMessageCampWeakListReq(int nServerID, CMessage* pMessage)
{
	SendCampWeakList(nServerID);
}

void CWeakProtecter::OnMessageCampAddHonerScore(int nServerID, CMessage* pMessage)
{
	CMessageCampAddHonerScore* tpMsgPara = (CMessageCampAddHonerScore *)pMessage->msgpara();
	if (NULL == tpMsgPara)
	{
		return;
	}
	AddHonerScore(tpMsgPara->camp_id(), tpMsgPara->honer_value(), tpMsgPara->role_id());
}

void CWeakProtecter::SendCampWeakList(int nServerID)
{
	CMessage tMessage;
	CMessageHead* tpHead = tMessage.mutable_msghead();

	if ( tpHead == NULL )
	{
		LOG_ERROR("default", "Message null[ %s : %d ] [ %s ]", __LK_FILE__, __LINE__, __FUNCTION__);
		return;
	}

	CMessageCampWeakListResponse tMsgPara;
	for (int i = WORLD_FIRST; i < WORLD_NUM; i ++)
	{
		CWorld* tpWorld = CWorldManager::GetSingleton().GetWorldByID(i);
		if (NULL == tpWorld)
		{
			LOG_NOTICE( "world", "[%s] get world err, worldid %d", __FUNCTION__, i);
			continue;
		}
		PBCampWeakForceInfo *tpCampInfo = tMsgPara.add_camp_info();
		tpCampInfo->set_camp_id(i);
		tpCampInfo->set_level_score(UI_LevelScore(tpWorld->m_nCampLevelScore));	//10/100
		tpCampInfo->set_honer_score(UI_HonerScore(tpWorld->m_nCampHonerScore)); //1/1000
		tpCampInfo->set_total_score(tpWorld->m_nCampTotalScore);
		tpCampInfo->set_weak_level(tpWorld->m_nWeekLevel);
	}
	tMsgPara.set_errcode(SUCCESS);
	tpHead->set_messageid( ID_G2S_CAMPWEAKLIST_RESPONSE );
	tMessage.set_msgpara( (long)&tMsgPara );

	if (nServerID == -1)
	{
		CSceneServer* pCSceneServer = NULL;
		CGateServer::GetSingleton( ).Send2Scene( pCSceneServer, &tMessage );
	}
	else
	{
		CGateServer::GetSingleton( ).Send2Scene( nServerID, &tMessage );
	}
}


int CWeakProtecter::AddHonerScore(int nCampID, int nScore, int nRoleID)
{
	if (!IS_WORLDID(nCampID))
	{
		return -1;
	}
	CWorld* tpWorld = CWorldManager::GetSingleton().GetWorldByID(nCampID);
	if (NULL == tpWorld)
	{
		LOG_NOTICE( "world", "[%s] get world err, worldid %d", __FUNCTION__, nCampID);
		return -1;
	}
	LOG_DEBUG( "world", "[%s] worldid %d, score %d, roleid %d", __FUNCTION__, nCampID, nScore, nRoleID);
	tpWorld->m_nHonerInc += nScore;
	return 0;
}




int CWeakProtecter::GetWeekLevelByWordID(int nWorldID)
{
	CWorld* tpWorld = CWorldManager::GetSingleton().GetWorldByID(nWorldID);
	if (NULL == tpWorld)
	{
		LOG_NOTICE( "world", "[%s] get world err, worldid %d", __FUNCTION__, nWorldID);
		return EWL_Normal;
	}
	return tpWorld->m_nWeekLevel;
}


// 根据当前值重新计算等级
void CWeakProtecter::RefreshWeakInfo()
{
	int nLevelScore[WORLD_NUM];
	int nTotalScore[WORLD_NUM];
	int nSumTotal = 0;
	for (int wid = WORLD_FIRST; wid < WORLD_NUM; wid ++)
	{
		CWorld* pWorld = CWorldManager::GetSingleton().GetWorldByID(wid);
		nLevelScore[wid] = pWorld->m_nCampLevelScore;
		nTotalScore[wid] = GET_TotalScore(pWorld->m_nCampLevelScore, pWorld->m_nCampHonerScore);
		nSumTotal += nTotalScore[wid];
	}
	
	JudgeWeakLevel(nSumTotal, nLevelScore, nTotalScore, false);
}



// 计算并更新弱势力阵营
void CWeakProtecter::CalculateCampWeakRank()
{
	LOG_INFO( "world", "[%s]", __FUNCTION__);
	//等级计算逻辑在这里
	StartGetActiveRoleInfo();
}


void CWeakProtecter::StartGetActiveRoleInfo()
{	
	char* pSqlStr = NULL;
	int SessinType;

	pSqlStr = (char*)"call ProcGetActiveRoleLevelInfo()";
	SessinType = em_dnsession_proc_active_role_info;

	LOG_INFO( "world", "[%s]", __FUNCTION__);
	CGateLogic::GetSingleton().ExecuteSql( SessinType, 0, 0, 5, SQL_NEED_CALLBACK, SELECT, pSqlStr );	
}

void CWeakProtecter::OnSessionGetActiveRoleInfo(CMessage* pMessage)
{
	LK_ASSERT( pMessage != NULL, return );	
	CMessageExecuteSqlResponse3* tpMsg = (CMessageExecuteSqlResponse3 *) pMessage->msgpara();
	LK_ASSERT( tpMsg != NULL, return );
	int tcolnum = tpMsg->colcount();
	int trownum = tpMsg->rowcount();	

	int tnWorldCount = 0;

	int nLevelScore[WORLD_NUM];
	int nTotalScore[WORLD_NUM];
	int nRoleNum[WORLD_NUM];
	int nSumTotal = 0;
	CWorld* tpWorld = NULL;
	CWorldManager* pWorldManager = CWorldManager::GetSingletonPtr();
	
	if ( tpMsg->resultcode() == 1 )
	{
		for ( int i = 0; i < trownum; ++i )
		{	
			// 阵营ID
			int tnWorldID = atoi( tpMsg->fieldvalue( i*tcolnum + 0 ).c_str() );	
			
			if (!IS_WORLDID(tnWorldID))
			{
				LOG_ERROR( "world","[%s] IS_WORLDID! ",__FUNCTION__);
				break;
			}
			
			// 级别积分
			nLevelScore[tnWorldID] = atoi( tpMsg->fieldvalue( i*tcolnum + 1 ).c_str() );
			nRoleNum[tnWorldID] = atoi( tpMsg->fieldvalue( i*tcolnum + 2 ).c_str() );
			LOG_INFO( "world", "[%s] WorldID: %d LevelScore: %d ActiveRoleNum: %d ", 
				__FUNCTION__, 
				tnWorldID, 
				nLevelScore[tnWorldID], 
				nRoleNum[tnWorldID]);
			
			// 总分 = 级别积分 + 当前荣誉积分
			tpWorld = pWorldManager->GetWorldByID(tnWorldID);
			if (NULL == tpWorld)
			{
				LOG_ERROR( "world", "[%s] get world err, worldid %d", __FUNCTION__, tnWorldID);
				continue;
			}
			nTotalScore[tnWorldID] = GET_TotalScore(nLevelScore[tnWorldID], tpWorld->m_nHonerInc);

			// 大总分用于取平均值
			nSumTotal = nSumTotal + nTotalScore[tnWorldID];
			tnWorldCount ++;
		}
		
		if (4 == tnWorldCount)
		{
			JudgeWeakLevel(nSumTotal, nLevelScore, nTotalScore, true);
		}
		else
		{
			LOG_ERROR( "world", "[%s] get num of world err %d", __FUNCTION__, tnWorldCount);
		}
	}	
	else
	{
		LOG_ERROR( "world","[%s,%d] GetActiveRoleInfo! ",__FUNCTION__, __LINE__ );
	}			   
}

void CWeakProtecter::JudgeWeakLevel(int nSumTotal, int* nLevelScore, int *nTotalScore, bool IsChange)
{
	// 平均分
	if (0 == nSumTotal)
	{
		LOG_ERROR( "world","[%s,%d] nSumTotal=0! ",__FUNCTION__, __LINE__ );
		return;
	}
	double fAverScore = (double)nSumTotal / 4;
	double fScoreL1 = fAverScore * 85 / 100;
	double fScoreL2 = fAverScore * 7 / 10;
	CWorld* tpWorld = NULL;
	CWorldManager* pWorldManager = CWorldManager::GetSingletonPtr();
	
	for (int wid = WORLD_FIRST; wid < WORLD_NUM; wid ++)
	{
		tpWorld = pWorldManager->GetWorldByID(wid);
		double fWorldScore = nTotalScore[wid];
		// 等级判定
		if ( fAverScore <= fWorldScore || 
			pWorldManager->IsWinner(wid) )
		{
			// 普通
			tpWorld->m_nWeekLevel = EWL_Normal;
		}else if ( fScoreL1 <= fWorldScore && fWorldScore < fAverScore )
		{
			// 弱一级
			tpWorld->m_nWeekLevel = EWL_Level_1;
		}
		else if ( fScoreL2 <= fWorldScore && fWorldScore < fScoreL1 )
		{
			// 弱二级
			tpWorld->m_nWeekLevel = EWL_Level_2;
		}
		else if ( fWorldScore < fScoreL2 )
		{
			// 弱三级
			tpWorld->m_nWeekLevel = EWL_Level_3;
		}

		tpWorld->m_nCampTotalScore = nTotalScore[wid];

		// 更新数据
		if (IsChange)
		{
			tpWorld->m_nCampLevelScore = nLevelScore[wid];
			tpWorld->m_nCampHonerScore = tpWorld->m_nHonerInc;	
			tpWorld->m_nHonerInc = 0;
		}

		LOG_INFO( "world", "[%s]W %d\tLs %d\tHs %d\tTs %d\tWKL %d\t\tinc %d", 
			__FUNCTION__, 
			wid, 
			tpWorld->m_nCampLevelScore, 
			tpWorld->m_nCampHonerScore, 
			tpWorld->m_nCampTotalScore, 
			tpWorld->m_nWeekLevel,
			tpWorld->m_nHonerInc);
	}
	if (IsChange)
	{
		// 发送到所有服务器
		SendCampWeakList(-1);
	}	
}


void CWeakProtecter::OnTimerCheckState()
{
	if (EWP_Normal == m_State)
	{
		time_t tNow = time( NULL );
		struct tm* tLocal = localtime( &tNow );

		// 匹配时间
		if (1 == tLocal->tm_wday &&
			0 == tLocal->tm_hour &&
			0 == tLocal->tm_min) 
		{
			// 时间到了
			CalculateCampWeakRank();
			m_State = tNow + CALC_WAIT_ZONE;
		}
	}
	else if (time(NULL) >= m_State)
	{
		m_State = EWP_Normal;
	}
}
