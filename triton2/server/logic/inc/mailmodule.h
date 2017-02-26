#pragma once

#include "mailmessage_pb.hxx.pb.h"
#include "servercore.h"
#include "servertool.h"
#include "proclient_svr.h"
#include "servermessage_pb.hxx.pb.h"

// ********************************************************************** //
// CFriendModule
// ********************************************************************** //
class CMailModule : public CLogicModule, public CSingleton< CMailModule >
{

public:
	static char			sModuleName[ NAME_LENGTH ];
public:
	CMailModule( ) : CLogicModule( sModuleName ){ }
public:
	// 处理消息CMessageOpenMailBoxRequest
	void OnmessageOpenMailBoxReq( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理消息CMessageShutDownMailBoxRequest
	void OnmessageShutDownMailBoxReq( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理消息CMessageOpenSendMailBox
	void OnMessageOpenSendMailBox( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理消息CMessageOpenSendMailBox
	void OnMessageOpenReadMailBox( CEntityPlayer* pPlayer, CMessage* pMessage );
	 
	// 处理消息CMessageMailChangeItem
	void OnMessageChangeItem( CEntityPlayer* pPlayer, CMessage* pMessage );		
	
	// 处理消息CMessageMailSend
	void OnMessageSendMail( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理消息CMessageIfPlayerOnLine
	void OnMessageCanMailSend( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理消息CMessageShutMailBoxRequest
	void OnMessageShutMailBox( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理消息CMessageShutReadMailBoxRequest
	void OnMessageShutReadMailBox( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理消息CMessageShutSendMailBoxRequest
	void OnMessageShutSendMailBox( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理消息CMessageMailRead
	void OnMessageReadMail( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理消息CMessageGetAccessory
	void OnMessageGetAccessory( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 处理消息CMessageDelMail
	void OnMessageDelMail( CEntityPlayer* pPlayer, CMessage* pMessage );
	
	// 处理消息CMessageReadMailEnd
	void OnMessageReadMailEnd( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 处理消息CMessageBackMail
	void OnMessageBackMail( CEntityPlayer *pPlayer, CMessage *pMessage );	
	
	// 处理消息CMessageLockMail
	void OnMessageLockMail( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 处理消息CMessageUnLockMail
	void OnMessageUnLockMail( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 处理消息CMessageGetMailPageRequest
	void OnMessageGetMailPageRequest( CEntityPlayer *pPlayer, CMessage *pMessage );	
	
	// 处理消息CMessageCannelMailSend
	void OnMessageCannelMailSend( CEntityPlayer *pPlayer, CMessage *pMessage );
	
	// 付费邮件付费的请求
	void OnMessagePayMailMoneyRequest( CEntityPlayer *pPlayer, CMessage *pMessage );	
	
public:
	// 打开邮箱
	void OpenMailBox( CEntityPlayer*pPlayer );	
	
	// 打开发件箱
	void OpenSendMailBox( CEntityPlayer *pPlayer );
	
	// 打开收件箱
	void OpenReadMailBox( CEntityPlayer *pPlayer );
	
	// 处理网关的名字查询结果,判断是否可以给目标玩家发送邮件
	void  CheckRoleNameInfo( CMessage* pMessage );
	
	// 发送邮件
	void SendMail( CEntityPlayer* pPlayer, CMessageMailSend* pMessage );
	
	// 接收来自其他服务器的邮件
	void ReceiveRouterMail( CMessage *pMessage );
	
	// 查询结婚接受人的角色ID信息
	void CheckInvitationReceiver( CEntityPlayer *pPlayer, CMessageGetPlayerMailInfoResponse *pMessage );

 	// 向用户发送系统邮件   
    void OnSendSysMailToPlayer( CMessage* pMessage );
	
public:
	
	// 给玩家发送邮件错误消息
	void SendMailErrorCode( CEntityPlayer *pPlayer, int ErrorCode );
	
	// 发送系统邮件的接口
	int SendSysMail( const char *pDesName, int DesCharid, const char *pTitle, const char *pSrcName, const char *pBody, CItemObject **tpItemObj,int  ItemObjNum, int CountryID, int nMoney = 0, int nCharID = 0 );
	
	// 发送系统邮件的接口
	int SendSysMail( const char *pDesName, int DesCharID, const char *pTitle, const char *pSrcName, const char *pBody, int *pItemIDList, int *pItemNumList, int nItemNum, int nCountryID, int nMoney = 0, int nCharID = 0 );
	
	// 发送系统邮件的接口
	int SendSysMailBySysID( int nSysMailID, const char *pDesName, int DesCharID, int *pItemIDList, int *pItemNumList, int nItemNum, int nCountryID, int nMoney = 0, char **pParam = NULL, int *pParamPos = NULL, int nNum = 0, int nCharID = 0 );
	
	// 检查npc的服务类型
	bool CheckMailService( CEntityPlayer *pPlayer, int nEntityID );
	
	// 检查数据库是否还存在邮件
	void CheckIfMailInDB( CEntityPlayer *pPlayer, int nMailID, int nCheckType );
	
	// 检查数据库是否还存在邮件
	void OnSessionCheckIfMailInDB( CMessage * pMessage, int nCharID, int nMailID );
	
	// 为付费邮件付费
	void PayMoneyForMail( CEntityPlayer *pPlayer, int nMailID, bool bMailDelted );
	
	// 初始化系统邮件信息
	int InitializeSysMail( const char *pFileName );
	
	// 邮件转阵营
	void ChangeCampMail( CEntityPlayer *pPlayer,int nOldCampID, int nNewCampID );	
	
public:
	static unsigned int CountSize( );
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

	// 建立实体
	virtual void OnCreateEntity( CEntity* pEntity , CCreator* pCreator, bool vFirst );

	// 销毁实体
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );

	// 存储数据
	virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode );

	// 加载配置
	virtual void OnLoadConfig( );

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( ) { return true; }

	// 处理Gate消息
	void OnGateMessage( CMessage *pMsg );

	// 加载系统邮件配置的回调函数
	static void LoadSysMailConfigCallback(const char* pcConfigPath);
	
	// 发送离线留言
	void SendOffLineChatMsg( CEntityPlayer *pPlayer, int nDesCharID, const char *pMsg );

	// 获取离线留言
	void OnSessionLoadOffLineChatMsg( CMessage *pMsg, int nParam1, int nParam2 );
};



