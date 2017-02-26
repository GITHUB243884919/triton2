#pragma once
#include "sceneobj_define.h"
#include "servertool.h"
#include "servercore.h"
#include "tinyxml.h"
#include "tinystr.h"
#include "base_define.h"
#include "lk_hashmap.h"
#define	MAX_MODULE_NUM	15	// 最大模块数
#define	MAX_DIR_LEN	255	// 路径名最大长度
#define	VAR_NAME_LEN	64	// 变量名长度

struct DEFAULT_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct SCENE_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct PROPERTY_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct TASK_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct TEAM_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct MAIL_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct FAMILY_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct CORPS_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct CHAT_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct TRADE_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct FRIEND_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct MAP_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct STALL_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct STAT_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct NATION_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct TASKLUA_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct ACTIVITY_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct GM_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct IBTRANS_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct ACHIEVE_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct RANK_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct MARRIAGE_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct REPE_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct BOUR_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct YQSACTIVITY_LOG
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;

};

struct PROPERTY_CFG
{
	unsigned int	mExp_award;
	unsigned int	mExp_task;
	unsigned int	mRefresh_week;
	unsigned int	mRefresh_hour;
	unsigned int	mRefresh_minute;
	unsigned int	mYuan_id;
	unsigned int	mJusticelevellimit;
	unsigned int	mTalentexp;
	unsigned int	mCreateunion_id;
	unsigned int	mResetsp_index;
	unsigned int	mResetpp_index;
	unsigned int	mResetsp_num;
	unsigned int	mResetpp_num;
	unsigned int	mVip_normal;
	unsigned int	mVip_middle;
	unsigned int	mVip_senior;
	unsigned int	mFcm_buff;
	unsigned int	mFlower_lmt_type;
	unsigned int	mFlower_lmt_perday;
	unsigned int	mNewuserpkgid;

};

struct TASK_CFG
{
	unsigned int	mIscanabort;
	unsigned int	mLooptasksubnum;
	unsigned int	mRefreshstoragetime;
	unsigned int	mSearchmaindownlevel;
	unsigned int	mSearchmainuplevel;
	unsigned int	mSearchzhidownlevel;
	unsigned int	mSearchzhiuplevel;
	unsigned int	mSearchxydownlevel;
	unsigned int	mSearchxyuplevel;

};

struct TEAM_CFG
{
	unsigned int	mValiddistance;
	unsigned int	mSheetpage;
	unsigned int	mUpdatetime;
	unsigned int	mCooltime;

};

struct FAMILY_CFG
{
	unsigned int	mExpperglory;
	unsigned int	mExploitperglory;
	unsigned int	mMinstabilitytime;

};

struct MW_CFG
{
	unsigned int	mLevelextraeff;

};

struct FRIEND_CFG
{
	unsigned int	mMaxoncefriends;
	unsigned int	mMaxrefreshtime;

};

struct CHAT_CFG
{
	unsigned int	mChatlimittime;
	unsigned int	mMsglimittime;
	unsigned int	mWorldlevellimit;
	unsigned int	mWorldaplimit;
	unsigned int	mMaxautochattime;
	unsigned int	mCommonautochattime;
	unsigned int	mWorldautochattime;
	unsigned int	mBugletime;
	unsigned int	mCountrytime;
	unsigned int	mFamilytime;
	unsigned int	mCorpstime;
	unsigned int	mBuglelevellimit;
	unsigned int	mCampviptime;

};

struct STALL_CFG
{
	unsigned int	mLevellimit;
	unsigned int	mSaftytime;
	unsigned int	mMaxnamelen;
	unsigned int	mMaxadcontent;
	unsigned int	mMaxprice;
	unsigned int	mFeerate;
	unsigned int	mFeeibid;
	unsigned int	mStallflag;
	unsigned int	mMinybprice;

};

struct TRADE_CFG
{
	unsigned int	mSaftytime;

};

struct NPC_CFG
{
	unsigned int	mPetattackinstance;

};

struct CORPS_CFG
{
	unsigned int	mBidbeginweek;
	unsigned int	mBidendweek;
	unsigned int	mBidbeginhour;
	unsigned int	mBidbeginmin;
	unsigned int	mBidendhour;
	unsigned int	mBidendmin;
	unsigned int	mBattlebeginweek;
	unsigned int	mBattlebeginhour;
	unsigned int	mMinbidmoney;
	unsigned int	mMendhpvalue;
	unsigned int	mBattlewaitingtime;
	unsigned int	mExpperglory;
	unsigned int	mExploitperglory;
	unsigned int	mEnterbattlebuffid;

};

