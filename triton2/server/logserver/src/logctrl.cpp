#include "logctrl.h"
#include "shm.h"
#include "tinyxml.h"
#include "proxymessage_pb.hxx.pb.h"
#include "servermessage_in.h"
#include "DatabaseMysql.h"
#include "QueryResult.h"
#include "base.h"

CSharedMem* CLogCtrl::mShmPtr = NULL;
template<> CLogCtrl* CSingleton< CLogCtrl >::spSingleton = NULL;


#ifdef _POSIX_MT_
pthread_mutex_t CLogCtrl::stMutex;
#endif

STServerStatisticInfo g_stServerStatisticForPrint;

void* CLogCtrl::operator new( size_t nSize )
{
    if(!mShmPtr)
    {
        return (void *)NULL;
    }

    BYTE* pTemp = (BYTE *)mShmPtr->CreateSegment( nSize );

    return (void *)pTemp;
}
void CLogCtrl::operator delete( void *pMem )
{
}

int CLogCtrl::CatalogID(int nCatalogType, int nEntityType, int nActionType, int nValueType)
{
	int nCalID = -1;

	switch (nCatalogType)
	{
		case EM_SERVER_ITEM:
		{
			CTplItem *ptpl = (CTplItem *) CDataStatic::SearchTpl(nValueType);
			if ( ptpl == NULL) return -1;

			int nItemType = ptpl->mItemType;	// 8bit
			int nLevel = 0;						// 4bit
			int nSubType1 =  0;					// 4bit
			int nSubType2 =	 0; 				// 4bit
			int nSubType = 0;


			switch (nItemType)
			{
				case CTplItem::ITEM_RETURN:			// 回城卷轴
				case CTplItem::ITEM_TELEPORT:		// 传送物品
				case CTplItem::ITEM_RELIFE:			// 复活卷轴
				case CTplItem::ITEM_PETEGG:			// 宠物蛋
				case CTplItem::ITEM_PETFOOD:		// 宠物食品
				case CTplItem::ITEM_UPGLUCK:		// 装备升级幸运符
				case CTplItem::ITEM_REFRESHTASK:	// 刷任务用道具
				case CTplItem::ITEM_TASK:			// 任务道具
				case CTplItem::ITEM_PRODUCE:		// 生产原料
				case CTplItem::ITEM_SHOUT:			// 喇叭喊话道具
				case CTplItem::ITEM_SKILL:			// 调用技能的物品
				case CTplItem::ITEM_EXP:			// 增加经验道具
				{
					break;
				}

				case CTplItem::ITEM_EQUIPUPG:		// 装备升级物品
				{
					nLevel = ((CTplItemEquipUpg*) ptpl)->mLimit;
					nSubType1 = ((CTplItemEquipUpg*) ptpl)->mType;  // [0-3]
					nSubType2 = ((CTplItemEquipUpg*) ptpl)->mUpType;// [0-1]

					nSubType = (nSubType1 << 4) + nSubType2;
					break;
				}

				case CTplItem::ITEM_UPGLEAST:		// 装备升级保底符
				{
					nSubType = ((CTplUpgLeast*) ptpl)->mType;	//[0-1];
					break;
				}

				case CTplItem::ITEM_PHYSIC:			// 药品
				{
					nSubType = ((CTplItemPhysic*) ptpl)->mShowType;
					break;
				}

				case CTplItem::ITEM_BOOK:			// 技能书
				{
					nLevel = ((CTplItemSkillBook*) ptpl)->mBookColor;
					break;
				}

				case CTplItem::ITEM_WEAPON:			// 武器装备
				{
					nLevel = ((CTemplateEquipment*) ptpl)->mShowLevel;
					nSubType1 = ((CTemplateEquipment*) ptpl)->mShowColour;
					nSubType2 =((CTemplateWeapon*) ptpl)->mWeaponType;

					nSubType = (nSubType1 << 4) + nSubType2;
					break;
				}

				case CTplItem::ITEM_MAGICWEAPON:	// 法宝
				{
					nLevel = ((CTemplateEquipment*) ptpl)->mShowLevel;
					nSubType = ((CTemplateEquipment*) ptpl)->mShowColour;
					break;
				}

				case CTplItem::ITEM_ARMOR:			// 防护装备
				{
					nLevel = ((CTemplateEquipment*) ptpl)->mShowLevel;
					nSubType1 = ((CTemplateEquipment*) ptpl)->mShowColour;
					nSubType2 = ((CTemplateArmor*) ptpl)->mShowType;

					nSubType = (nSubType1 << 4) + nSubType2;
					break;
				}

				case CTplItem::ITEM_FASHION:		// 时装装备
				{
					break;
				}

				case CTplItem::ITEM_RUNE:			// 符文道具
				{
					nSubType = ((CTplItemRune*) ptpl)->mSuitID;
					break;
				}

				case CTplItem::ITEM_JEWEL:			// 珠宝道具
				{
					nLevel = ((CTplItemJewel*) ptpl)->mLevel;
					nSubType = ((CTplItemJewel*) ptpl)->mShowColor;
					break;
				}

				case CTplItem::ITEM_YUANBUG:		// 元气虫道具
				case CTplItem::ITEM_YUANEGG:		// 元气蛋道具
				case CTplItem::ITEM_SPECIAL_TASK:	// 特殊任务物品
				case CTplItem::ITEM_AUTOSHOUT:      // 自动喊话道具
				case CTplItem::ITEM_HORSE:			// 坐骑道具
				case CTplItem::ITEM_BINDSTONE:      // 绑定石
				case CTplItem::ITEM_SCROLL:			// 仙轴
				{
					break;
				}

				case CTplItem::ITEM_CARD:			// 卡片
				{
					nLevel = ((CTplItemCard*) ptpl)->mType;
					break;
				}

				case CTplItem::ITEM_RETURNPAPER:	// 指定传送卷
				case CTplItem::ITEM_TELESYMBOL:		// 指定传送符
				case CTplItem::ITEM_REDSTONE:		// 内丹
				case CTplItem::ITEM_STOVE:			// 指定炉石
				case CTplItem::ITEM_QUESTIONPAPER:	// 题卷
				case CTplItem::ITEM_MINEMAP:		// 藏宝图
				case CTplItem::ITEM_TOKEN:			// 惩妖令牌
				{
					break;
				}

				case CTplItem::ITEM_KITBAG:			// 背包
				{
					// 4bit not enougth
					nLevel = ((CTplKitBag*) ptpl)->mIndexNum;
					break;
				}

				case CTplItem::ITEM_REMOVE:			// 宝石摘除符
				case CTplItem::ITEM_CHANGE:			// 抗性转化石
				case CTplItem::ITEM_ADAPTER:		// 物品适配器
				case CTplItem::ITEM_CALL:			// 召唤类物品
				case CTplItem::ITEM_GIFTBAG:		// 礼包类物品
				case CTplItem::ITEM_GOLDSTONE:		// 点金石
				case CTplItem::ITEM_PLANT:			// 采集的植物
				case CTplItem::ITEM_MINERAL:		// 采集产生的矿物  	 
				{
					break;
				}


				case CTplItem::ITEM_MAGICSTONE:		// 附魔物品
				case CTplItem::ITEM_LIFESKILLSCROLL:// 生活技能配方卷轴
				{
					break;
				}

				case CTplItem::ITEM_FUNCITEM:		// 功能性物品
				{
					nSubType = ((CTplFuncItem*) ptpl)->mFuncCode;
					break;
				}

				case CTplItem::ITEM_SKILLBOOK:		// 法宝技能书
				{
					nLevel = ((CTplItemSkillBook*) ptpl)->mBookColor;
					break;
				}

				case CTplItem::ITEM_CORPSTOKEN:		// 家族将军令
				{
					break; 
				}

				case CTplItem::ITEM_REDPAPER:		// 红包
				{
					nSubType = ((CTplItemRedPaper*) ptpl)->mRedPaperType;
					break;
				}

				default:
				{
					break;
				}
			}

			// 4bit 8bit(Action) 8bit(ItemType) 4bit(Level) 8bit(SubType)
			nCalID = (nCatalogType << 28) + (nActionType << 20)  + (nItemType << 12) + (nLevel << 8) + nSubType;
			break;
		}

		case EM_SERVER_MONEY:	
		case EM_SERVER_EVENT:
		case EM_SERVER_EXP:
		case EM_SERVER_FAMILY:
		case EM_SERVER_CORPS:
		{
			nCalID = (nCatalogType << 28) + (nActionType << 16)  + (nEntityType << 8) + nValueType;
			break;
		}

		default:
		{
			return -1;
		}
	}

	return nCalID;
}

