#pragma once

#include "sceneobj_define.h"
#include "chatmanager.h"
#include "proclient_svr.h"
#include "chat.h"



// ********************************************************************** //
// CChatModule
// ********************************************************************** //
class CChatModule : public CLogicModule, public CSingleton< CChatModule >
{
public:
	static char	sModuleName[ NAME_LENGTH ];
    
   // 用于喇叭频道时钟计数器时常8秒
	CWTimer	  mTimer;   	

   //用于自动喊话的计时器，每一秒检测一次喊话队列
    CWTimer   mAutoTimer;	

   //获取喊话队列中现在可以发言的消息内容
	sAutoMsg  mCurrentAutoChat;	
	CHAT_CFG*   m_sChatCfg;
	bool        m_bIfBugleEmpty;		// 用于标志喇叭队列为空时候，是否还有玩家的发言处于显示过程中	
public:
	CChatModule( ) : CLogicModule( sModuleName ), mTimer(8000),mAutoTimer(1000)	
	{	
		m_sChatCfg = NULL;
		m_bIfBugleEmpty = true;				
	}

	static unsigned int CountSize( );
public:
	// 检查频道规则
	int CheckChannelRule( CEntityPlayer *pPlayer, int nChannel );

	// 处理并发送聊天信息
    void CheckChatMsg( CEntityPlayer* pPlayer, CChatContent* pChatContent, int IsAutoMess = 0 ); 
    
    // 在聊天频道设置发送聊天消息
    void ChatOnChannel( CEntityPlayer* pPlayer, CChatContent* pChatContent  );  
 
	//检测自动喊话队列中是否有可以发言的玩家，如果有的话则将发言内容发送出去
	void SendAutoMessToClient( CAutoMsg *AutoChat );
	
	// 消耗喊话道具
	int ConsumeChatItem( int ItemId, int ItemNum , CEntityPlayer * player, int ItemIndex = -1 );

	// 检测自动喊话的时间是否满足要求
	int CheckAutoChatTime( unsigned int time , int channel );
    
	// 把一句话在普通频道发送出去
	//void SendMsgOnChannelCommon( CEntityPlayer *pPlayer , const char *pMess );     

	//处理消息CMessageChatSend
	void OnMessageChatSend( CEntityPlayer* pPlayer, CMessage* pMessage, int IsAutoMess = 0 );

	//处理消息CAutoChatMessage
	void OnMessageAutoChat( CEntityPlayer* pPlayer, CMessage* pMessage, int IsAutoMess = 0 );

	//处理消息CStopAutoChatMessage
	void OnMessageStopAutoChat(CEntityPlayer* pPlayer, CMessage* pMesage, int IsAutoMess = 0 );

	//处理消息CMessageUseItem
	void OnMessageUseItem( CEntityPlayer* pPlayer, CMessage* pMesage, int IsAutoMess = 0 );

	//处理消息CMesssageClickBugleItem
	void OnMessageClickBugleItem( CEntityPlayer* pPlayer, CMessage* pMesage, int IsAutoMess = 0 );
	
	//处理消息CMesssageCheckDirtyWord
	void OnMessageCheckDirtyWord( CEntityPlayer* pPlayer, CMessage* pMesage, int IsAutoMess = 0 );  
	
    // 处理普通频道消息
	void ChatOnChannelCommon( CEntityPlayer* pPlayer, CChatContent* pChatContent );

	// 处理密语频道消息
	void ChatOnChannelP2P( CEntityPlayer* pPlayer, CChatContent* pChatContent );
    
	// 处理世界频道消息
	void ChatOnChannelWorld( CEntityPlayer* pPlayer,CChatContent* pChatContent);
    
	// 处理队伍频道消息
	void ChatOnChannelTeam( CEntityPlayer* pPlayer, CChatContent* pChatContent);

	// 处理战场频道
	void ChatOnChannelBattle( CEntityPlayer* pPlayer, CChatContent* pChatContent);
    
	// 处理喇叭频道消息
	void ChatOnChannelBugle( CEntityPlayer* pPlayer, CChatContent* pChatContent, int ItemIndex = -1 );
	
	// 发送国家频道消息
	void ChatOnChannelCountry( CEntityPlayer* pPlayer,CChatContent* pChatContent );
	
	// 发送家族频道消息
	void ChatOnChannelFamily( CEntityPlayer* pPlayer, CChatContent* pChatContent);
	
	// 发送军团频道消息
	void ChantOnChannelCorps( CEntityPlayer* pPlayer, CChatContent* pChatContent);
	
	// 发送聊天信息结果
	void SendChatResultMsg( CEntityPlayer *pPlayer, int vChannel, int vChatRes, const char *vInfor, const char *vName, int vLeftTime = 0 );	
	
    // 禁止玩家在某个频道发言    
    void SetChannelFalse( int vCharID, int vChannel, int nPeriod ); 
        
    // 恢复玩家在某个频道的发言权利
    void SetChannelTrue( int vCharID, int vChannel ); 
    
    // 处理来自网关的家族和军团的消息
    void OnGateChatMsg( CMessage *pMsg );
    
    // 加载聊天配置
    int InitialChatUserConfig();
    
    // 获取玩家的聊天身份标志
    int GetChatUserFlag( CEntityPlayer *pPlayer, ChatCallBackMsg &rMsg );
public:
	// 检查脏词
	void CheckDirtyWordOfMsg( char *pMsg );
	
	// 获取频道时间限制信息
	int  GetChannelTime( int vChannel, CChat *tpChat, int &ChatLimitTime, int &MsgLimitTime, TimeLimit *&tpTime, bool bVip = false );
	
	// 检查发言时间
	int  CheckChatTime( int vChannel, char *pMsg, CChat *tpChat, bool bVip = false );
		
	// 检查等级
	int	 CheckLevel( int vChannel, CEntityPlayer *pPlayer );
	
	// 检查AP
	int  CheckAP( int vChannel, CEntityPlayer *pPlayer ); 
	
	// 检查自动喊话的相关信息
	int OnAutoChat( CEntityPlayer *pPlayer, char *pMsg );
		
	// 发送系统公告
	void SendSysNotice( const char *pMsg, int RepeatedNum );

	// 发送聊天内容到日志服务器
	void SendChat2Log( CEntityPlayer *pPlayer, CChatContent* npContent );
	
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
	virtual void OnLoadConfig( ) ;	 

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( ) { return true; }

	// 用于加载聊天配置的回调函数
	static void LoadChatUserConfigCallback(const char* pcConfigPath);
};
