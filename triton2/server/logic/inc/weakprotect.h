//=========================================================================
// weakprotect.h
//=========================================================================
#pragma once


#include "countrymessage_pb.hxx.pb.h"

class CEntityPlayer;
class CMessage;

#define CAMP_INDEX(x)			((x)-1)
#define MAX_CAMP_NUM	4	//阵营数量
class CWeakProtect
{
public:
	CWeakProtect();
	~CWeakProtect();

	void Init();

private:
	PBCampWeakForceInfo m_PBWeakInfo[MAX_CAMP_NUM];
	int m_nCampWeakLevel[MAX_CAMP_NUM];

public:
	// client request week list of camp
	void OnMessageCampWeakListReq( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// client get award of camp
	void OnMessageCampGetAward( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// syn camp list from gate
	void OnMessageGSCampWeakList( CMessage* pMessage );
	
	// 通知网关荣誉积分变化
	void Send2GatePlayerGetHonerScoreNotify( int nCampID, int nRoleID, int nScore );
	
	// 请求网关弱国数据
	void Send2GateCampWeakListRequest();

public:
	// 判断玩家是否能够获取弱势阵营任务
	bool CouldObtainTask( CEntityPlayer* pPlayer );
	
	// 判断阵营是否弱势
	bool IsWeakCamp( int nCampID );

	// 得到阵营等级
	int  GetWeekLevel( int nCampID );
	//判断 是否是积分最高的阵营
	bool IsTopTotalscoreCampID(int nCampID);

private:
	// 领取奖励
	int GetWeakCampAward(CEntityPlayer* pPlayer, int nNpcEntityID, int nAwardType);
};