int CLogCtrl::CountSize()
{
    int iTemp = 0;

    iTemp += sizeof(CLogCtrl);
    LOG_INFO( "logctrl", "CLogCtrl %d", sizeof(CLogCtrl) );

    iTemp += MAXHANDLENUMBER*CLogHandle::CountSize();
    LOG_INFO( "logctrl", "MAXHANDLENUMBER*CLogHandle::CountSize %d", MAXHANDLENUMBER*sizeof(CLogHandle::CountSize()) );

	LOG_INFO ( "logctrl", "Total Size is %d", iTemp );

    return iTemp;
}

void CLogCtrl::SetRunFlag( int iFlag )
{
	m_iRunFlag |= iFlag;
	LOG_INFO( "logctrl", "Set Run Flag %d, All Flag Is %d", iFlag, m_iRunFlag );
}

void CLogCtrl::ClearRunFlag( int iFlag )
{
	m_iRunFlag &= ~iFlag;
	LOG_INFO( "logctrl", "Clear Run Flag %d", iFlag );
}

bool CLogCtrl::IsRunFlagSet( int iFlag )
{
	return ( m_iRunFlag & iFlag ) == iFlag ;
}

CLogCtrl::CLogCtrl()
{
    int i;

    if(!mShmPtr)
    {
        return;
    }

    CLogHandle::ms_pCurrentShm = mShmPtr;

    for( i = 0; i < MAXHANDLENUMBER; i++ )
    {
        m_apHandles[i] = new CLogHandle;
    }

    m_sCheckPointHandle = 0;
    memset((void *)&m_stLogSvrdCfg, 0, sizeof(m_stLogSvrdCfg));

	memset( m_tLogStat, 0, sizeof(m_tLogStat) );

	m_iRunFlag = 0;

	m_Id2Table.initailize();

	m_Action2Catalog.initailize();

	m_CatalogDict.initailize();

	m_Life.initailize();

	m_ExpExceptRole.initailize();
	m_ItemFilter.initailize();
	m_DropFilter.initailize();

	m_nMinTalentExp = 0;
	m_nMinRoleExp = 0;
	m_nMinMWExp = 0;
}

CLogCtrl::~CLogCtrl()
{
	CDataStatic::Finialize();
}

int CLogCtrl::Initialize( )
{

#ifdef _POSIX_MT_
	pthread_mutex_init( &stMutex, NULL);
#endif

	if( ReadLogIpCfg( "../config/ip.xml" ) < 0 ) 
	{
		exit( 1 ); 
	}

	if( ReadLogSvrdCfg( "../config/data/logserver.xml" ) < 0 )
	{
		exit(1);
	}

	//LoadToken2String();

	LoadCatalogDetail();
	
	m_tLastSaveDataTime = time(NULL);
	//INIT_ROLLINGFILE_LOG( "sql", "../log/sql_error.log", LEVEL_DEBUG );
		
	CDataStatic::Initialize();
	LOG_INFO("default", "Template Ver %d", CDataStatic::sTemplateVersion );

	return 0;
}

void CLogCtrl::LoadCatalogDetail()
{
	DatabaseMysql db;		
	db.Initialize(m_stLogSvrdCfg.m_szMysql);

	const char* stmtsql = "SELECT type_id, value_id, code_group from SYS_CODE_VALUE where type_id in (9, 11, 14)";
	QueryResult* res = db.Query(stmtsql);
	if ( res == NULL)
	{
		LOG_ERROR( "default", "Can not find execute (%s)", stmtsql);
		return;
	}

	int rowcount = res->GetRowCount();
	int colcount = res->GetFieldCount();

	if( rowcount > 0 )
	{
		do
		{
			Field* pField = res->Fetch();
			if( pField == NULL )
			{
				LOG_ERROR("default", "sql %s success, row[%d], col[%d], but some row is null\n",
						stmtsql, rowcount, colcount);
				break;
			}
			
			int nAttrID = pField[0].GetUInt8();
			int nActionID =  pField[1].GetUInt8();
			int nGroupID  =  pField[2].GetUInt8();

			int nCatalogType = (nAttrID == 9 ? 3 : ( nAttrID == 11 ? 2 : 1));

			if((m_Life.insert(ITEMACTIONGROUP::value_type( (nCatalogType << 8) + nActionID, nGroupID))).second)
			{
				LOG_DEBUG("default", "Load action_group(%d,%d,%d) action_id(%d,%d) group_id(%d) ok", 
						nAttrID, nActionID, nGroupID, nCatalogType, nActionID, nGroupID);
			}else
			{
				LOG_ERROR("default", "Load action_group(%d,%d,%d) action_id(%d,%d) group_id(%d) error", 
						nAttrID, nActionID, nGroupID, nCatalogType, nActionID, nGroupID);
			}
		}while(  res->NextRow() );
	}
}

void CLogCtrl::LoadToken2String()
{
	const char* szFileList [] = 
	{
		"../config/langServerTemp.xml",
		"../config/langTask.xml",
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
			LOG_ERROR( "default", "No Root Element Found in file(%s)", *pathname );
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
					LOG_ERROR("default", "Element (%s) is not text context", tEleName.c_str());
					continue;
				}

				const char* szToken  = pIDElem->Attribute( "id" );
				const char* szString = pIDElem->Attribute( "value" );
				if ( !szToken || !szString || strlen( szToken) >= 32 )
				{
					LOG_ERROR("default", "Element(%s:%s) text attr error!", szToken, szString);
					continue;
				}

				std::pair<CTOKEN2STRING::iterator, bool> ret = 
					mToken2String.insert(CTOKEN2STRING::value_type(lk::CString32(szToken), CString256(szString)));

				if (ret.second != true)
				{
					LOG_ERROR("default", "mToken2String insert text (%s, %s) error", szToken, szString);
				}else
				{
					LOG_DEBUG("default", "mToken2String insert text (%s, %s) ok", szToken, szString);
				}
			}
		}
	}
}

