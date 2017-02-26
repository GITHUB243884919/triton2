#ifndef _IBSTORE_H_
#define _IBSTORE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "coretype.h"



// 物品的位置信息 erating 送进来的是%d-%d-%d, 分别为page_id, row_id,  col_id
// 其中 page_id 从101 开始计数, row_id, col_id 均从 1 开始计数 
class CLocation
{
  private:
	int m_nPageID;
	int m_nRow;
	int m_nCol;

  public:
	CLocation(){}
	~CLocation(){}

	void SetPage(int nPageID) { m_nPageID = nPageID; }
	const int  GetPage() const { return m_nPageID; }

	void SetRow(int nRow) { m_nRow = nRow; }
	const int  GetRow() const { return m_nRow; }

	void SetCol(int nCol) { m_nCol =  nCol; }
	const int  GetCol() const { return m_nCol; }

	const char* GetLocationString() const
	{ 
		static char szLocation[32+1];

		szLocation[0] = 0;
		int ineed = snprintf(szLocation, sizeof(szLocation), "%d-%d-%d", m_nPageID, m_nRow, m_nCol);

		return ineed < (int) sizeof(szLocation) && ineed > 0 ? szLocation : NULL;
	}

	bool SetLocationString(const char* szLocation)
	{
		int nPageID = 0, nRow = 0, nCol = 0;

		nPageID = atoi(szLocation);
		char* p = strchr(szLocation, '-');

		if ( p == NULL || *(p+1) == '\0') return false;
		nRow = atoi(p+1);

		p = strchr(p+1, '-');
		if ( p == NULL || *(p+1) == '\0') return false;
		nCol = atoi(p+1); 

		m_nPageID = nPageID;
		m_nRow = nRow;
		m_nCol = nCol;

		return true;
	}
};

// 商城中存储的单个物品的信息
class CGoods
{
  public:
	enum 
	{
		em_ibitem_active 	= 1, // 生效
		em_ibitem_inactive  = 2, // 无效
	};

	enum 
	{
		em_ibitem_ulimit = -1,	// 不限量
	};

  private:
	int  m_nGoodsID;		// 商品ID
	int  m_nGoodsCount;		// 商品的数目

	int  m_nItemID;			// 物品ID
	int  m_nItemCount;		// 物品的个数

	char m_szGoodsName[32+1];  // 商品名称
	char m_szGoodsDesc[255+1]; // 商品描述

	CLocation m_tLocation;	// 商品在货架的位置

	int  m_nPriceValue;		// 商品原价
	int  m_nDiscount;		// 商品折扣后的价格
	int  m_nPriceUnit;		// 货币单位
	int  m_nExpiryTime;		// 商品打折截至日期

	int  m_nColdTime; 		// 商品CD时间
	int  m_nElapsedTime;	// 已经逝去的CD时间

	bool m_bActiveFlag;		// 商品上架标记
	bool m_bIsPackage;		// 是否是打包产品

	int m_nOnSaleTime;		// 商品上架时间

	int m_nLimitPerChar;	// 玩家购买数量限制
	int m_nLimitType;		// 玩家限购数量类型

	int m_nFlag;			// 商品标签
  public:

	CGoods(){}
	~CGoods(){}

	// m_nGoodsID
	int GetGoodsID() { return m_nGoodsID; }
	void SetGoodsID(int nGoodsID) { m_nGoodsID = nGoodsID; }

	// m_nGoodsCount
	int  GetGoodsCount() { return m_nGoodsCount; }
	void SetGoodsCount(int nCount) { m_nGoodsCount = nCount; }

	// m_nItemId
	int GetItemID() { return m_nItemID; }
	void SetItemID(int nItemID) { m_nItemID = nItemID; }

	// m_nItemCount
	int  GetItemCount() { return m_nItemCount; }
	void SetItemCount(int nCount) { m_nItemCount = nCount; }

	// m_szGoodsName
	const char* GetGoodsName() const { return m_szGoodsName; } 
	void SetGoodsName(const char* szGoodsName) 
	{ 
		strncpy(m_szGoodsName, szGoodsName, sizeof(m_szGoodsName)); 
		m_szGoodsName[sizeof(m_szGoodsName)-1] = 0;
	}

	// m_szGoodsDesc
	const char* GetItemDesc() const { return m_szGoodsDesc; }
	void SetItemDesc(const char* szGoodsDesc)
	{
		strncpy(m_szGoodsDesc, szGoodsDesc, sizeof(m_szGoodsDesc));
		m_szGoodsDesc[sizeof(m_szGoodsDesc)-1] = 0;
	}

	// m_tLocation
	const CLocation& GetLocation() { return m_tLocation; }
	void SetLocation(const CLocation& rLocation) { m_tLocation = rLocation; }

