#pragma once			 

#include "servercore.h"
#include "servertool.h"
#include "property.h"
#include "familytype.h"
#include "corpstype.h"
#include "servermessage_pb.hxx.pb.h"

#define   SendCorpsMsgToGate( MsgHead, CorpsMsg, EntityPlayer  )	\
	CorpsMsg->clear_corphead();										\
	CorpsMsg->mutable_corphead()->set_roleid( EntityPlayer->GetCharID() );	\
	CorpsMsg->mutable_corphead()->set_entityid( EntityPlayer->GetEntityID() ); \
	CSceneLogic::GetSingleton().Send2Gate( MsgHead ) ;				   \

struct CorpsNpcInfo
{
	int mCorpsID;
	char mCorpsName[ MAX_CORPSNAME_LENGTH*MAX_UTF8_BYTES ];
};

class CCorpsModule : public CLogicModule, public CSingleton< CCorpsModule >
{	 
public:
	static char	sModuleName[ NAME_LENGTH ];

	typedef lk::hash_map< int , CorpsNpcInfo, MAX_CORPSNUM > CorpsNpcList;
	CorpsNpcList	mNpcList;

	bool			mNpcDataFlag;	// npc数据是否与网关同步
public:
	CCorpsModule( ):CLogicModule( sModuleName )
	{ 
		mNpcList.initailize();
		mNpcDataFlag = false;
	}	
	~CCorpsModule( ){   } 
	static unsigned int CountSize(  );
public:		
	// 网管消息
	void OnGateMessage( CMessage *pMsg );
	
	// 创建军团的请求
	void OnMessageCreatCorps( CEntityPlayer *pPlayer, CMessage *pMessage );	
	
	// 申请加入军团的请求消息
	void OnMessageApplyCorps( CEntityPlayer *pPlayer, CMessage *pMessage );		
	
	// 回复家族申请加入军团的消息请求
	void OnMessageReplyCorpsMemberApply( CEntityPlayer *pPlayer, CMessage *pMessage );		

	// 邀请家族加入军团的消息请求
	void OnMessageInviteCorpsMember( CEntityPlayer *pPlayer, CMessage *pMessage );			
	
	// 答复军团邀请的消息请求
	void OnMessageReplyCorpsInvite( CEntityPlayer *pPlayer, CMessage *pMessage );					

	// 家族离开军团的消息请求
	void OnMessageFamilyLeaveCorps( CEntityPlayer *pPlayer, CMessage *pMessage );						
	
	// 从军团踢出家族的消息请求
	void OnMessageKickFamily( CEntityPlayer *pPlayer, CMessage *pMessage );							

