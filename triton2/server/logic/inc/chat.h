#ifndef _CHAT_H_
#define _CHAT_H_

#pragma  once
#include "object.h"
#include "chattype.h"
#include <time.h>
#include "sceneobj_define.h"
#include "scenecfg_manager.h"
//#define  SPEAKCOUNT  6
// ********************************************************************** //
// CChat
// ********************************************************************** //

class TimeLimit
{
 //enum { MAX_CHATMSG_LEN = 200 };
private:	

	time_t ChatTime;
	time_t FirMessTime;
	time_t SecMessTime;		
	char MessBuffer[ MAX_MASSAGE_LEN ];
	int  MessCount;	

private:	
	int  CheckMess( const char *pMess, int MessTime );
public:
	enum
	{
		SendOk = 1,				// 可以发言
		NotInTimeLimit = 2,		// 发言频率过快
		NotInMessLimit = 3,		// 一句话6秒内重复三次
		INCOOLTIME	   = 4,		// 处于跨服或者刚登录服务器的冷却时间
	};
public:	
	TimeLimit();	
	int Initialize();
	int Resume(); 
	int CanSendMess( const char *p, int ChatLimitTime, int MessLimitTime );
	int SetChatTime( int nChatTime, int nFirMsgTime, int nSecMsgTime );	
};




class CChat : public CObj
{
private:
	int mForbiddenChannel;	
	int mFreeTime; // 禁止过期时间(未来时间), 单位: 秒
	
	enum { InCommonTime,InPtoPTime,InWorldTime };
public:
	
	CChat(  )
	{
		if( CObj::msCreateMode )
		{
			Initialize();
		}
		else
		{
			Resume();
		}
	}

	virtual ~CChat() {};

	virtual int Initialize()
    {
		mForbiddenChannel = 0;
		mFreeTime = 0;
		return 0;
	}
	virtual int Resume() {return 0;}

	bool InChannel( CHAT_CHANNEL vChannel );	
	
	// 设置禁言频道
	void SetChannel( int vChannel, bool IfSetChannelTrue = false );
	
	// 初始化禁言频道
	void InitialChannel( int vChannel, int nPeriod );
	
	// 得到被禁频道
	int GetForbidenChannel( );
	
	// 得到解禁时间
	inline int GetForbidenFreeTime( ) { return mFreeTime; }

	//时间限制代码
	//TimeLimit PtoPTimeLimit;
	TimeLimit CommonTimeLimit;
	TimeLimit WorldTimeLimit;
	TimeLimit CounrtyTimeLimit;
	
	//时间限制代码
	
};
struct sAutoMsg
{
	//char PlayerName[NAME_LENGTH];
	int  PlayerId;
	char MessBuff[ MAX_MASSAGE_LEN ];
	int  time;
	int  count;
	int  channel;
	sAutoMsg( )
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
	int Resume();
} ;

class CAutoMsg
{
private:	  
	int mSpeakNum;          //现在使用自动喊话的玩家个数	
	int mCurChatPos;        //获取队列中当前可以发言的玩家信息
    int mGetChatBegin;      //标志是否处于循环获取喊话队列内容的过程中
	sAutoMsg mDeletedChat;  //保存最近一次被删除的玩家信息
public:
	 sAutoMsg mAutoMessBuff[ MAXAUTOCHATNUM ];
	 enum 
	 {
		 CHATCOMPLETE = 1,    //完成喊话队列一次循环检测
		 CURCHATNOTINTIME,    //喊话队列中的当前玩家未到发言时间
		 CURCHATINTIME,        //喊话队列中当前玩家满足时间要求可以发言
		 CURCHATCOMPLETE      //当前玩家的自动喊话已经结束
	 };
public:  
  
   void SetCurPos( int pos = 0 );

   int InsertAutoMess(  int playerid, const char *pMess, int time, int channel, int chatcount );

   int GetCurrentChat( sAutoMsg *CurAutoChat);

   int ModifyAutoMess( int playerid, const char *pMess, int time, int channel, int chatcount = 1 );

   int DelAutoMess( int playerid, int iscurrentplayer = 1 );

   int ChangeChatCount( int playerid, int count ) ;
   
   int GetSpeakNum( );    

public:
   CAutoMsg( );
   int Initialize();
   int Resume();
   ~CAutoMsg( ){ };  
};
#endif


