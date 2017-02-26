#pragma once
#include "corpstype.h"
#include "familytype.h"
#include "module_manager.h"
#include "entity.h"
#include "familydbinfo_pb.hxx.pb.h"
#include "corpsinfo_pb.hxx.pb.h"
//#include "gatelogic.h"
#include "family.h"
#include "union.h"
#include "corps.h"

using namespace GateServer;
class SysProtocol;

enum EM_DB_TABLE_TYPE
{
	TAB_BID = 1,
	TAB_NPC = 2,
	TAB_BATTLE = 3,
	TAB_CORPS = 4,
	TAB_UNOIN = 5
};
enum EM_CORPSBOSS_TYPE
{
	CORPS_BOSS_TYPE_ERROR	= 0,
	CORPS_COMMON_BOSS_TYPE	= 1,
	CORPS_HARD_BOSS_TYPE	= 2,
};


class CCorpservice : public CDataModule, public CSingleton< CCorpservice > 
{	
public:
	void OnLaunchServer();
	void OnExitServer();
	void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
	void OnMessage( int nServerID, CMessage* pMessage );
	void OnSeverMessage(int nServerID, CMessage* pMessage );
	CCorpservice();
	virtual ~CCorpservice(){ };
	int Initialize();
	int Resume(); 
	virtual void OnNewDayCome(); 
	virtual void OnNewWeekCome();
	static unsigned int CountSize();  	

	virtual bool IsLaunched() { return mIsLaunched; }
	
public:
	// 创建军团的请求
	void OnMessageCreatCorps( int nServerID, CMessage* pMessage );
	
	// 申请加入军团的请求消息
	void OnMessageApplyCorps( int nServerID, CMessage* pMessage );
	
	// 回复家族申请加入军团的消息请求
	void OnMessageReplyMemberApply( int nServerID, CMessage* pMessage );	
	
	// 邀请家族加入军团的消息请求
	void OnMessageInviteCorpsMember( int nServerID, CMessage* pMessage );
	
	// 答复军团邀请的消息请求
	void OnMessageReplyCorpsInvite( int nServerID, CMessage* pMessage );
	
	// 家族离开军团的消息请求	
	void OnMessageLeaveCorps( int nServerID, CMessage* pMessage );
	
	// 从军团踢出家族的消息请求
	void OnMessageKickFamily( int nServerID, CMessage* pMessage );	
	
	// 获取军团家族成员信息的请求
	void OnMessageGetCorpsFamiyInfo( int nServerID, CMessage* pMessage );
	
	// 获取军团属性信息的请求消息
	void OnMessageGetCorpsPropertyInfo( int nServerID, CMessage* pMessage );
	
	// 改变军团公告的消息请求
	void OnMessageChangeCorpsBulletin( int nServerID, CMessage* pMessage );
	
	// 改变军团首领的消息请求
	void OnMessageChanggeCorpsHeader( int nServerID, CMessage* pMessage );
	
	// 获取申请列表的消息请求
	void OnMessageGetCorpsApplyList( int nServerID, CMessage* pMessage );
	
	// 解散军团的消息请求
	void OnMessageDisbandCorps( int nServerID, CMessage* pMessage );
	
	// 取消解散军团的消息请求
	void OnMessageCancelCorpsDisband( int nServerID, CMessage* pMessage );
	
	// 清除军团申请列表的消息
	void OnMessageClearCorpsApply( int nServerID, CMessage* pMessage );	 
	
	// 获取军团信息列表的消息
	void OnMessageGetCorpsList( int nServerID, CMessage* pMessage );
		
	// 刷新军团信息的消息请求
	void OnMessageRefreshCorpsInfo( int nServerID, CMessage* pMessage );

	//-------------------------------------------------------------------------
	/// 查询指定NPC的竞价列表
	void OnMessageQueryBidRankRequest( int nServerID, CMessage* pMessage );

	//-------------------------------------------------------------------------
	/// 军团竞价请求
	void OnMessageCorpsBidRequest( int nServerID, CMessage* pMessage );

	//-------------------------------------------------------------------------
	/// 竞价时间结束通知
	void OnMessageBidTimeOutRequest( int nServerID, CMessage* pMessage );

