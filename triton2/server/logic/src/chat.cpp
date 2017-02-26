#include "stdafx.hpp"
#include "chat.h"
#include "westserver.h"


int sAutoMsg::Initialize()
{
	time = 0;
	count = 0;
	channel = 0;
	memset( MessBuff, 0, sizeof( MessBuff ) );
	return 0;
}
int sAutoMsg::Resume()
{
	return 0;
}

bool CChat::InChannel( CHAT_CHANNEL vChannel )
{
	return CHAT_IN_FORBIDDENCHANNEL( mForbiddenChannel, vChannel );
}

int CChat::GetForbidenChannel( )
{
	return mForbiddenChannel;
}

void CChat::SetChannel( int vChannel, bool IfSetChannelTrue  )
{
	if( vChannel <= CHANNEL_ERR || vChannel >= CHANNEL_END )
	{
		return;
	}
	
	if ( IfSetChannelTrue == true )
	{
		mForbiddenChannel &= ~vChannel;	
	}
	else
	{
		mForbiddenChannel |= vChannel;
	}	
}

void CChat::InitialChannel( int vChannel,int nPeriod )
{
	mForbiddenChannel = vChannel;

	// 禁言的过期时间是从当前时间起计算
	mFreeTime = time(0) + nPeriod;
}

TimeLimit::TimeLimit( )
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

int TimeLimit::Initialize()
{ 
	ChatTime = 0;	
	FirMessTime = time( NULL );
	SecMessTime = time( NULL );
	memset( MessBuffer, 0, sizeof( MessBuffer ) );
	MessCount = 0;	
	return 0;
}

int TimeLimit::Resume()
{
	return 0;
}


int TimeLimit::CheckMess( const char *pMess, int MessTime )
{
	time_t timenow;
	timenow = time( NULL );
	if( strcmp( pMess, MessBuffer ) != 0 )//消息已经发过
	{
		memset( MessBuffer, 0, sizeof( MessBuffer ) );
		strncpy( MessBuffer, pMess, sizeof( MessBuffer )-1 ); //将新消息换成当前消息
		MessCount = 0;
	}

	if( MessCount == 0 )
	{
		FirMessTime = time( NULL );
		MessCount++;
		return 1;
	}
	else if( MessCount == 1 )
	{
		SecMessTime = time( NULL );
		MessCount++;
		return 1;		
	}
	else 
	{  
		if ( timenow-FirMessTime >= MessTime )
		{
			MessCount = 2;
			FirMessTime = SecMessTime;
			SecMessTime = time( NULL );
			return 1;
		}
		else
		{
			return 0;
		}			
	}		
}
int TimeLimit::CanSendMess( const char *pMess, int ChatLimitTime, int MessLimitTime )
{
	time_t timenow = time( NULL );
	if( timenow - ChatTime >= ChatLimitTime )
	{			
		if ( MessLimitTime == 0 )
		{
			ChatTime = time( NULL );
			return SendOk;
		}
		if( CheckMess( pMess, MessLimitTime ) == 1 )
		{	
			ChatTime = timenow;
			return SendOk;
		}
		else
		{
			return NotInMessLimit;
		}
	}
	else
	{			
		return NotInTimeLimit;
	}
}

int TimeLimit::SetChatTime( int nChatTime, int nFirMsgTime, int nSecMsgTime )
{
	ChatTime = nChatTime;
	FirMessTime = nFirMsgTime;
	SecMessTime = nSecMsgTime;	
	return 0;
}

CAutoMsg::CAutoMsg()
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
int CAutoMsg::Initialize()
{
	mSpeakNum = 0;
	mCurChatPos = 0;
	mGetChatBegin = 0;
	memset( &mDeletedChat, 0, sizeof( mDeletedChat ) );
	memset( &mAutoMessBuff, 0, sizeof( mAutoMessBuff ) );
	return 0;
}
int CAutoMsg::Resume()
{		   	
	return 0;
}
int CAutoMsg::GetCurrentChat( sAutoMsg *CurAutoChat)
{
	int mEmptyMessPos = -1;
	if( mGetChatBegin == 0 )
	{
		mCurChatPos = 0;
		mGetChatBegin = 1;
	}
	if ( mSpeakNum <= 0 || mCurChatPos >= mSpeakNum )
	{
		mGetChatBegin = 0;
		return CHATCOMPLETE;
	}		

	mAutoMessBuff[mCurChatPos].count--;	

	if(( mAutoMessBuff[mCurChatPos].count+1 ) % mAutoMessBuff[mCurChatPos].time != 0 )
	{   
		mCurChatPos++;

		return CURCHATNOTINTIME;
	}    

	if ( mAutoMessBuff[mCurChatPos].count < mAutoMessBuff[mCurChatPos].time )
	{
		mEmptyMessPos = mCurChatPos;	
	}

	memcpy(  CurAutoChat,  &mAutoMessBuff[mCurChatPos], sizeof( sAutoMsg ) ); 

	CurAutoChat->count = mAutoMessBuff[mCurChatPos].count / mAutoMessBuff[mCurChatPos].time ;

	mCurChatPos++;    

	if ( mEmptyMessPos == -1 )
	{		
		return CURCHATCOMPLETE;
	}
	
	DelAutoMess( 0, 1 );

	return CURCHATCOMPLETE;
}



