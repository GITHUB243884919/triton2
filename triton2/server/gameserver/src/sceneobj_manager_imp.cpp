#include "stdafx.hpp"
#include "sceneobj_manager_imp.h"

#define FMT_OBJ_SIZE_DUMP "%20s\t: %10d\t * %8d\t = %10d\t"

CSharedMem* CSceneObjManagerImp::mShmPtr = NULL; 

CSceneObjManagerImp::CSceneObjManagerImp( )
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

CSceneObjManagerImp::~CSceneObjManagerImp()
{
}


void* CSceneObjManagerImp::operator new( size_t size )
{
	return ( void* ) mShmPtr->CreateSegment( size );
}

void  CSceneObjManagerImp::operator delete( void* pointer ) 
{ }

void* CSceneObjManagerImp::operator new(size_t size, const char* file, int line)
{
	return operator new(size);
}
void CSceneObjManagerImp::operator delete(void* pointer, const char* file, int line)
{

}

unsigned int CSceneObjManagerImp::CountSize()
{
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CEntityPlayer",	sizeof( CEntityPlayer ),	SERVER_CAP_PLAYER,		sizeof( ObjMng_EntityPlayer ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CEntityFuncNpc",	sizeof( CEntityFuncNpc ),	SERVER_CAP_NPC_FUNCNPC,	sizeof( ObjMng_EntityNpc) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CEntityPet",		sizeof( CEntityPet ),		SERVER_CAP_NPC_PET,		sizeof( ObjMng_EntityPet ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CEntityPedal",		sizeof( CEntityPedal ),     SERVER_CAP_NPC_PEDAL,	sizeof( ObjMng_EntityPedal ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CEntityOgre",		sizeof( CEntityOgre ),		SERVER_CAP_NPC_OGRE,	sizeof( ObjMng_EntityOgre ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP,	"CEntityItem",		sizeof( CEntityItem ),		SERVER_CAP_NPC_ITEM,	sizeof( ObjMng_EntityItem ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CEntityCollect",	sizeof( CEntityCollect ),	SERVER_CAP_NPC_COLLECT,	sizeof( ObjMng_EntityCollect ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "COgreCreator",		sizeof( COgreCreator ),		SERVER_CAP_NPC_OGRE,	sizeof( ObjMng_OgreCreator ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CBoxCreator",		sizeof( CBoxCreator ),		SERVER_CAP_STOREBOX,	sizeof( ObjMng_BoxCreator ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CNpcCreator",		sizeof( CNpcCreator ),		SERVER_CAP_NPC_FUNCNPC,	sizeof( ObjMng_NpcCreator ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemCreator",		sizeof( CItemCreator ),		SERVER_CAP_NPC_ITEM,	sizeof( ObjMng_ItemCreator ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CPedalCreator",	sizeof( CPedalCreator ),	SERVER_CAP_NPC_PEDAL,	sizeof( ObjMng_PedalCreator )	);
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CCollectCreator",	sizeof( CCollectCreator),	SERVER_CAP_NPC_COLLECT,	sizeof(ObjMng_CollectCreator));
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemBook",		sizeof( CItemBook ),		SERVER_CAP_ITEM_BOOK,	sizeof( ObjMng_ItemBook ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemEquipUpg",	sizeof( CItemEquipUpg ),	SERVER_CAP_ITEM_EQUIPUPG,	sizeof( ObjMng_ItemEquipUpg ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemExp",			sizeof( CItemExp ),			SERVER_CAP_ITEM_EXP,	sizeof( ObjMng_ItemExp ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemPhysic",		sizeof( CItemPhysic ),		SERVER_CAP_ITEM_PHYSIC,	sizeof( ObjMng_ItemPhysic ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemRelive",		sizeof( CItemRelive ),		SERVER_CAP_ITEM_RELIFE, sizeof( ObjMng_ItemRelife ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemReturn",		sizeof( CItemReturn ),		SERVER_CAP_ITEM_RETURN, sizeof( ObjMng_ItemReturn ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemSkill",		sizeof( CItemSkill ),		SERVER_CAP_ITEM_SKILL,	sizeof( ObjMng_ItemSkill ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemTeleport",	sizeof( CItemTeleport ),	SERVER_CAP_ITEM_TELEPORT, sizeof( ObjMng_ItemTeleport ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemEquipment",	sizeof( CItemEquipment ),	SERVER_CAP_ITEM_EQUIPMENT,	sizeof( ObjMng_ItemEquipment ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemTask",		sizeof( CItemTask ),		SERVER_CAP_ITEM_TASK,	sizeof( ObjMng_ItemTask ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemSpecialTask",	sizeof( CItemSpecialTask ),	SERVER_CAP_ITEM_TASK,	sizeof( ObjMng_ItemSpecialTask ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemStuff",		sizeof( CItemStuff ),		SERVER_CAP_ITEM_STUFF,	sizeof( ObjMng_ItemStuff ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemNote",		sizeof( CItemNote ),		SERVER_CAP_ITEM_NOTE,	sizeof( ObjMng_ItemNote ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemRune",		sizeof( CItemRune),			SERVER_CAP_ITEM_RUNE,	sizeof(ObjMng_ItemRune) );   
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemJewel",		sizeof( CItemJewel ),		SERVER_CAP_ITEM_JEWEL, sizeof(ObjMng_ItemJewel) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemYuanBug",		sizeof( CItemYuanBug),		SERVER_CAP_ITEM_YUANBUG, sizeof(ObjMng_ItemYuanBug));
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemYuanEgg",		sizeof( CItemYuanEgg),		SERVER_CAP_ITEM_YUANEGG, sizeof(ObjMng_ItemYuanEgg));
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemShout",		sizeof( CItemShout ),		SERVER_CAP_ITEM_SHOUT,	sizeof( ObjMng_ItemShout ) ); 
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemAutoShout",	sizeof( CItemAutoShout ),	SERVER_CAP_ITEM_AUTOSHOUT,	sizeof( ObjMng_ItemAutoShout ) ); 
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemHorese",		sizeof(CItemHorse),			SERVER_CAP_ITEM_HORSE, sizeof(ObjMng_ItemHorse));
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CEntityBox",		sizeof(CEntityBox),			SERVER_CAP_STOREBOX, sizeof(ObjMng_StoreBox));
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CScroll",			sizeof( CItemScroll ),		SERVER_CAP_ITEM_SCROLL,sizeof( ObjMng_Scroll ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CCard",			sizeof( CItemCard ),		SERVER_CAP_ITEM_CARD,sizeof( ObjMng_Card ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CRelivePosition",	sizeof( CRelivePosition ),	SERVER_CAP_RELIVEPOSITION,	sizeof( ObjMng_RelivePosition ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CLMapObject",		sizeof( CMapObjectL ),		SERVER_CAP_L_MAP_OBJECT,	sizeof( ObjMng_LMapObject ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CMMapObject",		sizeof( CMapObjectM ),		SERVER_CAP_M_MAP_OBJECT,	sizeof( ObjMng_MMapObject ) );	
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CSMapObject",		sizeof( CMapObjectS ),		SERVER_CAP_S_MAP_OBJECT,	sizeof( ObjMng_SMapObject ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "ObjMng_NpcPolicy",  sizeof( CNpcPolicy),		SERVER_CAP_NPCPOLICY,  sizeof( ObjMng_NpcPolicy));
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "Chat",				sizeof( CChat ),			SERVER_CAP_PLAYER,	sizeof( ObjMng_Chat) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CTeamObject",		sizeof( CTeam ),			SERVER_CAP_PLAYER,	sizeof( ObjMng_Team) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CTradeObect",		sizeof( CTrade ),			SERVER_CAP_PLAYER,	sizeof( ObjMng_Trade ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CMailInforObect",	sizeof( CMail ),			SERVER_CAP_PLAYER,	sizeof( ObjMng_MailInfor ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CMailCacheObect",	sizeof( CMailCache ),		SERVER_CAP_PLAYER,	sizeof( ObjMng_MailCache ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemBindStone",	sizeof( CItemBindStone),		SERVER_CAP_ITEM_BINDSTONE, sizeof(ObjMng_BindStone));
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CTaskList" ,		sizeof(CTaskList),			SERVER_CAP_PLAYER, sizeof(ObjMng_Task));
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CStall",			sizeof( CStall ),			SERVER_CAP_ITEM_HORSE, sizeof( ObjMng_Stall ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CSession",			sizeof( CSession ),			SERVER_CAP_PLAYER,	   sizeof( ObjMng_Session ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CRepetion",		sizeof( CRepetion ),		SERVER_CAP_REPETION_OBJECT,	   sizeof( ObjMng_Repetion ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemReturnPaper",	sizeof( CItemReturnPaper ),	SERVER_CAP_ITEM_CARD,	   sizeof( ObjMng_ReturnPaper ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemTeleSymbol",	sizeof( CItemTeleSymbol ),	SERVER_CAP_ITEM_CARD,	   sizeof( ObjMng_TeleSymbol ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemStove",		sizeof( CItemStove ),		SERVER_CAP_ITEM_CARD,	   sizeof( ObjMng_Stove ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CEntityRedStone",	sizeof( CEntityRedStone ),	SERVER_CAP_EXPREDSTONE,	   sizeof( ObjMng_ExpRedStone ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CAnswerStatus",	sizeof( CAnswerStatus ),	SERVER_CAP_PLAYER,		   sizeof( ObjMng_AnswerStatus ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemQuestPaper",	sizeof( CItemQuestPaper ),	SERVER_CAP_ITEM_QUESTIONPAPER,sizeof( ObjMng_ExpRedStone ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemMineMap",		sizeof( CItemMineMap ),		SERVER_CAP_PLAYER,	sizeof( ObjMng_MineMap ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP,	"CItemKitBag",		sizeof( CItemKitBag ),		SERVER_CAP_ITEM_KITBAG,	sizeof( ObjMng_KitBag ) );            	
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemBlock",		sizeof(CEntityBlock), SERVER_CAP_BLOCK, sizeof(ObjMng_Block));
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CBlockCreator",	sizeof(CBlockCreator), SERVER_CAP_BLOCK, sizeof(ObjMng_Creator_Block));
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemToken",		sizeof( CItemToken ),		SERVER_CAP_ITEM_TOKEN,	sizeof( ObjMng_Token ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemRemove",		sizeof( CItemRemove ),		SERVER_CAP_PLAYER,	sizeof( ObjMng_Remove ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemChange",		sizeof( CItemChange ),		SERVER_CAP_PLAYER,	sizeof( ObjMng_Change ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemCall",		sizeof( CItemCall ),		SERVER_CAP_ITEM_CARD,sizeof( ObjMng_ItemCall ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemGiftBag",		sizeof( CItemGiftBag ),		SERVER_CAP_ITEM_GIFTBAG,sizeof( ObjMng_ItemGiftBag ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemMagicStone",	sizeof( CItemMagicStone ),	SERVER_CAP_MAGICSTONE,	sizeof( ObjMng_MagicStone ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemLifeSkillScroll",	sizeof( CItemLifeSkillScroll ),	SERVER_CAP_LIFESKILLSCROLL,	sizeof( ObjMng_LifeSkillScroll ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemMagicWeapon",	sizeof( CItemMagicWeapon ),	SERVER_CAP_ITEM_MAGICWEAPON,sizeof( ObjMng_ItemMagicWeapon ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemFuncItem",	sizeof( CItemFuncItem ),	SERVER_CAP_ITEM_FUNC, sizeof( ObjMng_ItemFuncItem ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "CItemSkillBook",	sizeof( CItemSkillBook ),	SERVER_CAP_ITEM_SKILLBOOK, sizeof( ObjMng_ItemSkillBook ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP,	"CFamilyInfo",		sizeof( CFamilyInfo ),		SERVER_CAP_FAMILYINFO,	   sizeof( ObjMng_FamilyInfo ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP,	"CIbTrans",			sizeof( CIbTrans ),			SERVER_CAP_IBTRANS,			sizeof( ObjMng_IbTrans ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP,	"CItemRedPaper",	sizeof( CItemRedPaper ),	SERVER_CAP_ITEM_REDPAPER,	sizeof( ObjMng_RedPaper ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP,	"CAchieveDataUnit",	sizeof( CAchieveDataUnit ),	SERVER_CAP_PLAYER,	sizeof( ObjMng_Achieve ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP,	"CItemLeagueBag",	sizeof( CItemLeagueBag ),	SERVER_CAP_ITEM_LEAGUEBAG,	sizeof( ObjMng_LeagueBag ) );
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP,	"CRedstoneController",	sizeof( CRedstoneController ),	SERVER_CAP_REDSTONECONTROLLER,	sizeof( ObjMng_RedStoneController ) );	
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP,	"CItemInvitation",	sizeof( CItemInvitation ),	SERVER_CAP_ITEM_INVITATION,	sizeof( ObjMng_Invitation ) );		
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP,	"CItemSysInvitation",	sizeof( CItemSysInvitation ),	SERVER_CAP_ITEM_SYSINVITATION,	sizeof( ObjMng_SysInvitation ) );			
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP,	"CItemBlackPills",	sizeof( CItemBlackPills ),	SERVER_CAP_ITEM_BLACKPILLS,	sizeof( ObjMng_BlackPills ) );			
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP,	"CEntityGhost",	    sizeof( CEntityGhost ),	    SERVER_CAP_GHOST,	sizeof( ObjMng_EntityGhost ) );			
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP,	"CGhostCreator",	sizeof( CGhostCreator ),	SERVER_CAP_GHOST,	sizeof( ObjMng_GhostCreator ) );	
	LOG_INFO( "default", FMT_OBJ_SIZE_DUMP, "cFuncTrans",		sizeof( CFuncTrans ),		SERVER_CAP_FUNCTRANS, sizeof( ObjMng_FuncTrans ) );
	return sizeof( CSceneObjManagerImp );
}

int CSceneObjManagerImp::Initailize( )
{
	mEntityPlayerPool.initailize( );
	mEntityNpcPool.initailize( );
	mEntityOgrePool.initailize();
	mOgreCreatorPool.initailize( );
	mBoxCreatorPool.initailize( );
	mNpcCreatorPool.initailize( );
	mItemCreatorPool.initailize( );
	mPedalCreatorPool.initailize( );
	mEntityCollectPool.initailize();
	mCollectCreatorPool.initailize();
	mItemBook.initailize( );
	mItemEquipUpg.initailize( );
	mItemExp.initailize( );
	mItemPhysic.initailize( );
	mItemRelife.initailize( );
	mItemReturn.initailize( );
	mItemShout.initailize( );
	mItemAutoShout.initailize();
	mItemSkill.initailize( );
	mItemTeleport.initailize( );
	mItemEquipment.initailize( );
	mItemTask.initailize( );
	mItemSpecialTask.initailize( );
	mItemStuff.initailize( );
	mItemNote.initailize( );
	mItemRune.initailize( );
	mItemJewel.initailize( );
	mItemYuanBug.initailize( );
	mItemYuanEgg.initailize( );
	mItemHorse.initailize();
	mStoreBox.initailize( );
	mRelivePosition.initailize( );
	mLMapObject.initailize( );
	mMMapObject.initailize( );
	mSMapObject.initailize( );
	mNpcPolicy.initailize( );
	mTeam.initailize( );
	mChat.initailize( );
	mEntityPetPool.initailize( );
	mTrade.initailize( );
	mEntityPedalPool.initailize();
	mEntityItemPool.initailize();
	mMailInfor.initailize();
	mMailCache.initailize();
	mItemBindStone.initailize();
	mTaskList.initailize();
	mStall.initailize();
	mSession.initailize();
	mItemScroll.initailize();
	mItemCard.initailize();
	mRepetion.initailize( );
	mItemPaper.initailize();
	mItemSymbol.initailize();
	mStove.initailize();
	mAnswerStatus.initailize();
	mExpRedStone.initailize();
	mQuestionPaper.initailize();
	mMineMap.initailize();
	mKitBag.initailize();	
	mBlock.initailize( );
	mBlockCreator.initailize( );
	mToken.initailize();
	mRemove.initailize();
	mChange.initailize();
	mItemCall.initailize();
	mItemGiftBag.initailize();
	mMagicStone.initailize();
	mLifeSkillScroll.initailize();
	mItemMagicWeapon.initailize();
	mItemFuncItem.initailize();
	mItemSkillBook.initailize();
	mFamilyInfo.initailize();
	mIbTransPool.initailize();
	mRedPaper.initailize();
	mAchieve.initailize();
	mItemVip.initailize();
	mItemClean.initailize();
	mLeagueBag.initailize();
	mRedstoneController.initailize();
	mInvitation.initailize();
	mMarriageInfo.initailize();
	mSysInvitation.initailize();
	mPetCreator.initailize();
	mBlackPills.initailize();
	mExpContainer.initailize();
	mExpPills.initailize();
	mGhostCreatorPool.initailize();
	mEntityGhostPool.initailize();
	mStallRecord.initailize();
	mFuncTrans.initailize();
	return 0;
}

int CSceneObjManagerImp::Resume()
{
	mEntityPlayerPool.resume( );
	mEntityNpcPool.resume( );
	mEntityOgrePool.resume();
	mEntityItemPool.resume( );
	mOgreCreatorPool.resume( );
	mBoxCreatorPool.resume( );
	mNpcCreatorPool.resume( );
	mItemCreatorPool.resume( );
	mPedalCreatorPool.resume( );
	mEntityCollectPool.resume();
	mCollectCreatorPool.resume();
	mItemBook.resume( );
	mItemEquipUpg.resume( );
	mItemExp.resume( );
	mItemPhysic.resume( );
	mItemRelife.resume( );
	mItemReturn.resume( );
	mItemSkill.resume( );
	mItemTeleport.resume( );
	mItemEquipment.resume( );
	mItemShout.resume( );
	mItemAutoShout.resume();
	mItemTask.resume( );
	mItemSpecialTask.resume( );
	mItemStuff.resume( );
	mItemNote.resume( );
	mItemRune.resume( );
	mItemJewel.resume( );
	mItemYuanBug.resume( );
	mItemYuanEgg.resume( );
	mItemHorse.resume();
	mStoreBox.resume( );
	mRelivePosition.resume( );
	mLMapObject.resume( );
	mMMapObject.resume( );
	mSMapObject.resume( );
	mNpcPolicy.resume( );
	mTeam.resume( );
	mChat.resume( );
	mEntityPetPool.resume( );
	mTrade.resume( ); 
	mEntityPedalPool.resume();
	mTaskList.resume();

	mMailCache.resume();
	mMailInfor.resume();
    mItemBindStone.resume();
	mStall.resume();
	mSession.resume();
	mItemScroll.resume();
	mItemCard.resume();
	mRepetion.resume( );
	mItemPaper.resume();
	mItemSymbol.resume();
	mStove.resume();
	mExpRedStone.resume();
	mQuestionPaper.resume();
	mAnswerStatus.resume();
	mMineMap.resume();
	mKitBag.resume();
	mBlock.resume( );
	mBlockCreator.resume( );
	mToken.resume();
	mRemove.resume();
	mChange.resume();
	mItemCall.resume();
	mItemGiftBag.resume();
	mMagicStone.resume();
	mLifeSkillScroll.resume();
	mItemMagicWeapon.resume();
	mItemFuncItem.resume();
	mItemSkillBook.resume();
	mFamilyInfo.resume();
	mIbTransPool.resume();
	mRedPaper.resume();
	mAchieve.resume();
	mItemVip.resume();
	mItemClean.resume();
	mLeagueBag.resume();
	mRedstoneController.resume();
	mInvitation.resume();
	mMarriageInfo.resume();
	mSysInvitation.resume();
	mPetCreator.resume();
	mBlackPills.resume();
	mExpContainer.resume();
	mExpPills.resume();
	mEntityGhostPool.resume();
	mGhostCreatorPool.resume();
	mStallRecord.resume();
	mFuncTrans.resume();
	return 0;
}

