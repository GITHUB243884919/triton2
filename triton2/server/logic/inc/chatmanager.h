#pragma once
#include "sceneobj_define.h"
#include "chat.h"
#include "chattype.h"
#include "chatmessage_pb.hxx.pb.h"
#include "message_pb.hxx.pb.h"
#include "coremessage_pb.hxx.pb.h"
#include "entity.h"




// 聊天内容
class CChatContent
{
public:
	enum {  MAX_CHATATTACH_ITEM = 5 };

	CChatContent(  )
	{
		mItemNum = 0;
		mSrcCharID = 0;
		mSrcEntityID = 0;
		mSrcPlayerType = 0;
		mDstPlayerType = 0;
		mChannel = 0;
		mRouterType = 0;
		mSrcCharName.clear();
		mDstCharName.clear();
		mMsg.clear();
		memset(mPkgIdx, 0, sizeof(mPkgIdx) );
		memset(mItemIdx, 0, sizeof(mItemIdx) );
		memset(mItemSeq, 0, sizeof(mItemSeq));
	}

	~CChatContent() {}
	
	int mSrcEntityID;
	int mSrcCharID;
	lk::string<NAME_LENGTH> mSrcCharName;
	lk::string<NAME_LENGTH> mDstCharName; // 目的玩家charname
	int mDstCharID;
	int mChannel;	// 频道
	lk::string<MAX_MASSAGE_LEN> mMsg;	// 聊天内容
	int mItemNum;
	int mPkgIdx[MAX_CHATATTACH_ITEM];
	int mItemIdx[MAX_CHATATTACH_ITEM];
	int mItemSeq[MAX_CHATATTACH_ITEM];
	int mSrcPlayerType;
	int mDstPlayerType;
	int mRouterType;
};





// ********************************************************************** //
// CChatManager
// ********************************************************************** //
//typedef  struct BugleChannelMess
//{
//	char PlayerName[NAME_LENGTH];
//	char MessBuff[MAX_MASSAGE_LEN];
//	char ItemBuff[MAX_ITEMBUFF_LENGTH];
//	int	 ItemBuffLen;
//	int  PlayerCharID;
//}BugleMess;
//
////喇叭频道
//class CBugleChatMess
//{
//private:	
//	BugleMess mBugleChat[MAXBUGLECHATNUM];//喇叭频道发言缓存
//	int mCurrentCur;           
//	int mInsertPos;
//	int mPlayerCount;
//
//public:
//    //将一个玩家发言插入到喇叭频道
//	int InsertBugleChat( int vCharID, const char * playername, const char *pMessage, const char *pItemInfo = NULL, int ItemBuffLen = 0 );
//
//	//获得现在的发言玩家
//	BugleMess * GetCurrentChat();
//
//	//获得发言玩家的个数
//	int GetPlayerCount();	
//
//public:
//	int Initialize();
//	int Resume();
//	CBugleChatMess();	
//	~CBugleChatMess()
//	{
//
//	}
//
//};


class CChatUser
{
public:
	CChatUser()
	{
		mType = 0;
		mFlag = 0;
		mPriority = 0;
	}
	~CChatUser(){}
public:
	int mType;		// 客户端的用户类型(enUserType)
	int mFlag;		// 类型标识ID
	int mPriority;	// 优先级
};


class CChatManager : public CSingleton< CChatManager >
{
public:
	typedef lk::vector< CChatUser, MAX_CHATUSER_NUM  > CChatUserList;	

public:
	// 喇叭队列管理器对象
	//CBugleChatMess BugleChat;	
		
	//服务器自动喊话管理器对象 
	CAutoMsg    AutoChat;    
	CChatUserList mChatUserList;
public:
	CChatManager( );
	
public:
	~CChatManager( );   
	void* operator new( size_t size );
	void  operator delete( void* pointer );
	void* operator new(size_t size, const char* file, int line);
	void operator delete(void* pointer, const char* file, int line);

	static unsigned int CountSize( );
	static CSharedMem* mShmPtr; // 共享内存地址
   
public:
	// 获取chat
	CChat* GetChat( CEntityPlayer *pPlayer );		
	// 添加Chat
	CChat* AddPlayer( CEntityPlayer *pPlayer );			
	// 删除Chat
	void Erase( CEntityPlayer *pPlayer ); 	
public:
	int InitializeChatUserList()
	{
		mChatUserList.initailize();
		return SUCCESS;
	}	
public:
	int InsertChatUser( CChatUser &rChatUser );	
	int GetChatUserInfo( CEntityPlayer *pPlayer, CChatUser &rChatUser );
};