struct SCENELOGIC_CFG
{
	unsigned int	mKicknonactivityplayertime;
	unsigned int	mPlayersavedbtime;
	unsigned int	mPlayerredirecttimeout;
	unsigned int	mMovecheckdistance;
	unsigned int	mMovechecktimespace;
	unsigned int	mValidatedataintegrity;
	unsigned int	mMsgcheckcountlimit;
	unsigned int	mMsglimitpersec;
	unsigned int	mTogateheartbeatsec;
	unsigned int	mToproxyheartbeatsec;
	unsigned int	mTologstatinfosec;
	unsigned int	mPingcheck;

};

struct FORMULA_CFG
{
	unsigned int	mHitparam1;
	unsigned int	mHitparam2;
	unsigned int	mHitparam3;
	unsigned int	mHitparam4;
	unsigned int	mHitparam5;
	unsigned int	mHitparam6;
	unsigned int	mHitparam7;
	unsigned int	mDuckparam1;
	unsigned int	mDuckparam2;
	unsigned int	mDuckparam3;
	unsigned int	mDuckparam4;
	unsigned int	mDuckparam5;
	unsigned int	mDuckparam6;
	unsigned int	mDeathparam1;
	unsigned int	mDeathparam2;
	unsigned int	mDeathparam3;
	unsigned int	mDeathparam4;
	unsigned int	mDeathparam5;
	unsigned int	mDeathparam6;
	unsigned int	mResparam1;
	unsigned int	mResparam2;
	unsigned int	mResparam3;
	unsigned int	mResparam4;
	unsigned int	mResparam5;
	unsigned int	mHotparam1;
	unsigned int	mHotparam2;
	unsigned int	mCureparam1;
	unsigned int	mCureparam2;
	unsigned int	mDotparam1;
	unsigned int	mDotparam2;
	unsigned int	mDamageparam1;
	unsigned int	mDamageparam2;
	unsigned int	mShieldparam1;
	unsigned int	mShieldparam2;
	unsigned int	mDeathchangeparam;

};

struct RANK_CFG
{
	unsigned int	mPantaomap1;
	unsigned int	mPantaomap2;
	unsigned int	mPantaomap3;
	unsigned int	mLonggongmap1;
	unsigned int	mLonggongmap2;
	unsigned int	mLonggongmap3;

};

struct MAP_CFG
{
	unsigned int	mMovecheck;

};

struct REPETION_CFG
{
	unsigned int	mNumcheck;
	unsigned int	mTimecheck;
	unsigned int	mPlayercheck;
	unsigned int	mTimehighlimit;
	unsigned int	mTimelowlimit;
	unsigned int	mWrongtimes;
	unsigned int	mQuestionanswertime;
	unsigned int	mRepetionverifytime;
	unsigned int	mRepetionkickouttime;
	unsigned int	mCampkickrate;
	unsigned int	mVotetime;

};

struct BOURSE_CFG
{
	unsigned int	mYbtax;
	unsigned int	mMoneytax;
	unsigned int	mTaxid;
	unsigned int	mUse;
	unsigned int	mBuynotify;
	unsigned int	mSellnotify;

};

struct LOGEVENT_CFG
{
	unsigned int	mLog_role_task;
	unsigned int	mLog_role_item;
	unsigned int	mLog_role_logout;
	unsigned int	mLog_role_exp;
	unsigned int	mLog_mw_exp;
	unsigned int	mLog_talent_exp;
	unsigned int	mLog_produce_exp;
    unsigned int	mLog_role_buff;
	unsigned int	mLog_life_exp;
	unsigned int	mLog_role_money;
	unsigned int	mLog_role_levelup;
	unsigned int	mLog_role_killed;
	unsigned int	mLog_npc_drop;
	unsigned int	mLog_item_dispear;
	unsigned int	mLog_item_levelup;
	unsigned int	mRole_exp_limit;
	unsigned int	mTalent_exp_limit;
	unsigned int	mMw_exp_limit;
	unsigned int	mProduce_exp_limit;

};

struct APEX
{
	char	mIdentifier[VAR_NAME_LEN];
	char	mDirectory[VAR_NAME_LEN];
	unsigned int	mPriority;
	unsigned int	mMaxfilesize;
	unsigned int	mMaxbackupindex;
	bool	mAppend;
	unsigned int	mMulti_client;
	unsigned int	mVm;
	unsigned int	mMulti_user;

};

