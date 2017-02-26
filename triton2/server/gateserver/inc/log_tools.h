#ifndef _LOG_TOOLS_H_
#define _LOG_TOOLS_H_

#include "log_msg_pb.hxx.pb.h"
#include "message_pb.hxx.pb.h"
#include "linux/types.h"

CMessage* BuildFamilyLogMessage(uint32_t nFamilyID, uint32_t nOpRoleID, 
		EOrganAction emAction, EFamilyAttr nAttrID, int nParam1, int nParam2,int nParam3 = 0 );

CMessage* BuildCorpsLogMessage(uint32_t nCorpsID, uint32_t nOpRoleID, 
		EOrganAction emAction, ECorpsAttr nAttrID, int nParam1, int nParam2, int nParam3 = 0 );

CMessage* BuildWorldLogMessage(int nWorldID, int nAction, int nParam1, int nParam2, int nParam3 = 0 );

void LogFamilyAction(uint32_t nFamilyID, uint32_t nOpRoleID, 
		EOrganAction emAction, EFamilyAttr nAttrID, int nParam1, int nParam2, int nParam3 = 0 );

void LogCorpsAction(uint32_t nCorpsID, uint32_t nOpRoleID, 
		EOrganAction emAction, ECorpsAttr nAttrID, int nParam1, int nParam2, int nParam3 = 0 );

void LogWorldAction(int nWorldID, int nAction, int nParam1, int nParam2, int nParam3 = 0);
		

// 记录聊天消息
void LogChatMsg( int nRoleID, const char* szRoleName, const char* szMsg, int nGateWayID, int nChannel, int nIP = 0 );



// - 成员加入
#define  LogMemberJoin( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_MEMBERJOIN, EM_FAMILY_MEMBER, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_MEMBERJOIN, EM_CORPS_MEMBER, nParam1, nParam2, nParam3 ); }	

// - 成员离开
#define  LogMemberLeave( LogType, nRganID, nOpRoleID, nParam1, nParam2 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_MEMBERLEAVE, EM_FAMILY_MEMBER, nParam1, nParam2 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_MEMBERLEAVE, EM_CORPS_MEMBER, nParam1, nParam2 ); }

// - 家族(军团)成立 (AGIP)
#define  LogOrganCreate( LogType, nRganID, nOpRoleID, nParam1, nParam2 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_CREATE, EM_FAMILY_MEMBER, nParam1, nParam2 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_CREATE, EM_CORPS_MEMBER, nParam1, nParam2 ); } 

// - 家族(军团)解散
#define  LogOrganDisband( LogType, nRganID, nOpRoleID, nParam1, nParam2 ) \
if( LogType == ORGANLOGTYPE_FAMILY )  \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_DISBAND, EM_FAMILY_MEMBER, nParam1, nParam2 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )			 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_DISBAND, EM_CORPS_MEMBER, nParam1, nParam2 ); } 

// - 消耗家族(军团)金钱
#define  LogOrganGetMoney( LogType, nRganID, nOpRoleID, nParam1, nParam2 ) \
if( LogType == ORGANLOGTYPE_FAMILY )\
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_GETMONEY, EM_FAMILY_MONEY, nParam1, nParam2 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_GETMONEY, EM_CORPS_MONEY, nParam1, nParam2 ); } 

// - 贡献家族(军团)金钱
#define  LogOrganContributeMoney( LogType, nRganID, nOpRoleID, nParam1, nParam2 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_CONTRIBUTEMONEY, EM_FAMILY_MONEY, nParam1, nParam2 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_CONTRIBUTEMONEY, EM_CORPS_MONEY, nParam1, nParam2 ); } 

// - 购买家族(军团)物品(红包)
#define  LogOrganBuyItem( LogType, nRganID, nOpRoleID, nParam1, nParam2 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_BUYITEM, EM_FAMILY_MONEY, nParam1, nParam2 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_BUYITEM, EM_CORPS_MONEY, nParam1, nParam2 ); } 

// - 家族(军团)升级
#define  LogOrganLevelUP( LogType, nRganID, nOpRoleID, nParam1, nParam2 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_LEVELUP, EM_FAMILY_LEVEL, nParam1, nParam2 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_LEVELUP, EM_CORPS_LEVEL, nParam1, nParam2 ); } 

// - 升级家族军团引起的家族金钱改变
#define  LogOrganMoneyByLevelUP( LogType, nRganID, nOpRoleID, nParam1, nParam2 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_LEVELUP, EM_FAMILY_MONEY, nParam1, nParam2 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_LEVELUP, EM_CORPS_MONEY, nParam1, nParam2 ); } 


