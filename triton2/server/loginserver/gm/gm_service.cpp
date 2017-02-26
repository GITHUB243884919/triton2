#include "gm_service.h"
#include "message_pb.hxx.pb.h"
#include "gmmessage_pb.hxx.pb.h"
#include "base.h"
#include "loginobj_manager.h"
#include "db_msg_pb.hxx.pb.h"
#include "template.h"
#include "tinyxml.h"
#include <iostream>
#include <fstream>
/*
 * 消息的相应流程
 * 1. 从GmCmdServer取得 Http协议的内容
 * 2. 按照字段名称，形成消息
 * 3. 第一次分流到网关或者日志服务器
 * 4. 网关接受到消息，如果玩家在世界，就发向游戏Server；
 *    如果玩家不在游戏世界，就要调用ROLE接口，操作数据库. 这个地方需要加锁，防止玩家此时登录; 
 *    或者按照玩家的分到特定的线程上, 并保证执行顺序不发生变化
 * 5. 等待执行结果，将googlebuffer形成xml，返回给GmCmdServer
 *    如果超时，也要返回结果
 * 6. 如果此时链接断开，删除session, 消息返回，找不到session，报错
 */

template<> CGmServer* CSingleton< CGmServer >::spSingleton = NULL;
CGmServer::CGmServer()
{
	IniAvailableGmCmd();

	for (int i = 0; i < MAX_SOCK_NUM+1; ++i)
	{
		mSocks[i].fd = -1;	
		mSocks[i].pactor = NULL;	
	}

	maxfd = 0;
	m_tpTempBuffer =NULL;

	mNpcDict.initailize();
	mMapDict.initailize();
    LoadCityIpfile();
	
	LoadMapNpc();

	LoadProtoHelper();

	LoadToken2String();
 
	mUmsFamilyFields=
		"family_id,family_name,family_money,family_status,create_role_id,create_role_name,"
		"country_id,corps_id,npc_id,bid_money,bid_time,disband_time,war_rank,war_winnum,war_lostnum,"
		"family_starlevel,war_exp,family_glory,war_status,family_npcmoney,family_npcglory,family_npcwelfaretime,"
		"family_level,family_creattime,family_repetionopendate,family_repetionopentimes,family_repetionscore,family_shopinfo";

	mUmsCorpsFields=
		"corps_id,corps_name,create_role_id,create_role_name,corps_level,corps_money,"
		"country_id,disband_time,corps_status,corps_glory,war_winnum,war_lostnum,war_score,war_starlevel,"
		"war_bindnpcid,war_bindmoney,war_battlestatus,war_campid,war_battlecity,war_battlekey,"
		"war_welfaregettime,corps_roommoney,corps_creattime,corps_color,family_info,corps_shopinfo";

	mUmsUnionFields=
		"union_head,union_name,corps0,corps1,corps2,corps3,corps4,corps5,corps6,corps7,corps8";


	mUmsWorldFields=
		"world_id,country_id,king_power,people_num,version,"
		"wizard0,wizard1,wizard2,wizard3,wizard4,wizard5,"
		"convalue0+conwizard0,convalue1+conwizard1,convalue2+conwizard2,"
		"convalue3+conwizard3,convalue4+conwizard4,convalue5+conwizard5,"
		"conlevel0,conlevel1,conlevel2,conlevel3,conlevel4,conlevel5";

	CDataStatic::Initialize();
	LOG_INFO("gm", "Template Ver %d", CDataStatic::sTemplateVersion );

}


CGmServer::~CGmServer()
{
	CDataStatic::Finialize();
}

void CGmServer::ReloadCfg()
{
	mNpcDict.clear();
	mMapDict.clear();

	mToken2String.clear();
	mHelperDict.clear();
	//ClearIpData();
	m_ipDatalist.clear();

	CDataStatic::Finialize();

	CDataStatic::Initialize();
	LOG_INFO("gm", "Template Ver %d", CDataStatic::sTemplateVersion );

	LoadMapNpc();
    LoadCityIpfile();
	LoadProtoHelper();

	LoadToken2String();
}

const char* CGmServer::Label(const char* proto, const char* field, const char** ppDomain, int* pFormat)
{
	static lk::CString64 strKey("");
	strKey.clear();

	strKey.append("%s.%s", proto, field);	

	HELPER_DICT::iterator iter = mHelperDict.find(strKey);

	if ( ppDomain != NULL)
	{
		*ppDomain = (iter == mHelperDict.end() ? NULL : 
			(iter->second.domain.length() == 0 ? NULL : iter->second.domain.c_str()));
	}

	if ( pFormat != NULL)
	{
		*pFormat = (iter == mHelperDict.end() ? 0 : iter->second.format); 
	}

	return iter == mHelperDict.end() ? strKey.c_str() : iter->second.label.c_str();
}

const char* CGmServer::Label(const char* proto, int nEnumID)
{
	static lk::CString64 strKey("");
	strKey.clear();

	if (strcmp(proto, "EValue") == 0)
	{
		strKey.append("%d", nEnumID);
		return strKey.c_str();
	}else
	if (strcmp(proto, "EItemID2Name") == 0)
	{
		return ItemID2Name(nEnumID);
	}else
	if ( strcmp(proto, "ETaskID2Name") == 0)
	{
		return TaskID2Name(nEnumID);
	}else
	if (strcmp(proto, "ENpcID2Name") == 0)
	{
		return NpcID2Name(nEnumID);
	}

	strKey.append("%s.%03d", proto, nEnumID);

	HELPER_DICT::iterator iter = mHelperDict.find(strKey);
	return iter == mHelperDict.end() ? strKey.c_str() : iter->second.label.c_str();
}

void CGmServer::LoadToken2String()
{
	/*const char* szFileList [] = 
	{
		"../config/data/template/langTemp.xml",
		"../config/data/template/langTask.xml",
		NULL
	};*/
	char langTemp[ CONFIG_STR_LEN ] = { 0 }; 
	char langTask[ CONFIG_STR_LEN ] = { 0 };
	CLoginLogic::GetSingleton().GetConfigDataFileNamePath(CLoginLogic::GetSingletonPtr()->GetGameID(),"template/langTemp.xml", langTemp, CONFIG_STR_LEN);
	CLoginLogic::GetSingleton().GetConfigDataFileNamePath(CLoginLogic::GetSingletonPtr()->GetGameID(),"template/langTask.xml", langTask, CONFIG_STR_LEN);
	const char* szFileList [] = 
	{
		langTemp,
		langTask,
		NULL
	};
	const char** pathname = szFileList;
	for( ;*pathname != NULL; pathname ++)
	{
		TiXmlDocument tTplDoc;
		tTplDoc.LoadFile( *pathname );

		TiXmlElement* tpRoot = tTplDoc.FirstChildElement("text_list");
		if( tpRoot == NULL)
		{
			LOG_ERROR( "map", "No Root Element Found in file(%s)", *pathname );
			return;
		}

		TiXmlElement* pGroupElem = tpRoot->FirstChildElement( );
		for ( ; pGroupElem != NULL; pGroupElem = pGroupElem->NextSiblingElement( ) )
		{

			TiXmlElement* pIDElem = pGroupElem->FirstChildElement( );
			for ( ; pIDElem != NULL; pIDElem = pIDElem->NextSiblingElement( ) )
			{
				std::string tEleName = pIDElem->Value( );
				if ( tEleName != "text" )
				{
					LOG_ERROR("gm", "Element (%s) is not text context", tEleName.c_str());
					continue;
				}

				const char* szToken  = pIDElem->Attribute( "id" );
				const char* szString = pIDElem->Attribute( "value" );
				if ( !szToken || !szString || strlen( szToken) >= 32 )
				{
					//LOG_ERROR("gm", "Element(%s:%s) text attr error!", szToken, szString);
					continue;
				}

				std::pair<TOKEN2STRING::iterator, bool> ret = 
					mToken2String.insert(TOKEN2STRING::value_type(lk::CString32(szToken), CString256(szString)));

				if (ret.second != true)
				{
					//LOG_ERROR("gm", "mToken2String insert text (%s, %s) error", szToken, szString);
				}else
				{
					//LOG_DEBUG("gm", "mToken2String insert text (%s, %s) ok", szToken, szString);
				}
			}
		}
	}
}

void CGmServer::LoadMapNpc()
{
	DIR* dir;
	char map[ CONFIG_STR_LEN ] = { 0 };
	CLoginLogic::GetSingleton().GetConfigDataFileNamePath(CLoginLogic::GetSingletonPtr()->GetGameID(),"map/", map, CONFIG_STR_LEN );
	if ((dir = opendir (map)) == NULL)
	{
		extern int errno;
		LOG_ERROR("gm", "opendir ../config/data/map/ error for error(%d:%s)", errno, strerror(errno));
		return;
	}

	struct dirent *dp;
	while((dp = readdir (dir)) != NULL)
	{
		if (dp->d_name[0] == '.') continue;

		char stfile[56] ={0};
		::snprintf(stfile,sizeof(stfile) -1,"map/%s", dp->d_name );
		char pathname[ 128 ] = { 0 };
		char tmppath[ CONFIG_STR_LEN ] = { 0 };
		CLoginLogic::GetSingleton().GetConfigDataFileNamePath(CLoginLogic::GetSingletonPtr()->GetGameID(),stfile, tmppath, CONFIG_STR_LEN );
		::snprintf( pathname, sizeof(pathname)-1, tmppath);

	/*	char pathname[256];
		snprintf(pathname, sizeof(pathname), "../config/data/map/%s", dp->d_name);*/


		TiXmlDocument tTplDoc;
		tTplDoc.LoadFile( pathname );

		TiXmlElement* tpRoot = tTplDoc.FirstChildElement("Scene_Conf");
		if( tpRoot == NULL)
		{
			LOG_ERROR( "map", "No Root Element Found in file(%s)", pathname );
			continue;
		}

		TiXmlElement* tpElement = tpRoot->FirstChildElement( );
		for ( ; tpElement != NULL; tpElement = tpElement->NextSiblingElement( ) )
		{
			std::string tEleName = tpElement->Value( );
			if ( tEleName != "Map" )
			{
				LOG_ERROR("gm", "Element (%s) is not Map context", tEleName.c_str());
				continue;
			}

			const char* tpMapName   = tpElement->Attribute( "MapName" );
			const char* tpMapID     = tpElement->Attribute( "MapID" );
			if ( tpMapName == NULL || tpMapID == NULL || ::strlen( tpMapName ) >= 32 )
			{
				LOG_ERROR("gm", "Element(%s:%s) Map attr error!", tpMapID, tpMapName);
				continue;
			}

			//insert into map_define object
			TMapDefine tMapDefine;
			tMapDefine.map_id = atoi(tpMapID);
			strncpy(tMapDefine.map_name, tpMapName, NAME_LENGTH);
			tMapDefine.map_name[NAME_LENGTH] = 0;

			LOG_DEBUG("gm", "load map_define(%d, %s)", tMapDefine.map_id, tMapDefine.map_name);
			mMapDict.insert(MAP_DICT::value_type(tMapDefine.map_id, tMapDefine));

			//insert into npc_define object
			TiXmlElement* tpMapEle = tpElement->FirstChildElement();
			for (; tpMapEle != NULL; tpMapEle =	tpMapEle->NextSiblingElement())	
			{
				std::string tEleName1 = tpMapEle->Value();
				if ( tEleName1 == "NpcData" )
				{
					TiXmlElement* tpNpcEle = tpMapEle->FirstChildElement();
					for( ; tpNpcEle != NULL; tpNpcEle = tpNpcEle->NextSiblingElement() )
					{
						std::string tEleName2 = tpNpcEle->Value( );
						if ( tEleName2 == "Npc" )
						{
							TNpcDefine tNpcDefine;
							const char* tpNpcID = tpNpcEle->Attribute("NpcID");
							const char* tpNpcName = tpNpcEle->Attribute("NpcName");

							tNpcDefine.npc_id = atoi(tpNpcID);
							strncpy(tNpcDefine.npc_name, tpNpcName, NAME_LENGTH);
							tNpcDefine.npc_name[NAME_LENGTH] = 0;

							TiXmlElement* tpPosEle = tpNpcEle->FirstChildElement( );
							for( ; tpPosEle != NULL; tpPosEle = tpPosEle->NextSiblingElement( ) )                  
							{                                                                                      
								std::string tEleName3 = tpPosEle->Value( );                                         
								if ( tEleName3 == "Pos" )                                                           
								{                                                                                  
									const char* tpX     = tpPosEle->Attribute( "x" );                              
									const char* tpY     = tpPosEle->Attribute( "y" );  

									tNpcDefine.x = atoi(tpX);
									tNpcDefine.y = atoi(tpY);
								}
							}

							tNpcDefine.map_id = tMapDefine.map_id;

							if (tpNpcID == NULL || tpNpcName == NULL || tNpcDefine.x == 0 || tNpcDefine.y == 0) 
							{
								LOG_ERROR("gm", "load npc_define(%d, %s@(%d,[%d,%d]))", 
										tNpcDefine.npc_id, tNpcDefine.npc_name, 
										tNpcDefine.map_id, tNpcDefine.x, tNpcDefine.y);
								continue;
							}

							LOG_DEBUG("gm", "load npc_define(%d, %s@(%d,[%d,%d]))", 
									tNpcDefine.npc_id, tNpcDefine.npc_name, 
									tNpcDefine.map_id, tNpcDefine.x, tNpcDefine.y);

							std::pair<NPC_DICT::iterator, bool> ret = 
								mNpcDict.insert(NPC_DICT::value_type(tNpcDefine.npc_name, tNpcDefine));

							if (ret.second != true)
							{
								LOG_ERROR("gm", "mNpcDict insert (%d, %s@(%d,[%d,%d]) error", 
										tNpcDefine.npc_id, tNpcDefine.npc_name, 
										tNpcDefine.map_id, tNpcDefine.x, tNpcDefine.y);
							}
						}
					}
				}
			}
		}
	}

	if(dir) closedir(dir); dir = NULL;
}
void CGmServer::ClearIpData()
{
	for (int i=0;i<m_ipDatalist.size();i++)
	{
		if (m_ipDatalist[i])
		{
			//free(m_ipDatalist[i]);
			m_ipDatalist[i] = NULL;
		}
		
	}
	

}
void CGmServer::AddIpData(int nID, unsigned short * pcProvince,int prolen, unsigned short * pcCity,int citylen, unsigned int  nStartIp, unsigned int  nEndIp)
{
	CIpData* pIpData = (CIpData*)malloc(sizeof(CIpData));
	if (pIpData == NULL)
	{
		return ;
	}
	pIpData->Init();
	pIpData->SetID(nID);
	pIpData->m_nProvinceLen = prolen;
	pIpData->SetProvince(pcProvince);
	pIpData->m_nCityLen = citylen;
	pIpData->SetCity(pcCity);
	pIpData->SetStartIp(nStartIp);
	pIpData->SetEndIp(nEndIp);
	m_ipDatalist.push_back(pIpData);


}

 const unsigned short * CGmServer::GetProvince(int nstartIP)
{
	for(int i = 0 ;i< m_ipDatalist.size();i++)
	{
		if(m_ipDatalist[i]->GetStartIp() <= nstartIP && nstartIP <= m_ipDatalist[i]->GetEndIp() )
		{
			return m_ipDatalist[i]->GetCity();
		}
	}
	return NULL;
}
 const int CGmServer::GetCityLen(int nstartIP)
 {
	 for(int i = 0 ;i< m_ipDatalist.size();i++)
	 {
		 if(m_ipDatalist[i]->GetStartIp() <= nstartIP && nstartIP <= m_ipDatalist[i]->GetEndIp() )
		 {
			 return m_ipDatalist[i]->m_nCityLen;
		 }
	 }
	 return 0;
 
 }