struct SPEEDCHECKER_CFG
{
	bool	mUsespeedcheckerforcstimediff;
	bool	mUsespeedcheckerforclientspeed;
	unsigned int	mTimesyncountforcstimediff;
	unsigned int	mTimesynintervalforcstimediff;
	unsigned int	mMaxtimeerrorforcstimediff;
	unsigned int	mMintimeerrorforcstimediff;
	unsigned int	mTimeerrorforcstimediff;
	unsigned int	mMaxabidecountsforcstimediff;
	unsigned int	mTimesyncountforclientspeed;
	unsigned int	mMaxtimeerrorforclientspeed;
	unsigned int	mMaxabidecountsforclientspeed;
	unsigned int	mMaxerrorcountforclientspeed;

};

struct FUNCCONSUME_CFG
{
	bool	mIsopen;

};

class CSceneCfgManager : public CSingleton< CSceneCfgManager >
{
private:
	ACHIEVE_LOG	mAchieve_log;
	ACTIVITY_LOG	mActivity_log;
	APEX	mApex;
	BOURSE_CFG	mBourse_cfg;
	BOUR_LOG	mBour_log;
	CHAT_CFG	mChat_cfg;
	CHAT_LOG	mChat_log;
	CORPS_CFG	mCorps_cfg;
	CORPS_LOG	mCorps_log;
	DEFAULT_LOG	mDefault_log;
	FAMILY_CFG	mFamily_cfg;
	FAMILY_LOG	mFamily_log;
	FORMULA_CFG	mFormula_cfg;
	FRIEND_CFG	mFriend_cfg;
	FRIEND_LOG	mFriend_log;
	FUNCCONSUME_CFG	mFuncconsume_cfg;
	GM_LOG	mGm_log;
	IBTRANS_LOG	mIbtrans_log;
	LOGEVENT_CFG	mLogevent_cfg;
	MAIL_LOG	mMail_log;
	MAP_CFG	mMap_cfg;
	MAP_LOG	mMap_log;
	MARRIAGE_LOG	mMarriage_log;
	MW_CFG	mMw_cfg;
	NATION_LOG	mNation_log;
	NPC_CFG	mNpc_cfg;
	PROPERTY_CFG	mProperty_cfg;
	PROPERTY_LOG	mProperty_log;
	RANK_CFG	mRank_cfg;
	RANK_LOG	mRank_log;
	REPETION_CFG	mRepetion_cfg;
	REPE_LOG	mRepe_log;
	SCENELOGIC_CFG	mScenelogic_cfg;
	SCENE_LOG	mScene_log;
	SPEEDCHECKER_CFG	mSpeedchecker_cfg;
	STALL_CFG	mStall_cfg;
	STALL_LOG	mStall_log;
	STAT_LOG	mStat_log;
	TASKLUA_LOG	mTasklua_log;
	TASK_CFG	mTask_cfg;
	TASK_LOG	mTask_log;
	TEAM_CFG	mTeam_cfg;
	TEAM_LOG	mTeam_log;
	TRADE_CFG	mTrade_cfg;
	TRADE_LOG	mTrade_log;
	YQSACTIVITY_LOG	mYqsactivity_log;
public:
	CSceneCfgManager();
	~CSceneCfgManager();

	static CSharedMem* mShmPtr;
	static unsigned int CountSize();

	int Initialize( char* pDir );

	ACHIEVE_LOG* GetAchieve_log();
	void SetAchieve_log(char* pStruct, TiXmlElement* pElement);

	ACTIVITY_LOG* GetActivity_log();
	void SetActivity_log(char* pStruct, TiXmlElement* pElement);

	APEX* GetApex();
	void SetApex(char* pStruct, TiXmlElement* pElement);

	BOURSE_CFG* GetBourse_cfg();
	void SetBourse_cfg(char* pStruct, TiXmlElement* pElement);

	BOUR_LOG* GetBour_log();
	void SetBour_log(char* pStruct, TiXmlElement* pElement);

	CHAT_CFG* GetChat_cfg();
	void SetChat_cfg(char* pStruct, TiXmlElement* pElement);

	CHAT_LOG* GetChat_log();
	void SetChat_log(char* pStruct, TiXmlElement* pElement);

	CORPS_CFG* GetCorps_cfg();
	void SetCorps_cfg(char* pStruct, TiXmlElement* pElement);

	CORPS_LOG* GetCorps_log();
	void SetCorps_log(char* pStruct, TiXmlElement* pElement);

	DEFAULT_LOG* GetDefault_log();
	void SetDefault_log(char* pStruct, TiXmlElement* pElement);

	FAMILY_CFG* GetFamily_cfg();
	void SetFamily_cfg(char* pStruct, TiXmlElement* pElement);

	FAMILY_LOG* GetFamily_log();
	void SetFamily_log(char* pStruct, TiXmlElement* pElement);

