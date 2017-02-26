#pragma once

#include "base_define.h"
#include "lk_vector.h"
#include "lk_string.h"
#include "lk_hashmap.h"
#include "servertool.h"
#include "module_manager.h"
#include "rankmessage_pb.hxx.pb.h"
#include "gatelogic.h"
#include "ranktype.h"
#include <iostream>
#include <sstream>
#include <map>

class CMessage;
class CSharedMem;
using namespace GateServer;


#define MAX_COUNTRY   6 // 最大国家数
#define TOPTEN_TYPE	  2 // 
#define TOPTEN_VALUE  10 //


template< int Max, class Node, int CountryNum >
class CRankBoard
{
public:
	// 国籍
	class CCountryRank
	{
	public:
		CCountryRank( )
		{
			if( CObj::msCreateMode == SHM_INIT )
			{
				mNum = 0;
				mKeyMap.initailize();
			}
		}
		~CCountryRank() {}
		unsigned int Size() { return mNum; }
		unsigned int MaxSize() { return Max; }
		
		// value数组
		int mNum;
		Node mContainer[Max];		

		// first: key值 ; second: 数组index
		typedef lk::hash_map< unsigned int, unsigned int, Max> KeyMap;
		KeyMap mKeyMap;

		std::string show( int iCountry )
		{
			std::stringstream lstrbuf;
			lstrbuf<< "CCountryRank::Show Country("<<iCountry<<") Num("<<mNum<<") \n";
			for ( int i=0; i<mNum; i++)
			{
				if ( mNum>=10 )
				{
					break;
				}
				lstrbuf<< mContainer[i].Show() << "\n";
			}
			lstrbuf << std::ends;
			return lstrbuf.str();
		}

		int ClearData()
		{
			for (int i = 0 ; i < std::min(mNum, Max); ++i )
				mContainer[i].Initialize();
			mNum = 0;
			mKeyMap.clear();
			return 0;
		}

		// 根据数组下标 删除一行
		int RemoveLine( int nIndex )
		{
			if ( nIndex < 0 || nIndex >= mNum )
				return -1;

			// 从数组中删除
			for ( int i = nIndex; i < std::min(mNum, Max)-1; ++i )
			{
				LOG_DEBUG("rank", "key %d = key %d", mContainer[i].HashKey(), mContainer[i+1].HashKey() );
				mContainer[i] = mContainer[i+1];
			}
			if ( mNum > Max )
			{
				LOG_ERROR("rank", "num > max? num=%d", mNum );
				return -1;
			}
			mContainer[mNum-1].Initialize();
			--mNum;
			LOG_NOTICE("rank", "remove line:%d now num:%d", nIndex, mNum );
			return 0;
		}

		// 根据数组得到指定数据
		Node* GetNodeByIndex( int nIndex )
		{
			if ( nIndex < 0 || nIndex >= mNum )
				return NULL;
			return &mContainer[nIndex];
		}

		// 重建索引
		void RebuildIndex()
		{
			mKeyMap.clear();
			for ( int i = 0; i < std::min(mNum, Max); ++i )
			{
				std::pair<typename KeyMap::iterator, bool> ret = 
					mKeyMap.insert( typename KeyMap::value_type(mContainer[i].HashKey(),i) );
				if (!ret.second)
				{
					LOG_ERROR("rank", "RebuildIndex error! i=%d key=%d", i, mContainer[i].HashKey() );
				}
			}
		}

		// 检查 返回0成功
		int CheckRank()
		{
			std::vector<int> _todel; // 要删除的下标们

			for (int i = 0 ; i < std::min(mNum, Max); ++i )
			{
				unsigned int unKey = mContainer[i].HashKey();
				if (unKey == 0 || !mContainer[i].MeetRequirement()) // 非法数据
				{
					_todel.push_back(i);
					continue;
				}

				typename KeyMap::iterator it = mKeyMap.find(unKey);
				if ( it == mKeyMap.end() ) // 没找到
				{
					_todel.push_back(i);
				}
				else // 找到了
				{
					if ( i != (int)it->second ) // 不一致 干掉
					{
						_todel.push_back(i);
					}
				}
			}

			// 干掉
			for ( std::vector<int>::iterator it = _todel.begin(); it != _todel.end(); ++it )
			{
				// 每移除1个 后面的索引都要减一！
				RemoveLine(*it-(it-_todel.begin()));
			}
			// 重建索引
			RebuildIndex();
			return 0;
		}

		// 从排行榜中删除指定char id
		int RemoveFromRank( int nCharID )
		{
			for( int i = 0; i < mNum; ++i )
			{
				if( mContainer[ i ].HashKey( ) == (unsigned int) nCharID )
				{
					RemoveLine( i );
				}
			}

			// 重建索引
			RebuildIndex();
	
			return 0;		// fuck write
		}

	};


public:

	CCountryRank* GetCountryRank( int nCountryID )
	{
		LK_ASSERT( nCountryID >= 0 && nCountryID < CountryNum, return NULL );
		return &mCountryRank[nCountryID];
	}

	// ContryID=0表示世界 其他表示国家
	int GetTopTen( int* vpCharList, int vNum, int nCountryID )
	{
		if ( vpCharList == NULL )
		{
			return -1;
		}
		if ( nCountryID < 0 || nCountryID >= CountryNum )
		{
			return -1;
		}
		int i = 0;
		for ( i = 0; i < Max && i < vNum && i < mCountryRank[nCountryID].mNum; i ++ )
		{
			vpCharList[ i ] = mCountryRank[nCountryID].mContainer[i].HashKey();
		}
		return i;
	}

	// 更新排行榜 (只用于玩家数据的更新 不用于数据库载入)
	int Update( const Node& rNode, bool bIsUpdateWorld )
	{
		LOG_INFO( "rank", "[%s] begin", __FUNCTION__ );
		int nResult = 0;
		
		// 更新本国榜单
		if ( 0 == rNode.GetCountry() ) // 没国籍的不参与
			return -1;

		LOG_INFO("rank", "test-debug func=%s, Player country=%d, hash_key=%d ", __FUNCTION__, rNode.GetCountry(), rNode.HashKey() );
		if ( rNode.GetCountry()==1 )
		{
			LOG_NOTICE( "rank", "before : %s", mCountryRank[rNode.GetCountry()].show( rNode.GetCountry() ).c_str() ); 
		}

		nResult = UpdateByCountry( rNode, rNode.GetCountry() );
		//if( nResult == 0 )
		{
			LOG_DEBUG( "rank", "Native Update: %s", rNode.Show() );
		}

		// 更新世界榜单
		if (bIsUpdateWorld)
		{
			LOG_DEBUG("rank", "test-debug func=%s, UpdateWorld Player country=%d, hash_key=%d", __FUNCTION__, rNode.GetCountry(), rNode.HashKey() );
			nResult = UpdateByCountry( rNode, 0 );
		}

		if ( rNode.GetCountry()==1 )
		{
			LOG_NOTICE( "rank", "after : %s", mCountryRank[rNode.GetCountry()].show( rNode.GetCountry() ).c_str() ); 
		}

		LOG_INFO( "rank", "[%s] finished", __FUNCTION__ );

		return 0;
	}

	// 处理从数据库加载进来的排行榜信息
	// 为了和老的兼容 2个参数都是从1开始！用的时候要-1
	int OnLoadRankData( const Node& rNode, int nIndex , int nRankCountry )
	{
		if ( nIndex <= 0 || nIndex > Max || nRankCountry <= 0 || nRankCountry > CountryNum )
			return -1;

		LOG_INFO("rank", "[%s], load data index=%d, country=%d, info=%s", __FUNCTION__, nIndex, nRankCountry, rNode.Show());

		std::pair<typename CCountryRank::KeyMap::iterator, bool> ret;
		ret = mCountryRank[nRankCountry-1].mKeyMap.insert( typename CCountryRank::KeyMap::value_type(rNode.HashKey(),nIndex-1) );
		if( ret.second == false )
		{
			LOG_ERROR( "rank", "[%s] load rank data failed! nIndex(%d) nRankCountry(%d) Rank(%s)", __FUNCTION__, nIndex, nRankCountry, rNode.Show() );
			return -2;
		}

		mCountryRank[nRankCountry-1].mContainer[nIndex-1] = rNode;
		++mCountryRank[nRankCountry-1].mNum;

		return 0;
	}
		//int PrepareSqlForInTimeTable( unsigned int unCountry, char* pszBuffer, int& nLen )
	//{
	//	LK_ASSERT( unCountry <= CountryNum, return -1 );

	//	int nTotalLen = 0;
	//	int nMaxLen = nLen;

	//	nTotalLen = snprintf( pszBuffer+nTotalLen, nMaxLen-nTotalLen, "%s", mCountryRank[0].mContainer[0].PreSqlStmtForInTimeTable( ) );
	//	
	//	for( unsigned int i = 0; i < mCountryRank[unCountry].Size(); i++ )
	//	{
	//		nTotalLen += snprintf( pszBuffer+nTotalLen, nMaxLen-nTotalLen, "%s%s",
	//			mCountryRank[unCountry].mContainer[i].ValueStmt(),
	//			i == mCountryRank[unCountry].Size()-1 ? ";" : "," );
	//	}