void CGmServer::LoadCityIpfile()
{
	FILE *fpChnFile = NULL;
	fpChnFile = fopen( "../config/data/template/ipData.dat", "r" ); 
	if( !fpChnFile )	 
	{ 
		return ;	
	}			   
	if (m_tpTempBuffer)
	{
		m_tpTempBuffer = NULL;
	}
	if( fpChnFile )
	{
		fseek( fpChnFile, 0L, SEEK_END );
		long lFileLen = ftell( fpChnFile );
		m_tpTempBuffer = (BYTE *)new BYTE [lFileLen+1];
		fseek( fpChnFile, 0L, SEEK_SET );
		fread( m_tpTempBuffer, lFileLen , 1, fpChnFile  );
		//LOG_NOTICE( "default", "read key %s from file %s", szKey, pFile );
		fclose( fpChnFile );
		int		nID = 0;
		unsigned short	acProvince[32] = {0};
		unsigned short	acCity[32] = {0};
		int		nStartIp = 0;
		int		nEndIp = 0;
		int		nProvinceLen = 0;
		int		nCityLen = 0;

		int		tLen = 0;
		int		nIntSize = 4;
		int     nTcharSize =2;
		int		nStrSize = 0;
		while( tLen < lFileLen)
		{
			//memset(acProvince, 0, sizeof(acProvince));
			//memset(acCity, 0, sizeof(acCity));
			acProvince[0] = L'\0';;
			acCity[0] =L'\0';
			memcpy(&nID, m_tpTempBuffer, nIntSize);
			m_tpTempBuffer += nIntSize;
			tLen +=  nIntSize;
			if (m_tpTempBuffer == NULL)
			{
				break;
			}
			memcpy(&nProvinceLen, m_tpTempBuffer, nIntSize);
			m_tpTempBuffer += nIntSize;
			tLen +=  nIntSize;
			nStrSize = nProvinceLen * nTcharSize;
			if (m_tpTempBuffer == NULL)
			{
				break;
			}
			memcpy(acProvince,m_tpTempBuffer,nStrSize);

			//setlocale(LC_ALL,"zh_CN.GBK");
			//mbstowcs(acProvince,(char*)m_tpTempBuffer,nStrSize);
			//setlocale(LC_ALL,"zh_CN.GBK");

			//mbstowcs(acProvince,nProvice,sizeof(nProvice)/2+1);

			//char * aa = {"您好"};
			//wchar_t  bb[2] = {0};
			//bb[0] =L'\0';
			//setlocale(LC_ALL,"zh_CN.GBK");
			//mbstowcs(bb,aa,sizeof(aa)/2+1);
			/*int wlen =  wcslen(acProvince);
			char *pp =  new char[2*wlen +1];
			pp[0] = '\0';
			setlocale(LC_ALL,"zh_CN.GBK");
			wcstombs(pp,acProvince,2*wlen+1);*/
			//printf(pp);
			//memcpy(acProvince, m_tpTempBuffer, nStrSize);
			m_tpTempBuffer += nStrSize;
			tLen +=  nStrSize;
			if (m_tpTempBuffer == NULL)
			{
				break;
			}
			memcpy(&nCityLen, m_tpTempBuffer, nIntSize);
			m_tpTempBuffer += nIntSize;
			tLen +=  nIntSize;
			nStrSize = nCityLen * nTcharSize;
			if (m_tpTempBuffer == NULL)
			{
				break;
			}
			/*setlocale(LC_ALL,"zh_CN.GBK");
			mbstowcs(acCity,(char*)m_tpTempBuffer,nStrSize);
			int wlen =  wcslen(acCity);
			char *pp =  new char[2*wlen +1];
			pp[0] = '\0';
			setlocale(LC_ALL,"zh_CN.GBK");
			wcstombs(pp,acCity,2*wlen+1);
			printf(pp);*/
			memcpy(acCity,m_tpTempBuffer, nStrSize);
			m_tpTempBuffer += nStrSize;
			tLen +=nStrSize;
			if (m_tpTempBuffer == NULL)
			{
				break;
			}
			memcpy(&nStartIp, m_tpTempBuffer, nIntSize);
			m_tpTempBuffer += nIntSize;
			tLen +=  nIntSize;
			if (m_tpTempBuffer == NULL)
			{
				break;
			}
			memcpy(&nEndIp, m_tpTempBuffer, nIntSize);
			m_tpTempBuffer += nIntSize;
			tLen +=  nIntSize;
			AddIpData(nID, acProvince,nProvinceLen, acCity,nCityLen, nStartIp, nEndIp);

		}
	}

}

void CGmServer::LoadProtoHelper()
{
	char pathname[ CONFIG_STR_LEN ] = { 0 };
	CLoginLogic::GetSingleton().GetConfigDataFileNamePath(CLoginLogic::GetSingletonPtr()->GetGameID(),"login_config/protohelper.xml", pathname, CONFIG_STR_LEN);

	TiXmlDocument tTplDoc;
	tTplDoc.LoadFile( pathname );

	TiXmlElement* tpRoot = tTplDoc.FirstChildElement("helper");
	if( tpRoot == NULL)
	{
		LOG_ERROR( "map", "No Root Element Found in file(%s)", pathname );
		return;
	}

	TiXmlElement* pProtoElem = tpRoot->FirstChildElement( );
	for ( ; pProtoElem != NULL; pProtoElem = pProtoElem->NextSiblingElement( ) )
	{
		std::string tEleName = pProtoElem->Value( );
		if ( tEleName != "proto" && tEleName != "enum" )
		{
			LOG_ERROR("gm", "Element (%s) is not proto context", tEleName.c_str());
			continue;
		}

		const char* szProtoName = pProtoElem->Attribute( "name" );
		const char* szProtoID = pProtoElem->Attribute( "id" );
		if ( !szProtoName || !szProtoID || strlen( szProtoName ) >= 32 )
		{
			LOG_ERROR("gm", "Element(%s:%s) proto attr error!", szProtoID, szProtoName);
			continue;
		}

		int nProtoID = atoi(szProtoID);
		lk::CString32 strProtoName = szProtoName;

		TiXmlElement* pFieldElem = pProtoElem->FirstChildElement();
		for (; pFieldElem != NULL; pFieldElem =	pFieldElem->NextSiblingElement())	
		{
			std::string tSubEleName = pFieldElem->Value();
			if ( tSubEleName != "field" ) continue;

			const char* szFieldName = pFieldElem->Attribute( "name" );
			const char* szFieldID = pFieldElem->Attribute( "id" );
			const char* szFieldLabel = pFieldElem->Attribute("label");
			const char* szFieldFormat = pFieldElem->Attribute("format");
			const char* szFieldDomain = pFieldElem->Attribute("domain");

			if ( !szFieldName || !szFieldID || !szFieldLabel || strlen( szProtoName ) >= 32 )
			{
				LOG_ERROR("gm", "Element(%s,%s,%s) field attr error!", szProtoID, szProtoName, szFieldLabel);
				continue;
			}

			TFieldHelper helper;
			helper.id = atoi(szFieldID);
			helper.name = szFieldName;
			helper.label = szFieldLabel;
			helper.format = szFieldFormat ? atoi(szFieldFormat) : 0;
			helper.domain = szFieldDomain ? szFieldDomain : "";

			std::pair<HELPER_DICT::iterator, bool> ret;
			if ( tEleName == "proto")
			{
				lk::CString64 strKey("");
				strKey.append("%s.%s", szProtoName, szFieldName);

				ret = mHelperDict.insert(HELPER_DICT::value_type(strKey, helper));
			}else
			{
				lk::CString64 strKey("");
				strKey.append("%s.%03d", szProtoName, helper.id);

				//enum
				ret = mHelperDict.insert(HELPER_DICT::value_type(strKey, helper));
			}

			if (ret.second != true)
			{
				LOG_ERROR("gm", "mHelperDict insert proto {%d, %s field {%d, %s, %s}} error", 
						nProtoID, szProtoName, helper.id, szFieldName, szFieldLabel );
			}else
			{
				LOG_DEBUG("gm", "mHelperDict insert proto {%d, %s field {%d, %s, %s}} ok", 
						nProtoID, szProtoName, helper.id, szFieldName, szFieldLabel );
			}
		}
	}
}