int CLogCtrl::ReadLogSvrdCfg(const char *szCfgFile)
{
	LOG_DEBUG( "logctrl", "Read Server Config Begin..." );

	TiXmlDocument tDoc;

	if( !tDoc.LoadFile( szCfgFile ) )
	{
		LOG_ERROR( "logctrl", "load xml %s failed", szCfgFile );
		return -1;
	}

	TiXmlElement* tpElement = NULL;
	TiXmlElement* tpChildElm = NULL;

	// 先得到根节点
	TiXmlElement* tpRoot = tDoc.FirstChildElement();	

	// Log1
	tpElement = tpRoot->FirstChildElement( "LOG1" );
	if( tpElement != NULL )
	{
		for( tpChildElm = tpElement->FirstChildElement( ); tpChildElm != NULL; 
			tpChildElm = tpChildElm->NextSiblingElement( ) )
		{
			const char* tpKey = tpChildElm->Attribute("KEY");
			const char* tpFile = tpChildElm->Attribute("FILE");
			const char* tpLevel = tpChildElm->Attribute( "LEVEL" );

			if( ( tpKey == NULL ) || ( tpFile == NULL ) || ( tpLevel == NULL ) )
			{
				continue;
			}

			INIT_ROLLINGFILE_LOG( tpKey, tpFile, ( LogLevel )atoi( tpLevel ) );
		}
	}

	// handle log level
	tpElement = tpRoot->FirstChildElement( "HANDLE_THREAD" );
	if( tpElement != NULL )
	{
		tpElement->Attribute("Level", (int*)&m_stLogSvrdCfg.mHandleLogLevel );	

		tpElement->Attribute("MONIT_TIMER", (int*)&m_stLogSvrdCfg.mMonitorTimer );	
	}

	// handle monitor timer
	

	tpElement = tpRoot->FirstChildElement( "TABLES" );
	if ( tpElement != NULL )
	{
		for( tpChildElm = tpElement->FirstChildElement( ); tpChildElm != NULL; tpChildElm = tpChildElm->NextSiblingElement() ) 
		{
			const char* szID = tpChildElm->Attribute("ID");
			const char* szTable = tpChildElm->Attribute("TABLE");
			const char* szNumber = tpChildElm->Attribute("NUMBER");
			const char* szHandle = tpChildElm->Attribute("HANDLE");

			if ( szID && szTable && szNumber && szHandle)
			{
				//typedef std::map <int, std::pair<int, lk::CString32> >::const_iterator const_iterator;
				typedef ID2TABLE::const_iterator const_iterator;

				IDINFO info;
				info.first = atoi(szNumber);
				info.second = lk::CString32(szTable);
				info.handle = atoi(szHandle);

				std::pair<const_iterator, bool > ret = 
					m_Id2Table.insert(std::make_pair(atoi(szID), info));

				if ( ret.second == false)
				{
					LOG_ERROR("default", "Error: Insert into ID2TABLE (%s->%s,%s,%s) failed.", szID, szTable, szNumber, szHandle);
				}else
				{
					LOG_INFO("default", "Success: Insert into ID2TABLE (%s->%s,%s,%s) ok.", szID, szTable, szNumber, szHandle);
				}
			}else
			{
				LOG_ERROR("default", "Error: Insert into ID2TABLE (%s->%s,%s,%s) failed.", szID, szTable, szNumber, szHandle);
			}
		}
	}

	tpElement = tpRoot->FirstChildElement( "LIMIT" );
	if ( tpElement != NULL )
	{
		tpChildElm = tpElement->FirstChildElement("EXP");
		if (tpChildElm != NULL)
		{
			const char* szMinRoleExp = tpChildElm->Attribute("MIN_ROLE_EXP");
			const char* szMinMWExp = tpChildElm->Attribute("MIN_MW_EXP");
			const char* szMinTalentExp = tpChildElm->Attribute("MIN_TALENT_EXP");

			szMinRoleExp == NULL ? m_nMinRoleExp = 0 : m_nMinRoleExp = atoi(szMinRoleExp);
			szMinMWExp == NULL ? m_nMinMWExp = 0 : m_nMinMWExp = atoi(szMinMWExp);
			szMinTalentExp == NULL ? m_nMinTalentExp = 0 : m_nMinTalentExp = atoi(szMinTalentExp);

			TiXmlElement* tpExcept = tpChildElm->FirstChildElement();
			for(; tpExcept != NULL; tpExcept = tpExcept->NextSiblingElement() )
			{
				const char* szExceptRole = tpExcept->Attribute("ROLEID");
				if ( szExceptRole != NULL )
				{
					m_ExpExceptRole.insert(TExpExceptDict::value_type(atoi(szExceptRole), 0));
				}
			}
		}

		tpChildElm = tpElement->FirstChildElement("ITEM");
		if (tpChildElm != NULL)
		{
			TiXmlElement* tpFilter = tpChildElm->FirstChildElement("FILTER");
			for(; tpFilter != NULL; tpFilter = tpFilter->NextSiblingElement() )
			{
				const char* szItemID = tpFilter->Attribute("ITEM_ID");
				const char* szOpType = tpFilter->Attribute("OP_TYPE");
			
				if ( szItemID != NULL && szOpType != NULL)
				{
					if ( AddFilterItem(atoi(szItemID), atoi(szOpType)))
					{
						LOG_INFO("logctrl", "Add filter item_id = %s and op_type = %s", szItemID, szOpType);
					}
				}
			}
		}

		tpChildElm = tpElement->FirstChildElement("DROP");
		if (tpChildElm != NULL)
		{
			TiXmlElement* tpFilter = tpChildElm->FirstChildElement("FILTER");
			for(; tpFilter != NULL; tpFilter = tpFilter->NextSiblingElement() )
			{
				const char* szItemID = tpFilter->Attribute("ITEM_ID");
			
				if ( szItemID != NULL )
				{
					if ( AddFilterDrop(atoi(szItemID)))
					{
						LOG_INFO("logctrl", "Add filter item_id = %s ", szItemID);
					}
				}
			}
		}
	}

	LOG_DEBUG( "logctrl", "Read Server Config End" );

    return 0;
}

