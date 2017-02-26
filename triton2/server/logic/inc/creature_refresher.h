#pragma once

#include "sceneobj_manager.h"
#include "sceneobj_manager_imp.h"
#include "entity.h"
#include "mapmodule.h"

class CEntity;
class CCreator;

// 最大延迟刷新数
#define MAX_DELAY_REFRESH_CREATURE_NUM	1000


// 延迟刷新
struct CDelayRefreshedCreature
{	
	CDelayRefreshedCreature(	unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, unsigned int unCreatorID, unsigned int vNpcID = 0 )
		: mLineID( vLineID ), mMapID( vMapID ), mMapIndex( vIndex ), mCreatorID( unCreatorID ), mNpcID( vNpcID )
	{}
	
	CDelayRefreshedCreature() 
	{
		if( CObj::msCreateMode )
		{
			mLineID = 0;
			mMapID = 0;
			mMapIndex = 0;
			mCreatorID = 0;
			mNpcID = 0;
		}
	}
	~CDelayRefreshedCreature() {}

	unsigned short	mLineID;		// 线ID
	unsigned short	mMapID;			// 地图ID
	unsigned short	mMapIndex;		// 地图索引
	unsigned int	mCreatorID;		// 创建器ID
	unsigned int	mNpcID;			// NpcID，可能会在外部指定，默认0
};


class CCreatureRefresher : public CSingleton< CCreatureRefresher >
{
public:
	CCreatureRefresher();
	~CCreatureRefresher();

	void Initailze();
	void Resume();

	/*
	1、定时刷新
	怪物在真实时间刷新。
	每天的特定时间（比如每天的12：00）
	每周的特定时间（比如每周的周6，12：00）
	每月的特定时间（比如5月的第1天的12：00）
	特定月的特定周的特定时间（比如五月的第2周的周5的20：00）
	以上4种均要求可同时配置多个时间（比如每天的8：00，12：00，16：00，20：00）
	以上的4这种刷新方式均针对怪物本身。
	5、计时触发的情况是如果在计时点该怪存在，则跳过此次刷新的时间。
	*/
	void OnTimeRefreshEvent( int vIndex = 0 );

	/*
	2、计数触发
	在一定区域内刷新N个怪以后刷新特定怪。（要求可配置数量与多个特定的怪）
	计数到了后N秒刷新特定怪（N秒由策划配置）
	该刷新规则针对一个区域。
	区域就一种怪：刷新N个A怪后刷新特定的B怪，然后A怪继续刷新，不因为B怪的出现而改变A怪的刷新策略。在B怪死亡以后重新开始计算区域中A怪刷新的数量。
	区域有N种怪集合：当区域所有怪刷新N个后刷新特定的B怪。B怪刷新后不因为B怪的出现而改变区域其他怪的刷新策略。在B怪死亡以后重新开始计算区域中其他怪的数量。
	要求可以刷新逻辑串联（1000个以后刷A，3000个以后刷B，A和B的刷新不相互干扰）。
	计数触发的同一区域必须符合相同的刷新规则，否则不作为同一个计数值存在。
	*/
	void OnNumericalRefreshEvent( CEntityNpc* pNpc );


	/*
	3、概率触发
	在一定区域内刷新A怪的同时有一定的概率刷新特定的B怪。
	在该区域内每刷新一个A怪则有一定几率刷新B怪。
	B怪是和A怪一起刷新，并不是覆盖A怪。
	该区域其他的怪并不能概率触发B怪的刷新（除非是其他怪也配置了B怪的刷新概率）
	B怪的刷新并不影响该区域内其他任何刷新规则。
	要求一个怪可概率触发多个怪的刷新。
	*/
	void OnPossibilityRefreshEvent( COgreCreator *pCreator, int vIndex );
	
	/*
	4、存活时间刷新
	怪物A存活一段时间没死后刷新另外一种怪物B。
	怪物A是可能移动的怪。
	怪物B刷新的时候在怪物A当前的点上刷新。
	怪物B刷新的时候怪物A消失。
	怪物A按其刷新逻辑刷新后开始计算存活时间
	存活时间由策划配置。
	要求可以配置多个串联（A存活一段时间刷B，B存活一段时候刷C）。
	*/
	void OnLifetimeRefreshEvent( CEntityNpc* pNpc, int index );


	/*
	8、死亡后一点多怪式刷新
	一个刷新点配置N个怪的刷新，每个怪都带有一个刷新概率，N个怪的刷新概率合为1，
	刷新某种怪后不作其他怪的刷新了。（50%刷A，50%刷B，B刷新后不刷A）。
	*/
	int OnDeathRefreshEvent( COgreCreator* pCreator );


	/*
	7、多地图多点刷新
	多地图的多点刷新，作为同一个刷新点处理。
	10个点都有一定几率刷新A怪，但是A怪在第1个点刷新后，后面9个点就不刷新了。
	各点的刷新几率由策划配置，概率合为1。
	*/
	COgreCreator* OnMutiMapRefreshEvent( COgreCreator* pCreator );

public:

	// 加入到延迟刷新列表
	bool AddToDelayRefreshList( const CDelayRefreshedCreature& pCreature );

	// 从延迟刷新列表删除
	bool RemoveFromDelayRefreshList( CDelayRefreshedCreature& pCreature );

	// 清除延迟刷新列表
	void ClearDelayRefreshList( ); 

	// 执行延迟刷新
	int DoDelayRefresh( );

	// 定时器
	void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );

private:

	typedef lk::vector< CDelayRefreshedCreature, MAX_DELAY_REFRESH_CREATURE_NUM > TDelayRefreshList;

	
	TDelayRefreshList	mDelayRefreshList;	// 延迟刷新列表
	CWTimer				mDelayRefreshTimer;	// 延迟刷新timer

};