int CAutoMsg::InsertAutoMess( int playerid, const char *pMess, int time, int channel, int chatcount )
{
	if ( mSpeakNum == MAXAUTOCHATNUM )
	{
		return -1;
	}	

	memset( mAutoMessBuff[mSpeakNum].MessBuff,0,sizeof( mAutoMessBuff[mSpeakNum].MessBuff ) );
	
	strncpy( mAutoMessBuff[mSpeakNum].MessBuff, pMess, sizeof( mAutoMessBuff[mSpeakNum].MessBuff ) - 1 );

	mAutoMessBuff[mSpeakNum].PlayerId = playerid;  

	mAutoMessBuff[mSpeakNum].count = chatcount*time;
	mAutoMessBuff[mSpeakNum].channel = channel;
	mAutoMessBuff[mSpeakNum].time = time;

	mSpeakNum++;

	return 1;
}

int CAutoMsg::GetSpeakNum()
{
	if ( mSpeakNum < 0 )
	{
		mSpeakNum = 0;
	}
	return mSpeakNum;
}

void CAutoMsg::SetCurPos( int pos )
{
	mCurChatPos = pos;
}
int  CAutoMsg::ModifyAutoMess( int playerid, const char *pMess, int time, int channel, int chatcount )
{
	int modifypos = -1;
	for ( int i = 0; i < mSpeakNum ; i++ )
	{
		if( playerid == mAutoMessBuff[i].PlayerId )
		{
			modifypos = i;
			break;
		}
	}
	if ( modifypos == -1 )
	{
		InsertAutoMess( playerid, pMess, time, channel, chatcount );
		return 0;
	}
    int MaxMessLen = sizeof( mAutoMessBuff[modifypos].MessBuff );
	memset( mAutoMessBuff[modifypos].MessBuff, 0, MaxMessLen );
	strncpy( mAutoMessBuff[modifypos].MessBuff, pMess , MaxMessLen -1 );

    mAutoMessBuff[modifypos].count = time * chatcount + mAutoMessBuff[modifypos].count % mAutoMessBuff[modifypos].time % time ;   
    mAutoMessBuff[modifypos].time = time;
	mAutoMessBuff[modifypos].channel = channel ;
	return 1;
}

//当玩家下线或者停止自动喊话的的时候删除玩家的自动喊话内容
int CAutoMsg::DelAutoMess( int playerid,int iscurrentplayer )  
{
	if ( mSpeakNum <= 0 )
	{
		return 0;
	}

	if ( iscurrentplayer == 1 )
	{
		mSpeakNum--;
		mCurChatPos--;

		memcpy(  &mDeletedChat, &mAutoMessBuff[ mCurChatPos ],sizeof(sAutoMsg));

		if ( mSpeakNum == 0 || mCurChatPos >= mSpeakNum  )
		{
			return 1;
		}		

		memcpy( &mAutoMessBuff[ mCurChatPos ], &mAutoMessBuff[mSpeakNum],sizeof(sAutoMsg));

		return 1;
	}
	for (int i=0;i<mSpeakNum;i++)
	{
		if( mAutoMessBuff[i].PlayerId == playerid )
		{

			mSpeakNum--;			
			if ( i >= mSpeakNum )
			{		
				return  1; 
			}
            memcpy( &mDeletedChat, &mAutoMessBuff[ mCurChatPos ],sizeof( sAutoMsg ) );
			memcpy( &mAutoMessBuff[i], &mAutoMessBuff[mSpeakNum],sizeof( sAutoMsg ) );
			return 1;
		}
	}
	return 0;
}

int CAutoMsg::ChangeChatCount( int playerid, int count )
{
	
	if ( mDeletedChat.PlayerId == playerid )
	{
		if ( mSpeakNum <= 0 )
		{
			mSpeakNum = 0;				
		}	
		
		if ( mDeletedChat.count / mDeletedChat.time  == 0  )
		{
			memcpy( &mAutoMessBuff[mSpeakNum], & mDeletedChat ,sizeof( sAutoMsg ) );
			mAutoMessBuff[mSpeakNum].count = mDeletedChat.time + count +1; 
			mSpeakNum ++;
		}

		return 1;
	}
	for ( int i=0; ( i< mSpeakNum )&&( sizeof(mAutoMessBuff)/sizeof( mAutoMessBuff[0]) ) ; i++ )
	{
		if ( mAutoMessBuff[i].PlayerId == playerid )
		{
			mAutoMessBuff[i].count = mAutoMessBuff[i].count + count +1 ;
			return 1;
		}
	}
	return -1;
}