int CLogCtrl::ReadLogIpCfg(const char *szIpFile)
{
	LOG_DEBUG( "logctrl", "Read IP Config Begin..." );

	TiXmlDocument tDoc;

	if( !tDoc.LoadFile( szIpFile ) )
	{
		LOG_ERROR( "logctrl", "load xml %s failed", szIpFile );
		return -1;
	}

	TiXmlElement* tpElement = NULL;
	TiXmlElement* tpChildElm = NULL;

	// 先得到根节点
	TiXmlElement* tpRoot = tDoc.FirstChildElement();

	// Mysql配置分支
	tpElement = tpRoot->FirstChildElement("MYSQL");
	if( tpElement != NULL )
	{
		const char* tpMysql = tpElement->Attribute( "mysql_info" );
		if( tpMysql != NULL )
		{
			Tokens tokens = StrSplit(tpMysql, ";");
			Tokens::iterator iter;

			std::string host, port_or_socket, user, password, database;
			int port;
			char const* unix_socket;

			iter = tokens.begin();

			if(iter != tokens.end()) host = *iter++;
			if(iter != tokens.end()) port_or_socket = *iter++;
			if(iter != tokens.end()) user = *iter++;
			if(iter != tokens.end()) password = *iter++;
			if(iter != tokens.end()) database = *iter++;

			char szUser[32+1] = {0};		
			int nUserLength = sizeof(szUser);

			char szPasswd[32+1] = {0};
			int nPasswdLength = sizeof(szPasswd);

			if ( DecryptAscIICode(user.c_str(), user.length(), szUser, &nUserLength) != true)
			{
				LOG_ERROR("default", "ERROR: Can NOT decrypt user(%s) section.", user.c_str());
				return -1;
			}

			if ( DecryptAscIICode(password.c_str(), password.length(), szPasswd, &nPasswdLength) != true)
			{
				LOG_ERROR("default", "ERROR: Can NOT decrypt user(%s) section.", user.c_str());
				return -1;
			}

			char szInputToken [256] = {0}; 
			int nResult = snprintf(szInputToken, sizeof(szInputToken)-1,  "%s;%s;%s;%s;%s", 
				host.c_str(), port_or_socket.c_str(), szUser, szPasswd, database.c_str());

			if (nResult <= 0)
			{
				LOG_ERROR("default", "ERROR: Section mysql_info is too large for requirement");
				return -1;
			}


			strncpy( m_stLogSvrdCfg.m_szMysql, szInputToken, sizeof(m_stLogSvrdCfg.m_szMysql)-1 );
			LOG_DEBUG( "logctrl", "mysql info sting: %s", m_stLogSvrdCfg.m_szMysql );
		}	
	}

	// 远程存储配置信息
	tpElement = tpRoot->FirstChildElement("REMOTE_STORAGE");
	if( tpElement != NULL )
	{
		const char* tpMysql = tpElement->Attribute( "mysql_info" );
		if( tpMysql != NULL )
		{
			Tokens tokens = StrSplit(tpMysql, ";");
			Tokens::iterator iter;

			std::string host, port_or_socket, user, password, database;
			int port;
			char const* unix_socket;

			iter = tokens.begin();

			if(iter != tokens.end()) host = *iter++;
			if(iter != tokens.end()) port_or_socket = *iter++;
			if(iter != tokens.end()) user = *iter++;
			if(iter != tokens.end()) password = *iter++;
			if(iter != tokens.end()) database = *iter++;

			char szUser[32+1] = {0};		
			int nUserLength = sizeof(szUser);

			char szPasswd[32+1] = {0};
			int nPasswdLength = sizeof(szPasswd);

			if ( DecryptAscIICode(user.c_str(), user.length(), szUser, &nUserLength) != true)
			{
				LOG_ERROR("default", "ERROR: Can NOT decrypt user(%s) section.", user.c_str());
				return -1;
			}

			if ( DecryptAscIICode(password.c_str(), password.length(), szPasswd, &nPasswdLength) != true)
			{
				LOG_ERROR("default", "ERROR: Can NOT decrypt user(%s) section.", user.c_str());
				return -1;
			}

			char szInputToken [256] = {0}; 
			int nResult = snprintf(szInputToken, sizeof(szInputToken)-1,  "%s;%s;%s;%s;%s", 
				host.c_str(), port_or_socket.c_str(), szUser, szPasswd, database.c_str());

			if (nResult <= 0)
			{
				LOG_ERROR("default", "ERROR: Section mysql_info is too large for requirement");
				return -1;
			}

			strncpy( m_stLogSvrdCfg.m_szRemoteStorage, szInputToken, sizeof(m_stLogSvrdCfg.m_szRemoteStorage)-1 );
			LOG_DEBUG( "logctrl", "remote storage mysql info sting: %s", m_stLogSvrdCfg.m_szMysql );
		}	
	}

	// log 服务器配置分支
	tpElement = tpRoot->FirstChildElement( "LOG" );
	if( tpElement != NULL )
	{
		tpChildElm = tpElement->FirstChildElement();
		if( tpChildElm != NULL )
		{	
			// Server ID
			tpChildElm->Attribute("ID", (int*)&m_stLogSvrdCfg.m_iLogSvrID );

			// IP
			const char* tpValue = tpChildElm->Attribute("IP");
			if( tpValue != NULL )
			{
				strncpy( m_stLogSvrdCfg.m_szServiceIP, tpValue, sizeof(m_stLogSvrdCfg.m_szServiceIP)-1 );
			}

			LOG_DEBUG( "logctrl", "log server info: id: %d ip:%s ",
				m_stLogSvrdCfg.m_iLogSvrID,
				m_stLogSvrdCfg.m_szServiceIP);
		}
	}

	// Proxy服务器 配置分支
	m_stLogSvrdCfg.m_sProxySvrdNum = 0; 

	tpElement = tpRoot->FirstChildElement("PROXY");
	if( tpElement != NULL )
	{
		for( tpChildElm = tpElement->FirstChildElement(); tpChildElm != NULL;  tpChildElm = tpChildElm->NextSiblingElement())
		{
			// SERVER ID
			tpChildElm->Attribute("ID", (int*)&m_stLogSvrdCfg.m_astProxySvrdCfg[m_stLogSvrdCfg.m_sProxySvrdNum].m_iServerID );

			// IP
			const char* tpValue = tpChildElm->Attribute("IP");
			if( tpValue != NULL )
			{
				strncpy( m_stLogSvrdCfg.m_astProxySvrdCfg[m_stLogSvrdCfg.m_sProxySvrdNum].m_szServiceIP,
					tpValue,
					sizeof( m_stLogSvrdCfg.m_astProxySvrdCfg[0].m_szServiceIP)-1 );
			}

			// Port
			tpValue = tpChildElm->Attribute("Port");
			if( tpValue != NULL )
			{
				strncpy( m_stLogSvrdCfg.m_astProxySvrdCfg[m_stLogSvrdCfg.m_sProxySvrdNum].m_szServicePort, 
					tpValue,
					sizeof(m_stLogSvrdCfg.m_astProxySvrdCfg[0].m_szServicePort)-1 );
			}


			LOG_DEBUG( "logctrl", "proxy server info: id: %d ip:%s port:%s",
				m_stLogSvrdCfg.m_astProxySvrdCfg[m_stLogSvrdCfg.m_sProxySvrdNum].m_iServerID,
				m_stLogSvrdCfg.m_astProxySvrdCfg[m_stLogSvrdCfg.m_sProxySvrdNum].m_szServiceIP,
				m_stLogSvrdCfg.m_astProxySvrdCfg[m_stLogSvrdCfg.m_sProxySvrdNum].m_szServicePort);

			m_stLogSvrdCfg.m_sProxySvrdNum ++;

		}
	}

	LOG_DEBUG( "logctrl", "Read IP Config End" );

	return 0;
}

int  CLogCtrl::ConnectToProxyServer()
{
    int i = 0;

    for( i = 0; i < m_stLogSvrdCfg.m_sProxySvrdNum;  i++ )
    {
        m_astProxySvrdCon[i].Initialize( CET_PROXYSERVER, m_stLogSvrdCfg.m_astProxySvrdCfg[i].m_iServerID,
            inet_addr(m_stLogSvrdCfg.m_astProxySvrdCfg[i].m_szServiceIP), atoi(m_stLogSvrdCfg.m_astProxySvrdCfg[i].m_szServicePort));
        if( m_astProxySvrdCon[i].ConnectToServer(m_stLogSvrdCfg.m_szServiceIP))
        {
            LOG_INFO( "default", "Error:connect to Proxy Server %d failed.\n", m_stLogSvrdCfg.m_astProxySvrdCfg[i].m_iServerID);
            continue;
        }

		if( RegisterToProxyServer( &(m_astProxySvrdCon[i]) ) )
        {
            LOG_ERROR( "default", "Error: Register to Proxy Server %d failed.\n", m_stLogSvrdCfg.m_astProxySvrdCfg[i].m_iServerID);
            continue;
        }
        time(&m_atLastPorxyKeepAliveTime[i]);
        LOG_INFO( "default", "Connect to Proxy server %d Succeed.\n", m_stLogSvrdCfg.m_astProxySvrdCfg[i].m_iServerID );
    }

    return i;
}