	//	nLen = nTotalLen;

	//	return 0;
	//}


	// 准备存数据库的SQL
	int PrepareSqlForRankTable( unsigned int unCountry, char* pszBuffer, int& nLen, int nFlag, bool bIsOutdated )
	{
		LK_ASSERT( unCountry <= CountryNum, return -1 );

		int nTotalLen = 0;
		int nMaxLen = nLen;

		if (bIsOutdated)
		{
			nTotalLen = snprintf( pszBuffer+nTotalLen, nMaxLen-nTotalLen, "%s", mCountryRank[0].mContainer[0].PreSqlStmtmForOutdatedTable( ) );
			LOG_NOTICE( "rank", "[%s] PreSqlStmtForInTimeTable( %s )", 
				__FUNCTION__, mCountryRank[0].mContainer[0].PreSqlStmtmForOutdatedTable( ) );
		}
		else
		{
			nTotalLen = snprintf( pszBuffer+nTotalLen, nMaxLen-nTotalLen, "%s", mCountryRank[0].mContainer[0].PreSqlStmtForInTimeTable( ) );
			LOG_NOTICE( "rank", "[%s] PreSqlStmtForInTimeTable( %s )", 
				__FUNCTION__, mCountryRank[0].mContainer[0].PreSqlStmtForInTimeTable( ) );
		}

		for( unsigned int i = 0; i < mCountryRank[unCountry].Size(); ++i )
		{
			nTotalLen += snprintf( pszBuffer+nTotalLen, nMaxLen-nTotalLen, "%s%s",
				mCountryRank[unCountry].mContainer[i].ValueStmt(nFlag, bIsOutdated, i+1, unCountry+1),
				i == mCountryRank[unCountry].Size()-1 ? ";" : "," );

			LOG_NOTICE( "rank", "[%s] nFlag(%d) i+1(%d) unCountry+1(%d) data( %s )", 
				__FUNCTION__, nFlag, (i+1), (unCountry+1), 
				mCountryRank[unCountry].mContainer[i].ValueStmt(nFlag, bIsOutdated, i+1, unCountry+1) );
		}

		nLen = nTotalLen;

		return 0;
	}

	// 把当前排行写入数据库 参数：排行榜类型，是否存的是历史表
	int SaveRankDataToDB( int nFlag, bool bIsOutdated )
	{
		char tSqlStmt[ 40000 ] = {0};
		time_t t = time( 0 );
		tm tmp;
		localtime_r( &t, &tmp );

		for( unsigned int i = 0; i < CountryNum; ++i )
		{
			int nStmtLen = sizeof(tSqlStmt);
			LOG_NOTICE("rank", "[%s], begin : rank_type=%d, isout=%d, rank_num=%d key_size=%d cur country=%d", __FUNCTION__, nFlag
				, bIsOutdated, mCountryRank[i].Size(), mCountryRank[i].mKeyMap.size(), i );
			if (mCountryRank[i].Size() == 0)
				continue;

			if (bIsOutdated)
			{
				const char* szDelStmt = "DELETE FROM UMS_RANK_PLAYER_OUTDATED WHERE rank_type = %d and rank_country = %d and rank_date='%04d-%02d-%02d'";
				CGateLogic::GetSingleton().ExecuteSql( em_dbsession_null, 0, 0, 1, SQL_NONE, DELETE, szDelStmt, nFlag, i+1, tmp.tm_year+1900, tmp.tm_mon+1, tmp.tm_mday); 
			}
			else
			{
				const char* szDelStmt = "DELETE FROM UMS_RANK_PLAYER WHERE rank_type = %d and rank_country = %d";
				CGateLogic::GetSingleton().ExecuteSql( em_dbsession_null, 0, 0, 1, SQL_NONE, DELETE, szDelStmt, nFlag, i+1); 
			}

			PrepareSqlForRankTable( i, tSqlStmt, nStmtLen, nFlag, bIsOutdated );
			if( CGateLogic::GetSingleton().ExecuteSql( em_dbsession_save_rank, 0, 0, 
				1, SQL_NONE, INSERT, tSqlStmt, nStmtLen ) == false )
			{
				LOG_ERROR( "rank", "[%s:%d] exec sql %s failed", __FUNCTION__, __LINE__, tSqlStmt );
				return -1;
			}

			LOG_NOTICE("rank", "[%s], finished : rank_type=%d, isout=%d, rank_num=%d key_size=%d cur country=%d", __FUNCTION__, nFlag
				, bIsOutdated, mCountryRank[i].Size(), mCountryRank[i].mKeyMap.size(), i );
		}

		return 0;
	}

	// 和老的比较 计算升降 等
	// 参数nContinusOnboardLimit:连续上榜次数上限(达到后变1)
	int CompareWithOld( CRankBoard<Max, Node, CountryNum>& rOldRank, unsigned int nContinusOnboardLimit )
	{
		for (int i = 0; i < CountryNum; ++i )
		{
			typename CRankBoard<Max, Node, CountryNum>::CCountryRank* pOldCountryData = rOldRank.GetCountryRank(i);
			LK_ASSERT(pOldCountryData!=NULL, continue); // 不可能

			for (int j = 0; j < Max; ++j )
			{
				unsigned int unThisKey = mCountryRank[i].mContainer[j].HashKey( );
				typename CCountryRank::KeyMap::iterator _old = pOldCountryData->mKeyMap.find( unThisKey );
				if ( _old == pOldCountryData->mKeyMap.end() ) // 如果没找到 则是新上榜的
				{
					mCountryRank[i].mContainer[j].SetRankChange(NEW_RANK_FLAG); // 新上榜的 设置标记
					if (j < CONTINUS_ONBOARD_TOPNUM)
						mCountryRank[i].mContainer[j].SetCoutinuseOnBoard(1); 
					else
						mCountryRank[i].mContainer[j].SetCoutinuseOnBoard(0); 

					if ( j == 0 ) // 这次第一名
						mCountryRank[i].mContainer[j].SetCoutinuseFirst(1); 
				}
				else // 上次就上榜的
				{
					int nLastIndex = _old->second;
					LK_ASSERT( nLastIndex >= 0 && nLastIndex < Max, continue );
					if (j < CONTINUS_ONBOARD_TOPNUM)
					{
						if (pOldCountryData->mContainer[j].GetCoutinuseOnBoard() < (int)nContinusOnboardLimit)
							mCountryRank[i].mContainer[j].SetCoutinuseOnBoard(pOldCountryData->mContainer[nLastIndex].GetCoutinuseOnBoard()+1); 
						else // 重置啦(TODO以后可能给奖励)
							mCountryRank[i].mContainer[j].SetCoutinuseOnBoard(1); 
					}
					else // 跌出前10
					{
						mCountryRank[i].mContainer[j].SetCoutinuseOnBoard(0); 
					}

					if ( j == 0 )// 第一名
					{
						if (pOldCountryData->mContainer[j].GetCoutinuseFirst() < (int)nContinusOnboardLimit)
							mCountryRank[i].mContainer[j].SetCoutinuseFirst(pOldCountryData->mContainer[nLastIndex].GetCoutinuseFirst()+1); 
						else // 重置啦(TODO以后可能给奖励)
							mCountryRank[i].mContainer[j].SetCoutinuseFirst(1); 
					}
					else
					{
						mCountryRank[i].mContainer[j].SetCoutinuseFirst(0); 
					}

					mCountryRank[i].mContainer[j].SetRankChange(nLastIndex-j);
					//if ( j > nLastIndex ) // 下降
					//{
					//	mCountryRank[i].mContainer[j].SetRankChange(nLastIndex-j);
					//}
					////else if ( j < nLastIndex ) // 上升了
					////{
					////	mCountryRank[i].mContainer[j].SetRankChange(1);
					////}
					//else
					//{
					//	mCountryRank[i].mContainer[j].SetRankChange(0);  // 没变
					//}
				}
			}
		}
		return 0;
	}

	// 清空排行榜
	int ClearData()
	{
		for (int i = 0 ; i < CountryNum; ++i )
			mCountryRank[i].ClearData();
		return 0;
	}

	// 检查排行榜逻辑 防止出现一样的 0之类的
	int CheckRank()
	{
		for (int i = 0 ; i < CountryNum; ++i )
		{
			mCountryRank[i].CheckRank();
		}
		return 0;
	}

	int RemoveFromRank( int nCharID )
	{
		for (int i = 0; i < CountryNum; ++i )
		{
			mCountryRank[i].RemoveFromRank( nCharID );
		}

		return 0;
	}

protected:

	// 更新排行榜
    int UpdateByCountry( const Node& rNode, unsigned int unCountry )
	{
		// 如果这个排行榜不排国家的 直接返回
		if( unCountry >= CountryNum )
			return -1 ;

		LOG_DEBUG("rank", "test-debug func=%s, unCountry=%d, CountryNum=%d", __FUNCTION__, unCountry, CountryNum );

		// 指定国排行榜当前榜单长度
		CCountryRank& rTheCountryRank = mCountryRank[unCountry];
		int nNativeSize = rTheCountryRank.Size();
		int nMaxSize = rTheCountryRank.MaxSize();

		// 是否能找到相同key的节点
		bool bFindSameNode = false;
		int nNodeChange = 0; // -1 减小; 0 不变; 1 变大

		// 查找是否存在相同节点，并判断节点的变化
		typename CCountryRank::KeyMap::iterator itFind = rTheCountryRank.mKeyMap.find( rNode.HashKey() );
		if( itFind != rTheCountryRank.mKeyMap.end() )
		{
			unsigned int unIndex = itFind->second;

			if ( rNode.HashKey() != rTheCountryRank.mContainer[ unIndex ].HashKey() )
			{
				LOG_ERROR( "rank", "error rNode! rNode.HashKey=%d, index in map=%d, truely hashkey=%d show=%s", rNode.HashKey()
					, unIndex, rTheCountryRank.mContainer[ unIndex ].HashKey(), rNode.Show() );
				return -1;
			}
			
			if( rNode < rTheCountryRank.mContainer[ unIndex ] )
			{
				nNodeChange = -1;
			}
			else if( rNode > rTheCountryRank.mContainer[ unIndex ] )
			{
				nNodeChange = 1;
			}

			rTheCountryRank.mContainer[ unIndex ] = rNode;
			bFindSameNode = true;
			LOG_NOTICE("rank", "test-debug func=%s, exsited!! node change=%d %s", __FUNCTION__, nNodeChange, rNode.Show() );
		}

		// 排除不满足排行要求的节点
		if( rNode.MeetRequirement() == false )
		{
			LOG_NOTICE("rank", "test-debug func=%s, not meet! %s", __FUNCTION__, rNode.Show() );
			// 如果排行榜里面已经有该节点了，则删除。
			if( bFindSameNode == true )
			{
				int nCurrent = itFind->second;
				nCurrent++;
				while( nCurrent < nNativeSize )
				{
					//typename CCountryRank::KeyMap::iterator itFind2 = rTheCountryRank.mKeyMap.end();
					//// 修改map中的secode: 数组下标.
					//itFind2 = rTheCountryRank.mKeyMap.find( rTheCountryRank.mContainer[ nCurrent ].HashKey() );
					//if(  itFind2 != rTheCountryRank.mKeyMap.end() )
					//{
					//	itFind2->second = nCurrent-1;
					//}

					//itFind2 = rTheCountryRank.mKeyMap.find( rTheCountryRank.mContainer[ nCurrent + 1].HashKey() );
					//if( itFind2 != rTheCountryRank.mKeyMap.end() )
					//{
					//	itFind2->second = nCurrent+1;
					//}

					rTheCountryRank.mContainer[ nCurrent-1 ]
					= rTheCountryRank.mContainer[ nCurrent ];

					nCurrent = nCurrent + 1;
				}

				// 初始化掉最后一个节点，不保留残余数据
				nCurrent--;
				rTheCountryRank.mContainer[ nCurrent ].Initialize();
				rTheCountryRank.mNum--;

				//rTheCountryRank.mKeyMap.erase( itFind );
				// 数组顺序改变以后 重建索引
				rTheCountryRank.RebuildIndex();
				return 0;
			}

			return -1;
		}

		// pos 为数组最大值表示 没有合适位置
		int nPos = nMaxSize;

	
		// 如果找到相同的节点，只需重新排序
		if( bFindSameNode == true )
		{
			// 对修改的位置重新定位, 如果比先前值小
			if( nNodeChange == -1 )
			{
				int nCurrent = itFind->second;

				while( nCurrent < nNativeSize-1 && 
					!( rTheCountryRank.mContainer[ nCurrent ] > rTheCountryRank.mContainer[ nCurrent + 1 ] ) )
				{
					LOG_NOTICE("rank", "--(chg-1)%d swap to %d", nCurrent, nCurrent+1 );
					// swap 
					Node tmp = rTheCountryRank.mContainer[ nCurrent ];

					rTheCountryRank.mContainer[ nCurrent ]
						= rTheCountryRank.mContainer[ nCurrent + 1 ];

					rTheCountryRank.mContainer[ nCurrent + 1 ] = tmp;

					// 同时修改map中的secode: 数组下标.
					//itFind = rTheCountryRank.mKeyMap.find( rTheCountryRank.mContainer[ nCurrent ].HashKey() );
					//if(  itFind != rTheCountryRank.mKeyMap.end() )
					//{
					//	itFind->second = nCurrent;
					//}

					//itFind = rTheCountryRank.mKeyMap.find( rTheCountryRank.mContainer[ nCurrent + 1].HashKey() );
					//if( itFind != rTheCountryRank.mKeyMap.end() )
					//{
					//	itFind->second = nCurrent+1;
					//}

					nCurrent = nCurrent + 1;
				}
			}
			else if( nNodeChange == 1 ) // 大于先前值
			{
				int nCurrent = itFind->second;
				while( nCurrent > 0 && 
					(rTheCountryRank.mContainer[ nCurrent - 1 ] < rTheCountryRank.mContainer[ nCurrent ] ) )
				{
					//LOG_NOTICE("rank", "--(chg+1)%d swap to %d", nCurrent, nCurrent-1 );
					Node tmp = rTheCountryRank.mContainer[ nCurrent ];

					rTheCountryRank.mContainer[ nCurrent ]
					= rTheCountryRank.mContainer[ nCurrent - 1 ];

					rTheCountryRank.mContainer[ nCurrent - 1 ] = tmp;

					//itFind = rTheCountryRank.mKeyMap.find( rTheCountryRank.mContainer[ nCurrent ].HashKey() );
					//if( itFind != rTheCountryRank.mKeyMap.end() )
					//{
					//	itFind->second = nCurrent;
					//}

					//itFind = rTheCountryRank.mKeyMap.find( rTheCountryRank.mContainer[ nCurrent - 1].HashKey() );
					//if( itFind != rTheCountryRank.mKeyMap.end() )
					//{
					//	itFind->second = nCurrent-1;
					//}

					nCurrent = nCurrent - 1;
				}
			}
			// 数组顺序改变以后 重建索引
			rTheCountryRank.RebuildIndex();
			return 0;
		}

		// 为新节点找合适位置插入
		for( int i = 0; i < nMaxSize ; i++ )
		{
			if( rTheCountryRank.mContainer[ i ] < rNode )
			{
				//LOG_NOTICE("rank", "test-debug func=%s, add new!", __FUNCTION__ );
				nPos = i;
				break;
			}
		}

		// 判断是否找到合适位置，没有则返回
		if( nPos == nMaxSize )
		{
			return -1;
		}

		// 满的话最后一个会被干掉 先把最后1个从map里干掉 (反正要重建索引 先不干掉)
		//if ( nNativeSize == nMaxSize )
		//{
		//	itFind = rTheCountryRank.mKeyMap.find( rTheCountryRank.mContainer[ nNativeSize-1 ].HashKey() );
		//	if( itFind != rTheCountryRank.mKeyMap.end() )
		//	{
		//		rTheCountryRank.mKeyMap.erase( itFind );
		//	}
		//}

		// 从倒数第一个值开始移位
		for( int k = (nNativeSize-1); (k >= nPos) && (nNativeSize > 0); k-- )
		{
			if( k < nMaxSize-1 )
			{
				rTheCountryRank.mContainer[ k+1 ] = rTheCountryRank.mContainer[k];

				//itFind = rTheCountryRank.mKeyMap.find( rTheCountryRank.mContainer[ k + 1].HashKey() );
				//if( itFind != rTheCountryRank.mKeyMap.end() )
				//{
				//	itFind->second = k+1;
				//}
			}
		}

		// 赋值
		//LOG_NOTICE("rank", "test-debug func=%s, replace!", __FUNCTION__ );
		rTheCountryRank.mContainer[ nPos ] = rNode;

		// 如果总数没够上限  才++！ 如果达到了上限  再加就越界了 不加的话没事啊 因为已经把最后一名挤掉了。
		if ( nNativeSize < nMaxSize )
			rTheCountryRank.mNum++;

		//rTheCountryRank.mKeyMap.insert( typename CCountryRank::KeyMap::value_type(rNode.HashKey(),nPos));
		//std::pair<typename CCountryRank::KeyMap::iterator,bool> ret 
		//	= rTheCountryRank.mKeyMap.insert( typename CCountryRank::KeyMap::value_type(rNode.HashKey(),nPos));
		//if (!ret.second)
		//{
		//	LOG_ERROR("rank", "insert into map error! hashkey=%d pos=%d", rNode.HashKey(), nPos);
		//	return -1;
		//}

		// 数组顺序改变以后 重建索引
		rTheCountryRank.RebuildIndex();
		return 0;
	}