	void OnMessageSelectBattleFamily( int nServerID, CMessage* pMessage );

	void OnMessageSelectBattleMember( int nServerID, CMessage* pMessage );

	//-------------------------------------------------------------------
	/// 创建/关闭房间
	void OnMessageOperateRoomRequest( int nServerID, CMessage* pMessage );

	/// 发起/放弃挑战
	void OnMessageChallegeRequest( int nServerID, CMessage* pMessage );

	/// 接受挑战
	void OnMessageReplyChallegeRequest( int nServerID, CMessage* pMessage );

	/// 获取挑战赛所有房间列表
	void OnMessageQueryRoomRequest( int nServerID, CMessage* pMessage );
	
	/// 获取自己对其他军团发起的挑战了列表
	void OnMessageQueryChallegeRequest( int nServerID, CMessage* pMessage );
	
	/// 获取对本军团的挑战列表
	void OnMessageQueryRecChallegeRequest( int nServerID, CMessage* pMessage );
	//-------------------------------------------------------------------

	// 增加玩家贡献度
	void OnMessagChangeCorpsContribute( CMessage* pMessage );

	// 改变军团金钱
	void OnMessageChangeCorpsMoney( CMessage* pMessage );
	
	// 改变军团荣耀
	void OnMessageChangeCorpsGlory( CMessage* pMessage );

	// 改变军团采集活跃度
	void OnMessageChangeCorpsCollectActive( int nServerID, CMessage* pMessage );
	
	// 聊天频道消息
	void OnChatMsg( int nServerID, CMessage* pMessage );	
	
	// 设置军团成员职位的消息请求 
	void OnMessageSetCorpsPost( int nServerID, CMessage* pMessage );

	// 升级军团的消息请求
	void OnMessageLevelUPCorps( int nServerID, CMessage* pMessage );

	// 贡献军团金钱的消息请求
	void OnMessageContributeCorpsMoney( int nServerID, CMessage* pMessage );

	//  家族给军团贡献金钱的消息请求
	void OnMessageFamilyContributeMoney( int nServerID, CMessage* pMessage );

	// 设置军团成员称号的消息请求
	void OnMessageSetMemberName( int nServerID, CMessage* pMessage );

	
	//-------------------------------------------------------------------
	// 处理军团战争结果
	void OnMessageBattleResultRequest( int nServerID, CMessage* pMessage );

	/// 处理军团长大旗归属
	void OnMessageBattleFlagRequest( int nServerID, CMessage* pMessage );

	/// 开始战争
	void OnMessageStartBattleRequest( int nServerID, CMessage* pMessage );

	void OnMessageAbortBattleCityRequest( int nServerID, CMessage* pMessage );

	void OnFireCorpsBattleEvent( int nRoleID, int nServerID );

	void OnDoBattleResultEvent( CCorps* npWinCorps, int nNpcID, int nMapID, CCorps* npLosCorps = NULL );
	//-------------------------------------------------------------------

	// 查询军团npc福利的消息请求
	void OnMessageQueryCorpsWelfare( int nServerID, CMessage* pMessage );
	
	// 获取军团npc福利的消息请求
	void OnMessageObtainCorpsWelfare( int nServerID, CMessage* pMessage );
	
	// 获取军团金钱的消息请求
	void OnMessageGetCorpsMoney( int nServerID, CMessage* pMessage );
	
	// 购买军团红包的消息请求
	void OnMessageBuyCorpsItem( int nServerID, CMessage* pMessage );

	// 申请加入联军的请求消息
	void OnMessageApplyUnion( int nServerID, CMessage* pMessage );

	// 回复军团申请加入联军的消息请求
	void OnMessageReplyUnionApply( int nServerID, CMessage* pMessage );	

	// 邀请军团加入联军的消息请求
	void OnMessageInviteUnionMember( int nServerID, CMessage* pMessage );

	// 答复联军邀请的消息请求
	void OnMessageReplyUnionInvite( int nServerID, CMessage* pMessage );

	// 军团离开联军的消息请求	
	void OnMessageLeaveUnion( int nServerID, CMessage* pMessage );

	// 从联军踢出军团的消息请求
	void OnMessageKickCorps( int nServerID, CMessage* pMessage );	