void CGmServer::IniAvailableGmCmd()
{
	mGmCmdList["KickRole"] = ID_M2G_KICKROLE_CMD;
	mGmCmdList["MoveRole"] = ID_M2G_MOVEROLE_CMD;
	mGmCmdList["ShutUp"] =   ID_M2G_SHUTUP_CMD;
	mGmCmdList["CancelShutUp"] = ID_M2G_CANCLESHUTUP_CMD;  
	mGmCmdList["UserSocialInfo"] = ID_M2G_USERSOCIALINFO_CMD;  


	//TO BE DELETED
	mGmCmdList["RoleDetail"] = ID_M2G_ROLEDETAIL_CMD;
	//mGmCmdList["RoleDetail"] = ID_M2G_ROLESKILLDETAIL_CMD;

	mGmCmdList["UserStorageInfo"] = ID_M2G_USERSTORAGEINFO_CMD;
	mGmCmdList["RoleLocation"] = ID_M2G_ROLELOCATION_CMD;
	mGmCmdList["NPCDetail"] = ID_M2G_NPCDETAIL_CMD;
	mGmCmdList["TaskInfo"] = ID_M2G_TASKINFO_CMD;
	mGmCmdList["DeleteRole"] = ID_M2G_DELETEROLE_CMD;
	mGmCmdList["FreezeAccount"] = ID_M2G_FROZENACCOUNT_CMD;
	mGmCmdList["UnfreezeAccount"] = ID_M2G_UNFROZENACCOUNT_CMD;
	mGmCmdList["FreezeMac"] = ID_M2G_FROZENMAC_CMD;
	mGmCmdList["UnfreezeMac"] = ID_M2G_UNFROZENMAC_CMD;
	mGmCmdList["TrusteeUser"] = ID_M2G_TRUSTEEUSER_CMD;
	mGmCmdList["UnTrusteeUser"] = ID_M2G_UNTRUSTEEUSER_CMD;
	mGmCmdList["RoleProto"] = ID_M2G_ROLEPROTO_CMD;
	mGmCmdList["EndTask"] = ID_M2G_ENDTASK_CMD;

	mGmCmdList["EndTask"] = ID_M2G_ENDTASK_CMD;
	mGmCmdList["TrusteeStorage"] = ID_M2G_TRUSTEESTORAGE_CMD;
	mGmCmdList["UntrusteeStorage"] = ID_M2G_UNTRUSTEESTORAGE_CMD;
	mGmCmdList["RecoverRole"] = ID_M2G_RECOVERROLE_CMD;
	mGmCmdList["PostBulletin"] = ID_M2G_POSTBULLETIN_CMD;
	mGmCmdList["MultiExp"] = ID_M2G_MULTIEXP_CMD;
	mGmCmdList["CancelMultiExp"] = ID_M2G_CANCELMULTIEXP_CMD;
	mGmCmdList["ChangeMoney"] = ID_M2G_CHANGEMONEY_CMD;
	mGmCmdList["ChangeExp"] = ID_M2G_CHANGEEXP_CMD;
	mGmCmdList["ChangeItem"] = ID_M2G_CHANGEITEM_CMD;
	mGmCmdList["ClearBag"] = ID_M2G_CLEARBAG_CMD;

	mGmCmdList["IbInfo"] = ID_M2G_IBINFO_CMD;
	mGmCmdList["SetIbPrice"] = ID_M2G_SETIBPRICE_CMD;
	mGmCmdList["SetIbLocation"] = ID_M2G_SETIBLOCATION_CMD;
	mGmCmdList["IbOnSale"] = ID_M2G_IBONSALE_CMD;
	mGmCmdList["IbNotSale"] = ID_M2G_IBNOTSALE_CMD;
	mGmCmdList["IbDescript"] = ID_M2G_IBDESCRIPT_CMD;
	mGmCmdList["IpBind"] = ID_M2G_IPBIND_CMD;

	//2.4 版本新增
	mGmCmdList["UserItemOnBody"] = ID_M2G_ROLEEQUIPMENT_CMD;
	mGmCmdList["UserPackInfo"] = ID_M2G_ROLEPACKINFO_CMD;
	mGmCmdList["UserItemInPack"] = ID_M2G_ROLEITEMINPACK_CMD;
	mGmCmdList["UserSkillInfo"] = ID_M2G_ROLESKILLINFO_CMD;
	mGmCmdList["UserSkillDetail"] = ID_M2G_ROLESKILLDETAIL_CMD;

	//mGmCmdList["SetOLGMAccount"] = ID_M2G_IBINFO_CMD;
	
	// 2009-06-03 新增日志查询类
	mGmCmdList["GoodsHistoryToNPC"] = ID_M2G_GOODSHISTORYTONPC_CMD;
	mGmCmdList["GoodsHistoryFromNPC"] = ID_M2G_GOODSHISTORYFROMNPC_CMD;
	mGmCmdList["GoodsHistoryBetweenUsers"] = ID_M2G_GOODSHISTORYBETWEENUSERS_CMD;
	mGmCmdList["GoodsHistory"] = ID_M2G_GOODSHISTORY_CMD;
	mGmCmdList["OperationDetail"] = ID_M2G_OPERATIONDETAIL_CMD;

	//2009/08/12 新增接口
	mGmCmdList["UserItemOnBodyDetail"] = ID_M2G_ROLEEQUIPMENT_DETAIL_CMD;
	mGmCmdList["UserItemInStorage"] = ID_M2G_ROLESTORAGE_DETAIL_CMD;
	
	//2010/02/02 新增接口
	mGmCmdList["ResetRoleStoragePassword"] = ID_S2G_REQUEST_RESETROLEPASSWORD;	

	//2010/02/05 新增查询类接口
	mGmCmdList["RoleItemOperate"] = ID_M2G_REQUEST_ROLEITEMOPERATE;	
	mGmCmdList["RoleMoneyOperate"] = ID_M2G_ROLEMONEYOPERATE;	
	mGmCmdList["RoleExp"] = ID_M2G_REQUEST_ROLEEXP;	
	mGmCmdList["RoleItemLevelUp"] = ID_M2G_REQUEST_ROLEITEMLEVELUP;	

	// 新增批量冻结接口 [3/10/2010 GaoHong]
	mGmCmdList["FastFreezeAccount"] = ID_M2G_REQUEST_FASTFREEZE_CMD;
	mGmCmdList["FastUnfreezeAccount"] = ID_M2G_REQUEST_UNFASTFREEZE_CMD;

	// 新增军团家族查询接口
	mGmCmdList["CorpsTotal2"] = ID_M2G_REQUEST_CORPSTOTAL_CMD;
	mGmCmdList["CorpsDetail2"] = ID_M2G_REQUEST_CORPSDETAIL_CMD;

	mGmCmdList["CorpsInfo"] = ID_M2G_REQUEST_CORPSINFO_CMD;
	mGmCmdList["RoleInfo"] = ID_M2G_REQUEST_ROLEINFO_CMD;

	// 新增装备回复查询接口
	mGmCmdList["RecoverRoleItem"] = ID_M2G_ROLE_ITEM_RECOVERY_CMD;

	// 查询玩家邮件接口
	mGmCmdList["RoleMail"] = ID_M2G_QUERY_ROLE_MAIL_CMD;

	// 查询玩家已经完成的任务
	mGmCmdList["FinishedTaskInfo"] = ID_M2G_FINISHED_TASKINFO_CMD;

	// 查询玩家称号
	mGmCmdList["QueryRoleTitle"] = ID_M2G_QUERY_ROLETITLE_CMD;
	mGmCmdList["UserLevelInfo"] = ID_M2G_USERLEVELINFO_CMD;

	// 根据IP查询在线离线玩家
	mGmCmdList["UserInfoByIp"] = ID_M2G_USERINFOBYIP_CMD;
	
    // 发送系统邮件
    mGmCmdList["SendMail"]  = ID_G2S_NOTIFY_SENDSYSMAILTOPLAYER;
}

int CGmServer::GetGmMessageID(const char* szCmdName)
{
	//TODO: 按照最常用的在前排序，或者做字符串查找
	CMDTYPEDICT::const_iterator iter4type = mGmCmdList.find(szCmdName);

	if (iter4type == mGmCmdList.end())
	{
		LOG_ERROR("gm", "Message (%s) does not be registed!", szCmdName);
		return -1;
	}

	return iter4type->second;
}

// 创建InfoSever服务器, 内网链接
bool CGmServer::CreateGmCmdServer(const char* szIpPort)
{
	unsigned short tPort = (unsigned short) atoi(szIpPort);

	if ( mGmCmdServer.CreateServer(tPort, NULL) != 0)
	{
		LOG_ERROR("gm", "gm server create server failure, maybe ip error.");
		return false;
	}

	int fd = mGmCmdServer.GetSocketFD(); 
	if ( fd > MAX_SOCK_NUM ) 
	{
		LOG_ERROR("gm", "sockfd(%d) is larger than the programe permited(%d)", fd, MAX_SOCK_NUM);
		mGmCmdServer.Close();
		return false;
	}

	if (fd > maxfd) maxfd = fd;
	mSocks[fd].fd = fd;
	mSocks[fd].create_ts = time(0);
	mSocks[fd].from = FD_IS_LISTENER;
	mSocks[fd].addr = htonl(INADDR_ANY);
	mSocks[fd].port = tPort; 

	return true;
}

// 处理新的Gate连接
void CGmServer::OnGmCmdServerAccept(int fd)
{
	// 取新的链接fd， 并将其写入mSocks		
	struct sockaddr_in cnt_addr;
	socklen_t cnt_len = sizeof(struct sockaddr);

	int cnt = accept(fd, (struct sockaddr *)&cnt_addr, (socklen_t *)&cnt_len);	
	if (cnt > 0)
	{
		if (cnt > MAX_SOCK_NUM) 
		{
			LOG_ERROR("gm", "sockfd(%d) is larger than the programe permited (%d)", cnt, MAX_SOCK_NUM);

			close(cnt);
			return;
		}

		// 是否在黑名单IP中	
		if ( CLoginServer::GetSingleton().IsBlackIP(cnt_addr.sin_addr.s_addr)) 
		{
			const unsigned char* p = (const unsigned char*) &(cnt_addr.sin_addr.s_addr);
			LOG_INFO("gm", "%d.%d.%d.%d is in black ip list. it will be blocked", *p, *(p+1), *(p+2), *(p+3));

			close(cnt);
			return;
		}

		// 首先尝试分配内存, 失败则关闭
		try
		{
			mSocks[cnt].pactor = new CTCPSocket<RECVBUFLENGTH, POSTBUFLENGTH>;
		}catch(...)
		{
			mSocks[cnt].pactor = NULL;
			close(cnt);

			LOG_ERROR("gm", "new CTCPSocket<RECVBUFLENGTH, POSTBUFLENGTH> for fd(%d) error", cnt);
			return;
		}

		mSocks[cnt].fd = cnt;
		mSocks[cnt].create_ts = time(0);	
		mSocks[cnt].from = FD_FROM_GMSERVER;
		mSocks[cnt].addr = cnt_addr.sin_addr.s_addr;
		mSocks[cnt].port = cnt_addr.sin_port;

		mSocks[cnt].pactor->Accept(cnt);

		if(cnt > maxfd ) maxfd = cnt;

		// 设置成NOBLOCK
		int flags = 1;
		if ((flags = fcntl(cnt, F_GETFL, 0)) < 0 || fcntl(cnt, F_SETFL, flags | O_NONBLOCK) < 0)
		{
			LOG_ERROR("gm","set nonblock on socket %d error!", cnt);

			//close(cnt); mSocks[cnt].fd = -1;
			UnRegistGateSockfd(cnt);
		}else
		{
			unsigned char *p = (unsigned char *) &(mSocks[cnt].addr); 
			LOG_INFO("gm", "client[%d.%d.%d.%d:%d]", *p, *(p+1), *(p+2), *(p+3), mSocks[cnt].port);
		}
	}else
	{
		unsigned char *p = (unsigned char *) &(cnt_addr.sin_addr.s_addr);

		LOG_INFO("gm", "accept(fd=%d) error on %d.%d.%d.%d:%d for error(%d:%s)", 
				fd, *p, *(p+1), *(p+2), *(p+3), cnt_addr.sin_port, errno, strerror(errno));
	}
}
// 接受从服务器过来的消息， 包括ERating, 各个GateServer, 以及侦听GateServer的链接
void CGmServer::RecvGmCmdServerMsg(int vMsgCount)
{
	//int fd_ndx = -1;
	fd_set fds_read;

	// 取得当前可以读的端口的数量
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 100000;

	FD_ZERO( &fds_read );

	for (int i=0; i<maxfd+1; ++i)
	{
		if ( mSocks[i].fd > 0 )//&& (mSocks[i].pactor->GetStatus() == tcs_connected  || mSocks[i].from == FD_IS_LISTENER))
		{
			FD_SET(mSocks[i].fd, &fds_read);
		}
	}

	int n= select( maxfd + 1, &fds_read, NULL, NULL, &timeout);
	if (n < 0 && errno != EINTR)
	{
		LOG_ERROR("gm", "%s:%d, fdevent_poll failed:",  __FILE__, __LINE__, strerror(errno));
		return;
	}

	for (int i = 0; i<maxfd+1; ++i)
	{
		if ( mSocks[i].fd > 0 && FD_ISSET(mSocks[i].fd, &fds_read))
		{
			switch(mSocks[i].from)
			{
				case FD_IS_LISTENER:
				{
					OnGmCmdServerAccept(mSocks[i].fd);
					break;
				}
	
				case FD_FROM_GMSERVER:
				{

					//char gbk_buffer[ MAX_CODE_LEN ] = {0};
					char utf8_buffer[ MAX_CODE_LEN ] = {0};

					//unsigned int gbk_length = sizeof( gbk_buffer );
					int utf8_length = sizeof(utf8_buffer);

					int iRet = -1;

					if ( mSocks[i].pactor == NULL || mSocks[i].fd == -1) 
					{
						continue;
					}

					int r = mSocks[i].pactor->RecvData();
					if (r == ERR_RECV_REMOTE || r == ERR_RECV_FALIED)
					{
						LOG_ERROR("gm", "gm server recv data error.");

						UnRegistGateSockfd(mSocks[i].fd);
						return;
					}

					iRet = mSocks[i].pactor->GetOneHttpCode(utf8_length, (BYTE*) utf8_buffer);
					if ( iRet <= 0 )
					{
						if(mSocks[i].pactor->GetStatus() == tcs_closed);
						{
							UnRegistGateSockfd(mSocks[i].fd);
						}

						return;
					}

					#ifdef _GMDEBUG_
					LOG_DEBUG("gm", "Recv From GMServer(%d)\n%s", utf8_length, utf8_buffer);
					#endif

					//删除message_buffer的尾部换行
					for(char* p = utf8_buffer + utf8_length - 1; p != NULL && *p == '\n' && p >= utf8_buffer; --p) *p = 0;

					//代码转换utf8 -> gbk
					/*
					if (code_convert("utf-8", "gbk", utf8_buffer, (size_t) utf8_length, gbk_buffer, &gbk_length) < 0)
					{
						LOG_ERROR("default", "Function %s convert utf8(len=%d) to gbk failure.",
								__FUNCTION__, utf8_length);
					}
					*/


					CMessage tMessage;
					if(DecodeFromGMStream(mSocks[i].fd, utf8_buffer, &tMessage) == 0)
					{
						OnGMServerMessage(mSocks[i].fd, &tMessage);
						pbmsg_delmsg(&tMessage);
						tMessage.Clear();
					}else
					{
						LOG_ERROR("gm", "DecodeFromGMStream Error(%d){%s}", 
								utf8_length, utf8_buffer);
					}

					break;
				}
			}
		}
	}
}

