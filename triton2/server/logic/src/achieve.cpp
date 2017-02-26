/*
 *	Achieve Data CPP
 */

// Created By GaoHong [11/18/2009 Macro]
#include "property.h"

#include "achieve.h"
#include "dbinterface.h"
#include "scenelogic.h"
#include "judgemodule.h"
#include "repetionmodule.h"

// 更新成就数据
int CAchieveDataUnit::UpdateAchieveData(int nID, ST_ACHIEVE_DATA *npData, CAchieveCfgItem *npCfgItem)
{
	if ( npData == NULL || npCfgItem == NULL )
	{
		return -1;
	}

	CAchieveData* tpAchieveData = GetAchieveData( nID );
	if ( tpAchieveData == NULL )
	{
		LOG_ERROR( "achieve", "Achieve Data error,ID(%d),[%s:%d]", nID, __LK_FILE__, __LINE__ );
		return -1;
	}

	if ( tpAchieveData->GetStatus() )
	{
		// 已经完成，不需要记录
		return -1;
	}

	if ( npCfgItem->CheckParamValid( npData->mKey ) == false )
	{
		return -1;
	}

	tpAchieveData->SetParam( npData->mKey, npData->mValue, npCfgItem->GetParamType() );

	bool bFinishRet = tpAchieveData->CheckAchieveFinish( npCfgItem, GetOwnerCharID() );
	if ( bFinishRet )
	{
		tpAchieveData->SetStatus( true );
		tpAchieveData->SetTime( time( NULL ) );
		IncTotalPoint( npCfgItem->GetPoint() );
		return ACHIEVE_FINISH;
	}

	return 0;
}

// 检查是否有成就数据对象
bool CAchieveDataUnit::AchieveDataExist(int nID)
{
	ACHIEVE_DATA_UNIT::iterator it = mAchieveUnit.find( nID );
	if ( it != mAchieveUnit.end() )
	{
		return true;
	}	
	return false;
}

// 同步数据(From DB) [11/20/2009 Macro]
int CAchieveDataUnit::InitFromSession( CMessage* npMessage )
{
	if ( npMessage == NULL )
	{
		return -1;
	}

	CMessageExecuteSqlResponse3* tpSqlRes = ( CMessageExecuteSqlResponse3* ) npMessage->msgpara();

	if ( tpSqlRes->resultcode() == 0 )
	{
		LOG_ERROR( "achieve", "Load db failed.[%s:%d]", __LK_FILE__, __LINE__ );
		return -1;
	}

	if( tpSqlRes->rowcount() != 1 )
	{
		return -1;
	}

	PBDBAchieveBuffer tAchieveBuffer;
	if ( tAchieveBuffer.ParseFromArray( tpSqlRes->fieldvalue( 1 ).c_str(), (int)tpSqlRes->fieldvaluelen( 1 ) ) )
	{
		SetFromDBPB( &tAchieveBuffer );
	}
	
	return SUCCESS;
}

// 存盘 [11/20/2009 Macro]
int CAchieveDataUnit::SaveSessionData( CEntityPlayer* npPlayer )
{
	if ( npPlayer == NULL )
	{
		return -1;
	}
	
	PBDBAchieveBuffer tAchieveBuffer;
	GetTlvDBPB( &tAchieveBuffer );

	char tBuffer[ PALYER_ACHIEVE_TLV_BUFFER ] = { 0 }; 
	int  tBufferSize = sizeof(tBuffer);

	tAchieveBuffer.SerializePartialToArray( tBuffer, tBufferSize );

	char tEscapStr[ PALYER_ACHIEVE_TLV_BUFFER ] = {0};
	char* tPointer = tEscapStr;

	int tRealLen = EscapeDataString( &tPointer, sizeof(tEscapStr)-1, tBuffer, tAchieveBuffer.ByteSize() );

	LK_ASSERT( tRealLen != -1 && "while escape_data_string, dst buff overflow" , return -1);

	CSceneLogic::GetSingletonPtr()->ExecuteSql( em_dbsession_achieve_save, 0, 0, npPlayer->GetCharID(), 
		SQL_NEED_CALLBACK, UPDATE, "replace into UMS_ACHIEVE values( '%d', '%s' );", npPlayer->GetCharID(), tEscapStr );

	return SUCCESS;
}