	CCountryRank mCountryRank[CountryNum]; // 0世界 ; 1-6 国家
};


// 排行榜基类
class CRankBase
{
public:
	CRankBase() {}
	virtual ~CRankBase() {}

	virtual int Initialize() = 0;
	
	// 得到hashkey
	virtual unsigned int HashKey( ) const = 0;

	// 取得国籍( 排行榜很多是按国籍 )
	virtual unsigned int GetCountry( ) const = 0;	
	
	// 是否满足排行条件
	virtual bool MeetRequirement( ) const = 0;		
	
	// 存盘sql语句Value部分
	virtual const char* ValueStmt( int nFlag, bool bIsOutdated, int nIndex, int nRankCountry ) const = 0;

};


// 单条玩家类的排行
class CPlayerRankMeta : public CRankBase
{
public:
	CPlayerRankMeta()
	{
		if( CObj::msCreateMode == SHM_INIT )
		{
			Initialize();
		}
	}

	CPlayerRankMeta(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle,  unsigned int unMetier, unsigned long long unValue2 = 0, int nRankChange = 0
		, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0) :
			mCharID( unCharID ), mCharName( pszName ), mValue( unValue),
			mCountry( unCountry ), mTitle( unTitle ), mMetier( unMetier ), mValue2( unValue2 ), 
			mRankChange(nRankChange), mCoutinuseOnBoard(nCoutinuseOnBoard), mCoutinuseFirst(nCoutinuseFirst)
	{
	}

	void SetValue2( unsigned long long unValue2 ) { mValue2 = unValue2; }

	virtual ~CPlayerRankMeta() {}

	virtual int Initialize()
	{
		mCharID = 0;
		mCharName.clear();
		mValue = 0;
		mCountry = 0;
		mTitle = 0;
		mMetier = 0;
		mValue2 = 0;
		mRankChange = 0;		// 排名变化
		mCoutinuseOnBoard = 0;	// 连续上榜数
		mCoutinuseFirst  = 0;	// 连续蝉联第一数
		return 0;
	}

	// 填充排行榜消息数据(不含排名)
	virtual void FillMessageData( PBSingleRankInfo* pMsgData )
	{
		pMsgData->set_charid( mCharID );
		pMsgData->set_charname( mCharName.c_str() );
		pMsgData->set_rankvalue( mValue );
		pMsgData->set_country( mCountry );
		pMsgData->set_metier( mMetier );
		if ( mRankChange > 0 )
		{
			pMsgData->set_rankchange( 1 );
			pMsgData->set_rankchangeval( mRankChange );
		}
		else if ( mRankChange < 0 )
		{
			pMsgData->set_rankchange( 2 );
			pMsgData->set_rankchangeval( 0-mRankChange );
		}
		else
			pMsgData->set_rankchange( 0 ); // 没变

		if ( mRankChange == NEW_RANK_FLAG ) // 新上榜的
			pMsgData->set_rankchangeval( 0 );

		//pMsgData->set_rankchange( mRankChange );
		pMsgData->set_continuousboard( mCoutinuseOnBoard );
		pMsgData->set_continuousfirst( mCoutinuseFirst );
		pMsgData->set_title( mTitle );
	}

	// 得到hashkey
	virtual unsigned int HashKey( ) const { return mCharID; }

	// 进入排行版最低要求
	virtual bool MeetRequirement() const = 0;

	// 输出结构字节流
	virtual const char* Show( ) const = 0;

	// 存盘sql语句  参数：当前正在存的排行榜类型 是否历史表 索引 排行榜国家数组索引
	virtual const char* ValueStmt( int nFlag, bool bIsOutdated, int nIndex, int nRankCountry ) const 
	{
		time_t t = time( 0 );
		tm tmp;
		localtime_r( &t, &tmp );

		static char _value[256];
		memset( _value, 0, sizeof(_value) );

		// escape 名字
		char tEscapeBuff[NAME_LENGTH*8] = { 0 };		
		char *tpshopEscapePtr =	tEscapeBuff;
		EscapeDataString( &tpshopEscapePtr, NAME_LENGTH*8, (char*)mCharName.c_str(), strlen(mCharName.c_str()) );

		if (!bIsOutdated)
		{
			snprintf( _value, sizeof(_value)-1, "(%d,'%s',%d,%d,%d,%d,'%04d-%02d-%02d', %d, %d, %d, %lld)",
				mCharID, tEscapeBuff, mValue, mCountry, mTitle, mMetier, tmp.tm_year+1900, tmp.tm_mon+1, tmp.tm_mday, nFlag, nIndex, nRankCountry, mValue2);
		}
		else
		{
			snprintf( _value, sizeof(_value)-1, "(%d,'%s',%d,%d,%d,%d,'%04d-%02d-%02d', %d, %d, %d, %d, %d, %d, %lld)",
				mCharID, tEscapeBuff, mValue, mCountry, mTitle, mMetier, tmp.tm_year+1900, tmp.tm_mon+1, tmp.tm_mday, nFlag, mRankChange, mCoutinuseOnBoard, mCoutinuseFirst, nIndex, nRankCountry, mValue2);
		}
		return _value;
	}

	// 必须的
	virtual unsigned int GetCountry( ) const { return mCountry; }
	//inline void SetCountry( unsigned int unCountry ) { mCountry = unCountry; }

	inline unsigned int GetCharID( ) const { return mCharID; }
	//inline void SetCharID( unsigned int unCharID ) { mCharID = unCharID; }

	inline const char* GetCharName( ) const { return mCharName.c_str() ; }

	// 得到GBK编码的用户名
	const char* GetCharNameShow( ) const
	{
		// 字符集用于显示
#ifdef CHARSET_SHOW_GBK
		static char cCharName[ NAME_LENGTH ] = {0};
		int nNameLen = sizeof(cCharName)-1;
		charset_convert( "utf8", "gbk", (char*)mCharName.c_str(), strlen( mCharName.c_str() ),
			cCharName, (size_t*)&nNameLen );
		return cCharName;
#else
		return mCharName.c_str();
#endif
	}
	//inline void SetCharName( const char* pszName ) { mCharName.append( pszName); }

	inline unsigned int GetMetier( ) const { return mMetier; }
	//inline void SetMetier( unsigned int unMetier ) { mMetier = unMetier ;}

	virtual unsigned int GetValue() const { return mValue; }
	virtual unsigned long long GetValue2() const { return mValue2; }
	virtual int GetTitle() const { return mTitle; }

	virtual void SetRankChange( int nRankChange ){ mRankChange = nRankChange; }
	virtual void SetCoutinuseOnBoard( int nCoutinuseOnBoard ){ mCoutinuseOnBoard = nCoutinuseOnBoard; }
	virtual void SetCoutinuseFirst( int nCoutinuseFirst ){ mCoutinuseFirst = nCoutinuseFirst; }

	virtual int GetRankChange( ){ return mRankChange; }
	virtual int GetCoutinuseOnBoard( ){ return mCoutinuseOnBoard ; }
	virtual int GetCoutinuseFirst( ){ return mCoutinuseFirst; }

	// 实时表前置sql语句
	virtual const char* PreSqlStmtForInTimeTable( )
	{
#ifdef _SERVER_214_
		return "INSERT INTO UMS_RANK_PLAYER_214(role_id, role_name, rank_value, role_country, order_title, metier_id, rank_date, rank_type, order_id, rank_country, value2) values" ;
#else
		return "INSERT INTO UMS_RANK_PLAYER(role_id, role_name, rank_value, role_country, order_title, metier_id, rank_date, rank_type, order_id, rank_country, value2) values" ;
#endif
	}

	// 历史表前置sql语句
	virtual const char* PreSqlStmtmForOutdatedTable( )
	{
#ifdef _SERVER_214_
		return "insert into UMS_RANK_PLAYER_OUTDATED_214 (role_id, role_name, rank_value, role_country, order_title, metier_id, rank_date, rank_type, order_change, coutinue_onboard, coutinue_first, order_id, rank_country, value2) VALUES";
#else
		return "insert into UMS_RANK_PLAYER_OUTDATED (role_id, role_name, rank_value, role_country, order_title, metier_id, rank_date, rank_type, order_change, coutinue_onboard, coutinue_first, order_id, rank_country, value2) VALUES";
#endif	
	}


private:
	unsigned int mCharID;	// 角色ID
	lk::string<NAME_LENGTH> mCharName;	// 角色名
	unsigned int mValue;	// 排序数值
	unsigned int mCountry;	// 国家
	unsigned int mTitle;	// 称号
	unsigned int mMetier;	// 门派
	unsigned long long mValue2;	// 排序附加数值
	int	mRankChange;		// 排名变化 正数表示上升 负数表示下跌 0表示平 NEW_RANK_FLAG表示新上榜
	int mCoutinuseOnBoard;	// 连续上榜数
	int mCoutinuseFirst;	// 连续蝉联第一数
};