	// 创建联军请求
	void OnMessageCreateUnion( int nServerID, CMessage* pMessage );

	void OnMessageMakeUnion( int nServerID, CMessage* pMessage );

	// 获得联军信息请求
	void OnMessageGetUnionInfo( int nServerID, CMessage* pMessage );

	// 解散联军请求
	void OnMessageDismissUnion( int nServerID, CMessage* pMessage );

	// 获得联军申请列表请求
	void OnMessageUnionApplyList( int nServerID, CMessage* pMessage );

	// 获得联军申请列表请求
	void OnMessageAllUnionList( int nServerID, CMessage* pMessage );

	//void OnMessageGetOtherMemberList( int nServerID, CMessage* pMessage );

	void OnMessageGetAllMapList( int nServerID, CMessage* npMessage );

	// 查询军盟参战列表 [12/16/2009 Macro]
	void OnMessageGetUnionBattleList( int nServerID, CMessage* npMessage );


	// 军团战金钱回滚 [3/10/2010 Administrator]
	void OnMessageRollCorpsMoney( int nServerID, CMessage* npMessage );

	void OnEventDeleteRoom( int nRoleID, int nServerID );

	// 附城升级
	void OnEventCityUpgrade( CCorps* npCorps, int nServerID );

	// 军盟竞价结束处理
	int OnEventUnionBattleTimeOut( int nMapID, CCorps* npCorps );
	
	// 军盟开战处理
	int OnEventGetUnionBattleList( int* npList, int& nNum, CCorps* npCorps, int nMapID );

	//// 增加占领一个城池 [1/27/2010 Macro]
	//int OnEventAddMapOwn( int nCorpsID, int nNpcID );

	//// 移除占领的一个城池 [1/27/2010 Macro]
	//int OnEventDelMapOwn( int nCorpsID, int nNpcID );
	 
	// 任命诸侯国国王
	void OnMessageAppointPrince( int nServerID, CMessage* npMessage );

	// 迁都
	void OnMessageMoveCapital( int nServerID, CMessage* npMessage );

	// 军团颜色设置
	void OnMessageSetCorpsColor( int nServerID, CMessage* npMessage );

	// 军团联盟改名
	void OnMessageChangeUnionName( int nServerID, CMessage* npMessage );
	
	// 获取军团商品列表的请求消息
	void OnMessageGetCorpsGoodsList( int nServerID, CMessage* pMessage );
	
	// 获取军团兑换商品列表的消息请求
	void OnMessageGetCorpsExchangeGoodsList( int nServerID, CMessage* pMessage );
	
	// 发送获取军团商品列表的消息回应
	void SendResponseGetCorpsGoodsList( int nServerID, int nErrcode, int nRoleID, int nEntityID, PBLeagueShopInfo &rLeagueShop, int nNpcTmpID );
	
	// 发送获取军团可兑换商品列表的消息请求
	void SendResponseGetCorpsExchangeGoodList( int nServerID, int nErrcode, int nRoleID, int nEntityID, PBLeagueShopInfo &pbLeagueShop, int nNpcTmpID );
	
	// 发送军团商品发生变化的通知
	void SendNotifyChangeCorpsGoodsList( int nCorpsID, int nNpcTmpID, int nItemID, int nItemNum, int nComposeID = 0 );
	
	// 进行军团兑换的消息请求
	void OnMessageCorpsExchange( int nServerID, CMessage *pMessage );
	
	// 发送军团兑换的回应消息
	void SendResponseCorpsExchange( int nServerID, int nErrcode, int nRoleID, int nComposeID, int nNpcTmpID, int nItemNum );
	
	// 发送军团兑换发生变化的通知
	void SendNotifyChangeCorpsExchangeGoodsList( int nCorpsID, CGoodSInfo *pGoodInfo, int nNum );
	
	// 发送使用军团特供物品的通知
	void SendNotifyUseCorpsLeagueItem( int nCorpsID, CGoodSInfo *pGoodInfo, int nNum, int nRoleID, const char *pName );
	
	// 增加军团可兑换物品的消息通知
	void OnMessageAddCorpsExchangeItem( int nServerID, CMessage *pMessage );

	// 进入军团战战场请求
	void OnMessageLoginBattleRequest( int nServerID, CMessage *npMessage );

