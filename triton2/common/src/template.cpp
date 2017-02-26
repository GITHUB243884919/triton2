// ********************************************************************** //
// Template.cpp
// 模版数据的定义文件
// 完成对所有模版数据的内存定义, 和读取
// *****************
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "template.h"
#include "template_pb.hxx.pb.h"
#include "protype.h"
/*
	服务器和客户端统一接口：
		1、TPL_NEW()和TPL_DELETE()在Linux版本下相当于calloc()和free()；
		2、PRINTF_LOG：Linux下为printf，在Windows下为GmDebugLog()，统一了日志输出。
	修改作者：Zhaofine
	修改日期：2009-02-04
*/
//#define TEMPLATE_CACHE_SIZE 32 * 1024 * 1024	// 大Cache。
#define PERCENT 10000.0f


#ifdef LINUX
#	define PRINTF_LOG	printf
#	define TPL_NEW(s)  new s
#	define TPL_DELETE(s)	delete s
#	include <set>
#else
#	define PRINTF_LOG	GmDebugLog
#	include "KmBlade.h"
#	include "KmMemory.h"
#	include "CommonDefine.h"
#	define TPL_NEW(s)  new s
#	define TPL_DELETE(s)	delete s
#endif


#ifndef TEMPEDIT	//编辑器定义该宏
	#ifdef LINUX
		#include "log.h"
		#include "base.h"
	#else
		#define LOG_ERROR	
	#endif
#else
	#define LOG_ERROR
#endif

//#pragma warning( disable : 4996 )
// ********************************************************************** //
// CDataStatic
// ********************************************************************** //
CTemplate* CDataStatic::spTemplate[ MAX_TEMPLATE ]							= { NULL };
void* CDataStatic::spSkill[ MAX_SKILL ][ MAX_LEVEL ]					= { {NULL} };
void* CDataStatic::spLevelExp											= NULL;
void* CDataStatic::spData												= NULL;
void* CDataStatic::spUpgrade											= NULL;
void* CDataStatic::spOpenSlot											= NULL;
void* CDataStatic::spProSuit											= NULL;
void* CDataStatic::spProExp												= NULL;
void* CDataStatic::spBeginner											= NULL;
void* CDataStatic::spDropLevel											= NULL;
void* CDataStatic::spHonorToExp											= NULL;
void* CDataStatic::spHonorObtain										= NULL;
void* CDataStatic::spReliveTable										= NULL;
unsigned short CDataStatic::sMetierID[ MAX_METIER ]						= { 0 };
unsigned short CDataStatic::sGenerID[ MAX_GENER ]						= { 0 };
unsigned short CDataStatic::sComposeID[ MAX_COMPOSE ]					= { 0 };
int CDataStatic::sTemplateVersion										= 0;
int CDataStatic::sTaskVersion											= 0;
CProduceTable< PRODUCE_RANK, PRODUCE_LEVEL, PRODUCE_DETAIL2 > CDataStatic::sProEquip[ PRODUCE_TYPE ][ PRODUCE_DETAIL1 ]	= { {{{{{0}}}}} };
int CDataStatic::sProPhys[ 8 ][ 4 ]										= { {0} };
//符文第二期
int CDataStatic::sRuneLevelID[4][400]									={{0}};
int CDataStatic::sSuitID[ MAX_SUIT ]									= { 0 };
unsigned int CDataStatic::sAutoChatItemId								= 0;
unsigned int CDataStatic::sBugleItemId									= 0;
void* CDataStatic::spMetierProperty[ MAX_METIER ][ PROPERTY_OPTION ]	= { {NULL} };
void* CDataStatic::spLevelProperty[ PROPERTY_OPTION ]					= { NULL };
void* CDataStatic::spQuestionExpTable									= NULL;
void* CDataStatic::spRedStoneExpTable									= NULL;
void* CDataStatic::spBindChargeTable									= NULL;
void* CDataStatic::spPKValueTable										= NULL;
void* CDataStatic::spTalent												= NULL;
void* CDataStatic::spTitleManager										= NULL;
void* CDataStatic::spMultiBuff											= NULL;
void* CDataStatic::spCDGroup											= NULL;
void* CDataStatic::spLight												= NULL;
void* CDataStatic::spApConfig											= NULL;
void* CDataStatic::spWizard												= NULL;
void* CDataStatic::spJudgeConfig										= NULL;
int CDataStatic::spOffical[ OFFICAL_NUM ][ MAX_POWER ]					= { { 0 } };
int CDataStatic::spKing[ MAX_POWER ]									= { 0 };
void* CDataStatic::spOfficalTable										= NULL;
void* CDataStatic::spConstructExp										= NULL;
void* CDataStatic::spAddPKTable											= NULL;
void* CDataStatic::spMapTable											= NULL;
void* CDataStatic::spMultiAttackTable									= NULL;
void* CDataStatic::spDoubleTimeTable									= NULL;
void* CDataStatic::spMatrixTable										= NULL;
void* CDataStatic::spCountTable											= NULL;
void* CDataStatic::spDirectTable										= NULL;
void* CDataStatic::spBattleConfig										= NULL;
void* CDataStatic::spMoneyLmtConfig										= NULL;
void* CDataStatic::spFangCunTree										= NULL;
void* CDataStatic::spTalentStage										= NULL;
void* CDataStatic::spHandinEncourage[MAX_HANDINENCOURAGE]               = {NULL};
void* CDataStatic::spHandinTable[MAX_HANDINTABLE]                       = {NULL};
void* CDataStatic::spAccuseConfig										= NULL;
void* CDataStatic::spAchieve											= NULL;
void* CDataStatic::spGlobalMapConfig									= NULL;
void* CDataStatic::spOffLineExpConfig									= NULL;
void* CDataStatic::spRepeFameTable										= NULL;
void* CDataStatic::spCastingSpiritTable									= NULL;

void* CDataStatic::spDecompund											= NULL;	//	装备分解表
int	 CDataStatic::mFamilyExpID											= 0;	//	家族经验令牌id
int CDataStatic::mFamilyRedstoneID										= 0;	// 家族内丹ID
void *CDataStatic::mMarriageConfig										= NULL;	// 结婚配置

CLifeSkillInfo CDataStatic::mCollect;										// 采集技能信息
CLifeSkillInfo CDataStatic::mGoldMake;										// 点金技能信息	
CLifeSkillInfo CDataStatic::mArtificer;										// 巧匠技能信息
CLifeSkillInfo CDataStatic::mPillMaker;										// 炼丹技能信息
CLifeSkillInfo CDataStatic::mCooker;										// 烹饪技能信息	

void* CDataStatic::spProgressConfig = NULL;									// 进度条配置信息
void* CDataStatic::mActivityConfig = NULL;									// 活动配置表信息

void* CDataStatic::spChangeCamp = NULL;
void* CDataStatic::spCorpsBOSSTable = NULL;

//void* CDataStatic::spRuneConfig  =NULL;

void*  CDataStatic::spComRuneConfig								= NULL;//
//---------------------------------------------
// Begin: 实现各类型的SetFromPB
//---------------------------------------------

int CTplItem::Initialize()
{
	mItemType = 0;			
	mProcessType = 0;			
	mPileLimit = 0;	 
	memset( mItemName, 0, sizeof(mItemName) );
	memset( mModelName, 0, sizeof( mModelName )) ;	
	memset( mIconName, 0, sizeof( mIconName ));
	mPriceSale = 0;
	mPricePurchase = 0;	
	memset( mDescribe, 0, sizeof( mDescribe )) ;
	memset( mTypeDesc, 0, sizeof( mTypeDesc ));
	mPurchaseBindMoney = 0;
	mLastTime = 0;	
	mSaleBindMoney = 0;	
	mStopAttack = 0;
	mCanUse = 0;
	mSkillID = 0;
	mLevel = 0;
	mDisappear=0;
	memset( mCDGroup, 0, sizeof(mCDGroup));
	mValidTime = 0;
	mValidType = 0;
	memset( mLmtMapID, 0, sizeof(mLmtMapID));
	memset( mLmtRepetionID, 0, sizeof( mLmtRepetionID ) );
	mCommonID = 0;
	mEatType = 0;
	mEatValue = 0;
	mCountType = 0;
	mLmtLineID = 0;
	memset( mLmtUseMapID, 0, sizeof(mLmtUseMapID));
	mCompID = 0;
	return 0;
}

void CTplItem::SetFromPB( bool bServer,PBItemBase* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	mTempID				= pItem->tempid();
	mItemType			= pItem->itemtype();
	mProcessType		= pItem->processtype();
	mPileLimit			= pItem->pilelimit();
	strncpy( mItemName, pItem->itemname().c_str(), sizeof( mItemName ) - 1 );
	strncpy( mModelName, pItem->modelname().c_str(), sizeof( mModelName ) - 1 );
	strncpy( mIconName, pItem->iconname().c_str(), sizeof( mIconName ) -1 );
	mPricePurchase		= pItem->purchaseprice();
	mPriceSale			= pItem->saleprice();
	strncpy( mDescribe, pItem->describe().c_str(), sizeof( mDescribe ) - 1 );
	mPurchaseBindMoney	= pItem->purchasebindmoney();
	mSaleBindMoney		= pItem->salebindmoney();
	mLastTime			= pItem->lasttime();
	mStopAttack			= pItem->stopattack();
	strncpy( mTypeDesc, pItem->typedesc().c_str(), sizeof( mTypeDesc ) - 1 );
	mCanUse				= pItem->canuse();
	mSkillID			= pItem->skillid();
	mLevel				= pItem->level();
	mDisappear			= pItem->disappear();
	for ( int i = 0; i < pItem->cdgroup_size() && i < (int)ARRAY_CNT( mCDGroup ); i ++ )
	{
		mCDGroup[ i ] = pItem->cdgroup( i );
	}
	for( int i = 0; i < pItem->lmtmapid_size( ) && i < ( int )ARRAY_CNT( mLmtMapID ); i++ )
	{
		mLmtMapID[ i ]			= pItem->lmtmapid( i );
	}
	for( int i = 0; i < pItem->lmtrepetionid_size( ) && i < ( int )ARRAY_CNT( mLmtRepetionID ); i++ )
	{
		mLmtRepetionID[ i ] 	= pItem->lmtrepetionid( i );
	}
	mValidTime			= pItem->validtime();
	mValidType			= pItem->validtype();
	mCommonID			= pItem->commonid();
	mEatType			= pItem->eattype();
	mEatValue			= pItem->eatvalue();
	mCountType			= pItem->counttype();
	mLmtLineID			= pItem->lmtlineid();
	for( int i = 0; i < pItem->lmtusemapid_size( ) && i < ( int )ARRAY_CNT( mLmtUseMapID ); i++ )
	{
		mLmtUseMapID[ i ]			= pItem->lmtusemapid( i );
	}
	mCompID			= pItem->compid();
	mInvilidDate    = pItem->inviliddate();
	return ;
}

void CTplItem::CreateTlvPB( bool bServer,PBItemBase* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	pItem->set_tempid( mTempID );
	pItem->set_itemtype( mItemType );
	pItem->set_processtype( mProcessType );
	pItem->set_pilelimit( mPileLimit );
	pItem->set_itemname( mItemName );
	pItem->set_modelname( mModelName );
	pItem->set_iconname( mIconName );
	pItem->set_purchaseprice( mPricePurchase );
	pItem->set_saleprice( mPriceSale );
	pItem->set_describe( mDescribe );
	pItem->set_purchasebindmoney( mPurchaseBindMoney );
	pItem->set_salebindmoney( mSaleBindMoney );
	pItem->set_lasttime( mLastTime );
	pItem->set_stopattack( mStopAttack );
	pItem->set_typedesc( mTypeDesc );
	pItem->set_canuse( mCanUse );
	pItem->set_skillid( mSkillID );
	pItem->set_level( mLevel );
	pItem->set_disappear( mDisappear );
	for ( int i = 0; i < (int)ARRAY_CNT( mCDGroup ); i ++ )
	{
		pItem->add_cdgroup( mCDGroup[ i ] );
	}
	for( int i = 0; i < ( int )ARRAY_CNT( mLmtMapID ); i++ )
	{
		if( mLmtMapID[ i ] == 0 )
		{
			break;
		}
		pItem->add_lmtmapid( mLmtMapID[ i ] );
	}

	for( int i = 0; i < ( int )ARRAY_CNT( mLmtRepetionID ); i++ )
	{
		if( mLmtRepetionID[ i ] == 0 )
		{
			break;
		}
		pItem->add_lmtrepetionid( mLmtRepetionID[ i ] );
	}
	pItem->set_validtime( mValidTime );
	pItem->set_validtype( mValidType );
	pItem->set_commonid( mCommonID );
	pItem->set_eattype( mEatType );
	pItem->set_eatvalue( mEatValue );
	pItem->set_counttype( mCountType );
	pItem->set_lmtlineid( mLmtLineID );
	for( int i = 0; i < ( int )ARRAY_CNT( mLmtUseMapID ); i++ )
	{
		if( mLmtUseMapID[ i ] == 0 )
		{
			break;
		}
		pItem->add_lmtusemapid( mLmtUseMapID[ i ] );
	}
	pItem->set_compid( mCompID );
	pItem->set_inviliddate(mInvilidDate);
	return ;
}

int CTplItemReturn::Initialize()
{
	mTimer = 0;	
	return 0;
}

void CTplItemReturn::SetFromPB( bool bServer,PBItemReturn *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mTimer	= pItem->timer();
	return ;
}

void CTplItemReturn::CreateTlvPB( bool bServer,PBItemReturn *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_timer( mTimer );
	return ;
}

int CTplFuncItem::Initialize()
{
	mFuncCode = 0;	
	memset( mParams, 0, sizeof(mParams));
	return 0;
}


void CTplFuncItem::SetFromPB( bool bServer,PBFuncItem *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mFuncCode = pItem->funccode();
	for ( int i = 0 ; ( i < (int)(sizeof(this->mParams)/sizeof(this->mParams[0])) && i < pItem->params_size() ); ++i )
	{
		mParams[i] = pItem->params(i);
	}
	return ;
}

void CTplFuncItem::CreateTlvPB( bool bServer,PBFuncItem *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_funccode( mFuncCode );
	for ( unsigned int i = 0 ; i < (int)(sizeof(this->mParams)/sizeof(this->mParams[0]))  ; ++i )
	{
		pItem->add_params( mParams[i] );
	}
	return ;
}


int CTplItemSkillBook::Initialize()
{
	mBookColor = 0;
	mSkillID = 0;
	return 0;
}

void CTplItemSkillBook::SetFromPB( bool bServer,PBItemSkillBook *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mBookColor = pItem->color();
	mSkillID = pItem->skillid();
	mSuccProb = pItem->succprob();
	mNeedMoneyType = pItem->needmoneytype();
	mNeedMoney = pItem->needmoney();
	return ;
}

void CTplItemSkillBook::CreateTlvPB( bool bServer,PBItemSkillBook *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_color( mBookColor );
	pItem->set_skillid( mSkillID );
	pItem->set_succprob(mSuccProb);
	pItem->set_needmoneytype(mNeedMoneyType);
	pItem->set_needmoney(mNeedMoney);
	return ;
}


void CTplItemBindStone::SetFromPB( bool bServer,PBItemBindStone *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mFaceGroupID = pItem->facegroupid();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mBindStoneType = pItem->bindstonetype();
	return ;
}


int CTplItemBindStone::Initialize()
{
	mFaceGroupID = 0;
	mBindStoneType = 0;
	return 0;
}

void CTplItemBindStone::CreateTlvPB( bool bServer,PBItemBindStone* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_facegroupid( mFaceGroupID );
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_bindstonetype( mBindStoneType );
	return ;
}


int CTplItemScroll::Initialize()
{
	mGetTaskID = 0;	
	return 0;
}

void CTplItemScroll::SetFromPB( bool bServer,PBItemScroll *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mGetTaskID	= pItem->gettaskid();
	return ;
}

void CTplItemScroll::CreateTlvPB( bool bServer,PBItemScroll* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_gettaskid( mGetTaskID );
	return ;
}

int CTplItemCall::Initialize()
{
	mCallNpcID = 0; 	
	mX = 0; 	
	mY = 0;	
	mIsDestroy = 0;	
	mMapID = 0;	
	mLmtMapID = 0;	
	mLmtX = 0;	
	mLmtY = 0;
	mLmtWide = 0;	
	mLmtHeight = 0; 	
	return 0;
}

void CTplItemCall::SetFromPB( bool bServer,PBItemCall *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mCallNpcID	= pItem->callnpcid();
	mX = pItem->x();
	mY = pItem->y();
	mIsDestroy = pItem->isdestroy();
	mMapID = pItem->mapid();
	mLmtMapID = pItem->lmtmapid();
	mLmtX = pItem->lmtx();
	mLmtY = pItem->lmty();
	mLmtWide = pItem->lmtwide();
	mLmtHeight = pItem->lmtheight();
	return ;
}

void CTplItemCall::CreateTlvPB( bool bServer,PBItemCall* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_callnpcid(mCallNpcID);
	pItem->set_x(mX);
	pItem->set_y(mY);
	pItem->set_isdestroy(mIsDestroy);
	pItem->set_mapid(mMapID);

	pItem->set_lmtmapid(mLmtMapID);
	pItem->set_lmtx(mLmtX);
	pItem->set_lmty(mLmtY);
	pItem->set_lmtwide(mLmtWide);
	pItem->set_lmtheight(mLmtHeight);
	return ;
}

int CTplItemCard::Initialize()
{
	memset( mShowText, 0, sizeof(mShowText));
	mOrder = 0;	
	memset( mWeapon, 0, sizeof(mWeapon));
	memset( mSkill, 0, sizeof(mSkill));
	memset( mLevel, 0, sizeof(mLevel));
	mExp = 0;	
	mCanExchange = 0;
	mCanChallenge = 0;
	mType = 0;	
	mPos = 0;
	mBonus = 0;
	mSkillID1 = 0;
	mSkillID2 = 0;
	return 0;
}


void CTplItemCard::SetFromPB( bool bServer,PBItemCard *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	strncpy( mShowText, pItem->showtext().c_str(), sizeof( mShowText ) - 1 );
	mOrder	= pItem->order();
	strncpy( mWeapon, pItem->weapon().c_str(), sizeof( mWeapon ) - 1 );
	strncpy( mSkill, pItem->skill().c_str(), sizeof( mSkill ) - 1 );
	strncpy( mLevel, pItem->level().c_str(), sizeof( mLevel ) - 1 );
	mExp = pItem->exp();
	mCanExchange = pItem->canexchange();
	mCanChallenge = pItem->canchallenge();
	mType = pItem->type();
	mPos = pItem->pos();
	mBonus = pItem->bonus();
	mSkillID1 = pItem->skillid1();
	mSkillID2 = pItem->skillid2();
	return ;
}

void CTplItemCard::CreateTlvPB( bool bServer,PBItemCard* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_showtext( mShowText );
	pItem->set_order( mOrder );
	pItem->set_weapon( mWeapon );
	pItem->set_skill( mSkill );
	pItem->set_level( mLevel );
	pItem->set_exp( mExp );
	pItem->set_canexchange( mCanExchange );
	pItem->set_canchallenge( mCanChallenge );
	pItem->set_type( mType );
	pItem->set_pos( mPos );
	pItem->set_bonus( mBonus );
	pItem->set_skillid1( mSkillID1 );
	pItem->set_skillid2( mSkillID2 );
	return ;
}

int CTplItemTeleport::Initialize()
{
	mMapID = 0;		
	mUseTimes = 0;
	return 0;
}

void CTplItemTeleport::SetFromPB( bool bServer,PBItemTeleport *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mMapID				= pItem->mapid();
	mUseTimes			= pItem->usetimes();
	return ;
}

void CTplItemTeleport::CreateTlvPB( bool bServer,PBItemTeleport* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_mapid( mMapID );
	pItem->set_usetimes(mUseTimes);
	return ;
}

int CTplReturnPaper::Initialize()
{
	mLineID = 0;
	mMapID = 0;
	mPosX = 0;
	mPosY = 0;
	return 0;
}

void CTplReturnPaper::SetFromPB( bool bServer,PBItemReturnPaper *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mLineID				= pItem->lineid();
	mMapID				= pItem->mapid();
	mPosX				= pItem->posx();
	mPosY				= pItem->posy();
	return ;
}

void CTplReturnPaper::CreateTlvPB( bool bServer,PBItemReturnPaper* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_lineid( mLineID );
	pItem->set_mapid( mMapID );
	pItem->set_posx( mPosX );
	pItem->set_posy( mPosY );
	return ;
}

int CTplTeleSymbol::Initialize()
{
	mTeleTable = 0;
	mUseTimes = 0;
	return 0;
}

void CTplTeleSymbol::SetFromPB( bool bServer,PBItemTeleSymbol* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mTeleTable			= pItem->teletable();
	mUseTimes			= pItem->usetimes();
	return ;
}

void CTplTeleSymbol::CreateTlvPB( bool bServer,PBItemTeleSymbol* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_teletable( mTeleTable );
	pItem->set_usetimes( mUseTimes );
	return ;
}

int CTplStove::Initialize()
{	
	mLineID = 0;
	mMapID = 0;
	mPosX = 0;
	mPosY = 0;
	mCoolTime = 0;
	mMetierRequire = 0;
	mLevelRequire = 0;

	return 0;
}

void CTplStove::SetFromPB( bool bServer,PBItemStove *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mLineID				= pItem->lineid();
	mMapID				= pItem->mapid();
	mPosX				= pItem->posx();
	mPosY				= pItem->posy();
	mCoolTime			= pItem->cooltime();
	mMetierRequire		= pItem->metierlimit();
	mLevelRequire		= pItem->levellimit();	
	return ;
}

void CTplStove::CreateTlvPB( bool bServer,PBItemStove *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_lineid( mLineID );
	pItem->set_mapid( mMapID );
	pItem->set_posx( mPosX );
	pItem->set_posy( mPosY );
	pItem->set_cooltime( mCoolTime );
	pItem->set_metierlimit( mMetierRequire );
	pItem->set_levellimit( mLevelRequire );	
	return ;
}

int CTplPetEgg::Initialize()
{
	mPetTempID = 0;				
	mNeedItemID = 0;			
	mRevertPrice = 0;			
	mInitAdvice = 0;	
	mInitLevel = 0;
	mInitExp = 0;				
	mInitSkillPoint = 0;		
	memset( mInitSkillID, 0, sizeof( mInitSkillID ) );		
	memset( mInitSkillLevel, 0, sizeof(mInitSkillLevel));
	return 0;
}

void CTplPetEgg::SetFromPB( bool bServer,PBItemPetEgg *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mPetTempID			= pItem->pettempid();
	mNeedItemID			= pItem->needitemid();
	mRevertPrice		= pItem->revertprice();
	mInitAdvice			= pItem->initadvice();
	mInitLevel			= pItem->initlevel();
	mInitExp			= pItem->initexp();
	mInitSkillPoint		= pItem->initskillpoint();
	for ( unsigned int i = 0; i < (unsigned int) pItem->initskillid_size() && i < (unsigned int) ARRAY_CNT( mInitSkillID ); ++ i )
	{
		mInitSkillID[ i ] = pItem->initskillid( i );
	}
	for ( unsigned int j = 0; j < (unsigned int) pItem->initskilllevel_size() && j < (unsigned int) ARRAY_CNT( mInitSkillLevel ); ++ j )
	{
		mInitSkillLevel[ j ] = pItem->initskilllevel( j );
	}
	return ;
}

void CTplPetEgg::CreateTlvPB( bool bServer,PBItemPetEgg* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_pettempid( mPetTempID );
	pItem->set_needitemid( mNeedItemID );
	pItem->set_revertprice( mRevertPrice );
	pItem->set_initadvice( mInitAdvice );
	pItem->set_initlevel( mInitLevel );
	pItem->set_initexp( mInitExp );
	pItem->set_initskillpoint( mInitSkillPoint );
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mInitSkillID ); i ++ )
	{
		pItem->add_initskillid( mInitSkillID[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mInitSkillLevel ); i ++ )
	{
		pItem->add_initskilllevel( mInitSkillLevel[ i ] );
	}
	return ;
}

int CTplPetFood::Initialize()
{
	mInitLevel = 0;	
	mFullLevel = 0;				
	mTypeMask = 0;
	return 0;
}

void CTplPetFood::SetFromPB( bool bServer,PBItemPetFood *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mInitLevel			= pItem->initlevel();
	mFullLevel			= pItem->fulllevel();
	mTypeMask			= pItem->typemask();
	return ;
}

void CTplPetFood::CreateTlvPB( bool bServer,PBItemPetFood* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_initlevel( mInitLevel );
	pItem->set_fulllevel( mFullLevel );
	pItem->set_typemask( mTypeMask );
	return ;
}

int CTplItemRelife::Initialize()
{	
	mTimer = 0;				
	mCooldown = 0;			
	mShareCD = 0;
	return 0;
}

void CTplItemRelife::SetFromPB( bool bServer,PBItemRelife *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mTimer				= pItem->timer();
	mCooldown			= pItem->cooltime();
	mShareCD			= pItem->sharecd();
	return ;
}

void CTplItemRelife::CreateTlvPB( bool bServer,PBItemRelife *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_timer( mTimer );
	pItem->set_cooltime( mCooldown );
	pItem->set_sharecd( mShareCD );
	return ;
}

int CTplItemSkill::Initialize()
{	
	mOprType = 0;
	mLevelLimit = 0;		
	mSkillID = 0;	
	mCooldown = 0;				
	mShareCD = 0;
	mBuffID = 0;
	return 0;
}

void CTplItemSkill::SetFromPB( bool bServer,PBItemSkill *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mLevelLimit			= pItem->levellimit();
	mSkillID			= pItem->skillid();
	mCooldown			= pItem->cooltime();
	mShareCD			= pItem->sharecd();
	mOprType			= pItem->oprtype();
	mBuffID				= pItem->buffid();
	return ;
}

void CTplItemSkill::CreateTlvPB( bool bServer,PBItemSkill* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_levellimit( mLevelLimit );
	pItem->set_skillid( mSkillID );
	pItem->set_cooltime( mCooldown );
	pItem->set_sharecd( mShareCD );
	pItem->set_buffid( mBuffID );
	pItem->set_oprtype( mOprType );
	return ;
}

int CTplItemEquipUpg::Initialize()
{
	mMaxLevel = 0;
	mType = 0;	 
	mLimit = 0;	
	mFail = 0;	 
	memset( mLuck, 0, sizeof(mLuck));
	mUpType = 0;
	return 0;
}

void CTplItemEquipUpg::SetFromPB( bool bServer, PBItemUpg* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mMaxLevel = pItem->maxlevel();
	mType = pItem->type();
	mLimit = pItem->limit();
	mFail = pItem->fail();
	for ( unsigned int i = 0; i < (unsigned int) pItem->luck_size() && i < (unsigned int) ARRAY_CNT( mLuck ); ++ i )
	{
		mLuck[ i ] = pItem->luck( i );
	}
	mUpType = pItem->uptype();
	return;
}
void CTplItemEquipUpg::CreateTlvPB( bool bServer, PBItemUpg* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_maxlevel( mMaxLevel );
	pItem->set_type( mType );
	pItem->set_limit( mLimit );
	pItem->set_fail( mFail );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mLuck ); i ++ )
	{
		pItem->add_luck( mLuck[ i ] );
	}
	pItem->set_uptype( mUpType );
	return;
}

int CTplItemBook::Initialize()
{
	mOPType = 0;
	return 0;
}

void CTplItemBook::SetFromPB( bool bServer,PBItemBook *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mOPType			= pItem->optype();
	return ;
}

void CTplItemBook::CreateTlvPB( bool bServer,PBItemBook* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_optype( mOPType );
	return ;
}


int CTplUpgLuck::Initialize()
{
	mIncLuck1 = 0;	
	mIncLuck2 = 0;	
	memset( mIncLuck3, 0, sizeof( mIncLuck3 ));
	mLevel= 0;	
	return 0;
}


void CTplUpgLuck::SetFromPB( bool bServer,PBItemUpgLuck *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mIncLuck1		= pItem->incluck1();
	mIncLuck2		= pItem->incluck2();
	for ( unsigned int i = 0; i < (unsigned int) pItem->incluck3_size() && i < (unsigned int) ARRAY_CNT( mIncLuck3 ); ++ i )
	{
		mIncLuck3[ i ] = pItem->incluck3( i );
	}
	mLevel = pItem->level();
	return ;
}

void CTplUpgLuck::CreateTlvPB( bool bServer,PBItemUpgLuck *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	
	pItem->set_incluck1( mIncLuck1 );
	pItem->set_incluck2( mIncLuck2 );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mIncLuck3 ); i ++ )
	{
		pItem->add_incluck3( mIncLuck3[ i ] );
	}
	pItem->set_level( mLevel );
	return ;
}

int CTplUpgLeast::Initialize()
{
	mIncLuck1 = 0;			
	mIncLuck2 = 0;	
	mType = 0;
	return 0;
}

void CTplUpgLeast::SetFromPB( bool bServer,PBItemUpgLeast *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mIncLuck1		= pItem->incluck1();
	mIncLuck2		= pItem->incluck2();
	mType = pItem->type();
	return ;
}

void CTplUpgLeast::CreateTlvPB( bool bServer,PBItemUpgLeast *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_incluck1( mIncLuck1 );
	pItem->set_incluck2( mIncLuck2 );
	pItem->set_type( mType );
	return ;
}

int CTplItemRemove::Initialize()
{
	memset( mRand, 0, sizeof(mRand));
	mBind = 0;
	mLevel = 0;
	mLow = 0;
	return 0;
}

void CTplItemRemove::SetFromPB( bool bServer,PBItemRemove *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	if ( bServer )
	{
		for ( unsigned int i = 0; i < (unsigned int) pItem->rand_size() && i < (unsigned int) ARRAY_CNT( mRand ); ++ i )
		{
			mRand[ i ] = pItem->rand( i );
		}
	}
	mBind = pItem->bind();
	mLevel = pItem->level();
	mLow = pItem->low();
	return ;
}

void CTplItemRemove::CreateTlvPB( bool bServer,PBItemRemove *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	if ( bServer )
	{
		for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mRand ); i ++ )
		{
			pItem->add_rand( mRand[ i ] );
		}
	}	
	pItem->set_bind( mBind );
	pItem->set_level( mLevel );
	pItem->set_low( mLow );
	return ;
}

int CTplItemChange::Initialize()
{
	mType = 0;
	mRes = 0;
	mMetierRequire = 0;
	return 0;
}

void CTplItemChange::SetFromPB( bool bServer,PBItemChange *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mType			= pItem->type();
	mRes			= pItem->res();
	mMetierRequire	= pItem->metierrequire();
	return ;
}

void CTplItemChange::CreateTlvPB( bool bServer,PBItemChange* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_type( mType );
	pItem->set_res( mRes );
	pItem->set_metierrequire( mMetierRequire );
	return ;
}

int CTplQuestionPaper::Initialize()
{
	memset( mQuestionRate, 0, sizeof( mQuestionRate )) ;	
	mExpAddRae = 0;	
	mEachTimeLength = 0;	
	mTotalTimeLength = 0;	
	mRepeatTimeAllowed = 0; 	
	return 0;
}

void CTplQuestionPaper::SetFromPB( bool bServer,PBItemQuestion *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	for ( unsigned int i = 0; i < (unsigned int) pItem->questionrate_size() && i < (unsigned int) ARRAY_CNT( mQuestionRate ); ++ i )
	{
		mQuestionRate[ i ] = pItem->questionrate( i );
	}
	mExpAddRae			= pItem->expaddrae();
	mEachTimeLength		= pItem->eachtimelength();
	mTotalTimeLength	= pItem->totaltimelength();
	mRepeatTimeAllowed	= pItem->repeattimeallowed();
	return ;
}

void CTplQuestionPaper::CreateTlvPB( bool bServer,PBItemQuestion *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mQuestionRate ); i ++ )
	{
		pItem->add_questionrate( mQuestionRate[ i ] );
	}
	pItem->set_expaddrae( mExpAddRae );
	pItem->set_eachtimelength( mEachTimeLength );
	pItem->set_totaltimelength( mTotalTimeLength );
	pItem->set_repeattimeallowed( mRepeatTimeAllowed );
	return ;
}

int CTplItemTask::Initialize()
{
	mUseTimes = 0;
	mUsingTime = 0;	
	memset( mShowText, 0, sizeof( mShowText )) ;
	mMapID = 0;	
	mX = 0;	
	mY = 0;	
	mWide = 0;	
	mHeight = 0;	
	mGetTaskID = 0;	
	mGiveItemID = 0;	
	mChangeTime = 0;
	mChangeItem = 0;	
	mFailTaskID = 0;	
	return 0;
}

void CTplItemTask::SetFromPB( bool bServer,PBItemTask *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mUseTimes			= pItem->usetimes();
	mUsingTime			= pItem->validtime();
	strncpy( mShowText, pItem->showtext().c_str(), sizeof( mShowText ) - 1 );
	mMapID				= pItem->mapid();
	mX					= pItem->posx();
	mY					= pItem->posy();
	mWide				= pItem->width();
	mHeight				= pItem->height();
	mGetTaskID			= pItem->gettaskid();
	mGiveItemID			= pItem->giveitemid();
	mChangeTime			= pItem->changetime();
	mChangeItem			= pItem->changeitem();
	mFailTaskID			= pItem->failtaskid();
	return ;
}

void CTplItemTask::CreateTlvPB( bool bServer,PBItemTask *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_usetimes( mUseTimes );
	pItem->set_validtime( mUsingTime );
	pItem->set_showtext( mShowText );
	pItem->set_mapid( mMapID );
	pItem->set_posx( mX );
	pItem->set_posy( mY );
	pItem->set_width( mWide );
	pItem->set_height( mHeight );
	pItem->set_gettaskid( mGetTaskID );
	pItem->set_giveitemid( mGiveItemID );
	pItem->set_changetime( mChangeTime );
	pItem->set_changeitem( mChangeItem );
	pItem->set_failtaskid( mFailTaskID );
	return ;
}

int CTplItemStuff::Initialize()
{
	mShowColour = 0;
	mComposeID = 0;
	return 0;
}
void CTplItemStuff::SetFromPB( bool bServer,PBItemStuff *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mShowColour	= pItem->showcolor();
	mComposeID	= pItem->composeid();
	return ;
}

void CTplItemStuff::CreateTlvPB( bool bServer,PBItemStuff* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_showcolor( mShowColour );
	pItem->set_composeid( mComposeID );
	return ;
}

int CTplItemExp::Initialize()
{
	mIncRate = 0;		 
	mTime = 0;	
	return 0;
}

void CTplItemExp::SetFromPB( bool bServer,PBItemExp *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mIncRate		= pItem->incrate();
	mTime			= pItem->time();
	return ;
}

void CTplItemExp::CreateTlvPB( bool bServer,PBItemExp* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_incrate( (int)( mIncRate  ) );
	pItem->set_time( mTime );
	return ;
}


int CTemPlateChangeCampCfg::Initialize()
{
	m_itemId = 0 ; //转阵营道具id
	m_giftId = 0; //转阵营到弱阵营 礼包id
    m_cd_time= 0; //cd时间
	return 0;
}

void CTemPlateChangeCampCfg::SetFromPB( bool bServer,PBItemChangeCamp *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	mTempID			= pItem->tmpid();
	m_itemId		= pItem->m_itemid();
	m_giftId		= pItem->m_giftid();
	m_cd_time		= pItem->m_cd_time();
	return ;
}

void CTemPlateChangeCampCfg::CreateTlvPB( bool bServer,PBItemChangeCamp* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	pItem->set_tmpid(mTempID);
	pItem->set_m_itemid(m_itemId);
	pItem->set_m_giftid(m_giftId);
	pItem->set_m_cd_time(m_cd_time);
	return ;
}


//符文复杂升级配置

int CTemplateComRuneCfg::Initialize()
{
	m_upRuneRand  =0;		//升级符的概率
	m_max   =0;
	m_min    =0;
	memset(m_Rune,0,sizeof(m_Rune));

	return 0;
}

void CTemplateComRuneCfg::SetFromPB( bool bServer,PBComRuneConfig *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	mTempID					= pItem->tmpid();
	m_upRuneRand			= pItem->m_uprunerand();
	m_max					= pItem->m_max();
	m_min					= pItem->m_min();
	for (int i = 0;i<pItem->m_upruneunit_size()&&i<(int)ARRAY_CNT(m_Rune);i++)
	{
		PBComRuneUnit* runeunit = pItem->mutable_m_upruneunit(i);
		m_Rune[i].m_whitenum = runeunit->m_whitenum();
		m_Rune[i].m_bluenum  = runeunit->m_bluenum();
		m_Rune[i].m_purplenum =  runeunit->m_purplenum();
		m_Rune[i].m_rand       = runeunit->m_rand();
		m_Rune[i].m_money      = runeunit->m_money();
		m_Rune[i].m_runeLevel  = runeunit->m_runelevel();
	}
	return ;
}

void CTemplateComRuneCfg::CreateTlvPB( bool bServer,PBComRuneConfig* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	pItem->set_tmpid(mTempID);
	pItem->set_m_uprunerand(m_upRuneRand);
	pItem->set_m_max(m_max);
	pItem->set_m_min(m_min);
	for( int i = 0; i < (int)ARRAY_CNT( m_Rune ); i++ )
	{
		PBComRuneUnit* runeunit = pItem->add_m_upruneunit();
		//runeunit->set_m_uprunetype(m_Rune[i].m_upRuneType);
		runeunit->set_m_whitenum(m_Rune[i].m_whitenum);
		runeunit->set_m_bluenum(m_Rune[i].m_bluenum);
		runeunit->set_m_purplenum(m_Rune[i].m_purplenum);
		runeunit->set_m_rand(m_Rune[i].m_rand);
		runeunit->set_m_money(m_Rune[i].m_money);
		runeunit->set_m_runelevel(m_Rune[i].m_runeLevel);
	}

	return ;
}



int CTplItemShout::Initialize()
{
	mFaceGroupID = 0;
	return 0;
}

void CTplItemShout::SetFromPB( bool bServer,PBItemShout *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mFaceGroupID		= pItem->facegroupid();
	return ;
}

void CTplItemShout::CreateTlvPB( bool bServer,PBItemShout *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_facegroupid( mFaceGroupID );
	return ;
}

int CTplItemAutoShout::Initialize()
{
	mFaceGroupID  = 0;
	return 0;
}

void CTplItemAutoShout::SetFromPB( bool bServer,PBItemAutoShout *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mFaceGroupID		= pItem->facegroupid();
	return ;
}

void CTplItemAutoShout::CreateTlvPB( bool bServer,PBItemAutoShout* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_facegroupid( mFaceGroupID );
	return ;
}

int CTplItemPhysic::Initialize()
{
	mShowType= 0;	
	mBuffID= 0;
	mLevelRequire= 0;	
	mHPValue= 0;			
	mHPTime= 0;
	mHPIncTime= 0;
	mHPIncValue= 0;
	mMPValue= 0;			
	mMPTime= 0;
	mMPIncTime= 0;
	mMPIncValue= 0;
	mCooldown= 0;			
	mShareCD= 0;	
	mHPPercent= 0;
	mMPPercent= 0;
	mMaxHPPercent= 0;
	mMaxMPPercent= 0;
	mLifeSkillType= 0;
	mDegreeValue= 0;
	mPhysicType= 0;	
	mApValue= 0;
	mColor = 0;
	return 0;
}

void CTplItemPhysic::SetFromPB( bool bServer,PBItemPhysic *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mShowType			= pItem->showtype();
	mBuffID				= pItem->buffid();
	mLevelRequire		= pItem->levellimit();
	mHPValue			= pItem->hpmaxvalue();
	mHPTime				= pItem->hptime();
	mHPIncTime			= pItem->hpinctime();
	mHPIncValue			= pItem->hpinvvalue();
	mMPValue			= pItem->mpmaxvalue();
	mMPTime				= pItem->mptime();
	mMPIncTime			= pItem->mpinctime();
	mMPIncValue			= pItem->mpinvvalue();
	mCooldown			= pItem->cooltime();
	mShareCD			= pItem->sharecd();
	mHPPercent			= pItem->hppercent();
	mMPPercent			= pItem->mppercent();
	mMaxHPPercent		= pItem->maxhppercent();
	mMaxMPPercent		= pItem->maxmppercent();
	mLifeSkillType		= pItem->lifeskiltype();
	mDegreeValue		= pItem->degreevalue();	
	mPhysicType			= pItem->phisictype();
	mApValue			= pItem->apvalue();
	mColor				= pItem->color();
	return ;
}

void CTplItemPhysic::CreateTlvPB( bool bServer,PBItemPhysic* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_showtype( mShowType );
	pItem->set_buffid( mBuffID );
	pItem->set_levellimit( mLevelRequire );
	pItem->set_hpmaxvalue( mHPValue );
	pItem->set_hptime( mHPTime );
	pItem->set_hpinctime( mHPIncTime );
	pItem->set_hpinvvalue( mHPIncValue );
	pItem->set_mpmaxvalue( mMPValue );
	pItem->set_mptime( mMPTime );
	pItem->set_mpinctime( mMPIncTime );
	pItem->set_mpinvvalue( mMPIncValue );
	pItem->set_cooltime( mCooldown );
	pItem->set_sharecd( mShareCD );
	pItem->set_hppercent( mHPPercent );
	pItem->set_mppercent( mMPPercent );
	pItem->set_maxhppercent( mMaxHPPercent );
	pItem->set_maxmppercent( mMaxMPPercent );
	
	pItem->set_lifeskiltype( mLifeSkillType );
	pItem->set_degreevalue( mDegreeValue );
	pItem->set_phisictype( mPhysicType );
	pItem->set_apvalue( mApValue );
	pItem->set_color( mColor );
	return ;
}

int CTplItemHorse::Initialize()
{
	mMetierRequire = 0;
	mLevelRequire = 0;
	mPersonRequire = 0;
	mUseTime = 0;
	mIncreaseSpeedPer = 0;
	memset( mModelName, 0, sizeof(mModelName));
	mCooldown = 0;
	mShareCD = 0;
	mSpeedBuff = 0;
	mUsingTime = 0;	
	memset( mShowText, 0, sizeof(mShowText));
	mOffHorseProb = 0;
	mOffBuffID = 0;
	mInitAP = 0;
	mDeltaAP = 0;
	return 0;
}

void CTplItemHorse::SetFromPB( bool bServer,PBItemHorse *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	mMetierRequire			= pItem->metierlimit();
	mLevelRequire			= pItem->levellimit();
	mPersonRequire			= pItem->personlimit();
	mUseTime				= pItem->usetime();
	mIncreaseSpeedPer		= pItem->increasespeedper();
	strncpy( mModelName, pItem->modelname().c_str(), sizeof( mModelName ) - 1 );
	mCooldown				= pItem->cooltime();
	mShareCD				= pItem->sharecd();
	mSpeedBuff				= pItem->speedbuff();
	mUsingTime				= pItem->usingtime();
	strncpy( mShowText, pItem->showtext().c_str(), sizeof( mShowText ) - 1 );
	mOffHorseProb			= pItem->offhorseprob();
	mOffBuffID				= pItem->offbuffid();
	mInitAP					= pItem->initap();
	mDeltaAP				= pItem->deltaap();
	return ;
}

void CTplItemHorse::CreateTlvPB( bool bServer,PBItemHorse* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_metierlimit( mMetierRequire );
	pItem->set_levellimit( mLevelRequire );
	pItem->set_personlimit( mPersonRequire );
	pItem->set_usetime( mUseTime );
	pItem->set_increasespeedper( mIncreaseSpeedPer );
	pItem->set_modelname( mModelName );
	pItem->set_cooltime( mCooldown );
	pItem->set_sharecd( mShareCD );
	pItem->set_speedbuff( mSpeedBuff );
	pItem->set_usingtime( mUsingTime );
	pItem->set_showtext( mShowText );
	pItem->set_offhorseprob( mOffHorseProb );
	pItem->set_offbuffid( mOffBuffID );
	pItem->set_initap( mInitAP );
	pItem->set_deltaap( mDeltaAP );
	return ;
}

int CTemplateProperty::Initialize()
{
	memset( mPropertyName, 0, sizeof(mPropertyName));	
	mPropertyFunc = 0;				
	mCanOut = 0;					
	mIntMinValue = 0;					
	mIntMaxValue = 0;		
	mArea = 0 ;
	mFloatMinValue = 0;				
	mFloatMaxValue = 0;				
	mSkillID = 0;
	return 0;
}

void CTemplateProperty::SetFromPB( bool bServer,PBTemplateProperty *pProperty)
{
	if ( pProperty == NULL )
	{
		return ;
	}
	mTempID				= pProperty->tempid();
	strncpy( mPropertyName, pProperty->propertyname().c_str(), sizeof( mPropertyName ) - 1 );
	mPropertyFunc		= pProperty->propertyfunc();
	mCanOut				= pProperty->canout();
	mIntMinValue		= pProperty->intminvalue();
	mIntMaxValue		= pProperty->intmaxvalue();
	mFloatMinValue		= pProperty->floatminvalue();
	mFloatMaxValue		= pProperty->floatmaxvalue();
	mSkillID			= pProperty->skillid();
	mArea				= pProperty->area();
	return ;
}

void CTemplateProperty::CreateTlvPB( bool bServer,PBTemplateProperty *pProperty)
{
	if ( pProperty == NULL )
	{
		return ;
	}
	pProperty->set_tempid( mTempID );
	pProperty->set_propertyname( mPropertyName );
	pProperty->set_propertyfunc( mPropertyFunc );
	pProperty->set_canout( mCanOut );
	pProperty->set_intminvalue( mIntMinValue );
	pProperty->set_intmaxvalue( mIntMaxValue );
	pProperty->set_floatminvalue( mFloatMinValue );
	pProperty->set_floatmaxvalue( mFloatMaxValue );
	pProperty->set_skillid( mSkillID );
	pProperty->set_area( mArea );
	return ;
}

int CTemplatePropertyTable::Initialize()
{
	memset( mProperty, 0, sizeof(mProperty));
	return 0;
}

void CTemplatePropertyTable::SetFromPB( bool bServer,PBTemplatePropertyTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}
	mTempID				= pTable->tempid();
	for ( unsigned int i = 0; i < (unsigned int) pTable->property_size() && i < (unsigned int) ARRAY_CNT( mProperty ); ++ i )
	{
		mProperty[ i ].mTempID = (pTable->property( i )).tempid();
		mProperty[ i ].mPercent = (pTable->property( i )).percent();
	}
	return ;
}

void CTemplatePropertyTable::CreateTlvPB( bool bServer,PBTemplatePropertyTable* pTable )
{
	if ( pTable == NULL )
	{
		return ;
	}
	pTable->set_tempid( mTempID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mProperty ); i ++ )
	{
		PBPropertyPercent* tpPercent = pTable->add_property();
		tpPercent->set_tempid( mProperty[ i ].mTempID );
		tpPercent->set_percent( mProperty[ i ].mPercent  );
	}
	return ;
}

int CTemplateEquipment::Initialize()
{	
	memset( mEquipModal, 0, sizeof(mEquipModal));	
	mShowLevel = 0;			
	mEquipType = 0;					
	mShowColour = 0;		
	memset( mStrRequire, 0, sizeof(mStrRequire));			
	memset( mConRequire, 0, sizeof(mConRequire ));
	memset( mStaRequire, 0, sizeof(mStaRequire));				
	memset( mWisRequire, 0, sizeof(mWisRequire));
	memset( mSprRequire, 0, sizeof(mSprRequire));
	mMetierRequire = 0;
	mLevelRequire = 0;				
	mSexRequire = 0;	
	mYuan = 0;
	mEquipmentDurability = 0;
	mBaseMendCharge = 0;
	mNpcAbrasionRate = 0;
	mPlayerAbrasionRate = 0;
	mCanUpgrade = 0;
	mCanDefUpgrade = 0;
	mCanAttUpgrade = 0;
	mUpgCharge = 0;	
	mNeedBindMoney = 0;
	memset( mUpgProperty, 0, sizeof( mUpgProperty ) );	
	memset( mUpgValue, 0, sizeof(mUpgValue));	
	memset( mPAttackDiff, 0, sizeof(mPAttackDiff));
	memset( mMAttackDiff, 0, sizeof(mMAttackDiff));
	memset( mSlotNumber1, 0, sizeof(mSlotNumber1));	
	mSlotNumber2 = 0;	
	memset( mPropertyPercent, 0, sizeof(mPropertyPercent));		
	memset( mPropertyTableID, 0, sizeof(mPropertyTableID ));
	memset( mPropertyPercentEx, 0, sizeof(mPropertyPercentEx));		
	memset( mPropertyTableIDEx, 0, sizeof(mPropertyTableIDEx));
	memset( mJudgePercent1, 0, sizeof(mJudgePercent1));
	memset( mJudgePercent2, 0, sizeof(mJudgePercent2));
	memset( mResDefPercent, 0, sizeof(mResDefPercent));
	memset( mResAttPercent, 0, sizeof(mResAttPercent));
	memset( mResDefValue, 0, sizeof(mResDefValue));
	memset( mResAttValue, 0, sizeof(mResAttValue));
	mSuitID = 0;
	mSuitPos = 0;
	mOpenCharge = 0;
	mAddCharge = 0;
	mJudgeCharge = 0;
	mReJudgeCharge = 0;
	mDefUpgCharge = 0;
	mDefChangeCharge = 0;
	mAttUpgCharge = 0;
	mAttChangeCharge = 0;	
	mBindBonus = 0;
	mMultiCompose = 0;
	mCanChange = 0;
	mChangeCharge = 0;
	mCanSplit = 0;
	mSplitCharge = 0;
	memset( mSplitID, 0, sizeof(mSplitID));
	memset( mSplitNum, 0, sizeof(mSplitNum));
	memset( mUpgExtra1, 0, sizeof(mUpgExtra1));
	memset( mUpgExtra2, 0, sizeof(mUpgExtra2));
	mExtCharge = 0;
	mResistTran = 0;
	mIsShenQi = 0;
	return 0;
}

void CTemplateEquipment::SetFromPB( bool bServer,PBEquipment *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	strncpy( mEquipModal, pItem->modalname().c_str(), sizeof( mEquipModal ) - 1 );
	mShowLevel		= pItem->showlevel();
	mEquipType			= pItem->equiptype();
	mShowColour			= pItem->showcolor();
	/*mStrRequire			= pItem->strrequire();
	mConRequire			= pItem->conrequire();
	mStaRequire			= pItem->starequire();
	mWisRequire			= pItem->wisrequire();
	mSprRequire			= pItem->sprrequire();*/
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) pItem->strrequire_size() && i < (unsigned int) ARRAY_CNT( mStrRequire ); ++ i )
	{
		mStrRequire[ i ] = pItem->strrequire( i );
	}
	for ( i = 0; i < (unsigned int) pItem->conrequire_size() && i < (unsigned int) ARRAY_CNT( mConRequire ); ++ i )
	{
		mConRequire[ i ] = pItem->conrequire( i );
	}
	for ( i = 0; i < (unsigned int) pItem->starequire_size() && i < (unsigned int) ARRAY_CNT( mStaRequire ); ++ i )
	{
		mStaRequire[ i ] = pItem->starequire( i );
	}
	for ( i = 0; i < (unsigned int) pItem->wisrequire_size() && i < (unsigned int) ARRAY_CNT( mWisRequire ); ++ i )
	{
		mWisRequire[ i ] = pItem->wisrequire( i );
	}
	for ( i = 0; i < (unsigned int) pItem->sprrequire_size() && i < (unsigned int) ARRAY_CNT( mSprRequire ); ++ i )
	{
		mSprRequire[ i ] = pItem->sprrequire( i );
	}
	mMetierRequire		= pItem->metierrequire();
	mLevelRequire		= pItem->levelrequire();
	mSexRequire			= pItem->sexrequire();
	mYuan				= pItem->yuanvalue();
	mEquipmentDurability = pItem->equipdurability();
	mBaseMendCharge		= pItem->basemendcharge();
	mNpcAbrasionRate	= pItem->npcabrasionrate();
	mPlayerAbrasionRate	= pItem->playerabrasionrate();
	mCanUpgrade			= pItem->canupgrade();
	mUpgCharge			= pItem->upgcharge();

	mSlotNumber2		= pItem->maxslotnumber2();	
	mSuitID				= pItem->suitid();
	mSuitPos			= pItem->suitpos();
	mOpenCharge			= pItem->opencharge();
	mAddCharge			= pItem->addcharge();
	mJudgeCharge		= pItem->judgecharge();
	mReJudgeCharge		= pItem->rejudgecharge();
	mDefUpgCharge		= pItem->defupgcharge();
	mDefChangeCharge	= pItem->defchangecharge();
	mAttUpgCharge		= pItem->attupgcharge();
	mAttChangeCharge	= pItem->attchangecharge();
	mCanDefUpgrade		= pItem->candefupgrade();
	mCanAttUpgrade		= pItem->canattupgrade();
	mBindBonus			= pItem->bindbonus();
	mMultiCompose		= pItem->multicompose();
	mCanChange			= pItem->canchange();
	mChangeCharge		= pItem->changecharge();
		
	for ( i = 0; i < (unsigned int) pItem->upgpropertyid_size() && i < (unsigned int) ARRAY_CNT( mUpgProperty ); ++ i )
	{
		mUpgProperty[ i ] = pItem->upgpropertyid( i );
	}
	for ( i = 0; i < (unsigned int) ARRAY_ROW( mUpgValue ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mUpgValue ) ); j ++ )
		{
			int tIndex = ( i * ARRAY_COL( mUpgValue ) + j );
			if ( tIndex < pItem->upgvalue_size() )
			{
				mUpgValue[ i ][ j ] = pItem->upgvalue( tIndex );
			}			
		}
	}
	for ( i = 0; i < (unsigned int) pItem->pattackdiff_size() && i < (unsigned int) ARRAY_CNT( mPAttackDiff ); ++ i )
	{
		mPAttackDiff[ i ] = pItem->pattackdiff( i );
	}
	for ( i = 0; i < (unsigned int) pItem->mattackdiff_size() && i < (unsigned int) ARRAY_CNT( mMAttackDiff ); ++ i )
	{
		mMAttackDiff[ i ] = pItem->mattackdiff( i );
	}
	for ( i = 0; i < (unsigned int) pItem->resdefvalue_size() && i < (unsigned int) ARRAY_CNT( mResDefValue ); ++ i )
	{
		mResDefValue[ i ] = pItem->resdefvalue( i );
	}
	for ( i = 0; i < (unsigned int) pItem->resattvalue_size() && i < (unsigned int) ARRAY_CNT( mResAttValue ); ++ i )
	{
		mResAttValue[ i ] = pItem->resattvalue( i );
	}

	if ( bServer )
	{
		for ( i = 0; i < (unsigned int) pItem->maxslotnumber1_size() && i < (unsigned int) ARRAY_CNT( mSlotNumber1 ); i ++ )
		{
			mSlotNumber1[ i ] = pItem->maxslotnumber1( i );
		}
		for ( i = 0; i < (unsigned int) pItem->propertypercent_size() && i < (unsigned int) ARRAY_CNT( mPropertyPercent ); i ++ )
		{
			mPropertyPercent[ i ] = pItem->propertypercent( i );
		}
		for ( i = 0; i < (unsigned int) pItem->propertytableid_size() && i < (unsigned int) ARRAY_CNT( mPropertyTableID ); i ++ )
		{
			mPropertyTableID[ i ] = pItem->propertytableid( i );
		}
		for ( i = 0; i < (unsigned int) pItem->propertypercentex_size() && i < (unsigned int) ARRAY_CNT( mPropertyPercentEx ); i ++ )
		{
			mPropertyPercentEx[ i ] = pItem->propertypercentex( i );
		}
		for ( i = 0; i < (unsigned int) pItem->propertytableidex_size() && i < (unsigned int) ARRAY_CNT( mPropertyTableIDEx ); i ++ )
		{
			mPropertyTableIDEx[ i ] = pItem->propertytableidex( i );
		}
		for ( i = 0; i < (unsigned int) pItem->judgepercent1_size() && i < (unsigned int) ARRAY_CNT( mJudgePercent1 ); i ++ )
		{
			mJudgePercent1[ i ] = pItem->judgepercent1( i );
		}
		for ( i = 0; i < (unsigned int) pItem->judgepercent2_size() && i < (unsigned int) ARRAY_CNT( mJudgePercent2 ); i ++ )
		{
			mJudgePercent2[ i ] = pItem->judgepercent2( i );
		}
		for ( i = 0; i < (unsigned int) pItem->resdefpercent_size() && i < (unsigned int) ARRAY_CNT( mResDefPercent ); i ++ )
		{
			mResDefPercent[ i ] = pItem->resdefpercent( i );
		}
		for ( i = 0; i < (unsigned int) pItem->resattpercent_size() && i < (unsigned int) ARRAY_CNT( mResAttPercent ); i ++ )
		{
			mResAttPercent[ i ] = pItem->resattpercent( i );
		}
	}

	mCanSplit = pItem->cansplit();
	mSplitCharge = pItem->splitcharge();
	for ( i = 0; i < (unsigned int) pItem->splitid_size() && i < (unsigned int) ARRAY_CNT( mSplitID ); ++ i )
	{
		mSplitID[ i ] = pItem->splitid( i );
	}
	for ( i = 0; i < (unsigned int) pItem->splitnum_size() && i < (unsigned int) ARRAY_CNT( mSplitNum ); ++ i )
	{
		mSplitNum[ i ] = pItem->splitnum( i );
	}
	for ( i = 0; i < (unsigned int) pItem->upgextra1_size() && i < (unsigned int) ARRAY_CNT( mUpgExtra1 ); ++ i )
	{
		mUpgExtra1[ i ] = pItem->upgextra1( i );
	}
	for ( i = 0; i < (unsigned int) pItem->upgextra2_size() && i < (unsigned int) ARRAY_CNT( mUpgExtra2 ); ++ i )
	{
		mUpgExtra2[ i ] = pItem->upgextra2( i );
	}
	mExtCharge			= pItem->extcharge();
	mResistTran			= pItem->resisttran();
	mIsShenQi			= pItem->isshenqi();
	return ;
}

void CTemplateEquipment::CreateTlvPB( bool bServer,PBEquipment* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	pItem->set_modalname( mEquipModal );
	pItem->set_showcolor( mShowColour );
	pItem->set_showlevel( mShowLevel );
	pItem->set_equiptype( mEquipType );
	/*pItem->set_strrequire( mStrRequire );
	pItem->set_conrequire( mConRequire );
	pItem->set_starequire( mStaRequire );
	pItem->set_wisrequire( mWisRequire );
	pItem->set_sprrequire( mSprRequire );*/
	unsigned int i = 0;	
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mStrRequire ); ++ i )
	{
		pItem->add_strrequire( mStrRequire[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mConRequire ); ++ i )
	{
		pItem->add_conrequire( mConRequire[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mStaRequire ); ++ i )
	{
		pItem->add_starequire( mStaRequire[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mWisRequire ); ++ i )
	{
		pItem->add_wisrequire( mWisRequire[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSprRequire ); ++ i )
	{
		pItem->add_sprrequire( mSprRequire[ i ] );
	}
	pItem->set_metierrequire( mMetierRequire );
	pItem->set_levelrequire( mLevelRequire );
	pItem->set_sexrequire( mSexRequire );
	pItem->set_yuanvalue( mYuan );
	pItem->set_equipdurability( mEquipmentDurability );
	pItem->set_basemendcharge( mBaseMendCharge );
	pItem->set_npcabrasionrate( mNpcAbrasionRate );
	pItem->set_playerabrasionrate( mPlayerAbrasionRate );
	pItem->set_canupgrade( mCanUpgrade );
	pItem->set_upgcharge( mUpgCharge );
	pItem->set_opencharge( mOpenCharge );
	pItem->set_addcharge( mAddCharge );
	pItem->set_judgecharge( mJudgeCharge );
	pItem->set_rejudgecharge( mReJudgeCharge );
	pItem->set_defupgcharge( mDefUpgCharge );
	pItem->set_defchangecharge( mDefChangeCharge );
	pItem->set_attupgcharge( mAttUpgCharge );
	pItem->set_attchangecharge( mAttChangeCharge );
	pItem->set_candefupgrade( mCanDefUpgrade );
	pItem->set_canattupgrade( mCanAttUpgrade );	
	pItem->set_bindbonus( mBindBonus );
	pItem->set_multicompose( mMultiCompose );
	pItem->set_canchange( mCanChange );
	pItem->set_changecharge( mChangeCharge );
	
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mUpgProperty ); ++ i )
	{
		pItem->add_upgpropertyid( mUpgProperty[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_ROW( mUpgValue ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mUpgValue ) ); j ++ )
		{
			pItem->add_upgvalue( mUpgValue[ i ][ j ] );
		}
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mPAttackDiff ); ++ i )
	{
		pItem->add_pattackdiff( mPAttackDiff[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mMAttackDiff ); ++ i )
	{
		pItem->add_mattackdiff( mMAttackDiff[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mResDefValue ); ++ i )
	{
		pItem->add_resdefvalue( mResDefValue[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mResAttValue ); ++ i )
	{
		pItem->add_resattvalue( mResAttValue[ i ] );
	}
	pItem->set_maxslotnumber2( mSlotNumber2 );
	pItem->set_suitid( mSuitID );
	pItem->set_suitpos( mSuitPos );
	if ( bServer )
	{
		for ( i = 0; i < (unsigned int) ARRAY_CNT( mSlotNumber1 ); i ++ )
		{
			pItem->add_maxslotnumber1( (int)( mSlotNumber1[ i ] ) );
		}
		for ( i = 0; i < (unsigned int) ARRAY_CNT( mPropertyPercent ); i ++ )
		{
			pItem->add_propertypercent( (int)( mPropertyPercent[ i ] ) );
		}
		for ( i = 0; i < (unsigned int) ARRAY_CNT( mPropertyTableID ); i ++ )
		{
			pItem->add_propertytableid( mPropertyTableID[ i ] );
		}
		for ( i = 0; i < (unsigned int) ARRAY_CNT( mPropertyPercentEx ); i ++ )
		{
			pItem->add_propertypercentex( (int)( mPropertyPercentEx[ i ] ) );
		}
		for ( i = 0; i < (unsigned int) ARRAY_CNT( mPropertyTableIDEx ); i ++ )
		{
			pItem->add_propertytableidex( mPropertyTableIDEx[ i ] );
		}
		for ( i = 0; i < (unsigned int) ARRAY_CNT( mJudgePercent1 ); i ++ )
		{
			pItem->add_judgepercent1( mJudgePercent1[ i ] );
		}
		for ( i = 0; i < (unsigned int) ARRAY_CNT( mJudgePercent2 ); i ++ )
		{
			pItem->add_judgepercent2( mJudgePercent2[ i ] );
		}
		for ( i = 0; i < (unsigned int) ARRAY_CNT( mResDefPercent ); i ++ )
		{
			pItem->add_resdefpercent( mResDefPercent[ i ] );
		}
		for ( i = 0; i < (unsigned int) ARRAY_CNT( mResAttPercent ); i ++ )
		{
			pItem->add_resattpercent( mResAttPercent[ i ] );
		}
	}
	pItem->set_cansplit( mCanSplit );
	pItem->set_splitcharge( mSplitCharge );
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSplitID ); ++ i )
	{
		pItem->add_splitid( mSplitID[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSplitNum ); ++ i )
	{
		pItem->add_splitnum( mSplitNum[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mUpgExtra1 ); ++ i )
	{
		pItem->add_upgextra1( mUpgExtra1[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mUpgExtra2 ); ++ i )
	{
		pItem->add_upgextra2( mUpgExtra2[ i ] );
	}
	pItem->set_extcharge( mExtCharge );
	pItem->set_resisttran( mResistTran );
	pItem->set_isshenqi( mIsShenQi );
	return ;
}

int CTemplateFashion::Initialize()
{
	mBuffID = 0;
	return 0;
}

void CTemplateFashion::SetFromPB( bool bServer,PBEquipFashion *pEquip)
{
	if ( pEquip == NULL )
	{
		return ; 
	}
	//PBEquipment* pItem = pEquip->mutable_equipinfo();
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	//CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );	
	
	CTemplateEquipment::SetFromPB( bServer,pEquip->mutable_equipinfo() );

	mBuffID			= pEquip->buffid();	
	return ;
}

void CTemplateFashion::CreateTlvPB( bool bServer,PBEquipFashion *pEquip)
{
	if ( pEquip == NULL )
	{
		return ;
	}
	//PBEquipment* pItem = pEquip->mutable_equipinfo(); 
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	//CTplItem::CreateTlvPB( tpBase );	
	CTemplateEquipment::CreateTlvPB( bServer,pEquip->mutable_equipinfo() );

	pEquip->set_buffid( mBuffID );
	return ;
}

int CTemplateWeapon::Initialize()
{
	mWeaponType = WEAPON_UNLIMIT;
	mAttackType = 0;
	mAttackDistance = 0;
	return 0;
}

void CTemplateWeapon::SetFromPB( bool bServer,PBEquipWeapon *pEquip)
{
	if ( pEquip == NULL )
	{
		return ;
	}
	//PBEquipment* pItem = pEquip->mutable_equipinfo();
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	//CTplItem::SetFromPB( tpBase ); 
	CTemplateEquipment::SetFromPB( bServer,pEquip->mutable_equipinfo() );

	mWeaponType				= (WEAPON_TYPE)pEquip->weapontype();
	mAttackType				= pEquip->attacktype();	
	mAttackDistance			= pEquip->attackdistance();	
	return ;
}

void CTemplateWeapon::CreateTlvPB( bool bServer,PBEquipWeapon *pEquip)
{
	if ( pEquip == NULL )
	{
		return ;
	}
	PBEquipment* pItem = pEquip->mutable_equipinfo();

	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	//CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() ); 
	CTemplateEquipment::CreateTlvPB( bServer,pItem );

	pEquip->set_weapontype( mWeaponType );
	pEquip->set_attacktype( mAttackType );
	pEquip->set_attackdistance( mAttackDistance );

	return ;
}

int CTemplateArmor::Initialize()
{
	mShowType = 0;
	memset( mArmorSuit, 0, sizeof(mArmorSuit));
	return 0;
}

void CTemplateArmor::SetFromPB( bool bServer,PBEquipArmor *pEquip)
{
	if ( pEquip == NULL )
	{
		return ;
	}
	PBEquipment* pItem = pEquip->mutable_equipinfo();

	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	//CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );

	CTemplateEquipment::SetFromPB( bServer,pItem );
	mShowType			= pEquip->showtype();
	strncpy( mArmorSuit, pEquip->armorsuit().c_str(), sizeof( mArmorSuit ) - 1 );
	
	return ;
}

void CTemplateArmor::CreateTlvPB( bool bServer,PBEquipArmor *pEquip)
{
	if ( pEquip == NULL )
	{
		return ;
	}
	PBEquipment* pItem = pEquip->mutable_equipinfo();

	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	//CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );

	CTemplateEquipment::CreateTlvPB( bServer,pItem );
	pEquip->set_showtype( mShowType );
	pEquip->set_armorsuit( mArmorSuit );
	
	return ;
}


int CTemplateMagicWeapon::Initialize()
{	
	memset( mMWShowLevel, 0, sizeof(mMWShowLevel));
	memset( mShowType, 0, sizeof(mShowType));
	memset( mNewWeaponName, 0, sizeof(mNewWeaponName));
	mInitLevel = 0;
	mInitMp = 0;
	mInitHp = 0;
	mInitMaxMp = 0;
	mInitMaxHp = 0;
	mInitBase = 0;
	mInitShape = 0;
	mInitEffect = 0;
	mInitSpir = 0;
	mIncMaxMp = 0;
	mIncMaxHp = 0;
	mMPIncValue = 0;
	mHPIncValue = 0;
	mMPIncValuePerMagic = 0;
	mHPIncValuePerMagic = 0;
	mAttackInc = 0;
	mRecovInc = 0;
	mMaxMPInc = 0;
	mMaxHPInc = 0;
	mDeadLineInc = 0;
	mCanUseBook = 0;
	mGetSkillScl = 0 ;
	mLevelTableID = 0;
	mSkillBoxTableID = 0;
	mDieOffHp = 0;
	mProJudgeCost = 0;
	mUmmiBuffTableID = 0; 
	memset( mDispResName, 0, sizeof(mDispResName));
	mDefaultSkill = 0;
	mDefaultSkill2 = 0;
	mJudgeEffTableID = 0;
	mJudgeSpirTableID = 0;
	mLevelQuotietyTabID = 0;	
	mDecomposeTabID = 0;
	mLvByNum = 0;
	mQuickUpTabID = 0;
	mUpLevelTabID = 0;
	return 0;
}


void CTemplateMagicWeapon::SetFromPB( bool bServer,PBItemMagicWeapon* pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}

	PBEquipment* pBase = pItem->mutable_base();
	CTemplateEquipment::SetFromPB( bServer, pBase );

	strncpy( mMWShowLevel, pItem->showlevel().c_str(), STRING32 );
	strncpy( mShowType, pItem->showtype().c_str(), STRING32 );
	strncpy( mNewWeaponName, pItem->newweaponname().c_str(), STRING32 );

	mUmmiBuffTableID = pItem->ummibuffid();

	mInitLevel = pItem->initlevel();
	mInitMp = pItem->initmp();
	mInitHp = pItem->inithp();
	mInitMaxMp = pItem->initmaxmp();
	mInitMaxHp = pItem->initmaxhp();
	mInitBase = pItem->initbase();
	mInitShape = pItem->initshape();
	mInitEffect = pItem->initeffect();
	mInitSpir = pItem->initspir();
	mIncMaxMp = pItem->incmaxmp();
	mIncMaxHp = pItem->incmaxhp();
	mMPIncValue = pItem->mpincvalue();
	mHPIncValue = pItem->hpincvalue();
	mMPIncValuePerMagic = pItem->mpincvaluepermagic();
	mHPIncValuePerMagic = pItem->hpincvaluepermagic();
	mAttackInc = pItem->attackinc();
	mRecovInc = pItem->recovinc();
	mMaxMPInc = pItem->maxmpinc();
	mMaxHPInc = pItem->maxhpinc();
	mDeadLineInc = pItem->deadlineinc();
	mCanUseBook = pItem->canusebook();
	mGetSkillScl = pItem->getskillscl();
	mLevelTableID = pItem->leveltableid();
	mSkillBoxTableID = pItem->skillboxtableid();
	mDieOffHp = pItem->dieoffhp();
	mProJudgeCost = pItem->projudgecost();
	strncpy( mDispResName, pItem->dispresname().c_str(), STRING32 );
	mDefaultSkill = pItem->defaultskill();
	mDefaultSkill2 = pItem->defaultskill2();
	mJudgeEffTableID = pItem->judgeefftableid();
	mJudgeSpirTableID = pItem->judgespirtableid();
	mLevelQuotietyTabID = pItem->levelquotietytabid();
	mDecomposeTabID = pItem->decomposetabid();
	mLvByNum = pItem->lvbynum();
	mQuickUpTabID = pItem->quickuptabid();
	mUpLevelTabID = pItem->upleveltabid();
	return ;
}

void CTemplateMagicWeapon::CreateTlvPB( bool bServer,PBItemMagicWeapon* pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	PBEquipment* pBase = pItem->mutable_base();
	CTemplateEquipment::CreateTlvPB( bServer,pBase );

	pItem->set_showlevel( mMWShowLevel ); 
	pItem->set_showtype( mShowType ); 
	pItem->set_newweaponname( mNewWeaponName  ); 
	pItem->set_ummibuffid( mUmmiBuffTableID ); 

	pItem->set_initlevel( mInitLevel ); 
	pItem->set_initmp( mInitMp ); 
	pItem->set_inithp( mInitHp ); 
	pItem->set_initmaxmp( mInitMaxMp ); 
	pItem->set_initmaxhp( mInitMaxHp ); 
	pItem->set_initbase( mInitBase ); 
	pItem->set_initshape( mInitShape ); 
	pItem->set_initeffect( mInitEffect ); 
	pItem->set_initspir( mInitSpir ); 
	pItem->set_incmaxmp( mIncMaxMp ); 
	pItem->set_incmaxhp( mIncMaxHp ); 
	pItem->set_mpincvalue( mMPIncValue ); 
	pItem->set_hpincvalue( mHPIncValue ); 
	pItem->set_mpincvaluepermagic( mMPIncValuePerMagic ); 
	pItem->set_hpincvaluepermagic( mHPIncValuePerMagic ); 
	pItem->set_attackinc( mAttackInc ); 
	pItem->set_recovinc( mRecovInc ); 
	pItem->set_maxmpinc( mMaxMPInc ); 
	pItem->set_maxhpinc( mMaxHPInc ); 
	pItem->set_deadlineinc( mDeadLineInc ); 
	pItem->set_canusebook( mCanUseBook ); 
	pItem->set_getskillscl( mGetSkillScl ); 
	pItem->set_leveltableid( mLevelTableID ); 
	pItem->set_skillboxtableid( mSkillBoxTableID ); 
	pItem->set_dieoffhp( mDieOffHp ); 
	pItem->set_projudgecost( mProJudgeCost );
	pItem->set_dispresname( mDispResName );
	pItem->set_defaultskill( mDefaultSkill );
	pItem->set_defaultskill2( mDefaultSkill2 );
	pItem->set_judgeefftableid( mJudgeEffTableID);
	pItem->set_judgespirtableid(mJudgeSpirTableID);
	pItem->set_levelquotietytabid( mLevelQuotietyTabID );
	pItem->set_decomposetabid(mDecomposeTabID);
	pItem->set_lvbynum(mLvByNum);
	pItem->set_quickuptabid(mQuickUpTabID);
	pItem->set_upleveltabid(mUpLevelTabID);
	return ;
}


int CTemplateMetier::Initialize()
{	
	memset( mMetierName, 0, sizeof(mMetierName));		
	mMetierID = 0;						
	memset( mGenerID, 0, sizeof(mGenerID ));
	mSkillPoint = 0;		
	mWalkSpeed = 0;		
	mPropertyPoint = 0;	
	mRunSpeed = 0;		
	mInitLevel = 0;		
	mInitHP = 0;			
	mInitMP = 0;			
	mInitPDefense = 0;		
	mInitPAttack = 0;
	mInitMAttack = 0;		
	mInitMDefense = 0;		
	mInitAbnResist = 0;			
	mInitWindResist = 0;		
	mInitFireResist = 0;		
	mInitWaterResist = 0;		
	mInitLightResist = 0;		
	mInitEarthResist = 0;		
	mInitShadowResist = 0;		
	mInitDeathRate = 0;		
	mInitDeathValue = 0;		
	mInitNormalHitrate = 0;	
	mInitNormalDuck = 0;	
	mInitSkillDuck = 0;		
	mInitIgnoreDuck = 0;
	mInitDeathDefense = 0;
	mDeltaHP = 0;				
	mDeltaMP = 0;					
	mDeltaPAttack = 0;				
	mDeltaPDefense = 0;				
	mDeltaMAttack = 0;			
	mDeltaMDefense = 0;				
	mDeltaAbnResist = 0;		
	mDeltaWindResist = 0;				
	mDeltaFireResist = 0;				
	mDeltaWaterResist = 0; 		
	mDeltaLightResist = 0;				
	mDeltaEarthResist = 0;				
	mDeltaShadowResist = 0;			
	mDeltaDeathRate = 0;			
	mDeltaDeathValue = 0;		
	mDeltaNormalHitrate = 0;		
	mDeltaNormalDuck = 0;			
	mDeltaSkillDuck = 0;	
	mDeltaIgnoreDuck = 0;
	mDeltaDeathDefense = 0;
	mPAttackPerStr = 0;
	mMAttackPerStr = 0;	
	mDeathRatePerStr = 0;
	mDeathValuePerStr = 0;	
	mMaxHPPerStr = 0;
	mMaxMPPerStr = 0;
	mPDefensePerStr = 0;
	mMDefensePerStr = 0;
	mNormalHitratePerStr = 0;		
	mNormalDuckPerStr = 0;	
	mIgnoreDuckPerStr = 0;
	mDeathDefensePerStr = 0;
	mPAttackPerCon = 0;	
	mMAttackPerCon = 0;	
	mDeathRatePerCon = 0;
	mDeathValuePerCon = 0;	
	mMaxHPPerCon = 0;
	mMaxMPPerCon = 0;
	mPDefensePerCon = 0;
	mMDefensePerCon = 0;
	mNormalHitratePerCon = 0;		
	mNormalDuckPerCon = 0;	
	mIgnoreDuckPerCon = 0;
	mDeathDefensePerCon = 0;
	mHPRSpeedPerCon = 0;			
	mMaxHPPerSta = 0;
	mPReducePerSta = 0;
	mMReducePerSta = 0;
	mAbnResistPerSta = 0;
	mDeathDefensePerSta = 0;
	mPAttackPerWis = 0;
	mMAttackPerWis = 0;	
	mMaxHPPerWis = 0;
	mMaxMPPerWis = 0;
	mDeathRatePerWis = 0;
	mDeathValuePerWis = 0;	
	mPDefensePerWis = 0;
	mMDefensePerWis = 0;
	mNormalHitratePerWis = 0;		
	mNormalDuckPerWis =0 ;	
	mIgnoreDuckPerWis = 0;
	mDeathDefensePerWis = 0;
	mPAttackPerSpr = 0;
	mMAttackPerSpr = 0;
	mMaxHPPerSpr = 0;
	mMaxMPPerSpr= 0;
	mDeathRatePerSpr = 0;
	mDeathValuePerSpr = 0;	
	mPDefensePerSpr = 0;
	mMDefensePerSpr = 0;
	mNormalHitratePerSpr = 0;	
	mNormalDuckPerSpr = 0;	
	mIgnoreDuckPerSpr = 0;
	mDeathDefensePerSpr = 0;
	mMPRSpeedPerSpr = 0;			
	mNormalHPRSpeed = 0;	
	mNormalSitHPRSpeed = 0;			
	mBattleHPRSpeed = 0; 		
	mBattleSitHPRSpeed = 0;			
	mNormalMPRSpeed = 0;	
	mNormalSitMPRSpeed = 0;			
	mBattleMPRSpeed = 0;		
	mBattleSitMPRSpeed = 0;
	return 0;
}

void CTemplateMetier::SetFromPB( bool bServer,PBMetier *pMetier)
{
	if ( pMetier == NULL )
	{
		return ;
	}
	mTempID				= pMetier->tempid();
	strncpy( mMetierName, pMetier->metiername().c_str(), sizeof( mMetierName ) - 1 );
	mMetierID			= pMetier->metierid();
	for ( unsigned int i = 0; i < (unsigned int) pMetier->generid_size() && i < (unsigned int) ARRAY_CNT( mGenerID ); i ++ )
	{
		mGenerID[ i ] = pMetier->generid( i );
	}
	mSkillPoint				= pMetier->skillpoint(); 	
	mPropertyPoint			= pMetier->propertypoint();
	mWalkSpeed				= pMetier->walkspeed();
	mRunSpeed				= pMetier->runspeed();
	mInitLevel				= pMetier->initlevel();
	mInitHP					= pMetier->inithp();
	mInitMP					= pMetier->initmp();
	mInitPAttack			= pMetier->initpattack();
	mInitPDefense			= pMetier->initpdefense();
	mInitMAttack			= pMetier->initmattack();
	mInitMDefense			= pMetier->initmdefense();
	mInitAbnResist			= pMetier->initabnresist();
	mInitWindResist			= pMetier->initwindresist();
	mInitFireResist			= pMetier->initfireresist();
	mInitWaterResist		= pMetier->initwaterresist();
	mInitLightResist		= pMetier->initlightresist();
	mInitEarthResist		= pMetier->initearthresist();
	mInitShadowResist		= pMetier->initsadowresist();
	mInitDeathRate			= pMetier->initdeathrate();

	mInitDeathValue			= pMetier->initdeathvalue();
	mInitNormalHitrate		= pMetier->initnmlhitrate();
	mInitNormalDuck			= pMetier->initnormalduck();
	mInitSkillDuck			= pMetier->initskillduck();
	mDeltaHP				= pMetier->deltahp();
	mDeltaMP				= pMetier->deltamp();
	mDeltaPAttack			= pMetier->deltapattack();
	mDeltaPDefense			= pMetier->deltapdefense();		
	mDeltaMAttack			= pMetier->deltamattack();
	mDeltaMDefense			= pMetier->deltamdefense();
	mDeltaAbnResist			= pMetier->deltaabnresist();
	mDeltaWindResist		= pMetier->deltawindresist();
	mDeltaFireResist		= pMetier->deltafireresist();
	mDeltaWaterResist		= pMetier->deltawaterresist();
	mDeltaLightResist		= pMetier->deltalightresist();
	mDeltaEarthResist		= pMetier->deltaearthresist();
	mDeltaShadowResist		= pMetier->deltashadowresist();
	mDeltaDeathRate			= pMetier->deltadeathrate();
	mDeltaDeathValue		= pMetier->deltadeathvalue();
	mDeltaNormalHitrate		= pMetier->deltanormalhitrate();
	mDeltaNormalDuck		= pMetier->deltanormalduck();
	mDeltaSkillDuck			= pMetier->deltaskillduck();
	mMAttackPerWis			= pMetier->mattackperwis();
	mAbnResistPerSta		= pMetier->abnresistpersta();
	mPAttackPerStr			= pMetier->pattackperstr();
	mDeathValuePerStr		= pMetier->deathvalueperstr();
	mMaxHPPerStr			= pMetier->maxhpperstr();
	mPDefensePerCon			= pMetier->pdefensepercon();
	mHPRSpeedPerCon			= pMetier->hprspeedpercon();
	mMaxHPPerSta			= pMetier->maxhppersta();
	mMaxMPPerWis			= pMetier->maxmpperwis();
	mDeathValuePerWis		= pMetier->deathvalueperwis();
	mMDefensePerSpr			= pMetier->mdefenseperspr();
	mMaxMPPerSpr			= pMetier->maxmpperspr();
	mMPRSpeedPerSpr			= pMetier->mprspeedperspr();
	mNormalHPRSpeed			= pMetier->normalhprspeed();
	mNormalSitHPRSpeed		= pMetier->normalsithprspeed();
	mBattleHPRSpeed			= pMetier->battlehprspeed();
	mBattleSitHPRSpeed		= pMetier->battlesithprspeed();
	mNormalMPRSpeed			= pMetier->normalmprspeed();
	mNormalSitMPRSpeed		= pMetier->normalsitmprspeed();
	mBattleMPRSpeed			= pMetier->battlemprspeed();
	mBattleSitMPRSpeed		= pMetier->battlesitmprspeed();

	mInitIgnoreDuck			= pMetier->initignoreduck();
	mDeltaIgnoreDuck		= pMetier->deltaignoreduck();
	mMAttackPerStr			= pMetier->mattackperstr();
	mDeathRatePerStr		= pMetier->deathrateperstr();
	mMaxMPPerStr			= pMetier->maxmpperstr();
	mPDefensePerStr			= pMetier->pdefenseperstr();
	mMDefensePerStr			= pMetier->mdefenseperstr();
	mNormalHitratePerStr	= pMetier->nmlhitrateperstr();
	mNormalDuckPerStr		= pMetier->normalduckperstr();
	mIgnoreDuckPerStr		= pMetier->ignoreduckperstr();
	mPAttackPerCon			= pMetier->pattackpercon();
	mMAttackPerCon			= pMetier->mattackpercon();
	mDeathRatePerCon		= pMetier->deathratepercon();
	mDeathValuePerCon		= pMetier->deathvaluepercon();
	mMaxHPPerCon			= pMetier->maxhppercon();
	mMaxMPPerCon			= pMetier->maxmppercon();
	mMDefensePerCon			= pMetier->mdefensepercon();
	mNormalHitratePerCon	= pMetier->nmlhitratepercon();
	mNormalDuckPerCon		= pMetier->normalduckpercon();
	mIgnoreDuckPerCon		= pMetier->ignoreduckpercon();
	mPReducePerSta			= pMetier->preducepersta();
	mMReducePerSta			= pMetier->mreducepersta();
	mPAttackPerWis			= pMetier->pattackperwis();
	mMaxHPPerWis			= pMetier->maxhpperwis();
	mDeathRatePerWis		= pMetier->deathrateperwis();
	mPDefensePerWis			= pMetier->pdefenseperwis();
	mMDefensePerWis			= pMetier->mdefenseperwis();
	mNormalHitratePerWis	= pMetier->nmlhitrateperwis();
	mNormalDuckPerWis		= pMetier->normalduckperwis();
	mIgnoreDuckPerWis		= pMetier->ignoreduckperwis();
	mPAttackPerSpr			= pMetier->pattackperspr();
	mMAttackPerSpr			= pMetier->mattackperspr();
	mDeathRatePerSpr		= pMetier->deathrateperspr();
	mDeathValuePerSpr		= pMetier->deathvalueperspr();
	mMaxHPPerSpr			= pMetier->maxhpperspr();
	mPDefensePerCon			= pMetier->pdefenseperspr();
	mNormalHitratePerSpr	= pMetier->nmlhitrateperspr();
	mNormalDuckPerSpr		= pMetier->normalduckperspr();
	mIgnoreDuckPerSpr		= pMetier->ignoreduckperspr();
	mInitDeathDefense		= pMetier->initdeathdefense();
	mDeltaDeathDefense		= pMetier->deltadeathdefense();
	mDeathDefensePerStr		= pMetier->deathdefenseperstr();
	mDeathDefensePerCon		= pMetier->deathdefensepercon();
	mDeathDefensePerSta		= pMetier->deathdefensepersta();
	mDeathDefensePerWis		= pMetier->deathdefenseperwis();
	mDeathDefensePerSpr		= pMetier->deathdefenseperspr();
	return ;
}

void CTemplateMetier::CreateTlvPB( bool bServer,PBMetier* pMetier )
{
	if ( pMetier == NULL )
	{
		return ;
	}
	pMetier->set_tempid( mTempID );
	pMetier->set_metiername( mMetierName );
	pMetier->set_metierid( mMetierID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mGenerID ); i ++ )
	{
		 pMetier->add_generid( mGenerID[ i ] );
	}
	pMetier->set_skillpoint( mSkillPoint );
	pMetier->set_propertypoint( mPropertyPoint );
	pMetier->set_walkspeed( mWalkSpeed );
	pMetier->set_runspeed( mRunSpeed );
	pMetier->set_initlevel( mInitLevel );
	pMetier->set_inithp( mInitHP );
	pMetier->set_initmp( mInitMP );
	pMetier->set_initpattack( mInitPAttack );
	pMetier->set_initpdefense( mInitPDefense );
	pMetier->set_initmattack( mInitMAttack );
	pMetier->set_initmdefense( mInitMDefense );
	pMetier->set_initabnresist( mInitAbnResist );
	pMetier->set_initwindresist( mInitWindResist );
	pMetier->set_initfireresist( mInitFireResist );
	pMetier->set_initwaterresist( mInitWaterResist );
	pMetier->set_initlightresist( mInitLightResist );
	pMetier->set_initearthresist( mInitEarthResist );
	pMetier->set_initsadowresist( mInitShadowResist );
	pMetier->set_initdeathrate( mInitDeathRate );
	pMetier->set_abnresistpersta( mAbnResistPerSta );
	pMetier->set_initdeathvalue(mInitDeathValue);
	pMetier->set_initnmlhitrate(mInitNormalHitrate);
	pMetier->set_initnormalduck(mInitNormalDuck);
	pMetier->set_initskillduck(mInitSkillDuck);
	pMetier->set_deltahp(mDeltaHP);
	pMetier->set_deltamp(mDeltaMP);
	pMetier->set_deltapattack(mDeltaPAttack);
	pMetier->set_deltapdefense(mDeltaPDefense);
	pMetier->set_deltamattack(mDeltaMAttack);
	pMetier->set_deltamdefense(mDeltaMDefense);
	pMetier->set_deltaabnresist(mDeltaAbnResist);
	pMetier->set_deltawindresist(mDeltaWindResist);
	pMetier->set_deltafireresist(mDeltaFireResist);
	pMetier->set_deltawaterresist(mDeltaWaterResist);
	pMetier->set_deltalightresist(mDeltaLightResist);
	pMetier->set_deltaearthresist(mDeltaEarthResist);
	pMetier->set_deltashadowresist(mDeltaShadowResist);
	pMetier->set_deltadeathrate(mDeltaDeathRate);
	pMetier->set_deltadeathvalue(mDeltaDeathValue);
	pMetier->set_deltanormalhitrate(mDeltaNormalHitrate);
	pMetier->set_deltanormalduck(mDeltaNormalDuck);
	pMetier->set_deltaskillduck(mDeltaSkillDuck);
	pMetier->set_mattackperwis( mMAttackPerWis );
	pMetier->set_pattackperstr((int)(mPAttackPerStr));
	pMetier->set_deathvalueperstr((int)(mDeathValuePerStr));
	pMetier->set_maxhpperstr((int)(mMaxHPPerStr));
	pMetier->set_pdefensepercon((int)(mPDefensePerCon));
	pMetier->set_hprspeedpercon((int)(mHPRSpeedPerCon));
	pMetier->set_maxhppersta((int)(mMaxHPPerSta));
	pMetier->set_maxmpperwis((int)(mMaxMPPerWis));
	pMetier->set_deathvalueperwis((int)(mDeathValuePerWis));
	pMetier->set_mdefenseperspr((int)(mMDefensePerSpr));
	pMetier->set_maxmpperspr((int)(mMaxMPPerSpr));
	pMetier->set_mprspeedperspr((int)(mMPRSpeedPerSpr));
	pMetier->set_normalhprspeed(mNormalHPRSpeed);
	pMetier->set_normalsithprspeed(mNormalSitHPRSpeed);
	pMetier->set_battlehprspeed(mBattleHPRSpeed);
	pMetier->set_battlesithprspeed(mBattleSitHPRSpeed);
	pMetier->set_normalmprspeed(mNormalMPRSpeed);
	pMetier->set_normalsitmprspeed(mNormalSitMPRSpeed);
	pMetier->set_battlemprspeed(mBattleMPRSpeed);
	pMetier->set_battlesitmprspeed(mBattleSitMPRSpeed);

	pMetier->set_initignoreduck(mInitIgnoreDuck);
	pMetier->set_deltaignoreduck(mDeltaIgnoreDuck);
	pMetier->set_mattackperstr(mMAttackPerStr);
	pMetier->set_deathrateperstr(mDeathRatePerStr);
	pMetier->set_maxmpperstr(mMaxMPPerStr);
	pMetier->set_pdefenseperstr(mPDefensePerStr);
	pMetier->set_mdefenseperstr(mMDefensePerStr);
	pMetier->set_nmlhitrateperstr(mNormalHitratePerStr);
	pMetier->set_normalduckperstr(mNormalDuckPerStr);
	pMetier->set_ignoreduckperstr(mIgnoreDuckPerStr);
	pMetier->set_pattackpercon(mPAttackPerCon);
	pMetier->set_mattackpercon(mMAttackPerCon);
	pMetier->set_deathratepercon(mDeathRatePerCon);
	pMetier->set_deathvaluepercon(mDeathValuePerCon);
	pMetier->set_maxhppercon(mMaxHPPerCon);
	pMetier->set_maxmppercon(mMaxMPPerCon);
	pMetier->set_mdefensepercon(mMDefensePerCon);
	pMetier->set_nmlhitratepercon(mNormalHitratePerCon);
	pMetier->set_normalduckpercon(mNormalDuckPerCon);
	pMetier->set_ignoreduckpercon(mIgnoreDuckPerCon);
	pMetier->set_preducepersta(mPReducePerSta);
	pMetier->set_mreducepersta(mMReducePerSta);
	pMetier->set_pattackperwis(mPAttackPerWis);
	pMetier->set_maxhpperwis(mMaxHPPerWis);
	pMetier->set_deathrateperwis(mDeathRatePerWis);
	pMetier->set_pdefenseperwis(mPDefensePerWis);
	pMetier->set_mdefenseperwis(mMDefensePerWis);
	pMetier->set_nmlhitrateperwis(mNormalHitratePerWis);
	pMetier->set_normalduckperwis(mNormalDuckPerWis);
	pMetier->set_ignoreduckperwis(mIgnoreDuckPerWis);
	pMetier->set_pattackperspr(mPAttackPerSpr);
	pMetier->set_mattackperspr(mMAttackPerSpr);
	pMetier->set_deathrateperspr(mDeathRatePerSpr);
	pMetier->set_deathvalueperspr(mDeathValuePerSpr);
	pMetier->set_maxhpperspr(mMaxHPPerSpr);
	pMetier->set_pdefenseperspr(mPDefensePerCon);
	pMetier->set_nmlhitrateperspr(mNormalHitratePerSpr);
	pMetier->set_normalduckperspr(mNormalDuckPerSpr);
	pMetier->set_ignoreduckperspr(mIgnoreDuckPerSpr);
	pMetier->set_initdeathdefense(mInitDeathDefense);
	pMetier->set_deltadeathdefense(mDeltaDeathDefense);
	pMetier->set_deathdefenseperstr(mDeathDefensePerStr);
	pMetier->set_deathdefensepercon(mDeathDefensePerCon);
	pMetier->set_deathdefensepersta(mDeathDefensePerSta);
	pMetier->set_deathdefenseperwis(mDeathDefensePerWis);
	pMetier->set_deathdefenseperspr(mDeathDefensePerSpr);
	return ;
}

int CTemplateSellTable::Initialize()
{
	memset(mItem, 0, sizeof(mItem ));	
	mSellTableType = 0;	
	memset( mItemGoldPrice, 0, sizeof(mItemGoldPrice));		 	
	memset( mItemNum, 0, sizeof(mItemNum));	
	mWarCityMapID = 0; 
	memset( mItemRefreshDay, 0, sizeof(mItemRefreshDay));
	return 0;
}

void CTemplateSellTable::SetFromPB( bool bServer,PBSellTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}
	mTempID			= pTable->tempid();
	for ( unsigned int i = 0; i < (unsigned int) pTable->item_size() && i < (unsigned int) ARRAY_CNT( mItem ); i ++ )
	{
		//PBSellItem* pItem = pTable->mutable_item( i );
		strncpy( mItem[ i ].mPage,(pTable->item(i)).pagename().c_str(), sizeof( mItem[ i ].mPage ) - 1 );
		for ( unsigned int j = 0; j < (unsigned int) (pTable->item(i)).itemid_size() && j < (unsigned int) ARRAY_CNT( mItem[ i ].mItemID ); j ++ )
		{			
			mItem[ i ].mItemID[ j ] = (pTable->item(i)).itemid( j );								
			
			if ( j < (unsigned int)(pTable->item(i) ).itemgoldprice_size() && j < ARRAY_COL( mItemGoldPrice ) )
			{
				mItemGoldPrice[i][j] = (pTable->item(i)).itemgoldprice( j );
			}
			
			if ( j < (unsigned int)( pTable->item(i) ).itemnum_size() && j < ARRAY_COL( mItemNum ) )
			{
				mItemNum[i][j] = (pTable->item(i)).itemnum( j );
			}
			
			if ( j < (unsigned int)( pTable->item(i) ).itemrefreshday_size() &&  j < ARRAY_COL( mItemRefreshDay ) )
			{
				mItemRefreshDay[i][j] = (pTable->item(i)).itemrefreshday( j );
			}			
		}
	}
	
	mSellTableType = pTable->selltabletype();
	mWarCityMapID =  pTable->warcitymapid();
	return ;
}

void CTemplateSellTable::CreateTlvPB( bool bServer,PBSellTable* pTable )
{
	if ( pTable == NULL )
	{
		return ;
	}
	pTable->set_tempid( mTempID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mItem ); i ++ )
	{
		PBSellItem* tpItem = pTable->add_item();
		tpItem->set_pagename( mItem[ i ].mPage );
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_CNT( mItem[ i ].mItemID ); j ++ )
		{				
			tpItem->add_itemid( mItem[ i ].mItemID[ j ] );						  
			tpItem->add_itemgoldprice( mItemGoldPrice[i][j] );
			tpItem->add_itemnum( mItemNum[i][j] );
			tpItem->add_itemrefreshday( mItemRefreshDay[i][j] );
		}
	}
	
	pTable->set_selltabletype( mSellTableType );
	pTable->set_warcitymapid( mWarCityMapID );
	return ;
}

void CTemplateTotalDrop::SetFromPB( bool bServer,PBTotalDrop *pDrop)
{
	if ( pDrop == NULL )
	{
		return; 
	}
	mTempID			= pDrop->tempid();
	for ( unsigned int i = 0; i < (unsigned int) pDrop->droptableid_size() && i < (unsigned int) ARRAY_CNT( mDropTableID ); i ++ )
	{
		mDropTableID[ i ] = pDrop->droptableid( i );
	}
	for ( unsigned int j = 0; j < (unsigned int) pDrop->dropcount_size() && j < (unsigned int) ARRAY_CNT( mDropCount ); j ++ )
	{
		mDropCount[ j ] = pDrop->dropcount( j );
	}
	return ;
}

int CTemplateTotalDrop::Initialize()
{
	memset( mDropTableID, 0, sizeof(mDropTableID));
	memset( mDropCount, 0, sizeof(mDropCount));
	return 0;
}

void CTemplateTotalDrop::CreateTlvPB( bool bServer,PBTotalDrop* pDrop )
{
	if ( pDrop == NULL )
	{
		return ;
	}
	pDrop->set_tempid( mTempID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mDropTableID ); i ++ )
	{
		pDrop->add_droptableid( mDropTableID[ i ] );
	}
	for ( unsigned int j = 0; j < (unsigned int) ARRAY_CNT( mDropCount ); j ++ )
	{
		pDrop->add_dropcount( mDropCount[ j ] );
	}
	return ;
}

int CTemplateDrop::Initialize()
{	
	mDropTurn = 0;
	memset( mDropNumberProbability, 0, sizeof(mDropNumberProbability));	
	memset( mDrops, 0, sizeof(mDrops));
	return 0;
}

void CTemplateDrop::SetFromPB( bool bServer,PBDropTable *pDrop)
{
	if ( pDrop == NULL )
	{
		return ;
	}
	mTempID			= pDrop->tempid();
	if ( bServer )
	{
		mDropTurn		= pDrop->dropturn();
		unsigned int i = 0;
		for ( i = 0; i < (unsigned int) pDrop->dropnumberrate_size() && i < (unsigned int) ARRAY_CNT( mDropNumberProbability ); i ++ )
		{
			mDropNumberProbability[ i ] = pDrop->dropnumberrate( i );
		}
		for ( i = 0; i < (unsigned int) pDrop->dropitem_size() && i < (unsigned int) ARRAY_CNT( mDrops ); i ++ )
		{
			PBDropItem* pItem = pDrop->mutable_dropitem( i );
			mDrops[ i ].mDropItemID = pItem->dropitemid();
			mDrops[ i ].mDropItemProbability = pItem->dropprobability();
			mDrops[ i ].mLevelProbability = pItem->levelprobablity();
			mDrops[ i ].mNotify = pItem->notify();
			mDrops[ i ].mSpecial = pItem->special();
			mDrops[ i ].mShow	= pItem->show();
		}
	}		
	return ;
}

void CTemplateDrop::CreateTlvPB( bool bServer,PBDropTable* pDrop )
{
	if ( pDrop == NULL )
	{
		return ;
	}
	pDrop->set_tempid( mTempID );
	if ( bServer )
	{
		pDrop->set_dropturn( mDropTurn );
		unsigned int i = 0;
		for ( i = 0; i < (unsigned int) ARRAY_CNT( mDropNumberProbability ); i ++ )
		{
			pDrop->add_dropnumberrate( (int)( mDropNumberProbability[ i ] ) );
		}
		for ( i = 0; i < (unsigned int) ARRAY_CNT( mDrops ); i ++ )
		{
			PBDropItem* tpItem = pDrop->add_dropitem();
			tpItem->set_dropitemid( mDrops[ i ].mDropItemID );
			tpItem->set_dropprobability( mDrops[ i ].mDropItemProbability );
			tpItem->set_levelprobablity( mDrops[ i ].mLevelProbability );
			tpItem->set_notify( mDrops[ i ].mNotify );
			tpItem->set_special( mDrops[ i ].mSpecial );
			tpItem->set_show( mDrops[ i ].mShow);
		}
	}	
	return ;
}

int CTemplateCollect::Initialize()
{

	memset( mName, 0, sizeof(mName));			
	memset( mModelName, 0, sizeof(mModelName));
	memset( mTaskID, 0, sizeof(mTaskID));
	memset( mItemID, 0, sizeof(mItemID));
	memset( mItemNum, 0, sizeof(mItemNum));
	memset( mItemProbality, 0, sizeof(mItemProbality));
	memset( mExtraItemNum, 0, sizeof(mExtraItemNum));
	memset( mExtraItemProbality, 0, sizeof(mExtraItemProbality));

	mNumber = 0;
	mDropID = 0;	
	mSkillType = 0;
	mSkillKind = 0;
	mApCost = 0; 
	mCollectDegree = 0;
	memset( mDegreeInfo, 0, sizeof(mDegreeInfo));
	mCollectLevel = 0;
	mCollectTaskType = 0;
	return 0;
}

void CTemplateCollect::SetFromPB( bool bServer,PBNPCCollect *pNpc)
{
	if ( pNpc == NULL )
	{
		return ;
	}
	mTempID				= pNpc->tempid();
	strncpy( mName, pNpc->name().c_str(), sizeof( mName ) - 1 );
	strncpy( mModelName, pNpc->modelname().c_str(), sizeof( mModelName ) - 1 );
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) pNpc->taskid_size() && i < (unsigned int) ARRAY_CNT( mTaskID ); i ++ )
	{
		mTaskID[ i ] = pNpc->taskid( i );
	}
	for ( i = 0; i < (unsigned int) pNpc->itemid_size() && i < (unsigned int) ARRAY_CNT( mItemID ); i ++ )
	{
		mItemID[ i ] = pNpc->itemid( i );
	}
	for ( i = 0; i < (unsigned int) pNpc->itemnum_size() && i < (unsigned int) ARRAY_CNT( mItemNum ); i ++ )
	{
		mItemNum[ i ] = pNpc->itemnum( i );
	}
	for( i = 0; i < (unsigned int) pNpc->itemprobality_size( ) && i < ARRAY_CNT( mItemProbality ); i++ )
	{
		mItemProbality[ i ] = pNpc->itemprobality( i );
	}
	for( i = 0; i < (unsigned int) pNpc->extraitemnum_size() && i < ARRAY_CNT( mExtraItemNum); i++)
	{
		mExtraItemNum[ i ] = pNpc->extraitemnum( i );
	}
	for ( i = 0; i < (unsigned int) pNpc->extraitemprobality_size() && i < ARRAY_CNT( mExtraItemProbality); i++)
	{
		mExtraItemProbality[ i ] = pNpc->extraitemprobality( i );
	}
	mDropID				= pNpc->dropid();
	mNumber				= pNpc->number();
	
	mSkillType			= pNpc->skilltype();	
	mSkillKind			= pNpc->skillkind();		
	mApCost				= pNpc->apcost();
	mCollectDegree		= pNpc->collectdegree();	
	
	for ( i = 0; i < (unsigned int)pNpc->value_size() && i < (unsigned int)pNpc->probality_size() &&
		i < (unsigned int)pNpc->degreevalue_size() && i < (unsigned int)ARRAY_CNT(mDegreeInfo ); ++i )
	{	
		mDegreeInfo[i].mValue		= pNpc->value( i );
		mDegreeInfo[i].mProbality	= pNpc->probality( i );
		mDegreeInfo[i].mDegreeValue	= pNpc->degreevalue( i );
	}
	mCollectLevel = pNpc->collectlevel();
	mCollectTaskType = pNpc->collecttasktype();
	return ;
}

void CTemplateCollect::CreateTlvPB( bool bServer,PBNPCCollect* pNpc )
{
	if ( pNpc == NULL )
	{
		return ;
	}
	pNpc->set_tempid( mTempID );
	pNpc->set_name( mName );
	pNpc->set_modelname( mModelName );
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mTaskID ); i ++ )
	{
		pNpc->add_taskid( mTaskID[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mItemID ); i ++ )
	{
		pNpc->add_itemid( mItemID[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mItemNum ); i ++ )
	{
		pNpc->add_itemnum( mItemNum[ i ] );
	}
	for( i = 0; i < (unsigned int) ARRAY_CNT( mExtraItemNum ); i++ )
	{
		pNpc->add_extraitemnum( mExtraItemNum[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mExtraItemProbality ); i++ )
	{
		pNpc->add_extraitemprobality( mExtraItemProbality[ i ] );
	}
	pNpc->set_dropid( mDropID );
	pNpc->set_number( mNumber );
	
	pNpc->set_skilltype( mSkillType );	
	pNpc->set_skillkind( mSkillKind );		
	pNpc->set_apcost( mApCost );
	pNpc->set_collectdegree( mCollectDegree );

	for ( i = 0; i < (unsigned int)ARRAY_CNT(mDegreeInfo ); ++i )
	{	
		pNpc->add_value( mDegreeInfo[i].mValue );
		pNpc->add_probality( mDegreeInfo[i].mProbality );
		pNpc->add_degreevalue( mDegreeInfo[i].mDegreeValue );
	}												
	for( i = 0; i < ARRAY_CNT( mItemProbality ); i++ )
	{
		pNpc->add_itemprobality( mItemProbality[ i ] );
	}
	pNpc->set_collectlevel( mCollectLevel );
	pNpc->set_collecttasktype( mCollectTaskType );
	return ;
}

int CTemplateOgre::Initialize()
{
	memset( mName, 0, sizeof(mName) );			
	memset( mModelName, 0, sizeof(mModelName));		
	mOgreType= 0;	
	mDropType= 0;	
	mStrategyID= 0;
	mDropID= 0;	
	mLevel= 0;				
	mDisplayLevel= 0;
	mAttackRate= 0;
	mAttackTime= 0;
	mHPRSpeed= 0;					
	mMPRSpeed= 0;							
	mRunSpeed= 0;		
	mInitLevel= 0;		
	mHP= 0;				
	mMP= 0;				
	mPAttack= 0;			
	mPDefence= 0;			
	mPAttackRange= 0;		
	mPReducePer= 0;
	mMAttack= 0;					
	mMDefence= 0;			
	mMAttackRange= 0;
	mMReducePer= 0;
	mDeathRate= 0;			
	mDeathValue= 0;	
	mDeathDefense= 0;
	mNormalHitrate= 0;	
	mNormalDuck= 0;
	mIgnoreDuck= 0;
	mRecover= 0;	
	memset( mResDef, 0, sizeof(mResDef));
	memset( mResAtt, 0, sizeof(mResAtt));
	mNeedDelay= 0;			
	mPatrolType= 0;	
	mIsInitiative= 0;		
	mWalkDistance= 0;
	mTeleTime= 0;
	mEnmityDistance= 0;	
	mEnmityTime= 0;	
	mClearTimer= 0;			
	mExp= 0;					
	mInitMoney= 0;					
	mDeltaMoney= 0;				 
	mResist= 0;					
	mTranID= 0;
	mPKType= 0;
	mGroupID= 0;
	mSkillID= 0;
	mSkillLevel= 0;
	mSearchDistance= 0;
	mLifeTime= 0;
	mResShow= 0;
	mJusticeValue= 0;
	memset( mFailTaskID, 0, sizeof(mFailTaskID));
	mBuffImmunity= 0;
	mObtainTitle= 0;
	mAwardID= 0;
	mRedstoneID = 0;
	mLmtLineID = 0;	
	mOgreEvent = 0;
	mCorpsValue = 0;
	mDropsDesc = 0;
	mDiffcultIndex = 0;
	mDiffcultAward = 0;
	return 0;
}

void CTemplateOgre::SetFromPB( bool bServer,PBOgreProperty *pOgre)
{
	if ( pOgre == NULL )
	{
		return ;
	}
	mTempID				= pOgre->tempid();
	strncpy( mName, pOgre->name().c_str(), sizeof( mName ) - 1 );
	strncpy( mModelName, pOgre->modelname().c_str(), sizeof( mModelName ) - 1 );
	mOgreType			= pOgre->ogretype();
	mDropType			= pOgre->droptype();
	mStrategyID			= pOgre->strategyid();
	mDropID				= pOgre->dropid();
	mLevel				= pOgre->level();
	mDisplayLevel		= pOgre->displayelevel();
	mAttackRate			= pOgre->attackrate();
	mAttackTime			= pOgre->attacktime();
	mHPRSpeed			= pOgre->hprspeed();
	mMPRSpeed			= pOgre->mprspeeds();
	mRunSpeed			= pOgre->runspeed();
	mInitLevel			= pOgre->initlevel();
	mHP					= pOgre->hp();
	mMP					= pOgre->mp();
	mPAttack			= pOgre->pattack();
	mPDefence			= pOgre->pdefence();
	mPAttackRange		= pOgre->pattackrange();
	mPReducePer			= pOgre->preduceper();
	mMAttack			= pOgre->mattack();
	mMDefence			= pOgre->mdefence();
	mMAttackRange		= pOgre->mattackrange();
	mMReducePer			= pOgre->mreduceper();
	mDeathRate			= pOgre->deathrate();
	mDeathValue			= pOgre->deathvalue();
	mDeathDefense		= pOgre->deathdefense();
	mNormalHitrate		= pOgre->normalhitrate();
	mNormalDuck			= pOgre->normalduck();
	mRecover			= pOgre->recover();
	mIgnoreDuck			= pOgre->ignoreduck();
	mNeedDelay			= pOgre->needdelay();
	mPatrolType			= pOgre->patroltype();
	mIsInitiative		= pOgre->isinitiative();
	mWalkDistance		= pOgre->walkdistance();
	mTeleTime			= pOgre->teletime();
	mEnmityDistance		= pOgre->enmitydistance();
	mEnmityTime			= pOgre->enmitytime();
	mClearTimer			= pOgre->cleartimer();
	mExp				= pOgre->exp();
	mInitMoney			= pOgre->initmoney();
	mDeltaMoney			= pOgre->deltamoney();
	mTranID				= pOgre->tranid();
	mPKType				= pOgre->pktype();
	mGroupID			= pOgre->groupid();
	mSkillID			= pOgre->skillid();
	mSkillLevel			= pOgre->skilllevel();
	mSearchDistance		= pOgre->searchdistance();
	mRedstoneID			= pOgre->redstoneid();
	for ( unsigned int i = 0; i < ( unsigned int )pOgre->resdef_size() && i < ARRAY_CNT( mResDef ); i++ )
	{
		mResDef[ i ] = pOgre->resdef( i );
	}
	for ( unsigned int i = 0; i < ( unsigned int )pOgre->resatt_size() && i < ARRAY_CNT( mResAtt ); i++ )
	{
		mResAtt[ i ] = pOgre->resatt( i );
	}
	mLifeTime			= pOgre->lefttime();
	mResShow			= pOgre->resshow();
	for ( unsigned int i = 0; i < ( unsigned int )pOgre->failtaskid_size() && i < ARRAY_CNT( mFailTaskID ); i++ )
	{
		mFailTaskID[ i ] = pOgre->failtaskid( i );
	}
	mJusticeValue		= pOgre->justicevalue();

	mBuffImmunity		= pOgre->buffimmunity( );
	mObtainTitle		= pOgre->obtaintitle();
	mAwardID			= pOgre->awardid();
	mLmtLineID			= pOgre->lmtlineid();
	mOgreEvent			= pOgre->ogreevent();
	mCorpsValue			= pOgre->corpsvalue();
	mDropsDesc			= pOgre->dropsdescid();
	mDiffcultIndex		= pOgre->diffcultindex();
	mDiffcultAward		= pOgre->diffcultaward();
	return ;
}

void CTemplateOgre::CreateTlvPB( bool bServer,PBOgreProperty* pOgre )
{
	if ( pOgre == NULL )
	{
		return ;
	}
	pOgre->set_tempid(mTempID);
	pOgre->set_name( mName );
	pOgre->set_modelname( mModelName );
	pOgre->set_ogretype(mOgreType);
	pOgre->set_droptype(mDropType);
	pOgre->set_strategyid(mStrategyID);
	pOgre->set_dropid(mDropID);
	pOgre->set_level(mLevel);
	pOgre->set_displayelevel(mDisplayLevel);
	pOgre->set_attackrate(mAttackRate);
	pOgre->set_attacktime(mAttackTime);
	pOgre->set_hprspeed(mHPRSpeed);
	pOgre->set_mprspeeds(mMPRSpeed);
	pOgre->set_runspeed(mRunSpeed);
	pOgre->set_initlevel(mInitLevel);
	pOgre->set_hp(mHP);
	pOgre->set_mp(mMP);
	pOgre->set_pattack(mPAttack);
	pOgre->set_pdefence(mPDefence);
	pOgre->set_pattackrange(mPAttackRange);
	pOgre->set_preduceper(mPReducePer);
	pOgre->set_mattack(mMAttack);
	pOgre->set_mdefence(mMDefence);
	pOgre->set_mattackrange(mMAttackRange);
	pOgre->set_mreduceper(mMReducePer);
	pOgre->set_deathrate(mDeathRate);
	pOgre->set_deathvalue(mDeathValue);
	pOgre->set_deathdefense(mDeathDefense);
	pOgre->set_normalhitrate(mNormalHitrate);
	pOgre->set_normalduck(mNormalDuck);
	pOgre->set_recover(mRecover);
	pOgre->set_ignoreduck(mIgnoreDuck);
	pOgre->set_needdelay(mNeedDelay);
	pOgre->set_patroltype(mPatrolType);
	pOgre->set_isinitiative(mIsInitiative);
	pOgre->set_walkdistance(mWalkDistance);
	pOgre->set_teletime(mTeleTime);
	pOgre->set_enmitydistance(mEnmityDistance);
	pOgre->set_enmitytime(mEnmityTime);
	pOgre->set_cleartimer(mClearTimer);
	pOgre->set_exp(mExp);
	pOgre->set_initmoney(mInitMoney);
	pOgre->set_deltamoney(mDeltaMoney);
	pOgre->set_tranid(mTranID);
	pOgre->set_pktype(mPKType);
	pOgre->set_groupid(mGroupID);
	pOgre->set_skillid(mSkillID);
	pOgre->set_skilllevel(mSkillLevel);
	pOgre->set_searchdistance(mSearchDistance);
	pOgre->set_buffimmunity( mBuffImmunity );
	for ( unsigned int i = 0; i < ARRAY_CNT( mResDef ); i++ )
	{
		pOgre->add_resdef( mResDef[ i ] );
	}
	for ( unsigned int i = 0; i < ARRAY_CNT( mResAtt ); i++ )
	{
		pOgre->add_resatt( mResAtt[ i ] );
	}
	pOgre->set_lefttime(mLifeTime);
	pOgre->set_resshow(mResShow);
	for ( unsigned int i = 0; i < ARRAY_CNT( mFailTaskID ); i++ )
	{
		pOgre->add_failtaskid( mFailTaskID[ i ] );
	}
	pOgre->set_justicevalue( mJusticeValue );
	pOgre->set_obtaintitle( mObtainTitle );
	pOgre->set_awardid( mAwardID );
	pOgre->set_lmtlineid( mLmtLineID );
	pOgre->set_redstoneid( mRedstoneID );
	pOgre->set_ogreevent( mOgreEvent );
	pOgre->set_corpsvalue( mCorpsValue);
	pOgre->set_dropsdescid(mDropsDesc);
	pOgre->set_diffcultindex( mDiffcultIndex );
	pOgre->set_diffcultaward( mDiffcultAward );
	return ;
}

int CTemplateBox::Initialize()
{
	memset( mClick, 0, sizeof(mClick));
	mType = BOX_NEEDKEY;
	memset( mDropTable, 0, sizeof(mDropTable));
	mKey = 0;
	return 0;
}

void CTemplateBox::SetFromPB( bool bServer,PBOgreBox *pBox)
{
	if ( pBox == NULL )
	{
		return ;
	}
	
	PBOgreProperty* pOgre = pBox->mutable_ogreinfo();
	CTemplateOgre::SetFromPB( bServer,pOgre );
	
	mType	= (BOX_TYPE)pBox->boxtype();
	mKey	= pBox->keyid();
	for ( int i = 0; i < pBox->click_size() && i < ( int )ARRAY_CNT( mClick ); i ++ )
	{
		
		PBBox_Click* pClick = pBox->mutable_click( i );
		mClick[ i ].mEvent = ( STORTBOX_EVENT )pClick->event();
		mClick[ i ].mProbability = pClick->probability( );
	}
	
	return ;
}

void CTemplateBox::CreateTlvPB( bool bServer,PBOgreBox* pBox )
{
	if ( pBox == NULL )
	{
		return ;
	}
	PBOgreProperty* pOgre = pBox->mutable_ogreinfo();
	CTemplateOgre::CreateTlvPB( bServer,pOgre ); 	
	for( int i = 0; i < ( int )ARRAY_CNT( mClick ); i++ )
	{
		PBBox_Click* tpClick = pBox->add_click();
		tpClick->set_event( mClick[ i ].mEvent );
		tpClick->set_probability( mClick[ i ].mProbability );
	}
	pBox->set_boxtype( (int)mType );
	pBox->set_keyid( mKey );	 
	return ;
}

int CTemplateShowPet::Initialize()
{
	memset( mName, 0, sizeof(mName));			
	memset( mModel, 0, sizeof(mModel));			
	memset( mIconName, 0, sizeof(mIconName));		
	mMetierID=0;
	mMaxLevel=0;
	mRequireLevel=0;
	mExceedLevel=0;
	mPetItemID=0;
	mRevertCost=0;
	mInitStorage=0;
	mLevelIncStorage=0;
	mIncStorage=0;
	mExpPerMin=0;
	mRepletionLimit=0;
	mRepletionHP=0;
	mRevertHPValue=0;
	mMinRepletion1=0;
	mMaxRepletion1=0;
	mExpSpeedRate1=0;
	mMinRepletion2=0;
	mMaxRepletion2=0;
	mExpSpeedRate2=0;
	mMinRepletion3=0;
	mMaxRepletion3=0;
	mExpSpeedRate3=0;
	mMinRepletion4=0;
	mMaxRepletion4=0;
	mExpSpeedRate4=0;
	mMinRepletion5=0;
	mMaxRepletion5=0;
	mExpSpeedRate5=0;
	mFoodCombo=0;
	return 0;
}


void CTemplateShowPet::SetFromPB( bool bServer,PBShowPet *pPet)
{
	if ( pPet == NULL )
	{
		return ;
	}
	mTempID			= pPet->tempid();
	strncpy( mName, pPet->name().c_str(), sizeof( mName ) - 1 );
	strncpy( mModel, pPet->model().c_str(), sizeof( mModel ) - 1 );
	strncpy( mIconName, pPet->iconname().c_str(), sizeof( mIconName ) - 1 );
	mMetierID		= pPet->metierid();
	mMaxLevel		= pPet->maxlevel();
	mRequireLevel	= pPet->levelrequire();
	mExceedLevel	= pPet->exceedlevel();
	mPetItemID		= pPet->petitemid();
	mRevertCost		= pPet->revertcost();
	mInitStorage	= pPet->initstorage();
	mLevelIncStorage = pPet->levelincstorage();
	mIncStorage		= pPet->incstorage();
	mExpPerMin		= pPet->exppermin();
	mRepletionHP	= pPet->repletionhp();
	mRepletionLimit	= pPet->repletionlimit();
	mRevertHPValue	= pPet->reverthpvalue();
	mMinRepletion1	= pPet->minrepletion1();
	mMaxRepletion1	= pPet->maxrepletion1();

	mExpSpeedRate1	= pPet->expspeedrate1();
	mMinRepletion2	= pPet->minrepletion2();
	mMaxRepletion2	= pPet->maxrepletion2();
	mExpSpeedRate2	= pPet->expspeedrate2();
	mMinRepletion3	= pPet->minrepletion3();
	mMaxRepletion3	= pPet->maxrepletion3();
	mExpSpeedRate3	= pPet->expspeedrate3();
	mMinRepletion4	= pPet->minrepletion4();
	mMaxRepletion4	= pPet->maxrepletion4();
	mExpSpeedRate4	= pPet->expspeedrate4();
	mMinRepletion5	= pPet->minrepletion5();
	mMaxRepletion5	= pPet->maxrepletion5();
	mExpSpeedRate5	= pPet->expspeedrate5();
	mFoodCombo		= pPet->foodcombo();
	return ;
}

void CTemplateShowPet::CreateTlvPB( bool bServer,PBShowPet* pPet )
{
	if ( pPet == NULL )
	{
		return ;
	}
	pPet->set_tempid(mTempID);
	pPet->set_name(mName);
	pPet->set_model(mModel);
	pPet->set_iconname(mIconName);
	pPet->set_metierid(mMetierID);
	pPet->set_maxlevel(mMaxLevel);
	pPet->set_levelrequire(mRequireLevel);
	pPet->set_exceedlevel(mExceedLevel);
	pPet->set_petitemid(mPetItemID);
	pPet->set_revertcost(mRevertCost);
	pPet->set_initstorage(mInitStorage);
	pPet->set_levelincstorage(mLevelIncStorage);
	pPet->set_incstorage(mIncStorage);
	pPet->set_exppermin(mExpPerMin);
	pPet->set_repletionhp(mRepletionHP);
	pPet->set_repletionlimit(mRepletionLimit);
	pPet->set_reverthpvalue(mRevertHPValue);
	pPet->set_minrepletion1(mMinRepletion1);
	pPet->set_maxrepletion1(mMaxRepletion1);

	pPet->set_expspeedrate1((int)(mExpSpeedRate1));
	pPet->set_minrepletion2(mMinRepletion2);
	pPet->set_maxrepletion2(mMaxRepletion2);
	pPet->set_expspeedrate2((int)(mExpSpeedRate2));
	pPet->set_minrepletion3(mMinRepletion3);
	pPet->set_maxrepletion3(mMaxRepletion3);
	pPet->set_expspeedrate3((int)(mExpSpeedRate3));
	pPet->set_minrepletion4(mMinRepletion4);
	pPet->set_maxrepletion4(mMaxRepletion4);
	pPet->set_expspeedrate4((int)(mExpSpeedRate4));
	pPet->set_minrepletion5(mMinRepletion5);
	pPet->set_maxrepletion5(mMaxRepletion5);
	pPet->set_expspeedrate5((int)(mExpSpeedRate5));
	pPet->set_foodcombo(mFoodCombo);
	return ;
}

int CTemplateBattlePet::Initialize()
{

	memset( mName, 0, sizeof(mName));			
	memset( mModel, 0, sizeof(mModel));			
	memset( mIconName, 0, sizeof(mIconName));		
	mMetierID=0;
	mMaxLevel=0;
	mRequireLevel=0;
	mExceedLevel=0;
	mPetItemID=0;
	mRevertCost=0;
	mRepletionLimit=0;
	mMinRepletion1=0;
	mMaxRepletion1=0;
	mExpSpeedRate1=0;
	mHPRate1=0;
	mAttackRate1=0;
	mMinRepletion2=0;
	mMaxRepletion2=0;
	mExpSpeedRate2=0;
	mHPRate2=0;
	mAttackRate2=0;
	mMinRepletion3=0;
	mMaxRepletion3=0;
	mExpSpeedRate3=0;
	mHPRate3=0;
	mAttackRate3=0;
	mMinRepletion4=0;
	mMaxRepletion4=0;
	mExpSpeedRate4=0;
	mHPRate4=0;
	mAttackRate4=0;
	mMinRepletion5=0;
	mMaxRepletion5=0;
	mExpSpeedRate5=0;
	mHPRate5=0;
	mAttackRate5=0;
	mHPValue=0;
	mHPPotential=0;
	mMPValue=0;
	mMPPotential=0;
	mPAttack=0;
	mPAttackPotential=0;
	mMAttack=0;
	mMAttackPotential=0;
	mPDefence=0;
	mPDefencePotential=0;
	mMDefence=0;
	mMDefencePotential=0;
	mPResist=0;
	mPResistPotential=0;
	mMResist=0;
	mMResistPotential=0;
	mAbnResist=0;
	mAbnResistPotential=0;
	mFireResist=0;
	mFireResistPotential=0;
	mWindResist=0;
	mWindResistPotential=0;
	mLightResist=0;
	mLightResistPotential=0;
	mShadowResist=0;
	mShadowResistPotential=0;
	mWaterResist=0;
	mWaterResistPotential=0;
	mEarthResist=0;
	mEarthResistPotential=0;
	mFoodCombo=0;
	return 0;
}


void CTemplateBattlePet::SetFromPB( bool bServer,PBBattlePet *pPet)
{
	if ( pPet == NULL )
	{
		return ;
	}
	mTempID			= pPet->tempid();
	strncpy( mName, pPet->name().c_str(), sizeof( mName ) - 1 );
	strncpy( mModel, pPet->model().c_str(), sizeof( mModel ) - 1 );
	strncpy( mIconName, pPet->iconname().c_str(), sizeof( mIconName ) - 1 );
	mMetierID		= pPet->metierid();
	mMaxLevel		= pPet->maxlevel();
	mRequireLevel	= pPet->levelrequire();
	mExceedLevel	= pPet->exceedlevel();
	mPetItemID		= pPet->petitemid();
	mRevertCost		= pPet->revertcost();
	mRepletionLimit	= pPet->repletionlimit();
	mMinRepletion1	= pPet->minrepletion1();
	mMaxRepletion1	= pPet->maxrepletion1();
	mExpSpeedRate1	= pPet->expspeedrate1();
	mHPRate1		= pPet->hprate1();
	mAttackRate1	= pPet->attackrate1();
	mMinRepletion2	= pPet->minrepletion2();
	mMaxRepletion2	= pPet->maxrepletion2();
	mExpSpeedRate2	= pPet->expspeedrate2();
	mHPRate2		= pPet->hprate2();
	mAttackRate2	= pPet->attackrate2();

	mMinRepletion3	= pPet->minrepletion3();
	mMaxRepletion3	= pPet->maxrepletion3();
	mExpSpeedRate3	= pPet->expspeedrate3();
	mHPRate3		= pPet->hprate3();
	mAttackRate3	= pPet->attackrate3();
	mMinRepletion4	= pPet->minrepletion4();
	mMaxRepletion4	= pPet->maxrepletion4();
	mExpSpeedRate4	= pPet->expspeedrate4();
	mHPRate4		= pPet->hprate4();
	mAttackRate4	= pPet->attackrate4();
	mMinRepletion5	= pPet->minrepletion5();
	mMaxRepletion5	= pPet->maxrepletion5();
	mExpSpeedRate5	= pPet->expspeedrate5();
	mHPRate5		= pPet->hprate5();
	mAttackRate5	= pPet->attackrate5();
	mHPValue		= pPet->hpvalue();
	mHPPotential	= pPet->hppotential();
	mMPValue		= pPet->mpvalue();
	mMPPotential	= pPet->mppotential();
	mPAttack		= pPet->pattack();
	mPAttackPotential = pPet->pattackpotential();
	mMAttack		= pPet->mattack();
	mPAttackPotential = pPet->pattackpotential();

	mPDefence		= pPet->pdefence();
	mPDefencePotential = pPet->pdefencepotential();
	mMDefence		= pPet->mdefence();
	mMDefencePotential = pPet->mdefencepotential();
	mPResist		= pPet->presist();
	mPResistPotential = pPet->presistpotential();
	mMResist		= pPet->mresist();
	mMResistPotential = pPet->mresistpotential();
	mAbnResist		= pPet->abnresist();
	mAbnResistPotential	= pPet->abnresistpotential();
	mFireResist		= pPet->fireresist();
	mFireResistPotential = pPet->fireresistpotential();
	mWindResist		= pPet->windresist();
	mWindResistPotential = pPet->windresistpotential();
	mLightResist	= pPet->lightresist();
	mLightResistPotential = pPet->lightresistpotential();
	mShadowResist	= pPet->shadowresist();
	mShadowResistPotential = pPet->shadowresistpotential();
	mWaterResist	= pPet->waterresist();
	mWaterResistPotential = pPet->waterresistpotential();
	mEarthResist	= pPet->earthresist();
	mEarthResistPotential = pPet->earthresistpotential();
	mFoodCombo		= pPet->foodcombo();
	return ;
}	

void CTemplateBattlePet::CreateTlvPB( bool bServer,PBBattlePet *pPet)
{
	if ( pPet == NULL )
	{
		return ;
	}
	pPet->set_tempid(mTempID);
	pPet->set_name(mName);
	pPet->set_model(mModel);
	pPet->set_iconname(mIconName);
	pPet->set_metierid(mMetierID);
	pPet->set_maxlevel(mMaxLevel);
	pPet->set_levelrequire(mRequireLevel);
	pPet->set_exceedlevel(mExceedLevel);
	pPet->set_petitemid(mPetItemID);
	pPet->set_revertcost(mRevertCost);
	pPet->set_repletionlimit(mRepletionLimit);
	pPet->set_minrepletion1(mMinRepletion1);
	pPet->set_maxrepletion1(mMaxRepletion1);
	pPet->set_expspeedrate1((int)(mExpSpeedRate1));
	pPet->set_hprate1((int)(mHPRate1));
	pPet->set_attackrate1((int)(mAttackRate1));
	pPet->set_minrepletion2(mMinRepletion2);
	pPet->set_maxrepletion2(mMaxRepletion2);
	pPet->set_expspeedrate2((int)(mExpSpeedRate2));
	pPet->set_hprate2((int)(mHPRate2));
	pPet->set_attackrate2((int)(mAttackRate2));

	pPet->set_minrepletion3(mMinRepletion3);
	pPet->set_maxrepletion3(mMaxRepletion3);
	pPet->set_expspeedrate3((int)(mExpSpeedRate3));
	pPet->set_hprate3((int)(mHPRate3));
	pPet->set_attackrate3((int)(mAttackRate3));
	pPet->set_minrepletion4(mMinRepletion4);
	pPet->set_maxrepletion4(mMaxRepletion4);
	pPet->set_expspeedrate4((int)(mExpSpeedRate4));
	pPet->set_hprate4((int)(mHPRate4));
	pPet->set_attackrate4((int)(mAttackRate4));
	pPet->set_minrepletion5(mMinRepletion5);
	pPet->set_maxrepletion5(mMaxRepletion5);
	pPet->set_expspeedrate5((int)(mExpSpeedRate5));
	pPet->set_hprate5((int)(mHPRate5));
	pPet->set_attackrate5((int)(mAttackRate5));
	pPet->set_hpvalue(mHPValue);
	pPet->set_hppotential(mHPPotential);
	pPet->set_mpvalue(mMPValue);
	pPet->set_mppotential(mMPPotential);
	pPet->set_pattack(mPAttack);
	pPet->set_pattackpotential(mPAttackPotential);
	pPet->set_mattack(mMAttack);
	pPet->set_pattackpotential(mPAttackPotential);

	pPet->set_pdefence(mPDefence);
	pPet->set_pdefencepotential(mPDefencePotential);
	pPet->set_mdefence(mMDefence);
	pPet->set_mdefencepotential(mMDefencePotential);
	pPet->set_presist(mPResist);
	pPet->set_presistpotential(mPResistPotential);
	pPet->set_mresist(mMResist);
	pPet->set_mresistpotential(mMResistPotential);
	pPet->set_abnresist(mAbnResist);
	pPet->set_abnresistpotential(mAbnResistPotential);
	pPet->set_fireresist(mFireResist);
	pPet->set_fireresistpotential(mFireResistPotential);
	pPet->set_windresist(mWindResist);
	pPet->set_windresistpotential(mWindResistPotential);
	pPet->set_lightresist(mLightResist);
	pPet->set_lightresistpotential(mLightResistPotential);
	pPet->set_shadowresist(mShadowResist);
	pPet->set_shadowresistpotential(mShadowResistPotential);
	pPet->set_waterresist(mWaterResist);
	pPet->set_waterresistpotential(mWaterResistPotential);
	pPet->set_earthresist(mEarthResist);
	pPet->set_earthresistpotential(mEarthResistPotential);
	pPet->set_foodcombo(mFoodCombo);
	return ;
}

int CTemplateBlueprint::Initialize()
{
	mNeedSkillID = 0;
	mNeedSkillLevel = 0;
	mProduceCount = 0;
	mProduceCost = 0;
	mObtainExp = 0;
	memset( mStuff, 0, sizeof( mStuff ));
	memset( mProduct, 0, sizeof(mProduct));
	return 0;
}

void CTemplateBlueprint::SetFromPB( bool bServer,PBItemBluePrint *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,tpBase );
	mTempID				= pItem->tempid();
	mNeedSkillID		= pItem->needskillid();
	mNeedSkillLevel		= pItem->needskilllevel();
	mProduceCount		= pItem->producecount();
	mProduceCost		= pItem->producecost();
	mObtainExp			= pItem->obtainexp();
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) pItem->stuff_size() && i < (unsigned int) ARRAY_CNT( mStuff ); i ++ )
	{
		PBStuff* pStuff = pItem->mutable_stuff( i );
		mStuff[ i ].mNumber = pStuff->number();
		mStuff[ i ].mStuffID = pStuff->stuffid();
	}
	for ( i = 0; i < (unsigned int) pItem->product_size() && i < (unsigned int) ARRAY_CNT( mProduct ); i ++ )
	{
		PBProduct* pProduct = pItem->mutable_product( i );
		mProduct[ i ].mProductID = pProduct->productid();
		mProduct[ i ].mPercent = pProduct->percent();
	}
	return ;
}

void CTemplateBlueprint::CreateTlvPB( bool bServer,PBItemBluePrint* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer, tpBase );
	pItem->set_tempid( mTempID );
	pItem->set_needskillid( mNeedSkillID );
	pItem->set_needskilllevel( mNeedSkillLevel );
	pItem->set_producecount( mProduceCount );
	pItem->set_producecost( mProduceCost );
	pItem->set_obtainexp( mObtainExp );
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mStuff ); i ++ )
	{
		PBStuff* tpStuff = pItem->add_stuff();
		tpStuff->set_number( mStuff[ i ].mNumber );
		tpStuff->set_stuffid( mStuff[ i ].mStuffID );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mProduct ); i ++ )
	{
		PBProduct* tpProduct = pItem->add_product();
		tpProduct->set_productid( mProduct[ i ].mProductID );
		tpProduct->set_percent( mProduct[ i ].mPercent  ); 
	}
	return ;
}

int CTemplateNpc::Initialize()
{

	mFuncType=0;	
	memset( mTalk, 0, sizeof(mTalk));
	mStageID=0;	
	mTalkServiceID=0;
	mSellServiceID=0;
	mMailService=0;
	mStorageService=0;
	mUpgService=0;
	mEqtBindService=0;		
	mWelfareService=0;		
	memset( mDesc, 0, sizeof(mDesc));
	mStudySkillMetierID=0;	
	mTownX=0;
	mTownY=0;
	mStorageTaskNPC=0;
	mStorageTaskID=0;
	mDownLevel=0;
	mUpLevel=0;	
	mResetPService=0;	
	mDiploidTime=0;
	mFamilyService=0;	
	mCorpService=0;
	mJudgeService=0;
	mChangeService=0;	
	mActivityService=0;	
	mExchangeService=0;
	mHonorService=0;
	mBattle=0;
	mLifeSkillService=0;	
	mBidDay=0;
	mBidHour=0;
	mBidMinute=0;
	mCorpsBattle=0;
	mMinBidPrice=0;
	mBattleCity=0;
	mMWProJudgeService=0;
	mMWSkillJudgeService=0;
	mEquipExchangeService=0;
	mExChangeTableID=0;	
	mCorpsBidDay=0;
	mCorpsBidHour=0;
	mCorpsBidMinute=0;
	mMWClearPointService=0;
	mMWSecondJudgeService=0;
	mUnionService=0;
	mCountryService=0;
	mFriendService=0;
	mCountryType=0;
	mGoldenYBService=0;
	mChairManTaskService=0;
	mChairManTaskID=0;
	mMarriageService=0;
	mMatrixService=0;
	mFamilyType=0;	
	mStoneBuff=0;
	mDisappear=0;
	mRefreshService=0;
	mPedalIndex=0;
	mDirectService=0;
	mExtraService=0;
	mWarService=0;
	mWarOpType=0;
	mWarValue=0;
	mWarCamp=0;
	mWarHonor=0;
	mFangCunService=0;
	mResistTran=0;
	mTaxService=0;
	mChangeCampService =0;
	mWeakProtectService=0;
    mYaoQianShuRankService=0;
    mYaoQianShuServiceInfo=0;
    mHandInService =0;
	mSpecialEvent=0;
	mTreasureBowlService=0;
	mCorpsBidStartDay=0;
	mCorpsBidStartHour=0;
	mCorpsBidStartMinute=0;
	mLastOneService=0;
	mFamilyBidStartHour=0;
	mFamilyBidStartMinute=0;
	mRandomTeleService=0;
	mShiTuService=0;
	mSwornService=0;
	mGlobalWarService=0;
	mSuperEquipExchange=0;
	return 0;
}

void CTemplateNpc::SetFromPB( bool bServer,PBNpc *pNpc)
{
	if ( pNpc == NULL )
	{
		return ;
	}
	CTemplateOgre::SetFromPB( bServer,pNpc->mutable_ogre() );
	mFuncType		= pNpc->functype();
	strncpy( mTalk, pNpc->talk().c_str(), sizeof( mTalk ) - 1 );
	mStageID		= pNpc->stageid();
	mTalkServiceID	= pNpc->talkserviceid();
	mSellServiceID	= pNpc->sellserviceid();
	mMailService	= pNpc->mailservice();
	mStorageService	= pNpc->storageservice();
	mUpgService		= pNpc->upgservice();
	mEqtBindService	= pNpc->eqtbindservice();
	mWelfareService	= pNpc->welfareservice();
	strncpy( mDesc, pNpc->describe().c_str(), sizeof( mDesc ) - 1 );
	mStudySkillMetierID = pNpc->studyskillid();
	mTownX			= pNpc->townx();
	mTownY			= pNpc->towny();
	mStorageTaskNPC	= pNpc->storagetasknpc();
	mStorageTaskID	= pNpc->storagetaskid();
	mDownLevel		= pNpc->downlevel();
	mUpLevel		= pNpc->uplevel();
	mDiploidTime	= pNpc->diploidtable();
	mResetPService	= pNpc->resetpservice();
	mFamilyService	= pNpc->familyservice();
	mCorpService	= pNpc->corpservice();
	mChangeService  = pNpc->changeservice();
	mJudgeService	= pNpc->judgeservice();
	mActivityService = pNpc->activityservice();
	mExchangeService = pNpc->exchangeservice();
	mHonorService	= pNpc->honorservice();
	mBattle			= pNpc->battle( );
	mLifeSkillService = pNpc->lifeskillservice();
	mBidDay			= pNpc->bidday();
	mBidHour		= pNpc->bidhour();
	mBidMinute		= pNpc->bidminute();
	mCorpsBattle	= pNpc->coprsbattle();
	mMinBidPrice	= pNpc->minbidprice();
	mBattleCity		= pNpc->battlecity();
	mMWProJudgeService = pNpc->mwprojudgeservice();
	mMWSkillJudgeService = pNpc->mwskilljudgeservice();
	mEquipExchangeService = pNpc->equipexchangeservice();
	mExChangeTableID	  = pNpc->exchangetableid();
	mCorpsBidDay	= pNpc->corpsbidday();
	mCorpsBidHour	= pNpc->corpsbidhour();
	mCorpsBidMinute	= pNpc->corpsbidminute();
	mMWClearPointService = pNpc->mwclearpointservice();
	mMWSecondJudgeService = pNpc->mwsecondjudgeservice();
	mUnionService = pNpc->unionservice();
	mCountryService = pNpc->countryservice();
	mFriendService = pNpc->friendservice();
	mCountryType = pNpc->countrytype();
	mGoldenYBService = pNpc->goldenybservice();
	mChairManTaskService = pNpc->chairmantaskservice();
	mChairManTaskID		= pNpc->chairmantaskid();
	mMarriageService = pNpc->marriageservice();
	mMatrixService = pNpc->matrixservice();
	mFamilyType	= pNpc->familytype();
	mStoneBuff = pNpc->stonebuff();
	mDisappear = pNpc->disappear();
	mRefreshService = pNpc->refreshservice();
	mPedalIndex = pNpc->pedalindex();
	mDirectService = pNpc->directservice();
	mExtraService = pNpc->extraservice();
	mWarService = pNpc->warservice();
	mWarOpType = pNpc->waroptype();
	mWarValue = pNpc->warvalue();
	mWarCamp = pNpc->warcamp();
    mEquipLevelService = pNpc->equiplevelservice();
	mWarHonor = pNpc->warhonor();
	mFangCunService = pNpc->fangcunservice();
	mResistTran = pNpc->resisttran();
	mFBFunc = pNpc->fbfunc( );
	mFBFuncParame1 = pNpc->fbfunc_parame1( );
	mFBFuncParame2 = pNpc->fbfunc_parame2( );
	strncpy( mFBFuncStr, pNpc->fbfunc_str( ).c_str( ) , sizeof( mFBFuncStr ));
	mTaxService = pNpc->taxservice();
	mChangeCampService = pNpc->changecampservice();
	mWeakProtectService = pNpc->weakprotectservice();
    mYaoQianShuRankService = pNpc->yaoqianshurank();
    mYaoQianShuServiceInfo = pNpc->yaoqianinfo();
    mHandInService = pNpc->handin();
	mSpecialEvent = pNpc->specialevent();
	mTreasureBowlService = pNpc->treasurebowl();
	mMWDecomposeService	 = pNpc->mwdecomposeservice();
	mMWImprintService	 = pNpc->mwimprintservice();
	mMWUpdateService	 = pNpc->mwupdateservice();
	mMWUpdateExService	 = pNpc->mwupdateexservice();
	mCorpsBidStartDay	= pNpc->corpsbidstartday();
	mCorpsBidStartHour	= pNpc->corpsbidstarthour();
	mCorpsBidStartMinute	= pNpc->corpsbidstartminute();
	mLastOneService		= pNpc->lastoneservice();
	mFamilyBidStartHour		= pNpc->familybidstarthour();
	mFamilyBidStartMinute	= pNpc->familybidstartminute();
	mMWQLComposeService     = pNpc->mwqlcomposeservice();
	mRandomTeleService	=  pNpc->randomteleservice();
	mIntroduceMapID = pNpc->introducemapid( );
	mShiTuService = pNpc->shituservice();
	mSwornService = pNpc->swornservice();
	mGlobalWarService = pNpc->globalwarservice();
	mSuperEquipExchange = pNpc->superequipexchange();
	return ;
}

void CTemplateNpc::CreateTlvPB( bool bServer,PBNpc* pNpc )
{
	if ( pNpc == NULL )
	{
		return ;
	}
	CTemplateOgre::CreateTlvPB( bServer,pNpc->mutable_ogre() );
	pNpc->set_functype(mFuncType);
	pNpc->set_talk(mTalk);
	pNpc->set_stageid(mStageID);
	pNpc->set_talkserviceid(mTalkServiceID);
	pNpc->set_sellserviceid(mSellServiceID);
	pNpc->set_mailservice(mMailService);
	pNpc->set_storageservice(mStorageService);
	pNpc->set_upgservice(mUpgService);
	pNpc->set_eqtbindservice(mEqtBindService);
	pNpc->set_welfareservice(mWelfareService);
	pNpc->set_describe(mDesc);
	pNpc->set_studyskillid(mStudySkillMetierID);
	pNpc->set_townx(mTownX);
	pNpc->set_towny(mTownY);
	pNpc->set_storagetasknpc(mStorageTaskNPC);
	pNpc->set_storagetaskid(mStorageTaskID);
	pNpc->set_downlevel(mDownLevel);
	pNpc->set_uplevel(mUpLevel);
	pNpc->set_diploidtable(mDiploidTime);
	pNpc->set_resetpservice( mResetPService );
	pNpc->set_familyservice( mFamilyService );
	pNpc->set_corpservice( mCorpService );
	pNpc->set_changeservice( mChangeService );
	pNpc->set_judgeservice( mJudgeService );
	pNpc->set_activityservice( mActivityService );
	pNpc->set_exchangeservice( mExchangeService );
	pNpc->set_honorservice( mHonorService );
	pNpc->set_battle( mBattle );
	pNpc->set_lifeskillservice( mLifeSkillService );
	pNpc->set_bidday( mBidDay );
	pNpc->set_bidhour( mBidHour );
	pNpc->set_bidminute( mBidMinute );
	pNpc->set_coprsbattle( mCorpsBattle );
	pNpc->set_minbidprice( mMinBidPrice );
	pNpc->set_battlecity( mBattleCity );
	pNpc->set_mwprojudgeservice( mMWProJudgeService );
	pNpc->set_mwskilljudgeservice( mMWSkillJudgeService );
	pNpc->set_equipexchangeservice( mEquipExchangeService );
	pNpc->set_exchangetableid( mExChangeTableID );
	pNpc->set_corpsbidday( mCorpsBidDay );
	pNpc->set_corpsbidhour( mCorpsBidHour );
	pNpc->set_corpsbidminute( mCorpsBidMinute );
	pNpc->set_mwclearpointservice( mMWClearPointService );
	pNpc->set_mwsecondjudgeservice( mMWSecondJudgeService );
	pNpc->set_unionservice( mUnionService );
	pNpc->set_countryservice( mCountryService );
	pNpc->set_friendservice( mFriendService );
	pNpc->set_countrytype( mCountryType );
	pNpc->set_goldenybservice( mGoldenYBService );
	pNpc->set_chairmantaskservice( mChairManTaskService );
	pNpc->set_chairmantaskid( mChairManTaskID );
	pNpc->set_marriageservice( mMarriageService );
	pNpc->set_matrixservice( mMatrixService );
	pNpc->set_familytype( mFamilyType );
	pNpc->set_stonebuff( mStoneBuff );
	pNpc->set_disappear( mDisappear );
	pNpc->set_refreshservice( mRefreshService );
	pNpc->set_pedalindex( mPedalIndex );
	pNpc->set_directservice( mDirectService );
	pNpc->set_extraservice( mExtraService );
	pNpc->set_warservice( mWarService );
	pNpc->set_waroptype( mWarOpType );
	pNpc->set_warvalue( mWarValue );
	pNpc->set_warcamp( mWarCamp );
	pNpc->set_equiplevelservice( mEquipLevelService );
	pNpc->set_warhonor( mWarHonor );
	pNpc->set_fangcunservice( mFangCunService );
	pNpc->set_resisttran( mResistTran );
	pNpc->set_fbfunc( mFBFunc );
	pNpc->set_fbfunc_parame1( mFBFuncParame1 );
	pNpc->set_fbfunc_parame2( mFBFuncParame2 );
	pNpc->set_fbfunc_str( mFBFuncStr );
	pNpc->set_taxservice( mTaxService );
	pNpc->set_changecampservice(mChangeCampService);
	pNpc->set_weakprotectservice(mWeakProtectService);
    pNpc->set_yaoqianshurank( mYaoQianShuRankService );
    pNpc->set_yaoqianinfo( mYaoQianShuServiceInfo );
    pNpc->set_handin( mHandInService );
	pNpc->set_specialevent( mSpecialEvent );
	pNpc->set_treasurebowl( mTreasureBowlService );
	pNpc->set_mwdecomposeservice(mMWDecomposeService);
	pNpc->set_mwimprintservice(  mMWImprintService	);
	pNpc->set_mwupdateservice(   mMWUpdateService	);
	pNpc->set_mwupdateexservice( mMWUpdateExService	);
	pNpc->set_corpsbidstartday( mCorpsBidStartDay );
	pNpc->set_corpsbidstarthour( mCorpsBidStartHour );
	pNpc->set_corpsbidstartminute( mCorpsBidStartMinute );
	pNpc->set_lastoneservice( mLastOneService );
	pNpc->set_familybidstarthour( mFamilyBidStartHour );
	pNpc->set_familybidstartminute( mFamilyBidStartMinute );
	pNpc->set_mwqlcomposeservice(mMWQLComposeService);
	pNpc->set_randomteleservice( mRandomTeleService );
	pNpc->set_introducemapid( mIntroduceMapID );
	pNpc->set_shituservice( mShiTuService );
	pNpc->set_swornservice( mSwornService );
	pNpc->set_globalwarservice( mGlobalWarService );
	pNpc->set_superequipexchange( mSuperEquipExchange );
	return ;
}

int CTemplateMine::Initialize()
{
	memset( mName, 0, sizeof(mName) );
	mLevel = 0;
	mLevelRequire = 0;
	mToolID = 0;
	mDecRate = 0;
	mMineTimeLow = 0;
	mMineTimeHigh = 0;
	memset( mModel, 0, sizeof(mModel));
	memset( mOutput, 0, sizeof(mOutput));
	memset( mOgre, 0, sizeof(mOgre));
	return 0;
}

void CTemplateMine::SetFromPB( bool bServer,PBMine *pMine)
{
	if ( pMine == NULL )
	{
		return ;
	}
	mTempID				= pMine->tempid();
	strncpy( mName, pMine->name().c_str(), sizeof( mName ) - 1 );
	mLevel				= pMine->level();
	mLevelRequire		= pMine->levelrequire();
	mToolID				= pMine->toolid();
	mDecRate			= pMine->decrate();
	mMineTimeLow		= pMine->minetimelow();
	mMineTimeHigh		= pMine->minetimehigh();
	strncpy( mModel, pMine->model().c_str(), sizeof( mModel ) - 1 );
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) pMine->output_size() && i < (unsigned int) ARRAY_CNT( mOutput ); i ++ )
	{
		PBMineOutput* pOutput = pMine->mutable_output( i );
		mOutput[ i ].mStuffID = pOutput->stuffid();
		mOutput[ i ].mPrecent = pOutput->percent();
	}
	for ( i = 0; i < (unsigned int) pMine->mineogre_size() && i < (unsigned int) ARRAY_CNT( mOgre ); i ++ )
	{
		PBMineOgre* pOgre = pMine->mutable_mineogre( i );
		mOgre[ i ].mNpcID = pOgre->npcid();
		mOgre[ i ].mNumber = pOgre->number();
		mOgre[ i ].mPrecent = pOgre->percent();
	}
	return ;
}

void CTemplateMine::CreateTlvPB( bool bServer,PBMine* pMine )
{
	if ( pMine == NULL )
	{
		return ;
	}
	pMine->set_tempid( mTempID );
	pMine->set_name( mName );
	pMine->set_level( mLevel );
	pMine->set_levelrequire( mLevelRequire );
	pMine->set_toolid( mToolID );
	pMine->set_decrate( (int)(mDecRate) );
	pMine->set_minetimelow( mMineTimeLow );
	pMine->set_minetimehigh( mMineTimeHigh );
	pMine->set_model( mModel );
	unsigned int i = 0; 
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mOutput ); i ++ )
	{
		PBMineOutput* tpOutput = pMine->add_output();
		tpOutput->set_stuffid( mOutput[ i ].mStuffID );
		tpOutput->set_percent( (int)(mOutput[ i ].mPrecent) );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mOgre ); i ++ )
	{
		PBMineOgre* tpOgre = pMine->add_mineogre();
		tpOgre->set_npcid( mOgre[ i ].mNpcID );
		tpOgre->set_number( mOgre[ i ].mNumber );
		tpOgre->set_percent( mOgre[ i ].mPrecent );
	}
	return ;
}

void CSkillTreeNode::SetFromPB( bool bServer,PBSkillTreeNode *pNode)
{
	if ( pNode == NULL )
	{
		return ;
	}
	mSkillID	= pNode->skillid();
	mMaxLevel	= pNode->maxlevel();
	mNeedSkillID = pNode->needskillid();
	mNeedSkillPoint = pNode->needskillpoint();
	mNeedGenerID	= pNode->needgenerid();
	mNeedGenerPoint	= pNode->needgenerpoint();
	mLevel			= pNode->level();
	mNeedBook		= pNode->needbook();
	return ;
}

void CSkillTreeNode::CreateTlvPB( bool bServer,PBSkillTreeNode* pNode )
{
	if ( pNode == NULL )
	{
		return ;
	}
	pNode->set_skillid( mSkillID );
	pNode->set_maxlevel( mMaxLevel );
	pNode->set_needskillid( mNeedSkillID );
	pNode->set_needskillpoint( mNeedSkillPoint );
	pNode->set_needgenerid( mNeedGenerID );
	pNode->set_needgenerpoint( mNeedGenerPoint );
	pNode->set_level( mLevel );
	pNode->set_needbook( mNeedBook );
	return ;
}


int CTemplateSkillTree::Initialize()
{
	memset( mGenerName, 0, sizeof(mGenerName));		
	mGenerID = 0;		
	memset( mLayerNode, 0, sizeof(mLayerNode));
	return 0;
}

void CTemplateSkillTree::SetFromPB( bool bServer,PBSkillTree *pSkillTree)
{
	if ( pSkillTree == NULL )
	{
		return ;
	}
	mTempID			= pSkillTree->tempid();
	strncpy( mGenerName, pSkillTree->genername().c_str(), sizeof( mGenerName ) - 1 );
	mGenerID		= pSkillTree->generid();
	
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mLayerNode ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mLayerNode ); j ++ )
		{
			PBSkillTreeNode* pNode = pSkillTree->mutable_skillnode( (i*ARRAY_COL( mLayerNode ))+j );
			mLayerNode[ i ][ j ].SetFromPB( bServer,pNode );
		}
	}
	return ;
}



void CTemplateSkillTree::CreateTlvPB( bool bServer,PBSkillTree *pSkillTree)
{
	if ( pSkillTree == NULL )
	{
		return ;
	}
	pSkillTree->set_tempid( mTempID );
	pSkillTree->set_genername( mGenerName );
	pSkillTree->set_generid( mGenerID );
	for ( unsigned int i = 0; i < (unsigned int) 4; i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) 10; j ++ )
		{
			PBSkillTreeNode* tpNode = pSkillTree->add_skillnode();
			mLayerNode[ i ][ j ].CreateTlvPB( bServer, tpNode );
		}
	}
	return ;
}

int CTemplateSkill::Initialize()
{
	
	memset( mName, 0 ,sizeof(mName));	
	mSkillID = 0;				
	mRequireWeapon = CTemplateWeapon::WEAPON_UNLIMIT;			
	mMetierID = 0;			
	mGenerID = 0;		
	mSkillLevel = 0;		
	memset( mSkillIcon, 0, sizeof(mSkillIcon));	
	memset( mSkillDesc, 0, sizeof(mSkillDesc));		
	mFireType = FIRETYPE_NORMAL;
	mSkillType = SKILLTYPE_ATTACK;
	mTimeType = TIMETYPE_NORMAL;
	mAniTime = 0;
	mFireTime = 0;
	mCDTime = 0;
	memset( mCost, 0, sizeof(mCost));
	mSkillFuncID1 = PASSINC_NONE;
	mNeedDelay = 0;
	memset( mParameter1, 0, sizeof(mParameter1));
	mSkillFuncID2 = PASSINC_NONE;
	memset( mParameter2, 0, sizeof(mParameter2));
	mSkillFuncID3 = PASSINC_NONE;
	memset( mParameter3, 0, sizeof(mParameter3));
	mSkillFuncID4 = PASSINC_NONE;
	memset( mParameter4, 0, sizeof(mParameter4));
	mAutoRepeat = 0;	
	memset( mSkillAction, 0, sizeof(mSkillAction));
	mAutoCoolingSkill = 0;
	memset( mUseCondition, 0, sizeof(mUseCondition));
	mStudyGold = 0;
	mAutoNormalAttack = 0;
	mBindStudyGold = 0;
	mSkillListShowType = SKILLlIST_SHOWTYPE_SINGLE;
	mSkillTemplateType = SKILL_TEMPLATE_TYPE_AUREOLE;
	mStudySkillLevel = 0;
	mInsertAutoState = 0;
	mBreakAutoState = 0;
	mNeedTarget = 0;
	mNeedPet = 0;
	mActionCamp = 0;
	mDamageType = 0;
	mAttackFix = 0;			
	mAttackPer = 0;  	
	mResType = 0;
	mResValue = 0;
	memset( mSkillTypeDesc, 0, sizeof(mSkillTypeDesc));
	mIsMagicWeaponSkill = 0;
	mIsMWOffset = 0;
	memset( mCDGroup, 0, sizeof(mCDGroup));
	mCanClean = 0;
	mTargetLevel = 0;
	mSummonIndex = 0;
	return 0;

}

void CTemplateSkill::SetFromPB( bool bServer,PBSkill *pSkill)
{
	if ( pSkill == NULL )
	{
		return ;
	}
	mTempID				= pSkill->tempid();
	strncpy( mName, pSkill->name().c_str(), sizeof( mName ) - 1 );
	mSkillID			= pSkill->skillid();
	mRequireWeapon		= (CTemplateWeapon::WEAPON_TYPE)pSkill->requireweapon();
	mMetierID			= pSkill->metierid();
	mGenerID			= pSkill->generid();
	mSkillLevel			= pSkill->skilllevel();
	strncpy( mSkillIcon, pSkill->skillicon().c_str(), sizeof( mSkillIcon ) - 1 );
	strncpy( mSkillDesc, pSkill->skilldescribe().c_str(), sizeof( mSkillDesc ) - 1 );
	mFireType			= (FIRE_TYPE)pSkill->firetype();
	mSkillType			= (SKILL_TYPE)pSkill->skilltype();
	mTimeType			= (FIRE_TIME_TYPE)pSkill->timetype();
	mAniTime			= pSkill->anitime();
	mFireTime			= pSkill->firetime();
	mCDTime				= pSkill->cdtime();
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mCost ) && i < (unsigned int) pSkill->cost_size(); i ++ )
	{
		PBCost* pCost = pSkill->mutable_cost( i );
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_CNT( mCost[ i ].mCostParam ) && j < (unsigned int) pCost->costparam_size(); j ++ )
		{
			mCost[ i ].mCostParam[ j ] = pCost->costparam( j );
		}
		mCost[ i ].mCostType = pCost->costtype();
	}
	mNeedDelay			= pSkill->needdelay();
	mSkillFuncID1		= (PASSINC_TYPE)pSkill->skillfuncid1();
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mParameter1 ) && i < (unsigned int) pSkill->parameter1_size(); i ++ )
	{		
		mParameter1[ i ] = pSkill->parameter1( i );
	}
	mSkillFuncID2		= (PASSINC_TYPE)pSkill->skillfuncid2();
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mParameter2 ) && i < (unsigned int) pSkill->parameter2_size(); i ++ )
	{		
		mParameter2[ i ] = pSkill->parameter2( i );
	}
	mSkillFuncID3		= (PASSINC_TYPE)pSkill->skillfuncid3();
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mParameter3 ) && i < (unsigned int) pSkill->parameter3_size(); i ++ )
	{		
		mParameter3[ i ] = pSkill->parameter3( i );
	}
	mSkillFuncID4		= (PASSINC_TYPE)pSkill->skillfuncid4();
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mParameter4 ) && i < (unsigned int) pSkill->parameter4_size(); i ++ )
	{		
		mParameter4[ i ] = pSkill->parameter4( i );
	}
	strncpy( mSkillAction, pSkill->skillaction().c_str(), sizeof( mSkillAction ) - 1 );
	mAutoRepeat			= pSkill->autorepeat();
	mAutoNormalAttack	= pSkill->autonormalattack();
	mAutoCoolingSkill	= pSkill->autocoolingskill();
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mUseCondition ) && i < (unsigned int) pSkill->usecondition_size(); i ++ )
	{
		PBUseCondition* pCondition = pSkill->mutable_usecondition( i );
		mUseCondition[ i ].mConditionType = (SKILL_USECONDITION)pCondition->conditiontype();
		mUseCondition[ i ].mParam = pCondition->param();
	}
	mStudyGold			= pSkill->studygold();
	mSkillListShowType	= (SKILLLIST_SHOWTYPE)pSkill->skilllistshowtype();
	mSkillTemplateType	= (SKILL_TEMPLATE_TYPE)pSkill->skilltemptype();
	mStudySkillLevel	= pSkill->studyskilllevel();
	mInsertAutoState	= pSkill->insertautostate();
	mBreakAutoState		= pSkill->breakautostate();
	mNeedTarget			= pSkill->needtarget();
	mNeedPet			= pSkill->needpet();
	mActionCamp			= pSkill->actioncamp();
	mDamageType			= pSkill->damagetype();
	mBindStudyGold		= pSkill->bindstudygold();
	mAttackFix			= pSkill->attackfix();
	mAttackPer			= pSkill->attackper();
	mResType			= pSkill->restype();
	mResValue			= pSkill->resvalue();
	mIsMagicWeaponSkill = pSkill->ismwskill();
	strncpy( mSkillTypeDesc, pSkill->skilltypedesc().c_str(), sizeof( mSkillTypeDesc ) - 1 );
	mIsMWOffset			= pSkill->ismwoffset();
	for ( int i = 0; i < pSkill->cdgroup_size() && i < (int)ARRAY_CNT( mCDGroup ); i ++ )
	{
		mCDGroup[ i ] = pSkill->cdgroup( i );
	}
	mCanClean = pSkill->canclean();
	mTargetLevel = pSkill->targetlevel();
	mSummonIndex = pSkill->summonindex();
	return ;
}

void CTemplateSkill::CreateTlvPB( bool bServer,PBSkill* pSkill )
{
	if ( pSkill == NULL )
	{
		return ;
	}
	pSkill->set_tempid( mTempID );
	pSkill->set_name( mName );
	pSkill->set_skillid( mSkillID );
	pSkill->set_requireweapon( (int)mRequireWeapon );
	pSkill->set_metierid( mMetierID );
	pSkill->set_generid( mGenerID );
	pSkill->set_skilllevel( mSkillLevel );
	pSkill->set_skillicon( mSkillIcon );
	pSkill->set_skilldescribe( mSkillDesc );
	pSkill->set_firetype( mFireType );
	pSkill->set_firetime( mFireTime );
	pSkill->set_skilltype( mSkillType );
	pSkill->set_timetype( mTimeType );
	pSkill->set_anitime( mAniTime );
	pSkill->set_cdtime( mCDTime );

	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mCost ); i ++ )
	{
		PBCost* tpCost = pSkill->add_cost();
		tpCost->set_costtype( mCost[ i ].mCostType );
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_CNT( mCost[ i ].mCostParam ); j ++ )
		{
			tpCost->add_costparam( mCost[ i ].mCostParam[ j ] );
		}
	}
	pSkill->set_needdelay( mNeedDelay );
	pSkill->set_skillfuncid1(mSkillFuncID1);
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mParameter1 ); i ++ )
	{		
		pSkill->add_parameter1( mParameter1[ i ] );
	}
	pSkill->set_skillfuncid2(mSkillFuncID2);
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mParameter2 ); i ++ )
	{		
		pSkill->add_parameter2( mParameter2[ i ] );
	}
	pSkill->set_skillfuncid3(mSkillFuncID3);
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mParameter3 ); i ++ )
	{		
		pSkill->add_parameter3( mParameter3[ i ] );
	}
	pSkill->set_skillfuncid4(mSkillFuncID4);
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mParameter4 ); i ++ )
	{		
		pSkill->add_parameter4( mParameter4[ i ] );
	}
	pSkill->set_skillaction( mSkillAction );
	pSkill->set_autorepeat(mAutoRepeat);
	pSkill->set_autonormalattack(mAutoNormalAttack);
	pSkill->set_autocoolingskill(mAutoCoolingSkill);
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mUseCondition ); i ++ )
	{
		PBUseCondition* tpCondition = pSkill->add_usecondition();
		tpCondition->set_conditiontype( mUseCondition[ i ].mConditionType );
		tpCondition->set_param( mUseCondition[ i ].mParam );
	}
	pSkill->set_studygold(mStudyGold);
	pSkill->set_skilllistshowtype(mSkillListShowType);
	pSkill->set_skilltemptype(mSkillTemplateType);
	pSkill->set_studyskilllevel(mStudySkillLevel);
	pSkill->set_insertautostate( mInsertAutoState );
	pSkill->set_breakautostate( mBreakAutoState );
	pSkill->set_needtarget( mNeedTarget );
	pSkill->set_needpet( mNeedPet );
	pSkill->set_actioncamp( mActionCamp );
	pSkill->set_damagetype( mDamageType );
	pSkill->set_bindstudygold( mBindStudyGold );
	pSkill->set_attackfix( mAttackFix );
	pSkill->set_attackper( mAttackPer  );
	pSkill->set_restype( mResType );
	pSkill->set_resvalue( mResValue );
	pSkill->set_ismwskill( mIsMagicWeaponSkill );
	pSkill->set_skilltypedesc( mSkillTypeDesc );
	pSkill->set_ismwoffset( mIsMWOffset );
	for ( int i = 0; i < (int)ARRAY_CNT( mCDGroup ); i ++ )
	{
		pSkill->add_cdgroup( mCDGroup[ i ] );
	}
	pSkill->set_canclean( mCanClean );
	pSkill->set_targetlevel( mTargetLevel );
	pSkill->set_summonindex( mSummonIndex );
	return ;
}


int CTemplateAureoleSkill::Initialize()
{
	mIncessary = emTRUE;;
	mIncessaryTime = 0;
	memset( mCost, 0, sizeof(mCost));
	mAureoleDistance = 0;
	memset( mAureoleType, 0, sizeof(mAureoleType));
	return 0;
}

void CTemplateAureoleSkill::SetFromPB( bool bServer,PBAureoleSkill *pAureoleSkill)
{
	if ( pAureoleSkill == NULL )
	{
		return ;
	}
	PBSkill* pSkill = pAureoleSkill->mutable_skillinfo();
	CTemplateSkill::SetFromPB( bServer,pSkill );
	mIncessary			= (AUREOLE_BOOL)pAureoleSkill->inceessary();
	mIncessaryTime		= pAureoleSkill->inceessarytime();
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mCost ); i ++ )
	{
		PBCost* pCost = pAureoleSkill->mutable_cost( i );
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_CNT( mCost[ i ].mCostParam ) && j < (unsigned int) pCost->costparam_size(); j ++ )
		{
			mCost[ i ].mCostParam[ j ] = pCost->costparam( j );
		}
		mCost[ i ].mCostType = pCost->costtype();
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mAureoleType ) && i < (unsigned int) pAureoleSkill->aureoletype_size(); i ++ )
	{
		PBAureoleType* pType = pAureoleSkill->mutable_aureoletype( i );
		mAureoleType[ i ].mBuffID = pType->buffid();
		mAureoleType[ i ].mAureoleArea = (AUREOLE_AREA)pType->aureolearea();
	}
	return ;
}

void CTemplateAureoleSkill::CreateTlvPB( bool bServer,PBAureoleSkill* pAureoleSkill )
{
	if ( pAureoleSkill == NULL )
	{
		return ;
	}
	PBSkill* pSkill	= pAureoleSkill->mutable_skillinfo();
	CTemplateSkill::CreateTlvPB( bServer,pSkill );
	pAureoleSkill->set_inceessary( mIncessary );
	pAureoleSkill->set_inceessarytime( mIncessaryTime );
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mCost ); i ++ )
	{
		PBCost* tpCost = pAureoleSkill->add_cost();
		tpCost->set_costtype( mCost[ i ].mCostType );
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_CNT( mCost[ i ].mCostParam ); j ++ )
		{
			tpCost->add_costparam( mCost[ i ].mCostParam[ j ] );
		}
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mAureoleType ); i ++ )
	{
		PBAureoleType* tpType = pAureoleSkill->add_aureoletype();
		tpType->set_buffid( mAureoleType[ i ].mBuffID );
		tpType->set_aureolearea( mAureoleType[ i ].mAureoleArea );
	}
	return ;
}

int CTemplateNormalSkill::Initialize()
{
	mSkillHitrate = 0;				
	mMinDistance = 0;
	mMaxDistance = 0;		
	mEffectMaxNumber = 0;
	mAttackRange = RANGE_SINGLE;	
	memset( mRandParame, 0, sizeof(mRandParame));	
	memset( mBuffGroupID, 0, sizeof(mBuffGroupID));	
	mContinuedRule = 0;
	mContinuedInterval = 0;
	mContinuedNum = 0;
	mIgnoreArmor = 0;
	return 0;
}

void CTemplateNormalSkill::SetFromPB( bool bServer,PBNormalSkill* pNormalSkill )
{
	if ( pNormalSkill == NULL )
	{
		return ;
	}
	
	PBSkill* pSkill = pNormalSkill->mutable_skillinfo();
	CTemplateSkill::SetFromPB( bServer,pSkill );
	mSkillHitrate			= pNormalSkill->skillhitrate();	
	mMinDistance			= pNormalSkill->mindistance();
	mMaxDistance			= pNormalSkill->maxdistance();
	mEffectMaxNumber		= pNormalSkill->efktmaxnum();
	mAttackRange			= (RANGE_TYPE)pNormalSkill->attackrange();
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) pNormalSkill->randparam_size() && i < (unsigned int) ARRAY_CNT( mRandParame ); i ++ )
	{
		mRandParame[ i ] = pNormalSkill->randparam( i );
	}
	for ( i = 0; i < (unsigned int) pNormalSkill->buffgroupid_size() && i < (unsigned int) ARRAY_CNT( mBuffGroupID ); i ++ )
	{
		mBuffGroupID[ i ] = pNormalSkill->buffgroupid( i );
	}
	
	mContinuedRule		= pNormalSkill->continuedrule();
	mContinuedInterval	= pNormalSkill->continuedtime();
	mContinuedNum		= pNormalSkill->continuednum();
	mIgnoreArmor		= pNormalSkill->ignorearmor();	
	return ;
}

void CTemplateNormalSkill::CreateTlvPB( bool bServer,PBNormalSkill* pNormalSkill )
{
	if ( pNormalSkill ==  NULL )
	{
		return ;
	}
	PBSkill* pSkill = pNormalSkill->mutable_skillinfo();
	CTemplateSkill::CreateTlvPB( bServer,pSkill );
	pNormalSkill->set_skillhitrate( mSkillHitrate );	
	pNormalSkill->set_mindistance( mMinDistance );
	pNormalSkill->set_maxdistance( mMaxDistance );
	pNormalSkill->set_efktmaxnum( mEffectMaxNumber );
	pNormalSkill->set_attackrange( mAttackRange );
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mRandParame ); i ++ )
	{
		pNormalSkill->add_randparam( mRandParame[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mBuffGroupID ); i ++ )
	{
		pNormalSkill->add_buffgroupid( mBuffGroupID[ i ] );
	}
	
	pNormalSkill->set_continuedrule(mContinuedRule);
	pNormalSkill->set_continuedtime(mContinuedInterval);
	pNormalSkill->set_continuednum(mContinuedNum);
	pNormalSkill->set_ignorearmor( mIgnoreArmor );	
	return ;
}

int CTemplateTrapSkill::Initialize()
{	
	mLastTime = 0;
	mApplyType = APPLYTYPE_SINGLE;
	mTouchRange = 0;
	mApplyRange = 0;
	memset( mTrapRes, 0, sizeof(mTrapRes));
	memset( mBuffGroupID, 0, sizeof(mBuffGroupID));	
	memset( mCondition, 0, sizeof(mCondition));
	mEffectMaxNumber = 0;
	return 0;
}

void CTemplateTrapSkill::SetFromPB( bool bServer,PBTrapSkill *pTrapSkill)
{
	if ( pTrapSkill == NULL )
	{
		return ;
	}

	PBSkill* pSkill = pTrapSkill->mutable_skillinfo();
	CTemplateSkill::SetFromPB( bServer,pSkill );
	mLastTime			= pTrapSkill->lasttime();
	mApplyType			= (CTemplateTrapSkill::APPLY_TYPE)pTrapSkill->applytype();
	mTouchRange			= pTrapSkill->touchrange();
	mApplyRange			= pTrapSkill->applyrange();
	strncpy( mTrapRes, pTrapSkill->trapres().c_str(), sizeof( mTrapRes ) - 1 );
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) pTrapSkill->buffgroupid_size() && i < (unsigned int) ARRAY_CNT( mBuffGroupID ); i ++ )
	{
		mBuffGroupID[ i ] = pTrapSkill->buffgroupid( i );
	}
	for ( i = 0; i < (unsigned int) pTrapSkill->condition_size() && i < (unsigned int) ARRAY_CNT( mCondition ); i ++ )
	{
		mCondition[ i ] = pTrapSkill->condition( i );
	}
	mEffectMaxNumber = pTrapSkill->effectmaxnumber();
	return; 
}

void CTemplateTrapSkill::CreateTlvPB( bool bServer,PBTrapSkill* pTrapSkill )
{
	if ( pTrapSkill == NULL )
	{
		return ;
	}
	PBSkill* pSkill = pTrapSkill->mutable_skillinfo();
	CTemplateSkill::CreateTlvPB( bServer,pSkill );
	pTrapSkill->set_lasttime( mLastTime );
	pTrapSkill->set_applytype( mApplyType );
	pTrapSkill->set_touchrange( mTouchRange );
	pTrapSkill->set_applyrange( mApplyRange );
	pTrapSkill->set_trapres( mTrapRes );
	unsigned int i = 0; 
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mBuffGroupID ); i ++ )
	{
		pTrapSkill->add_buffgroupid( mBuffGroupID[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mCondition ); i ++ )
	{
		pTrapSkill->add_condition( mCondition[ i ] );
	}
	pTrapSkill->set_effectmaxnumber( mEffectMaxNumber );
	return ;
}


int CTemplateBuff::Initialize()
{
	mBuffSeriesID = 0;
	mIdenValue = 0;
	mBuffStyle = BUFF_COMM; 
	mBuffFirstActionTime = 0;
	mBuffActionTime = 0;
	memset( mBuff, 0, sizeof(mBuff) );
	memset( mBuffName, 0, sizeof(mBuffName));
	memset( mBuffIcon, 0, sizeof(mBuffIcon));
	memset( mBuffDesc, 0, sizeof(mBuffDesc));
	mHitrate = 0;
	mBuffValue = 0;
	mResistType = 0;
	mLastTime = 0;
	mBuffType = BUFF_INC;
	mCancelBuff = 0;
	mEnableBattle = 0;
	memset( mBuffAni, 0, sizeof(mBuffAni));
	mBuffRow = 0;
	mBuffDieDel = 0;
	mTimerType = 0;
	mBuffClear = 0;
	mBuffMapID = 0;
	mBuffLineID = 0;
	mSkillLevel = 0;
	mCountryBuff = 0;
	mBuffMapType = NON_BUFF;
	mBuffSuperposition = NONE_SUPERPOSITION;	
	mAccurate = 0;
	mOffRemove = 0;
	mTransform = 0;
	mRemoveCountID = 0;
	return 0;
}

void CTemplateBuff::SetFromPB( bool bServer,PBBuff *pBuff)
{
	if ( pBuff == NULL )
	{
		return ;
	}
	mTempID					= pBuff->tempid();
	mBuffSeriesID			= pBuff->buffserviceid();
	mIdenValue				= pBuff->idenvalue();
	mBuffStyle				= (BUFF_STYLE)pBuff->buffstyle();
	mBuffFirstActionTime	= pBuff->firstactivetime();
	mBuffActionTime			= pBuff->activetime();
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) pBuff->bufffunc_size() && i < (unsigned int) ARRAY_CNT( mBuff ); i ++ )
	{
		PBBuffFunc* pFunc = pBuff->mutable_bufffunc( i );
		mBuff[ i ].mBuffFunc = (BUFF_FUN_TYPE)pFunc->functype();
		for ( unsigned int j = 0; j < (unsigned int) pFunc->parame_size() && j < (unsigned int) ARRAY_CNT( mBuff[ i ].mBuffParam ); j ++ )
		{
			mBuff[ i ].mBuffParam[ j ] = pFunc->parame( j );
		}
	}
	strncpy( mBuffName, pBuff->buffname().c_str(), sizeof( mBuffName ) - 1 );
	strncpy( mBuffIcon, pBuff->bufficon().c_str(), sizeof( mBuffIcon ) - 1 );
	strncpy( mBuffDesc, pBuff->buffdescribe().c_str(), sizeof( mBuffDesc ) - 1 );
	mHitrate				= pBuff->hitrate();
	mBuffValue				= pBuff->buffvalue();
	mResistType				= pBuff->resisttype();
	mLastTime				= pBuff->lasttime();
	mBuffType				= (BUFF_TYPE)pBuff->bufftype();
	mCancelBuff				= pBuff->cancelbuff();
	mEnableBattle			= pBuff->enablebattle();
	strncpy( mBuffAni, pBuff->buffani().c_str(), sizeof( mBuffAni ) - 1 );
	mBuffRow				= pBuff->buffrow();
	mBuffDieDel				= pBuff->buffdiedel();
	mTimerType				= pBuff->timertype();
	mBuffClear				= pBuff->buffclear();
	mBuffMapID				= pBuff->buffmapid();
	mBuffLineID				= pBuff->bufflineid();
	mSkillLevel				= pBuff->skilllevel();
	mBuffMapType			= ( BUFF_MAP_TYPE )pBuff->buffmaptype( );
	mCountryBuff			= pBuff->countrybuff( );
	mBuffSuperposition		= ( CTemplateBuff::BUFF_SUPERPOSITION_TYPE )pBuff->buffsuperposition( );
	mAccurate				= pBuff->accurate();
	mOffRemove				= pBuff->offremove();
	mTransform				= pBuff->transform();
	mRemoveCountID			= pBuff->removecountid();
	return ;
}

void CTemplateBuff::CreateTlvPB( bool bServer,PBBuff* pBuff )
{
	if ( pBuff == NULL )
	{
		return ;
	}
	pBuff->set_tempid(mTempID);
	pBuff->set_buffserviceid(mBuffSeriesID);
	pBuff->set_idenvalue(mIdenValue);
	pBuff->set_buffstyle(mBuffStyle);
	pBuff->set_firstactivetime(mBuffFirstActionTime);
	pBuff->set_activetime(mBuffActionTime);
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mBuff ); i ++ )
	{
		PBBuffFunc* tpFunc = pBuff->add_bufffunc();
		tpFunc->set_functype( mBuff[ i ].mBuffFunc );
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_CNT( mBuff[ i ].mBuffParam ); j ++ )
		{
			tpFunc->add_parame( mBuff[ i ].mBuffParam[ j ] );
		}
	}
	pBuff->set_lasttime( mLastTime );
	pBuff->set_bufftype( (int)mBuffType );
	pBuff->set_buffname( mBuffName );
	pBuff->set_bufficon( mBuffIcon );
	pBuff->set_buffdescribe( mBuffDesc );
	pBuff->set_hitrate(mHitrate);
	pBuff->set_buffvalue(mBuffValue);
	pBuff->set_resisttype(mResistType);
	pBuff->set_cancelbuff(mCancelBuff);
	pBuff->set_enablebattle(mEnableBattle);
	pBuff->set_buffani( mBuffAni );
	pBuff->set_buffrow(mBuffRow);
	pBuff->set_buffdiedel(mBuffDieDel);
	pBuff->set_timertype(mTimerType);
	pBuff->set_buffclear(mBuffClear);
	pBuff->set_buffmapid(mBuffMapID);
	pBuff->set_bufflineid(mBuffLineID);
	pBuff->set_skilllevel(mSkillLevel);
	pBuff->set_buffmaptype( mBuffMapType );
	pBuff->set_countrybuff( mCountryBuff );
	pBuff->set_buffsuperposition( mBuffSuperposition );
	pBuff->set_accurate( mAccurate );
	pBuff->set_offremove( mOffRemove );
	pBuff->set_transform( mTransform );
	pBuff->set_removecountid( mRemoveCountID );
	return ;
}

int CTemplateLevelExp::Initialize()
{
	memset( mLevelExp, 0, sizeof(mLevelExp));
	return 0;
}

void CTemplateLevelExp::SetFromPB( bool bServer,PBLevelExp *pLevelExp)
{
	if ( pLevelExp == NULL )
	{
		return ;
	}
	mTempID			= pLevelExp->tempid();
	for ( unsigned int i = 0; i < (unsigned int) pLevelExp->levelexp_size() && i < (unsigned int) ARRAY_CNT( mLevelExp ); i ++ )
	{
		mLevelExp[ i ] = pLevelExp->levelexp( i );
	}
	return ;
}

void CTemplateLevelExp::CreateTlvPB( bool bServer,PBLevelExp* pLevelExp )
{
	if ( pLevelExp == NULL )
	{
		return ;
	}
	pLevelExp->set_tempid( mTempID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mLevelExp ); i ++ )
	{
		pLevelExp->add_levelexp( mLevelExp[ i ] );
	}
	return ;
}

int CTemplateLevelMoneyLmt::Initialize()
{
	memset( mLmtTable, 0, sizeof(mLmtTable));
	return 0;
}

void CTemplateLevelMoneyLmt::SetFromPB( bool bServer,PBLevelMoneyLmt *pLevelMoney)
{
	if ( pLevelMoney == NULL )
	{
		return ;
	}
	mTempID	= pLevelMoney->tempid();
	for ( unsigned int i = 0; i < (unsigned int) pLevelMoney->lmttable_size() && i < (unsigned int) ARRAY_CNT( mLmtTable ); i ++ )
	{
		PBSingleMoneyLmt tTable = pLevelMoney->lmttable(i);
		mLmtTable[ i ].mBagMoneyLmt = tTable.bagmoneylmt();
		mLmtTable[ i ].mBagBindMoneyLmt = tTable.bagbindmoneylmt();
		mLmtTable[ i ].mStorageMoneyLmt = tTable.storagemoneylmt();
		mLmtTable[ i ].mStorageBindMoneyLmt = tTable.storagebindmoneylmt();
	}
	return ;
}

void CTemplateLevelMoneyLmt::CreateTlvPB( bool bServer,PBLevelMoneyLmt* pLevelMoney )
{
	if ( pLevelMoney == NULL )
	{
		return ;
	}
	pLevelMoney->set_tempid( mTempID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mLmtTable ); i ++ )
	{
		PBSingleMoneyLmt* tpTable = pLevelMoney->add_lmttable();
		tpTable->set_bagmoneylmt(mLmtTable[ i ].mBagMoneyLmt );
		tpTable->set_bagbindmoneylmt(mLmtTable[ i ].mBagBindMoneyLmt);
		tpTable->set_storagemoneylmt(mLmtTable[ i ].mStorageMoneyLmt);
		tpTable->set_storagebindmoneylmt(mLmtTable[ i ].mStorageBindMoneyLmt);
	}
	return ;
}

int CTemplatePetLevelExp::Initialize()
{
	memset( mLevelExp, 0, sizeof(mLevelExp) );
	return 0;
}

void CTemplatePetLevelExp::SetFromPB( bool bServer,PBPetLevelExp *pLevelExp)
{
	if ( pLevelExp == NULL )
	{
		return ;
	}
	mTempID				= pLevelExp->tempid();
	for ( unsigned int i = 0; i < (unsigned int) pLevelExp->levelexp_size() && i < (unsigned int) ARRAY_CNT( mLevelExp ); i ++ )
	{
		mLevelExp[ i ]	= pLevelExp->levelexp( i );
	}
	return ;
}

void CTemplatePetLevelExp::CreateTlvPB( bool bServer,PBPetLevelExp* pLevelExp )
{
	if ( pLevelExp == NULL )
	{
		return ;
	}
	pLevelExp->set_tempid( mTempID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mLevelExp );i ++ )
	{
		pLevelExp->add_levelexp( mLevelExp[ i ] );
	}
	return ;
}

int CTemplateProExp::Initialize()
{
	memset( mLevelExp, 0, sizeof(mLevelExp));
	return 0;
}

void CTemplateProExp::SetFromPB( bool bServer,PBProduceExp *pExp)
{
	if ( pExp == NULL )
	{
		return ;
	}
	mTempID				= pExp->tempid();
	for ( unsigned int i = 0; i < (unsigned int) pExp->levelexp_size() && i < (unsigned int) ARRAY_CNT( mLevelExp ); i ++ )
	{
		mLevelExp[ i ] = pExp->levelexp( i );
	}
	return ;
}

void CTemplateProExp::CreateTlvPB( bool bServer,PBProduceExp* pExp )
{
	if ( pExp == NULL )
	{
		return ;
	}
	pExp->set_tempid( mTempID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mLevelExp ); i ++ )
	{
		pExp->add_levelexp( mLevelExp[ i ] );
	}
	return ;
}

int CTemplateEquipUpg::Initialize()
{
	memset( mEquipUpg, 0, sizeof(mEquipUpg));
	return 0;
}

void CTemplateEquipUpg::SetFromPB( bool bServer,PBEquipUpg *pEquip)
{
	if ( pEquip == NULL )
	{
		return ;
	}
	mTempID				= pEquip->tempid();
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mEquipUpg ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mEquipUpg ) ); j ++ )
		{
			PBUpg* pUpg = pEquip->mutable_equipupg( (ARRAY_COL( mEquipUpg )*i) + j );
			mEquipUpg[ i ][ j ].mSuccess = pUpg->success();
			mEquipUpg[ i ][ j ].mFail = pUpg->failure();
			mEquipUpg[ i ][ j ].mIncLevel = pUpg->inclevel();
			mEquipUpg[ i ][ j ].mResult	= pUpg->result();
			mEquipUpg[ i ][ j ].mExtSuccess	= pUpg->extsuccess();
		}
	}
	return; 
}

void CTemplateEquipUpg::CreateTlvPB( bool bServer,PBEquipUpg* pEquip )
{
	if ( pEquip == NULL )
	{
		return ;
	}
	pEquip->set_tempid( mTempID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mEquipUpg ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mEquipUpg ) ); j ++ )
		{
			PBUpg* tpUpg = pEquip->add_equipupg( );
			tpUpg->set_success( (int)(mEquipUpg[ i ][ j ].mSuccess) );
			tpUpg->set_failure( (int)(mEquipUpg[ i ][ j ].mFail) );
			tpUpg->set_inclevel( mEquipUpg[ i ][ j ].mIncLevel );
			tpUpg->set_result( mEquipUpg[ i ][ j ].mResult );
			tpUpg->set_extsuccess( mEquipUpg[ i ][ j ].mExtSuccess );
		}
	}
	return ;
}

int CTemplateSuitProducer::Initialize()
{
	memset( mPropertyTempID, 0, sizeof(mPropertyTempID));
	memset( mUpgTempID, 0, sizeof(mUpgTempID));
	memset( mExtTempID, 0, sizeof(mExtTempID));
	memset( mDesText, 0, sizeof(mDesText));
	return 0;
}

void CTemplateSuitProducer::SetFromPB( bool bServer,PBSuitProducer *pSuit)
{
	if ( pSuit == NULL )
	{
		return;
	}
	mTempID				= pSuit->tempid();
	for ( unsigned int i = 0; i < (unsigned int) pSuit->propertytempid_size() && i < (unsigned int) ARRAY_CNT( mPropertyTempID ); i ++ )
	{
		mPropertyTempID[ i ] = pSuit->propertytempid( i );
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mUpgTempID ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mUpgTempID ) ); j ++ )
		{
			int tIndex = (ARRAY_COL( mUpgTempID )*i) + j;
			if ( tIndex < pSuit->upgtempid_size() )
			{
				mUpgTempID[ i ][ j ] = pSuit->upgtempid( tIndex );
			}			
		}
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mExtTempID ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mExtTempID ); j ++ )
		{
			int tTotal = (ARRAY_COL( mExtTempID )*i) + j;
			if ( tTotal >= pSuit->exttempid_size() )
			{
				break;
			}
			mExtTempID[ i ][ j ] = pSuit->exttempid( tTotal );
		}
	}
	for ( unsigned int i = 0; i < (unsigned int) pSuit->describetext_size() && i < (unsigned int) ARRAY_ROW( mDesText ); i ++ )
	{
		strncpy( mDesText[ i ], pSuit->describetext( i ).c_str(), sizeof( mDesText[ i ] ) - 1 );
	}
	return ;
}

void CTemplateSuitProducer::CreateTlvPB( bool bServer,PBSuitProducer* pSuit )
{
	if ( pSuit == NULL )
	{
		return ;
	}
	pSuit->set_tempid( mTempID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mPropertyTempID ); i++ )
	{
		pSuit->add_propertytempid( mPropertyTempID[ i ] );
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mUpgTempID ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mUpgTempID ) ); j ++ )
		{
			pSuit->add_upgtempid( mUpgTempID[ i ][ j ] );
		}
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mExtTempID ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mExtTempID ); j ++ )
		{
			pSuit->add_exttempid( mExtTempID[ i ][ j ] );
		}
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mDesText ); i ++ )
	{
		pSuit->add_describetext( mDesText[ i ] );
	}
	return ;
}

int CTemplateSuitNormal::Initialize()
{
	memset( mName, 0, sizeof(mName));
	memset( mEquipTempID, 0, sizeof(mEquipTempID));
	memset( mActiveTitle, 0, sizeof(mActiveTitle));
	memset( mPropertyTempID, 0, sizeof(mPropertyTempID));
	return 0;
}

void CTemplateSuitNormal::SetFromPB( bool bServer,PBSuitNormal *pSuit)
{
	if ( pSuit == NULL )
	{
		return; 
	}
	mTempID				= pSuit->tempid();
	strncpy( mName, pSuit->name().c_str(), sizeof( mName ) - 1 );
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) pSuit->equiptempid_size() && i < (unsigned int) ARRAY_CNT( mEquipTempID ); i ++ )
	{
		mEquipTempID[ i ] = pSuit->equiptempid( i );
	}
	strncpy( mActiveTitle, pSuit->activetitle().c_str(), sizeof( mActiveTitle ) - 1 );
	for ( i = 0; i < (unsigned int) pSuit->propertytempid_size() && i < (unsigned int) ARRAY_CNT( mPropertyTempID ); i ++ )
	{
		mPropertyTempID[ i ] = pSuit->propertytempid( i );
	}
	return ;
}

void CTemplateSuitNormal::CreateTlvPB( bool bServer,PBSuitNormal* pSuit )
{
	if ( pSuit == NULL )
	{
		return ;
	}
	pSuit->set_tempid( mTempID );
	pSuit->set_name( mName );
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mEquipTempID ); i ++ )
	{
		pSuit->add_equiptempid( mEquipTempID[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mPropertyTempID ); i ++ )
	{
		pSuit->add_propertytempid( mPropertyTempID[ i ] );
	}
	pSuit->set_activetitle( mActiveTitle );
	return ;
}

int CTemplateSpell::Initialize()
{
	memset( mName, 0, sizeof(mName));
	mSuitID = 0;
	memset( mLetterID, 0, sizeof(mLetterID));
	memset( mRequire, 0, sizeof(mRequire));
	memset( mPropertyTempID, 0, sizeof(mPropertyTempID));
	memset( mDesText, 0, sizeof(mDesText));
	return 0;
}

void CTemplateSpell::SetFromPB( bool bServer,PBSpell *pSpell)
{
	if ( pSpell == NULL )
	{
		return ;
	}
	mTempID			= pSpell->tempid();
	strncpy( mName, pSpell->name().c_str(), sizeof( mName ) - 1 );
	mSuitID			= pSpell->suitid();
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) pSpell->letterid_size() && i < (unsigned int) ARRAY_CNT( mLetterID ); i ++ )
	{
		mLetterID[ i ] = pSpell->letterid( i );
	}
	for ( i = 0; i < (unsigned int) pSpell->require_size() && i < (unsigned int) ARRAY_CNT( mRequire ); i ++ )
	{
		mRequire[ i ] = pSpell->require( i );
	}
	int tTotal = 0;
	for ( i = 0; i < (unsigned int) ARRAY_ROW( mPropertyTempID ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mPropertyTempID ); j ++ )
		{
			tTotal = (ARRAY_COL( mPropertyTempID )*i) + j;
			if ( tTotal >= pSpell->propertytempid_size() )
			{
				break;
			}
			mPropertyTempID[ i ][ j ] = pSpell->propertytempid( tTotal );
		}
	}
	for ( i = 0; i < (unsigned int) pSpell->describetext_size() && i < (unsigned int) ARRAY_ROW( mDesText ); i ++ )
	{
		strncpy( mDesText[ i ], pSpell->describetext( i ).c_str(), sizeof( mDesText[ i ] ) - 1 );
	}
	return ;
}

void CTemplateSpell::CreateTlvPB( bool bServer,PBSpell* pSpell )
{
	if ( pSpell == NULL )
	{
		return ;
	}
	pSpell->set_tempid( mTempID );
	pSpell->set_name( mName );
	pSpell->set_suitid( mSuitID );
	unsigned int i = 0; 
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mLetterID ); i ++ )
	{
		pSpell->add_letterid( mLetterID[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mRequire ); i ++ )
	{
		pSpell->add_require( mRequire[ i ] );
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mPropertyTempID ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mPropertyTempID ); j ++ )
		{
			pSpell->add_propertytempid( mPropertyTempID[ i ][ j ] );
		}
	}
	for ( i = 0; i < (unsigned int) ARRAY_ROW( mDesText ); i ++ )
	{
		pSpell->add_describetext( mDesText[ i ] );
	}
	return ;
}

int CTemplateProEquip::Initialize()
{
	mType = 0;
	mDetailID1 = 0;
	mDetailID2 = 0;
	mLevel = 0;
	memset( mName, 0, sizeof(mName));
	mMaterialID = 0;
	mMaterialNum = 0;
	mMaterialRank = 0;
	mItemID1 = 0 ;
	mItemRan1 = 0;
	mItemID2 = 0;
	mItemRan2 = 0;
	mItemID3 = 0;
	mItemRan3 = 0;
	mMoney = 0;
	mBindMoney = 0; 
	mIconID = 0;
	memset( mDesc, 0, sizeof(mDesc));
	mExp = 0;
	return 0;
}

void CTemplateProEquip::SetFromPB( bool bServer,PBProduceEquip *pEquip)
{
	if ( pEquip == NULL )
	{
		return ;
	}
	mTempID			= pEquip->tempid();
	mType			= pEquip->type();
	mDetailID1		= pEquip->detailid1();
	mDetailID2		= pEquip->detailid2();	
	mLevel			= pEquip->level();
	strncpy( mName, pEquip->name().c_str(), sizeof( mName ) - 1 );
	mMaterialID		= pEquip->materialid();
	mMaterialNum	= pEquip->materialnum();
	mMaterialRank	= pEquip->materialrank();
	mItemID1		= pEquip->itemid1();
	mItemRan1		= pEquip->itemrand1();
	mItemID2		= pEquip->itemid2();
	mItemRan2		= pEquip->itemrand2();
	mItemID3		= pEquip->itemid3();
	mItemRan3		= pEquip->itemrand3();
	mMoney			= pEquip->money();
	mBindMoney		= pEquip->bindmoney();
	mIconID			= pEquip->iconid();
	strncpy( mDesc, pEquip->describe().c_str(), sizeof( mDesc ) );
	mExp			= pEquip->exp();
	return ;
}

void CTemplateProEquip::CreateTlvPB( bool bServer,PBProduceEquip *pEquip)
{
	if ( pEquip == NULL )
	{
		return ;
	}
	pEquip->set_tempid( mTempID );
	pEquip->set_type( mType );
	pEquip->set_detailid1(mDetailID1);
	pEquip->set_detailid2(mDetailID2);
	pEquip->set_level(mLevel);
	pEquip->set_name( mName );
	pEquip->set_materialid(mMaterialID);
	pEquip->set_materialnum(mMaterialNum);
	pEquip->set_materialrank(mMaterialRank);
	pEquip->set_itemid1(mItemID1);
	pEquip->set_itemrand1(mItemRan1);
	pEquip->set_itemid2(mItemID2);
	pEquip->set_itemrand2(mItemRan2);
	pEquip->set_itemid3(mItemID3);
	pEquip->set_itemrand3(mItemRan3);
	pEquip->set_money(mMoney);
	pEquip->set_bindmoney( mBindMoney );
	pEquip->set_iconid(mIconID);
	pEquip->set_describe( mDesc );
	pEquip->set_exp( mExp );
	return ;
}

int CTemplateProPhysics::Initialize()
{
	mType = 0;
	mLevel = 0;
	memset( mName, 0, sizeof(mName));
	mMaterialID = 0;
	mMaterialNum = 0;
	mItemID = 0;
	mExp = 0;
	mMoney = 0;
	mBindMoney = 0;
	return 0;
}

void CTemplateProPhysics::SetFromPB( bool bServer,PBProducePhysic *pPhysic)
{
	if ( pPhysic == NULL )
	{
		return ;
	}
	mTempID				= pPhysic->tempid();
	mType				= pPhysic->type();
	mLevel				= pPhysic->level();
	strncpy( mName, pPhysic->name().c_str(), sizeof( mName ) - 1 );
	mMaterialID			= pPhysic->materialid();
	mMaterialNum		= pPhysic->materialnum();
	mItemID				= pPhysic->itemid();
	mExp				= pPhysic->exp();
	mMoney				= pPhysic->money();
	mBindMoney			= pPhysic->bindmoney();
	return;
}

void CTemplateProPhysics::CreateTlvPB( bool bServer,PBProducePhysic* pPhysic )
{
	if ( pPhysic == NULL )
	{
		return ;
	}
	pPhysic->set_tempid( mTempID );
	pPhysic->set_type( mType );
	pPhysic->set_level( mLevel );
	pPhysic->set_name( mName );
	pPhysic->set_materialid( mMaterialID );
	pPhysic->set_materialnum( mMaterialNum );
	pPhysic->set_itemid( mItemID );
	pPhysic->set_exp( mExp );
	pPhysic->set_money( mMoney );
	pPhysic->set_bindmoney( mBindMoney );
	return ;
}

int CTplItemRune::Initialize()
{
	mSuitID = 0;
	memset( mWord, 0, sizeof(mWord));
	mOrderID = 0;
	mInsertCharge = 0;
	mPropertyID = 0;
	mRuneIndex = 0;
	mShowColor = 0;
	return 0;
}

void CTplItemRune::SetFromPB( bool bServer,PBItemRune *pRuneItem)
{
	if ( pRuneItem == NULL )
	{
		return ;
	}	
	PBItemBase* pItem = pRuneItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem );
	mSuitID		= pRuneItem->suitid();
	strncpy( mWord, pRuneItem->word().c_str(), sizeof( mWord ) - 1 );
	mOrderID	= pRuneItem->orderid();
	mInsertCharge = pRuneItem->insertcharge();
	mPropertyID = pRuneItem->propertyid();
	mRuneIndex = pRuneItem->runeindex();
	mShowColor = pRuneItem->showcolor();
	return ;
}

void CTplItemRune::CreateTlvPB( bool bServer,PBItemRune *pRuneItem)
{
	if ( pRuneItem == NULL )
	{
		return ;
	}
	PBItemBase* pItem = pRuneItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem );
	pRuneItem->set_suitid( mSuitID );
	pRuneItem->set_word( mWord );
	pRuneItem->set_orderid( mOrderID );
	pRuneItem->set_insertcharge( mInsertCharge );
	pRuneItem->set_propertyid( mPropertyID );
	pRuneItem->set_runeindex(mRuneIndex);
	pRuneItem->set_showcolor( mShowColor );
	return ;
}

int CTplItemJewel::Initialize()
{	
	mTypeID=0;
	mPos=0;
	mShowColor=0;
	mProperty=0;
	mUnique=0;
	memset(mJewelID, 0, sizeof(mJewelID));
	mComposeID=0;
	mInsertCharge=0;
	mRemoveCharge=0;
	mComposeCharge=0;
	mLevel=0;
	memset( mRand, 0, sizeof(mRand));
	mNewJewelID=0;
	memset( mNewRand, 0, sizeof(mNewRand));
	memset( mCarveJewelID, 0, sizeof(mCarveJewelID));
	memset( mCarveRand, 0, sizeof(mCarveRand));
	mCarveCharge=0;
	mCardID=0;
	mCanCarve=0;
	mAffectRand=0;
	mJewComID=0;
	mComLevel=0;
	return 0;
}

void CTplItemJewel::SetFromPB( bool bServer,PBItemJewel *pJewelItem)
{
	if ( pJewelItem == NULL )
	{
		return ;
	}
	PBItemBase* pItem = pJewelItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem );
	mTypeID			= pJewelItem->typeid_();
	mPos			= pJewelItem->pos();
	mShowColor		= pJewelItem->showcolor();
	mProperty		= pJewelItem->property();
	mUnique			= pJewelItem->unique();
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mJewelID ) && i < (unsigned int)pJewelItem->jewelid_size(); i ++ )
	{
		mJewelID[ i ] = pJewelItem->jewelid( i );
	}
	mComposeID		= pJewelItem->composeid();
	mInsertCharge	= pJewelItem->insertcharge();
	mRemoveCharge	= pJewelItem->removecharge();
	mComposeCharge	= pJewelItem->composecharge();
	mLevel			= pJewelItem->level();
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mRand ) && i < (unsigned int)pJewelItem->rand_size(); i ++ )
	{
		mRand[ i ] = pJewelItem->rand( i );
	}
	mNewJewelID		= pJewelItem->newjewelid();
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mNewRand ) && i < (unsigned int)pJewelItem->newrand_size(); i ++ )
	{
		mNewRand[ i ] = pJewelItem->newrand( i );
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mCarveJewelID ) && i < (unsigned int)pJewelItem->carvejewelid_size(); i ++ )
	{
		mCarveJewelID[ i ] = pJewelItem->carvejewelid( i );
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mCarveRand ) && i < (unsigned int)pJewelItem->carverand_size(); i ++ )
	{
		mCarveRand[ i ] = pJewelItem->carverand( i );
	}	
	mCarveCharge		= pJewelItem->carvecharge();
	mCardID = pJewelItem->cardid();
	mCanCarve = pJewelItem->cancarve();
	mAffectRand = pJewelItem->affectrand();
	mJewComID = pJewelItem->jewcomid();
	mComLevel = pJewelItem->comlevel();
	return ;
}

void CTplItemJewel::CreateTlvPB( bool bServer,PBItemJewel* pJewelItem )
{
	if ( pJewelItem == NULL )
	{
		return ;
	}
	PBItemBase* pItem = pJewelItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer, pItem );
	pJewelItem->set_typeid_( mTypeID );
	pJewelItem->set_pos( mPos );
	pJewelItem->set_showcolor( mShowColor );
	pJewelItem->set_property( mProperty );
	pJewelItem->set_unique( mUnique );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mJewelID ); i ++ )
	{
		pJewelItem->add_jewelid( mJewelID[ i ] );
	}
	pJewelItem->set_composeid( mComposeID );
	pJewelItem->set_insertcharge( mInsertCharge );
	pJewelItem->set_removecharge( mRemoveCharge );
	pJewelItem->set_composecharge( mComposeCharge );
	pJewelItem->set_level( mLevel );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mRand ); i ++ )
	{
		pJewelItem->add_rand( mRand[ i ] );
	}
	pJewelItem->set_newjewelid( mNewJewelID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mNewRand ); i ++ )
	{
		pJewelItem->add_newrand( mNewRand[ i ] );
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mCarveJewelID ); i ++ )
	{
		pJewelItem->add_carvejewelid( mCarveJewelID[ i ] );
	}	
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mCarveRand ); i ++ )
	{
		pJewelItem->add_carverand( mCarveRand[ i ] );
	}
	pJewelItem->set_carvecharge( mCarveCharge );
	pJewelItem->set_cardid( mCardID );
	pJewelItem->set_cancarve( mCanCarve );
	pJewelItem->set_affectrand( mAffectRand );
	pJewelItem->set_jewcomid( mJewComID );
	pJewelItem->set_comlevel( mComLevel );
	return ;
}

int CTplItemYuanBug::Initialize()
{
	mYuan = 0;
	mEggID = 0;
	mYuanType = 0;
	return 0;
}

void CTplItemYuanBug::SetFromPB( bool bServer,PBItemYuanBug *pBugItem)
{
	if ( pBugItem == NULL )
	{
		return ;
	}
	PBItemBase* pItem = pBugItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem );
	mYuan			= pBugItem->yuan();
	mEggID			= pBugItem->eggid();
	mYuanType		= pBugItem->yuantype();
	return ;
}

void CTplItemYuanBug::CreateTlvPB( bool bServer,PBItemYuanBug *pBugItem)
{
	if ( pBugItem == NULL )
	{
		return ;
	}
	PBItemBase* pItem = pBugItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem );
	pBugItem->set_yuan( mYuan );
	pBugItem->set_eggid( mEggID );
	pBugItem->set_yuantype( mYuanType );
	return ;
}

int CTplItemYuanEgg::Initialize()
{
	mLevel = 0;
	mComposeID = 0;
	mMaxEquip = 0;
	return 0;
}

void CTplItemYuanEgg::SetFromPB( bool bServer,PBItemYuanEgg *pEggItem)
{
	if ( pEggItem == NULL )
	{
		return ;
	}
	PBItemBase* pItem = pEggItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem );
	mLevel		= pEggItem->level();
	mComposeID	= pEggItem->composeid();
	mMaxEquip	= pEggItem->maxequip();
	return ;
}

void CTplItemYuanEgg::CreateTlvPB( bool bServer,PBItemYuanEgg* pEggItem )
{
	if ( pEggItem == NULL )
	{
		return ;
	}
	PBItemBase* pItem = pEggItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem );
	pEggItem->set_level( mLevel );
	pEggItem->set_composeid( mComposeID );
	pEggItem->set_maxequip( mMaxEquip );
	return ;
}

int CTplItemMagicStone::Initialize()
{
	mPart = 0;
	memset( mAttribute, 0, sizeof(mAttribute));
	mUsedTimes = 0;
	memset( mProbability, 0, sizeof(mProbability));
	mLevelLimit = 0;
	return 0;
}

// 附魔物品
void CTplItemMagicStone::SetFromPB( bool bServer, PBItemMagicStone* pMagicStone )
{
	if ( pMagicStone == NULL )
	{
		return;
	}
	CTplItem::SetFromPB( bServer, pMagicStone->mutable_baseinfo() );
	mPart = pMagicStone->part();
	
	for ( int i = 0; i < (int)ARRAY_CNT( mAttribute ) && i < pMagicStone->attribute_size(); ++i )
	{
		mAttribute[i] = pMagicStone->attribute(i);			
	}  		
	mUsedTimes = pMagicStone->usedtimes();
	for ( int i = 0; i < (int)ARRAY_CNT(mProbability) && i < pMagicStone->probability_size(); ++i )
	{
		mProbability[i] = pMagicStone->probability(i);
	}
	mLevelLimit	= pMagicStone->levellimit();
}

void CTplItemMagicStone::CreateTlvPB( bool bServer, PBItemMagicStone* pMagicStone )
{
	if ( pMagicStone == NULL )
	{
		return;
	}
	CTplItem::CreateTlvPB( bServer, pMagicStone->mutable_baseinfo() );
	pMagicStone->set_part( mPart );
	for ( int i = 0; i < (int)ARRAY_CNT( mAttribute ); ++i )
	{
		pMagicStone->add_attribute( mAttribute[i] );
	}		
	pMagicStone->set_usedtimes( mUsedTimes );
	for ( int i = 0; i < (int)ARRAY_CNT(mProbability); ++i )
	{
		pMagicStone->add_probability( mProbability[i] );
	}
	pMagicStone->set_levellimit( mLevelLimit );
}

// 生活技能配方卷轴
int CTplItemLifeSkillScroll::Initialize()
{
	mComposeID = 0;
	mComposeType = 0;
	return 0;
}

void CTplItemLifeSkillScroll::SetFromPB( bool bServer, PBLifeSkillScroll* pScroll )
{
	if ( pScroll == NULL )
	{
		return;
	}
	CTplItem::SetFromPB( bServer, pScroll->mutable_baseinfo() );
	mComposeID = pScroll->composeid();
	mComposeType = pScroll->composetype();
}

void CTplItemLifeSkillScroll::CreateTlvPB( bool bServer, PBLifeSkillScroll* pScroll )
{
	if ( pScroll == NULL )
	{						
		return;
	}
	CTplItem::CreateTlvPB( bServer, pScroll->mutable_baseinfo() );
	pScroll->set_composeid( mComposeID );
	pScroll->set_composetype( mComposeType );
}

int CTemplateOpenSlot::Initialize()
{
	memset( mSlotLevel, 0, sizeof(mSlotLevel));
	memset( mSlotNumber, 0, sizeof(mSlotNumber));
	return 0;
}

void CTemplateOpenSlot::SetFromPB( bool bServer,PBOpenSlot *pSlot)
{
	if ( pSlot == NULL )
	{
		return ;
	}
	mTempID			= pSlot->tempid();
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) pSlot->slotlevel_size() && i < (unsigned int) ARRAY_CNT( mSlotLevel ); i ++ )
	{
		mSlotLevel[ i ] = pSlot->slotlevel( i );
	}
	for ( i = 0; i < (unsigned int) pSlot->slotnumber_size() && i < (unsigned int)  ARRAY_CNT( mSlotNumber ); i ++ )
	{
		mSlotNumber[ i ] = pSlot->slotnumber( i );
	}
	return ;
}  

void CTemplateOpenSlot::CreateTlvPB( bool bServer,PBOpenSlot* pSlot )
{
	if ( pSlot == NULL )
	{
		return ;
	}
	pSlot->set_tempid( mTempID );
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSlotLevel ); i ++ )
	{
		pSlot->add_slotlevel( mSlotLevel[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSlotNumber ); i ++ )
	{
		pSlot->add_slotnumber( mSlotNumber[ i ] );
	}
	return ;
}	

int CTemplateCompose::Initialize()
{
	mMaterialID = 0;
	mMaterialNum = 0;
	mItemID = 0;
	mItemNum = 0;
	return 0;
}

void CTemplateCompose::SetFromPB( bool bServer,PBCompose *pCompose)
{
	if ( pCompose == NULL )
	{
		return ;
	}
	mTempID			= pCompose->tempid();
	mMaterialID		= pCompose->materialid();
	mMaterialNum	= pCompose->materialnum();
	mItemID			= pCompose->itemid();
	mItemNum		= pCompose->itemnum();
	return ;
}

void CTemplateCompose::CreateTlvPB( bool bServer,PBCompose* pCompose )
{
	if ( pCompose == NULL )
	{
		return ;
	}
	pCompose->set_tempid( mTempID );
	pCompose->set_materialid( mMaterialID );
	pCompose->set_materialnum( mMaterialNum );
	pCompose->set_itemid( mItemID );
	pCompose->set_itemnum( mItemNum );
	return ;
}

int CTemplateMultiCompose::Initialize()
{
	mOrder=0;
	memset( mDescribe, 0, sizeof(mDescribe));
	mComposeType=0;
	memset( mMaterialID, 0, sizeof(mMaterialID));	   	
	memset( mMaterialNum, 0, sizeof(mMaterialNum));
	memset( mItemID, 0, sizeof(mItemID));
	memset( mItemNum, 0, sizeof(mItemNum));
	mExp=0;  	 			
	memset( mItemProbability, 0, sizeof(mItemProbability));
	mLifeSkillType=0;
	mSkillKind=0;	  
	mSpecializedDegree=0; 	
	mLifeComposeType=0;
	mCreatValue=0;	
	mStudyMoney=0;		
	mSkillCreatDegree=0;
	memset( mDegreeInfo, 0, sizeof(mDegreeInfo));	
	mApNeedCost=0;	
	mUsedTimes=0;		
	mMinValue=0;	
	mSrcEquipID=0;
	mDesEquipID=0;
	mContribute=0;
	memset( mUpgItemID, 0, sizeof(mUpgItemID));
	memset( mUpgItemNum, 0, sizeof(mUpgItemNum));
	return 0;
}

void CTemplateMultiCompose::SetFromPB( bool bServer,PBMultiCompose *pCompose)
{
	if ( pCompose == NULL )
	{
		return ;
	}
	mTempID			= pCompose->tempid();
	strncpy( mDescribe, pCompose->describe().c_str(), sizeof( mDescribe ) - 1 );
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) pCompose->materialid_size() && i < (unsigned int) ARRAY_CNT( mMaterialID ); i ++ )
	{
		mMaterialID[ i ]		= pCompose->materialid(i);
	}
	for ( i = 0; i < (unsigned int) pCompose->materialnum_size() && i < (unsigned int) ARRAY_CNT( mMaterialNum ); i ++ )
	{
		mMaterialNum[ i ]	= pCompose->materialnum(i);
	}
	for ( i = 0; i < (unsigned int) pCompose->itemid_size() && i < (unsigned int) ARRAY_CNT( mItemID ); i ++ )
	{
		mItemID[ i ]			= pCompose->itemid(i);
	}
	for ( i = 0; i < (unsigned int) pCompose->itemnum_size() && i < (unsigned int) ARRAY_CNT( mItemNum ); i ++ )
	{
		mItemNum[ i ]		= pCompose->itemnum(i);
	}
	mExp = pCompose->exp();
	mOrder = pCompose->order();	
	
	mLifeSkillType		= pCompose->lifeskilltype();
	mSkillKind			= pCompose->lifeskillkind(); 
	mSpecializedDegree	= pCompose->specializeddegree(); 	
	mCreatValue			= pCompose->creatvalue();		
	mLifeComposeType	= pCompose->lifecomposetype();
	mStudyMoney			= pCompose->studymoney();		
	mSkillCreatDegree	= pCompose->skillcreatdegree();
	for ( i = 0; i < (unsigned int)pCompose->value_size() && i < (unsigned int)pCompose->probality_size() &&
		i < (unsigned int)pCompose->degreevalue_size() && i < (unsigned int)ARRAY_CNT(mDegreeInfo ); ++i )
	{	
		mDegreeInfo[i].mValue		= pCompose->value( i );
		mDegreeInfo[i].mProbality	= pCompose->probality( i );
		mDegreeInfo[i].mDegreeValue	= pCompose->degreevalue( i );
	}
	mApNeedCost		= pCompose->apneedcost();	
	mUsedTimes		= pCompose->usedtimes();	
	mMinValue		= pCompose->minvalue();
	mComposeType	= pCompose->composetype();	
	mSrcEquipID		= pCompose->srcequipid();
	mDesEquipID		= pCompose->desequipid();
	mContribute		= pCompose->contribute();
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mUpgItemID ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mUpgItemID ) ); j ++ )
		{
			int tIndex = ( i * ARRAY_COL( mUpgItemID ) + j );
			if ( tIndex < pCompose->upgitemid_size() )
			{
				mUpgItemID[ i ][ j ] = pCompose->upgitemid( tIndex );
			}			
		}
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mUpgItemNum ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mUpgItemNum ) ); j ++ )
		{
			int tIndex = ( i * ARRAY_COL( mUpgItemNum ) + j );
			if ( tIndex < pCompose->upgitemnum_size() )
			{
				mUpgItemNum[ i ][ j ] = pCompose->upgitemnum( tIndex );
			}			
		}
	}

	for ( i = 0; i < (unsigned int) pCompose->needvaluetype_size() && i < (unsigned int) ARRAY_CNT( mNeedValueType ); i ++ )
	{
		mNeedValueType[ i ]		= pCompose->needvaluetype(i);
	}
	for ( i = 0; i < (unsigned int) pCompose->needvalue_size() && i < (unsigned int) ARRAY_CNT( mNeedValue ); i ++ )
	{
		mNeedValue[ i ]	= pCompose->needvalue(i);
	}
	return ;
}

void CTemplateMultiCompose::CreateTlvPB( bool bServer,PBMultiCompose* pCompose )
{
	if ( pCompose == NULL )
	{
		return ;
	}
	pCompose->set_tempid( mTempID );
	pCompose->set_describe( mDescribe );
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mMaterialID ); i ++ )
	{
		pCompose->add_materialid( mMaterialID[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mMaterialNum ); i ++ )
	{
		pCompose->add_materialnum( mMaterialNum[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mItemID ); i ++ )
	{
		pCompose->add_itemid( mItemID[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mItemNum ); i ++ )
	{
		pCompose->add_itemnum( mItemNum[ i ] );
	}
	pCompose->set_exp( mExp );
	pCompose->set_order( mOrder );
	
	pCompose->set_lifeskilltype( mLifeSkillType );
	pCompose->set_lifeskillkind( mSkillKind ); 
	pCompose->set_lifecomposetype( mLifeComposeType );
	pCompose->set_specializeddegree( mSpecializedDegree ); 	
	pCompose->set_creatvalue( mCreatValue );		
	pCompose->set_studymoney( mStudyMoney );		
	pCompose->set_skillcreatdegree( mSkillCreatDegree );
	
	for ( i = 0; i < (unsigned int)ARRAY_CNT(mDegreeInfo ); ++i )
	{	
		pCompose->add_value( mDegreeInfo[i].mValue );
		pCompose->add_probality( mDegreeInfo[i].mProbality );
		pCompose->add_degreevalue( mDegreeInfo[i].mDegreeValue );
	}
	pCompose->set_apneedcost( mApNeedCost );	
	pCompose->set_usedtimes( mUsedTimes );	
	pCompose->set_minvalue( mMinValue );
	pCompose->set_composetype( mComposeType );	
	pCompose->set_srcequipid( mSrcEquipID );
	pCompose->set_desequipid( mDesEquipID );
	pCompose->set_contribute( mContribute ); 
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mUpgItemID ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mUpgItemID ) ); j ++ )
		{
			pCompose->add_upgitemid( mUpgItemID[ i ][ j ] );
		}
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mUpgItemNum ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mUpgItemNum ) ); j ++ )
		{
			pCompose->add_upgitemnum( mUpgItemNum[ i ][ j ] );
		}
	}

	for ( i = 0; i < (unsigned int) ARRAY_CNT( mNeedValueType ); i ++ )
	{
		pCompose->add_needvaluetype( mNeedValueType[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mNeedValue ); i ++ )
	{
		pCompose->add_needvalue( mNeedValue[ i ] );
	}
	return ;
}

int CTemplateTeleTable::Initialize()
{
	memset( mLineID, 0, sizeof(mLineID)) ;
	memset( mMapID, 0, sizeof(mMapID));
	memset( mPosX, 0, sizeof(mPosX));
	memset( mPosY, 0, sizeof(mPosY));
	memset( mDes, 0, sizeof(mDes) );	
	memset( mFee, 0, sizeof(mFee));
	memset( mNeedTempID, 0, sizeof(mNeedTempID));
	memset( mNeedBuffID, 0, sizeof(mNeedBuffID));
	memset( mNeedLevel, 0, sizeof(mNeedLevel));
	return 0;
}

void CTemplateTeleTable::SetFromPB( bool bServer,PBTeleTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}
	mTempID			= pTable->tempid();
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) pTable->lineid_size() && i < (unsigned int) ARRAY_CNT( mLineID ); i ++ )
	{
		mLineID[ i ] = pTable->lineid( i );
	}
	for ( i = 0; i < (unsigned int) pTable->mapid_size() && i < (unsigned int) ARRAY_CNT( mMapID ); i ++ )
	{
		mMapID[ i ] = pTable->mapid( i );
	}
	for ( i = 0; i < (unsigned int) pTable->posx_size() && i < (unsigned int) ARRAY_CNT( mPosX ); i ++ )
	{
		mPosX[ i ] = pTable->posx( i );
	}
	for ( i = 0; i < (unsigned int) pTable->posy_size() && i < (unsigned int) ARRAY_CNT( mPosY ); i ++ )
	{
		mPosY[ i ] = pTable->posy( i );
	}
	for ( i = 0; i < (unsigned int) pTable->describe_size(); i ++ )
	{
		strncpy( mDes[ i ], pTable->describe( i ).c_str(), sizeof(mDes[i])-1 );
	}
	for ( i = 0; i < (unsigned int) pTable->money_size() && i < (unsigned int) ARRAY_CNT( mFee ); i ++ )
	{
		mFee[ i ] = pTable->money( i );
	}
	for ( i = 0; i < (unsigned int) pTable->needtempid_size() && i < (unsigned int) ARRAY_CNT( mNeedTempID ); i ++ )
	{
		mNeedTempID[ i ] = pTable->needtempid( i );
	}
	for ( i = 0; i < (unsigned int) pTable->needbuffid_size() && i < (unsigned int) ARRAY_CNT( mNeedBuffID ); i ++ )
	{
		mNeedBuffID[ i ] = pTable->needbuffid( i );
	}
	for ( i = 0; i < (unsigned int) pTable->needlevel_size() && i < (unsigned int) ARRAY_CNT( mNeedLevel ); i ++ )
	{
		mNeedLevel[ i ] = pTable->needlevel( i );
	}
	return ;
}

void CTemplateTeleTable::CreateTlvPB( bool bServer,PBTeleTable* pTable )
{
	if ( pTable == NULL )
	{
		return ;
	}
	pTable->set_tempid( mTempID );
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mLineID ); i ++ )
	{
		pTable->add_lineid( mLineID[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mMapID ); i ++ )
	{
		pTable->add_mapid( mMapID[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mPosX ); i ++ )
	{
		pTable->add_posx( mPosX[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mPosY ); i ++ )
	{
		pTable->add_posy( mPosY[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mDes ); i ++ )
	{
		pTable->add_describe( mDes[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mFee ); i ++ )
	{
		pTable->add_money( mFee[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mNeedTempID ); i ++ )
	{
		pTable->add_needtempid( mNeedTempID[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mNeedBuffID ); i ++ )
	{
		pTable->add_needbuffid( mNeedBuffID[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mNeedLevel ); i ++ )
	{
		pTable->add_needlevel( mNeedLevel[ i ] );
	}
	return ;
}

int CTemplateData::Initialize()
{	
	memset( mLevelFix, 0, sizeof(mLevelFix)) ;
	memset( mTeamFix, 0, sizeof(mTeamFix)) ;
	return 0;
}

void CTemplateData::SetFromPB( bool bServer,PBData *pData)
{
	if ( pData == NULL )
	{
		return ;
	}
	mTempID					= pData->tempid();
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mLevelFix ) && i < (unsigned int) pData->levelfix_size(); i ++ )
	{
		mLevelFix[ i ] = pData->levelfix( i );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mTeamFix ) && i < (unsigned int) pData->teamfix_size(); i ++ )
	{
		mTeamFix[ i ] = pData->teamfix( i );
	}
	return ;
}

void CTemplateData::CreateTlvPB( bool bServer,PBData* pData )
{
	if ( pData == NULL )
	{
		return ;
	}
	pData->set_tempid( mTempID );
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mLevelFix ); i ++ )
	{
		pData->add_levelfix( (int)( mLevelFix[ i ] ) );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mTeamFix ); i ++ )
	{
		pData->add_teamfix( (int)( mTeamFix[ i ] ) );
	}
	return ;
}

int CTemplateSummonBeast::Initialize()
{
	memset( mName, 0, sizeof(mName));			
	memset( mModel, 0, sizeof(mModel));	
	mLevel = 0;
	memset( mLevelUpDelayTime, 0, sizeof(mLevelUpDelayTime));
	memset( mPetData, 0, sizeof(mPetData));
	mAttackFrequency = 0;
	return 0;
}

void CTemplateSummonBeast::SetFromPB( bool bServer,PBSummonBeast *pBeast)
{
	if ( pBeast == NULL )
	{
		return ;
	}
	mTempID				= pBeast->tempid();
	strncpy( mName, pBeast->name().c_str(), sizeof( mName ) - 1 );
	strncpy( mModel, pBeast->model().c_str(), sizeof( mModel ) - 1 );
	mLevel				= pBeast->level();
	unsigned int i = 0;
	if ( bServer )
	{
		for ( i = 0; i < (unsigned int) pBeast->levelupdelaytime_size() && i < (unsigned int) ARRAY_CNT( mLevelUpDelayTime ); i ++ )
		{
			mLevelUpDelayTime[ i ] = pBeast->levelupdelaytime( i );
		}		
	}
	mAttackFrequency			= pBeast->attackfrequency();
	for ( i = 0; i < (unsigned int) pBeast->petdata_size() && ARRAY_CNT( mPetData ); i ++ )
	{
		PBPetData* pPetData = pBeast->mutable_petdata( i );
		mPetData[ i ].mAttackType	= (ATTACK_TYPE)pPetData->attacktype();
		mPetData[ i ].mDeathRate	= pPetData->deathrate();
		mPetData[ i ].mDeathValue	= pPetData->deathvalue();
		mPetData[ i ].mDeathDefense	= pPetData->deathdefense();
		mPetData[ i ].mHPRSpeed		= pPetData->hprspeed();
		mPetData[ i ].mMAttackLow	= pPetData->mattacklow();
		mPetData[ i ].mMAttackRange = pPetData->mattackrange();
		mPetData[ i ].mMAttackUp	= pPetData->mattackup();
		mPetData[ i ].mMaxHP		= pPetData->maxhp();
		mPetData[ i ].mMDefence		= pPetData->mdefence();
		mPetData[ i ].mMReducePer	= pPetData->mreduceper();
		mPetData[ i ].mNameColor	= (COLOR_TYPE)pPetData->namecolor();
		mPetData[ i ].mNormalDuck	= pPetData->normalduck();
		mPetData[ i ].mPAttackLow	= pPetData->pattacklow();
		mPetData[ i ].mPAttackRange = pPetData->pattackrange();
		mPetData[ i ].mPAttackUp	= pPetData->pattackup();
		mPetData[ i ].mPDefence		= pPetData->pdefence();
		mPetData[ i ].mPReducePer	= pPetData->preduceper();
		mPetData[ i ].mSkillDuck	= pPetData->skillduck();
		mPetData[ i ].mIgnoreDuck	= pPetData->ignoreduck();
		mPetData[ i ].mSkillID		= pPetData->skillid();
		mPetData[ i ].mSkillLevel	= pPetData->skilllevel();
		mPetData[ i ].mSpeed		= pPetData->speed();
		for ( unsigned j = 0; j < ( unsigned )pPetData->resatt_size() && j < ARRAY_CNT( mPetData[ i ].mResAtt ); j++ )
		{
			mPetData[ i ].mResAtt[ j ] = pPetData->resatt( j );
		}
		for ( unsigned j = 0; j < ( unsigned )pPetData->resdef_size() && j < ARRAY_CNT( mPetData[ i ].mResDef ); j++ )
		{
			mPetData[ i ].mResDef[ j ] = pPetData->resdef( j );
		}
		mPetData[ i ].mAttPercent = pPetData->attpercent();
		mPetData[ i ].mDefPercent = pPetData->defpercent();
		mPetData[ i ].mHPPercent = pPetData->hppercent();
	}	
	return ;
}

void CTemplateSummonBeast::CreateTlvPB( bool bServer,PBSummonBeast* pBeast )
{
	if ( pBeast == NULL )
	{
		return ;
	}
	pBeast->set_tempid( mTempID );
	pBeast->set_name( mName );
	pBeast->set_model( mModel );
	pBeast->set_level( mLevel );
	unsigned int i = 0;
	if ( bServer )
	{		
		for ( i = 0; i < (unsigned int) ARRAY_CNT( mLevelUpDelayTime ); i ++ )
		{
			pBeast->add_levelupdelaytime( mLevelUpDelayTime[ i ] );
		}		
	}	
	pBeast->set_attackfrequency(mAttackFrequency);
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mPetData ); i ++ )
	{
		PBPetData* pPetData = pBeast->add_petdata();
		pPetData->set_attacktype(mPetData[ i ].mAttackType);
		pPetData->set_deathrate(mPetData[ i ].mDeathRate);
		pPetData->set_deathvalue(mPetData[ i ].mDeathValue);
		pPetData->set_deathdefense(mPetData[ i ].mDeathDefense);
		pPetData->set_hprspeed(mPetData[ i ].mHPRSpeed);
		pPetData->set_mattacklow(mPetData[ i ].mMAttackLow);
		pPetData->set_mattackrange(mPetData[ i ].mMAttackRange);
		pPetData->set_mattackup(mPetData[ i ].mMAttackUp);
		pPetData->set_mreduceper(mPetData[ i ].mMReducePer);
		pPetData->set_maxhp(mPetData[ i ].mMaxHP);
		pPetData->set_mdefence(mPetData[ i ].mMDefence);
		pPetData->set_namecolor(mPetData[ i ].mNameColor);
		pPetData->set_normalduck(mPetData[ i ].mNormalDuck);
		pPetData->set_ignoreduck(mPetData[ i ].mIgnoreDuck);
		pPetData->set_pattacklow(mPetData[ i ].mPAttackLow);
		pPetData->set_pattackrange(mPetData[ i ].mPAttackRange);
		pPetData->set_pattackup(mPetData[ i ].mPAttackUp);
		pPetData->set_preduceper(mPetData[ i ].mPReducePer);
		pPetData->set_pdefence(mPetData[ i ].mPDefence);
		pPetData->set_skillduck(mPetData[ i ].mSkillDuck);
		pPetData->set_skillid(mPetData[ i ].mSkillID);
		pPetData->set_skilllevel(mPetData[ i ].mSkillLevel);
		pPetData->set_speed(mPetData[ i ].mSpeed);
		for ( unsigned j = 0; j < ARRAY_CNT( mPetData[ i ].mResAtt ); j++ )
		{
			 pPetData->add_resatt( mPetData[ i ].mResAtt[ j ] );
		}
		for ( unsigned j = 0; j < ARRAY_CNT( mPetData[ i ].mResDef ); j++ )
		{
			pPetData->add_resdef( mPetData[ i ].mResDef[ j ]  );
		}
		pPetData->set_attpercent( mPetData[ i ].mAttPercent );
		pPetData->set_defpercent( mPetData[ i ].mDefPercent );
		pPetData->set_hppercent( mPetData[ i ].mHPPercent );
	}	
	return ;
}

int CTemplateBeginner::Initialize()
{
	memset( mEquipItemID, 0, sizeof(mEquipItemID)) ;
	memset( mEquipItemNum, 0, sizeof(mEquipItemNum)) ;
	memset( mBaggageItemID, 0, sizeof(mBaggageItemID));
	memset( mBaggageItemNum, 0, sizeof(mBaggageItemNum));
	memset( mSkillID, 0, sizeof(mSkillID));
	memset( mSkillLevel, 0, sizeof(mSkillLevel));
	mStrNum = 0;
	mConNum = 0;
	mStaNum = 0;
	mWisNum = 0;
	mSprNum = 0;
	mSkillNum = 0; 
	mPosX1 = 0;
	mPosX2 = 0;
	mPosY1 = 0;
	mPosY2 = 0;
	memset( mSkillID1, 0, sizeof(mSkillID1));
	memset( mSkillLevel1, 0, sizeof(mSkillLevel1));
	memset( mSkillID2, 0, sizeof(mSkillID2));
	memset( mSkillLevel2, 0, sizeof(mSkillLevel2) );
	memset( mSkillID3, 0, sizeof(mSkillID3));
	memset( mSkillLevel3, 0, sizeof(mSkillLevel3) );
	memset( mSkillID4, 0, sizeof(mSkillID4));
	memset( mSkillLevel4, 0, sizeof(mSkillLevel4));
	memset( mSkillID5, 0, sizeof(mSkillID5));
	memset( mSkillLevel5, 0, sizeof(mSkillLevel5));
	memset( mSkillID6, 0, sizeof(mSkillID6));
	memset( mSkillLevel6, 0, sizeof(mSkillLevel6));
	mBeginnerProtectLevel = 0;
	mTransProtectTime = 0;
	mMapID = 0;
	mDeathDuraRate = 0;	
	mCanMendUseBindMoney = 0;	
	mBeginnerAP = 0;
	return 0;
}

void CTemplateBeginner::SetFromPB( bool bServer,PBBeginner *pBeginner)
{
	if ( pBeginner == NULL )
	{
		return ;
	}
	mTempID				= pBeginner->tempid();
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) pBeginner->equipitemid_size() && i < (unsigned int) ARRAY_CNT( mEquipItemID ); i ++ )
	{
		mEquipItemID[ i ] = pBeginner->equipitemid( i );
	}
	for ( i = 0; i < (unsigned int) pBeginner->equipitemnum_size() && i < (unsigned int) ARRAY_CNT( mEquipItemNum ); i ++ )
	{
		mEquipItemNum[ i ] = pBeginner->equipitemnum( i );
	}
	for ( i = 0; i < (unsigned int) pBeginner->baggageitemid_size() && i < (unsigned int) ARRAY_CNT( mBaggageItemID ); i ++ )
	{
		mBaggageItemID[ i ] = pBeginner->baggageitemid( i );
	}
	for ( i = 0; i < (unsigned int) pBeginner->baggageitemnum_size() && i < (unsigned int) ARRAY_CNT( mBaggageItemNum ); i ++ )
	{
		mBaggageItemNum[ i ] = pBeginner->baggageitemnum( i );
	}
	for ( i = 0; i < (unsigned int) pBeginner->skillid_size() && i < (unsigned int) ARRAY_CNT( mSkillID ); i ++ )
	{
		mSkillID[ i ] = pBeginner->skillid( i );
	}
	for ( i = 0; i < (unsigned int) pBeginner->skilllevel_size() && i < (unsigned int) ARRAY_CNT( mSkillLevel ); i ++ )
	{
		mSkillLevel[ i ] = pBeginner->skilllevel( i );
	}
	mStrNum				= pBeginner->strnum();
	mConNum				= pBeginner->connum();
	mStaNum				= pBeginner->stanum();
	mWisNum				= pBeginner->wisnum();
	mSprNum				= pBeginner->sprnum();
	mSkillNum			= pBeginner->skillnum();
	mPosX1				= pBeginner->poxx1();
	mPosX2				= pBeginner->poxx2();
	mPosY1				= pBeginner->poxy1();
	mPosY2				= pBeginner->poxy2();
	for ( i = 0; i < (unsigned int) pBeginner->skillid1_size() && i < (unsigned int) ARRAY_CNT( mSkillID1 ); i ++ )
	{
		mSkillID1[ i ] = pBeginner->skillid1( i );
	}
	for ( i = 0; i < (unsigned int) pBeginner->skilllevel1_size() && i < (unsigned int) ARRAY_CNT( mSkillLevel1 ); i ++ )
	{
		mSkillLevel1[ i ] = pBeginner->skilllevel1( i );
	}
	for ( i = 0; i < (unsigned int) pBeginner->skillid2_size() && i < (unsigned int) ARRAY_CNT( mSkillID2 ); i ++ )
	{
		mSkillID2[ i ] = pBeginner->skillid2( i );
	}
	for ( i = 0; i < (unsigned int) pBeginner->skilllevel2_size() && i < (unsigned int) ARRAY_CNT( mSkillLevel2 ); i ++ )
	{
		mSkillLevel2[ i ] = pBeginner->skilllevel2( i );
	}
	for ( i = 0; i < (unsigned int) pBeginner->skillid3_size() && i < (unsigned int) ARRAY_CNT( mSkillID3 ); i ++ )
	{
		mSkillID3[ i ] = pBeginner->skillid3( i );
	}
	for ( i = 0; i < (unsigned int) pBeginner->skilllevel3_size() && i < (unsigned int) ARRAY_CNT( mSkillLevel3 ); i ++ )
	{
		mSkillLevel3[ i ] = pBeginner->skilllevel3( i );
	}
	for ( i = 0; i < (unsigned int) pBeginner->skillid4_size() && i < (unsigned int) ARRAY_CNT( mSkillID4 ); i ++ )
	{
		mSkillID4[ i ] = pBeginner->skillid4( i );
	}
	for ( i = 0; i < (unsigned int) pBeginner->skilllevel4_size() && i < (unsigned int) ARRAY_CNT( mSkillLevel4 ); i ++ )
	{
		mSkillLevel4[ i ] = pBeginner->skilllevel4( i );
	}
	for ( i = 0; i < (unsigned int) pBeginner->skillid5_size() && i < (unsigned int) ARRAY_CNT( mSkillID5 ); i ++ )
	{
		mSkillID5[ i ] = pBeginner->skillid5( i );
	}
	for ( i = 0; i < (unsigned int) pBeginner->skilllevel5_size() && i < (unsigned int) ARRAY_CNT( mSkillLevel5 ); i ++ )
	{
		mSkillLevel5[ i ] = pBeginner->skilllevel5( i );
	}
	for ( i = 0; i < (unsigned int) pBeginner->skillid6_size() && i < (unsigned int) ARRAY_CNT( mSkillID6 ); i ++ )
	{
		mSkillID6[ i ] = pBeginner->skillid6( i );
	}
	for ( i = 0; i < (unsigned int) pBeginner->skilllevel6_size() && i < (unsigned int) ARRAY_CNT( mSkillLevel6 ); i ++ )
	{
		mSkillLevel6[ i ] = pBeginner->skilllevel6( i );
	}
	mBeginnerProtectLevel		= pBeginner->beginnerprotectlevel();
	mTransProtectTime			= pBeginner->transprotecttime();
	mMapID						= pBeginner->mapid();
	mDeathDuraRate				= pBeginner->deathdurarate();
	mCanMendUseBindMoney		= pBeginner->canmendusebindmoney();
	mBeginnerAP					= pBeginner->beginnerap();
	return ;
}

void CTemplateBeginner::CreateTlvPB( bool bServer,PBBeginner* pBeginner )
{
	if ( pBeginner == NULL )
	{
		return ;
	}
	pBeginner->set_tempid( mTempID );
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mEquipItemID ); i ++ )
	{
		pBeginner->add_equipitemid( mEquipItemID[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mEquipItemNum ); i ++ )
	{
		pBeginner->add_equipitemnum( mEquipItemNum[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mBaggageItemID ); i ++ )
	{
		pBeginner->add_baggageitemid( mBaggageItemID[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mBaggageItemNum ); i ++ )
	{
		pBeginner->add_baggageitemnum( mBaggageItemNum[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSkillID ); i ++ )
	{
		pBeginner->add_skillid( mSkillID[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSkillLevel ); i ++ )
	{
		pBeginner->add_skilllevel( mSkillLevel[ i ] );
	}
	pBeginner->set_strnum(mStrNum);
	pBeginner->set_connum(mConNum);
	pBeginner->set_stanum(mStaNum);
	pBeginner->set_wisnum(mWisNum);
	pBeginner->set_sprnum(mSprNum);
	pBeginner->set_skillnum(mSkillNum);
	pBeginner->set_poxx1(mPosX1);
	pBeginner->set_poxx2(mPosX2);
	pBeginner->set_poxy1(mPosY1);
	pBeginner->set_poxy2(mPosY2);
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSkillID1 ); i ++ )
	{
		pBeginner->add_skillid1( mSkillID1[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSkillLevel1 ); i ++ )
	{
		pBeginner->add_skilllevel1( mSkillLevel1[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSkillID2 ); i ++ )
	{
		pBeginner->add_skillid2( mSkillID2[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSkillLevel2 ); i ++ )
	{
		pBeginner->add_skilllevel2( mSkillLevel2[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSkillID3 ); i ++ )
	{
		pBeginner->add_skillid3( mSkillID3[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSkillLevel3 ); i ++ )
	{
		pBeginner->add_skilllevel3( mSkillLevel3[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSkillID4 ); i ++ )
	{
		pBeginner->add_skillid4( mSkillID4[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSkillLevel4 ); i ++ )
	{
		pBeginner->add_skilllevel4( mSkillLevel4[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSkillID5 ); i ++ )
	{
		pBeginner->add_skillid5( mSkillID5[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSkillLevel5 ); i ++ )
	{
		pBeginner->add_skilllevel5( mSkillLevel5[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSkillID6 ); i ++ )
	{
		pBeginner->add_skillid6( mSkillID6[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mSkillLevel6 ); i ++ )
	{
		pBeginner->add_skilllevel6( mSkillLevel6[ i ] );
	}
	pBeginner->set_beginnerprotectlevel(mBeginnerProtectLevel);
	pBeginner->set_transprotecttime(mTransProtectTime);
	pBeginner->set_mapid( mMapID );
	pBeginner->set_deathdurarate( mDeathDuraRate );
	pBeginner->set_canmendusebindmoney( mCanMendUseBindMoney );
	pBeginner->set_beginnerap( mBeginnerAP );
	return ;
}


int CTplBuffImmunity::Initialize()
{
	memset( mBuffImmunity, 0, sizeof(mBuffImmunity));
	return 0;
}

void CTplBuffImmunity::SetFromPB( bool vServer, PBTplBuffImmunity* pImmunity )
{
	if( pImmunity == NULL )
	{
		return;
	}	

	mTempID = pImmunity->tempid( );

	for( unsigned int i = 0; i < ARRAY_CNT( mBuffImmunity ) && i < ( unsigned int )pImmunity->bufftype_size( ); i++ )
	{
		mBuffImmunity[ i ] = ( CTemplateBuff::BUFF_FUN_TYPE )pImmunity->bufftype( i );
	}

	return;
}

void CTplBuffImmunity::CreateTlvPB( bool vServer, PBTplBuffImmunity* pImmunity )
{
	if( pImmunity == NULL )
	{
		return;
	}

	pImmunity->set_tempid( mTempID );

	for( unsigned int i = 0; i < ARRAY_CNT( mBuffImmunity ); i++ )
	{
		if( mBuffImmunity[ i ] == CTemplateBuff::BUFFINC_NONE )
		{
			break;
		}

		pImmunity->add_bufftype( mBuffImmunity[ i ] );	
	}
}

//void CTempLoopTaskAwds::SetFromPB( bool bServer,PBLoopTaskAwds *pTask)
//{
//	if ( pTask == NULL )
//	{
//		return ;
//	}
//	mTempID			= pTask->tempid();
//	unsigned int i = 0, j = 0, tTotal = 0;
//	for ( i = 0; i < (unsigned int) ARRAY_ROW( mExp ); i ++ )
//	{
//		for ( j = 0; j < (unsigned int) ARRAY_COL( mExp ); j ++ )
//		{
//			tTotal = (i*ARRAY_COL( mExp )) + j;
//			if ( tTotal >= (unsigned int)pTask->exp_size() )
//			{
//				break;
//			}
//			mExp[ i ][ j ] = pTask->exp( tTotal );
//		}
//	}
//	for ( i = 0; i < (unsigned int) ARRAY_ROW( mMoney ); i ++ )
//	{
//		for ( j = 0; j < (unsigned int) ARRAY_COL( mMoney ); j ++ )
//		{
//			tTotal = (i*ARRAY_COL( mMoney )) + j;
//			if ( tTotal >= (unsigned int)pTask->money_size() )
//			{
//				break;
//			}
//			mMoney[ i ][ j ] = pTask->money( tTotal );
//		}
//	}
//	return ;
//}
//
//void CTempLoopTaskAwds::CreateTlvPB( bool bServer,PBLoopTaskAwds* pTask )
//{
//	if ( pTask == NULL )
//	{
//		return ;
//	}
//	pTask->set_tempid( mTempID );
//	unsigned int i = 0, j = 0;
//	for ( i = 0; i < (unsigned int) ARRAY_ROW( mExp ); i ++ )
//	{
//		for ( j = 0; j < (unsigned int) ARRAY_COL( mExp ); j ++ )
//		{
//			pTask->add_exp( mExp[ i ][ j ] );
//		}
//	}
//	for ( i = 0; i < (unsigned int) ARRAY_ROW( mMoney ); i ++ )
//	{
//		for ( j = 0; j < (unsigned int) ARRAY_COL( mMoney ); j ++ )
//		{
//			pTask->add_money( mMoney[ i ][ j ] );
//		}
//	}
//	return ;
//}

int CItemSequence::Initialize()
{
	memset(mSequence, 0, sizeof(mSequence));
	return 0;
}

void CItemSequence::SetFromPB( bool bServer,PBItemSequence* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	mTempID = pItem->tempid();
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mSequence ) && i < (unsigned int) pItem->sequence_size(); i ++ )
	{
		mSequence[ i ] = pItem->sequence( i );
	}
	return ;
}

void CItemSequence::CreateTlvPB( bool bServer,PBItemSequence* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	pItem->set_tempid( mTempID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mSequence ); i ++ )
	{
		pItem->add_sequence( mSequence[ i ] );
	}
	return ;
}

int CTemplateDropLevel::Initialize()
{
	memset( mRan, 0, sizeof(mRan) );
	return 0;
}

void CTemplateDropLevel::SetFromPB( bool bServer,PBDropLevel *pLevel)
{
	if ( pLevel == NULL )
	{
		return ;
	}
	mTempID			= pLevel->tempid();	
	if ( bServer )
	{
		int tTotal = 0;
		for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mRan ); i ++ )
		{
			for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mRan ); j ++ )
			{
				tTotal = (i*ARRAY_COL( mRan )) + j ;
				if ( tTotal >= pLevel->ran_size() )
					break;
				mRan[ i ][ j ] = pLevel->ran( tTotal );
			}
		}
	}
	return ;
}

void CTemplateDropLevel::CreateTlvPB( bool bServer,PBDropLevel* pLevel )
{
	if ( pLevel == NULL )
	{
		return ;
	}
	pLevel->set_tempid( mTempID );
	if ( bServer )
	{
		for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mRan ); i ++ )
		{
			for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mRan ); j ++ )
			{
				pLevel->add_ran( (int)( mRan[ i ][ j ] ) );
			}
		}
	}
	return ;
}

int CTemplateOgreTran::Initialize()
{
	memset( mDrop, 0, sizeof(mDrop));
	memset( mHP, 0, sizeof(mHP));
	memset( mMP, 0, sizeof(mMP));
	memset( mPA, 0, sizeof(mPA));
	memset( mMA, 0, sizeof(mMA));
	memset( mPD, 0, sizeof(mPD));
	memset( mMD, 0, sizeof(mMD));
	memset( mHit, 0, sizeof(mHit));
	memset( mDodge, 0, sizeof(mDodge));
	memset( mIgnore, 0, sizeof(mIgnore));
	memset( mDeath, 0, sizeof(mDeath));
	memset( mResAtt, 0, sizeof(mResAtt));
	memset( mResDef, 0, sizeof(mResDef));	
	memset( mExp, 0, sizeof(mExp));
	return 0;
}

void CTemplateOgreTran::SetFromPB( bool bServer,PBOgreTran *pOgre)
{
	if ( pOgre == NULL )
	{
		return ;
	}
	mTempID				= pOgre->tempid();
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) pOgre->drop_size() && i < (unsigned int) ARRAY_CNT( mDrop ); i ++ )
	{
		mDrop[ i ] = pOgre->drop( i );
	}
	for ( i = 0; i < (unsigned int) pOgre->hp_size() && i < (unsigned int) ARRAY_CNT( mHP ); i ++ )
	{
		mHP[ i ] = pOgre->hp( i );
	}
	for ( i = 0; i < (unsigned int) pOgre->mp_size() && i < (unsigned int) ARRAY_CNT( mMP ); i ++ )
	{
		mMP[ i ] = pOgre->mp( i );
	}
	for ( i = 0; i < (unsigned int) pOgre->pa_size() && i < (unsigned int) ARRAY_CNT( mPA ); i ++ )
	{
		mPA[ i ] = pOgre->pa( i );
	}
	for ( i = 0; i < (unsigned int) pOgre->ma_size() && i < (unsigned int) ARRAY_CNT( mMA ); i ++ )
	{
		mMA[ i ] = pOgre->ma( i );
	}
	for ( i = 0; i < (unsigned int) pOgre->pd_size() && i < (unsigned int) ARRAY_CNT( mPD ); i ++ )
	{
		mPD[ i ] = pOgre->pd( i );
	}
	for ( i = 0; i < (unsigned int) pOgre->md_size() && i < (unsigned int) ARRAY_CNT( mMD ); i ++ )
	{
		mMD[ i ] = pOgre->md( i );
	}
	for ( i = 0; i < (unsigned int) pOgre->hit_size() && i < (unsigned int) ARRAY_CNT( mHit ); i ++ )
	{
		mHit[ i ] = pOgre->hit( i );
	}
	for ( i = 0; i < (unsigned int) pOgre->dodge_size() && i < (unsigned int) ARRAY_CNT( mDodge ); i ++ )
	{
		mDodge[ i ] = pOgre->dodge( i );
	}
	for ( i = 0; i < (unsigned int) pOgre->ignore_size() && i < (unsigned int) ARRAY_CNT( mIgnore ); i ++ )
	{
		mIgnore[ i ] = pOgre->ignore( i );
	}
	for ( i = 0; i < (unsigned int) pOgre->death_size() && i < (unsigned int) ARRAY_CNT( mDeath ); i ++ )
	{
		mDeath[ i ] = pOgre->death( i );
	}
	for ( i = 0; i < (unsigned int) ARRAY_ROW( mResDef ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mResDef ) ); j ++ )
		{
			int tTotal = (ARRAY_COL( mResDef )*i) + j;
			if ( tTotal >= pOgre->resdef_size() )
			{
				break ;
			}
			mResDef[ i ][ j ] = pOgre->resdef( tTotal );
		}
	}
	for ( i = 0; i < (unsigned int) ARRAY_ROW( mResAtt ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mResAtt ) ); j ++ )
		{
			int tTotal = (ARRAY_COL( mResAtt )*i) + j;
			if ( tTotal >= pOgre->resatt_size() )
			{
				break ;
			}
			mResAtt[ i ][ j ] = pOgre->resatt( tTotal );
		}
	}
	for ( i = 0; i < (unsigned int) pOgre->exp_size() && i < (unsigned int) ARRAY_CNT( mExp ); i ++ )
	{
		mExp[ i ] = pOgre->exp( i );
	}
	return ;
}

void CTemplateOgreTran::CreateTlvPB( bool bServer,PBOgreTran* pOgre )
{
	if ( pOgre == NULL )
	{
		return ;
	}
	pOgre->set_tempid(mTempID);
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mDrop ); i ++ )
	{
		pOgre->add_drop( mDrop[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mHP ); i ++ )
	{
		pOgre->add_hp( mHP[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mMP ); i ++ )
	{
		pOgre->add_mp( mMP[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mPA ); i ++ )
	{
		pOgre->add_pa( mPA[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mMA ); i ++ )
	{
		pOgre->add_ma( mMA[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mPD ); i ++ )
	{
		pOgre->add_pd( mPD[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mMD ); i ++ )
	{
		pOgre->add_md( mMD[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mHit ); i ++ )
	{
		pOgre->add_hit( mHit[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mDodge ); i ++ )
	{
		pOgre->add_dodge( mDodge[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_ROW( mResDef ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mResDef ) ); j ++ )
		{			
			pOgre->add_resdef( mResDef[ i ][ j ] );
		}
	}
	for ( i = 0; i < (unsigned int) ARRAY_ROW( mResAtt ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mResAtt ) ); j ++ )
		{
			pOgre->add_resatt( mResAtt[ i ][ j ] );
		}
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mExp ); i ++ )
	{
		pOgre->add_exp( mExp[ i ] );
	}
	return ;
}

int CTemplateMetierProperty::Initialize()
{
	mMetierRequire = 0;
	mOption = 0;
	memset( mMetierProperty, 0, sizeof(mMetierProperty));
	return 0;
}

void CTemplateMetierProperty::SetFromPB( bool bServer,PBMetierPro *pMetier)
{
	if ( pMetier == NULL )
	{
		return ;
	}	
	mTempID					= pMetier->tempid();
	mMetierRequire			= pMetier->metierrequire();
	mOption					= pMetier->option();
	int tTotal = 0;
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mMetierProperty ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mMetierProperty ); j ++ )
		{
			tTotal = (ARRAY_COL( mMetierProperty )*i) + j;
			if ( tTotal >= pMetier->metierproperty_size() )
			{
				break ;
			}
			mMetierProperty[ i ][ j ] = pMetier->metierproperty( tTotal );
		}
	}
	return ;
}

void CTemplateMetierProperty::CreateTlvPB( bool bServer,PBMetierPro *pMetier)
{
	if ( pMetier == NULL )
	{
		return ;
	}
	pMetier->set_tempid( mTempID );
	pMetier->set_metierrequire( mMetierRequire );
	pMetier->set_option( mOption );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mMetierProperty ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mMetierProperty ); j ++ )
		{
			pMetier->add_metierproperty( mMetierProperty[ i ][ j ] );
		}
	}
	return ;
}

int CTemplateLevelProperty::Initialize()
{	
	mOption = 0;
	memset( mLevelProperty, 0, sizeof(mLevelProperty));
	return 0;
}

void CTemplateLevelProperty::SetFromPB( bool bServer,PBLevelProperty *pLevel)
{
	if ( pLevel == NULL )
	{
		return ;
	}
	mTempID				= pLevel->tempid();
	mOption				= pLevel->option();
	for ( unsigned int i = 0; i < (unsigned int) pLevel->levelproperty_size() && i < (unsigned int) ARRAY_CNT( mLevelProperty ); i ++ )
	{
		mLevelProperty[ i ] = pLevel->levelproperty( i );
	}
	return ;
}

void CTemplateLevelProperty::CreateTlvPB( bool bServer,PBLevelProperty* pLevel )
{
	if ( pLevel == NULL )
	{
		return ;
	}
	pLevel->set_tempid( mTempID );
	pLevel->set_option( mOption );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mLevelProperty ); i ++ )
	{
		pLevel->add_levelproperty( mLevelProperty[ i ] );
	}
	return ;
}

int CTemplateHonorToExp::Initialize()
{	
	memset( mExp, 0, sizeof(mExp)) ;
	memset( mWizard, 0, sizeof(mWizard));
	mExpLimit=0;
	mItemID=0;
	mItemHonor=0;
	mItemLimit=0;
	return 0;
}

void CTemplateHonorToExp::SetFromPB( bool bServer,PBHonorToExp *pHonor)
{
	if ( pHonor == NULL )
	{
		return ;
	}
	mTempID			= pHonor->tempid();
	for ( unsigned int i = 0; i < (unsigned int) pHonor->exp_size() && i < (unsigned int) ARRAY_CNT( mExp ); i ++ )
	{
		mExp[ i ] = pHonor->exp( i );
	}
	for ( unsigned int i = 0; i < (unsigned int) pHonor->wizard_size() && i < (unsigned int) ARRAY_CNT( mWizard ); i ++ )
	{
		mWizard[ i ] = pHonor->wizard( i );
	}
	mExpLimit = pHonor->explimit();
	mItemID = pHonor->itemid();
	mItemHonor = pHonor->itemhonor();
	mItemLimit = pHonor->itemlimit();
	return ;
}

void CTemplateHonorToExp::CreateTlvPB( bool bServer,PBHonorToExp* pHonor )
{
	if ( pHonor == NULL )
	{
		return ;
	}
	pHonor->set_tempid( mTempID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mExp ); i ++ )
	{
		pHonor->add_exp( mExp[ i ] );
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mWizard ); i ++ )
	{
		pHonor->add_wizard( mWizard[ i ] );
	}
	pHonor->set_explimit( mExpLimit );
	pHonor->set_itemid( mItemID );
	pHonor->set_itemhonor( mItemHonor );
	pHonor->set_itemlimit( mItemLimit );
	return ;
}

int CTemplateHonorObtain::Initialize()
{
	mInLimit = 0;
	mOutLimit = 0;
	mOutDieExp = 0;
	mInDieExp = 0;
	mBorderDieExp = 0;
	memset( mOutLowExp, 0, sizeof(mOutLowExp));
	memset( mOutHighExp, 0, sizeof(mOutHighExp));
	memset( mInLowExp, 0, sizeof(mInLowExp));
	memset( mInHighExp, 0, sizeof(mInHighExp));
	memset( mBorderLowExp, 0, sizeof(mBorderLowExp));
	memset( mBorderHighExp, 0, sizeof(mBorderHighExp));
	memset( mLostLowExp, 0, sizeof(mLostLowExp));
	memset( mLostHighExp, 0, sizeof(mLostHighExp));
	memset( mBasicExp, 0, sizeof(mBasicExp));
	return 0;
}

void CTemplateHonorObtain::SetFromPB( bool bServer,PBHonorObtain *pObtain)
{
	if ( pObtain == NULL )
	{
		return ;
	}
	mTempID				= pObtain->tempid();
	mInLimit			= pObtain->inlimit();
	mOutLimit			= pObtain->outlimit();
	mOutDieExp			= pObtain->outdieexp();
	mInDieExp			= pObtain->indieexp();
	mBorderDieExp		= pObtain->borderdieexp();
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) pObtain->outlowexp_size() && i < (unsigned int) ARRAY_CNT( mOutLowExp ); i ++ )
	{
		mOutLowExp[ i ] = pObtain->outlowexp( i );
	}
	for ( i = 0; i < (unsigned int) pObtain->outhighexp_size() && i < (unsigned int) ARRAY_CNT( mOutHighExp ); i ++ )
	{
		mOutHighExp[ i ] = pObtain->outhighexp( i );
	}
	for ( i = 0; i < (unsigned int) pObtain->inlowexp_size() && i < (unsigned int) ARRAY_CNT( mInLowExp ); i ++ )
	{
		mInLowExp[ i ] = pObtain->inlowexp( i );
	}
	for ( i = 0; i < (unsigned int) pObtain->inhighexp_size() && i < (unsigned int) ARRAY_CNT( mInHighExp ); i ++ )
	{
		mInHighExp[ i ] = pObtain->inhighexp( i );
	}
	for ( i = 0; i < (unsigned int) pObtain->borderlowexp_size() && i < (unsigned int) ARRAY_CNT( mBorderLowExp ); i ++ )
	{
		mBorderLowExp[ i ] = pObtain->borderlowexp( i );
	}
	for ( i = 0; i < (unsigned int) pObtain->borderhighexp_size() && i < (unsigned int) ARRAY_CNT( mBorderHighExp ); i ++ )
	{
		mBorderHighExp[ i ] = pObtain->borderhighexp( i );
	}
	for ( i = 0; i < (unsigned int) pObtain->lostlowexp_size() && i < (unsigned int) ARRAY_CNT( mLostLowExp ); i ++ )
	{
		mLostLowExp[ i ] = pObtain->lostlowexp( i );
	}
	for ( i = 0; i < (unsigned int) pObtain->losthighexp_size() && i < (unsigned int) ARRAY_CNT( mLostHighExp ); i ++ )
	{
		mLostHighExp[ i ] = pObtain->losthighexp( i );
	}
	for ( i = 0; i < (unsigned int) pObtain->basicexp_size() && i < (unsigned int) ARRAY_CNT( mBasicExp ); i ++ )
	{
		mBasicExp[ i ] = pObtain->basicexp( i );
	}
	return ;
}

void CTemplateHonorObtain::CreateTlvPB( bool bServer,PBHonorObtain* pObtain )
{
	if ( pObtain == NULL )
	{
		return ;
	}
	pObtain->set_tempid(mTempID);
	pObtain->set_inlimit(mInLimit);
	pObtain->set_outlimit(mOutLimit);
	pObtain->set_outdieexp(mOutDieExp);
	pObtain->set_indieexp(mInDieExp);
	pObtain->set_borderdieexp(mBorderDieExp);
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mOutLowExp ); i ++ )
	{
		pObtain->add_outlowexp( mOutLowExp[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mOutHighExp ); i ++ )
	{
		 pObtain->add_outhighexp( mOutHighExp[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mInLowExp ); i ++ )
	{
		pObtain->add_inlowexp( mInLowExp[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mInHighExp ); i ++ )
	{
		pObtain->add_inhighexp( mInHighExp[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mBorderLowExp ); i ++ )
	{
		pObtain->add_borderlowexp( mBorderLowExp[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mBorderHighExp ); i ++ )
	{
		pObtain->add_borderhighexp( mBorderHighExp[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mLostLowExp ); i ++ )
	{
		pObtain->add_lostlowexp( mLostLowExp[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mLostHighExp ); i ++ )
	{
		pObtain->add_losthighexp( mLostHighExp[ i ] );
	}
	for ( i = 0; i < (unsigned int) ARRAY_CNT( mBasicExp ); i ++ )
	{
		pObtain->add_basicexp( mBasicExp[ i ] );
	}
	return ;
}

int CTemplateDiploidTimeTable::Initialize()
{
	mTotalTime = 0;
	mTypeNumber = 0;
	mValidMax = 0;
	memset( mBuffID, 0, sizeof(mBuffID));
	return 0;
}

void CTemplateDiploidTimeTable::SetFromPB( bool bServer,PBDiploidTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}
	mTempID			= pTable->tempid();
	mTotalTime		= pTable->totaltime();
	mValidMax		= pTable->validmax();
	mTypeNumber		= pTable->typenumber();
	for ( unsigned int i = 0; i < (unsigned int) pTable->buffid_size() && i < (unsigned int) ARRAY_CNT( mBuffID ); i ++ )
	{
		mBuffID[ i ] = pTable->buffid( i );
	}
	return ;
}

void CTemplateDiploidTimeTable::CreateTlvPB( bool bServer,PBDiploidTable* pTable )
{
	if ( pTable == NULL )
	{
		return ;
	}
	pTable->set_tempid( mTempID );
	pTable->set_totaltime( mTotalTime );
	pTable->set_validmax( mValidMax );
	pTable->set_typenumber( mTypeNumber );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mBuffID ); i ++ )
	{
		pTable->add_buffid( mBuffID[ i ] );
	}
	return ;
}

int CTplRedStone::Initialize()
{	
	memset( mMagicIconName, 0, sizeof(mMagicIconName));			
	mGuageTime = 0 ;
	mMagicSustainTime = 0;
	mExpTime = 0;
	mRadius = 0;
	mRedStoneType = 0;
	mFamilyExpRate = 0;
	mExpTableID = 0;	  	
	mNumberEffected = 0;
	mMetierID = 0;
	memset(mKeyBuffID, 0, sizeof(mKeyBuffID));
	memset(mEncBuffID, 0, sizeof(mEncBuffID));
	memset(mEncPercent, 0, sizeof(mEncPercent));
	mIsEternal = 0;
	return 0;
}

void CTplRedStone::SetFromPB( bool bServer,PBRedStone *pStone)
{
	if ( pStone == NULL )
	{
		return ;
	}
	
	CTplItem::SetFromPB( bServer, pStone->mutable_baseitem() );
	//mTempID			= pStone->tempid();
	//mItemType		= pStone->itemtype();
	//strncpy( mModelName, pStone->modelname().c_str(), sizeof( mModelName ) - 1 );
	strncpy( mMagicIconName, pStone->magiciconname().c_str(), sizeof( mMagicIconName ) - 1 );
	//strncpy( mIconName, pStone->iconname().c_str(), sizeof( mIconName ) - 1 );
	mGuageTime		= pStone->guagetime();
	mMagicSustainTime = pStone->magicsustaintime();
	mExpTime		= pStone->exptime();
	mRadius			= pStone->radius();
	mRedStoneType	= pStone->redstonetype();
	mFamilyExpRate	= pStone->familyexprate();
	mExpTableID		= pStone->exptableid();
	mNumberEffected = pStone->numbereffected();
	mMetierID		= pStone->metierid();
	for ( unsigned int i = 0; i < (unsigned int) pStone->keybuffid_size() && i < (unsigned int) ARRAY_CNT( mKeyBuffID ); i ++ )
	{
		mKeyBuffID[ i ] = pStone->keybuffid( i );
	}
	for ( unsigned int i = 0; i < (unsigned int) pStone->encbuffid_size() && i < (unsigned int) ARRAY_CNT( mEncBuffID ); i ++ )
	{
		mEncBuffID[ i ] = pStone->encbuffid( i );
	}
	for ( unsigned int i = 0; i < (unsigned int) pStone->encpercent_size() && i < (unsigned int) ARRAY_CNT( mEncPercent ); i ++ )
	{
		mEncPercent[ i ] = pStone->encpercent( i );
	}
	mIsEternal = pStone->iseternal();
	return ;
	 
}

void CTplRedStone::CreateTlvPB( bool bServer,PBRedStone* pStone )
{
	if ( pStone == NULL )
	{
		return ;
	}
	CTplItem::CreateTlvPB( bServer, pStone->mutable_baseitem() );
	//pStone->set_tempid( mTempID );
	//pStone->set_modelname( mModelName );
	//pStone->set_itemtype( mItemType );
	pStone->set_magiciconname( mMagicIconName );
	//pStone->set_iconname( mIconName );
	pStone->set_guagetime( mGuageTime );
	pStone->set_magicsustaintime( mMagicSustainTime );
	pStone->set_exptime( mExpTime );
	pStone->set_radius( mRadius );
	pStone->set_redstonetype( mRedStoneType );
	pStone->set_familyexprate( mFamilyExpRate );
	pStone->set_exptableid( mExpTableID );
	pStone->set_numbereffected( mNumberEffected );
	pStone->set_metierid( mMetierID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mKeyBuffID ); i ++ )
	{
		pStone->add_keybuffid( mKeyBuffID[ i ] );
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mEncBuffID ); i ++ )
	{
		pStone->add_encbuffid( mEncBuffID[ i ] );
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mEncPercent ); i ++ )
	{
		pStone->add_encpercent( mEncPercent[ i ] );
	}
	pStone->set_iseternal( mIsEternal );
	return;
}

int CTemplateRedStoneExpTable::Initialize()
{
	memset( mExp, 0, sizeof(mExp));	
	memset( mEffectName, 0, sizeof(mEffectName));
	return 0;
}

void CTemplateRedStoneExpTable::SetFromPB( bool bServer,PBRedStoneExpTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}
	mTempID					= pTable->tempid();
	for ( unsigned int i = 0; i < (unsigned int) pTable->exp_size() && i < (unsigned int) ARRAY_CNT( mExp ); i ++ )
	{
		mExp[ i ] = pTable->exp( i );
	}
	strncpy( mEffectName, pTable->effectname().c_str(), sizeof( mEffectName ) - 1 );
	return ;
}	

void CTemplateRedStoneExpTable::CreateTlvPB( bool bServer,PBRedStoneExpTable* pTable )
{
	if ( pTable == NULL )
	{
		return ;
	}
	pTable->set_tempid( mTempID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mExp ); i ++ )
	{
		pTable->add_exp( mExp[ i ] );
	}
	pTable->set_effectname( mEffectName ); 
	return ;
}

int CTemplateReliveTable::Initialize()
{	
	memset( mFee, 0, sizeof(mFee) );
	mRate = 0;
	return 0;
}

void CTemplateReliveTable::SetFromPB( bool bServer,PBReliveTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}
	mTempID				= pTable->tempid();
	for ( unsigned int i = 0; i < (unsigned int) pTable->fee_size() && i < (unsigned int) ARRAY_CNT( mFee ); i ++ )
	{
		mFee[ i ] = pTable->fee( i );
	}
	mRate = pTable->rate();
	return ;
}

void CTemplateReliveTable::CreateTlvPB( bool bServer,PBReliveTable* pTable )
{
	if ( pTable == NULL )
	{
		return ;
	}
	pTable->set_tempid( mTempID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mFee ); i ++ )
	{
		pTable->add_fee( mFee[ i ] );
	}
	pTable->set_rate( mRate );
	return ;
}

int CTemplateQuestionExpTable::Initialize()
{
	mNpcQuestionNum = 0;			
	memset( mExp, 0, sizeof(mExp));
	mNpcExpAddRate = 0;
	return 0;
}

void CTemplateQuestionExpTable::SetFromPB( bool bServer,PBQuestionExp* pExp )
{
	if ( pExp == NULL )
	{
		return ;
	}
	mTempID				= pExp->tempid();
	mNpcQuestionNum		= pExp->npcquestionnum();
	for ( unsigned int i = 0; i < (unsigned int) pExp->exp_size() && i < (unsigned int) ARRAY_CNT( mExp ); i ++ )
	{
		mExp[ i ] = pExp->exp( i );
	}
	mNpcExpAddRate		= pExp->npcexpaddrate();
	return ;
}

void CTemplateQuestionExpTable::CreateTlvPB( bool bServer,PBQuestionExp *pExp)
{
	if ( pExp == NULL )
	{
		return ;
	}
	pExp->set_tempid( mTempID );
	pExp->set_npcquestionnum( mNpcQuestionNum );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_CNT( mExp ); i ++ )
	{
		pExp->add_exp( mExp[ i ] );
	}
	pExp->set_npcexpaddrate( mNpcExpAddRate );
	return ;
}

int CTplBindChargeTable::Initialize()
{	
	mMoneyBindNeed = 0; 	
	mCanBindUseBM = 0;	
	mMoneyUnBindNeed = 0;		
	mCanUnBindUseBM = 0; 	
	mMoneyResumeBindNeed = 0;	
	mCanResumeBindUseBM = 0;		
	return 0;
}

void CTplBindChargeTable::SetFromPB( bool bServer,PBBindChargeTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}
	mTempID				 = pTable->tempid();
	mCanBindUseBM		 = pTable->canbindusebm();
	mMoneyBindNeed		 = pTable->moneybindneed();
	mCanUnBindUseBM		 = pTable->unbindusebm();
	mMoneyUnBindNeed	 = pTable->moneyunbindneed();	
	mMoneyResumeBindNeed = pTable->moneyresumebind();
	mCanResumeBindUseBM	 = pTable->canresumebind();
	return ;
}

void CTplBindChargeTable::CreateTlvPB( bool bServer,PBBindChargeTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}
	pTable->set_tempid( mTempID );
	pTable->set_canbindusebm( mCanBindUseBM );
	pTable->set_moneyunbindneed( mMoneyUnBindNeed );
	pTable->set_moneyresumebind( mMoneyResumeBindNeed );
	pTable->set_canresumebind( mCanResumeBindUseBM );
	pTable->set_unbindusebm( mCanUnBindUseBM );
	pTable->set_moneybindneed( mMoneyBindNeed );
	return ;
}

int CTplMineMap::Initialize()
{
	memset( mPos, 0, sizeof(mPos)) ;
	memset( mEvent, 0, sizeof(mEvent));
	mRadius = 0;
	mDropTable = 0;
	mDecBuff = 0;
	mOgreID = 0;
	mOgreNum = 0;
	mMapID = 0;
	mPosX = 0;
	mPosY = 0;
	mSpecial = 0;
	return 0;
}

void CTplMineMap::CreateTlvPB( bool bServer,PBItemMineMap* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	CTplItem::CreateTlvPB( bServer,pItem->mutable_baseinfo() );
	for ( int i = 0; i < (int)ARRAY_CNT( mPos ); i ++ )
	{
		PBMinePos* tpPos = pItem->add_minepos();
		tpPos->set_mapid( mPos[ i ].mMapID );
		tpPos->set_posx( mPos[ i ].mPosX );
		tpPos->set_posy( mPos[ i ].mPosY );
		tpPos->set_probability( mPos[ i ].mProbability );
	}
	if ( bServer )
	{
		for ( int i = 0; i < (int)ARRAY_CNT( mEvent ); i ++ )
		{
			PBMineEvent* tpEvent = pItem->add_mineevent();
			tpEvent->set_eventid( (int)mEvent[ i ].mEventID );
			tpEvent->set_touchrate( mEvent[ i ].mTouchRate );
		}
	}	
	pItem->set_radius( mRadius );
	pItem->set_droptable( mDropTable );
	pItem->set_decbuff( mDecBuff );
	pItem->set_ogreid( mOgreID );
	pItem->set_ogrenum( mOgreNum );
	pItem->set_mapid( mMapID );
	pItem->set_posx( mPosX );
	pItem->set_posy( mPosY );
	pItem->set_special( mSpecial );
	return ;
}

void CTplMineMap::SetFromPB( bool bServer,PBItemMineMap *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	CTplItem::SetFromPB( bServer,pItem->mutable_baseinfo() );
	for ( int i = 0; i < pItem->minepos_size() && i < (int)ARRAY_CNT( mPos ); i ++ )
	{
		PBMinePos* tpPos = pItem->mutable_minepos( i );
		mPos[ i ].mMapID = tpPos->mapid();
		mPos[ i ].mPosX = tpPos->posx();
		mPos[ i ].mPosY = tpPos->posy();
		mPos[ i ].mProbability = tpPos->probability();
	}
	if ( bServer )
	{
		for ( int i = 0; i < pItem->mineevent_size() && i < (int)ARRAY_CNT( mEvent ); i ++ )
		{
			PBMineEvent* tpEvent = pItem->mutable_mineevent( i );
			mEvent[ i ].mEventID = ( EVENT_TYPE ) tpEvent->eventid();
			mEvent[ i ].mTouchRate = tpEvent->touchrate();
		}
	}
	mRadius = pItem->radius();
	mDropTable = pItem->droptable();
	mDecBuff = pItem->decbuff();
	mOgreID = pItem->ogreid();
	mOgreNum = pItem->ogrenum();
	mMapID = pItem->mapid();
	mPosX = pItem->posx();
	mPosY = pItem->posy();
	mSpecial = pItem->special();
	return ;
}

int CTemplateBlock::Initialize()
{	
	memset( mPos, 0, sizeof(mPos));
	mSelect = CAN_SELECT;
	return 0;
}

void CTemplateBlock::CreateTlvPB( bool bServer,PBBlock* pBlock )
{
	if ( pBlock == NULL )
	{
		return ;
	}
	CTemplateOgre::CreateTlvPB( bServer, pBlock->mutable_baseinfo() );

	for( int i = 0; i < ( int )ARRAY_CNT( mPos ); i++ )
	{
		PBRelatively_Pos* tpPos = pBlock->add_pos( );
		tpPos->set_x( mPos[ i ].mX );
		tpPos->set_y( mPos[ i ].mY );
	}

	pBlock->set_select( mSelect );
	return ;
}

void CTemplateBlock::SetFromPB( bool bServer,PBBlock* pBlock)
{
	if ( pBlock == NULL )
	{
		return ;
	}
	CTemplateOgre::SetFromPB( bServer, pBlock->mutable_baseinfo() );
	for ( int i = 0; i < pBlock->pos_size() && i < (int)ARRAY_CNT( mPos ); i ++ )
	{
		PBRelatively_Pos* tpPos = pBlock->mutable_pos( i );
		mPos[ i ].mX = tpPos->x( );
		mPos[ i ].mY = tpPos->y( );
	}

	mSelect = ( IS_SELECT )pBlock->select( );

	return ;
}

int CTplPKValueTable::Initialize()
{
	mNavyBlue = 0;	
	mBlue = 0; 	
	mCambridgeBlue = 0;	
	mWhite = 0;	
	mRosiness = 0;	
	mRed = 0;	
	mCarmine = 0;	
	mYellowTime = 0;
	mDecValue = 0;
	memset( mDecExpRate, 0, sizeof(mDecExpRate)) ;
	memset( mTopLimitDecExp, 0, sizeof(mTopLimitDecExp));
	memset( mPriceChange, 0, sizeof(mPriceChange));
	mLineID = 0;
	mMapID = 0;
	mPosX = 0;
	mPosY = 0;
	memset( mDrop, 0, sizeof(mDrop));
	mFreeMapID = 0;
	mFreePosX = 0;
	mFreePosY = 0;
	mPrisonTime = 0;
	mBasePKValue = 0;
	memset( mPKValueLevel, 0, sizeof(mPKValueLevel));
	mDropRateInEnemy = 0;
	mPrisonBuffID = 0;
	return 0;
}

void CTplPKValueTable::SetFromPB(bool bServer, PBPKValueTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}
	mTempID			= pTable->tempid();
	mNavyBlue		= pTable->navyblue();
	mBlue			= pTable->blue();
	mCambridgeBlue	= pTable->cambridgeblue();
	mWhite			= pTable->white();
	mRosiness		= pTable->rosiness();
	mRed			= pTable->red();
	mCarmine		= pTable->carmine();
	mYellowTime		= pTable->yellowtime();
	mDecValue		= pTable->decvalue();
	for ( int i = 0; i < (int) ARRAY_CNT( mDecExpRate ) && i < pTable->decexprate_size(); i ++ )
	{
		mDecExpRate[ i ] = pTable->decexprate( i );
	}
	for ( int i = 0; i < (int) ARRAY_CNT( mTopLimitDecExp ) && i < pTable->toplimitdecexp_size(); i ++ )
	{
		mTopLimitDecExp[ i ] = pTable->toplimitdecexp( i );
	}
	for ( int i = 0; i < (int) ARRAY_CNT( mPriceChange ) && i < pTable->pricechange_size(); i ++ )
	{
		mPriceChange[ i ] = pTable->pricechange( i );
	}
	mLineID			= pTable->lineid();
	mMapID			= pTable->mapid();
	mPosX			= pTable->posx();
	mPosY			= pTable->posy();
	for ( int i = 0; i < (int) ARRAY_CNT( mDrop ) && i < pTable->drop_size(); i ++ )
	{
		PBPKDrop* tpDrop = pTable->mutable_drop( i );
		mDrop[ i ].mState = (PK_NAMESTATE)tpDrop->state();
		mDrop[ i ].mEquipDropRate = tpDrop->equipdroprate();
		mDrop[ i ].mBagDropRate1 = tpDrop->bagdroprate1();
		mDrop[ i ].mBagDropRate2 = tpDrop->bagdroprate2();
		mDrop[ i ].mGoPrisonRate = tpDrop->goprisonrate();
		mDrop[ i ].mPrisonTime = tpDrop->prisontime();
	}
	mFreeMapID	= pTable->freemapid();
	mFreePosX	= pTable->freeposx();
	mFreePosY	= pTable->freeposy();
	mPrisonTime	= pTable->prisontime();
	mBasePKValue = pTable->basepkvalue();
	for ( int i = 0; i < (int) ARRAY_CNT( mPKValueLevel ) && i < pTable->pkvaluelevel_size(); i ++ )
	{
		mPKValueLevel[ i ] = pTable->pkvaluelevel( i );
	}
	mDropRateInEnemy = pTable->droprateinenemy();
	mPrisonBuffID = pTable->prisonbuffid();
	return ;
}

void CTplPKValueTable::CreateTlvPB(bool bServer, PBPKValueTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}
	pTable->set_tempid( mTempID );
	pTable->set_navyblue( mNavyBlue );
	pTable->set_blue( mBlue );
	pTable->set_cambridgeblue( mCambridgeBlue );
	pTable->set_white( mWhite );
	pTable->set_rosiness( mRosiness );
	pTable->set_red( mRed );
	pTable->set_carmine( mCarmine );
	pTable->set_yellowtime( mYellowTime );
	pTable->set_decvalue( mDecValue );
	for ( int i = 0; i < (int) ARRAY_CNT( mDecExpRate ); i ++ )
	{
		pTable->add_decexprate( mDecExpRate[ i ] );
	}
	for ( int i = 0; i < (int) ARRAY_CNT( mTopLimitDecExp ); i ++ )
	{
		pTable->add_toplimitdecexp( mTopLimitDecExp[ i ] );
	}
	for ( int i = 0; i < (int) ARRAY_CNT( mPriceChange ); i ++ )
	{
		pTable->add_pricechange( mPriceChange[ i ] );
	}
	pTable->set_lineid( mLineID );
	pTable->set_mapid( mMapID );
	pTable->set_posx( mPosX );
	pTable->set_posy( mPosY );
	for ( int i = 0; i < (int) ARRAY_CNT( mDrop ); i ++ )
	{
		PBPKDrop* tpDrop = pTable->add_drop();
		tpDrop->set_state( mDrop[ i ].mState );
		tpDrop->set_equipdroprate( mDrop[ i ].mEquipDropRate );
		tpDrop->set_bagdroprate1( mDrop[ i ].mBagDropRate1 );
		tpDrop->set_bagdroprate2( mDrop[ i ].mBagDropRate2 );
		tpDrop->set_goprisonrate( mDrop[ i ].mGoPrisonRate );
		tpDrop->set_prisontime( mDrop[ i ].mPrisonTime );
	}
	pTable->set_freemapid( mFreeMapID );
	pTable->set_freeposx( mFreePosX );
	pTable->set_freeposy( mFreePosY );
	pTable->set_prisontime( mPrisonTime );
	pTable->set_basepkvalue( mBasePKValue );
	for ( int i = 0; i < (int) ARRAY_CNT( mPKValueLevel ); i ++ )
	{
		pTable->add_pkvaluelevel( mPKValueLevel[ i ] ) ;
	}
	pTable->set_droprateinenemy( mDropRateInEnemy );
	pTable->set_prisonbuffid( mPrisonBuffID );
	return ;
}

int CTplToken::Initialize()
{
	memset( mPos, 0, sizeof(mPos));
	mRadius = 0;
	mOgreID = 0;
	mOgreNum = 0;
	return 0;
}

void CTplToken::CreateTlvPB(bool bServer, PBItemToken *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	CTplItem::CreateTlvPB( bServer, pItem->mutable_baseinfo() );
	for ( int i = 0; i < (int) ARRAY_CNT( mPos ); i ++ )
	{
		PBMinePos* tpPos = pItem->add_pos();
		tpPos->set_mapid( mPos[ i ].mMapID );
		tpPos->set_posx( mPos[ i ].mPosX );
		tpPos->set_posy( mPos[ i ].mPosY );
		tpPos->set_probability( mPos[ i ].mProbability );
	}
	pItem->set_radius( mRadius );
	pItem->set_ogreid( mOgreID );
	pItem->set_ogrenum( mOgreNum );
	return ;
}

void CTplToken::SetFromPB(bool bServer, PBItemToken *pItem)
{
	if ( pItem == NULL )
	{
		return; 
	}
	CTplItem::SetFromPB( bServer, pItem->mutable_baseinfo() );
	for ( int i = 0; i < (int) ARRAY_CNT( mPos ) && i < pItem->pos_size(); i ++ )
	{
		PBMinePos* tpPos = pItem->mutable_pos( i );
		mPos[ i ].mMapID = tpPos->mapid();
		mPos[ i ].mPosX = tpPos->posx();
		mPos[ i ].mPosY = tpPos->posy();
		mPos[ i ].mProbability = tpPos->probability();
	}
	mRadius = pItem->radius();
	mOgreID = pItem->ogreid();
	mOgreNum = pItem->ogrenum();
	return ;
}

// 红包
int CTplItemRedPaper::Initialize()
{
	mBaseCharge = 0;
	memset( mMoney, 0, sizeof(mMoney) );
	memset( mBindMoney, 0, sizeof(mBindMoney));
	mRedPaperType = 0;
	return 0;
}

void CTplItemRedPaper::CreateTlvPB( bool bServer, PBItemRedPaper* pItem )
{
	if ( pItem == NULL )
	{
		return;
	}	
	CTplItem::CreateTlvPB( bServer, pItem->mutable_baseinfo() );	
	pItem->set_basecharge( mBaseCharge );
	
	for ( int i = 0; i < ( int )ARRAY_CNT( mMoney ); ++i  )
	{
		PBPaperValue *tpPaperValue = pItem->add_moneyinfo();			
		tpPaperValue->set_value( mMoney[i].mValue );
		tpPaperValue->set_probality( mMoney[i].mProbality );
	}
	
	for ( int i = 0; i < ( int )ARRAY_CNT( mBindMoney ); ++i  )
	{
		PBPaperValue *tpPaperValue = pItem->add_bindmoneyinfo();   		
		tpPaperValue->set_value( mBindMoney[i].mValue );
		tpPaperValue->set_probality( mBindMoney[i].mProbality );
	}	
	
	pItem->set_redpapertype( mRedPaperType );		
}

void CTplItemRedPaper::SetFromPB( bool bServer, PBItemRedPaper* pItem )
{  
	if ( pItem == NULL )
	{
		return;
	}
	
	CTplItem::SetFromPB( bServer, pItem->mutable_baseinfo() );
	
	for ( int i = 0; i < ( int )ARRAY_CNT( mMoney ) && i < pItem->moneyinfo_size(); ++i )
	{
		PBPaperValue *tpPaperValue = pItem->mutable_moneyinfo( i );
		mMoney[i].mValue = tpPaperValue->value();
		mMoney[i].mProbality = tpPaperValue->probality();
	}
	
	for ( int i = 0; i < ( int )ARRAY_CNT( mBindMoney ) && i < pItem->bindmoneyinfo_size(); ++i )
	{
		PBPaperValue *tpPaperValue = pItem->mutable_bindmoneyinfo( i );
		mBindMoney[i].mValue = tpPaperValue->value();
		mBindMoney[i].mProbality = tpPaperValue->probality();
	}
	
	mBaseCharge = pItem->basecharge();		
	mRedPaperType = pItem->redpapertype();
} 

// 黑狗丸
void CTplItemBlackPills::SetFromPB(  bool bServer, PBItemBlackPills* pItem )
{
	if ( pItem == NULL )
	{
		return;
	}
	CTplItem::SetFromPB( bServer, pItem->mutable_itembase() );
	mExchangeTime = pItem->exchangetime();
	mExpTableID = pItem->exptableid();
}

void CTplItemBlackPills::CreateTlvPB( bool bServer, PBItemBlackPills* pItem )
{
	if ( pItem == NULL )
	{
		return;
	}
	CTplItem::CreateTlvPB( bServer, pItem->mutable_itembase() );
	pItem->set_exchangetime( mExchangeTime );
	pItem->set_exptableid( mExpTableID );
}

int CTplItemBlackPills::Initialize()
{
	mExchangeTime = 0;
	mExpTableID = 0;
	return 0;
}

// 修炼经验丹
void CTplItemExpContainer::SetFromPB( bool bServer, PBItemExpContainer* pItem )
{
	if ( pItem == NULL )
	{
		return;
	}		
	
	CTplItem::SetFromPB( bServer, pItem->mutable_itembase() );
	mExpPillsID = pItem->exppillsid( );
	mMaxExp = pItem->maxexp( );
	mOgreExpRate = pItem->ogreexprate( );
}

void CTplItemExpContainer::CreateTlvPB( bool bServer, PBItemExpContainer* pItem )
{
	if ( pItem == NULL )
	{
		return;
	}
	CTplItem::CreateTlvPB( bServer, pItem->mutable_itembase() );
	
	pItem->set_maxexp( mMaxExp );
	pItem->set_exppillsid( mExpPillsID );
	pItem->set_ogreexprate( mOgreExpRate );
}

int CTplItemExpContainer::Initialize()
{
	mMaxExp = 0;
	mExpPillsID = 0;
	mOgreExpRate= 0;
	return 0;
}

// 经验丹
void CTplItemExpPills::SetFromPB( bool bServer, PBItemExpPills* pItem )
{
	if ( pItem == NULL )
	{
		return;
	}
	
	CTplItem::SetFromPB( bServer, pItem->mutable_itembase() );		
	mLevelLimit = pItem->levellimit( );
	mExp = pItem->exp( );
}

void CTplItemExpPills::CreateTlvPB( bool bServer, PBItemExpPills* pItem )
{
	if ( pItem == NULL )
	{
		return;
	}
	
	CTplItem::CreateTlvPB( bServer, pItem->mutable_itembase() );
	pItem->set_exp( mExp );
	pItem->set_levellimit( mLevelLimit );
}

int CTplItemExpPills::Initialize()
{
	mExp = 0;
	mLevelLimit = 0;
	return 0;
}



// 经验心德
void CTplItemExpTome::SetFromPB( bool bServer, PBItemExpTome* pItem )
{
	if ( pItem == NULL )
	{
		return;
	}

	CTplItem::SetFromPB( bServer, pItem->mutable_itembase() );		
	mLevelLimit = pItem->levellimit( );
	mExp = pItem->exp( );

	for ( int i = 0 ; i < (int)ARRAY_CNT(mOpenItemID) && i < pItem->openitemid_size()  ; ++i )
	{
		mOpenItemID[i] = pItem->openitemid(i);
	}
}

void CTplItemExpTome::CreateTlvPB( bool bServer, PBItemExpTome* pItem )
{
	if ( pItem == NULL )
	{
		return;
	}

	CTplItem::CreateTlvPB( bServer, pItem->mutable_itembase() );
	pItem->set_exp( mExp );
	pItem->set_levellimit( mLevelLimit );

	for ( int i = 0 ; i < (int)ARRAY_CNT( mOpenItemID ) ; ++i )
	{
		pItem->add_openitemid( mOpenItemID[i] );
	}
}

int CTplItemExpTome::Initialize()
{
	mExp = 0;
	mLevelLimit = 0;
	memset(mOpenItemID, 0, sizeof(mOpenItemID));
	return 0;
}





// 背包
int CTplKitBag::Initialize()
{
	mFaceGroupID = 0;
	mIndexNum = 0;	  
	return 0;
}

void CTplKitBag::CreateTlvPB( bool bServer, PBItemKitBag* pItem )
{
	if ( pItem == NULL )
	{
		return;
	}
	CTplItem::CreateTlvPB( bServer, pItem->mutable_baseinfo() );
	pItem->set_indexnum( mIndexNum );
}

void CTplKitBag::SetFromPB( bool bServer, PBItemKitBag* pItem )
{	
	if ( pItem == NULL )
	{
		return;
	}
	CTplItem::SetFromPB( bServer, pItem->mutable_baseinfo() );
	mIndexNum = pItem->indexnum();	
}


int CTplGiftBag::Initialize()
{	
	mMinLevel = 0 ;
	mMinMoney = 0;
	mMaxMoney = 0;
	mMinBindMoney = 0;
	mMaxBindMoney = 0;
	mMinSilverYB = 0;
	mMaxSilverYB = 0;
	mMinExp = 0;
	mMaxExp = 0;
	mDropTableID = 0;
	mDropInBag = 0;
	mCheckBag = 0;
	memset( mLevelTableID, 0, sizeof(mLevelTableID));
	mLiHuaID = 0;
	memset( mOpenItemID, 0, sizeof(mOpenItemID));
	memset( mOpenTableID, 0, sizeof(mOpenTableID));
	return 0;
}

void CTplGiftBag::CreateTlvPB( bool bServer, PBItemGiftBag* pItem )
{
	if ( pItem == NULL )
	{
		return;
	}
	CTplItem::CreateTlvPB( bServer, pItem->mutable_baseinfo() );

	if ( mDropTableID != 0)
	{
		pItem->set_droptableid( mDropTableID );
	}

	if ( mMinLevel != 0)
	{
		pItem->set_minlevel(mMinLevel);
	}

	if ( mMinBindMoney > 0)
	{
		pItem->set_minbindmoney(mMinBindMoney);
	}
	if ( mMaxBindMoney > 0)
	{
		pItem->set_maxbindmoney(mMaxBindMoney);
	}

	if ( mMinExp > 0)
	{
		pItem->set_minexp(mMinExp);
	}
	if ( mMaxExp > 0)
	{
		pItem->set_maxexp(mMaxExp);
	}

	if ( mMinMoney > 0 ) 
	{
		pItem->set_minmoney(mMinMoney);
	}
	if ( mMaxMoney > 0 ) 
	{
		pItem->set_maxmoney(mMaxMoney);
	}

	if ( mMinSilverYB > 0)
	{
		pItem->set_minsilveryb(mMinSilverYB);
	}
	if ( mMaxSilverYB > 0)
	{
		pItem->set_maxsilveryb(mMaxSilverYB);
	}

	pItem->set_dropinbag(mDropInBag);	
	pItem->set_checkbag(mCheckBag);

	for ( int i = 0; i < ( int )ARRAY_CNT( mLevelTableID ); ++i  )
	{
		pItem->add_leveltableid( mLevelTableID[ i ] );
	}
	pItem->set_lihuaid( mLiHuaID );
	for ( int i = 0; i < ( int )ARRAY_CNT( mOpenItemID ); ++i  )
	{
		pItem->add_openitemid( mOpenItemID[ i ] );
	}
	for ( int i = 0; i < ( int )ARRAY_CNT( mOpenTableID ); ++i  )
	{
		pItem->add_opentableid( mOpenTableID[ i ] );
	}
}

void CTplGiftBag::SetFromPB( bool bServer, PBItemGiftBag* pItem )
{	
	if ( pItem == NULL )
	{
		return;
	}

	CTplItem::SetFromPB( bServer, pItem->mutable_baseinfo() );

	mDropTableID = pItem->droptableid();
	mMinLevel = pItem->minlevel();
	mMinBindMoney = pItem->minbindmoney();
	mMaxBindMoney = pItem->maxbindmoney();
	mMinMoney = pItem->minmoney();
	mMaxMoney = pItem->maxmoney();
	mMinSilverYB = pItem->minsilveryb();
	mMaxSilverYB = pItem->maxsilveryb();
	mMinExp = pItem->minexp();
	mMaxExp = pItem->maxexp();
	mDropInBag = pItem->dropinbag();
	mCheckBag = pItem->checkbag();

	for ( int i = 0; i < ( int )ARRAY_CNT( mLevelTableID ) && i < pItem->leveltableid_size(); ++i  )
	{
		mLevelTableID[ i ] = pItem->leveltableid( i );
	}
	mLiHuaID = pItem->lihuaid();
	for ( int i = 0; i < ( int )ARRAY_CNT( mOpenItemID ) && i < pItem->openitemid_size(); ++i  )
	{
		mOpenItemID[ i ] = pItem->openitemid( i );
	}
	for ( int i = 0; i < ( int )ARRAY_CNT( mOpenTableID ) && i < pItem->opentableid_size(); ++i  )
	{
		mOpenTableID[ i ] = pItem->opentableid( i );
	}
}

int CTplItemAdapter::Initialize()
{
	mItemType = 0;		
	mRealType = 0;
	mRealTempID = 0;
	mMaxLevelMinus = 0;
	mParam = 0;
	mMaxValue = 0;
	mMinValue = 0;
	return 0;
}

void CTplItemAdapter::CreateTlvPB( bool bServer, PBItemAdapter* pAdapter )
{
	if ( pAdapter == NULL )
	{
		return ;
	}
	pAdapter->set_tempid( mTempID );
	pAdapter->set_itemtype( mItemType );
	pAdapter->set_realtype( mRealType );
	pAdapter->set_realtempid( mRealTempID );
	pAdapter->set_maxlevelminus( mMaxLevelMinus );
	pAdapter->set_param( mParam );
	pAdapter->set_maxvalue( mMaxValue );
	pAdapter->set_minvalue( mMinValue );
	return ;
}

void CTplItemAdapter::SetFromPB( bool bServer, PBItemAdapter *pAdapter)
{
	if ( pAdapter == NULL )
	{
		return ;
	}
	mTempID			= pAdapter->tempid();
	mItemType		= pAdapter->itemtype();
	mRealType		= pAdapter->realtype();
	mRealTempID		= pAdapter->realtempid();
	mMaxLevelMinus	= pAdapter->maxlevelminus();
	mParam			= pAdapter->param();
	mMaxValue		= pAdapter->maxvalue();
	mMinValue		= pAdapter->minvalue();
	return ;

}

int CTplHierogram::Initialize()
{	
	memset( mRes, 0, sizeof(mRes));
	mAlive = 0;
	mType = USE_SKILL;
	memset( mParame, 0, sizeof(mParame)) ;
	return 0;
}

void CTplHierogram::SetFromPB( bool bServer, PBTplHierogram* pTpl )
{
	mTempID = pTpl->tempid( );
	mType   = ( CTplHierogram::HIEROGRAM_TYPE )pTpl->type( );
	strncpy( mRes, pTpl->res( ).c_str( ), sizeof( mRes ) );
	mAlive = pTpl->alive( );
	mParame[ 0 ] = pTpl->parame1( );
	mParame[ 1 ] = pTpl->parame2( );
	return;
}

void CTplHierogram::CreateTlvPB( bool bServer, PBTplHierogram* pTpl )
{
	pTpl->set_tempid( mTempID );
	pTpl->set_type( mType );
	pTpl->set_res( mRes );
	pTpl->set_alive( mAlive );
	pTpl->set_parame1( mParame[ 0 ] );
	pTpl->set_parame2( mParame[ 1 ] );
	return;
}


int CTplTitleManager::Initialize()
{
	memset( mTitle, 0, sizeof(mTitle));
	return 0;
}

void CTplTitleManager::CreateTlvPB(bool bServer, PBTitleManager *pManager)
{
	if ( pManager == NULL )
	{
		return ;
	}
	pManager->set_tempid( mTempID );
	for ( int i = 0; i < (int) ( ARRAY_CNT( mTitle ) ); i ++ )
	{
		PBTitle* tpTitle = pManager->add_title();
		tpTitle->set_title( mTitle[ i ].mTitle );
		tpTitle->set_type( mTitle[ i ].mType );
		tpTitle->set_obtaincondition( mTitle[ i ].mObtainCondition );
		tpTitle->set_subcondition( mTitle[ i ].mSubCondition );
		tpTitle->set_ishidden( mTitle[ i ].mIsHidden );
		tpTitle->set_isreplace( mTitle[ i ].mIsReplace );
		tpTitle->set_color( mTitle[ i ].mColor );
		tpTitle->set_descrbe( mTitle[ i ].mDescribe );
		tpTitle->set_addproperty( mTitle[ i ].mAddProperty );
		tpTitle->set_validtime( mTitle[ i ].mValidTime );
		tpTitle->set_isteamtitle( mTitle[ i ].mIsTeamTitle );
		tpTitle->set_param( mTitle[ i ].mParm );
		tpTitle->set_detail( mTitle[ i ].mDetail );
	}
}

int CTemplateTalent::Initialize()
{
	memset( mStep, 0, sizeof(mStep));	
	memset( mExp, 0, sizeof(mExp));
	return 0;
}

void CTemplateTalent::CreateTlvPB( bool bServer,PBTemplateTalent* pTalent )
{
	if ( pTalent == NULL )
	{
		return ;
	}
	pTalent->set_tempid( mTempID );
	for( int i = 0; i < (int)ARRAY_CNT( mStep ); i++ )
	{
		PBStep* tpStep = pTalent->add_step( );
		tpStep->set_stageid( mStep[ i ].mStageID );
		tpStep->set_stagedesc( mStep[ i ].mStageDesc );
		tpStep->set_skilltree( mStep[ i ].mSkillTree );
		for ( int j = 0; j < (int)ARRAY_CNT(mStep[ i ].mNext); j++ )
		{
			tpStep->add_next( mStep[ i ].mNext[ j ] );
		}
		//tpStep->set_exp( mStep[ i ].mExp );
		tpStep->set_maxpoint( mStep[ i ].mMaxPoint );
		tpStep->set_layer( mStep[ i ].mLayer );
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mExp ); i++ )
	{
		pTalent->add_exp( mExp[ i ] );
	}
	return ;
}

void CTplTitleManager::SetFromPB(bool bServer, PBTitleManager *pManager)
{
	if ( pManager == NULL )
	{
		return ;
	}
	mTempID = pManager->tempid();
	for ( int i = 0; i < (int) ( ARRAY_CNT( mTitle ) ) && i < pManager->title_size(); i ++ )
	{
		PBTitle* tpTitle = pManager->mutable_title( i );
		strncpy( mTitle[ i ].mTitle, tpTitle->title().c_str(), sizeof( mTitle[ i ].mTitle ) - 1 );
		strncpy( mTitle[ i ].mDescribe, tpTitle->descrbe().c_str(), sizeof( mTitle[ i ].mDescribe ) - 1 );
		mTitle[ i ].mType = tpTitle->type();
		mTitle[ i ].mObtainCondition = tpTitle->obtaincondition();
		mTitle[ i ].mSubCondition = tpTitle->subcondition();
		mTitle[ i ].mIsHidden = tpTitle->ishidden();
		mTitle[ i ].mIsReplace = tpTitle->isreplace();
		mTitle[ i ].mColor = tpTitle->color();
		mTitle[ i ].mAddProperty = tpTitle->addproperty();
		mTitle[ i ].mValidTime = tpTitle->validtime();
		mTitle[ i ].mIsTeamTitle = tpTitle->isteamtitle();
		mTitle[ i ].mParm = tpTitle->param();
		strncpy( mTitle[ i ].mDetail, tpTitle->detail().c_str(), sizeof( mTitle[ i ].mDetail ) - 1 );
	}
}

void CTemplateTalent::SetFromPB( bool bServer,PBTemplateTalent* pTalent)
{
	if ( pTalent == NULL )
	{
		return ;
	}
	mTempID = pTalent->tempid();
	for ( int i = 0; i < pTalent->step_size() && i < (int)ARRAY_CNT( mStep ); i ++ )
	{
		PBStep* tpStep = pTalent->mutable_step( i );
		mStep[ i ].mStageID = tpStep->stageid();
		strncpy( mStep[ i ].mStageDesc, tpStep->stagedesc().c_str(), sizeof( mStep[ i ].mStageDesc ) - 1 );
		mStep[ i ].mSkillTree = tpStep->skilltree();
		for ( int j = 0; j < tpStep->next_size() && j < (int)ARRAY_CNT(mStep[ i ].mNext); j++ )
		{
			mStep[ i ].mNext[ j ] = tpStep->next( j );
		}
		//mStep[ i ].mExp = tpStep->exp();
		mStep[ i ].mMaxPoint = tpStep->maxpoint();
		mStep[ i ].mLayer = tpStep->layer();
	}
	for ( int i = 0; i < pTalent->exp_size() && i < (int)ARRAY_CNT( mExp ); i ++ )
	{
		mExp[ i ] = pTalent->exp(i);
	}

	return ;
}

// 生活技能表
int CTemplateLifeSkill::Initialize()
{
	mLifeSkillType = 0;  
	mLifeSkillKind = 0; 
	memset( mStudyDegree, 0, sizeof(mStudyDegree));
	memset( mDegreeStarted, 0, sizeof(mDegreeStarted));
	memset( mStudyMoney, 0, sizeof(mStudyMoney));
	memset( mLevel, 0, sizeof(mLevel)); 
	memset( mDegreeValue, 0, sizeof(mDegreeValue));
	memset( mSkillName, 0, sizeof(mSkillName));
	memset( mCharacterNameID, 0, sizeof(mCharacterNameID));	
	mSubSkillLevelLimit = 0;
	return 0;
}

void CTemplateLifeSkill::SetFromPB( bool bServer, PBTemplateLifeSkill* pLifeSkill )
{	
	if ( pLifeSkill == NULL )
	{
		return;
	}
	mTempID			= pLifeSkill->tempid();
	mLifeSkillType	= pLifeSkill->lifeskilltype();		
	mLifeSkillKind	= pLifeSkill->lifeskillkind(); 
	
	for ( int i = 0; i < pLifeSkill->degreestarted_size() && i < (int)ARRAY_CNT(mDegreeStarted); ++i )
	{
		mDegreeStarted[i] = pLifeSkill->degreestarted( i );
	}
	
	for ( int i = 0; i < pLifeSkill->degreevalue_size() && i < (int)ARRAY_CNT(mDegreeValue) ; ++i  )
	{
		mDegreeValue[i]	= pLifeSkill->degreevalue( i );
	}
	
	for ( int i = 0; i < pLifeSkill->studydegree_size() && i < (int)ARRAY_CNT(mStudyDegree); ++i )
	{
		mStudyDegree[i] = pLifeSkill->studydegree( i );
	}
	
	for ( int i = 0; i < pLifeSkill->studymoney_size() && i < (int)ARRAY_CNT(mStudyMoney); ++i )
	{
		mStudyMoney[i] = pLifeSkill->studymoney( i );
	}
	
	for ( int i = 0; i < pLifeSkill->level_size( ) && i < (int)ARRAY_CNT(mLevel); ++i )
	{	
		mLevel[i] = pLifeSkill->level( i );
	}
	
	for ( int i = 0; i < pLifeSkill->skillname_size() && i < (int)ARRAY_CNT(mSkillName); ++i )
	{
		memset( mSkillName[i], 0, sizeof( mSkillName[i] ) );
		strncpy( mSkillName[i], pLifeSkill->mutable_skillname( i )->c_str(), sizeof( mSkillName[i] ) -1 );
	}
	
	for ( int i = 0; i < pLifeSkill->characternameid_size() && i < (int)ARRAY_CNT(mCharacterNameID); ++i )
	{
		mCharacterNameID[i] = pLifeSkill->characternameid( i );
	}
	
	mSubSkillLevelLimit = pLifeSkill->subskilllevellimit( );
}

void CTemplateLifeSkill::CreateTlvPB( bool bServer, PBTemplateLifeSkill* pLifeSkill )
{		
	if ( pLifeSkill == NULL )
	{
		return;
	}
	 
	pLifeSkill->set_tempid( mTempID );
	pLifeSkill->set_lifeskilltype( mLifeSkillType );		
	pLifeSkill->set_lifeskillkind( mLifeSkillKind ); 
	
	for ( int i = 0; i < (int)ARRAY_CNT(mDegreeStarted); ++i )
	{
		pLifeSkill->add_degreestarted( mDegreeStarted[i] );
	}
	
	for ( int i = 0; i < (int)ARRAY_CNT(mDegreeValue) ; ++i  )
	{
		pLifeSkill->add_degreevalue( mDegreeValue[i] );
	}

	for ( int i = 0; i < (int)ARRAY_CNT(mStudyDegree); ++i )
	{
		pLifeSkill->add_studydegree( mStudyDegree[i] );
	}

	for ( int i = 0; i < (int)ARRAY_CNT(mStudyMoney); ++i )
	{
		pLifeSkill->add_studymoney( mStudyMoney[i] );
	}

	for ( int i = 0; i < (int)ARRAY_CNT(mLevel); ++i )
	{	
		pLifeSkill->add_level( mLevel[i] );
	}

	for ( int i = 0; i < (int)ARRAY_CNT(mSkillName); ++i )
	{
		pLifeSkill->add_skillname( mSkillName[i] );
	}

	for ( int i = 0; i < (int)ARRAY_CNT(mCharacterNameID); ++i )
	{	
		pLifeSkill->add_characternameid( mCharacterNameID[i] );
	}
	
	pLifeSkill->set_subskilllevellimit( mSubSkillLevelLimit );
}

int CTemplateDecompound::Initialize()
{
	memset( mWhitDecompond, 0, sizeof(mWhitDecompond));	// 白装的装备分解表
	memset( mBlueDecompond, 0, sizeof(mBlueDecompond));	// 蓝装的装备分解表
	memset( mPurpleDecompond, 0, sizeof(mPurpleDecompond));	// 紫装的装备分解表
	memset( mValue, 0, sizeof(mValue)) ;	
	memset( mProbality, 0, sizeof(mProbality));	
	memset( mDegreeValue, 0, sizeof(mDegreeValue));
	return 0;
}

// 装备分解表
void CTemplateDecompound::SetFromPB( bool bServer, PBCTemplateDecompound* pCompound )
{ 
	if ( pCompound == NULL )
	{
		return;
	}
	mTempID = pCompound->tempid();
	
	int i = 0;
	for ( i = 0; i < pCompound->whitedecompond_size() && i < (int)ARRAY_CNT( mWhitDecompond ); ++i )
	{
		mWhitDecompond[i].DegreeNeeded		= pCompound->mutable_whitedecompond( i )->degreeneeded();
		mWhitDecompond[i].FstItemID			= pCompound->mutable_whitedecompond( i )->fstitemid();
		mWhitDecompond[i].FstProbability	= pCompound->mutable_whitedecompond( i )->fstprobability();
		mWhitDecompond[i].SecItemID			= pCompound->mutable_whitedecompond( i )->secitemid();
		mWhitDecompond[i].SecProbability	= pCompound->mutable_whitedecompond( i )->secprobability();
		mWhitDecompond[i].ThirdItemID		= pCompound->mutable_whitedecompond( i )->thirditemid();
		mWhitDecompond[i].ThirdProbability	= pCompound->mutable_whitedecompond( i )->thirdprobability();
		mWhitDecompond[i].FstItemNum		= pCompound->mutable_whitedecompond( i )->fstitemnum();
		mWhitDecompond[i].SecItemNum		= pCompound->mutable_whitedecompond( i )->secitemnum();
		mWhitDecompond[i].ThirdItemNum		= pCompound->mutable_whitedecompond( i )->thirditemnum();
	}
	
	for ( i = 0; i < pCompound->bluedecompond_size() && i < (int)ARRAY_CNT( mBlueDecompond ); ++i )
	{
		mBlueDecompond[i].DegreeNeeded		= pCompound->mutable_bluedecompond( i )->degreeneeded();
		mBlueDecompond[i].FstItemID			= pCompound->mutable_bluedecompond( i )->fstitemid();
		mBlueDecompond[i].FstProbability	= pCompound->mutable_bluedecompond( i )->fstprobability();
		mBlueDecompond[i].SecItemID			= pCompound->mutable_bluedecompond( i )->secitemid();
		mBlueDecompond[i].SecProbability	= pCompound->mutable_bluedecompond( i )->secprobability();
		mBlueDecompond[i].ThirdItemID		= pCompound->mutable_bluedecompond( i )->thirditemid();
		mBlueDecompond[i].ThirdProbability	= pCompound->mutable_bluedecompond( i )->thirdprobability();		
		mBlueDecompond[i].FstItemNum		= pCompound->mutable_bluedecompond( i )->fstitemnum();
		mBlueDecompond[i].SecItemNum		= pCompound->mutable_bluedecompond( i )->secitemnum();
		mBlueDecompond[i].ThirdItemNum		= pCompound->mutable_bluedecompond( i )->thirditemnum();
	}

	for ( i = 0; i < pCompound->purpledecompond_size() && i < (int)ARRAY_CNT( mPurpleDecompond ); ++i )
	{
		mPurpleDecompond[i].DegreeNeeded		= pCompound->mutable_purpledecompond( i )->degreeneeded();
		mPurpleDecompond[i].FstItemID			= pCompound->mutable_purpledecompond( i )->fstitemid();
		mPurpleDecompond[i].FstProbability		= pCompound->mutable_purpledecompond( i )->fstprobability();
		mPurpleDecompond[i].SecItemID			= pCompound->mutable_purpledecompond( i )->secitemid();
		mPurpleDecompond[i].SecProbability		= pCompound->mutable_purpledecompond( i )->secprobability();
		mPurpleDecompond[i].ThirdItemID			= pCompound->mutable_purpledecompond( i )->thirditemid();
		mPurpleDecompond[i].ThirdProbability	= pCompound->mutable_purpledecompond( i )->thirdprobability();
		mPurpleDecompond[i].FstItemNum			= pCompound->mutable_purpledecompond( i )->fstitemnum();
		mPurpleDecompond[i].SecItemNum			= pCompound->mutable_purpledecompond( i )->secitemnum();
		mPurpleDecompond[i].ThirdItemNum		= pCompound->mutable_purpledecompond( i )->thirditemnum();
	}	
	
	for ( i = 0; i < pCompound->value_size() && i < (int)ARRAY_CNT( mValue ); ++i )
	{
		mValue[i] = pCompound->value( i );
	}
	
	for ( i = 0; i < pCompound->probability_size( ) && i < (int)ARRAY_CNT( mProbality ); ++i )
	{
		mProbality[i] = pCompound->probability( i );
	}
			
	for ( i = 0; i < pCompound->degreevalue_size( ) && i < (int)ARRAY_CNT( mDegreeValue ); ++i )
	{
		mDegreeValue[i] = pCompound->degreevalue( i );
	}				
}

void CTemplateDecompound::CreateTlvPB( bool bServer, PBCTemplateDecompound* pCompound )
{
	if ( pCompound == NULL )
	{
		return;
	}
	
	pCompound->set_tempid( mTempID );

	int i = 0;
	for ( i = 0;  i < (int)ARRAY_CNT( mWhitDecompond ); ++i )
	{
		PBDecompondTable  *tpDecompond = pCompound->add_whitedecompond( );
		tpDecompond->set_degreeneeded( mWhitDecompond[i].DegreeNeeded );
		
		tpDecompond->set_fstitemid( mWhitDecompond[i].FstItemID );
		tpDecompond->set_fstitemnum( mWhitDecompond[i].FstItemNum );
		tpDecompond->set_fstprobability( mWhitDecompond[i].FstProbability );
		
		tpDecompond->set_secitemid( mWhitDecompond[i].SecItemID );
		tpDecompond->set_secitemnum( mWhitDecompond[i].SecItemNum );
		tpDecompond->set_secprobability( mWhitDecompond[i].SecProbability );
		
		tpDecompond->set_thirditemid( mWhitDecompond[i].ThirdItemID );
		tpDecompond->set_thirdprobability( mWhitDecompond[i].ThirdProbability );
		tpDecompond->set_thirditemnum( mWhitDecompond[i].ThirdItemNum );
	}

	for ( i = 0;i < (int)ARRAY_CNT( mBlueDecompond ); ++i )
	{
		PBDecompondTable  *tpDecompond = pCompound->add_bluedecompond( );
		tpDecompond->set_degreeneeded( mBlueDecompond[i].DegreeNeeded );
		
		tpDecompond->set_fstitemid( mBlueDecompond[i].FstItemID );
		tpDecompond->set_fstitemnum( mBlueDecompond[i].FstItemNum );		
		tpDecompond->set_fstprobability( mBlueDecompond[i].FstProbability );
		
		tpDecompond->set_secitemid( mBlueDecompond[i].SecItemID );		
		tpDecompond->set_secitemnum( mBlueDecompond[i].SecItemNum );
		tpDecompond->set_secprobability( mBlueDecompond[i].SecProbability );
		
		tpDecompond->set_thirditemid( mBlueDecompond[i].ThirdItemID );
		tpDecompond->set_thirditemnum( mBlueDecompond[i].ThirdItemNum );
		tpDecompond->set_thirdprobability( mBlueDecompond[i].ThirdProbability );
	}

	for ( i = 0; i < (int)ARRAY_CNT( mPurpleDecompond ); ++i )
	{
		PBDecompondTable  *tpDecompond =   pCompound->add_purpledecompond( );
		tpDecompond->set_degreeneeded( mPurpleDecompond[i].DegreeNeeded );
		
		tpDecompond->set_fstitemid( mPurpleDecompond[i].FstItemID );
		tpDecompond->set_fstprobability( mPurpleDecompond[i].FstProbability );
		tpDecompond->set_fstitemnum( mPurpleDecompond[i].FstItemNum );
		
		tpDecompond->set_secitemid( mPurpleDecompond[i].SecItemID );
		tpDecompond->set_secprobability( mPurpleDecompond[i].SecProbability );
		tpDecompond->set_secitemnum( mPurpleDecompond[i].SecItemNum );
		
		tpDecompond->set_thirditemid( mPurpleDecompond[i].ThirdItemID );
		tpDecompond->set_thirdprobability( mPurpleDecompond[i].ThirdProbability );
		tpDecompond->set_thirditemnum( mPurpleDecompond[i].ThirdItemNum );
	}	

	for ( i = 0; i < (int)ARRAY_CNT( mValue ); ++i )
	{
		pCompound->add_value( mValue[i] );
	}

	for ( i = 0; i < (int)ARRAY_CNT( mProbality ); ++i )
	{
		pCompound->add_probability( mProbality[i] );
	}

	for ( i = 0; i < (int)ARRAY_CNT( mDegreeValue ); ++i )
	{
		pCompound->add_degreevalue( mDegreeValue[i] );
	}	
}

int CTplMagicWeaponProIncTable::Initialize()
{
	memset( mValues, 0, sizeof(mValues));
	return 0;
}

void CTplMagicWeaponProIncTable::CreateTlvPB( bool bServer, PBTplMagicWeaponProIncTable* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}

	pItem->set_tempid(mTempID);

	for( int i = 0; i < (int)ARRAY_CNT( mValues ); i++ )
	{
		PBMagicWeaponProIncValue* tpSingle = pItem->add_values();
		tpSingle->set_levelexp( mValues[i].mLevelExp ); 
		tpSingle->set_levelpoints( mValues[i].mLevelPoints ); 
		tpSingle->set_levelprocessval( mValues[i].mLevelProcessVal ); 
		tpSingle->set_levelprocesscurel( mValues[i].mLevelProcessCurel ); 
		tpSingle->set_levelprocesscurelval( mValues[i].mLevelProcessCurelVal ); 
		tpSingle->set_levelkeen( mValues[i].mLevelKeen ); 
		tpSingle->set_levelcalm( mValues[i].mLevelCalm ); 
		tpSingle->set_levelmagic( mValues[i].mLevelMagic ); 
		tpSingle->set_levelbier( mValues[i].mLevelBier ); 
		tpSingle->set_levelimmortal( mValues[i].mLevelImmortal ); 
	}
	return ;
}

void CTplMagicWeaponProIncTable::SetFromPB( bool bServer, PBTplMagicWeaponProIncTable* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}

	mTempID = pItem->tempid();

	for ( int i = 0; i < pItem->values_size() && i < (int)ARRAY_CNT( mValues ); i ++ )
	{
		PBMagicWeaponProIncValue* tpSingle = pItem->mutable_values(i);
		mValues[i].mLevelExp = tpSingle->levelexp();
		mValues[i].mLevelPoints = tpSingle->levelpoints();
		mValues[i].mLevelProcessVal = tpSingle->levelprocessval();
		mValues[i].mLevelProcessCurel = tpSingle->levelprocesscurel();
		mValues[i].mLevelProcessCurelVal = tpSingle->levelprocesscurelval();
		mValues[i].mLevelKeen = tpSingle->levelkeen();
		mValues[i].mLevelCalm = tpSingle->levelcalm();
		mValues[i].mLevelMagic = tpSingle->levelmagic();
		mValues[i].mLevelBier = tpSingle->levelbier();
		mValues[i].mLevelImmortal = tpSingle->levelimmortal();
	}
}

// 装备兑换表
void CTempateEquipExchangeTable::SetFromPB( bool bServer, PBEquipExchangeTable* pExchange )
{
	if ( pExchange == NULL )
	{						
		return;
	}	
	mTempID = pExchange->tempid();	
	for (  int i = 0; i < pExchange->composeid_size() && i < (int)ARRAY_CNT(mMultiComposeID); ++i )
	{
		mMultiComposeID[i] = pExchange->composeid( i );
	}	
	mExchangeType = pExchange->exchangetype();	
}

// 装备兑换表
int CTempateEquipExchangeTable::Initialize()
{	
	memset( mMultiComposeID, 0, sizeof(mMultiComposeID));	// 兑换的配方ID
	mExchangeType = 0;
	return 0;
}

void CTempateEquipExchangeTable::CreateTlvPB( bool bServer, PBEquipExchangeTable* pExchange )
{
	if ( pExchange == NULL )
	{
		return;
	}
	pExchange->set_tempid( mTempID );
	for ( int i = 0; i < (int)ARRAY_CNT(mMultiComposeID); ++i )
	{
		pExchange->add_composeid( mMultiComposeID[i] );
	}
	pExchange->set_exchangetype( mExchangeType );
}

// 好感、恶感
int CTplJudgeConfig::Initialize()
{
	mOpenCD = 0;	
	memset( mJudgeLevel, 0, sizeof(mJudgeLevel));
	mScoreBase = 0;
	mScoreRate = 0;
	mScoreLevel = 0;
	return 0;
}

void CTplJudgeConfig::CreateTlvPB(bool bServer, PBJudgeConfig *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}
	pTable->set_tempid( mTempID );
	pTable->set_opencd( mOpenCD );
	
	for ( int i = 0; i < (int)ARRAY_CNT( mJudgeLevel ); i ++ )
	{
		PBJudgeLevel* tpLevel = pTable->add_judgelevel();
		tpLevel->set_judgetype( mJudgeLevel[ i ].mType );
		tpLevel->set_levelmin( mJudgeLevel[ i ].mLevelMin );
		tpLevel->set_levelmax( mJudgeLevel[ i ].mLevelMax );
		tpLevel->set_leveltype( mJudgeLevel[ i ].mLevelType );
		tpLevel->set_star( mJudgeLevel[ i ].mStar );
	}

	pTable->set_scorebase( mScoreBase );
	pTable->set_scorerate( mScoreRate );
	pTable->set_scorelevel( mScoreLevel );
}

void CTplJudgeConfig::SetFromPB(bool bServer, PBJudgeConfig *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	mTempID = pTable->tempid();
	mOpenCD = pTable->opencd();

	for ( int i = 0; i < pTable->judgelevel_size() && i < (int)ARRAY_CNT(mJudgeLevel); i ++ )
	{
		PBJudgeLevel* tpLevel = pTable->mutable_judgelevel( i );
		mJudgeLevel[ i ].mType = tpLevel->judgetype();
		mJudgeLevel[ i ].mLevelMin = tpLevel->levelmin();
		mJudgeLevel[ i ].mLevelMax = tpLevel->levelmax();
		mJudgeLevel[ i ].mLevelType = tpLevel->judgetype();
		mJudgeLevel[ i ].mStar = tpLevel->star();
	}

	mScoreBase = pTable->scorebase();
	mScoreRate = pTable->scorerate();
	mScoreLevel = pTable->scorelevel();
}


// 生活技能
int CLifeSkillInfo::Initialize()
{
	spLifeSkillTable = NULL;
	memset( mComposeInfo, 0, sizeof( mComposeInfo ) );
	mComposeKindNum	= 0;	
	memset( mCompose, 0, sizeof( mCompose ) );
	mComposeNum	= 0;
	return 0;
}

// 插入一种新配方
int CLifeSkillInfo::InsertComposeInfo( int nComposeID, int nComposeKind, int nDegreeValue, int nComposeType )
{	
	int i = 0;
	for ( ; i < mComposeKindNum; ++i )
	{
		if ( nComposeKind == mComposeInfo[i].mComposeType )
		{
			break;
		}
	}
	mComposeInfo[i].mComposeID[mComposeInfo[i].mComposeNum++]  = nComposeID;
	if ( i >= mComposeKindNum )
	{
		mComposeInfo[i].mComposeType = nComposeKind;
		++mComposeKindNum;
	}	
	
	// 如果是npc可学的话，插入列表
	if ( nComposeType & 1 )
	{
		InsertCompose( nComposeID, nDegreeValue );
	}  	
	return 0;								
}

int CLifeSkillInfo::InsertCompose( int nComposeID, int nDegreeValue )
{
	int i = 0;
	for ( i = 0; i < mComposeNum; ++i )
	{
		if ( nDegreeValue <= mCompose[i].mDegreeValue )
		{
			break;
		}	 									
	}
	
	for ( int j = mComposeNum; j > i ; --j )
	{
		mCompose[j].mComposeID		= mCompose[j-1].mComposeID;
		mCompose[j].mDegreeValue	= mCompose[j-1].mDegreeValue;				
	}
	
	mCompose[i].mComposeID = nComposeID;
	mCompose[i].mDegreeValue = nDegreeValue;
	++mComposeNum;
	return 0;
}

void CMagicWeaponDecomposeTable::SetFromPB( bool bServer, PBMagicWeaponDecomposeTable* pItem )
{
	mTempID=pItem->tempid();
	mNeedMoneyType = pItem->needmoneytype();
	mNeedMoney = pItem->needmoney();
	for (int i =0 ; i < pItem->items_size() && i < (int)ARRAY_CNT(mItems); ++i)
	{
		SingleMWDecomposeData sData=pItem->items(i);
		mItems[i].mTempID = 	sData.tempid();
		mItems[i].mProb   =		sData.prob(  );
		mItems[i].mNumMin = 	sData.nummin();
		mItems[i].mNumMax = 	sData.nummax();
	}
}
void CMagicWeaponDecomposeTable::CreateTlvPB( bool bServer, PBMagicWeaponDecomposeTable* pItem )
{
	pItem->set_tempid( mTempID );
	pItem->set_needmoneytype(mNeedMoneyType);
	pItem->set_needmoney(mNeedMoney);
	for (unsigned int i =0 ; i < ARRAY_CNT(mItems); ++i)
	{
		SingleMWDecomposeData* pData=pItem->add_items();
		pData->set_tempid(mItems[i].mTempID);
		pData->set_prob(mItems[i].mProb);
		pData->set_nummin(mItems[i].mNumMin);
		pData->set_nummax(mItems[i].mNumMax);
	}
}

int  CMagicWeaponDecomposeTable::Initialize()
{
	mNeedMoneyType=0;
	mNeedMoney =0;
	memset(mItems, 0 , sizeof(mItems));
	return 0;
}

void CMagicWeaponQuickUpTable::SetFromPB( bool bServer, PBMagicWeaponQuickUpTable* pItem )
{
	mTempID=pItem->tempid();
	mNeedItem		=	pItem->needitem();		
	mNeedItemNum	=	pItem->needitemnum();	
	mNeedMoneyType	=	pItem->needmoneytype();		
	mNeedMoney		=	pItem->needmoney();		
	mSuccessProb	=	pItem->successprob();	
	mNewMWID		=	pItem->newmwid();
	mNeedLevel		=	pItem->needlevel();
	mLeastLeve      =   pItem->leastleve();
}

void CMagicWeaponQuickUpTable::CreateTlvPB( bool bServer, PBMagicWeaponQuickUpTable* pItem )
{
	pItem->set_tempid( mTempID );
	pItem->set_needitem		(mNeedItem		);		
	pItem->set_needitemnum	(mNeedItemNum	);	
	pItem->set_needmoneytype(mNeedMoneyType	);		
	pItem->set_needmoney	(mNeedMoney		);		
	pItem->set_successprob	(mSuccessProb	);	
	pItem->set_newmwid		(mNewMWID		);
	pItem->set_needlevel(mNeedLevel);
	pItem->set_leastleve(mLeastLeve);
}

int  CMagicWeaponQuickUpTable::Initialize()
{
	mNeedItem		=	0;
	mNeedItemNum	=	0;
	mNeedMoneyType	=	0;
	mNeedMoney		=	0;
	mSuccessProb	=	0;
	mNewMWID		=	0;
	mLeastLeve		=	0;
	return 0;
}


// CTplSkillStorage

int CTplSkillStorage::Initialize()
{
	memset( mValues, 0, sizeof(mValues));
	return 0;
}

void CTplSkillStorage::CreateTlvPB( bool bServer, PBSkillStorage* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}

	pItem->set_tempid(mTempID);

	for( int i = 0; i < (int)ARRAY_CNT( mValues ); i++ )
	{
		PBSingleSkillStorage* tpSingle = pItem->add_values();
		tpSingle->set_skillid( mValues[i].mSkillID ); 
		tpSingle->set_firstprob( mValues[i].mFirstProb ); 
		tpSingle->set_otherprob( mValues[i].mOtherProb ); 
	}
	return ;
}


void CTplSkillStorage::SetFromPB( bool bServer, PBSkillStorage* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}

	mTempID = pItem->tempid();

	for ( int i = 0; i < pItem->values_size() && i < (int)ARRAY_CNT( mValues ); i ++ )
	{
		PBSingleSkillStorage* tpSingle = pItem->mutable_values(i);
		mValues[i].mSkillID = tpSingle->skillid();
		mValues[i].mFirstProb = tpSingle->firstprob();
		mValues[i].mOtherProb = tpSingle->otherprob();
	}

	return ;
}


// CTplMagicWeaponSkillBoxProTable
int CTplMagicWeaponSkillBoxProTable::Initialize()
{
	memset( mValues, 0, sizeof(mValues));
	return 0;
}

void CTplMagicWeaponSkillBoxProTable::CreateTlvPB( bool bServer, PBMagicWeaponSkillBoxProTable* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}

	pItem->set_tempid(mTempID);

	for( int i = 0; i < (int)ARRAY_CNT( mValues ); i++ )
	{
		PBSkillBoxProperty* tpSingle = pItem->add_values();
		tpSingle->set_skillboxcolor( mValues[i].mSkillBoxColor ); 
		tpSingle->set_skillboxneedval( mValues[i].mSkillBoxNeedVal ); 
		tpSingle->set_initstate( mValues[i].mInitState ); 
		tpSingle->set_initskillid( mValues[i].mInitSkillID ); 
		tpSingle->set_skillboxtype( mValues[i].mSkillBoxType ); 
		tpSingle->set_skillboxskill( mValues[i].mSkillBoxSkill ); 
		tpSingle->set_skillboxactmoney( mValues[i].mSkillBoxActMoney ); 
		tpSingle->set_skillboxskillstorage( mValues[i].mSkillBoxSkillStorage ); 
	}
	return ;
}


void CTplMagicWeaponSkillBoxProTable::SetFromPB( bool bServer, PBMagicWeaponSkillBoxProTable* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}

	mTempID = pItem->tempid();

	for ( int i = 0; i < pItem->values_size() && i < (int)ARRAY_CNT( mValues ); i ++ )
	{
		PBSkillBoxProperty* tpSingle = pItem->mutable_values(i);
		mValues[i].mSkillBoxColor = tpSingle->skillboxcolor();
		mValues[i].mSkillBoxNeedVal = tpSingle->skillboxneedval();
		mValues[i].mInitState = tpSingle->initstate();
		mValues[i].mInitSkillID = tpSingle->initskillid();
		mValues[i].mSkillBoxType = tpSingle->skillboxtype();
		mValues[i].mSkillBoxSkill = tpSingle->skillboxskill();
		mValues[i].mSkillBoxActMoney = tpSingle->skillboxactmoney();
		mValues[i].mSkillBoxSkillStorage = tpSingle->skillboxskillstorage();
	}

	return ;
}

int CTplMWProJudgeEffTable::Initialize()
{
	memset( mProbFirst, 0, sizeof( mProbFirst ));
	memset( mProbSecond, 0, sizeof( mProbSecond));
	memset( mShowName, 0, sizeof(mShowName));
	memset( mJudgeLevel, 0, sizeof(mJudgeLevel ));
	return 0;
}

void CTplMWProJudgeEffTable::SetFromPB( bool bServer, PBMWProJudgeEffTable* pTable )
{
	if ( pTable == NULL )
		return;
	mTempID = pTable->tempid();
	for ( int i = 0 ; i < MAX_MW_PERCENT_NUM && i < pTable->probfirst_size(); ++i )
	{
		mProbFirst[i]  = pTable->probfirst(i);
		mProbSecond[i] = pTable->probsecond(i);
		strncpy( mShowName[i], pTable->showname(i).c_str(), STRING32 );
		mJudgeLevel[i] = pTable->judgelevel(i);
	}
}

void CTplMWProJudgeEffTable::CreateTlvPB( bool bServer, PBMWProJudgeEffTable* pTable )
{
	if ( pTable == NULL )
		return;

	pTable->set_tempid( mTempID );
	for ( int i = 0 ; i < MAX_MW_PERCENT_NUM ; ++i )
	{
		pTable->add_probfirst( mProbFirst[i] );
		pTable->add_probsecond( mProbSecond[i] );
		pTable->add_showname(mShowName[i]);
		pTable->add_judgelevel( mJudgeLevel[i] );
	}
}

int CTplMWProJudgeSpirTable::Initialize()
{
	memset( mProbFirst, 0, sizeof(mProbFirst) );
	memset( mProbSecond, 0, sizeof(mProbSecond));
	memset( mShowName, 0, sizeof(mShowName));		
	memset( mJudgeLevel, 0, sizeof(mJudgeLevel));
	return 0;
}

void CTplMWProJudgeSpirTable::SetFromPB( bool bServer, PBMWProJudgeSpirTable* pTable )
{
	if ( pTable == NULL )
		return;
	mTempID = pTable->tempid();
	for ( int i = 0 ; i < MAX_MW_PERCENT_NUM && i < pTable->probfirst_size(); ++i )
	{
		mProbFirst[i]  = pTable->probfirst(i);
		mProbSecond[i] = pTable->probsecond(i);
		strncpy( mShowName[i], pTable->showname(i).c_str(), STRING32 );
		mJudgeLevel[i] = pTable->judgelevel(i);
	}
}

void CTplMWProJudgeSpirTable::CreateTlvPB( bool bServer, PBMWProJudgeSpirTable* pTable )
{
	if ( pTable == NULL )
		return;

	pTable->set_tempid( mTempID );
	for ( int i = 0 ; i < MAX_MW_PERCENT_NUM ; ++i )
	{
		pTable->add_probfirst( mProbFirst[i] );
		pTable->add_probsecond( mProbSecond[i] );
		pTable->add_showname(mShowName[i]);
		pTable->add_judgelevel( mJudgeLevel[i] );
	}
}


int CTplMWLevelQuotietyTab::Initialize()
{
	memset( mKeenQuotiety, 0, sizeof(mKeenQuotiety ));
	memset( mCalmQuotiety, 0, sizeof(mCalmQuotiety));
	memset( mMagicQuotiety, 0, sizeof(mMagicQuotiety ));
	memset( mBierQuotiety, 0, sizeof(mBierQuotiety ));
	memset( mImmorQuotiety, 0, sizeof( mImmorQuotiety )) ;
	return 0;
}

void CTplMWLevelQuotietyTab::SetFromPB( bool bServer, PBMWLevelQuotietyTab* pTable )
{
	if ( pTable == NULL )
		return;
	mTempID = pTable->tempid();
	for ( int i = 0 ; i < 20 && i < pTable->keenquotiety_size(); ++i )
	{
		mKeenQuotiety[i]  = pTable->keenquotiety(i);
	}

	for ( int i = 0 ; i < 20 && i < pTable->calmquotiety_size(); ++i )
	{
		mCalmQuotiety[i]  = pTable->calmquotiety(i);
	}

	for ( int i = 0 ; i < 20 && i < pTable->magicquotiety_size(); ++i )
	{
		mMagicQuotiety[i]  = pTable->magicquotiety(i);
	}

	for ( int i = 0 ; i < 20 && i < pTable->bierquotiety_size(); ++i )
	{
		mBierQuotiety[i]  = pTable->bierquotiety(i);
	}

	for ( int i = 0 ; i < 20 && i < pTable->immorquotiety_size(); ++i )
	{
		mImmorQuotiety[i]  = pTable->immorquotiety(i);
	}
}

void CTplMWLevelQuotietyTab::CreateTlvPB( bool bServer, PBMWLevelQuotietyTab* pTable )
{
	if ( pTable == NULL )
		return;

	pTable->set_tempid( mTempID );
	for ( int i = 0 ; i < 20 ; ++i )
	{
		pTable->add_keenquotiety( mKeenQuotiety[i] );
	}

	for ( int i = 0 ; i < 20 ; ++i )
	{
		pTable->add_calmquotiety( mCalmQuotiety[i] );
	}

	for ( int i = 0 ; i < 20 ; ++i )
	{
		pTable->add_magicquotiety( mMagicQuotiety[i] );
	}

	for ( int i = 0 ; i < 20 ; ++i )
	{
		pTable->add_bierquotiety( mBierQuotiety[i] );
	}

	for ( int i = 0 ; i < 20 ; ++i )
	{
		pTable->add_immorquotiety( mImmorQuotiety[i] );
	}
}

int CTemplateMultiBuffConfig::Initialize()
{
	memset( mEvent, 0, sizeof(mEvent));
	memset( mEventParam, 0, sizeof(mEventParam));
	memset( mEventBuff, 0, sizeof(mEventBuff));
	return 0;
}

void CTemplateMultiBuffConfig::CreateTlvPB( bool bServer, PBMultiBuff* pMBuff )
{
	if ( pMBuff == NULL)
	{
		return ;
	}
	pMBuff->set_tempid( mTempID );
	for ( int i = 0; i < (int) ARRAY_CNT( mEvent ); i ++ )
	{
		PBMultiBuffEvent* tpEvent = pMBuff->add_buffevent();
		tpEvent->set_eventid( mEvent[ i ].mEventID );
		tpEvent->set_teamflag( mEvent[ i ].mTeamFlag );
		tpEvent->set_distance( mEvent[ i ].mDistance );
		tpEvent->set_buffid( mEvent[ i ].mBuffID );
        
        for (int k = 0; k < (int)ARRAY_CNT( mEvent[ i ].mRepetionIndex ); k++)
        {
            tpEvent->add_repetionindex(mEvent[ i ].mRepetionIndex[k]);
        }
	}
	for ( int i = 0; i < (int) ARRAY_ROW( mEventParam ); i ++ )
	{
		for ( int j = 0; j < (int) ARRAY_COL( mEventParam ); j ++ )
		{
			pMBuff->add_param( mEventParam[ i ][ j ] );
		}
	}
	for ( int i = 0; i < (int) ARRAY_ROW( mEventBuff ); i ++ )
	{
		for ( int j = 0; j < (int) ARRAY_COL( mEventBuff ); j ++ )
		{
			pMBuff->add_newbuff( mEventBuff[ i ][ j ] );
		}
	}
	return ;
}

void CTemplateMultiBuffConfig::SetFromPB( bool bServer, PBMultiBuff* pMBuff )
{
	if ( pMBuff == NULL )
	{
		return ;
	}
	mTempID = pMBuff->tempid();
	for ( int i = 0; i < pMBuff->buffevent_size() && i < (int)ARRAY_CNT( mEvent ); i ++ )
	{
		PBMultiBuffEvent* tpEvent = pMBuff->mutable_buffevent( i );
		mEvent[ i ].mEventID = tpEvent->eventid();
		mEvent[ i ].mTeamFlag = tpEvent->teamflag();
		mEvent[ i ].mDistance = tpEvent->distance();
		mEvent[ i ].mBuffID = tpEvent->buffid();

        for (int k = 0; k < (int)ARRAY_CNT( mEvent[ i ].mRepetionIndex ); k++)
        {
            mEvent[ i ].mRepetionIndex[k] = tpEvent->repetionindex(k); 
        }
	}
	int tTotal = 0;
	for ( int i = 0; i < (int) ARRAY_ROW( mEventParam ); i ++ )
	{
		for ( int j = 0; j < (int) ARRAY_COL( mEventParam ); j ++ )
		{
			tTotal = (i*ARRAY_COL( mEventParam )) + j ;
			if ( tTotal >= pMBuff->param_size() )
				break;
			mEventParam[ i ][ j ] = pMBuff->param( tTotal );
		}
	}
	tTotal = 0;
	for ( int i = 0; i < (int) ARRAY_ROW( mEventBuff ); i ++ )
	{
		for ( int j = 0; j < (int) ARRAY_COL( mEventBuff ); j ++ )
		{
			tTotal = (i*ARRAY_COL( mEventBuff )) + j ;
			if ( tTotal >= pMBuff->newbuff_size() )
				break;
			mEventBuff[ i ][ j ] = pMBuff->newbuff( tTotal );
		}
	}
	return ;
}

int CTemplateCDGroupTable::Initialize()
{
	memset( mCDTime, 0, sizeof(mCDTime));
	return 0;
}

void CTemplateCDGroupTable::CreateTlvPB( bool bServer, PBCDGroupTable* pTable )
{
	if ( pTable == NULL )
	{
		return ;
	}

	pTable->set_tempid(mTempID);

	for( int i = 0; i < (int)ARRAY_CNT( mCDTime ); i++ )
	{
		pTable->add_cdtime( mCDTime[ i ] );
	}
	return ;
}


void CTemplateCDGroupTable::SetFromPB( bool bServer, PBCDGroupTable* pTable )
{
	if ( pTable == NULL )
	{
		return ;
	}

	mTempID = pTable->tempid();

	for ( int i = 0; i < pTable->cdtime_size() && i < (int)ARRAY_CNT( mCDTime ); i ++ )
	{
		mCDTime[ i ] = pTable->cdtime(i);
	}

	return ;
}


int CTplApConfig::Initialize()
{
	mConsumeHook = 0;	
	mWorldChat = 0;	
	mCampChat = 0;	
	mIncApMax = 0;
	memset( mMetierAp, 0, sizeof(mMetierAp));
	memset( mRedPhysic, 0, sizeof(mRedPhysic));
	memset( mBluePhysic, 0, sizeof(mBluePhysic));
	memset( mApPhysic, 0, sizeof(mApPhysic));
	memset( mFbHpPhysic, 0, sizeof(mFbHpPhysic));
	memset( mFbMpPhysic, 0, sizeof(mFbMpPhysic));
	memset( mMendPhysic, 0, sizeof(mMendPhysic));
	return 0;
}

void CTplApConfig::CreateTlvPB(bool bServer, PBApConfig *pConfig)
{
	if ( pConfig == NULL )
	{
		return ;
	}

	pConfig->set_tempid( mTempID );
	pConfig->set_consumehook( mConsumeHook );
	pConfig->set_worldchat( mWorldChat );
	pConfig->set_campchat( mCampChat );
	pConfig->set_incapmax( mIncApMax );
	
	for ( int i = 0; i < (int)ARRAY_CNT( mMetierAp ); i ++ )
	{
		PBMetierAp* tpMetier = pConfig->add_metierap();
		tpMetier->set_metier( mMetierAp[ i ].mMetier );
		tpMetier->set_ap( mMetierAp[ i ].mAp );
	}

	for ( int i = 0; i < (int)ARRAY_CNT( mRedPhysic ); i ++ )
	{
		pConfig->add_redphysic( mRedPhysic[ i ] );
	}

	for ( int i = 0; i < (int)ARRAY_CNT( mBluePhysic ); i ++ )
	{
		pConfig->add_bluephysic( mBluePhysic[ i ] );
	}
	
	for ( int i = 0; i < (int)ARRAY_CNT( mApPhysic ); i ++ )
	{
		pConfig->add_apphysic( mApPhysic[ i ] );
	}

	for ( int i = 0; i < (int)ARRAY_CNT( mFbHpPhysic ); i ++ )
	{
		pConfig->add_fbhpphysic( mFbHpPhysic[ i ] );
	}

	for ( int i = 0; i < (int)ARRAY_CNT( mFbMpPhysic ); i ++ )
	{
		pConfig->add_fbmpphysic( mFbMpPhysic[ i ] );
	}
	
	for ( int i = 0; i < (int)ARRAY_CNT( mMendPhysic ); i ++ )
	{
		pConfig->add_mendphysic( mMendPhysic[ i ] );
	}
	return ;
}

void CTplApConfig::SetFromPB( bool bServer, PBApConfig* pConfig )
{
	if ( pConfig == NULL )
	{
		return ;
	}

	mTempID = pConfig->tempid();
	mConsumeHook = pConfig->consumehook();
	mWorldChat = pConfig->worldchat();
	mCampChat = pConfig->campchat();
	mIncApMax = pConfig->incapmax();

	for ( int i = 0; i < (int) ARRAY_CNT( mMetierAp ) && i < pConfig->metierap_size(); i ++ )
	{
		PBMetierAp* tpMetier = pConfig->mutable_metierap( i );
		mMetierAp[ i ].mMetier = tpMetier->metier();
		mMetierAp[ i ].mAp = tpMetier->ap();
	}

	for ( int i = 0; i < (int) ARRAY_CNT( mRedPhysic ) && i < pConfig->redphysic_size(); i ++ )
	{
		mRedPhysic[ i ] = pConfig->redphysic( i );
	}

	for ( int i = 0; i < (int) ARRAY_CNT( mBluePhysic ) && i < pConfig->bluephysic_size(); i ++ )
	{
		mBluePhysic[ i ] = pConfig->bluephysic( i );
	}

	for ( int i = 0; i < (int) ARRAY_CNT( mApPhysic ) && i < pConfig->apphysic_size(); i ++ )
	{
		mApPhysic[ i ] = pConfig->apphysic( i );
	}

	for ( int i = 0; i < (int) ARRAY_CNT( mFbHpPhysic ) && i < pConfig->fbhpphysic_size(); i ++ )
	{
		mFbHpPhysic[ i ] = pConfig->fbhpphysic( i );
	}

	for ( int i = 0; i < (int) ARRAY_CNT( mFbMpPhysic ) && i < pConfig->fbmpphysic_size(); i ++ )
	{
		mFbMpPhysic[ i ] = pConfig->fbmpphysic( i );
	}

	for ( int i = 0; i < (int) ARRAY_CNT( mMendPhysic ) && i < pConfig->mendphysic_size(); i ++ )
	{
		mMendPhysic[ i ] = pConfig->mendphysic( i );
	}
	return ;
}

int CTemplateLightTable::Initialize()
{	
	memset( mLevelPoint, 0, sizeof(mLevelPoint));	
	memset( mJewelPoint, 0, sizeof(mJewelPoint));	
	memset( mLevelGrade1, 0, sizeof(mLevelGrade1));	
	memset( mLevelGrade2, 0, sizeof(mLevelGrade2));	
	memset( mJewelGrade1, 0, sizeof(mJewelGrade1));	
	memset( mJewelGrade2, 0, sizeof(mJewelGrade2));
	return 0;
}

void CTemplateLightTable::CreateTlvPB( bool bServer, PBLightTable* pTable )
{
	if ( pTable == NULL )
	{
		return ;
	}

	pTable->set_tempid(mTempID);

	for( int i = 0; i < (int)ARRAY_CNT( mLevelPoint ); i++ )
	{
		pTable->add_levelpoint( mLevelPoint[ i ] );
	}
	for( int i = 0; i < (int)ARRAY_CNT( mJewelPoint ); i++ )
	{
		pTable->add_jewelpoint( mJewelPoint[ i ] );
	}
	for( int i = 0; i < (int)ARRAY_CNT( mLevelGrade1 ); i++ )
	{
		pTable->add_levelgrade1( mLevelGrade1[ i ] );
	}
	for( int i = 0; i < (int)ARRAY_CNT( mJewelGrade1 ); i++ )
	{
		pTable->add_jewelgrade1( mJewelGrade1[ i ] );
	}
	for( int i = 0; i < (int)ARRAY_CNT( mLevelGrade2 ); i++ )
	{
		pTable->add_levelgrade2( mLevelGrade2[ i ] );
	}
	for( int i = 0; i < (int)ARRAY_CNT( mJewelGrade2 ); i++ )
	{
		pTable->add_jewelgrade2( mJewelGrade2[ i ] );
	}
	return ;
}


void CTemplateLightTable::SetFromPB( bool bServer, PBLightTable* pTable )
{
	if ( pTable == NULL )
	{
		return ;
	}

	mTempID = pTable->tempid();

	for ( int i = 0; i < pTable->levelpoint_size() && i < (int)ARRAY_CNT( mLevelPoint ); i ++ )
	{
		mLevelPoint[ i ] = pTable->levelpoint(i);
	}
	for ( int i = 0; i < pTable->jewelpoint_size() && i < (int)ARRAY_CNT( mJewelPoint ); i ++ )
	{
		mJewelPoint[ i ] = pTable->jewelpoint(i);
	}
	for ( int i = 0; i < pTable->levelgrade1_size() && i < (int)ARRAY_CNT( mLevelGrade1 ); i ++ )
	{
		mLevelGrade1[ i ] = pTable->levelgrade1(i);
	}
	for ( int i = 0; i < pTable->jewelgrade1_size() && i < (int)ARRAY_CNT( mJewelGrade1 ); i ++ )
	{
		mJewelGrade1[ i ] = pTable->jewelgrade1(i);
	}
	for ( int i = 0; i < pTable->levelgrade2_size() && i < (int)ARRAY_CNT( mLevelGrade2 ); i ++ )
	{
		mLevelGrade2[ i ] = pTable->levelgrade2(i);
	}
	for ( int i = 0; i < pTable->jewelgrade2_size() && i < (int)ARRAY_CNT( mJewelGrade2 ); i ++ )
	{
		mJewelGrade2[ i ] = pTable->jewelgrade2(i);
	}

	return ;
}

int CTemplateWizardTable::Initialize()
{	
	memset( mExp, 0, sizeof( mExp)) ;	
	mKeepRand = 0;
	memset( mTrendRand, 0, sizeof(mTrendRand));
	memset( mTrendResult, 0, sizeof(mTrendResult));
	mMoney = 0;
	return 0;
}

void CTemplateWizardTable::CreateTlvPB( bool bServer, PBWizardTable* pTable )
{
	if ( pTable == NULL )
	{
		return ;
	}

	pTable->set_tempid(mTempID);

	for( int i = 0; i < (int)ARRAY_CNT( mExp ); i++ )
	{
		pTable->add_exp( mExp[ i ] );
	}
	for( int i = 0; i < (int)ARRAY_CNT( mTrendRand ); i++ )
	{
		pTable->add_trendrand( mTrendRand[ i ] );
	}
	for( int i = 0; i < (int)ARRAY_CNT( mTrendResult ); i++ )
	{
		pTable->add_trendresult( mTrendResult[ i ] );
	}
	pTable->set_keeprand( mKeepRand );
	pTable->set_money( mMoney );
	return ;
}


void CTemplateWizardTable::SetFromPB( bool bServer, PBWizardTable* pTable )
{
	if ( pTable == NULL )
	{
		return ;
	}

	mTempID = pTable->tempid();

	for ( int i = 0; i < pTable->exp_size() && i < (int)ARRAY_CNT( mExp ); i ++ )
	{
		mExp[ i ] = pTable->exp(i);
	}
	for ( int i = 0; i < pTable->trendrand_size() && i < (int)ARRAY_CNT( mTrendRand ); i ++ )
	{
		mTrendRand[ i ] = pTable->trendrand(i);
	}
	for ( int i = 0; i < pTable->trendresult_size() && i < (int)ARRAY_CNT( mTrendResult ); i ++ )
	{
		mTrendResult[ i ] = pTable->trendresult(i);
	}
	mKeepRand = pTable->keeprand();
	mMoney = pTable->money();

	return ;
}

int CTemplateTrend::Initialize()
{
	memset( mName, 0, sizeof(mName));	
	memset( mDesc, 0, sizeof(mDesc));
	memset( mBonus, 0, sizeof(mBonus));
	return 0;

};

void CTemplateTrend::CreateTlvPB( bool bServer, PBTrend* pTrend )
{
	if ( pTrend == NULL )
	{
		return ;
	}

	pTrend->set_tempid(mTempID);

	for( int i = 0; i < (int)ARRAY_CNT( mBonus ); i++ )
	{
		pTrend->add_bonus( mBonus[ i ] );
	}
	pTrend->set_name( mName );
	pTrend->set_desc( mDesc );
	
	return ;
}


void CTemplateTrend::SetFromPB( bool bServer, PBTrend* pTrend )
{
	if ( pTrend == NULL )
	{
		return ;
	}

	mTempID = pTrend->tempid();

	for ( int i = 0; i < pTrend->bonus_size() && i < (int)ARRAY_CNT( mBonus ); i ++ )
	{
		mBonus[ i ] = pTrend->bonus(i);
	}
	strncpy( mName, pTrend->name().c_str(), sizeof( mName ) );
	strncpy( mDesc, pTrend->desc().c_str(), sizeof( mDesc ) );
	
	return ;
}

int CTplAchieveAward::Initialize()
{
	memset( mAwardID, 0, sizeof(mAwardID));
	memset( mAwardNum, 0, sizeof(mAwardNum));
	memset( mPoint, 0, sizeof(mPoint));
	mSysMailID = 0;
	return 0;
}

void CTplAchieveAward::CreateTlvPB(bool bServer, PBAchieveAward *pAchieveAward)
{
	if ( pAchieveAward == NULL )
	{
		return ;
	}

	pAchieveAward->set_tempid( mTempID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mAwardID ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mAwardID ) ); j ++ )
		{	
			pAchieveAward->add_awardid( mAwardID[ i ][ j ] );
		}
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mAwardID ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mAwardID ) ); j ++ )
		{	
			pAchieveAward->add_awardnum( mAwardNum[ i ][ j ] );
		}
	}
	for( int i = 0; i < (int)ARRAY_CNT( mPoint ); i++ )
	{
		pAchieveAward->add_point( mPoint[ i ] );
	}
	pAchieveAward->set_sysmailid( mSysMailID );
	return ;
}

void CTplAchieveAward::SetFromPB(bool bServer, PBAchieveAward *pAchieveAward)
{
	if ( pAchieveAward == NULL )
	{
		return ;
	}

	mTempID = pAchieveAward->tempid();
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mAwardID ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mAwardID ) ); j ++ )
		{
			int tIndex = (ARRAY_COL( mAwardID )*i) + j;
			if ( tIndex >= pAchieveAward->awardid_size() )
				continue;
			mAwardID[ i ][ j ] = pAchieveAward->awardid(tIndex);
		}
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mAwardNum ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mAwardNum ) ); j ++ )
		{
			int tIndex = (ARRAY_COL( mAwardNum )*i) + j;
			if ( tIndex >= pAchieveAward->awardnum_size() )
				continue;
			mAwardNum[ i ][ j ] = pAchieveAward->awardnum(tIndex);
		}
	}
	for ( int i = 0; i < pAchieveAward->point_size() && i < (int)ARRAY_CNT( mPoint ); i ++ )
	{
		mPoint[ i ] = pAchieveAward->point(i);
	}
	mSysMailID = pAchieveAward->sysmailid();
	return ;
}


int CTemplatePower::Initialize()
{
	memset( mName, 0, sizeof(mName));	
	memset( mDesc, 0, sizeof(mDesc));
	mType = 0;
	mLevel = 0;
	mOfficial = 0;	
	mOrder = 0; 
	mParam1 = 0;
	mParam2 = 0;
	memset( mUseTime, 0, sizeof(mUseTime));
	mConstruct = 0;
	mPower = 0;
	mMoney = 0;
	memset( mIcon, 0, sizeof( mIcon));
	mCountry = 0;	
	mAllCount = 0;
	mStartHour = 0;
	mStartMin = 0;
	mEndHour = 0;
	mEndMin = 0;
	return 0;
}


void CTemplatePower::CreateTlvPB( bool bServer, PBPower* pPower )
{
	if ( pPower == NULL )
	{
		return ;
	}

	pPower->set_tempid(mTempID);

	pPower->set_name( mName );
	pPower->set_desc( mDesc );
	pPower->set_type( mType );
	pPower->set_official( mOfficial );
	pPower->set_order( mOrder );
	pPower->set_param1( mParam1 );
	pPower->set_param2( mParam2 );
	for ( unsigned int i = 0; i < ARRAY_CNT(mUseTime); i++ )
	{
		pPower->add_usetime( mUseTime[ i ] );
	}	
	pPower->set_construct( mConstruct );
	pPower->set_power( mPower );
	pPower->set_money( mMoney );
	pPower->set_icon( mIcon );
	pPower->set_country( mCountry );
	pPower->set_level( mLevel );
	pPower->set_allcount( mAllCount );
	pPower->set_starthour( mStartHour );
	pPower->set_startmin( mStartMin );
	pPower->set_endhour( mEndHour );
	pPower->set_endmin( mEndMin );
	return ;
}


void CTemplatePower::SetFromPB( bool bServer, PBPower* pPower )
{
	if ( pPower == NULL )
	{
		return ;
	}

	mTempID = pPower->tempid();

	strncpy( mName, pPower->name().c_str(), sizeof( mName ) );
	strncpy( mDesc, pPower->desc().c_str(), sizeof( mDesc ) );
	mType = pPower->type();
	mOfficial = pPower->official();
	mOrder = pPower->order();
	mParam1 = pPower->param1();
	mParam2 = pPower->param2();
	for ( unsigned int i = 0; i < ARRAY_CNT(mUseTime) && i < (unsigned int)pPower->usetime_size(); i++ )
	{
		mUseTime[ i ] = pPower->usetime(i);
	}	
	mConstruct = pPower->construct();
	mPower = pPower->power();
	mMoney = pPower->money();
	strncpy( mIcon , pPower->icon().c_str(), sizeof( mIcon ) );
	mCountry = pPower->country();
	mLevel = pPower->level();
	mAllCount = pPower->allcount();
	mStartHour = pPower->starthour();
	mStartMin = pPower->startmin();
	mEndHour = pPower->endhour();
	mEndMin = pPower->endmin();

	return ;
}

int CTemplateOfficial::Initialize()
{
	memset( mName, 0, sizeof(mName));	
	memset( mDesc, 0, sizeof(mDesc));
	mType = 0;
	memset( mWizard, 0, sizeof(mWizard));
	mPower = 0;
	mCountryLevel = 0;
	mConstructLevel = 0;
	return 0;
}

void CTemplateOfficial::CreateTlvPB( bool bServer, PBOfficial* pOfficial )
{
	if ( pOfficial == NULL )
	{
		return ;
	}

	pOfficial->set_tempid(mTempID);

	pOfficial->set_name( mName );
	pOfficial->set_desc( mDesc );
	pOfficial->set_type( mType );
	for ( unsigned int i = 0; i < ARRAY_CNT(mWizard); i++ )
	{
		pOfficial->add_wizard( mWizard[ i ] );
	}
	pOfficial->set_power( mPower );
	pOfficial->set_countrylevel( mCountryLevel );
	pOfficial->set_constructlevel( mConstructLevel );

	return ;
}


void CTemplateOfficial::SetFromPB( bool bServer, PBOfficial* pOfficial )
{
	if ( pOfficial == NULL )
	{
		return ;
	}

	mTempID = pOfficial->tempid();

	strncpy( mName, pOfficial->name().c_str(), sizeof( mName ) );
	strncpy( mDesc, pOfficial->desc().c_str(), sizeof( mDesc ) );
	mType = pOfficial->type();
	for ( unsigned int i = 0; i < ARRAY_CNT(mWizard) && i < (unsigned int)pOfficial->wizard_size(); i++ )
	{
		mWizard[ i ] = pOfficial->wizard(i);
	}
	mPower = pOfficial->power();
	mCountryLevel = pOfficial->countrylevel();
	mConstructLevel = pOfficial->constructlevel();

	return ;
}

int CTemplateOfficialTable::Initialize()
{
	memset( mOfficial, 0, sizeof(mOfficial));
	return 0;
}

void CTemplateOfficialTable::CreateTlvPB(bool bServer, PBOfficialTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	pTable->set_tempid( mTempID );

	for ( unsigned int i = 0; i <  ARRAY_ROW( mOfficial ); ++ i )
	{
		for ( unsigned int j = 0; j < ARRAY_COL( mOfficial ); j ++ )
		{
			pTable->add_officialdata( mOfficial[ i ][ j ] );
		}
	}

	return ;
}

void CTemplateOfficialTable::SetFromPB(bool bServer, PBOfficialTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	mTempID = pTable->tempid();
	for ( unsigned int i = 0; i < ARRAY_ROW( mOfficial ); ++ i )
	{
		for ( unsigned int j = 0; j < ARRAY_COL( mOfficial ); j ++ )
		{
			int tIndex = ( i * ARRAY_COL( mOfficial ) + j );
			if ( tIndex < pTable->officialdata_size() )
			{
				mOfficial[ i ][ j ] = pTable->officialdata(tIndex);
			}			
		}
	}
	return ;
}

int CTemplateConstructExp::Initialize()
{
	memset( mExp, 0, sizeof(mExp)) ;
	memset( mBonus, 0, sizeof(mBonus));
	return 0;
}

void CTemplateConstructExp::CreateTlvPB(bool bServer, PBConstructExp *pExp)
{
	if ( pExp == NULL )
	{
		return ;
	}

	pExp->set_tempid( mTempID );

	for ( unsigned int i = 0; i <  ARRAY_CNT( mExp ); ++ i )
	{
		pExp->add_exp( mExp[ i ] );
	}
	for ( unsigned int i = 0; i <  ARRAY_CNT( mBonus ); ++ i )
	{
		pExp->add_bonus( mBonus[ i ] );
	}

	return ;
}

void CTemplateConstructExp::SetFromPB(bool bServer, PBConstructExp *pExp)
{
	if ( pExp == NULL )
	{
		return ;
	}

	mTempID = pExp->tempid();
	for (unsigned int i = 0; i < ARRAY_CNT( mExp ) && i < (unsigned int)pExp->exp_size(); ++ i )
	{
		mExp[ i ] = pExp->exp(i);
	}
	for (unsigned int i = 0; i < ARRAY_CNT( mBonus ) && i < (unsigned int)pExp->bonus_size(); ++ i )
	{
		mBonus[ i ] = pExp->bonus(i);
	}
	return ;
}


int CTemplateAddPKTable::Initialize()
{
	mKFChangePower=0;	
	mKPChangePower=0;	
	mKWChangePower=0;	
	mDFChangePower=0;	
	mDPChangePower=0;	
	mDWChangePower=0;	
	mKFChangeFriend=0;	
	mKPChangeFriend=0;	
	mKWChangeFriend=0;
	mKFChangePK=0;	
	mKWChangePK=0;
	memset( mKPChangePK, 0, sizeof(mKPChangePK));
	memset( mPFriendMin, 0, sizeof(mPFriendMin));
	memset( mPFriendMax, 0, sizeof(mPFriendMax));
	mKFChangeHonor=0;	
	mKPChangeHonor=0;	
	mKWChangeHonor=0;
	return 0;
}


void CTemplateAddPKTable::CreateTlvPB(bool bServer, PBAddPKTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	pTable->set_tempid( mTempID );

	pTable->set_kfchangepower( mKFChangePower );
	pTable->set_kpchangepower( mKPChangePower );
	pTable->set_kwchangepower( mKWChangePower );
	pTable->set_dfchangepower( mDFChangePower );
	pTable->set_dpchangepower( mDPChangePower );
	pTable->set_dwchangepower( mDWChangePower );
	pTable->set_kfchangefriend( mKFChangeFriend );
	pTable->set_kpchangefriend( mKPChangeFriend );
	pTable->set_kwchangefriend( mKWChangeFriend );
	pTable->set_kfchangepk( mKFChangePK );
	pTable->set_kwchangepk( mKWChangePK );
	for ( unsigned int i = 0; i <  ARRAY_CNT( mKPChangePK ); ++ i )
	{
		pTable->add_kpchangepk( mKPChangePK[ i ] );
	}
	for ( unsigned int i = 0; i <  ARRAY_CNT( mPFriendMin ); ++ i )
	{
		pTable->add_pfriendmin( mPFriendMin[ i ] );
	}
	for ( unsigned int i = 0; i <  ARRAY_CNT( mPFriendMax ); ++ i )
	{
		pTable->add_pfriendmax( mPFriendMax[ i ] );
	}
	pTable->set_kfchangehonor( mKFChangeHonor );
	pTable->set_kpchangehonor( mKPChangeHonor );
	pTable->set_kwchangehonor( mKWChangeHonor );

	return ;
}

void CTemplateAddPKTable::SetFromPB(bool bServer, PBAddPKTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	mTempID = pTable->tempid();

	mKFChangePower = pTable->kfchangepower();
	mKPChangePower = pTable->kpchangepower();
	mKWChangePower = pTable->kwchangepower();
	mDFChangePower = pTable->dfchangepower();
	mDPChangePower = pTable->dpchangepower();
	mDWChangePower = pTable->dwchangepower();
	mKFChangeFriend = pTable->kfchangefriend();
	mKPChangeFriend = pTable->kpchangefriend();
	mKWChangeFriend = pTable->kwchangefriend();
	mKFChangePK = pTable->kfchangepk();
	mKWChangePK = pTable->kwchangepk();
	for ( unsigned int i = 0; i < ARRAY_CNT( mKPChangePK ) && i < (unsigned int)pTable->kpchangepk_size(); ++ i )
	{
		mKPChangePK[ i ] = pTable->kpchangepk(i);
	}
	for ( unsigned int i = 0; i < ARRAY_CNT( mPFriendMin ) && i < (unsigned int)pTable->pfriendmin_size(); ++ i )
	{
		mPFriendMin[ i ] = pTable->pfriendmin(i);
	}
	for ( unsigned int i = 0; i < ARRAY_CNT( mPFriendMax ) && i < (unsigned int)pTable->pfriendmax_size(); ++ i )
	{
		mPFriendMax[ i ] = pTable->pfriendmax(i);
	}
	mKFChangeHonor = pTable->kfchangehonor();
	mKPChangeHonor = pTable->kpchangehonor();
	mKWChangeHonor = pTable->kwchangehonor();
	return ;
}


int CTemplateMapTable::Initialize()
{
	memset( mLevel, 0, sizeof(mLevel));	
	memset( mPower, 0, sizeof(mPower));
	return 0;
}

void CTemplateMapTable::CreateTlvPB(bool bServer, PBMapTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	pTable->set_tempid( mTempID );

	for ( unsigned int i = 0; i <  ARRAY_CNT( mLevel ); ++ i )
	{
		pTable->add_level( mLevel[ i ] );
	}
	for ( unsigned int i = 0; i <  ARRAY_CNT( mPower ); ++ i )
	{
		pTable->add_power( mPower[ i ] );
	}

	return ;
}

void CTemplateMapTable::SetFromPB(bool bServer, PBMapTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	mTempID = pTable->tempid();
	for ( unsigned int i = 0; i < ARRAY_CNT( mLevel ); ++ i )
	{
		mLevel[ i ] = pTable->level(i);
	}
	for ( unsigned int i = 0; i < ARRAY_CNT( mPower ); ++ i )
	{
		mPower[ i ] = pTable->power(i);
	}
	return ;
}

int CTemplateMultiAttackTable::Initialize()
{
	memset( mReduce, 0, sizeof(mReduce));
	return 0;
}

void CTemplateMultiAttackTable::CreateTlvPB(bool bServer, PBMultiAttackTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	pTable->set_tempid( mTempID );

	for ( unsigned int i = 0; i <  ARRAY_CNT( mReduce ); ++ i )
	{
		pTable->add_reduce( mReduce[ i ] );
	}

	return ;
}

void CTemplateMultiAttackTable::SetFromPB(bool bServer, PBMultiAttackTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	mTempID = pTable->tempid();
	for ( unsigned int i = 0; i < ARRAY_CNT( mReduce ); ++ i )
	{
		mReduce[ i ] = pTable->reduce(i);
	}
	return ;
}

int CTplItemVip::Initialize()
{
	mVipLevel = 0;
	mVipTime = 0;
	return 0;
}


void CTplItemVip::CreateTlvPB( bool bServer, PBItemVip* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}

	CTplItem::CreateTlvPB( bServer, pItem->mutable_baseinfo() );
	
	pItem->set_viplevel( mVipLevel );
	pItem->set_viptime( mVipTime ); 

	return ;
}

void CTplItemVip::SetFromPB(bool bServer, PBItemVip *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}

	CTplItem::SetFromPB( bServer, pItem->mutable_baseinfo() );

	mVipLevel = pItem->viplevel();
	mVipTime = pItem->viptime();

	return ;
}

int CTplItemClean::Initialize()
{
	mCleanType = 0;
	mCleanLevel = 0;
	mStep = 0;
	return 0;
}

void CTplItemClean::CreateTlvPB( bool bServer, PBItemClean* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}

	CTplItem::CreateTlvPB( bServer, pItem->mutable_baseinfo() );

	pItem->set_cleantype( mCleanType );
	pItem->set_cleanlevel( mCleanLevel ); 
	pItem->set_step( mStep );

	return ;
}

void CTplItemClean::SetFromPB(bool bServer, PBItemClean *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}

	CTplItem::SetFromPB( bServer, pItem->mutable_baseinfo() );

	mCleanType = pItem->cleantype();
	mCleanLevel = pItem->cleanlevel();
	mStep = pItem->step();

	return ;
}

int CTemplateAwardInfo::Initialize()
{	
	memset( mName, 0, sizeof(mName));	
	memset( mDesc, 0, sizeof(mDesc));
	memset( mItemID, 0, sizeof(mItemID));
	memset( mItemNum, 0, sizeof(mItemNum));
	mBuffID = 0;
	mTimes = 0;
	mNpcID = 0;
	mCountry = 0;
	return 0;
}

void CTemplateAwardInfo::CreateTlvPB( bool bServer, PBAwardInfo* pInfo )
{
	if ( pInfo == NULL )
	{
		return ;
	}

	pInfo->set_tempid(mTempID);

	pInfo->set_name( mName );
	pInfo->set_desc( mDesc );
	pInfo->set_buffid( mBuffID );
	for ( unsigned int i = 0; i < ARRAY_CNT(mItemID); i++ )
	{
		pInfo->add_itemid( mItemID[ i ] );
	}
	for ( unsigned int i = 0; i < ARRAY_CNT(mItemNum); i++ )
	{
		pInfo->add_itemnum( mItemNum[ i ] );
	}
	pInfo->set_times( mTimes );
	pInfo->set_npcid( mNpcID );
	pInfo->set_country( mCountry );
	pInfo->set_nofamily( mNoFamily );

	return ;
}


void CTemplateAwardInfo::SetFromPB( bool bServer, PBAwardInfo* pInfo )
{
	if ( pInfo == NULL )
	{
		return ;
	}

	mTempID = pInfo->tempid();

	strncpy( mName, pInfo->name().c_str(), sizeof( mName ) );
	strncpy( mDesc, pInfo->desc().c_str(), sizeof( mDesc ) );
	mBuffID = pInfo->buffid();
	for ( unsigned int i = 0; i < ARRAY_CNT(mItemID) && i < (unsigned int)pInfo->itemid_size(); i++ )
	{
		mItemID[ i ] = pInfo->itemid(i);
	}
	for ( unsigned int i = 0; i < ARRAY_CNT(mItemNum) && i < (unsigned int)pInfo->itemnum_size(); i++ )
	{
		mItemNum[ i ] = pInfo->itemnum(i);
	}
	mTimes = pInfo->times();
	mNpcID = pInfo->npcid();
	mCountry = pInfo->country();
	mNoFamily = pInfo->nofamily();

	return ;
}

int CTplLeagueBag::Initialize()
{
	mLeagueBagType = 0;
	mExchangeTableID = 0;
	memset( mLeagueInfo, 0, sizeof(mLeagueInfo));
	mMapID = 0;
	mPosX = 0;
	mPosY = 0;
	return 0;
}

void CTplLeagueBag::CreateTlvPB( bool bServer, PBLeagueBag* pItem )
{
	if ( pItem == NULL )
	{
		return;
	}
	
	CTplItem::CreateTlvPB( bServer, pItem->mutable_itembase() );
	
	pItem->set_exchangetableid( mExchangeTableID );
	pItem->set_leaguebagtype( mLeagueBagType );
	for ( int i = 0; i < (int)ARRAY_CNT( mLeagueInfo ); ++i )
	{
		PBLeagueItem *tpLeagueItem = pItem->add_leagueiteminfo();
		tpLeagueItem->set_itemid( mLeagueInfo[i].mItemID );
		tpLeagueItem->set_itemnum( mLeagueInfo[i].mItemNum );
		tpLeagueItem->set_probality( mLeagueInfo[i].mProbality );
	}
	
	pItem->set_mapid( mMapID );
	pItem->set_posx( mPosX );
	pItem->set_posy( mPosY );
}

void CTplLeagueBag::SetFromPB( bool bServer, PBLeagueBag* pItem )
{
	if ( pItem == NULL )
	{
		return;
	}
	
	CTplItem::SetFromPB( bServer, pItem->mutable_itembase() );
	
	mLeagueBagType = pItem->leaguebagtype();
	mExchangeTableID = pItem->exchangetableid();
	for ( int i = 0; i < pItem->leagueiteminfo_size() && i < (int)ARRAY_CNT( mLeagueInfo ); ++i )
	{
		mLeagueInfo[i].mItemID = pItem->mutable_leagueiteminfo( i )->itemid();
		mLeagueInfo[i].mItemNum = pItem->mutable_leagueiteminfo( i )->itemnum(); 
		mLeagueInfo[i].mProbality = pItem->mutable_leagueiteminfo( i )->probality();
	}
	
	mMapID = pItem->mapid();
	mPosX = pItem->posx();
	mPosY = pItem->posy();
}

int CTemplateMatrixTable::Initialize()
{
	memset( mLevel, 0, sizeof(mLevel));
	mMaxLimit = 0;	
	mMinLimit = 0;
	memset( mMasterItemID, 0, sizeof(mMasterItemID));
	memset( mMasterItemNum, 0, sizeof(mMasterItemNum));
	memset( mMailID, 0, sizeof(mMailID));
	mFormalLevel = 0;
	mEducatedLevel = 0;
	mEnterLimit = 0;
	mRemoveItem = 0;
	mRemoveMoney = 0;
	mFormalPercent = 0;
	mEducatedPercent = 0;
	mReturnLimit = 0;
	mAlterMax = 0;
	mAlterMin = 0;
	mReturnMailID = 0;
	memset( mSlaveItemID, 0, sizeof(mSlaveItemID));
	memset( mSlaveItemNum, 0, sizeof(mSlaveItemNum));
	mAlterAll = 0;
	mAlterDefault=0;
	mSlaveMailID=0;
	return 0;
}

void CTemplateMatrixTable::CreateTlvPB(bool bServer, PBMatrixTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	pTable->set_tempid( mTempID );

	for ( unsigned int i = 0; i <  ARRAY_CNT( mLevel ); ++ i )
	{
		pTable->add_level( mLevel[ i ] );
	}
	pTable->set_maxlimit( mMaxLimit );
	pTable->set_minlimit( mMinLimit );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mMasterItemID ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mMasterItemID ); j ++ )
		{
			pTable->add_masteritemid( mMasterItemID[ i ][ j ] );
		}
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mMasterItemNum ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mMasterItemNum ); j ++ )
		{
			pTable->add_masteritemnum( mMasterItemNum[ i ][ j ] );
		}
	}
	for ( unsigned int i = 0; i <  ARRAY_CNT( mMailID ); ++ i )
	{
		pTable->add_mailid( mMailID[ i ] );
	}
	pTable->set_formallevel( mFormalLevel );
	pTable->set_educatedlevel( mEducatedLevel );
	pTable->set_enterlimit( mEnterLimit );
	pTable->set_removeitem( mRemoveItem );
	pTable->set_removemoney( mRemoveMoney );
	pTable->set_formalpercent( mFormalPercent );
	pTable->set_educatedpercent( mEducatedPercent );
	pTable->set_returnlimit( mReturnLimit );
	pTable->set_altermax( mAlterMax );
	pTable->set_altermin( mAlterMin );
	pTable->set_returnmailid( mReturnMailID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mSlaveItemID ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mSlaveItemID ); j ++ )
		{
			pTable->add_slaveitemid( mSlaveItemID[ i ][ j ] );
		}
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mSlaveItemNum ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mSlaveItemNum ); j ++ )
		{
			pTable->add_slaveitemnum( mSlaveItemNum[ i ][ j ] );
		}
	}
	pTable->set_alterall( mAlterAll );
	pTable->set_alterdefault( mAlterDefault );
	pTable->set_slavemailid( mSlaveMailID );
	return ;
}

void CTemplateMatrixTable::SetFromPB(bool bServer, PBMatrixTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	mTempID = pTable->tempid();
	for ( unsigned int i = 0; i < ARRAY_CNT( mLevel ) && i < (unsigned int) pTable->level_size(); ++ i )
	{
		mLevel[ i ] = pTable->level(i);
	}
	mMaxLimit = pTable->maxlimit();
	mMinLimit = pTable->minlimit();
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mMasterItemID ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mMasterItemID ); j ++ )
		{
			int tTotal = (ARRAY_COL( mMasterItemID )*i) + j;
			if ( tTotal >= pTable->masteritemid_size() )
			{
				break ;
			}
			mMasterItemID[ i ][ j ] = pTable->masteritemid( tTotal );
		}
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mMasterItemNum ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mMasterItemNum ); j ++ )
		{
			int tTotal = (ARRAY_COL( mMasterItemNum )*i) + j;
			if ( tTotal >= pTable->masteritemnum_size() )
			{
				break ;
			}
			mMasterItemNum[ i ][ j ] = pTable->masteritemnum( tTotal );
		}
	}
	for ( unsigned int i = 0; i < ARRAY_CNT( mMailID ) && i < (unsigned int) pTable->mailid_size(); ++ i )
	{
		mMailID[ i ] = pTable->mailid(i);
	}
	mFormalLevel = pTable->formallevel();
	mEducatedLevel = pTable->educatedlevel();
	mEnterLimit = pTable->enterlimit();
	mRemoveItem = pTable->removeitem();
	mRemoveMoney = pTable->removemoney();
	mFormalPercent = pTable->formalpercent();
	mEducatedPercent = pTable->educatedpercent();
	mReturnLimit = pTable->returnlimit();
	mAlterMax = pTable->altermax();
	mAlterMin = pTable->altermin();
	mReturnMailID = pTable->returnmailid();
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mSlaveItemID ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mSlaveItemID ); j ++ )
		{
			int tTotal = (ARRAY_COL( mSlaveItemID )*i) + j;
			if ( tTotal >= pTable->slaveitemid_size() )
			{
				break ;
			}
			mSlaveItemID[ i ][ j ] = pTable->slaveitemid( tTotal );
		}
	}
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mSlaveItemNum ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mSlaveItemNum ); j ++ )
		{
			int tTotal = (ARRAY_COL( mSlaveItemNum )*i) + j;
			if ( tTotal >= pTable->slaveitemnum_size() )
			{
				break ;
			}
			mSlaveItemNum[ i ][ j ] = pTable->slaveitemnum( tTotal );
		}
	}
	mAlterAll = pTable->alterall();
	mAlterDefault = pTable->alterdefault();
	mSlaveMailID = pTable->slavemailid();
	return ;
}

// 婚姻配置表
void CTemplateMarriageConfig::SetFromPB( bool bServer, PBTemplateMarriageConfig* pTable )
{
	if ( pTable == NULL )
	{
		return;
	}
	
	mTempID = pTable->tempid();
	mMarriageLevel = pTable->marriagelevel();
	mMarriageMoney = pTable->marriagemoney();
	
	for ( unsigned int i = 0; i < (unsigned int)pTable->weddinginfo_size() && i < ARRAY_CNT( mWeddinInfo ); ++i )
	{
		mWeddinInfo[i].mDelayMoneyNeeded = pTable->mutable_weddinginfo( i )->delaymoneyneeded();
		mWeddinInfo[i].mFemaleKeepsakeID = pTable->mutable_weddinginfo( i )->femalekeepsakeid();
		mWeddinInfo[i].mInvitationCardID = pTable->mutable_weddinginfo( i )->invitationcardid();
		mWeddinInfo[i].mInvitationCardNum = pTable->mutable_weddinginfo( i )->invitationcardnum();
		mWeddinInfo[i].mMaleKeepsakeID = pTable->mutable_weddinginfo( i )->malekeepsakeid();
		mWeddinInfo[i].mMoneyNeeded = pTable->mutable_weddinginfo( i )->moneyneeded();
		mWeddinInfo[i].mWeddingHallMapID = pTable->mutable_weddinginfo( i )->weddinghallmapid();
		mWeddinInfo[i].mWeddingHallRedstoneID = pTable->mutable_weddinginfo( i )->weddinghallredstoneid();
		mWeddinInfo[i].mWeddingLength = pTable->mutable_weddinginfo( i )->weddinglength();
		mWeddinInfo[i].mWeddingRedstoneID = pTable->mutable_weddinginfo( i )->weddingredstoneid();
		mWeddinInfo[i].mPosX = pTable->mutable_weddinginfo( i )->posx();
		mWeddinInfo[i].mPosY = pTable->mutable_weddinginfo( i )->posy();
		mWeddinInfo[i].mMaleKeepsakeTwo = pTable->mutable_weddinginfo( i )->malekeepsaketwo();
		mWeddinInfo[i].mFemaleKeepsakeTwo = pTable->mutable_weddinginfo( i )->femalekeepsaketwo();
		mWeddinInfo[i].mMaleKeepsakeThree = pTable->mutable_weddinginfo( i )->malekeepsakethree();
		mWeddinInfo[i].mFemaleKeepsakeThree = pTable->mutable_weddinginfo( i )->femalekeepsakethree();
		mWeddinInfo[i].mMaleBufferID	=	pTable->mutable_weddinginfo( i )->malebufferid();
		mWeddinInfo[i].mFemaleBufferID	=	pTable->mutable_weddinginfo( i )->femalebufferid();
		mWeddinInfo[i].mMaleItemID		=	pTable->mutable_weddinginfo( i )->maleitemid();
		mWeddinInfo[i].mFemaleItemID	=	pTable->mutable_weddinginfo( i )->femaleitemid();
		
		mWeddinInfo[i].mGroomPosX		=	pTable->mutable_weddinginfo( i )->groomposx();
		mWeddinInfo[i].mGroomPosY		=	pTable->mutable_weddinginfo( i )->groomposy();
		mWeddinInfo[i].mBridePosX		=	pTable->mutable_weddinginfo( i )->brideposx();
		mWeddinInfo[i].mBridePosY		=	pTable->mutable_weddinginfo( i )->brideposy();
		mWeddinInfo[i].mGroomBufferID	=	pTable->mutable_weddinginfo( i )->groombufferid();
		mWeddinInfo[i].mBrideBufferID	=	pTable->mutable_weddinginfo( i )->bridebufferid();
		mWeddinInfo[i].mMarriageItemID	=	pTable->mutable_weddinginfo( i )->marriageitemid();
		mWeddinInfo[i].mWeddingScroll	=	pTable->mutable_weddinginfo( i )->weddingscroll();
	} 		
	
	for ( unsigned int i = 0; i < (unsigned int)pTable->femalekeepsakeinfo_size() && i < ARRAY_CNT( mFemaleKeepsakeInfo ); ++i )
	{	  
		mFemaleKeepsakeInfo[i].mKeepsakeID = pTable->mutable_femalekeepsakeinfo( i )->keepsakeid();
		mFemaleKeepsakeInfo[i].mEquipID = pTable->mutable_femalekeepsakeinfo( i )->equipid();
	}
	
	for ( unsigned int i = 0; i < (unsigned int)pTable->malekeepsakeinfo_size() && i < ARRAY_CNT( mMaleKeepsakeInfo ); ++i )
	{	  
		mMaleKeepsakeInfo[i].mKeepsakeID = pTable->mutable_malekeepsakeinfo( i )->keepsakeid();
		mMaleKeepsakeInfo[i].mEquipID = pTable->mutable_malekeepsakeinfo( i )->equipid();
	}
	
	for ( unsigned int i = 0; i < (unsigned int)pTable->weddingnpc_size() && i < sizeof( mWeddingNpc ) ; ++i )
	{
		unsigned int tRow = i / ( ARRAY_COL( mWeddingNpc ) );
		unsigned int tColumn = i % ( ARRAY_COL( mWeddingNpc ) );		
		if ( tRow >= ARRAY_ROW( mWeddingNpc ) || tColumn >= ARRAY_COL( mWeddingNpc ) )
		{
			break;
		}
		mWeddingNpc[tRow][tColumn] = pTable->weddingnpc( i ); 
	}
	
	mMapID = pTable->mapid();
	mPosX = pTable->posx();
	mPosY = pTable->posy();
	mDivorceItemID = pTable->divorceitemid();
	mMarriageTitleID = pTable->marriagetitleid();
	
	mMaleKeepsakeID = pTable->malekeepsakeid();
	mFemaleKeepsakeID = pTable->femalekeepsakeid();
	mInvitationID = pTable->invitationid();
	mInvitationNum = pTable->invitationnum();
	mMaleItemID = pTable->maleitemid();
	mFemaleItemID = pTable->femaleitemid();
	mBaitangBuffID = pTable->baitangbuffid();
	mBasicMarriageItem = pTable->basicmarriageitem();
	mBasicMarriageItemNum = pTable->basicmarriageitemnum();
	mMarriageNpcMapID = pTable->marriagenpcmapid();
	mMarriageNpcPosX = pTable->marriagenpcposx();
	mMarriageNpcPosY = pTable->marriagenpcposy();
	mMarriageIntimacy= pTable->marriageintimacy();
	mIntimacyItemNum = pTable->intimacyitemnum();
}

int CTemplateMarriageConfig::Initialize()
{
	mMarriageLevel = 0;
	mMarriageMoney = 0; 		
	memset( mWeddinInfo, 0, sizeof( mWeddinInfo));
	memset( mWeddingNpc, 0, sizeof( mWeddingNpc));
	memset( mFemaleKeepsakeInfo, 0, sizeof(mFemaleKeepsakeInfo));
	memset( mMaleKeepsakeInfo, 0, sizeof(mMaleKeepsakeInfo));
	mMapID = 0;
	mPosX = 0;
	mPosY = 0;
	mDivorceItemID = 0;
	mMarriageTitleID = 0;
	mMaleKeepsakeID = 0;
	mFemaleKeepsakeID = 0;
	mInvitationID = 0;
	mInvitationNum = 0;
	mMaleItemID = 0;			
	mFemaleItemID = 0;
	mBaitangBuffID = 0;	
	mBasicMarriageItem = 0;	
	mBasicMarriageItemNum = 0;	
	mMarriageNpcMapID = 0;
	mMarriageNpcPosX = 0;
	mMarriageNpcPosY = 0;	
	mMarriageIntimacy= 0;
	mIntimacyItemNum = 0;
	return 0;
}

void CTemplateMarriageConfig::CreateTlvPB( bool bServer, PBTemplateMarriageConfig* pTable )
{
	if ( pTable == NULL )
	{
		return;
	}
	
	pTable->set_marriagelevel( mMarriageLevel );
	pTable->set_marriagemoney( mMarriageMoney );
	pTable->set_tempid( mTempID );
	
	for ( unsigned int i = 0; i < ARRAY_CNT( mWeddinInfo ); ++i )
	{
		PBWeddingInfo *tpWeddingInfo = pTable->add_weddinginfo();
		tpWeddingInfo->set_delaymoneyneeded( mWeddinInfo[i].mDelayMoneyNeeded );
		tpWeddingInfo->set_femalekeepsakeid( mWeddinInfo[i].mFemaleKeepsakeID );
		tpWeddingInfo->set_invitationcardid( mWeddinInfo[i].mInvitationCardID );
		tpWeddingInfo->set_invitationcardnum( mWeddinInfo[i].mInvitationCardNum );
		tpWeddingInfo->set_malekeepsakeid( mWeddinInfo[i].mMaleKeepsakeID );
		tpWeddingInfo->set_moneyneeded( mWeddinInfo[i].mMoneyNeeded );
		tpWeddingInfo->set_weddinghallmapid( mWeddinInfo[i].mWeddingHallMapID );
		tpWeddingInfo->set_weddinghallredstoneid( mWeddinInfo[i].mWeddingHallRedstoneID );
		tpWeddingInfo->set_weddinglength( mWeddinInfo[i].mWeddingLength );
		tpWeddingInfo->set_weddingredstoneid( mWeddinInfo[i].mWeddingRedstoneID );
		tpWeddingInfo->set_posx( mWeddinInfo[i].mPosX );
		tpWeddingInfo->set_posy( mWeddinInfo[i].mPosY );
		tpWeddingInfo->set_malekeepsaketwo( mWeddinInfo[i].mMaleKeepsakeTwo );
		tpWeddingInfo->set_femalekeepsaketwo( mWeddinInfo[i].mFemaleKeepsakeTwo );
		tpWeddingInfo->set_malekeepsakethree( mWeddinInfo[i].mMaleKeepsakeThree );  
		tpWeddingInfo->set_femalekeepsakethree( mWeddinInfo[i].mFemaleKeepsakeThree ); 
		tpWeddingInfo->set_malebufferid( mWeddinInfo[i].mMaleBufferID );
		tpWeddingInfo->set_femalebufferid( mWeddinInfo[i].mFemaleBufferID );
		tpWeddingInfo->set_maleitemid( mWeddinInfo[i].mMaleItemID );
		tpWeddingInfo->set_femaleitemid( mWeddinInfo[i].mFemaleItemID );
		
		tpWeddingInfo->set_groomposx( mWeddinInfo[i].mGroomPosX );
		tpWeddingInfo->set_groomposy( mWeddinInfo[i].mGroomPosY );
		tpWeddingInfo->set_brideposx( mWeddinInfo[i].mBridePosX );
		tpWeddingInfo->set_brideposy( mWeddinInfo[i].mBridePosY );
		tpWeddingInfo->set_groombufferid( mWeddinInfo[i].mGroomBufferID );
		tpWeddingInfo->set_bridebufferid( mWeddinInfo[i].mBrideBufferID );
		tpWeddingInfo->set_marriageitemid( mWeddinInfo[i].mMarriageItemID );
		tpWeddingInfo->set_weddingscroll( mWeddinInfo[i].mWeddingScroll );
	}
	
	for ( unsigned int i = 0; i < ARRAY_CNT( mFemaleKeepsakeInfo ); ++i )
	{
		PBKeepsnakeInfo *tpKeepsakeInfo = pTable->add_femalekeepsakeinfo();
		tpKeepsakeInfo->set_equipid( mFemaleKeepsakeInfo[i].mEquipID );
		tpKeepsakeInfo->set_keepsakeid( mFemaleKeepsakeInfo[i].mKeepsakeID );
	}
	
	for ( unsigned int i = 0; i < ARRAY_CNT( mMaleKeepsakeInfo ); ++i )
	{
		PBKeepsnakeInfo *tpKeepsakeInfo = pTable->add_malekeepsakeinfo();
		tpKeepsakeInfo->set_equipid( mMaleKeepsakeInfo[i].mEquipID );
		tpKeepsakeInfo->set_keepsakeid( mMaleKeepsakeInfo[i].mKeepsakeID );
	}
	
	for ( unsigned int i = 0; i < ARRAY_ROW( mWeddingNpc ); ++i )
	{
		for ( unsigned j = 0; j < ARRAY_COL( mWeddingNpc ); ++j )
		{
			pTable->add_weddingnpc( mWeddingNpc[i][j] );
		}
	}
	
	pTable->set_mapid( mMapID );
	pTable->set_posx( mPosX );
	pTable->set_posy( mPosY );
	pTable->set_divorceitemid( mDivorceItemID );
	pTable->set_marriagetitleid( mMarriageTitleID );
	
	pTable->set_malekeepsakeid( mMaleKeepsakeID );
	pTable->set_femalekeepsakeid( mFemaleKeepsakeID );
	pTable->set_invitationid( mInvitationID );
	pTable->set_invitationnum( mInvitationNum );
	pTable->set_maleitemid( mMaleItemID );
	pTable->set_femaleitemid( mFemaleItemID );
	pTable->set_baitangbuffid( mBaitangBuffID );
	pTable->set_basicmarriageitem( mBasicMarriageItem );
	pTable->set_basicmarriageitemnum( mBasicMarriageItemNum );
	pTable->set_marriagenpcmapid( mMarriageNpcMapID );
	pTable->set_marriagenpcposx( mMarriageNpcPosX );
	pTable->set_marriagenpcposy( mMarriageNpcPosY );
	pTable->set_marriageintimacy( mMarriageIntimacy );
	pTable->set_intimacyitemnum( mIntimacyItemNum );
}


// 请帖
void CTplInvitation::SetFromPB( bool bServer, PBInvitation* pTable )
{
	if ( pTable == NULL )
	{
		return;
	}
	CTplItem::SetFromPB( bServer, pTable->mutable_itembase() );
	mExchangeItemID = pTable->exchangeitemid(); 
}
void CTplInvitation::CreateTlvPB( bool bServer, PBInvitation* pTable )
{
	if ( pTable == NULL )
	{
		return;
	}
	CTplItem::CreateTlvPB( bServer, pTable->mutable_itembase() );	
	pTable->set_exchangeitemid( mExchangeItemID );	  
}

int CTplInvitation::Initialize()
{
	mExchangeItemID = 0;
	return 0;
}

// 系统请帖
void CTplSysInvitation::SetFromPB( bool bServer, PBSysInvitation* pTable )
{	
	if ( pTable == NULL )
	{
		return;
	}
	
	CTplItem::SetFromPB( bServer, pTable->mutable_itembase() );
	mInvitationID = pTable->invitationid();
}

int CTplSysInvitation::Initialize()
{
	mInvitationID = 0;
	return 0;
}

void CTplSysInvitation::CreateTlvPB( bool bServer, PBSysInvitation* pTable )
{
	if ( pTable == NULL )
	{
		return;
	}
	CTplItem::CreateTlvPB( bServer, pTable->mutable_itembase() );
	pTable->set_invitationid( mInvitationID );
}

// CTemplateProgressConfig:进度条配置表 

int CTemplateProgressConfig::Initialize()
{
	memset( mProgressInfo, 0, sizeof(mProgressInfo));
	return 0;
}

void  CTemplateProgressConfig::SetFromPB( bool bServer, PBProGressConfig* pTable )
{
	if ( pTable == NULL )
	{
		return;
	}
	mTempID = pTable->tmpid();
	for ( int i = 0; i < pTable->progress_size() && i < (int)ARRAY_CNT( mProgressInfo ) ; ++i )
	{
		mProgressInfo[i].mFunctype = pTable->mutable_progress( i )->functype();
		mProgressInfo[i].mTime	   = pTable->mutable_progress( i )->progresstime();
	}
}
void CTemplateProgressConfig::CreateTlvPB( bool bServer, PBProGressConfig* pTable )
{
	if ( pTable == NULL )
	{
		return;
	}
	
	pTable->set_tmpid( mTempID );
	for ( int i = 0; i < (int)ARRAY_CNT( mProgressInfo ); ++i )
	{
		ProGressInfo *tpProGressInfo = pTable->add_progress();
		tpProGressInfo->set_functype( mProgressInfo[i].mFunctype );
		tpProGressInfo->set_progresstime( mProgressInfo[i].mTime );
	}
}

void CTemplateCountTable::SetFromPB( bool bServer, PBCountTable* pTable )
{
	if ( pTable == NULL )
	{
		return;
	}

	mTempID = pTable->tempid();

	for ( unsigned int i = 0; i < (unsigned int)pTable->countdata_size() && i < ARRAY_CNT( mCountData ); ++i )
	{
		mCountData[ i ].mCountType = pTable->mutable_countdata( i )->counttype();
		mCountData[ i ].mCountTime = pTable->mutable_countdata( i )->counttime();
		mCountData[ i ].mVipTime = pTable->mutable_countdata( i )->viptime();
		mCountData[ i ].mClearType = pTable->mutable_countdata( i )->cleartype();
	} 
}

int CTemplateCountTable::Initialize()
{
	memset( mCountData, 0, sizeof(mCountData));
	return 0;
}

void CTemplateCountTable::CreateTlvPB( bool bServer, PBCountTable* pTable )
{
	if ( pTable == NULL )
	{
		return;
	}

	pTable->set_tempid( mTempID );

	for ( unsigned int i = 0; i < ARRAY_CNT( mCountData ); ++i )
	{
		PBCount *tpCount = pTable->add_countdata();
		tpCount->set_counttype( mCountData[ i ].mCountType );
		tpCount->set_counttime( mCountData[ i ].mCountTime );
		tpCount->set_viptime( mCountData[ i ].mVipTime );
		tpCount->set_cleartype( mCountData[ i ].mClearType );
	}
}


// 活动配置
int CTemplateActivityConfig::Initialize()
{
	mHydrangeaThrowTime = 0;
	memset( mHydrangeaSugarID, 0, sizeof(mHydrangeaSugarID));
	mSugarSpreadTime = 0;
	mSugarSpreadNum = 0;
	mHydrangeaActivityTime = 0;
	mHydrangeaHolderTime = 0;
	mHydrangeaGrabTime = 0;
	mHydrangeaRedStone = 0;	
	mHydrangeaExpTableID = 0;
			
	mPreliminaryTitleID = 0;			
	mPreliminaryBaseScore = 0;	
	memset( mPreliminaryExtralScore, 0, sizeof( mPreliminaryExtralScore ) );	
	mPreliminaryTipsTime = 0;	
	mPreliminaryAnswerTime = 0;
	mPreliminaryQuestionTime = 0;	
	mEffectiveOffLineExchangeRate = 0;
	mMaxOnLineEffectiveTime = 0;
	mMaxEffectiveOffLineTime = 0;
	mMaxOffLineTime = 0;
	mOffLineTimeLimit = 0;
	mVipOffLineExchangeRate = 0;
	return 0;
}

void CTemplateActivityConfig::SetFromPB( bool bServer, PBActivityConfig* pTable )
{
	if ( pTable == NULL )
	{
		return;
	}
	
	mTempID = pTable->tempid();	
	mHydrangeaThrowTime = pTable->hydrangeathrowtime();	
	for ( int i = 0; i < (int)ARRAY_CNT( mHydrangeaSugarID ) && i < pTable->hydrangeasugarid_size(); ++i )
	{	  
		mHydrangeaSugarID[i] = pTable->hydrangeasugarid( i );	
	}
	mSugarSpreadTime = pTable->sugarspreadtime();	
	mSugarSpreadNum = pTable->sugarspreadnum();	
	mHydrangeaActivityTime = pTable->hydrangeaactivitytime();	
	mHydrangeaHolderTime = pTable->hydrangeaholdertime();	
	mHydrangeaGrabTime = pTable->hydrangearabtime();
	mHydrangeaRedStone = pTable->hydrangearedstoneid();
	mHydrangeaExpTableID = pTable->hydrangeaexptableid();
	
	mPreliminaryTitleID = pTable->preliminarytitleid();			
	mPreliminaryBaseScore = pTable->preliminarybasescore();	
	for ( int i = 0; i < pTable->preliminaryextralscore_size() && i < (int)ARRAY_CNT( mPreliminaryExtralScore ); ++i )
	{
		mPreliminaryExtralScore[i] = pTable->preliminaryextralscore( i );
	}	
	mPreliminaryTipsTime = pTable->preliminarytipstime();	
	mPreliminaryAnswerTime = pTable->preliminaryanswertime();
	mPreliminaryQuestionTime = pTable->preliminaryquestiontime();
	mPreliminaryQuestionNum = pTable->preliminaryquestionnum();	
	mPreliminaryExpTableID = pTable->preliminaryexptableid();	
	mEffectiveOffLineExchangeRate = pTable->effectiveofflineexchangerate();
	mMaxOnLineEffectiveTime = pTable->maxonlineeffectivetime();
	mMaxEffectiveOffLineTime = pTable->maxeffectiveofflinetime();  
	mMaxOffLineTime = pTable->maxofflinetime();	
	mOffLineTimeLimit = pTable->offlinetimelimit();
	mVipOffLineExchangeRate = pTable->vipofflineexchangerate();
}
void CTemplateActivityConfig::CreateTlvPB( bool bServer, PBActivityConfig* pTable )
{
	if ( pTable == NULL )
	{
		return;
	}
	
	pTable->set_tempid( mTempID );	
	pTable->set_hydrangeathrowtime( mHydrangeaThrowTime );	
	for ( int i = 0; i < (int)ARRAY_CNT( mHydrangeaSugarID ); ++i )
	{	  
		pTable->add_hydrangeasugarid( mHydrangeaSugarID[i] );	
	}
	pTable->set_sugarspreadtime( mSugarSpreadTime );	
	pTable->set_sugarspreadnum( mSugarSpreadNum );	
	pTable->set_hydrangeaactivitytime( mHydrangeaActivityTime );	
	pTable->set_hydrangeaholdertime( mHydrangeaHolderTime );	
	pTable->set_hydrangearabtime( mHydrangeaGrabTime );
	pTable->set_hydrangearedstoneid( mHydrangeaRedStone );
	pTable->set_hydrangeaexptableid( mHydrangeaExpTableID );
	
	pTable->set_preliminarytitleid( mPreliminaryTitleID );			
	pTable->set_preliminarybasescore( mPreliminaryBaseScore );	
	for ( int i = 0; i < (int)ARRAY_CNT( mPreliminaryExtralScore ); ++i )
	{
		pTable->add_preliminaryextralscore( mPreliminaryExtralScore[i] );
	}	
	pTable->set_preliminarytipstime( mPreliminaryTipsTime );	
	pTable->set_preliminaryanswertime( mPreliminaryAnswerTime );
	pTable->set_preliminaryquestiontime( mPreliminaryQuestionTime );
	pTable->set_preliminaryquestionnum( mPreliminaryQuestionNum );	
	pTable->set_preliminaryexptableid( mPreliminaryExpTableID );	
	pTable->set_effectiveofflineexchangerate( mEffectiveOffLineExchangeRate );
	pTable->set_maxonlineeffectivetime( mMaxOnLineEffectiveTime );    
	pTable->set_maxeffectiveofflinetime( mMaxEffectiveOffLineTime );
	pTable->set_maxofflinetime( mMaxOffLineTime );
	pTable->set_offlinetimelimit( mOffLineTimeLimit );
	pTable->set_vipofflineexchangerate( mVipOffLineExchangeRate );
}


int CTplItemRefresh::Initialize()
{
	mColor = 0;
	mLevel = 0;
	memset( mLimitPos, 0, sizeof(mLimitPos) );
	return 0;
}
void CTplItemRefresh::SetFromPB( bool bServer,PBItemRefresh *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_itembase() );
	mColor = pItem->color();
	mLevel = pItem->level();
	for ( int i = 0; i < (int)ARRAY_CNT( mLimitPos ) && i < pItem->limitpos_size(); ++i )
	{	  
		mLimitPos[ i ] = pItem->limitpos( i );	
	}
	return ;
}

void CTplItemRefresh::CreateTlvPB( bool bServer,PBItemRefresh* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_itembase() );
	pItem->set_color( mColor );
	pItem->set_level( mLevel );
	for ( int i = 0; i < (int)ARRAY_CNT( mLimitPos ); ++i )
	{	  
		pItem->add_limitpos( mLimitPos[ i ] );	
	}
	return ;
}

void CTplItemSummon::SetFromPB( bool bServer,PBItemSummon *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_itembase() );
	mSummonType = pItem->summontype();
	return ;
}

void CTplItemSummon::CreateTlvPB( bool bServer,PBItemSummon* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::CreateTlvPB( bServer,pItem->mutable_itembase() );
	pItem->set_summontype( mSummonType );
	return ;
}

int CTplItemSummon::Initialize()
{
	mSummonType = 0;
	return 0;
}

int CTemplateDirectTable::Initialize()
{
	memset( mDirectID, 0, sizeof(mDirectID) );
	memset( mLuckID, 0, sizeof(mLuckID) );
	memset( mLuckRate, 0, sizeof(mLuckRate) );
	return 0;
}

void CTemplateDirectTable::SetFromPB( bool bServer, PBDirectTable* pTable )
{
	if ( pTable == NULL )
	{
		return;
	}

	mTempID = pTable->tempid();	
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mDirectID ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mDirectID ) ); j ++ )
		{
			int tIndex = ( i * ARRAY_COL( mDirectID ) + j );
			if ( tIndex < pTable->directid_size() )
			{
				mDirectID[ i ][ j ] = pTable->directid( tIndex );
			}			
		}
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mLuckID ) && i < pTable->luckid_size(); ++i )
	{	  
		mLuckID[ i ] = pTable->luckid( i );	
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mLuckRate ) && i < pTable->luckrate_size(); ++i )
	{	  
		mLuckRate[ i ] = pTable->luckrate( i );	
	}
}
void CTemplateDirectTable::CreateTlvPB( bool bServer, PBDirectTable* pTable )
{
	if ( pTable == NULL )
	{
		return;
	}

	pTable->set_tempid( mTempID );	
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mDirectID ); ++ i )
	{
		for ( unsigned int j = 0; j < (unsigned int) ( ARRAY_COL( mDirectID ) ); j ++ )
		{
			pTable->add_directid( mDirectID[ i ][ j ] );
		}
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mLuckID ); ++i )
	{	  
		pTable->add_luckid( mLuckID[ i ] );	
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mLuckRate ); ++i )
	{	  
		pTable->add_luckrate( mLuckRate[ i ] );	
	}
}

void CTplItemDirect::SetFromPB( bool bServer,PBItemDirect *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	//PBItemBase* tpBase = pItem->mutable_baseinfo();
	CTplItem::SetFromPB( bServer,pItem->mutable_itembase() );
	mLevel = pItem->level();
	mStar = pItem->star();
	return ;
}

void CTplItemDirect::CreateTlvPB( bool bServer,PBItemDirect* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	CTplItem::CreateTlvPB( bServer,pItem->mutable_itembase() );
	pItem->set_level( mLevel );
	pItem->set_star( mStar );
	return ;
}

int CTplItemDirect::Initialize()
{
	mLevel = 0;
	mStar = 0;
	return 0;
}

void CTplItemGetStar::SetFromPB( bool bServer,PBItemGetStar *pItem)
{
	if ( pItem == NULL )
	{
		return ;
	}
	CTplItem::SetFromPB( bServer,pItem->mutable_itembase() );
	mLevel = pItem->level();
	return ;
}

void CTplItemGetStar::CreateTlvPB( bool bServer,PBItemGetStar* pItem )
{
	if ( pItem == NULL )
	{
		return ;
	}
	CTplItem::CreateTlvPB( bServer,pItem->mutable_itembase() );
	pItem->set_level( mLevel );
	return ;
}

int CTplItemGetStar::Initialize()
{
	mLevel = 0;
	return 0;
}

int CTemplateWorldBattleConfig::Initialize()
{
	mInitBuffID=0;	
	mFlagSource=0;	
	mKillSource=0;	
	mFlagBonus=0;
	memset( &mSubmitBegin, 0, sizeof(mSubmitBegin));
	memset( &mBattleBegin, 0, sizeof(mBattleBegin));
	memset( &mBattleEnd, 0, sizeof(mBattleEnd));
	memset( mStoneID, 0, sizeof(mStoneID));
	memset( mPerNum, 0, sizeof(mPerNum));
	memset( mMapID, 0, sizeof(mMapID));
	memset( mSubmitMoney, 0, sizeof(mSubmitMoney));
	memset( mPosX, 0, sizeof(mPosX));
	memset( mPosY, 0, sizeof(mPosY));
	mOutMapID = 0;
	mOutPosX = 0;
	mOutPosY = 0;
	memset( mFlagIndex, 0, sizeof(mFlagIndex));
	memset( &mBidEnd, 0, sizeof(mBidEnd));
	mMaxWarHonor = 0;
	memset( mTitle, 0, sizeof(mTitle));
	memset( &mNoticeBegin, 0, sizeof(mNoticeBegin));
	mTitleValidTime = 0;
	mStonePosX = 0;
	mStonePosY = 0;
	memset( mTax, 0, sizeof(mTax));
	mWinTax = 0;
	return 0;
}


void CTemplateWorldBattleConfig::CreateTlvPB(bool bServer, PBWorldBattleConfig *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	pTable->set_tempid( mTempID );

	pTable->set_initbuffid( mInitBuffID );
	pTable->set_flagsource( mFlagSource );
	pTable->set_killsource( mKillSource );
	pTable->set_flagbonus( mFlagBonus );

	PBWarDate *tpPBSubmitBegin = pTable->mutable_submitbegin();
	tpPBSubmitBegin->set_day( mSubmitBegin.mDay );
	tpPBSubmitBegin->set_hour( mSubmitBegin.mHour );
	tpPBSubmitBegin->set_minute( mSubmitBegin.mMinute );
	PBWarDate *tpPBBattleBegin = pTable->mutable_battlebegin();
	tpPBBattleBegin->set_day( mBattleBegin.mDay );
	tpPBBattleBegin->set_hour( mBattleBegin.mHour );
	tpPBBattleBegin->set_minute( mBattleBegin.mMinute );
	PBWarDate *tpPBBattleEnd = pTable->mutable_battleend();
	tpPBBattleEnd->set_day( mBattleEnd.mDay );
	tpPBBattleEnd->set_hour( mBattleEnd.mHour );
	tpPBBattleEnd->set_minute( mBattleEnd.mMinute );

	for ( int i = 0; i < (int)ARRAY_CNT( mStoneID ); ++i )
	{	  
		pTable->add_stoneid( mStoneID[ i ] );	
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mPerNum ); ++i )
	{	  
		pTable->add_pernum( mPerNum[ i ] );	
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mMapID ); ++i )
	{	  
		pTable->add_mapid( mMapID[ i ] );	
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mSubmitMoney ); ++i )
	{	  
		pTable->add_submitmoney( mSubmitMoney[ i ] );	
	}
	pTable->set_submitlevel( mSubmitLevel );
	for ( int i = 0; i < (int)ARRAY_CNT( mPosX ); ++i )
	{	  
		pTable->add_posx( mPosX[ i ] );	
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mPosY ); ++i )
	{	  
		pTable->add_posy( mPosY[ i ] );	
	}
	pTable->set_outmapid( mOutMapID );
	pTable->set_outposx( mOutPosX );
	pTable->set_outposy( mOutPosY );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mFlagIndex ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mFlagIndex ); j ++ )
		{
			pTable->add_flagindex( mFlagIndex[ i ][ j ] );
		}
	}
	PBWarDate *tpPBBidEnd = pTable->mutable_bidend();
	tpPBBidEnd->set_day( mBidEnd.mDay );
	tpPBBidEnd->set_hour( mBidEnd.mHour );
	tpPBBidEnd->set_minute( mBidEnd.mMinute );
	pTable->set_maxwarhonor( mMaxWarHonor );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mTitle ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mTitle ); j ++ )
		{
			pTable->add_title( mTitle[ i ][ j ] );
		}
	}
	PBWarDate *tpPBNoticeBegin = pTable->mutable_noticebegin();
	tpPBNoticeBegin->set_day( mNoticeBegin.mDay );
	tpPBNoticeBegin->set_hour( mNoticeBegin.mHour );
	tpPBNoticeBegin->set_minute( mNoticeBegin.mMinute );
	pTable->set_titlevalidtime( mTitleValidTime );
	pTable->set_stoneposx( mStonePosX );
	pTable->set_stoneposy( mStonePosY );

	for ( int i = 0; i < (int)ARRAY_CNT( mTax ); ++i )
	{
		PBTax *tpPBTax = pTable->add_tax();
		tpPBTax->set_taxrate( mTax[ i ].mTaxRate );	
		tpPBTax->set_incomerate( mTax[ i ].mIncomeRate );
		tpPBTax->set_limit( mTax[ i ].mLimit );
	}
	pTable->set_wintax( mWinTax );
	return ;
}

void CTemplateWorldBattleConfig::SetFromPB(bool bServer, PBWorldBattleConfig *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	mTempID = pTable->tempid();

	mInitBuffID = pTable->initbuffid();
	mFlagSource = pTable->flagsource();
	mKillSource = pTable->killsource();
	mFlagBonus = pTable->flagbonus();
	
	PBWarDate *tpPBSubmitBegin = pTable->mutable_submitbegin();
	mSubmitBegin.mDay = tpPBSubmitBegin->day();
	mSubmitBegin.mHour = tpPBSubmitBegin->hour();
	mSubmitBegin.mMinute = tpPBSubmitBegin->minute();
	PBWarDate *tpPBBattleBegin = pTable->mutable_battlebegin();
	mBattleBegin.mDay = tpPBBattleBegin->day();
	mBattleBegin.mHour = tpPBBattleBegin->hour();
	mBattleBegin.mMinute = tpPBBattleBegin->minute();
	PBWarDate *tpPBBattleEnd = pTable->mutable_battleend();
	mBattleEnd.mDay = tpPBBattleEnd->day();
	mBattleEnd.mHour = tpPBBattleEnd->hour();
	mBattleEnd.mMinute = tpPBBattleEnd->minute();

	for ( int i = 0; i < (int)ARRAY_CNT( mStoneID ) && i < pTable->stoneid_size(); ++i )
	{	  
		mStoneID[ i ] = pTable->stoneid( i );	
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mPerNum ) && i < pTable->pernum_size(); ++i )
	{	  
		mPerNum[ i ] = pTable->pernum( i );	
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mMapID ) && i < pTable->mapid_size(); ++i )
	{	  
		mMapID[ i ] = pTable->mapid( i );	
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mPosX ) && i < pTable->posx_size(); ++i )
	{	  
		mPosX[ i ] = pTable->posx( i );	
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mPosY ) && i < pTable->posy_size(); ++i )
	{	  
		mPosY[ i ] = pTable->posy( i );	
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mSubmitMoney ) && i < pTable->submitmoney_size(); ++i )
	{	  
		mSubmitMoney[ i ] = pTable->submitmoney( i );	
	}
	mSubmitLevel = pTable->submitlevel();
	mOutMapID = pTable->outmapid();
	mOutPosX = pTable->outposx();
	mOutPosY = pTable->outposy();
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mFlagIndex ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mFlagIndex ); j ++ )
		{
			int tTotal = (ARRAY_COL( mFlagIndex )*i) + j;
			if ( tTotal >= pTable->flagindex_size() )
			{
				break ;
			}
			mFlagIndex[ i ][ j ] = pTable->flagindex( tTotal );
		}
	}
	PBWarDate *tpPBBidEnd = pTable->mutable_bidend();
	mBidEnd.mDay = tpPBBidEnd->day();
	mBidEnd.mHour = tpPBBidEnd->hour();
	mBidEnd.mMinute = tpPBBidEnd->minute();
	mMaxWarHonor = pTable->maxwarhonor();
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mTitle ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mTitle ); j ++ )
		{
			int tTotal = (ARRAY_COL( mTitle )*i) + j;
			if ( tTotal >= pTable->title_size() )
			{
				break ;
			}
			mTitle[ i ][ j ] = pTable->title( tTotal );
		}
	}
	PBWarDate *tpPBNoticeBegin = pTable->mutable_noticebegin();
	mNoticeBegin.mDay = tpPBNoticeBegin->day();
	mNoticeBegin.mHour = tpPBNoticeBegin->hour();
	mNoticeBegin.mMinute = tpPBNoticeBegin->minute();
	mTitleValidTime = pTable->titlevalidtime();
	mStonePosX = pTable->stoneposx();
	mStonePosY = pTable->stoneposy();

	for ( int i = 0; i < (int)ARRAY_CNT( mTax ) && i < pTable->tax_size(); ++i )
	{
		PBTax *tpPBTax = pTable->mutable_tax(i);
		mTax[ i ].mTaxRate = tpPBTax->taxrate();	
		mTax[ i ].mIncomeRate = tpPBTax->incomerate();
		mTax[ i ].mLimit = tpPBTax->limit();
	}
	mWinTax = pTable->wintax();
	return ;
}

// CTplItemAutoCollect
void CTplItemAutoCollect::SetFromPB( bool bServer, PBItemAutoCollect* pItem )
{
	if ( pItem == NULL )
	{
		return;
	}
	
	for ( unsigned int i = 0; i < (unsigned int)pItem->degreeinfo_size() && i < ARRAY_CNT( mDegreeInfo ); ++i )
	{
		mDegreeInfo[i].mDegreeValue = pItem->mutable_degreeinfo( i )->degreevalue();
		mDegreeInfo[i].mProbality = pItem->mutable_degreeinfo( i )->probality();
		mDegreeInfo[i].mValue = pItem->mutable_degreeinfo( i )->value(); 
	}
	
	for ( unsigned int i = 0; i < (unsigned int)pItem->iteminfo_size() && i < ARRAY_CNT( mItemInfo ); ++i )
	{
		mItemInfo[i].mItemID = pItem->mutable_iteminfo( i )->itemid();
		mItemInfo[i].mItemNum = pItem->mutable_iteminfo( i )->itemnum();
		mItemInfo[i].mProbality = pItem->mutable_iteminfo( i )->probality();
	}
	
	mMapID = pItem->mapid();
	mPosX = pItem->posx();
	mPosX = pItem->posy();
	mGrid = pItem->grid();	
	mMaxUseTimes = pItem->maxusetimes();
}
void CTplItemAutoCollect::CreateTlvPB( bool bServer, PBItemAutoCollect* pItem )
{
	 if ( pItem == NULL )
	 {
		return;
	 }
	 
	 for ( unsigned int i = 0; i < ARRAY_CNT( mDegreeInfo ); ++i )
	 {
		PBDegreeInfo *tpDegreeInfo = pItem->add_degreeinfo();
		tpDegreeInfo->set_degreevalue( mDegreeInfo[i].mDegreeValue );
		tpDegreeInfo->set_probality( mDegreeInfo[i].mProbality );
		tpDegreeInfo->set_value( mDegreeInfo[i].mValue );
	 }

	 for ( unsigned int i = 0; i < ARRAY_CNT( mItemInfo ); ++i )
	 {
		PBItemInfo *tpItemInfo = pItem->add_iteminfo();
		tpItemInfo->set_itemid( mItemInfo[i].mItemID );		 
		tpItemInfo->set_itemnum( mItemInfo[i].mItemNum );
		tpItemInfo->set_probality( mItemInfo[i].mProbality );
	 }
	 
	 pItem->set_mapid( mMapID );
	 pItem->set_posx( mPosX );
	 pItem->set_posy( mPosY );
	 pItem->set_grid( mGrid );
	 pItem->set_maxusetimes( mMaxUseTimes );
}
int CTplItemAutoCollect::Initialize()
{
	memset( mItemInfo, 0, sizeof( mItemInfo ) ); 
	memset( mDegreeInfo, 0, sizeof( mDegreeInfo ) );
	mMapID = 0;
	mPosX = 0;	
	mPosY = 0;
	mGrid = 0;
	mMaxUseTimes = 0;	
	return 0;
}

void CFangCunTreeNode::SetFromPB( bool bServer,PBFangCunTreeNode *pNode)
{
	if ( pNode == NULL )
	{
		return ;
	}
	mSkillID	= pNode->skillid();
	mMaxLevel	= pNode->maxlevel();
	for ( int i = 0; i < (int)ARRAY_CNT( mNeedSkillID ) && i < pNode->needskillid_size(); ++i )
	{	  
		mNeedSkillID[ i ] = pNode->needskillid( i );	
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mNeedSkillPoint ) && i < pNode->needskillpoint_size(); ++i )
	{	  
		mNeedSkillPoint[ i ] = pNode->needskillpoint( i );	
	}
	mLevel			= pNode->level();
	mNeedBook		= pNode->needbook();
	mStage		= pNode->stage();
	mOrder		= pNode->order();
	return ;
}

void CFangCunTreeNode::CreateTlvPB( bool bServer,PBFangCunTreeNode* pNode )
{
	if ( pNode == NULL )
	{
		return ;
	}
	pNode->set_skillid( mSkillID );
	pNode->set_maxlevel( mMaxLevel );
	for ( int i = 0; i < (int)ARRAY_CNT( mNeedSkillID ); ++i )
	{	  
		pNode->add_needskillid( mNeedSkillID[ i ] );	
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mNeedSkillPoint ); ++i )
	{	  
		pNode->add_needskillpoint( mNeedSkillPoint[ i ] );	
	}
	pNode->set_level( mLevel );
	pNode->set_needbook( mNeedBook );
	pNode->set_stage( mStage );
	pNode->set_order( mOrder );
	return ;
}

int CTemplateFangCunTree::Initialize()
{
	memset( mNode, 0, sizeof(mNode));
	return 0;
}

void CTemplateFangCunTree::SetFromPB( bool bServer,PBFangCunTree *pSkillTree)
{
	if ( pSkillTree == NULL )
	{
		return ;
	}
	mTempID			= pSkillTree->tempid();
	
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mNode ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mNode ); j ++ )
		{
			int tTotal = (ARRAY_COL( mNode )*i) + j;
			if ( tTotal >= pSkillTree->skillnode_size() )
			{
				break ;
			}
			PBFangCunTreeNode* pNode = pSkillTree->mutable_skillnode( tTotal );
			mNode[ i ][ j ].SetFromPB( bServer,pNode );
		}
	}
	return ;
}

void CTemplateFangCunTree::CreateTlvPB( bool bServer,PBFangCunTree *pSkillTree)
{
	if ( pSkillTree == NULL )
	{
		return ;
	}
	pSkillTree->set_tempid( mTempID );
	for ( unsigned int i = 0; i < (unsigned int) ARRAY_ROW( mNode ); i ++ )
	{
		for ( unsigned int j = 0; j < (unsigned int) ARRAY_COL( mNode ); j ++ )
		{
			PBFangCunTreeNode* tpNode = pSkillTree->add_skillnode();
			mNode[ i ][ j ].CreateTlvPB( bServer, tpNode );
		}
	}
	return ;
}

int CTemplateTalentStage::Initialize()
{
	memset( mStage, 0, sizeof(mStage));
	return 0;
}

void CTemplateTalentStage::CreateTlvPB( bool bServer,PBTemplateTalentStage* pTalent )
{
	if ( pTalent == NULL )
	{
		return ;
	}
	pTalent->set_tempid( mTempID );
	for( int i = 0; i < (int)ARRAY_CNT( mStage ); i++ )
	{
		PBStage* tpStage = pTalent->add_stage( );
		tpStage->set_stageid( mStage[ i ].mStageID );
		for ( int j = 0; j < (int)ARRAY_CNT(mStage[ i ].mNext); j++ )
		{
			tpStage->add_next( mStage[ i ].mNext[ j ] );
		}
		tpStage->set_layer( mStage[ i ].mLayer );
	}
	return ;
}

void CTemplateTalentStage::SetFromPB( bool bServer,PBTemplateTalentStage* pTalent)
{
	if ( pTalent == NULL )
	{
		return ;
	}
	mTempID = pTalent->tempid();
	for ( int i = 0; i < pTalent->stage_size() && i < (int)ARRAY_CNT( mStage ); i ++ )
	{
		PBStage* tpStage = pTalent->mutable_stage( i );
		mStage[ i ].mStageID = tpStage->stageid();
		for ( int j = 0; j < tpStage->next_size() && j < (int)ARRAY_CNT(mStage[ i ].mNext); j++ )
		{
			mStage[ i ].mNext[ j ] = tpStage->next( j );
		}
		mStage[ i ].mLayer = tpStage->layer();
	}

	return ;
}

int CTemplateAccuseConfig::Initialize()
{
	mAccuseBeginHour=0;	
	mAccuseBeginMinute=0;	
	mAccuseLevel=0;	
	mAccuseEndHour=0;
	mAccuseEndMinute = 0;
	mAccuseMoney = 0;
	mAccusePerson = 0;
	mAccusePercent = 0;
	mAwardPower = 0;
	mGateNum = 0;
	mFlagNum = 0;
	mGatePower = 0;
	mFlagPower = 0;
	mAccuseNoticeHour=0;
	mAccuseNoticeMinute=0;
	mNoticePower=0;
	mGateParam1=0;
	mFlagParam1=0;
	mGateParam2=0;
	mFlagParam2=0;
	mGateParam3=0;
	mFlagParam3=0;
	mLWPercent=0;
	return 0;
}


void CTemplateAccuseConfig::CreateTlvPB(bool bServer, PBAccuseConfig *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	pTable->set_tempid( mTempID );

	pTable->set_accusebeginhour( mAccuseBeginHour );
	pTable->set_accusebeginminute( mAccuseBeginMinute );
	pTable->set_accuselevel( mAccuseLevel );
	pTable->set_accuseendhour( mAccuseEndHour );
	pTable->set_accuseendminute( mAccuseEndMinute );
	pTable->set_accusemoney( mAccuseMoney );
	pTable->set_accuseperson( mAccusePerson );
	pTable->set_accusepercent( mAccusePercent );
	pTable->set_awardpower( mAwardPower );
	pTable->set_gatenum( mGateNum );
	pTable->set_flagnum( mFlagNum );
	pTable->set_gatepower( mGatePower );
	pTable->set_flagpower( mFlagPower );
	pTable->set_accusenoticehour( mAccuseNoticeHour );
	pTable->set_accusenoticeminute( mAccuseNoticeMinute );
	pTable->set_noticepower( mNoticePower );
	pTable->set_gateparam1( mGateParam1 );
	pTable->set_flagparam1( mFlagParam1 );
	pTable->set_gateparam2( mGateParam2 );
	pTable->set_flagparam2( mFlagParam2 );
	pTable->set_gateparam3( mGateParam3 );
	pTable->set_flagparam3( mFlagParam3 );
	pTable->set_lwpercent( mLWPercent );
	return ;
}

void CTemplateAccuseConfig::SetFromPB(bool bServer, PBAccuseConfig *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	mTempID = pTable->tempid();

	mAccuseBeginHour = pTable->accusebeginhour();
	mAccuseBeginMinute = pTable->accusebeginminute();
	mAccuseLevel = pTable->accuselevel();
	mAccuseEndHour = pTable->accuseendhour();
	mAccuseEndMinute = pTable->accuseendminute();
	mAccuseMoney = pTable->accusemoney();
	mAccusePerson = pTable->accuseperson();
	mAccusePercent = pTable->accusepercent();
	mAwardPower = pTable->awardpower();
	mGateNum = pTable->gatenum();
	mFlagNum = pTable->flagnum();
	mGatePower = pTable->gatepower();
	mFlagPower = pTable->flagpower();
	mAccuseNoticeHour = pTable->accusenoticehour();
	mAccuseNoticeMinute = pTable->accusenoticeminute();
	mNoticePower = pTable->noticepower();	
	mGateParam1 = pTable->gateparam1();
	mFlagParam1 = pTable->flagparam1();
	mGateParam2 = pTable->gateparam2();
	mFlagParam2 = pTable->flagparam2();
	mGateParam3 = pTable->gateparam3();
	mFlagParam3 = pTable->flagparam3();
	mLWPercent = pTable->lwpercent();
	return ;
}

int CTemplateGlobalMapConfig::Initialize()
{
	mMapID=0;	
	mBuffID=0;	
	mLimit=0;
	mOutMap=0;
	mOutX=0;
	mOutY=0;
	return 0;
}


void CTemplateGlobalMapConfig::CreateTlvPB(bool bServer, PBGlobalMapConfig *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	pTable->set_tempid( mTempID );

	pTable->set_mapid( mMapID );
	pTable->set_buffid( mBuffID );
	pTable->set_limit( mLimit );
	pTable->set_outmap( mOutMap );
	pTable->set_outx( mOutX );
	pTable->set_outy( mOutY );
	pTable->set_escapebuffid( mEscapeBuffID );
	return ;
}

void CTemplateGlobalMapConfig::SetFromPB(bool bServer, PBGlobalMapConfig *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	mTempID = pTable->tempid();

	mMapID = pTable->mapid();
	mBuffID = pTable->buffid();
	mLimit = pTable->limit();
	mOutMap = pTable->outmap();
	mOutX = pTable->outx();
	mOutY = pTable->outy();
	mEscapeBuffID = pTable->escapebuffid();
	return ;
}

int CTemplateOffLineExpConfig::Initialize()
{	
	mOffLineUnit=0;	
	memset( mOffLineExp, 0, sizeof(mOffLineExp));
	mOffLineLevel=0;
	memset( mExpLimit, 0, sizeof(mExpLimit));
	mOffLinePercent=0;
	return 0;
}


void CTemplateOffLineExpConfig::CreateTlvPB(bool bServer, PBOffLineExpConfig *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	pTable->set_tempid( mTempID );

	pTable->set_offlineunit( mOffLineUnit );
	for ( int i = 0; i < (int)ARRAY_CNT( mOffLineExp ); ++i )
	{	  
		pTable->add_offlineexp( mOffLineExp[ i ] );	
	}
	pTable->set_offlinelevel( mOffLineLevel );
	for ( int i = 0; i < (int)ARRAY_CNT( mExpLimit ); ++i )
	{	  
		pTable->add_explimit( mExpLimit[ i ] );	
	}
	pTable->set_offlinepercent( mOffLinePercent );
	return ;
}

void CTemplateOffLineExpConfig::SetFromPB(bool bServer, PBOffLineExpConfig *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	mTempID = pTable->tempid();

	mOffLineUnit = pTable->offlineunit();
	for ( int i = 0; i < (int)ARRAY_CNT( mOffLineExp ) && i < pTable->offlineexp_size(); ++i )
	{	  
		mOffLineExp[ i ] = pTable->offlineexp( i );	
	}	
	mOffLineLevel = pTable->offlinelevel();
	for ( int i = 0; i < (int)ARRAY_CNT( mExpLimit ) && i < pTable->explimit_size(); ++i )
	{	  
		mExpLimit[ i ] = pTable->explimit( i );	
	}
	mOffLinePercent = pTable->offlinepercent();
	return ;
}

int CTemplateRepeFameTable::Initialize()
{
	memset( mFameType, 0, sizeof(mFameType));
	memset( mFameValue, 0, sizeof(mFameValue));
	memset( mItemValue, 0, sizeof(mItemValue));
	mCombatItemID = 0;
	mRepeItemID = 0;
	mCommonItemID = 0;
	mCombatLimit = 0;
	mRepeLimit = 0;
	mCommonLimit = 0;
	return 0;
}


void CTemplateRepeFameTable::CreateTlvPB(bool bServer, PBRepeFameTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	pTable->set_tempid( mTempID );

	for ( int i = 0; i < (int)ARRAY_CNT( mFameType ); ++i )
	{	  
		pTable->add_fametype( mFameType[ i ] );	
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mFameValue ); ++i )
	{	  
		pTable->add_famevalue( mFameValue[ i ] );	
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mItemValue ); ++i )
	{	  
		pTable->add_itemvalue( mItemValue[ i ] );	
	}
	pTable->set_combatitemid( mCombatItemID );
	pTable->set_repeitemid( mRepeItemID );
	pTable->set_commonitemid( mCommonItemID );
	pTable->set_combatlimit( mCombatLimit );
	pTable->set_repelimit( mRepeLimit );
	pTable->set_commonlimit( mCommonLimit );
	return ;
}

void CTemplateRepeFameTable::SetFromPB(bool bServer, PBRepeFameTable *pTable)
{
	if ( pTable == NULL )
	{
		return ;
	}

	mTempID = pTable->tempid();

	for ( int i = 0; i < (int)ARRAY_CNT( mFameType ) && i < pTable->fametype_size(); ++i )
	{	  
		mFameType[ i ] = pTable->fametype( i );	
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mFameValue ) && i < pTable->famevalue_size(); ++i )
	{	  
		mFameValue[ i ] = pTable->famevalue( i );	
	}
	for ( int i = 0; i < (int)ARRAY_CNT( mItemValue ) && i < pTable->itemvalue_size(); ++i )
	{	  
		mItemValue[ i ] = pTable->itemvalue( i );	
	}
	mCombatItemID = pTable->combatitemid();
	mRepeItemID = pTable->repeitemid();
	mCommonItemID = pTable->commonitemid();
	mCombatLimit = pTable->combatlimit();
	mRepeLimit = pTable->repelimit();
	mCommonLimit = pTable->commonlimit();
	return ;
}

int CTplCastingSpiritTable::Initialize()
{
	memset(m_anIsOpen, 0, sizeof(m_anIsOpen));
	memset(m_acPositionName, 0, sizeof(m_acPositionName));
	memset(m_acPositionDesc, 0, sizeof(m_acPositionDesc));
	memset(m_acTypeName, 0, sizeof(m_acTypeName));
	memset(m_acTypeDesc, 0, sizeof(m_acTypeDesc));
	memset(m_anSpiritItem, 0, sizeof(m_anSpiritItem));
	memset(m_anSpiritItemNum, 0, sizeof(m_anSpiritItemNum));
	memset(m_anUpgradeMoney, 0, sizeof(m_anUpgradeMoney));
	memset(m_anDowngradeMoney, 0, sizeof(m_anDowngradeMoney));
	m_nUpgradeItem = 0;
	m_nDowngradeItem = 0;

	return 0;
}

void CTplCastingSpiritTable::CreateTlvPB( bool bServer, PBCastingSpiritTable* pTable )
{
	if (pTable == NULL)
	{
		return;
	}

	pTable->set_tempid(mTempID);
	for ( int i = 0; i < (int)ARRAY_CNT(m_anIsOpen); ++i )
	{	  
		pTable->add_openstatus(m_anIsOpen[i]);	
	}
	for ( int i = 0; i < (int)ARRAY_CNT(m_acPositionName); ++i )
	{	  
		pTable->add_positionname(m_acPositionName[i]);	
	}
	for ( int i = 0; i < (int)ARRAY_CNT(m_acPositionDesc); ++i )
	{	  
		pTable->add_positiondesc(m_acPositionDesc[i]);	
	}
	for ( int i = 0; i < (int)ARRAY_CNT(m_acTypeName); ++i )
	{	  
		pTable->add_spirittypename(m_acTypeName[i]);	
	}
	for ( int i = 0; i < (int)ARRAY_CNT(m_acTypeDesc); ++i )
	{	  
		pTable->add_spirittypedesc(m_acTypeDesc[i]);	
	}
	for ( int i = 0; i < (int)ARRAY_CNT(m_anSpiritItem); ++i )
	{	  
		pTable->add_spirititem(m_anSpiritItem[i]);	
	}
	for ( int i = 0; i < (int)ARRAY_CNT(m_anSpiritItemNum); ++i )
	{	  
		pTable->add_spirititemnum(m_anSpiritItemNum[i]);	
	}
	for ( int i = 0; i < (int)ARRAY_CNT(m_anUpgradeMoney); ++i )
	{	  
		pTable->add_upgrademoney(m_anUpgradeMoney[i]);	
	}
	for ( int i = 0; i < (int)ARRAY_CNT(m_anDowngradeMoney); ++i )
	{	  
		pTable->add_downgrademoney(m_anDowngradeMoney[i]);	
	}
	pTable->set_upgradeitem(m_nUpgradeItem);
	pTable->set_downgradeitem(m_nDowngradeItem);
}

void CTplCastingSpiritTable::SetFromPB(bool bServer, PBCastingSpiritTable *pTable)
{
	if (pTable == NULL)
	{
		return;
	}

	mTempID = pTable->tempid();

	for ( int i = 0; i < (int)ARRAY_CNT(m_anIsOpen) && i < pTable->openstatus_size(); ++i )
	{	  
		m_anIsOpen[i] = pTable->openstatus( i );	
	}
	for ( int i = 0; i < (int)ARRAY_CNT(m_acPositionName) && i < pTable->positionname_size(); ++i )
	{	  
		strncpy(m_acPositionName[i], pTable->positionname(i).c_str(), sizeof(m_acPositionName[i]) - 1);	
	}
	for ( int i = 0; i < (int)ARRAY_CNT(m_acPositionDesc) && i < pTable->positiondesc_size(); ++i )
	{	  
		strncpy(m_acPositionDesc[i], pTable->positiondesc(i).c_str(), sizeof(m_acPositionDesc[i]) - 1);	
	}
	for ( int i = 0; i < (int)ARRAY_CNT(m_acTypeName) && i < pTable->spirittypename_size(); ++i )
	{	  
		strncpy(m_acTypeName[i], pTable->spirittypename(i).c_str(), sizeof(m_acTypeName[i]) - 1);	
	}
	for ( int i = 0; i < (int)ARRAY_CNT(m_acTypeDesc) && i < pTable->spirittypedesc_size(); ++i )
	{	  
		strncpy(m_acTypeDesc[i], pTable->spirittypedesc(i).c_str(), sizeof(m_acTypeDesc[i]) - 1);	
	}
	for ( int i = 0; i < (int)ARRAY_CNT(m_anSpiritItem) && i < pTable->spirititem_size(); ++i )
	{
		m_anSpiritItem[i] = pTable->spirititem(i);
	}
	for ( int i = 0; i < (int)ARRAY_CNT(m_anSpiritItemNum) && i < pTable->spirititemnum_size(); ++i )
	{
		m_anSpiritItemNum[i] = pTable->spirititemnum(i);
	}
	for ( int i = 0; i < (int)ARRAY_CNT(m_anUpgradeMoney) && i < pTable->upgrademoney_size(); ++i )
	{
		m_anUpgradeMoney[i] = pTable->upgrademoney(i);
	}
	for ( int i = 0; i < (int)ARRAY_CNT(m_anDowngradeMoney) && i < pTable->downgrademoney_size(); ++i )
	{
		m_anDowngradeMoney[i] = pTable->downgrademoney(i);
	}
	m_nUpgradeItem = pTable->upgradeitem();
	m_nDowngradeItem = pTable->downgradeitem();
}

int CTplSpiritItem::Initialize()
{
	m_nGrade = 0;
	m_nColorGrade = 0;
	m_nSpiritType = 0;
	m_nLevelLimit = 0;
	m_nEffectPoint = 0;
	
	return 0;
}

void CTplSpiritItem::CreateTlvPB(bool bServer, PBSpiritItem *pItem)
{
	if (pItem == NULL)
	{
		return;
	}

	pItem->set_grade(m_nGrade);
	pItem->set_colorgrade(m_nColorGrade);
	pItem->set_spirittype(m_nSpiritType);
	pItem->set_levellimit(m_nLevelLimit);
	pItem->set_effectpoint(m_nEffectPoint);
	CTplItem::CreateTlvPB(bServer, pItem->mutable_baseinfo());
}

void CTplSpiritItem::SetFromPB(bool bServer, PBSpiritItem *pItem)
{
	if (pItem == NULL)
	{
		return;
	}

	m_nGrade = pItem->grade();
	m_nColorGrade = pItem->colorgrade();
	m_nSpiritType = pItem->spirittype();
	m_nLevelLimit = pItem->levellimit();
	m_nEffectPoint = pItem->effectpoint();
	CTplItem::SetFromPB(bServer, pItem->mutable_baseinfo());
}

//---------------------------------------------
// End	: 实现各类型的SetFromPB
//---------------------------------------------

/*
	修改作者：Edwin
	修改日期：2009-03-05
	修改说明：

	修改作者：Zhaofine
	修改日期：2010/01/01
	修改内容：
		客户端的加载过程修改了一下。兼容数据库文件的访问。
*/

int CDataStatic::Initialize()
{
	PBTemplate* tpTemp = new PBTemplate();
	if ( tpTemp == NULL )
	{
#ifdef LINUX
		PRINTF_LOG( "New PBTemplate() failed.\n" );
#else
		PRINTF_LOG(LOG_FATAL, _T("New PBTemplate() failed.\n"));
#endif
		
		return -1;
	}	

	

#ifdef LINUX
	char* pcRetBuffer = ( char* )malloc( TEMPLATE_CACHE_SIZE );
	if ( pcRetBuffer == NULL )
	{
		return -1;
	}

	std::fstream fstm;
	fstm.open( "../config/data/template/template_server.dat", std::ios::in | std::ios::binary );
	if ( fstm.is_open() == false )
	{
		PRINTF_LOG( "open fsteam failed.\n" );
		return -1;
	}
	
	fstm.read( (char*)&sTemplateVersion, sizeof( int ) );

	char* tpTempBuffer = pcRetBuffer;
	int tLen = 0;

	while( !fstm.eof() && tLen < TEMPLATE_CACHE_SIZE )
	{
		fstm.read( tpTempBuffer, 1 );
		tpTempBuffer += 1;
		tLen ++;
	}

	if ( tpTemp->ParseFromArray( pcRetBuffer, tLen - 1 ) == false )
	{
		PRINTF_LOG( "parse failed.\n" );
		return -1;
	}

	fstm.close();

#ifdef _DEBUG_
	std::ofstream* fLog = new std::ofstream("template.txt");
	*fLog << tpTemp->DebugString() << std::endl;
	delete fLog;
#else
#endif
	free( pcRetBuffer );
	pcRetBuffer = NULL;
#else		

	char* pcRetBuffer = NULL;
	KmBigFile	PakFile;
	KmMemory	TempBuffer;
	// 先从包或文件中读。
	if ( !PakFile.Open(_T("\\data\\template_client.dat")))
	{
		return -1;
	}

	int nRetLength = PakFile.Size();

	if (!TempBuffer.Alloc(nRetLength))
	{
		return -1;
	}
	
	// 读取版本号
	//PakFile.Read( (char*)&sTemplateVersion, sizeof(int) );
	//nRetLength = nRetLength - sizeof( int );
	
	pcRetBuffer = (char*)TempBuffer.GetMemPtr();
	int nReadBytes = 0;
	while (nReadBytes < nRetLength)
	{
		nReadBytes += PakFile.Read(pcRetBuffer + nReadBytes, nRetLength - nReadBytes);
	} 
	PakFile.Close();
	
	
	memcpy(&sTemplateVersion, pcRetBuffer, sizeof(int));
	pcRetBuffer += sizeof(int);

	nRetLength -= sizeof(int);
	if ( tpTemp->ParseFromArray(pcRetBuffer, nRetLength) == false )
	{
		PRINTF_LOG(LOG_FATAL, _T("parse failed.\n"));
		return -1;
	}
	TempBuffer.Free();
#endif

	PBItemSet* tpItemSet = tpTemp->mutable_itemset();
	unsigned int i = 0;
	for ( i = 0; i < (unsigned int) tpItemSet->item_size(); i ++ )
	{
		//PBItemBase* tpBase = tpTemp->mutable_item( i );
		CTplItem* tpItem =( CTplItem* )TPL_NEW( CTplItem );
		if ( tpItem == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItem->SetFromPB( true, tpItemSet->mutable_item( i ) );
#else
		tpItem->SetFromPB( false, tpItemSet->mutable_item( i ) );
#endif
		if ( spTemplate[ tpItem->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItem->mTempID ] );
			spTemplate[ tpItem->mTempID ] = NULL;
		}
		spTemplate[ tpItem->mTempID ] = tpItem;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->bindstone_size(); i ++ )
	{
		//PBItemBindStone* tpBindStone = tpTemp->mutable_bindstone( i );
		CTplItemBindStone* tpItemBindStone = ( CTplItemBindStone* )TPL_NEW(  CTplItemBindStone );
		if ( tpItemBindStone == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemBindStone->SetFromPB( true, tpItemSet->mutable_bindstone( i ) );
#else
		tpItemBindStone->SetFromPB( false, tpItemSet->mutable_bindstone( i ) );
#endif
		if ( spTemplate[ tpItemBindStone->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemBindStone->mTempID ] );
			spTemplate[ tpItemBindStone->mTempID ] = NULL;
		}
		spTemplate[ tpItemBindStone->mTempID ] = tpItemBindStone;		
	}
	for ( i = 0; i < (unsigned int) tpItemSet->blueprint_size(); i ++ )
	{
		//PBItemBluePrint* tpBluePrint = tpTemp->mutable_blueprint( i );
		CTemplateBlueprint* tpItemBluePrint = ( CTemplateBlueprint* )TPL_NEW(  CTemplateBlueprint  );
		if ( tpItemBluePrint == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemBluePrint->SetFromPB( true, tpItemSet->mutable_blueprint( i ) );
#else
		tpItemBluePrint->SetFromPB( false, tpItemSet->mutable_blueprint( i ) );
#endif
		if ( spTemplate[ tpItemBluePrint->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemBluePrint->mTempID ] );
			spTemplate[ tpItemBluePrint->mTempID ] = NULL;
		}
		spTemplate[ tpItemBluePrint->mTempID ] = tpItemBluePrint;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->itembook_size(); i ++ )
	{
		//PBItemBook* tpBook = tpTemp->mutable_itembook( i );
		CTplItemBook* tpItemBook = ( CTplItemBook* )TPL_NEW( CTplItemBook );
		if ( tpItemBook == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemBook->SetFromPB( true, tpItemSet->mutable_itembook( i ) );
#else
		tpItemBook->SetFromPB( false, tpItemSet->mutable_itembook( i ) );
#endif
		if ( spTemplate[ tpItemBook->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemBook->mTempID ] );
			spTemplate[ tpItemBook->mTempID ] = NULL;
		}
		spTemplate[ tpItemBook->mTempID ] = tpItemBook;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->itemcard_size(); i ++ )
	{
		//PBItemCard* tpCard = tpTemp->mutable_itemcard( i );
		CTplItemCard* tpItemCard = ( CTplItemCard* )TPL_NEW( CTplItemCard );
		if ( tpItemCard == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemCard->SetFromPB( true, tpItemSet->mutable_itemcard( i ) );
#else
		tpItemCard->SetFromPB( false, tpItemSet->mutable_itemcard( i ) );
#endif
		if ( spTemplate[ tpItemCard->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemCard->mTempID ] );
			spTemplate[ tpItemCard->mTempID ] = NULL;
		}
		spTemplate[ tpItemCard->mTempID ] = tpItemCard;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->itemexp_size(); i ++ )
	{
		//PBItemExp* tpExp = tpItemSet->mutable_itemexp( i );
		CTplItemExp* tpItemExp = ( CTplItemExp* )TPL_NEW( CTplItemExp );
		if ( tpItemExp == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemExp->SetFromPB( true, tpItemSet->mutable_itemexp( i ) );
#else
		tpItemExp->SetFromPB( false, tpItemSet->mutable_itemexp( i ) );
#endif
		if ( spTemplate[ tpItemExp->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemExp->mTempID ] );
			spTemplate[ tpItemExp->mTempID ] = NULL;
		}
		spTemplate[ tpItemExp->mTempID ] = tpItemExp;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->horse_size(); i ++ )
	{
		//PBItemHorse* tpHorse = tpItemSet->mutable_horse( i );
		CTplItemHorse* tpItemHorse = ( CTplItemHorse* )TPL_NEW( CTplItemHorse );
		if ( tpItemHorse == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemHorse->SetFromPB( true, tpItemSet->mutable_horse( i ) );
#else
		tpItemHorse->SetFromPB( false, tpItemSet->mutable_horse( i ) );
#endif
		if ( spTemplate[ tpItemHorse->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemHorse->mTempID ] );
			spTemplate[ tpItemHorse->mTempID ] = NULL;
		}
		spTemplate[ tpItemHorse->mTempID ] = tpItemHorse;
	}


	for ( i = 0; i < (unsigned int) tpItemSet->jewel_size(); i ++ )
	{
		//PBItemJewel* tpJewel = tpItemSet->mutable_jewel( i );
		CTplItemJewel* tpItemJewel = ( CTplItemJewel* )TPL_NEW( CTplItemJewel );
		if ( tpItemJewel == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemJewel->SetFromPB( true, tpItemSet->mutable_jewel( i ) );
#else
		tpItemJewel->SetFromPB( false, tpItemSet->mutable_jewel( i ) );
#endif
		if ( spTemplate[ tpItemJewel->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemJewel->mTempID ] );
			spTemplate[ tpItemJewel->mTempID ] = NULL;
		}
		spTemplate[ tpItemJewel->mTempID ] = tpItemJewel;
	}

	for ( i = 0; i < (unsigned int) tpItemSet->skillbook_size(); i ++ )
	{
		CTplItemSkillBook* tpItemJewel = ( CTplItemSkillBook* )TPL_NEW(  CTplItemSkillBook  );
		if ( tpItemJewel == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemJewel->SetFromPB( true, tpItemSet->mutable_skillbook( i ) );
#else
		tpItemJewel->SetFromPB( false, tpItemSet->mutable_skillbook( i ) );
#endif
		if ( spTemplate[ tpItemJewel->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemJewel->mTempID ] );
			spTemplate[ tpItemJewel->mTempID ] = NULL;
		}
		spTemplate[ tpItemJewel->mTempID ] = tpItemJewel;
	}


	for ( i = 0; i < (unsigned int) tpItemSet->petegg_size(); i ++ )
	{
		//PBItemPetEgg* tpEgg = tpItemSet->mutable_petegg( i );
		CTplPetEgg* tpItemEgg = ( CTplPetEgg* )TPL_NEW(  CTplPetEgg  );
		if ( tpItemEgg == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemEgg->SetFromPB( true, tpItemSet->mutable_petegg( i ) );
#else
		tpItemEgg->SetFromPB( false, tpItemSet->mutable_petegg( i ) );
#endif
		if ( spTemplate[ tpItemEgg->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemEgg->mTempID ] );
			spTemplate[ tpItemEgg->mTempID ] = NULL;
		}
		spTemplate[ tpItemEgg->mTempID ] = tpItemEgg;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->petfood_size(); i ++ )
	{
		//PBItemPetFood* tpFood = tpItemSet->mutable_petfood( i );
		CTplPetFood* tpItemFood = ( CTplPetFood* )TPL_NEW(  CTplPetFood  );
		if ( tpItemFood == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemFood->SetFromPB( true, tpItemSet->mutable_petfood( i ) );
#else
		tpItemFood->SetFromPB( false, tpItemSet->mutable_petfood( i ) );
#endif
		if ( spTemplate[ tpItemFood->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemFood->mTempID ] );
			spTemplate[ tpItemFood->mTempID ] = NULL;
		}
		spTemplate[ tpItemFood->mTempID ] = tpItemFood;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->physic_size(); i ++ )
	{
		//PBItemPhysic* tpPhysic = tpItemSet->mutable_physic( i );
		CTplItemPhysic* tpItemPhysic = ( CTplItemPhysic* )TPL_NEW(  CTplItemPhysic  );
		if ( tpItemPhysic == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemPhysic->SetFromPB( true, tpItemSet->mutable_physic( i ) );
#else
		tpItemPhysic->SetFromPB( false, tpItemSet->mutable_physic( i ) );
#endif
		if ( spTemplate[ tpItemPhysic->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemPhysic->mTempID ] );
			spTemplate[ tpItemPhysic->mTempID ] = NULL;
		}
		spTemplate[ tpItemPhysic->mTempID ] = tpItemPhysic;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->question_size(); i ++ )
	{
		//PBItemQuestion* tpQuestion = tpItemSet->mutable_question( i );
		CTplQuestionPaper* tpItemQuestion = ( CTplQuestionPaper* )TPL_NEW(  CTplQuestionPaper  );
		if ( tpItemQuestion == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemQuestion->SetFromPB( true, tpItemSet->mutable_question( i ) );
#else
		tpItemQuestion->SetFromPB( false, tpItemSet->mutable_question( i ) );
#endif
		if ( spTemplate[ tpItemQuestion->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemQuestion->mTempID ] );
			spTemplate[ tpItemQuestion->mTempID ] = NULL;
		}
		spTemplate[ tpItemQuestion->mTempID ] = tpItemQuestion;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->relife_size(); i ++ )
	{
		//PBItemRelife* tpRelife = tpItemSet->mutable_relife( i );
		CTplItemRelife* tpItemRelife = ( CTplItemRelife* )TPL_NEW(  CTplItemRelife  );
		if ( tpItemRelife == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemRelife->SetFromPB( true, tpItemSet->mutable_relife( i ) );
#else
		tpItemRelife->SetFromPB( false, tpItemSet->mutable_relife( i ) );
#endif
		if ( spTemplate[ tpItemRelife->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemRelife->mTempID ] );
			spTemplate[ tpItemRelife->mTempID ] = NULL;
		}
		spTemplate[ tpItemRelife->mTempID ] = tpItemRelife;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->return__size(); i ++ )
	{
		//PBItemReturn* tpReturn = tpItemSet->mutable_return_( i );
		CTplItemReturn* tpItemReturn = ( CTplItemReturn* )TPL_NEW(  CTplItemReturn );
		if ( tpItemReturn == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemReturn->SetFromPB( true, tpItemSet->mutable_return_( i ) );
#else
		tpItemReturn->SetFromPB( false, tpItemSet->mutable_return_( i ) );
#endif
		if ( spTemplate[ tpItemReturn->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemReturn->mTempID ] );
			spTemplate[ tpItemReturn->mTempID ] = NULL;
		}
		spTemplate[ tpItemReturn->mTempID ] = tpItemReturn;
	}


	for ( i = 0; i < (unsigned int) tpItemSet->returnpaper_size(); i ++ )
	{
		//PBItemReturnPaper* tpReturnPaper = tpItemSet->mutable_returnpaper( i );
		CTplReturnPaper* tpItemReturnPaper = ( CTplReturnPaper* )TPL_NEW(  CTplReturnPaper  );
		if ( tpItemReturnPaper == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemReturnPaper->SetFromPB( true, tpItemSet->mutable_returnpaper( i ) );
#else
		tpItemReturnPaper->SetFromPB( false, tpItemSet->mutable_returnpaper( i ) );
#endif
		if ( spTemplate[ tpItemReturnPaper->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemReturnPaper->mTempID ] );
			spTemplate[ tpItemReturnPaper->mTempID ] = NULL;
		}
		spTemplate[ tpItemReturnPaper->mTempID ] = tpItemReturnPaper;
	}

	for ( i = 0; i < (unsigned int) tpItemSet->itemcall_size(); i ++ )
	{
		CTplItemCall* tpItem = ( CTplItemCall* )TPL_NEW(  CTplItemCall  );
		if ( tpItem == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItem->SetFromPB( true, tpItemSet->mutable_itemcall( i ) );
#else
		tpItem->SetFromPB( false, tpItemSet->mutable_itemcall( i ) );
#endif
		if ( spTemplate[ tpItem->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItem->mTempID ] );
			spTemplate[ tpItem->mTempID ] = NULL;
		}
		spTemplate[ tpItem->mTempID ] = tpItem;
	}


	for ( i = 0; i < (unsigned int) tpItemSet->rune_size(); i ++ )
	{
		//PBItemRune* tpRune = tpItemSet->mutable_rune( i );
		CTplItemRune* tpItemRune = ( CTplItemRune* )TPL_NEW(  CTplItemRune  );
		if ( tpItemRune == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemRune->SetFromPB( true, tpItemSet->mutable_rune( i ) );
#else
		tpItemRune->SetFromPB( false, tpItemSet->mutable_rune( i ) );
#endif
		if ( spTemplate[ tpItemRune->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemRune->mTempID ] );
			spTemplate[ tpItemRune->mTempID ] = NULL;
		}
		spTemplate[ tpItemRune->mTempID ] = tpItemRune;
		if (tpItemRune->mRuneIndex > 0 &&( tpItemRune->mProcessType & CTplItem::PROCESSTYPE_BINDONPICK ) == 0 )
		{	
			sRuneLevelID[tpItemRune->mLevel][tpItemRune->mRuneIndex-1] = tpItemRune->mTempID;//add by qiufufu
		}

	}
	for ( i = 0; i < (unsigned int) tpItemSet->scroll_size(); i ++ )
	{
		//PBItemScroll* tpScroll = tpItemSet->mutable_scroll( i );
		CTplItemScroll* tpItemScroll = ( CTplItemScroll* )TPL_NEW(  CTplItemScroll  );
		if ( tpItemScroll == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemScroll->SetFromPB( true, tpItemSet->mutable_scroll( i ) );
#else
		tpItemScroll->SetFromPB( false, tpItemSet->mutable_scroll( i ) );
#endif
		if ( spTemplate[ tpItemScroll->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemScroll->mTempID ] );
			spTemplate[ tpItemScroll->mTempID ] = NULL;
		}
		spTemplate[ tpItemScroll->mTempID ] = tpItemScroll;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->sequence_size(); i ++ )
	{
		//PBItemSequence* tpSequence = tpItemSet->mutable_sequence( i );
		CItemSequence* tpItemSequence = ( CItemSequence* )TPL_NEW(  CItemSequence  );
		if ( tpItemSequence == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemSequence->SetFromPB( true, tpItemSet->mutable_sequence( i ) );
#else
		tpItemSequence->SetFromPB( false, tpItemSet->mutable_sequence( i ) );
#endif
		if ( spTemplate[ tpItemSequence->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemSequence->mTempID ] );
			spTemplate[ tpItemSequence->mTempID ] = NULL;
		}
		spTemplate[ tpItemSequence->mTempID ] = tpItemSequence;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->shout_size(); i ++ )
	{
		//PBItemShout* tpShout = tpItemSet->mutable_shout( i );
		CTplItemShout* tpItemShout = ( CTplItemShout* )TPL_NEW(  CTplItemShout );
		if ( tpItemShout == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemShout->SetFromPB( true, tpItemSet->mutable_shout( i ) );
#else
		tpItemShout->SetFromPB( false, tpItemSet->mutable_shout( i ) );
#endif
		if ( spTemplate[ tpItemShout->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemShout->mTempID ] );
			spTemplate[ tpItemShout->mTempID ] = NULL;
		}
		spTemplate[ tpItemShout->mTempID ] = tpItemShout;
		sBugleItemId = tpItemShout->mTempID;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->itemskill_size(); i ++ )
	{
		//PBItemSkill* tpSkill = tpItemSet->mutable_itemskill( i );
		CTplItemSkill* tpItemSkill = ( CTplItemSkill* )TPL_NEW(  CTplItemSkill  );
		if ( tpItemSkill == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemSkill->SetFromPB( true, tpItemSet->mutable_itemskill( i ) );
#else
		tpItemSkill->SetFromPB( false, tpItemSet->mutable_itemskill( i ) );
#endif
		if ( spTemplate[ tpItemSkill->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemSkill->mTempID ] );
			spTemplate[ tpItemSkill->mTempID ] = NULL;
		}
		spTemplate[ tpItemSkill->mTempID ] = tpItemSkill;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->autoshout_size(); i ++ )
	{
		CTplItemAutoShout* tpShout = ( CTplItemAutoShout* )TPL_NEW(  CTplItemAutoShout  );
		if ( tpShout == NULL )
		{
			break;
		}
#ifdef LINUX
		tpShout->SetFromPB( true, tpItemSet->mutable_autoshout( i ) );
#else
		tpShout->SetFromPB( false, tpItemSet->mutable_autoshout( i ) );
#endif
		if ( spTemplate[ tpShout->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpShout->mTempID ] );
			spTemplate[ tpShout->mTempID ] = NULL;
		}
		spTemplate[ tpShout->mTempID ] = tpShout;
		sAutoChatItemId = tpShout->mTempID;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->stove_size(); i ++ )
	{
		//PBItemStove* tpStove = tpItemSet->mutable_stove( i );
		CTplStove* tpItemStove = ( CTplStove* )TPL_NEW(  CTplStove  );
		if ( tpItemStove == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemStove->SetFromPB( true, tpItemSet->mutable_stove( i ) );
#else
		tpItemStove->SetFromPB( false, tpItemSet->mutable_stove( i ) );
#endif
		if ( spTemplate[ tpItemStove->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemStove->mTempID ] );
			spTemplate[ tpItemStove->mTempID ] = NULL;
		}
		spTemplate[ tpItemStove->mTempID ] = tpItemStove;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->stuff_size(); i ++ )
	{
		//PBItemStuff* tpStuff = tpItemSet->mutable_stuff( i );
		CTplItemStuff* tpItemStuff = ( CTplItemStuff* )TPL_NEW(  CTplItemStuff  );
		if ( tpItemStuff == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemStuff->SetFromPB( true, tpItemSet->mutable_stuff( i ) );
#else
		tpItemStuff->SetFromPB( false, tpItemSet->mutable_stuff( i ) );
#endif
		if ( spTemplate[ tpItemStuff->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemStuff->mTempID ] );
			spTemplate[ tpItemStuff->mTempID ] = NULL;
		}
		spTemplate[ tpItemStuff->mTempID ] = tpItemStuff;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->itemtask_size(); i ++ )
	{
		//PBItemTask* tpTask = tpItemSet->mutable_itemtask( i );
		CTplItemTask* tpItemTask = ( CTplItemTask* )TPL_NEW(  CTplItemTask  );
		if ( tpItemTask == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemTask->SetFromPB( true, tpItemSet->mutable_itemtask( i ) );
#else
		tpItemTask->SetFromPB( false, tpItemSet->mutable_itemtask( i ) );
#endif
		if ( spTemplate[ tpItemTask->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemTask->mTempID ] );
			spTemplate[ tpItemTask->mTempID ] = NULL;
		}
		spTemplate[ tpItemTask->mTempID ] = tpItemTask;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->teleport_size(); i ++ )
	{
		//PBItemTeleport* tpTeleport = tpItemSet->mutable_teleport( i );
		CTplItemTeleport* tpItemTele = ( CTplItemTeleport* )TPL_NEW(  CTplItemTeleport  );
		if ( tpItemTele == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemTele->SetFromPB( true, tpItemSet->mutable_teleport( i ) );
#else
		tpItemTele->SetFromPB( false, tpItemSet->mutable_teleport( i ) );
#endif
		if ( spTemplate[ tpItemTele->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemTele->mTempID ] );
			spTemplate[ tpItemTele->mTempID ] = NULL;
		}
		spTemplate[ tpItemTele->mTempID ] = tpItemTele;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->telesymbol_size(); i ++ )
	{
		//PBItemTeleSymbol* tpTeleSymbol = tpItemSet->mutable_telesymbol( i );
		CTplTeleSymbol* tpItemTeleSymbol = ( CTplTeleSymbol* )TPL_NEW(  CTplTeleSymbol  );
		if ( tpItemTeleSymbol == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemTeleSymbol->SetFromPB( true, tpItemSet->mutable_telesymbol( i ) );
#else
		tpItemTeleSymbol->SetFromPB( false, tpItemSet->mutable_telesymbol( i ) );
#endif
		if ( spTemplate[ tpItemTeleSymbol->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemTeleSymbol->mTempID ] );
			spTemplate[ tpItemTeleSymbol->mTempID ] = NULL;
		}
		spTemplate[ tpItemTeleSymbol->mTempID ] = tpItemTeleSymbol;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->upgleast_size(); i ++ )
	{
		//PBItemUpgLeast* tpUpgLeast = tpItemSet->mutable_upgleast( i );
		CTplUpgLeast* tpItemUpgLeast = ( CTplUpgLeast* )TPL_NEW(  CTplUpgLeast  );
		if ( tpItemUpgLeast == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemUpgLeast->SetFromPB( true, tpItemSet->mutable_upgleast( i ) );
#else
		tpItemUpgLeast->SetFromPB( false, tpItemSet->mutable_upgleast( i ) );
#endif
		if ( spTemplate[ tpItemUpgLeast->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemUpgLeast->mTempID ] );
			spTemplate[ tpItemUpgLeast->mTempID ] = NULL;
		}
		spTemplate[ tpItemUpgLeast->mTempID ] = tpItemUpgLeast;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->upgluck_size(); i ++ )
	{
		//PBItemUpgLuck* tpUpgLuck = tpItemSet->mutable_upgluck( i );
		CTplUpgLuck* tpItemUpgLuck = ( CTplUpgLuck* )TPL_NEW(  CTplUpgLuck  );
		if ( tpItemUpgLuck == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemUpgLuck->SetFromPB( true, tpItemSet->mutable_upgluck( i ) );
#else
		tpItemUpgLuck->SetFromPB( false, tpItemSet->mutable_upgluck( i ) );
#endif
		if ( spTemplate[ tpItemUpgLuck->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemUpgLuck->mTempID ] );
			spTemplate[ tpItemUpgLuck->mTempID ] = NULL;
		}
		spTemplate[ tpItemUpgLuck->mTempID ] = tpItemUpgLuck;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->yuanbug_size(); i ++ )
	{
		//PBItemYuanBug* tpYuanBug = tpItemSet->mutable_yuanbug( i );
		CTplItemYuanBug* tpItemYuanBug = ( CTplItemYuanBug* )TPL_NEW(  CTplItemYuanBug  );
		if ( tpItemYuanBug == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemYuanBug->SetFromPB( true, tpItemSet->mutable_yuanbug( i ) );
#else
		tpItemYuanBug->SetFromPB( false, tpItemSet->mutable_yuanbug( i ) );
#endif
		if ( spTemplate[ tpItemYuanBug->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemYuanBug->mTempID ] );
			spTemplate[ tpItemYuanBug->mTempID ] = NULL;
		}
		spTemplate[ tpItemYuanBug->mTempID ] = tpItemYuanBug;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->yuanegg_size(); i ++ )
	{
		//PBItemYuanEgg* tpYuanEgg = tpItemSet->mutable_yuanegg( i );
		CTplItemYuanEgg* tpItemYuanEgg = ( CTplItemYuanEgg* )TPL_NEW(  CTplItemYuanEgg  );
		if ( tpItemYuanEgg == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemYuanEgg->SetFromPB( true, tpItemSet->mutable_yuanegg( i ) );
#else
		tpItemYuanEgg->SetFromPB( false, tpItemSet->mutable_yuanegg( i ) );
#endif
		if ( spTemplate[ tpItemYuanEgg->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemYuanEgg->mTempID ] );
			spTemplate[ tpItemYuanEgg->mTempID ] = NULL;
		}
		spTemplate[ tpItemYuanEgg->mTempID ] = tpItemYuanEgg;
	}
	PBSkillSet* tpSkillSet = tpTemp->mutable_skillset();
	for ( i = 0; i < (unsigned int) tpSkillSet->aureoleskill_size(); i ++ )
	{
		//PBAureoleSkill* tpAureole = tpTemp->mutable_aureoleskill( i );
		CTemplateAureoleSkill* tpAureoleSkill = ( CTemplateAureoleSkill* )TPL_NEW(  CTemplateAureoleSkill  );
		if ( tpAureoleSkill == NULL )
		{
			break;
		}
#ifdef LINUX
		tpAureoleSkill->SetFromPB( true, tpSkillSet->mutable_aureoleskill( i ) );
#else
		tpAureoleSkill->SetFromPB( false, tpSkillSet->mutable_aureoleskill( i ) );
#endif
		if ( spSkill[ tpAureoleSkill->mSkillID ][ tpAureoleSkill->mSkillLevel ] == NULL 
			&& spTemplate[ tpAureoleSkill->mTempID ] == NULL )
		{
			spSkill[ tpAureoleSkill->mSkillID ][ tpAureoleSkill->mSkillLevel ] = tpAureoleSkill;
			spTemplate[ tpAureoleSkill->mTempID ] = tpAureoleSkill;
		}
	}
	for ( i = 0; i < (unsigned int) tpTemp->battlepet_size(); i ++ )
	{
		//PBBattlePet* tpBattlePet = tpTemp->mutable_battlepet( i );
		CTemplateBattlePet* tpTplBattle = ( CTemplateBattlePet* )TPL_NEW(  CTemplateBattlePet  );
		if ( tpTplBattle == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplBattle->SetFromPB( true, tpTemp->mutable_battlepet( i ) );
#else
		tpTplBattle->SetFromPB( false, tpTemp->mutable_battlepet( i ) );
#endif
		if ( spTemplate[ tpTplBattle->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplBattle->mTempID ] );
			spTemplate[ tpTplBattle->mTempID ] = NULL;
		}
		spTemplate[ tpTplBattle->mTempID ] = tpTplBattle;
	}
	PBConfigTable* tpConfigTable = tpTemp->mutable_configtable();
	for ( i = 0; i < (unsigned int) tpConfigTable->beginner_size(); i ++ )
	{
		//PBBeginner* tpBeg = tpTemp->mutable_beginner( i );
		CTemplateBeginner* tpTplBeg = ( CTemplateBeginner* )TPL_NEW(  CTemplateBeginner  );
		if ( tpTplBeg == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplBeg->SetFromPB( true, tpConfigTable->mutable_beginner( i ) );
#else
		tpTplBeg->SetFromPB( false, tpConfigTable->mutable_beginner( i ) );
#endif
		if ( spTemplate[ tpTplBeg->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplBeg->mTempID ] );
			spTemplate[ tpTplBeg->mTempID ] = NULL;
		}
		spTemplate[ tpTplBeg->mTempID ] = tpTplBeg;
		spBeginner = tpTplBeg;
	}

	for ( i = 0; i < (unsigned int) tpConfigTable->mwdecompostab_size(); i ++ )
	{
		//PBBeginner* tpBeg = tpTemp->mutable_beginner( i );
		CMagicWeaponDecomposeTable* tpTplBeg = ( CMagicWeaponDecomposeTable* )TPL_NEW(  CMagicWeaponDecomposeTable  );
		if ( tpTplBeg == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplBeg->SetFromPB( true, tpConfigTable->mutable_mwdecompostab( i ) );
#else
		tpTplBeg->SetFromPB( false, tpConfigTable->mutable_mwdecompostab( i ) );
#endif
		if ( spTemplate[ tpTplBeg->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplBeg->mTempID ] );
			spTemplate[ tpTplBeg->mTempID ] = NULL;
		}
		spTemplate[ tpTplBeg->mTempID ] = tpTplBeg;
	}

	for ( i = 0; i < (unsigned int) tpConfigTable->mwquickuptab_size(); i ++ )
	{
		//PBBeginner* tpBeg = tpTemp->mutable_beginner( i );
		CMagicWeaponQuickUpTable* tpTplBeg = ( CMagicWeaponQuickUpTable* )TPL_NEW(  CMagicWeaponQuickUpTable  );
		if ( tpTplBeg == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplBeg->SetFromPB( true, tpConfigTable->mutable_mwquickuptab( i ) );
#else
		tpTplBeg->SetFromPB( false, tpConfigTable->mutable_mwquickuptab( i ) );
#endif
		if ( spTemplate[ tpTplBeg->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplBeg->mTempID ] );
			spTemplate[ tpTplBeg->mTempID ] = NULL;
		}
		spTemplate[ tpTplBeg->mTempID ] = tpTplBeg;
	}


	for ( i = 0; i < (unsigned int) tpSkillSet->buff_size(); i ++ )
	{
		//PBBuff* tpBuff = tpSkillSet->mutable_buff( i );
		CTemplateBuff* tpTplBuff = ( CTemplateBuff* )TPL_NEW(  CTemplateBuff  );
		if ( tpTplBuff == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplBuff->SetFromPB( true, tpSkillSet->mutable_buff( i ) );
#else
		tpTplBuff->SetFromPB( false, tpSkillSet->mutable_buff( i ) );
#endif
		if ( spTemplate[ tpTplBuff->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplBuff->mTempID ] );
			spTemplate[ tpTplBuff->mTempID ] = NULL;
		}
		spTemplate[ tpTplBuff->mTempID ] = tpTplBuff;
	}
	PBEquipSet* tpEquipSet = tpTemp->mutable_equipset();
	for ( i = 0; i < (unsigned int) tpEquipSet->compose_size(); i ++ )
	{
		//PBCompose* tpCom = tpEquipSet->mutable_compose( i );
		CTemplateCompose* tpTplCom = ( CTemplateCompose* )TPL_NEW(  CTemplateCompose  );
		if ( tpTplCom == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplCom->SetFromPB( true, tpEquipSet->mutable_compose( i ) );
#else
		tpTplCom->SetFromPB( false, tpEquipSet->mutable_compose( i ) );
#endif
		if ( spTemplate[ tpTplCom->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplCom->mTempID ] );
			spTemplate[ tpTplCom->mTempID ] = NULL;
		}
		spTemplate[ tpTplCom->mTempID ] = tpTplCom;
	}
	
	// 生活技能信息
	mCollect.Initialize();
	mGoldMake.Initialize();
	mArtificer.Initialize();
	mCooker.Initialize();
	mPillMaker.Initialize();
	
	for ( i = 0; i < (unsigned int) tpEquipSet->multicompose_size(); i ++ )
	{
		//PBCompose* tpCom = tpEquipSet->mutable_compose( i );
		CTemplateMultiCompose* tpTplCom = ( CTemplateMultiCompose* )TPL_NEW(  CTemplateMultiCompose  );
		if ( tpTplCom == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplCom->SetFromPB( true, tpEquipSet->mutable_multicompose( i ) );
#else
		tpTplCom->SetFromPB( false, tpEquipSet->mutable_multicompose( i ) );
#endif
		if ( spTemplate[ tpTplCom->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplCom->mTempID ] );
			spTemplate[ tpTplCom->mTempID ] = NULL;
		}
		spTemplate[ tpTplCom->mTempID ] = tpTplCom;
		if( tpTplCom->mOrder > 0 && tpTplCom->mOrder < MAX_COMPOSE )
		{
			sComposeID[ tpTplCom->mOrder ] = tpTplCom->mTempID;
		}	
		
		CDataStatic::InsertComposeInfo( tpTplCom->mLifeSkillType, tpTplCom->mTempID, tpTplCom->mSkillKind, tpTplCom->mSpecializedDegree, tpTplCom->mLifeComposeType );
	}
	for ( i = 0; i < (unsigned int) tpConfigTable->data_size(); i ++ )
	{
		//PBData* tpData = tpConfigTable->mutable_data( i );
		CTemplateData* tpTplData = ( CTemplateData* )TPL_NEW( CTemplateData  );
		if ( tpTplData == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplData->SetFromPB( true, tpConfigTable->mutable_data( i ) );
#else
		tpTplData->SetFromPB( false, tpConfigTable->mutable_data( i ) );
#endif
		if ( spTemplate[ tpTplData->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplData->mTempID ] );
			spTemplate[ tpTplData->mTempID ] = NULL;
		}
		spTemplate[ tpTplData->mTempID ] = tpTplData;
		spData = tpTplData;
	}
	for ( i = 0; i < (unsigned int) tpConfigTable->diploid_size(); i ++ )
	{
		//PBDiploidTable* tpDiploid = tpConfigTable->mutable_diploid( i );
		CTemplateDiploidTimeTable* tpTplDiploid = ( CTemplateDiploidTimeTable* )TPL_NEW(  CTemplateDiploidTimeTable  );
		if ( tpTplDiploid == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplDiploid->SetFromPB( true, tpConfigTable->mutable_diploid( i ) );
#else
		tpTplDiploid->SetFromPB( false, tpConfigTable->mutable_diploid( i ) );
#endif
		if ( spTemplate[ tpTplDiploid->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplDiploid->mTempID ] );
			spTemplate[ tpTplDiploid->mTempID ] = NULL;
		}
		spTemplate[ tpTplDiploid->mTempID ] = tpTplDiploid;
		spDoubleTimeTable = tpTplDiploid;
	}
	for ( i = 0; i < (unsigned int) tpConfigTable->droplevel_size(); i ++ )
	{
		//PBDropLevel* tpLevel = tpConfigTable->mutable_droplevel( i );
		CTemplateDropLevel* tpTplDropLevel = ( CTemplateDropLevel* )TPL_NEW(  CTemplateDropLevel  );
		if ( tpTplDropLevel == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplDropLevel->SetFromPB( true, tpConfigTable->mutable_droplevel( i ) );
#else
		tpTplDropLevel->SetFromPB( false, tpConfigTable->mutable_droplevel( i ) );
#endif
		if ( spTemplate[ tpTplDropLevel->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplDropLevel->mTempID ] );
			spTemplate[ tpTplDropLevel->mTempID ] = NULL;
		}
		spTemplate[ tpTplDropLevel->mTempID ] = tpTplDropLevel;
		spDropLevel = tpTplDropLevel;
	}
	for ( i = 0; i < (unsigned int) tpConfigTable->droptable_size(); i ++ )
	{
		//PBDropTable* tpDropTable = tpConfigTable->mutable_droptable( i );
		CTemplateDrop* tpTplDropTable = ( CTemplateDrop* )TPL_NEW(  CTemplateDrop  );
		if ( tpTplDropTable == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplDropTable->SetFromPB( true, tpConfigTable->mutable_droptable( i ) );
#else
#ifdef _DEBUG
		tpTplDropTable->SetFromPB( true, tpConfigTable->mutable_droptable( i ) );
#else
		tpTplDropTable->SetFromPB( false, tpConfigTable->mutable_droptable( i ) );
#endif
#endif
		if ( spTemplate[ tpTplDropTable->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplDropTable->mTempID ] );
			spTemplate[ tpTplDropTable->mTempID ] = NULL;
		}
		spTemplate[ tpTplDropTable->mTempID ] = tpTplDropTable;
	}
	for ( i = 0; i < (unsigned int) tpEquipSet->equiparmor_size(); i ++ )
	{
		//PBEquipArmor* tpArmor = tpEquipSet->mutable_equiparmor( i );
		CTemplateArmor* tpEquipArmor = ( CTemplateArmor* )TPL_NEW(  CTemplateArmor  );
		if ( tpEquipArmor == NULL )
		{
			break;
		}
#ifdef LINUX
		tpEquipArmor->SetFromPB( true, tpEquipSet->mutable_equiparmor( i ) );
#else
		tpEquipArmor->SetFromPB( false, tpEquipSet->mutable_equiparmor( i ) );
#endif
		if ( spTemplate[ tpEquipArmor->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpEquipArmor->mTempID ] );
			spTemplate[ tpEquipArmor->mTempID ] = NULL;
		}
		spTemplate[ tpEquipArmor->mTempID ] = tpEquipArmor;
	}
	for ( i = 0; i < (unsigned int) tpEquipSet->equipfashion_size(); i ++ )
	{
		//PBEquipFashion* tpFashion = tpEquipSet->mutable_equipfashion( i );
		CTemplateFashion* tpTplFashion = ( CTemplateFashion* )TPL_NEW(  CTemplateFashion  );
		if ( tpTplFashion == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplFashion->SetFromPB( true, tpEquipSet->mutable_equipfashion( i ) );
#else
		tpTplFashion->SetFromPB( false, tpEquipSet->mutable_equipfashion( i ) );
#endif
		if ( spTemplate[ tpTplFashion->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplFashion->mTempID ] );
			spTemplate[ tpTplFashion->mTempID ] = NULL;
		}
		spTemplate[ tpTplFashion->mTempID ] = tpTplFashion;
	}
	for ( i = 0; i < (unsigned int) tpEquipSet->equipment_size(); i ++ )
	{
		//PBEquipment* tpEquip = tpEquipSet->mutable_equipment( i );
		CTemplateEquipment* tpTplEquip = ( CTemplateEquipment* )TPL_NEW(  CTemplateEquipment  );
		if ( tpTplEquip == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplEquip->SetFromPB( true, tpEquipSet->mutable_equipment( i ) );
#else
		tpTplEquip->SetFromPB( false, tpEquipSet->mutable_equipment( i ) );
#endif
		if ( spTemplate[ tpTplEquip->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplEquip->mTempID ] );
			spTemplate[ tpTplEquip->mTempID ] = NULL;
		}
		spTemplate[ tpTplEquip->mTempID ] = tpTplEquip;
	}
	for ( i = 0; i < (unsigned int) tpEquipSet->equipupg_size(); i ++ )
	{
		//PBEquipUpg* tpUpg = tpEquipSet->mutable_equipupg( i );
		CTemplateEquipUpg* tpTplUpg = ( CTemplateEquipUpg* )TPL_NEW(  CTemplateEquipUpg  );
		if ( tpTplUpg == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplUpg->SetFromPB( true, tpEquipSet->mutable_equipupg( i ) );
#else
		tpTplUpg->SetFromPB( false, tpEquipSet->mutable_equipupg( i ) );
#endif
		if ( spTemplate[ tpTplUpg->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplUpg->mTempID ] );
			spTemplate[ tpTplUpg->mTempID ] = NULL;
		}
		spTemplate[ tpTplUpg->mTempID ] = tpTplUpg;
		spUpgrade = tpTplUpg;
	}
	for ( i = 0; i < (unsigned int) tpEquipSet->weapon_size(); i ++ )
	{
		//PBEquipWeapon* tpWeapon = tpEquipSet->mutable_weapon( i );
		CTemplateWeapon* tpTplWeapon = ( CTemplateWeapon* )TPL_NEW(  CTemplateWeapon  );
		if ( tpTplWeapon == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplWeapon->SetFromPB( true, tpEquipSet->mutable_weapon( i ) );
#else
		tpTplWeapon->SetFromPB( false, tpEquipSet->mutable_weapon( i ) );
#endif
		if ( spTemplate[ tpTplWeapon->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplWeapon->mTempID ] );
			spTemplate[ tpTplWeapon->mTempID ] = NULL;
		}
		spTemplate[ tpTplWeapon->mTempID ] = tpTplWeapon;
	}
	for ( i = 0; i < (unsigned int) tpTemp->honorobtain_size(); i ++ )
	{
		//PBHonorObtain* tpObtain = tpTemp->mutable_honorobtain( i );
		CTemplateHonorObtain* tpTplObtain = ( CTemplateHonorObtain* )TPL_NEW(  CTemplateHonorObtain  );
		if ( tpTplObtain == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplObtain->SetFromPB( true, tpTemp->mutable_honorobtain( i ) );
#else
		tpTplObtain->SetFromPB( false, tpTemp->mutable_honorobtain( i ) );
#endif
		if ( spTemplate[ tpTplObtain->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplObtain->mTempID ] );
			spTemplate[ tpTplObtain->mTempID ] = NULL;
		}
		spTemplate[ tpTplObtain->mTempID ] = tpTplObtain;
		spHonorObtain = tpTplObtain;
	}
	for ( i = 0; i < (unsigned int) tpTemp->honortoexp_size(); i ++ )
	{
		//PBHonorToExp* tpToExp = tpTemp->mutable_honortoexp( i );
		CTemplateHonorToExp* tpTplToExp = ( CTemplateHonorToExp* )TPL_NEW(  CTemplateHonorToExp  );
		if ( tpTplToExp == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplToExp->SetFromPB( true, tpTemp->mutable_honortoexp( i ) );
#else
		tpTplToExp->SetFromPB( false, tpTemp->mutable_honortoexp( i ) );
#endif
		if ( spTemplate[ tpTplToExp->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplToExp->mTempID ] );
			spTemplate[ tpTplToExp->mTempID ] = NULL;
		}
		spTemplate[ tpTplToExp->mTempID ] = tpTplToExp;
		spHonorToExp = tpTplToExp;
	}
	for ( i = 0; i < (unsigned int) tpTemp->levelexp_size(); i ++ )
	{
		//PBLevelExp* tpLevelExp = tpTemp->mutable_levelexp( i );
		CTemplateLevelExp* tpTplLevelExp = ( CTemplateLevelExp* )TPL_NEW(  CTemplateLevelExp  );
		if ( tpTplLevelExp == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplLevelExp->SetFromPB( true, tpTemp->mutable_levelexp( i ) );
#else
		tpTplLevelExp->SetFromPB( false, tpTemp->mutable_levelexp( i ) );
#endif
		if ( spTemplate[ tpTplLevelExp->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplLevelExp->mTempID ] );
			spTemplate[ tpTplLevelExp->mTempID ] = NULL;
		}
		spTemplate[ tpTplLevelExp->mTempID ] = tpTplLevelExp;

        //TODO  经验表id 不会改变
        if( tpTplLevelExp->mTempID == 7305 )
        {
		    spLevelExp = tpTplLevelExp;
        }
	}
	for ( i = 0; i < (unsigned int) tpTemp->levelpro_size(); i ++ )
	{
		//PBLevelProperty* tpLevelPro = tpTemp->mutable_levelpro( i );
		CTemplateLevelProperty* tpTplLevelPro = ( CTemplateLevelProperty* )TPL_NEW(  CTemplateLevelProperty  );
		if ( tpTplLevelPro == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplLevelPro->SetFromPB( true, tpTemp->mutable_levelpro( i ) );
#else
		tpTplLevelPro->SetFromPB( false, tpTemp->mutable_levelpro( i ) );
#endif
		if ( spTemplate[ tpTplLevelPro->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplLevelPro->mTempID ] );
			spTemplate[ tpTplLevelPro->mTempID ] = NULL;
		}
		spTemplate[ tpTplLevelPro->mTempID ] = tpTplLevelPro;
		spLevelProperty[ tpTplLevelPro->mOption ] = tpTplLevelPro;
	}
	//for ( i = 0; i < (unsigned int) tpTemp->taskawds_size(); i ++ )
	//{
	//	//PBLoopTaskAwds* tpTaskAwds = tpTemp->mutable_taskawds( i );
	//	CTempLoopTaskAwds* tpTplTaskAwds = new CTempLoopTaskAwds();
	//	if ( tpTplTaskAwds == NULL )
	//	{
	//		break;
	//	}
	//	tpTplTaskAwds->SetFromPB( tpTemp->mutable_taskawds( i ) );
	//	if ( spTemplate[ tpTplTaskAwds->mTempID ] != NULL )
	//	{
	//		TPL_DELETE( spTemplate[ tpTplTaskAwds->mTempID ] );
	//		spTemplate[ tpTplTaskAwds->mTempID ] = NULL;
	//	}
	//	spTemplate[ tpTplTaskAwds->mTempID ] = tpTplTaskAwds;
	//	spLoopTaskAwds = tpTplTaskAwds;
	//}
	for ( i = 0; i < (unsigned int) tpTemp->metier_size(); i ++ )
	{
		//PBMetier* tpMetier = tpTemp->mutable_metier( i );
		CTemplateMetier* tpTplMetier = ( CTemplateMetier* )TPL_NEW(  CTemplateMetier  );
		if ( tpTplMetier == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplMetier->SetFromPB( true, tpTemp->mutable_metier( i ) );
#else
		tpTplMetier->SetFromPB( false, tpTemp->mutable_metier( i ) );
#endif
		if ( spTemplate[ tpTplMetier->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplMetier->mTempID ] );
			spTemplate[ tpTplMetier->mTempID ] = NULL;
		}
		spTemplate[ tpTplMetier->mTempID ] = tpTplMetier;
		sMetierID[ tpTplMetier->mMetierID ] = tpTplMetier->mTempID;
	}
	for ( i = 0; i < (unsigned int) tpTemp->metierpro_size(); i ++ )
	{
		//PBMetierPro* tpMetierPro = tpTemp->mutable_metierpro( i );
		CTemplateMetierProperty* tpTplMetierPro = ( CTemplateMetierProperty* )TPL_NEW(  CTemplateMetierProperty  );
		if ( tpTplMetierPro == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplMetierPro->SetFromPB( true, tpTemp->mutable_metierpro( i ) );
#else
		tpTplMetierPro->SetFromPB( false, tpTemp->mutable_metierpro( i ) );
#endif
		if ( spTemplate[ tpTplMetierPro->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplMetierPro->mTempID ] );
			spTemplate[ tpTplMetierPro->mTempID ] = NULL;
		}
		spTemplate[ tpTplMetierPro->mTempID ] = tpTplMetierPro;
		spMetierProperty[ tpTplMetierPro->mMetierRequire ][ tpTplMetierPro->mOption ] = tpTplMetierPro;
	}
	for ( i = 0; i < (unsigned int) tpTemp->mine_size(); i ++ )
	{
		//PBMine* tpMine = tpTemp->mutable_mine( i );
		CTemplateMine* tpTplMine = ( CTemplateMine* )TPL_NEW(  CTemplateMine  );
		if ( tpTplMine == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplMine->SetFromPB( true, tpTemp->mutable_mine( i ) );
#else
		tpTplMine->SetFromPB( false, tpTemp->mutable_mine( i ) );
#endif
		if ( spTemplate[ tpTplMine->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplMine->mTempID ] );
			spTemplate[ tpTplMine->mTempID ] = NULL;
		}
		spTemplate[ tpTplMine->mTempID ] = tpTplMine;
	}
	for ( i = 0; i < (unsigned int) tpSkillSet->normalskill_size(); i ++ )
	{
		//PBNormalSkill* tpNormalSkill = tpSkillSet->mutable_normalskill( i );
		CTemplateNormalSkill* tpTplNormalSkill = ( CTemplateNormalSkill* )TPL_NEW(  CTemplateNormalSkill  );
		if ( tpTplNormalSkill == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplNormalSkill->SetFromPB( true, tpSkillSet->mutable_normalskill( i ) );
#else
		tpTplNormalSkill->SetFromPB( false, tpSkillSet->mutable_normalskill( i ) );
#endif
		if ( spTemplate[ tpTplNormalSkill->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplNormalSkill->mTempID ] );
			spTemplate[ tpTplNormalSkill->mTempID ] = NULL;
		}
		spTemplate[ tpTplNormalSkill->mTempID ] = tpTplNormalSkill;
		spSkill[ tpTplNormalSkill->mSkillID ][ tpTplNormalSkill->mSkillLevel ] = tpTplNormalSkill;
	}
	for ( i = 0; i < (unsigned int) tpTemp->npc_size(); i ++ )
	{
		//PBNpc* tpNpc = tpTemp->mutable_npc( i );
		CTemplateNpc* tpTplNpc = ( CTemplateNpc* )TPL_NEW(  CTemplateNpc  );
		if ( tpTplNpc == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplNpc->SetFromPB( true, tpTemp->mutable_npc( i ) );
#else
		tpTplNpc->SetFromPB( false, tpTemp->mutable_npc( i ) );
#endif
		if ( spTemplate[ tpTplNpc->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplNpc->mTempID ] );
			spTemplate[ tpTplNpc->mTempID ] = NULL;
		}
		spTemplate[ tpTplNpc->mTempID ] = tpTplNpc;
	}
	for ( i = 0; i < (unsigned int) tpTemp->npccollect_size(); i ++ )
	{
		//PBNPCCollect* tpNpcCollect = tpTemp->mutable_npccollect( i );
		CTemplateCollect* tpTplCollect = ( CTemplateCollect* )TPL_NEW(  CTemplateCollect  );
		if ( tpTplCollect == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplCollect->SetFromPB( true, tpTemp->mutable_npccollect( i ) );
#else
		tpTplCollect->SetFromPB( false, tpTemp->mutable_npccollect( i ) );
#endif
		if ( spTemplate[ tpTplCollect->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplCollect->mTempID ] );
			spTemplate[ tpTplCollect->mTempID ] = NULL;
		}
		spTemplate[ tpTplCollect->mTempID ] = tpTplCollect;
	}
	for ( i = 0; i < (unsigned int) tpTemp->box_size(); i ++ )
	{
		//PBOgreBox* tpBox = tpTemp->mutable_box( i );
		CTemplateBox* tpTplBox = ( CTemplateBox* )TPL_NEW(  CTemplateBox  );
		if ( tpTplBox == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplBox->SetFromPB( true, tpTemp->mutable_box( i ) );
#else
		tpTplBox->SetFromPB( false, tpTemp->mutable_box( i ) );
#endif
		if ( spTemplate[ tpTplBox->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplBox->mTempID ] );
			spTemplate[ tpTplBox->mTempID ] = NULL;
		}
		spTemplate[ tpTplBox->mTempID ] = tpTplBox;
	}
	for ( i = 0; i < (unsigned int) tpTemp->ogreproperty_size(); i ++ )
	{
		//PBOgreProperty* tpOgrePro = tpTemp->mutable_ogreproperty( i );
		CTemplateOgre* tpTplOgre = ( CTemplateOgre* )TPL_NEW(  CTemplateOgre  );
		if ( tpTplOgre == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplOgre->SetFromPB( true, tpTemp->mutable_ogreproperty( i ) );
#else
		tpTplOgre->SetFromPB( false, tpTemp->mutable_ogreproperty( i ) );
#endif
		if ( spTemplate[ tpTplOgre->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplOgre->mTempID ] );
			spTemplate[ tpTplOgre->mTempID ] = NULL;
		}
		spTemplate[ tpTplOgre->mTempID ] = tpTplOgre;
	}
	for ( i = 0; i < (unsigned int) tpConfigTable->ogretran_size(); i ++ )
	{
		//PBOgreTran* tpOgreTran = tpConfigTable->mutable_ogretran( i );
		CTemplateOgreTran* tpTplOgreTran = ( CTemplateOgreTran* )TPL_NEW(  CTemplateOgreTran  );
		if ( tpTplOgreTran == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplOgreTran->SetFromPB( true, tpConfigTable->mutable_ogretran( i ) );
#else
		tpTplOgreTran->SetFromPB( false, tpConfigTable->mutable_ogretran( i ) );
#endif
		if ( spTemplate[ tpTplOgreTran->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplOgreTran->mTempID ] );
			spTemplate[ tpTplOgreTran->mTempID ] = NULL;
		}
		spTemplate[ tpTplOgreTran->mTempID ] = tpTplOgreTran;
	}
	for ( i = 0; i < (unsigned int) tpEquipSet->openslot_size(); i ++ )
	{
		//PBOpenSlot* tpOpenSlot = tpEquipSet->mutable_openslot( i );
		CTemplateOpenSlot* tpTplSlot = ( CTemplateOpenSlot* )TPL_NEW(  CTemplateOpenSlot  );
		if ( tpTplSlot == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplSlot->SetFromPB( true, tpEquipSet->mutable_openslot( i ) );
#else
		tpTplSlot->SetFromPB( false, tpEquipSet->mutable_openslot( i ) );
#endif
		if ( spTemplate[ tpTplSlot->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplSlot->mTempID ] );
			spTemplate[ tpTplSlot->mTempID ] = NULL;
		}
		spTemplate[ tpTplSlot->mTempID ] = tpTplSlot;
		spOpenSlot = tpTplSlot;
	}
	for ( i = 0; i < (unsigned int) tpTemp->petlevelexp_size(); i ++ )
	{
		//PBPetLevelExp* tpPetExp = tpTemp->mutable_petlevelexp( i );
		CTemplatePetLevelExp* tpTplPetExp = ( CTemplatePetLevelExp* )TPL_NEW(  CTemplatePetLevelExp );
		if ( tpTplPetExp == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplPetExp->SetFromPB( true, tpTemp->mutable_petlevelexp( i ) );
#else
		tpTplPetExp->SetFromPB( false, tpTemp->mutable_petlevelexp( i ) );
#endif
		if ( spTemplate[ tpTplPetExp->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplPetExp->mTempID ] );
			spTemplate[ tpTplPetExp->mTempID ] = NULL;
		}
		spTemplate[ tpTplPetExp->mTempID ] = tpTplPetExp;
	}
	for ( i = 0; i < (unsigned int) tpEquipSet->produceequip_size(); i ++ )
	{
		//PBProduceEquip* tpProEquip = tpEquipSet->mutable_produceequip( i );
		CTemplateProEquip* tpTplProEquip = ( CTemplateProEquip* )TPL_NEW(  CTemplateProEquip  );
		if ( tpTplProEquip == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplProEquip->SetFromPB( true, tpEquipSet->mutable_produceequip( i ) );
#else
		tpTplProEquip->SetFromPB( false, tpEquipSet->mutable_produceequip( i ) );
#endif
		if ( spTemplate[ tpTplProEquip->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplProEquip->mTempID ] );
			spTemplate[ tpTplProEquip->mTempID ] = NULL;
		}
		spTemplate[ tpTplProEquip->mTempID ] = tpTplProEquip;
		CDataStatic::sProEquip[tpTplProEquip->mType][tpTplProEquip->mDetailID1]
		.mTable[tpTplProEquip->mMaterialRank - 1][tpTplProEquip->mLevel][tpTplProEquip->mDetailID2 > 0 ? tpTplProEquip->mDetailID2 - 1 : tpTplProEquip->mDetailID2]
		= tpTplProEquip->mTempID;
	}
	for ( i = 0; i < (unsigned int) tpEquipSet->produceexp_size(); i ++ )
	{
		//PBProduceExp* tpProExp = tpEquipSet->mutable_produceexp( i );
		CTemplateProExp* tpTplProExp = ( CTemplateProExp* )TPL_NEW(  CTemplateProExp  );
		if ( tpTplProExp == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplProExp->SetFromPB( true, tpEquipSet->mutable_produceexp( i ) );
#else
		tpTplProExp->SetFromPB( false, tpEquipSet->mutable_produceexp( i ) );
#endif
		if ( spTemplate[ tpTplProExp->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplProExp->mTempID ] );
			spTemplate[ tpTplProExp->mTempID ] = NULL;
		}
		spTemplate[ tpTplProExp->mTempID ] = tpTplProExp;
		spProExp = tpTplProExp;
	}
	for ( i = 0; i < (unsigned int) tpEquipSet->producephysic_size(); i ++ )
	{
		//PBProducePhysic* tpProPhysic = tpEquipSet->mutable_producephysic( i );
		CTemplateProPhysics* tpTplPhysic = ( CTemplateProPhysics* )TPL_NEW(  CTemplateProPhysics  );
		if ( tpTplPhysic == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplPhysic->SetFromPB( true, tpEquipSet->mutable_producephysic( i ) );
#else
		tpTplPhysic->SetFromPB( false, tpEquipSet->mutable_producephysic( i ) );
#endif
		if ( spTemplate[ tpTplPhysic->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplPhysic->mTempID ] );
			spTemplate[ tpTplPhysic->mTempID ] = NULL;
		}
		spTemplate[ tpTplPhysic->mTempID ] = tpTplPhysic;
		sProPhys[ tpTplPhysic->mType ][ tpTplPhysic->mLevel ] = tpTplPhysic->mTempID;
	}
	for ( i = 0; i < (unsigned int) tpTemp->questionexp_size(); i ++ )
	{
		//PBQuestionExp* tpQuesExp = tpTemp->mutable_questionexp( i );
		CTemplateQuestionExpTable* tpTplQuesExp = ( CTemplateQuestionExpTable* )TPL_NEW(  CTemplateQuestionExpTable  );
		if ( tpTplQuesExp == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplQuesExp->SetFromPB( true, tpTemp->mutable_questionexp( i ) );
#else
		tpTplQuesExp->SetFromPB( false, tpTemp->mutable_questionexp( i ) );
#endif
		if ( spTemplate[ tpTplQuesExp->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplQuesExp->mTempID ] );
			spTemplate[ tpTplQuesExp->mTempID ] = NULL;
		}
		spTemplate[ tpTplQuesExp->mTempID ] = tpTplQuesExp;
		spQuestionExpTable = tpTplQuesExp;
	}
	for ( i = 0; i < (unsigned int) tpConfigTable->redstonetable_size(); i ++ )
	{
		//PBRedStoneExpTable* tpRedStoneTable = tpConfigTable->mutable_redstonetable( i );
		CTemplateRedStoneExpTable* tpTplRedStoneTable = ( CTemplateRedStoneExpTable* )TPL_NEW(  CTemplateRedStoneExpTable  );
		if ( tpTplRedStoneTable == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplRedStoneTable->SetFromPB( true, tpConfigTable->mutable_redstonetable( i ) );
#else
		tpTplRedStoneTable->SetFromPB( false, tpConfigTable->mutable_redstonetable( i ) );
#endif
		if ( spTemplate[ tpTplRedStoneTable->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplRedStoneTable->mTempID ] );
			spTemplate[ tpTplRedStoneTable->mTempID ] = NULL;
		}
		spTemplate[ tpTplRedStoneTable->mTempID ] = tpTplRedStoneTable;
		spRedStoneExpTable = tpTplRedStoneTable;
	}
	for ( i = 0; i < (unsigned int) tpItemSet->redstone_size(); i ++ )
	{
		CTplRedStone* tpTplRedStone = ( CTplRedStone* )TPL_NEW(  CTplRedStone  );
		if ( tpTplRedStone == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplRedStone->SetFromPB( true, tpItemSet->mutable_redstone( i ) );
#else
		tpTplRedStone->SetFromPB( false, tpItemSet->mutable_redstone( i ) );
#endif
		if ( spTemplate[ tpTplRedStone->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplRedStone->mTempID ] );
			spTemplate[ tpTplRedStone->mTempID ] = NULL;
		}
		spTemplate[ tpTplRedStone->mTempID ] = tpTplRedStone;
		if( tpTplRedStone->mRedStoneType == CTplRedStone::TYPE_FAMILY )
		{
			mFamilyRedstoneID = tpTplRedStone->mTempID;
		}
	}
	for ( i = 0; i < (unsigned int) tpConfigTable->relivetable_size(); i ++ )
	{
		//PBReliveTable* tpReliveTable = tpConfigTable->mutable_relivetable( i );
		CTemplateReliveTable* tpTplReliveTable = ( CTemplateReliveTable* )TPL_NEW(  CTemplateReliveTable  );
		if ( tpTplReliveTable == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplReliveTable->SetFromPB( true, tpConfigTable->mutable_relivetable( i ) );
#else
		tpTplReliveTable->SetFromPB( false, tpConfigTable->mutable_relivetable( i ) );
#endif
		if ( spTemplate[ tpTplReliveTable->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplReliveTable->mTempID ] );
			spTemplate[ tpTplReliveTable->mTempID ] = NULL;
		}
		spTemplate[ tpTplReliveTable->mTempID ] = tpTplReliveTable;
		spReliveTable = tpTplReliveTable;
	}
	for ( i = 0; i < (unsigned int) tpConfigTable->selltable_size(); i ++ )
	{
		//PBSellTable* tpSellTable = tpConfigTable->mutable_selltable( i );
		CTemplateSellTable* tpTplSellTable = ( CTemplateSellTable* )TPL_NEW(  CTemplateSellTable  );
		if ( tpTplSellTable == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplSellTable->SetFromPB( true, tpConfigTable->mutable_selltable( i ) );
#else
		tpTplSellTable->SetFromPB( false, tpConfigTable->mutable_selltable( i ) );
#endif
		if ( spTemplate[ tpTplSellTable->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplSellTable->mTempID ] );
			spTemplate[ tpTplSellTable->mTempID ] = NULL;
		}
		spTemplate[ tpTplSellTable->mTempID ] = tpTplSellTable;
	}
	for ( i = 0; i < (unsigned int) tpTemp->showpet_size(); i ++ )
	{
		//PBShowPet* tpPet = tpTemp->mutable_showpet( i );
		CTemplateShowPet* tpTplShowPet = ( CTemplateShowPet* )TPL_NEW(  CTemplateShowPet  );
		if ( tpTplShowPet == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplShowPet->SetFromPB( true, tpTemp->mutable_showpet( i ) );
#else
		tpTplShowPet->SetFromPB( false, tpTemp->mutable_showpet( i ) );
#endif
		if ( spTemplate[ tpTplShowPet->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplShowPet->mTempID ] );
			spTemplate[ tpTplShowPet->mTempID ] = NULL;
		}
		spTemplate[ tpTplShowPet->mTempID ] = tpTplShowPet;
	}
	for ( i = 0; i < (unsigned int) tpSkillSet->skill_size(); i ++ )
	{
		//PBSkill* tpSkill = tpSkillSet->mutable_skill( i );
		CTemplateSkill* tpTplSkill = ( CTemplateSkill* )TPL_NEW(  CTemplateSkill  );
		if ( tpTplSkill == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplSkill->SetFromPB( true, tpSkillSet->mutable_skill( i ) );
#else
		tpTplSkill->SetFromPB( false, tpSkillSet->mutable_skill( i ) );
#endif
		if ( spTemplate[ tpTplSkill->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplSkill->mTempID ] );
			spTemplate[ tpTplSkill->mTempID ] = NULL;
		}
		spTemplate[ tpTplSkill->mTempID ] = tpTplSkill;
		spSkill[ tpTplSkill->mSkillID ][ tpTplSkill->mSkillLevel ] = tpTplSkill;
	}
	for ( i = 0; i < (unsigned int) tpSkillSet->skilltree_size(); i ++ )
	{
		//PBSkillTree* tpSkillTree = tpSkillSet->mutable_skilltree( i );
		CTemplateSkillTree* tpTplSkillTree = ( CTemplateSkillTree* )TPL_NEW(  CTemplateSkillTree  );
		if ( tpTplSkillTree == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplSkillTree->SetFromPB( true, tpSkillSet->mutable_skilltree( i ) );
#else
		tpTplSkillTree->SetFromPB( false, tpSkillSet->mutable_skilltree( i ) );
#endif
		if ( spTemplate[ tpTplSkillTree->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplSkillTree->mTempID ] );
			spTemplate[ tpTplSkillTree->mTempID ] = NULL;
		}
		spTemplate[ tpTplSkillTree->mTempID ] = tpTplSkillTree;
		CDataStatic::sGenerID[ tpTplSkillTree->mGenerID ] = tpTplSkillTree->mTempID;
	}
	for ( i = 0; i < (unsigned int) tpSkillSet->skilltreenode_size(); i ++ )
	{
		//PBSkillTreeNode* tpSkillNode = tpSkillSet->mutable_skilltreenode( i );
		CSkillTreeNode* tpTplSkillNode = ( CSkillTreeNode* )TPL_NEW(  CSkillTreeNode  );
		if ( tpTplSkillNode == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplSkillNode->SetFromPB( true, tpSkillSet->mutable_skilltreenode( i ) );
#else
		tpTplSkillNode->SetFromPB( false, tpSkillSet->mutable_skilltreenode( i ) );
#endif
	}
	for ( i = 0; i < (unsigned int) tpEquipSet->spell_size(); i ++ )
	{
		PBSpell* tpSpell = tpEquipSet->mutable_spell( i );
		CTemplateSpell* tpTplSpell = ( CTemplateSpell* )TPL_NEW(  CTemplateSpell  );
		if ( tpTplSpell == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplSpell->SetFromPB( true, tpSpell );
#else
		tpTplSpell->SetFromPB( false, tpSpell );
#endif
		if ( spTemplate[ tpTplSpell->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplSpell->mTempID ] );
			spTemplate[ tpTplSpell->mTempID ] = NULL;
		}
		spTemplate[ tpTplSpell->mTempID ] = tpTplSpell;
		sSuitID[ tpTplSpell->mSuitID ] = tpTplSpell->mTempID;
	}
	for ( i = 0; i < (unsigned int) tpEquipSet->suitnormal_size(); i ++ )
	{
		PBSuitNormal* tpSuitNormal = tpEquipSet->mutable_suitnormal( i );
		CTemplateSuitNormal* tpTplSuitNormal = ( CTemplateSuitNormal* )TPL_NEW(  CTemplateSuitNormal  );
		if ( tpTplSuitNormal == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplSuitNormal->SetFromPB( true, tpSuitNormal );
#else
		tpTplSuitNormal->SetFromPB( false, tpSuitNormal );
#endif
		if ( spTemplate[ tpTplSuitNormal->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplSuitNormal->mTempID ] );
			spTemplate[ tpTplSuitNormal->mTempID ] = NULL;
		}
		spTemplate[ tpTplSuitNormal->mTempID ] = tpTplSuitNormal;
	}
	for ( i = 0; i < (unsigned int) tpEquipSet->suitproducer_size(); i ++ )
	{
		PBSuitProducer* tpSuitPro = tpEquipSet->mutable_suitproducer( i );
		CTemplateSuitProducer* tpTplSuitPro = ( CTemplateSuitProducer* )TPL_NEW(  CTemplateSuitProducer  );
		if ( tpTplSuitPro == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplSuitPro->SetFromPB( true, tpSuitPro );
#else
		tpTplSuitPro->SetFromPB( false, tpSuitPro );
#endif
		if ( spTemplate[ tpTplSuitPro->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplSuitPro->mTempID ] );
			spTemplate[ tpTplSuitPro->mTempID ] = NULL;
		}
		spTemplate[ tpTplSuitPro->mTempID ] = tpTplSuitPro;
		spProSuit = tpTplSuitPro;
	}
	for ( i = 0; i < (unsigned int) tpTemp->summonbeast_size(); i ++ )
	{
		PBSummonBeast* tpBeast = tpTemp->mutable_summonbeast( i );
		CTemplateSummonBeast* tpTplBeast = ( CTemplateSummonBeast* )TPL_NEW(  CTemplateSummonBeast );
		if ( tpTplBeast == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplBeast->SetFromPB( true, tpBeast );
#else
		tpTplBeast->SetFromPB( false, tpBeast );
#endif
		if ( spTemplate[ tpTplBeast->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplBeast->mTempID ] );
			spTemplate[ tpTplBeast->mTempID ] = NULL;
		}
		spTemplate[ tpTplBeast->mTempID ] = tpTplBeast;
	}
	for ( i = 0; i < (unsigned int) tpConfigTable->teletable_size(); i ++ )
	{
		PBTeleTable* tpTable = tpConfigTable->mutable_teletable( i );
		CTemplateTeleTable* tpTplTable = ( CTemplateTeleTable* )TPL_NEW(  CTemplateTeleTable  );
		if ( tpTplTable == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplTable->SetFromPB( true, tpTable );
#else
		tpTplTable->SetFromPB( false, tpTable );
#endif
		if ( spTemplate[ tpTplTable->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplTable->mTempID ] );
			spTemplate[ tpTplTable->mTempID ] = NULL;
		}
		spTemplate[ tpTplTable->mTempID ] = tpTplTable;
	}
	for ( i = 0; i < (unsigned int) tpTemp->tplproperty_size(); i ++ )
	{
		PBTemplateProperty* tpPro = tpTemp->mutable_tplproperty( i );
		CTemplateProperty* tpTplPro = ( CTemplateProperty* )TPL_NEW(  CTemplateProperty  );
		if ( tpTplPro == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplPro->SetFromPB( true, tpPro );
#else
		tpTplPro->SetFromPB( false, tpPro );
#endif
		if ( spTemplate[ tpTplPro->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplPro->mTempID ] );
			spTemplate[ tpTplPro->mTempID ] = NULL;
		}
		spTemplate[ tpTplPro->mTempID ] = tpTplPro;
	}
	for ( i = 0; i < (unsigned int) tpTemp->tplprotable_size(); i ++ )
	{
		PBTemplatePropertyTable* tpProTable = tpTemp->mutable_tplprotable( i );
		CTemplatePropertyTable* tpTplProTable = ( CTemplatePropertyTable* )TPL_NEW(  CTemplatePropertyTable  );
		if ( tpTplProTable == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplProTable->SetFromPB( true, tpProTable );
#else
		tpTplProTable->SetFromPB( false, tpProTable );
#endif
		if ( spTemplate[ tpTplProTable->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplProTable->mTempID ] );
			spTemplate[ tpTplProTable->mTempID ] = NULL;
		}
		spTemplate[ tpTplProTable->mTempID ] = tpTplProTable;
	}
	for ( i = 0; i < (unsigned int) tpTemp->totaldrop_size(); i ++ )
	{
		PBTotalDrop* tpDrop = tpTemp->mutable_totaldrop( i );
		CTemplateTotalDrop* tpTplDrop = ( CTemplateTotalDrop* )TPL_NEW(  CTemplateTotalDrop  );
		if ( tpTplDrop == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplDrop->SetFromPB( true, tpDrop );
#else
		tpTplDrop->SetFromPB( false, tpDrop );
#endif
		if ( spTemplate[ tpTplDrop->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplDrop->mTempID ] );
			spTemplate[ tpTplDrop->mTempID ] = NULL;
		}
		spTemplate[ tpTplDrop->mTempID ] = tpTplDrop;
	}
	for ( i = 0; i < (unsigned int) tpSkillSet->trapskill_size(); i ++ )
	{
		PBTrapSkill* tpTrap = tpSkillSet->mutable_trapskill( i );
		CTemplateTrapSkill* tpTplTrap = ( CTemplateTrapSkill* )TPL_NEW(  CTemplateTrapSkill  );
		if ( tpTplTrap == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplTrap->SetFromPB( true, tpTrap );
#else
		tpTplTrap->SetFromPB( false, tpTrap );
#endif
		if ( spTemplate[ tpTplTrap->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplTrap->mTempID ] );
			spTemplate[ tpTplTrap->mTempID ] = NULL;
		}
		spTemplate[ tpTplTrap->mTempID ] = tpTplTrap;
		spSkill[ tpTplTrap->mSkillID ][ tpTplTrap->mSkillLevel ] = tpTplTrap;
	}
	for ( i = 0; i < (unsigned int)tpConfigTable->bindchargetabel_size(); i ++ )
	{
		PBBindChargeTable* tpTable = tpConfigTable->mutable_bindchargetabel( i );
		CTplBindChargeTable* tpTplTable = ( CTplBindChargeTable* )TPL_NEW(  CTplBindChargeTable  );
		if ( tpTplTable == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplTable->SetFromPB( true, tpTable );
#else
		tpTplTable->SetFromPB( false, tpTable );
#endif
		if ( spTemplate[ tpTplTable->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplTable->mTempID ] );
			spTemplate[ tpTplTable->mTempID ] = NULL;
		}
		spTemplate[ tpTplTable->mTempID ] = tpTplTable;
		spBindChargeTable = tpTplTable;
	}
	for ( i = 0; i < (unsigned int)tpItemSet->minemap_size(); i ++ )
	{
		PBItemMineMap* tpMineMap = tpItemSet->mutable_minemap( i );
		CTplMineMap* tpTplMap = ( CTplMineMap* )TPL_NEW(  CTplMineMap  );
		if ( tpTplMap == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplMap->SetFromPB( true, tpMineMap );
#else
		tpTplMap->SetFromPB( false, tpMineMap );
#endif
		if ( spTemplate[ tpTplMap->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplMap->mTempID ] );
			spTemplate[ tpTplMap->mTempID ] = NULL;
		}
		spTemplate[ tpTplMap->mTempID ] = tpTplMap;
	}
	for ( i = 0; i < (unsigned int)tpItemSet->itemupg_size(); i ++ )
	{
		PBItemUpg* tpItemUpg = tpItemSet->mutable_itemupg( i );
		CTplItemEquipUpg* tpTplUpg = ( CTplItemEquipUpg* )TPL_NEW(  CTplItemEquipUpg  );
		if ( tpTplUpg == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplUpg->SetFromPB( true, tpItemUpg );
#else
		tpTplUpg->SetFromPB( false, tpItemUpg );
#endif
		if ( spTemplate[ tpTplUpg->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplUpg->mTempID ] );
			spTemplate[ tpTplUpg->mTempID ] = NULL;
		}
		spTemplate[ tpTplUpg->mTempID ] = tpTplUpg;
	}
	for ( i = 0; i < (unsigned int)tpConfigTable->pktable_size(); i ++ )
	{
		PBPKValueTable* tpTable = tpConfigTable->mutable_pktable( i );
		CTplPKValueTable* tpTplTable = ( CTplPKValueTable* )TPL_NEW(  CTplPKValueTable  );
		if ( tpTplTable == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplTable->SetFromPB( true, tpTable );
#else
		tpTplTable->SetFromPB( false, tpTable );
#endif
		if ( spTemplate[ tpTplTable->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplTable->mTempID ] );
			spTemplate[ tpTplTable->mTempID ] = NULL;
		}
		spTemplate[ tpTplTable->mTempID ] = tpTplTable;
		spPKValueTable = tpTplTable;
	}
	for ( i = 0; i < (unsigned int)tpItemSet->token_size(); i ++ )
	{
		PBItemToken* tpToken = tpItemSet->mutable_token( i );
		CTplToken* tpTplToken = ( CTplToken* )TPL_NEW(  CTplToken  );
		if ( tpTplToken == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplToken->SetFromPB( true, tpToken );
#else
		tpTplToken->SetFromPB( false, tpToken );
#endif
		if ( spTemplate[ tpTplToken->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplToken->mTempID ] );
			spTemplate[ tpTplToken->mTempID ] = NULL;
		}
		spTemplate[ tpTplToken->mTempID ] = tpTplToken;
	}
	for ( i = 0; i < (unsigned int)tpTemp->block_size(); i ++ )
	{
		PBBlock* tpBlock = tpTemp->mutable_block( i );
		CTemplateBlock* tpTplBlock = ( CTemplateBlock* )TPL_NEW(  CTemplateBlock  );
		if ( tpTplBlock == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplBlock->SetFromPB( true, tpBlock );
#else
		tpTplBlock->SetFromPB( false, tpBlock );
#endif
		if ( spTemplate[ tpTplBlock->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplBlock->mTempID ] );
			spTemplate[ tpTplBlock->mTempID ] = NULL;
		}
		spTemplate[ tpTplBlock->mTempID ] = tpTplBlock;
	}
	for ( i = 0; i < (unsigned int)tpItemSet->kitbag_size(); ++i )
	{
		CTplKitBag *tpItemKitBag = ( CTplKitBag * )TPL_NEW(  CTplKitBag  ); 
		if( tpItemKitBag == NULL )
		{
			break;	
		}		  
#ifdef  LINUX
		tpItemKitBag->SetFromPB( true, tpItemSet->mutable_kitbag(i) );		
#else
		tpItemKitBag->SetFromPB( false, tpItemSet->mutable_kitbag(i) );	
#endif
		if ( spTemplate[tpItemKitBag->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpItemKitBag->mTempID] );  
			spTemplate[tpItemKitBag->mTempID] = NULL;
		}
		spTemplate[tpItemKitBag->mTempID] = tpItemKitBag;
	}
	for ( i = 0; i < (unsigned int)tpItemSet->itemremove_size(); ++i )
	{
		CTplItemRemove *tpItemRemove = ( CTplItemRemove * )TPL_NEW(  CTplItemRemove  ); 
		if( tpItemRemove == NULL )
		{
			break;	
		}		  
#ifdef  LINUX
		tpItemRemove->SetFromPB( true, tpItemSet->mutable_itemremove(i) );		
#else
		tpItemRemove->SetFromPB( false, tpItemSet->mutable_itemremove(i) );	
#endif
		if ( spTemplate[tpItemRemove->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpItemRemove->mTempID] );  
			spTemplate[tpItemRemove->mTempID] = NULL;
		}
		spTemplate[tpItemRemove->mTempID] = tpItemRemove;
	}
	for ( i = 0; i < (unsigned int)tpItemSet->itemchange_size(); ++i )
	{
		CTplItemChange *tpItemChange = ( CTplItemChange * )TPL_NEW(  CTplItemChange  ); 
		if( tpItemChange == NULL )
		{
			break;	
		}		  
#ifdef  LINUX
		tpItemChange->SetFromPB( true, tpItemSet->mutable_itemchange(i) );		
#else
		tpItemChange->SetFromPB( false, tpItemSet->mutable_itemchange(i) );	
#endif
		if ( spTemplate[tpItemChange->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpItemChange->mTempID] );  
			spTemplate[tpItemChange->mTempID] = NULL;
		}
		spTemplate[tpItemChange->mTempID] = tpItemChange;
	}
	for ( i = 0; i < (unsigned int)tpItemSet->itemadapter_size(); ++i )
	{
		CTplItemAdapter *tpItemAdapter = ( CTplItemAdapter * )TPL_NEW(  CTplItemAdapter  ); 
		if( tpItemAdapter == NULL )
		{
			break;	
		}		  
#ifdef  LINUX
		tpItemAdapter->SetFromPB( true, tpItemSet->mutable_itemadapter(i) );		
#else
		tpItemAdapter->SetFromPB( false, tpItemSet->mutable_itemadapter(i) );	
#endif
		if ( spTemplate[tpItemAdapter->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpItemAdapter->mTempID] );  
			spTemplate[tpItemAdapter->mTempID] = NULL;
		}
		spTemplate[tpItemAdapter->mTempID] = tpItemAdapter;
	}

	for ( i = 0; i < (unsigned int) tpItemSet->giftbag_size(); i ++ )
	{
		//PBItemReturnPaper* tpReturnPaper = tpItemSet->mutable_returnpaper( i );
		CTplGiftBag* tpItem = ( CTplGiftBag* )TPL_NEW(  CTplGiftBag  );
		if ( tpItem == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItem->SetFromPB( true, tpItemSet->mutable_giftbag( i ) );
#else
		tpItem->SetFromPB( false, tpItemSet->mutable_giftbag( i ) );
#endif
		if ( spTemplate[ tpItem->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItem->mTempID ] );
			spTemplate[ tpItem->mTempID ] = NULL;
		}
		spTemplate[ tpItem->mTempID ] = tpItem;
	}


	for ( i = 0; i < (unsigned int)tpTemp->hierogram_size(); ++i )
	{
		CTplHierogram *tpHierogram = ( CTplHierogram * )TPL_NEW(  CTplHierogram  ); 
		if( tpHierogram == NULL )
		{
			break;	
		}		  
#ifdef  LINUX
		tpHierogram->SetFromPB( true, tpTemp->mutable_hierogram(i) );		
#else
		tpHierogram->SetFromPB( false, tpTemp->mutable_hierogram(i) );	
#endif
		if ( spTemplate[tpHierogram->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpHierogram->mTempID] );  
			spTemplate[tpHierogram->mTempID] = NULL;
		}
		spTemplate[tpHierogram->mTempID] = tpHierogram;
	}

	for ( i = 0; i < (unsigned int)tpTemp->titlemanager_size(); ++i )
	{
		CTplTitleManager *tpTitle = ( CTplTitleManager * )TPL_NEW(  CTplTitleManager  ); 
		if( tpTitle == NULL )
		{
			break;	
		}		  
#ifdef  LINUX
		tpTitle->SetFromPB( true, tpTemp->mutable_titlemanager( i ) );		
#else
		tpTitle->SetFromPB( false, tpTemp->mutable_titlemanager( i ) );	
#endif
		if ( spTemplate[tpTitle->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpTitle->mTempID] );  
			spTemplate[tpTitle->mTempID] = NULL;
		}
		spTemplate[tpTitle->mTempID] = tpTitle;
		spTitleManager = tpTitle;
	}

	for ( i = 0; i < (unsigned int)tpTemp->talent_size(); ++i )
	{
		CTemplateTalent *tpTalent = ( CTemplateTalent * )TPL_NEW(  CTemplateTalent  ); 
		if( tpTalent == NULL )
		{
			break;	
		}		  
#ifdef  LINUX
		tpTalent->SetFromPB( true, tpTemp->mutable_talent(i) );		
#else
		tpTalent->SetFromPB( false, tpTemp->mutable_talent(i) );	
#endif
		if ( spTemplate[tpTalent->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpTalent->mTempID] );  
			spTemplate[tpTalent->mTempID] = NULL;
		}
		spTemplate[tpTalent->mTempID] = tpTalent;
		spTalent = tpTalent;
	}

	for ( i = 0; i < (unsigned int)tpItemSet->magicweapons_size(); ++i )
	{
		CTemplateMagicWeapon *tpTalent = ( CTemplateMagicWeapon * )TPL_NEW(  CTemplateMagicWeapon ); 
		if( tpTalent == NULL )
		{
			break;	
		}		  
#ifdef  LINUX
		tpTalent->SetFromPB( true, tpItemSet->mutable_magicweapons(i) );		
#else
		tpTalent->SetFromPB( false, tpItemSet->mutable_magicweapons(i) );	
#endif
		if ( spTemplate[tpTalent->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpTalent->mTempID] );  
			spTemplate[tpTalent->mTempID] = NULL;
		}
		spTemplate[tpTalent->mTempID] = tpTalent;
	}

	for ( i = 0; i < (unsigned int) tpItemSet->funcitem_size(); i ++ )
	{
		CTplFuncItem* tpItem = ( CTplFuncItem* )TPL_NEW(  CTplFuncItem  );
		if ( tpItem == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItem->SetFromPB( true, tpItemSet->mutable_funcitem( i ) );
#else
		tpItem->SetFromPB( false, tpItemSet->mutable_funcitem( i ) );
#endif
		if ( spTemplate[ tpItem->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItem->mTempID ] );
			spTemplate[ tpItem->mTempID ] = NULL;
		}
		if ( tpItem->mItemType == FUNCCODE_FAMILYEXPCARD )
		{
			mFamilyExpID = tpItem->mTempID;
		}
		spTemplate[ tpItem->mTempID ] = tpItem;
	}

	for ( i = 0; i < (unsigned int)tpConfigTable->magicweaponprotable_size(); ++i )
	{
		CTplMagicWeaponProIncTable *tpTalent = ( CTplMagicWeaponProIncTable * )TPL_NEW(  CTplMagicWeaponProIncTable ); 
		if( tpTalent == NULL )
		{
			break;	
		}		  
#ifdef  LINUX
		tpTalent->SetFromPB( true, tpConfigTable->mutable_magicweaponprotable(i) );		
#else
		tpTalent->SetFromPB( false, tpConfigTable->mutable_magicweaponprotable(i) );	
#endif
		if ( spTemplate[tpTalent->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpTalent->mTempID] );  
			spTemplate[tpTalent->mTempID] = NULL;
		}
		spTemplate[tpTalent->mTempID] = tpTalent;
	}


	for ( i = 0; i < (unsigned int)tpTemp->magicweaponskillstorage_size(); ++i )
	{
		CTplSkillStorage *tpTalent = ( CTplSkillStorage * )TPL_NEW(  CTplSkillStorage  ); 
		if( tpTalent == NULL )
		{
			break;	
		}		  
#ifdef  LINUX
		tpTalent->SetFromPB( true, tpTemp->mutable_magicweaponskillstorage(i) );		
#else
		tpTalent->SetFromPB( false, tpTemp->mutable_magicweaponskillstorage(i) );	
#endif
		if ( spTemplate[tpTalent->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpTalent->mTempID] );  
			spTemplate[tpTalent->mTempID] = NULL;
		}
		spTemplate[tpTalent->mTempID] = tpTalent;
	}

	for ( i = 0; i < (unsigned int)tpConfigTable->magicweaponskillbox_size(); ++i )
	{
		CTplMagicWeaponSkillBoxProTable *tpTalent = ( CTplMagicWeaponSkillBoxProTable * )TPL_NEW(  CTplMagicWeaponSkillBoxProTable  ); 
		if( tpTalent == NULL )
		{
			break;	
		}		  
#ifdef  LINUX
		tpTalent->SetFromPB( true, tpConfigTable->mutable_magicweaponskillbox(i) );		
#else
		tpTalent->SetFromPB( false, tpConfigTable->mutable_magicweaponskillbox(i) );	
#endif
		if ( spTemplate[tpTalent->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpTalent->mTempID] );  
			spTemplate[tpTalent->mTempID] = NULL;
		}
		spTemplate[tpTalent->mTempID] = tpTalent;
	}



	for ( i = 0; i < (unsigned int)tpEquipSet->mwjudgeeff_size(); ++i )
	{
		CTplMWProJudgeEffTable *tpTalent = ( CTplMWProJudgeEffTable * )TPL_NEW(  CTplMWProJudgeEffTable  ); 
		if( tpTalent == NULL )
		{
			break;	
		}		  
#ifdef  LINUX
		tpTalent->SetFromPB( true, tpEquipSet->mutable_mwjudgeeff(i) );		
#else
		tpTalent->SetFromPB( false, tpEquipSet->mutable_mwjudgeeff(i) );	
#endif
		if ( spTemplate[tpTalent->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpTalent->mTempID] );  
			spTemplate[tpTalent->mTempID] = NULL;
		}
		spTemplate[tpTalent->mTempID] = tpTalent;
	}


	for ( i = 0; i < (unsigned int)tpEquipSet->mwjudgespir_size(); ++i )
	{
		CTplMWProJudgeSpirTable *tpTalent = ( CTplMWProJudgeSpirTable * )TPL_NEW(  CTplMWProJudgeSpirTable ); 
		if( tpTalent == NULL )
		{
			break;	
		}		  
#ifdef  LINUX
		tpTalent->SetFromPB( true, tpEquipSet->mutable_mwjudgespir(i) );		
#else
		tpTalent->SetFromPB( false, tpEquipSet->mutable_mwjudgespir(i) );	
#endif
		if ( spTemplate[tpTalent->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpTalent->mTempID] );  
			spTemplate[tpTalent->mTempID] = NULL;
		}
		spTemplate[tpTalent->mTempID] = tpTalent;
	}

	for ( i = 0; i < (unsigned int)tpEquipSet->mwquotiety_size(); ++i )
	{
		CTplMWLevelQuotietyTab *tpTalent = ( CTplMWLevelQuotietyTab * )TPL_NEW(  CTplMWLevelQuotietyTab ); 
		if( tpTalent == NULL )
		{
			break;	
		}		  
#ifdef  LINUX
		tpTalent->SetFromPB( true, tpEquipSet->mutable_mwquotiety(i) );		
#else
		tpTalent->SetFromPB( false, tpEquipSet->mutable_mwquotiety(i) );	
#endif
		if ( spTemplate[tpTalent->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpTalent->mTempID] );  
			spTemplate[tpTalent->mTempID] = NULL;
		}
		spTemplate[tpTalent->mTempID] = tpTalent;
	}


	for ( i = 0; i < (unsigned int)tpTemp->buffimmunity_size(); ++i )
	{
		CTplBuffImmunity *tpTalent = ( CTplBuffImmunity * )TPL_NEW(  CTplBuffImmunity  ); 
		if( tpTalent == NULL )
		{
			break;	
		}		  
#ifdef  LINUX
		tpTalent->SetFromPB( true, tpTemp->mutable_buffimmunity(i) );		
#else
		tpTalent->SetFromPB( false, tpTemp->mutable_buffimmunity(i) );	
#endif
		if ( spTemplate[tpTalent->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpTalent->mTempID] );  
			spTemplate[tpTalent->mTempID] = NULL;
		}
		spTemplate[tpTalent->mTempID] = tpTalent;
	}
	
	for ( i = 0; i < ( unsigned int )tpTemp->lifeskill_size(); ++i )
	{
		CTemplateLifeSkill *tpLifeSkill = ( CTemplateLifeSkill * )TPL_NEW(  CTemplateLifeSkill ); 
		if ( tpLifeSkill == NULL )
		{
			break;
		}
		
#ifdef  LINUX
		tpLifeSkill->SetFromPB( true, tpTemp->mutable_lifeskill(i) );		
#else
		tpLifeSkill->SetFromPB( false, tpTemp->mutable_lifeskill(i) );		
#endif
		if ( spTemplate[tpLifeSkill->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpLifeSkill->mTempID] );
			spTemplate[tpLifeSkill->mTempID] = NULL;
		}		
		spTemplate[tpLifeSkill->mTempID] = tpLifeSkill;	
		switch( tpLifeSkill->mLifeSkillType )					
		{
			case LIFESKILL_COLLECT:
			{
				CDataStatic::mCollect.spLifeSkillTable = tpLifeSkill;
				break;
			}
			case LIFESKILL_GOLDMAKE:
			{
				CDataStatic::mGoldMake.spLifeSkillTable = tpLifeSkill;
				break;	
			}
			case LIFESKILL_ARTIFICER:
			{	
				CDataStatic::mArtificer.spLifeSkillTable = tpLifeSkill;
				break;
			}	
			case LIFESKILL_PILLMAKER:
			{
				CDataStatic::mPillMaker.spLifeSkillTable = tpLifeSkill;
				break;
			}	
			case LIFESKILL_KOOKER:
			{
				CDataStatic::mCooker.spLifeSkillTable = tpLifeSkill;
				break;
			}
			default:
				break;
		}
	}
					   
	for ( i = 0; i < ( unsigned int )tpEquipSet->decompound_size(); ++i )
	{
		CTemplateDecompound *tpDecompound = ( CTemplateDecompound * )TPL_NEW(  CTemplateDecompound  ); 
		if ( tpDecompound == NULL )
		{
			break;
		}				
#ifdef  LINUX
		tpDecompound->SetFromPB( true, tpEquipSet->mutable_decompound( i ) );
#else
		tpDecompound->SetFromPB( false, tpEquipSet->mutable_decompound( i ) );
#endif
		if ( spTemplate[tpDecompound->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpDecompound->mTempID] );
			spTemplate[tpDecompound->mTempID] = NULL;
		}
		spTemplate[tpDecompound->mTempID] = tpDecompound;
		CDataStatic::spDecompund = tpDecompound;
	}	
	
	for ( i = 0; i < ( unsigned int )tpItemSet->magicstone_size(); ++i )
	{
		CTplItemMagicStone *tpMagicStone = ( CTplItemMagicStone * )TPL_NEW(  CTplItemMagicStone  );
		if ( tpMagicStone == NULL  )
		{
			break;
		}
#ifdef  LINUX
		tpMagicStone->SetFromPB( true, tpItemSet->mutable_magicstone( i ) );
#else
		tpMagicStone->SetFromPB( false, tpItemSet->mutable_magicstone( i ) );
#endif
		if ( spTemplate[tpMagicStone->mTempID] != NULL )
		{	  
			TPL_DELETE( spTemplate[tpMagicStone->mTempID] );
		}
		spTemplate[tpMagicStone->mTempID] = tpMagicStone;
	}
	
	for ( i = 0; i < ( unsigned int )tpTemp->lifeskillscroll_size(); ++i )
	{
		CTplItemLifeSkillScroll *tpLifeSkillScroll = ( CTplItemLifeSkillScroll * )TPL_NEW( CTplItemLifeSkillScroll );
		if ( tpLifeSkillScroll == NULL )
		{
			break;
		}  
#ifdef  LINUX
		tpLifeSkillScroll->SetFromPB( true, tpTemp->mutable_lifeskillscroll( i ) );
#else
		tpLifeSkillScroll->SetFromPB( false, tpTemp->mutable_lifeskillscroll( i ) );
#endif				
		
		if ( spTemplate[tpLifeSkillScroll->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpLifeSkillScroll->mTempID] );	
		}
		
		spTemplate[tpLifeSkillScroll->mTempID] = tpLifeSkillScroll;
	}
	
	for( i = 0; i < ( unsigned int )tpConfigTable->equipexchange_size(); ++i )
	{
		CTempateEquipExchangeTable *tpExchange = ( CTempateEquipExchangeTable * )TPL_NEW( CTempateEquipExchangeTable );
		if ( tpExchange == NULL )
		{
			break;
		}
#ifdef	LINUX
		tpExchange->SetFromPB( true, tpConfigTable->mutable_equipexchange( i ) );
#else
		tpExchange->SetFromPB( false, tpConfigTable->mutable_equipexchange( i ) );
#endif	
		if ( spTemplate[tpExchange->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpExchange->mTempID] );
		}
		spTemplate[tpExchange->mTempID] = tpExchange;		
	}

	for ( i = 0; i < (unsigned int)tpSkillSet->multibuff_size(); ++i )
	{
		CTemplateMultiBuffConfig* tpMultiBuff = ( CTemplateMultiBuffConfig* )TPL_NEW( CTemplateMultiBuffConfig );
		if ( tpMultiBuff == NULL )
		{
			break;
		}
#ifdef	LINUX
		tpMultiBuff->SetFromPB( true, tpSkillSet->mutable_multibuff( i ) );
#else
		tpMultiBuff->SetFromPB( false, tpSkillSet->mutable_multibuff( i ) );
#endif	
		if ( spTemplate[tpMultiBuff->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpMultiBuff->mTempID] );
		}
		spTemplate[tpMultiBuff->mTempID] = tpMultiBuff;	
		spMultiBuff = tpMultiBuff;
	}

	for ( i = 0; i < (unsigned int)tpTemp->cdgrouptable_size(); ++i )
	{
		CTemplateCDGroupTable* tpCDGroup = ( CTemplateCDGroupTable* )TPL_NEW( CTemplateCDGroupTable );
		if ( tpCDGroup == NULL )
		{
			break;
		}
#ifdef	LINUX
		tpCDGroup->SetFromPB( true, tpTemp->mutable_cdgrouptable( i ) );
#else
		tpCDGroup->SetFromPB( false, tpTemp->mutable_cdgrouptable( i ) );
#endif	
		if ( spTemplate[tpCDGroup->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpCDGroup->mTempID] );
		}
		spTemplate[tpCDGroup->mTempID] = tpCDGroup;	
		spCDGroup = tpCDGroup;
	}

	for ( i = 0; i < (unsigned int)tpEquipSet->lighttable_size(); ++i )
	{
		CTemplateLightTable* tpLight = ( CTemplateLightTable* )TPL_NEW( CTemplateLightTable );
		if ( tpLight == NULL )
		{
			break;
		}
#ifdef	LINUX
		tpLight->SetFromPB( true, tpEquipSet->mutable_lighttable( i ) );
#else
		tpLight->SetFromPB( false, tpEquipSet->mutable_lighttable( i ) );
#endif	
		if ( spTemplate[tpLight->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpLight->mTempID] );
		}
		spTemplate[tpLight->mTempID] = tpLight;	
		spLight = tpLight;
	}

	for ( i = 0; i < (unsigned int)tpTemp->apconfig_size(); ++i )
	{
		CTplApConfig* tpConfig = ( CTplApConfig* )TPL_NEW( CTplApConfig );
		if ( tpConfig == NULL )
		{
			break;
		}
#ifdef	LINUX
		tpConfig->SetFromPB( true, tpTemp->mutable_apconfig( i ) );
#else
		tpConfig->SetFromPB( false, tpTemp->mutable_apconfig( i ) );
#endif	
		if ( spTemplate[tpConfig->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpConfig->mTempID] );
		}
		spTemplate[tpConfig->mTempID] = tpConfig;	
		spApConfig = tpConfig;
	}

	for ( i = 0; i < (unsigned int)tpTemp->wizardtable_size(); ++i )
	{
		CTemplateWizardTable* tpWizard = ( CTemplateWizardTable* )TPL_NEW( CTemplateWizardTable );
		if ( tpWizard == NULL )
		{
			break;
		}
#ifdef	LINUX
		tpWizard->SetFromPB( true, tpTemp->mutable_wizardtable( i ) );
#else
		tpWizard->SetFromPB( false, tpTemp->mutable_wizardtable( i ) );
#endif	
		if ( spTemplate[tpWizard->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpWizard->mTempID] );
		}
		spTemplate[tpWizard->mTempID] = tpWizard;	
		spWizard = tpWizard;
	}
	
	for ( i = 0; i < (unsigned int)tpTemp->judgeconfig_size(); ++i )
	{
		CTplJudgeConfig* tpConfig = ( CTplJudgeConfig* )TPL_NEW( CTplJudgeConfig );
		if ( tpConfig == NULL )
		{
			break;
		}
#ifdef	LINUX
		tpConfig->SetFromPB( true, tpTemp->mutable_judgeconfig( i ) );
#else
		tpConfig->SetFromPB( false, tpTemp->mutable_judgeconfig( i ) );
#endif	
		if ( spTemplate[tpConfig->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpConfig->mTempID] );
		}
		spTemplate[tpConfig->mTempID] = tpConfig;	
		spJudgeConfig = tpConfig;
	}

	for ( i = 0; i < (unsigned int)tpItemSet->redpaper_size(); ++i )
	{
		CTplItemRedPaper *tpRedPaper = ( CTplItemRedPaper * )TPL_NEW(  CTplItemRedPaper  );
		if ( tpRedPaper == NULL )
		{
			break;
		}
#ifdef LINUX
		tpRedPaper->SetFromPB( true, tpItemSet->mutable_redpaper( i ) );
#else
		tpRedPaper->SetFromPB( false, tpItemSet->mutable_redpaper( i ) );
#endif
		if ( spTemplate[tpRedPaper->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpRedPaper->mTempID] );
		}
		spTemplate[tpRedPaper->mTempID]	= tpRedPaper;		
	}

	for ( i = 0; i < ( unsigned int )tpTemp->trend_size(); ++i )
	{
		CTemplateTrend *tpTrend = ( CTemplateTrend * )TPL_NEW( CTemplateTrend );
		if ( tpTrend == NULL )
		{
			break;
		}  
#ifdef  LINUX
		tpTrend->SetFromPB( true, tpTemp->mutable_trend( i ) );
#else
		tpTrend->SetFromPB( false, tpTemp->mutable_trend( i ) );
#endif				

		if ( spTemplate[tpTrend->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpTrend->mTempID] );	
		}

		spTemplate[tpTrend->mTempID] = tpTrend;
	}

	for ( i = 0; i < ( unsigned int )tpTemp->achieveaward_size(); ++i )
	{
		CTplAchieveAward *tpAward = ( CTplAchieveAward * )TPL_NEW( CTplAchieveAward );
		if ( tpAward == NULL )
		{
			break;
		}  
#ifdef  LINUX
		tpAward->SetFromPB( true, tpTemp->mutable_achieveaward( i ) );
#else
		tpAward->SetFromPB( false, tpTemp->mutable_achieveaward( i ) );
#endif				

		if ( spTemplate[tpAward->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpAward->mTempID] );	
		}

		spTemplate[tpAward->mTempID] = tpAward;
		spAchieve = tpAward;
	}

	for ( i = 0; i < ( unsigned int )tpTemp->power_size(); ++i )
	{
		CTemplatePower *tpPower = ( CTemplatePower * )TPL_NEW( CTemplatePower );
		if ( tpPower == NULL )
		{
			break;
		}  
#ifdef  LINUX
		tpPower->SetFromPB( true, tpTemp->mutable_power( i ) );
#else
		tpPower->SetFromPB( false, tpTemp->mutable_power( i ) );
#endif				

		if ( spTemplate[tpPower->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpPower->mTempID] );	
		}

		spTemplate[tpPower->mTempID] = tpPower;
		if ( tpPower->mOfficial >= 0 && tpPower->mOfficial < OFFICAL_NUM && tpPower->mOrder >= 0 && tpPower->mOrder < MAX_POWER )
		{
			spOffical[ tpPower->mOfficial ][ tpPower->mOrder ] = tpPower->mTempID;
		}
		if ( tpPower->mOfficial == OFFICIAL_FUNCTION_KING && tpPower->mOrder < MAX_POWER )
		{
			spKing[ tpPower->mOrder ] = tpPower->mTempID;
		}
	}

	for ( i = 0; i < ( unsigned int )tpTemp->official_size(); ++i )
	{
		CTemplateOfficial *tpOfficial = ( CTemplateOfficial * )TPL_NEW( CTemplateOfficial );
		if ( tpOfficial == NULL )
		{
			break;
		}  
#ifdef  LINUX
		tpOfficial->SetFromPB( true, tpTemp->mutable_official( i ) );
#else
		tpOfficial->SetFromPB( false, tpTemp->mutable_official( i ) );
#endif				

		if ( spTemplate[tpOfficial->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpOfficial->mTempID] );	
		}

		spTemplate[tpOfficial->mTempID] = tpOfficial;
	}

	for ( i = 0; i < (unsigned int)tpConfigTable->officialtable_size(); ++i )
	{
		CTemplateOfficialTable* tpConfig = ( CTemplateOfficialTable* )TPL_NEW( CTemplateOfficialTable );
		if ( tpConfig == NULL )
		{
			break;
		}
#ifdef	LINUX
		tpConfig->SetFromPB( true, tpConfigTable->mutable_officialtable( i ) );
#else
		tpConfig->SetFromPB( false, tpConfigTable->mutable_officialtable( i ) );
#endif	
		if ( spTemplate[tpConfig->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpConfig->mTempID] );
		}
		spTemplate[tpConfig->mTempID] = tpConfig;	
		spOfficalTable = tpConfig;
	}

	for ( i = 0; i < (unsigned int)tpTemp->constructexp_size(); ++i )
	{
		CTemplateConstructExp* tpConfig = ( CTemplateConstructExp* )TPL_NEW( CTemplateConstructExp );
		if ( tpConfig == NULL )
		{
			break;
		}
#ifdef	LINUX
		tpConfig->SetFromPB( true, tpTemp->mutable_constructexp( i ) );
#else
		tpConfig->SetFromPB( false, tpTemp->mutable_constructexp( i ) );
#endif	
		if ( spTemplate[tpConfig->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpConfig->mTempID] );
		}
		spTemplate[tpConfig->mTempID] = tpConfig;	
		spConstructExp = tpConfig;
	}

	for ( i = 0; i < (unsigned int)tpConfigTable->addpktable_size(); ++i )
	{
		CTemplateAddPKTable* tpConfig = ( CTemplateAddPKTable* )TPL_NEW( CTemplateAddPKTable );
		if ( tpConfig == NULL )
		{
			break;
		}
#ifdef	LINUX
		tpConfig->SetFromPB( true, tpConfigTable->mutable_addpktable( i ) );
#else
		tpConfig->SetFromPB( false, tpConfigTable->mutable_addpktable( i ) );
#endif	
		if ( spTemplate[tpConfig->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpConfig->mTempID] );
		}
		spTemplate[tpConfig->mTempID] = tpConfig;	
		spAddPKTable = tpConfig;
	}

	for ( i = 0; i < (unsigned int)tpConfigTable->maptable_size(); ++i )
	{
		CTemplateMapTable* tpConfig = ( CTemplateMapTable* )TPL_NEW( CTemplateMapTable );
		if ( tpConfig == NULL )
		{
			break;
		}
#ifdef	LINUX
		tpConfig->SetFromPB( true, tpConfigTable->mutable_maptable( i ) );
#else
		tpConfig->SetFromPB( false, tpConfigTable->mutable_maptable( i ) );
#endif	
		if ( spTemplate[tpConfig->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpConfig->mTempID] );
		}
		spTemplate[tpConfig->mTempID] = tpConfig;	
		spMapTable = tpConfig;
	}

	for ( i = 0; i < (unsigned int)tpConfigTable->multiattacktable_size(); ++i )
	{
		CTemplateMultiAttackTable* tpConfig = ( CTemplateMultiAttackTable* )TPL_NEW( CTemplateMultiAttackTable);
		if ( tpConfig == NULL )
		{
			break;
		}
#ifdef	LINUX
		tpConfig->SetFromPB( true, tpConfigTable->mutable_multiattacktable( i ) );
#else
		tpConfig->SetFromPB( false, tpConfigTable->mutable_multiattacktable( i ) );
#endif	
		if ( spTemplate[tpConfig->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpConfig->mTempID] );
		}
		spTemplate[tpConfig->mTempID] = tpConfig;	
		spMultiAttackTable = tpConfig;
	}

	for ( i = 0; i < (unsigned int)tpItemSet->vipitem_size(); ++i )
	{
		CTplItemVip* tpItem = ( CTplItemVip* )TPL_NEW( CTplItemVip );
		if ( tpItem == NULL )
		{
			break;
		}
#ifdef	LINUX
		tpItem->SetFromPB( true, tpItemSet->mutable_vipitem( i ) );
#else
		tpItem->SetFromPB( false, tpItemSet->mutable_vipitem( i ) );
#endif	
		if ( spTemplate[tpItem->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpItem->mTempID] );
		}
		spTemplate[tpItem->mTempID] = tpItem;	
	}

	for ( i = 0; i < (unsigned int)tpItemSet->cleanitem_size(); ++i )
	{
		CTplItemClean* tpItem = ( CTplItemClean* )TPL_NEW( CTplItemClean );
		if ( tpItem == NULL )
		{
			break;
		}
#ifdef	LINUX
		tpItem->SetFromPB( true, tpItemSet->mutable_cleanitem( i ) );
#else
		tpItem->SetFromPB( false, tpItemSet->mutable_cleanitem( i ) );
#endif	
		if ( spTemplate[tpItem->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpItem->mTempID] );
		}
		spTemplate[tpItem->mTempID] = tpItem;	
	}
	
	for ( i = 0; i < (unsigned int)tpItemSet->leaguebag_size(); ++i )
	{
		CTplLeagueBag *tpLeagueBag = ( CTplLeagueBag * )TPL_NEW(  CTplLeagueBag );
		if ( tpLeagueBag == NULL )
		{
			break;
		}
		
#ifdef LINUX
		tpLeagueBag->SetFromPB( true, tpItemSet->mutable_leaguebag( i ) );
#else
		tpLeagueBag->SetFromPB( false, tpItemSet->mutable_leaguebag( i ) );
#endif
		if ( spTemplate[tpLeagueBag->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpLeagueBag->mTempID] );
		}
		spTemplate[tpLeagueBag->mTempID] = tpLeagueBag;
	}

	for ( i = 0; i < (unsigned int)tpItemSet->awardinfo_size(); ++i )
	{
		CTemplateAwardInfo* tpInfo = ( CTemplateAwardInfo* )TPL_NEW( CTemplateAwardInfo );
		if ( tpInfo == NULL )
		{
			break;
		}
#ifdef	LINUX
		tpInfo->SetFromPB( true, tpItemSet->mutable_awardinfo( i ) );
#else
		tpInfo->SetFromPB( false, tpItemSet->mutable_awardinfo( i ) );
#endif	
		if ( spTemplate[tpInfo->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpInfo->mTempID] );
		}
		spTemplate[tpInfo->mTempID] = tpInfo;	
	}

	for ( i = 0; i < (unsigned int)tpConfigTable->matrixtable_size(); ++i )
	{
		CTemplateMatrixTable* tpTable = ( CTemplateMatrixTable* )TPL_NEW( CTemplateMatrixTable );
		if ( tpTable == NULL )
		{
			break;
		}
#ifdef	LINUX
		tpTable->SetFromPB( true, tpConfigTable->mutable_matrixtable( i ) );
#else
		tpTable->SetFromPB( false, tpConfigTable->mutable_matrixtable( i ) );
#endif	
		if ( spTemplate[tpTable->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpTable->mTempID] );
		}
		spTemplate[tpTable->mTempID] = tpTable;	
		spMatrixTable = tpTable;
	}

	for ( i = 0; i < (unsigned int)tpConfigTable->marriageconfig_size(); ++i )
	{
		CTemplateMarriageConfig *tpMarriageConfig = ( CTemplateMarriageConfig * )TPL_NEW(  CTemplateMarriageConfig  );
		if ( tpMarriageConfig == NULL )
		{
			break;
		}
#ifdef LINUX
		tpMarriageConfig->SetFromPB( true, tpConfigTable->mutable_marriageconfig( i ) );
#else
		tpMarriageConfig->SetFromPB( false, tpConfigTable->mutable_marriageconfig( i ) );
#endif
		if ( spTemplate[tpMarriageConfig->mTempID] != NULL )
		{	
			TPL_DELETE( spTemplate[tpMarriageConfig->mTempID] );
		}
		spTemplate[tpMarriageConfig->mTempID] = tpMarriageConfig;
		mMarriageConfig = tpMarriageConfig;		
	}	  
	
	for ( i = 0; i < ( unsigned int )tpItemSet->invitation_size(); ++i )
	{
		CTplInvitation *tpInvitation = ( CTplInvitation * )TPL_NEW(  CTplInvitation );
		if ( tpInvitation == NULL )
		{
			break;
		}
#ifdef LINUX 
		tpInvitation->SetFromPB( true, tpItemSet->mutable_invitation( i ) );
#else
		tpInvitation->SetFromPB( false, tpItemSet->mutable_invitation( i ) );
#endif
		if ( spTemplate[tpInvitation->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpInvitation->mTempID] );
		}
		spTemplate[tpInvitation->mTempID] = tpInvitation;
	}
	
	for ( i = 0; i < (unsigned int)tpItemSet->sysinvitation_size(); ++i )
	{
		CTplSysInvitation *tpSysInvitation = ( CTplSysInvitation * )TPL_NEW(  CTplSysInvitation  );
		if ( tpSysInvitation == NULL )
		{
			break;
		}
#ifdef LINUX
		tpSysInvitation->SetFromPB( true, tpItemSet->mutable_sysinvitation( i ) );
#else
		tpSysInvitation->SetFromPB( false, tpItemSet->mutable_sysinvitation( i ) );
#endif
		if ( spTemplate[tpSysInvitation->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpSysInvitation->mTempID] );
		}
		spTemplate[tpSysInvitation->mTempID] = tpSysInvitation;
	}
		
		
	for ( i = 0; i < ( unsigned int )tpConfigTable->progressconfig_size(); ++i )
	{
		CTemplateProgressConfig *tpProgressConfig = ( CTemplateProgressConfig * )TPL_NEW(  CTemplateProgressConfig );
		if ( tpProgressConfig == NULL )
		{
			break;
		}
#ifdef LINUX
		tpProgressConfig->SetFromPB( true, tpConfigTable->mutable_progressconfig( i ) );
#else
		tpProgressConfig->SetFromPB( false, tpConfigTable->mutable_progressconfig( i ) );
#endif	
		if ( spTemplate[tpProgressConfig->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpProgressConfig->mTempID] );
		}			  
		spTemplate[tpProgressConfig->mTempID] = tpProgressConfig;
		spProgressConfig = tpProgressConfig;
	}

	for ( i = 0; i < ( unsigned int )tpConfigTable->counttable_size(); ++i )
	{
		CTemplateCountTable *tpCountTable = ( CTemplateCountTable * )TPL_NEW(  CTemplateCountTable  );
		if ( tpCountTable == NULL )
		{
			break;
		}
#ifdef LINUX
		tpCountTable->SetFromPB( true, tpConfigTable->mutable_counttable( i ) );
#else
		tpCountTable->SetFromPB( false, tpConfigTable->mutable_counttable( i ) );
#endif	
		if ( spTemplate[tpCountTable->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpCountTable->mTempID] );
		}			  
		spTemplate[tpCountTable->mTempID] = tpCountTable;
		spCountTable = tpCountTable;
	}
	
	for ( i = 0; i < ( unsigned int )tpConfigTable->activityconfig_size(); ++i )
	{
		CTemplateActivityConfig *tpActivityConfig = ( CTemplateActivityConfig * )TPL_NEW(  CTemplateActivityConfig  ); 
		if ( tpActivityConfig == NULL )
		{
			break;
		}
#if LINUX
		tpActivityConfig->SetFromPB( true, tpConfigTable->mutable_activityconfig( i ) );
#else
		tpActivityConfig->SetFromPB( false, tpConfigTable->mutable_activityconfig( i ) );
#endif
		if ( spTemplate[tpActivityConfig->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpActivityConfig->mTempID] );
		}
		spTemplate[tpActivityConfig->mTempID]  = tpActivityConfig;		
		mActivityConfig = tpActivityConfig;
	}

	for ( i = 0; i < ( unsigned int )tpItemSet->itemrefresh_size(); ++i )
	{
		CTplItemRefresh *tpRefresh = ( CTplItemRefresh * )TPL_NEW( CTplItemRefresh );
		if ( tpRefresh == NULL )
		{
			break;
		}
#ifdef LINUX 
		tpRefresh->SetFromPB( true, tpItemSet->mutable_itemrefresh( i ) );
#else
		tpRefresh->SetFromPB( false, tpItemSet->mutable_itemrefresh( i ) );
#endif
		if ( spTemplate[tpRefresh->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpRefresh->mTempID] );
		}
		spTemplate[tpRefresh->mTempID] = tpRefresh;
	}

	for ( i = 0; i < ( unsigned int )tpItemSet->itemsummon_size(); ++i )
	{
		CTplItemSummon *tpSummon = ( CTplItemSummon * )TPL_NEW( CTplItemSummon );
		if ( tpSummon == NULL )
		{
			break;
		}
#ifdef LINUX 
		tpSummon->SetFromPB( true, tpItemSet->mutable_itemsummon( i ) );
#else
		tpSummon->SetFromPB( false, tpItemSet->mutable_itemsummon( i ) );
#endif
		if ( spTemplate[tpSummon->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpSummon->mTempID] );
		}
		spTemplate[tpSummon->mTempID] = tpSummon;
	}

	for ( i = 0; i < ( unsigned int )tpItemSet->itemdirect_size(); ++i )
	{
		CTplItemDirect *tpDirect = ( CTplItemDirect * )TPL_NEW( CTplItemDirect );
		if ( tpDirect == NULL )
		{
			break;
		}
#ifdef LINUX 
		tpDirect->SetFromPB( true, tpItemSet->mutable_itemdirect( i ) );
#else
		tpDirect->SetFromPB( false, tpItemSet->mutable_itemdirect( i ) );
#endif
		if ( spTemplate[tpDirect->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpDirect->mTempID] );
		}
		spTemplate[tpDirect->mTempID] = tpDirect;
	}

	for ( i = 0; i < ( unsigned int )tpConfigTable->directtable_size(); ++i )
	{
		CTemplateDirectTable *tpDirectTable = ( CTemplateDirectTable * )TPL_NEW(  CTemplateDirectTable  ); 
		if ( tpDirectTable == NULL )
		{
			break;
		}
#if LINUX
		tpDirectTable->SetFromPB( true, tpConfigTable->mutable_directtable( i ) );
#else
		tpDirectTable->SetFromPB( false, tpConfigTable->mutable_directtable( i ) );
#endif
		if ( spTemplate[tpDirectTable->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpDirectTable->mTempID] );
		}
		spTemplate[tpDirectTable->mTempID]  = tpDirectTable;		
		spDirectTable = tpDirectTable;
	}

	for ( i = 0; i < ( unsigned int )tpItemSet->itemgetstar_size(); ++i )
	{
		CTplItemGetStar *tpStar = ( CTplItemGetStar * )TPL_NEW( CTplItemGetStar );
		if ( tpStar == NULL )
		{
			break;
		}
#ifdef LINUX 
		tpStar->SetFromPB( true, tpItemSet->mutable_itemgetstar( i ) );
#else
		tpStar->SetFromPB( false, tpItemSet->mutable_itemgetstar( i ) );
#endif
		if ( spTemplate[tpStar->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpStar->mTempID] );
		}
		spTemplate[tpStar->mTempID] = tpStar;
	}

    /////////////////////////////////////////////////////////////////////////////////////////
    // 上缴奖励

    for ( i = 0; i < ( unsigned int )tpConfigTable->handinencourage_size(); ++i )
    {
        if ( i>=MAX_HANDINENCOURAGE )
        {
            break;
        }

        CTplHandinEncourage *tpConfig = ( CTplHandinEncourage * )TPL_NEW(  CTplHandinEncourage  ); 
        if ( tpConfig == NULL )
        {
            break;
        }
#if LINUX
        tpConfig->SetFromPB( true, tpConfigTable->mutable_handinencourage( i ) );
#else
        tpConfig->SetFromPB( false, tpConfigTable->mutable_handinencourage( i ) );
#endif
        if ( spTemplate[tpConfig->mTempID] != NULL )
        {
            TPL_DELETE( spTemplate[tpConfig->mTempID] );
        }
        spTemplate[tpConfig->mTempID]  = tpConfig;		
        spHandinEncourage[i] = tpConfig;
    }

    /////////////////////////////////////////////////////////////////////////////////////////
    // 上缴奖励对应表

    for ( i = 0; i < ( unsigned int )tpConfigTable->handintable_size(); ++i )
    {
        if ( i>=MAX_HANDINTABLE )
        {
            break;
        }

        CTplHandinTable *tpConfig = ( CTplHandinTable * )TPL_NEW(  CTplHandinTable  ); 
        if ( tpConfig == NULL )
        {
            break;
        }
#if LINUX
        tpConfig->SetFromPB( true, tpConfigTable->mutable_handintable( i ) );
#else
        tpConfig->SetFromPB( false, tpConfigTable->mutable_handintable( i ) );
#endif
        if ( spTemplate[tpConfig->mTempID] != NULL )
        {
            TPL_DELETE( spTemplate[tpConfig->mTempID] );
        }
        spTemplate[tpConfig->mTempID]  = tpConfig;		
        spHandinTable[i] = tpConfig;
    }

    /////////////////////////////////////////////////////////////////////////////////////////

	for ( i = 0; i < ( unsigned int )tpConfigTable->battleconfig_size(); ++i )
	{
		CTemplateWorldBattleConfig *tpConfig = ( CTemplateWorldBattleConfig * )TPL_NEW(  CTemplateWorldBattleConfig  ); 
		if ( tpConfig == NULL )
		{
			break;
		}
#if LINUX
		tpConfig->SetFromPB( true, tpConfigTable->mutable_battleconfig( i ) );
#else
		tpConfig->SetFromPB( false, tpConfigTable->mutable_battleconfig( i ) );
#endif
		if ( spTemplate[tpConfig->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpConfig->mTempID] );
		}
		spTemplate[tpConfig->mTempID]  = tpConfig;		
		spBattleConfig = tpConfig;
	}

	for ( i = 0; i < ( unsigned int )tpConfigTable->moneylmt_size(); ++i )
	{
		CTemplateLevelMoneyLmt *tpConfig = ( CTemplateLevelMoneyLmt * )TPL_NEW(  CTemplateLevelMoneyLmt  ); 
		if ( tpConfig == NULL )
		{
			break;
		}
#if LINUX
		tpConfig->SetFromPB( true, tpConfigTable->mutable_moneylmt( i ) );
#else
		tpConfig->SetFromPB( false, tpConfigTable->mutable_moneylmt( i ) );
#endif
		if ( spTemplate[tpConfig->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpConfig->mTempID] );
		}
		spTemplate[tpConfig->mTempID]  = tpConfig;		
		spMoneyLmtConfig = tpConfig;
	}
	
	for ( i = 0; i < ( unsigned int )tpItemSet->itemautocollect_size(); ++i )
	{
		CTplItemAutoCollect *tpAutoCollect = ( CTplItemAutoCollect * )TPL_NEW( CTplItemAutoCollect );
		if ( tpAutoCollect == NULL )
		{
			break;
		}
#if LINUX
		tpAutoCollect->SetFromPB( true, tpItemSet->mutable_itemautocollect( i ) );
#else
		tpAutoCollect->SetFromPB( false, tpItemSet->mutable_itemautocollect( i ) );
#endif
	}

	for ( i = 0; i < (unsigned int) tpSkillSet->fangcuntree_size(); i ++ )
	{
		//PBSkillTree* tpSkillTree = tpSkillSet->mutable_skilltree( i );
		CTemplateFangCunTree* tpTplSkillTree = ( CTemplateFangCunTree* )TPL_NEW(  CTemplateFangCunTree  );
		if ( tpTplSkillTree == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplSkillTree->SetFromPB( true, tpSkillSet->mutable_fangcuntree( i ) );
#else
		tpTplSkillTree->SetFromPB( false, tpSkillSet->mutable_fangcuntree( i ) );
#endif
		if ( spTemplate[ tpTplSkillTree->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpTplSkillTree->mTempID ] );
			spTemplate[ tpTplSkillTree->mTempID ] = NULL;
		}
		spTemplate[ tpTplSkillTree->mTempID ] = tpTplSkillTree;
		spFangCunTree = tpTplSkillTree;
	}
	for ( i = 0; i < (unsigned int) tpSkillSet->fangcunnode_size(); i ++ )
	{
		//PBSkillTreeNode* tpSkillNode = tpSkillSet->mutable_skilltreenode( i );
		CFangCunTreeNode* tpTplSkillNode = ( CFangCunTreeNode* )TPL_NEW(  CFangCunTreeNode  );
		if ( tpTplSkillNode == NULL )
		{
			break;
		}
#ifdef LINUX
		tpTplSkillNode->SetFromPB( true, tpSkillSet->mutable_fangcunnode( i ) );
#else
		tpTplSkillNode->SetFromPB( false, tpSkillSet->mutable_fangcunnode( i ) );
#endif
	}
	
	for ( i = 0; i < (unsigned int)tpItemSet->itemblackpills_size(); ++i )
	{
		CTplItemBlackPills *tpBlackPills = ( CTplItemBlackPills * )TPL_NEW( CTplItemBlackPills );
		if ( tpBlackPills == NULL )
		{
			break;
		}
#ifdef LINUX
		tpBlackPills->SetFromPB( true, tpItemSet->mutable_itemblackpills( i ) );
#else
		tpBlackPills->SetFromPB( false, tpItemSet->mutable_itemblackpills( i ) );
#endif
		if( spTemplate[tpBlackPills->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpBlackPills->mTempID] );
		}
		spTemplate[tpBlackPills->mTempID] = tpBlackPills;
	}

	for ( i = 0; i < (unsigned int)tpTemp->talentstage_size(); ++i )
	{
		CTemplateTalentStage *tpTalent = ( CTemplateTalentStage * )TPL_NEW(  CTemplateTalentStage  ); 
		if( tpTalent == NULL )
		{
			break;	
		}		  
#ifdef  LINUX
		tpTalent->SetFromPB( true, tpTemp->mutable_talentstage(i) );		
#else
		tpTalent->SetFromPB( false, tpTemp->mutable_talentstage(i) );	
#endif
		if ( spTemplate[tpTalent->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpTalent->mTempID] );  
			spTemplate[tpTalent->mTempID] = NULL;
		}
		spTemplate[tpTalent->mTempID] = tpTalent;
		spTalentStage = tpTalent;
	}
	
	for ( i = 0; i < (unsigned int)tpTemp->corpsboss_size(); ++i )
	{
		CTemplateCorpsBOSSTable *tpCorpsBOSS = ( CTemplateCorpsBOSSTable * )TPL_NEW( CTemplateCorpsBOSSTable );
		if ( tpCorpsBOSS == NULL )
		{
			break;
		}
#ifdef LINUX
		tpCorpsBOSS->SetFromPB(true, tpTemp->mutable_corpsboss(i) );
#else
		tpCorpsBOSS->SetFromPB(false, tpTemp->mutable_corpsboss(i) );
#endif
		if ( spTemplate[ tpCorpsBOSS->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpCorpsBOSS->mTempID ]);
			spTemplate[ tpCorpsBOSS->mTempID] = NULL;
		}
		spTemplate[tpCorpsBOSS->mTempID] = tpCorpsBOSS;
		spCorpsBOSSTable = tpCorpsBOSS;

	}

	for (i = 0; i < (unsigned int)tpTemp->mutable_itemset()->expcontainer_size(); ++i )
	{
		CTplItemExpContainer *tpExpContainer = ( CTplItemExpContainer * )TPL_NEW( CTplItemExpContainer );
		if ( tpExpContainer == NULL )
		{
			break;
		}
#ifdef LINUX 
		tpExpContainer->SetFromPB( true, tpTemp->mutable_itemset()->mutable_expcontainer( i ) );
#else
		tpExpContainer->SetFromPB( true, tpTemp->mutable_itemset()->mutable_expcontainer( i ) );
#endif		
		if ( spTemplate[tpExpContainer->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpExpContainer->mTempID] );
			spTemplate[tpExpContainer->mTempID] = NULL;
		}
		spTemplate[tpExpContainer->mTempID] = tpExpContainer;
	}
	
	for (i = 0; i < (unsigned int)tpTemp->mutable_itemset()->exppills_size(); ++i  )
	{
		CTplItemExpPills *tpExpPills = ( CTplItemExpPills * )TPL_NEW( CTplItemExpPills );
		if ( tpExpPills == NULL )
		{
			break;
		}
#ifdef LINUX
		tpExpPills->SetFromPB( true, tpTemp->mutable_itemset()->mutable_exppills( i ) );
#else
		tpExpPills->SetFromPB( false, tpTemp->mutable_itemset()->mutable_exppills( i ) );
#endif				
		if ( spTemplate[tpExpPills->mTempID]  != NULL )
		{
			TPL_DELETE( spTemplate[tpExpPills->mTempID] );
		}
		
		spTemplate[tpExpPills->mTempID] = tpExpPills;
	}

	//修炼心德
	for (i=0; i < (unsigned int)tpItemSet->exptome_size(); ++i)
	{
		CTplItemExpTome *tpExpTome = ( CTplItemExpTome *)TPL_NEW(CTplItemExpTome);
		if (tpExpTome == NULL)
		{
			break ;
		}
#ifdef	 LINUX
		tpExpTome->SetFromPB(true, tpItemSet->mutable_exptome(i));
#else
		tpExpTome->SetFromPB(false, tpItemSet->mutable_exptome(i));
#endif
		if (spTemplate[tpExpTome->mTempID] != NULL)
		{
			TPL_DELETE(spTemplate[tpExpTome->mTempID]);
		}
		spTemplate[tpExpTome->mTempID] = tpExpTome;
	}


	//转阵营 
	for ( i = 0; i < (unsigned int) tpConfigTable->changecamp_size(); i ++ )
	{
		//PBItemExp* tpExp = tpItemSet->mutable_itemexp( i );
		CTemPlateChangeCampCfg* tpItemChangeCamp = ( CTemPlateChangeCampCfg* )TPL_NEW( CTemPlateChangeCampCfg );
		if ( tpItemChangeCamp == NULL )
		{
			break;
		}
#ifdef LINUX
		tpItemChangeCamp->SetFromPB( true, tpConfigTable->mutable_changecamp( i ) );
#else
		tpItemChangeCamp->SetFromPB( false, tpConfigTable->mutable_changecamp( i ) );
#endif
		if ( spTemplate[ tpItemChangeCamp->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpItemChangeCamp->mTempID ] );
			spTemplate[ tpItemChangeCamp->mTempID ] = NULL;
		}
		spTemplate[ tpItemChangeCamp->mTempID ] = tpItemChangeCamp;
		spChangeCamp =  tpItemChangeCamp;
	}

	for ( i = 0; i < ( unsigned int )tpConfigTable->accuseconfig_size(); ++i )
	{
		CTemplateAccuseConfig *tpConfig = ( CTemplateAccuseConfig * )TPL_NEW(  CTemplateAccuseConfig  ); 
		if ( tpConfig == NULL )
		{
			break;
		}
#if LINUX
		tpConfig->SetFromPB( true, tpConfigTable->mutable_accuseconfig( i ) );
#else
		tpConfig->SetFromPB( false, tpConfigTable->mutable_accuseconfig( i ) );
#endif
		if ( spTemplate[tpConfig->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpConfig->mTempID] );
		}
		spTemplate[tpConfig->mTempID]  = tpConfig;		
		spAccuseConfig = tpConfig;
	}

	//复杂符文配置
	for ( i = 0; i < (unsigned int) tpConfigTable->comruneconfig_size(); i ++ )
	{
		//PBItemExp* tpExp = tpItemSet->mutable_itemexp( i );
		CTemplateComRuneCfg* tpRuneCfg = ( CTemplateComRuneCfg* )TPL_NEW( CTemplateComRuneCfg );
		if ( tpRuneCfg == NULL )
		{
			break;
		}
#ifdef LINUX
		tpRuneCfg->SetFromPB( true, tpConfigTable->mutable_comruneconfig( i ) );
#else
		tpRuneCfg->SetFromPB( false, tpConfigTable->mutable_comruneconfig( i ) );
#endif
		if ( spTemplate[ tpRuneCfg->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ tpRuneCfg->mTempID ] );
			spTemplate[ tpRuneCfg->mTempID ] = NULL;
		}
		spTemplate[ tpRuneCfg->mTempID ] = tpRuneCfg;
		spComRuneConfig = tpRuneCfg ;
	}

	for ( i = 0; i < ( unsigned int )tpConfigTable->globalmapconfig_size(); ++i )
	{
		CTemplateGlobalMapConfig *tpConfig = ( CTemplateGlobalMapConfig * )TPL_NEW(  CTemplateGlobalMapConfig  ); 
		if ( tpConfig == NULL )
		{
			break;
		}
#if LINUX
		tpConfig->SetFromPB( true, tpConfigTable->mutable_globalmapconfig( i ) );
#else
		tpConfig->SetFromPB( false, tpConfigTable->mutable_globalmapconfig( i ) );
#endif
		if ( spTemplate[tpConfig->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpConfig->mTempID] );
		}
		spTemplate[tpConfig->mTempID]  = tpConfig;		
		spGlobalMapConfig = tpConfig;
	}

	for ( i = 0; i < ( unsigned int )tpConfigTable->offlineexpconfig_size(); ++i )
	{
		CTemplateOffLineExpConfig *tpConfig = ( CTemplateOffLineExpConfig * )TPL_NEW(  CTemplateOffLineExpConfig  ); 
		if ( tpConfig == NULL )
		{
			break;
		}
#if LINUX
		tpConfig->SetFromPB( true, tpConfigTable->mutable_offlineexpconfig( i ) );
#else
		tpConfig->SetFromPB( false, tpConfigTable->mutable_offlineexpconfig( i ) );
#endif
		if ( spTemplate[tpConfig->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpConfig->mTempID] );
		}
		spTemplate[tpConfig->mTempID]  = tpConfig;		
		spOffLineExpConfig = tpConfig;
	}

	for ( i = 0; i < ( unsigned int )tpConfigTable->repefametable_size(); ++i )
	{
		CTemplateRepeFameTable *tpConfig = ( CTemplateRepeFameTable * )TPL_NEW(  CTemplateRepeFameTable  ); 
		if ( tpConfig == NULL )
		{
			break;
		}
#if LINUX
		tpConfig->SetFromPB( true, tpConfigTable->mutable_repefametable( i ) );
#else
		tpConfig->SetFromPB( false, tpConfigTable->mutable_repefametable( i ) );
#endif
		if ( spTemplate[tpConfig->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpConfig->mTempID] );
		}
		spTemplate[tpConfig->mTempID]  = tpConfig;		
		spRepeFameTable = tpConfig;
	}
	// 铸灵配置表
	for ( i = 0; i < ( unsigned int )tpConfigTable->castingspirittable_size(); ++i )
	{
		CTplCastingSpiritTable *tpConfig = (CTplCastingSpiritTable*)TPL_NEW(CTplCastingSpiritTable); 
		if ( tpConfig == NULL )
		{
			break;
		}
#if LINUX
		tpConfig->SetFromPB( true, tpConfigTable->mutable_castingspirittable( i ) );
#else
		tpConfig->SetFromPB( false, tpConfigTable->mutable_castingspirittable( i ) );
#endif
		if ( spTemplate[tpConfig->mTempID] != NULL )
		{
			TPL_DELETE( spTemplate[tpConfig->mTempID] );
		}
		spTemplate[tpConfig->mTempID]  = tpConfig;		
		spCastingSpiritTable = tpConfig;
	}
	// 真灵道具
	for ( i = 0; i < (unsigned int) tpItemSet->spirititem_size(); i ++ )
	{
		CTplSpiritItem* pSpiritItem = ( CTplSpiritItem* )TPL_NEW(  CTplSpiritItem  );
		if ( pSpiritItem == NULL )
		{
			break;
		}
#ifdef LINUX
		pSpiritItem->SetFromPB( true, tpItemSet->mutable_spirititem(i));
#else
		pSpiritItem->SetFromPB( false, tpItemSet->mutable_spirititem(i) );
#endif
		if ( spTemplate[ pSpiritItem->mTempID ] != NULL )
		{
			TPL_DELETE( spTemplate[ pSpiritItem->mTempID ] );
			spTemplate[ pSpiritItem->mTempID ] = NULL;
		}
		spTemplate[pSpiritItem->mTempID] = pSpiritItem;
	}
	delete ( tpTemp );
	return 0;
}

void CDataStatic::Finialize( )
{
	for (unsigned int i = 0; i < (unsigned int) MAX_TEMPLATE; i++)
	{
		CTemplate* pTemplate = CDataStatic::spTemplate[i];
		if (pTemplate != NULL)
		{
			TPL_DELETE(pTemplate);
			pTemplate = NULL;
			CDataStatic::spTemplate[i] = NULL;
		}
	}
	return ;
}

CTemplate* CDataStatic::SearchTpl( unsigned int vTempID )
{
	if ( vTempID <= 0 || vTempID >= MAX_TEMPLATE )
	{
#ifdef LINUX
		//LOG_ERROR( "default", "can't find obj TempID: %d", vTempID );
#endif
		return NULL;
	}
#ifdef LINUX
	if ( vTempID != 0 && spTemplate[ vTempID ] == NULL )
	{
		static std::set<int> tTempErroSet;
		if( tTempErroSet.find( vTempID ) == tTempErroSet.end() )
		{
			tTempErroSet.insert( vTempID );
			LOG_ERROR( "default", "can't find obj template id %d", vTempID );
			//print_trace_fd( "default" );
		}
	}
#endif
	return spTemplate[ vTempID ];
}

void* CDataStatic::SearchSkill( unsigned short vSkillID, unsigned short vLevel )
{
	if ( vSkillID <= 0 || vSkillID >= MAX_SKILL )
	{
#ifdef LINUX
		LOG_ERROR( "default", "can't find obj SkillID: %d  SkillLevel: %d", vSkillID, vLevel );
#endif
		return NULL;
	}
	if ( vLevel < 0 || vLevel >= MAX_LEVEL )
	{
#ifdef LINUX
		LOG_ERROR( "default", "can't find obj SkillID: %d  SkillLevel: %d", vSkillID, vLevel );
#endif
		return NULL;
	}
#ifdef LINUX
	if ( vSkillID != 0 && vLevel != 0 && spSkill[ vSkillID ][ vLevel ] == NULL )
	{
		LOG_ERROR( "default", "can't find obj SkillID: %d  SkillLevel: %d", vSkillID, vLevel );
	}
#endif
	return spSkill[ vSkillID ][ vLevel ];
}

void* CDataStatic::GetLevelExp( )
{
	return spLevelExp;
}

void* CDataStatic::GetData( )
{
	return spData;
}

void* CDataStatic::GetUpgrade( )
{
	return spUpgrade;
}

void* CDataStatic::GetOpenSlot( )
{
	return spOpenSlot;
}

void* CDataStatic::GetProSuit( )
{
	return spProSuit;
}

void* CDataStatic::GetProExp( )
{
	return spProExp;
}

void* CDataStatic::GetBeginner( )
{
	return spBeginner;
}

//void* CDataStatic::GetLoopTaskAwds()
//{
//	return spLoopTaskAwds;
//}

void* CDataStatic::GetDropLevel()
{
	return spDropLevel;
}

void* CDataStatic::GetHonorToExp()
{
	return spHonorToExp;
}
void* CDataStatic::GetChangeCamp()
{
	return spChangeCamp;
}
//void* CDataStatic::GetRuneConfig()
//{
//	return spRuneConfig;
//}
void*  CDataStatic::GetComRuneConfig()
{
	return spComRuneConfig;
}
void* CDataStatic::GetHonorObtain()
{
	return spHonorObtain;
}

void* CDataStatic::GetReliveTable()
{
	return spReliveTable;
}

void* CDataStatic::GetBindChargeTable()
{
	return spBindChargeTable;
}

void* CDataStatic::GetTemp( TEMPTYPE vType )
{
	switch ( vType )
	{
	case TEMP_LEVELEXP:
		return spLevelExp;
	case TEMP_DATA:
		return spData;
	case TEMP_UPGRADE:
		return spUpgrade;
	case TEMP_OPENSLOT:
		return spOpenSlot;
	case TEMP_PROSUIT:
		return spProSuit;
	case TEMP_PROEXP:
		return spProExp;
	case TEMP_BEGINNER:
		return spBeginner;
	case TEMP_DROPLEVEL:
		return spDropLevel;
	case TEMP_HONORTOEXP:
		return spHonorToExp;
	case TEMP_HONOROBTAIN:
		return spHonorObtain;
	case TEMP_RELIVETABLE:
		return spReliveTable;
	case TEMP_QUESTIONEXPTABLE:
		return spQuestionExpTable;
	case TEMP_REDSTONEEXPTABLE:
		return spRedStoneExpTable;
	case TEMP_BINDCHARGETABLE:
		return spBindChargeTable;
	case TEMP_PKVALUE:
		return spPKValueTable;
	case TEMP_TITLE:
		return spTitleManager;
	case TEMP_TALENT:
		return spTalent;
	case TEMP_MULTIBUFF:
		return spMultiBuff;
	case TEMP_CDGROUP:
		return spCDGroup;
	case TEMP_LIGHT:
		return spLight;
	case TEMP_APCONFIG:
		return spApConfig;
	case TEMP_WIZARD:
		return spWizard;
	case TEMP_JUDGECONFIG:
		return spJudgeConfig;
	case TEMP_OFFICAL:
		return spOffical;
	case TEMP_OFFICALLIST:
		return spOfficalTable;
	case TEMP_KING:
		return spKing;
	case TEMP_CONSTRUCTEXP:
		return spConstructExp;
	case TEMP_ADDPKTABLE:
		return spAddPKTable;
	case TEMP_MAPTABLE:
		return spMapTable;
	case TEMP_MULTIATTACKTABLE:
		return spMultiAttackTable;
	case TEMP_DOUBLE:
		return spDoubleTimeTable;
	case TEMP_MATRIXTABLE:
		return spMatrixTable;
	case TEMP_COUNT:
		return spCountTable;
	case TEMP_DIRECT:
		return spDirectTable;
	case TEMP_BATTLECONFIG:
		return spBattleConfig;
	case TEMP_MONEYLMT_CFG:
		return spMoneyLmtConfig;
	case TEMP_FANGCUNTREE:
		return spFangCunTree;
	case TEMP_TALENTSTAGE:
		return spTalentStage;
	case TEMP_CHANGECAMP: //转阵营
		return  spChangeCamp;
	case TEMP_ACCUSECONFIG:
		return spAccuseConfig;
	case TEMP_RUNECOMCONFIG:
		return  spComRuneConfig;//符文升级
	case TEMP_ACHIEVEAWARD:
		return spAchieve;
	case TEMP_GLOBALMAPCONFIG:
		return spGlobalMapConfig;	
	case TEMP_CORPSBOSSTABLE:
		return spCorpsBOSSTable;
	case TEMP_OFFLINEEXPCONFIG:
		return spOffLineExpConfig;
	case TEMP_REPEFAMETABLE:
		return spRepeFameTable;
	case TEMP_CASTING_SPIRIT_TABLE:
		return spCastingSpiritTable;
	default:
		return NULL;
	}
	return NULL;
}

void* CDataStatic::SearchMetier( unsigned short vMetierID )
{
	if ( vMetierID <= 0 || vMetierID >= MAX_METIER )
		return NULL;

	unsigned short tMetierTempID = sMetierID[ vMetierID ];
	return SearchTpl( tMetierTempID );
}

void* CDataStatic::SearchGener( unsigned short vGenerID )
{
	if ( vGenerID <= 0 || vGenerID >= MAX_GENER )
		return NULL;

	unsigned short tGenerTempID = sGenerID[ vGenerID ];
	return SearchTpl( tGenerTempID );
}

int CDataStatic::GetProNum( )
{
	return sProNum;
}

int CDataStatic::GetProEquip( unsigned short vType1, unsigned short vType2, unsigned short vLevel, unsigned short vRank, unsigned short vType3 )
{
	if ( vType1 > PRODUCE_TYPE || vType2 > PRODUCE_DETAIL1 || vLevel > PRODUCE_LEVEL || vRank < 1 ||vRank > PRODUCE_RANK || vType3 > PRODUCE_DETAIL2 )
		return 0;
	if ( vType3 == 0 )
	{
		if ( sProEquip[ vType1 ][ vType2 ].mTable[ vRank - 1 ][ vLevel ][ 0 ] == 0 )
			return sProEquip[ vType1 ][ vType2 ].mTable[ vRank - 1 ][ vLevel ][ 1 ];
		else
			return sProEquip[ vType1 ][ vType2 ].mTable[ vRank - 1 ][ vLevel ][ 0 ];
	}
	else
	{
		return sProEquip[ vType1 ][ vType2 ].mTable[ vRank - 1 ][ vLevel ][ vType3 - 1 ];
	}
}

int CDataStatic::GetProPhy( unsigned short vType1, unsigned short vType2 )
{
	if( vType1 > 7 || vType2 < 1 || vType2 > 4 )
		return 0;
	return sProPhys[ vType1 ][ vType2 - 1 ];
}

int CDataStatic::GetChangeRandRuneId(unsigned short level , int itemid )
{
	if (level>=0 && level<4)
	{		
		for (int i = 0;i< 10;i++)
		{
			int randindex = 0;
			int maxindex =0;
			randindex  = (int)( ( GetMaxRuneIndex(level,maxindex) +1)*(rand( )/(float)( RAND_MAX + 1.0 ) ));
			//randindex = rand() * GetMaxRuneIndex(level,maxindex)/RAND_MAX;

			if (sRuneLevelID[level][randindex] != itemid )
			{
				return sRuneLevelID[level][randindex];
			}
		}
		
	}
	return 0;
	
}
int CDataStatic::GetMaxRuneIndex(unsigned short level,int maxindex)
{
	if (level>=0 && level<4)
	{
		for (int i=0 ;i< 400;i++)
		{
			if (sRuneLevelID[level][i] > 0)
			{
				maxindex =  i;
			}
		}
	}
	return maxindex;
}


CTemplateBuff::CBuffFunc* CDataStatic::GetBuffFunc( CTemplateBuff* tpBuff, CTemplateBuff::BUFF_FUN_TYPE vType )
{
	for( unsigned i = 0; i < sizeof( tpBuff->mBuff ) / sizeof( tpBuff->mBuff[ 0 ] ); i++ )
	{
		if( tpBuff->mBuff[ i ].mBuffFunc == vType )
			return &tpBuff->mBuff[ i ];
	}
	return NULL;
}

int CDataStatic::GetSuitID( unsigned short vIndex )
{
	return sSuitID[ vIndex ];
}

void CDataStatic::Clear()
{
	memset( spTemplate, 0, sizeof(spTemplate));			// 模版表
	spLevelExp = NULL;							// 等级经验表
	spData = NULL;								// 数值修正表（等级，组队）
	spUpgrade = NULL;								// 装备升级表
	memset( spSkill, 0, sizeof( spSkill ));		// 技能表
	spOpenSlot = NULL;							// 开槽表
	spProSuit = NULL;								// 生产者套装
	spProExp = NULL;								// 生产等级的经验
	spBeginner = NULL;							// 新手设置
//	spLoopTaskAwds = NULL;						// 门派任务按等级奖励数值表
	spHonorToExp = NULL;
	spHonorObtain = NULL;
	spReliveTable = NULL;
	spBindChargeTable = NULL;
	spPKValueTable = NULL;
	spTitleManager = NULL;
	spMultiBuff = NULL;
	spCDGroup = NULL;
	spApConfig = NULL;
	spJudgeConfig = NULL;
	memset( sMetierID, 0, sizeof(sMetierID));		// 职业ID
	memset( sGenerID, 0, sizeof(sGenerID));		// 职业技能系别ID
	sTemplateVersion = 0;						// 模板文件版本号
	sTaskVersion = 0;;							// 任务文件版本号
	memset( sProEquip, 0, sizeof(sProEquip));	// 生产装备的列表
	memset( sProPhys, 0, sizeof(sProPhys));							// 生产药品的列表
	memset( sSuitID, 0, sizeof(sSuitID));
	memset( spOffical, 0, sizeof(spOffical));
	memset( spKing, 0, sizeof(spKing));
	spOfficalTable = NULL;

	sBugleItemId = 0;                            // 喇叭道具id
	sAutoChatItemId = 0;						 // 自动喊话道具id

	for ( int i = 0; i < MAX_METIER; i++ )
	{
		for ( int j = 0; j < PROPERTY_OPTION; j++ )
		{
			spMetierProperty[ i ][ j ] = NULL;
		}
	}

	for ( int i = 0; i < PROPERTY_OPTION; i++ )
	{
		spLevelProperty[ i ] = NULL;
	}
	spChangeCamp = NULL;
	memset( sRuneLevelID, 0, sizeof(sRuneLevelID));							// 符文等级列表
	spCorpsBOSSTable = NULL;
}

int CDataStatic::GetMetierProperty( int vMetier, int vLevel, int vOption, int vProperty )
{
	if ( vMetier <= 0 || vMetier >= MAX_METIER )
		return 0;
	
	if ( vLevel <= 0 || vLevel > MAX_GRADE )
		return 0;

	if ( vOption < 0 || vOption >= PROPERTY_OPTION )
		return 0;

	if ( vProperty <= OPTION_NONE || vProperty >= MAX_PROPERTY )
		return 0;

	return ( ( (CTemplateMetierProperty*) spMetierProperty[ vMetier ][ vOption ] )->mMetierProperty[ vLevel - 1 ][ vProperty ] );
}

int CDataStatic::GetLevelProperty( int vLevel, int vOption )
{
	if ( vLevel <= 0 || vLevel > MAX_GRADE )
		return 0;

	if ( vOption < 0 || vOption >= PROPERTY_OPTION )
		return 0;

	return ( ( (CTemplateLevelProperty*) spLevelProperty[ vOption ] )->mLevelProperty[ vLevel - 1 ] );
}

int CDataStatic::GetCompose( int vComposeID )
{
	if ( vComposeID < 0 || vComposeID >= MAX_COMPOSE )
		return 0;

	return sComposeID[ vComposeID ];
}


//**********************************
// 主要为客户端留这个接口, 服务器在场景服务器启动时，已经加载template.dat
// 直接用 CDataStatic::sTemplateVersion
//**********************************
/*
	修改日期：2010/01/01
	修改作者：Zhaofine
	修改内容：客户端也不需要这个接口。
*/
int CDataStatic::GetTempVersion()
{
	return sTemplateVersion;
}


// 获取技能等级表
void *CDataStatic::GetSkillLifeTable( int nSkillType )
{		
	
	switch ( nSkillType )
	{ 
		case LIFESKILL_COLLECT:
		{
			return CDataStatic::mCollect.spLifeSkillTable;
			break;	
		}
		case LIFESKILL_GOLDMAKE:
		{
			return  CDataStatic::mGoldMake.spLifeSkillTable;
			break;	
		}
		case LIFESKILL_ARTIFICER:
		{
			return  CDataStatic::mArtificer.spLifeSkillTable;
			break;	
		}
		case LIFESKILL_PILLMAKER:
		{
			return  CDataStatic::mPillMaker.spLifeSkillTable;
			break;	
		}
		case LIFESKILL_KOOKER:
		{
			return  CDataStatic::mCooker.spLifeSkillTable;
			break;	
		}	
		default:
			break;		
	}
	return NULL;
}



// 某种生活技能相关信息
void *CDataStatic::GetSkillLifeInfo( int nSkillType )
{		
	switch ( nSkillType )
	{ 
	case LIFESKILL_COLLECT:
		{
			return &CDataStatic::mCollect;
			break;	
		}
	case LIFESKILL_GOLDMAKE:
		{
			
			return &CDataStatic::mGoldMake;
			break;	
		}
	case LIFESKILL_ARTIFICER:
		{
			return &CDataStatic::mArtificer;
			break;	
		}
	case LIFESKILL_PILLMAKER:
		{
			return &CDataStatic::mPillMaker;
			break;	
		}
	case LIFESKILL_KOOKER:
		{
			return &CDataStatic::mCooker;
			break;	
		}	
	default:
		break;		
	}
	return NULL;
}

int CDataStatic::InsertComposeInfo( int nLifeSkillType, int nComposeID, int nComposeKind, int nDegreeValue, int nComposeType )
{
	switch ( nLifeSkillType )
	{ 
	case LIFESKILL_COLLECT:
		{
			mCollect.InsertComposeInfo( nComposeID, nComposeKind, nDegreeValue, nComposeType );
			break;	
		}
	case LIFESKILL_GOLDMAKE:
		{

			mGoldMake.InsertComposeInfo( nComposeID, nComposeKind, nDegreeValue, nComposeType );
			break;	
		}
	case LIFESKILL_ARTIFICER:
		{
			mArtificer.InsertComposeInfo( nComposeID, nComposeKind, nDegreeValue, nComposeType );
			break;	
		}
	case LIFESKILL_PILLMAKER:
		{
			mPillMaker.InsertComposeInfo( nComposeID, nComposeKind, nDegreeValue, nComposeType );
			break;	
		}
	case LIFESKILL_KOOKER:
		{
			mCooker.InsertComposeInfo( nComposeID, nComposeKind, nDegreeValue, nComposeType );
			break;	
		}	
	default:
		break;		
	}	
	return 0;
}

int CDataStatic::GetCompoundIDList( int nDegreeValue, int nEquipLevel, int nEquipColor, int *pIDList,int *pNumList, int &nNum, int &rDegreeObtained )
{	
	CTemplateDecompound::DecompondTable *tpTable = NULL;
	
	nNum = 0;
	if ( spDecompund == NULL )
	{
		return -1;
	}
	CTemplateDecompound *tpDecompond = ( CTemplateDecompound * )spDecompund;
	
	switch( nEquipColor )
	{
		case CTemplateEquipment::EQUIPMENT_WHITE:
		{
			tpTable =	( CTemplateDecompound::DecompondTable * )&tpDecompond->mWhitDecompond;
			break;
		}		
		case CTemplateEquipment::EQUIPMENT_BLUE:
		{
			tpTable =	( CTemplateDecompound::DecompondTable * )&tpDecompond->mBlueDecompond;
			break;
		}
		case CTemplateEquipment::EQUIPMENT_PURPLE:
		{
			tpTable	=	( CTemplateDecompound::DecompondTable * )&tpDecompond->mPurpleDecompond;
			break;
		}
		default:
			break;
	}
	
	if ( tpTable == NULL )
	{
		return -1;
	}	
	
	int Index =	( nEquipLevel - 1 )/5;
	if ( Index >= DECOMPOUND_MAXTABLEINDEX )
	{
		return -1;
	} 
	
	// 熟练度判断
	if ( tpTable[Index].DegreeNeeded > nDegreeValue )
	{
		return DECOMPOUND_DEGREENOTENOUGH;
	}
		
	// 第一个物品ID
	if ( tpTable[Index].FstProbability + tpTable[Index].SecProbability + tpTable[Index].ThirdProbability <= 0 )
	{
		return -1;
	}	
		
	int tRandNum = rand()%( tpTable[Index].FstProbability + tpTable[Index].SecProbability + tpTable[Index].ThirdProbability );
	int tItemID		= 0;
	int tItemNum	= 0;
	if ( tRandNum >= 0 &&  tRandNum < tpTable[Index].FstProbability )
	{
		tItemID = tpTable[Index].FstItemID;
		if ( tpTable[Index].FstItemNum <= 0 )
		{
			tItemNum = 1;
		}
		else
		{
			tItemNum = rand() % tpTable[Index].FstItemNum + 1;	
		}
		
	}
	else if ( tRandNum >= tpTable[Index].FstProbability  &&
		 tRandNum < ( tpTable[Index].FstProbability + tpTable[Index].SecProbability ) )
	{
		tItemID = tpTable[Index].SecItemID;
		if (  tpTable[Index].SecItemNum <= 0 )
		{
			tItemNum = 1;
		}
		else
		{
			tItemNum = rand() % tpTable[Index].SecItemNum + 1;	
		}		
	}
	else
	{
		tItemID = tpTable[Index].ThirdItemID;
		if ( tpTable[Index].ThirdItemNum == 0 )
		{
			tItemNum = 1;
		}
		else
		{
			tItemNum = rand() % tpTable[Index].ThirdItemNum + 1;	
		}		
	}
	
	pIDList[0]  = tItemID;
	pNumList[0] = tItemNum;
	nNum		= 1;	
	
	// 计算获取的熟练度
	int tValue		= nDegreeValue - tpTable[Index].DegreeNeeded;
	int tOldValue	= 0;
	int i			= 0;
	for ( ; i < MAX_LIFESKILL_DEGREESTEP; ++i  )
	{
		if ( tValue >= tOldValue && tValue < tpDecompond->mValue[i] )
		{
			break;
		}
		tOldValue = tpDecompond->mValue[i];
	}

	rDegreeObtained = 0;
	if ( tpDecompond->mProbality[i] >= rand()%10000 )
	{
		rDegreeObtained = tpDecompond->mDegreeValue[i];
	}		
	return DECOMPOUND_SUCESS;
}



CTitle* CDataStatic::GetTitle( int vType, int vRootCndsn, int vSubCndsn, int& vTitleID )
{
	CTplTitleManager* tpTitleManager = ( CTplTitleManager* ) spTitleManager;
	if ( tpTitleManager == NULL )
	{
		return NULL;
	}

	for ( int i = 0; i < MAX_TITLE; i ++ )
	{
		if ( tpTitleManager->mTitle[ i ].mType == vType 
			&& tpTitleManager->mTitle[ i ].mObtainCondition == vRootCndsn 
			&& tpTitleManager->mTitle[ i ].mSubCondition == vSubCndsn )
		{
			vTitleID = i + 1;
			return &(tpTitleManager->mTitle[ i ]);
		}
	}
	return NULL;
}

/*
 *	传进地图ID，转换军团国家类称号ID
 */

int CDataStatic::GetTitle( int nParam, int nAction, int nLevel )
{
	CTplTitleManager* tpTitleManager = ( CTplTitleManager* ) spTitleManager;
	if ( tpTitleManager == NULL )
	{
		return 0;
	}

	for ( int i = 0; i < MAX_TITLE; i ++ )
	{
		if ( tpTitleManager->mTitle[ i ].mType == CTplTitleManager::SPECIAL_NATION
			&& tpTitleManager->mTitle[ i ].mObtainCondition == nAction 
			&& tpTitleManager->mTitle[ i ].mParm == nParam
			&& tpTitleManager->mTitle[ i ].mSubCondition == nLevel )
		{
			return i + 1;
		}
	}
	return 0;
}

/*
 *	TitleID 为TitleManager维护的称号数组的下标 + 1
 *	加一是为了区别在玩家身上维护的时候零无效表示
 */

CTitle* CDataStatic::GetTitle(int vTitleID)
{
	CTplTitleManager* tpTitleManager = ( CTplTitleManager* ) spTitleManager;
	if ( tpTitleManager == NULL )
	{
		return NULL;
	}

	if ( vTitleID > MAX_TITLE || vTitleID <= 0 )
	{
		return NULL;
	}
	return &(tpTitleManager->mTitle[ vTitleID - 1 ]);
}

/*
 *	nType : 好感、恶感
 *	nValue: 点数
 *  nNumerator: 返回分子
 *  nDenominator: 返回分母
 */
CJudgeLevel* CDataStatic::GetJudgeLevel( int nValue, int nType, int& nNumerator, int& nDenominator )
{
	CTplJudgeConfig* tpConfig = ( CTplJudgeConfig* ) spJudgeConfig;
	if ( tpConfig == NULL )
	{
		return NULL;
	}

	for ( int i = 0; i < MAX_JUDGE_LEVEL; i ++ )
	{
		if ( tpConfig->mJudgeLevel[ i ].mType != nType)
			continue;

		if ( nValue < tpConfig->mJudgeLevel[ i ].mLevelMax 
			&& nValue >= tpConfig->mJudgeLevel[ i ].mLevelMin )
		{
			nNumerator = nValue - tpConfig->mJudgeLevel[ i ].mLevelMin;
			nDenominator = tpConfig->mJudgeLevel[ i ].mLevelMax - tpConfig->mJudgeLevel[ i ].mLevelMin;
			return &(tpConfig->mJudgeLevel[ i ]);
		}
	}
	return NULL;
}

/*
 *	nLevel: 当前玩家等级
 */
int CDataStatic::GetJudgeNumMax( int nLevel )
{
	CTplJudgeConfig* tpConfig = ( CTplJudgeConfig* ) spJudgeConfig;
	if ( tpConfig == NULL )
	{
		return 0;
	}
#ifdef LINUX
	int tRet = tpConfig->mScoreBase + tpConfig->mScoreRate * (std::max( ( nLevel - tpConfig->mScoreLevel ), 0 ));
	return std::min( MAX_JUDGE_OTHER, tRet );
#else
	int tRet = tpConfig->mScoreBase + tpConfig->mScoreRate * (max( ( nLevel - tpConfig->mScoreLevel ), 0 ));
	return min( MAX_JUDGE_OTHER, tRet );
#endif
	
}

int CDataStatic::GetWeddingMoney( int nWeddingModule )
{
	if ( mMarriageConfig == NULL )
	{
		return 0;
	}	
	
	CTemplateMarriageConfig *tpMarriageConfig = ( CTemplateMarriageConfig * )mMarriageConfig;
	if ( nWeddingModule < 0 || nWeddingModule >= (int)ARRAY_CNT( tpMarriageConfig->mWeddinInfo ) )
	{
		return 0;
	}
	
	return tpMarriageConfig->mWeddinInfo[nWeddingModule].mMoneyNeeded;
}

int CDataStatic::GetProgressTimeLength( int nFuncType )
{
	int tTimeLength = 0;
	CTemplateProgressConfig *tpProgressConfig = ( CTemplateProgressConfig * )CDataStatic::spProgressConfig;
	if ( tpProgressConfig == NULL )
	{
		return tTimeLength;
	}
	for (  int i = 0; i < (int)ARRAY_CNT( tpProgressConfig->mProgressInfo ); ++i )
	{	   
		if ( nFuncType == tpProgressConfig->mProgressInfo[i].mFunctype )
		{
			tTimeLength = tpProgressConfig->mProgressInfo[i].mTime;
			break;
		}
	}
	return tTimeLength;
}

bool CDataStatic::IsWeddingHall( int nMapID )
{
	CTemplateMarriageConfig *tpConfig = ( CTemplateMarriageConfig * )CDataStatic::mMarriageConfig;
	if ( tpConfig == NULL )
	{
		return false;
	}
	
	for ( int i = 0; i < (int)ARRAY_CNT( tpConfig->mWeddinInfo ); ++i )
	{
		if ( tpConfig->mWeddinInfo[i].mWeddingHallMapID == nMapID )
		{
			return true;
		}
	}	
	return false;
}

bool CDataStatic::IsInBaitangPos( int nWeddingModue, int nMapID, int nBridePosX, int nBridePosY, int nGroomPosX, int nGroomPosY )
{
	CTemplateMarriageConfig *tpConfig = ( CTemplateMarriageConfig * )CDataStatic::mMarriageConfig;
	if ( tpConfig == NULL )
	{
		return false;
	}
	
	if ( IsWeddingHall( nMapID ) == false )
	{
		return false;					
	}
	
	if ( nWeddingModue < 0 || nWeddingModue > 2 )
	{
		return false;
	}
	
	if ( tpConfig->mWeddinInfo[nWeddingModue].mBridePosX != nBridePosX || tpConfig->mWeddinInfo[nWeddingModue].mBridePosY != nBridePosY )
	{
		return false;
	}
	
	if ( tpConfig->mWeddinInfo[nWeddingModue].mGroomPosX != nGroomPosX || tpConfig->mWeddinInfo[nWeddingModue].mGroomPosY != nGroomPosY )
	{
		return false;
	}
	
	return true;		
}

int CDataStatic::GetPreliminaryScore( int nPreliminaryToplistRank )
{
	CTemplateActivityConfig *tpConfig = ( CTemplateActivityConfig * )CDataStatic::mActivityConfig;
	if ( tpConfig == NULL )
	{
		return 0;
	}
	
	if ( nPreliminaryToplistRank < 0 || nPreliminaryToplistRank >= MAX_PRELIMINARY_TIMELYLISTNUM )
	{
		return tpConfig->mPreliminaryBaseScore;
	}
		
	return tpConfig->mPreliminaryBaseScore + tpConfig->mPreliminaryExtralScore[nPreliminaryToplistRank];
}

int CDataStatic::GetFameLimit( int nFameType )
{
	CTemplateRepeFameTable *tpConfig = ( CTemplateRepeFameTable * )CDataStatic::spRepeFameTable;
	if ( tpConfig == NULL )
	{
		return 0;
	}
	
	if ( nFameType == FAME_TYPE_COMBAT )
	{
		return tpConfig->mCombatLimit;
	}
	else if ( nFameType == FAME_TYPE_REPE )
	{
		return tpConfig->mRepeLimit;
	}
	else if ( nFameType == FAME_TYPE_COMMON )
	{
		return tpConfig->mCommonLimit;
	}
	else
	{
		return 0;
	}
}


void* CDataStatic::GetHandinEncourage( int nTempId )
{
    for ( int i=0; i<MAX_HANDINENCOURAGE; i++ )
    {
        if ( spHandinEncourage[i]==NULL )
        {
            return NULL;
        }

        CTplHandinEncourage* lpTplHandinEncourage = (CTplHandinEncourage*)spHandinEncourage[i];

        if ( lpTplHandinEncourage->mTempID == nTempId )
        {
            return (void*)lpTplHandinEncourage;
        }

    }

    return NULL;
}

void* CDataStatic::GetHandinTable( int nActivityId )
{
    for ( int i=0; i<MAX_HANDINTABLE; i++ )
    {
        if ( spHandinTable[i] == NULL )
        {
            return NULL;
        }


        CTplHandinTable* lpTplHandinTable = (CTplHandinTable*)spHandinTable[i];
        if ( lpTplHandinTable->mActivityId == nActivityId )
        {
            return (void*)lpTplHandinTable;
        }
    }

    return NULL;
}
void CTplHandinTable::SetFromPB( bool bServer, PBHandinTable* pHandinTable )
{
    if ( pHandinTable == NULL )
    {
        return ;
    }
    mTempID             = pHandinTable->tempid();
    mActivityId			= pHandinTable->activityid();
    for ( int i = 0; i < pHandinTable->encouragetplid_size() && i < (int)ARRAY_CNT( mEncourageTplId ); i ++ )
    {
        mEncourageTplId[ i ] = pHandinTable->encouragetplid( i );
    }
    return ;

}

void CTplHandinTable::CreateTlvPB( bool bServer, PBHandinTable* pHandinTable )
{
    if ( pHandinTable == NULL )
    {
        return ;
    }
    pHandinTable->set_tempid( mTempID );
    pHandinTable->set_activityid( mActivityId );
    for ( int i = 0; i < (int)ARRAY_CNT( mEncourageTplId ); i ++ )
    {
        pHandinTable->add_encouragetplid( mEncourageTplId[i] );
    }
    return ;
}

int CTplHandinTable::Initialize()
{
    mTempID             = 0;
    mActivityId         = 0;
    memset( &mEncourageTplId, 0x00, sizeof(mEncourageTplId) );
    return 0;
}

int CTplHandinEncourage::Initialize()
{
    mTempID             = 0;
    mHandinItemType     = 0;
    mHandinItemID       = 0;
    mHandinItemNum      = 0;
    mEncourageType      = 0;
    mEncourageId        = 0;
    mEncourageNum       = 0;
    return 0;
}

void CTplHandinEncourage::SetFromPB( bool bServer, PBHandinEncourage* pHandinEncourage )
{
    if ( pHandinEncourage == NULL )
    {
        return ;
    }
    mTempID             = pHandinEncourage->tempid();
    mHandinItemType     = pHandinEncourage->handinitemtype();
    mHandinItemID       = pHandinEncourage->handinitemid();
    mHandinItemNum      = pHandinEncourage->handinitemnum();
    mEncourageType      = pHandinEncourage->encouragetype();
    mEncourageId        = pHandinEncourage->encourageid();
    mEncourageNum       = pHandinEncourage->encouragenum();
    return ;
}

void CTplHandinEncourage::CreateTlvPB( bool bServer, PBHandinEncourage* pHandinEncourage )
{
    if ( pHandinEncourage == NULL )
    {
        return ;
    }
    pHandinEncourage->set_tempid( mTempID );
    pHandinEncourage->set_handinitemtype(mHandinItemType);
    pHandinEncourage->set_handinitemid(mHandinItemID);
    pHandinEncourage->set_handinitemnum(mHandinItemNum);
    pHandinEncourage->set_encouragetype(mEncourageType);
    pHandinEncourage->set_encourageid(mEncourageId);
    pHandinEncourage->set_encouragenum(mEncourageNum);
    return;
}

int CTemplateCorpsBOSSTable:: Initialize()
{
	memset( mBOSSID, 0 ,sizeof( mBOSSID ) );
	memset( mHardBOSSID, 0, sizeof(mBOSSID) ); 
	memset( mValueLimit, 0 ,sizeof( mValueLimit ));
	memset( mSummonMinLevel, 0, sizeof( mSummonMinLevel ) );
	memset( mHardSummonMinLevel, 0, sizeof( mHardSummonMinLevel ) );
	memset( mDropDesc, 0, sizeof(mDropDesc));
	memset( mHardDropDesc, 0, sizeof(mHardDropDesc));
	memset( mSummonMinValue, 0, sizeof(mSummonMinValue));
	memset( mHardSummonMinValue, 0, sizeof(mHardSummonMinValue));
	return 0;
}

void CTemplateCorpsBOSSTable:: SetFromPB( bool bServer, PBCorpsBOSS* pTable )
{
	if ( pTable == NULL )
	{
		return ;
	}

	mTempID = pTable->tempid();

	for ( int i = 0;i < pTable->bossid_size() && i < (int)ARRAY_CNT( mBOSSID ); i++)
	{
		mBOSSID[ i ] = pTable->bossid( i );
	}
	for ( int i = 0; i < pTable->dropdesc_size() && i < (int)ARRAY_CNT(mDropDesc); i++)
	{
		strncpy( mDropDesc[i], pTable->dropdesc(i).c_str(), STRING256 - 1);
	}

	for ( int i = 0; i < pTable->hardbossid_size() && i < (int)ARRAY_CNT(mHardBOSSID); i++ )
	{
		mHardBOSSID[ i ] = pTable->hardbossid( i );
	}

	for ( int i = 0; i < pTable->harddropdesc_size() && i < (int)ARRAY_CNT(mHardDropDesc); i++)
	{
		strncpy( mHardDropDesc[i], pTable->harddropdesc(i).c_str(), STRING256 - 1 );
	}

	for ( int i = 0; i < pTable->valuelimit_size() && i < (int)ARRAY_CNT(mValueLimit); i++ )
	{
		mValueLimit[ i ] = pTable->valuelimit( i );
	}

	for ( int i = 0; i < pTable->summonminlevel_size() && i < (int)ARRAY_CNT(mSummonMinLevel); i++ )
	{
		mSummonMinLevel[ i ] = pTable->summonminlevel( i );
	}

	for ( int i = 0; i < pTable->hardsummonminlevel_size() && i < (int)ARRAY_CNT(mHardSummonMinLevel); i++ )
	{
		mHardSummonMinLevel[ i ] = pTable->hardsummonminlevel( i );
	}

	for ( int i = 0; i < pTable->summonminvalue_size() && i < (int)ARRAY_CNT(mSummonMinValue); i++ )
	{
		mSummonMinValue[ i ] = pTable->summonminvalue( i );
	}

	for ( int i = 0; i < pTable->hardsummonminvalue_size() && i < (int)ARRAY_CNT(mHardSummonMinLevel); i++ )
	{
		mHardSummonMinValue[ i ] = pTable->hardsummonminvalue( i );
	}

	return;

}

void CTemplateCorpsBOSSTable:: CreateTlvPB( bool bServer, PBCorpsBOSS* pTable )
{
	if ( pTable == NULL )
	{
		return ;
	}

	pTable->set_tempid(mTempID);

	for( unsigned int i = 0; i < ARRAY_CNT( mBOSSID ); i++)
	{
		pTable->add_bossid( mBOSSID[i] );
		pTable->add_dropdesc(mDropDesc[i]);
		pTable->add_summonminlevel( mSummonMinLevel[i] );
		pTable->add_summonminvalue( mSummonMinValue[i] );
	}

	for ( unsigned int i = 0; i < ARRAY_CNT( mHardBOSSID ); i++)
	{
		pTable->add_hardbossid( mHardBOSSID[i]);
		pTable->add_harddropdesc( mHardDropDesc[i]);
		pTable->add_hardsummonminlevel( mHardSummonMinLevel[i] );
		pTable->add_hardsummonminvalue( mHardSummonMinValue[i]);
	}

	for( unsigned int i = 0; i < ARRAY_CNT( mValueLimit ); i++ )
	{
		pTable->add_valuelimit( mValueLimit[i] );
	}

	return;
}