#pragma once

#include "gmmessage_pb.hxx.pb.h"
#include "servercore.h"
#include "servertool.h"
#include "errcode.h"
#include "entity.h"

class CMapObject;


// ********************************************************************** //
// CGMModule
// ********************************************************************** //
//
//
#define ACTIVITY_ITEM_LIMIT 12
#define INVALID_ITEM_ID -1

class CGMModule : public CLogicModule, public CSingleton< CGMModule >
{
  private:
	enum em_gm_right
	{
		em_priv_none				= 0x00000000,	// 无效值
		em_priv_is_gm				= 0x10000000,	//GM
		em_priv_kickoff_player		= 0x00000001,	//踢人
		em_priv_shutoff_player		= 0x00000002, 	//禁言
		em_priv_cancel_shutoff		= 0x00000004,	//解禁言
		em_priv_freeze_player		= 0x00000008,	//冻结帐号
		em_priv_unfreezen_player	= 0x00000010,	//解冻帐号
		em_priv_fly					= 0x00000020,	//飞
		em_priv_insert_item			= 0x00000040,	//插入物品
		em_priv_insert_money		= 0x00000080,	//插入金钱
		em_priv_move				= 0x00000100,	//拉人
		em_priv_hide				= 0x00000200,	// 隐身/现身
		em_priv_fb					= 0x00000400,
		em_priv_gate				= 0x00000800,	// 修改Gate数据		
		em_priv_clear_safe_pwd		= 0x00001000,	// 重置安全码
		em_priv_create_npc			= 0x00002000,	// 刷npc
		em_priv_destroy_npc			= 0x00004000,	// 销毁npc
		em_priv_reload_map			= 0x00008000,	// 重新加载地图
		em_priv_rank_remove			= 0x00010000,
		em_priv_show_npc_info		= 0x00020000,	// 显示实体的信息
		em_priv_change_repetion		= 0x00040000,	// 修改副本进入次数
		em_priv_clean_all_repetion	= 0x00080000,
		em_priv_callback_corpsmoney = 0x00100000,	
		em_priv_battle				= 0x00200000,	// 战争权限（家族争霸战，军团争霸战等）
		em_priv_tel					= 0x00400000,
		em_priv_war					= 0x00800000,	// 修改国战时间
		em_priv_change_camp         = 0x01000000,   // 转阵营

	};

	enum em_user_right
	{
		em_account_trusted			= 0x00000002,	// 被托管状态
	};

	inline bool HasRight(CEntityPlayer* tpPlayer, enum em_gm_right right)
	{
		return (tpPlayer != NULL && IsGm(tpPlayer->GetGmStatus()) && ((tpPlayer->GetGmStatus() & right) == (uint32_t) right));
	}


	//typedef std::pair<lk::CString32, int> CPairItemCount;
	class CPairItemCount
	{
		public:
			lk::CString32 ItemCode;
			int ItemID;
			int ItemNum;

			CPairItemCount(const char* szCode, int id, int num)
			{
				ItemCode = szCode; 
				ItemID = id;
				ItemNum = num;
			}

			CPairItemCount() {}
	};

	typedef lk::vector<CPairItemCount, ACTIVITY_ITEM_LIMIT > CActItemList;
	typedef lk::hash_map<int , CActItemList, SERVER_CAP_PLAYER>  CActItemBuffer; 

	CActItemBuffer mActItemBuffer;

public:
	static char			sModuleName[ NAME_LENGTH ];

	CGMModule() : CLogicModule( sModuleName )
	{ 
		if( CObj::msCreateMode )
		{
			mActItemBuffer.initailize();
		}
	}
	
	static unsigned int CountSize();

	// ly移动到public 因为外部要判断是否GM
	inline bool IsGm(unsigned int gm_status) 
	{
		return ((gm_status & em_priv_is_gm) == em_priv_is_gm );
	}