// - 家族(军团)职位设定
#define  LogOrganSetPost( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_SETPOST, EM_FAMILY_RANK, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_SETPOST, EM_CORPS_RANK, nParam1, nParam2, nParam3 ); }	

// - 获取家族(军团)npc福利	
#define  LogOrganGetNpcWelfareMoney( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_GETNPCWELFARE, EM_FAMILY_MONEY, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_GETNPCWELFARE, EM_CORPS_MONEY, nParam1, nParam2, nParam3 ); } 

#define  LogOrganGetNpcWelfareGlory( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
	if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_GETNPCWELFARE, EM_FAMILY_GLORY, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_GETNPCWELFARE, EM_CORPS_GLORY, nParam1, nParam2, nParam3 ); } 


// - 贡献金钱改变家族(军团)贡献度
#define  LogOrganContributeByContribute( LogType, nRganID, nOpRoleID, nParam1, nParam2 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_CONTRIBUTEMONEY, EM_FAMILY_CONTRIBUTION, nParam1, nParam2 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_CONTRIBUTEMONEY, EM_CORPS_CONTRIBUTION, nParam1, nParam2 ); } 


// - 任务改变家族（军团）金钱
#define  LogOrganMoneyByTask( LogType, nRganID, nOpRoleID, nParam1, nParam2 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_TASK, EM_FAMILY_MONEY, nParam1, nParam2 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_TASK, EM_CORPS_MONEY, nParam1, nParam2 ); } 


// - 使用功能性物品改变家族（军团）金钱
#define  LogOrganMoneyByUseFunItem( LogType, nRganID, nOpRoleID, nParam1, nParam2 ) \
	if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_USEFUNCITEM, EM_FAMILY_MONEY, nParam1, nParam2 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_USEFUNCITEM, EM_CORPS_MONEY, nParam1, nParam2 ); } 


// - 使用功能性物品改变军团炼化值
#define LogOrganRefineValueByUseFunItem( LogType, nRganID, nOpRoleID, nParam1, nParam2 ) \
if( LogType == ORGANLOGTYPE_CORPS )\
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_USEFUNCITEM, EM_CORPS_REFINEVALUE, nParam1, nParam2 ); }

// - 召唤军团boss消耗军团炼化值
#define LogOrganRefineValueBySummonBoss( LogType, nRganID, nOpRoleID, nParam1, nParam2 ) \
if( LogType == ORGANLOGTYPE_CORPS )\
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_SUMMONCORPSBOSS, EM_CORPS_REFINEVALUE, nParam1, nParam2 ); }

// - 完成军团fb奖励军团积分
#define LogOrganCorpsScoreByCorpsFb( LogType, nRganID, nOpRoleID, nParam1, nParam2 ) \
if( LogType == ORGANLOGTYPE_CORPS )\
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_CORPSREPETION, EM_CORPS_SCORE, nParam1, nParam2 ); }

// - 完成军团（采集）任务改变军团（采集）活跃度
#define LogOrganCorpsActivityByTask( LogType, nRganID, nOpRoleID, nParam1, nParam2 ) \
if( LogType == ORGANLOGTYPE_CORPS )\
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_TASK, EM_CORPS_ACTIVITY, nParam1, nParam2 ); }

// - 任务改变家族(军团) 个人贡献度
#define  LogOrganContribteByTask( LogType, nRganID, nOpRoleID, nParam1, nParam2 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_TASK, EM_FAMILY_CONTRIBUTION, nParam1, nParam2 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_TASK, EM_CORPS_CONTRIBUTION, nParam1, nParam2 ); }

// - 家族兑换改变家族(军团) 个人贡献度
#define  LogOrganContribteByExchange( LogType, nRganID, nOpRoleID, nParam1, nParam2 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_EXCHANGE, EM_FAMILY_CONTRIBUTION, nParam1, nParam2 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_EXCHANGE, EM_CORPS_CONTRIBUTION, nParam1, nParam2 ); }
 
// - 家族/军团对NPC竞价
#define  LogBidNpc( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_BID_, EM_FAMILY_BATTLE, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_BID_, EM_CORPS_BATTLE, nParam1, nParam2, nParam3 ); }

// - 竞价结束事件
#define  LogConfirm( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_CONFIRM, EM_FAMILY_BATTLE, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_CONFIRM, EM_CORPS_BATTLE, nParam1, nParam2, nParam3 ); }

