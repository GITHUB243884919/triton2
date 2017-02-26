#ifndef _ACTIVITY_SERVICE_H_
#define _ACTIVITY_SERVICE_H_

#include "module_manager.h"
#include "entity.h"
#include "lk_hashmap.h"
#include "lk_string.h"
#include "gateobj_define.h"
#include "lk_set.h"
#include "SysProtocol.h"
#include "property_pb.hxx.pb.h"
#include "gmmessage_pb.hxx.pb.h"
#include "activitytype.h"
#include "servermessage_pb.hxx.pb.h"
#include "protype.h"
#define ACTIVITY_CAP 255
using namespace GateServer;

// 场景活动控制对象
class CSingleSceneActivityObj
{
public:
	int m_nTime;	// 触发时间
	int m_nType;	// 时间类型 见 enActivityType枚举
	int m_nID;		// 活动ID
	int m_nLmtLevel;	// 等级限制
	int m_nMaxTimes;	// 每天最大次数

	CSingleSceneActivityObj()
	{
		if ( CObj::msCreateMode )		
		{
			m_nTime = 0;
			m_nType = 0;
			m_nID   = 0;
			m_nLmtLevel = 0;
			m_nMaxTimes = 0;
		}
	}
};

// 每个活动相关的静态数据
class CSingleActivityInfo
{
public:
	struct DropInfo
	{
		int mOgreID;
		int mDropID;
		DropInfo()
		{
			mOgreID = 0;
			mDropID = 0;			
		}
	};
	struct MapDropInfo
	{
		int mDropID;
		int mDropType;
		char mMaps[DROP_MAPS_STR_LEN];
		MapDropInfo()
		{
			mDropID=0;
			mDropType=0;
			memset (mMaps, 0, sizeof(mMaps));
		}
	};
	lk::vector<DropInfo, ACTIVITY_DROP_SZ> mDropInfo;
	lk::vector<MapDropInfo, ACTIVITY_MAPDROP_SIZE> mMapDropInfo;
	lk::vector<int, ACTIVITY_OGREINDEX_SZ> mOgreIndex;
	CSingleActivityInfo()
	{
		if (CObj::msCreateMode)
		{
			mDropInfo.initailize();
			mOgreIndex.initailize();
			mMapDropInfo.initailize();
		}
	}
};
// 按时间排序
bool operator < ( const CSingleSceneActivityObj& rLeft, const CSingleSceneActivityObj& rRight );

class CServiceActivity :public CDataModule, public CSingleton< CServiceActivity >
{
public:
	void OnLaunchServer();
	void OnExitServer();
	void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
	void OnMessage(int nServerID, CMessage* pMessage );
	void ReloadCfg();
	void RebuildItemIndex();

	CServiceActivity();
	~CServiceActivity();

	int Initialize();
	int Resume();
	static unsigned int CountSize();

	void* operator new( size_t size );
	void  operator delete( void* pointer );

	virtual bool IsLaunched() { return true; }

	// 收到erating对玩家领取物品nItemID的回应
	void OnAGIPSessionUserItemMinus( SysProtocol* pSysProto, 
			int nRoleID, int nRoleLevel, int nRoleMetier, int nItemNum, const char* szItemCode);

	// 收到erating对玩家查看物品的请求
	void OnAGIPSessionUserItemList( SysProtocol* pSysProto, int nParam1, int nParam2 , int nParam3, int nParam4);

	// 收到数据库返回的活动物品列表
	// void OnSessionLoadActivityItemFromDB(CMessage* tpMessage, int nStart, int ONCECOUNT);
private:
	//应游戏不同设计xml 管理gateserver活动配置 add by  qiufufu 
	void LoadActivityManagerFromXml(const char* xmlfile);

	// 从给定的xmlfile中装载未过期的活动列表
	void LoadActivityFromXml(const char* xmlfile);

	// 响应游戏服务器过来的活动列表请求消息
	void OnMessageActivityListRequest(int nServerID, CMessage* tpMessage);