/********************************************************
  Function:     RegisterToProxyServer
  Description:  向proxy发送注册消息
  Input:          pstTcpConn:  连接指针
  Output:      
  Return:       0 :   成功 ，其他失败
  Others:		
********************************************************/
int CLogCtrl::RegisterToProxyServer(CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH> *pstTcpConn)
{
	if(!pstTcpConn)
	{
		LOG_ERROR( "default", "register to Proxy Server null para.");
		return -1;
	}

	CProxyHead stProxyHead;
	char acCodeBuf[MAXPROXYCODELEN] = {0};
	short shCodeLength = sizeof(acCodeBuf);

	stProxyHead.set_srcfe( CET_OTHERSERVER );
	stProxyHead.set_srcid( m_stLogSvrdCfg.m_iLogSvrID );
	stProxyHead.set_dstfe( CET_PROXYSERVER ) ;
	stProxyHead.set_transfertype( TRANS_P2P );
	stProxyHead.set_opflag( CMD_REGSERVER );

	// 空出头部4bytes( ushort , ushort )放长度数据
	bool bSerializeRet = stProxyHead.SerializeToArray( acCodeBuf+4, shCodeLength );
	if( bSerializeRet != true )
	{
		LOG_ERROR( "default", "Encode ProxyHead Failed While RegisterToProxyServer." );
		return -1;
	}
	unsigned short ushProxyHeadLen = stProxyHead.ByteSize() ;
	shCodeLength = 2  + ushProxyHeadLen + 2;

	*((unsigned short*)acCodeBuf) =shCodeLength;
	*((unsigned short*)(acCodeBuf+2)) = ushProxyHeadLen;

	pstTcpConn->GetSocket()->SendOneCode(shCodeLength, (BYTE *)acCodeBuf);
	
	return 0;
}

/********************************************************
  Function:     SendkeepAliveToProxy
  Description:  向proxy发送心跳消息
  Input:          pstTcpConn:  连接指针
  Output:      
  Return:       0 :   成功 ，其他失败
  Others:		
********************************************************/
int CLogCtrl::SendkeepAliveToProxy(CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH> *pstTcpConn)
{
	if(!pstTcpConn)
	{
		LOG_ERROR( "logctrl", "Error:send keey alive to Proxy null para.\n");
		return -1;
	}

	CProxyHead  tHead;
	char message_buffer[MAXPROXYCODELEN];
	pbmsg_setproxy(&tHead, CET_OTHERSERVER, CET_PROXYSERVER, m_stLogSvrdCfg.m_iLogSvrID, 1, TRANS_P2P, CMD_KEEPALIVE);

	unsigned short message_length = tHead.ByteSize() + 4;
	*((unsigned short*) message_buffer)  = message_length;

	*((unsigned short*) (message_buffer + 2))  = tHead.ByteSize();

	if(tHead.SerializeToArray(message_buffer+4, message_length - 4))
	{
		pstTcpConn->GetSocket()->SendOneCode(message_length, (BYTE *)message_buffer);
	}else
	{
		LOG_ERROR("default", " [ %s : %d ][ %s ] Encode Failed ", __LK_FILE__, __LINE__, __FUNCTION__);
		return -1;
	}

	return 0;
}

// ***************************************************************
//  Function: 	DisPatchOneCode   
//  Description:分派一个消息
//  Date: 		04/21/2009
// 
// ***************************************************************
int CLogCtrl::DispatchOneCode(int iProxyIdx, int nCodeLength, BYTE* pbyCode)
{
    int iHandleChoice = -1;
    int iTempRet = 0;

    if( iProxyIdx < 0 || iProxyIdx >= m_stLogSvrdCfg.m_sProxySvrdNum || nCodeLength < MINPROXYMSGLEN || !pbyCode )
    {
        LOG_ERROR( "logctrl", "Invalid input in CLogCtrl::DispatchOneCode.\n");
        return -1;
    }

	// 只解CProxyHead部分 ( totallen , proxyheadlen, cproxyhead ), 注意头4字节被赋值成了 proxy idx,供loghandle使用,跳过
	unsigned short usProxyHeadLen = *((unsigned short*)(pbyCode+sizeof(int)+2));
	CProxyHead tProxyHead;
	bool bParseRet = tProxyHead.ParseFromArray( pbyCode+4+sizeof(int), usProxyHeadLen );
	if( bParseRet != true )
	{
		LOG_ERROR( "logctrl", "Decode ProxyHead faild." );
		return -1;
	}

	//加入proxy命令处理
	if( CET_PROXYSERVER == tProxyHead.srcfe())
	{
		//处理心跳消息
		if( CMD_KEEPALIVE == tProxyHead.opflag() )
		{
			time(&m_atLastPorxyKeepAliveTime[iProxyIdx]);
			return 0;
		}
	}

    //加入proxy命令处理
    switch( tProxyHead.transfertype() )
    {
		case TRANS_P2P:
        case TRANS_BYKEY:
        {
            //g_stServerStatisticForPrint.m_iRecvMsgNumTransByKey++;

            //交给指定的Handle处理
            //iHandleChoice = tProxyHead.uin() % MAXHANDLENUMBER;
			ID2TABLE::const_iterator iter = m_Id2Table.find(tProxyHead.uin());

			if ( iter != m_Id2Table.end())
			{
            	iHandleChoice = iter->second.handle;
			}

			if ( iHandleChoice > MAXHANDLENUMBER || iHandleChoice < 0 || m_apHandles[iHandleChoice] == NULL) return -4;

            iTempRet = m_apHandles[iHandleChoice]->PostOneCode(nCodeLength, pbyCode);

            LOG_DEBUG( "logctrl", "Post code (%d) to handle %d returns %d.", tProxyHead.uin(), iHandleChoice, iTempRet);

            if( iTempRet >= 0 )
            {
                m_apHandles[iHandleChoice]->WakeUp();
            }
            else
            {
                g_stServerStatisticForPrint.m_iPostCodeFailedTimes++;
            }
            break;
        }
        case TRANS_BROADCAST:
        {
            //g_stServerStatisticForPrint.m_iRecvMsgNumTransBroadCast++;
            ////交给所有Handle处理
            //for( iHandleChoice = 0; iHandleChoice < MAXHANDLENUMBER; iHandleChoice++ )
            //{
            //    iTempRet = m_apHandles[iHandleChoice]->PostOneCode(nCodeLength, pbyCode);
            //    if( iTempRet >= 0 )
            //    {
            //        m_apHandles[iHandleChoice]->WakeUp();
            //    }
            //    else
            //    {
            //        g_stServerStatisticForPrint.m_iPostCodeFailedTimes++;
            //    }
            //}
            ////增加自己处理关心的消息
            //short nHeadLength = stProxyHead.m_nTotalLength - stProxyHead.m_nBodyLength;
            //ProcessOneCode(pcTempCode+nHeadLength, stProxyHead.m_nBodyLength);
            //iTempRet = 0;
            break;
        }
        default:
        {
            LOG_ERROR( "logctrl", "In CLogCtrl::DispatchOneCode, invalid trans type %d.\n", tProxyHead.transfertype() );
            return -3;
        }
    }

    return iTempRet;
}

//处理ctrl自己关心的消息
int CLogCtrl::ProcessOneCode(char *pacCode, int shCodeLength)
{
    CMessage stTempMsg;
    int iTempRet = 0;

    if((shCodeLength <= 0) || (NULL == pacCode))
    {
        LOG_ERROR( "logctrl", "In ProcessOneCode, invalid input para.\n");
        return -1;
    }

    iTempRet = Event( &stTempMsg );

    return iTempRet;
}

int CLogCtrl::Event(CMessage *pstMsg)
{
    int iTempRet = 0;

    if(NULL == pstMsg)
    {
        LOG_ERROR( "logctrl", "In Event, invalid input para.\n");
        return -1;
    }


    return iTempRet;
}