CAchieveData* CAchieveDataUnit::GetAchieveData(int nID)
{
	ACHIEVE_DATA_UNIT::iterator it = mAchieveUnit.find( nID );
	if ( it == mAchieveUnit.end() )
	{
		return NULL;
	}

	return (CAchieveData*) &( it->second);
}

int CAchieveDataUnit::SetAchieveData( CAchieveData& nData )
{
	ACHIEVE_DATA_UNIT::iterator it = mAchieveUnit.find( nData.GetID() );
	if ( it != mAchieveUnit.end() )
	{
		mAchieveUnit.erase( it );
	}

	mAchieveUnit.insert( ACHIEVE_DATA_UNIT::value_type( nData.GetID(), nData ) );
	return SUCCESS;
}


int CAchieveDataUnit::GetTlvPB( PBAchieveBuffer* npBuffer )
{
	if ( npBuffer == NULL )
	{
		return -1;
	}

	ACHIEVE_DATA_UNIT::iterator it = mAchieveUnit.begin();
	for ( ; it != mAchieveUnit.end(); ++ it )
	{
		CAchieveData* tpData = GetAchieveData( it->first );
		if ( tpData == NULL )
		{
			continue;
		}
		if ( tpData->GetStatus() == false )
		{
			continue;
		}
		PBAchieve* tpAchieve = npBuffer->add_achieve();
		tpData->GetTlvPB( tpAchieve );
	}
	
	return SUCCESS;
}

// DB PB Operation [11/19/2009 Macro]
int CAchieveDataUnit::GetTlvDBPB( PBDBAchieveBuffer* npBuffer )
{
	if ( npBuffer == NULL )
	{
		return -1;
	}

	ACHIEVE_DATA_UNIT::iterator it = mAchieveUnit.begin();
	for ( ; it != mAchieveUnit.end(); ++ it )
	{
		CAchieveData* tpData = GetAchieveData( it->first );
		if ( tpData == NULL )
		{
			continue;
		}
		PBDBAchieve* tpAchieve = npBuffer->add_achieve();
		tpData->GetTlvDBPB( tpAchieve );
	}

	HONOR_TYPE::iterator itHonor = mHonorList.begin();
	for ( ; itHonor != mHonorList.end(); ++ itHonor )
	{
		PBHonorData* tpHonor = npBuffer->add_honordata();
		tpHonor->set_mapid( itHonor->mMapID );
		tpHonor->set_time( itHonor->mTime );
	}

	npBuffer->set_charid( GetOwnerCharID() );
	npBuffer->set_totalpoint( GetTotalPoint() );
	npBuffer->set_stage( GetStage() );

	return SUCCESS;
}

// DB PB Operation [11/19/2009 Macro]
int CAchieveDataUnit::SetFromDBPB( PBDBAchieveBuffer* npBuffer )
{
	if ( npBuffer == NULL )
	{
		return -1;
	}

	for ( int i = 0; i < npBuffer->achieve_size(); i ++ )
	{
		PBDBAchieve* tpAchieve = npBuffer->mutable_achieve( i );
		CAchieveData tData;
		tData.SetFromDBPB( tpAchieve );
		SetAchieveData( tData );
	}

	for ( int i = 0; i < npBuffer->honordata_size(); i ++ )
	{
		PBHonorData* tpHonor = npBuffer->mutable_honordata( i );
		ST_HONOR_DATA tHonor;
		tHonor.mMapID = tpHonor->mapid();
		tHonor.mTime = tpHonor->time();
		mHonorList.push_back( tHonor );
	}

	mCharID = npBuffer->charid();
	mTotalPoint = npBuffer->totalpoint();
	mStage = npBuffer->stage();
	
	return SUCCESS;
}