	// m_nPriceUnit
	int GetPriceUnit() { return m_nPriceUnit; }
	void SetPriceUnit( int nPriceUnit ) { m_nPriceUnit = nPriceUnit; }

	// m_nPriceValue
	int GetPriceValue() { return m_nPriceValue; }
	void SetPriceValue( int nPriceValue ) { m_nPriceValue = nPriceValue; }

	// m_nDiscount
	int GetDiscount() { return m_nDiscount; }
	void SetDiscount( int nPriceValue ) { m_nDiscount = nPriceValue; }

	// m_nExpiryTime
	int GetExpiryTime() { return m_nExpiryTime; }
	void SetExpiryTime( int nExpiryTime ) { m_nExpiryTime = nExpiryTime; }

	// m_bActiveFlag
	bool IsActive() { return m_bActiveFlag == em_ibitem_active; }
	void SetActive(bool bActiveFlag) { m_bActiveFlag =  ( bActiveFlag ? em_ibitem_active : em_ibitem_inactive); }

	// m_bIsPackage
	bool IsPackage() { return m_bIsPackage; }
	void SetIsPackage(bool bIsPackage) { m_bIsPackage = bIsPackage; } 

	// m_nColdTime 
	int GetColdTime() { return m_nColdTime; }
	void SetColdTime( int nCDTime ) { m_nColdTime = nCDTime; }
	
	// m_nElapsedTime
	int GetElapsedTime() { return m_nElapsedTime; }
	void SetElapsedTime( int nCDTime ) { m_nElapsedTime = nCDTime; }

	// m_nOnSaleTime
	int GetOnsaleTime() { return m_nOnSaleTime; }
	void SetOnsaleTime( int nOnSaleTime ) { m_nOnSaleTime = nOnSaleTime; }

	// m_nLimitPerChar 
	int GetLimitPerChar() { return m_nLimitPerChar; }
	void SetLimitPerChar( int nLimitPerChar ) { m_nLimitPerChar = nLimitPerChar; }

	// m_nLimitType
	int GetLimitType() { return m_nLimitType; }
	void SetLimitType( int nLimitType) { m_nLimitType = nLimitType; }
	// m_nFlag
	int GetFlag() { return m_nFlag; }
	void SetFlag( int nFlag) { m_nFlag = nFlag; }
};

class CPromotion
{
	private:
		int m_nGoodsID;					// 商品ID
		uint32_t m_nBeginTime;			// 促销起始时间 HH24MISS
		uint32_t m_nEndTime;			// 促销截至时间 HH24MISS
		uint32_t m_nBeginVal;			// 促销类型的开始值	
		uint32_t m_nEndVal;				// 促销类型的结束值

		time_t	 m_nActiveTimeStamp;	// 开始促销的时间点
		time_t	 m_nExpirityTimeStamp;	// 此次促销结束的时间点

		int m_nType;		// 促销类型

		int m_nRemainCount;		// 当前剩余
		int m_nMaxCount;		// 促销的总数量

		/*
		int  m_nColdTime; 		// 商品CD时间
		int  m_nElapsedTime;	// 已经逝去的CD时间
		int  m_nColdNum;		// 每CD时间产生的商品个数
		*/

		int m_nUpdateTime;		// 重置的

		/*
		int  m_nNoticeTimerCount;		
		int  m_nNoticeTimer[10];	// 距离开始时间的分钟数为设定的值时提示

		int  m_nNoticeNumberCount;
		int  m_nNoticeNumber[10];	// 当跳变为此数量时候需要提示
		*/

	public:
		// m_nGoodsID
		int GetGoodsID() { return m_nGoodsID; }
		void SetGoodsID( int nGoodsID) { m_nGoodsID = nGoodsID; }

		// m_nStartTime
		int GetBeginTime() { return m_nBeginTime; }
		void SetBeginTime(int nBeginTime) { m_nBeginTime = nBeginTime; }

		// m_nEndTime
		int GetEndTime() { return m_nEndTime; }
		void SetEndTime( int nEndTime ) { m_nEndTime = nEndTime; }

		// Active From TimeStamp To TimeStamp
		int GetActiveTime() const { return m_nActiveTimeStamp; }
		int GetExpirityTime() const { return m_nExpirityTimeStamp; }

		// m_nRemainCount
		int GetRemainCount() { return m_nRemainCount; }
		void SetRemainCount(int nRemainCount)  { m_nRemainCount = nRemainCount; }

		// m_nMaxCount
		int GetMaxCount() { return m_nMaxCount; }
		void SetMaxCount(int nMaxCount)  { m_nMaxCount = nMaxCount; }

		// m_nUpdateTime
		int GetUpdateTime() { return m_nUpdateTime; }
		void SetUpdateTime( int nTime ) { m_nUpdateTime = nTime; }