class ChatSendMsg
{
public:
	CMessage         MessageHead;
	CMessageChatSend ChatSend;
public:
	ChatSendMsg(  ){};   
    CMessage *SetChatSendMsg( int vChannel, const char *vInfor , const char *vPlayerName )
	{
		ChatSend.set_channel( vChannel );
		if ( vPlayerName != NULL )
		{
			ChatSend.set_playername( vPlayerName );
		}
		if ( vInfor != NULL )
		{
			ChatSend.set_msg( vInfor );
		}
		MessageHead.mutable_msghead()->set_messageid( ID_C2S_CMESSCHATSEND );
		MessageHead.set_msgpara( ( int )&MessageHead );		
		return &MessageHead;
	}
    ~ChatSendMsg()
	{

	}
};


class ChatCallBackMsg
{
public:
	CMessage                 MessageHead;
	CMessageChatSendCallBack ChatCallBack;
public:
	ChatCallBackMsg(){}
	//CMessage * ChatCallBackMsg::SetChatCallBackMsg( int vChannel, const char* vInfor, const char* vSrcName, const char*vItemInfo = NULL, int vItemBuffLen = 0, const char* vTargetName = NULL ,int vSend2Player = 1, int vRouterType = CMessageRouter::ROUTER_NAME  , int vShowInTalble = 1, int vDesCharID = 0 ,int vSrcCharID = 0 , enUserType enType = USERTYPE_NONE, enUserType enDesType = USERTYPE_NONE );
	CMessage * SetChatCallBackMsg( CChatContent* pChatContent );
	~ChatCallBackMsg(){}
};








class  ChatResultMsg
{
public:
	CMessage           MessageHead;
	CMessageChatResult ChatResult;
public:
	ChatResultMsg(){}
	CMessage * SetChatResultMsg( int vChannel, int vChatRes, const char *vInfor , const char *vName = NULL, int vLeftTime=0   )
	{	
		ChatResult.set_channel( vChannel );
		ChatResult.set_chatres( vChatRes );
		if ( vInfor != NULL )
		{
			ChatResult.set_msg( vInfor );
		}		
		if ( vName != NULL )
		{
			ChatResult.set_name( vName );
		}
		ChatResult.set_lefttime( vLeftTime );		

		MessageHead.mutable_msghead()->set_messageid( ID_S2C_CHATRES2CLIENT );
		MessageHead.set_msgpara( ( int )&ChatResult );
		return &MessageHead;
	}
};






class ChatUseItemMsg
{
public:
	CMessage         MessageHead;
	CMessageUseItem  UseItem;
public:
	ChatUseItemMsg(){}
     CMessage *SetChatUseItemMsg(  int vItemIndex ,char * vInfor  )
	{		
		UseItem.set_itemindex( vItemIndex );
		if ( vInfor != NULL )
		{
			UseItem.set_msg( vInfor );
		}

		MessageHead.mutable_msghead()->set_messageid( ID_C2S_NOTIFY_USECHATITEM  );
		MessageHead.set_msgpara( ( int )&UseItem );	
		return &MessageHead;
	}
};

class AutoChatMsg
{
public:
	CMessage            MessageHead;
	CAutoChatMessage    AutoChat;
public:
	AutoChatMsg(  ){}
	CMessage *  SetAutoChatMsg( int vChannel, const char *vInfor, const char * vName ,int vTime ,int vCount  )
	{		
		if ( vInfor != NULL )
		{
			AutoChat.set_msg( vInfor );
		}

		AutoChat.set_channel( vChannel );
		if ( vName != NULL )
		{
			AutoChat.set_name( vName );
		}
		AutoChat.set_time( vTime );
		AutoChat.set_count( vCount ); 
		MessageHead.mutable_msghead()->set_messageid( ID_C2S_NOTIFY_AUTOCHATMESSAGE );
		MessageHead.set_msgpara( ( int )&AutoChat);		
		return &MessageHead;
	}
};

class ChatRouterMsg
{
public:
	CMessage               MessageHead;
	CMessageRouterChatMsg  RouterChatMsg;

public:
	ChatRouterMsg(){}
	CMessage * SetChatRouterMsg( int vChannel, const char* vInfor, const char* vSrcName, const char* vTargetName , int vSend2Player )
	{
		RouterChatMsg.set_channel( vChannel );	
		if ( vInfor != NULL )
		{
			RouterChatMsg.set_msg( vInfor );
		}

		RouterChatMsg.mutable_router()->set_send2player( vSend2Player );		

		if ( vSrcName != NULL )
		{
			RouterChatMsg.mutable_router()->set_srcname( vSrcName );
		}
		if ( vTargetName != NULL )
		{
			RouterChatMsg.mutable_router()->set_desname( vTargetName );
		}

		MessageHead.mutable_msghead()->set_messageid( ID_S2C_ROUTERCHATMESSAGE );
		MessageHead.set_msgpara( ( int )&RouterChatMsg );	
		return &MessageHead;
	}
};