// 释放fd
void CGmServer::UnRegistGateSockfd(int fd)
{
	LK_ASSERT(fd > 0 && mSocks[fd].pactor != NULL, return);

	mSocks[fd].pactor->Close();

	mSocks[fd].fd = -1;
	if (fd == maxfd)
	{
		for (int i = maxfd - 1; i > 0 ; --i)
		{
			if (mSocks[i].fd != -1)
			{
				maxfd = i;
				break;
			}
		}
	}

	delete mSocks[fd].pactor; 
	mSocks[fd].pactor = NULL;
}

// 响应游戏服务器或者Gate服务器给的回复
void CGmServer::OnGMCmdMessageResult(CMessage* tpMessage)
{
	LK_ASSERT( tpMessage != NULL && tpMessage->has_msgpara() == true, return );
	CMessageHead* tpHead = tpMessage->mutable_msghead();
	LK_ASSERT( tpHead != NULL, return);

	switch ( tpHead->messageid())
	{
		case ID_G2M_CMDNORMAL_RESULT:
		{
			OnGMCmdNormalResult<CGMMessageCmdNormalResult>(tpMessage, FlashGmCmdNormalResult);
			break;
		}

		case ID_G2M_ROLELOCATION_RESULT:
		{
			OnGMCmdNormalResult<CGMMessageRoleLocationResult>(tpMessage, FlashGmCmdRoleLocationResult);
			break;
		}

		case ID_G2M_TASKINFO_RESULT:
		{
			OnGMCmdNormalResult<CGMMessageTaskInfoResult>(tpMessage, FlashGmCmdTaskInfoResult);
			break;
		}

		case ID_G2M_ROLEEQUIPMENT_RESULT:
		{
			OnGMCmdNormalResult<CGMMessageRoleEquipmentResult>(tpMessage, FlashGmCmdRoleEquipmentResult);
			break;
		}

		case ID_G2M_ROLEPACKINFO_RESULT: 
		{
			OnGMCmdNormalResult<CGMMessageRolePackInfoResult>(tpMessage, FlashGmCmdRolePackInfoResult);
			break;
		}

		case ID_G2M_ROLEITEMINPACK_RESULT: 
		{
			OnGMCmdNormalResult<CGMMessageRoleItemInPackResult>(tpMessage, FlashGmCmdRoleItemInPackResult);
			break;
		}

		case ID_G2M_ROLEEQUIPMENT_DETAIL_RESULT:
		{
			OnGMCmdNormalResult<CGMMessageRoleEquipmentDetailResult>(tpMessage, FlashGmCmdRoleEquipmentDetailResult);
			break;
		}

		case ID_G2M_ROLESTORAGE_DETAIL_RESULT:
		{
			OnGMCmdNormalResult<CGMMessageRoleStorageDetailResult>(tpMessage, FlashGmCmdRoleStorageDetailResult);
			break;
		}

		case ID_G2M_ROLESKILLINFO_RESULT:
		{
			OnGMCmdNormalResult<CGMMessageRoleSkillInfoResult>(tpMessage, FlashGmCmdRoleSkillInfoResult);
			break;
		}

		case ID_G2M_ROLEDETAIL_RESULT:
		{
			OnGMCmdNormalResult<CGMMessageRoleDetailResult>(tpMessage, FlashGmCmdRoleDetailResult);
			break;
		}

		case ID_G2M_ROLESKILLDETAIL_RESULT:
		{
			OnGMCmdNormalResult<CGMMessageRoleDetailResult>(tpMessage, FlashGmCmdRoleSkillDetailResult);
			break;
		}

		case ID_G2M_USERSTORAGEINFO_RESULT:
		{
			OnGMCmdNormalResult<CGMMessageRoleDetailResult>(tpMessage, FlashGmCmdUserStorageInfoResult);
			break;
		}

		case ID_G2M_IBINFO_RESULT:
		{
			OnGMCmdNormalResult<CGMMessageIbInfoResult>(tpMessage, FlashGmCmdIbInfoResult);
			break;
		}

		case ID_G2M_USERSOCIALINFO_RESULT:
		{
			OnGMCmdNormalResult<CGMMessageUserSocialInfoResult>(tpMessage, FlashGmCmdUserSocialInfoResult);
			break;
		}
		case ID_G2S_RESPONSE_RESETROLEPASSWORD:
		{
			OnGMCmdNormalResult<CMessageResetRolePasswordResponse>( tpMessage, FlashGmCmdResetRolePassword );
			break;
		}

		case ID_G2M_SQL_QUERY_RESULT:
		{
			OnGmMsgSqlQueryResult(tpMessage);
			break;
		}

		case ID_G2M_USERINFOBYIP_RESULT:
		{
			OnGmMsgUserInfoByIPResult(tpMessage);
			break;
		}
				
		default:
		{
			LOG_ERROR("gm", "There is no action for this Message(ID=%d)", 
					tpHead->messageid());
		}
	}
}

void CGmServer:: OnGmMsgUserInfoByIPResult( CMessage* tpMessage)
{
	LK_ASSERT( tpMessage != NULL && tpMessage->msgpara() != 0, return );
	CGMMsgUserInfoByIPResult* pResult = (CGMMsgUserInfoByIPResult*) tpMessage->msgpara();
	int nSessionID = pResult->session_id();
	int nfd = pResult->fd();
	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdUserInfoByIPResult);
}

void CGmServer:: OnGmMsgSqlQueryResult( CMessage* tpMessage)
{
	LK_ASSERT( tpMessage != NULL && tpMessage->msgpara() != 0, return );
	CGMMessageSqlQueryResult* pResult = (CGMMessageSqlQueryResult*) tpMessage->msgpara();
	int nSessionID = pResult->session_id();
	int nfd = pResult->fd();

	switch (pResult->type())
	{
		// 邮件列表
		case 0:		
		{
			OnGMCmdNormalResult<CGMMessageSqlQueryResult>(tpMessage, FlashGmCmdRoleMailResult);
			break;
		}

		// 称号信息
		case 1:
		{
			OnGMCmdNormalResult<CGMMessageSqlQueryResult>(tpMessage, FlashGmCmdRoleTitleResult);
			break;
		}

		default:
		{
			Send2GMNormalResult(0, nSessionID, nfd);		
		}
	}
}


void CGmServer::DealMsgBySessionAndFD(CMessage* tpMessage, 
		int nSessionID, int nfd, int(*pfflash) (CMessage*, CMaxString&))
{
	//char szStream[10240];
	char szToStream[MAX_HTML_LENGTH];
	szToStream[0] = 0;

	if ( nfd <= maxfd && mSocks[nfd].from == FD_FROM_GMSERVER )
	{
		if ( mSocks[nfd].pactor == NULL || mSocks[nfd].fd == -1) 
		{
			return;
		}

		if ( mSocks[nfd].pactor->GetStatus() == tcs_connected )
		{
			if ( mSocks[nfd].session_id == nSessionID)
			{
				int nToMessageLength = FlashToGMStream(tpMessage, szToStream, sizeof(szToStream), nSessionID, pfflash);
				if(nToMessageLength < 0)
				{
					LOG_ERROR("gm", "FlashToGMStream Message(%s) Error", __FUNCTION__);

					UnRegistGateSockfd(nfd);
					return;
				}

				/*
				size_t nToMessageLength = sizeof(szToStream);
				if (code_convert("gbk", "utf-8", szStream, (size_t) nMessageLength, szToStream, &nToMessageLength) < 0)
				{
					LOG_ERROR("default", "Function %s convert gbk(len=%d) to utf-8 failure.",
							__FUNCTION__, nMessageLength); 

					UnRegistGateSockfd(nfd);
					return;
				}
				*/

				#ifdef _GMDEBUG_
				LOG_DEBUG("gm", "Send2GM(%d):\n%s", nToMessageLength, szToStream);
				#endif

				mSocks[nfd].pactor->SendOneCode32(nToMessageLength, (BYTE*) szToStream );

				UnRegistGateSockfd(nfd);

			}else
			{
				LOG_ERROR("gm", "The Session_id(%d) is not the resgist id(%d)",
						nSessionID, mSocks[nfd].session_id);
			}
		}else
		{
			LOG_ERROR("gm", "The connection is closed on session_id(%d)", nSessionID); 
		}
	}
}

