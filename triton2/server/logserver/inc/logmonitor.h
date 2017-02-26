#ifndef _LOG_MONITOR_H_
#define _LOG_MONITOR_H_
#include "lk_hashmap.h"
#include "Database.h"

enum EM_STAT_ENTITY
{
	EM_STAT_ROLE 	= 1,
	EM_STAT_FAMILY 	= 2,
	EM_STAT_CORPS 	= 3,
};

enum EM_TABLE_ID
{
	LOG_MONEY_PRODUCE = 1,
	LOG_MONEY_RECYLE = 2,
	LOG_EXP_PRODUCE = 3,
	LOG_ITEM_PRODUCE = 4,
	LOG_ITEM_RECYCLE = 5,
	LOG_ACTION_MONIT = 6,
	LOG_MONEY_CIRCULATION = 7,
	LOG_ITEM_CIRCULATION = 8,
};

enum EItemLife
{
	EM_PRODUCE = 1,
	EM_CIRCULATION = 2,
	EM_RECYCLE =3,
};

enum ERoleItemLife
{
	EM_OBTAIN = 1,
	EM_LOSE = 2,
};

class CLogMonitor
{
  public:
	typedef std::pair<int64_t, int64_t> value_pair_t;
	typedef lk::hash_map<int, value_pair_t, 2000 > catalog_t;

	CLogMonitor()
	{
		mCatalog.initailize();
	}
		
	~CLogMonitor() {}

	void ClearStatInfo(){ mCatalog.clear(); }
	void SaveStatInfo(Database* pdb);
	void LoadStatInfo(Database* pdb);
	void Catalog(int nCatalogID, int64_t nValue)
	{
		catalog_t::iterator iter = mCatalog.find(nCatalogID);
		if ( iter != mCatalog.end())
		{
			iter->second.second += nValue;
		}else
		{
			mCatalog.insert(catalog_t::value_type(nCatalogID, value_pair_t(0, nValue)));
		}
	}

	void SetTableID(int nTableID) { m_nTableID = nTableID; }

  private:
	  catalog_t mCatalog;
	  int m_nTableID;
};


#endif
