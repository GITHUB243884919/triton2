#include "stdafx.hpp"
#include "message_maker.h"
#include "promessage_pb.hxx.pb.h"
#include "mapmessage_pb.hxx.pb.h"
#include "entity.h"
#include "proclient_svr.h"
#include "taskmodule.h"
#include "teammessage_pb.hxx.pb.h"
#include "teammodule.h"
#include "stallmodule.h"
#include "log_msg_pb.hxx.pb.h"
#include "logservertype.h"
#include "proclient_svr.h"
#include "template.h"
#include "npcmodule.h"
#include "familymodule.h"
#include "gmmodule.h"
#include "mailmodule.h"
#include "mapobjmanager.h"
#include "marriagemodule.h"
#include "logmodule.h"
#include "friendmanager.h"
#include "friendmodule.h"
#ifdef LEAK_CHK_DEBUG_NEW
#include "debug_new.h"
#endif


void SetPlayerProMessageForSinglePlayer( CEntityPlayer* pPlayer, CMessageCreatePlayerProNotice& rCreatePlayerProMsg )
{
	CPropertyPlayer* tpProperty = ( CPropertyPlayer* ) pPlayer->GetProperty();

	CItemEquipment* tpWeapon	= (CItemEquipment*) tpProperty->GetEquipment( )->GetItemObjPtr( EQUIPMENT_WEAPON );
	CItemEquipment* tpChest		= (CItemEquipment*) tpProperty->GetEquipment( )->GetItemObjPtr( EQUIPMENT_CHEST );
	CItemEquipment* tpFashion	= (CItemEquipment*) tpProperty->GetEquipment( )->GetItemObjPtr( EQUIPMENT_FASHION );
	CItemEquipment* tpMarriageEquip = (CItemEquipment*)tpProperty->GetEquipment()->GetItemObjPtr( EQUIPMENT_MARRY );

	//unsigned int tTime = time(0);
	if ( tpWeapon != NULL && tpWeapon->IsOvertimed() == true )
		tpWeapon = NULL;
	else if ( tpChest != NULL && tpChest->IsOvertimed() == true )
		tpChest = NULL;
	else if ( tpFashion != NULL && tpFashion->IsOvertimed() == true )
		tpFashion = NULL;
	else if ( tpMarriageEquip != NULL && tpMarriageEquip->IsOvertimed() == true )
		tpMarriageEquip = NULL;

	int tWeaponLevel = 0, tChestLevel = 0, tWeaponJewel = 0, tChestJewel = 0;
	tpProperty->GetEquipment( )->GetLightPoint( tWeaponLevel, tChestLevel, tWeaponJewel, tChestJewel );

	CPlayerProInfo* pPlayerInfo = rCreatePlayerProMsg.add_playerlist();
	LK_ASSERT( pPlayerInfo != NULL, return );

	pPlayerInfo->set_entityid( pPlayer->GetEntityID( ) );
	pPlayerInfo->set_metier( pPlayer->GetMetier() );
	pPlayerInfo->set_level( pPlayer->GetLevel() );
	pPlayerInfo->set_sex( pPlayer->GetSex() );
	pPlayerInfo->set_curhp( tpProperty->GetCurHP( ) );
	pPlayerInfo->set_curmp( tpProperty->GetCurMP( ) );
	pPlayerInfo->set_curap( tpProperty->GetCurAP( ));
	pPlayerInfo->set_weapon( tpWeapon != NULL ? tpWeapon->GetItemID() : 0 );
	pPlayerInfo->set_chest( tpChest != NULL ? tpChest->GetItemID() : 0 );
    pPlayerInfo->set_fashion( tpFashion != NULL ? tpFashion->GetItemID() : 0 );
	pPlayerInfo->set_marriageid( tpMarriageEquip != NULL ? tpMarriageEquip->GetItemID() : 0 );
	pPlayerInfo->set_weaponlevel( tWeaponLevel );
	pPlayerInfo->set_chestlevel( tChestLevel );
	pPlayerInfo->set_weaponjewel( tWeaponJewel );
	pPlayerInfo->set_chestjewel( tChestJewel );
    pPlayerInfo->set_equipshowstate( tpProperty->GetIsShowEquip() );
    pPlayerInfo->set_fashionshowstate( tpProperty->GetIsShowFashion() );
	
	if( pPlayer->GetTeamFlag() != 0 )
		pPlayerInfo->set_teamflag( pPlayer->GetTeamFlag() );

	pPlayerInfo->set_maxhp( tpProperty->GetMaxHP( ) );
	pPlayerInfo->set_maxmp( tpProperty->GetMaxMP( ) );
	pPlayerInfo->set_maxap( tpProperty->GetMaxAP( ) ); 
	pPlayerInfo->set_facetype( pPlayer->GetFaceType() );
	pPlayerInfo->set_nationality( pPlayer->GetNationality( ) );

	CBuffList *tpBuff = tpProperty->GetBuffList( );
	tpBuff->GetTlvBufferTeamShow( pPlayerInfo->mutable_bufflist() ); // bugf仅显示用

	tpProperty->GetStateListTlvBuffer2( pPlayerInfo->mutable_statelist() );

	pPlayerInfo->set_name( pPlayer->GetCharName() );
	pPlayerInfo->set_posx( pPlayer->GetPosX() );
	pPlayerInfo->set_posy( pPlayer->GetPosY() );

	if( pPlayer->GetDirection() != 0 )
		pPlayerInfo->set_direction( pPlayer->GetDirection() );
	
	pPlayerInfo->set_motionstate( pPlayer->GetMotionState() );
	pPlayerInfo->set_alivestate( pPlayer->GetAliveStatus() );
	pPlayerInfo->set_speed( tpProperty->GetCurSpeed( ) );

	if( tpProperty->GetHorseState( ) != 0 )
	{
		pPlayerInfo->set_horsestate( tpProperty->GetHorseState( ) );
		pPlayerInfo->set_horsetempid( tpProperty->GetHorseTempID( ) );
	}

	pPlayerInfo->set_charid( pPlayer->GetCharID() );

	if( tpProperty->GetHasRestoneStatus() != 0 )
		pPlayerInfo->set_hasredstonesta( tpProperty->GetHasRestoneStatus() );

	if( pPlayer->GetFamilyID() != 0 )
	{
		pPlayerInfo->set_familyid( pPlayer->GetFamilyID() ); 
		CFamilyInfo *tpFamilyInfo = CFamilyModule::GetSingleton().GetFamilyInfo( pPlayer->GetFamilyID() );
		if ( tpFamilyInfo != NULL )
		{
			pPlayerInfo->set_familyname( tpFamilyInfo->GetFamilyName() ); 
		}		
		// pPlayerInfo->set_familyname( pPlayer->GetFamilyName() ); 
	}
	
	if( pPlayer->GetCorpsID() != 0 )
	{
		pPlayerInfo->set_corpsid( pPlayer->GetCorpsID() );
		CFamilyInfo *tpFamilyInfo  = CFamilyModule::GetSingleton().GetFamilyInfo( pPlayer->GetFamilyID() );
		if ( tpFamilyInfo != NULL )
		{
			pPlayerInfo->set_corpsname( tpFamilyInfo->GetCorpsName() );	
		}		
	}

	if ( pPlayer->GetUnionID() != 0 )
	{
		pPlayerInfo->set_unionid( pPlayer->GetUnionID() );
		if ( pPlayer->GetUnionName() != NULL )
		{
			pPlayerInfo->set_unionname( pPlayer->GetUnionName() );
		}
	}
	pPlayerInfo->set_officialid( pPlayer->GetOfficialID() );

	// 如果有求组、或招募信息，搭车
	CTeamModule::GetSingleton().LoadTeamMsg( pPlayer, pPlayerInfo->mutable_msg() );

	// 如果有摊位，发送摊位消息
	CStallModule::GetSingleton().TouchStallPedal( pPlayer, pPlayerInfo->mutable_stallinfo() );

	//pPlayerInfo->set_pkvalue( tpProperty->GetPKValue() );

	pPlayerInfo->set_yellowstatus( (int)tpProperty->GetYellowStatus() );	
	
	if( pPlayer->GetCreatMethod() != 0 )
		pPlayerInfo->set_creatmethod( pPlayer->GetCreatMethod() );	

	if( tpProperty->GetCurrentTitle() != 0 )
		pPlayerInfo->set_currenttitle( tpProperty->GetCurrentTitle() );

	pPlayerInfo->set_pkvalue( tpProperty->GetPKValue() );

	CItemMagicWeapon* tpMW = (CItemMagicWeapon*)tpProperty->GetEquipment()->GetItemObjPtr( EQUIPMENT_MAGICWEAPON );
	if ( tpMW != NULL )
	{
		pPlayerInfo->set_mwid( tpMW->GetItemID() );
		pPlayerInfo->set_mwqlid( tpMW->GetCurQL() );
	}	

	if( pPlayer->GetCampID( ) != 0 ) 
		pPlayerInfo->set_campid( pPlayer->GetCampID( ) );

	// 判断是否GM
	if ( CGMModule::GetSingleton().IsGm( pPlayer->GetGmStatus() ) )
		pPlayerInfo->set_usertype(USERTYPE_GM);
	else
		pPlayerInfo->set_usertype(USERTYPE_NONE);

	if( tpProperty->IsVip() )
		pPlayerInfo->set_vipflag( (int) true );
		
	CMarriageInfo *tpMarriageInfo = CMarriageManager::GetSingleton().GetMarriageInfoByID( pPlayer->GetNationality(), pPlayer->GetMarriageID() );
	if ( tpMarriageInfo != NULL )
	{
		if ( pPlayer->GetCharID() == tpMarriageInfo->GetGroomID() )
		{
			pPlayerInfo->set_spousename( tpMarriageInfo->GetBrideName() );
		}
		else
		{
			pPlayerInfo->set_spousename( tpMarriageInfo->GetGroomName() );
		}
	}

	pPlayerInfo->set_protectstatus( pPlayer->GetProtectStatus( ) );
	pPlayerInfo->set_hydrangeastatus( pPlayer->GetHydrangeaStatus() );	
	
	CSwornTeam *tpSwornTeam = CFriendModule::GetSingleton().GetSwornTeam( pPlayer->GetSwornID() );
	if ( tpSwornTeam != NULL )
	{
		pPlayerInfo->set_swornname( tpSwornTeam->GetSwornName() );
	}	
}


