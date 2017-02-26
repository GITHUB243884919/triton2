//-------------------------------------------------------------------------
// weakprotecter.h
//-------------------------------------------------------------------------
#pragma  once


#define UI_LevelScore(x)	((x) / 10)
#define UI_HonerScore(x)	((x) / 1000)
#define GET_TotalScore(level, honer)	(UI_LevelScore(level) + UI_HonerScore(honer))
class CMessage;

class CWeakProtecter
{

public:
	CWeakProtecter();
	~CWeakProtecter();
public:
	enum EWP_STATUS
	{
		EWP_Normal = 0,
		EWP_Waiting,
		EWP_Max
	};
	// 请求弱势阵营列表
	void OnMessageCampWeakListReq(int nServerID, CMessage* pMessage);
	
	// 添加荣誉值
	void OnMessageCampAddHonerScore(int nServerID, CMessage* pMessage);
	
	// 发送阵营列表
	void SendCampWeakList(int nServerID);

	// 调用存储过程统计活跃用户积分
	void StartGetActiveRoleInfo();
	
	// 数据库返回存储过程统计结果
	void OnSessionGetActiveRoleInfo(CMessage* pMessage);
public:
	// 计算并更新弱势力阵营
	void CalculateCampWeakRank();
	
	// 
	void RefreshWeakInfo();

public:

	int GetWeekLevelByWordID(int nWorldID);

	void OnTimerCheckState();
private:
	int AddHonerScore(int nCampID, int nScore, int nRoleID);
	void JudgeWeakLevel(int nSumTotal, int* nLevelScore, int *nTotalScore, bool IsChange);

	int m_State;
};
