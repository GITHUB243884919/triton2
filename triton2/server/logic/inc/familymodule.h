#pragma once		
#include "familytype.h"	 
#include "servercore.h"
#include "servertool.h"
#include "lk_vector.h"
#include "lk_hashmap.h"
#include "corpstype.h"

#define   SenMsgToGate( MsgHead, FamilyMsg, EntityPlayer  )	 \
FamilyMsg->clear_headmsg();										\
FamilyMsg->mutable_headmsg()->set_roleid( EntityPlayer->GetCharID() );	\
FamilyMsg->mutable_headmsg()->set_entityid( EntityPlayer->GetEntityID() ); \
CSceneLogic::GetSingleton().Send2Gate( MsgHead ) ;

class CMemberInfo
{
public:
	bool	mIfHasFamilyApplies;	// 是否有申请信息
	bool	mIfHasCorpsApply;		// 是否有军团申请信息
	int		mFamilyContribute;
	int		mFamilyPost;
	int		mCorpsContribute;
	int		mCorpsPost;	 
	int		mExpGloryNeeded;		// 兑换家族荣耀的经验
	int		mApGloryNeeded;			// 兑换家族经验的功勋	
	int		mFamilyHeaderID;		// 家族首领ID	
	int		mCorpsHeaderID;			// 军团首领ID	
	int		mFamilyMoney;			// 家族金钱
	int		mCorpsMoney;			// 军团金钱
	int		mEntityID;				// 玩家的实体ID	
public:	
	CMemberInfo()
	{
		if ( CObj::msCreateMode )
		{
			Initailize( );
		}
		else
		{
			Resume();	
		}
	}
	~CMemberInfo(){ }
	
	int Initailize()
	{
		mFamilyContribute = 0;
		mCorpsPost = 0;
		mCorpsContribute = 0;
		mFamilyPost = 0;
		
		mCorpsPost =  CORPS_POST_MEMBER;	 
		mIfHasFamilyApplies = false;	
		mExpGloryNeeded = 0;
		mApGloryNeeded = 0;
		mFamilyHeaderID = 0;
		mCorpsHeaderID = 0;
		mIfHasCorpsApply = false;
		mFamilyMoney = 0;
		mCorpsMoney = 0;
		mEntityID = 0;
		return  SUCCESS;			
	}
	
	int Resume()
	{
		return SUCCESS;
	}
};

struct CFamilyTaskInfo 
{
	int mValue;
};

class  CFamilyInfo : public CObj
{
public:
	typedef lk::hash_map< int, CMemberInfo, MAX_FAMILYMEMBER_NUM > FamilyMemberList;		
public:
	FamilyMemberList	mMemberList;
	char				mFamilyName[MAX_FAMILYNAME_LENGTH*MAX_UTF8_BYTES];	
	int					mCorpsID;
	char				mCorpsName[MAX_FAMILYNAME_LENGTH*MAX_UTF8_BYTES];	 	
public:	
	CFamilyInfo()														    
	{	
		if ( CObj::msCreateMode )
		{
			Initialize();
		}
		else
		{
			Resume();	
		}	
	}	
	int Initialize();
	int Resume(){ return 0; }

	~CFamilyInfo(){ }
public:
	
	// 当家族成员进入场景时,加入列表
	int InsertMember( int nRoleID, CMemberInfo &rMemberInfo );
	
	// 玩家下线时删除玩家信息
	int DeleteMember( int nRoleID );
	
	// 判断成员列表中是否有某个玩家
	bool HasMember( int nRoleID );
	
	// 改变成员的信息
	int ChangeMemberInfo( int nRoleID, CMemberInfo &rMemberInfo );
	
	// 获取成员信息
	CMemberInfo *GetMemberInfo( int nRoleID );	
	
	// 获取本家族的玩家列表
	int GetMemberList( int *pIDList, int &rNum ); 
	
	// 设置军团ID
	void SetCorpsID( int nCorpsID ){ mCorpsID = nCorpsID;  }
	
	// 获取军团ID
	int GetCorpsID(  ){ return mCorpsID; }	
	
	// 设置家族名称
	void SetFamilyName( const char *pName, bool bRefresh = false );
	
	// 设置军团名称
	void SetCorpsName( const char *pName, bool bRefresh = false );
	