// 填充参数 [12/10/2009 Macro]
int CAchieveData::SetParamValue( int nValue, int nParam /* = 0 */ )
{
	PARAM_TYPE::iterator it = mParamList.begin();
	if ( it != mParamList.end() )
		it->mKey = nValue;
	else
	{
		ST_ACHIEVE_DATA tParam;
		tParam.mKey = nValue;
		mParamList.push_back( tParam );
	}
	return SUCCESS;
}

int CAchieveData::GetParamValue()
{
	PARAM_TYPE::iterator it = mParamList.begin();
	if ( it != mParamList.end() )
		return it->mKey;
	return SUCCESS;
}

// 设置成就参数数据 [11/20/2009 Macro]
int CAchieveData::SetParam(int nKey, int nValue, int nType )
{
	switch ( nType )
	{
	case emSameKindCnt:
		{
			PARAM_TYPE::iterator it = mParamList.begin();
			if ( it != mParamList.end() )
				it->mKey += nKey;
			else
			{
				ST_ACHIEVE_DATA tParam;
				tParam.mKey = nKey;
				mParamList.push_back( tParam );
			}			
			break;
		}
	case emDiffKindCnt:
		{
			PARAM_TYPE::iterator it = mParamList.begin();
			bool bExist = false;
			for ( ; it != mParamList.end(); ++ it )
			{
				if ( it->mKey == nKey )
				{
					bExist = true;
					it->mValue += 1;
					break;
				}
			}
			if ( !bExist )
			{
				ST_ACHIEVE_DATA tParam;
				tParam.mKey = nKey; tParam.mValue = 1;
				mParamList.push_back( tParam );
			}
			break;
		}
	case emEnumerate:
		{
			PARAM_TYPE::iterator it = mParamList.begin();
			bool bExist = false;
			for ( ; it != mParamList.end(); ++ it )
			{
				if ( it->mKey == nKey )
				{
					bExist = true;
					it->mValue = 1;
					break;
				}
			}
			if ( !bExist )
			{
				ST_ACHIEVE_DATA tParam;
				tParam.mKey = nKey; tParam.mValue = 1;
				mParamList.push_back( tParam );
			}
			break;
		}
	case emSpecial:
		{
			break;
		}
	case emCountReSet:
		{
			PARAM_TYPE::iterator it = mParamList.begin();
			if ( it != mParamList.end() )
				it->mKey = nKey;
			else
			{
				ST_ACHIEVE_DATA tParam;
				tParam.mKey = nKey;
				mParamList.push_back( tParam );
			}			
			break;
		}
	default:
		break;
	}

	return SUCCESS;
}

// 获取成就数据参数列表 [11/20/2009 Macro]
int CAchieveData::GetParam( int* npParamList, int& nNum, int nType )
{
	if ( npParamList == NULL )
	{
		return -1;
	}

	nNum = 0;

	switch ( nType )
	{
	case emSameKindCnt:
	case emCountReSet:
		{
			PARAM_TYPE::iterator it = mParamList.begin();
			npParamList[ 0 ] = it->mKey;
			break;
		}
	case emDiffKindCnt:
		{
			PARAM_TYPE::iterator it = mParamList.begin();
			for ( ; it != mParamList.end(); ++ it )
			{
				if ( nNum < MAX_PARAM )
				{
					npParamList[ nNum ] = it->mKey;
					npParamList[ MAX_PARAM + nNum ] = it->mValue;
					nNum ++;
				}
			}
			break;
		}
	case emEnumerate:
		{
			PARAM_TYPE::iterator it = mParamList.begin();
			for ( ; it != mParamList.end(); ++ it )
			{
				if ( nNum < MAX_PARAM )
				{
					npParamList[ nNum ++ ] = it->mKey;
				}
			}
			break;
		}
	case emTrustReport:
	case emSpecial:
		{
			break;
		}
	default:
		break;
	}

	return SUCCESS;
}