// 响应消息
void CGmServer::OnGMServerMessage(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT( tpMessage != NULL && tpMessage->has_msgpara() == true, return );
	CMessageHead* tpHead = tpMessage->mutable_msghead();
	LK_ASSERT( tpHead != NULL, return);

	switch ( tpHead->messageid())
	{
		case ID_M2G_KICKROLE_CMD:
		case ID_M2G_MOVEROLE_CMD:
		case ID_M2G_TASKINFO_CMD:
		case ID_M2G_FINISHED_TASKINFO_CMD:
		case ID_M2G_DELETEROLE_CMD:
		case ID_M2G_FROZENACCOUNT_CMD:
		case ID_M2G_REQUEST_FASTFREEZE_CMD:
		case ID_M2G_REQUEST_UNFASTFREEZE_CMD:
		case ID_M2G_UNFROZENACCOUNT_CMD:
		case ID_M2G_TRUSTEEUSER_CMD:
		case ID_M2G_UNTRUSTEEUSER_CMD:
		case ID_M2G_SHUTUP_CMD:
		case ID_M2G_CANCLESHUTUP_CMD:
		case ID_M2G_POSTBULLETIN_CMD:
		case ID_M2G_ENDTASK_CMD:
		case ID_M2G_TRUSTEESTORAGE_CMD:
		case ID_M2G_UNTRUSTEESTORAGE_CMD:
		case ID_M2G_RECOVERROLE_CMD:
		case ID_M2G_MULTIEXP_CMD:
		case ID_M2G_CANCELMULTIEXP_CMD:
		case ID_M2G_CHANGEMONEY_CMD:
		case ID_M2G_CHANGEEXP_CMD:
		case ID_M2G_CHANGEITEM_CMD:
		case ID_M2G_CLEARBAG_CMD:
		case ID_M2G_USERSOCIALINFO_CMD:
		case ID_M2G_QUERY_ROLE_MAIL_CMD:
		case ID_M2G_QUERY_ROLETITLE_CMD:
		{
			CLoginLogic::GetSingleton().Send2Gate(tpMessage);
			break;
		}

        // 发送系统消息
        case ID_G2S_NOTIFY_SENDSYSMAILTOPLAYER:
        {
            OnGMMessageSendMail( nCommHandle, tpMessage );
            break;
        }

		case ID_M2G_ROLEDETAIL_CMD:
		case ID_M2G_ROLEEQUIPMENT_CMD:
		case ID_M2G_ROLEPACKINFO_CMD:
		case ID_M2G_ROLEITEMINPACK_CMD:
		case ID_M2G_ROLESKILLINFO_CMD:
		case ID_M2G_ROLESKILLDETAIL_CMD:
		case ID_M2G_USERSTORAGEINFO_CMD:
		case ID_M2G_ROLEEQUIPMENT_DETAIL_CMD:
		case ID_M2G_ROLESTORAGE_DETAIL_CMD:
		{
			//OnGMMessageRoleDetailCmd(nCommHandle, tpMessage);
			CLoginLogic::GetSingleton().Send2Gate(tpMessage);
			break;
		}

		/*
		case ID_M2G_ROLESKILLDETAIL_CMD:
		{
			OnGMMessageRoleSkillDetailCmd(nCommHandle, tpMessage);
			break;
		}

		case ID_M2G_POSTBULLETIN_CMD:
		{
			OnGmMessagePostBulletinCmd(nCommHandle, tpMessage);
			break;
		}
		*/

		case ID_M2G_ROLELOCATION_CMD:
		{
			CLoginLogic::GetSingleton().Send2Gate(tpMessage);
			break;
		}

		case ID_M2G_NPCDETAIL_CMD:
		{
			OnGMMessageNpcDetailCmd(nCommHandle, tpMessage);
			break;
		}

		case ID_M2G_ROLEPROTO_CMD:
		{
			OnGMMessageRoleProtoCmd(nCommHandle, tpMessage);
			break;
		}

		case ID_M2G_IBINFO_CMD:
		case ID_M2G_SETIBPRICE_CMD:
		case ID_M2G_SETIBLOCATION_CMD:
		case ID_M2G_IBONSALE_CMD:
		case ID_M2G_IBNOTSALE_CMD:
		case ID_M2G_IBDESCRIPT_CMD:
		{
			//商城货架消息直接转发给网关处理
			CLoginLogic::GetSingleton().Send2Gate(tpMessage);
			break;
		}

		case ID_M2G_IPBIND_CMD:
		{
			OnGMMessageIpBindCmd(nCommHandle, tpMessage);
			break;
		}

		case ID_M2G_GOODSHISTORYTONPC_CMD:
		{
			OnGMMsgGoodsHistoryToNpcCmd(nCommHandle, tpMessage);
			break;
		}

		case ID_M2G_GOODSHISTORYFROMNPC_CMD:
		{
			OnGMMsgGoodsHistoryFromNpcCmd(nCommHandle, tpMessage);
			break;
		}

		case ID_M2G_GOODSHISTORYBETWEENUSERS_CMD:
		{
			OnGMMsgGoodsHistoryBetweenUsersCmd(nCommHandle, tpMessage);
			break;
		}

		case ID_M2G_GOODSHISTORY_CMD:
		{
			OnGMMsgGoodsHistoryCmd(nCommHandle, tpMessage);
			break;
		}

		case ID_M2G_OPERATIONDETAIL_CMD:
		{
			OnGMMsgOperationDetailCmd(nCommHandle, tpMessage);
			break;
		}
		
		// 重置密码的消息直接发往网关
		case ID_S2G_REQUEST_RESETROLEPASSWORD:
		{
			CLoginLogic::GetSingleton().Send2Gate(tpMessage);			
			break;
		}

		case ID_M2G_REQUEST_ROLEITEMOPERATE:
		{
			OnGMMsgRoleItemOperate(nCommHandle, tpMessage);
			break;
		}
		/*case ID_M2G_QUERY_ROLE_PICKITEM_FROMBOSS_CMD:
		{
			OnGMMsgRolePickItemFromBoss(nCommHandle,tpMessage);
			break;
		}*/

		case ID_M2G_ROLEMONEYOPERATE:
		{
			OnGMMsgRoleMoneyOperate(nCommHandle, tpMessage);
			break;
		}

		case ID_M2G_REQUEST_ROLEEXP:
		{
			OnGMMsgRoleExpOperate(nCommHandle, tpMessage);
			break;
		}

		case ID_M2G_REQUEST_ROLEITEMLEVELUP:
		{
			OnGMMsgItemUpgrade(nCommHandle, tpMessage);
			break;
		}

		case ID_M2G_FROZENMAC_CMD:
		case ID_M2G_UNFROZENMAC_CMD:
		{
			OnGMMsgFreezeMacCmd(nCommHandle, tpMessage);
			break;
		}

		case ID_M2G_REQUEST_CORPSTOTAL_CMD:
		{
			OnGMMsgCorpsTotalCmd(nCommHandle, tpMessage);
			break;
		}

		case ID_M2G_REQUEST_CORPSINFO_CMD:
		{
			OnGMMsgCorpsLogCmd(nCommHandle, tpMessage);
			break;
		}

		case ID_M2G_REQUEST_ROLEINFO_CMD:
		{
			OnGMMsgRoleLogCmd(nCommHandle, tpMessage);
			break;
		}

		case ID_M2G_REQUEST_CORPSDETAIL_CMD:
		{
			OnGMMsgCorpsDetailCmd(nCommHandle, tpMessage);
			break;
		}

		case ID_M2G_ROLE_ITEM_RECOVERY_CMD:
		{
			OnGMMsgRecoverRoleEquipCmd(nCommHandle, tpMessage);
			break;
		}

		case ID_M2G_USERLEVELINFO_CMD:
		{
			OnGMMsgRoleLevelInfoCmd(nCommHandle, tpMessage);
			break;
		}

		case ID_M2G_USERINFOBYIP_CMD:
		{
			OnGmMsgRoleInfoByIPCmd(nCommHandle, tpMessage);
			break;
		}

		default:
		{
			LOG_ERROR("gm", "there is no action for this GmCmdID(%d)", tpHead->messageid());
		}
	}
}

void CGmServer::OnGMMessageNpcDetailCmd(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	CGMMessageNPCDetailCmd* tpGmCmd = (CGMMessageNPCDetailCmd *) tpMessage->msgpara();

	CGMMessageNPCDetailResult tMsgResult; 

	tMsgResult.set_session_id(tpGmCmd->session_id());
	tMsgResult.set_fd(tpGmCmd->fd());

	NPC_DICT::iterator iter4npc = mNpcDict.find(tpGmCmd->npc_name().c_str()); 
	if (iter4npc != mNpcDict.end())
	{
		MAP_DICT::iterator iter4map = mMapDict.find(iter4npc->second.map_id);
		if (iter4map != mMapDict.end())
		{
			char location[256];

			location[0] = 0;
			snprintf(location, sizeof(location) - 1, "%s, %d, %d", 
					iter4map->second.map_name, iter4npc->second.x, iter4npc->second.y);

			PBNPCDetail* pPbNpcDetail = tMsgResult.add_npc_detail();

			pPbNpcDetail->set_map_id(iter4npc->second.map_id);
			pPbNpcDetail->set_npc_location(location);
		}
	}

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_NPCDETAIL_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tMsgResult);

	DealMsgBySessionAndFD(&tMessage, tpGmCmd->session_id(), tpGmCmd->fd(), FlashGmCmdNpcDetailResult);
}

void CGmServer::OnGMMessageSendMail(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CMessageSendSysMailToPlayer* lpGmCmd = (CMessageSendSysMailToPlayer*) tpMessage->msgpara(); 
    CLoginLogic::GetSingleton().Send2Gate(tpMessage);
    Send2GMNormalResult( 1, lpGmCmd->session_id(), lpGmCmd->fd() );
}

void CGmServer::OnGMMessageRoleProtoCmd(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageRoleProtoCmd* pGmCmd = (CGMMessageRoleProtoCmd*) tpMessage->msgpara(); 

	if (!CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_roleproto, pGmCmd->session_id(), pGmCmd->fd(), 
				pGmCmd->role_id(), SQL_NEED_CALLBACK, SELECT,
				"select '%s', %s from UMS_ROLE_DETAIL_%02d where role_id = %d ", 
				pGmCmd->field_name().c_str(), pGmCmd->field_name().c_str(), pGmCmd->role_id()%16, pGmCmd->role_id()
				 ))
	{
		LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
	}
}

void CGmServer::OnGMMessageRoleSkillDetailCmd(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageRoleSkillDetailCmd * pGmCmd = (CGMMessageRoleSkillDetailCmd *) tpMessage->msgpara(); 

	CGMMessageRoleSkillDetailResult tMsgResult; 

	tMsgResult.set_session_id(pGmCmd->session_id());
	tMsgResult.set_fd(pGmCmd->fd());

	PBSkillObject* pproto = tMsgResult.mutable_proto();
	LK_ASSERT(pproto != NULL, return);

	pproto->set_skillid(pGmCmd->skill_id());
	//pproto->set_skilllevel(1);

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_ROLESKILLDETAIL_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tMsgResult);

	DealMsgBySessionAndFD(&tMessage, pGmCmd->session_id(), pGmCmd->fd(), FlashGmCmdRoleSkillDetailResult);
}

void CGmServer::OnGmMessageNormalSqlDmlResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	//CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdSqlDmlResult);
}

void CGmServer::OnGMMessageIpBindCmd(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageIpBindCmd * pGmCmd = (CGMMessageIpBindCmd *) tpMessage->msgpara(); 

	if (!CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_ipbind, pGmCmd->session_id(), pGmCmd->fd(), 
				pGmCmd->user_id(), SQL_NEED_CALLBACK, UPDATE,
				"update UMS_ACCOUNT set binded_ip = '%s', account_status = %d  where account_id = %d", 
				pGmCmd->ip().c_str(), pGmCmd->type(), pGmCmd->user_id()))	
	{
		LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
	}
}

/*
void CGmServer::OnGMMessageRoleDetailCmd(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageRoleDetailCmd * pGmCmd = (CGMMessageRoleDetailCmd *) tpMessage->msgpara(); 

	if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_roledetail, pGmCmd->session_id(), pGmCmd->fd(), 
				 pGmCmd->role_id(), SQL_NEED_CALLBACK, SELECT,
				"select role_name, is_alive, gender_id, metier_id, nationality_id, \
				 face_type, role_level, role_expr, role_money, role_property, role_status \
				 from UMS_ROLE_USING where role_id = %d ", pGmCmd->role_id()
				 ))
	{
		LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
	}
}
*/

void CGmServer::OnGmMessageRoleProtoResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	//char szStream[10240] = {0};

	//int colnum = tpSqlResult->colcount();
	int rownum = tpSqlResult->rowcount();

	if ( tpSqlResult->resultcode() == 1 && rownum == 1)
	{
		DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdRoleProtoResult);
	}
}

void CGmServer::OnGmMessageRoleDetailResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	CMessageExecuteSqlResponse3* tpSqlResult = (CMessageExecuteSqlResponse3 *) tpMessage->msgpara();

	//char szStream[10240] = {0};

	//int colnum = tpSqlResult->colcount();
	int rownum = tpSqlResult->rowcount();

	if ( tpSqlResult->resultcode() == 1 && rownum == 1)
	{
		DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdRoleDetailResult);
	}
}

void  CGmServer::OnGMMsgGoodsHistoryToNpcCmd(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageGoodsHistoryToNPCCmd * pGmCmd = (CGMMessageGoodsHistoryToNPCCmd *) tpMessage->msgpara(); 

	NPC_DICT::iterator iter4npc = mNpcDict.find(pGmCmd->npc_name().c_str());
	if (iter4npc == mNpcDict.end())
	{
		//TODO:
		return;
	}


	if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_goods2npc, pGmCmd->session_id(), pGmCmd->fd(), 
				 pGmCmd->role_id(), SQL_NEED_CALLBACK, SELECT,
				 "call ProcGoods2Npc(%d, %d, '%s', '%s') ", 
				 pGmCmd->role_id(), 
				 iter4npc->second.npc_id,
				 pGmCmd->start_time().c_str(),
				 pGmCmd->end_time().c_str()))
	{
		LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
	}
}

