#include "ibstoremodule.h"
#include "tinyxml.h"
#include "gmmessage_pb.hxx.pb.h"
#include "servermessage_in.h"
#include "gatelogic.h"
#include "gateobj_manager.h"
#include "sequence.h"
#include "core_service.h"
#include "activity.h"

int CalGoodsFlag(const char* szFlag);
bool CIbStoreManager::ExchangeIbItemPos(unsigned int nItemID, CLocation& tLocation)
{
	CGoods* pIbItem = GetGoods(nItemID);
	if ( pIbItem == NULL ) 
	{
		LOG_ERROR("ibtrans", "can not find ibitem(%d)", nItemID);
		return false;
	}

	CGoods* pOldIbItem = GetGoods(tLocation);

	// 如果不存在直接占用
	if (pOldIbItem == NULL)
	{
		pIbItem->SetLocation(tLocation);
		return true;
	}

	// 如果位置上已经有pOldItem, 则交换位置
	pOldIbItem->SetLocation(pIbItem->GetLocation());
	pIbItem->SetLocation(tLocation);

	return true;
}

void CIbStoreManager::Initialize()
{
	for(size_t i = 0; i < ARRAY_CNT(m_pbIbStore); ++ i) 
	{
		m_pbIbStore [i] = NULL;
	}

	// initialize
	for(size_t i=0;i < ARRAY_CNT(m_pbFocus); ++ i)
	{
		m_pbFocus[i] = NULL;
	}

	m_tGoods.initailize();
	m_tPromotion.initailize();
	m_tFuncConsume.initailize();
}

// insert CGoods to m_tGoods
bool CIbStoreManager::InsertGoods(CGoods* pIbItem)
{
	LK_ASSERT(pIbItem != NULL, return false);
	return (m_tGoods.insert(value_type(pIbItem->GetGoodsID(), *pIbItem))).second;
}

// insert promotion 
bool CIbStoreManager::InsertPromotion(CPromotion& rPromotion)
{
	return (m_tPromotion.insert(promotion_t::value_type(rPromotion.GetGoodsID(), rPromotion))).second;
}

// insert FuncConsume
bool CIbStoreManager::InsertFuncConsume(CFuncConsume& rFuncComsume )
{
	return ( m_tFuncConsume.insert(funcconsum_t::value_type(rFuncComsume.GetFuncID(), rFuncComsume))).second;
}

// Get CGoods by nItemID
CGoods* CIbStoreManager::GetGoods(int nItemID)
{
	iterator iter = m_tGoods.find(nItemID);
	return (iter != m_tGoods.end() ) ? &(iter->second) : NULL;
}

// get promotion
CPromotion* CIbStoreManager::GetPromotion(int nGoodsID)
{
	promotion_t::iterator iter = m_tPromotion.find(nGoodsID);
	return (iter != m_tPromotion.end() ) ? &(iter->second) : NULL;
}

//get funcconsume
CFuncConsume* CIbStoreManager::GetFuncConsume( int nFuncConsumeID )
{
	funcconsum_t::iterator iter = m_tFuncConsume.find( nFuncConsumeID );
	return (iter != m_tFuncConsume.end() ) ?&(iter->second ): NULL; 
}
CGoods* CIbStoreManager::GetGoods(CLocation& tLocation)
{
	iterator iter = m_tGoods.begin();
	for( ; iter != m_tGoods.end(); ++iter)
	{
		const CLocation& p = iter->second.GetLocation();

		if (p.GetPage() == tLocation.GetPage() 
			&&  p.GetRow() == tLocation.GetRow()
			&&  p.GetCol() == tLocation.GetCol())
		{
			return &(iter->second);
		}
	}

	return NULL;
}

