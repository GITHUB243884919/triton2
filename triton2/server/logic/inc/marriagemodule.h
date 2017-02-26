#pragma  once	   
#include "servercore.h"
#include "servertool.h"
#include "marriagemanager.h"
#include "marriagemessage_pb.hxx.pb.h"

class CMarriageModule : public CLogicModule , public CSingleton< CMarriageModule >
{ 
public:
	CMarriageModule():CLogicModule(sModuleName)
	{
		if ( CObj::msCreateMode )
		{						
			Initialize();
		}
		else
		{
			
		}
	}
	 ~CMarriageModule(){ }
public:	
	static unsigned int CountSize();
	static char	sModuleName[ NAME_LENGTH ];	
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
	virtual bool OnCheckLaunch( ) { return mCampInfoComplete&&mMarriageComplete; }
public:
	int Initialize();		
		
public:		
	// 请求结婚的消息请求
	void OnMessageApplyMarryRequest( CEntityPlayer *pPlayer, CMessage* pMessage );
	
	// 请求结婚的消息回应
	void SendApplyMarryResponse( CEntityPlayer *pPlayer, int nErrcode );

	// 请求结婚的消息通知
	void SendMarryRequestNotify( CEntityPlayer *pPlayer, const char *pName, int nRoleID, int nWeddingTime, int nWeddingModule );
	
	// 答复结婚的消息请求
	void OnMessageReplyMarryRequest( CEntityPlayer *pPlayer, CMessage* pMessage );
	
	// 答复结婚的消息回应
	void SendReplyMarryResponse( CEntityPlayer *pPlayer, int nErrcode, const char *pSpouseName = NULL, int nResult = 0, int nMarriageStatus = 0 );
	
	// 玩家结婚成功的消息
	void SendPersonalMarriedNotify( const char *pGroomName, const char *pBrideName, int nServerSequence );

	// 婚期提醒
	void SendMarriageTipsNotify( CMarriageInfo *pMarriageInfo, CEntityPlayer *pPlayer = NULL );

	// 开始婚礼的消息请求
	void OnMessageBegginWeddingRequest( CEntityPlayer *pPlayer, CMessage* pMessage );
	
	// 开始婚礼的消息回应
	void SendBeginWeddingResponse( CEntityPlayer *pPlayer, int nErrcode );

	// 开始婚礼结婚另一方请求开始婚礼的通知
	void SendMarriageNoticeNotify( CEntityPlayer *pPlayer );		

	// 答复配偶开始婚礼的消息请求
	void OnMessageReplyBeginWeddingRequest( CEntityPlayer *pPlayer, CMessage* pMessage );
	
	// 答复配偶开始婚礼的消息回应	
	void SenndReplyBeginWeddingResponse( CEntityPlayer *pPlayer, int nErrcode );

	// 婚礼开始的通知(全服通知)	
	void SendWeddingNotify( CMarriageInfo *pMarriageInfo );	

	// 进入礼堂的通知
	void SendEnterMarriageHallNotify( CEntityPlayer *pPlayer );
	
	// 是否同意进入礼堂的答复请求
	void OnMessageReplyEnterMarriageHallRequest( CEntityPlayer *pPlayer, CMessage* pMessage );
	
	// 是否同意进入礼堂的答复回应
	void SendReplyEnterMarriageHallResponse( CEntityPlayer *pPlayer, int nErrcode );