void  CGmServer::OnGMMsgGoodsHistoryFromNpcCmd(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageGoodsHistoryFromNPCCmd * pGmCmd = (CGMMessageGoodsHistoryFromNPCCmd *) tpMessage->msgpara(); 

	NPC_DICT::iterator iter4npc = mNpcDict.find(pGmCmd->npc_name().c_str());
	if (iter4npc == mNpcDict.end())
	{
		//TODO:
		return;
	}

	if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_goodsfromnpc, pGmCmd->session_id(), pGmCmd->fd(), 
				 pGmCmd->role_id(), SQL_NEED_CALLBACK, SELECT,
				 "call ProcGoodsFromNpc(%d, %d, '%s', '%s') ", 
				 pGmCmd->role_id(), 
				 iter4npc->second.npc_id,
				 pGmCmd->start_time().c_str(),
				 pGmCmd->end_time().c_str()))
	{
		LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
	}

}

void  CGmServer::OnGMMsgGoodsHistoryBetweenUsersCmd(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageGoodsHistoryBetweenUsersCmd * pGmCmd = (CGMMessageGoodsHistoryBetweenUsersCmd *) tpMessage->msgpara(); 

	if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_goodsbetweenusers, pGmCmd->session_id(), pGmCmd->fd(), 
				 pGmCmd->role_id1(), SQL_NEED_CALLBACK, SELECT,
				 "call ProcGoodsBetweenUsers(%d, %d, '%s', '%s') ", 
				 pGmCmd->role_id1(), 
				 pGmCmd->role_id2(),
				 pGmCmd->start_time().c_str(),
				 pGmCmd->end_time().c_str()))
	{
		LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
	}

}

void  CGmServer::OnGMMsgGoodsHistoryCmd(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageGoodsHistoryCmd * pGmCmd = (CGMMessageGoodsHistoryCmd *) tpMessage->msgpara(); 


	int nSqlOperation = SQL_NEED_CALLBACK;

	if( ChoseDBByTimeRegion( pGmCmd->start_time().c_str(), pGmCmd->end_time().c_str() ) == 1 )
	{
		nSqlOperation |= SQL_REMOTE_HOST;
	}

	if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_goodshistory, pGmCmd->session_id(), pGmCmd->fd(), 
				 0, nSqlOperation, SELECT,
				 "call ProcGoodsHistory(%s, '%s', '%s') ", 
				 pGmCmd->item_id().c_str(),
				 pGmCmd->start_time().c_str(),
				 pGmCmd->end_time().c_str()))
	{
		LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
	}
}

void  CGmServer::OnGMMsgOperationDetailCmd(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageOperationDetailCmd * pGmCmd = (CGMMessageOperationDetailCmd *) tpMessage->msgpara(); 

	if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_operationdetail, pGmCmd->session_id(), pGmCmd->fd(), 
				 pGmCmd->role_id(), SQL_NEED_CALLBACK, SELECT,
				 "call ProcOperationDetail(%d, '%s', '%s') ", 
				 pGmCmd->role_id(),
				 pGmCmd->start_time().c_str(),
				 pGmCmd->end_time().c_str()))
	{
		LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
	}
}

void  CGmServer::OnGMMsgRoleItemOperate(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageRoleItemOperatedCmd * pGmCmd = (CGMMessageRoleItemOperatedCmd *) tpMessage->msgpara(); 
	
	int nSqlOperation = SQL_NEED_CALLBACK;
	
	if( ChoseDBByTimeRegion( pGmCmd->start_time().c_str(), pGmCmd->end_time().c_str() ) == 1 )
	{
		nSqlOperation |= SQL_REMOTE_HOST;
	}

	if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_roleitemoperate, pGmCmd->session_id(), pGmCmd->fd(), 
				 pGmCmd->page(), nSqlOperation, SELECT,
				 "call QueryRoleItem(%d, %d, %d, '%s', '%s', %d)",
				 pGmCmd->role_id(),
				 pGmCmd->op_type(),
				 pGmCmd->item_id(),
				 pGmCmd->start_time().c_str(),
				 pGmCmd->end_time().c_str(),
				 pGmCmd->page()
			))
	{
		LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
	}
}
//void  CGmServer::OnGMMsgRolePickItemFromBoss(int nCommHandle, CMessage* tpMessage)
//{
//	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
//	CGMMessageRolePickItemFromBoss * pGmCmd = (CGMMessageRolePickItemFromBoss *) tpMessage->msgpara(); 
//
//	int nSqlOperation = SQL_NEED_CALLBACK;
//
//	if( ChoseDBByTimeRegion( pGmCmd->start_time().c_str(), pGmCmd->end_time().c_str() ) == 1 )
//	{
//		nSqlOperation |= SQL_REMOTE_HOST;
//	}
//
//	if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_query_rolepickitemfromboss, pGmCmd->session_id(), pGmCmd->fd(), 
//		0, nSqlOperation, SELECT,
//		"call QueryRolePickItemFromBoss(%d, '%s', '%s')",
//		pGmCmd->npc_id(),
//		pGmCmd->start_time().c_str(),
//		pGmCmd->end_time().c_str()
//		))
//	{
//		LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
//	}
//}
void  CGmServer::OnGMMsgRoleMoneyOperate(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageRoleMoneyOperateCmd * pGmCmd = (CGMMessageRoleMoneyOperateCmd *) tpMessage->msgpara(); 

	int nSqlOperation = SQL_NEED_CALLBACK;

	if( ChoseDBByTimeRegion( pGmCmd->start_time().c_str(), pGmCmd->end_time().c_str() ) == 1 )
	{
		nSqlOperation |= SQL_REMOTE_HOST;
	}


	if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_rolemoneyoperate, pGmCmd->session_id(), pGmCmd->fd(), 
				 pGmCmd->page(), nSqlOperation, SELECT,
				 "call QueryRoleMoney(%d, %d, %d, '%s', '%s', %d)",
				 pGmCmd->role_id(),
				 pGmCmd->op_type(),
				 pGmCmd->item_type(),
				 pGmCmd->start_time().c_str(),
				 pGmCmd->end_time().c_str(),
				 pGmCmd->page()
			))
	{
		LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
	}
}

void  CGmServer::OnGMMsgRoleExpOperate(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageRoleExpCmd * pGmCmd = (CGMMessageRoleExpCmd *) tpMessage->msgpara(); 

	int nSqlOperation = SQL_NEED_CALLBACK;

	if( ChoseDBByTimeRegion( pGmCmd->start_time().c_str(), pGmCmd->end_time().c_str() ) == 1 )
	{
		nSqlOperation |= SQL_REMOTE_HOST;
	}


	if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_roleexpoperate, pGmCmd->session_id(), pGmCmd->fd(), 
				 pGmCmd->page(), nSqlOperation, SELECT,
				 "call QueryRoleExp(%d, %d, %d, '%s', '%s', %d)",
				 pGmCmd->role_id(),
				 pGmCmd->op_type(),
				 pGmCmd->exp_type(),
				 pGmCmd->start_time().c_str(),
				 pGmCmd->end_time().c_str(),
				 pGmCmd->page()
			))
	{
		LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
	}
}

void  CGmServer::OnGMMsgItemUpgrade(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return);
	CGMMessageRoleItemLevelUpCmd * pGmCmd = (CGMMessageRoleItemLevelUpCmd *) tpMessage->msgpara(); 

	int nSqlOperation = SQL_NEED_CALLBACK;

	if( ChoseDBByTimeRegion( pGmCmd->start_time().c_str(), pGmCmd->end_time().c_str() ) == 1 )
	{
		nSqlOperation |= SQL_REMOTE_HOST;
	}

	if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_itemupgrade, pGmCmd->session_id(), pGmCmd->fd(), 
				 pGmCmd->page(), nSqlOperation, SELECT,
				 "call QueryItemUpgrade(%d, %d, '%s', '%s', %d)",
				 pGmCmd->role_id(),
				 pGmCmd->item_id(),
				 pGmCmd->start_time().c_str(),
				 pGmCmd->end_time().c_str(),
				 pGmCmd->page()
			))
	{
		LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
	}
}

void CGmServer::OnGmMessageRoleItemOperateResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdRoleItemOperateResult);
}
//void CGmServer::OnGMMsgRolePickItemFromBossResult(CMessage* tpMessage,int nSessionID, int nfd)
//{
//	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
//	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdRoleItemOperateResult);
//}

void CGmServer::OnGmMessageRoleMoneyOperateResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdRoleMoneyOperateResult);
}

void CGmServer::OnGmMessageRoleExpOperateResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdRoleExpOperateResult);
}

void CGmServer::OnGmMessageItemUpgradeResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdItemUpgradeResult);
}

void CGmServer::OnGmMessageGoods2NpcResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdGoodAtNpcResult);
}

void CGmServer::OnGmMessageGoods4NpcResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdGoodAtNpcResult);
}

void CGmServer::OnGmMessageGoodsAtUsersResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdGoodAtUsersResult);
}

void CGmServer::OnGmMessageGoodsHistoryResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdGoodHistoryResult);
}

void CGmServer::OnGmMessageOperationDetailResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdOperationDetailResult);
}

void CGmServer::OnGmMessagePostBulletinCmd(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessagePostBulletinCmd* pGmCmd = (CGMMessagePostBulletinCmd*) tpMessage->msgpara();

	CLoginLogic::GetSingleton().Send2Gate(tpMessage);

	int nResult = 1;
	CGMMessageCmdNormalResult tResultMsg;
	tResultMsg.set_session_id(pGmCmd->session_id());
	tResultMsg.set_fd(pGmCmd->fd());
	tResultMsg.set_result(nResult);

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResultMsg);

	OnGMCmdNormalResult<CGMMessageCmdNormalResult>(&tMessage, FlashGmCmdNormalResult);
}

void CGmServer::OnGMMsgFreezeMacCmd(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

	CGMMessageFreezeMacCmd* pGmCmd = (CGMMessageFreezeMacCmd*) tpMessage->msgpara();

	int nResult = 1;
	CGMMessageCmdNormalResult tResultMsg;
	tResultMsg.set_session_id(pGmCmd->session_id());
	tResultMsg.set_fd(pGmCmd->fd());
	tResultMsg.set_result(nResult);

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResultMsg);

	OnGMCmdNormalResult<CGMMessageCmdNormalResult>(&tMessage, FlashGmCmdNormalResult);
}

void  CGmServer::OnGMMsgCorpsLogCmd(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);
	
	CGMMessageCorpsInfoCmd* pGmCmd = (CGMMessageCorpsInfoCmd*) tpMessage->msgpara();
	int nResult = 0;

	int nSqlOperation = SQL_NEED_CALLBACK;

	if( ChoseDBByTimeRegion( pGmCmd->start_time().c_str(), pGmCmd->end_time().c_str() ) == 1 )
	{
		nSqlOperation |= SQL_REMOTE_HOST;
	}

	switch (pGmCmd->type())
	{
		// 家族
		case 1:
		{
			if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_familylog, pGmCmd->session_id(), pGmCmd->fd(), 
						pGmCmd->page(), nSqlOperation, SELECT,
						"call QueryFamily(%s, '%s', '%s', %d)",
						pGmCmd->corps_name().c_str(),
						pGmCmd->start_time().c_str(),
						pGmCmd->end_time().c_str(),
						pGmCmd->page()
						))
			{
				LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
				nResult = -1;
			}

			break;
		}
		
		// 军团
		case 0:
		{
			if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_corpslog, pGmCmd->session_id(), pGmCmd->fd(), 
						pGmCmd->page(), nSqlOperation, SELECT,
						"call QueryCorps(%s, '%s', '%s', %d)",
						pGmCmd->corps_name().c_str(),
						pGmCmd->start_time().c_str(),
						pGmCmd->end_time().c_str(),
						pGmCmd->page()
						))
			{
				LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
				nResult = -1;
			}
			break;
		}

		default:
		{
			nResult = -1;
			break;	
		}
	}

	if ( nResult == -1)
	{
		CGMMessageCmdNormalResult tResultMsg;
		tResultMsg.set_session_id(pGmCmd->session_id());
		tResultMsg.set_fd(pGmCmd->fd());
		tResultMsg.set_result(0);

		CMessage tMessage;
		pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara((uint32_t) &tResultMsg);

		OnGMCmdNormalResult<CGMMessageCmdNormalResult>(&tMessage, FlashGmCmdNormalResult);
	}
}

void CGmServer::OnGmMsgQueryLogoutIPResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdQueryLogoutIPResult);
}

void CGmServer::OnGmMsgQueryStockResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdQueryStockResult);
}

void CGmServer::OnGmMsgQueryYBStallResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdQueryYBStallResult);
}

void CGmServer::OnGmMsgCorpsLogResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdCorpsLogResult);
}

void CGmServer::OnGmMsgFamilyLogResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdFamilyLogResult);
}

void  CGmServer::OnGMMsgRoleLogCmd(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);
	
	CGMMessageRoleInfoCmd* pGmCmd = (CGMMessageRoleInfoCmd*) tpMessage->msgpara();
	int nResult = 0;

	int nSqlOperation = SQL_NEED_CALLBACK;

	if( ChoseDBByTimeRegion( pGmCmd->start_time().c_str(), pGmCmd->end_time().c_str() ) == 1 )
	{
		nSqlOperation |= SQL_REMOTE_HOST;
	}

	switch (pGmCmd->type())
	{
		// 0 : 登录日志
		case 0:
		{
			if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_querylogin, pGmCmd->session_id(), pGmCmd->fd(), 
						pGmCmd->page(), nSqlOperation, SELECT,
						"call QueryRoleLogout(%d, '%s', '%s', %d)",
						pGmCmd->role_id(),
						pGmCmd->start_time().c_str(),
						pGmCmd->end_time().c_str(),
						pGmCmd->page()
						))
			{
				LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
				nResult = -1;
			}

			break;
		}
		
		// 1: 死亡
		case 1:
		{
			if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_querydeath, pGmCmd->session_id(), pGmCmd->fd(), 
						pGmCmd->page(), nSqlOperation, SELECT,
						"call QueryRoleDeath(%d, '%s', '%s', %d)",
						pGmCmd->role_id(),
						pGmCmd->start_time().c_str(),
						pGmCmd->end_time().c_str(),
						pGmCmd->page()
						))
			{
				LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
				nResult = -1;
			}
			break;
		}

		// 2:  任务
		case 2:
		{
			if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_querytask, pGmCmd->session_id(), pGmCmd->fd(), 
						pGmCmd->page(), nSqlOperation, SELECT,
						"call QueryRoleTask(%d, '%s', '%s', %d)",
						pGmCmd->role_id(),
						pGmCmd->start_time().c_str(),
						pGmCmd->end_time().c_str(),
						pGmCmd->page()
						))
			{
				LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
				nResult = -1;
			}

			break;
		}

		// 3: 元宝挂单日志
		case 3:
		{
			if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_query_stock1, pGmCmd->session_id(), pGmCmd->fd(), 
						pGmCmd->page(), SQL_NEED_CALLBACK, SELECT,
						" SELECT bourse_trade_detail, a.role_id, c.role_name, trade_kind, trade_type, price, yb_count, trade_yb, "
						" FROM_UNIXTIME(bourse_date), FROM_UNIXTIME(bourse_end_date), bourse_tax "
						" FROM UMS_BOURSE_TRADE_HISTORY a, UMS_ROLE b, UMS_ROLE c "
						" WHERE b.role_id = %d AND a.role_id = c.role_id AND a.account_id = b.account_id "
						" AND bourse_date >= UNIX_TIMESTAMP('%s') AND bourse_date < UNIX_TIMESTAMP('%s') "
						" ORDER BY bourse_date desc limit %d, 100 ",
						pGmCmd->role_id(),
						pGmCmd->start_time().c_str(),
						pGmCmd->end_time().c_str(),
						(pGmCmd->page() > 0 ? pGmCmd->page() -1 : 0) * 100
						))
			{
				LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
				nResult = -1;
			}

			break;
		}

		// 4: 元宝买单日志
		case 4:
		{
			if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_query_stock2, pGmCmd->session_id(), pGmCmd->fd(), 
						pGmCmd->page(), SQL_NEED_CALLBACK, SELECT,
						" SELECT from_unixtime(bourse_date), yb_count, d.role_name, buy_price, c.role_name, sell_price, bourse_buy_detail, bourse_sell_detail "
						" FROM UMS_BOURSE_LOG a, UMS_ROLE b , UMS_ROLE c, UMS_ROLE d "
						" WHERE b.role_id = %d AND a.buy_account = b.account_id AND bourse_date >= UNIX_TIMESTAMP('%s') AND bourse_date < UNIX_TIMESTAMP('%s') "
						" AND c.role_id = a.sell_role AND a.buy_role = d.role_id "
						" ORDER BY bourse_buy_detail LIMIT %d, 100 ",
						pGmCmd->role_id(),
						pGmCmd->start_time().c_str(),
						pGmCmd->end_time().c_str(),
						(pGmCmd->page() > 0 ? pGmCmd->page() -1 : 0) * 100
						))
			{
				LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
				nResult = -1;
			}
			
			break;
		}

		// 5: 元宝卖单日志
		case 5:
		{
			if (! CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_query_stock3, pGmCmd->session_id(), pGmCmd->fd(), 
						pGmCmd->page(), SQL_NEED_CALLBACK, SELECT,
						" SELECT from_unixtime(bourse_date), yb_count, c.role_name, sell_price,  d.role_name, buy_price, bourse_buy_detail, bourse_sell_detail "
						" FROM UMS_BOURSE_LOG a, UMS_ROLE b , UMS_ROLE c, UMS_ROLE d "
						" WHERE b.role_id = %d AND a.sell_account = b.account_id AND bourse_date >= UNIX_TIMESTAMP('%s') AND bourse_date < UNIX_TIMESTAMP('%s') "
						" AND c.role_id = a.sell_role AND a.buy_role = d.role_id "
						" ORDER BY bourse_sell_detail LIMIT %d, 100 ",
						pGmCmd->role_id(),
						pGmCmd->start_time().c_str(),
						pGmCmd->end_time().c_str(),
						(pGmCmd->page() > 0 ? pGmCmd->page() -1 : 0) * 100
						))
			{
				LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
				nResult = -1;
			}
			
			break;
		}

		// 6: 元宝摆摊交易查询 
		case 6:
		{
			if (!CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_query_ybstall, pGmCmd->session_id(), pGmCmd->fd(), pGmCmd->page(), SQL_NEED_CALLBACK, SELECT,
				"SELECT log_id,from_unixtime(exch_time), s.role_name, b.role_name, amount, item_id, item_count, price, fee, item_guid "
				" FROM UMS_YB_STALL_LOG l, UMS_ROLE s, UMS_ROLE b"
				" WHERE (l.seller_role_id = %d OR l.buyer_role_id = %d) AND exch_time between UNIX_TIMESTAMP('%s') AND UNIX_TIMESTAMP('%s') AND l.seller_role_id = s.role_id AND l.buyer_role_id = b.role_id ORDER BY log_id LIMIT %d, 100 ",
				pGmCmd->role_id(),
				pGmCmd->role_id(),
				pGmCmd->start_time().c_str(),
				pGmCmd->end_time().c_str(),
				(pGmCmd->page() > 0 ? pGmCmd->page() -1 : 0) * 100)
				)
			{
				LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
				nResult = -1;
			}

			break;
			
		}

		default:
		{
			nResult = -1;
			break;	
		}
	}

	if ( nResult == -1)
	{
		CGMMessageCmdNormalResult tResultMsg;
		tResultMsg.set_session_id(pGmCmd->session_id());
		tResultMsg.set_fd(pGmCmd->fd());
		tResultMsg.set_result(0);

		CMessage tMessage;
		pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara((uint32_t) &tResultMsg);

		OnGMCmdNormalResult<CGMMessageCmdNormalResult>(&tMessage, FlashGmCmdNormalResult);
	}

}

void CGmServer::OnGmMsgQueryDeathResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdQueryDeathResult);
}

void CGmServer::OnGmMsgQueryTaskResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdQueryTaskResult);
}

void CGmServer::OnGmMsgQueryLoginResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdQueryLoginResult);
}

void  CGmServer::OnGMMsgCorpsTotalCmd(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);
	
	CGMMessageCorpsTotalCmd* pGmCmd = (CGMMessageCorpsTotalCmd*) tpMessage->msgpara();
	int nResult = 0;
	const int const_per_page = 100;

	switch (pGmCmd->type())
	{
		// 家族
		case 1:
		{
			if (!CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_corpslist, pGmCmd->session_id(), pGmCmd->fd(), 
						pGmCmd->page(), SQL_NEED_CALLBACK, SELECT,
						"select %d, family_id, family_name, create_role_name, family_level, family_creattime, country_id from UMS_FAMILY order by country_id limit %d, %d" , 
						pGmCmd->type(), (pGmCmd->page() - 1)*const_per_page, const_per_page
						))
			{
				LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
				nResult = -1;
			}

			break;
		}

		// 军团
		case 0:
		{
			if (!CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_corpslist, pGmCmd->session_id(), pGmCmd->fd(), 
						pGmCmd->page(), SQL_NEED_CALLBACK, SELECT,
						"select %d, corps_id, corps_name, create_role_name, corps_level, corps_creattime, country_id from UMS_CORPS order by country_id  limit %d, %d" ,
						pGmCmd->type(), (pGmCmd->page() - 1)*const_per_page, const_per_page
						))
			{
				LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
				nResult = -1;
			}

			break;
		}

		// 军盟
		case 2:
		{
			if (!CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_corpslist, pGmCmd->session_id(), pGmCmd->fd(), 
						pGmCmd->page(), SQL_NEED_CALLBACK, SELECT,
						"select %d, union_id, union_name, create_role_name, corps0,  corps1, corps2, corps3, corps4, corps5, corps6, corps7, corps8, b.country_id from UMS_UNION a, UMS_CORPS b where a.union_head = b.corps_id order by b.country_id LIMIT %d,%d" ,
						pGmCmd->type(), (pGmCmd->page() - 1)*const_per_page, const_per_page
						))
			{
				LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
				nResult = -1;
			}

			break;
		}

		// 国家
		case 3:
		{
			if (!CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_corpslist, pGmCmd->session_id(), pGmCmd->fd(), 
						0, SQL_NEED_CALLBACK, SELECT,
						" SELECT %d, country_id, create_role_name, npc_id mod 10000000, map_id mod 1000000, owner_id, occupy_time, welfare_time, corps_name "
						" FROM UMS_NPC_STATUS a, UMS_CORPS b where a.owner_id = b.corps_id order by country_id, map_id limit %d,%d ",
						pGmCmd->type(), (pGmCmd->page() - 1)*const_per_page, const_per_page
						))
			{
				LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
				nResult = -1;
			}
			break;
		}
	}

	if ( nResult == -1)
	{
		CGMMessageCmdNormalResult tResultMsg;
		tResultMsg.set_session_id(pGmCmd->session_id());
		tResultMsg.set_fd(pGmCmd->fd());
		tResultMsg.set_result(0);

		CMessage tMessage;
		pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara((uint32_t) &tResultMsg);

		OnGMCmdNormalResult<CGMMessageCmdNormalResult>(&tMessage, FlashGmCmdNormalResult);
	}
}

void CGmServer::OnGmMessageCorpsTotalResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdCorpsTotalResult);
}

void CGmServer::OnGmMsgRoleInfoByIPCmd( int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);
	
	CGMMessageUserInfoByIPCmd *pGmCmd = (CGMMessageUserInfoByIPCmd *) tpMessage->msgpara(); 

	// 0 在线 1 离线
	if ( pGmCmd->type() == 0)
	{
		// send to gate
		CLoginLogic::GetSingleton().Send2Gate(tpMessage);
	}else
	{
		// send to log
		int nSqlOperation = SQL_NEED_CALLBACK;
		if( ChoseDBByTimeRegion( pGmCmd->start_time().c_str(), pGmCmd->end_time().c_str() ) == 1 )
		{
			nSqlOperation |= SQL_REMOTE_HOST;
		}

		struct in_addr inp;
		inet_aton(pGmCmd->ip().c_str(), &inp); 


		CSession* pSession = CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_queryiplog, pGmCmd->session_id(), pGmCmd->fd(), 
				pGmCmd->page(), nSqlOperation, SELECT,
				"call QueryIPLogoutCnt('%s', '%s', %u) ",
				pGmCmd->start_time().c_str(), 
				pGmCmd->end_time().c_str(), 
				inp.s_addr);

		if ( !pSession)
		{
			LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
			return;
		}
	}
}