		/* m_nColdTime 
		int GetColdTime() { return m_nColdTime; }
		void SetColdTime( int nCDTime ) { m_nColdTime = nCDTime; }
	
		// m_nElapsedTime
		int GetElapsedTime() { return m_nElapsedTime; }
		void SetElapsedTime( int nCDTime ) { m_nElapsedTime = nCDTime; }

		// m_nColdNum
		int GetColdNum() { return m_nColdNum; }
		void SetColdNum( int nCDNum ) { m_nColdNum = nCDNum; }
		*/

		// 将 ptm中的HHMISS重置
		time_t RebuildTime(struct tm *ptm, int HH24MISS)
		{
			ptm->tm_hour = HH24MISS / 10000;		
			ptm->tm_min = HH24MISS / 100 % 100;
			ptm->tm_sec = HH24MISS % 100;

			return mktime(ptm);
		}

		// 根据YYYYMMDDHH24MISS 形成新的时间
		time_t RebuildTime(int YYYYMMDD, int HH24MISS)
		{
			struct tm tmCurr;

			tmCurr.tm_year = YYYYMMDD / 10000 - 1900;
			tmCurr.tm_mon = YYYYMMDD / 100 % 100 - 1;
			tmCurr.tm_mday = YYYYMMDD % 100;

			tmCurr.tm_hour = HH24MISS / 10000;		
			tmCurr.tm_min = HH24MISS / 100 % 100;
			tmCurr.tm_sec = HH24MISS % 100;

			return mktime(&tmCurr);
		}

