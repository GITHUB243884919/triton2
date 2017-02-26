#include "stdafx.hpp"
#include "sceneobj_manager.h"
#include "sceneobj_manager_imp.h"


template<> CSceneObjManager* CSingleton< CSceneObjManager >::spSingleton = NULL;

CSharedMem* CSceneObjManager::mShmPtr = NULL;

//static const char* EGlobalObjTypeName[] =
//{
//	"OBJTYPE_START" ,			// start 
//	"OBJTYPE_ENTITY_PLAYER",	//1
//	"OBJTYPE_ENTITY_FUNCNPC",	//2
//	"OBJTYPE_ENTITY_PET",		//3
//	"OBJTYPE_ENTITY_PEDAL",		//4
//	"OBJTYPE_ENTITY_OGRE",		//5
//	"OBJTYPE_ENTITY_MONEY",		//6
//	"OBJTYPE_ENTITY_ITEM",		//7
//	"OBJTYPE_CREATOR_OGRE",		//8
//	"OBJTYPE_CREATOR_BOX",		//9
//	"OBJTYPE_CREATOR_NPC",		//10
//	"OBJTYPE_CREATOR_ITEM",		//11
//	"OBJTYPE_CREATOR_MONEY",	//12
//	"OBJTYPE_CREATOR_PEDAL",	//13
//	"OBJTYPE_MAP_NPC",			//14
//	"OBJTYPE_MAP_ENTITY",		//15
//	"OBJTYPE_MAP_PEDAL",		//16
//	"OBJTYPE_ITEM_BOOK",		//17
//	"OBJTYPE_ITEM_SHOUT",		//18
//	"OBJTYPE_ITEM_AUTOSHOUT",	//19
//	"OBJTYPE_ITEM_EQUIPUPG",	//20
//	"OBJTYPE_ITEM_EXP",			//21
//	"OBJTYPE_ITEM_PHYSIC",		//22
//	"OBJTYPE_ITEM_RELIFE",		//23
//	"OBJTYPE_ITEM_RETURN",		//24
//	"OBJTYPE_ITEM_SKILL",		//25
//	"OBJTYPE_ITEM_TELEPORT",	//26
//	"OBJTYPE_ITEM_EQUIPMENT",	//27
//	"OBJTYPE_ITEM_TASK",		//28
//	"OBJTYPE_ITEM_SPECIAL_TASK",//29		
//	"OBJTYPE_ITEM_STUFF",		//30
//	"OBJTYPE_ITEM_NOTE",		//31
//	"OBJTYPE_ITEM_RUNE",		//32
//	"OBJTYPE_ITEM_JEWEL",		//33
//	"OBJTYPE_ITEM_YUANBUG",		//34
//	"OBJTYPE_ITEM_YUANEGG",		//35
//	"OBJTYPE_TEAM",				//36
//	"OBJTYPE_RELIVEPOSITION",	//37
//	"OBJTYPE_MAP_L_OBJECT",		//38
//	"OBJTYPE_MAP_M_OBJECT",		//39
//	"OBJTYPE_MAP_S_OBJECT",		//40
//	"OBJTYPE_POLICY_NPC",		//41
//	"OBJTYPE_CHAT",				//42	
//	"OBJTYPE_TRADE",			//43
//	"OBJTYPE_MAIL_MAILINFOR",	//44
//	"OBJTYPE_MAIL_MAILCACHE",	//45
//	"OBJTYPE_ITEM_HORSE",		//46
//	"OBJTYPE_ITEM_BINDSTONE",	//47
//	"OBJTYPE_TASKLIST",			//48
//	"OBJTYPE_STALL",			//49
//	"OBJTYPE_SESSION",			//50
//	"OBJTYPE_ITEM_SCROLL",		//51
//	"OBJTYPE_ENTITY_COLLECT",	//52
//	"OBJTYPE_CREATOR_COLLECT",	//53
//	"OBJTYPE_ITEM_CARD",		//54
//	"OBJTYPE_REPETION",			//55
//	"OBJTYPE_ENTITY_STOREBOX",			//56
//	"OBJTYPE_DELAYACTION",		//57
//	"OBJTYPE_RETURNPAPER",		//58
//	"OBJTYPE_TELESYMBOL",		//59
//	"OBJTYPE_ITEMSTOVE",		//60
//	"OBJTYPE_ENTITY_EXPREDSTONE",		//61
//	"OBJTYPE_ANSWERSTATUS",		//62
//	"OBJTYPE_ITEM_QUESTIONPAPER",//63
//	"OBJTYPE_ITEM_MINEMAP",		//64
//	"OBJTYPE_ITEM_KITBAG",		//65
//	"OBJTYPE_ITEM_TOKEN",		//66
//	"OBJTYPE_ENTITY_BLOCK"		//67
//	"OBJTYPE_CREATOR_BLOCK"		//68
//	"OBJTYPE_ITEM_CALL",		//69
//	"OBJTYPE_ITEM_REMOVE"
//	"OBJTYPE_ITEM_CHANGE"
//	"OBJTYPE_ITEM_GIFTBAG"
//	"OBJTYPE_END",				// end
//};

CSceneObjManager::CSceneObjManager()
{
	if ( mShmPtr->GetInitMode() == SHM_INIT )
	{
		Initailize();
	} 
	else
	{
		Resume();
	}
}

CSceneObjManager::~CSceneObjManager()
{

}

void* CSceneObjManager::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CSceneObjManager::operator delete( void* pointer ) 
{ 
}

void* CSceneObjManager::operator new(size_t size, const char* file, int line)
{
	return operator new(size);
}
void CSceneObjManager::operator delete(void* pointer, const char* file, int line)
{

}

unsigned int CSceneObjManager::CountSize( )
{
	LOG_INFO( "default", "DataMng_SharedCellsEntityID:	%10d", sizeof( DataMng_SharedCellsEntityID ));

	unsigned int tSize = sizeof( CSceneObjManager );

	tSize += CSceneObjManagerImp::CountSize();

	return tSize;
}

int CSceneObjManager::Initailize()
{
	CSceneObjManagerImp::mShmPtr = mShmPtr;
	mSceneObjManagerImp = new CSceneObjManagerImp;
	mSharedCellsEntityID.initailize_pool( );

	mSharedSeenPlayers.initailize_pool( );
	mSharedSeenCharNpcs.initailize_pool( );
	mSharedSeenStaticNpcs.initailize_pool( ); 

	mIncreasingCount = 0;

	return 0;
}

int CSceneObjManager::Resume()
{
	CSceneObjManagerImp::mShmPtr = mShmPtr;
	mSceneObjManagerImp = new CSceneObjManagerImp;

	return 0;
}

unsigned short CSceneObjManager::GetGUID( )
{	
	if ( mIncreasingCount >= 0XFFFE )
	{
		mIncreasingCount = 0;
	}
	return mIncreasingCount++;
}