void CGmServer::OnGMMsgRoleLevelInfoCmd(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);
	
	CGMMessageUserLevelInfoCmd *pGmCmd = (CGMMessageUserLevelInfoCmd *) tpMessage->msgpara(); 
	int nSqlOperation = SQL_NEED_CALLBACK;

	time_t nSeconds = time(0);
	// 如果没有起始时间, 则赋值为7天之前
	if (!pGmCmd->has_start_time())
	{
		char szTime[32] = {0};

		time_t nStartTime = nSeconds - (15 * 24 * 3600);
		struct tm* ptmnow = localtime( &nStartTime);
		strftime(szTime, sizeof(szTime) -1, "%F %T", ptmnow);
			
		pGmCmd->set_start_time(szTime);
	}

	// 如果没有结束时间, 则赋值为当天
	if (!pGmCmd->has_end_time())
	{
		char szTime[32] = {0};
		struct tm* ptmnow = localtime( &nSeconds);
		strftime(szTime, sizeof(szTime) -1, "%F %T", ptmnow);
			
		pGmCmd->set_end_time(szTime);
	}

	if( ChoseDBByTimeRegion( pGmCmd->start_time().c_str(), pGmCmd->end_time().c_str() ) == 1 )
	{
		nSqlOperation |= SQL_REMOTE_HOST;
	}

	CSession* pSession = NULL;
	pSession = CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_userlevel, pGmCmd->session_id(), pGmCmd->fd(), 
				0, nSqlOperation, SELECT,
				"call ProcRoleLevelInfo(%d, '%s', '%s') ", 
				pGmCmd->role_id(), 
				pGmCmd->start_time().c_str(), 
				pGmCmd->end_time().c_str()); 

	if ( !pSession)
	{
		LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);

		CGMMessageCmdNormalResult tResultMsg;
		tResultMsg.set_session_id(pGmCmd->session_id());
		tResultMsg.set_fd(pGmCmd->fd());
		tResultMsg.set_result(0);

		CMessage tMessage;
		pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara((uint32_t) &tResultMsg);

		OnGMCmdNormalResult<CGMMessageCmdNormalResult>(&tMessage, FlashGmCmdNormalResult);
	}
}

void CGmServer::OnGMMsgRecoverRoleEquipCmd(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);
	
	CGMMsgRoleItemRecoveryCmd *pGmCmd = (CGMMsgRoleItemRecoveryCmd *) tpMessage->msgpara(); 
	int nSqlOperation = SQL_NEED_CALLBACK;

	if( ChoseDBByTimeRegion( pGmCmd->start_time().c_str(), pGmCmd->end_time().c_str() ) == 1 )
	{
		nSqlOperation |= SQL_REMOTE_HOST;
	}

	// 0 - 查询, 1 - 恢复
	CSession* pSession = NULL;
	pSession = CLoginLogic::GetSingleton().ExecuteSql( em_dbsession_recoverequip, pGmCmd->session_id(), pGmCmd->fd(), 
				pGmCmd->page(), nSqlOperation, SELECT,
				"call QueryRoleEquipGhost(%d, %llu, '%s', '%s', %d) ", 
				pGmCmd->role_id(), 
				pGmCmd->item_id(), 
				pGmCmd->start_time().c_str(), 
				pGmCmd->type() == 1 ? "" : pGmCmd->end_time().c_str(), 
				pGmCmd->type() == 1 ? 0 : pGmCmd->page());

	if ( !pSession)
	{
		LOG_ERROR("gm", "object pool create serversession error on function(%s)!", __FUNCTION__);
		return;
	}

	pSession->SetParam4(pGmCmd->type());
}

void CGmServer::OnGmMsgQueryRoleEquipResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	

	// 如果是回复，则向网关发送系统邮件，等待返回
	CMessageExecuteSqlResponse3* pSqlResult = (CMessageExecuteSqlResponse3*) tpMessage->msgpara();

	int nMsgSessionID = pSqlResult->sessionid();
	CServerSession* pSession = (CServerSession*) CLoginObjManager::GetSingletonPtr()->GetObject(nMsgSessionID);
	
	if ( pSession == NULL ) 
	{
		Send2GMNormalResult(0, nSessionID, nfd);		
		return;
	}

	if ( pSession->GetParam4() == 0)
	{
		DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdRecoverRoleEquipResult);
	}else
	{
		int colnum = pSqlResult->colcount();
		int rownum = pSqlResult->rowcount();

		// 设置为Session终结标志	
		pSession->SetParam4(0);

		if ( rownum != 1 || colnum != 4)
		{
			Send2GMNormalResult(0, nSessionID, nfd);		
			return;
		}

		// 恢复装备
		CMessageOfflineSysMailNotify proto;
		proto.set_roleid(atoi(pSqlResult->fieldvalue(0).c_str()));
		proto.set_mailtype(32);
		proto.set_attachmoney(0);
		proto.set_session_id(nSessionID);
		proto.set_fd(nfd);
		PBItemObject * pbItem = proto.add_attachitem();
		if (!pbItem ->ParseFromArray(pSqlResult->fieldvalue(3).c_str(),
					                         pSqlResult->fieldvaluelen(3)))
		{
			Send2GMNormalResult(0, nSessionID, nfd);		
			return;
		}

		CMessage tMessage;
		tMessage.set_msgpara((uint32_t) &proto);

		pbmsg_sethead(&tMessage, ID_S2G_NOTIFY_OFFLINEMAIL, 0, 0, 0, 0, 0, 0);
		CLoginLogic::GetSingleton().Send2Gate(&tMessage);
	}
}

void  CGmServer::OnGMMsgCorpsDetailCmd(int nCommHandle, CMessage* tpMessage)
{
	LK_ASSERT(tpMessage != NULL, return);
	LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);
	
	CGMMessageCorpsDetailCmd* pGmCmd = (CGMMessageCorpsDetailCmd*) tpMessage->msgpara();
	int nResult = 0;

	switch (pGmCmd->type())
	{
		// 家族
		case 1:
		{
			const char* stmtsql = "SELECT %d, %s FROM UMS_FAMILY WHERE family_name = '%s'";
			if (!CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_corpsdetail, pGmCmd->session_id(), pGmCmd->fd(), 
						0, SQL_NEED_CALLBACK, SELECT, 
						stmtsql, pGmCmd->type(), mUmsFamilyFields.c_str(), pGmCmd->corps_name().c_str()))
			{
				LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
				nResult = -1;
			}

			break;
		}

		// 军团
		case 0:
		{
			const char* stmtsql = "SELECT %d, %s FROM UMS_CORPS where corps_name = '%s'";
			if (!CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_corpsdetail, pGmCmd->session_id(), pGmCmd->fd(), 
						0, SQL_NEED_CALLBACK, SELECT, 
						stmtsql, pGmCmd->type(), mUmsCorpsFields.c_str(), pGmCmd->corps_name().c_str()))
			{
				LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
				nResult = -1;
			}

			break;
		}
			
		// 军盟
		case 2:
		{
			const char* stmtsql = "SELECT %d, %s FROM UMS_UNION WHERE union_name = '%s'";
		
			if (!CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_corpsdetail, pGmCmd->session_id(), pGmCmd->fd(), 
						0, SQL_NEED_CALLBACK, SELECT, 
						stmtsql, pGmCmd->type(), mUmsUnionFields.c_str(), pGmCmd->corps_name().c_str()))
			{
				LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
				nResult = -1;
			}
			break;
		}

		// 国家
		case 3:
		{
			const char* stmtsql = "SELECT %d, %s FROM UMS_WORLD WHERE world_id = %d and country_id = %d";

			Tokens token = StrSplit( pGmCmd->corps_name(), ".");
			if ( token.size () != 2)
			{
				LOG_ERROR("gm", "on %s StrSplit %s by '.' is %d fields, but 2 is needed ", 
						__FUNCTION__, pGmCmd->corps_name().c_str(), token.size());
				nResult = -1;
				break;
			}

			if (!CLoginLogic::GetSingleton().ExecuteSql(em_dbsession_corpsdetail, pGmCmd->session_id(), pGmCmd->fd(), 
						0, SQL_NEED_CALLBACK, SELECT, 
						stmtsql, pGmCmd->type(), mUmsWorldFields.c_str(), atoi(token[0].c_str()), atoi(token[1].c_str())))
			{
				LOG_ERROR("gm", "object pool create serversession error on function %s!", __FUNCTION__);
				nResult = -1;
			}

			break;
		}
	}

	if ( nResult == -1)
	{
		CGMMessageCmdNormalResult tResultMsg;
		tResultMsg.set_session_id(pGmCmd->session_id());
		tResultMsg.set_fd(pGmCmd->fd());
		tResultMsg.set_result(0);

		CMessage tMessage;
		pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);
		tMessage.set_msgpara((uint32_t) &tResultMsg);

		OnGMCmdNormalResult<CGMMessageCmdNormalResult>(&tMessage, FlashGmCmdNormalResult);
	}
}

void CGmServer::OnGmMsgRoleLevelInfoResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdRoleLevelInfoResult);
}

void CGmServer::OnGmMessageCorpsDetailResult(CMessage* tpMessage, int nSessionID, int nfd)
{
	LK_ASSERT(tpMessage != NULL && tpMessage->msgpara() != 0, return );	
	DealMsgBySessionAndFD(tpMessage, nSessionID, nfd, FlashGmCmdCorpsDetailResult);
}

void CGmServer::Send2GMNormalResult(int nResult, int nSessionID, int nfd)
{
	CGMMessageCmdNormalResult tResultMsg;
	tResultMsg.set_session_id(nSessionID);
	tResultMsg.set_fd(nfd);
	tResultMsg.set_result(nResult != 0 ? 1 : 0);

	CMessage tMessage;
	pbmsg_sethead(&tMessage, ID_G2M_CMDNORMAL_RESULT, 0, 0, 0, 0, 0, 0);
	tMessage.set_msgpara((uint32_t) &tResultMsg);

	OnGMCmdNormalResult<CGMMessageCmdNormalResult>(&tMessage, FlashGmCmdNormalResult);
}

// 通过时间来为GM2.0请求指定DB; 0 默认； 1 远程服务器db
int CGmServer::ChoseDBByTimeRegion( const char* pszBeginTime, const char* pszEndTime )
{

	if( pszBeginTime == NULL )
	{
		 return 0;
	}

	// logserver本地日志只保存最近7天数据
	if(  TimeStrToRealTime( pszBeginTime ) <= time(0) - 7*24*60*60 )
	{
		return 1;
	}

	return 0;
}

CIpData::CIpData()
{
	Init();
}

CIpData::~CIpData()
{
	Free();
}

bool CIpData::Init()
{
	m_nID = 0;
	//m_pcProvince = NULL;
	//m_pcCity = NULL;
	memset(m_pcProvince,0,32);
	memset(m_pcCity,0,32);
	m_nStartIp = 0;
	m_nEndIp = 0;
	m_nProvinceLen = 0;
	m_nCityLen = 0;
	return true;
}

void CIpData::Free()
{
	m_nID = 0;
	m_nStartIp = 0;
	m_nEndIp = 0;
	m_nCityLen = 0;
	m_nProvinceLen = 0;
	//GmDestoryText(m_pcProvince);
	//GmDestoryText(m_pcCity);
}

void CIpData::SetID(int nID)
{
	m_nID = nID;
}

void CIpData::SetProvince(unsigned short * pcProvince)
{
	//int len  = wcslen(pcProvince)+1;
	//wcsncpy(m_pcProvince,pcProvince,len);
	memcpy(m_pcProvince,pcProvince,m_nProvinceLen*2+1);
	
}

void CIpData::SetCity(unsigned short * pcCity)
{
	/*int len  = wcslen(pcCity)+1;
	wcsncpy(m_pcCity,pcCity,len);
	int wlen =  wcslen(m_pcCity);
	char *pp =  new char[2*wlen +1];
	pp[0] = '\0';
	setlocale(LC_ALL,"zh_CN.GBK");
	wcstombs(pp,m_pcCity,2*wlen+1);*/
	memcpy(m_pcCity,pcCity,m_nCityLen*2+1);
}

void CIpData::SetStartIp(unsigned int  nIp)
{
	m_nStartIp = nIp;
}

void CIpData::SetEndIp(unsigned int  nIp)
{
	m_nEndIp = nIp;
}

int	 CIpData::GetID()
{
	return m_nID;
}

unsigned short * CIpData::GetProvince()
{
	return m_pcProvince;
}

unsigned short * CIpData::GetCity()
{
	return m_pcCity;
}

unsigned int  CIpData::GetStartIp()
{
	return m_nStartIp;
}

unsigned int  CIpData::GetEndIp()
{
	return m_nEndIp;
}

bool CIpData::IpIsHere(unsigned int  nIp)
{
	if (nIp >= m_nStartIp && nIp <= m_nEndIp)
	{
		return true;
	}
	return false;
}

int CIpData::GetProvinceLen()
{
	return m_nProvinceLen;
}
int CIpData::GetCityLen()
{
	return m_nCityLen;
}