// load m_tGoods from xmlfile
void CIbStoreManager::Load4Xml(const char* xmlfile, bool bReloadFlag)
{
	TiXmlDocument xmldoc;
	xmldoc.LoadFile( xmlfile );
	enum  { em_golden_unit = 5, em_silver_unit = 4 };

	TiXmlElement* tpRoot= xmldoc.FirstChildElement();
	if (tpRoot == NULL)
	{
		LOG_WARN("default", "load items from xmlfile(%s) failure.", xmlfile);
		return;
	}

	// load section ibitems
	TiXmlElement* pElemItems = tpRoot->FirstChildElement("items");
	if ( pElemItems == NULL )
	{
		LOG_WARN("default", "Can NOT Find itmes seciont in file %s. ", xmlfile);
		return;
	}

	TiXmlElement* pElemGoods = pElemItems->FirstChildElement("goods"); 
	for ( ; pElemGoods != NULL; pElemGoods = pElemGoods->NextSiblingElement())
	{
		CGoods goods;

		// id
		const char* p = pElemGoods->Attribute("id");
		if(p) goods.SetGoodsID( atoi(p)); else continue;

		// count
		p = pElemGoods->Attribute("count");
		goods.SetGoodsCount(p ? atoi(p) : -1);

		// item_id
		p = pElemGoods->Attribute("item_id");
		if(p) goods.SetItemID( atoi(p)); else continue;

		// item_count
		p = pElemGoods->Attribute("item_count");
		goods.SetItemCount(p ? atoi(p) : 1);

		// price unit 
		p = pElemGoods->Attribute("unit");
		if(p) goods.SetPriceUnit(atoi(p)); else continue;

		// price value
		p = pElemGoods->Attribute("price");
		if(p) goods.SetPriceValue(atoi(p)); else continue;

		// discount 
		p = pElemGoods->Attribute("discount");
		if(p) goods.SetDiscount(atoi(p)); else continue;

		// expiry_time
		p = pElemGoods->Attribute("expiry_date");
		if(p) goods.SetExpiryTime(atoi(p)); else continue;

		// active
		p = pElemGoods->Attribute("active");
		if(p) goods.SetActive( atoi(p) ? true : false); else continue;

		// limit
		p = pElemGoods->Attribute("limit");
		goods.SetLimitPerChar(p ? atoi(p) : 0);

		char* pv = NULL;
		if ( p && (pv = strchr(p, ',')) != NULL)
		{
			goods.SetLimitType(atoi(pv+1));
		}else
		{
			goods.SetLimitType(0);
		}

		// is_package 
		p = pElemGoods->Attribute("ispackage");
		goods.SetIsPackage( p ? atoi(p) == 1 : false);

		// flag
		p = pElemGoods->Attribute("flag");
		goods.SetFlag( p ? CalGoodsFlag(p) : 0);

		if (InsertGoods(&goods) != true)
		{
			LOG_ERROR("ibtrans", "load ibitem [ID:%d, ItemID:%d ItemCount: %d Price:%d Unit:%d ] error",
				goods.GetGoodsID(), goods.GetItemID(), goods.GetItemCount(), goods.GetPriceValue(), goods.GetPriceUnit());
		}else
		{
			LOG_INFO("ibtrans", "load ibitem [ID:%d, ItemID:%d ItemCount: %d Price:%d Unit:%d ] successfully!",
				goods.GetGoodsID(), goods.GetItemID(), goods.GetItemCount(), goods.GetPriceValue(), goods.GetPriceUnit());
		}
	}

	// load section pages 
	TiXmlElement* pElemPages = tpRoot->FirstChildElement("pages");
	if ( pElemPages == NULL)
	{
		LOG_WARN("default", "Can NOT Find page seciont in file %s. ", xmlfile);
		return;
	}

	TiXmlElement* pElemPage = pElemPages->FirstChildElement("page");
	for (; pElemPage != NULL; pElemPage = pElemPage->NextSiblingElement())
	{
		const char* szPageID = pElemPage->Attribute("id");
		const char* szPageName = pElemPage->Attribute("desc");
		if ( szPageID == NULL || szPageName == NULL)
		{
			LOG_ERROR("default", "Can Not find `id` or `desc' in `page` elem");
			continue;
		}

		size_t nPageID = atoi(szPageID);

		if ( nPageID >= ARRAY_CNT(m_pbIbStore)  || nPageID < 0) 
		{
			LOG_ERROR("ibtrans", "The page id (%d) is bigger than ibstore size", nPageID);
			continue;
		}

		if ( m_pbIbStore[nPageID] != NULL)
		{
			LOG_ERROR("ibtrans", "The page id(%d:%s) is dup!", nPageID, szPageName);
			continue;
		}

		m_pbIbStore[nPageID] = new PBIbStore;

		m_pbIbStore[nPageID]->set_pageid(atoi(szPageID));
		m_pbIbStore[nPageID]->set_pagename(szPageName);

		TiXmlElement* ptab = pElemPage->FirstChildElement("tab");
		for (; ptab != NULL; ptab = ptab->NextSiblingElement())
		{
			const char* szTabID = ptab->Attribute("id");
			const char* szTabDesc = ptab->Attribute("desc");

			if ( szTabDesc == NULL || szTabID == NULL)
			{
				LOG_ERROR("default", "Can Not find `id` or `desc` in `tab` elem");
				continue;
			}

			PBIbTab* pbIbTab = m_pbIbStore[nPageID]->add_tab();
			if ( pbIbTab == NULL)
			{
				LOG_FATAL("default", "Can Not add_tab for PBIbStore for tab `%s'", szTabDesc);
				continue;
			}

			pbIbTab->set_tabid(atoi(szTabID));
			pbIbTab->set_tabname(szTabDesc);

			TiXmlElement* psubtab = ptab->FirstChildElement("sub_tab");
			for (; psubtab != NULL; psubtab = psubtab->NextSiblingElement())
			{
				const char* szSubDesc = psubtab->Attribute("desc");
				const char* szSubID = psubtab->Attribute("id");

				if (szSubDesc == NULL || szSubID == NULL) 
				{
					LOG_ERROR("default", "Can Not find `id` or `desc` in `sub_tab` elem");
					continue;
				}

				PBIbSubTab* pbIbSubTab = pbIbTab->add_subtab();
				if ( pbIbSubTab == NULL)
				{
					LOG_FATAL("default", "Can Not add_tab for PBIbTab for tab `%s'", szSubDesc);
					continue;
				}

				pbIbSubTab->set_subtabid(atoi(szSubID));
				pbIbSubTab->set_subtabname(szSubDesc);

				TiXmlElement* pitem = psubtab->FirstChildElement("goods");
				for (int nIndex = 0; pitem != NULL; pitem = pitem->NextSiblingElement())
				{
					const char* szGoodsID  = pitem->Attribute("id");
					if ( szGoodsID == NULL )
					{
						LOG_ERROR("default", "Can NOT find `id` in ibitem node, pls check it");
						continue;
					}

					int nGoodsID = atoi(szGoodsID);
					CGoods* pIbItem = GetGoods(nGoodsID);
					if (pIbItem  == NULL)
					{
						LOG_ERROR("default", "Can NOT Find Item(%d) in IbItem library.", nGoodsID);
						continue;
					}

					pbIbSubTab->add_goodsid(nGoodsID);
					const int const_count_per_row = 2;

					CLocation location; 
					location.SetPage( atoi(szPageID)*100 + atoi(szTabID) * 10 + atoi(szSubID));

					location.SetRow( nIndex / const_count_per_row);
					location.SetCol( nIndex % const_count_per_row);

					pIbItem->SetLocation(location);

					nIndex ++;
				}
			}
		}
	}

	// load promotion
	TiXmlElement* pElemPromNode = tpRoot->FirstChildElement("promotion");
	if ( pElemPromNode != NULL)
	{
		TiXmlElement* ptab = pElemPromNode->FirstChildElement("goods");
		for (; ptab != NULL; ptab = ptab->NextSiblingElement())
		{
			const char* szGoodsID = ptab->Attribute("id");

			int nType = 0;
			int	nBeginVal = 0;
			int	nEndVal = 0;
			int nBeginTime = 0 ;
			int nEndTime = 0;

			ptab->Attribute("type", &nType);
			ptab->Attribute("begin_val", &nBeginVal);
			ptab->Attribute("end_val", &nEndVal);
			ptab->Attribute("begin_time", &nBeginTime);
			ptab->Attribute("end_time", &nEndTime);

			const char* szMaxCount = ptab->Attribute("max_count");
			/*
			const char* szColdTime = ptab->Attribute("cold_time");
			const char* szColdNum = ptab->Attribute("cold_num");
			const char* szTimerNotice = ptab->Attribute("timer_notice");
			const char* szNumberNotice = ptab->Attribute("number_notice");
			*/

			if ( szGoodsID == NULL ) continue;

			CPromotion* pPromotion = NULL;
			CPromotion rPromotion;

			// 如果是重新读取的配置文件，直接使用原来的内容
			if ( bReloadFlag == true)
			{
				pPromotion = GetPromotion(atoi(szGoodsID));
			}

			if ( pPromotion == NULL)
			{
				pPromotion = &rPromotion;

				pPromotion->SetGoodsID(atoi(szGoodsID));
				// pPromotion->SetElapsedTime(0);
				pPromotion->SetRemainCount(szMaxCount ? atoi(szMaxCount) : CGoods::em_ibitem_ulimit);
			}else
			{
				if ( szMaxCount != NULL)
				{
					// 本次变化的数值
					int nIncCount = atoi(szMaxCount) - pPromotion->GetMaxCount();

					// 剩余数值做增减
					int nWillRemainCount = pPromotion->GetRemainCount() + nIncCount;
					pPromotion->SetRemainCount(nWillRemainCount > 0 ? nWillRemainCount : 0);
				}
			}

			// 默认最大数量是无穷大
			pPromotion->SetMaxCount(szMaxCount ? atoi(szMaxCount) : CGoods::em_ibitem_ulimit);

			// 如果给定的最大数量是无穷大,那么剩余的数量强制为无穷大
			if ( pPromotion->GetMaxCount() == CGoods::em_ibitem_ulimit)
			{
				pPromotion->SetRemainCount(CGoods::em_ibitem_ulimit);
			}

			/*
			pPromotion->SetColdTime(szColdTime ? atoi(szColdTime) : -1);
			pPromotion->SetColdNum(szColdNum ? atoi(szColdNum) : 1);
			*/

			// 设置促销时间方式
			if ( pPromotion->SetTimePeriod(nType, nBeginVal, nEndVal, nBeginTime, nEndTime) == false)
			{
				LOG_ERROR("ibtrans", "promotion of goods(%s) set time period error(%d, %d, %d, %d, %d)", 
						szGoodsID, nType, nBeginVal, nEndVal, nBeginTime, nEndTime);
				continue;	
			}

			/*
			if ( pPromotion->GetMaxCount() != CGoods::em_ibitem_ulimit)
			{
			if ( szNumberNotice != NULL)
			Tokens timers = StrSplit(szNumberNotice, ",");
			Tokens::iterator iter = timers.begein();
			for ( int i= 0; iter != timers.end() && i < pPromotion->GetNoticeTimerSize(); ++iter, ++i)
			{
			if ( (iter->c_str())[0] != 0 )
			{
			pPromotion->AddNoticeTimer(atoi(iter->c_str()));
			}
			}
			}

			if ( pPromotion->GetBeginTime() != -1)
			{
			if ( szTimerNotice != NULL)
			Tokens timers = StrSplit(szTimerNotice, ",");
			Tokens::iterator iter = timers.begein();
			for ( int i= 0; iter != timers.end() && i < pPromotion->GetNoticeNumberSize(); ++iter, ++i)
			{
			if ( (iter->c_str())[0] != 0 )
			{
			pPromotion->AddNoticeNumber(atoi(iter->c_str()));
			}
			}
			}
			*/

			if (pPromotion->ResetActivePeriod(time(0)))
			{
				LOG_INFO("ibtrans", "Reset Promotion[%d, %d %d - %d %d] [%d - %d] of GoodsID(%d) number to %d from %d to %d",                                                         
						pPromotion->GetType(), pPromotion->GetBeginVal(), pPromotion->GetBeginTime(), pPromotion->GetEndVal(), pPromotion->GetEndTime(),
						pPromotion->GetActiveTime(), pPromotion->GetExpirityTime(),
						pPromotion->GetGoodsID(), pPromotion->GetRemainCount(),
						pPromotion->GetActiveTime(), pPromotion->GetExpirityTime());   

				// 如果上次更新的时间在下个生效时间之前
				if ( pPromotion->GetActiveTime() > pPromotion->GetUpdateTime())
				{
					pPromotion->SetRemainCount(pPromotion->GetMaxCount());
				}
			}

			if ( pPromotion == &rPromotion )
			{
				if (!InsertPromotion(rPromotion))
				{
					LOG_ERROR("ibtrans", "insert promotion for goods(%d)  error, pls check xml file", rPromotion.GetGoodsID());
				}
			}
		}
	}

	// load version
	struct stat buf;
	m_nVersion = 0;
	TiXmlElement* pElemHotNode = NULL; 
	if (stat(xmlfile, &buf) != -1)
	{
		m_nVersion = buf.st_mtime;
	}else
	{
		pElemHotNode = tpRoot->FirstChildElement("version");
		if ( pElemHotNode != NULL)
		{
			const char* szVersion = pElemHotNode->Attribute("value");
			m_nVersion = szVersion ? atoi(szVersion) : 0;
		}
	}

	// load section for hot area
	pElemHotNode = tpRoot->FirstChildElement("focus");
	if ( pElemHotNode == NULL)
	{
		LOG_WARN("default", "Can NOT Find hot seciont in file %s. ", xmlfile);
	}

	// bulletin
	m_szBulletin[0] = 0;
	const char* szBulletin = pElemHotNode->Attribute("bulletin");
	if ( szBulletin )
	{
		snprintf(m_szBulletin, sizeof(m_szBulletin)-1, "%s", szBulletin);
	}

	// focus area
	TiXmlElement* ptab = pElemHotNode->FirstChildElement("tab");
	for (; ptab != NULL; ptab = ptab->NextSiblingElement())
	{
		const char* szTabType = ptab->Attribute("id");
		const char* szTabName = ptab->Attribute("desc");
		if (szTabType == NULL) continue;

		size_t nFocusID = atoi( szTabType );
		if ( nFocusID >= ARRAY_CNT(m_pbFocus) )
		{
			LOG_ERROR("ibtrans", "The focus tab id (%d) is bigger than focus tab size", nFocusID);
			continue;
		}

		if ( m_pbFocus[nFocusID] != NULL)
		{
			LOG_ERROR("ibtrans", "The dup focus tab id(%d), pls check it", nFocusID);
			continue;
		}

		m_pbFocus[nFocusID] = new PBIbSubTab;
		m_pbFocus[nFocusID]->set_subtabid(nFocusID);
		if ( szTabName != NULL)
		{
			m_pbFocus[nFocusID]->set_subtabname(szTabName);
		}

		TiXmlElement* pElemHotGoods = ptab->FirstChildElement("goods");
		for (; pElemHotGoods!= NULL; pElemHotGoods= pElemHotGoods->NextSiblingElement())
		{
			const char* szGoodsID  = pElemHotGoods->Attribute("id");
			if ( szGoodsID == NULL )
			{
				LOG_ERROR("default", "Can NOT find `id` in ibitem node, pls check it");
				continue;
			}

			if ( m_pbFocus[nFocusID] != NULL )
			{
				m_pbFocus[nFocusID]->add_goodsid(atoi(szGoodsID));
			}
		}
	}
}