	// 新人请求进入礼堂的消息请求
	void OnMessageNewlywedEnterHallRequest( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 新人请求进入礼堂的消息回应
	void SendNewlywedEnterHallResponse( CEntityPlayer *pPlayer, int nErrcode, int nWeddingModule, int nBaitangEnd );

	// 拥有请帖的人请求进入礼堂的消息请求
	void OnMessageGuestEnterHallRequest( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 拥有请帖的人请求进入礼堂的消息回应
	void SendGuestEnterHallResponse( CEntityPlayer *pPlayer, int nErrcode );

	// 新人拜堂的请求
	void OnMessageBaiTangRequest( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 新人拜堂的回应
	void SendBaitangResponse( CEntityPlayer *pPlayer, int nErrcode );

	// 新人拜堂的消息通知		
	void SendBaitangNotify( CEntityPlayer *pPlayer, const char *pGroomName, const char *pBrideName, int nGroomEntityID, int nBrideEntityID );

	// 放置信物的消息请求
	void OnMessagePutKeepsakeRequest( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 放置信物的消息回应
	void SendPutKeepsakeResponse( CEntityPlayer *pPlayer, int nErrcode, int nIndex );
	
	// 发送放置信物的通知
	void SendTimeToPutKeepSakeNotify( CMarriageInfo *pMarriage, CEntityPlayer *pPlayer = NULL );

	// 交换信物的请求
	void OnMessageExchangeKeepsakeRequest( CEntityPlayer *pPlayer, CMessage *pMessage );	
	
	// 交换信物的回应消息
	void SendExchangeKeepsakeResponse( CEntityPlayer *pPlayer, int nErrcode );

	// 交换信物的通知
	void SendExchangeKeepsakeNotify( const char *pGroomName, CItemObject *pBrideKeepsake, const char *pBrideName, CItemObject *pGroomKeepsake );

	// 离婚的消息请求
	void OnMessageDivorceRequest( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 离婚的消息回应
	void SendVivorceResponse( CEntityPlayer *pPlayer, int nErrcode );

	// 离婚的消息通知	
	void BroadCastDivorce( CEntityPlayer *pPlayer, const char *pGroomName, const char *pBrideName, int nGroomCharID, int nBrideCharID );
	
	// 发给配偶的离婚消息通知
	void SendDivorceNotify( CEntityPlayer *pPlayer, int nRoleID );

	// 结婚结果通知(对方是否同意)
	void SendMarryResultNotify( CEntityPlayer *pPlayer, int nResult, const char *pSpuseName, int nMarriageStatus );
	
	// 开始婚礼的结果通知(配偶是否同意)	
	void SendBeginWeddinResultNotify( CEntityPlayer *pPlayer, int nResult );
	
	// 获取本阵营婚礼信息的消息请求
	void OnMessageGetCampMarriageInfoRequest( CEntityPlayer *pPlayer, CMessage *pMessage );
			
	// 本阵营婚礼信息的消息回应	
	void SendGetCampMarriageInfoResponse( CEntityPlayer *pPlayer );	
	
	// 发送个人的婚姻信息
	void SendPersonMarriageInfoNotify( CEntityPlayer *pPlayer, CMarriageInfo *pMarriageInfo );
	
	// 配偶离婚的路由消息
	void OnRoterMessageSpouseDivorce( CMessage *pMessage );
	
	// 检查npc的服务和距离
	bool CheckMarriageNpc( CEntityPlayer *pPlayer, int nEntityID );
	
	// 使用系统请帖的请求
	void OnMessageUseSysInvitation( CEntityPlayer *pPlayer, CMessage *pMessage );

	// 发送使用系统请帖的回应消息
	void SendUseSysInvitationResponse( CEntityPlayer *pPlayer, int nErrcode );
	
	// 发送新人结婚成功的通知
	void SendNewlyMarriedNotify( CMarriageInfo *tpMarriage );
	
	// 开始信物兑换的消息请求
	void OnMessageStartKeepsakeExchangeRequest( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 发送开始信物兑换的消息回应
	void SendStartKeepsakeExchangeResponse( CEntityPlayer *pPlayer, int nErrcode );
	
	// 终止信物兑换的消息请求
	void OnMessageStopKeepsakeRequest( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 终止信物兑换的消息回应
	void SendStopKeepsakeResponse( CEntityPlayer *pPlayer, int nErrcode );
	
	// 发送终止信物的消息通知
	void SendStopKeepsakeNotify( CEntityPlayer *pPlayer );
	
	// 请帖兑换的请求
	void OnMessageInvitationExchange( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 发送请帖兑换的消息回应
	void SendInvitationExchangeResponse( CEntityPlayer *pPlayer, int nErrcode );
	
	// 发送信物交换的结果通知
	void SendKeepsakeExchangeResultNotify( CMarriageInfo *pMarriage, int nGroomItemID, int nBrideItemID, CEntityPlayer *pPlayer = NULL );
	
	// 信物交换
	void InvitationExchange( CEntityPlayer *pPlayer, int nItemIndex );
	
	// 请求传送到婚礼npc的消息
	void OnMessageTeletoMarriageNpcRequest( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 发送请求传送到婚礼npc的消息回应
	void SendTeletoMarriageNpcResponse( CEntityPlayer *pPlayer, int nErrcode );
	
	// 重温补办婚礼的消息请求
	void OnMessageReviewWeddingRequest( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 重温补办婚礼的消息回应
	void SendReviewWeddingResponse( CEntityPlayer *pPlayer, int nErrcode );
	
	// 重温补办婚礼的消息通知
	void SendReviewWeddingNotify( CMarriageInfo *pMarriage );
	
	// 玩家重温或者补办婚礼
	void PlayerReviewWedding( CEntityPlayer *pPlayer, int nWeddingModule, int nTimeID, int nDayID );			
		
public:
	
	// 玩家申请结婚
	void PlayerMarryApply( CEntityPlayer *pPlayer, int nWeddingTime, int nWeddingTimeLength, int nWeddingModule );
	
	// 玩家答复结婚请求
	void PlayerReplyMarry( CEntityPlayer *pPlayer, int nResult );
	
	// 玩家开始婚礼的请求信息
	void PlayerBeginWedding( CEntityPlayer *pPlayer, CEntity *pNpc = NULL );
	
	// 玩家答复婚礼开始
	void PlayerReplyWeddingBegin( CEntityPlayer *pPlayer, int nResult );
	
	// 玩家开始进入礼堂
	void PlayerEnterMarriageHall( CEntityPlayer *pPlayer, bool bNewly = true );
	
	// 婚礼结束的时间
	void OnTimeOutWeddingEnd( CTimerItem* pTimer );
	
	// 结束婚礼
	void EndWedding();
	
	// 结婚申请失效
	void OnTimeOutMarryApplyTimeOut( CTimerItem* pTimer );
	
	// 发送婚礼通知
	void OnTimeWeddingNotify( CTimerItem* pTimer );
	
	// 婚礼游行结束
	void OnTimeOutWeddingOnStreet( CTimerItem* pTimer );
	
	// 玩家拜堂
	void PlayerBaitang( CEntityPlayer *pPlayer, CEntity *pEntity );
	
	// 玩家获取结婚信息列表
	void PlayerGetMarriageInfo( CEntityPlayer *pPlayer );
	
	// 玩家放置信物的请求
	void PlayerPutKeepsake( CEntityPlayer *pPlayer, int nKeepSeekIndex );
	
	// 交换信物的请求
	void PlayerExchangeKeepsake( CEntityPlayer *pPlayer );
	
	// 玩家离婚的请求
	void PlayerDivorce( CEntityPlayer *pPlayer );
	
	// 玩家请求取消婚礼
	void PlayerCancelWedding( CEntityPlayer *pPlayer );
	
	// 使用系统请帖
	void UseSysInvitation( CEntityPlayer *pPlayer, int nIndex, const char *pName );
	
	// 给玩家通过系统邮件发送请帖
	void SendInvitationByMail( CEntityPlayer *pPlayer, int nSysInvitationIndex, int nRoleID, const char *pName );
	
	// 获取被占用的时间段信息
	PBMarriageInfo *GetSysWeddingTimeUsed( CMarriageInfo *tpMarriageInfo, PBMarriageList &rTimeList, time_t nTime );	
	
	// 创建移动npc	
	
	// 初始化系统婚礼时间
	int InitializeSysWeddingInfo( const char *pFileName );		
	
	// 初始化npc的移动路径
	int InitializeWeddingNpcPath( const char *pFileName );	
	
	// 清除玩家的婚礼信息
	int ClearWeddingInfo( CMarriageInfo *pMarriage );
	
	// 给整个地图的玩家发送消息
	void SendMapMessage( int nMapID, CMessage *pMessage );
	
	// 获取npc的头顶应该显示的新人名称
	const char *GetNewlyName( CEntity *tpEntity );
	
public:
	// 创建婚礼的数据库信息
	void CreatMarriageDBInfo( CMarriageInfo *tpMarriageInfo );
	
	// 创建婚礼数据库信息的返回消息
	void OnSessionCreatMarriageDBInfo( CMessage *pMessage, int nParam1, int nParam2 );	
	
	// 检查婚礼状态
	void CheckWeddingStatus();	
	
	// 检查婚礼npc的移动情况
	void CheckWeddingNpc();
	
	// 创建婚礼游行信息
	void CreatWeddingInfo( CMarriageInfo *pMarriageInfo );
	
	// 初始化礼堂
	void InitialWeddingHall( CMarriageInfo *tpMarriageInfo );
	
	// 发送婚礼结束的通知
	void SendWeddingEndNotify( CMarriageInfo *pMarriageInfo );
	
	// 检查婚礼状态
	bool CheckNewlyBaitangStatus( );

public:	
	// 加载婚姻表的婚姻记录数量
	void LoadMarriageCountFromDB( int nCampID );
	
	// 加载婚姻表的婚姻记录数量的数据库返回消息
	void OnSessionLoadMarriageCountFromDB( CMessage *pMessage, int nParam1, int nParam2 );
	
	// 加载婚姻的数据库信息
	void LoadMarriageInfoFromDB( int nTotalCount, int nLoadCount );	
	
	// 加载婚姻数据库信息的返回消息
	void OnSessionLoadMarriageInfoFromDB( CMessage *pMessage, int nParam1, int nParam2 );	
	
	
	// 获取数据库中阵营信息
	void LoadCampInfo( int nCampID );
	
	// 获取数据库中阵营信息的返回消息
	void OnSessionLoadCampInfo( CMessage *pMessage, int nParam1, int nParam2 );
	
	// 加载玩家的婚姻信息
	void LoadPlayerMarriageInfo( CEntityPlayer *pPlayer );
	
	// 加载玩家的婚姻信息的数据库返回消息
	void OnsessionLoadPlayerMarriageInfo( CMessage *pMessage, int nRoleID, int nParam2 );
	
	// 创建阵营信息表
	void CreatCampDBInfo( int nCampID, int nMarriedNum );
	
	// 创建阵营信息表的回应消息
	void OnSessionCreatCampDBInfo( CMessage *pMessage, int nCampID, int nParam2 );
	
	// 更新玩家角色信息中的婚姻ID字段
	void UpDatePlayerMarriageID( int nRoleID, int nMarriageID );
	
	// 更新玩家的婚姻信息数据库
	void UpDateMarriageInfo( CMarriageInfo *tpMarriageInfo );
	
	// 更新数据库的阵营信息
	void UpDateCampDBInfo( CCampInfo *tpCampInfo );
	
	// 删除数据库的阵营信息
	void DeleteMarriageDBInfo( CMarriageInfo *tpMarriage );
	
	// 开始婚礼
	void StartWedding( CMarriageInfo *pMarriageInfo, EWeddingStartType emType );
	
private:
	bool mMarriageComplete;
	bool mCampInfoComplete;	
	CWTimer mWeddingNpcTimer;
	CWTimer mMarriageStatusTimer;
	CWTimer mBaitangTimer;	
};