		// 取得下一个生效周期
		bool ResetActivePeriod(time_t nThisTime)
		{
			struct tm tmCurr;
			uint64_t nThisVal = 0;

			// 下一个有效的某天时间段
			switch( m_nType )
			{
				// eg: 从start_val 到end_var 的给定时间段, 
				// [1-7, HH24MISS]
				case 1:
				{
					if (localtime_r(&nThisTime,  &tmCurr) == NULL)  return false;
					nThisVal = (tmCurr.tm_wday == 0 ? 7 : tmCurr.tm_wday) ; 
					if (nThisVal < m_nBeginVal)
					{
						time_t nIncTime = nThisTime + 3600 * 24 * ( m_nBeginVal - nThisVal);
						if (localtime_r(&nIncTime,  &tmCurr) == NULL)  return false;

						m_nActiveTimeStamp = RebuildTime(&tmCurr, m_nBeginTime);
						m_nExpirityTimeStamp = RebuildTime(&tmCurr, m_nEndTime);
					}else if ( nThisVal <= m_nEndVal)
					{
						uint32_t nOffset = tmCurr.tm_hour * 10000 + tmCurr.tm_min * 100 + tmCurr.tm_sec; 
						if ( nOffset < m_nEndTime ) 
						{
							// 显示当天的时间
							m_nActiveTimeStamp = RebuildTime(&tmCurr, m_nBeginTime);
							m_nExpirityTimeStamp = RebuildTime(&tmCurr, m_nEndTime);
						}else
						{
							// 判断第二天是否在有效范围内
							time_t nIncTime = nThisTime + 3600 * 24 ;
							if (localtime_r(&nIncTime,  &tmCurr) == NULL)  return false;

							uint64_t nNextVal = (tmCurr.tm_wday == 0 ? 7 : tmCurr.tm_wday); 
							if ( nNextVal < m_nBeginVal || nNextVal > m_nEndVal )
							{
								// 取下周开始的时间
								nIncTime = nThisTime + 3600 * 24 * (7 -  nThisVal + m_nBeginVal);
								if (localtime_r(&nIncTime,  &tmCurr) == NULL)  return false;

								m_nActiveTimeStamp = RebuildTime(&tmCurr, m_nBeginTime);
								m_nExpirityTimeStamp = RebuildTime(&tmCurr, m_nEndTime);
							}else
							{
								// 取当天的时间
								m_nActiveTimeStamp = RebuildTime(&tmCurr, m_nBeginTime);
								m_nExpirityTimeStamp = RebuildTime(&tmCurr, m_nEndTime);
							}
						}
					}else
					{
						time_t nIncTime = nThisTime + 3600 * 24 * ( 7 - nThisVal + m_nBeginVal);
						if (localtime_r(&nIncTime,  &tmCurr) == NULL)  return false;

						m_nActiveTimeStamp = RebuildTime(&tmCurr, m_nBeginTime);
						m_nExpirityTimeStamp = RebuildTime(&tmCurr, m_nEndTime);
					}

					return true;
				}

				// [YYYYMMDD, HH24MISS]
				case 2:
				{
					if (localtime_r(&nThisTime,  &tmCurr) == NULL)  return false;

					nThisVal = (tmCurr.tm_year + 1900) * 10000 + (tmCurr.tm_mon + 1)* 100 + tmCurr.tm_mday;
					if (nThisVal < m_nBeginVal) 
					{
						m_nActiveTimeStamp = RebuildTime(m_nBeginVal, m_nBeginTime);
						m_nExpirityTimeStamp = RebuildTime(m_nBeginVal, m_nEndTime);
					} else if (nThisVal <= m_nEndVal)
					{
						uint32_t nOffset = tmCurr.tm_hour * 10000 + tmCurr.tm_min * 100 + tmCurr.tm_sec; 
						if ( nOffset < m_nEndTime )
						{
							// 显示当天的时间
							m_nActiveTimeStamp = RebuildTime(&tmCurr, m_nBeginTime);
							m_nExpirityTimeStamp = RebuildTime(&tmCurr, m_nEndTime);
						}else
						{
							// 显示下一天的时间
							time_t nIncTime = nThisTime + 3600 * 24;
							if (localtime_r(&nIncTime,  &tmCurr) == NULL)  return false;

							uint64_t nNextVal = (tmCurr.tm_year + 1900) * 10000 + (tmCurr.tm_mon + 1)* 100 + tmCurr.tm_mday;
							if (nNextVal > m_nEndVal || nNextVal < m_nBeginVal)  
							{
								m_nActiveTimeStamp = RebuildTime(m_nEndVal, m_nBeginTime);
								m_nExpirityTimeStamp = RebuildTime(m_nEndVal, m_nEndTime);
								return false;
							}

							m_nActiveTimeStamp = RebuildTime(&tmCurr, m_nBeginTime);
							m_nExpirityTimeStamp = RebuildTime(&tmCurr, m_nEndTime);
						}

						return true;
					}else
					{
						m_nActiveTimeStamp = RebuildTime(m_nEndVal, m_nBeginTime);
						m_nExpirityTimeStamp = RebuildTime(m_nEndVal, m_nEndTime);
					}

					break;
				}

				// [MMDD, HH24MISS]
				case 3:
				{
					if (localtime_r(&nThisTime,  &tmCurr) == NULL)  return false;
					nThisVal =  (tmCurr.tm_mon + 1)* 100 + tmCurr.tm_mday;
					if (nThisVal < m_nBeginVal )
					{
						m_nActiveTimeStamp = RebuildTime((tmCurr.tm_year + 1900) * 10000 + m_nBeginVal, m_nBeginTime);
						m_nExpirityTimeStamp = RebuildTime((tmCurr.tm_year + 1900) * 10000 + m_nBeginVal, m_nEndTime);
					}else if (nThisVal <= m_nEndVal)
					{
						uint32_t nOffset = tmCurr.tm_hour * 10000 + tmCurr.tm_min * 100 + tmCurr.tm_sec; 
						if ( nOffset < m_nEndTime)
						{
							// 显示当天的时间
							m_nActiveTimeStamp = RebuildTime(&tmCurr, m_nBeginTime);
							m_nExpirityTimeStamp = RebuildTime(&tmCurr, m_nEndTime);
						}else
						{
							// 显示下一天的时间
							time_t nIncTime = nThisTime + 3600 * 24;
							if (localtime_r(&nIncTime,  &tmCurr) == NULL)  return false;

							uint64_t nNextVal =  (tmCurr.tm_mon + 1)* 100 + tmCurr.tm_mday;
							if (nNextVal > m_nEndVal || nNextVal < m_nBeginVal)  
							{
								// 取下一年的时间
								if (localtime_r(&nThisTime,  &tmCurr) == NULL)  return false;
								m_nActiveTimeStamp = RebuildTime((tmCurr.tm_year + 1900 + 1) * 10000 + m_nBeginVal, m_nBeginTime);
								m_nExpirityTimeStamp = RebuildTime((tmCurr.tm_year + 1900 + 1) * 10000 + m_nBeginVal, m_nEndTime);
							}else
							{
								// 取这天的时间
								m_nActiveTimeStamp = RebuildTime(&tmCurr, m_nBeginTime);
								m_nExpirityTimeStamp = RebuildTime(&tmCurr, m_nEndTime);
							}
						}
					}else
					{
						// 取下一年的时间
						m_nActiveTimeStamp = RebuildTime((tmCurr.tm_year + 1900 + 1) * 10000 + m_nBeginVal, m_nBeginTime);
						m_nExpirityTimeStamp = RebuildTime((tmCurr.tm_year + 1900 + 1) * 10000 + m_nBeginVal, m_nEndTime);
					}

					return true;
				}

				//[DD, HH24MISS]
				case 4:
				{
					if (localtime_r(&nThisTime,  &tmCurr) == NULL)  return false;

					nThisVal =  tmCurr.tm_mday;
					if (nThisVal < m_nBeginVal ) 
					{
						// 取第一个月份的
						m_nActiveTimeStamp = RebuildTime((tmCurr.tm_year+1900) * 10000 + (tmCurr.tm_mon+1) * 100 + m_nBeginVal, m_nBeginTime);
						m_nExpirityTimeStamp = RebuildTime((tmCurr.tm_year+1900) * 10000 + (tmCurr.tm_mon+1) * 100 +  m_nBeginVal, m_nEndTime);
					} else if (nThisVal <= m_nEndVal)
					{
						// 取当前月份的
						uint32_t nOffset = tmCurr.tm_hour * 10000 + tmCurr.tm_min * 100 + tmCurr.tm_sec; 
						if ( nOffset < m_nEndTime) 
						{
							// 显示当天的时间
							m_nActiveTimeStamp = RebuildTime(&tmCurr, m_nBeginTime);
							m_nExpirityTimeStamp = RebuildTime(&tmCurr, m_nEndTime);
						} else
						{
							// 取第二天
							time_t nIncTime = nThisTime + 3600 * 24;
							if (localtime_r(&nIncTime,  &tmCurr) == NULL)  return false;

							uint64_t nNextVal =  tmCurr.tm_mday;
							if (nNextVal < m_nBeginVal || nNextVal > m_nEndVal) 
							{
								// 取下个月的第一天
								if (localtime_r(&nThisTime,  &tmCurr) == NULL)  return false;
								if ( tmCurr.tm_mon == 11)
								{
									m_nActiveTimeStamp = RebuildTime((tmCurr.tm_year+1901) * 10000 + 1 * 100 + m_nBeginVal, m_nBeginTime);
									m_nExpirityTimeStamp = RebuildTime((tmCurr.tm_year+1901) * 10000 + 1 * 100 +  m_nBeginVal, m_nEndTime);
								}else
								{
									m_nActiveTimeStamp = RebuildTime((tmCurr.tm_year+1900) * 10000 + (tmCurr.tm_mon+2) * 100 + m_nBeginVal, m_nBeginTime);
									m_nExpirityTimeStamp = RebuildTime((tmCurr.tm_year+1900) * 10000 + (tmCurr.tm_mon+2) * 100 +  m_nBeginVal, m_nEndTime);
								}
							}else
							{
								m_nActiveTimeStamp = RebuildTime(&tmCurr, m_nBeginTime);
								m_nExpirityTimeStamp = RebuildTime(&tmCurr, m_nEndTime);
							}
						}
					}else
					{
						// 取下个月的第一天
						if ( tmCurr.tm_mon == 11)
						{
							m_nActiveTimeStamp = RebuildTime((tmCurr.tm_year+1901) * 10000 + 1 * 100 + m_nBeginVal, m_nBeginTime);
							m_nExpirityTimeStamp = RebuildTime((tmCurr.tm_year+1901) * 10000 + 1 * 100 +  m_nBeginVal, m_nEndTime);
						}else
						{
							m_nActiveTimeStamp = RebuildTime((tmCurr.tm_year+1900) * 10000 + (tmCurr.tm_mon+2) * 100 + m_nBeginVal, m_nBeginTime);
							m_nExpirityTimeStamp = RebuildTime((tmCurr.tm_year+1900) * 10000 + (tmCurr.tm_mon+2) * 100 +  m_nBeginVal, m_nEndTime);
						}
					}

					return true;
				}

				// 从start_val 到 end_val,  绝对时间
				// [YYYYMMDD,HH24MISS]
				case 5:
				{
					if (localtime_r(&nThisTime,  &tmCurr) == NULL)  return false;

					nThisVal =  (tmCurr.tm_year + 1900 )* 10000000000ll + (tmCurr.tm_mon + 1) * 100000000 + tmCurr.tm_mday * 1000000 
						+ tmCurr.tm_hour * 10000 + tmCurr.tm_min * 100  + tmCurr.tm_sec;
					if ( nThisVal  < (m_nEndVal * 1000000 + m_nEndTime)) 
					{
						m_nActiveTimeStamp = RebuildTime(m_nBeginVal, m_nBeginTime);
						m_nExpirityTimeStamp = RebuildTime(m_nEndVal, m_nEndTime);

						return true;
					}else
					{
						m_nActiveTimeStamp = RebuildTime(m_nBeginVal, m_nBeginTime);
						m_nExpirityTimeStamp = RebuildTime(m_nEndVal, m_nEndTime);
					}

					break;
				}

				// [MMDD,HH24MISS]
				case 6:
				{
					if (localtime_r(&nThisTime,  &tmCurr) == NULL)  return false;

					nThisVal =  (tmCurr.tm_mon + 1)* 100000000 + tmCurr.tm_mday * 1000000 + tmCurr.tm_hour * 10000 + tmCurr.tm_min * 100  + tmCurr.tm_sec;
					if ( nThisVal  < (m_nEndVal * 1000000 + m_nEndTime)) 
					{
						m_nActiveTimeStamp = RebuildTime((tmCurr.tm_year + 1900)*10000 + m_nBeginVal, m_nBeginTime);
						m_nExpirityTimeStamp = RebuildTime((tmCurr.tm_year + 1900)*10000 + m_nEndVal, m_nEndTime);
					}else
					{
						m_nActiveTimeStamp = RebuildTime((tmCurr.tm_year + 1901)*10000 + m_nBeginVal, m_nBeginTime);
						m_nExpirityTimeStamp = RebuildTime((tmCurr.tm_year + 1901)*10000 + m_nEndVal, m_nEndTime);
					}

					return true;
				}

				// [DD,HH24MISS]
				case 7:
				{
					if (localtime_r(&nThisTime,  &tmCurr) == NULL)  return false;

					nThisVal =  tmCurr.tm_mday * 1000000 + tmCurr.tm_hour * 10000 + tmCurr.tm_min * 100  + tmCurr.tm_sec;
					if ( nThisVal  < (m_nEndVal * 1000000 + m_nEndTime)) 
					{
						m_nActiveTimeStamp = RebuildTime((tmCurr.tm_year + 1900)*10000 + (tmCurr.tm_mon+1)*100 + m_nBeginVal, m_nBeginTime);
						m_nExpirityTimeStamp = RebuildTime((tmCurr.tm_year + 1900)*10000 +(tmCurr.tm_mon+1)*100 + m_nEndVal, m_nEndTime);
					}else
					{
						if ( tmCurr.tm_mon == 11)
						{
							m_nActiveTimeStamp = RebuildTime((tmCurr.tm_year + 1901)*10000 + 100 + m_nBeginVal, m_nBeginTime);
							m_nExpirityTimeStamp = RebuildTime((tmCurr.tm_year + 1901)*10000 + 100 + m_nEndVal, m_nEndTime);
						}else
						{
							m_nActiveTimeStamp = RebuildTime((tmCurr.tm_year + 1900)*10000 + (tmCurr.tm_mon+2)*100 + m_nBeginVal, m_nBeginTime);
							m_nExpirityTimeStamp = RebuildTime((tmCurr.tm_year + 1900)*10000 + (tmCurr.tm_mon+2)*100 + m_nEndVal, m_nEndTime);
						}
					}

					return true;
				}

				default:
				{
					break;
				}
			}

			return false;
		}