// 等级排行
class CPlayerLevelRank : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里  等级要参考数值2
	bool operator < (const CPlayerLevelRank& other) const
	{
		if ( this->GetValue() == other.GetValue() )
			return this->GetValue2() < other.GetValue2();
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerLevelRank& other) const
	{
		if ( this->GetValue() == other.GetValue() )
			return this->GetValue2() > other.GetValue2();
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by level]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 以后改成30)
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerLevelRank(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0
		, int nCoutinuseFirst = 0, unsigned long long unValue2 = 0 ) :
		CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst, unValue2 ) 
	{	}

	CPlayerLevelRank()
	{	}
};

// 单行金钱排行
class CPlayerMoneyRank : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerMoneyRank& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerMoneyRank& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by money]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 金钱要求达到金以上才排
	virtual bool MeetRequirement() const { return GetValue() > 10000; }

	CPlayerMoneyRank(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerMoneyRank()
	{	}
};


// 杀敌数量排行
class CPlayerKillRank : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerKillRank& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerKillRank& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by kill]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerKillRank(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerKillRank()
	{	}
};


// 功勋(荣誉)排行
class CPlayerHonorRank : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerHonorRank& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerHonorRank& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by honer]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerHonorRank(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerHonorRank()
	{	}
};


// 死亡次数排行
class CPlayerDieNumRank : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerDieNumRank& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerDieNumRank& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by dienum]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerDieNumRank(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerDieNumRank()
	{	}
};

// 完成任务数量排行
class CPlayerEndTaskRank : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerEndTaskRank& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerEndTaskRank& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by endtask]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerEndTaskRank(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerEndTaskRank()
	{	}
};

// 在线时间排行
class CPlayerOnLineRank : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerOnLineRank& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerOnLineRank& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by online]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerOnLineRank(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerOnLineRank()
	{	}
};



// 蟠桃排行
class CPlayerPantaoRank : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerPantaoRank& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerPantaoRank& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by pantao]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerPantaoRank(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerPantaoRank()
	{	}
};

// 龙宫排行
class CPlayerLonggongRank : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerLonggongRank& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerLonggongRank& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by longgong]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerLonggongRank(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerLonggongRank()
	{	}
};

// 原地复活次数排行榜
class CPlayerOriginReliveTimes : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerOriginReliveTimes& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerOriginReliveTimes& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by CPlayerOriginReliveTimes]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerOriginReliveTimes(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerOriginReliveTimes()
	{	}
};

// 杀死本阵营排行榜
class CPlayerRankKillSameNationNum : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerRankKillSameNationNum& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerRankKillSameNationNum& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by CPlayerRankKillSameNationNum]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerRankKillSameNationNum(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerRankKillSameNationNum()
	{	}
};


// 总杀怪数排行榜
class CPlayerRankKillOgreNum : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerRankKillOgreNum& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

		bool operator > (const CPlayerRankKillOgreNum& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by CPlayerRankKillOgreNum]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerRankKillOgreNum(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerRankKillOgreNum()
	{	}
};


// 升级失败排行榜
class CPlayerRankUpFailNum : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerRankUpFailNum& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerRankUpFailNum& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by CPlayerRankUpFailNum]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerRankUpFailNum(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerRankUpFailNum()
	{	}
};

// 大雁塔排行
class CPlayerRankDayanta : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerRankDayanta& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerRankDayanta& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by Dayanta]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerRankDayanta(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerRankDayanta()
	{	}
};

// 大乱斗排行
class CPlayerRankLastOne : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerRankLastOne& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerRankLastOne& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by LastOne]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerRankLastOne(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerRankLastOne()
	{	}
};


// 莲花洞排行
class CPlayerRankLianhuadong : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerRankLianhuadong& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerRankLianhuadong& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by Lianhuadong]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerRankLianhuadong(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerRankLianhuadong()
	{	}
};

// 飞行副本排行
class CPlayerRankFlyDung : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerRankFlyDung& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerRankFlyDung& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by RankFlyDung]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerRankFlyDung(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerRankFlyDung()
	{	}
};


// 通用榜！【以后一般情况下都用这个就行】
class CPlayerRankCommon : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerRankCommon& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerRankCommon& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank ID=%d]%d\t %s\t %d\t %d\t %d\t %d", 0,
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerRankCommon(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{ }

	CPlayerRankCommon()
	{	}
};


// 黄蜂洞排行
class CPlayerRankHuangfengdong : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerRankHuangfengdong& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerRankHuangfengdong& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by Huangfengdong]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerRankHuangfengdong(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerRankHuangfengdong()
	{	}
};

// 成就排行
class CPlayerRankAchive : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerRankAchive& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerRankAchive& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by Achive]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerRankAchive(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerRankAchive()
	{	}
};

// 好感排行
class CPlayerRankFavor : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerRankFavor& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerRankFavor& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by Favor]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerRankFavor(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerRankFavor()
	{	}
};


// 猥琐排行
class CPlayerRankBadPoint : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerRankBadPoint& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerRankBadPoint& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by BadPoint]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerRankBadPoint(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerRankBadPoint()
	{	}
};


// 魅力值排行
class CPlayerRankCharm : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerRankCharm& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerRankCharm& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by Charm]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerRankCharm(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerRankCharm()
	{	}
};


// 团队荣誉排行
class CPlayerRankTeamPoints : public CPlayerRankMeta
{
public:
	// 必须的
	// 可能不都按按照升序排的 所以放到子类里
	bool operator < (const CPlayerRankTeamPoints& other) const
	{
		return ( this->GetValue() < other.GetValue() );
	}

	bool operator > (const CPlayerRankTeamPoints& other) const
	{
		return ( this->GetValue() > other.GetValue() );
	}

	// 输出结构字节流
	virtual const char* Show( ) const
	{
		static char _buff[256];
		memset( _buff, 0, sizeof(_buff) );
		snprintf( _buff, sizeof(_buff)-1, "[rank by TeamPoints]%d\t %s\t %d\t %d\t %d\t %d",
			GetCharID(), GetCharNameShow(), GetValue(), GetCountry(), GetTitle(), GetMetier() );
		return _buff;
	}

	// 进入排行版最低要求 (TODO 测试用 )
	virtual bool MeetRequirement() const { return GetValue() > 0; }

	CPlayerRankTeamPoints(unsigned int unCharID, const char* pszName, unsigned int unValue, 
		unsigned int unCountry, unsigned int unTitle, unsigned int unMetier, int nRankChange = 0, int nCoutinuseOnBoard = 0, int nCoutinuseFirst = 0 ) :
	CPlayerRankMeta(unCharID, pszName, unValue, unCountry, unTitle, unMetier, nRankChange , nCoutinuseOnBoard , nCoutinuseFirst ) 
	{	}

	CPlayerRankTeamPoints()
	{	}
};


/** CRankModule 
**	排行榜系统
**  场景服务器把所有玩家排行数据汇总于此，
**  然后根据具体策略(1天/1周)来进行排行
**	a.玩家下线前汇报一次; 
**  b.新的1天到来，网关主动向各场景要一次
**
**  排行榜分2种：
**  a. 当前排行榜 - 日排行/周排行
**  b. 实时排行榜 - 为下次排行做准备，只有满足特定条件的记录才能进榜，其余数据忽略掉
**
**  排名都要存数据库 -
**  a.当前排行榜是存历史表，150条/天 * 365天 = 54750 条
**	即时是运营3年，数据量也很小。主键用天来表示。
**	b.实时排行就只保存150条记录，定时update,truncat掉以后insert进去。
**	
*/
class CRankModule : public CDataModule, public CSingleton< CRankModule >
{
public:

	CRankModule();
	~CRankModule();

	static int CountSize( ) { return sizeof( CRankModule ); }

	// 启动服务器
	virtual void OnLaunchServer( );

	// 退出服务器
	virtual void OnExitServer( );