//************************************
// Method:    BuildCreatePlayerProMessage
// FullName:  BuildCreatePlayerProMessage
// Access:    public 
// Returns:   CMessage*
// Qualifier: 创建多个玩家属性消息
// Parameter: std::vector<CEntityPlayer * > * pPlayerSet
// Parameter: CEntityPlayer * pPlayer
//************************************
CMessage* BuildCreatePlayerProMessage(std::vector<CEntityPlayer*>* pPlayerSet )
{
	static CMessage tMessage;
	static CMessageCreatePlayerProNotice tCreatePlayerMessage;

	tMessage.Clear();
	tCreatePlayerMessage.Clear();

	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CREATEPLAYER );
	tMessage.set_msgpara( (unsigned int)&tCreatePlayerMessage );

	for( std::vector<CEntityPlayer*>::iterator it = pPlayerSet->begin(); it != pPlayerSet->end(); ++it )
	{
		CEntityPlayer* tpPlayer = (*it);
		SetPlayerProMessageForSinglePlayer( tpPlayer, tCreatePlayerMessage );
	}
	
	return &tMessage;
}

//************************************
// Method:    BuildCreatePlayerProMessage
// FullName:  BuildCreatePlayerProMessage
// Access:    public 
// Returns:   CMessage*
// Qualifier: 创建单个玩家
// Parameter: CEntityPlayer * pPlayer
//************************************
CMessage* BuildCreatePlayerProMessage(CEntityPlayer* pPlayer)
{
	static CMessage tMessage;
	static CMessageCreatePlayerProNotice tCreatePlayerMessage;

	tMessage.Clear();
	tCreatePlayerMessage.Clear();

	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CREATEPLAYER );
	tMessage.set_msgpara( (unsigned int)&tCreatePlayerMessage );

	SetPlayerProMessageForSinglePlayer( pPlayer, tCreatePlayerMessage );

	return &tMessage;
}