		bool SetTimePeriod(int nType, uint32_t nBeginVal, uint32_t nEndVal, uint32_t nBeginTime, uint32_t nEndTime)
		{
			//TODO: check the param range.
			m_nType = nType;
			m_nBeginVal = nBeginVal;
			m_nEndVal = nEndVal;
			m_nBeginTime = nBeginTime;
			m_nEndTime = nEndTime;

			return true;
		}

		int GetType() const { return m_nType; }
		int GetBeginVal() const { return m_nBeginVal; }
		int GetEndVal() const { return m_nEndVal; }

		bool IsValidTime(time_t nThisTime)
		{
			return nThisTime >= m_nActiveTimeStamp && nThisTime <= m_nExpirityTimeStamp;
		}

		/*
		bool IsValidTime(time_t nThisTime)
		{
			// 1-星期型的每天时间段，星期日为7；
			// 2-YYYYMMDD型每天的时间段；
			// 3-MMDD型每天的时间段；
			// 4-DD型每天的时间段；
			// 5-YYYYMMDD型从起始日期时间到终止日期时间；
			// 6-MMDD型从起始日期时间到终止日期时间；
			// 7-DD型从起始日期时间到终止日期时间。

			struct tm tmCurr;
			if (localtime_r(&nThisTime,  &tmCurr) == NULL)  return false;

			uint64_t nThisVal = 0;

			switch(m_nType) 
			{
				// eg: 从start_val 到end_var 的给定时间段, 
				// [1-7, HH24MISS]
				case 1:
				{
					nThisVal = tmCurr.tm_wday + 1; 
					if (nThisVal >= m_nBeginVal && nThisVal <= m_nEndVal)
					{
						uint32_t nOffset = tmCurr.tm_hour * 10000 + tmCurr.tm_min * 100 + tmCurr.tm_sec; 
						return nOffset >= m_nBeginTime && nOffset <= m_nEndTime;
					}
					break;
				}

				// [YYYYMMDD, HH24MISS]
				case 2:
				{
					nThisVal = (tmCurr.tm_year + 1900) * 10000 + tmCurr.tm_mon * 100 + tmCurr.tm_mday;
					if (nThisVal >= m_nBeginVal && nThisVal <= m_nEndVal)
					{
						uint32_t nOffset = tmCurr.tm_hour * 10000 + tmCurr.tm_min * 100 + tmCurr.tm_sec; 
						return nOffset >= m_nBeginTime && nOffset <= m_nEndTime;
					}
					break;
				}

				// [MMDD, HH24MISS]
				case 3:
				{
					nThisVal =  tmCurr.tm_mon * 100 + tmCurr.tm_mday;
					if (nThisVal >= m_nBeginVal && nThisVal <= m_nEndVal)
					{
						uint32_t nOffset = tmCurr.tm_hour * 10000 + tmCurr.tm_min * 100 + tmCurr.tm_sec; 
						return nOffset >= m_nBeginTime && nOffset <= m_nEndTime;
					}
					break;
				}

				//[DD, HH24MISS]
				case 4:
				{
					nThisVal =  tmCurr.tm_mday;
					if (nThisVal >= m_nBeginVal && nThisVal <= m_nEndVal)
					{
						uint32_t nOffset = tmCurr.tm_hour * 10000 + tmCurr.tm_min * 100 + tmCurr.tm_sec; 
						return nOffset >= m_nBeginTime && nOffset <= m_nEndTime;
					}
					break;
				}

				// 从start_val 到 end_val,  绝对时间
				// [YYYYMMDD,HH24MISS]
				case 5:
				{
					nThisVal =  tmCurr.tm_year * 10000000000ll * tmCurr.tm_mon * 100000000 + tmCurr.tm_mday * 1000000 + tmCurr.tm_hour * 10000 + tmCurr.tm_min * 100  + tmCurr.tm_sec;
					return nThisVal  >= (m_nBeginVal * 1000000ull + m_nBeginTime) && nThisVal  < (m_nEndVal * 1000000 + m_nEndTime);
				}

				// [MMDD,HH24MISS]
				case 6:
				{
					nThisVal =  tmCurr.tm_mon * 100000000 + tmCurr.tm_mday * 1000000 + tmCurr.tm_hour * 10000 + tmCurr.tm_min * 100  + tmCurr.tm_sec;
					return nThisVal  >= (m_nBeginVal * 1000000ull + m_nBeginTime) && nThisVal  < (m_nEndVal * 1000000 + m_nEndTime);
				}

				// [DD,HH24MISS]
				case 7:
				{
					nThisVal =  tmCurr.tm_mday * 1000000 + tmCurr.tm_hour * 10000 + tmCurr.tm_min * 100  + tmCurr.tm_sec;
					return nThisVal  >= (m_nBeginVal * 1000000ull + m_nBeginTime) && nThisVal  < (m_nEndVal * 1000000 + m_nEndTime);
				}
			}

			return false;
		}
		*/