	// 服务器Timer
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );

	// 客户端发送消息
	virtual void OnMessage( int nServerID, CMessage* pMessage );

	// 启动是否完成
	virtual bool IsLaunched();

	virtual void OnNewDayCome();

	virtual void OnNewWeekCome();

	// 处理场景上报的个人排行信息
	int OnMessagePersonalRankInfoReport(int nServerID, CMessage* pMessage );
	int OnMessageRankRemove( int nServerID, CMessage* pMsg );

	// 从数据库载入当前玩家排名
	int LoadCurrentPlayerRankFromDB( );

	// 从数据库载入实时等级排名
	int LoadInTimeLevelRankFromDB( );

	// 把实时等级排行写入数据库
	int SaveInTimePlayerRankToDB( );

	// 加载等级排行信息
	int OnSessionLoadIntimePlayerRankInfoFromDB( CMessage* tpMessage, int nParam1, int nParam2 );

	// 加载历史排行信息
	int OnSessionLoadOutdatedPlayerRankInfoFromDB( CMessage* tpMessage, int nParam1, int nParam2 );

	// 每日更新排行榜
	int DailyUpdate( );
	int OnDailyUpdate( );
	int HalfHourUpdate(); // 半小时刷新
	int OnHalfHourUpdate( );

	// 每周更新排行榜
	int WeeklyUpdate( );

	// 从榜中删除指定角色
	int RemovePlayerFromRank( int nCharID, int nRankType );

	// ----------------- 处理场景服务器消息 ------------------------
	// TODO: 场景发来的消息就是客户端请求的消息，只是原封不动转发过来了，
	// 只是为了简单起见，不用定义超多雷同的消息，不要搞混淆了。
	// -------------------------------------------------------------
	
	// 请求等级排行列表
	int OnRouteMessagePlayerRankReqeust( int nServerID, CMessage* pMessage );
	// 请求玩家所有的排行榜称号
	int OnMessageObtainTopTenRequest( int nServerID, CMessage* pMessage );
	int mLaunchFlag;
	time_t mLastDayTick; // 用于计算每周
