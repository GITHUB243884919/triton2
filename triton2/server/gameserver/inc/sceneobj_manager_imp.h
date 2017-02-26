/******************************************************************************
	版权所有 (C ), 2008-2018, 蓝港在线（北京）科技有限公司

******************************************************************************
	文件名		: sceneobj_manager_imp.h
	版本号		: 1.0
	作者			: ZHL
	生成日期		: 2008-07-09
	最近修改		: 
	功能描述		: 管理场景服务器游戏对象,创建在共享内存中 
	函数列表		: 
	修改历史		: 
		1 日期	: 2008-07-09
		作者		: ZHL
		修改内容	: 生成
*******************************************************************************/
#pragma once

#include "servercore.h"
#include "sceneobj_define.h"
#include "proclient_svr.h"
#include "entity.h"
#include "propertymodule.h"
#include "mapobjmanager.h"
#include "npcmodule.h"
#include "chat.h"
#include "property.h"
#include "trade.h"
#include "mail.h"
#include "stall.h"
#include "taskmanager.h"
#include "dbinterface.h"
#include "repetionmodule.h"
#include "activity.h"
#include "ibstoremodule.h"
#include "teammodule.h"
#include "familymodule.h"
#include "achieve.h"
#include "marriagemanager.h"


#define OBJ_ID_START(obj_type)	(((obj_type) << OBJ_ID_BITS) + 1)
#define OBJ_ID_END(obj_type)	((((obj_type) + 1) << OBJ_ID_BITS) - 10)
#define OBJ_ID_COUNT(obj_type, count) OBJ_ID_START(obj_type), count, OBJ_ID_END(obj_type)


class CSharedMem;