void CIbStoreManager:: LoadFunc4Xml(const char* xmlfile, bool bReloadFlag  )
{
	TiXmlDocument xmldoc;
	xmldoc.LoadFile( xmlfile );

	TiXmlElement* tpRoot= xmldoc.FirstChildElement();
	if (tpRoot == NULL)
	{
		LOG_WARN("default", "load items from xmlfile(%s) failure.", xmlfile);
		return;
	}

	TiXmlElement* pElemItems = tpRoot->FirstChildElement("funcs");
	if ( pElemItems == NULL )
	{
		LOG_WARN("default", "Can NOT Find Funcs seciont in file %s. ", xmlfile);
		return;
	}

	TiXmlElement* pElemFunc = pElemItems->FirstChildElement("func");
	
	for ( ; pElemFunc != NULL; pElemFunc = pElemFunc->NextSiblingElement())
	{
		CFuncConsume func;
		int			nTemp = 0 ;

		// id
		const char* p = pElemFunc->Attribute("id");
		if(p) func.SetFuncID( atoi(p)); else continue;

		//uint_type
		p = pElemFunc->Attribute("unit_type");
		if (p) func.SetFuncUnitType(atoi(p));else continue;

		//uint
		p = pElemFunc->Attribute("unit");
		func.SetFuncUnit( p ? atoi(p): 1 );
		
		//name
		p = pElemFunc->Attribute("name");
		if (p)  func.SetFuncName(p); else continue;

		////comment
		//p = pElemFunc->Attribute(("comment");
		//if (p) func.SetFuncComment(p); else continue;
		
		//value type
		p = pElemFunc->Attribute("value_type");
		if (p) func.SetFuncValueType( atoi(p)); else continue;

		//show 
		p = pElemFunc->Attribute("show");
		if (p) func.SetFuncIsShow( atoi(p)); else continue;

		p = pElemFunc->Attribute("default_type");
		if (p) func.SetDefaultType( atoi(p)); else continue;

		p = pElemFunc->Attribute("count_limit");
		if(p) func.SetFuncCountLimit( atoi(p) ); else continue;

		//price
		//yb
		TiXmlElement* pElemPrice = pElemFunc->FirstChildElement("consume");
		for( ;pElemPrice != NULL ;pElemPrice = pElemPrice->NextSiblingElement())
		{
			p = pElemPrice->Attribute("consume_type");
			if( p && atoi(p) > 0 && atoi(p) <=FUNC_CONSUME_COUNT && atoi(p) != FUNC_LINEKONG_MONEY )
			{
				nTemp = atoi(p);
				p = pElemPrice->Attribute("value");
				func.SetFuncValue( p ? atoi( p): -1,nTemp);
			}else{
				continue;
			}
		}


		if (InsertFuncConsume(func) != true)
		{
			LOG_ERROR("ibtrans", "load funcItme [ID:%d, Unit:%d yb: %d bounded_yb:%d money:%d  bounded_money:%d ] error",
				func.GetFuncID(), func.GetFuncUnit(), func.GetFuncValue(FUNC_BIND_MONEY), func.GetFuncValue(FUNC_SILVER_YB),
				func.GetFuncValue(FUNC_UNBIND_MONEY), func.GetFuncValue(FUNC_BIND_MONEY));
		}else
		{
			LOG_INFO("ibtrans", "load funcItme [ID:%d, Unit:%d yb: %d bounded_yb:%d money:%d  bounded_money:%d ] successfully!",
				func.GetFuncID(), func.GetFuncUnit(), func.GetFuncValue(FUNC_BIND_MONEY), func.GetFuncValue(FUNC_SILVER_YB),
				func.GetFuncValue(FUNC_UNBIND_MONEY), func.GetFuncValue(FUNC_BIND_MONEY));
		}


	}

}