	inline bool IsGmTrusted(unsigned int gm_status)
	{
		return (!IsGm(gm_status)) && ((gm_status & em_account_trusted) == em_account_trusted); 
	}

private:
public:
	void OnGmMessageKickRoleCmd(CMessage* tpMessage);
	void OnGmMessageMoveRoleCmd(CMessage* tpMessage);
	void OnGmMessageShutUpCmd(CMessage* tpMessage);
	void OnGmMessageCancelShutUpCmd(CMessage* tpMessage);
	void OnGmMessageEndTaskCmd(CMessage* tpMessage);
	void OnGmMessageTrusteeStorageCmd(CMessage* tpMessage);
	void OnGmMessageUnTrusteeStorageCmd(CMessage* tpMessage);

	void OnGmMessageChangeMoneyCmd(CMessage* tpMessage);
	void OnGmMessageChangeExpCmd(CMessage* tpMessage);
	void OnGmMessageChangeItemCmd(CMessage* tpMessage);
	void OnGmMessageRoleLocationCmd(CMessage* tpMessage);
	void OnGmMessageTaskInfoCmd(CMessage* tpMessage);

	void OnMessageFangChenMiResponse(CMessage* tpMessage);

	bool GetOneValidPos4Player(CMapObject* pMap, CWTPoint here, CWTPoint* pwhere);

	EFcmStatus CheckPlayerFcmStatus(int nRemainSeconds);
	void OnMessageFCMPlayerListNotify(CMessage* tpMessage);
	void OnMessageFCMPlayerResetNotify(CMessage* tpMessage);

	// 2.4 新增
	void OnGmMessageRoleDetailCmd(CMessage* tpMessage);
	void OnGmMessageRoleEquipmentCmd(CMessage* tpMessage);
	void OnGmMessageRolePackInfoCmd(CMessage* tpMessage);
	void OnGmMessageRoleItemInPackCmd(CMessage* tpMessage);
	void OnGmMessageRoleSkillInfoCmd(CMessage* tpMessage);
	void OnGmMessageRoleSkillDetailCmd(CMessage* tpMessage);
	void OnGmMessagePostBulletinCmd(CMessage* tpMessage);
	void OnGmMessageUserStorageInfoCmd(CMessage* tpMessage);

	// 查找玩家的社会关系
	void OnGmMessageUserSocialInfoCmd(CMessage* tpMessage);

	// 取得玩家补偿或活动的物品列表
	void OnGmMessageUserItemListResponse(CMessage* tpMessage);

	// 玩家领取物品回应
	void OnGmMessageUserItemMinusResponse(CMessage* tpMessage);

	// 玩家点击活动NPC回应
	void OnGmMessageActivityListResponse(CMessage* tpMessage);

	//void FillPbItem(PBItemObject* pbItem, const PBExItem& pbActivityItem);
	void OnMessageMinusActivityItem(CEntityPlayer* tpPlayer, CMessage* tpMessage);	

	// MultiExp
	void OnGmMessageMultiExpCmd(CMessage* tpMessage);

	// CanMultiExp
	void OnGmMessageCancelMultiExpCmd(CMessage* tpMessage);

	// 发送使用GM指令回应
	void SendUseGmCommandResult( CEntityPlayer* pPlayer, const char* pMsg );


public:
	// 启动服务器
	virtual void OnLaunchServer();

	// 退出服务器
	virtual void OnExitServer();

	// 服务器Timer
	virtual void OnTimer( unsigned int vTickCount, unsigned int vTickOffset );

	// 建立场景
	virtual int OnCreateMap( unsigned short vLineID, unsigned short vMapID, unsigned short vIndex, const char* pTplName ){	return SUCCESS; }

	// 路由消息
	virtual void OnRouterMessage( CMessage* pMessage ){}
	
	// GM消息
	void OnGateMessage( CMessage* pMessage );

	// 客户端发送消息
	virtual void OnClientMessage( CEntityPlayer* pPlayer, CMessage* pMessage );

	// 建立实体
	virtual void OnCreateEntity( CEntity* pEntity , CCreator* pCreator, bool vFirst );

	// 销毁实体
	virtual void OnDestroyEntity( CEntity* pEntity, int vCode );

	// 存储数据
	virtual bool OnSaveData( CEntityPlayer* pPlayer, int vSaveCode ) { return true; }

	// 加载配置
	virtual void OnLoadConfig( ) ;

	// 场景启动完毕检查
	virtual bool OnCheckLaunch( ) { return true; }
};

