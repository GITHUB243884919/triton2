#ifdef WIN32
#include "win32network.h"
#elif LINUX
#include "linuxnetwork.h"
#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif
#include "scenecfg_manager.h"

CSharedMem* CSceneCfgManager::mShmPtr = NULL;
template<> CSceneCfgManager* CSingleton< CSceneCfgManager >::spSingleton = NULL;



CSceneCfgManager::CSceneCfgManager()
{
	EvaluateInit();
}


CSceneCfgManager::~CSceneCfgManager()
{

}

unsigned int CSceneCfgManager::CountSize()
{
	return sizeof( CSceneCfgManager );
}
int CSceneCfgManager::Initialize( char* pDir )
{
	LK_ASSERT( pDir != NULL, return -1 );
	TiXmlDocument tDoc;
	TiXmlElement *pRoot, *pElement;
	if( !tDoc.LoadFile( pDir ) )
		return -1;

	pRoot = tDoc.RootElement();
	if( pRoot == NULL )
		return -1;
	pElement = pRoot->FirstChildElement();
	while( pElement != NULL )
	{
		char tName[ VAR_NAME_LEN ] = "";
		FormatUpper( const_cast<char*>(pElement->Attribute("name")), tName );
		if(strncmp(tName,"ACHIEVE_LOG", strlen("ACHIEVE_LOG")) == 0)
		{
			SetAchieve_log( tName, pElement );
		}
		else if(strncmp(tName,"ACTIVITY_LOG", strlen("ACTIVITY_LOG")) == 0)
		{
			SetActivity_log( tName, pElement );
		}
		else if(strncmp(tName,"APEX", strlen("APEX")) == 0)
		{
			SetApex( tName, pElement );
		}
		else if(strncmp(tName,"BOURSE_CFG", strlen("BOURSE_CFG")) == 0)
		{
			SetBourse_cfg( tName, pElement );
		}
		else if(strncmp(tName,"BOUR_LOG", strlen("BOUR_LOG")) == 0)
		{
			SetBour_log( tName, pElement );
		}
		else if(strncmp(tName,"CHAT_CFG", strlen("CHAT_CFG")) == 0)
		{
			SetChat_cfg( tName, pElement );
		}
		else if(strncmp(tName,"CHAT_LOG", strlen("CHAT_LOG")) == 0)
		{
			SetChat_log( tName, pElement );
		}
		else if(strncmp(tName,"CORPS_CFG", strlen("CORPS_CFG")) == 0)
		{
			SetCorps_cfg( tName, pElement );
		}
		else if(strncmp(tName,"CORPS_LOG", strlen("CORPS_LOG")) == 0)
		{
			SetCorps_log( tName, pElement );
		}
		else if(strncmp(tName,"DEFAULT_LOG", strlen("DEFAULT_LOG")) == 0)
		{
			SetDefault_log( tName, pElement );
		}
		else if(strncmp(tName,"FAMILY_CFG", strlen("FAMILY_CFG")) == 0)
		{
			SetFamily_cfg( tName, pElement );
		}
		else if(strncmp(tName,"FAMILY_LOG", strlen("FAMILY_LOG")) == 0)
		{
			SetFamily_log( tName, pElement );
		}
		else if(strncmp(tName,"FORMULA_CFG", strlen("FORMULA_CFG")) == 0)
		{
			SetFormula_cfg( tName, pElement );
		}
		else if(strncmp(tName,"FRIEND_CFG", strlen("FRIEND_CFG")) == 0)
		{
			SetFriend_cfg( tName, pElement );
		}
		else if(strncmp(tName,"FRIEND_LOG", strlen("FRIEND_LOG")) == 0)
		{
			SetFriend_log( tName, pElement );
		}
		else if(strncmp(tName,"FUNCCONSUME_CFG", strlen("FUNCCONSUME_CFG")) == 0)
		{
			SetFuncconsume_cfg( tName, pElement );
		}
		else if(strncmp(tName,"GM_LOG", strlen("GM_LOG")) == 0)
		{
			SetGm_log( tName, pElement );
		}
		else if(strncmp(tName,"IBTRANS_LOG", strlen("IBTRANS_LOG")) == 0)
		{
			SetIbtrans_log( tName, pElement );
		}
		else if(strncmp(tName,"LOGEVENT_CFG", strlen("LOGEVENT_CFG")) == 0)
		{
			SetLogevent_cfg( tName, pElement );
		}
		else if(strncmp(tName,"MAIL_LOG", strlen("MAIL_LOG")) == 0)
		{
			SetMail_log( tName, pElement );
		}
		else if(strncmp(tName,"MAP_CFG", strlen("MAP_CFG")) == 0)
		{
			SetMap_cfg( tName, pElement );
		}
		else if(strncmp(tName,"MAP_LOG", strlen("MAP_LOG")) == 0)
		{
			SetMap_log( tName, pElement );
		}
		else if(strncmp(tName,"MARRIAGE_LOG", strlen("MARRIAGE_LOG")) == 0)
		{
			SetMarriage_log( tName, pElement );
		}
		else if(strncmp(tName,"MW_CFG", strlen("MW_CFG")) == 0)
		{
			SetMw_cfg( tName, pElement );
		}
		else if(strncmp(tName,"NATION_LOG", strlen("NATION_LOG")) == 0)
		{
			SetNation_log( tName, pElement );
		}
		else if(strncmp(tName,"NPC_CFG", strlen("NPC_CFG")) == 0)
		{
			SetNpc_cfg( tName, pElement );
		}
		else if(strncmp(tName,"PROPERTY_CFG", strlen("PROPERTY_CFG")) == 0)
		{
			SetProperty_cfg( tName, pElement );
		}
		else if(strncmp(tName,"PROPERTY_LOG", strlen("PROPERTY_LOG")) == 0)
		{
			SetProperty_log( tName, pElement );
		}
		else if(strncmp(tName,"RANK_CFG", strlen("RANK_CFG")) == 0)
		{
			SetRank_cfg( tName, pElement );
		}
		else if(strncmp(tName,"RANK_LOG", strlen("RANK_LOG")) == 0)
		{
			SetRank_log( tName, pElement );
		}
		else if(strncmp(tName,"REPETION_CFG", strlen("REPETION_CFG")) == 0)
		{
			SetRepetion_cfg( tName, pElement );
		}
		else if(strncmp(tName,"REPE_LOG", strlen("REPE_LOG")) == 0)
		{
			SetRepe_log( tName, pElement );
		}
		else if(strncmp(tName,"SCENELOGIC_CFG", strlen("SCENELOGIC_CFG")) == 0)
		{
			SetScenelogic_cfg( tName, pElement );
		}
		else if(strncmp(tName,"SCENE_LOG", strlen("SCENE_LOG")) == 0)
		{
			SetScene_log( tName, pElement );
		}
		else if(strncmp(tName,"SPEEDCHECKER_CFG", strlen("SPEEDCHECKER_CFG")) == 0)
		{
			SetSpeedchecker_cfg( tName, pElement );
		}
		else if(strncmp(tName,"STALL_CFG", strlen("STALL_CFG")) == 0)
		{
			SetStall_cfg( tName, pElement );
		}
		else if(strncmp(tName,"STALL_LOG", strlen("STALL_LOG")) == 0)
		{
			SetStall_log( tName, pElement );
		}
		else if(strncmp(tName,"STAT_LOG", strlen("STAT_LOG")) == 0)
		{
			SetStat_log( tName, pElement );
		}
		else if(strncmp(tName,"TASKLUA_LOG", strlen("TASKLUA_LOG")) == 0)
		{
			SetTasklua_log( tName, pElement );
		}
		else if(strncmp(tName,"TASK_CFG", strlen("TASK_CFG")) == 0)
		{
			SetTask_cfg( tName, pElement );
		}
		else if(strncmp(tName,"TASK_LOG", strlen("TASK_LOG")) == 0)
		{
			SetTask_log( tName, pElement );
		}
		else if(strncmp(tName,"TEAM_CFG", strlen("TEAM_CFG")) == 0)
		{
			SetTeam_cfg( tName, pElement );
		}
		else if(strncmp(tName,"TEAM_LOG", strlen("TEAM_LOG")) == 0)
		{
			SetTeam_log( tName, pElement );
		}
		else if(strncmp(tName,"TRADE_CFG", strlen("TRADE_CFG")) == 0)
		{
			SetTrade_cfg( tName, pElement );
		}
		else if(strncmp(tName,"TRADE_LOG", strlen("TRADE_LOG")) == 0)
		{
			SetTrade_log( tName, pElement );
		}
		else if(strncmp(tName,"YQSACTIVITY_LOG", strlen("YQSACTIVITY_LOG")) == 0)
		{
			SetYqsactivity_log( tName, pElement );
		}
		pElement = pElement->NextSiblingElement();
	}
	return 0;
}
void CSceneCfgManager::SetAchieve_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mAchieve_log.mDirectory, tpValue, sizeof( mAchieve_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mAchieve_log.mIdentifier, tpValue, sizeof( mAchieve_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mAchieve_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mAchieve_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mAchieve_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mAchieve_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetActivity_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mActivity_log.mDirectory, tpValue, sizeof( mActivity_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mActivity_log.mIdentifier, tpValue, sizeof( mActivity_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mActivity_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mActivity_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mActivity_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mActivity_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetApex( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mApex.mDirectory, tpValue, sizeof( mApex.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mApex.mIdentifier, tpValue, sizeof( mApex.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mApex.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mApex.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mMulti_client", tParam, strlen( tParam )) == 0)
			{
				mApex.mMulti_client = atoi( tpValue );
			}
			else if(strncmp("mMulti_user", tParam, strlen( tParam )) == 0)
			{
				mApex.mMulti_user = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mApex.mPriority = atoi( tpValue );
			}
			else if(strncmp("mVm", tParam, strlen( tParam )) == 0)
			{
				mApex.mVm = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mApex.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetBourse_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mBuynotify", tParam, strlen( tParam )) == 0)
			{
				mBourse_cfg.mBuynotify = atoi( tpValue );
			}
			else if(strncmp("mMoneytax", tParam, strlen( tParam )) == 0)
			{
				mBourse_cfg.mMoneytax = atoi( tpValue );
			}
			else if(strncmp("mSellnotify", tParam, strlen( tParam )) == 0)
			{
				mBourse_cfg.mSellnotify = atoi( tpValue );
			}
			else if(strncmp("mTaxid", tParam, strlen( tParam )) == 0)
			{
				mBourse_cfg.mTaxid = atoi( tpValue );
			}
			else if(strncmp("mUse", tParam, strlen( tParam )) == 0)
			{
				mBourse_cfg.mUse = atoi( tpValue );
			}
			else if(strncmp("mYbtax", tParam, strlen( tParam )) == 0)
			{
				mBourse_cfg.mYbtax = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{

			//TODO ...

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetBour_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mBour_log.mDirectory, tpValue, sizeof( mBour_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mBour_log.mIdentifier, tpValue, sizeof( mBour_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mBour_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mBour_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mBour_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mBour_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetChat_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mBuglelevellimit", tParam, strlen( tParam )) == 0)
			{
				mChat_cfg.mBuglelevellimit = atoi( tpValue );
			}
			else if(strncmp("mBugletime", tParam, strlen( tParam )) == 0)
			{
				mChat_cfg.mBugletime = atoi( tpValue );
			}
			else if(strncmp("mCampviptime", tParam, strlen( tParam )) == 0)
			{
				mChat_cfg.mCampviptime = atoi( tpValue );
			}
			else if(strncmp("mChatlimittime", tParam, strlen( tParam )) == 0)
			{
				mChat_cfg.mChatlimittime = atoi( tpValue );
			}
			else if(strncmp("mCommonautochattime", tParam, strlen( tParam )) == 0)
			{
				mChat_cfg.mCommonautochattime = atoi( tpValue );
			}
			else if(strncmp("mCorpstime", tParam, strlen( tParam )) == 0)
			{
				mChat_cfg.mCorpstime = atoi( tpValue );
			}
			else if(strncmp("mCountrytime", tParam, strlen( tParam )) == 0)
			{
				mChat_cfg.mCountrytime = atoi( tpValue );
			}
			else if(strncmp("mFamilytime", tParam, strlen( tParam )) == 0)
			{
				mChat_cfg.mFamilytime = atoi( tpValue );
			}
			else if(strncmp("mMaxautochattime", tParam, strlen( tParam )) == 0)
			{
				mChat_cfg.mMaxautochattime = atoi( tpValue );
			}
			else if(strncmp("mMsglimittime", tParam, strlen( tParam )) == 0)
			{
				mChat_cfg.mMsglimittime = atoi( tpValue );
			}
			else if(strncmp("mWorldaplimit", tParam, strlen( tParam )) == 0)
			{
				mChat_cfg.mWorldaplimit = atoi( tpValue );
			}
			else if(strncmp("mWorldautochattime", tParam, strlen( tParam )) == 0)
			{
				mChat_cfg.mWorldautochattime = atoi( tpValue );
			}
			else if(strncmp("mWorldlevellimit", tParam, strlen( tParam )) == 0)
			{
				mChat_cfg.mWorldlevellimit = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{

			//TODO ...

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetChat_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mChat_log.mDirectory, tpValue, sizeof( mChat_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mChat_log.mIdentifier, tpValue, sizeof( mChat_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mChat_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mChat_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mChat_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mChat_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetCorps_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mBattlebeginhour", tParam, strlen( tParam )) == 0)
			{
				mCorps_cfg.mBattlebeginhour = atoi( tpValue );
			}
			else if(strncmp("mBattlebeginweek", tParam, strlen( tParam )) == 0)
			{
				mCorps_cfg.mBattlebeginweek = atoi( tpValue );
			}
			else if(strncmp("mBattlewaitingtime", tParam, strlen( tParam )) == 0)
			{
				mCorps_cfg.mBattlewaitingtime = atoi( tpValue );
			}
			else if(strncmp("mBidbeginhour", tParam, strlen( tParam )) == 0)
			{
				mCorps_cfg.mBidbeginhour = atoi( tpValue );
			}
			else if(strncmp("mBidbeginmin", tParam, strlen( tParam )) == 0)
			{
				mCorps_cfg.mBidbeginmin = atoi( tpValue );
			}
			else if(strncmp("mBidbeginweek", tParam, strlen( tParam )) == 0)
			{
				mCorps_cfg.mBidbeginweek = atoi( tpValue );
			}
			else if(strncmp("mBidendhour", tParam, strlen( tParam )) == 0)
			{
				mCorps_cfg.mBidendhour = atoi( tpValue );
			}
			else if(strncmp("mBidendmin", tParam, strlen( tParam )) == 0)
			{
				mCorps_cfg.mBidendmin = atoi( tpValue );
			}
			else if(strncmp("mBidendweek", tParam, strlen( tParam )) == 0)
			{
				mCorps_cfg.mBidendweek = atoi( tpValue );
			}
			else if(strncmp("mEnterbattlebuffid", tParam, strlen( tParam )) == 0)
			{
				mCorps_cfg.mEnterbattlebuffid = atoi( tpValue );
			}
			else if(strncmp("mExploitperglory", tParam, strlen( tParam )) == 0)
			{
				mCorps_cfg.mExploitperglory = atoi( tpValue );
			}
			else if(strncmp("mExpperglory", tParam, strlen( tParam )) == 0)
			{
				mCorps_cfg.mExpperglory = atoi( tpValue );
			}
			else if(strncmp("mMendhpvalue", tParam, strlen( tParam )) == 0)
			{
				mCorps_cfg.mMendhpvalue = atoi( tpValue );
			}
			else if(strncmp("mMinbidmoney", tParam, strlen( tParam )) == 0)
			{
				mCorps_cfg.mMinbidmoney = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{

			//TODO ...

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetCorps_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mCorps_log.mDirectory, tpValue, sizeof( mCorps_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mCorps_log.mIdentifier, tpValue, sizeof( mCorps_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mCorps_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mCorps_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mCorps_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mCorps_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetDefault_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mDefault_log.mDirectory, tpValue, sizeof( mDefault_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mDefault_log.mIdentifier, tpValue, sizeof( mDefault_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mDefault_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mDefault_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mDefault_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mDefault_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetFamily_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mExploitperglory", tParam, strlen( tParam )) == 0)
			{
				mFamily_cfg.mExploitperglory = atoi( tpValue );
			}
			else if(strncmp("mExpperglory", tParam, strlen( tParam )) == 0)
			{
				mFamily_cfg.mExpperglory = atoi( tpValue );
			}
			else if(strncmp("mMinstabilitytime", tParam, strlen( tParam )) == 0)
			{
				mFamily_cfg.mMinstabilitytime = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{

			//TODO ...

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetFamily_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mFamily_log.mDirectory, tpValue, sizeof( mFamily_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mFamily_log.mIdentifier, tpValue, sizeof( mFamily_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mFamily_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mFamily_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mFamily_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mFamily_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetFormula_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mCureparam1", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mCureparam1 = atoi( tpValue );
			}
			else if(strncmp("mCureparam2", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mCureparam2 = atoi( tpValue );
			}
			else if(strncmp("mDamageparam1", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mDamageparam1 = atoi( tpValue );
			}
			else if(strncmp("mDamageparam2", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mDamageparam2 = atoi( tpValue );
			}
			else if(strncmp("mDeathchangeparam", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mDeathchangeparam = atoi( tpValue );
			}
			else if(strncmp("mDeathparam1", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mDeathparam1 = atoi( tpValue );
			}
			else if(strncmp("mDeathparam2", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mDeathparam2 = atoi( tpValue );
			}
			else if(strncmp("mDeathparam3", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mDeathparam3 = atoi( tpValue );
			}
			else if(strncmp("mDeathparam4", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mDeathparam4 = atoi( tpValue );
			}
			else if(strncmp("mDeathparam5", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mDeathparam5 = atoi( tpValue );
			}
			else if(strncmp("mDeathparam6", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mDeathparam6 = atoi( tpValue );
			}
			else if(strncmp("mDotparam1", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mDotparam1 = atoi( tpValue );
			}
			else if(strncmp("mDotparam2", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mDotparam2 = atoi( tpValue );
			}
			else if(strncmp("mDuckparam1", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mDuckparam1 = atoi( tpValue );
			}
			else if(strncmp("mDuckparam2", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mDuckparam2 = atoi( tpValue );
			}
			else if(strncmp("mDuckparam3", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mDuckparam3 = atoi( tpValue );
			}
			else if(strncmp("mDuckparam4", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mDuckparam4 = atoi( tpValue );
			}
			else if(strncmp("mDuckparam5", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mDuckparam5 = atoi( tpValue );
			}
			else if(strncmp("mDuckparam6", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mDuckparam6 = atoi( tpValue );
			}
			else if(strncmp("mHitparam1", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mHitparam1 = atoi( tpValue );
			}
			else if(strncmp("mHitparam2", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mHitparam2 = atoi( tpValue );
			}
			else if(strncmp("mHitparam3", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mHitparam3 = atoi( tpValue );
			}
			else if(strncmp("mHitparam4", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mHitparam4 = atoi( tpValue );
			}
			else if(strncmp("mHitparam5", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mHitparam5 = atoi( tpValue );
			}
			else if(strncmp("mHitparam6", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mHitparam6 = atoi( tpValue );
			}
			else if(strncmp("mHitparam7", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mHitparam7 = atoi( tpValue );
			}
			else if(strncmp("mHotparam1", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mHotparam1 = atoi( tpValue );
			}
			else if(strncmp("mHotparam2", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mHotparam2 = atoi( tpValue );
			}
			else if(strncmp("mResparam1", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mResparam1 = atoi( tpValue );
			}
			else if(strncmp("mResparam2", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mResparam2 = atoi( tpValue );
			}
			else if(strncmp("mResparam3", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mResparam3 = atoi( tpValue );
			}
			else if(strncmp("mResparam4", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mResparam4 = atoi( tpValue );
			}
			else if(strncmp("mResparam5", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mResparam5 = atoi( tpValue );
			}
			else if(strncmp("mShieldparam1", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mShieldparam1 = atoi( tpValue );
			}
			else if(strncmp("mShieldparam2", tParam, strlen( tParam )) == 0)
			{
				mFormula_cfg.mShieldparam2 = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{

			//TODO ...

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetFriend_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxoncefriends", tParam, strlen( tParam )) == 0)
			{
				mFriend_cfg.mMaxoncefriends = atoi( tpValue );
			}
			else if(strncmp("mMaxrefreshtime", tParam, strlen( tParam )) == 0)
			{
				mFriend_cfg.mMaxrefreshtime = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{

			//TODO ...

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetFriend_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mFriend_log.mDirectory, tpValue, sizeof( mFriend_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mFriend_log.mIdentifier, tpValue, sizeof( mFriend_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mFriend_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mFriend_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mFriend_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mFriend_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetFuncconsume_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mIsopen", tParam, strlen( tParam )) == 0)
			{
				mFuncconsume_cfg.mIsopen = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetGm_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mGm_log.mDirectory, tpValue, sizeof( mGm_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mGm_log.mIdentifier, tpValue, sizeof( mGm_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mGm_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mGm_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mGm_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mGm_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetIbtrans_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mIbtrans_log.mDirectory, tpValue, sizeof( mIbtrans_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mIbtrans_log.mIdentifier, tpValue, sizeof( mIbtrans_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mIbtrans_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mIbtrans_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mIbtrans_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mIbtrans_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetLogevent_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mLog_item_dispear", tParam, strlen( tParam )) == 0)
			{
				mLogevent_cfg.mLog_item_dispear = atoi( tpValue );
			}
			else if(strncmp("mLog_item_levelup", tParam, strlen( tParam )) == 0)
			{
				mLogevent_cfg.mLog_item_levelup = atoi( tpValue );
			}
			else if(strncmp("mLog_life_exp", tParam, strlen( tParam )) == 0)
			{
				mLogevent_cfg.mLog_life_exp = atoi( tpValue );
			}
			else if(strncmp("mLog_mw_exp", tParam, strlen( tParam )) == 0)
			{
				mLogevent_cfg.mLog_mw_exp = atoi( tpValue );
			}
			else if(strncmp("mLog_npc_drop", tParam, strlen( tParam )) == 0)
			{
				mLogevent_cfg.mLog_npc_drop = atoi( tpValue );
			}
			else if(strncmp("mLog_produce_exp", tParam, strlen( tParam )) == 0)
			{
				mLogevent_cfg.mLog_produce_exp = atoi( tpValue );
			}
			else if(strncmp("mLog_role_exp", tParam, strlen( tParam )) == 0)
			{
				mLogevent_cfg.mLog_role_exp = atoi( tpValue );
			}
			else if(strncmp("mLog_role_item", tParam, strlen( tParam )) == 0)
			{
				mLogevent_cfg.mLog_role_item = atoi( tpValue );
			}
			else if(strncmp("mLog_role_killed", tParam, strlen( tParam )) == 0)
			{
				mLogevent_cfg.mLog_role_killed = atoi( tpValue );
			}
			else if(strncmp("mLog_role_levelup", tParam, strlen( tParam )) == 0)
			{
				mLogevent_cfg.mLog_role_levelup = atoi( tpValue );
			}
			else if(strncmp("mLog_role_logout", tParam, strlen( tParam )) == 0)
			{
				mLogevent_cfg.mLog_role_logout = atoi( tpValue );
			}
			else if(strncmp("mLog_role_money", tParam, strlen( tParam )) == 0)
			{
				mLogevent_cfg.mLog_role_money = atoi( tpValue );
			}
			else if(strncmp("mLog_role_task", tParam, strlen( tParam )) == 0)
			{
				mLogevent_cfg.mLog_role_task = atoi( tpValue );
			}
			else if(strncmp("mLog_talent_exp", tParam, strlen( tParam )) == 0)
			{
				mLogevent_cfg.mLog_talent_exp = atoi( tpValue );
			}
			else if(strncmp("mMw_exp_limit", tParam, strlen( tParam )) == 0)
			{
				mLogevent_cfg.mMw_exp_limit = atoi( tpValue );
			}
			else if(strncmp("mProduce_exp_limit", tParam, strlen( tParam )) == 0)
			{
				mLogevent_cfg.mProduce_exp_limit = atoi( tpValue );
			}
			else if(strncmp("mRole_exp_limit", tParam, strlen( tParam )) == 0)
			{
				mLogevent_cfg.mRole_exp_limit = atoi( tpValue );
			}
			else if(strncmp("mTalent_exp_limit", tParam, strlen( tParam )) == 0)
			{
				mLogevent_cfg.mTalent_exp_limit = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{

			//TODO ...

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetMail_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mMail_log.mDirectory, tpValue, sizeof( mMail_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mMail_log.mIdentifier, tpValue, sizeof( mMail_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mMail_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mMail_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mMail_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mMail_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetMap_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMovecheck", tParam, strlen( tParam )) == 0)
			{
				mMap_cfg.mMovecheck = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{

			//TODO ...

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetMap_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mMap_log.mDirectory, tpValue, sizeof( mMap_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mMap_log.mIdentifier, tpValue, sizeof( mMap_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mMap_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mMap_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mMap_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mMap_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetMarriage_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mMarriage_log.mDirectory, tpValue, sizeof( mMarriage_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mMarriage_log.mIdentifier, tpValue, sizeof( mMarriage_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mMarriage_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mMarriage_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mMarriage_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mMarriage_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetMw_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mLevelextraeff", tParam, strlen( tParam )) == 0)
			{
				mMw_cfg.mLevelextraeff = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{

			//TODO ...

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetNation_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mNation_log.mDirectory, tpValue, sizeof( mNation_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mNation_log.mIdentifier, tpValue, sizeof( mNation_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mNation_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mNation_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mNation_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mNation_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetNpc_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mPetattackinstance", tParam, strlen( tParam )) == 0)
			{
				mNpc_cfg.mPetattackinstance = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{

			//TODO ...

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetProperty_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mCreateunion_id", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mCreateunion_id = atoi( tpValue );
			}
			else if(strncmp("mExp_award", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mExp_award = atoi( tpValue );
			}
			else if(strncmp("mExp_task", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mExp_task = atoi( tpValue );
			}
			else if(strncmp("mFcm_buff", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mFcm_buff = atoi( tpValue );
			}
			else if(strncmp("mFlower_lmt_perday", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mFlower_lmt_perday = atoi( tpValue );
			}
			else if(strncmp("mFlower_lmt_type", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mFlower_lmt_type = atoi( tpValue );
			}
			else if(strncmp("mJusticelevellimit", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mJusticelevellimit = atoi( tpValue );
			}
			else if(strncmp("mNewuserpkgid", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mNewuserpkgid = atoi( tpValue );
			}
			else if(strncmp("mRefresh_hour", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mRefresh_hour = atoi( tpValue );
			}
			else if(strncmp("mRefresh_minute", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mRefresh_minute = atoi( tpValue );
			}
			else if(strncmp("mRefresh_week", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mRefresh_week = atoi( tpValue );
			}
			else if(strncmp("mResetpp_index", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mResetpp_index = atoi( tpValue );
			}
			else if(strncmp("mResetpp_num", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mResetpp_num = atoi( tpValue );
			}
			else if(strncmp("mResetsp_index", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mResetsp_index = atoi( tpValue );
			}
			else if(strncmp("mResetsp_num", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mResetsp_num = atoi( tpValue );
			}
			else if(strncmp("mTalentexp", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mTalentexp = atoi( tpValue );
			}
			else if(strncmp("mVip_middle", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mVip_middle = atoi( tpValue );
			}
			else if(strncmp("mVip_normal", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mVip_normal = atoi( tpValue );
			}
			else if(strncmp("mVip_senior", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mVip_senior = atoi( tpValue );
			}
			else if(strncmp("mYuan_id", tParam, strlen( tParam )) == 0)
			{
				mProperty_cfg.mYuan_id = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{

			//TODO ...

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetProperty_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mProperty_log.mDirectory, tpValue, sizeof( mProperty_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mProperty_log.mIdentifier, tpValue, sizeof( mProperty_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mProperty_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mProperty_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mProperty_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mProperty_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetRank_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mLonggongmap1", tParam, strlen( tParam )) == 0)
			{
				mRank_cfg.mLonggongmap1 = atoi( tpValue );
			}
			else if(strncmp("mLonggongmap2", tParam, strlen( tParam )) == 0)
			{
				mRank_cfg.mLonggongmap2 = atoi( tpValue );
			}
			else if(strncmp("mLonggongmap3", tParam, strlen( tParam )) == 0)
			{
				mRank_cfg.mLonggongmap3 = atoi( tpValue );
			}
			else if(strncmp("mPantaomap1", tParam, strlen( tParam )) == 0)
			{
				mRank_cfg.mPantaomap1 = atoi( tpValue );
			}
			else if(strncmp("mPantaomap2", tParam, strlen( tParam )) == 0)
			{
				mRank_cfg.mPantaomap2 = atoi( tpValue );
			}
			else if(strncmp("mPantaomap3", tParam, strlen( tParam )) == 0)
			{
				mRank_cfg.mPantaomap3 = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{

			//TODO ...

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetRank_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mRank_log.mDirectory, tpValue, sizeof( mRank_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mRank_log.mIdentifier, tpValue, sizeof( mRank_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mRank_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mRank_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mRank_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mRank_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetRepetion_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mCampkickrate", tParam, strlen( tParam )) == 0)
			{
				mRepetion_cfg.mCampkickrate = atoi( tpValue );
			}
			else if(strncmp("mNumcheck", tParam, strlen( tParam )) == 0)
			{
				mRepetion_cfg.mNumcheck = atoi( tpValue );
			}
			else if(strncmp("mPlayercheck", tParam, strlen( tParam )) == 0)
			{
				mRepetion_cfg.mPlayercheck = atoi( tpValue );
			}
			else if(strncmp("mQuestionanswertime", tParam, strlen( tParam )) == 0)
			{
				mRepetion_cfg.mQuestionanswertime = atoi( tpValue );
			}
			else if(strncmp("mRepetionkickouttime", tParam, strlen( tParam )) == 0)
			{
				mRepetion_cfg.mRepetionkickouttime = atoi( tpValue );
			}
			else if(strncmp("mRepetionverifytime", tParam, strlen( tParam )) == 0)
			{
				mRepetion_cfg.mRepetionverifytime = atoi( tpValue );
			}
			else if(strncmp("mTimecheck", tParam, strlen( tParam )) == 0)
			{
				mRepetion_cfg.mTimecheck = atoi( tpValue );
			}
			else if(strncmp("mTimehighlimit", tParam, strlen( tParam )) == 0)
			{
				mRepetion_cfg.mTimehighlimit = atoi( tpValue );
			}
			else if(strncmp("mTimelowlimit", tParam, strlen( tParam )) == 0)
			{
				mRepetion_cfg.mTimelowlimit = atoi( tpValue );
			}
			else if(strncmp("mVotetime", tParam, strlen( tParam )) == 0)
			{
				mRepetion_cfg.mVotetime = atoi( tpValue );
			}
			else if(strncmp("mWrongtimes", tParam, strlen( tParam )) == 0)
			{
				mRepetion_cfg.mWrongtimes = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{

			//TODO ...

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetRepe_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mRepe_log.mDirectory, tpValue, sizeof( mRepe_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mRepe_log.mIdentifier, tpValue, sizeof( mRepe_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mRepe_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mRepe_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mRepe_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mRepe_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetScenelogic_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mKicknonactivityplayertime", tParam, strlen( tParam )) == 0)
			{
				mScenelogic_cfg.mKicknonactivityplayertime = atoi( tpValue );
			}
			else if(strncmp("mMovecheckdistance", tParam, strlen( tParam )) == 0)
			{
				mScenelogic_cfg.mMovecheckdistance = atoi( tpValue );
			}
			else if(strncmp("mMovechecktimespace", tParam, strlen( tParam )) == 0)
			{
				mScenelogic_cfg.mMovechecktimespace = atoi( tpValue );
			}
			else if(strncmp("mMsgcheckcountlimit", tParam, strlen( tParam )) == 0)
			{
				mScenelogic_cfg.mMsgcheckcountlimit = atoi( tpValue );
			}
			else if(strncmp("mMsglimitpersec", tParam, strlen( tParam )) == 0)
			{
				mScenelogic_cfg.mMsglimitpersec = atoi( tpValue );
			}
			else if(strncmp("mPingcheck", tParam, strlen( tParam )) == 0)
			{
				mScenelogic_cfg.mPingcheck = atoi( tpValue );
			}
			else if(strncmp("mPlayerredirecttimeout", tParam, strlen( tParam )) == 0)
			{
				mScenelogic_cfg.mPlayerredirecttimeout = atoi( tpValue );
			}
			else if(strncmp("mPlayersavedbtime", tParam, strlen( tParam )) == 0)
			{
				mScenelogic_cfg.mPlayersavedbtime = atoi( tpValue );
			}
			else if(strncmp("mTogateheartbeatsec", tParam, strlen( tParam )) == 0)
			{
				mScenelogic_cfg.mTogateheartbeatsec = atoi( tpValue );
			}
			else if(strncmp("mTologstatinfosec", tParam, strlen( tParam )) == 0)
			{
				mScenelogic_cfg.mTologstatinfosec = atoi( tpValue );
			}
			else if(strncmp("mToproxyheartbeatsec", tParam, strlen( tParam )) == 0)
			{
				mScenelogic_cfg.mToproxyheartbeatsec = atoi( tpValue );
			}
			else if(strncmp("mValidatedataintegrity", tParam, strlen( tParam )) == 0)
			{
				mScenelogic_cfg.mValidatedataintegrity = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{

			//TODO ...

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetScene_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mScene_log.mDirectory, tpValue, sizeof( mScene_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mScene_log.mIdentifier, tpValue, sizeof( mScene_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mScene_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mScene_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mScene_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mScene_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetSpeedchecker_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxabidecountsforclientspeed", tParam, strlen( tParam )) == 0)
			{
				mSpeedchecker_cfg.mMaxabidecountsforclientspeed = atoi( tpValue );
			}
			else if(strncmp("mMaxabidecountsforcstimediff", tParam, strlen( tParam )) == 0)
			{
				mSpeedchecker_cfg.mMaxabidecountsforcstimediff = atoi( tpValue );
			}
			else if(strncmp("mMaxerrorcountforclientspeed", tParam, strlen( tParam )) == 0)
			{
				mSpeedchecker_cfg.mMaxerrorcountforclientspeed = atoi( tpValue );
			}
			else if(strncmp("mMaxtimeerrorforclientspeed", tParam, strlen( tParam )) == 0)
			{
				mSpeedchecker_cfg.mMaxtimeerrorforclientspeed = atoi( tpValue );
			}
			else if(strncmp("mMaxtimeerrorforcstimediff", tParam, strlen( tParam )) == 0)
			{
				mSpeedchecker_cfg.mMaxtimeerrorforcstimediff = atoi( tpValue );
			}
			else if(strncmp("mMintimeerrorforcstimediff", tParam, strlen( tParam )) == 0)
			{
				mSpeedchecker_cfg.mMintimeerrorforcstimediff = atoi( tpValue );
			}
			else if(strncmp("mTimeerrorforcstimediff", tParam, strlen( tParam )) == 0)
			{
				mSpeedchecker_cfg.mTimeerrorforcstimediff = atoi( tpValue );
			}
			else if(strncmp("mTimesyncountforclientspeed", tParam, strlen( tParam )) == 0)
			{
				mSpeedchecker_cfg.mTimesyncountforclientspeed = atoi( tpValue );
			}
			else if(strncmp("mTimesyncountforcstimediff", tParam, strlen( tParam )) == 0)
			{
				mSpeedchecker_cfg.mTimesyncountforcstimediff = atoi( tpValue );
			}
			else if(strncmp("mTimesynintervalforcstimediff", tParam, strlen( tParam )) == 0)
			{
				mSpeedchecker_cfg.mTimesynintervalforcstimediff = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mUsespeedcheckerforclientspeed", tParam, strlen( tParam )) == 0)
			{
				mSpeedchecker_cfg.mUsespeedcheckerforclientspeed = (bool)atoi( tpValue );
			}
			else if(strncmp("mUsespeedcheckerforcstimediff", tParam, strlen( tParam )) == 0)
			{
				mSpeedchecker_cfg.mUsespeedcheckerforcstimediff = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetStall_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mFeeibid", tParam, strlen( tParam )) == 0)
			{
				mStall_cfg.mFeeibid = atoi( tpValue );
			}
			else if(strncmp("mFeerate", tParam, strlen( tParam )) == 0)
			{
				mStall_cfg.mFeerate = atoi( tpValue );
			}
			else if(strncmp("mLevellimit", tParam, strlen( tParam )) == 0)
			{
				mStall_cfg.mLevellimit = atoi( tpValue );
			}
			else if(strncmp("mMaxadcontent", tParam, strlen( tParam )) == 0)
			{
				mStall_cfg.mMaxadcontent = atoi( tpValue );
			}
			else if(strncmp("mMaxnamelen", tParam, strlen( tParam )) == 0)
			{
				mStall_cfg.mMaxnamelen = atoi( tpValue );
			}
			else if(strncmp("mMaxprice", tParam, strlen( tParam )) == 0)
			{
				mStall_cfg.mMaxprice = atoi( tpValue );
			}
			else if(strncmp("mMinybprice", tParam, strlen( tParam )) == 0)
			{
				mStall_cfg.mMinybprice = atoi( tpValue );
			}
			else if(strncmp("mSaftytime", tParam, strlen( tParam )) == 0)
			{
				mStall_cfg.mSaftytime = atoi( tpValue );
			}
			else if(strncmp("mStallflag", tParam, strlen( tParam )) == 0)
			{
				mStall_cfg.mStallflag = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{

			//TODO ...

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetStall_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mStall_log.mDirectory, tpValue, sizeof( mStall_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mStall_log.mIdentifier, tpValue, sizeof( mStall_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mStall_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mStall_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mStall_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mStall_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetStat_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mStat_log.mDirectory, tpValue, sizeof( mStat_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mStat_log.mIdentifier, tpValue, sizeof( mStat_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mStat_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mStat_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mStat_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mStat_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetTasklua_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mTasklua_log.mDirectory, tpValue, sizeof( mTasklua_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mTasklua_log.mIdentifier, tpValue, sizeof( mTasklua_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mTasklua_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mTasklua_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mTasklua_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mTasklua_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetTask_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mIscanabort", tParam, strlen( tParam )) == 0)
			{
				mTask_cfg.mIscanabort = atoi( tpValue );
			}
			else if(strncmp("mLooptasksubnum", tParam, strlen( tParam )) == 0)
			{
				mTask_cfg.mLooptasksubnum = atoi( tpValue );
			}
			else if(strncmp("mRefreshstoragetime", tParam, strlen( tParam )) == 0)
			{
				mTask_cfg.mRefreshstoragetime = atoi( tpValue );
			}
			else if(strncmp("mSearchmaindownlevel", tParam, strlen( tParam )) == 0)
			{
				mTask_cfg.mSearchmaindownlevel = atoi( tpValue );
			}
			else if(strncmp("mSearchmainuplevel", tParam, strlen( tParam )) == 0)
			{
				mTask_cfg.mSearchmainuplevel = atoi( tpValue );
			}
			else if(strncmp("mSearchxydownlevel", tParam, strlen( tParam )) == 0)
			{
				mTask_cfg.mSearchxydownlevel = atoi( tpValue );
			}
			else if(strncmp("mSearchxyuplevel", tParam, strlen( tParam )) == 0)
			{
				mTask_cfg.mSearchxyuplevel = atoi( tpValue );
			}
			else if(strncmp("mSearchzhidownlevel", tParam, strlen( tParam )) == 0)
			{
				mTask_cfg.mSearchzhidownlevel = atoi( tpValue );
			}
			else if(strncmp("mSearchzhiuplevel", tParam, strlen( tParam )) == 0)
			{
				mTask_cfg.mSearchzhiuplevel = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{

			//TODO ...

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetTask_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mTask_log.mDirectory, tpValue, sizeof( mTask_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mTask_log.mIdentifier, tpValue, sizeof( mTask_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mTask_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mTask_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mTask_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mTask_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetTeam_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mCooltime", tParam, strlen( tParam )) == 0)
			{
				mTeam_cfg.mCooltime = atoi( tpValue );
			}
			else if(strncmp("mSheetpage", tParam, strlen( tParam )) == 0)
			{
				mTeam_cfg.mSheetpage = atoi( tpValue );
			}
			else if(strncmp("mUpdatetime", tParam, strlen( tParam )) == 0)
			{
				mTeam_cfg.mUpdatetime = atoi( tpValue );
			}
			else if(strncmp("mValiddistance", tParam, strlen( tParam )) == 0)
			{
				mTeam_cfg.mValiddistance = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{

			//TODO ...

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetTeam_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mTeam_log.mDirectory, tpValue, sizeof( mTeam_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mTeam_log.mIdentifier, tpValue, sizeof( mTeam_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mTeam_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mTeam_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mTeam_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mTeam_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetTrade_cfg( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{

			//TODO ...

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mSaftytime", tParam, strlen( tParam )) == 0)
			{
				mTrade_cfg.mSaftytime = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{

			//TODO ...

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetTrade_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mTrade_log.mDirectory, tpValue, sizeof( mTrade_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mTrade_log.mIdentifier, tpValue, sizeof( mTrade_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mTrade_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mTrade_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mTrade_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mTrade_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
void CSceneCfgManager::SetYqsactivity_log( char* pStruct, TiXmlElement* pElement )
{
	LK_ASSERT(pElement != NULL && pStruct != NULL, return);
	char tStruct[VAR_NAME_LEN] = "";
	char tInput[VAR_NAME_LEN] = "";
	FormatUpper( const_cast<char*>(pElement->Attribute("name")), tStruct );
	FormatUpper( pStruct, tInput );
	if(strncmp(tInput, tStruct, strlen(tStruct)) != 0)
	{
		return;
	}
	TiXmlElement* pItem= pElement->FirstChildElement();
	while( pItem != NULL )
	{
		if(pItem->Attribute("name") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("value") == NULL)
			pItem = pItem->NextSiblingElement();
		if(pItem->Attribute("type") == NULL)
			pItem = pItem->NextSiblingElement();
		char tParam[ VAR_NAME_LEN ] = "";
		FormatParam( const_cast<char*>(pItem->Attribute("name")), tParam );
		char* tpValue = const_cast<char*>(pItem->Attribute("value"));
		if(strncmp("string",pItem->Attribute("type"), strlen("string") ) == 0)
		{
			if(strncmp("mDirectory", tParam, strlen( tParam )) == 0)
			{
				strncpy(mYqsactivity_log.mDirectory, tpValue, sizeof( mYqsactivity_log.mDirectory ) - 1 );
			}
			else if(strncmp("mIdentifier", tParam, strlen( tParam )) == 0)
			{
				strncpy(mYqsactivity_log.mIdentifier, tpValue, sizeof( mYqsactivity_log.mIdentifier ) - 1 );
			}

		}
		else if(strncmp("int",pItem->Attribute("type"), strlen("int") ) == 0)
		{
			if(strncmp("mMaxbackupindex", tParam, strlen( tParam )) == 0)
			{
				mYqsactivity_log.mMaxbackupindex = atoi( tpValue );
			}
			else if(strncmp("mMaxfilesize", tParam, strlen( tParam )) == 0)
			{
				mYqsactivity_log.mMaxfilesize = atoi( tpValue );
			}
			else if(strncmp("mPriority", tParam, strlen( tParam )) == 0)
			{
				mYqsactivity_log.mPriority = atoi( tpValue );
			}

		}
		else if(strncmp("bool",pItem->Attribute("type"), strlen("bool") ) == 0)
		{
			if(strncmp("mAppend", tParam, strlen( tParam )) == 0)
			{
				mYqsactivity_log.mAppend = (bool)atoi( tpValue );
			}

		}

		pItem = pItem->NextSiblingElement();
	}

	return;
}
ACHIEVE_LOG* CSceneCfgManager::GetAchieve_log()
{
	return &mAchieve_log;
}
ACTIVITY_LOG* CSceneCfgManager::GetActivity_log()
{
	return &mActivity_log;
}
APEX* CSceneCfgManager::GetApex()
{
	return &mApex;
}
BOURSE_CFG* CSceneCfgManager::GetBourse_cfg()
{
	return &mBourse_cfg;
}
BOUR_LOG* CSceneCfgManager::GetBour_log()
{
	return &mBour_log;
}
CHAT_CFG* CSceneCfgManager::GetChat_cfg()
{
	return &mChat_cfg;
}
CHAT_LOG* CSceneCfgManager::GetChat_log()
{
	return &mChat_log;
}
CORPS_CFG* CSceneCfgManager::GetCorps_cfg()
{
	return &mCorps_cfg;
}
CORPS_LOG* CSceneCfgManager::GetCorps_log()
{
	return &mCorps_log;
}
DEFAULT_LOG* CSceneCfgManager::GetDefault_log()
{
	return &mDefault_log;
}
FAMILY_CFG* CSceneCfgManager::GetFamily_cfg()
{
	return &mFamily_cfg;
}
FAMILY_LOG* CSceneCfgManager::GetFamily_log()
{
	return &mFamily_log;
}
FORMULA_CFG* CSceneCfgManager::GetFormula_cfg()
{
	return &mFormula_cfg;
}
FRIEND_CFG* CSceneCfgManager::GetFriend_cfg()
{
	return &mFriend_cfg;
}
FRIEND_LOG* CSceneCfgManager::GetFriend_log()
{
	return &mFriend_log;
}
FUNCCONSUME_CFG* CSceneCfgManager::GetFuncconsume_cfg()
{
	return &mFuncconsume_cfg;
}
GM_LOG* CSceneCfgManager::GetGm_log()
{
	return &mGm_log;
}
IBTRANS_LOG* CSceneCfgManager::GetIbtrans_log()
{
	return &mIbtrans_log;
}
LOGEVENT_CFG* CSceneCfgManager::GetLogevent_cfg()
{
	return &mLogevent_cfg;
}
MAIL_LOG* CSceneCfgManager::GetMail_log()
{
	return &mMail_log;
}
MAP_CFG* CSceneCfgManager::GetMap_cfg()
{
	return &mMap_cfg;
}
MAP_LOG* CSceneCfgManager::GetMap_log()
{
	return &mMap_log;
}
MARRIAGE_LOG* CSceneCfgManager::GetMarriage_log()
{
	return &mMarriage_log;
}
MW_CFG* CSceneCfgManager::GetMw_cfg()
{
	return &mMw_cfg;
}
NATION_LOG* CSceneCfgManager::GetNation_log()
{
	return &mNation_log;
}
NPC_CFG* CSceneCfgManager::GetNpc_cfg()
{
	return &mNpc_cfg;
}
PROPERTY_CFG* CSceneCfgManager::GetProperty_cfg()
{
	return &mProperty_cfg;
}
PROPERTY_LOG* CSceneCfgManager::GetProperty_log()
{
	return &mProperty_log;
}
RANK_CFG* CSceneCfgManager::GetRank_cfg()
{
	return &mRank_cfg;
}
RANK_LOG* CSceneCfgManager::GetRank_log()
{
	return &mRank_log;
}
REPETION_CFG* CSceneCfgManager::GetRepetion_cfg()
{
	return &mRepetion_cfg;
}
REPE_LOG* CSceneCfgManager::GetRepe_log()
{
	return &mRepe_log;
}
SCENELOGIC_CFG* CSceneCfgManager::GetScenelogic_cfg()
{
	return &mScenelogic_cfg;
}
SCENE_LOG* CSceneCfgManager::GetScene_log()
{
	return &mScene_log;
}
SPEEDCHECKER_CFG* CSceneCfgManager::GetSpeedchecker_cfg()
{
	return &mSpeedchecker_cfg;
}
STALL_CFG* CSceneCfgManager::GetStall_cfg()
{
	return &mStall_cfg;
}
STALL_LOG* CSceneCfgManager::GetStall_log()
{
	return &mStall_log;
}
STAT_LOG* CSceneCfgManager::GetStat_log()
{
	return &mStat_log;
}
TASKLUA_LOG* CSceneCfgManager::GetTasklua_log()
{
	return &mTasklua_log;
}
TASK_CFG* CSceneCfgManager::GetTask_cfg()
{
	return &mTask_cfg;
}
TASK_LOG* CSceneCfgManager::GetTask_log()
{
	return &mTask_log;
}
TEAM_CFG* CSceneCfgManager::GetTeam_cfg()
{
	return &mTeam_cfg;
}
TEAM_LOG* CSceneCfgManager::GetTeam_log()
{
	return &mTeam_log;
}
TRADE_CFG* CSceneCfgManager::GetTrade_cfg()
{
	return &mTrade_cfg;
}
TRADE_LOG* CSceneCfgManager::GetTrade_log()
{
	return &mTrade_log;
}
YQSACTIVITY_LOG* CSceneCfgManager::GetYqsactivity_log()
{
	return &mYqsactivity_log;
}

void CSceneCfgManager::FormatParam( char* pSrcStr, char* pDesStr )
{
 	LK_ASSERT( pSrcStr != NULL ,return );
	int i = 0;
	char tParam[VAR_NAME_LEN] = "";
	while( pSrcStr[i] != '\0' )
	{
      if( i == 0 )
		{
			tParam[i] = toupper( pSrcStr[i] );
			++ i;
			continue;
		}
		tParam[i] = tolower( pSrcStr[i] );      
		++ i;
	}
	char tTemp[VAR_NAME_LEN] = "";
	tTemp[0] = 'm';
	strncat( tTemp, tParam, VAR_NAME_LEN - 1 );
	strncpy( pDesStr, tTemp, VAR_NAME_LEN - 1 );
	return ;
}

void CSceneCfgManager::FormatUpper( char* pSrcStr, char* pDesStr )
{
 	LK_ASSERT( pSrcStr != NULL , return );
	while( *pSrcStr != '\0' )
	{
		*pDesStr++ = toupper( *pSrcStr++ );
	}
	return ;
}

void CSceneCfgManager::EvaluateInit()
{
	mAchieve_log.mAppend = ( bool ) 1;
	memset( mAchieve_log.mDirectory, 0, sizeof( mAchieve_log.mDirectory ) );
	strncpy( mAchieve_log.mDirectory, "../log/achieve.log", sizeof( mAchieve_log.mDirectory ) - 1 );
	memset( mAchieve_log.mIdentifier, 0, sizeof( mAchieve_log.mIdentifier ) );
	strncpy( mAchieve_log.mIdentifier, "achieve", sizeof( mAchieve_log.mIdentifier ) - 1 );
	mAchieve_log.mMaxbackupindex = 60;
	mAchieve_log.mMaxfilesize = 10485760;
	mAchieve_log.mPriority = 700;

	mActivity_log.mAppend = ( bool ) 1;
	memset( mActivity_log.mDirectory, 0, sizeof( mActivity_log.mDirectory ) );
	strncpy( mActivity_log.mDirectory, "../log/activity.log", sizeof( mActivity_log.mDirectory ) - 1 );
	memset( mActivity_log.mIdentifier, 0, sizeof( mActivity_log.mIdentifier ) );
	strncpy( mActivity_log.mIdentifier, "Activity", sizeof( mActivity_log.mIdentifier ) - 1 );
	mActivity_log.mMaxbackupindex = 60;
	mActivity_log.mMaxfilesize = 10485760;
	mActivity_log.mPriority = 700;

	mApex.mAppend = ( bool ) 1;
	memset( mApex.mDirectory, 0, sizeof( mApex.mDirectory ) );
	strncpy( mApex.mDirectory, "../log/apex.log", sizeof( mApex.mDirectory ) - 1 );
	memset( mApex.mIdentifier, 0, sizeof( mApex.mIdentifier ) );
	strncpy( mApex.mIdentifier, "apex", sizeof( mApex.mIdentifier ) - 1 );
	mApex.mMaxbackupindex = 50;
	mApex.mMaxfilesize = 10485760;
	mApex.mMulti_client = 47001;
	mApex.mMulti_user = 47011;
	mApex.mPriority = 600;
	mApex.mVm = 47011;

	mBourse_cfg.mBuynotify = 3000;
	mBourse_cfg.mMoneytax = 1;
	mBourse_cfg.mSellnotify = 3000;
	mBourse_cfg.mTaxid = 67726;
	mBourse_cfg.mUse = 1;
	mBourse_cfg.mYbtax = 1;

	mBour_log.mAppend = ( bool ) 1;
	memset( mBour_log.mDirectory, 0, sizeof( mBour_log.mDirectory ) );
	strncpy( mBour_log.mDirectory, "../log/bourse.log", sizeof( mBour_log.mDirectory ) - 1 );
	memset( mBour_log.mIdentifier, 0, sizeof( mBour_log.mIdentifier ) );
	strncpy( mBour_log.mIdentifier, "bourse", sizeof( mBour_log.mIdentifier ) - 1 );
	mBour_log.mMaxbackupindex = 60;
	mBour_log.mMaxfilesize = 10485760;
	mBour_log.mPriority = 700;

	mChat_cfg.mBuglelevellimit = 20;
	mChat_cfg.mBugletime = 8;
	mChat_cfg.mCampviptime = 50;
	mChat_cfg.mChatlimittime = 1;
	mChat_cfg.mCommonautochattime = 3;
	mChat_cfg.mCorpstime = 3;
	mChat_cfg.mCountrytime = 60;
	mChat_cfg.mFamilytime = 1;
	mChat_cfg.mMaxautochattime = 300;
	mChat_cfg.mMsglimittime = 6;
	mChat_cfg.mWorldaplimit = 20;
	mChat_cfg.mWorldautochattime = 180;
	mChat_cfg.mWorldlevellimit = 20;

	mChat_log.mAppend = ( bool ) 1;
	memset( mChat_log.mDirectory, 0, sizeof( mChat_log.mDirectory ) );
	strncpy( mChat_log.mDirectory, "../log/chat.log", sizeof( mChat_log.mDirectory ) - 1 );
	memset( mChat_log.mIdentifier, 0, sizeof( mChat_log.mIdentifier ) );
	strncpy( mChat_log.mIdentifier, "chat", sizeof( mChat_log.mIdentifier ) - 1 );
	mChat_log.mMaxbackupindex = 60;
	mChat_log.mMaxfilesize = 10485760;
	mChat_log.mPriority = 700;

	mCorps_cfg.mBattlebeginhour = 19;
	mCorps_cfg.mBattlebeginweek = 5;
	mCorps_cfg.mBattlewaitingtime = 15;
	mCorps_cfg.mBidbeginhour = 1;
	mCorps_cfg.mBidbeginmin = 0;
	mCorps_cfg.mBidbeginweek = 0;
	mCorps_cfg.mBidendhour = 23;
	mCorps_cfg.mBidendmin = 59;
	mCorps_cfg.mBidendweek = 6;
	mCorps_cfg.mEnterbattlebuffid = 64000;
	mCorps_cfg.mExploitperglory = 20;
	mCorps_cfg.mExpperglory = 100000;
	mCorps_cfg.mMendhpvalue = 10;
	mCorps_cfg.mMinbidmoney = 5;

	mCorps_log.mAppend = ( bool ) 1;
	memset( mCorps_log.mDirectory, 0, sizeof( mCorps_log.mDirectory ) );
	strncpy( mCorps_log.mDirectory, "../log/corps.log", sizeof( mCorps_log.mDirectory ) - 1 );
	memset( mCorps_log.mIdentifier, 0, sizeof( mCorps_log.mIdentifier ) );
	strncpy( mCorps_log.mIdentifier, "corps", sizeof( mCorps_log.mIdentifier ) - 1 );
	mCorps_log.mMaxbackupindex = 60;
	mCorps_log.mMaxfilesize = 10485760;
	mCorps_log.mPriority = 700;

	mDefault_log.mAppend = ( bool ) 1;
	memset( mDefault_log.mDirectory, 0, sizeof( mDefault_log.mDirectory ) );
	strncpy( mDefault_log.mDirectory, "../log/sceneserver.log", sizeof( mDefault_log.mDirectory ) - 1 );
	memset( mDefault_log.mIdentifier, 0, sizeof( mDefault_log.mIdentifier ) );
	strncpy( mDefault_log.mIdentifier, "default", sizeof( mDefault_log.mIdentifier ) - 1 );
	mDefault_log.mMaxbackupindex = 100;
	mDefault_log.mMaxfilesize = 10485760;
	mDefault_log.mPriority = 700;

	mFamily_cfg.mExploitperglory = 20;
	mFamily_cfg.mExpperglory = 100000;
	mFamily_cfg.mMinstabilitytime = 3600;

	mFamily_log.mAppend = ( bool ) 1;
	memset( mFamily_log.mDirectory, 0, sizeof( mFamily_log.mDirectory ) );
	strncpy( mFamily_log.mDirectory, "../log/family.log", sizeof( mFamily_log.mDirectory ) - 1 );
	memset( mFamily_log.mIdentifier, 0, sizeof( mFamily_log.mIdentifier ) );
	strncpy( mFamily_log.mIdentifier, "family", sizeof( mFamily_log.mIdentifier ) - 1 );
	mFamily_log.mMaxbackupindex = 60;
	mFamily_log.mMaxfilesize = 10485760;
	mFamily_log.mPriority = 700;

	mFormula_cfg.mCureparam1 = 3500;
	mFormula_cfg.mCureparam2 = 12000;
	mFormula_cfg.mDamageparam1 = 6000;
	mFormula_cfg.mDamageparam2 = 10000;
	mFormula_cfg.mDeathchangeparam = 30;
	mFormula_cfg.mDeathparam1 = 10000;
	mFormula_cfg.mDeathparam2 = 22000000;
	mFormula_cfg.mDeathparam3 = 10200;
	mFormula_cfg.mDeathparam4 = 300000;
	mFormula_cfg.mDeathparam5 = 20000;
	mFormula_cfg.mDeathparam6 = 100;
	mFormula_cfg.mDotparam1 = 2000;
	mFormula_cfg.mDotparam2 = 12000;
	mFormula_cfg.mDuckparam1 = 200;
	mFormula_cfg.mDuckparam2 = 10000;
	mFormula_cfg.mDuckparam3 = 10000;
	mFormula_cfg.mDuckparam4 = 2000000;
	mFormula_cfg.mDuckparam5 = 15000;
	mFormula_cfg.mDuckparam6 = 50000;
	mFormula_cfg.mHitparam1 = 8500;
	mFormula_cfg.mHitparam2 = 50;
	mFormula_cfg.mHitparam3 = 11500;
	mFormula_cfg.mHitparam4 = 10000;
	mFormula_cfg.mHitparam5 = 10000000;
	mFormula_cfg.mHitparam6 = 15600;
	mFormula_cfg.mHitparam7 = 90000;
	mFormula_cfg.mHotparam1 = 4500;
	mFormula_cfg.mHotparam2 = 12000;
	mFormula_cfg.mResparam1 = 50000;
	mFormula_cfg.mResparam2 = 10000;
	mFormula_cfg.mResparam3 = 500000;
	mFormula_cfg.mResparam4 = 20000;
	mFormula_cfg.mResparam5 = 18300;
	mFormula_cfg.mShieldparam1 = 5000;
	mFormula_cfg.mShieldparam2 = 11000;

	mFriend_cfg.mMaxoncefriends = 30;
	mFriend_cfg.mMaxrefreshtime = 30;

	mFriend_log.mAppend = ( bool ) 1;
	memset( mFriend_log.mDirectory, 0, sizeof( mFriend_log.mDirectory ) );
	strncpy( mFriend_log.mDirectory, "../log/FRIEND.log", sizeof( mFriend_log.mDirectory ) - 1 );
	memset( mFriend_log.mIdentifier, 0, sizeof( mFriend_log.mIdentifier ) );
	strncpy( mFriend_log.mIdentifier, "FRIEND", sizeof( mFriend_log.mIdentifier ) - 1 );
	mFriend_log.mMaxbackupindex = 60;
	mFriend_log.mMaxfilesize = 10485760;
	mFriend_log.mPriority = 700;

	mFuncconsume_cfg.mIsopen = ( bool ) 1;

	mGm_log.mAppend = ( bool ) 1;
	memset( mGm_log.mDirectory, 0, sizeof( mGm_log.mDirectory ) );
	strncpy( mGm_log.mDirectory, "../log/gm.log", sizeof( mGm_log.mDirectory ) - 1 );
	memset( mGm_log.mIdentifier, 0, sizeof( mGm_log.mIdentifier ) );
	strncpy( mGm_log.mIdentifier, "gm", sizeof( mGm_log.mIdentifier ) - 1 );
	mGm_log.mMaxbackupindex = 60;
	mGm_log.mMaxfilesize = 10485760;
	mGm_log.mPriority = 700;

	mIbtrans_log.mAppend = ( bool ) 1;
	memset( mIbtrans_log.mDirectory, 0, sizeof( mIbtrans_log.mDirectory ) );
	strncpy( mIbtrans_log.mDirectory, "../log/ibtrans.log", sizeof( mIbtrans_log.mDirectory ) - 1 );
	memset( mIbtrans_log.mIdentifier, 0, sizeof( mIbtrans_log.mIdentifier ) );
	strncpy( mIbtrans_log.mIdentifier, "ibtrans", sizeof( mIbtrans_log.mIdentifier ) - 1 );
	mIbtrans_log.mMaxbackupindex = 60;
	mIbtrans_log.mMaxfilesize = 10485760;
	mIbtrans_log.mPriority = 700;

	mLogevent_cfg.mLog_item_dispear = 1;
	mLogevent_cfg.mLog_item_levelup = 1;
	mLogevent_cfg.mLog_life_exp = 1;
	mLogevent_cfg.mLog_mw_exp = 1;
	mLogevent_cfg.mLog_npc_drop = 1;
	mLogevent_cfg.mLog_produce_exp = 1;
	mLogevent_cfg.mLog_role_exp = 1;
	mLogevent_cfg.mLog_role_item = 1;
	mLogevent_cfg.mLog_role_killed = 1;
	mLogevent_cfg.mLog_role_levelup = 1;
	mLogevent_cfg.mLog_role_logout = 1;
	mLogevent_cfg.mLog_role_money = 1;
	mLogevent_cfg.mLog_role_task = 1;
	mLogevent_cfg.mLog_talent_exp = 1;
	mLogevent_cfg.mMw_exp_limit = 1000;
	mLogevent_cfg.mProduce_exp_limit = 100;
	mLogevent_cfg.mRole_exp_limit = 200;
	mLogevent_cfg.mTalent_exp_limit = 100;

	mMail_log.mAppend = ( bool ) 1;
	memset( mMail_log.mDirectory, 0, sizeof( mMail_log.mDirectory ) );
	strncpy( mMail_log.mDirectory, "../log/mail.log", sizeof( mMail_log.mDirectory ) - 1 );
	memset( mMail_log.mIdentifier, 0, sizeof( mMail_log.mIdentifier ) );
	strncpy( mMail_log.mIdentifier, "mail", sizeof( mMail_log.mIdentifier ) - 1 );
	mMail_log.mMaxbackupindex = 60;
	mMail_log.mMaxfilesize = 10485760;
	mMail_log.mPriority = 700;

	mMap_cfg.mMovecheck = 0;

	mMap_log.mAppend = ( bool ) 1;
	memset( mMap_log.mDirectory, 0, sizeof( mMap_log.mDirectory ) );
	strncpy( mMap_log.mDirectory, "../log/map.log", sizeof( mMap_log.mDirectory ) - 1 );
	memset( mMap_log.mIdentifier, 0, sizeof( mMap_log.mIdentifier ) );
	strncpy( mMap_log.mIdentifier, "map", sizeof( mMap_log.mIdentifier ) - 1 );
	mMap_log.mMaxbackupindex = 100;
	mMap_log.mMaxfilesize = 10485760;
	mMap_log.mPriority = 700;

	mMarriage_log.mAppend = ( bool ) 1;
	memset( mMarriage_log.mDirectory, 0, sizeof( mMarriage_log.mDirectory ) );
	strncpy( mMarriage_log.mDirectory, "../log/marriage.log", sizeof( mMarriage_log.mDirectory ) - 1 );
	memset( mMarriage_log.mIdentifier, 0, sizeof( mMarriage_log.mIdentifier ) );
	strncpy( mMarriage_log.mIdentifier, "marriage", sizeof( mMarriage_log.mIdentifier ) - 1 );
	mMarriage_log.mMaxbackupindex = 60;
	mMarriage_log.mMaxfilesize = 10485760;
	mMarriage_log.mPriority = 700;

	mMw_cfg.mLevelextraeff = 100;

	mNation_log.mAppend = ( bool ) 1;
	memset( mNation_log.mDirectory, 0, sizeof( mNation_log.mDirectory ) );
	strncpy( mNation_log.mDirectory, "../log/Nation.log", sizeof( mNation_log.mDirectory ) - 1 );
	memset( mNation_log.mIdentifier, 0, sizeof( mNation_log.mIdentifier ) );
	strncpy( mNation_log.mIdentifier, "Nation", sizeof( mNation_log.mIdentifier ) - 1 );
	mNation_log.mMaxbackupindex = 60;
	mNation_log.mMaxfilesize = 10485760;
	mNation_log.mPriority = 700;

	mNpc_cfg.mPetattackinstance = 1500;

	mProperty_cfg.mCreateunion_id = 57501;
	mProperty_cfg.mExp_award = 10000;
	mProperty_cfg.mExp_task = 10000;
	mProperty_cfg.mFcm_buff = 63852;
	mProperty_cfg.mFlower_lmt_perday = 20;
	mProperty_cfg.mFlower_lmt_type = 0;
	mProperty_cfg.mJusticelevellimit = 10;
	mProperty_cfg.mNewuserpkgid = 76849;
	mProperty_cfg.mRefresh_hour = 0;
	mProperty_cfg.mRefresh_minute = 0;
	mProperty_cfg.mRefresh_week = 1;
	mProperty_cfg.mResetpp_index = 20090604;
	mProperty_cfg.mResetpp_num = 3;
	mProperty_cfg.mResetsp_index = 20090604;
	mProperty_cfg.mResetsp_num = 3;
	mProperty_cfg.mTalentexp = 100;
	mProperty_cfg.mVip_middle = 4;
	mProperty_cfg.mVip_normal = 2;
	mProperty_cfg.mVip_senior = 6;
	mProperty_cfg.mYuan_id = 33305;

	mProperty_log.mAppend = ( bool ) 1;
	memset( mProperty_log.mDirectory, 0, sizeof( mProperty_log.mDirectory ) );
	strncpy( mProperty_log.mDirectory, "../log/property.log", sizeof( mProperty_log.mDirectory ) - 1 );
	memset( mProperty_log.mIdentifier, 0, sizeof( mProperty_log.mIdentifier ) );
	strncpy( mProperty_log.mIdentifier, "pro", sizeof( mProperty_log.mIdentifier ) - 1 );
	mProperty_log.mMaxbackupindex = 100;
	mProperty_log.mMaxfilesize = 10485760;
	mProperty_log.mPriority = 700;

	mRank_cfg.mLonggongmap1 = 7;
	mRank_cfg.mLonggongmap2 = 8;
	mRank_cfg.mLonggongmap3 = 9;
	mRank_cfg.mPantaomap1 = 10;
	mRank_cfg.mPantaomap2 = 11;
	mRank_cfg.mPantaomap3 = 12;

	mRank_log.mAppend = ( bool ) 1;
	memset( mRank_log.mDirectory, 0, sizeof( mRank_log.mDirectory ) );
	strncpy( mRank_log.mDirectory, "../log/rank.log", sizeof( mRank_log.mDirectory ) - 1 );
	memset( mRank_log.mIdentifier, 0, sizeof( mRank_log.mIdentifier ) );
	strncpy( mRank_log.mIdentifier, "rank", sizeof( mRank_log.mIdentifier ) - 1 );
	mRank_log.mMaxbackupindex = 60;
	mRank_log.mMaxfilesize = 10485760;
	mRank_log.mPriority = 700;

	mRepetion_cfg.mCampkickrate = 60;
	mRepetion_cfg.mNumcheck = 1;
	mRepetion_cfg.mPlayercheck = 1;
	mRepetion_cfg.mQuestionanswertime = 30;
	mRepetion_cfg.mRepetionkickouttime = 10;
	mRepetion_cfg.mRepetionverifytime = 10;
	mRepetion_cfg.mTimecheck = 1;
	mRepetion_cfg.mTimehighlimit = 300;
	mRepetion_cfg.mTimelowlimit = 30;
	mRepetion_cfg.mVotetime = 20;
	mRepetion_cfg.mWrongtimes = 3;

	mRepe_log.mAppend = ( bool ) 1;
	memset( mRepe_log.mDirectory, 0, sizeof( mRepe_log.mDirectory ) );
	strncpy( mRepe_log.mDirectory, "../log/repetion.log", sizeof( mRepe_log.mDirectory ) - 1 );
	memset( mRepe_log.mIdentifier, 0, sizeof( mRepe_log.mIdentifier ) );
	strncpy( mRepe_log.mIdentifier, "repeion", sizeof( mRepe_log.mIdentifier ) - 1 );
	mRepe_log.mMaxbackupindex = 60;
	mRepe_log.mMaxfilesize = 10485760;
	mRepe_log.mPriority = 700;

	mScenelogic_cfg.mKicknonactivityplayertime = 120;
	mScenelogic_cfg.mMovecheckdistance = 30;
	mScenelogic_cfg.mMovechecktimespace = 60000;
	mScenelogic_cfg.mMsgcheckcountlimit = 400;
	mScenelogic_cfg.mMsglimitpersec = 40;
	mScenelogic_cfg.mPingcheck = 1;
	mScenelogic_cfg.mPlayerredirecttimeout = 30;
	mScenelogic_cfg.mPlayersavedbtime = 60;
	mScenelogic_cfg.mTogateheartbeatsec = 20;
	mScenelogic_cfg.mTologstatinfosec = 300;
	mScenelogic_cfg.mToproxyheartbeatsec = 10;
	mScenelogic_cfg.mValidatedataintegrity = 0;

	mScene_log.mAppend = ( bool ) 1;
	memset( mScene_log.mDirectory, 0, sizeof( mScene_log.mDirectory ) );
	strncpy( mScene_log.mDirectory, "../log/sceneserver.log", sizeof( mScene_log.mDirectory ) - 1 );
	memset( mScene_log.mIdentifier, 0, sizeof( mScene_log.mIdentifier ) );
	strncpy( mScene_log.mIdentifier, "sceneserver", sizeof( mScene_log.mIdentifier ) - 1 );
	mScene_log.mMaxbackupindex = 100;
	mScene_log.mMaxfilesize = 10485760;
	mScene_log.mPriority = 700;

	mSpeedchecker_cfg.mMaxabidecountsforclientspeed = 10;
	mSpeedchecker_cfg.mMaxabidecountsforcstimediff = 15;
	mSpeedchecker_cfg.mMaxerrorcountforclientspeed = 100;
	mSpeedchecker_cfg.mMaxtimeerrorforclientspeed = 15;
	mSpeedchecker_cfg.mMaxtimeerrorforcstimediff = 10000;
	mSpeedchecker_cfg.mMintimeerrorforcstimediff = 20;
	mSpeedchecker_cfg.mTimeerrorforcstimediff = 1000;
	mSpeedchecker_cfg.mTimesyncountforclientspeed = 30;
	mSpeedchecker_cfg.mTimesyncountforcstimediff = 1000;
	mSpeedchecker_cfg.mTimesynintervalforcstimediff = 300000;
	mSpeedchecker_cfg.mUsespeedcheckerforclientspeed = ( bool ) 1;
	mSpeedchecker_cfg.mUsespeedcheckerforcstimediff = ( bool ) 1;

	mStall_cfg.mFeeibid = 72803;
	mStall_cfg.mFeerate = 30;
	mStall_cfg.mLevellimit = 30;
	mStall_cfg.mMaxadcontent = 90;
	mStall_cfg.mMaxnamelen = 27;
	mStall_cfg.mMaxprice = 20000000;
	mStall_cfg.mMinybprice = 1;
	mStall_cfg.mSaftytime = 10;
	mStall_cfg.mStallflag = 1;

	mStall_log.mAppend = ( bool ) 1;
	memset( mStall_log.mDirectory, 0, sizeof( mStall_log.mDirectory ) );
	strncpy( mStall_log.mDirectory, "../log/stall.log", sizeof( mStall_log.mDirectory ) - 1 );
	memset( mStall_log.mIdentifier, 0, sizeof( mStall_log.mIdentifier ) );
	strncpy( mStall_log.mIdentifier, "stall", sizeof( mStall_log.mIdentifier ) - 1 );
	mStall_log.mMaxbackupindex = 100;
	mStall_log.mMaxfilesize = 10485760;
	mStall_log.mPriority = 700;

	mStat_log.mAppend = ( bool ) 1;
	memset( mStat_log.mDirectory, 0, sizeof( mStat_log.mDirectory ) );
	strncpy( mStat_log.mDirectory, "../log/stat.log", sizeof( mStat_log.mDirectory ) - 1 );
	memset( mStat_log.mIdentifier, 0, sizeof( mStat_log.mIdentifier ) );
	strncpy( mStat_log.mIdentifier, "stat", sizeof( mStat_log.mIdentifier ) - 1 );
	mStat_log.mMaxbackupindex = 100;
	mStat_log.mMaxfilesize = 10485760;
	mStat_log.mPriority = 700;

	mTasklua_log.mAppend = ( bool ) 1;
	memset( mTasklua_log.mDirectory, 0, sizeof( mTasklua_log.mDirectory ) );
	strncpy( mTasklua_log.mDirectory, "../log/tasklua.log", sizeof( mTasklua_log.mDirectory ) - 1 );
	memset( mTasklua_log.mIdentifier, 0, sizeof( mTasklua_log.mIdentifier ) );
	strncpy( mTasklua_log.mIdentifier, "TASK_LUA", sizeof( mTasklua_log.mIdentifier ) - 1 );
	mTasklua_log.mMaxbackupindex = 60;
	mTasklua_log.mMaxfilesize = 10485760;
	mTasklua_log.mPriority = 700;

	mTask_cfg.mIscanabort = 1;
	mTask_cfg.mLooptasksubnum = 10;
	mTask_cfg.mRefreshstoragetime = 300;
	mTask_cfg.mSearchmaindownlevel = 20;
	mTask_cfg.mSearchmainuplevel = 10;
	mTask_cfg.mSearchxydownlevel = 2;
	mTask_cfg.mSearchxyuplevel = 0;
	mTask_cfg.mSearchzhidownlevel = 5;
	mTask_cfg.mSearchzhiuplevel = 10;

	mTask_log.mAppend = ( bool ) 1;
	memset( mTask_log.mDirectory, 0, sizeof( mTask_log.mDirectory ) );
	strncpy( mTask_log.mDirectory, "../log/task.log", sizeof( mTask_log.mDirectory ) - 1 );
	memset( mTask_log.mIdentifier, 0, sizeof( mTask_log.mIdentifier ) );
	strncpy( mTask_log.mIdentifier, "TASK", sizeof( mTask_log.mIdentifier ) - 1 );
	mTask_log.mMaxbackupindex = 100;
	mTask_log.mMaxfilesize = 10485760;
	mTask_log.mPriority = 700;

	mTeam_cfg.mCooltime = 1;
	mTeam_cfg.mSheetpage = 10;
	mTeam_cfg.mUpdatetime = 3000;
	mTeam_cfg.mValiddistance = 15;

	mTeam_log.mAppend = ( bool ) 1;
	memset( mTeam_log.mDirectory, 0, sizeof( mTeam_log.mDirectory ) );
	strncpy( mTeam_log.mDirectory, "../log/team.log", sizeof( mTeam_log.mDirectory ) - 1 );
	memset( mTeam_log.mIdentifier, 0, sizeof( mTeam_log.mIdentifier ) );
	strncpy( mTeam_log.mIdentifier, "team", sizeof( mTeam_log.mIdentifier ) - 1 );
	mTeam_log.mMaxbackupindex = 100;
	mTeam_log.mMaxfilesize = 10485760;
	mTeam_log.mPriority = 700;

	mTrade_cfg.mSaftytime = 10;

	mTrade_log.mAppend = ( bool ) 1;
	memset( mTrade_log.mDirectory, 0, sizeof( mTrade_log.mDirectory ) );
	strncpy( mTrade_log.mDirectory, "../log/trade.log", sizeof( mTrade_log.mDirectory ) - 1 );
	memset( mTrade_log.mIdentifier, 0, sizeof( mTrade_log.mIdentifier ) );
	strncpy( mTrade_log.mIdentifier, "trade", sizeof( mTrade_log.mIdentifier ) - 1 );
	mTrade_log.mMaxbackupindex = 100;
	mTrade_log.mMaxfilesize = 10485760;
	mTrade_log.mPriority = 700;

	mYqsactivity_log.mAppend = ( bool ) 1;
	memset( mYqsactivity_log.mDirectory, 0, sizeof( mYqsactivity_log.mDirectory ) );
	strncpy( mYqsactivity_log.mDirectory, "../log/YaoQianShu.log", sizeof( mYqsactivity_log.mDirectory ) - 1 );
	memset( mYqsactivity_log.mIdentifier, 0, sizeof( mYqsactivity_log.mIdentifier ) );
	strncpy( mYqsactivity_log.mIdentifier, "YaoQianShu", sizeof( mYqsactivity_log.mIdentifier ) - 1 );
	mYqsactivity_log.mMaxbackupindex = 60;
	mYqsactivity_log.mMaxfilesize = 10485760;
	mYqsactivity_log.mPriority = 700;


}