// 检查成就是否完成
bool CAchieveData::CheckAchieveFinish( CAchieveCfgItem* npCfgItem, int nCharID )
{
	if ( npCfgItem == NULL )
	{
		return false;
	}

	int tParamArray[ MAX_PARAM * 2 ] = { 0 };
	int tParamConf[ MAX_PARAM * 2 ] = { 0 };

	int tParamNum = ARRAY_CNT( tParamArray );
	int tConfNum = ARRAY_CNT( tParamConf );

	int tType = npCfgItem->GetParamType();
	if ( GetParam( tParamArray, tParamNum, tType ) < 0
		|| npCfgItem->GetParam( tParamConf, tConfNum ) < 0 )
		return false;

	bool bRet = false;

	switch ( tType )
	{
		// 任意类型、ID得计数
	case emSameKindCnt:
	case emCountReSet:
		{
			if ( tParamArray[ 0 ] >= tParamConf[ 0 ] )
				bRet = true;
			break;
		}
		// 不同指定ID得计数
	case emDiffKindCnt:
		{
			// 全部满足
			if ( npCfgItem->GetRelation() == emRelationAnd )
			{
				bool bFinish = true, bCheckFlag = false;
				for ( int i = 0; i < MAX_PARAM; i ++ )
				{
					if ( tParamConf[ i ] == 0 )
						continue;
						
					bool bExist = false;
					for ( int j = 0; j < MAX_PARAM; j ++ )
					{
						if ( tParamConf[ j ] == 0 )
							continue;
						
						if ( tParamConf[ i ] == tParamArray[ j ] 
						&& tParamArray[ j + MAX_PARAM ] >= tParamConf[ i + MAX_PARAM ] )
						{
							bExist = true;
							bCheckFlag = true;
							break;
						}
					}
					if ( !bExist )
					{
						bFinish = false;
						break;
					}
				}
				if ( bFinish && bCheckFlag )
				{
					bRet = true;
				}
			}
			// 满足之一即可
			else
			{
				bool bFinish = true, bCheckFlag = false;
				for ( int i = 0; i < MAX_PARAM; i ++ )
				{
					if ( tParamConf[ i ] == 0 )
					{
						continue;
					}
					bool bExist = false;
					for ( int j = 0; j < MAX_PARAM; j ++ )
					{
						if ( tParamArray[ j ] == 0 )
						{
							continue;
						}
						if ( tParamConf[ i ] == tParamArray[ j ] 
						&& tParamArray[ j + MAX_PARAM ] >= tParamConf[ i + MAX_PARAM ] )
						{
							bExist = true;
							bCheckFlag = true;
							break;
						}
					}
					if ( bExist )
					{
						break;
					}
				}
				if ( bFinish && bCheckFlag )
				{
					bRet = true;
				}
			}
			
			break;
		}
		// 列举类
	case emEnumerate:
		{
			// 全部满足
			if ( npCfgItem->GetRelation() == emRelationAnd )
			{
				bool bFinish = true, bCheckFlag = false;
				for ( int i = 0; i < MAX_PARAM; i ++ )
				{
					if ( tParamConf[ i ] == 0 )
					{
						continue;
					}
					bool bExist = false;
					for ( int j = 0; j < MAX_PARAM; j ++ )
					{
						if ( tParamArray[ j ] == 0 )
						{
							continue;
						}						
						if ( tParamConf[ i ] == tParamArray[ j ] )
						{
							bExist = true;
							bCheckFlag = true;
							break;
						}
					}
					if ( !bExist )
					{
						bFinish = false;
						break;
					}
				}
				if ( bFinish && bCheckFlag )
				{
					bRet = true;
				}
			}
			else
			{
				bool bFinish = true, bCheckFlag = false;
				for ( int i = 0; i < MAX_PARAM; i ++ )
				{
					if ( tParamConf[ i ] == 0 )
					{
						continue;
					}
					bool bExist = false;
					for ( int j = 0; j < MAX_PARAM; j ++ )
					{
						if ( tParamArray[ j ] == 0 )
						{
							continue;
						}						
						if ( tParamConf[ i ] == tParamArray[ j ] )
						{
							bExist = true;
							bCheckFlag = true;
							break;
						}
					}
					if ( bExist )
					{
						break;
					}
				}
				if ( bFinish && bCheckFlag )
				{
					bRet = true;
				}
			}
			break;
		}
		// 特别验证类
	case emSpecial:
		{
			bRet = CheckSpecialAchieve( tParamConf, tConfNum, nCharID );
			break;
		}
		// 上报即可获得类
	case emTrustReport:
		{
			bRet = true;
			break;
		}
	default:
		break;
	}

	return bRet;
}