	// 收到玩家查看物品的请求
	void OnMessageUserItemListRequest( int vServerID, CMessage* tpMessage );

	// 收到玩家领取物品的请求
	void OnMessageUserItemMinusRequest( int vServerID, CMessage* tpMessage );

	// load activity items from gamedb.gms_activity_item
	// void LoadActivityItemFromDB(int nStart, int nCount);

	// Fill PBActivityItem Proto
	bool FillPBActivityItem(PBActivityItem* pActItem, const char* pszItemCode, int nItemNum, int nRoleLevel, int nRoleMetier);

	// Fill PBExItem Proto
	// void FillPBExItem(PBExItem* pproto, CActivityItem* pActivityItem);

	// typedef std::multimap<lk::CString32, int>  ACTIVITYITEMSTYPE;
	// ACTIVITYITEMSTYPE* m_pActivityItems;

	typedef lk::hash_map<int, CActivity, ACTIVITY_CAP> ACTIVITYTYPE;
	ACTIVITYTYPE m_Activities;

	typedef lk::hash_map<lk::CString32, int, 1024>  ACTIVITYITEMSTYPE;
	ACTIVITYITEMSTYPE* m_pActivityItems;

	typedef lk::hash_map<int, lk::CString32,ACTIVITY_CAP> ACTIVITYMANAGER; //add by qiufufu 

	ACTIVITYMANAGER m_pActivityManager;
	//************************************************************************
	// 以下：场景服务器相关活动的控制
	//************************************************************************
	typedef lk::vector<CSingleSceneActivityObj,ACTIVITY_SIZE*3> SCENE_ACTIVITYTYPE;
	SCENE_ACTIVITYTYPE	m_ActivityActions; // 活动时间控制器

	typedef lk::hash_map<int, int, ACTIVITY_SIZE> ACTIVITY_STATE_TYPE;
	ACTIVITY_STATE_TYPE	m_ActivityStates;  // 活动状态控制器

	typedef lk::hash_map<int, CSingleActivityInfo, ACTIVITY_SIZE> ACTIVITY_INFO_TYPE;
	ACTIVITY_INFO_TYPE	m_ActivityInfo;  // 活动综合信息管理器

	int m_nNextActivityIndex;	// 下一个将要进行的活动的索引 开始是0

	CWTimer mCheckActivityTimer; // 检查间隔
	
	// 从场景活动配置文件中加载当天的未进行的活动
	void LoadSceneAvtivitys( const char* pFileName );

	// 排序 整理数组
	void SortActivitys();

	// 判断时间是否过时了 过时了返回true 时间格式是HHMM
	bool IsPassed( int nTime );

	// 检查活动，如果开始了就通知场景服务器
	void CheckActivitys(bool bStart=false);

	// 判断是不是今天
	bool IsToday( int nDate );

	// 获得活动状态
	int GetCurState( int nID );

	// 活动状态改变的处理
	void OnActivityStateChanged( int nActivityID, int nTymeType, int nTime, SingleActivity* pMsg );
	void OnFillSceneActivityChgMsg(int nActivityID, int nNewState, SingleActivity* pMsg);

public:
	// 通知场景服务器 当天活动列表
	// 在场景服务器注册完毕后给他发 参数：是否服务器刚启动
	void SendTodayList2Scene( int nServerID, bool bStart = false );

	// 新一天到来
	virtual void OnNewDayCome();

	// 新一周到来
	virtual void OnNewWeekCome() ;
	void SendActivityEvent2Scene( int nSvrID, CMessageActivityEventNotify* pMsg );

	// 加载配置的回调函数
	static void LoadActivityXmlCallback(const char* pcConfigPath);
	static void LoadInductXmlCallback(const char* pcConfigPath);

};

// string fmt must be is YYYY-MM-DD HH24:MI:SS
time_t GetTimeSecond(const char* szTime);
#endif