void SetNpcProMessage( CEntity* pEntity, CMessageCreateNpcProNotice& rCreatNpcProMsg, int nListID, CEntityPlayer* pSelf )
{
	CProperty* tpProperty = ( CProperty* )pEntity->GetProperty();

	CNpcProInfo* pNpcPro = rCreatNpcProMsg.add_npclist();
	LK_ASSERT( pNpcPro != NULL, return );

	pNpcPro->set_entityid( pEntity->GetEntityID( ) );
	pNpcPro->set_npctype( pEntity->GetEntityType() );
	pNpcPro->set_npcid( tpProperty->GetPropertyTempID( ) );
	pNpcPro->set_pktype( ((CEntityNpc*)pEntity)->GetPKType( ) );
	pNpcPro->set_posx( pEntity->GetPosX() );
	pNpcPro->set_posy( pEntity->GetPosY() );
	pNpcPro->set_direction( pEntity->GetDirection() );

	if( pEntity->IsCharacter() )
	{
		CEntityCharacter* pEntityChar = (CEntityCharacter*)pEntity;
		CPropertyCharacter* tpProChar = (CPropertyCharacter*)tpProperty;
		pNpcPro->set_curhp( tpProChar->GetCurHP( ) );
		pNpcPro->set_curmp( tpProChar->GetCurMP( ) );
		pNpcPro->set_curap( tpProChar->GetCurAP( ) );
		pNpcPro->set_maxhp( tpProChar->GetMaxHP( ) );
		pNpcPro->set_maxmp( tpProChar->GetMaxMP( ) );
		pNpcPro->set_level( tpProChar->GetLevel( ) );
		CBuffList *tpBuff = tpProChar->GetBuffList( );
		tpBuff->GetTlvBufferShow( pNpcPro->mutable_bufflist() );
		pNpcPro->set_speed( tpProChar->GetCurSpeed( ) );
		if( pEntityChar->GetMotionState() != 0 )
			pNpcPro->set_motionstate( pEntityChar->GetMotionState() );
		//CWTPoint tPoint = CNpcModule::GetSingletonPtr()->GetTargetPos( pEntity->GetEntityID( ) );
		//pNpcPro->set_targetx( tPoint.mX );
		//pNpcPro->set_targety( tPoint.mY );
		if( pEntityChar->GetCampID() != 0 )
			pNpcPro->set_campid( pEntityChar->GetCampID( ) );

		pNpcPro->set_alivestate( pEntityChar->GetAliveStatus() );

		// 如果要显示所有者   // TODO: 暂时不支持怪物在家族护送或任务中的应用
		if ( pEntity->IsFunNpc()/* || pEntity->IsOgre()*/ )
		{
			CEntityNpc* pEntityNpc = (CEntityNpc*)pEntity;
			
			if ( (pEntityNpc->GetIsShowOwner() == 1) && (pEntityNpc->GetOwnercharID() != 0 ))
			{
				CEntityPlayer* pPlayer = CSceneLogic::GetSingleton().GetPlayerByCharID(pEntityNpc->GetOwnercharID());
				if (pPlayer != NULL)
				{
					pNpcPro->set_ownername( pPlayer->GetCharName() );
				}
			}
			int tFamilyID = CFamilyModule::GetSingleton().GetFamilyIDByNpcID( tpProperty->GetPropertyTempID( ) );
			char *pName = CFamilyModule::GetSingleton().GetFamilyName( tFamilyID );
			if ( pName != NULL )
			{
				pNpcPro->set_familyname( pName );
			}	

			const char* tpCorpsName = CCorpsModule::GetSingleton().GetCorpsNameByNpcID( tpProperty->GetPropertyTempID( ) );
			if ( tpCorpsName != NULL )
			{
				pNpcPro->set_corpsname( tpCorpsName );
			}
		}
	
		if ( pSelf != NULL && ( pEntity->IsFunNpc() || pEntity->IsOgre() ) )
		{
			int tTip = CTaskModule::GetSingleton().GetNpcTip( pSelf, tpProperty->GetPropertyTempID( ), pEntity->GetEntityID() );
			LOG_DEBUG("TASK", "send task tip : %d", tTip );
			pNpcPro->set_tasktip( tTip );
		}

		if( pEntity->IsPet()  )
		{
			// 召唤兽临时等级
			CEntityPet* pPet = (CEntityPet*)pEntity ;
			pNpcPro->set_pettmplevel( pPet->GetCurLevel() );
			pNpcPro->set_petattackmode( pPet->GetCurMode() );
			pNpcPro->set_ownerid( pEntity->GetOwnerID() ); // 只有召唤兽有主人
		}

		// 等待变身的时间(如果是怪，这个值是存活刷新)
		if( ((CEntityNpc*)pEntity)->GetOccurTime() > 0 )
		{
			pNpcPro->set_waittranstime( ((CEntityNpc*)pEntity)->GetOccurTime() );
		}

		tpProChar->GetStateListTlvBuffer2( pNpcPro->mutable_statelist() );
		pNpcPro->set_owncampid( ((CEntityNpc*)pEntity)->GetOwnCampID() );
	}
	else if ( pEntity->IsRedStone() )
	{
		CEntityRedStone *tpRedStone = ( CEntityRedStone * ) pEntity;
		CPropertyRedStone *tpRedStoneProty = (CPropertyRedStone *) tpRedStone->GetProperty();
		pNpcPro->set_npcid( tpRedStoneProty->GetPropertyTempID() );
		pNpcPro->set_redstonestatus( tpRedStone->GetStatus() );		
		pNpcPro->set_pktype( FRIEND_TO_SC|FRIEND_TO_FC|FRIEND_WITH_FC|FRIEND_WITH_SC );
	}
	else if( pEntity->IsPedal() )
	{
		CEntityPedal* pPedal = (CEntityPedal*)pEntity;
		pNpcPro->set_motionstate( pPedal->GetSkillLevel()  );
		pNpcPro->set_alivestate( pPedal->GetSkillID() );
		pNpcPro->set_pedaltype( pPedal->GetPedalType() );
	}
	//else if( pEntity->IsMoney() )
	//{
	//	CPropertyMoney* tpMoney = (CPropertyMoney*)( pEntity->GetProperty() );
	//	pNpcPro->set_money( tpMoney->GetMoney( ) );
	//}
	else if( pEntity->IsItem() )
	{
		CPropertyItem* tpItem = (CPropertyItem*)( pEntity->GetProperty() );
		// 道具归属
		for( unsigned int i = 0; i < TEAMNUM; i++ )
		{
			if( tpItem->GetOwnerCharID( i ) == 0 )
			{
				break;
			}
			pNpcPro->add_appertainings( tpItem->GetOwnerCharID( i ) );
		}
	}

	pNpcPro->set_nationnality( pEntity->GetNationality() );

	// 不是所有的实体都赋值了这个时间，怪物,pet等是有的
	if( pEntity->GetCreateTime() > 0 )
		pNpcPro->set_alivetime( LINEKONG::GetTickCount() - pEntity->GetCreateTime() ); 

	if( nListID != 0 )
		rCreatNpcProMsg.set_listid( nListID );
	const char *tpNewlyname = CMarriageModule::GetSingleton().GetNewlyName( pEntity );
	if ( tpNewlyname != NULL )
	{
		pNpcPro->set_newlyname( tpNewlyname );
	}
}