CObj* CSceneObjManager::CreateObject( EGlobalObjType vObjType )
{
	CObj* pObj = NULL;

	switch( vObjType )
	{
	case OBJTYPE_ENTITY_PLAYER:
		{
			pObj = mSceneObjManagerImp->mEntityPlayerPool.create();
			break;
		}
	case OBJTYPE_ENTITY_FUNCNPC:
		{
			pObj = mSceneObjManagerImp->mEntityNpcPool.create();
			break;
		}
	case OBJTYPE_ENTITY_OGRE:
		{
			pObj = mSceneObjManagerImp->mEntityOgrePool.create();
		}
		break;
	case OBJTYPE_ENTITY_ITEM:
		{
			pObj = mSceneObjManagerImp->mEntityItemPool.create();
		}
		break;
	case OBJTYPE_CREATOR_OGRE:
		{
			pObj = mSceneObjManagerImp->mOgreCreatorPool.create();
			break;
		}
	case OBJTYPE_CREATOR_BOX:
		{
			pObj = mSceneObjManagerImp->mBoxCreatorPool.create( );
			break;
		}
	case OBJTYPE_CREATOR_NPC:
		{
			pObj = mSceneObjManagerImp->mNpcCreatorPool.create();
		}
		break;
	case OBJTYPE_CREATOR_ITEM:
		{
			pObj = mSceneObjManagerImp->mItemCreatorPool.create();
			break;
		}
	case OBJTYPE_CREATOR_PEDAL:
		{
			pObj = mSceneObjManagerImp->mPedalCreatorPool.create();
			break;
		}
	case OBJTYPE_ITEM_BOOK:
		{
			pObj = mSceneObjManagerImp->mItemBook.create();
			break;
		}
	case OBJTYPE_ITEM_EQUIPUPG:
		{
			pObj = mSceneObjManagerImp->mItemEquipUpg.create();
			break;
		}
	case OBJTYPE_ITEM_EXP:
		{
			pObj = mSceneObjManagerImp->mItemExp.create();
			break;
		}
	case OBJTYPE_ITEM_PHYSIC:
		{
			pObj = mSceneObjManagerImp->mItemPhysic.create();
			break;
		}
	case OBJTYPE_ITEM_RELIFE:
		{
			pObj = mSceneObjManagerImp->mItemRelife.create();
			break;
		}
	case OBJTYPE_ITEM_RETURN:
		{
			pObj = mSceneObjManagerImp->mItemReturn.create();
			break;
		}
	case OBJTYPE_ITEM_SKILL:
		{
			pObj = mSceneObjManagerImp->mItemSkill.create();
			break;
		}
	case OBJTYPE_ITEM_TELEPORT:
		{
			pObj = mSceneObjManagerImp->mItemTeleport.create();
			break;
		}
	case OBJTYPE_ITEM_EQUIPMENT:
		{
			pObj = mSceneObjManagerImp->mItemEquipment.create();
			break;
		}
	case OBJTYPE_ITEM_TASK:
		{
			pObj = mSceneObjManagerImp->mItemTask.create();
			break;
		}
	case OBJTYPE_ITEM_SPECIAL_TASK:
		{
			pObj = mSceneObjManagerImp->mItemSpecialTask.create();
			break;
		}
	case OBJTYPE_ITEM_STUFF:
		{
			pObj = mSceneObjManagerImp->mItemStuff.create();
			break;
		}
	case OBJTYPE_ITEM_NOTE:
		{
			pObj = mSceneObjManagerImp->mItemNote.create();
			break;
		}
	case OBJTYPE_ITEM_RUNE:
		{
			pObj = mSceneObjManagerImp->mItemRune.create();
			break;
		}
	case OBJTYPE_ITEM_JEWEL:
		{
			pObj = mSceneObjManagerImp->mItemJewel.create();
			break;
		}
	case OBJTYPE_ITEM_YUANBUG:
		{
			pObj = mSceneObjManagerImp->mItemYuanBug.create();
			break;
		}
	case OBJTYPE_ITEM_YUANEGG:
		{
			pObj = mSceneObjManagerImp->mItemYuanEgg.create();
			break;
		}
	case OBJTYPE_ITEM_SHOUT:
		{
			pObj = mSceneObjManagerImp->mItemShout.create();
			break;
		}
	case OBJTYPE_ITEM_AUTOSHOUT:
		{
			pObj = mSceneObjManagerImp->mItemAutoShout.create();
			break;
		}
	case OBJTYPE_RELIVEPOSITION:
		{
			pObj = mSceneObjManagerImp->mRelivePosition.create();
			break;
		}
	case OBJTYPE_MAP_L_OBJECT:
		{
			pObj = mSceneObjManagerImp->mLMapObject.create();
			break;
		}
	case OBJTYPE_MAP_M_OBJECT:
		{
			pObj = mSceneObjManagerImp->mMMapObject.create();
			break;
		}
	case OBJTYPE_MAP_S_OBJECT:
		{
			pObj = mSceneObjManagerImp->mSMapObject.create();
			break;
		}
	case OBJTYPE_POLICY_NPC:
		{
			pObj = mSceneObjManagerImp->mNpcPolicy.create();
			break;
		}
	case OBJTYPE_CHAT:
		{
			pObj = mSceneObjManagerImp->mChat.create( );
			break;
		}
	case OBJTYPE_TEAM:
		{
			pObj = mSceneObjManagerImp->mTeam.create( );
			break;
		}
	case OBJTYPE_ENTITY_PET:
		{
			pObj = mSceneObjManagerImp->mEntityPetPool.create( );
			break;
		}
	case OBJTYPE_TRADE:
		{
			pObj = mSceneObjManagerImp->mTrade.create( );
			break ;
		}
	case OBJTYPE_ENTITY_PEDAL:
		{
			pObj = mSceneObjManagerImp->mEntityPedalPool.create( );
			break;
		}
	case OBJTYPE_MAIL_MAILINFOR:
		{
			pObj = mSceneObjManagerImp->mMailInfor.create();
			break;
		}
	case OBJTYPE_MAIL_MAILCACHE:
		{
			pObj = mSceneObjManagerImp->mMailCache.create();
			break;
		}
	case OBJTYPE_ITEM_HORSE:
		{
			pObj = mSceneObjManagerImp->mItemHorse.create();
			break;
		}
	case OBJTYPE_ENTITY_STOREBOX:
		{
			pObj = mSceneObjManagerImp->mStoreBox.create( );	
		}
		break;
	case OBJTYPE_ITEM_BINDSTONE:
		{
			pObj = mSceneObjManagerImp->mItemBindStone.create();
			break;
		}
	case OBJTYPE_TASKLIST:
		{
			pObj = mSceneObjManagerImp->mTaskList.create();
			break;
		}
	case OBJTYPE_STALL:
		{
			pObj = mSceneObjManagerImp->mStall.create();
			break;
		}
	case OBJTYPE_SESSION:
		{
			pObj = mSceneObjManagerImp->mSession.create();
			break;
		}
	case OBJTYPE_ITEM_SCROLL:
		{
			pObj = mSceneObjManagerImp->mItemScroll.create();
			break;
		}
	case OBJTYPE_ITEM_CARD:
		{
			pObj = mSceneObjManagerImp->mItemCard.create();
			break;
		}
	case OBJTYPE_ITEM_CALL:
		{
			pObj = mSceneObjManagerImp->mItemCall.create();
			break;
		}
	case OBJTYPE_ENTITY_COLLECT:
		{
			pObj = mSceneObjManagerImp->mEntityCollectPool.create();
			break;
		}
	case OBJTYPE_CREATOR_COLLECT:
		{
			pObj = mSceneObjManagerImp->mCollectCreatorPool.create();
			break;
		}
	case OBJTYPE_REPETION:
		{
			pObj = mSceneObjManagerImp->mRepetion.create();
			break;
		}

	case OBJTYPE_RETURNPAPER:
		{
			pObj = mSceneObjManagerImp->mItemPaper.create();
			break;
		}
	case OBJTYPE_TELESYMBOL:
		{
			pObj = mSceneObjManagerImp->mItemSymbol.create();
			break;
		}
	case OBJTYPE_ITEMSTOVE:
		{
			pObj = mSceneObjManagerImp->mStove.create();
			break;
		}
	case OBJTYPE_ENTITY_EXPREDSTONE:
		{
			pObj = mSceneObjManagerImp->mExpRedStone.create();
			break;	
	 	}
	case OBJTYPE_ANSWERSTATUS:
		{
			pObj = mSceneObjManagerImp->mAnswerStatus.create( );
			break;
		}
	case OBJTYPE_ITEM_QUESTIONPAPER:
		{
			pObj = mSceneObjManagerImp->mQuestionPaper.create( );
			break;
		}
	case OBJTYPE_ITEM_MINEMAP:
		{
			pObj = mSceneObjManagerImp->mMineMap.create();
			break;
		}
	case OBJTYPE_ITEM_KITBAG:
		{
			pObj = mSceneObjManagerImp->mKitBag.create( );
			break;
		}
	case OBJTYPE_ENTITY_BLOCK:
		{
			pObj = mSceneObjManagerImp->mBlock.create( );
		}
		break;
	case OBJTYPE_CREATOR_BLOCK:
		{
			pObj = mSceneObjManagerImp->mBlockCreator.create( );	
		}
		break;
	case OBJTYPE_ITEM_TOKEN:
		{
			pObj = mSceneObjManagerImp->mToken.create();
			break;
		}
	case OBJTYPE_ITEM_REMOVE:
		{
			pObj = mSceneObjManagerImp->mRemove.create();
			break;
		}
	case OBJTYPE_ITEM_CHANGE:
		{
			pObj = mSceneObjManagerImp->mChange.create();
			break;
		}
	case OBJTYPE_ITEM_GIFTBAG:
		{
			pObj = mSceneObjManagerImp->mItemGiftBag.create();
			break;
		}
	case OBJTYPE_ITEM_MAGICSTONE:
		{
			pObj = mSceneObjManagerImp->mMagicStone.create( );
			break;
		}
	case OBJTYPE_ITEM_LIFESKILLSCROLL:
		{
			pObj = mSceneObjManagerImp->mLifeSkillScroll.create( );
			break;
		}
	case OBJTYPE_ITEM_MAGICWEAPON:
		{
			pObj = mSceneObjManagerImp->mItemMagicWeapon.create();
			break;
		}
	case OBJTYPE_ITEM_FUNCITEM:
		{
			pObj = mSceneObjManagerImp->mItemFuncItem.create();
			break;
		}
	case OBJTYPE_ITEM_SKILLBOOK:
		{
			pObj = mSceneObjManagerImp->mItemSkillBook.create();
			break;
		}
	case OBJTYPE_FAMILYINFO:
		{
			pObj = mSceneObjManagerImp->mFamilyInfo.create( );
			break;
		}
	case OBJTYPE_IBTRANS:
		{
			pObj = mSceneObjManagerImp->mIbTransPool.create();
			break;
		}
	case OBJTYPE_ITEM_REDPAPER:
		{
			pObj = mSceneObjManagerImp->mRedPaper.create( );
			break;
		}
	case OBJTYPE_ACHIEVE:
		{
			pObj = mSceneObjManagerImp->mAchieve.create( );
			break;
		}
	case OBJTYPE_ITEM_VIP:
		{
			pObj = mSceneObjManagerImp->mItemVip.create( );
			break;
		}
	case OBJTYPE_ITEM_CLEAN:
		{
			pObj = mSceneObjManagerImp->mItemClean.create( );
			break;
		}
	case OBJTYPE_ITEM_LEAGUEBAG:
		{
			pObj = mSceneObjManagerImp->mLeagueBag.create( );
			break;
		}
	case OBJTYPE_REDSTONECONTROLLER:
		{
			pObj = mSceneObjManagerImp->mRedstoneController.create( );
			break;
		}
	case OBJTYPE_ITEM_INVITATION:
		{
			pObj = mSceneObjManagerImp->mInvitation.create( );
			break;
		}
	case OBJTYPE_MARRIAGEINFO:
		{
			pObj = mSceneObjManagerImp->mMarriageInfo.create( );
			break;
		}
	case OBJTYPE_ITEM_SYSINVITATION:
		{
			pObj = mSceneObjManagerImp->mSysInvitation.create( );
			break;
		}	
	case OBJTYPE_CREATOR_PET:
		{
			pObj = mSceneObjManagerImp->mPetCreator.create( );
			break;		
		}
	case OBJTYPE_ITEM_BLACKPILLS:
		{
			pObj = mSceneObjManagerImp->mBlackPills.create();
			break;		
		}
	case OBJTYPE_ITEM_EXPCONTAINER:
		{
			pObj = mSceneObjManagerImp->mExpContainer.create();
			break;
		}
	case OBJTYPE_ITEM_EXPPILLS:
		{
			pObj = mSceneObjManagerImp->mExpPills.create();
			break;
		}
	case OBJTYPE_ENTITY_GHOST:
		{
			pObj = mSceneObjManagerImp->mEntityGhostPool.create();
			break;
		}
	case OBJTYPE_CREATOR_GHOST:
		{

			pObj = mSceneObjManagerImp->mGhostCreatorPool.create();
			break;
		}
	case OBJTYPE_STALL_RECORD:
		{
			pObj = mSceneObjManagerImp->mStallRecord.create();
			break;
		}
	case OBJTYPE_FUNCTRANS:
		{
			pObj = mSceneObjManagerImp->mFuncTrans.create();
			break;
		}
	default:
		{
			//TODO: log it 
			break;
		}
	}

//	if ( pObj == NULL )
//	{
//		LOG_ERROR( "default" , "Create Object type(%d: %s ) failed",
//					vObjType , EGlobalObjTypeName[vObjType-1]);
//	}
//#ifdef _DEBUG_
//	else
//	{
//		LOG_DEBUG( "default" , "Create Object(%d) type(%d: ) OK", pObj->get_id(), vObjType );
//	}
//#endif	

	return pObj;
}