	// 获取家族名称
	const char *GetFamilyName(){ return mFamilyName; }
	
	// 获取军团名称
	const char *GetCorpsName( ){ return mCorpsName; }
	
	// 获取家族名称
	const char *GetFamilyNameShow(); 
	
	// 获取军团名称
	const char *GetCorpsNameShow();
	
	// 获取成员数量
	int GetMemberNum(){ return mMemberList.size(); }
};


class CFamilyModule : public CLogicModule, public CSingleton< CFamilyModule >
{
public:
	typedef lk::hash_map< int, lk::string<MAX_FAMILYNAME_LENGTH*MAX_UTF8_BYTES>, MAX_FAMILY_NUM > IDNameList;
	typedef lk::hash_map< int, int, 1000 > NPCList;
	typedef lk::hash_map< int, int, MAX_FAMILY_NUM > FamilyList;
	typedef lk::hash_map< int, CFamilyTaskInfo, MAX_FAMILYTASK_NUM > FamilyTaskList;
	static char	sModuleName[ NAME_LENGTH ];

	IDNameList	mNameList;
	NPCList		mNpcList;
	FamilyList	mFamilyList;
	FamilyTaskList mFamilyTask;
public:
	CFamilyModule():CLogicModule( sModuleName ){  }
	~CFamilyModule(){  }
	static unsigned int CountSize( );
public:	
	// 处理消息  CMessageCreatFamilyRequest
	void OnMessageCreatFamily( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 处理消息  CMessageInviteMemberRequest
	void OnMessageInviteMember( CEntityPlayer *pPlayer, CMessage *pMessage );	  
	
	// 处理消息 CMessageReplyFamilyInviteRequest
	void OnMessageReplyFamilyInvite( CEntityPlayer *pPlayer, CMessage *pMessage );	  
	
	// 处理消息 CMessageApplyFamilyRequest
	void OnMessageApplyFamily( CEntityPlayer *pPlayer, CMessage *pMessage );	  	
	
	// 处理消息CMessageReplyMemberApplyRequest   
	void OnMessageReplyMemberApply( CEntityPlayer *pPlayer, CMessage *pMessage );	
		
	// 处理消息 CMessagePostAppointRequest   
	void OnMessagePostAppoint( CEntityPlayer *pPlayer, CMessage *pMessage );	  	
	
	// 处理消息 CMessageLeaveFamilyRequest
	void OnMessageLeaveFamily( CEntityPlayer *pPlayer, CMessage *pMessage );	  	
	
	// 处理消息 CMessageGetFamilyMemberInfoRequest		 
	void OnMessageGetFamilyMemberInfo( CEntityPlayer *pPlayer, CMessage *pMessage );	  	
	
	// 处理消息 CMessageGetFamilyPropertyRequest
	void OnMessageGetFamilyPropetyInfo( CEntityPlayer *pPlayer, CMessage *pMessage );	  	
	
	// 处理消息 CMessageGetAllFamilyInfoResponse
	void OnMessageGetAllFamilyInfo( CEntityPlayer *pPlayer, CMessage *pMessage );	  	
	
	// 处理消息 CMessageGetApplyListRequest
	void OnMessageGetApplyList( CEntityPlayer *pPlayer, CMessage *pMessage );	  	
	
	// 处理消息	CMessageKickPlayerRequest
	void OnMessageKickPlayer( CEntityPlayer *pPlayer, CMessage *pMessage );	  	
	
	// 处理消息 CMessageDisbandFamilyRequest
	void OnMessageDisbandFamily( CEntityPlayer *pPlayer, CMessage *pMessage );	  	
	
	// 处理消息 CMessageAppointHeaderRequest
	void OnMessageAppointHeader( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 处理消息 CMessageChangeFamilyNoticeRequest
	void OnMessageChangeFamilyNotice( CEntityPlayer *pPlayer, CMessage *pMessage );	
		
	// 处理消息 CMessageClearFamilyApplyRequest
	void OnMessageClearFamilyApply( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 处理消息 CMessageCancelFamilyDisbandRequest
	void OnMessageCancelDisbandFamily( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 处理消息 CMessageRefreshFamilyInfoRequest
	void OnMessageRefreshFamilyInfo( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 处理争夺NPC报名消息
	void OnMessageBidNpc( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 处理族长报名进入消息
	void OnMessageMemberSubmit( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 处理查询NPC争夺
	void OnMessageBidInfo( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 处理NPC放弃消息
	void OnMessageAbortNpc( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 处理名字ID消息
	void OnMessageIDNameNotify( CMessage *pMessage );

	// 处理NPC占领消息
	void OnMessageNpcListNotify( CMessage *pMessage );	

	// 处理重新竞价通知
	void OnMessageBidRestartNotify( CMessage *pMessage );

	// 处理开设房间消息
	void OnMessageMakeRoom( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 处理挑战房间消息
	void OnMessageChaRoom( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 处理同意挑战消息
	void OnMessageAgreeBattle( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 处理查询房间列表消息
	void OnMessageQueryAllRoom( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 处理查询自己的房间消息
	void OnMessageQueryMyRoom( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 处理查询自己的挑战消息
	void OnMessageQueryMyCha( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 处理放弃房间消息
	void OnMessageAbortRoom( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 处理放弃挑战消息
	void OnMessageAbortCha( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 通过ID获得名字
	char* GetFamilyName( int vFamilyID );

	// 获得占领NPC的家族ID
	int GetFamilyIDByNpcID( int vNpcID );
	
	
	// 设置家族成员职位的消息请求
	void OnMessageSetFamilyMemberPost( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 升级家族的请求
	void OnMessageLevelUPFamily( CEntityPlayer *pPlayer, CMessage *pMessage );	

	// 捐献家族金钱的消息请求
	void OnMessageContributeFamilyMoney( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 开启家族boss的通知
	void OnMessageStartFamilyBoss( CEntityPlayer *pPlayer, CMessage *pMessage );	
	
	// 领取家族经验令牌的消息请求
	void OnMessageGetFamilyExpCard( CEntityPlayer *pPlayer, CMessage *pMessage );	
	
	// 玩家进入家族副本的消息请求
	void OnMessageEnterFamilyRepetion( CEntityPlayer *pPlayer, CMessage *pMessage );	
	
	// 获取家族出售列表的消息请求
	void OnMessageGetFamilyGoodsList( CEntityPlayer *pPlayer, CMessage *pMessage );	
	
	// 获取家族兑换物品列表的消息请求
	void OnMessageGetFamilyExchangeGoodsList( CEntityPlayer *pPlayer, CMessage *pMessage );	
	
	// 获取家族出售列表的网关返回消息
	void OnGateMsgGetFamilyGoodsListResponse( CEntityPlayer *pPlayer, CMessage *pMessage );	
	
	// 获取家族兑换物品列表的网关返回消息
	void OnGateMsgGetFamilyExchangeGoodsListResponse( CEntityPlayer *pPlayer, CMessage *pMessage );	
	
	// 家族兑换
	void FamilyExchange( CEntityPlayer *pPlayer, int nComposeID, int nEntityID, int nItemNum );
	
	// 家族兑换的回应消息
	void OnGateResponseFamilyExchange( CMessage *pMessage );
	
	// 增加家族兑换物品的接口
	int AddFamilyExchangeItem( CEntityPlayer *pPlayer, CTplLeagueBag *tpTplLeagueBag );
		
	public:
	// 创建家族
	void CreatFamily(  CEntityPlayer *pPlayer, const char *pFamilyName );	  	
	// 邀请成员
	void InviteFamilyMember( CEntityPlayer *pPlayer, int vEntityID, const char *pName = NULL );
	// 答复邀请
	void ReplyFamilyInvite( CEntityPlayer *pPlayer, int Result, int FamilyID );
	// 申请加入家族
	void ApplyFamily( CEntityPlayer *pPlayer, int vEntityID, int FamilyID = 0 );
	// 回复玩家申请
	void ReplyMemberApply( CEntityPlayer *pPlayer, int Result, int PlayerCharID );
	// 职位任免
	void AppiontPost(  CEntityPlayer *pPlayer, int PlayerCharID, int ActionKind );
	// 离开家族
	void LeaveFamily( CEntityPlayer *pPlayer );
	// 获取家族申请列表
	CMemberInfo* GetFamilyMemberInfo( CEntityPlayer *pPlayer );	
	// 获取家族属性信息
	void GetFamilyPropertyInfo( CEntityPlayer *pPlayer );	
	// 获取所有家族的基本信息
	void GetAllFamilyInfo( CEntityPlayer *pPlayer ); 
	// 获取家族的申请列表
	void GetApplyList( CEntityPlayer *pPlayer );
	// 踢除家族成员
	void KickFamilyMember( CEntityPlayer *pPlayer, int PlayerCharID );
	// 解散家族
	void DisbandFamily( CEntityPlayer *pPlayer );
	// 转让宗正职位
	void AppointHeader( CEntityPlayer *pPlayer, int PlayerCharID );
	// 改变家族公告
	void ChangeFamilyNotice( CEntityPlayer *pPlayer, const char *pNotice );
	// 检查npc是否有家族的相关服务
	bool CheckNpcFamilyService( CEntityPlayer *pPlayer, int nNpcEntityID, int eService );
public:

	// ID_S2C_RESPONSE_CREATFAMILY 创建家族的回应 
	void OnGSCreatFamilyResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// ID_S2C_RESPONSE_INVITEMEMBER	 邀请加入的回应
	void OnGSIviteMemberResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// ID_S2C_NOTIY_FAMILYINVITED	 被别人邀请的通知	 	
	void OnGSFamilyInvitedNotify( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// ID_S2C_RESPONSE_REPLYFAMILYINVITE	答复家族邀请的回复 
	void OnGSReplyFamilyInviteResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
		
	// ID_S2C_RESPONSE_FAMILYAPPLY	 申请加入家族的回应	
	void OnGSFamilyApplyResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// ID_S2C_NOTIFY_MEMBERAPPLY	发给家族管理者玩家请求加入家族的通知	
	void OnGSMemberApplyNotify( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// ID_S2C_RESPONSE_REPLYMEMBERAPPLY 回复玩家申请加入家族的回应
	void OnOnGSReplyMemberApplyResponse( CEntityPlayer *pPlayer, CMessage *pMessage );					
	
	// ID_S2C_NOTIFY_FAMILYAPPLYSUCESS	 通知玩家申请加入家族成功
	void OnGSFamilyApplySucessNotify( CEntityPlayer *pPlayer, CMessage *pMessage );					
	
	// ID_S2C_NOTIFY_FAMILYMEMBERADDED		通知家族成员有新成员加入（邀请和申请通用）	
	void OnGSFamilyMemberAddedNotify( CEntityPlayer *pPlayer, CMessage *pMessage );			
	
	// ID_S2C_RESPONSE_POSTAPPOINT		 家族管理者职位任免的回应
	void OnGSPostAppointResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// ID_S2C_NOTIFY_POSTAPPOINT	 家族管理者任命职位的通知
	void OnGSPostAppointNotify( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// ID_S2C_RESPONSE_LEAVEFAMILY	家族成员离开家族的回应
	void OnGSLeaveFamilyResponse( CEntityPlayer *pPlayer, CMessage *pMessage );		
	
	// ID_S2C_NOTIFY_MEMBERLEAVEFAMILY	家族成员离开家族的通知	 
	void OnGSMemberLeaveFamilyNotify( CEntityPlayer *pPlayer, CMessage *pMessage );
		
	// ID_S2C_RESPONSE_GETFAMILYMEMBERINFO 获取家族成员信息的回应	
	void OnGSGetFamilyMemberInfoResponse( CEntityPlayer *pPlayer, CMessage *pMessage );			
	
	// ID_S2C_RESPONSE_FAMILYPROPERTY	 获取家族信息的回应：家族属性信息
	void OnGSFamilyPropertyResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
											 				
	// ID_S2C_RESPONSE_GETALLFAMILYINFO	 获取所有家族基本信息的回复消息  
	void OnGSGetAllFamilyInfoResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
		
	// ID_S2C_RESPONSE_GETAPPLYLIST	发送申请列表	
	void OnGSGetApplyListResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// ID_S2C_RESPONSE_KICKMEMBER	 宗正踢人的回应
	void OnGSKickMemberResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// ID_S2C_NOTIFY_KICKPLAYER	 宗正踢人的通知
	void OnGSKickPlayerNotify( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// ID_S2C_RESPONSE_DISBANDFAMILY	解散家族的回应
	void OnGSDisbandFamilyResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
		
	// ID_S2C_NOTIFY_DISBANDFAMILY  解散家族的通知
	void OnGSDisbandFamilyNotify( CEntityPlayer *pPlayer, CMessage *pMessage );
		
	// ID_S2C_RESPONSE_CHANGENOTICE	改变家族公告的回应	
	void OnGSChangeNoticeResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// ID_S2C_RESPONSE_APPOINTHEADER	 宗正转让职位的回应消息
	void OnGSAppointHeaderResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// ID_S2C_NOTIFY_CHANGERHEADER	宗正转让职位的通知	
	void OnGSChangeHeaderNotify( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// ID_S2C_NOTIFY_FAMILYPEROPERTY	家族属性信息通知
	void OnGSFamilyPropertyNotify( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// ID_S2C_NOTIFY_INVITERESULT 邀请的结果被拒绝还是其他原因
	void OnGSInviteMemberResultNotify( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	//	取消解散家族的消息回应
	void OnGSCancelDisbandCorpsResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 清除家族申请列表的消息回应
	void OnGSClearFamilyApplyListResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 改变家族公告的通知
	void OnGSChangeFamilyNoticeNotify( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 刷新家族成员的消息回应
	void OnGSRefreshFamilyInfoResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 家族加入军团的通知
	void OnGSJoinCorpsNotify( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 竞价争夺NPC回应
	void OnGSBidNpcResponse( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 报名争夺NPC通知
	void OnGSMemberSubmitNotify( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 报名争夺NPC回应
	void OnGSMemberSubmitResponse( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 放弃NPC回应
	void OnGSAbortNpcResponse( CEntityPlayer *pPlayer, CMessage *pMessage ); 	


	// 向网关发送战斗结束通知
	void SendBattleEndNotify( KEY_TYPE vKey, int vWinSide );

	// 查询NPC争夺回应
	void OnGSBidInfoResponse( CEntityPlayer *pPlayer, CMessage *pMessage );	

	// 开设房间回应
	void OnGSMakeRoomResponse( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 挑战房间回应
	void OnGSChaRoomResponse( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 同意挑战回应
	void OnGSAgreeBattleResponse( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 查询房间列表回应
	void OnGSQueryAllRoomResponse( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 查询自己的房间回应
	void OnGSQueryMyRoomResponse( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 查询自己的挑战回应
	void OnGSQueryMyChaResponse( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 放弃房间回应
	void OnGSAbortRoomResponse( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 放弃挑战回应
	void OnGSAbortChaResponse( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 收到挑战通知
	void OnGSFamilyChaNotify( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 获取获取家族内丹的回应消息
	void OnGateMsgGetExpCard( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 玩家贡献度改变的消息
	void OnGateNotifyFamilyContributeChanged( CEntityPlayer *pPlayer, CMessage *pMessage );
			
	// 接收来自网关的消息
	void OnGateMsg( CMessage *pMessage );
	
	// 给网关发送家族消息
	void SendToGate( void *pMsg, int MsgID, CEntityPlayer *pPlayer );
	
	// 给客户端发送回应消息
	void SendToClient( CEntityPlayer *pPlayer, void *pMsg, int MsgID );	
	

	// 设置家族成员职位的消息消息回应
	void OnGateMsgSetFamilyMemberPostResponse( CEntityPlayer *pPlayer, CMessage *pMessage );	
		
	// 升级家族的回应
	void OnGateMsgLevelUPFamilyResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 捐献家族金钱的消息回应
	void OnGateMsgContributeFamilyMoneyResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 从占领npc处领取家族金钱的消息回应
	void OnGateMsgObtainFamilyMoneyResponse( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 设置家族职位的通知
	void OnGateMsgSetFamilyPostNotify( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 家族升级的通知	
	void OnGateMsgLevelUPFamilyNotify( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// pvp星级升级的通知
	void OnGateMsgPVPStarLevelUPNotify( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 开启家族boss活动的回应消息
	void OnGateMsgStartFamilyBossResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 查询家族npc福利
	void OnGateMsgQueryNpcWelfareResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 获取家族金钱
	void OnGateMsgGetFamilyMoney( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 增加玩家的贡献度
	void AddPlayerFamilyContribute( int nRoleID, int nContribute, int nAddType = 0 );
	
	// 改变家族金钱
	void ChangeFamilyMoney( CEntityPlayer *pPlayer, int nMoney, bool bPayOut = false, int nReason = REASON_TASK );
	
	// 改变家族荣耀值
	void ChangeFamilyGlory( CEntityPlayer *pPlayer, int nValue, int bReduced = false, int nReason = REASON_TASK );
	
	// 改变家族安定度
	void ChangeFamilyStability( CEntityPlayer *pPlayer, int nValue, int nReason = REASON_TASK, int nParam1 = 0, int nParam2 = 0 );
	
	// 查询家族福利
	void OnMessageQueryFamilyWelfare( CEntityPlayer *pPlayer, CMessage *pMessage );		

	// 从占领npc处领取家族金钱的消息请求
	void OnMessageObtainFamilyMoney( CEntityPlayer *pPlayer, CMessage *pMessage );		
	
	// 获取家族金钱到个人
	void OnMessageGetFamilyMoney( CEntityPlayer *pPlayer, CMessage *pMessage );	
	
	// 购买家族红包的消息回应
	void OnGateResponseBuyFamilyItem( CMessage *pMessage );	
	
	// 购买家族物品的接口
	void BuyFamilyItem( CEntityPlayer *pPlayer, int nItemID, int nItemNum, int nNpcTmpID );	
	
	// 进入家族副本的网关回应
	void OnGateMsgEnterFamilyRepetion( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 清除家族副本信息
	void ClearFamilyRepetionInfo( int nFamilyID, int nRepetionLevel, int nOgreNum, int nTimeLength , int nBigWaveNum, int nSamllWaveNum, int nMaxHitNum, int nScore );
	
	// 获取任务的家族安定度奖励
	void OnTaksChangeStability( int nTaskID, CEntityPlayer *pPlayer );
public:
	// 插入一个玩家的信息
	int InsertFamilyMember( int nCorpsID, int nFamilyID, int nRoleID, CMemberInfo &rMemberInfo,  const char *pFamilyName = NULL, const char *pCorpsName = NULL );
	
	// 删除一个玩家的信息
	int DeleteFamilyMember( int nFamilyID, int nRoleID );
	
	// 获取某个家族的玩家信息列表
	int GetFamilyMemberList( int nFamilyID, int *pIDList, int &rNum );
	
	// 删除家族信息( 解散家族的时候 )
	int DeleteFamilyInfo( int nFamilyID );
	
	// 获取本场景服务器的某个军团的成员列表
	int GetCorpsMemberList( int nCorpsID, int *pIDList, int &rNum );
	
	// 获取家族在当前场景服务器的所有玩家的平均等级
	int GetFamilyAverageLevel( int nFamilyID, int &rLevel );
	
	// 获取家族名称,必须有家族成员在本场景时才有效
	const char *GetFamilyNameByID( int nFamilyID );
	
	// 获取军团名称,必须有军团成员在本场景时才有效
	const char *GetCorspNameByID( int nFamilyID, int nCorpsID );
	
	// 获取家族成员的个人信息
	CMemberInfo *GetFamilyMemberInfo( int nFamilyID, int nRoleID );
	
	// 获取家族信息
	CFamilyInfo *GetFamilyInfo( int nFamilyID );
	
	// 判断玩家是否是家族首领
	bool HasFamilyRight( CEntityPlayer *pPlayer, int nRight );
	
	// 判断是否是军团首领
	bool HasCorpsRight( CEntityPlayer *pPlayer, int nRight );
	
	// 初始化家族配置
	int InitialFamilyConfig();
	
	// 判断是否有某个任务
	bool IsFamilyTask( int nTaskID );

public:	
	// 启动服务器
	virtual void OnLaunchServer( );

	// 退出服务器
	virtual void OnExitServer( );

	// 服务器Timer
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffsekt );

	// 建立场景
	virtual int OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName );

	// 路由消息
	virtual void OnRouterMessage( CMessage* pMessage );

	// 客户端发送消息
	virtual void OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 建立实体
	virtual void OnCreateEntity( CEntity* pEntity , CCreator* pCreator, bool vFirst );

	// 销毁实体
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );

	// 存储数据
	virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode );

	// 加载配置
	virtual void OnLoadConfig( );	

	// 超时消息
	virtual void OnTimeoutMessage( CMessage* pMessage );

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( ) { return true; }

	// 加载家族配置的回调函数
	static void LoadFamilyConfigCallback(const char* pcConfigPath);

};