	// 进入战场通知客户端战场名
	void OnMessageSendBattleMapRequest( int nServerID, CMessage* npMessage );

	// 强制战斗结束处理
	void OnEventBattleOver( int nWinCamp, KEY_TYPE nKey );
	
	// 军团boss挑战次数改变
	void OnMessageChangeBossTimes( int nServerID, CMessage* npMessage );
	
	// 军团炼化值改变
	void OnMessageChangeBossRefineValue( int nServerID, CMessage * npMessage );
	// 军团boss副本开启
	int StartCorpBossRepetion(int  vCorpsID, int vBossID, int vBossType);
public:	
	// 创建军团的回应消息		
	void SendResponseCreatCorps( int nServerID, int nRoleID, int nRet, int nCorpsID, const char *pCorpsName, int nEntityID, int nCorpsColor );
	
	// 申请加入军团的回应消息
	void SendResponseApplyCorps( int nServerID, int nRoleID, int nRet, int nCorpsID, const char *pCorpsName, int nEntityID = 0  );
	
	// 有家族申请加入军团的消息通知	
	void SendNotifyCorpsMemberApply( CGatePlayer *pPlayer, int nRoleID, FamilyData &pbFamilyData );
	
	// 回复家族申请加入军团的消息回应
	void SendResponseReplyCorpsMemberApply( int nServerID, int nRoleID, int nRet, int nResult, int nFamilyID, int nEntityID = 0 );
	
	// 申请加入军团的结果通知 
	void SendNotifyApplyCorpsResult( CGatePlayer *pPlayer, int nRoleID, int nRet, int nCorpsID, const char *pCorpsName, int nUnionID = 0, const char *pUnionName = NULL );
	
	// 邀请家族加入军团的消息回应	
	void SendResponseInviteCorpsMember( int nServerID, int nRoleID, int nRet, const char *pFamilyName, int nEntityID );
	
	// 邀请加入军团的消息通知	 	
	void SendNotifyCorpsInvite( CGatePlayer *pPlayer, int nRoleID, PBCorpsBasicInfo &pbCorpsInfo, int nInvitingID, const char * pInvitingName );
	
	// 答复军团邀请的消息回复
	void SendResponseReplyCorpsInvite( int nServerID, int nRoleID, int nRet, int nResult, int nCorpsID, const char *pCorpsName, int nEntityID, int nUnionID = 0, const char *pUnionName = NULL );
	
	// 邀请家族加入军团的结果通知
	void SendNotifyCorpsInviteResult( CGatePlayer *pPlayer, int nRoleID, int nResult, const char *pFamilyName );
	
	// 有家族加入军团的消息通知	 	
	void SendNotifyFamilyJoinCorps( PBCorpsMember &pbCorpsMember, int *pRoleIDList, int vRoleNum );
		
	// 家族离开军团的消息回应	 
	void SendResponseLeaveCorps( int nServerID, int nRoleID, int nRet, int nEntityID );
	
	// 家族离开军团的消息通知	
	void SendNotifyFamilyLeaved( int nFamilyID, const char *pFamilyName, int *pRoleIDList, int nRoleNum );
	
	// 从军团踢出家族的消息回应
	void SendResponseKickFamily( int nServerID, int nRoleID, int nRet, int nFamilyID, const char *pFamilyName, int nEntityID );
	
	// 家族从军团里面被踢掉的通知		
	void SendNotifyFamilyKicked( int nFamilyID, const char * pFamilyName, int *pRoleIDList, int tNum );
	
	// 获取军团家族成员信息的回应消息  	
	void SendResponseGetCorpsFamilyInfo( int nServerID, int nRoleID, PBCorpsMemberList &pbCorpsMemberList, int nEntityID );
	
	// 获取军团属性信息的回应消息	
	void SendResponseGetCorpsPropertyInfo( int nServerID, int nRoleID, CorpsProperty &pbCorpsProperty, int nEntityID );
	
	// 改变军团公告的消息回复
	void SendResponseChangeCorpsBulletin( int nServerID, int nRoleID, int nRet, const char *pBulletin, int nEntityID );
	
	// 改变军团公告的消息通知
	void SendNotifyChangeCorpsBulletin( const char *pBulletin, int *pRoleIDlist, int tNum );
			
