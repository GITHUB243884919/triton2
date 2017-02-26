#pragma once

// #ifndef _FCM_SERVICE_H_
// #define _FCM_SERVICE_H_

#include "module_manager.h"
#include "entity.h"
#include "lk_hashmap.h"
#include "lk_string.h"
#include "gateobj_define.h"
#include "lk_set.h"
#include "familydbinfo_pb.hxx.pb.h"
#include "familymessage_pb.hxx.pb.h"
#include "family.h"

using namespace GateServer;	

class CServiceFamily :public CDataModule, public CSingleton< CServiceFamily >
{
public:
	void OnLaunchServer();
	void OnExitServer();
	void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );
	void OnMessage(int nServerID, CMessage* pMessage );
	void OnSeverMessage(int nServerID, CMessage* pMessage );
	CServiceFamily();
	~CServiceFamily();
	int Initialize();
	int Resume();
	static unsigned int CountSize();
	void* operator new( size_t nsize );
	void  operator delete( void* pointer );	

	virtual bool IsLaunched() { return mIsLaunched; }	
	
	void OnLaunchComplete();
public:
	void LoadFamilyFromDB( int nStart, int nTotalCount );
	void OnSessionLoadFamilyFromDB( CMessage* pMessage, int nStart, int nTotalCount );
	void OnMessageFangChenMiRequest( int nServerID, CMessage* pMessage );
	
	void GetFamilyCountFromDB();
	
	void OnSessionGetFamilyCountFromDB( CMessage* pMessage, int nParam1, int nParam2 );