private:

	enum 
	{
		MAX_LEVEL_RANK_NUM = 150, // 等级排行榜长度
		MAX_MONEY_RANK_NUM = 150, // 金钱排行榜长度
		MAX_KILL_RANK_NUM  = 150, // 杀敌排行榜长度
		MAX_HONOR_RANK_NUM  = 150, // 功勋排行榜长度
		MAX_DIE_RANK_NUM  = 150, // 死亡排行榜长度
		MAX_ENDTASK_RANK_NUM  = 150, // 完成任务排行榜长度
		MAX_ONLINE_RANK_NUM  = 150, // 在线时长排行榜长度
		MAX_PANTAO_RANK_NUM  = 150, // 蟠桃副本排行榜长度
		MAX_LONGGONG_RANK_NUM  = 150, // 龙宫副本排行榜长度
        MAX_LASTONE_RANK_NUM  =  150 , //大逃杀积分排行榜长度
		MAX_ORIGINRELIVETIMES_RANK_NUM  = 150,   // 原地复活次数(全服)
		MAX_KILLSAMENATIONNUM_RANK_NUM  = 150,   // 杀本阵营人数(全服)
		MAX_KILLOGRENUM_RANK_NUM  = 150,		 // 杀怪数(全服)
		MAX_UPFAILNUM_RANK_NUM  = 150,			 // 精炼失败次数(全服)
		MAX_DAYANTA_NUM = 150, // 大雁塔
		MAX_LIANHUADONG_NUM = 150, // 莲花洞
		MAX_HUANGFENGDONG_NUM = 150, // 黄蜂洞
		MAX_RANK_ACHIVE_NUM= 150, // 成就点
		MAX_RANK_FAVOR_NUM= 150, // 好感度
		MAX_RANK_BAD_NUM= 150, // 猥琐度
		MAX_RANK_CHARM_NUM= 150, // 魅力值排行榜
		MAX_FAMILY_HONOR_NUM = 30, // 家族荣誉
		MAX_CORPS_HONOR_NUM = 30, // 军团荣誉
		MAX_FLY_DUNG_NUM = 150, // 飞行副本积分
		MAX_COMMON_NUM	= 150,  // 通用总数
	};

	enum MONEY
	{
		FREE_MONEY, // 非绑定钱
		ALL_MONEY,  // 所有钱
	};

	enum KILL
	{
		KILL_PROT_NATION,
		KILL_OUT,			
		KILL_TOTAL,
	};

	typedef CRankBoard<MAX_LEVEL_RANK_NUM, CPlayerLevelRank, MAX_COUNTRY+1> RANK_LEVEL_TYPE;
	RANK_LEVEL_TYPE mLevelRankOutdated;	// 历史等级排行( 客户端显示用 )
	RANK_LEVEL_TYPE mLevelRankInTime;	// 实时等级排行( 计算用 )

	typedef CRankBoard<MAX_MONEY_RANK_NUM, CPlayerMoneyRank, 1> RANK_MONEY_TYPE;
	RANK_MONEY_TYPE mMoneyRankOutdated[2];	// 历史金钱排行( 客户端显示用 )
	RANK_MONEY_TYPE mMoneyRankInTime[2];	// 实时金钱排行( 计算用 )

	typedef CRankBoard<MAX_KILL_RANK_NUM, CPlayerKillRank, 1> RANK_KILL_TYPE;
	RANK_KILL_TYPE mKillRankOutdated[3];	// 历史杀敌排行( 客户端显示用 )
	RANK_KILL_TYPE mKillRankInTime[3];		// 实时杀敌排行( 计算用 )

	RANK_KILL_TYPE mDayKillRankOutdated[3];	// 当日历史杀敌排行( 客户端显示用 )
	RANK_KILL_TYPE mDayKillRankInTime[3];	// 当日实时杀敌排行( 计算用 )

	typedef CRankBoard<MAX_HONOR_RANK_NUM, CPlayerHonorRank, MAX_COUNTRY+1> RANK_HONOR_TYPE;
	RANK_HONOR_TYPE mHonorRankOutdated;		// 历史功勋排行( 客户端显示用 )
	RANK_HONOR_TYPE mHonorRankInTime;		// 实时功勋排行( 计算用 )

	RANK_HONOR_TYPE mDayHonorRankOutdated;	// 当日历史功勋排行( 客户端显示用 )
	RANK_HONOR_TYPE mDayHonorRankInTime;	// 当日实时功勋排行( 计算用 )

	typedef CRankBoard<MAX_DIE_RANK_NUM, CPlayerDieNumRank, 1> RANK_DIE_TYPE;
	RANK_DIE_TYPE mDieNumRankOutdated;	// 历史死亡排行( 客户端显示用 )
	RANK_DIE_TYPE mDieNumRankInTime;	// 实时死亡排行( 计算用 )

	typedef CRankBoard<MAX_ENDTASK_RANK_NUM, CPlayerEndTaskRank, 1> RANK_ENDTASK_TYPE;
	RANK_ENDTASK_TYPE mEndTaskRankOutdated;	// 历史完成任务排行( 客户端显示用 )
	RANK_ENDTASK_TYPE mEndTaskRankInTime;		// 实时完成任务排行( 计算用 )

	typedef CRankBoard<MAX_ONLINE_RANK_NUM, CPlayerOnLineRank, 1> RANK_ONLINE_TYPE;
	RANK_ONLINE_TYPE mOnLineRankOutdated;		// 历史在线时长排行( 客户端显示用 )
	RANK_ONLINE_TYPE mOnLineRankInTime;		// 实时在线时长排行( 计算用 )

	typedef CRankBoard<MAX_PANTAO_RANK_NUM, CPlayerPantaoRank, 1> RANK_PANTAO_TYPE;
	RANK_PANTAO_TYPE mPantaoRankOutdated;		// 历史排行( 客户端显示用 )
	RANK_PANTAO_TYPE mPantaoRankInTime;		// 实时排行( 计算用 )

	typedef CRankBoard<MAX_LONGGONG_RANK_NUM, CPlayerLonggongRank, 1> RANK_LONGGONG_TYPE;
	RANK_LONGGONG_TYPE mLonggongRankOutdated;	// 历史排行( 客户端显示用 )
	RANK_LONGGONG_TYPE mLonggongRankInTime;		// 实时排行( 计算用 )

	typedef CRankBoard<MAX_ORIGINRELIVETIMES_RANK_NUM, CPlayerOriginReliveTimes, 1> RANK_ORIGINRELIVETIMES_TYPE;
	RANK_ORIGINRELIVETIMES_TYPE mOriginReliveRankOutdated;	// 历史原地复活排行( 客户端显示用 )
	RANK_ORIGINRELIVETIMES_TYPE mOriginReliveRankInTime;		// 实时原地复活排行( 计算用 )

	typedef CRankBoard<MAX_KILLSAMENATIONNUM_RANK_NUM, CPlayerRankKillSameNationNum, 1> RANK_KILLSAMENATIONNUM_TYPE;
	RANK_KILLSAMENATIONNUM_TYPE mKillSameNationRankOutdated;	// 历史杀死本阵营排行( 客户端显示用 )
	RANK_KILLSAMENATIONNUM_TYPE mKillSameNationRankInTime;		// 实时杀死本阵营排行( 计算用 )

	typedef CRankBoard<MAX_KILLOGRENUM_RANK_NUM, CPlayerRankKillOgreNum, 1> RANK_KILLOGRENUM_TYPE;
	RANK_KILLOGRENUM_TYPE mKillOgreRankOutdated;	// 历史杀怪总数排行( 客户端显示用 )
	RANK_KILLOGRENUM_TYPE mKillOgreRankInTime;		// 实时杀怪总数排行( 计算用 )

	typedef CRankBoard<MAX_UPFAILNUM_RANK_NUM, CPlayerRankUpFailNum, 1> RANK_UPFAILNUM_TYPE;
	RANK_UPFAILNUM_TYPE mUpFailRankOutdated;	// 历史升级失败排行( 客户端显示用 )
	RANK_UPFAILNUM_TYPE mUpFailRankInTime;		// 实时升级失败排行( 计算用 )

	//12.16新加
	typedef CRankBoard<MAX_DAYANTA_NUM, CPlayerRankDayanta, 1> RANK_DAYANTA_TYPE;
	RANK_DAYANTA_TYPE mDayantaRankOutdated;	// 大雁塔历史排行( 客户端显示用 )
	RANK_DAYANTA_TYPE mDayantaRankInTime;	// 大雁塔实时排行( 计算用 )

    typedef CRankBoard<MAX_LASTONE_RANK_NUM, CPlayerRankLastOne, 1> RANKTYPE_LASTONE_TYPE;
    RANKTYPE_LASTONE_TYPE mDaLuanDouOutdated;  // 大乱斗历史排行( 客户端显示用 )
    RANKTYPE_LASTONE_TYPE mDaLuanDouInTime;    // 大乱斗实时排行( 计算用 )
	RANKTYPE_LASTONE_TYPE mDaLuanDouTotalOutdated;  // 大乱斗历史排行总积分( 客户端显示用 )
	RANKTYPE_LASTONE_TYPE mDaLuanDouTotalInTime;    // 大乱斗总积分实时排行( 计算用 )

	typedef CRankBoard<MAX_LIANHUADONG_NUM, CPlayerRankLianhuadong, 1> RANK_LIANHUADONG_TYPE;
	RANK_LIANHUADONG_TYPE mLianhuadongRankOutdated;	// 莲花洞历史排行( 客户端显示用 )
	RANK_LIANHUADONG_TYPE mLianhuadongRankInTime;	// 莲花洞实时排行( 计算用 )

	typedef CRankBoard<MAX_HUANGFENGDONG_NUM, CPlayerRankHuangfengdong, 1> RANK_HUANGFENGDONG_TYPE;
	RANK_HUANGFENGDONG_TYPE mHuangfengdongRankOutdated;	// 黄蜂洞历史排行( 客户端显示用 )
	RANK_HUANGFENGDONG_TYPE mHuangfengdongRankInTime;	// 黄蜂洞实时排行( 计算用 )

	typedef CRankBoard<MAX_RANK_ACHIVE_NUM, CPlayerRankAchive, 1> RANK_ACHIVE_TYPE;
	RANK_ACHIVE_TYPE mAchiveRankOutdated;	// 成就点历史排行( 客户端显示用 )
	RANK_ACHIVE_TYPE mAchiveRankInTime;		// 成就点实时排行( 计算用 )

	typedef CRankBoard<MAX_RANK_FAVOR_NUM, CPlayerRankFavor, 1> RANK_FAVOR_TYPE;
	RANK_FAVOR_TYPE mFavorRankOutdated;	// 好感度历史排行( 客户端显示用 )
	RANK_FAVOR_TYPE mFavorRankInTime;	// 好感度实时排行( 计算用 )

	typedef CRankBoard<MAX_RANK_BAD_NUM, CPlayerRankBadPoint, 1> RANK_BAD_TYPE;
	RANK_BAD_TYPE mBadRankOutdated;	// 猥琐度历史排行( 客户端显示用 )
	RANK_BAD_TYPE mBadRankInTime;	// 猥琐度实时排行( 计算用 )

	typedef CRankBoard<MAX_FAMILY_HONOR_NUM , CPlayerRankTeamPoints, 1> RANK_FAMILY_POINTS_TYPE;
	RANK_FAMILY_POINTS_TYPE mFamilyRankOutdated;	// 家族PVP历史排行( 客户端显示用 )
	RANK_FAMILY_POINTS_TYPE mFamilyRankInTime;		// 家族PVP实时排行( 计算用 )

	RANK_FAMILY_POINTS_TYPE mFamilyTaoRankOutdated;	// 家族品仙桃历史排行( 客户端显示用 )
	RANK_FAMILY_POINTS_TYPE mFamilyTaoRankInTime;	// 家族品仙桃实时排行( 计算用 )

	typedef CRankBoard<MAX_CORPS_HONOR_NUM , CPlayerRankTeamPoints, 1> RANK_CORPS_POINTS_TYPE;
	RANK_CORPS_POINTS_TYPE mCorpsRankOutdated;	// 军团PVP历史排行( 客户端显示用 )
	RANK_CORPS_POINTS_TYPE mCorpsRankInTime;	// 军团PVP实时排行( 计算用 )

	RANK_CORPS_POINTS_TYPE mCorpsBossScoreRankOutdated;		// 军团boss积分历史排行( 客户端显示用 )
	RANK_CORPS_POINTS_TYPE mCorpsBossScoreRankInTime;		// 军团boss积分实时排行( 计算用 )

	RANK_CORPS_POINTS_TYPE mCorpsCollectActiveRankOutdated;		// 军团采集活跃度历史排行( 客户端显示用 )
	RANK_CORPS_POINTS_TYPE mCorpsCollectActiveRankInTime;		// 军团采集活跃度实时排行( 计算用 )

	typedef CRankBoard<MAX_RANK_CHARM_NUM, CPlayerRankCharm, 1> RANK_CHARM_TYPE;
	RANK_CHARM_TYPE mCharmRankOutdated;	// 历史排行( 客户端显示用 )
	RANK_CHARM_TYPE mCharmRankInTime;	// 实时排行( 计算用 )

	typedef CRankBoard<MAX_FLY_DUNG_NUM, CPlayerRankFlyDung, 1> RANK_FLY_DUNG_TYPE;
	RANK_FLY_DUNG_TYPE mFlyDungRankOutdated;	// 飞行副本历史排行( 客户端显示用 )
	RANK_FLY_DUNG_TYPE mFlyDungRankInTime;		// 飞行副本实时排行( 计算用 )

	typedef CRankBoard<MAX_COMMON_NUM, CPlayerRankCommon, 1> RANK_COMMON_WORLDRANK_TYPE;
	RANK_COMMON_WORLDRANK_TYPE mZhanGongOutdated;	// 战功( 客户端显示用 )
	RANK_COMMON_WORLDRANK_TYPE mZhanGongInTime;		// 战功( 计算用 )

	RANK_COMMON_WORLDRANK_TYPE mZhiGuoOutDated;		// 治国( 客户端显示用 )
	RANK_COMMON_WORLDRANK_TYPE mZhiGuoInTime;		// 治国( 计算用 )

	RANK_COMMON_WORLDRANK_TYPE mTongShuaiOutdated;		// 统帅( 客户端显示用 )
	RANK_COMMON_WORLDRANK_TYPE mTongShuaiInTime;		// 统帅( 计算用 )

	RANK_COMMON_WORLDRANK_TYPE mWisAllOutdated;		// 才华总值( 客户端显示用 )
	RANK_COMMON_WORLDRANK_TYPE mWisAllInTime;		// 才华总值( 计算用 )

	RANK_COMMON_WORLDRANK_TYPE mYunxiaoOutdated;	// 云霄塔积分( 客户端显示用 )
	RANK_COMMON_WORLDRANK_TYPE mYunxiaoInTime;		// 云霄塔积分( 计算用 )

	typedef CRankBoard<MAX_COMMON_NUM, CPlayerRankCommon, MAX_COUNTRY+1> RANK_COMMON_ALLRANK_TYPE;
	RANK_COMMON_ALLRANK_TYPE mWeekCharmIncOutdated;		// 周累计魅力值排行( 客户端显示用 )
	RANK_COMMON_ALLRANK_TYPE mWeekCharmIncInTime;		// 周累计魅力值排行( 计算用 )

	RANK_COMMON_WORLDRANK_TYPE mShideOutdated;		// 师德( 客户端显示用 )
	RANK_COMMON_WORLDRANK_TYPE mShideInTime;		// 师德( 计算用 )

	RANK_COMMON_WORLDRANK_TYPE mXianYuanOutdated;	// 仙缘( 客户端显示用 )
	RANK_COMMON_WORLDRANK_TYPE mXianYuanInTime;		// 仙缘( 计算用 )

	RANK_COMMON_WORLDRANK_TYPE mChuShiOutdated;		// 出师( 客户端显示用 )
	RANK_COMMON_WORLDRANK_TYPE mChuShiInTime;		// 出师( 计算用 )

	RANK_COMMON_WORLDRANK_TYPE mYunLvOutdated;		// 云霄塔层数( 客户端显示用 )
	RANK_COMMON_WORLDRANK_TYPE mYunLvInTime;		// 云霄塔层数( 计算用 )

	RANK_COMMON_ALLRANK_TYPE   mLastWeekCharmInc;		// 上周累计魅力值排行( 客户端显示用 )

	RANK_COMMON_WORLDRANK_TYPE mWuDidongOutdated;		// 无底洞积分( 客户端显示用 )
	RANK_COMMON_WORLDRANK_TYPE mWuDidongInTime;			// 无底洞积分( 计算用 )

	RANK_COMMON_WORLDRANK_TYPE mGlobalWarOutdated;		// 跨服战积分( 客户端显示用 )
	RANK_COMMON_WORLDRANK_TYPE mGlobalWarInTime;		// 跨服战积分( 计算用 )

	RANK_COMMON_WORLDRANK_TYPE m_SpiritOutdated;		// 铸灵评分( 客户端显示用 )
	RANK_COMMON_WORLDRANK_TYPE m_SpiritInTime;			// 铸灵评分( 计算用 )


	//CWTimer	mRankUpdateTimer;		// 定时检测间隔(是否是新一天，新一周)
	CWTimer mInTimeRankSaveTimer;	// 定时保存实时表的间隔
	CWTimer mHalfHourTimer;			// 半小时timer

	typedef lk::hash_map<int, int, SCENE_AT_GATE_CAP> RECV_SCENE_LIST_TYPE;
	
	RECV_SCENE_LIST_TYPE mRecvSceneList[REFRESHTYPE_ALL];

	template< typename TYPE >
	int FillTopTenData( TYPE* vpMsg, int nCountryID )
	{
		if ( vpMsg == NULL )
		{
			return -1;
		}
		
		PBRankTopTen* tpTopTen = vpMsg->add_toptenlist();
		int tCharList[ TOPTEN_VALUE ] = { 0 };
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
			tCharList[i] = 0;

		// -- 人物等级排行榜
		// 本国
		mLevelRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), nCountryID );
		tpTopTen->set_ranktype( RANKTYPE_LEVEL_MYNATION );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		// 全服务器
		tpTopTen = vpMsg->add_toptenlist();
		mLevelRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_LEVEL_ALL );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		// -- 拥有财富排行榜
		// 非帮定金
		tpTopTen = vpMsg->add_toptenlist();
		mMoneyRankOutdated[FREE_MONEY].GetTopTen( tCharList, ARRAY_CNT( tCharList ), nCountryID );
		tpTopTen->set_ranktype( RANKTYPE_MONEY_NOBIND );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		// 总计
		tpTopTen = vpMsg->add_toptenlist();
		mMoneyRankOutdated[ALL_MONEY].GetTopTen( tCharList, ARRAY_CNT( tCharList ), nCountryID );
		tpTopTen->set_ranktype( RANKTYPE_MONEY_ALL );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		// -- 杀敌数排行榜
		// 本周护国
		tpTopTen = vpMsg->add_toptenlist();
		mKillRankOutdated[KILL_PROT_NATION].GetTopTen( tCharList, ARRAY_CNT( tCharList ), nCountryID );
		tpTopTen->set_ranktype( RANKTYPE_KILL_PROT_NATION );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		// 本周出征
		tpTopTen = vpMsg->add_toptenlist();
		mKillRankOutdated[KILL_OUT].GetTopTen( tCharList, ARRAY_CNT( tCharList ), nCountryID );
		tpTopTen->set_ranktype( RANKTYPE_KILL_OUT );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		// 本周总杀敌
		tpTopTen = vpMsg->add_toptenlist();
		mKillRankOutdated[KILL_TOTAL].GetTopTen( tCharList, ARRAY_CNT( tCharList ), nCountryID );
		tpTopTen->set_ranktype( RANKTYPE_KILL_TOTAL );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		// -- 功勋排行榜
		// 世界总功勋
		tpTopTen = vpMsg->add_toptenlist();
		mHonorRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_KILL_TOTAL );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		// 本国总功勋
		tpTopTen = vpMsg->add_toptenlist();
		mHonorRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), nCountryID );
		tpTopTen->set_ranktype( RANKTYPE_HONER_MYNATION );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		// -- 死亡次数排行榜
		tpTopTen = vpMsg->add_toptenlist();
		mDieNumRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_DIE );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		// -- 完成任务数排行榜
		tpTopTen = vpMsg->add_toptenlist();
		mEndTaskRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_END_TASK );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		// -- 在线时间排行榜
		tpTopTen = vpMsg->add_toptenlist();
		mOnLineRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_ONLINE_TIME );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		// -- 蟠桃副本
		tpTopTen = vpMsg->add_toptenlist();
		mPantaoRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_PANTAO_MARK );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		// -- 龙宫副本
		tpTopTen = vpMsg->add_toptenlist();
		mLonggongRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_LONGGONG_MARK );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		// 原地复活次数(全服)
		tpTopTen = vpMsg->add_toptenlist();
		mOriginReliveRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_ORIGINRELIVETIMES );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		// 杀本阵营人数(全服)
		tpTopTen = vpMsg->add_toptenlist();
		mKillSameNationRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_KILLSAMENATIONNUM );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		// 杀怪数(全服)
		tpTopTen = vpMsg->add_toptenlist();
		mKillOgreRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_KILLOGRENUM );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		// 精炼失败次数(全服)
		tpTopTen = vpMsg->add_toptenlist();
		mUpFailRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_UPFAILNUM );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}

		// 排行榜称号新增修改 [12/18/2009 Macro]

		// 大雁塔积分排行
		tpTopTen = vpMsg->add_toptenlist();
		mDayantaRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_DAYANTA );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}

		// 大乱斗积分排行
		tpTopTen = vpMsg->add_toptenlist();
		mDaLuanDouOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_LASTONE );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		
		// 大乱斗总积分排行榜
		tpTopTen = vpMsg->add_toptenlist();
		mDaLuanDouTotalOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_TOTAL_LASTONE );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}

		// 莲花洞积分排行
		tpTopTen = vpMsg->add_toptenlist();
		mLianhuadongRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_LIANHUADONG );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		
		// 黄蜂洞积分排行
		tpTopTen = vpMsg->add_toptenlist();
		mHuangfengdongRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_HUANGFENGDONG );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
		
		// 成就点排行
		tpTopTen = vpMsg->add_toptenlist();
		mAchiveRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_ACHIVE_POINT );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}
	
		// 好感度排行
		tpTopTen = vpMsg->add_toptenlist();
		mFavorRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_FAVOR_POINT );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}

		// 萎缩度排行
		tpTopTen = vpMsg->add_toptenlist();
		mBadRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_BAD_POINT );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}

		// 家族pvp积分排行
		tpTopTen = vpMsg->add_toptenlist();
		mFamilyRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_FAMILY_PVP );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}

		// 家族品仙桃排行
		tpTopTen = vpMsg->add_toptenlist();
		mFamilyTaoRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_FAMILY_TAO );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}

		// 军团pvp积分排行
		tpTopTen = vpMsg->add_toptenlist();
		mCorpsRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_CORPS_PVP );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}

		// 军团boss积分排行
		FILL_TOP_TEN( RANKTYPE_CORPSBOSS_SCORE,mCorpsBossScoreRankOutdated );

		// 军团采集活跃度排行
		FILL_TOP_TEN( RANKTYPE_CORPSCOLLECT_ACTIVE,mCorpsCollectActiveRankOutdated );


		// 今日杀敌数排行(护卫)
		tpTopTen = vpMsg->add_toptenlist();
		mDayKillRankOutdated[ KILL_PROT_NATION ].GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_KILL_PROT_NATION_TODAY );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}

		// 今日杀敌数排行(出征)
		tpTopTen = vpMsg->add_toptenlist();
		mDayKillRankOutdated[ KILL_OUT ].GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_KILL_OUT_TODAY );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}

		// 今日本周总杀敌排行
		tpTopTen = vpMsg->add_toptenlist();
		mDayKillRankOutdated[ KILL_TOTAL ].GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_KILL_TOTAL_TODAY );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}

		// 奇闻异事 魅力值 [2/3/2010 Macro]
		tpTopTen = vpMsg->add_toptenlist();
		mCharmRankOutdated.GetTopTen( tCharList, ARRAY_CNT( tCharList ), 0 );
		tpTopTen->set_ranktype( RANKTYPE_CHARM_VALUE );
		for( unsigned int i = 0; i < ARRAY_CNT( tCharList ); i ++ )
		{
			tpTopTen->add_charlist( tCharList[i] );
			tCharList[ i ] = 0; 
		}

		FILL_TOP_TEN( RANKTYPE_ZHANGONG,mZhanGongOutdated )
		FILL_TOP_TEN( RANKTYPE_ZHIGUO,mZhiGuoOutDated )
		FILL_TOP_TEN( RANKTYPE_TONGSHUAI,mTongShuaiOutdated )
		FILL_TOP_TEN( RANKTYPE_WIS,mWisAllOutdated )
		FILL_TOP_TEN( RANKTYPE_YUNXIAO,mYunxiaoOutdated )
		FILL_TOP_TEN( RANKTYPE_WUDIDONG,mWuDidongOutdated )
		FILL_TOP_TEN( RANKTYPE_GLOBALWAR,mGlobalWarOutdated )
		FILL_TOP_TEN( RANKTYPE_SPIRIT,m_SpiritOutdated )

		return 0;
	}
};