		bool IsEnougth(int nNum)
		{
			return m_nMaxCount == -1 || nNum <= m_nRemainCount; 
		}

		/*
		int GetNoticeTimerCount() { return m_nNoticeTimerCount; }
		int GetNoticeTimerSize() 
		{ 
			return sizeof(m_nNoticeTimer)/sizeof(m_nNoticeTimer[0]); 
		}

		int GetNoticeNumberCount() { return m_nNoticeNumberCount; }
		int GetNoticeNumberSize() 
		{ 
			return sizeof(m_nNoticeNumber)/sizeof(m_nNoticeNumber[0]);
		}

		void AddNoticeTimer(int nValue) 
		{ 
			if (m_nNoticeTimerCount < GetNoticeTimerSize())
			{
				m_nNoticeTimer[m_nNoticeTimerCount ++] = nValue;
			}
		}

		int GetNoticeTimer(int nIndex) 
		{
			if (nIndex < GetNoticeTimerCount())
			{
				return m_nNoticeTimer[nIndex];
			}
		}


		void AddNoticenumber(int nValue);
		{ 
			if (m_nNoticeNumberCount < GetNoticeNumberSize())
			{
				m_nNoticeNumber[m_nNoticeNumberCount ++] = nValue;
			}
		}

		int GetNoticeNumber(int nIndex) 
		{
			if (nIndex < GetNoticeNumberCount())
			{
				return m_nNoticeNumber[nIndex];
			}
		}
		*/
};