public:	
	// 处理消息  CMessageCreatFamilyRequest
	void OnMessageCreatFamily( int nServerID, CMessage *pMessage );

	// 处理消息  CMessageInviteMemberRequest
	void OnMessageInviteMember( int nServerID, CMessage *pMessage );	  

	// 处理消息 CMessageReplyFamilyInviteRequest
	void OnMessageReplyFamilyInvite( int nServerID, CMessage *pMessage );	  

	// 处理消息 CMessageApplyFamilyRequest
	void OnMessageApplyFamily( int nServerID, CMessage *pMessage );	  	

	// 处理消息CMessageReplyMemberApplyRequest   
	void OnMessageReplyMemberApply( int nServerID, CMessage *pMessage );	

	// 处理消息 CMessagePostAppointRequest   
	void OnMessagePostAppoint( int nServerID, CMessage *pMessage );	  	

	// 处理消息 CMessageLeaveFamilyRequest
	void OnMessageLeaveFamily( int nServerID, CMessage *pMessage );	  	

	// 处理消息 CMessageGetFamilyMemberInfoRequest		 
	void OnMessageGetFamilyMemberInfo( int nServerID, CMessage *pMessage );	  	

	// 处理消息 CMessageGetFamilyPropertyRequest
	void OnMessageGetFamilyPropetyInfo( int nServerID, CMessage *pMessage );	  	

	// 处理消息 CMessageGetAllFamilyInfoResponse
	void OnMessageGetAllFamilyInfo( int nServerID, CMessage *pMessage );	  	

	// 处理消息 CMessageGetApplyListRequest
	void OnMessageGetApplyList( int nServerID, CMessage *pMessage );	  	

	// 处理消息	CMessageKickPlayerRequest
	void OnMessageKickPlayer( int nServerID, CMessage *pMessage );	  	

	// 处理消息 CMessageDisbandFamilyRequest
	void OnMessageDisbandFamily( int nServerID, CMessage *pMessage );	  	

	// 处理消息 CMessageAppointHeaderRequest
	void OnMessageAppointHeader( int nServerID, CMessage *pMessage );

	// 处理消息 CMessageChangeFamilyNoticeRequest
	void OnMessageChangeFamilyNotice( int nServerID, CMessage *pMessage );		

	// 处理消息	CMessageClearFamilyApplyRequest
	void OnMessageClearFamilyApply( int nServerID, CMessage *pMessage );
	
	// 处理消息 CMessageCancelFamilyDisbandRequest
	void OnMessageCancelFamilyDisband( int nServerID, CMessage *pMessage );
	
	// 处理消息 CMessageRefreshFamilyInfoRequest
	void OnMessageRefreshFamilyInfo( int nServerID, CMessage *pMessage );
	

	// 设置家族成员职位的消息请求
	void OnMessageSetFamilyMemberPost( int nServerID, CMessage *pMessage );

	// 升级家族的请求
	void OnMessageLevelUPFamily( int nServerID, CMessage *pMessage );

	// 捐献家族金钱的消息请求
	void OnMessageContributeFamilyMoney( int nServerID, CMessage *pMessage );	

	// 处理争夺NPC报名消息
	void OnMessageBidNpc( int nServerID, CMessage *pMessage );

	// 处理NPC争夺到时消息
	void OnMessageBidTimeOut( int nServerID, CMessage *pMessage );

	// 处理族长报名成员消息
	void OnMessageMemberSubmit( int nServerID, CMessage *pMessage );

	// 处理进入战场消息
	void OnMessageBattleStart( int nServerID, CMessage *pMessage );

	// 处理战场结束消息
	void OnMessageBattleEnd( int nServerID, CMessage *pMessage );

	// 处理NPC查询请求
	void OnMessageNpcInfo( int nServerID, CMessage *pMessage );

	// 处理放弃NPC请求
	void OnMessageAbortNpc( int nServerID, CMessage *pMessage );

	// 开设房间请求
	void OnMessageMakeRoom( int nServerID, CMessage *pMessage );

	// 挑战房间请求
	void OnMessageChaRoom( int nServerID, CMessage *pMessage );

	// 同意挑战请求
	void OnMessageAgreeBattle( int nServerID, CMessage *pMessage );

	// 查询房间列表请求
	void OnMessageQueryAllRoom( int nServerID, CMessage *pMessage );

	// 查询自己的房间请求
	void OnMessageQueryMyRoom( int nServerID, CMessage *pMessage );

	// 查询自己的挑战请求
	void OnMessageQueryMyCha( int nServerID, CMessage *pMessage );

	// 放弃房间请求
	void OnMessageAbortRoom( int nServerID, CMessage *pMessage );

	// 放弃挑战请求
	void OnMessageAbortCha( int nServerID, CMessage *pMessage );	
	
	// 改变家族金钱
	void OnMessageChangeFamilyMoney( CMessage *pMessage );
	
	// 改变玩家荣耀值
	void OnMessageChangeFamilyGlory( CMessage *pMessage );
	
	// 增加成员贡献
	void OnMessageChangeFamilyContribute( CMessage *pMessage );
	
	// 开启家族boss副本的请求
	void OnMessageStartFamilyBoss( int nServerID, CMessage *pMessage );
	
	// 查询家族Npc福利
	void OnMessageQueryFamilyNpcWelfare( int nServerID, CMessage *pMessage );
	
	// 获取家族金钱( 把家族金钱换成个人金钱 )
	void OnMessageGetFamilyMoney( int nServerID, CMessage *pMessage );
		
	// 从占领npc处领取家族金钱( 和荣耀 )的消息请求
	void OnMessageObtainFamilyMoney( int nServerID, CMessage *pMessage );
	
	// 领取家族内丹的消息
	void OnMessageGetExpCard( int nServerID, CMessage *pMessage );
	
	// 购买家族红包的消息请求
	void OnMessageBuyFamilyItem( int nServerID, CMessage *pMessage );
	
	// 进入家族副本的消息请求
	void OnMessageEnterFamilyRepetion( int nServerID, CMessage *pMessage );
	
	// 清除家族副本信息
	void OnMessageClearFamilyRepetionInfo( CMessage *pMessage );
	
	// 获取家族商品列表的消息请求
	void OnMessageGetFamilyGoodsList( int nServerID, CMessage *pMessage );
	
	// 获取家族兑换商品列表的消息请求
	void OnMessageGetFamilyExchangeGoodsList( int nServerID, CMessage *pMessage );
	
	// 发送获取家族商品列表的消息回应
	void SendResponseGetFamilyGoodsList( int nServerID, int nErrcode, int nRoleID, int nEntityID, PBLeagueShopInfo &rLeagueShop, int nNpcTmpID );
		
	// 发送获取家族兑换商品列表的消息回应
	void SendResponseGetFamilyExchangeGoodsList( int nServerID, int nErrcode, int nRoleID, int nEntityID, PBLeagueShopInfo &rLeagueShop, int nNpcTmpID );
	
	// 发送家族商品列表发生变化的通知
	void SendNotifyChangeFamilyShopItem( int nFamilyID,  int nNpcTmpID, int nItemID, int nItemNum, int nComposeID = 0 );
	
	// 发送家族兑换商品列表发生变话的通知
	void SendNotifyChangeExchangeShopItem( int nFamilyID,  CGoodSInfo *pGoodInfo, int nGoodNum );

	// 发送使用家族特供物品的通知
	void SendNotifyUseFamilyLeagueItem( int nFamilyID,  CGoodSInfo *pGoodInfo, int nGoodNum, int nRoleID, const char *pName );
	
	// 进行家族兑换的消息请求
	void OnMessageFamilyExchange( int nServerID, CMessage *pMessage );
	
	// 发送家族兑换的消息回应
	void SendResponseFamilyExchange( int nServerID, int nErrcode, int nRoleID, int nComposeID, int nNpcTmpID, int nItemNum );
	
	// 增加家族兑换的消息通知
	void OnMessageAddFamilyExchangeItem( int nServerID, CMessage *pMessage );