	// 改变军团首领的消息回复
	void SendResponseChangeCorpsHeader( int nServerID, int nRoleID, int nRet, int nMemberID, const char *pMemberName, int nEntityID );

	// 改变军团首领的消息通知
	void SendNotifyChangeCorpsHeader( int nHeaderID, const char *pHeaderName, int nMemberID, const char *pMemberName, int *pRoleIDList, int nNum );

	// 获取申请列表的消息回应 	
	void SendResponseGetCorpsApplyList( int nServerID, int nRoleID, int nRet, PBCorpsApplyList &pbApplyList, int nEntityID );

	// 解散军团的消息回应
	void SendResponseDisbandCorps( int nServerID, int nRoleID, int nRet, int nEntityID );
	
	// 解散军团的消息通知
	void SendNotifyDisbandCorps( int *pRoleIDList, int tNum );
	
	// 取消解散军团的消息请求
	void SendResponseCancelCorpsDisband( int nServerID, int nRoleID, int nRet, int nEntityID = 0 );
	
	// 发送军团的消息通知
	void SendCorpsNotifyMsg( CMessage *pMessage, int *pRoleIDList, int vRoleNum );
	
	// 发送清除军团申请列表的消息回应
	void SendResponseClearCorpsApply( int nServerID, int nRet, int nRoleID, int nEntityID );
	
	// 发送刷新军团信息的回应消息
	void SendResponseRefreshCorpsInfo( int nServerID, int nRoleID, int nEntityID, PBCorpsMemberList &pbCorpsMemberList );
	
	// 发送获取军团信息的回应消息
	void SendResponseGetCorpsList( int nServerID, int nRoleID, int nEntityID, CorpsInfoList &pbCorpsList, int nCurPage, int nTotalPage );
	
	
	// 设置军团成员职位的消息回应	
	void SendResponseSetCorpMemberPost( int nServerID, int nRoleID, int nErrcode, int nMemberID, int nPost, int nEntityID = 0 );  

	// 升级军团的消息回应
	void SendResponseLevelUPCorps( int nServerID, int nErrcode, int nLevel, int nRoleID, int nEntityID = 0 );  

	// 贡献军团金钱的消息回应
	void SendResponseContributeCorpsMoney( int nServerID, int nErrcode, int nMoney, int nRoleID, int nEntityID = 0 );  

	// 家族给军团贡献金钱的消息回应
	void SendResponseFamilyContributeMoney( int nServerID, int nErrcode, int nMoney, int nRoleID, int nEntityID = 0 );  

	// 设置军团成员称号的消息回应
	void SendResponseSetMemberName( int nServerID, int nMemberID, const char *pName, int nRoleID, int nEntityID, int nErrcode );  

	// 设置军团职位的通知
	void SendNotifySetCorpsPost( int *pIDList, int nNum, int nManagerID, const char *pManagerName, int nMemberID, const char *pMemberName, int nOriginalPost, int nNewPost );

	// 军团升级的通知
	void SendNotifyCorpsLevelUP( int *pIDList, int nNum, int nLevel, int nMoney, int nGlory, int nMaxFamilyNum, int nMaxGeneralNum, int nCounsellor, int nLevelMoney, int nLevelGlory );  
	
	// 发送军团金钱改变的消息通知
	void SendNotifyCorpsMoneyChanged( int nCorpsID, int nCorpsMoney, int nRoleID, int nValue, int nReason = REASON_CONTRIBUTE, const char *pName = NULL );
																		  
	// 发送军团荣耀改变的消息通知
	void SendNotifyCorpsGlroyChangedNotify( int nCorpsID, int nCorpsGlroy, int nRoleID, int nValue, int nReason = REASON_NPCWELFARE, const char *pName = NULL );
	
	// 发送军团的个人贡献度改变的消息通知
	void SendNotifyCorpsContributeChangedNotify( int nCorpsID, int nMemberContribute, int nRoleID, int nValue );

	// 发送军团采集活跃度改变的消息通知
	void SendNotifyCorpsCollectActiveChange( int nCorpsID, int nRoleID, int nActiveValue, int nValue=0, int nChangeType=0 );

	// 军团pvp等级提升的通知	
	void SendNotifyCorpsPVPStarLevelUP( int nServerID );
	