int CSceneObjManager::DestroyObject( int vObjID )
{

	switch( CObj::ID2TYPE( vObjID )  )
	{
	case OBJTYPE_ENTITY_PLAYER:
		{
			mSceneObjManagerImp->mEntityPlayerPool.remove( vObjID );
			break;
		}
	case OBJTYPE_ENTITY_FUNCNPC: // 专门用于移除任务护送NPC
		{
			mSceneObjManagerImp->mEntityNpcPool.remove( vObjID );
			break;
		}
	case OBJTYPE_ENTITY_OGRE:
		{
			mSceneObjManagerImp->mEntityOgrePool.remove( vObjID );
			break;
		}
	case OBJTYPE_ENTITY_ITEM:
		{
			mSceneObjManagerImp->mEntityItemPool.remove( vObjID );
			break;
		}
	case OBJTYPE_CREATOR_OGRE:
		{
			mSceneObjManagerImp->mOgreCreatorPool.remove( vObjID );
			break;
		}
	case OBJTYPE_CREATOR_BOX:
		{
			mSceneObjManagerImp->mBoxCreatorPool.remove( vObjID );
			break;
		}
	case OBJTYPE_CREATOR_NPC:
		{
			mSceneObjManagerImp->mNpcCreatorPool.remove( vObjID );
		}
		break;
	case OBJTYPE_CREATOR_ITEM:
		{
			mSceneObjManagerImp->mItemCreatorPool.remove( vObjID );
			break;
		}

	case OBJTYPE_CREATOR_PEDAL:
		{
			mSceneObjManagerImp->mPedalCreatorPool.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_BOOK:
		{
			mSceneObjManagerImp->mItemBook.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_EQUIPUPG:
		{
			mSceneObjManagerImp->mItemEquipUpg.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_EXP:
		{
			mSceneObjManagerImp->mItemExp.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_PHYSIC:
		{
			mSceneObjManagerImp->mItemPhysic.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_SHOUT:
		{
			mSceneObjManagerImp->mItemShout.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_AUTOSHOUT:
		{
			mSceneObjManagerImp->mItemAutoShout.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_RELIFE:
		{
			mSceneObjManagerImp->mItemRelife.remove( vObjID);
			break;
		}
	case OBJTYPE_ITEM_RETURN:
		{
			mSceneObjManagerImp->mItemReturn.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_SKILL:
		{
			mSceneObjManagerImp->mItemSkill.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_TELEPORT:
		{
			mSceneObjManagerImp->mItemTeleport.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_EQUIPMENT:
		{
			mSceneObjManagerImp->mItemEquipment.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_TASK:
		{
			mSceneObjManagerImp->mItemTask.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_SPECIAL_TASK:
		{
			mSceneObjManagerImp->mItemSpecialTask.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_STUFF:
		{
			mSceneObjManagerImp->mItemStuff.remove( vObjID);
			break;
		}
	case OBJTYPE_ITEM_NOTE:
		{
			mSceneObjManagerImp->mItemNote.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_RUNE:
		{
			mSceneObjManagerImp->mItemRune.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_JEWEL:
		{
			mSceneObjManagerImp->mItemJewel.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_YUANBUG:
		{
			mSceneObjManagerImp->mItemYuanBug.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_YUANEGG:
		{
			mSceneObjManagerImp->mItemYuanEgg.remove( vObjID );
			break;
		}
	case OBJTYPE_RELIVEPOSITION:
		{
			mSceneObjManagerImp->mRelivePosition.remove( vObjID );
			break;
		}
	case OBJTYPE_MAP_L_OBJECT:
		{
			mSceneObjManagerImp->mLMapObject.remove ( vObjID );
			break;
		}
	case OBJTYPE_MAP_M_OBJECT:
		{
			mSceneObjManagerImp->mMMapObject.remove ( vObjID );
			break;
		}
	case OBJTYPE_MAP_S_OBJECT:
		{
			mSceneObjManagerImp->mSMapObject.remove ( vObjID );
			break;
		}
	case OBJTYPE_POLICY_NPC:
		{
			mSceneObjManagerImp->mNpcPolicy.remove ( vObjID );
			break;
		}
	case OBJTYPE_CHAT:
		{
			mSceneObjManagerImp->mChat.remove( vObjID );
			break;
		}
	case OBJTYPE_TEAM:
		{
			mSceneObjManagerImp->mTeam.remove( vObjID );
			break ;
		}
	case OBJTYPE_ENTITY_PET:
		{
			mSceneObjManagerImp->mEntityPetPool.remove( vObjID );
			break;
		}
	case OBJTYPE_TRADE:
		{
			mSceneObjManagerImp->mTrade.remove( vObjID );
			break ;
		}
	case OBJTYPE_ENTITY_PEDAL:
		{
			mSceneObjManagerImp->mEntityPedalPool.remove( vObjID );
			break;
		}		
	case OBJTYPE_MAIL_MAILINFOR:
		{
			mSceneObjManagerImp->mMailInfor.remove( vObjID );
			break;
		}
	case OBJTYPE_MAIL_MAILCACHE:
		{
			mSceneObjManagerImp->mMailCache.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_HORSE:
		{
			mSceneObjManagerImp->mItemHorse.remove( vObjID );
			break;
		}
	case OBJTYPE_ENTITY_STOREBOX:
		{
			mSceneObjManagerImp->mStoreBox.remove( vObjID );
		}
		break;
	case OBJTYPE_ITEM_BINDSTONE:
		{
			mSceneObjManagerImp->mItemBindStone.remove( vObjID );
			break;
		}
	case OBJTYPE_TASKLIST:
		{
			mSceneObjManagerImp->mTaskList.remove( vObjID );
			break;
		}
	case OBJTYPE_STALL:
		{
			mSceneObjManagerImp->mStall.remove( vObjID );
			break;
		}
	case OBJTYPE_SESSION:
		{
			mSceneObjManagerImp->mSession.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_SCROLL:
		{
			mSceneObjManagerImp->mItemScroll.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_CARD:
		{
			mSceneObjManagerImp->mItemCard.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_CALL:
		{
			mSceneObjManagerImp->mItemCall.remove( vObjID );
			break;
		}
	case OBJTYPE_ENTITY_COLLECT:
		{
			mSceneObjManagerImp->mEntityCollectPool.remove( vObjID );
			break;
		}
	case OBJTYPE_CREATOR_COLLECT:
		{
			mSceneObjManagerImp->mCollectCreatorPool.remove( vObjID );
			break;
		}
	case OBJTYPE_REPETION:
		{
			mSceneObjManagerImp->mRepetion.remove( vObjID );
			break;
		}

	case OBJTYPE_RETURNPAPER:
		{
			mSceneObjManagerImp->mItemPaper.remove( vObjID );
			break;
		}
	case OBJTYPE_TELESYMBOL:
		{
			mSceneObjManagerImp->mItemSymbol.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEMSTOVE:
		{
			mSceneObjManagerImp->mStove.remove( vObjID );
			break;
		}
	case OBJTYPE_ENTITY_EXPREDSTONE:
		{
			mSceneObjManagerImp->mExpRedStone.remove( vObjID );
			break;
		}
	case OBJTYPE_ANSWERSTATUS:
		{
			mSceneObjManagerImp->mAnswerStatus.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_QUESTIONPAPER:
		{
			mSceneObjManagerImp->mQuestionPaper.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_MINEMAP:
		{
			mSceneObjManagerImp->mMineMap.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_KITBAG:
		{
			mSceneObjManagerImp->mKitBag.remove( vObjID );
			break;
		}
	case OBJTYPE_ENTITY_BLOCK:
		{
			mSceneObjManagerImp->mBlock.remove( vObjID );
		}
		break;
	case OBJTYPE_CREATOR_BLOCK:
		{
			mSceneObjManagerImp->mBlockCreator.remove( vObjID );
		}
		break;
	case OBJTYPE_ITEM_TOKEN:
		{
			mSceneObjManagerImp->mToken.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_REMOVE:
		{
			mSceneObjManagerImp->mRemove.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_CHANGE:
		{
			mSceneObjManagerImp->mChange.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_GIFTBAG:
		{
			mSceneObjManagerImp->mItemGiftBag.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_MAGICSTONE:
		{
			mSceneObjManagerImp->mMagicStone.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_LIFESKILLSCROLL:
		{
			mSceneObjManagerImp->mLifeSkillScroll.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_MAGICWEAPON:
		{
			mSceneObjManagerImp->mItemMagicWeapon.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_FUNCITEM:
		{
			mSceneObjManagerImp->mItemFuncItem.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_SKILLBOOK:
		{
			mSceneObjManagerImp->mItemSkillBook.remove( vObjID );
			break;
		}
	case OBJTYPE_FAMILYINFO:
		{
			mSceneObjManagerImp->mFamilyInfo.remove( vObjID );
			break;
		}
	case OBJTYPE_IBTRANS:
		{
			mSceneObjManagerImp->mIbTransPool.remove(vObjID);
			break;
		}
	case OBJTYPE_ITEM_REDPAPER:
		{
			mSceneObjManagerImp->mRedPaper.remove( vObjID );
			break;
		}
	case OBJTYPE_ACHIEVE:
		{
			mSceneObjManagerImp->mAchieve.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_VIP:
		{
			mSceneObjManagerImp->mItemVip.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_CLEAN:
		{
			mSceneObjManagerImp->mItemClean.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_LEAGUEBAG:
		{
			mSceneObjManagerImp->mLeagueBag.remove( vObjID );
			break;
		}
	case OBJTYPE_REDSTONECONTROLLER:
		{
			mSceneObjManagerImp->mRedstoneController.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_INVITATION:
		{
			mSceneObjManagerImp->mInvitation.remove( vObjID );
			break;
		}
	case OBJTYPE_MARRIAGEINFO:
		{
			mSceneObjManagerImp->mMarriageInfo.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_SYSINVITATION:
		{
			mSceneObjManagerImp->mSysInvitation.remove( vObjID );
			break;
		}	
	case OBJTYPE_CREATOR_PET:
		{
			mSceneObjManagerImp->mPetCreator.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_BLACKPILLS:
		{
			mSceneObjManagerImp->mBlackPills.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_EXPCONTAINER:
		{
			mSceneObjManagerImp->mExpContainer.remove( vObjID );
			break;
		}
	case OBJTYPE_ITEM_EXPPILLS:
		{
		{	mSceneObjManagerImp->mExpPills.remove( vObjID );
			break;
		}
		}
	case OBJTYPE_ENTITY_GHOST:
		{
			mSceneObjManagerImp->mEntityGhostPool.remove( vObjID );
			break;
		}
	case OBJTYPE_CREATOR_GHOST:
		{

			mSceneObjManagerImp->mGhostCreatorPool.remove( vObjID );
			break;
		}
	case OBJTYPE_STALL_RECORD:
		{
			mSceneObjManagerImp->mStallRecord.remove( vObjID );
			break;
		}
	case OBJTYPE_FUNCTRANS:
		{
			mSceneObjManagerImp->mFuncTrans.remove( vObjID );
			break;
		}
	default:
		{
			//TODO: log it 
			return -1;
		}
	}

	/*LOG_DEBUG( "default", "Destory ObjType(%s)  ObjID %d",
			EGlobalObjTypeName[CObj::ID2TYPE(vObjID)-1], vObjID );*/

	return 0;	
}


CObj* CSceneObjManager::GetObject( int vObjID )
{
	if( vObjID == INVALID_OBJ_ID )
	{
		return NULL;
	}

	CObj* pObj = NULL;

	switch( CObj::ID2TYPE( vObjID )  )
	{
	case OBJTYPE_ENTITY_PLAYER:
		{
			pObj = mSceneObjManagerImp->mEntityPlayerPool.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ENTITY_FUNCNPC:
		{
			pObj = mSceneObjManagerImp->mEntityNpcPool.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ENTITY_ITEM:
		{
			pObj = mSceneObjManagerImp->mEntityItemPool.find_by_id( vObjID );
		}
		break;
	case OBJTYPE_ENTITY_OGRE:
		{
			pObj = mSceneObjManagerImp->mEntityOgrePool.find_by_id( vObjID );
		}
		break;
	case OBJTYPE_CREATOR_OGRE:
		{
			pObj = mSceneObjManagerImp->mOgreCreatorPool.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_CREATOR_BOX:
		{
			pObj = mSceneObjManagerImp->mBoxCreatorPool.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_CREATOR_NPC:
		{
			pObj = mSceneObjManagerImp->mNpcCreatorPool.find_by_id( vObjID );
		}
		break;
	case OBJTYPE_CREATOR_ITEM:
		{
			pObj = mSceneObjManagerImp->mItemCreatorPool.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_CREATOR_PEDAL:
		{
			pObj = mSceneObjManagerImp->mPedalCreatorPool.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_BOOK:
		{
			pObj = mSceneObjManagerImp->mItemBook.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_EQUIPUPG:
		{
			pObj = mSceneObjManagerImp->mItemEquipUpg.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_EXP:
		{
			pObj = mSceneObjManagerImp->mItemExp.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_PHYSIC:
		{
			pObj = mSceneObjManagerImp->mItemPhysic.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_SHOUT:
		{
			pObj = mSceneObjManagerImp->mItemShout.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_AUTOSHOUT:
		{
			pObj = mSceneObjManagerImp->mItemAutoShout.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_RELIFE:
		{
			pObj = mSceneObjManagerImp->mItemRelife.find_by_id( vObjID);
			break;
		}
	case OBJTYPE_ITEM_RETURN:
		{
			pObj = mSceneObjManagerImp->mItemReturn.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_SKILL:
		{
			pObj = mSceneObjManagerImp->mItemSkill.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_TELEPORT:
		{
			pObj = mSceneObjManagerImp->mItemTeleport.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_EQUIPMENT:
		{
			pObj = mSceneObjManagerImp->mItemEquipment.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_TASK:
		{
			pObj = mSceneObjManagerImp->mItemTask.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_SPECIAL_TASK:
		{
			pObj = mSceneObjManagerImp->mItemSpecialTask.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_STUFF:
		{
			pObj = mSceneObjManagerImp->mItemStuff.find_by_id( vObjID);
			break;
		}
	case OBJTYPE_ITEM_NOTE:
		{
			pObj = mSceneObjManagerImp->mItemNote.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_RUNE:
		{
			pObj = mSceneObjManagerImp->mItemRune.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_JEWEL:
		{
			pObj = mSceneObjManagerImp->mItemJewel.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_YUANBUG:
		{
			pObj = mSceneObjManagerImp->mItemYuanBug.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_YUANEGG:
		{
			pObj = mSceneObjManagerImp->mItemYuanEgg.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_RELIVEPOSITION:
		{
			pObj = mSceneObjManagerImp->mRelivePosition.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_MAP_L_OBJECT:
		{
			pObj = mSceneObjManagerImp->mLMapObject.find_by_id ( vObjID );
			break;
		}
	case OBJTYPE_MAP_M_OBJECT:
		{
			pObj = mSceneObjManagerImp->mMMapObject.find_by_id ( vObjID );
			break;
		}
	case OBJTYPE_MAP_S_OBJECT:
		{
			pObj = mSceneObjManagerImp->mSMapObject.find_by_id ( vObjID );
			break;
		}
	case OBJTYPE_POLICY_NPC:
		{
			pObj = mSceneObjManagerImp->mNpcPolicy.find_by_id ( vObjID );
			break;
		}
	case OBJTYPE_CHAT:
		{
			pObj = mSceneObjManagerImp->mChat.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_TEAM:
		{
			pObj = mSceneObjManagerImp->mTeam.find_by_id( vObjID );
			break ;
		}
	case OBJTYPE_ENTITY_PET:
		{
			pObj = mSceneObjManagerImp->mEntityPetPool.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_TRADE:
		{
			pObj = mSceneObjManagerImp->mTrade.find_by_id( vObjID );
			break ;
		}
	case OBJTYPE_ENTITY_PEDAL:
		{
			pObj = mSceneObjManagerImp->mEntityPedalPool.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_MAIL_MAILINFOR:
		{
			pObj = mSceneObjManagerImp->mMailInfor.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_MAIL_MAILCACHE:
		{
			pObj = mSceneObjManagerImp->mMailCache.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_HORSE:
		{
			pObj = mSceneObjManagerImp->mItemHorse.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ENTITY_STOREBOX:
		{
			pObj = mSceneObjManagerImp->mStoreBox.find_by_id( vObjID );
		}
		break;
	case OBJTYPE_ITEM_BINDSTONE:
		{
			pObj = mSceneObjManagerImp->mItemBindStone.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_TASKLIST:
		{
			pObj = mSceneObjManagerImp->mTaskList.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_STALL:
		{
			pObj = mSceneObjManagerImp->mStall.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_SESSION:
		{
			pObj = mSceneObjManagerImp->mSession.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_SCROLL:
		{
			pObj = mSceneObjManagerImp->mItemScroll.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_CARD:
		{
			pObj = mSceneObjManagerImp->mItemCard.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_CALL:
		{
			pObj = mSceneObjManagerImp->mItemCall.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ENTITY_COLLECT:
		{
			pObj = mSceneObjManagerImp->mEntityCollectPool.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_CREATOR_COLLECT:
		{
			pObj = mSceneObjManagerImp->mCollectCreatorPool.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_REPETION:
		{
			pObj = mSceneObjManagerImp->mRepetion.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_RETURNPAPER:
		{
			pObj = mSceneObjManagerImp->mItemPaper.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_TELESYMBOL:
		{
			pObj = mSceneObjManagerImp->mItemSymbol.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEMSTOVE:
		{
			pObj = mSceneObjManagerImp->mStove.find_by_id(vObjID );
			break;
		}
	case OBJTYPE_ENTITY_EXPREDSTONE:
		{
			pObj = mSceneObjManagerImp->mExpRedStone.find_by_id(vObjID );
			break;
		}
	case OBJTYPE_ANSWERSTATUS:
		{
			pObj = mSceneObjManagerImp->mAnswerStatus.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_QUESTIONPAPER:
		{
			pObj = mSceneObjManagerImp->mQuestionPaper.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_MINEMAP:
		{
			pObj = mSceneObjManagerImp->mMineMap.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_KITBAG:
		{
			pObj = mSceneObjManagerImp->mKitBag.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ENTITY_BLOCK:
		{
			pObj = mSceneObjManagerImp->mBlock.find_by_id( vObjID );
		}
		break;
	case OBJTYPE_CREATOR_BLOCK:
		{
			pObj = mSceneObjManagerImp->mBlockCreator.find_by_id( vObjID );
		}
		break;
	case OBJTYPE_ITEM_TOKEN:
		{
			pObj = mSceneObjManagerImp->mToken.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_REMOVE:
		{
			pObj = mSceneObjManagerImp->mRemove.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_CHANGE:
		{
			pObj = mSceneObjManagerImp->mChange.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_GIFTBAG:
		{
			pObj = mSceneObjManagerImp->mItemGiftBag.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_MAGICSTONE:
		{
			pObj = mSceneObjManagerImp->mMagicStone.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_LIFESKILLSCROLL:
		{
			pObj = mSceneObjManagerImp->mLifeSkillScroll.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_MAGICWEAPON:
		{
			pObj = mSceneObjManagerImp->mItemMagicWeapon.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_FUNCITEM:
		{
			pObj = mSceneObjManagerImp->mItemFuncItem.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_SKILLBOOK:
		{
			pObj = mSceneObjManagerImp->mItemSkillBook.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_FAMILYINFO:
		{
			pObj = mSceneObjManagerImp->mFamilyInfo.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_IBTRANS:
		{
			pObj = mSceneObjManagerImp->mIbTransPool.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_REDPAPER:
		{
			pObj = mSceneObjManagerImp->mRedPaper.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ACHIEVE:
		{
			pObj = mSceneObjManagerImp->mAchieve.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_VIP:
		{
			pObj = mSceneObjManagerImp->mItemVip.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_CLEAN:
		{
			pObj = mSceneObjManagerImp->mItemClean.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_LEAGUEBAG:
		{
			pObj = mSceneObjManagerImp->mLeagueBag.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_REDSTONECONTROLLER:
		{
			pObj = mSceneObjManagerImp->mRedstoneController.find_by_id( vObjID );
			break;
		}
	
	case OBJTYPE_ITEM_INVITATION:
		{
			pObj = mSceneObjManagerImp->mInvitation.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_MARRIAGEINFO:
		{
			pObj = mSceneObjManagerImp->mMarriageInfo.find_by_id( vObjID );
			break;	
		}
	case OBJTYPE_ITEM_SYSINVITATION:
		{
			pObj = mSceneObjManagerImp->mSysInvitation.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_CREATOR_PET:
		{
			pObj = mSceneObjManagerImp->mPetCreator.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_BLACKPILLS:
		{
			pObj = mSceneObjManagerImp->mBlackPills.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_EXPCONTAINER:
		{
		
			pObj = mSceneObjManagerImp->mExpContainer.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ITEM_EXPPILLS:
		{
			pObj = mSceneObjManagerImp->mExpPills.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_ENTITY_GHOST:
		{
			pObj = mSceneObjManagerImp->mEntityGhostPool.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_CREATOR_GHOST:
		{
			pObj = mSceneObjManagerImp->mGhostCreatorPool.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_STALL_RECORD:
		{
			pObj = mSceneObjManagerImp->mStallRecord.find_by_id( vObjID );
			break;
		}
	case OBJTYPE_FUNCTRANS:
		{
			pObj = mSceneObjManagerImp->mFuncTrans.find_by_id( vObjID );
			break;
		}
	default:
		{
			//TODO: log it 
			//LOG_ERROR( "default","[ %s : %d ][ %s ] type %d not registerd", __LK_FILE__, __LINE__, __FUNCTION__, CObj::ID2TYPE( vObjID ));
			break;
		}
	}

	return pObj;	
}                       



// 打印对象使用情况
#define OBJ_USAGE_DUMP(obj_pool) \
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", \
	"#obj_pool" , \
	obj_pool.size(), \
	obj_pool.max_size() ); \
 
// ***************************************************************
//  Function:		Dump
//  Description:	输出各个对象管理器的使用情况(used/total)	
//	Input:			pBuffer+unLen	- 用于储存信息的buff
//					unLen	- buff的长度
//	OutPut:			无
//	Return:			无
//	Others:	
//  Date:			11/13/2008
// 
// ***************************************************************
void CSceneObjManager::Dump( char* pBuffer, unsigned int& unLen )
{
	unsigned int unMaxLen = unLen;
	unLen = 0;
	
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "--------------------------------------------------------\n");

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6s\t%6s\n", "name", "free", "total");
	
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mSharedCellsEntityID" ,
		mSharedCellsEntityID.size(), 
		mSharedCellsEntityID.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mSharedSeenPlayers" ,
		mSharedSeenPlayers.size(), 
		mSharedSeenPlayers.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mEntityPlayerPool" ,
		mSceneObjManagerImp->mEntityPlayerPool.size(), 
		mSceneObjManagerImp->mEntityPlayerPool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mEntityItemPool", 
		mSceneObjManagerImp->mEntityItemPool.size(),
		mSceneObjManagerImp->mEntityItemPool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mEntityNpcPool", 
		mSceneObjManagerImp->mEntityNpcPool.size(),
		mSceneObjManagerImp->mEntityNpcPool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mEntityCollectPool", 
		mSceneObjManagerImp->mEntityCollectPool.size(),
		mSceneObjManagerImp->mEntityCollectPool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mOgreCreatorPool",
		mSceneObjManagerImp->mOgreCreatorPool.size(),
		mSceneObjManagerImp->mOgreCreatorPool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mBoxCreatorPool",
		mSceneObjManagerImp->mBoxCreatorPool.size(),
		mSceneObjManagerImp->mBoxCreatorPool.max_size() );


	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mItemCreatorPool",
		mSceneObjManagerImp->mItemCreatorPool.size(), 
		mSceneObjManagerImp->mItemCreatorPool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mPedalCreatorPool", 
		mSceneObjManagerImp->mPedalCreatorPool.size(), 
		mSceneObjManagerImp->mPedalCreatorPool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemBook",
		mSceneObjManagerImp->mItemBook.size(), 
		mSceneObjManagerImp->mItemBook.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemEquipUpg",
		mSceneObjManagerImp->mItemEquipUpg.size(), 
		mSceneObjManagerImp->mItemEquipUpg.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemExp",
		mSceneObjManagerImp->mItemExp.size(), 
		mSceneObjManagerImp->mItemExp.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemPhysic",
		mSceneObjManagerImp->mItemPhysic.size(), 
		mSceneObjManagerImp->mItemPhysic.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemRelife",
		mSceneObjManagerImp->mItemRelife.size(), 
		mSceneObjManagerImp->mItemRelife.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemReturn",
		mSceneObjManagerImp->mItemReturn.size(), 
		mSceneObjManagerImp->mItemReturn.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemSkill",
		mSceneObjManagerImp->mItemSkill.size(), 
		mSceneObjManagerImp->mItemSkill.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemTeleport",
		mSceneObjManagerImp->mItemTeleport.size(), 
		mSceneObjManagerImp->mItemTeleport.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemEquipment",
		mSceneObjManagerImp->mItemEquipment.size(), 
		mSceneObjManagerImp->mItemEquipment.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemTask",
		mSceneObjManagerImp->mItemTask.size(), 
		mSceneObjManagerImp->mItemTask.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemSpecialTask",
		mSceneObjManagerImp->mItemSpecialTask.size(), 
		mSceneObjManagerImp->mItemSpecialTask.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemStuff",
		mSceneObjManagerImp->mItemStuff.size(), 
		mSceneObjManagerImp->mItemStuff.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemNote",
		mSceneObjManagerImp->mItemNote.size(), 
		mSceneObjManagerImp->mItemNote.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemRune",
		mSceneObjManagerImp->mItemRune.size(), 
		mSceneObjManagerImp->mItemRune.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemJewel",
		mSceneObjManagerImp->mItemJewel.size(), 
		mSceneObjManagerImp->mItemJewel.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemYuanBug",
		mSceneObjManagerImp->mItemYuanBug.size(), 
		mSceneObjManagerImp->mItemYuanBug.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemYuanEgg",
		mSceneObjManagerImp->mItemYuanEgg.size(), 
		mSceneObjManagerImp->mItemYuanEgg.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemShout",
		mSceneObjManagerImp->mItemShout.size(), 
		mSceneObjManagerImp->mItemShout.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemAutoShout",
		mSceneObjManagerImp->mItemAutoShout.size(), 
		mSceneObjManagerImp->mItemAutoShout.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mEntityOgrePool",
		mSceneObjManagerImp->mEntityOgrePool.size(), 
		mSceneObjManagerImp->mEntityOgrePool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mEntityPetPool",
		mSceneObjManagerImp->mEntityPetPool.size(), 
		mSceneObjManagerImp->mEntityPetPool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mEntityPedalPool",
		mSceneObjManagerImp->mEntityPedalPool.size(), 
		mSceneObjManagerImp->mEntityPedalPool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mRelivePosition",
		mSceneObjManagerImp->mRelivePosition.size(), 
		mSceneObjManagerImp->mRelivePosition.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mLMapObject",
		mSceneObjManagerImp->mLMapObject.size(), 
		mSceneObjManagerImp->mLMapObject.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mMMapObject",
		mSceneObjManagerImp->mMMapObject.size(), 
		mSceneObjManagerImp->mMMapObject.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mSMapObject",
		mSceneObjManagerImp->mSMapObject.size(), 
		mSceneObjManagerImp->mSMapObject.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mNpcPolicy",
		mSceneObjManagerImp->mNpcPolicy.size(), 
		mSceneObjManagerImp->mNpcPolicy.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mChat",
		mSceneObjManagerImp->mChat.size(), 
		mSceneObjManagerImp->mChat.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mTeam",
		mSceneObjManagerImp->mTeam.size(), 
		mSceneObjManagerImp->mTeam.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mTrade",
		mSceneObjManagerImp->mTrade.size(), 
		mSceneObjManagerImp->mTrade.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mMailInfor",
		mSceneObjManagerImp->mMailInfor.size(), 
		mSceneObjManagerImp->mMailInfor.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mMailCache",
		mSceneObjManagerImp->mMailCache.size(), 
		mSceneObjManagerImp->mMailCache.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemHorse",
		mSceneObjManagerImp->mItemHorse.size(), 
		mSceneObjManagerImp->mItemHorse.max_size() );
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mStoreBox",
		mSceneObjManagerImp->mStoreBox.size(), 
		mSceneObjManagerImp->mStoreBox.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mItemBindStone",
		mSceneObjManagerImp->mItemBindStone.size(), 
		mSceneObjManagerImp->mItemBindStone.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mSession",
		mSceneObjManagerImp->mSession.size(), 
		mSceneObjManagerImp->mSession.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mScroll",
		mSceneObjManagerImp->mItemScroll.size(), 
		mSceneObjManagerImp->mItemScroll.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mCard",
		mSceneObjManagerImp->mItemCard.size(), 
		mSceneObjManagerImp->mItemCard.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mEntityCollect",
		mSceneObjManagerImp->mEntityCollectPool.size(), 
		mSceneObjManagerImp->mEntityCollectPool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mCreatorCollect",
		mSceneObjManagerImp->mCollectCreatorPool.size(), 
		mSceneObjManagerImp->mCollectCreatorPool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mRepetion",
		mSceneObjManagerImp->mRepetion.size(), 
		mSceneObjManagerImp->mRepetion.max_size() );	

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mPaper",
		mSceneObjManagerImp->mItemPaper.size(), 
		mSceneObjManagerImp->mItemPaper.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mSymbol",
		mSceneObjManagerImp->mItemSymbol.size(), 
		mSceneObjManagerImp->mItemSymbol.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mStove",
		mSceneObjManagerImp->mStove.size(), 
		mSceneObjManagerImp->mStove.max_size() );
		
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mExpRedStone",
		mSceneObjManagerImp->mExpRedStone.size(), 
		mSceneObjManagerImp->mExpRedStone.max_size() );
		
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mAnswerStatus",
		mSceneObjManagerImp->mAnswerStatus.size(), 
		mSceneObjManagerImp->mAnswerStatus.max_size() );
		
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mQuestionPaper",
		mSceneObjManagerImp->mQuestionPaper.size(), 
		mSceneObjManagerImp->mQuestionPaper.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mMineMap",
		mSceneObjManagerImp->mMineMap.size(), 
		mSceneObjManagerImp->mMineMap.max_size() );
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mKitBag",
		mSceneObjManagerImp->mKitBag.size(), 
		mSceneObjManagerImp->mKitBag.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mBlock",
		mSceneObjManagerImp->mBlock.size( ),
		mSceneObjManagerImp->mBlock.max_size( ) );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n",
		"mBlockCreator",
		mSceneObjManagerImp->mBlockCreator.size( ),
		mSceneObjManagerImp->mBlockCreator.max_size( ) );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mToken",
		mSceneObjManagerImp->mToken.size(), 
		mSceneObjManagerImp->mToken.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mRemove",
		mSceneObjManagerImp->mRemove.size(), 
		mSceneObjManagerImp->mRemove.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mChange",
		mSceneObjManagerImp->mChange.size(), 
		mSceneObjManagerImp->mChange.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mItemCall",
		mSceneObjManagerImp->mItemCall.size(), 
		mSceneObjManagerImp->mItemCall.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mItemGiftBag",
		mSceneObjManagerImp->mItemGiftBag.size(), 
		mSceneObjManagerImp->mItemGiftBag.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mMagicStone",
		mSceneObjManagerImp->mMagicStone.size(), 
		mSceneObjManagerImp->mMagicStone.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mLifeSkillScroll",
		mSceneObjManagerImp->mLifeSkillScroll.size(), 
		mSceneObjManagerImp->mLifeSkillScroll.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mItemMagicWeapon",
		mSceneObjManagerImp->mItemMagicWeapon.size(), 
		mSceneObjManagerImp->mItemMagicWeapon.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mItemFuncItem",
		mSceneObjManagerImp->mItemFuncItem.size(), 
		mSceneObjManagerImp->mItemFuncItem.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mItemSkillBook",
		mSceneObjManagerImp->mItemSkillBook.size(), 
		mSceneObjManagerImp->mItemSkillBook.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mFamilyInfo",
		mSceneObjManagerImp->mFamilyInfo.size(), 
		mSceneObjManagerImp->mFamilyInfo.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mIbTransPool",
		mSceneObjManagerImp->mIbTransPool.size(), 
		mSceneObjManagerImp->mIbTransPool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mRedPaper",
		mSceneObjManagerImp->mRedPaper.size(), 
		mSceneObjManagerImp->mRedPaper.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mAchieve",
		mSceneObjManagerImp->mAchieve.size(), 
		mSceneObjManagerImp->mAchieve.max_size() );
	
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mLeagueBag",
		mSceneObjManagerImp->mLeagueBag.size(), 
		mSceneObjManagerImp->mLeagueBag.max_size() );
	
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"CRedstoneController",
		mSceneObjManagerImp->mRedstoneController.size(), 
		mSceneObjManagerImp->mRedstoneController.max_size() );
		
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"CItemInvitation", 		
		mSceneObjManagerImp->mInvitation.size(), 
		mSceneObjManagerImp->mInvitation.max_size() );
	
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"CMarriageInfo", 	
		mSceneObjManagerImp->mMarriageInfo.size(), 
		mSceneObjManagerImp->mMarriageInfo.max_size() );
	
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"CItemSysInvitation", 	
		mSceneObjManagerImp->mSysInvitation.size(), 
		mSceneObjManagerImp->mSysInvitation.max_size() );	

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"CPetCreator", 	
		mSceneObjManagerImp->mPetCreator.size(), 
		mSceneObjManagerImp->mPetCreator.max_size() );	
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"CItemBlackPills", 	
		mSceneObjManagerImp->mBlackPills.size(), 
		mSceneObjManagerImp->mBlackPills.max_size() );	
		
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"CItemExpContainer", 	
		mSceneObjManagerImp->mExpContainer.size(), 
		mSceneObjManagerImp->mExpContainer.max_size() );	
	
	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"CItemExpPills", 	
		mSceneObjManagerImp->mExpPills.size(), 
		mSceneObjManagerImp->mExpPills.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mGhostCreatorPool", 	
		mSceneObjManagerImp->mGhostCreatorPool.size(), 
		mSceneObjManagerImp->mGhostCreatorPool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mEntityGhostPool", 	
		mSceneObjManagerImp->mEntityGhostPool.size(), 
		mSceneObjManagerImp->mEntityGhostPool.max_size() );

	unLen += snprintf( pBuffer+unLen, unMaxLen-unLen, "%20s\t%6d\t%6d\n", 
		"mStallRecord", 	
		mSceneObjManagerImp->mStallRecord.size(), 
		mSceneObjManagerImp->mStallRecord.max_size() );
}

                         