public:
	// 发送邀请别人成功与否的通知
	void SendNotifyInviteResult( CGatePlayer *pPlayer, int nRoleID, int nResult, const char *pName );
	
	// ID_S2C_RESPONSE_CREATFAMILY 创建家族的回应 
	void SendResponseCreatFamily( int nServerID, int nRoleID, int nRet, int FamilyID, const char *pFamilyName,  int EntityID = 0 );

	// ID_S2C_RESPONSE_INVITEMEMBER	 邀请加入的回应
	void SendResponseIviteMember( int vServerID, int RoleID, const char *pName, int tRet, int EntityID = 0  );

	// ID_S2C_NOTIY_FAMILYINVITED	 被别人邀请的通知	 	
	void SendNotityFamilyInvited( CGatePlayer *pPlayer, int RoleID, int InviteID, const char *InviteName, int InviteFamilyID, const char *InviteFamilyName, int nMemberNum, int nGlory, int nPvpStar, int nNpcID, const char *pRooterName, int nFamilyLevel = 1, const char *pCorpsName = NULL, int nUionID = 0, const char *pUnionName = NULL );

	// ID_S2C_RESPONSE_REPLYFAMILYINVITE	答复家族邀请的回复 
	void SendResponseReplyFamilyInvite( int vServerID, int RoleID, int nRet, int nResult, const char *pInvitingRoleName, int nFamilyID, const char *pFamilyName, int EntityID = 0 , int nCorpsID = 0, const char *pCorpsName = NULL, int nFamilyHeader = 0, int nCorpsHeader = 0, int nUnionID = 0, const char *pUnionName = NULL );

	// ID_S2C_RESPONSE_FAMILYAPPLY	 申请加入家族的回应	
	void SendResponseFamilyApply( int vServerID, int RoleID, int Result, int FamilyID, const char * FamilyName, int EntityID = 0 );					

	// ID_S2C_NOTIFY_MEMBERAPPLY	发给家族管理者玩家请求加入家族的通知	
	void SendNotifyMemberApply( CGatePlayer *pPlayer, int RoleID, int ApplyID, int Level, int Metier, const char *pName, int nFaceType, int nSex, int MapID, int LineID );		

	// ID_S2C_RESPONSE_REPLYMEMBERAPPLY 回复玩家申请加入家族的回应
	void SendResponseReplyMemberApply( int vServerID, int RoleID, int Errcode, int nResult, int nCharID, const char *pApplyName, int EntityID = 0 );						

	// ID_S2C_NOTIFY_FAMILYAPPLYSUCESS	 通知玩家申请加入家族成功
	void SendNotifyFamilyApplySucess( CGatePlayer *pPlayer, int RoleID, int Result, int FamilyID, const char *pFamilyName, int nCorpsID = 0, const char *pCorpsName = NULL, int nFamilyHeader = 0, int nCorpsHeader = 0, int nUnionID = 0, const char *pUnionName = NULL );					

	// ID_S2C_NOTIFY_FAMILYMEMBERADDED		通知家族成员有新成员加入（邀请和申请通用）	
	void SendNotifyFamilyMemberAdded( int MemberRoleID, int nLevel, int nSex, int nFaceID, int nMetierID, const char *pName, const char *pManagerName, int *pRoleIDList, int RoleNum, int OnLine,  int nMapID = 0, int nLineID = 0 );

	// ID_S2C_RESPONSE_POSTAPPOINT		 家族管理者职位任免的回应
	void SendResponsePostAppoint( int vServerID, int RoldID, int Errcode, int MemberID, int Post,int EntityID = 0 );

	// ID_S2C_NOTIFY_POSTAPPOINT	 家族管理者任命职位的通知
	void SendNotifyPostAppoint( int ActionKind, int RoleID, const char *pManagerName, int *RoleIDList, int RoleNum, const char *pRoleName );

	// ID_S2C_RESPONSE_LEAVEFAMILY	家族成员离开家族的回应
	void SendResponseLeaveFamily( int vServerID, int RoleID, int Errcode, int EntityID = 0 );				

	// ID_S2C_NOTIFY_MEMBERLEAVEFAMILY	家族成员离开家族的通知	 
	void SendNotifyMemberLeaveFamily( int vLeaverID, const char *pLeaverName, int *pRoleIDList, int RoleNum );

	// ID_S2C_RESPONSE_GETFAMILYMEMBERINFO 获取家族成员信息的回应	
	void SendResponseGetFamilyMemberInfo( int vServerID, int RoleID, int *PFamilyList, int nFamilyNum, int EntityID = 0 );				

	// ID_S2C_RESPONSE_FAMILYPROPERTY	 获取家族信息的回应：家族属性信息
	void SendResponseFamilyProperty( int vServerID, int RoleID, PBFamilyPropery &pbFamilData, int EntityID = 0 );

	// ID_S2C_RESPONSE_GETALLFAMILYINFO	 获取所有家族基本信息的回复消息  
	void SendResponseGetAllFamilyInfo( int vServerID, int RoleID, CMessageGetAllFamilyInfoResponse &pbFamilyInfo, int nCurPage, int nTotalPage, int EntityID = 0 );

	// ID_S2C_RESPONSE_GETAPPLYLIST	发送申请列表	
	void SendResponseGetApplyList( int vServerID, int RoleID,  CMessageGetApplyListResponse &pbApplyList, int EntityID = 0 );

	// ID_S2C_RESPONSE_KICKMEMBER	 宗正踢人的回应
	void SendResponseKickMember( int vServerID, int RoleID, int KickedPlayerID, const char *pKickedName, int Errcode, int EntityID = 0 );

	// ID_S2C_NOTIFY_KICKPLAYER	 宗正踢人的通知
	void SendNotifyKickPlayer( const char *pName, int RoleID, int *pRoleIDList, int RoleNum, const char * pKickedName, int nKickedRoleID, const char *pManageName, int nManageID );

	// ID_S2C_RESPONSE_DISBANDFAMILY	解散家族的回应
	void SendResponseDisbandFamily( int vServerID, int RoleID, int Errcode, int EntityID = 0 );

	// ID_S2C_NOTIFY_DISBANDFAMILY  解散家族的通知
	void SendNotifyDisbandFamily( int FamilyID, int *pRoleIDList, int RoleNum );

	// ID_S2C_RESPONSE_CHANGENOTICE	改变家族公告的回应	
	void SendResponseChangeNotice( int vServerID, int RoleID, int Errcode, const char *pNotice, int EntityID = 0 );

	// ID_S2C_RESPONSE_APPOINTHEADER	 宗正转让职位的回应消息
	void SendResponseAppointHeader( int vServerID, int RoleID, int MemberID, const char *pRoleName, int Errcode, int EntityID = 0 );

	// ID_S2C_NOTIFY_CHANGERHEADER	宗正转让职位的通知	
	void SendNotifyChangeHeader( int ManagerID, const char *pNanagerName, int MemberID, const char *pMemberName, int *pRoleIDList, int RoleNum );

	// ID_S2C_NOTIFY_FAMILYPEROPERTY	家族属性信息通知
	void SendNotifyFamilyProperty( int nFamilyID );
	
	// 发送清除家族申请列表的回应消息
	void SendResponseClearFamilyApply( int nServerID, int nErrcode, int nRoleID, int nEntityID = 0 );
	
	// 发送家族加入军团的消息通知
	void SendNotifyJoinCorps( int nCorpsID, const char *pCorpsName, int *pRoleIDList, int nNum, int nJoinMethod = JOINCORPS_OTHERFAMILYCORPS, int nUnionID = 0, const char *pUnionName = NULL );
	
	// 发送取消军团解散的消息回复
	void SendResponseCancelFamilyDisband( int nServerID, int tErrcode, int nRoleID, int nEntityID = 0 );
	
	// 发送家族通告改变的通知
	void SendNotifyChangeFamilyNotice( const char *pNotice, int *pRoleIDList, int nRoleNum );
	
	// 发送刷新家族信息的回复
	void SendResponseRefreshFamilyInfo( int nServerID, int nRoleID, int nEntityID, PBFamilyMemberList &pbMemList );

	// 发送争夺NPC报名回应
	void SendBidNpcResponse( int vServerID, int vResult, int vNpcID, int vMoney, int vRoleID, int vEntityID = 0 );

	// 向族长发送报名通知
	void SendMemberSubmitNotify( int vServerID, int vNpcID, const char *vFamilyName, int vFirst, int vRoleID, int vEntityID = 0 );

	// 发送争夺NPC到时回应
	void SendBidTimeOutResponse( int vServerID, int vNpcID, int *vFamilyID, int *vMoney, int vAttID, int vDefID, int Num );

	// 发送族长报名回应
	void SendMemberSubmitResponse( int vServerID, int vResult, int vNpcID, int vRoleID, int vEntityID = 0 );

	// 发送战场开始回应
	void SendBattleStartResponse( int vServerID, int vNpcID, int *vFamilyID, int *vMoney, int vAttID, int vDefID, int Num );

	// 发送查询争夺回应
	void SendNpcInfoResponse( int vServerID, int vNpcID, int vCountryID, int vRoleID, int vEntityID = 0 );

	// 发送放弃NPC回应
	void SendAbortNpcResponse( int vServerID, int vResult, int vNpcID, int vRoleID, int vEntityID );

	// 发送放弃NPC通知
	void SendAbortNpcNotify( int vServerID, int vNpcID, char *vName, int vRoleID, int vEntityID );

	// 发送重新竞价消息
	void SendBidRestartNotify( int vServerID, int vNpcID );

	// 发送战场结果消息
	void SendBattleResultNotify( int vServerID, int vNpcID, int vAttFamilyID, int vDefFamilyID, int vWinSide );		

	// 发送战场即将开战消息
	void SendPreBattleNotify( int vServerID, int vNpcID, int vAttFamilyID, int vDefFamilyID, int vMoney );	

	// 发送开设房间回应
	void SendMakeRoomResponse( int vServerID, int vResult, int vMoney, char *vName, int vRoleID, int vEntityID );

	// 发送挑战房间回应
	void SendChaRoomResponse( int vServerID, int vResult, int vFamilyID, int vRoleID, int vEntityID );

	// 发送同意挑战回应
	void SendAgreeBattleResponse( int vServerID, int vResult, int vFamilyID, int vRoleID, int vEntityID );

	// 发送放弃房间回应
	void SendAbortRoomResponse( int vServerID, int vResult, int vRoleID, int vEntityID );

	// 发送放弃挑战回应
	void SendAbortChaResponse( int vServerID, int vResult, int vFamilyID, int vRoleID, int vEntityID );	

	// 发送开设房间通知
	void SendMakeRoomNotify( int vServerID, int vFamilyID );

	// 发送取消房间通知
	void SendAbortRoomNotify( int vServerID, int vFamilyID );

	// 发送挑战房间通知
	void SendChaRoomNotify( int vServerID, int vSrcFamilyID, int vDesFamilyID );

	// 发送取消房间通知
	void SendAbortChaNotify( int vServerID, int vSrcFamilyID, int vDesFamilyID );
	
	
	// 设置家族成员职位的消息消息回应
	void SendResponseSetFamilyMemberPost( int nServerID, int nErrcode, int nMemberID, int nPost, int nRoleID, int nEntityID = 0 );

	// 升级家族的回应
	void SendResponseLevelUPFamily( int nServerID, int nErrcode, int nLevel, int nRoleID, int nEntityID = 0 );

	// 捐献家族金钱的消息回应
	void SendResponseContributeFamilyMoney( int nServerID, int nErrcode, int nMoney, int nRoleID, int nEntityID = 0 );	

	// 从占领npc处领取家族金钱的消息回应
	void SendResponseObtainFamilyMoney( int nServerID );	

	// 设置家族职位的通知
	void SendNotifySetFamilyPost( int *pIDList, int nNum, int nManagerID, const char *pManagerName, int nMemberID, const char *pMemberName, int nOldPost, int nNewPost );	

	// 家族升级的通知	
	void SendNotifyLevelUPFamily( int *pIDList, int nNum, int nLevel, int nMoney, int nGlory, int nMaxMemberNum, int nMaxPresterNum, int nLevelMoney, int nLevelGlory, int nStability, int nReduceStability );

	// pvp星级升级的通知
	void SendNotifyPVPStarLevelUP( int *pIDList, int nNum, int nPvPLevel );	
	
	// 给家族成员发送消息
	void SendFamilyMsg( int nRoleID, CMessage *pMessage );
	
	// 给家族成员发送消息
	void SendFamilyMsgByID( int nFamilyID, CMessage *pMessage );
	
	// 发送家族通知
	void SendFamilyNotify( CMessage *pMessage, int *pRoleIDList, int vRoleNum );
	
	// 发送家族金钱改变的通知 
	void SendNotifyFamilyMoneyChanged( int nMoney, int nRoleID, int nFamilyID, int nReason, int tValue, const char *pName = NULL );	
	
	// 发送家族荣耀改变的通知
	void SendNotifyFamilyGloryChanged( int nGlory, int nRoleID, int nFamilyID, int nReason, int tValue, const char *pName = NULL );
	
	// 发送家族成员贡献度改变的通知	
	void SendNotifyFamilyContributeChanged( int nContribute, int nRoleID, int nFamilyID, int tValues );
	
	// 发送家族可以升级的通知
	void SendNotifyFamilyCanLevelUP( int nFamilyID );
	
	// 发送购买家族红包的消息请求
	void SendResponseBuyFamilyItem( int nServerID, int nRoleID, int nErrcode, int nRedPaperID, int nRedPaperNum, int nEntityID = 0  );
	
	// 组合消息
	void MakeMessage( CMessage &MsgHead, int RoleID, void *pMsg, int MsgID, int EntityID = 0 );	
	
	// 发送进入家族副本的消息回应
	void SendResponseEnterFamilyRepetion( CGatePlayer *pPlayer, int nErrcode, int nEntityID );	
		
	// 刷新家族状态
	void ReFreshFamilyStatus();

	// 改变家族国籍
	void ChangeFamilyCountry( int *pRoleIDList, int vRoleNum, int vCountryID );
	
	// 发送家族安定度改变的通知
	void SendNotifyFamilyStabilityChanged( int nStablity, int nRoleID, int nFamilyID, int nReason, int tValue, const char *pName = NULL, int nOnLineTime = 0 );
	
	// 改变家族安定度的消息通知
	void OnMessageChangeFamilyStability( CMessage *pMessage );	
	
	// 判断家族安定度是否过低
	void JudgeFamilyStability( CFamily *pFamily, CGatePlayer *pPlayer );
	