//************************************
// Method:    BuildCreateNpcProMessage
// FullName:  BuildCreateNpcProMessage
// Access:    public 
// Returns:   CMessage*
// Qualifier: 创建多个NPC
// Parameter: std::vector<CEntity * > * pNpcSet
// Parameter: int vListID
// Parameter: CEntityPlayer * pSelf
//************************************
CMessage* BuildCreateNpcProMessage(std::vector<CEntity*>* pNpcSet, int vListID /*= 0*/,  CEntityPlayer* pSelf /*= NULL*/)
{
	static CMessage tMessage;
	static CMessageCreateNpcProNotice tCreateNpcProPara;

	tMessage.Clear();
	tCreateNpcProPara.Clear();

	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CREATENPC );
	tMessage.set_msgpara( (long)&tCreateNpcProPara );

	for( std::vector<CEntity*>::iterator it = pNpcSet->begin(); it != pNpcSet->end(); ++it )
	{
		CEntity* pEntity = (*it);
		SetNpcProMessage( pEntity, tCreateNpcProPara, vListID, pSelf );
	}
		
	return &tMessage;
}

//************************************
// Method:    BuildCreateNpcProMessage
// FullName:  BuildCreateNpcProMessage
// Access:    public 
// Returns:   CMessage*
// Qualifier: 创建单个NPC
// Parameter: CEntity * pEntity
// Parameter: int vListID
// Parameter: CEntityPlayer * pSelf
//************************************
CMessage* BuildCreateNpcProMessage(CEntity* pEntity, int vListID /*= 0*/, CEntityPlayer* pSelf /*= NULL*/)
{
	static CMessage tMessage;
	static CMessageCreateNpcProNotice tCreateNpcProPara;

	tMessage.Clear();
	tCreateNpcProPara.Clear();

	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CREATENPC );
	tMessage.set_msgpara( (long)&tCreateNpcProPara );

	SetNpcProMessage( pEntity, tCreateNpcProPara, vListID, pSelf );

	return &tMessage;	
}

// 构建实体移动消息
CMessage* BuildEntityPathMessage( CEntity* pEntity, const CWTPoint& rSrcPos, const CWTPoint& rDstPos )
{
	static CMessage tMessage;
	static CMessageEntityPathNotice tEntityNotice;
	
	tMessage.Clear();
	tEntityNotice.Clear();

	// 移动消息屏蔽, 临时用 msgseq 字段来标示是否记录消息
	//tMessage.mutable_msghead()->set_msgseq( 1 );
	tMessage.mutable_msghead()->set_messageid( ID_C2S_NOTICE_ENTITYPATH );
	tMessage.set_msgpara( (uint32)&tEntityNotice );

	tEntityNotice.set_entityid( pEntity->GetEntityID() );
	tEntityNotice.set_srcposx( rSrcPos.mX );
	tEntityNotice.set_srcposy( rSrcPos.mY );
	tEntityNotice.set_desposx( rDstPos.mX );
	tEntityNotice.set_desposy( rDstPos.mY );

	return &tMessage;
}



CMessage* BuildDestroyEntityMessage( CEntity* pEntitySelf )
{
	static CMessage tMessage;
	static CMessageDestroyEntityNotice tDestroyEntity;

	tMessage.Clear();
	tDestroyEntity.Clear();

	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTICE_DESTROYENTITY );
	tMessage.set_msgpara( (uint32)&tDestroyEntity );

	tDestroyEntity.add_entityid(  pEntitySelf->GetEntityID() );

	return &tMessage;
}