// - NPC争夺战开战
#define  LogJoinNPCBattle( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_JOIN_BATTLE, EM_FAMILY_BATTLE, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_JOIN_BATTLE, EM_CORPS_BATTLE, nParam1, nParam2, nParam3 ); }

// - NPC争夺战结束
#define  LogNPCFin( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_FIN, EM_FAMILY_BATTLE, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_FIN, EM_CORPS_BATTLE, nParam1, nParam2, nParam3 ); }

// - 放弃对NPC/城池的占领
#define  LogAbort( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_ABORT, EM_FAMILY_BATTLE, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_ABORT, EM_CORPS_BATTLE, nParam1, nParam2, nParam3 ); }

// - 开设挑战赛房间
#define  LogOpenRoom( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_OPEN_ROOM, EM_FAMILY_CHANLENGE, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_OPEN_ROOM, EM_CORPS_CHANLENGE, nParam1, nParam2, nParam3 ); }

// - 关闭挑战赛房间
#define  LogCloseRoom( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_CLOSE_ROOM, EM_FAMILY_CHANLENGE, nParam1,  nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_CLOSE_ROOM, EM_CORPS_CHANLENGE, nParam1,  nParam2, nParam3 ); }

// - 发送挑战申请
#define  LogSendChallege( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_SEND_CHALLEGE, EM_FAMILY_CHANLENGE, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_SEND_CHALLEGE, EM_CORPS_CHANLENGE, nParam1, nParam2, nParam3 ); }

#define  LogRecChallege( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_REC_CHALLEGE, EM_FAMILY_CHANLENGE, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_REC_CHALLEGE, EM_CORPS_CHANLENGE, nParam1, nParam2, nParam3 ); }

// - 同意挑战请求
#define  LogJoinChaBattle( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_JOIN_BATTLE, EM_FAMILY_CHANLENGE, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_JOIN_BATTLE, EM_CORPS_CHANLENGE, nParam1, nParam2, nParam3 ); }

#define  LogSubmit( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_SUBMIT, EM_FAMILY_BATTLE, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_SUBMIT, EM_CORPS_BATTLE, nParam1, nParam2, nParam3 ); }

// - 放弃发起的挑战请求
#define  LogCloseChallenge( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_ABORT, EM_FAMILY_CHANLENGE, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_ABORT, EM_CORPS_CHANLENGE, nParam1, nParam2, nParam3 ); }



// - 任务完成获得经验
#define  LogGlroyByTask( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_TASK, EM_FAMILY_GLORY, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_TASK, EM_CORPS_GLORY, nParam1, nParam2, nParam3 ); }

// - 家族品仙桃减少家族经验
#define  LogGlroyByRepetion( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_REPETION, EM_FAMILY_GLORY, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_REPETION, EM_CORPS_GLORY, nParam1, nParam2, nParam3 ); }

// - 进入家族副本减少家族金钱
#define  LogOrganMoneyByRepetion( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_REPETION, EM_FAMILY_MONEY, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_REPETION, EM_CORPS_MONEY, nParam1, nParam2, nParam3 ); }

// - NPC争夺赛胜利
#define  LogOrganBattleVictory( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_BATTLEVICTORY, EM_FAMILY_BATTLE, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_BATTLEVICTORY, EM_CORPS_BATTLE, nParam1, nParam2, nParam3 ); }

// - 挑战赛胜利
#define  LogOrganChallengeVictory( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
	if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_BATTLEVICTORY, EM_FAMILY_CHANLENGE, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_BATTLEVICTORY, EM_CORPS_CHANLENGE, nParam1, nParam2, nParam3 ); }


// - 领取家族内丹、军团暂时没有
#define  LogOrganStartRedstone( LogType, nRganID, nOpRoleID, nParam1, nParam2, nParam3 ) \
	if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_REDSTONE, EM_FAMILY_NULL, nParam1, nParam2, nParam3 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_REDSTONE, EM_CORPS_NULL, nParam1, nParam2, nParam3 ); }

#endif

// - 军团领税、家族暂时没有	
#define  LogOrganGetTax( LogType, nRganID, nOpRoleID, nParam1, nParam2 ) \
if( LogType == ORGANLOGTYPE_FAMILY ) \
{ LogFamilyAction(nRganID, nOpRoleID, EM_ORGAN_TAX, EM_FAMILY_MONEY, nParam1, nParam2 ); }	\
else if( LogType == ORGANLOGTYPE_CORPS )				 \
{ LogCorpsAction(nRganID, nOpRoleID, EM_ORGAN_TAX, EM_CORPS_MONEY, nParam1, nParam2 ); }