public:	  
	
	// 增加pvp积分的消息
	void AddPvPScore( int nFamily, int nScore );
	
	// 消耗家族金钱, Freeze表示是实际消耗还是冻结起来
	int ConsumeFamilyMoney( int nRoleID, int nMoney, bool bFreeze = false, int nReason = REASON_CHALLENGEWAR );
	
	// 消耗家族金钱
	int AddFamilyMoney( int nRoleID, int nMoney, const char *pName, bool bFreeze = false, int nReason = REASON_WARWIN );
	
public:						   	
	// 创建家族数据库信息的回应消息
	int OnCreatFamilyDBInfo( CMessage* tpMessage, int FamilyObjID, int Para );
	
	// 收到名称回应
	void OnAGIPSessionCreateGroup( SysProtocol* pproto, int nParam1, int nParam2);
	void OnAGIPTimeoutCreateGroup(int nParam1, int nParam2);
public:	
	// 初始化家族配置信息
	int InitializeFamilyconfig(  );

	virtual void ReloadCfg( );

	// 加载配置的回调函数
	static void LoadFamilyConfigCallback(const char* pcConfigPath);
	
private:
	CWTimer mUpdateTimer;	// 检查家族更新的时间间隔
	CWTimer mDisbandTimer;	// 检查家族解散的时间间隔
	bool	mIsLaunched;
	//CWTimer m

};

// #endif

