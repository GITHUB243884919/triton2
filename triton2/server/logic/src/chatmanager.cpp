#include "stdafx.hpp"
#include "chat.h"
#include "chatmanager.h"
#include "sceneobj_manager.h"
#include "westserver.h"		
#include "worldmodule.h"


template<> CChatManager* CSingleton< CChatManager >::spSingleton = NULL;
CSharedMem* CChatManager::mShmPtr = NULL; 
//
//CBugleChatMess::CBugleChatMess()
//{	
//	if ( CObj::msCreateMode )
//	{
//		Initialize();
//	}
//	else
//	{
//		Resume();
//	}  	
//}
//int CBugleChatMess::Initialize( )
//{
//	mCurrentCur  = 0;
//	mInsertPos   = 0;
//	mPlayerCount = 0;
//	memset( mBugleChat, 0, sizeof( mBugleChat ) );
//	return 0;
//}	
//int CBugleChatMess::Resume()
//{
//	return 0;
//}	    
//int CBugleChatMess::InsertBugleChat( int vCharID, const char * playername, const char *pMessage, const char *pItemInfo /* = NULL */, int vItemBuffLen /* = 0 */ )
//{	
//	if ( mPlayerCount == MAXBUGLECHATNUM )
//	{
//		return -1;
//	}
//	else
//	{		
//		memset( mBugleChat[mInsertPos].PlayerName, 0, sizeof( mBugleChat[mInsertPos].PlayerName ) );
//		memset( mBugleChat[mInsertPos].MessBuff, 0, sizeof( mBugleChat[mInsertPos].MessBuff ) );
//		strncpy( mBugleChat[mInsertPos].MessBuff,pMessage,sizeof( mBugleChat[mInsertPos].MessBuff )-1 );
//		strncpy( mBugleChat[mInsertPos].PlayerName,playername,sizeof( mBugleChat[mInsertPos].PlayerName )-1 );
//		mBugleChat[mInsertPos].PlayerCharID = vCharID;
//		mBugleChat[mInsertPos].ItemBuffLen = 0;
//		if ( pItemInfo != NULL && vItemBuffLen > 0 && vItemBuffLen < MAX_ITEMBUFF_LENGTH )
//		{	
//			memcpy( mBugleChat[mInsertPos].ItemBuff, pItemInfo, vItemBuffLen );
//			mBugleChat[mInsertPos].ItemBuffLen = vItemBuffLen;
//		}
//				     
//		mInsertPos = ( mInsertPos+1 ) % MAXBUGLECHATNUM;
//		mPlayerCount++;
//		return 1;
//	}	   
//}
//BugleMess * CBugleChatMess::GetCurrentChat( )
//{
//	int pos;
//	if ( mPlayerCount == 0 )
//	{
//		return NULL;
//	}
//	pos = mCurrentCur;
//	mCurrentCur = ( mCurrentCur+1 ) % MAXBUGLECHATNUM;
//	mPlayerCount--;
//	return &mBugleChat[ pos ];
//}
//int CBugleChatMess::GetPlayerCount( )
//{
//	return mPlayerCount;
//}
//
//