	// 获取军团家族成员信息的请求
	void OnMessageGetCorpsFamilyInfo( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 获取军团属性信息的请求消息
	void OnMessageGetCorpsPropertyInfo( CEntityPlayer *pPlayer, CMessage *pMessage );	

	// 获取军团中某个家族的成员信息的消息请求
	void OnMessageGetCorpsFamilyMember( CEntityPlayer *pPlayer, CMessage *pMessage );	

	// 改变军团公告的消息请求
	void OnMessageChangeCorpsBulletin( CEntityPlayer *pPlayer, CMessage *pMessage );	

	// 改变军团首领的消息请求
	void OnMessageChangeCorpsHeader( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 获取军团的申请列表
	void OnMessageCorpsApplyList( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 解散家族的消息请求
	void OnMessageDisbandCorps( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 取消家族解散的消息
	void OnMessageCancelDisbandCorps( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 清除军团申请列表的消息
	void OnMessageClearCorpsApply( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 刷新军团信息的消息
	void OnMessageRefreshCorpsInfo( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 获取军团信息列表
	void OnMessageGetCorpsList( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 创建联军的请求
	void OnMessageCreateUnion( CEntityPlayer *pPlayer, CMessage *pMessage );	

	// 申请加入联军的请求消息
	void OnMessageApplyUnion( CEntityPlayer *pPlayer, CMessage *pMessage );		

	// 回复军团申请加入联军的消息请求
	void OnMessageReplyUnionMemberApply( CEntityPlayer *pPlayer, CMessage *pMessage );		

	// 邀请军团加入联军的消息请求
	void OnMessageInviteUnionMember( CEntityPlayer *pPlayer, CMessage *pMessage );			

	// 答复联军邀请的消息请求
	void OnMessageReplyUnionInvite( CEntityPlayer *pPlayer, CMessage *pMessage );					

	// 军团离开联军的消息请求
	void OnMessageCorpsLeaveUnion( CEntityPlayer *pPlayer, CMessage *pMessage );						

	// 从联军踢出军团的消息请求
	void OnMessageKickCorps( CEntityPlayer *pPlayer, CMessage *pMessage );	

	// 创建联军的请求
	void OnMessageMakeUnion( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 获得列表的请求
	void OnMessageGetUnionInfo( CEntityPlayer *pPlayer, CMessage *pMessage );	

	// 解散联军的请求
	void OnMessageDismissUnion( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 获得联军申请列表的请求
	void OnMessageUnionApplyList( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 获得联军列表的请求
	void OnMessageAllUnionList( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 军团联盟改名
	void OnMessageChangeUnionName( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	
	// 设置军团成员职位的消息请求 
	void OnMessageSetCorpsPost( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 升级军团的消息请求
	void OnMessageLevelUPCorps( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 贡献军团金钱的消息请求
	void OnMessageContributeCorpsMoney( CEntityPlayer *pPlayer, CMessage *pMessage );

	//  家族给军团贡献金钱的消息请求
	void OnMessageFamilyContributeMoney( CEntityPlayer *pPlayer, CMessage *pMessage );	

	// 设置军团成员称号的消息请求
	void OnMessageSetMemberName( CEntityPlayer *pPlayer, CMessage *pMessage );		

	// 获取军团战竞价排行榜信息
	void OnMessageGetCorpsBattleBidRank( CEntityPlayer *pPlayer, CMessage* pMessage );

	// 竞价报名
	void OnMessageCorpsBattleBidRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 选择参战家族
	void OnMessageSelectBattleFamilyRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 选择参战族人
	void OnMessageSelectBattleMemberRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 修理圣火台
	void OnMessageMendShengHuoTaiRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 挑战赛请求消息
	void OnMessageCorpsChallegeRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 房间请求
	void OnMessageOperateRoomRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 挑战请求
	void OnMessageChallegeRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 回应挑战请求
	void OnMessageReplyChallegeRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 查询房间列表请求
	void OnMessageQueryRoomListRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 查询挑战列表请求
	void OnMessageQueryChallegeListRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 查询军团npc福利的消息请求
	void OnMessageQueryNpcWelfareRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 获取军团npc福利的消息请求
	void OnMessageObtainNpcWelfareRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 获取军团金钱的消息请求
	void OnMessageGetCorpsMoneyRequest( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 获取军团商品列表的信息请求
	void OnMessageGetCorpsGoodsList( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 获取军团兑换商品列表的信息请求
	void OnMessageGetCorpsExchangeGoodsList( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 获取军团商品列表的信息请求
	void OnGateMessageGetCorpsGoodsList( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 获取军团兑换商品列表的信息请求
	void OnGateMessageGetCorpsExchangeGoodsList( CEntityPlayer* pPlayer, CMessage* pMessage );	
	
	// 军团兑换
	void CorpsExchange( CEntityPlayer *pPlayer, int nComposeID, int nEntityID, int nItemNum );
	
	// 军团兑换的回应消息
	void OnGateResponseCorpsExchange( CMessage *pMsg );
	
	// 增加军团兑换物品
	int AddCorpsExchangeItem( CEntityPlayer *pPlayer, CTplLeagueBag *tpTplLeagueBag );
	
public:	
	// 创建军团的回应消息
	void OnGSCreatCorpsResponse( CEntityPlayer *pPlayer, CMessage *pMessage );	
	
	// 申请加入军团的回应消息
	void OnGSApplyCorpsResponse( CEntityPlayer *pPlayer, CMessage *pMessage );	

	// 有家族申请加入军团的消息通知
	void OnGSCorpsMemberApplyNotify( CEntityPlayer *pPlayer, CMessage *pMessage );		
	
	// 回复家族申请加入军团的消息回应
	void OnGSReplyMemberApplyResponse( CEntityPlayer *pPlayer, CMessage *pMessage );	
	
	// 申请加入军团的结果通知
	void OnGSApplyCorpsResultNotify( CEntityPlayer *pPlayer, CMessage *pMessage );	
		
	// 邀请家族加入军团的消息回应
	void OnGSInviteCorpsMemberResponse( CEntityPlayer *pPlayer, CMessage *pMessage );	 
		
	// 邀请加入军团的消息通知
	void OnGSCorpsInviteNotify( CEntityPlayer *pPlayer, CMessage *pMessage );	
		
	// 答复军团邀请的消息回复
	void OnGSReplyCorpsInviteResponse( CEntityPlayer *pPlayer, CMessage *pMessage );		
		
	// 邀请家族加入军团的结果通知
	void OnGSCorpsInviteResultNotify( CEntityPlayer *pPlayer, CMessage *pMessage );		
		
	// 有家族加入军团的消息通知
	void OnGSFamilyJoinCorpsNotify( CEntityPlayer *pPlayer, CMessage *pMessage );			
		
	// 家族离开军团的消息回应
	void OnGSFamilyLeaveCorpsResponse( CEntityPlayer *pPlayer, CMessage *pMessage );			

	// 家族离开军团的消息通知
	void OnGSFamilyLeaveCorpsNotify( CEntityPlayer *pPlayer, CMessage *pMessage );
		
	// 从军团踢出家族的消息回应
	void OnGSKickFamilyResponse( CEntityPlayer *pPlayer, CMessage *pMessage );		
	
	// 家族从军团里面被踢掉的通知	
	void OnGSFamilyKickedNotify( CEntityPlayer *pPlayer, CMessage *pMessage );		
		
	// 获取军团家族成员信息的回应消息
	void OnGSGetCorpsFamilyInfoResponse( CEntityPlayer *pPlayer, CMessage *pMessage );		
		
	// 获取军团属性信息的回应消息
	void OnGSGetCorpsPropertyInfoResponse( CEntityPlayer *pPlayer, CMessage *pMessage );	
			
	// 获取军团中某个家族的成员信息的消息回应
	void OnGSGetCorpsFamilyMemberResponse( CEntityPlayer *pPlayer, CMessage *pMessage );		
		
	// 改变军团公告的消息回复
	void OnGSChangeCorpsBulletinResponse( CEntityPlayer *pPlayer, CMessage *pMessage );			

	// 改变军团公告的消息通知
	void OnGSChangeCorpsBulletinNotify( CEntityPlayer *pPlayer, CMessage *pMessage );		
																								
	// 改变军团首领的消息回复
	void OnGSChangeCorpsHeaderResponse( CEntityPlayer *pPlayer, CMessage *pMessage );		
	
	// 改变军团首领的消息通知							
	void OnGSChangeCorpsHeaderNotify( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 获取军团的申请列表的消息回应
	void OnGSGetCorpsApplyListResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 解散家族的消息回应
	void OnGSDisbandCorpsResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 解散家族的消息通知
	void OnGSDisbandCorpsNotify( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 取消解散家族的消息回应
	void OnGSCancelDisbandCorpsResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 刷新军团信息的消息回应
	void OnGSRefreshCorpsInfoResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 清除军团申请列表的消息请求
	void OnGSClearCorpsApplyResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 获取军团信息列表的消息请求
	void OnGSGetCorpsListResponse( CEntityPlayer *pPlayer, CMessage *pMessage );  

	// 军团联盟改名
	void OnGSChangeUnionNameResponse( CEntityPlayer *pPlayer, CMessage *pMessage ); 
	void OnGSChangeUnionNameNotify( CEntityPlayer *pPlayer, CMessage *pMessage ); 
	void OnGateMessageSummonCorpsBossResponse( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	
	
	// 设置军团成员职位的消息回应	
	void OnGSSetCorpMemberPostResponse( CEntityPlayer *pPlayer, CMessage *pMessage );  
	
	// 升级军团的消息回应
	void OnGSLevelUPCorpsResponse( CEntityPlayer *pPlayer, CMessage *pMessage );  
	
	// 贡献军团金钱的消息回应
	void OnGSContributeCorpsMoneyResponse( CEntityPlayer *pPlayer, CMessage *pMessage );   
		
	// 家族给军团贡献金钱的消息回应
	void OnGSFamilyContributeMoneyResponse( CEntityPlayer *pPlayer, CMessage *pMessage );  
	
	// 设置军团成员称号的消息回应
	void OnGSSetMemberNameResponse( CEntityPlayer *pPlayer, CMessage *pMessage );  

	// 设置军团职位的通知
	void OnGSSetCorpsPostNotify( CEntityPlayer *pPlayer, CMessage *pMessage );  

	// 军团升级的通知
	void OnGSCorpsLevelUPNotify( CEntityPlayer *pPlayer, CMessage *pMessage );  

	// 军团pvp等级提升的通知	
	void OnGSCorpsPVPStarLevelUPNotify( CEntityPlayer *pPlayer, CMessage *pMessage );  
	
	// 查询军团npc福利的消息回应
	void OnGateMsgQueryNpcWelfareResponse( CEntityPlayer *pPlayer, CMessage *pMessage );  
	
	// 领取军团npc福利的消息回应
	void OnGateMsgObtainNpcWelfareResponse( CEntityPlayer *pPlayer, CMessage *pMessage );  
	
	// 获取军团金钱的消息回应
	void OnGateMsgGetCorpsMoneyResponse( CEntityPlayer *pPlayer, CMessage *pMessage );  

	void OnGSQueryBidRankResponse( CEntityPlayer* pPlayer, CMessage* pMessage );

	void OnGSCorpsBidResponse( CEntityPlayer* pPlayer, CMessage* pMessage );

	void OnGSCorpsJoinBattleNotice( CMessage* pMessage );

	void OnGSSelectBattleFamilyResponse( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnGSSelectBattleMemberResponse( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnGSCorpsBattleRightsNotify( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnGSFamilyBattleRightsNotify( CEntityPlayer* pPlayer, CMessage* pMessage );
	void OnGSMemberBattleRightsNotify( CEntityPlayer* pPlayer, CMessage* pMessage );

	void OnGSSendBattleFlagOwnerNoitce( CMessage* pMessage );

	void OnGSCorpsChallgeReponse( CEntityPlayer* pPlayer, CMessage* pMessage );

	void OnGSCorpsBattleResultNotice( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 改变军团成员贡献度的通知
	void OnGateNotifyChangeCorpsContribute( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 改变军团采集活跃度的通知
	void OnGateNotifyChangeCorpsCollectActive( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 购买军团红包的消息回应
	void OnGateResponseBuyCorpsItem( CMessage* pMessage );
	
	// 军团称号改变的消息通知
	void OnGateMsgCorpsTitleChangedNotify( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 创建联军的回应消息
	void OnGSCreateUnionResponse( CEntityPlayer *pPlayer, CMessage *pMessage );	

	// 创建联军的回应通知
	void OnGSCreateUnionNotify( CEntityPlayer *pPlayer, CMessage *pMessage );	

	// 申请加入联军的回应消息
	void OnGSApplyUnionResponse( CEntityPlayer *pPlayer, CMessage *pMessage );	

	// 有军团申请加入联军的消息通知
	void OnGSUnionApplyNotify( CEntityPlayer *pPlayer, CMessage *pMessage );		

	// 回复军团申请加入联军的消息回应
	void OnGSReplyUnionApplyResponse( CEntityPlayer *pPlayer, CMessage *pMessage );	

	// 申请加入联军的结果通知
	void OnGSApplyUnionResultNotify( CEntityPlayer *pPlayer, CMessage *pMessage );	

	// 邀请军团加入联军的消息回应
	void OnGSInviteUnionMemberResponse( CEntityPlayer *pPlayer, CMessage *pMessage );	 

	// 邀请加入联军的消息通知
	void OnGSUnionInviteNotify( CEntityPlayer *pPlayer, CMessage *pMessage );	

	// 答复联军邀请的消息回复
	void OnGSReplyUnionInviteResponse( CEntityPlayer *pPlayer, CMessage *pMessage );		

	// 邀请军团加入联军的结果通知
	void OnGSUnionInviteResultNotify( CEntityPlayer *pPlayer, CMessage *pMessage );		

	// 有军团加入联军的消息通知
	void OnGSCorpsJoinUnionNotify( CEntityPlayer *pPlayer, CMessage *pMessage );			

	// 军团离开联军的消息回应
	void OnGSCorpsLeaveUnionResponse( CEntityPlayer *pPlayer, CMessage *pMessage );			

	// 军团离开联军的消息通知
	void OnGSCorpsLeaveUnionNotify( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 从联军踢出军团的消息回应
	void OnGSKickCorpsResponse( CEntityPlayer *pPlayer, CMessage *pMessage );		

	// 军团从联军里面被踢掉的通知	
	void OnGSCorpsKickedNotify( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 创建联军的回应消息
	void OnGSMakeUnionResponse( CEntityPlayer *pPlayer, CMessage *pMessage );	

	// 获得联军信息的回应
	void OnGSGetUnionInfoResponse( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 解散联军的消息回应
	void OnGSDismissUnionResponse( CEntityPlayer *pPlayer, CMessage *pMessage );		

	// 解散联军的通知	
	void OnGSDismissUnionNotify( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 获得联军申请列表的消息回应
	void OnGSUnionApplyListResponse( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 获得联军列表的消息回应
	void OnGSAllUnionListResponse( CEntityPlayer *pPlayer, CMessage *pMessage );

public:

	// 检查npc是否有军团相关的服务，并且距离是否合法
	bool CheckNpcCorpsService( CEntityPlayer *pPlayer, int nNpcEntityID, int eService );

	// 检查是否处于军团战竞价开放时间
	bool CheckInBidOpenTime( int nNpcID );
	
	// 增加玩家的贡献度
	void AddPlayerCorpsContribute( int nRoleID, int nContribute, int nAddType = 0 );

	// 改变玩家的军团采集活跃度
	void ChangeCorpsCollectActive( int nRoleID, int nChangeValue, int nChangeType = 0 );

	// 改变家族金钱
	void ChangeCorpsMoney( CEntityPlayer *pPlayer, int nMoney, bool bPayOut = false, int nReason = REASON_TASK ); 

	// 改变家族荣耀值
	void ChangeCorpsGlory( CEntityPlayer *pPlayer, int nValue, int bReduced = false, int nReason = REASON_TASK );			

	int OnStartFireBattleTimer( int nEntityID );

	void InsertNpcList( CMessageNpcListNotify* vpNotify );

	const char* GetCorpsNameByNpcID( int nNpcID );
	
	int	GetCorpsIDByNpcID( int nNpcID );
	
	// 购买军团物品的接口
	void BuyCorpsItem( CEntityPlayer *pPlayer, int nItemID, int nItemNum, int nNpcTmpID, int nCityMapID );	

	// 是否有军团联盟服务
	bool HasUnionService( CEntityPlayer *pPlayer, int vNpcEntityID );
	// 添加军团boss挑战次数
	void ChangeCorpsBossTimes( CEntityPlayer *pPlayer, int nBossId ,int nTimes , int nReason );

	// 增加军团boss炼化值
	void ChangeCorpsBOSSRefineValue( CEntityPlayer * pPlayer, int nValue , int nReason );

	//召唤军团boss请求
	void OnMessageSummonCorpsBossRequest( CEntityPlayer* pPlayer, CMessage* pMessage );

public:
	// 启动服务器
	virtual void OnLaunchServer( );

	// 退出服务器
	virtual void OnExitServer( );

	// 服务器Timer
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );

	// 建立场景
	virtual int OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName );

	// 路由消息
	virtual void OnRouterMessage( CMessage* pMessage );

	// 客户端发送消息
	virtual void OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 内部超时消息
	virtual void OnTimeoutMessage( CMessage* pMessage );

	// 建立实体
	virtual void OnCreateEntity( CEntity* pEntity, CCreator* pCreator, bool vFirst );

	// 销毁实体
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );

	// 存储数据
	virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode );

	// 加载配置
	virtual void OnLoadConfig( );

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( );
};