inline uint64_t world_trans_id (int nWorldID, uint32_t unPurchaseID)
{
	//nWorldID must be = MMM *  100 + NN
	//MMM = world_id   NN = server_id
	//world_trans_id must be  15 digits like as 101010000000001

	return nWorldID * 10000000000ll + unPurchaseID;
}

inline void local_trans_id( uint64_t ullWorldTransID, int* pnWorldID, uint32_t* punPurchaseID)
{
	 *punPurchaseID = ullWorldTransID % 10000000000ll;
	 *pnWorldID = (ullWorldTransID / 10000000000ll);
}

enum{
	FUNC_TEMPLATE_DXBUFFID		= 34001,		//双倍经验的buffid		
	FUNC_TEMPLATE_BlackPillID	= 70007,		//黑狗丸大  ID
	FUNC_TEMPLATE_PROTECTTIME	= 56005,		//大护法金丹 buffID
};

//应只有5 #define BIND_MONEY 1; UNBIND_MONEY  2;  LINEKONG_MONEY   3; SILVER_YUANBAO   4; GOLDEN_YUANBAO   5

enum{
	FUNC_CONSUME_NOMARL_PRICE	= 0,	//直接用可用的价钱
	FUNC_CONSUME_FROM_TAMPLATE	= 1,	//从模板读，然后和m_szFuncPrice相乘
};
//功能付费单位
class CFuncConsume
{
	private:

		//struct Price{
		//	int64	mPrice;			//价值
		//	bool	mFlag;			//是否生效
		//};
	private:
		int m_nFuncID;			   // 功能ID
		//int	m_nFuncPrice;		   // 功能加个
		int	m_nFuncValueType;		// value类型 price /template_rate 
		int m_nFuncUnitType;
		int m_nFuncUnit;			// 功能消耗单位
		char m_szFuncName[32+1];  // 功能名称
		char m_szFuncComment[255+1]; // 功能描述

		int	 m_szFuncValue[FUNC_CONSUME_COUNT];		//消费种类价格
		int m_nIsShow;				// 是否在界面显示
		int m_nDefaultType;			// 默认消费类型
		int mCountLimit;			// 一次限制购买的数量

	public:
		CFuncConsume( ){
			m_nFuncID = -1;			   // 功能ID
			//int	m_nFuncPrice;		   // 功能加个
			m_nFuncValueType = -1;		// value类型 price /template_rate 
			m_nFuncUnitType = -1;
			m_nFuncUnit = -1;			// 功能消耗单位

			memset(m_szFuncName ,0 ,sizeof(m_szFuncName));

			memset( m_szFuncComment,0, sizeof(m_szFuncComment));

			memset(m_szFuncValue, 0, sizeof(m_szFuncValue));
			m_nIsShow = -1;				// 是否在界面显示
			m_nDefaultType = -1;			// 默认消费类型
			mCountLimit = 0;
		};
		~CFuncConsume( ){};
		//m_nFuncID
		int GetFuncID( ){ return m_nFuncID ;}
		void SetFuncID( int nFuncID ){ m_nFuncID = nFuncID; }

		////m_nFuncPrice
		//int GetFuncPrice( ){ return m_nFuncPrice ; }
		//void SetFuncPrice( int nFuncPrice ){  m_nFuncPrice = nFuncPrice }

		//m_szFuncPriceType 
		int GetFuncValueType(){ return m_nFuncValueType; }
		void SetFuncValueType( int nFuncValueType ) { m_nFuncValueType = nFuncValueType; }

		//m_nConsumeUnit
		int GetFuncUnit( ){ return m_nFuncUnit; }
		void SetFuncUnit( int nFuncUnit ){ m_nFuncUnit = nFuncUnit ;}

		//m_nConsumeType
		int GetFuncUnitType(){ return m_nFuncUnitType;}
		void SetFuncUnitType( int nFuncUnitType ){ m_nFuncUnitType = nFuncUnitType;}
		//m_szGoodsName
		const char * GetFuncName( ) const{ return m_szFuncName; }
		void SetFuncName( const char* szFuncName )
		{
			strncpy(m_szFuncName, szFuncName, sizeof(m_szFuncName)); 
			m_szFuncName[sizeof(m_szFuncName)-1] = 0;
		}

		//m_szFuncComment
		const char* GetFuncComment( ) const{ return m_szFuncComment; }
		void SetFuncComment( const char * szFuncComment )
		{
			strncpy(m_szFuncComment, szFuncComment, sizeof( m_szFuncComment ) );
			m_szFuncComment[sizeof(m_szFuncComment) - 1] = 0;
		}

		//m_szFuncPrice  return -1无效
		int GetFuncValue( int vConsume ) const{ 
			if( vConsume <= 0 || vConsume > FUNC_CONSUME_COUNT )
			{
				return ERR_INVALID_PARAMETER;
			}
			return m_szFuncValue[vConsume - 1];
		}
		void SetFuncValue( int nValue, int nConsume)
		{
			if( nConsume <= 0 || nConsume > FUNC_CONSUME_COUNT )
				return;
			m_szFuncValue[nConsume - 1] = nValue;
			return;
		}

		void SetFuncIsShow(int vIsShow)
		{
			m_nIsShow = vIsShow;
		}

		int GetFuncIsShow()
		{
			return m_nIsShow;
		}

		void SetDefaultType( int vType )
		{
			m_nDefaultType = vType;
		}

		int GetDefaultType()
		{
			return m_nDefaultType;
		}

		void SetFuncCountLimit( int vCountLimit ){ mCountLimit = vCountLimit ;}

		int GetFuncCountLimit(){ return mCountLimit ;}
};		
#endif