int CLogCtrl::CheckRunFlags()
{	
	int i;

    if( IsRunFlagSet( EFLG_CTRL_RELOAD ) )
    {
        ReadLogIpCfg( "../config/ip.xml" );
		ReadLogSvrdCfg( "../config/data/logserver.xml" );
        for( i = 0; i < MAXHANDLENUMBER; i++ )
        {
			if (m_apHandles[i] == NULL) continue;

            m_apHandles[i]->ProcessThreadEnd();
			m_apHandles[i]->InitLogFile( NULL, NULL, (LogLevel)m_stLogSvrdCfg.mHandleLogLevel );
        }
        ClearRunFlag( EFLG_CTRL_RELOAD );
    }

    if( ! IsRunFlagSet( EFLG_CTRL_QUIT ) )
    {
        return 0;
    }


    //停止所有的线程
    for( i = 0; i < MAXHANDLENUMBER; i++ )
    {
		if (m_apHandles[i] == NULL) continue;

        m_apHandles[i]->ProcessThreadEnd();
        m_apHandles[i]->StopThread();
    }

	LOG_NOTICE( "default", "xyj-logserver exit !");

    exit(0);

    return 0;
}

int CLogCtrl::CheckAndDispatchInputMsg()
{
    fd_set fds_read;
    struct timeval stMonTime;
    int i, iTempFD = -1, iTempStatus = tcs_closed;
    int iOpenFDNum = 0;
    int iCount = 0;
    socklen_t iSockAddrSize;
    unsigned short nTmpCodeLength;
    BYTE  abyCodeBuf[MAX_SERVER_TRANS_LEN];
    char szTempIP[30];

    FD_ZERO(&fds_read);
    stMonTime.tv_sec = 0;
    stMonTime.tv_usec = 10000;

	for(i = 0; i < m_stLogSvrdCfg.m_sProxySvrdNum; i++)
	{
		iTempFD = m_astProxySvrdCon[i].GetSocket()->GetSocketFD();
		iTempStatus = m_astProxySvrdCon[i].GetSocket()->GetStatus();

		if(iTempFD > 0 && iTempStatus == tcs_connected)
		{
			FD_SET(iTempFD, &fds_read);
		}
	}

    iOpenFDNum = select(FD_SETSIZE, &fds_read, NULL, NULL, &stMonTime);

    if(iOpenFDNum <= 0)
    {
        return 0;
    }


	for(i = 0; i < m_stLogSvrdCfg.m_sProxySvrdNum; i++)
	{
		iTempFD = m_astProxySvrdCon[i].GetSocket()->GetSocketFD();
		if(iTempFD < 0)
		{
			continue;
		}


		if(FD_ISSET(iTempFD, &fds_read))
		{
			// LOG_INFO( "logctrl", "Proxy(%d) had req to process.", m_astProxySvrdCon[i].GetEntityID());
			m_astProxySvrdCon[i].GetSocket()->RecvData();
			while(1)
			{
				nTmpCodeLength = sizeof(abyCodeBuf) - sizeof(int);
				if(!(m_astProxySvrdCon[i].GetSocket()->GetOneCode(nTmpCodeLength, (BYTE *)&abyCodeBuf[sizeof(int)]) > 0))
				{
             		  break;
				}

				LOG_DEBUG( "logctrl", "Get one code from ( EntityType:%d, EntityID:%d) Len=%d, dispatch it",
					m_astProxySvrdCon[i].GetEntityType(),
					m_astProxySvrdCon[i].GetEntityID(),
					nTmpCodeLength);
				memcpy((void *)abyCodeBuf, (const void *)&i, sizeof(int));
				nTmpCodeLength += sizeof(int);
				DispatchOneCode(i, nTmpCodeLength, (BYTE *)abyCodeBuf);
				iCount++;
			}

		}
    }	


    return iCount;
}

/*
int CDBCtrl::CreateServer()
{
    short unTempPort = (u_short)atoi((const char *)m_stDBSvrdCfg.m_szServicePort);
    if(m_stServerConn.Initialize(cet_dbsvr, m_stDBSvrdCfg.m_iDBSvrID,   inet_addr(m_stDBSvrdCfg.m_szServiceIP), unTempPort) < 0)
    {
        SAY("Error in CDBCtrl::CreateServer  Initialize Conn  failed\n");
        return -1;
    }
    return m_stServerConn.CreateServer();
}*/

int CLogCtrl::PrepareToRun()
{
    int i;

    if(ConnectToProxyServer() < 0)
    {
		LOG_ERROR( "logctrl", "Error: in CLogCtrl::PrepareToRun connect proxy  server  failed, try to reconnect later ... \n");
		printf( "Error: connect proxy  server  failed, try to reconnect later ... \n" );      
		//return -1;
    }
	
    //初始化连接DB的互斥变量
    pthread_mutex_init( &CLogHandle::stLogConnMutex, NULL );

    for(i = 0; i < MAXHANDLENUMBER; i++)
    {	
		if (m_apHandles[i] == NULL) continue;
		
        m_apHandles[i]->Initialize(i, &m_astProxySvrdCon[0], GetLogSvrConf().m_szMysql );
		m_apHandles[i]->InitLogFile( NULL, NULL, (LogLevel)m_stLogSvrdCfg.mHandleLogLevel );
    }
	


    LOG_INFO( "logctrl", "Handles initialized OK, now begin to create threads.\n");
    for(i = 0; i < MAXHANDLENUMBER; i++)
    {
		if (m_apHandles[i] == NULL) continue;

        if(m_apHandles[i]->CreateThread())
        {
            LOG_ERROR( "logctrl", "Create handle thread %d failed.\n", i);
            return -1;
        }
    }

    LOG_INFO( "logctrl", "Successfully create %d threads to handle request.\n", i);

    return 0;
}

int CLogCtrl::Run()
{
    while(True)
    {
        CheckRunFlags();

        RoutineCheck();

        CheckAndDispatchInputMsg();

		OnTime();

        g_stServerStatisticForPrint.m_iCtrlLoopNum++;
    }

    return 0;
}

const STLogSvrdCfg CLogCtrl::GetLogSvrConf()
{
    return m_stLogSvrdCfg;
}

int CLogCtrl::RoutineCheck()
{
	static time_t iLastCheckTime = time(NULL);
	time_t tNow = time(NULL);

	if(tNow - iLastCheckTime < CHECKINTERVAL )
	{
		return 0;
	}

	iLastCheckTime = tNow;

	// 和proxy保持心跳
	for(int i = 0; i < m_stLogSvrdCfg.m_sProxySvrdNum; i++)
	{
		if( (m_astProxySvrdCon[i].GetSocket()->GetStatus() == tcs_connected) 
			&& (m_astProxySvrdCon[i].GetSocket()->GetSocketFD() > 0)
			&& (tNow - m_atLastPorxyKeepAliveTime[i] < PROXYKEEPALIVESECONDS))
		{
			SendkeepAliveToProxy(&(m_astProxySvrdCon[i]));
			continue;
		}

		LOG_ERROR("default", "Proxy[%d](ID = %d) is not connected, try to reconnect it",i, m_astProxySvrdCon[i].GetEntityID());

		// 如果已经断开了， 则重新连接
		if(m_astProxySvrdCon[i].ConnectToServer(m_stLogSvrdCfg.m_szServiceIP))
		{
			LOG_ERROR("default", "Connect proxy[%d] failed.", i);
			continue;
		}

		// 然后注册
		if(RegisterToProxyServer(&(m_astProxySvrdCon[i])))
		{
			LOG_ERROR("default", "Register proxy[%d] failed.", i);
			continue;
		}

		time(&m_atLastPorxyKeepAliveTime[i]);
		LOG_INFO("default", "Connect to proxy %d succeeded.", i);
	}



	return 0;
}