	// 发送军团可以升级的消息通知
	void SendNotifyCorpsCanLevelUP( int nCorpsID );

	// 军团发布任务消息通知
	void SendNotifyCorpsPublishTask( int nCorpsID );
	
	// 消耗军团金钱
	int ConsumeCorpsMoney( int nRoleID, int nMoney, int nReason = REASON_ACTIVIYCONSUME );
		
	// 给军团成员发送消息
	void SendCorpsMsg( int nRoleID, CMessage *pMessage );
	
	// 给军团成员发送消息
	void SendCorpsMsgByID( int nCorpsID, CMessage *pMessage );

	// 发送军团属性
	void SendCorpsProperty( int nCorpsID );
	
	// 发送称号被改变的通知
	void SendNotifyCorpsTitleChanged( CGatePlayer *pPlayer, int nManagerID, const char *pManagerName, const char *pCorpsTitle );
	
	// 发送购买军团红包的消息回应
	void SendResponseBuyCorpsItem( CGatePlayer *pPlayer, int nErrcode, int nRedPaperID, int nRedPaperNum, int nEntityID = 0 );
	
	// 初始化军团信息列表
	int InitializeCorpsConfig();

	virtual void ReloadCfg( );

	// 发送创建联军回应
	void SendCreateUnionResponse( CGatePlayer *pPlayer, int vRoleID, int vResult, int vUnionID );

	// 发送创建联军通知
	void SendCreateUnionNotify( int vServerID, int vRoleID, CUnion *pUnion );

	// 发送创建联军回应
	void SendMakeUnionResponse( CGatePlayer *pPlayer, int vRoleID, int vResult, int vOpType );

	// 发送创建联军通知
	void SendMakeUnionNotify( CGatePlayer *pPlayer, int vRoleID, int vOpType, CCorps *pCorps );

	// 申请加入联军的回应消息
	void SendResponseApplyUnion( int nServerID, int nRoleID, int nRet, int nUnionID, const char *pUnionName, int nEntityID = 0  );

	// 申请加入联军的回应消息
	void SendResponseReplyApplyUnion( int nServerID, int nRoleID, int nRet, int nResult, int nCorpsID );

	// 有军团申请加入联军的消息通知	
	void SendNotifyUnionMemberApply( CGatePlayer *pPlayer, int nRoleID, PBCorpsBasicInfo vInfo );	

	// 邀请军团加入联军的消息回应	
	void SendResponseInviteUnionMember( int nServerID, int nRoleID, int nRet, const char *pCorpsName, int nEntityID );

	// 邀请加入联军的消息通知	 	
	void SendNotifyUnionInvite( CGatePlayer *pPlayer, int nRoleID, PBUnionBasicInfo &pbCorpsInfo, int nInvitingID, const char * pInvitingName );

	// 答复联军邀请的消息回复
	void SendResponseReplyUnionInvite( int nServerID, int nRoleID, int nRet, int nResult, int nUnionID, const char *pUnionName, int nEntityID );

	// 邀请军团加入联军的结果通知
	void SendNotifyUnionInviteResult( CGatePlayer *pPlayer, int nRoleID, int nResult, const char *pCorpsName );

	// 有军团加入联军的消息通知	 	
	void SendNotifyCorpsJoinUnion( PBCorpsBasicInfo CorpsInfo, int vUnionID, char *vUnionName, int *pRoleIDList, int vRoleNum );

	// 军团离开联军的消息回应	 
	void SendResponseLeaveUnion( int nServerID, int nRoleID, int nRet, int nEntityID );

	// 军团离开联军的消息通知	
	void SendNotifyCorpsLeaved( int nCorpsID, const char *pCorpsName, int *pRoleIDList, int nRoleNum );

	// 从联军踢出军团的消息回应
	void SendResponseKickCorps( int nServerID, int nRoleID, int nRet, int nCorpsID, const char *pCorpsName, int nEntityID );

	// 军团从联军里面被踢掉的通知		
	void SendNotifyCorpsKicked( int nCorpsID, const char * pCorpsName, int *pRoleIDList, int tNum );

	// 发送获得联军回应
	void SendGetUnionInfoResponse( CGatePlayer *pPlayer, int vRoleID, int vResult, CUnion *pUnion );