// 验证成就数据是否满足 [11/20/2009 Macro]
bool CAchieveData::CheckSpecialAchieve(int *npParamList, int nNum, int nCharID)
{
	if ( npParamList == NULL )
	{
		return false;
	}

	bool bRet = false;

	CEntityPlayer* tpPlayer = ( CEntityPlayer* ) CSceneLogic::GetSingleton().GetPlayerByCharID( nCharID );
	if ( tpPlayer == NULL )
	{
		return false;
	}

	CPropertyPlayer* tpProperty = ( CPropertyPlayer* ) tpPlayer->GetProperty();

	switch ( npParamList[ 0 ] )
	{
		// 人物等级
	case emAchieveSpcLevel:
		{
			int tIndex = 1;
			if ( tpPlayer->GetLevel() >= npParamList[ tIndex ] )
				bRet = true;
			break;
		}
		// 造化小阶成就
	case emAchieveTalentLevel:
		{
			int tIndex = 1;
			if ( tpProperty->GetTalentStep() == npParamList[ tIndex ] )
			{
				bRet = true;
			}
			break;
		}
		//获得的好感度总值
	case emAchieveJudgeSystemIGiveFavorTotal:
		{
			CJudgeModule::GetSingleton().OnEventQueryJudge( tpPlayer->GetCharID(), Judge_Query_Rec_Sum, mID );
			break;
		}
		//多少人对我进行过好感度评价
	case emAchieveJudgeSystemIGiveFavorPeopleNum:
		{
			CJudgeModule::GetSingleton().OnEventQueryJudge( tpPlayer->GetCharID(), Judge_Query_Rec_Cnt, mID );
			break;
		}
		// 收到的评价值达到某值
	case emAchieveJudgeSystemIGiveFavorValue:
		{
			CJudgeModule::GetSingleton().OnEventQueryJudge( tpPlayer->GetCharID(), Judge_Query_Rec_Max, mID );
			break;
		}
        //副本层数成就  add by yucx 20100730
    case emAchieveDungeonQuest:
        {
            CPlayerRepetion * tpPlayerRepetion = tpProperty->GetRepetion();
            if( tpPlayerRepetion == NULL)
            {
                return false;
            }
            int tIndex = 1;
            int tInRepetionIndex = tpPlayerRepetion->GetInRepetionIndex();
            int tRepeFloorNo = CRepetionModule::GetSingleton().GetPlayerRepeFloorNo(tpPlayer);
            if(tInRepetionIndex == npParamList[ tIndex ] && tRepeFloorNo == npParamList[ tIndex + MAX_PARAM ] )
            {
                bRet = true;
            }
            break;
        }
	default:
		break;
	}
	
	return bRet;
}