CMessage* BuildDestroyEntityMessage( std::vector<CEntityPlayer*>* pPlayers, std::vector<CEntity*>* pNpcs )
{
	static CMessage tMessage;
	static CMessageDestroyEntityNotice tDestroyEntity;

	tMessage.Clear();
	tDestroyEntity.Clear();

	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTICE_DESTROYENTITY );
	tMessage.set_msgpara( (uint32)&tDestroyEntity );


	for( std::vector<CEntityPlayer*>::iterator itPlayer = pPlayers->begin() ;  itPlayer != pPlayers->end(); ++itPlayer )
	{
		tDestroyEntity.add_entityid( (*itPlayer)->GetEntityID() );
	}

	for( std::vector<CEntity*>::iterator itNpc = pNpcs->begin() ;  itNpc != pNpcs->end(); ++itNpc )
	{
		tDestroyEntity.add_entityid( (*itNpc)->GetEntityID() );
	}

	return &tMessage;
}



CMessage* BuildFuncResultMessage( CEntity* pEntity, unsigned short vFunc, unsigned short vResult, int vListID, unsigned int vValue1, unsigned int vValue2, unsigned int vValue3 /*= 0*/, unsigned int vValue4 /*= 0*/, unsigned int vValue5 /*= 0*/ )
{
	static CMessage 					tMsg;
	static CMessageFuncResultCallBack	tMsgCallBack;

	tMsg.Clear( );
	tMsgCallBack.Clear( );

	tMsg.mutable_msghead()->set_messageid( ID_S2C_FUNCRESULT_CALLBACK );

	tMsgCallBack.set_resultnumber( 1 );			
	tMsgCallBack.set_entityid( pEntity->GetEntityID( ) );

	PBFuncResult* tpPBResult = tMsgCallBack.add_funcresult();

	if( vResult != 0 )
		tpPBResult->set_result( vResult );
	if (vValue1 != 0)
		tpPBResult->set_value1( vValue1 );
	if (vValue2 != 0)
		tpPBResult->set_value2( vValue2 );

	tpPBResult->set_func( vFunc );

	if( vFunc != FUNC_COOLDOWN && vFunc != FUNC_PHCOOLDOWN )
	{
		if (vValue3 != 0)
			tpPBResult->set_value3( vValue3 );
		if (vValue4 != 0)
			tpPBResult->set_value4( vValue4 );
		if (vValue5 != 0)
			tpPBResult->set_value5( vValue5 );
		if( vListID != 0 )
			tpPBResult->set_listid( vListID );	

		tMsgCallBack.set_posx( pEntity->GetPosX() );
		tMsgCallBack.set_posy( pEntity->GetPosY() );
	}

	tMsg.set_msgpara( (long)&tMsgCallBack );

	return &tMsg; 
}




CMessage* BuildChangeStateNoticeMessage(unsigned int vEntityID, int vState, int vParame1, int vParame2, bool vInState )
{
	static CMessage tMessage;
	static CMessageChangeStateNotice tMessagePara;

	tMessage.Clear( );
	tMessagePara.Clear( );

	tMessage.set_msgpara( (uint32)&tMessagePara );
	tMessage.mutable_msghead()->set_messageid( ID_S2C_NOTIFY_CHANGESTATE );

	tMessagePara.set_entityid( vEntityID );
	tMessagePara.set_state( vState );
	tMessagePara.set_parame1( vParame1 );
	tMessagePara.set_parame2( vParame2 );
	tMessagePara.set_instate( vInState );
	tMessagePara.set_listid( CPropertyModule::GetSingleton().GetResultList()->GetListID() );

	return &tMessage;
}

// 构建完成任务的LOG消息
CMessage* BuildPlayerOpTaskLogMessage( CEntityPlayer* pPlayer, 
		int nTaskID, ETaskAction emAction, int nParam1, int nParam2, uint64_t nParam3 )
{
	static CMessage tMessage;
	static CMessageLogRoleOpTask proto;
	
	tMessage.Clear( );
	proto.Clear( );

	tMessage.set_msgpara( (uint32)&proto );
	tMessage.mutable_msghead( )->set_messageid( ID_S2L_ROLE_OPTASK );

	// Time
	time_t nNow = time(NULL);
	proto.set_optime(nNow);

	// Where
	proto.set_lineid( pPlayer->GetLineID() );
	proto.set_repetionid(pPlayer->GetMapObject() ? pPlayer->GetMapObject()->GetKey() : INVITED_KEY_VALUE);
	proto.set_mapid( GetMapID4Log(pPlayer) );

	// Role
	proto.set_roleid( pPlayer->GetCharID() );
	proto.set_metierid( pPlayer->GetMetier() );
	proto.set_rolelevel( pPlayer->GetLevel() );

	proto.set_taskid( nTaskID);

	// 行为
	proto.set_optype(emAction); 	
	proto.set_opparam1(nParam1);
	proto.set_opparam2(nParam2);
	proto.set_opparam3(nParam3);

	return &tMessage;
}

// 构建玩家死亡的LOG消息
CMessage* BuildPlayerBeKilledLogMessage( CEntityPlayer* pPlayer, 
		int nKillerType, int nKillerID, int nKillerMetier, int nKillerLevel)
{
	static CMessage tMessage;
	static CMessageLogRoleBeKilled proto;

	tMessage.Clear( );
	proto.Clear( );

	tMessage.set_msgpara( (uint32)&proto );
	tMessage.mutable_msghead( )->set_messageid( ID_S2L_ROLE_KILLED );

	// Time
	time_t nNow = time(NULL);
	proto.set_optime(nNow);

	// Where
	proto.set_lineid( pPlayer->GetLineID() );
	proto.set_repetionid(pPlayer->GetMapObject() ? pPlayer->GetMapObject()->GetKey() : INVITED_KEY_VALUE);
	proto.set_mapid( GetMapID4Log(pPlayer) );

	// Role
	proto.set_roleid( pPlayer->GetCharID() );
	proto.set_metierid( pPlayer->GetMetier() );
	proto.set_rolelevel( pPlayer->GetLevel() );

	// opp entity
	proto.set_killertype( nKillerType );
	proto.set_killerid( nKillerID );
	proto.set_killermetier( nKillerMetier);
	proto.set_killerlevel( nKillerLevel);

	return &tMessage;
}