class CSceneObjManagerImp 
{
public:
	// 游戏对象
	typedef ObjManager< CEntityPlayer,	    OBJ_ID_COUNT( OBJTYPE_ENTITY_PLAYER,	SERVER_CAP_PLAYER				) > ObjMng_EntityPlayer;
	typedef ObjManager< CEntityFuncNpc,		OBJ_ID_COUNT( OBJTYPE_ENTITY_FUNCNPC,	SERVER_CAP_NPC_FUNCNPC			) > ObjMng_EntityNpc;
	typedef ObjManager< CEntityOgre,		OBJ_ID_COUNT( OBJTYPE_ENTITY_OGRE,		SERVER_CAP_NPC_OGRE				) > ObjMng_EntityOgre;
	//typedef ObjManager< CEntityMoney,		OBJ_ID_COUNT( OBJTYPE_ENTITY_MONEY,		SERVER_CAP_NPC_MONEY			) > ObjMng_EntityMoney;
	typedef ObjManager< CEntityItem,		OBJ_ID_COUNT( OBJTYPE_ENTITY_ITEM,		SERVER_CAP_NPC_ITEM				) > ObjMng_EntityItem;
	typedef ObjManager< CEntityPet,			OBJ_ID_COUNT( OBJTYPE_ENTITY_PET,		SERVER_CAP_NPC_PET				) > ObjMng_EntityPet;  	
	typedef ObjManager< CEntityPedal,		OBJ_ID_COUNT( OBJTYPE_ENTITY_PEDAL,		SERVER_CAP_PEDAL				) > ObjMng_EntityPedal;  
	typedef ObjManager< COgreCreator,	    OBJ_ID_COUNT( OBJTYPE_CREATOR_OGRE,		SERVER_CAP_NPC_OGRE				) > ObjMng_OgreCreator;
	typedef ObjManager< CBoxCreator,		OBJ_ID_COUNT( OBJTYPE_CREATOR_BOX,		SERVER_CAP_STOREBOX				) > ObjMng_BoxCreator;
	typedef ObjManager< CNpcCreator,		OBJ_ID_COUNT( OBJTYPE_CREATOR_NPC,		SERVER_CAP_NPC_FUNCNPC			) > ObjMng_NpcCreator;
	typedef ObjManager< CItemCreator,	    OBJ_ID_COUNT( OBJTYPE_CREATOR_ITEM,		SERVER_CAP_NPC_ITEM				) > ObjMng_ItemCreator;
	typedef ObjManager< CPetCreator,	    OBJ_ID_COUNT( OBJTYPE_CREATOR_PET,		SERVER_CAP_PLAYER/20			) > ObjMng_PetCreator;
	typedef ObjManager< CPedalCreator,	    OBJ_ID_COUNT( OBJTYPE_CREATOR_PEDAL,	SERVER_CAP_NPC_PEDAL			) > ObjMng_PedalCreator;
	typedef ObjManager< CItemBook,		    OBJ_ID_COUNT( OBJTYPE_ITEM_BOOK,		SERVER_CAP_ITEM_BOOK			) > ObjMng_ItemBook;
	typedef ObjManager< CItemEquipUpg,	    OBJ_ID_COUNT( OBJTYPE_ITEM_EQUIPUPG,	SERVER_CAP_ITEM_EQUIPUPG		) > ObjMng_ItemEquipUpg;
	typedef ObjManager< CItemExp,	    	OBJ_ID_COUNT( OBJTYPE_ITEM_EXP,			SERVER_CAP_ITEM_EXP				) > ObjMng_ItemExp;
	typedef ObjManager< CItemPhysic,	    OBJ_ID_COUNT( OBJTYPE_ITEM_PHYSIC,		SERVER_CAP_ITEM_PHYSIC			) > ObjMng_ItemPhysic;
	typedef ObjManager< CItemRelive,	    OBJ_ID_COUNT( OBJTYPE_ITEM_RELIFE,		SERVER_CAP_ITEM_RELIFE			) > ObjMng_ItemRelife;
	typedef ObjManager< CItemReturn,	    OBJ_ID_COUNT( OBJTYPE_ITEM_RETURN,		SERVER_CAP_ITEM_RETURN			) > ObjMng_ItemReturn;
	typedef ObjManager< CItemSkill,		    OBJ_ID_COUNT( OBJTYPE_ITEM_SKILL,		SERVER_CAP_ITEM_SKILL			) > ObjMng_ItemSkill;
	typedef ObjManager< CItemTeleport,	    OBJ_ID_COUNT( OBJTYPE_ITEM_TELEPORT,	SERVER_CAP_ITEM_TELEPORT		) > ObjMng_ItemTeleport;
	typedef ObjManager< CItemEquipment,     OBJ_ID_COUNT( OBJTYPE_ITEM_EQUIPMENT,	SERVER_CAP_ITEM_EQUIPMENT		) > ObjMng_ItemEquipment;
	typedef ObjManager< CItemTask,		    OBJ_ID_COUNT( OBJTYPE_ITEM_TASK,		SERVER_CAP_ITEM_TASK			) > ObjMng_ItemTask;
	typedef ObjManager< CItemSpecialTask,   OBJ_ID_COUNT( OBJTYPE_ITEM_SPECIAL_TASK,SERVER_CAP_ITEM_TASK			) > ObjMng_ItemSpecialTask;
	typedef ObjManager< CItemStuff,		    OBJ_ID_COUNT( OBJTYPE_ITEM_STUFF,		SERVER_CAP_ITEM_STUFF			) > ObjMng_ItemStuff;
	typedef ObjManager< CItemNote,		    OBJ_ID_COUNT( OBJTYPE_ITEM_NOTE,		SERVER_CAP_ITEM_NOTE			) > ObjMng_ItemNote;
	typedef ObjManager< CItemRune,		    OBJ_ID_COUNT( OBJTYPE_ITEM_RUNE,		SERVER_CAP_ITEM_RUNE			) > ObjMng_ItemRune;
	typedef ObjManager< CItemJewel,		    OBJ_ID_COUNT( OBJTYPE_ITEM_JEWEL,		SERVER_CAP_ITEM_JEWEL			) > ObjMng_ItemJewel;
	typedef ObjManager< CItemYuanBug,	    OBJ_ID_COUNT( OBJTYPE_ITEM_YUANBUG,		SERVER_CAP_ITEM_YUANBUG			) > ObjMng_ItemYuanBug;
	typedef ObjManager< CItemYuanEgg,	    OBJ_ID_COUNT( OBJTYPE_ITEM_YUANEGG,		SERVER_CAP_ITEM_YUANEGG			) > ObjMng_ItemYuanEgg;
    typedef ObjManager< CItemShout,         OBJ_ID_COUNT( OBJTYPE_ITEM_SHOUT,       SERVER_CAP_ITEM_SHOUT			) > ObjMng_ItemShout ;  
    typedef ObjManager< CItemAutoShout,     OBJ_ID_COUNT( OBJTYPE_ITEM_AUTOSHOUT,   SERVER_CAP_ITEM_AUTOSHOUT		) > ObjMng_ItemAutoShout ;
	typedef ObjManager< CItemHorse,			OBJ_ID_COUNT( OBJTYPE_ITEM_HORSE,		SERVER_CAP_ITEM_HORSE			) > ObjMng_ItemHorse ;
	typedef ObjManager< CEntityBox,			OBJ_ID_COUNT( OBJTYPE_ENTITY_STOREBOX,	SERVER_CAP_STOREBOX				) > ObjMng_StoreBox;
	typedef ObjManager< CRelivePosition,	OBJ_ID_COUNT( OBJTYPE_RELIVEPOSITION,	SERVER_CAP_RELIVEPOSITION		) > ObjMng_RelivePosition;
	typedef ObjManager< CMapObjectL,		OBJ_ID_COUNT( OBJTYPE_MAP_L_OBJECT,		SERVER_CAP_L_MAP_OBJECT			) > ObjMng_LMapObject;
	typedef ObjManager< CMapObjectM,		OBJ_ID_COUNT( OBJTYPE_MAP_M_OBJECT,		SERVER_CAP_M_MAP_OBJECT			) > ObjMng_MMapObject;
	typedef ObjManager< CMapObjectS,		OBJ_ID_COUNT( OBJTYPE_MAP_S_OBJECT,		SERVER_CAP_S_MAP_OBJECT			) > ObjMng_SMapObject;
	typedef ObjManager< CNpcPolicy,			OBJ_ID_COUNT( OBJTYPE_POLICY_NPC,		SERVER_CAP_NPCPOLICY			) > ObjMng_NpcPolicy;
	typedef ObjManager< CChat,				OBJ_ID_COUNT( OBJTYPE_CHAT,				SERVER_CAP_PLAYER				) > ObjMng_Chat;
	typedef ObjManager< CTeam,				OBJ_ID_COUNT( OBJTYPE_TEAM,				SERVER_CAP_PLAYER				) > ObjMng_Team;
	typedef ObjManager< CTrade,				OBJ_ID_COUNT( OBJTYPE_TRADE,			SERVER_CAP_PLAYER				) > ObjMng_Trade;
    typedef ObjManager< CMail,				OBJ_ID_COUNT( OBJTYPE_MAIL_MAILINFOR,	SERVER_CAP_PLAYER				) > ObjMng_MailInfor;
	typedef ObjManager< CMailCache,			OBJ_ID_COUNT( OBJTYPE_MAIL_MAILCACHE,	SERVER_CAP_MAILCACHE    		) > ObjMng_MailCache;
	typedef ObjManager< CItemBindStone,		OBJ_ID_COUNT( OBJTYPE_ITEM_BINDSTONE,	SERVER_CAP_ITEM_BINDSTONE		) > ObjMng_BindStone;
	typedef ObjManager< CTaskList,			OBJ_ID_COUNT( OBJTYPE_TASKLIST,			SERVER_CAP_PLAYER				) > ObjMng_Task;
	typedef ObjManager< CStall,				OBJ_ID_COUNT( OBJTYPE_STALL,			SERVER_CAP_PLAYER				) > ObjMng_Stall;
	typedef ObjManager< CSession,			OBJ_ID_COUNT( OBJTYPE_SESSION,			SERVER_CAP_SESSION				) > ObjMng_Session;
	typedef ObjManager< CItemScroll,		OBJ_ID_COUNT( OBJTYPE_ITEM_SCROLL,		SERVER_CAP_ITEM_SCROLL			) > ObjMng_Scroll;
	typedef ObjManager< CEntityCollect,		OBJ_ID_COUNT( OBJTYPE_ENTITY_COLLECT,	SERVER_CAP_NPC_COLLECT			) > ObjMng_EntityCollect;
	typedef ObjManager< CCollectCreator,	OBJ_ID_COUNT( OBJTYPE_CREATOR_COLLECT,	SERVER_CAP_NPC_COLLECT			) > ObjMng_CollectCreator;
	typedef ObjManager< CItemCard,			OBJ_ID_COUNT( OBJTYPE_ITEM_CARD,		SERVER_CAP_ITEM_CARD			) > ObjMng_Card;
	typedef ObjManager< CRepetion,			OBJ_ID_COUNT( OBJTYPE_REPETION,			SERVER_CAP_REPETION_OBJECT		) > ObjMng_Repetion;
	typedef ObjManager< CItemReturnPaper,	OBJ_ID_COUNT( OBJTYPE_RETURNPAPER,		SERVER_CAP_ITEM_CARD			) > ObjMng_ReturnPaper;
	typedef ObjManager< CItemTeleSymbol,	OBJ_ID_COUNT( OBJTYPE_TELESYMBOL,		SERVER_CAP_ITEM_CARD			) > ObjMng_TeleSymbol;
	typedef ObjManager< CItemStove,			OBJ_ID_COUNT( OBJTYPE_ITEMSTOVE,		SERVER_CAP_ITEM_CARD			) > ObjMng_Stove;
	typedef ObjManager< CEntityRedStone,	OBJ_ID_COUNT( OBJTYPE_ENTITY_EXPREDSTONE,SERVER_CAP_EXPREDSTONE			) > ObjMng_ExpRedStone;	
	typedef ObjManager< CAnswerStatus,		OBJ_ID_COUNT( OBJTYPE_ANSWERSTATUS,		SERVER_CAP_PLAYER				) > ObjMng_AnswerStatus;	
	typedef ObjManager< CItemQuestPaper,	OBJ_ID_COUNT( OBJTYPE_ITEM_QUESTIONPAPER,SERVER_CAP_ITEM_QUESTIONPAPER  ) > ObjMng_QuestionPaper;
	typedef ObjManager< CItemMineMap,		OBJ_ID_COUNT( OBJTYPE_ITEM_MINEMAP,		SERVER_CAP_ITEM_MINE			) > ObjMng_MineMap;
	typedef ObjManager< CItemKitBag,		OBJ_ID_COUNT( OBJTYPE_ITEM_KITBAG,		SERVER_CAP_ITEM_KITBAG			) > ObjMng_KitBag;	
	typedef ObjManager< CEntityBlock,		OBJ_ID_COUNT( OBJTYPE_ENTITY_BLOCK,		SERVER_CAP_BLOCK				) > ObjMng_Block;
	typedef ObjManager< CBlockCreator,		OBJ_ID_COUNT( OBJTYPE_CREATOR_BLOCK,	SERVER_CAP_BLOCK				) > ObjMng_Creator_Block;
	typedef ObjManager< CItemToken,			OBJ_ID_COUNT( OBJTYPE_ITEM_TOKEN,		SERVER_CAP_ITEM_TOKEN			) > ObjMng_Token;
	typedef ObjManager< CItemRemove,		OBJ_ID_COUNT( OBJTYPE_ITEM_REMOVE,		SERVER_CAP_ITEM_REMOVE			) > ObjMng_Remove;
	typedef ObjManager< CItemChange,		OBJ_ID_COUNT( OBJTYPE_ITEM_CHANGE,		SERVER_CAP_ITEM_CHANGE			) > ObjMng_Change;
	typedef ObjManager< CItemCall,			OBJ_ID_COUNT( OBJTYPE_ITEM_CALL,		SERVER_CAP_ITEM_CALL			) > ObjMng_ItemCall;
	typedef ObjManager< CItemGiftBag,		OBJ_ID_COUNT( OBJTYPE_ITEM_GIFTBAG,		SERVER_CAP_ITEM_GIFTBAG			) > ObjMng_ItemGiftBag;
	typedef ObjManager< CItemMagicStone,	OBJ_ID_COUNT( OBJTYPE_ITEM_MAGICSTONE,	SERVER_CAP_MAGICSTONE			) > ObjMng_MagicStone;
	typedef ObjManager< CItemLifeSkillScroll, OBJ_ID_COUNT( OBJTYPE_ITEM_LIFESKILLSCROLL,	SERVER_CAP_LIFESKILLSCROLL ) > ObjMng_LifeSkillScroll;
	typedef ObjManager< CItemMagicWeapon,	OBJ_ID_COUNT( OBJTYPE_ITEM_MAGICWEAPON,	SERVER_CAP_ITEM_MAGICWEAPON		) > ObjMng_ItemMagicWeapon;
	typedef ObjManager< CItemFuncItem,		OBJ_ID_COUNT( OBJTYPE_ITEM_FUNCITEM,	SERVER_CAP_ITEM_FUNC			) > ObjMng_ItemFuncItem;
	typedef ObjManager< CItemSkillBook,		OBJ_ID_COUNT( OBJTYPE_ITEM_SKILLBOOK,		SERVER_CAP_ITEM_SKILLBOOK	) > ObjMng_ItemSkillBook;
	typedef ObjManager< CFamilyInfo,		OBJ_ID_COUNT( OBJTYPE_FAMILYINFO,		SERVER_CAP_FAMILYINFO			) >	ObjMng_FamilyInfo;
	typedef ObjManager< CIbTrans,			OBJ_ID_COUNT( OBJTYPE_IBTRANS,			SERVER_CAP_IBTRANS				) >	ObjMng_IbTrans;
	typedef ObjManager< CItemRedPaper,		OBJ_ID_COUNT( OBJTYPE_ITEM_REDPAPER,	SERVER_CAP_ITEM_REDPAPER		) > ObjMng_RedPaper;
	typedef ObjManager< CAchieveDataUnit,	OBJ_ID_COUNT( OBJTYPE_ACHIEVE,			SERVER_CAP_PLAYER				) > ObjMng_Achieve;
	typedef ObjManager< CItemVip,			OBJ_ID_COUNT( OBJTYPE_ITEM_VIP,			SERVER_CAP_ITEM_VIP				) > ObjMng_ItemVip;
	typedef ObjManager< CItemClean,			OBJ_ID_COUNT( OBJTYPE_ITEM_CLEAN,		SERVER_CAP_ITEM_CLEAN			) > ObjMng_ItemClean;
	typedef ObjManager< CItemLeagueBag,		OBJ_ID_COUNT( OBJTYPE_ITEM_LEAGUEBAG,	SERVER_CAP_ITEM_LEAGUEBAG		) > ObjMng_LeagueBag;
	typedef ObjManager< CRedstoneController, OBJ_ID_COUNT( OBJTYPE_REDSTONECONTROLLER, SERVER_CAP_REDSTONECONTROLLER ) > ObjMng_RedStoneController;
	typedef ObjManager< CItemInvitation,	OBJ_ID_COUNT( OBJTYPE_ITEM_INVITATION,	SERVER_CAP_ITEM_INVITATION ) > ObjMng_Invitation;	
	typedef ObjManager< CMarriageInfo,		OBJ_ID_COUNT( OBJTYPE_MARRIAGEINFO,		SERVER_CAP_MARRIAGEINFO ) >	ObjMng_MarriageInfo;	
	typedef ObjManager< CItemSysInvitation,	OBJ_ID_COUNT( OBJTYPE_ITEM_SYSINVITATION,	SERVER_CAP_ITEM_SYSINVITATION ) >	ObjMng_SysInvitation;		
	typedef ObjManager< CItemBlackPills,	OBJ_ID_COUNT( OBJTYPE_ITEM_BLACKPILLS, SERVER_CAP_ITEM_BLACKPILLS ) >   ObjMng_BlackPills;		
	typedef ObjManager< CItemExpContainer,	OBJ_ID_COUNT( OBJTYPE_ITEM_EXPCONTAINER, SERVER_CAP_ITEM_EXPCONTAINER  ) > ObjMng_ExpContainer;
	typedef ObjManager< CItemExpPills,	OBJ_ID_COUNT( OBJTYPE_ITEM_EXPPILLS, SERVER_CAP_ITEM_EXPPILLS               ) > ObjMng_ExpPills; 
	typedef ObjManager< CEntityGhost,	    OBJ_ID_COUNT( OBJTYPE_ENTITY_GHOST,     SERVER_CAP_GHOST	            ) > ObjMng_EntityGhost;	
	typedef ObjManager< CGhostCreator,	    OBJ_ID_COUNT( OBJTYPE_CREATOR_GHOST,     SERVER_CAP_GHOST	            ) > ObjMng_GhostCreator;	
	typedef ObjManager< CStallRecord,		OBJ_ID_COUNT( OBJTYPE_STALL_RECORD,		SERVER_CAP_STALLRECORD			) > ObjMng_StallRecord;
	typedef ObjManager< CFuncTrans,			OBJ_ID_COUNT( OBJTYPE_FUNCTRANS,		SERVER_CAP_FUNCTRANS			) > ObjMng_FuncTrans;
public:

	CSceneObjManagerImp();
	~CSceneObjManagerImp();

	void* operator new( size_t size );
	void  operator delete( void* pointer );
	void* operator new(size_t size, const char* file, int line);
	void operator delete(void* pointer, const char* file, int line);

	int Initailize();
	int Resume();

	static unsigned int CountSize();

public:

	static CSharedMem* mShmPtr; // 共享内存指针

	// 游戏对象
	ObjMng_EntityPlayer		mEntityPlayerPool;
	ObjMng_EntityNpc		mEntityNpcPool;
	ObjMng_EntityPet		mEntityPetPool;
	ObjMng_EntityOgre		mEntityOgrePool;
	ObjMng_EntityItem		mEntityItemPool;
	ObjMng_EntityPedal		mEntityPedalPool;
	ObjMng_EntityCollect	mEntityCollectPool;
	ObjMng_OgreCreator		mOgreCreatorPool;
	ObjMng_BoxCreator		mBoxCreatorPool;
	ObjMng_NpcCreator		mNpcCreatorPool;
	ObjMng_ItemCreator		mItemCreatorPool;
	ObjMng_PedalCreator		mPedalCreatorPool;
	ObjMng_CollectCreator	mCollectCreatorPool;
	ObjMng_ItemBook			mItemBook;
	ObjMng_ItemEquipUpg		mItemEquipUpg;
	ObjMng_ItemExp			mItemExp;
	ObjMng_ItemPhysic		mItemPhysic;
	ObjMng_ItemRelife		mItemRelife;
	ObjMng_ItemReturn		mItemReturn;
	ObjMng_ItemSkill		mItemSkill;
	ObjMng_ItemTeleport		mItemTeleport;
	ObjMng_ItemEquipment	mItemEquipment;
	ObjMng_ItemTask			mItemTask;
	ObjMng_ItemSpecialTask	mItemSpecialTask;
	ObjMng_ItemStuff		mItemStuff;
	ObjMng_ItemNote			mItemNote;
	ObjMng_ItemRune			mItemRune;
	ObjMng_ItemJewel		mItemJewel;
	ObjMng_ItemYuanBug		mItemYuanBug;
	ObjMng_ItemYuanEgg      mItemYuanEgg;
    ObjMng_ItemShout		mItemShout;
	ObjMng_ItemAutoShout    mItemAutoShout;
	ObjMng_ItemHorse		mItemHorse;
	ObjMng_StoreBox			mStoreBox;
	ObjMng_Card				mItemCard;
	ObjMng_Scroll			mItemScroll;
	ObjMng_RelivePosition	mRelivePosition;
	ObjMng_LMapObject		mLMapObject;
	ObjMng_MMapObject		mMMapObject;
	ObjMng_SMapObject		mSMapObject;
	ObjMng_NpcPolicy		mNpcPolicy;
	ObjMng_Chat				mChat;
	ObjMng_Team			    mTeam;
	ObjMng_Trade			mTrade;
	ObjMng_MailInfor        mMailInfor;
	ObjMng_MailCache        mMailCache;  
	ObjMng_BindStone        mItemBindStone;
	ObjMng_Task				mTaskList;
	ObjMng_Stall			mStall;
	ObjMng_Session			mSession;
	ObjMng_Repetion			mRepetion;
	ObjMng_ReturnPaper		mItemPaper;
	ObjMng_TeleSymbol		mItemSymbol;
	ObjMng_Stove			mStove;
	ObjMng_ExpRedStone		mExpRedStone;
	ObjMng_AnswerStatus     mAnswerStatus;
	ObjMng_QuestionPaper	mQuestionPaper;
	ObjMng_MineMap			mMineMap;
	ObjMng_KitBag			mKitBag;
	ObjMng_Block			mBlock;
	ObjMng_Creator_Block	mBlockCreator;
	ObjMng_Token			mToken;
	ObjMng_Remove			mRemove;
	ObjMng_Change			mChange;
	ObjMng_ItemCall			mItemCall;
	ObjMng_ItemGiftBag		mItemGiftBag;
	ObjMng_MagicStone		mMagicStone;
	ObjMng_LifeSkillScroll	mLifeSkillScroll;
	ObjMng_ItemMagicWeapon	mItemMagicWeapon;
	ObjMng_ItemFuncItem		mItemFuncItem;
	ObjMng_ItemSkillBook    mItemSkillBook;
	ObjMng_FamilyInfo		mFamilyInfo;
	ObjMng_IbTrans			mIbTransPool;
	ObjMng_RedPaper			mRedPaper;
	ObjMng_Achieve			mAchieve;
	ObjMng_ItemVip			mItemVip;
	ObjMng_ItemClean		mItemClean;
	ObjMng_LeagueBag		mLeagueBag;
	ObjMng_RedStoneController mRedstoneController;
	ObjMng_Invitation		mInvitation;
	ObjMng_MarriageInfo		mMarriageInfo;
	ObjMng_SysInvitation	mSysInvitation;	
	ObjMng_PetCreator		mPetCreator;
	ObjMng_BlackPills		mBlackPills;
	ObjMng_ExpContainer		mExpContainer;
	ObjMng_ExpPills			mExpPills;
	ObjMng_EntityGhost		mEntityGhostPool;
	ObjMng_GhostCreator		mGhostCreatorPool;
	ObjMng_StallRecord		mStallRecord;
	ObjMng_FuncTrans		mFuncTrans;
};