// 获得指定ID成就的PB结构数据 [11/20/2009 Macro]
int CAchieveDataUnit::GetAchievePB(int nID, PBAchieve *npAchievePB)
{
	if ( npAchievePB == NULL )
	{
		return -1;
	}
	
	CAchieveData* tpData = GetAchieveData( nID );
	if ( tpData == NULL )
	{
		return -1;
	}
	
	tpData->GetTlvPB( npAchievePB );
	return SUCCESS;
}

// 成就数据填充到PB [11/18/2009 Macro]
int CAchieveData::GetTlvPB( PBAchieve* npAchievePB )
{
	if ( npAchievePB == NULL )
	{
		return -1;
	}

	npAchievePB->set_achieveid( GetID() );
	npAchievePB->set_status( GetStatus() );
	npAchievePB->set_time( GetTime() );
	npAchievePB->set_roottype( GetRootType() );

	PARAM_TYPE::iterator it = mParamList.begin();
	for ( ; it != mParamList.end(); ++ it )
	{
		PBAchieveParam* tpParam = npAchievePB->add_param();
		tpParam->set_key( it->mKey );
		tpParam->set_value( it->mValue );
	}
	
	return SUCCESS;
}

// PB数据赋值到成就 [11/18/2009 Macro]
int CAchieveData::SetFromDBPB( PBDBAchieve* npAchievePB )
{
	if ( npAchievePB == NULL )
	{
		return -1;
	}

	SetID( npAchievePB->achieveid() );
	SetStatus( npAchievePB->status() );
	SetTime( npAchievePB->time() );
	SetRootType( npAchievePB->roottype() );

	for ( int i = 0; i < npAchievePB->param_size(); i ++ )
	{
		PBAchieveParam* tpAchieveParam = npAchievePB->mutable_param( i );
		ST_ACHIEVE_DATA tParam;
		tParam.mKey = tpAchieveParam->key();
		tParam.mValue = tpAchieveParam->value();
		mParamList.push_back( tParam );
	}

	return SUCCESS;	
}

int CAchieveData::GetTlvDBPB(PBDBAchieve *npAchievePB)
{
	if ( npAchievePB == NULL )
	{
		return -1;
	}

	npAchievePB->set_achieveid( GetID() );
	npAchievePB->set_status( GetStatus() );
	npAchievePB->set_time( GetTime() );
	npAchievePB->set_roottype( GetRootType() );

	PARAM_TYPE::iterator it = mParamList.begin();
	for ( ; it != mParamList.end(); ++ it )
	{
		PBAchieveParam* tpAchieveParam = npAchievePB->add_param();
		tpAchieveParam->set_key( it->mKey );
		tpAchieveParam->set_value( it->mValue );
	}

	return SUCCESS;
}

// 组装Honor PB List[12/7/2009 Macro]
int CAchieveDataUnit::GetHonorPBList(CMessageGetAllHonorResponse *npResponse)
{
	if ( npResponse == NULL )
	{
		return -1;
	}
	HONOR_TYPE::iterator it = mHonorList.begin();
	for ( ; it != mHonorList.end(); ++ it )
	{
		PBHonorData* tpHonor = npResponse->add_honordata();
		tpHonor->set_mapid( it->mMapID );
		tpHonor->set_time( it->mTime );
	}
	return SUCCESS;	
}

// 添加功德 [12/7/2009 Macro]
int CAchieveDataUnit::PushHonor(ST_HONOR_DATA &nData)
{
	HONOR_TYPE::iterator it = mHonorList.begin();
	for ( ; it != mHonorList.end(); ++ it )
	{
		if ( it->mMapID == nData.mMapID 
			&& nData.mMapID != INVALID_MAP_ID )
		{
			return -1;
		}
	}
	mHonorList.push_back( nData );
	return SUCCESS;
}

int CAchieveCfgItem::PushParam(ST_ACHIEVE_PARAM &nParam)
{
	if ( mParam.size() == MAX_PARAM )
	{
		return -1;
	}	
	mParam.push_back( nParam );
	return SUCCESS;
}