//CMessage * ChatCallBackMsg::SetChatCallBackMsg( int vChannel, const char* vInfor, const char* vSrcName, const char*vItemInfo = NULL, int vItemBuffLen = 0, const char* vTargetName = NULL ,int vSend2Player = 1, int vRouterType = CMessageRouter::ROUTER_NAME  , int vShowInTalble = 1, int vDesCharID = 0 ,int vSrcCharID = 0 , enUserType enType = USERTYPE_NONE, enUserType enDesType = USERTYPE_NONE )
//{		
//	ChatCallBack.set_channel( vChannel );
//	if ( vInfor != NULL )
//	{
//		ChatCallBack.set_msg( vInfor );
//	}
//	if (enType != USERTYPE_NONE) // 为了节省一点点流量  如果是NONE 就不设置这个字段
//		ChatCallBack.set_usertype( enType );
//	if (enDesType != USERTYPE_NONE) // 为了节省一点点流量  如果是NONE 就不设置这个字段
//		ChatCallBack.set_desusertype( enDesType );
//	ChatCallBack.set_showintable( vShowInTalble ); 
//	if ( vTargetName != NULL )
//	{
//		ChatCallBack.mutable_router( )->set_desname( vTargetName );
//	}
//	if ( vSrcName != NULL )
//	{
//		ChatCallBack.mutable_router( )->set_srcname( vSrcName );
//	}		
//	/*		if ( vItemInfo != NULL && vItemBuffLen > 0 )
//	{
//	ChatCallBack.set_iteminfo( vItemInfo, vItemBuffLen );
//	}	*/	
//	ChatCallBack.mutable_router( )->set_srccharid( vSrcCharID );
//	ChatCallBack.mutable_router( )->set_descharid( vDesCharID );
//	ChatCallBack.mutable_router( )->set_routertype( vRouterType );
//	ChatCallBack.mutable_router( )->set_send2player( vSend2Player );
//	if ( vChannel == CHANNEL_P2P )
//	{
//		ChatCallBack.mutable_router( )->set_send2player( 0 );
//	}
//	else
//	{
//		ChatCallBack.mutable_router( )->set_send2player( vSend2Player );
//	}  		
//	MessageHead.mutable_msghead()->set_messageid( ID_S2C_CHATSENDCALLBACK );
//	MessageHead.set_msgpara( ( int )&ChatCallBack );
//	return &MessageHead;
//};

CMessage * ChatCallBackMsg::SetChatCallBackMsg( CChatContent* pChatContent )
{
	ChatCallBack.set_channel( pChatContent->mChannel );
	if ( pChatContent->mMsg.c_str() != NULL )
	{
		ChatCallBack.set_msg( pChatContent->mMsg.c_str() );
	}
	if ( pChatContent->mSrcPlayerType != USERTYPE_NONE) // 为了节省一点点流量  如果是NONE 就不设置这个字段
		ChatCallBack.set_usertype( pChatContent->mSrcPlayerType );

	if ( pChatContent->mDstPlayerType != USERTYPE_NONE) // 为了节省一点点流量  如果是NONE 就不设置这个字段
		ChatCallBack.set_desusertype( pChatContent->mDstPlayerType );

	if ( pChatContent->mDstCharName.c_str() != NULL )
	{
		ChatCallBack.mutable_router( )->set_desname( pChatContent->mDstCharName.c_str()  );
	}
	if ( pChatContent->mSrcCharName.c_str()  != NULL )
	{
		ChatCallBack.mutable_router( )->set_srcname( pChatContent->mSrcCharName.c_str()  );
	}
	ChatCallBack.mutable_router( )->set_srccharid( pChatContent->mSrcCharID );
	ChatCallBack.mutable_router( )->set_descharid( pChatContent->mDstCharID );

	if ( pChatContent->mChannel == CHANNEL_P2P )
	{
		ChatCallBack.mutable_router( )->set_send2player( 0 );
	}
	else
	{
		ChatCallBack.mutable_router( )->set_send2player( 1 );
	}

	if( pChatContent->mChannel == CHANNEL_BUGLE )
	{
		ChatCallBack.set_showintable( 0 ); 
	}

	if( pChatContent->mChannel == CHANNEL_COMMON || pChatContent->mChannel == CHANNEL_P2P )
	{
		ChatCallBack.mutable_router(  )->set_routertype( CMessageRouter::ROUTER_NAME );
	}
	else
	{
		ChatCallBack.mutable_router(  )->set_routertype( CMessageRouter::ROUTER_CHAR );
	}

	if( (pChatContent->mItemNum > 0) && (pChatContent->mSrcEntityID != 0) )
	{
		CEntityPlayer* pPlayer = (CEntityPlayer*) CSceneObjManager::GetSingletonPtr()->GetObject( pChatContent->mSrcEntityID );
		if( pPlayer != NULL )
		{
			CPropertyPlayer* pProperty = (CPropertyPlayer*) pPlayer->GetProperty();			
			for( int i = 0; i < pChatContent->mItemNum; i++ )
			{
				CItemObject* pItemObj = NULL;
				if( pChatContent->mPkgIdx[i] == SLOT_BAGGAGE )
				{
					pItemObj = pProperty->GetBaggage()->GetItemObjPtr( pChatContent->mItemIdx[i] );
				}
				else if( pChatContent->mPkgIdx[i] == SLOT_EQUIPMENT )
				{
					pItemObj = pProperty->GetEquipment()->GetItemObjPtr( pChatContent->mItemIdx[i] );
				}
				
				if ( pItemObj == NULL )				
				{
					continue;
				}	  		
				PBItemObject* pPBItemObj = ChatCallBack.add_iteminfo();								
				pItemObj->GetItemBinaryBuffer2( pPBItemObj );
				pPBItemObj->set_index( pChatContent->mItemSeq[i] );				
			}
		}
	}

	MessageHead.mutable_msghead()->set_messageid( ID_S2C_CHATSENDCALLBACK );
	MessageHead.set_msgpara( ( int )&ChatCallBack );
	return &MessageHead;
}