// 构建离开场景的LOG消息
CMessage* BuildPlayerLogoutMessage( CEntityPlayer* pPlayer, EMLeaveReason emLeave, EMSaveResult emSave)
{
	static CMessage tMessage;
	static CMessageLogRoleLogout proto;

	tMessage.Clear( );
	proto.Clear( );

	tMessage.set_msgpara( (uint32)&proto );
	tMessage.mutable_msghead( )->set_messageid( ID_S2L_ROLE_LOGOUT );

	// Time
	time_t nNow = time(NULL);
	proto.set_optime(nNow);

	// Where
	proto.set_lineid( pPlayer->GetLineID() );
	proto.set_repetionid(pPlayer->GetMapObject() ? pPlayer->GetMapObject()->GetKey() : INVITED_KEY_VALUE);
	proto.set_mapid( GetMapID4Log(pPlayer) );

	// Role
	proto.set_roleid( pPlayer->GetCharID() );
	proto.set_metierid( pPlayer->GetMetier() );
	proto.set_rolelevel( pPlayer->GetLevel() );

	// Logout
	proto.set_loginip(pPlayer->GetClientInfo()->mClientAddr);
	proto.set_logintime(pPlayer->GetClientInfo()->mSockTime);
	proto.set_logouttime(nNow);
	proto.set_savereason(pPlayer->GetCommState());
	proto.set_saveresult(emSave);
	proto.set_leavereason(emLeave);
	proto.set_kickreason(pPlayer->GetKickoutReason());
	proto.set_kickcode(0);

	return &tMessage;
}

// 构建获得金币的LOG消息
CMessage* BuildPlayerOpMoneyLogMessage( CEntityPlayer* pPlayer, int nUnit, int nDelta, 
		EMoneyAction emAction, int nParam1, int nParam2, uint64_t nParam3 )
{
	static CMessage tMessage;
	static CMessageLogRoleOpMoney proto;

	tMessage.Clear( );
	proto.Clear( );

	tMessage.set_msgpara( (uint32)&proto );
	tMessage.mutable_msghead( )->set_messageid( ID_S2L_ROLE_OPMONEY );

	// Time
	proto.set_optime( time( NULL ) );

	// Where
	proto.set_lineid( pPlayer->GetLineID() );
	proto.set_repetionid(pPlayer->GetMapObject() ? pPlayer->GetMapObject()->GetKey() : INVITED_KEY_VALUE);
	proto.set_mapid( GetMapID4Log(pPlayer) );

	// Role
	proto.set_roleid( pPlayer->GetCharID() );
	proto.set_metierid( pPlayer->GetMetier() );
	proto.set_rolelevel( pPlayer->GetLevel() );

	// Money
	proto.set_moneytype(nUnit);
	proto.set_deltamoney( nDelta );

	CPropertyPlayer* tpProperty = ( CPropertyPlayer* ) pPlayer->GetProperty();
	switch(nUnit)
	{
		case CYuanBao::em_bind_money :
		{
			proto.set_curmoney( pPlayer->GetBindMoney());
			break;
		}

		case CYuanBao::em_unbind_money:
		{
			proto.set_curmoney(pPlayer->GetMoney());
			break;
		}

		case CYuanBao::em_golden_yuanbao:
		{
			proto.set_curmoney( tpProperty ? tpProperty->GoldenYB().remain() : 0);
			break;
		}

		case CYuanBao::em_silver_yuanbao:
		{
			proto.set_curmoney( tpProperty ? tpProperty->SilverYB().remain() : 0);
			break;
		}
		case CYuanBao::em_combat_fame:
		{
			proto.set_curmoney( tpProperty ? tpProperty->mWizard.GetCombatFame() : 0 );
			break;
		}
		case CYuanBao::em_repe_fame:
		{
			proto.set_curmoney( tpProperty ? tpProperty->mWizard.GetRepeFame() : 0 );
			break;
		}
		case CYuanBao::em_common_fame:
		{
			proto.set_curmoney( tpProperty ? tpProperty->mWizard.GetCommonFame() : 0 );
			break;
		}
		default:
		{
			proto.set_curmoney( 0 );
			break;
		}
	}

	proto.set_optype(emAction); 	
	proto.set_opparam1(nParam1);
	proto.set_opparam2(nParam2);
	proto.set_opparam3(nParam3);

	return &tMessage;
}

CMessage* BuildPlayerBuffLogMessage( CEntityPlayer* pPlayer, int BuffID, int PersistTime, 
		 int nParam1 , int nParam2 , unsigned short bufftype )
{   
	LK_ASSERT( pPlayer != NULL , return NULL );

	static CMessage tMessage;
	static CMessageLogRoleBuffer proto;

	tMessage.Clear( );
	proto.Clear( );

	tMessage.set_msgpara( (uint32)&proto );
	tMessage.mutable_msghead( )->set_messageid( ID_S2L_ROLE_BUFFER );
     
	proto.set_optime( time( NULL ) );

	// Where
	proto.set_lineid( pPlayer->GetLineID() );
	proto.set_mapid( GetMapID4Log(pPlayer) );
	// Role
	proto.set_roleid( pPlayer->GetCharID() );

    CPropertyPlayer* tpProperty = ( CPropertyPlayer* ) pPlayer->GetProperty();
	CBuffObject* tpBuff = tpProperty->GetBuffList()->GetBuffObjectByID( BuffID );

	//BUFF
	proto.set_optype(bufftype);
	proto.set_buffid(BuffID);
	proto.set_persisttime(PersistTime);
    proto.set_opparam1(nParam1);
	proto.set_opparam2(nParam2);

	return &tMessage;

}