int CLogCtrl::PrintServerStatistic()
{
    //FILE *fpStatistic = NULL;
    //char fname[128];
    //TStruTime stTempNow;
    //char szCurDate[32];
    //char szCurDateTime[32];
    //char szCurAddr[32];
    //char szCurMon[32];
    //char szStatisticDir[64];
    //int iAveHandleTime;
    //int i, iTotal;

    //GetCurStructTime( &stTempNow );
    //snprintf(szCurMon, sizeof(szCurMon)-1,"%04d-%02d", stTempNow.m_iYear, stTempNow.m_iMon);
    //snprintf(szCurDate,sizeof(szCurDate)-1, "%04d-%02d-%02d", stTempNow.m_iYear, stTempNow.m_iMon, stTempNow.m_iDay);
    //snprintf(szCurDateTime, sizeof(szCurDateTime)-1,"%04d-%02d-%02d %02d:%02d:%02d", stTempNow.m_iYear, stTempNow.m_iMon, stTempNow.m_iDay, stTempNow.m_iHour, stTempNow.m_iMin, stTempNow.m_iSec);
    //snprintf(szStatisticDir,sizeof(szStatisticDir)-1, "../statistic/%s", szCurMon);

    //snprintf( fname, sizeof(fname)-1,"%s/dbs-%s.txt", szStatisticDir, szCurDate);
    //DIR* dpStatistic = opendir( szStatisticDir);
    //if ( !dpStatistic )
    //{
    //    if ( mkdir(szStatisticDir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) )
    //    {
    //        TLib_Log_LogMsg("in CDBCtrl::PrintServerStatistic, can't create bill dir %s.\n", szStatisticDir);
    //        return Wrong;
    //    }
    //}
    //else
    //{
    //    closedir( dpStatistic);
    //}
    //fpStatistic = fopen( fname , "a+" );

    //if ( !fpStatistic )
    //{
    //    TLib_Log_LogMsg("Open statistic file %s failed.\n", fname);
    //    return Wrong;
    //}
    //fprintf(fpStatistic, "\n=======================%s======================\n", szCurDateTime);
    ////打印系统统计信息
    //fprintf(fpStatistic, "CtrlLoopNum     :       %8d\n"
    //                     "ReceiveMsgNum   :       %8d        Size :       %d\n"
    //                     "SendMsgNum      :       %8d        Size :       %d\n"
    //                     "ReceiveMsgNumTransByKey         :      %8d\n"
    //                     "ReceiveMsgNumTransBroadCast     :      %8d\n",
    //                 g_stServerStatisticForPrint.m_iCtrlLoopNum, 
    //                 g_stServerStatisticForPrint.m_iRecvMsgNum, g_stServerStatisticForPrint.m_iRecvPkgSize,
    //                 g_stServerStatisticForPrint.m_iSendMsgNum, g_stServerStatisticForPrint.m_iSendPkgSize, 
    //                 g_stServerStatisticForPrint.m_iRecvMsgNumTransByKey,
    //                 g_stServerStatisticForPrint.m_iRecvMsgNumTransBroadCast);

    //fprintf(fpStatistic, "InsertNum     :          %d\n"
    //                     "UpdateNum     :          %d\n"
    //                     "SelectNum     :          %d\n"
    //                     "DeleteNum     :          %d\n",
    //                g_stServerStatisticForPrint.m_iInsertNum,
    //                g_stServerStatisticForPrint.m_iUpdateNum,
    //                g_stServerStatisticForPrint.m_iSelectNum,
    //                g_stServerStatisticForPrint.m_iDeleteNum);

    //iAveHandleTime = 0;
    //if(g_stServerStatisticForPrint.m_iSyncInsertNum > 0)
    //{
    //    iAveHandleTime = 1000000 * g_stServerStatisticForPrint.m_tSyncInsertTime.tv_sec;
    //    iAveHandleTime += g_stServerStatisticForPrint.m_tSyncInsertTime.tv_usec;
    //    iAveHandleTime /= g_stServerStatisticForPrint.m_iSyncInsertNum;
    //}
    //fprintf(fpStatistic, "SyncInsertNum   :%8d, CostTime:%d.%06d(sec), AVE:%u(usec) \n",
    //                g_stServerStatisticForPrint.m_iSyncInsertNum,
    //                g_stServerStatisticForPrint.m_tSyncInsertTime.tv_sec,
    //                g_stServerStatisticForPrint.m_tSyncInsertTime.tv_usec,
    //                iAveHandleTime);

    //iAveHandleTime = 0;
    //if(g_stServerStatisticForPrint.m_iSyncUpdateNum > 0)
    //{
    //    iAveHandleTime = 1000000 * g_stServerStatisticForPrint.m_tSyncUpdateTime.tv_sec;
    //    iAveHandleTime += g_stServerStatisticForPrint.m_tSyncUpdateTime.tv_usec;
    //    iAveHandleTime /= g_stServerStatisticForPrint.m_iSyncUpdateNum;
    //}
    //fprintf(fpStatistic, "SyncUpdateNum   :%8d, CostTime:%d.%06d(sec), AVE:%u(usec) \n",
    //                g_stServerStatisticForPrint.m_iSyncUpdateNum,
    //                g_stServerStatisticForPrint.m_tSyncUpdateTime.tv_sec,
    //                g_stServerStatisticForPrint.m_tSyncUpdateTime.tv_usec,
    //                iAveHandleTime);

    //iAveHandleTime = 0;
    //if(g_stServerStatisticForPrint.m_iSyncSelectNum > 0)
    //{
    //    iAveHandleTime = 1000000 * g_stServerStatisticForPrint.m_tSyncSelectTime.tv_sec;
    //    iAveHandleTime += g_stServerStatisticForPrint.m_tSyncSelectTime.tv_usec;
    //    iAveHandleTime /= g_stServerStatisticForPrint.m_iSyncSelectNum;
    //}
    //fprintf(fpStatistic, "SyncSelectNum   :%8d, CostTime:%d.%06d(sec), AVE:%u(usec) \n",
    //                g_stServerStatisticForPrint.m_iSyncSelectNum,
    //                g_stServerStatisticForPrint.m_tSyncSelectTime.tv_sec,
    //                g_stServerStatisticForPrint.m_tSyncSelectTime.tv_usec,
    //                iAveHandleTime);

    //iAveHandleTime = 0;
    //if(g_stServerStatisticForPrint.m_iSyncDeleteNum > 0)
    //{
    //    iAveHandleTime = 1000000 * g_stServerStatisticForPrint.m_tSyncDeleteTime.tv_sec;
    //    iAveHandleTime += g_stServerStatisticForPrint.m_tSyncDeleteTime.tv_usec;
    //    iAveHandleTime /= g_stServerStatisticForPrint.m_iSyncDeleteNum;
    //}
    //fprintf(fpStatistic, "SyncDeleteNum   :%8d, CostTime:%d.%06d(sec), AVE:%u(usec) \n",
    //                g_stServerStatisticForPrint.m_iSyncDeleteNum,
    //                g_stServerStatisticForPrint.m_tSyncDeleteTime.tv_sec,
    //                g_stServerStatisticForPrint.m_tSyncDeleteTime.tv_usec,
    //                iAveHandleTime);

    //fprintf(fpStatistic, "DecodeProxyMsgFailedTimes   :          %d\n"
    //                     "EncodeProxyMsgFailedTimes   :          %d\n"
    //                     "GetCodeFailedTimes          :          %d\n"
    //                     "PostCodeFailedTimes         :          %d\n",
    //                g_stServerStatisticForPrint.m_iDecodeProxyMsgFailedTimes,
    //                g_stServerStatisticForPrint.m_iEncodeProxyMsgFailedTimes,
    //                g_stServerStatisticForPrint.m_iGetCodeFailedTimes,
    //                g_stServerStatisticForPrint.m_iPostCodeFailedTimes);

    //iAveHandleTime = 0;
    //if(g_stServerStatisticForPrint.m_iExecSqlAsyncSuccNum > 0)
    //{
    //    iAveHandleTime = 1000000 * g_stServerStatisticForPrint.m_tExecSqlAsyncSuccTime.tv_sec;
    //    iAveHandleTime += g_stServerStatisticForPrint.m_tExecSqlAsyncSuccTime.tv_usec;
    //    iAveHandleTime /= g_stServerStatisticForPrint.m_iExecSqlAsyncSuccNum;
    //}
    //fprintf(fpStatistic, "ExecSqlAsyncFailedNum :%d\n"
		  //       "ExecSqlAsyncSuccNum   :%d, CostTime:%d.%06d(sec), AVE:%u(usec) \n",
    //                g_stServerStatisticForPrint.m_iExecSqlAsyncFailedNum,
    //                g_stServerStatisticForPrint.m_iExecSqlAsyncSuccNum,
    //                g_stServerStatisticForPrint.m_tExecSqlAsyncSuccTime.tv_sec,
    //                g_stServerStatisticForPrint.m_tExecSqlAsyncSuccTime.tv_usec,
    //                iAveHandleTime);

    //fclose(fpStatistic);
    return 0;
}