	// 发送解散联军回应
	void SendDismissUnionResponse( CGatePlayer *pPlayer, int vRoleID, int vResult );

	// 发送解散联军通知
	void SendDismissUnionNotify( int nServerID, int vRoleID, int *vCorpsID, int vNum );

	// 发送获得联军申请列表回应
	void SendUnionApplyListResponse( CGatePlayer *pPlayer, int vRoleID, CUnion *pUnion );

	// 发送联军邀请结果通知
	void SendNotifyUnionInviteResult( int nRoleID, int nResult, const char * pInvitedName );

	// 改变军团成员国籍
	void ChangeCorpsCountry( int vMapID, int vCorpsID );

	// 申请加入军盟通知
	void SendNotifyApplyUnion( int nServerID, int nRoleID, int nRet, int nUnionID, const char *pUnionName );

	void SendChangeUnionNameResponse( int nRoleID, int nRet );
	void SendChangeUnionNameNotify( int nRoleID, const char *pUnionName, const char *pUnionNotice );

	void SendBattleJoinNotice( int nServerID, int nNpcID, const char* npCorpsName, int nCharID, int nType );
	//改变军团boss挑战次数
	void SendNotifyBossTimesChanged(int nCorpsID, int nBossID, int nTimes,int nValue, int nRoleId, int nReason, const char * pName  );

	//改变军团的炼化值
	void SendNotifyBossRefineValueChanged( int nCorpsID, int nRefineValue, int nValue, int nRoleID, int nReason , const char *pName  );
	
	//发送军团boss副本开启的通知
	void SendNotifyCorpsBossRepetionOpen( int nCorpsID, int nBossId, int nRoleId , const char *pName );
	//判断corpsboss的 类型
	int  GetCorpsBossType(int nBossID );
	// 玩家进入军团boss的回应
	void SendIntoCorpsBossRepetionResponse( int nRoleID, int nRet);

	void SendSummonCorpsBossRespones( int nRoleID, int nRet );
	void OnMessageSummonCorpsBoss( int nServerID, CMessage * npMessage );
	// 发送军团副本结束通知
	void SendNotifyCorpsBossRepetionEnd(int nCorpsID );
public:
	void LoadCorpsFromDB( int nStart, int nTotalCount );
	void OnSessionLoadCorps( CMessage* pMessage, int nStart, int nTotalCount );
	void OnSessionLoadCorpsBid( CMessage* pMessage, int nStart, int nTotalCount );
	void OnSessionLoadCorpsBattle( CMessage* pMessage, int nStart, int nTotalCount );
	void OnSessionLoadNpcStatus( CMessage* pMessage, int nStart, int nTotalCount );

	int OnSessionCreatCorpsDBInfo( CMessage* pMessage, int nCorpsObjID, int nRooterID );
	int OnSessionCreateUnionDBInfo( CMessage* pMessage, int nHeadID, int nUnionID );
	void LoadUnionFromDB( int nStart, int nCount );
	void OnSessionLoadUnion( CMessage* pMessage, int nStart, int nCount );
	void OnAGIPSessionCreateUnion(SysProtocol* pSysProto, int nObjectID, int nGroupType);
	void OnAGIPSessionCreateCorps(SysProtocol* pSysProto, int nObjectID, int nGroupType);

	void OnAGIPTimeoutCreateUnion(int nObjectID, int nGroupType);
	void OnAGIPTimeoutCreateCorps(int nObjectID, int nGroupType);
	
	void StartLoadAllFromDB(int _Type);
	void OnSessionGetDataNumFromDB( CMessage *pMessage, int nParam1, int nParam2, int nTableType );

	// 加载配置的回调函数
	static void LoadCorpConfigCallback(const char* pcConfigPath);

private: 
	CWTimer mUpdateTimer;		// 检查军团数据更新的计时器
	CWTimer mDisbandTimer;		// 检查军团解散时间的计时器
	CWTimer mBattleNoticeTimer;	// 参战通知定时器
	CWTimer m_CheckNpcStateTimer; // 军团战NPC状态检查定时器
	bool	mIsLaunched;

	bool m_bMigratieMode;	//判断是否迁移数据
};