// 构造玩家对物品的行为事件
CMessage* BuildPlayerOpItemLogMessage( CEntityPlayer* pPlayer, uint64_t ullGuID, 
		int nItemID, int nItemNum, int nTotal, EItemAction emAction, int nParam1, int nParam2, uint64_t nParam3)
{
	LK_ASSERT( pPlayer != NULL , return NULL );
	
	static CMessage tMessage;
	static CMessageLogRoleOpItem proto;

	tMessage.Clear( );
	proto.Clear( );

	tMessage.set_msgpara( (uint32)&proto );
	tMessage.mutable_msghead( )->set_messageid( ID_S2L_ROLE_OPITEM );

	// 时间
	proto.set_optime(time(0));

	// 地点
	proto.set_lineid( pPlayer->GetLineID() );
	proto.set_mapid( GetMapID4Log(pPlayer) );
	proto.set_repetionid(pPlayer->GetMapObject() ? pPlayer->GetMapObject()->GetKey() : INVITED_KEY_VALUE);

	// 人物
	proto.set_roleid( pPlayer->GetCharID() );
	proto.set_metierid( pPlayer->GetMetier() );
	proto.set_rolelevel( pPlayer->GetLevel() );

	// 物品ID
	proto.set_itemid( nItemID);

	// 物品数量
	proto.set_itemnum( nItemNum );

	// 可堆叠物品，是最后堆叠的物品GUID
	proto.set_itemguid(ullGuID);

	// 此时物品的总数量
	proto.set_itemtotal(nTotal);

	
	// 行为
	proto.set_optype(emAction); 	
	proto.set_opparam1(nParam1);
	proto.set_opparam2(nParam2);
	proto.set_opparam3(nParam3);

	return &tMessage;
}

// 构建获得经验的LOG消息
CMessage* BuildPlayerOpExpLogMessage( CEntityPlayer* pPlayer, 
		EUpgradeType emType, uint64_t nCurrent, int nDelta, int nOpType, int nParam1, int nParam2, uint64_t nParam3 )
{
	static CMessage tMessage;
	static CMessageLogRoleOpExp proto;

	tMessage.Clear( );
	proto.Clear( );

	tMessage.set_msgpara( (uint32)&proto );
	tMessage.mutable_msghead( )->set_messageid( ID_S2L_ROLE_OPEXP );

	// 时间
	proto.set_optime(time(0));

	// 地点
	proto.set_lineid( pPlayer->GetLineID() );
	proto.set_mapid( GetMapID4Log(pPlayer) );
	proto.set_repetionid(pPlayer->GetMapObject() ? pPlayer->GetMapObject()->GetKey() : INVITED_KEY_VALUE);

	// 人物
	proto.set_roleid( pPlayer->GetCharID() );
	proto.set_metierid( pPlayer->GetMetier() );
	proto.set_rolelevel( pPlayer->GetLevel() );

	// 经验
	proto.set_exptype(emType);
	proto.set_deltaexp( nDelta);
	proto.set_curexp(nCurrent);

	// 行为
	proto.set_optype(nOpType); 	
	proto.set_opparam1(nParam1);
	proto.set_opparam2(nParam2);
	proto.set_opparam3(nParam3);

	return &tMessage;
}

CMessage* BuildNpcDropLogMessage( CEntityNpc* pDropingEntity, std::vector<CEntity*>& rDropedEntityVec)
{
	static CMessage tMessage;
	static CMessageLogOrgeDropItem proto;

	tMessage.Clear( );
	proto.Clear( );

	if ( rDropedEntityVec.size() < 0) return NULL;

	tMessage.set_msgpara( (uint32)&proto );
	tMessage.mutable_msghead( )->set_messageid(ID_S2L_ORGE_DROPED);

	// 人物
	proto.set_npcid( pDropingEntity->GetNpcID() );
	proto.set_npcindex( 0 );
	proto.set_npclevel( pDropingEntity->GetProperty()->GetLevel());

	CCreator* pCreator = (CCreator*) CSceneObjManager::GetSingletonPtr()->GetObject( pDropingEntity->GetCreatorID() );
	if( pCreator != NULL )
	{
		proto.set_npcindex( pCreator->GetIndex() );
	}

	// 时间
	proto.set_optime(time(0));

	// 地点
	proto.set_lineid( pDropingEntity->GetLineID());
	proto.set_repetionid(pDropingEntity->GetMapObject() ? pDropingEntity->GetMapObject()->GetKey() : INVITED_KEY_VALUE);
	proto.set_mapid( GetMapID4Log(pDropingEntity) );

	std::vector<CEntity*>::iterator iter = rDropedEntityVec.begin();
	for( ; iter != rDropedEntityVec.end(); ++iter)
	{
		PBDropedItem *pbDrop = proto.add_dropitem();
		if ( pbDrop != NULL)
		{
			pbDrop->set_itemid((*iter)->GetProperty()->GetPropertyTempID());
			pbDrop->set_itemlevel((*iter)->GetProperty()->GetLevel());
			pbDrop->set_itemtype((*iter)->GetEntityType());
		}
	}

	return &tMessage;
}


// UseItemDrop
CMessage* BuildUseItemDropLogMessage( CEntityPlayer* pPlayer, int nItemID, std::vector<CEntity*>& rDropedEntityVec )
{
	static CMessage tMessage;
	static CMessageLogUseItemDropItem proto;

	tMessage.Clear( );
	proto.Clear( );

	if ( rDropedEntityVec.size() < 0) return NULL;

	tMessage.set_msgpara( (uint32)&proto );
	tMessage.mutable_msghead( )->set_messageid(ID_S2L_USEITEM_DROP);

	// 人物
	proto.set_charid( pPlayer->GetCharID() );
	proto.set_itemid( nItemID );
	
	// 时间
	proto.set_optime(time(0));

	// 地点
	proto.set_lineid( pPlayer->GetLineID());
	proto.set_repetionid(pPlayer->GetMapObject() ? pPlayer->GetMapObject()->GetKey() : INVITED_KEY_VALUE);

	proto.set_mapid( GetMapID4Log(pPlayer) );

	std::vector<CEntity*>::iterator iter = rDropedEntityVec.begin();
	for( ; iter != rDropedEntityVec.end(); ++iter)
	{
		PBDropedItem *pbDrop = proto.add_dropitem();
		if ( pbDrop != NULL)
		{
			pbDrop->set_itemid((*iter)->GetProperty()->GetPropertyTempID());
			pbDrop->set_itemlevel((*iter)->GetProperty()->GetLevel());
			pbDrop->set_itemtype((*iter)->GetEntityType());
		}
	}

	return &tMessage;
}