/*
*	npParamList 数组大小为 2 * MAX_PARAM
*/
int CAchieveCfgItem::GetParam(int *npParamList, int &nNum)
{
	if ( npParamList == NULL )
	{
		return -1;
	}

	int tSize = nNum; nNum = 0;

	// 数组大小小于1,逻辑无效
	if ( INVALID_ARRAY( tSize ) ) return -1;		

	switch ( mParamType )
	{
	case emSameKindCnt:
	case emCountReSet:
		{
			PARAM_TYPE::iterator it = mParam.begin();
			if ( it != mParam.end() )
			{
				npParamList[ 0 ] = it->mValue;
				nNum = 1;
			}					
			break;
		}
	case emDiffKindCnt:
		{
			PARAM_TYPE::iterator it = mParam.begin();
			for ( ; it != mParam.end(); ++ it )
			{
				npParamList[ nNum ] = it->mValue;
				npParamList[ nNum + MAX_PARAM ] = it->mParam;
				nNum ++;
			}
			break;
		}
	case emEnumerate:
		{
			PARAM_TYPE::iterator it = mParam.begin();
			for ( ; it != mParam.end(); ++ it )
			{
				npParamList[ nNum ++ ] = it->mValue;
			}
			break;
		}
	case emTrustReport:
		{
			PARAM_TYPE::iterator it = mParam.begin();
			for ( ; it != mParam.end(); ++ it )
			{
				npParamList[ 0 ] = it->mType;
				break;
			}
			break;
		}
	case emSpecial:
		{
			PARAM_TYPE::iterator it = mParam.begin();
			nNum = 1;
			for ( ; it != mParam.end() && nNum < MAX_PARAM; ++ it )
			{
				npParamList[ 0 ] = it->mType;
				npParamList[ nNum ] = it->mValue;
				npParamList[ nNum + MAX_PARAM ] = it->mParam;
				nNum += 1;
			}
			break;
		}
	default:
		break;
	}

	return SUCCESS;
}

// 检查上报的数值是否是有效的 [12/2/2009 Macro]
bool CAchieveCfgItem::CheckParamValid( int nKey)
{
	bool bRet = true;
	switch ( mParamType )
	{
	case emDiffKindCnt:
	case emEnumerate:
		{
			bool bValid = false;
			PARAM_TYPE::iterator it = mParam.begin();
			for ( ; it != mParam.end(); ++ it )
			{
				if ( it->mValue == nKey )
				{
					bValid = true;
					break;
				}
			}
			if ( !bValid )
			{
				bRet = false;
			}
			break;
		}
	default:
		break;
	}

	return bRet;
}

// 服务器端对成就数据进行处理 [12/10/2009 Macro]
int CAchieveDataUnit::UpdateAchieveDataInServer(int nID, int nValue, CAchieveCfgItem *npCfgItem)
{
	if ( npCfgItem == NULL )
	{
		return -1;
	}
	
	CAchieveData* tpData = GetAchieveData( nID );
	if ( tpData == NULL )
	{
		return -1;
	}

	tpData->SetParamValue( nValue );

	int tCfgArray[ MAX_PARAM ] = { 0 };
	int tArrCount = ARRAY_CNT( tCfgArray );

	npCfgItem->GetParam( tCfgArray, tArrCount );
	
	if ( tArrCount != 2 )
	{
		LOG_ERROR( "achieve", "Cfg[%d] Error, more config params.[%s:%d]", nID, __LK_FILE__, __LINE__ );
		return -1;
	}
	
	int tParam = tCfgArray[ 1 ];
	if ( nValue >= tParam )
	{
		tpData->SetStatus( true );
		tpData->SetTime( time( NULL ) );
		IncTotalPoint( npCfgItem->GetPoint() );
		return ACHIEVE_FINISH;
	}
	
	return SUCCESS;
}