void CIbStoreManager::BackupAndSave()
{
	//目前暂不backup
	return;

	time_t seconds = time(0);
	struct tm* pnow = localtime(&seconds);

	char szBackupFileName[64];
	strftime(szBackupFileName, sizeof(szBackupFileName), "../config/backup/ibstore_%m%d_%H%M%S", pnow);
	/*if (rename("../config/data/ibstore/ibstore.xml", szBackupFileName) != 0)
	{
		LOG_ERROR("ibtrans", "rename ../config/data/ibstore/ibstore.xml to %s failure.", szBackupFileName);
	}else*/
	if (rename(CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"ibstore/ibstore.xml"), szBackupFileName) != 0)
	{
		LOG_ERROR("ibtrans", "rename ../config/data/ibstore/ibstore.xml to %s failure.", szBackupFileName);
	}else
	{
		LOG_INFO("ibtrans", "backup ../config/data/ibstore/ibstore.xml to %s successfully.", szBackupFileName);
	}

	/*Save2Xml("../config/data/ibstore/ibstore.xml");*/
	Save2Xml(CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"ibstore/ibstore.xml"));
}

// save m_tGoods to xmlfile
void CIbStoreManager::Save2Xml(const char* xmlfile)
{
	// 创建xml 文档对象
	TiXmlDocument* pxmldoc = new TiXmlDocument;

	// 创建根元素并连接
	TiXmlElement* ptRootElem = new TiXmlElement("config");
	pxmldoc->LinkEndChild(ptRootElem);

	// 创建<pages>
	TiXmlElement *pPagesElem = new TiXmlElement("pages");	

	// 货架
	for ( size_t nPageID = 0; nPageID < ARRAY_CNT(m_pbIbStore); ++ nPageID)
	{
		if ( m_pbIbStore[nPageID] == NULL) continue;

		TiXmlElement *pPageElem = new TiXmlElement("page");	
		pPageElem->SetAttribute("id", nPageID);
		pPageElem->SetAttribute("desc", m_pbIbStore[nPageID]->pagename().c_str());

		LOG_DEBUG("ibtrans", " before save : %s", m_pbIbStore[nPageID]->ShortDebugString().c_str());

		for (int i = 0; i < m_pbIbStore[nPageID]->tab_size(); ++i)
		{
			PBIbTab* pbTab = m_pbIbStore[nPageID]->mutable_tab(i);

			TiXmlElement *pTabElem = new TiXmlElement("tab");	
			pTabElem->SetAttribute("id", pbTab->tabid());
			pTabElem->SetAttribute("desc", pbTab->tabname().c_str());

			for (int j = 0; j < pbTab->subtab_size(); ++j)
			{
				PBIbSubTab* pbSubTab = pbTab->mutable_subtab(j); 

				TiXmlElement *pSubTabElem = new TiXmlElement("sub_tab");	
				pSubTabElem->SetAttribute("id", pbSubTab->subtabid());
				pSubTabElem->SetAttribute("desc", pbSubTab->subtabname().c_str());

				for (int k = 0; k < pbSubTab->goodsid_size(); ++k)
				{
					TiXmlElement *pGoodsElem = new TiXmlElement("goods");	
					pGoodsElem->SetAttribute("id", pbSubTab->goodsid(k));

					pSubTabElem->LinkEndChild(pGoodsElem);

				}
				pTabElem->LinkEndChild(pSubTabElem);
			}

			pPageElem->LinkEndChild(pTabElem);
		}
		pPagesElem->LinkEndChild(pPageElem);
	}

	ptRootElem->LinkEndChild(pPagesElem);

	// 创建<items>
	TiXmlElement *pItemsElem = new TiXmlElement("items");	
	ptRootElem->LinkEndChild(pItemsElem);


	// 向 <items> 增加 <goods> 节点
	iterator iter = m_tGoods.begin();
	for(; iter != m_tGoods.end(); ++iter)
	{
		// 创建ibtem 元素并链接
		TiXmlElement *ptChildElem = new TiXmlElement("goods");	

		ptChildElem->SetAttribute("id", iter->second.GetGoodsID());
		ptChildElem->SetAttribute("item_id", iter->second.GetItemID());
		ptChildElem->SetAttribute("item_count", iter->second.GetItemCount());
		ptChildElem->SetAttribute("unit", iter->second.GetPriceUnit());
		ptChildElem->SetAttribute("price", iter->second.GetPriceValue());
		ptChildElem->SetAttribute("active", iter->second.IsActive() ? 1 : 0);
		ptChildElem->SetAttribute("count", iter->second.GetGoodsCount());
		ptChildElem->SetAttribute("discount", iter->second.GetDiscount());
		ptChildElem->SetAttribute("expiry_date", iter->second.GetExpiryTime());

		pItemsElem->LinkEndChild(ptChildElem);
	}

	pxmldoc->SaveFile(xmlfile);
	delete pxmldoc;
}
int CalGoodsFlag(const char* szFlag)
{
	typedef struct st_flag
	{
		const char* s; 
		int n;
	}st_flag;

	static st_flag lstflag[] = 
	{
		{ "REC",  1},
		{ "NUM",  2},
		{ "TIM",  3},
		{ "DIS",  4},
		{ "HOT",  5},
		{ "NEW",  6},
		{ "OUT",  7},
		{ "INA",  8},
		{ "TAC",  9},
		{ NULL,   0}
	};

	st_flag* pflag = lstflag;
	for (; pflag->s != NULL; ++pflag)	
	{
		if ( strncasecmp(pflag->s, szFlag, 3) == 0)
		{
			return pflag->n;
		}
	}

	return 0;
}