CMessage* BuildPlayerLevelUpLogMessage( CEntityPlayer* pPlayer, EUpgradeType emUpgrade, int nSubType, int nCurrentLevel, int nLevel)
{
	static CMessage tMessage;
	static CMessageLogRoleUpgrade proto;

	tMessage.Clear( );
	proto.Clear( );

	if ( pPlayer == NULL) return NULL;

	tMessage.set_msgpara( (uint32)&proto );
	tMessage.mutable_msghead( )->set_messageid(ID_S2L_ROLE_UPGRADE);

	proto.set_roleid(pPlayer->GetCharID());
	proto.set_metierid (pPlayer->GetMetier());
	proto.set_rolelevel(pPlayer->GetLevel());

	proto.set_upgradetime(time(0));
	proto.set_upgradetype(emUpgrade);
	proto.set_subtype(nSubType);
	proto.set_currentlevel(nCurrentLevel);
	proto.set_deltalevel( nLevel );

	return &tMessage;
}

CMessage* BuildDropedItemDestroyLogMessage(CEntity* pEntity)
{
	static CMessage tMessage;
	static CMessageLogItemDestroy proto;

	tMessage.Clear( );
	proto.Clear( );

	if ( pEntity == NULL) return NULL;

	tMessage.set_msgpara( (uint32)&proto );
	tMessage.mutable_msghead( )->set_messageid(ID_S2L_ITEM_DISPEAR);

	proto.set_itemid(pEntity->GetProperty()->GetPropertyTempID());
	proto.set_level(pEntity->GetProperty()->GetLevel());
	proto.set_optime(time(0));
	proto.set_lineid(pEntity->GetLineID());
	proto.set_repetionid(pEntity->GetMapObject() ? pEntity->GetMapObject()->GetKey() : INVITED_KEY_VALUE);
	proto.set_mapid( GetMapID4Log(pEntity) );
	proto.set_posx(pEntity->GetPosX());
	proto.set_posy(pEntity->GetPosY());

	return &tMessage;

}

CMessage* BuildItemLevelUpLogMessage(CEntityPlayer* pPlayer, 
		int nItemID, uint64_t ullGUID, int nType, int nResult, int nCurrLevel, int nPrevLevel, int nItem1, int nItem2)
{
	static CMessage tMessage;
	static CMessageLogItemUpgrade proto;

	tMessage.Clear( );
	proto.Clear( );

	if ( pPlayer == NULL) return NULL;

	tMessage.set_msgpara( (uint32)&proto );
	tMessage.mutable_msghead( )->set_messageid(ID_S2L_ITEM_UPGRADE);

	proto.set_itemid(nItemID);
	proto.set_itemguid(ullGUID);
	proto.set_uptime(time(0));
	proto.set_uptype(nType);
	proto.set_result(nResult);

	proto.set_roleid(pPlayer->GetCharID());
	proto.set_prevlevel(nPrevLevel);
	proto.set_currlevel(nCurrLevel);
	proto.set_item1(nItem1);
	proto.set_item2(nItem2);

	return &tMessage;
}

CMessage* BuildMailLogMessage( CEntityPlayer *tpPlayer, int nRoleID, int nMailID, int nOpType, int nOpTime, int nMailType, const char *pMailTitle = NULL, 
	int nTotalNum = 0, int *pItemIDList = 0, int *pItemNumList = 0, int nItemNum = 0 )
{
	static CMessage tMessage;
	static CMessageLogMail tMsgMailLog;
	
	if ( tpPlayer == NULL )
	{
		return NULL;
	}
	
	tMessage.set_msgpara( (int)&tMsgMailLog );
	tMessage.mutable_msghead()->set_messageid( ID_S2L_MAIL );
	
	tMsgMailLog.set_mailid( nMailID );
	if ( pMailTitle != NULL )
	{
		tMsgMailLog.set_mailtitle( pMailTitle );
	}
	tMsgMailLog.set_mailtype( nMailType );
	tMsgMailLog.set_optime( nOpTime );
	tMsgMailLog.set_optype( nOpType );
	tMsgMailLog.set_roleid( nRoleID );
	tMsgMailLog.set_totalnum( nTotalNum );
	
	if ( pItemIDList == NULL || pItemNumList == NULL || nItemNum <= 0 )
	{	  
		return &tMessage;
	}
	
	switch( nItemNum )	
	{
		case 5:
		{
			tMsgMailLog.set_itemid5( pItemIDList[4] );
			tMsgMailLog.set_itemnum5( pItemNumList[4] );
		}
		
		case 4:
		{
			tMsgMailLog.set_itemid4( pItemIDList[3] );
			tMsgMailLog.set_itemnum4( pItemNumList[3] );
		}
		
		case 3:
		{
			tMsgMailLog.set_itemid3( pItemIDList[2] );
			tMsgMailLog.set_itemnum3( pItemNumList[2] );			
		}
		
		case 2:
		{
			tMsgMailLog.set_itemid2( pItemIDList[1] );
			tMsgMailLog.set_itemnum2( pItemNumList[1] );						
		}
		
		case 1:
		{
			tMsgMailLog.set_itemid1( pItemIDList[0] );
			tMsgMailLog.set_itemnum1( pItemNumList[0] );												
		}
		default:
			break;
	}	
	return &tMessage;
}