	FORMULA_CFG* GetFormula_cfg();
	void SetFormula_cfg(char* pStruct, TiXmlElement* pElement);

	FRIEND_CFG* GetFriend_cfg();
	void SetFriend_cfg(char* pStruct, TiXmlElement* pElement);

	FRIEND_LOG* GetFriend_log();
	void SetFriend_log(char* pStruct, TiXmlElement* pElement);

	FUNCCONSUME_CFG* GetFuncconsume_cfg();
	void SetFuncconsume_cfg(char* pStruct, TiXmlElement* pElement);

	GM_LOG* GetGm_log();
	void SetGm_log(char* pStruct, TiXmlElement* pElement);

	IBTRANS_LOG* GetIbtrans_log();
	void SetIbtrans_log(char* pStruct, TiXmlElement* pElement);

	LOGEVENT_CFG* GetLogevent_cfg();
	void SetLogevent_cfg(char* pStruct, TiXmlElement* pElement);

	MAIL_LOG* GetMail_log();
	void SetMail_log(char* pStruct, TiXmlElement* pElement);

	MAP_CFG* GetMap_cfg();
	void SetMap_cfg(char* pStruct, TiXmlElement* pElement);

	MAP_LOG* GetMap_log();
	void SetMap_log(char* pStruct, TiXmlElement* pElement);

	MARRIAGE_LOG* GetMarriage_log();
	void SetMarriage_log(char* pStruct, TiXmlElement* pElement);

	MW_CFG* GetMw_cfg();
	void SetMw_cfg(char* pStruct, TiXmlElement* pElement);

	NATION_LOG* GetNation_log();
	void SetNation_log(char* pStruct, TiXmlElement* pElement);

	NPC_CFG* GetNpc_cfg();
	void SetNpc_cfg(char* pStruct, TiXmlElement* pElement);

	PROPERTY_CFG* GetProperty_cfg();
	void SetProperty_cfg(char* pStruct, TiXmlElement* pElement);

	PROPERTY_LOG* GetProperty_log();
	void SetProperty_log(char* pStruct, TiXmlElement* pElement);

	RANK_CFG* GetRank_cfg();
	void SetRank_cfg(char* pStruct, TiXmlElement* pElement);

	RANK_LOG* GetRank_log();
	void SetRank_log(char* pStruct, TiXmlElement* pElement);

	REPETION_CFG* GetRepetion_cfg();
	void SetRepetion_cfg(char* pStruct, TiXmlElement* pElement);

	REPE_LOG* GetRepe_log();
	void SetRepe_log(char* pStruct, TiXmlElement* pElement);

	SCENELOGIC_CFG* GetScenelogic_cfg();
	void SetScenelogic_cfg(char* pStruct, TiXmlElement* pElement);

	SCENE_LOG* GetScene_log();
	void SetScene_log(char* pStruct, TiXmlElement* pElement);

	SPEEDCHECKER_CFG* GetSpeedchecker_cfg();
	void SetSpeedchecker_cfg(char* pStruct, TiXmlElement* pElement);

	STALL_CFG* GetStall_cfg();
	void SetStall_cfg(char* pStruct, TiXmlElement* pElement);

	STALL_LOG* GetStall_log();
	void SetStall_log(char* pStruct, TiXmlElement* pElement);

	STAT_LOG* GetStat_log();
	void SetStat_log(char* pStruct, TiXmlElement* pElement);

	TASKLUA_LOG* GetTasklua_log();
	void SetTasklua_log(char* pStruct, TiXmlElement* pElement);

	TASK_CFG* GetTask_cfg();
	void SetTask_cfg(char* pStruct, TiXmlElement* pElement);

	TASK_LOG* GetTask_log();
	void SetTask_log(char* pStruct, TiXmlElement* pElement);

	TEAM_CFG* GetTeam_cfg();
	void SetTeam_cfg(char* pStruct, TiXmlElement* pElement);

	TEAM_LOG* GetTeam_log();
	void SetTeam_log(char* pStruct, TiXmlElement* pElement);

	TRADE_CFG* GetTrade_cfg();
	void SetTrade_cfg(char* pStruct, TiXmlElement* pElement);

	TRADE_LOG* GetTrade_log();
	void SetTrade_log(char* pStruct, TiXmlElement* pElement);

	YQSACTIVITY_LOG* GetYqsactivity_log();
	void SetYqsactivity_log(char* pStruct, TiXmlElement* pElement);

	void FormatParam( char* pSrcStr, char* pDesStr );
	void FormatUpper( char* pSrcStr, char* pDesStr );
	void EvaluateInit();

};