CChatManager::CChatManager( )
{	
	if ( mShmPtr->GetInitMode() == SHM_INIT )
	{
	
	} 
	else
	{
		
	}	
}

CChatManager::~CChatManager( )
{
}

unsigned int CChatManager::CountSize( )
{
	return sizeof( CChatManager );
}


void* CChatManager::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CChatManager::operator delete( void* pointer ) { }

void* CChatManager::operator new(size_t size, const char* file, int line)
{
	return operator new( size );
}
void CChatManager::operator delete(void* pointer, const char* file, int line) {}

CChat* CChatManager::GetChat( CEntityPlayer *pPlayer )
{	  
	LK_ASSERT( pPlayer != NULL, return NULL );	
	CChat *tpChat = ( CChat * )CSceneObjManager::GetSingleton().GetObject( pPlayer->GetChatObjID() );
	return tpChat;
}

void CChatManager::Erase( CEntityPlayer *pPlayer )
{
	CSceneObjManager::GetSingletonPtr( )->DestroyObject( pPlayer->GetChatObjID() );		
}

CChat* CChatManager::AddPlayer( CEntityPlayer *pPlayer )
{ 
	LK_ASSERT( pPlayer != NULL, return NULL );
	CChat* tpChat = ( CChat* ) CSceneObjManager::GetSingletonPtr( )->CreateObject( OBJTYPE_CHAT );
	if ( tpChat != NULL )
	{
		pPlayer->SetChatObjID( tpChat->get_id() );
	}
	return tpChat;	
}	   

// 插入一个玩家特殊身份信息
int CChatManager::InsertChatUser( CChatUser &rChatUser )
{
	mChatUserList.push_back( rChatUser );
	return SUCCESS;
}


int CChatManager::GetChatUserInfo( CEntityPlayer *pPlayer, CChatUser &rChatUser )
{	
	memset( &rChatUser, 0, sizeof( rChatUser ) );
	LK_ASSERT( pPlayer != NULL, return -1 )
	CPropertyPlayer *tpProperty = ( CPropertyPlayer * )pPlayer->GetProperty();
	CChatUserList::iterator it = mChatUserList.begin();
	for ( ; it != mChatUserList.end(); ++it )
	{
		if ( (*it).mPriority <= rChatUser.mPriority )
		{
			continue;
		}
		
		// 判断玩家有没有改标志
		switch( (*it).mType )
		{
			case  USERTYPE_OFFICE:
			{
				if ( pPlayer->GetOfficialID() == (*it).mFlag )
				{
					memcpy( &rChatUser, it, sizeof( CChatUser ) );					
				}
				break;
			}			
			case USERTYPE_TITLE:
			{
				if ( tpProperty->HasTitle( (*it).mFlag ) == true )
				{
					memcpy( &rChatUser, it, sizeof( CChatUser ) );
				}
				break;
			}
			case USERTYPE_KING:
			{
				int tCampID = pPlayer->GetNationality();
				int tRoleID = CWorldModule::GetSingleton().mKingID[tCampID][COUNTRY_CHANGAN];
				if ( pPlayer->GetCharID() == (unsigned int)tRoleID )
				{
					memcpy( &rChatUser, it, sizeof( CChatUser ) );
				}
			}
			default:
				break;
		}
		
	}
	return SUCCESS;
}