int CLogCtrl::OnTime( )
{
	// time_t (秒)
	time_t tCurTime;
	tCurTime = time( &tCurTime );

	if ( (tCurTime - m_tLastSaveDataTime) >= SAVE_DATA_CIRCLE )
	{
		// 打印统计信息
		PrintStatistic();

		// 通知各HANDLE,记录事件
		EvtTimeOut( tCurTime );

		CreateLogDB( tCurTime );
	}

}

bool CLogCtrl::EvtTimeOut( time_t tCurTime )
{
	// 给各HANDLE发包,触发timeout事件
	CMessage tMessage;
	CMessageHandleTimeOut tTimeOutMsg;

	tMessage.set_msgpara( (uint32)&tTimeOutMsg );
	tMessage.mutable_msghead()->set_messageid( ID_L2L_HANDLETIMEOUT );

	CProxyHead tProxyHead;

	BYTE buffer[ MAX_CODE_LEN ];
	unsigned short len = sizeof(buffer);

	pbmsg_setproxy(&tProxyHead, CET_GAMESERVER, CET_OTHERSERVER, 1, 4, TRANS_P2P, CMD_NULL);

	// logctrl在收到proxy发过来的消息后,会把收到的BUFF偏移4个字节做为proxy idx用
	// 这里处理构造buff时也需要偏移4个字节
	len = pbmsg_setmsg<CProxyHead>( tProxyHead, tMessage, (char*)&buffer[sizeof(int)], len );

	len += sizeof(int);

	for ( int nHandle = 0; nHandle < ARRAY_CNT( m_apHandles ); nHandle++ )
	{
		if (m_apHandles[nHandle] == NULL) continue;

		// 投递消息
		m_apHandles[nHandle]->PostOneCode(len, buffer);
		// 唤醒线程
		m_apHandles[nHandle]->WakeUp();
	}

	// 重置计时
	m_tLastSaveDataTime = tCurTime;
}



int CLogCtrl::PrintStatistic()
{
	FILE *fpStatistic = NULL;
	char fname[128];
	TStruTime stTempNow;
	char szCurDate[32];
	char szCurDateTime[32];
	char szCurMon[32];
	char szStatisticDir[64];

	GetStructTime( time(NULL), &stTempNow );

	snprintf(szCurMon, sizeof(szCurMon)-1, "%04d-%02d", stTempNow.m_iYear, stTempNow.m_iMon);
	snprintf(szCurDate, sizeof(szCurDate)-1,"%04d-%02d-%02d", stTempNow.m_iYear, stTempNow.m_iMon, stTempNow.m_iDay);
	snprintf(szCurDateTime, sizeof(szCurDateTime)-1,"%04d-%02d-%02d %02d:%02d:%02d", stTempNow.m_iYear, stTempNow.m_iMon, stTempNow.m_iDay, stTempNow.m_iHour, stTempNow.m_iMin, stTempNow.m_iSec);
	snprintf(szStatisticDir, sizeof(szStatisticDir)-1,"../statistic/%s", szCurMon);

	snprintf( fname, sizeof(fname)-1,"%s/log-%s.txt", szStatisticDir, szCurDate);
	DIR* dpStatistic = opendir( szStatisticDir);
	if ( !dpStatistic )
	{
		if ( mkdir(szStatisticDir, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) )
		{
			return -1;
		}
	}
	else
	{
		closedir( dpStatistic);
	}
	fpStatistic = fopen( fname , "a+" );

	if ( !fpStatistic )
	{
		return -1;
	}

	fprintf(fpStatistic, "\n=======================%s======================\n", szCurDateTime);

	for(int iHd =0 ; iHd < MAXHANDLENUMBER; ++iHd)
	{
		for( int iMsgID = BASE_LOGMSG_ID; iMsgID < BASE_LOGMSG_ID+MAX_LOGMSG_TYPE; iMsgID++ )
		{
			CLogStat* pLogStat = m_apHandles[iHd]->GetLogStat( iMsgID );
			if( pLogStat == NULL )
			{
				continue;
			}

			m_tLogStat[iMsgID-BASE_LOGMSG_ID] = m_tLogStat[iMsgID-BASE_LOGMSG_ID] + *pLogStat;
		}
	}


	for( ID2TABLE::iterator it = m_Id2Table.begin(); it != m_Id2Table.end(); ++it )
	{	
		unsigned int unMsgID2Index = it->first - BASE_LOGMSG_ID;

		fprintf(fpStatistic,"Log:%32s RecvMsg:%8u Insert Num:%8u Time:%4ds %6dus\n",
			(it->second).second.c_str(), 
			m_tLogStat[ unMsgID2Index ].m_uiRecvNum,
			m_tLogStat[ unMsgID2Index ].m_uiInsertNum,
			m_tLogStat[ unMsgID2Index ].m_tSyncInsertTime.tv_sec,
			m_tLogStat[ unMsgID2Index ].m_tSyncInsertTime.tv_usec);
	}

	fclose(fpStatistic);

	memset(&m_tLogStat, 0, sizeof(m_tLogStat));

	return 0;
}

int CLogCtrl::CreateLogDB( time_t time )
{
	static time_t old_time = 0;
	char command[ 128 ] = { 0 };

	Tokens tToken = StrSplit( m_stLogSvrdCfg.m_szMysql, ";" );

	if( IsANewDay( old_time, time ) == true )
	{
		system( "chmod 755 ../config/data/create_logtable_today.sh" );
		system( "chmod 755 ../config/data/create_logtable.sh" );
		system( "chmod 755 ../config/data/create_logtable_tomorrow.sh" );
		snprintf( command, sizeof( command ), "../config/data/create_logtable_today.sh"
			" ../config/data/create_logtable.sh %s %s %s %s %s", tToken[ 0 ].c_str( ), tToken[ 1 ].c_str( ), 
			tToken[ 2 ].c_str( ), tToken[ 3 ].c_str( ), tToken[ 4 ].c_str( ) );
		system( command );
		snprintf( command, sizeof( command ), "../config/data/create_logtable_tomorrow.sh"
			" ../config/data/create_logtable.sh %s %s %s %s %s", tToken[ 0 ].c_str( ), tToken[ 1 ].c_str( ),
			tToken[ 2 ].c_str( ), tToken[ 3 ].c_str( ), tToken[ 4 ].c_str( ) );
		system( command );
	}
	
	old_time = time;
	
	return 0;
}



