import "ibstore_pb.hxx";
import "property_pb.hxx";
import "countrymessage_pb.hxx";
import "boursemessage_pb.hxx";

// 以最快速度序列/反序列
option optimize_for = SPEED;


enum EMServerMessageID
{
	// 0x0000 + 0x8000
    ID_INTER_TIMEOUT					= 0x8000;	// 服务器内部的超时消息(仅内部使用)
    ID_G2L_GATE_INFO_NOTICE				= 0x8001;
    ID_L2G_GATE_INFO_RESPONSE			= 0x8002;
    ID_S2G_SERVER_INFO_NOTICE			= 0x8006;
    ID_G2S_SERVER_INFO_RESPONSE			= 0x8007;
    ID_G2S_CREATE_MAP_NOTICE			= 0x800F;
    ID_S2G_CREATE_MAP_RESPONSE			= 0x8010;
    ID_G2S_SCENE_NOT_OPEN_NOTICE		= 0x8014;
    ID_G2L_UPDATE_VERSION_NOTICE		= 0x8015;
    ID_X2X_ERATING_PROTO_STREAM			= 0x8016;
    ID_L2G_ERATING_STATUS_NOTICE		= 0x8017;
    ID_G2L_GATE_SHUTDOWN_NOTICE			= 0x8018;
	ID_G2L_HEARTBEAT_NOTICE				= 0x8019; // 网关到登陆服务器的心跳
    ID_X2P_REGIST_SERVER_REQUEST		= 0x8020;
    ID_P2X_REGIST_SERVER_RESPONSE		= 0x8021;
    ID_X2P_HEART_BEAT_NOTICE			= 0x8022;
    ID_G2S_COUNTRY_INFO					= 0x8030;
    ID_S2G_GAME_HEART_BEAT_NOTICE		= 0x8031;
    ID_A2G_GAME_PRIVILEGE_NOTICE		= 0x8032;	// 从全局服务器取得权限的通知
	ID_G2S_NOTIFY_SAFE_SHUTDOWN			= 0x8033;	// 安全关闭服务器通知
	ID_S2G_REGIST_REPETION_INFO			= 0x8034;
	ID_S2G_ACK_SAFE_SHUTDOWN			= 0x8035;	// 安全关闭服务器回应
	ID_L2G_LOGINSVRSTARTUP_NOTICE		= 0x8040;	// Loginserver启动通知

	// 0x0400 + 0x8000
    ID_L2G_PLAYER_ENTER_GATE_NOTICE		= 0x8403;  
    ID_G2L_PLAYER_ENTER_GATE_RESPONSE	= 0x8404;
    ID_G2L_PLAYER_LEAVE_GATE_NOTICE		= 0x8405;
    ID_G2S_PLAYER_ENTER_MAP_NOTICE		= 0x8408;
    ID_S2G_PLAYER_ENTER_MAP_RESPONSE	= 0x8409;
    ID_G2S_PLAYER_LOGIN_COMPLETE_NOTICE	= 0x840A;
    ID_S2G_PLAYER_LEAVE_MAP_NOTICE		= 0x840B;
    ID_G2S_PLAYER_LEAVE_MAP_RESPONSE	= 0x840C;
    ID_S2G_PLAYER_RETURN2GATE_NOTICE	= 0x840D;
    ID_G2S_PLAYER_RETURN2GATE_RESPONSE	= 0x840E;
    ID_S2G_PLAYER_REDIRECT_REQUEST		= 0x8411;
    ID_G2S_PLAYER_REDIRECT_RESPONSE		= 0x8412;
    ID_S2G_PLAYER_ENTER_CONFIRM_NOTICE	= 0x8413;
    ID_G2S_KICKOFF_PLAYER_REQUEST		= 0x8423;
    ID_S2G_KICKOFF_PLAYER_RESPONSE		= 0x8424;
    ID_G2S_REDIRECT_MAP_NOTICE			= 0x8426;
    ID_S2G_REDIRECT_ENTER_MAP_RESPONSE	= 0x8427;
    ID_G2S_REDIRECT_G_ERROR_CODE		= 0x8428;
    ID_S2G_REDIRECT_S_ERROR_CODE		= 0x8429;
    ID_S2G_SERVICE_MAILCOUNT_NOTICE		= 0x8435;

	ID_S2G_ENTITY_GM_MOVE				= 0x8436;	// gm移动
	ID_G2S_ENTITY_GM_MOVE				= 0x8437;


	ID_S2G_ROLECHANGECAMP_NOTICE		= 0x8439;	// 玩家选择阵营后通知网关
	ID_S2G_NOTIFY_ROLEGAMEINFO2GATE		= 0x8441;	// 场景通知网关玩家游戏数据变化	
	ID_G2S_TRACEDPLAYER_NOTIFY			= 0x8442;   // 网关通知场景服务器待追踪的玩家列表	
	ID_G2S_NOTIFY_CREATE_REPETION		= 0x8443;
	ID_S2G_NOTIFY_CHANGEPASSWORDSTATUS	= 0x8444;	// 玩家安全密码状态改变的通知	

	ID_S2G_NOTIFY_VIPFLAG				= 0x8445;	// 玩家会员标志改变的通知
	ID_G2S_NOTIFY_RESETROLEPASSWORD		= 0x8446;	// 网关通知场景重设玩家密码

	ID_S2G_ROLE_MOVE					= 0x8447;
	ID_G2S_ROLE_MOVE					= 0x8448;

	// 网关请求重新存盘
	ID_G2S_REQUEST_RESAVEPLAYER			= 0x8449;
	ID_S2G_RESPONSE_RESAVEPLAYER		= 0x844A;

	//	......							//0x81FE; // 占位
	//	......							//0x81FF; // 占位
	

	// 0x1000 + 0x8000
	// 发布任务模块
	ID_S2G_REQUEST_PLAYERPBULISHTASK	= 0x9001; // 场景->网关 玩家发布任务
	ID_G2S_RESPONSE_PBULISHTASKRESULT	= 0x9002; // 网关->场景 玩家发布任务回应

	ID_S2G_REQUEST_CHECKPUBLISHBOARD	= 0x9003; // 场景->网关 玩家查看展板请求	
	ID_G2S_RESPONSE_CHECKPUBLISHBOARD	= 0x9004; // 网关->场景 查看展板回应


    // 好友模块的消息ID 0x1800 + 0x8000
    ID_S2G_PLAYER_ONLINE_FRIEND_REQUEST	= 0x9801;
    ID_G2S_PLAYER_ONLINE_FIREND_RESPONSE= 0x9802;
    ID_S2G_PLAYER_CHANGE_STATUS_NOTICE	= 0x9803;
	ID_S2G_ADDFRIEND_REQUEST			= 0x9804;  
	ID_G2S_ADDFRIEND_RESPONSE			= 0x9805;  
	ID_S2G_GETCHARID_REQUEST			= 0x9806;  
	ID_G2S_GETCHARID_RESPONSE			= 0x9807;  
	ID_S2G_ADDMASTERPOINT_REQUEST		= 0x9808;
	ID_G2S_ADDMASTERPOINT_NOTIFY		= 0x9809;
	ID_S2G_ACQUIREMASTER_NOTIFY			= 0x980A;

    // 组队模块的消息ID 0x1C00 + 0x8000
    ID_S2G_REQUEST_CREATETEAMATGATE		= 0x9C01;
    ID_S2G_REQUEST_JOINREQUEST			= 0x9C02;
	ID_S2G_REQUEST_REPLYJOINREQUEST		= 0x9C03;
	ID_S2G_REQUEST_JOININVITE			= 0x9C04;
	ID_S2G_REQUEST_REPLYJOININVITE		= 0x9C05;
	ID_S2G_REQUEST_KICKOUTTEAM			= 0x9C06;
	ID_S2G_REQUEST_CHANGEHEADER			= 0x9C07;
	ID_S2G_REQUEST_LEAVETEAM			= 0x9C08;
	ID_S2G_REQUEST_ENDTEAM				= 0x9C09;
	ID_G2S_NOTIFY_ERRORNOTICE			= 0x9C0A;
	ID_G2S_RESPONSE_CREATETEAM			= 0x9C0B;
	ID_G2S_RESPONSE_JOINREQUEST			= 0x9C0C;
	ID_G2S_RESPONSE_JOININVITE			= 0x9C0D;
	ID_G2S_NOTIFY_UPDATETEAM			= 0x9C0E;
	ID_G2S_NOTIFY_SENDLIST				= 0x9C0F;
	ID_G2S_RESPONSE_INVITEREQUEST		= 0x9C10;
	ID_S2G_REQUEST_UPDATETEAMPRO		= 0x9C11;
	ID_G2S_RESPONSE_UPDATETEAMPRO		= 0x9C12;
	ID_S2G_REQUEST_CLAIMRECRUIT			= 0x9C13;
	ID_S2G_REQUEST_QUERYCLAIMRECRUIT	= 0x9C14;
	ID_G2S_RESPONSE_QUERYCLAIMRECRUIT	= 0x9C15;
	ID_G2S_RESPONSE_CLAIMRECRUIT		= 0x9C16;
	ID_S2G_REQUEST_FOLLOW				= 0x9C17;
	ID_G2S_RESPONSE_FOLLOW				= 0x9C18;
	ID_S2G_REQUEST_DELETECLAIMRECRUIT	= 0x9C19;
	ID_G2S_RESPONSE_DELETECLAIMRECRUIT	= 0x9C1A;
	ID_S2G_REQUEST_DISTRIBUTE			= 0x9C1B;
	ID_G2S_RESPONSE_DISTRIBUTE			= 0x9C1C;
	ID_S2G_REQUEST_OBTAINTEAMATGATE		= 0x9C1D;
	ID_S2G_REQUEST_CLEARRECORD			= 0x9C1E;
	
	// 邮件模块的消息ID 0x2000 + 0x8000
    ID_C2G_REQUEST_GETMAILINFO			= 0xA001;		// 获取玩家邮件相关信息
    ID_G2S_RESPONSE_GETMAIINFO			= 0xA002;		// 返回玩家邮件相关信息
    ID_G2S_NOTIFY_SENDSYSMAILTOPLAYER   = 0xA003;       // 向玩家发送系统邮件
    
	// 副本模块的消息ID 0x2C00 + 0x8000
	ID_S2G_DESTROY_REPETION_MAP			= 0xAC01;
	ID_S2G_BATTLE_TEAM_WAIT				= 0xAC02;
	ID_S2G_BATTLE_WAIT_LEAVE_REQUEST	= 0xAC03;
	ID_G2S_BATTLE_JOIN_NOTICE			= 0xAC04;
	ID_S2G_BATTLE_PLAYER_JOIN			= 0xAC05;
	ID_G2S_BATTLE_WAIT_NOTICE			= 0xAC06;
	ID_G2S_BATTLE_WAIT_LEAVE_RESPONSE	= 0xAC07;
	ID_S2G_BATTLE_CREATE_REQUEST		= 0xAC08;
	ID_G2S_BATTLE_CREATE_RESPONSE		= 0xAC09;
	ID_S2G_CREATE_REPETION_ERR			= 0xAC0A;
	ID_S2G_INTO_REPETION_TEAM			= 0xAC0B;
	ID_S2G_INTO_REPETION_SINGLE			= 0xAC0C;
	ID_G2S_INTO_REPETION_NOTIFY			= 0xAC0D;
	ID_G2S_REPETION_EVENT_CODE			= 0xAC0E;
        ID_S2G_BATTLE_TEAM_FIX_WAIT             = 0xAC0F;
        ID_S2G_BATTLE_SINGLE_FIX_WAIT           = 0xAC10;
        ID_S2G_BATTLE_FIX_WAIT_LEAVE_REQUEST    = 0xAC11;
	ID_S2G_REPETION_BATTLE_WINNER_NOTIFY= 0xAC12;
	ID_S2G_JOIN_ONTIME_OPEN_BATTLE_REQUEST = 0xAC13;
	ID_S2G_BATTLE_CLOSE					= 0xAC14;
	ID_S2G_INTO_CORPSBOSS_REPETION		= 0xAC15;
	ID_S2G_END_CORPSBOSS_REPETION		= 0xAC16;

	// 活动模块的消息ID 0x3000 + 0x8000	
	ID_G2S_TODAYACTIVITYS_LIST_NOTIFY	= 0xB001;
	ID_G2S_ACTIVITY_CHANGE_NOTIFY		= 0xB002;
	ID_G2S_ACTIVITY_EVENT_NOTIFY		= 0xB003;
	
    	// 获取摇钱树活动消息
    	ID_S2G_GETCAMPRANKFORYQS_REQUEST    	= 0xB004;    // 获取摇钱数阵营排名(S->G)
    	ID_G2S_GETCAMPRANKFORYQS_RESPONSE   	= 0xB005;    // 获取摇钱树阵营排名(G->S)

	ID_S2G_UPDATEYQSGROWING_REQUEST     	= 0xB006;    // 上报摇钱树成长度(S->G)
    	ID_G2S_UPDATEYQSGROWING_RESPONSE    	= 0xB007;    // 上报摇钱树成长度(G->S)
    	ID_G2S_SETTLE_NOTICE                	= 0xB008;    // 结算通知
    	ID_G2S_YAOQIANSHUSTART_NOTICE      	= 0xB009;    // 摇钱树活动开始通知

    // 家族模块的消息ID 0x3400 + 0x8000	家族模块的消息头
    ID_S2G_REQUEST_FAMILYMSG				= 0xB401;		
    ID_G2S_RESPONSE_FAMILYMSG				= 0xB402;
	ID_G2S_NOTIFY_FAMILYIDNAME				= 0xB403;
	ID_G2S_NOTIFY_NPCLIST					= 0xB404;
	ID_S2G_NOTIFY_BIDTIMEOUT				= 0xB405;
	ID_S2G_REQUEST_BATTLESTART				= 0xB406;
	ID_G2S_RESPONSE_BATTLESTART				= 0xB407;
	ID_S2G_NOTIFY_BATTLEEND					= 0xB408;
	ID_G2S_NOTIFY_BIDRESTART				= 0xB409;  
	
	ID_S2G_NOTIFY_CHANGEFAMILYCONTRIBUTE	= 0xB40A;			// 改变玩家家族贡献度的通知
	ID_S2G_NOTIFY_CHANGEFAMILYGLORY			= 0xB40B;			// 改变家族荣耀的通知
	ID_S2G_NOTIFY_CHANGEFAMILYMONEY			= 0xB40C;			// 改变家族金钱的通知
	
	ID_S2G_NOTIFY_FAMILYAPANDEXPCHANGED		= 0xB40D;			// 家族成员ap和exp改变的通知

	ID_S2G_REQUEST_BUYFAMILYITEM			= 0xB40E;			// 购买家族物品的请求消息
	ID_G2S_RESPONSE_BUYFAMILYITEM			= 0xB40F;			// 购买家族红包的回应消息	
	ID_G2S_NOTIFY_CLEARFAMILYREPETIONIFO	= 0xB410;			// 清除家族副本的信息
	ID_S2G_REQUEST_FAMILYEXCHANGE			= 0xB411;			// 进行家族兑换的消息请求
	ID_G2S_RESPONSE_FAMILYEXCHANGE			= 0xB412;			// 进行家族兑换的消息回应
	ID_S2G_NOTIFY_ADDFAMILYEXCHANGEITEM		= 0xB413;			// 增加家族兑换物品的消息通知
	ID_S2G_NOTIFY_CHANGEFAMILYSTABILITY		= 0xB414;			// 改变家族安定度的通知

	// 军团模块的消息ID 0x3800 + 0x8000

	ID_S2G_REQUEST_BIDTIMERSTOP			= 0xB801;	// 竞价报名时间结束
	ID_G2S_NOTIFY_JOINBATTLE			= 0xB802;	// 参战通知
	ID_S2G_REQUEST_BATTLERESULT			= 0xB803;	// 战场结果
	ID_S2G_REQUEST_BATTLEFLAG			= 0xB804;
	ID_G2S_RESPONSE_BATTLEFLAG			= 0xB805;
	ID_S2G_REQUEST_STARTCOPRSBATTLE		= 0xB806;

	ID_S2G_NOTIFY_CHANGECORPSCONTRIBUTE	= 0xB807;			// 改变玩家军团贡献度的通知
	ID_S2G_NOTIFY_CHANGECORPSGLORY		= 0xB808;			// 改变军团荣耀的通知
	ID_S2G_NOTIFY_CHANGECORPSMONEY		= 0xB809;			// 改变军团金钱的通知
	
	ID_S2G_NOTIFY_CORPSAPEXPCHANGED		= 0xB80A;			// 军团玩家AP和EXP改变的通知
	
	ID_S2G_REQUEST_BUYCORPSITEM			= 0xB80B;			// 购买军团物品的消息请求
	ID_G2S_RESPONSE_BUYCORPSITEM		= 0xB80C;			// 购买军团物品的消息回应
	
	ID_S2G_REQUEST_CORPSEXCHANGE		=  0xB80D;			// 进行军团兑换的消息请求
	ID_G2S_RESPONSE_CORPSEXCHANGE		=  0xB80E;			// 进行军团兑换的消息回应
	
	ID_S2G_REQUEST_ADDCORPSEXCHANGEITEM	= 0xB80F;			// 增加军团兑换物品的消息通知

	ID_S2G_REQUEST_CALLBACK_CORPSMONEY	= 0xB810;			//  回滚军团站相关金钱

	ID_S2G_REQUEST_BATTLEMAP			= 0xB811;		// 战争地图通知
	ID_S2G_CHANGE_CORPSBOSSTIMES		= 0xB812;			// 添加军团boss次数
	ID_S2G_CHANGE_CORPSBOSSREFINEVALUE	= 0xB813;			// 改变军团的boss炼化值
	ID_S2G_CHANGE_CORPSCOLLECTACTIVE	= 0xB814;			// 改变军团采集活跃度

	// 排行榜模块消息ID 0x3C00 + 0x8000
	ID_S2G_NOTIFY_PERSONALRANKINFO		= 0xBC01;	// 发送个人排行信息到网关
	ID_G2S_REQUEST_PERSONALRANKINFO		= 0xBC02;	// 请求发送个人排行信息到网关
	ID_G2S_NOTIFY_NEWTOPTEN				= 0xBC03;	// TopTen 通知场景更新称号
	ID_S2G_REQUEST_TOPTEN				= 0xBC04;
	ID_G2S_RESPONSE_TOPTEN				= 0xBC05;
	ID_S2G_RANK_REMOVE					= 0xBC06;

	// ERATING 商城消息 0x4800 + 0x8000
	ID_G2E_IBEXCHANGE_NOTICE			= 0xC801;
	ID_S2E_IBITEMUSED_NOTICE			= 0xC802;
	ID_S2G_LISTIBITEMS_REQUEST			= 0xC803;	// 商城物品请求
	ID_G2S_LISTIBITEMS_RESPONSE			= 0xC804;	// 商城物品列表回应
	ID_S2G_PURCHASEIBITEM_REQUEST		= 0xC805;	// 购买商城物品请求
	ID_G2S_PURCHASEIBITEM_RESPONSE		= 0xC806;	// 购买商城物品回应
	ID_S2G_ROLEIBTRANS_NOTIFY			= 0xC807;	// 交易完成通知	  	
	
	ID_S2V_SERVERVALIDATE_REQUEST		= 0xC808;	// 服务器验证请求
	ID_V2S_SERVERVALIDATE_RESPONSE		= 0xC809;	// 服务器验证回应  

	ID_S2G_SYNIBSTORE_REQUEST			= 0xC80A;	// 商城物品请求
	ID_G2S_SYNIBSTORE_RESPONSE			= 0xC80B;	// 商城物品列表回应

	ID_S2G_QUERYYUANBAO_REQUEST			= 0xC80C;	// 查询元宝余额
	ID_G2S_QUERYYUANBAO_RESPONSE			= 0xC80D;	// 返回元宝余额
	ID_S2G_DRAWYUANBAO_REQUEST			= 0xC80E;	// 领取元宝请求
	ID_G2S_DRAWYUANBAO_RESPONSE			= 0xC80F;	// 领取元宝回应
	ID_S2G_LOCKGOODS_REQUEST			= 0xC810;	// 申请锁定物品个数请求
	ID_G2S_LOCKGOODS_RESPONSE			= 0xC811;	// 申请锁定物品个数回应	
	ID_G2S_NOTIFY_IBSTOREPROMOTION			= 0xC812;	// 商城动态信息通知
	ID_S2G_SYNFUNCSTORE_REQUEST			= 0xC813;	// 同步功能付费请求
	ID_G2S_SYNFUNCSTORE_RESPONSE		= 0xC814;	// 同步功能付费回应

	// 好感、恶感模块消息ID 0x4C00 + 0x8000
	ID_S2G_REQUEST_CHANGESCORE			= 0xCC01;	// 改变评价分值上限 
	ID_G2S_NOTIFY_JUDGEMODIFY			= 0xCC02;	// 通知场景，评价成功

	// 国家消息 0x5400 + 0x8000
	ID_S2G_REQUEST_CHANGEWIZARD			= 0xD401;	// 改变才华值请求
	ID_G2S_RESPONSE_CHANGEWIZARD		= 0xD402;	// 改变才华值请求

	ID_S2G_REQUEST_CHANGECONSTRUCT		= 0xD403;	// 改变建设度请求
	ID_G2S_REQPONSE_CHANGECONSTRUCT		= 0xD404;	// 改变建设度回应

	ID_S2G_REQUEST_CHANGEPOWER			= 0xD405;	// 改变声望请求
	ID_G2S_RESPONSE_CHANGEPOWER			= 0xD406;	// 改变声望回应

	ID_G2S_NOTIFY_USEFUNCTION			= 0xD407;	// 使用功能通知
	ID_G2S_NOTIFY_SETOFFICIAL			= 0xD408;	// 设定官员通知
	ID_S2G_NOTIFY_CHANGEFRIEND			= 0xD409;	// 改变友好度通知
	ID_S2G_REQUEST_KILLPERSON			= 0xD40A;	// 杀人通知
	ID_G2S_RESPONSE_KILLPERSON			= 0xD40B;	// 
	ID_G2S_NOTIFY_SYNCNATION			= 0xD40C;	// 同步国家关系

	ID_G2S_NOTIFY_NATIONTITLE			= 0xD40D;	// 国家称号通知
	ID_S2G_REQUEST_NATIONTITLELIST		= 0xD40E;	// 获取国家称号列表
	ID_G2S_RESPONSE_NATIONTITLELIST		= 0xD40F;	// 获取国家称号列表

	ID_S2G_REQUEST_TRYSUMMON			= 0xD410;	// 申请召唤请求
	ID_G2S_RESPONSE_TRYSUMMON			= 0xD411;	// 申请召唤回应
	ID_S2G_REQUEST_REALSUMMON			= 0xD412;	// 实际召唤请求
	ID_G2S_NOTIFY_REALSUMMON			= 0xD413;	// 实际召唤通知


	ID_S2G_NOTIFY_WARCHANGE				= 0xD414;	// 国战改变通知
	ID_G2S_NOTIFY_RELIVECHANGE			= 0xD415;	// 国战复活点改变通知
	ID_S2G_NOTIFY_OBTAINWARHONOR		= 0xD416;	// 国战获得荣誉通知
	ID_G2S_NOTIFY_SYNCWARSTATUS			= 0xD417;	// 国战同步战争状态通知
	ID_S2G_NOTIFY_WARCONTROL			= 0xD418;	// 修改国战状态通知
	ID_S2G_NOTIFY_WARSTONEOVER			= 0xD419;	// 内丹结束通知
	ID_G2S_NOTIFY_CREATEWARSTONE		= 0xD41A;	// 创建国战内丹通知

	ID_S2G_NOTIFY_ADDTAX				= 0xD41B;	// 增加税金通知

	// 弱国保护系统相关消息
	ID_S2G_ADDHONER_NOTICE				= 0xD41C;	// 个人荣誉值获取通知
	ID_G2S_CAMPWAEKLIST_NOTICE			= 0xD41D;	// 弱势阵营变化通知
	ID_S2G_CAMPWEAKLIST_REQUEST			= 0xD41E;	// 获取弱势阵营列表请求
	ID_G2S_CAMPWEAKLIST_RESPONSE		= 0xD41F;	// 弱势阵营列表回复

	ID_S2G_NOTIFY_SPECIALEVENT			= 0xD420;	// 特殊事件通知

	// 交易行 0x5800 + 0x8000
	ID_D2G_BOURSELIST_NOTICE            = 0xD801;
	ID_S2G_BOURSELIST_LOAD				= 0xD802;
	ID_G2S_BOURSELIST_LOAD				= 0xD803;
	ID_D2G_BOURSE_TRADE_NOTICE			= 0xD804;
	ID_G2S_BOURSE_TRADE_BUY				= 0xD805;
	ID_G2S_BOURSE_TRADE_SELL			= 0xD806;
	ID_D22_BOURSE_LOG					= 0xD807;

	ID_G2S_NOTIFY_SYNCGTANDLW			= 0xD808;	// 同步国探乱舞信息
	
	// 全局服务器消息 0x8000 + 0x8000


}

enum EMFriendAddRetCode
{
	RET_SUCCESS					= 0x0000;
	RET_INVALID_PLAYER			= 0x0001;	// 没有这个好友
	RET_OFFLINE					= 0x0002;	// 对方没在线
}


// 存盘方式
enum EMSaveResult
{
	EM_SAVE2DB   = 0;
	EM_SAVE2FILE = 1;
	EM_NOSAVE    = 2;
}

// 离开场景的原因
enum EMLeaveReason
{
    EM_LEAVE_REDIRECT = 0x01;
    EM_LEAVE_RETURN2GATE = 0x02;
    EM_LEAVE_LEAVEGAME = 0x03;
    EM_LEAVE_KICKOUT = 0x04;
	EM_LEAVE_CLOSESOCKET = 0x05;
};

// ********************************************************************** //
// CMessageYQSPhaseChangeNotice 网关服务器 -> 场景服务器
// 获取阵营排名请求
// ********************************************************************** //
message CMessageYQSActivtyStartNotice
{
    optional uint32    id                 = 1;     // 活动标示
    optional uint32    campid             = 2;     // 阵营ID
    optional uint32    camprankid         = 3;     // 阵营排名
    optional uint32    growing            = 4;     // 摇钱树成长度
    optional uint32    level              = 5;     // 摇钱树等级
    optional uint32    handindawnum       = 6;     // 摇钱树上缴仙露数
    optional uint32    settlephase        = 7;     // 结算阶段
};

// ********************************************************************** //
// CMessageYQSPhaseChangeNotice 场景服务器 -> 网关服务器
// 获取阵营排名请求
// ********************************************************************** //
message CMessageS2GGetCampRankForYQSRequest
{
    optional uint32  playerId           = 1;        // 玩家编号
    optional uint32  param1             = 2;    // 附属参数

};

// ********************************************************************** //
// CMessageYQSPhaseChangeNotice 网关服务器 -> 场景服务器
// 获取阵营排名应答
// ********************************************************************** //

message CampRankForYQSForSever
{
    optional uint32 rank				= 1;    // 排名
    optional uint32 campid				= 2;    // 阵营ID
    optional uint32 growinggrade		= 3;    // 成长度
    optional uint32 growingphase		= 4;    // 成长阶段
};

message CMessageG2SGetCampRankForYQSResponse
{
    optional uint32  result             = 1;        // 返回结果，1为成功，其他为失败
    optional uint32  playerId        	= 2;        // 玩家编号
    optional uint32  param1             = 3;        // 附属参数
    repeated  CampRankForYQSForSever	camprankinfo = 4; // 阵营排名信息
};

// ********************************************************************** //
// CMessageS2GUpdateYQSGrowingRequest 场景服务器 -> 网关服务器
// 上报摇钱树成长度
// ********************************************************************** //
message CMessageS2GUpdateYQSGrowingRequest
{
     optional uint32 playerid           = 1;        // 用户ID
     optional uint32 campid             = 2;        // 阵营ID
     optional uint32 growingchangevalue = 3;        // 摇钱树成长度
};

// ********************************************************************** //
// CMessageG2SUpdateYQSGrowingResponse 网关服务器 -> 场景服务器
// 上报摇钱树成长度
// ********************************************************************** //
message CMessageG2SUpdateYQSGrowingResponse
{
     optional uint32 result             = 1;        // 返回结果，1为成功，其他为失败
     optional uint32 playerid           = 2;        // 用户ID
     optional uint32 id                 = 3;        // 活动标示    
     optional uint32 campid             = 4;        // 阵营ID
     optional uint32 camprankid         = 5;        // 阵营排名
     optional uint32 growing            = 6;        // 摇钱树成长度
     optional uint32 level              = 7;        // 摇钱树等级
     optional uint32 handindawnum       = 8;        // 摇钱树上缴仙露数
     optional uint32 settlephase        = 9;        // 摇钱树结算阶段
	 optional uint32 deltagrowing		= 10;		// 增加的成长度
};

// ********************************************************************** //
// CMessageG2SSettleNotice 网关服务器 -> 场景服务器
// 活动结算通知
// ********************************************************************** //
message CMessageG2SSettleNotice
{
     optional uint32    id                 = 1;     // 活动标示
     optional uint32    campid             = 2;     // 阵营ID
     optional uint32    camprankid         = 3;     // 阵营排名
     optional uint32    growing            = 4;     // 摇钱树成长度
     optional uint32    level              = 5;     // 摇钱树等级
     optional uint32    handindawnum       = 6;     // 摇钱树上缴仙露数
     optional uint32    settlephase        = 7;     // 结算阶段
     optional uint32    settlefinish       = 8;     // 结算是否结束
     optional uint32    noticeid           = 9;     // 发送消息ID
     optional uint32    noticeparam1       = 10;    // 消息参数
     optional uint32    noticeparam2       = 11;    // 消息参数
     optional uint32    noticeparam3       = 12;    // 消息参数
     optional uint32    encourage          = 13;     // 是否发放奖励
};

// 服务器内部超时消息
message CMessageInternalTimeOut
{
	optional uint32 TimerID		= 1; // 定时器ID
	optional uint32	OwnerID		= 2; // 定时器拥有者实体ID
	optional uint32	TimerMark	= 3; // 定时器标志
	optional uint32 MillSeconds = 4; // 流逝时间，ms
	optional uint32 CalledNum	= 5; // 执行次数
	repeated uint32 OtherInfo	= 6; // 其他信息
}

message CMessageCountryInfo
{
	optional uint32 Length = 1;
	optional bytes  CountryList = 2;
}

message CMessageRedirectGErrorCode
{
	optional uint32 CharID = 1;
	optional int32 ErrorCode = 2;
	optional uint32 LineID = 3;
	optional uint32 MapID = 4;
}

message CMessageRedirectSErrorCode
{
	optional uint32 CharID = 1;
	optional int32 ErrorCode = 2;
	optional uint32 LineID = 3;
	optional uint32 MapID = 4;
}

message CMessagePlayerRedirectEnterMapResponse
{
    optional uint32 CommHandle = 1;  
    optional uint32 CharID = 2; 
    optional uint32 LineID = 3; 
    optional uint32 MapID = 4;  
    optional uint32 PosX = 5;   
    optional uint32 PosY = 6;   
    optional string MapName = 7;
}


// ********************************************************************** //
// CMessageGateInfoNotice 网关服务器 -> 登陆服务器
// ********************************************************************** //

message CMessageGateInfoNotice 
{
    optional string Address = 1;
    optional string HostName = 2;
    optional uint32 GateVersion = 3;
    optional uint32 WorldID = 4;
    optional string GateCode = 5;
    optional string GatePasswd = 6;
    optional string GateMac = 7;
    optional uint32 GameID = 8;
}

// ********************************************************************** //
// CMessageGateInfoResponse 登陆服务器 -> 网关服务器、场景服务器
// ********************************************************************** //

enum  EMGateInfoResponse
{
    ERROR_SUCCESS = 0;
    ERROR_OUTOFDATE = 1;
}

message CMessageGateInfoResponse 
{
    optional int32 ErrorCode = 1;
}	

// ********************************************************************** //
// CMessageUserLoginNotice 登陆服务器 -> 网关服务器
// 玩家进入服务器通知
// ********************************************************************** //

message CMessagePlayerEnterGateNotice 
{
    optional uint32 CommHandle = 1;
    optional uint32 LoginToken = 2;
    optional uint32 AccountID = 3;
    optional uint32 AdultFlag = 4;      // 成人标志， 1成人, 0 非成年人
    optional uint32 UserFlag =  5;      // 跨区控制的标记
    optional uint32 UserClass = 6;      // vip 级别
    optional uint32 UserPoint = 7;      // 玩家积分
	optional uint32 WorldPlayerLimit = 8; // 登录服务器的人数限制
	optional uint32 UserStatus	 = 9; // 用户状态 比如GM等
	optional uint32 GameID		= 10; // 玩家从哪里来的
	optional int32  cpuid		= 11;	// cpu 序列
	optional int32  diskid		= 12;	// disk 序列
}

// ********************************************************************** //
// CMessageUserLoginResponse 网关服务器 -> 登陆服务器
// 玩家进入服务器回应
// ********************************************************************** //

message CMessagePlayerEnterGateResponse
{
    optional uint32 CommHandle = 1;
    optional uint32 AccountID = 2;
    optional uint32 LoginToken = 3;
    optional int32  ErrorCode = 4; 
    optional int32  WorldID = 5;
    optional uint32 OtherClient = 6; 
}

// ********************************************************************** //
// CMessageUserLeaveNotice 网关服务器 -> 登陆服务器
// 玩家离开服务器通知
// ********************************************************************** //

message CMessagePlayerLeaveGateNotice 
{
    optional uint32 AccountID = 1;
    optional uint32 Reason = 2;
}

// ********************************************************************** //
// CMessageServerInfoNotice 场景服务器 -> 网关服务器
// 场景服务器向网关报告场景模版
// ********************************************************************** //

message CMessageServerInfoNotice 
{
    optional string Address = 1;		// 场景服务器地址
    optional string ServerName = 2;		// 场景服务器名称
    optional uint32 ServerVersion = 3;		// 场景服务器版本号
    optional uint32 TemplateVersion = 4;        // 模板文件版本号
    optional uint32 TaskVersion = 5;		// 任务文件版本号
    optional uint32 ServerID = 6;		// 服务器ID
    optional uint32 LaunchMode = 7;             // 服务器的启动模式, LAUNCH_INITIAL LAUNCH_RECOVERY
    optional uint32 PlayerCapacity = 8;         // 服务器玩家的容量
}

// ********************************************************************** //
// CMessageServerInfoResponse 网关服务器 -> 场景服务器
// ********************************************************************** //

message CMessageServerInfoResponse 
{
    optional uint32 WorldID = 1;        // 世界ID，全局唯一, 一般为1XX
	optional uint32 LineID = 2;			// 服务器LineID
	optional uint32 GameID = 3;			// 游戏ID
}

// ********************************************************************** //
// CMessagePlayerEnterMapNotice　网关服务器 -> 场景服务器
// ********************************************************************** //
message CMessagePlayerEnterMapNotice 
{
    optional uint32 LoginToken = 1;		// 登陆令牌
    optional uint32 LineID = 2;			// 线ID
    optional uint32 MapID = 3;			// 地图ID
    optional uint32 PosX = 4;			// 横坐标
    optional uint32 PosY = 5;			// 纵坐标
    optional uint32 CharID = 6;			// 玩家角色ID
    optional uint32 AccountID = 7;		// 账号ID
    optional uint32 AdultFlag = 8;		// 成年人标志 1: 成年人 0: 非成年人
    optional int32	ValidSeconds = 9;	// 防沉迷系统剩余的有效益秒数
    optional uint32 FamilyID = 10;		// 家族ID
    optional string FamilyName = 11;	// 家族名称
    optional uint32	IsFamilyHeader = 12;// 是否家族族长	1表示是族长 0表示不是
    optional uint32 IfHasApplies   = 13;// 是否含有申请信息 1表示有 0表示没有
    optional uint32 CorpsID		= 14;	// 军团ID
    optional string CorpsName	= 15;	// 军团名称
    optional uint32 IsCorperHeader = 16;// 是否军团首领 
    optional uint32 IfHasCorpsApply	= 17;// 是否含有申请信息 
    optional uint32 IfTracedPlayer = 18; // 是否是要跟踪的玩家	
	optional uint64 Key	= 19;			 // 地图Key
	optional uint32	FamilyHeaderID	= 20;// 家族首领角色ID
	optional uint32 CorpsHeaderID	= 21;// 军团首领角色ID
	optional uint32	FamilyPost		= 22;// 家族职位
	optional uint32	CorpsPost		= 23;// 军团职位
	optional uint32 FamilyContribute= 24;// 家族贡献度
	optional uint32	CorpsContribute = 25;// 军团贡献度  
	optional uint32	FamilyMoney		= 26;// 家族金钱
	optional uint32	CorpsMoney		= 27;// 军团金钱	
	optional uint32 UnionID			= 28;
	optional string UnionName		= 29;
	optional uint32 HeadCorpsID		= 30;
	optional uint32 CountryID		= 31;// 国籍
	optional uint32	OfficialID		= 32;// 官职
	optional uint32 KingID			= 33;// 国王ID
	optional string	Password		= 34;// 安全密码
	optional uint32	ProtectTime		= 35;// 安全保护的到期时间	
	optional uint32	PasswordStatus	= 36;// 密码保护的状态
	optional uint32	ProtectTimeLength = 37;	// 密码保护的总时长
	optional uint32	RoleWorldID		= 38;	// 玩家的区组ID
}

message CMessagePlayerRedirectMapNotice
{
	optional uint32 CommHandle = 1;
	optional uint32 LoginToken = 2;
	optional uint32 CharID = 3;
	optional uint32 AccountID = 4;
	optional uint32	LineID = 5;
	optional uint32 MapID = 6;
	optional uint32 PosX = 7;
	optional uint32 PosY = 8;

    optional string CharName    = 9;
    optional uint32 Gender      = 10;
    optional uint32 Metier      = 11;
    optional uint32 Nationality = 12;
    optional uint32 Facetype    = 13;
    optional uint32 Charlevel   = 14;
    optional uint64 Exp         = 15;
    optional uint32 Lastlogin   = 16;
    optional uint32 DataLength  = 17;
    optional bytes  Data        = 18;
	optional uint32 TeamFlag	= 19;
	optional uint64 Key			= 20;
	optional bytes	EncryptKey	= 21; // 上个服务器的加密密钥， 16位

	optional uint32 PetTempID		= 22;
	optional uint32	PetOnlineTime	= 23;
	optional uint32 PetMode			= 24;

	optional PBTeamInfo TeamData	= 25;
	optional PBMsg		TeamMsg		= 26;
	
	optional uint32 FamilyID = 27;		// 家族ID
	optional string FamilyName = 28;	// 家族名称
	optional uint32	IsFamilyHeader = 29;// 是否家族族长	1表示是族长 0表示不是
	optional uint32 IfHasApplies   = 30;// 是否含有申请信息 1表示有 0表示没有
	optional uint32 CorpsID		= 31;	// 军团ID
	optional string CorpsName	= 32;	// 军团名称
	optional uint32 IsCorperHeader = 33;// 是否军团首领 
	optional uint32 IfHasCorpsApply	= 34;// 是否含有申请信息 	

	optional uint32	Camp		= 35;	// 战场:阵营
	optional uint32 IfTracedPlayer = 36;// 是否是要跟踪的玩家
	optional uint32	FamilyHeaderID	= 37;// 家族首领角色ID
	optional uint32 CorpsHeaderID	= 38;// 军团首领角色ID			

    optional int32	ValidSeconds = 39;	// 防沉迷系统剩余的有效益秒数
    optional uint32 AdultFlag = 40;		// 成年人标志 1: 成年人 0: 非成年人
	optional uint32	FamilyPost	= 41;	// 家族职位
	optional uint32	CorpsPost	= 42;	// 军团职位
	optional uint32 FamilyContribute= 43;// 家族贡献度
	optional uint32	CorpsContribute = 44;// 军团贡献度  
	optional uint32	FamilyMoney		= 45;// 家族金钱
	optional uint32	CorpsMoney		= 46;// 军团金钱
	optional uint32 UnionID			= 47;
	optional string UnionName		= 48;
	optional uint32 HeadCorpsID		= 49;
	optional uint32	CountryID		= 50;// 国籍
	optional uint32	OfficialID		= 51;// 官职
	optional uint32 KingID			= 52;// 国王ID		  	
	optional string	Password		= 53;// 安全密码
	optional uint32	ProtectTime		= 54;// 安全保护的到期时间
	optional uint32	PasswordStatus	= 55;// 密码保护状态
	optional uint32	ProtectTimeLength = 56;	// 密码保护的总时长
	optional uint32 RoleWorldID		= 57; // 玩家的世界ID
}

// ********************************************************************** //
// CMessagePlayerEnterMapResponse　场景服务器 -> 网关服务器
// ********************************************************************** //
message CMessagePlayerEnterMapResponse 
{
    optional uint32 AccountID = 1;     // 通讯端口ID
    optional uint32 CharID = 2;	// 角色ID
    optional uint32 LineID = 3;	// 线ID
    optional uint32 MapID = 4;	// 地图ID
    optional uint32 PosX = 5;	// 横坐标
    optional uint32 PosY = 6;	// 纵坐标
    optional int32 ErrorCode = 7;       // 0 成功, 其它失败	
}

// ********************************************************************** //
// CMessageUserLoginCompleteNotice　网关服务器 -> 场景服务器
// ********************************************************************** //
message CMessagePlayerLoginCompleteNotice 
{
    optional uint32 AccountID = 1; // 账号ID
}


// ********************************************************************** //
// CMessagePlayerLeaveMapNotice　场景服务器 -> 网关服务器
// ********************************************************************** //

message CMessagePlayerLeaveMapNotice 
{
    optional uint32 CharID = 1; // 角色ID
    optional uint32 AccountID = 2; // 帐号ID
    optional EMLeaveReason Reason = 3; // 离开原因
    optional uint32 CharLevel = 4; // 角色级别
    optional uint32 CharMetier = 5; // 角色职业
    optional uint32 Money1 = 6; // 非绑定金币
    optional uint32 Money2 = 7; // 绑定金币
    optional uint64 Exp = 8; // 当前经验值
    optional uint32 LogoutFlag = 9; // Erating使用
    optional EMSaveResult SaveFlag = 10;  // 下线存盘标志
	optional uint32	GlobalSceneWorldID = 11; // 大于0时表示跨服时目标服务器的世界Id
}

// ********************************************************************** //
// CMessagePlayerLeaveMapResponse　网关服务器 -> 场景服务器
// ********************************************************************** //

message CMessagePlayerLeaveMapResponse 
{
    optional uint32 CharID = 1; // 角色ID
}

// ********************************************************************** //
// CMessagePlayerReturnToGateNotice　场景服务器 -> 网关服务器
// 场景服务器通知路由服务器,玩家将要返回网关
// ********************************************************************** //

message CMessagePlayerReturn2GateNotice 
{
    optional uint32 AccountID = 1; // 账号ID
    optional uint32 CharID = 2;	   // 角色ID

}

// ********************************************************************** //
// CMessagePlayerReturnToGateResponse　网关服务器 -> 场景服务器
// 路由服务器通知场景服务器,已经确认玩家返回,正在等待玩家
// ********************************************************************** //

message CMessagePlayerReturn2GateResponse 
{
    optional uint32 CharID = 1; // 角色ID
}

// ********************************************************************** //
// CMessageCreateMapNotice　网关服务器 -> 场景服务器
// 建立地图通知
// ********************************************************************** //

message CMessageCreateMapNotice  
{
    /*optional uint32 LineID = 1;
    optional uint32 MapID = 2;
	optional uint32 Count = 3;
    optional string TplName = 4;*/
	repeated uint32 LineID = 1;
	repeated uint32 MapID = 2;	
	repeated string TplName = 3;
	optional uint32 Count = 4;
	optional uint32	RtnCode	= 5;
	optional string Notice = 6;
}

// ********************************************************************** //
// CMessageCreateMapResponse　场景服务器 -> 网关服务器
// 建立地图通知
// ********************************************************************** //

message CMessageCreateMapResponse 
{
    repeated uint32 LineID = 1;
    repeated uint32 MapID = 2;
    repeated string TplName = 3;
}
// ********************************************************************** //
// CMessageRegistRepetionInfo 场景服务器 -> 网关服务器
// 副本地图信息
// ********************************************************************** //
message BattleLevel
{
	optional uint32	LevelMin		= 1;
	optional uint32	LevelMax		= 2;
};

message PBDate
{
	optional uint32 Year			= 1;
	optional uint32 Month			= 2;
	optional uint32 Day				= 3;
	optional uint32	Week			= 4;
	optional uint32 Hr				= 5;
	optional uint32 Minute			= 6;
	optional uint32 Sec				= 7;
};

message PBDateSpace
{
	optional PBDate Start				= 1;
	optional PBDate finish 			= 2;
};

message CMessageRegistRepetionInfo
{
	optional uint32 LineID = 1;
	repeated uint32 MapID = 2;
	optional uint32 PlayerMax = 3;
	optional uint32 BackMapID = 4;
	optional uint32 BackPosX = 5;
	optional uint32 BackPosY = 6;
	optional uint32 CountMax = 7;
	optional uint32 RepetionID = 8;

	repeated BattleLevel	Level = 9;
	repeated PBDateSpace		Space = 10;
	optional uint32 IsOnTimeOpen = 11;         
	optional uint32 NotLimitTime = 12;
	optional uint32	Relogin	= 13;	// 是否可以重入
}

message CMessageDestroyRepetionMap
{
	optional uint32 RepetionIndex = 1;
	optional uint64 Key = 2;
	optional uint32 Parame = 3;
}

message CMessageG2SRepetionEventCode
{
	optional int32 RoleID			= 1;
	optional int32 Code				= 2;
	optional int32 RepetionID		= 3;
	optional int32 Parame			= 4;
}

// ********************************************************************** //
// CMessagePlayerRedirectRequest　场景服务器 -> 网关服务器
// 玩家切换场景通知
// ********************************************************************** //
message CMessagePlayerRedirectRequest 
{
    optional uint32 LineID		= 1;
    optional uint32 MapID		= 2;
    optional uint32 PosX		= 3;
    optional uint32 PosY		= 4;
    optional uint32 AccountID	= 5;
    optional uint32 CharID		= 6;

	optional string CharName		= 7;
	optional uint32 Gender			= 8;
	optional uint32 Metier			= 9;
	optional uint32 Nationality		= 10;
	optional uint32 Facetype		= 11;
	optional uint32 Charlevel		= 12;
	optional uint64 Exp				= 13;
	optional uint32 Lastlogin		= 14;
	optional uint32 DataLength		= 16;
	optional bytes  Data			= 17;
	optional uint32 TeamFlag		= 18;
	optional uint32	TeamLevel		= 19;
	optional bytes	EncryptKey		= 20;		// 密钥 , 16位
	optional PBTeamInfo TeamInfo	= 21;		// 队伍信息

	optional uint32	PetTempID			= 22;		// 当前召唤兽模板ID(跨服或副本传送使用)
	optional uint32	PetOnlineTime		= 23;		// 召唤兽在线时间
	optional uint32 PetMode				= 24;		// 召唤兽攻击模式
}

// ********************************************************************** //
// CMessagePlayerRedirectResponse　网关服务器 -> 场景服务器
// 玩家切换场景回应
// ********************************************************************** //
message CMessagePlayerRedirectResponse 
{
    optional uint32 CharID = 1;
	optional bool	Redirect = 2;
    optional uint32 LineID = 3;
    optional uint32 MapID = 4;
    optional uint32 PosX = 5;
    optional uint32 PosY = 6;
    optional string Address = 7;
}

// ********************************************************************** //
// CMessagePlayerEnterConfirmNotice　场景服务器 -> 网关服务器
// 玩家进入场景完成确认
// ********************************************************************** //
message CMessagePlayerEnterConfirmNotice 
{
    optional uint32 AccountID = 1;
    optional uint32 CharID = 2;
    optional string CharName = 3;
    optional uint32 LineID = 4;
    optional uint32 MapID = 5;
	// 玩家进入场景返回，携带玩家的face，metier, level, sex, nationality
	optional uint32	Face = 6;
	optional uint32	Metier = 7;
	optional uint32	Level = 8;
	optional uint32	Sex = 9;
	optional uint32	Nationality = 10;
    optional bytes  CSKey = 11;         // 服务器与客户端通讯使用的加密key
	optional uint32	VipFlag = 12;		// 会员标志
}

// ********************************************************************** //
// CMessageSceneNotOpenNotice　网关服务器 -> 场景服务器
// 玩家跨场景时,目标场景没有找到
// ********************************************************************** //
message CMessageSceneNotOpenNotice 
{
    optional uint32 CharID = 1;
}

// ********************************************************************** //
// CMessageUpdateGateInfoNotice 网关服务器 -> 登陆服务器
// ********************************************************************** //

message CMessageUpdateVersionNotice 
{
    optional uint32 TemplateVersion = 1;// 模板版本号
    optional uint32 TaskVersion = 2;	// 任务版本号
}

// ********************************************************************** //
// CMessageRegistServerResponse 注册请求 其他服务器 -> 中转服务器
// ********************************************************************** //

message CMessageRegistServerRequest
{
	optional uint32 ServerType	= 1;	// 服务器类型
	optional uint32 ServerID	= 2;	// 服务器ID
}

// ********************************************************************** //
// CMessageRegistServerResponse 注册回应 中转服务器 -> 其他服务器
// ********************************************************************** //
message CMessageRegistServerResponse
{
	optional uint32 ServerType	= 1;	// 服务器类型
	optional uint32 ServerID	= 2;	// 服务器ID
}

// ********************************************************************** //
// CMessageHeartBeatNotice 中转服务器 -> 其他服务器
// ********************************************************************** //
message CMessageHeartBeatNotice 
{
	optional uint32 ServerType	= 1;	// 服务器类型
	optional uint32 ServerID	= 2;	// 服务器ID
}

// ********************************************************************** //
// CMessageGetPlayerInfoRequest 场景服务器 -> 网关服务器
// 获取玩家名字和状态
// ********************************************************************** //
message CMessagePlayerOnLineFriendRequest
{
	repeated uint32 CharIDs		= 1;	// 角色ID表
	optional uint32 Requester	= 2;	// 请求者的char id，回传消息的时候要发回来
	optional uint32 Flag		= 3;	// 标识，是好友？仇敌？还是。。。
}

message PBSinglePlayerInfo
{
	optional uint32 CharID		= 1;	// 角色ID
	optional uint32 Status		= 2;	// 状态
	optional string Name		= 3;	// 名字
	optional uint32 Sex			= 4;	// 性别
	optional uint32 Face		= 5;	// 头像
	optional uint32 Nation		= 6;	// 国家
	optional uint32	VipFlag		= 7;	// 0:非会员, 1:普通会员
	optional uint32	noExist		= 8;	// 是否存在  0:不存在   1:存在
};
// ********************************************************************** //
// CMessageGetPlayerInfoNotice   网关服务器 -> 场景服务器
// 服务器返回玩家名字和状态请求
// ********************************************************************** //
message CMessagePlayerOnLineFriendResponse
{
	repeated PBSinglePlayerInfo Friends = 1; // 好友信息
	optional uint32 Requester			= 2;	// 请求者的char id，回传消息的时候要发回来
	optional uint32 Flag				= 3;	// 标识，见enGateFlags
}

// ********************************************************************** //
// CMessageChangeStatusRequest   场景服务器 -> 网关服务器
// 修改状态请求
// ********************************************************************** //
message CMessagePlayerChangeStatusNotice
{
	optional uint32 CharID		= 1;	// 角色ID
	optional uint32 Status		= 2;	// 新的状态
}

// ********************************************************************** //
// CMessageGameHeartBeatNotice 游戏服务器 -> 网关服务器
// ********************************************************************** //
message CMessageGameHeartBeatNotice 
{
	optional uint32 ServerID		= 1;	// 服务器ID
	repeated uint32	CharIDs			= 2;	// 在线玩家char id	
	repeated uint32	ExternalCharIDs	= 3;	// 其他区组在服务器的玩家ID列表
}

// ********************************************************************** //
// CMessageERatingProto 游戏服务器 -> 网关服务器
// ********************************************************************** //
message CMessageERatingProtoStream
{
	optional uint32 Length	= 1;	// 流长度
	optional bytes  buffer	= 2;	// 流内容
}

// ********************************************************************** //
// CMessageERatingStateNotice 游戏服务器 -> 网关服务器
// ********************************************************************** //
message CMessageERatingStateNotice
{
        optional uint32 GatewayID = 1;
        optional uint32 binded = 2;
}

// ********************************************************************** //
// CMessageCreateGateTeamRequest 游戏服务器 -> 网关服务器
// ********************************************************************** //

message CMessageCreateGateTeamRequest
{
	optional uint32	CharID = 1;	// 自建队伍申请的玩家
};

message CMessageCreateGateTeamResponse
{
	optional uint32	CharID = 1;
	optional uint32	TeamID = 2;
};

message CMessageJoinGateTeamRequest
{
	optional uint32	SrcCharID = 1; // 申请者
	optional uint32	DesCharID = 2; // 队长、被申请的玩家
	optional uint32	SrcNation = 3;
	optional uint32	DesNaiton = 4;	// 对方国籍
};

message CMessageJoinGateTeamResponse
{
	optional uint32	DesCharID = 1;	// 发送对象,队长
	optional PBTeamItem Item  = 2;	// 队员信息
};

message CMessageRequestJoinReply
{
	optional uint32	SrcCharID = 1; // 队长、具有同意申请权限的玩家
	optional uint32	DesCharID = 2; // 申请人
	optional uint32	RtnCode	  = 3;	// 返回结果：同意、拒绝
};

message CMessageJoinGateTeamInvite
{
	optional uint32	SrcCharID = 1; // 邀请人、有队伍的玩家
	optional uint32	DesCharID = 2; // 邀请对象、无队伍玩家
	optional uint32	SrcNation = 3;
	optional uint32	DesNaiton = 4;	// 对方国籍
};

message CMessageJoinTeamInviteResponse
{
	optional uint32 DesCharID = 1;	// 发送对象，被邀请者
	optional uint32 TeamID = 2;
	optional uint32	SrcCharID = 3;
	optional string SrcName = 4;	// 邀请者姓名
	optional string HeadName = 5;
	optional PBTeamInfo TeamInfo = 6;
};

message CMessageInviteJoinReply
{
	optional uint32	SrcCharID = 1;	// 收到邀请的玩家
	optional uint32	DesCharID = 2;	// 邀请者
	optional uint32	TeamID = 3;
	optional uint32	RtnCode	  = 4;	// 返回结果：同意、拒绝
};

message CMessageInvite2Request
{
	optional PBTeamItem SrcItem = 1;
	optional uint32	DesCharID = 2;
	optional string InviterName = 3;
	optional uint32	InviterCharID = 4;
};

message CMessageKickoutGateTeamRequest
{
	optional uint32	HeadCharID = 1;	// 队长
	optional uint32	MemberCharID = 2; // 被踢的队员
};

message CMessageChangeHeaderRequest
{
	optional uint32	OldHeader = 1;
	optional uint32	NewHeader = 2;
};

message CMessageChangeDistributeRequest
{
	optional uint32	CharID	  = 1;
	optional uint32	Mode	  = 2;
};

message CMessageChangeDistributeResponse
{
	optional uint32	CharID	 = 1;
	optional uint32	Mode	 = 2;
};

message CMessageLeaveGateTeamRequest
{
	optional uint32	CharID = 1;
};

message CMessageEndGateTeamRequest
{
	optional uint32	CharID = 1;
};

message CMessageNotifySceneToUpdateTeam
{
	optional uint32	Code	= 1;	// 更新类型:加人、减人、解散队伍、转移队长
	optional uint32 DesCharID = 2;
	optional uint32	HeadID = 3;
	optional PBTeamItem Item = 4;
};

message CMessageNotifySceneToSendTeamList
{
	optional uint32	DesCharID = 1;	// 列表发送对象
	optional PBTeamInfo TeamInfo = 2;
};

message CMessageNotifyErrToScene
{
	optional uint32 DesCharID = 1;
	optional uint32	Code	= 2;
	optional string Param1	= 3;
	optional string Param2	= 4;
};

message CMessageUpdateTeamPropertyRequest
{
	optional uint32 CharID = 1;
	optional PBTeamInfo Info = 2;
};

message CMessageUpdateTeamPropertyResponse
{
	optional uint32 CharID = 1;
	optional PBTeamInfo Info = 2;
};

message CMessageFollowRequest
{
	optional uint32	CharID = 1;
	optional uint32	ActionType = 2;
	optional uint32	ResultCode = 3;
};

message CMessageFollowResponse
{
	optional uint32	CharID = 1;
	optional uint32	HeadID = 2;
	optional uint32	ActionType = 3;
	optional uint32	ResultCode = 4;
};

message CMessageSendClaimRecruitRequest
{
	optional uint32	CharID = 1;
	optional PBMsg	Msg	= 2;
	optional uint32 ActionType = 3;
};

message CMessageSendClaimRecruitResponse
{
	optional uint32	CharID = 1;
	optional PBMsg	Msg = 2;
	optional uint32	ActionType = 3;
};

message CMessageQueryClaimRecruitRequest
{
	optional uint32 CharID = 1;
	optional uint32	ActionType = 2;
	optional uint32	RootCondition = 3;
	optional uint32	SubCondition = 4;
	optional uint32	Level = 5;
	optional uint32	Metier = 6;
	optional uint32	MinLevel = 7;
	optional uint32	MaxLevel = 8;
	optional uint32	Nationality = 9;
	optional uint32	PageIndex = 10;
};

message CMessageQueryClaimRecruitResponse
{
	optional uint32	CharID = 1;
	repeated PBMsg	Msg = 2;
	optional uint32	PageIndex = 3;
	optional uint32 TotalIndex = 4;
	optional uint32	ActionType = 5;
};

message CMessageDeleteClaimRecruitRequest
{
	optional uint32	CharID = 1;
	optional uint32	ActionType = 2;
};

message CMessageDeleteClaimRecruitResponse
{
	optional uint32 CharID = 1;
	optional uint32	ActionType =2;
};

// ********************************************************************** //
// CMessageObtainGateTeamRequest 游戏服务器 -> 网关服务器
// ********************************************************************** //

message CMessageObtainGateTeamRequest
{
	optional uint32 CharID		= 1;
};

message CMessageClearInvReqRecordRequest
{
	optional uint32	Type		= 1;	// req or inv
	optional uint32 CharID		= 2;
};

//
//// ********************************************************************** //
//// CMessageObtainGateTeamResponse 网关服务器 -> 游戏服务器 
//// ********************************************************************** //
//
//message CMessageObtainGateTeamResponse
//{
//	optional uint32 HeadCharID	 = 1;
//	optional string Name		 = 2;
//	optional uint32 AccountID	 = 3;
//	optional uint32 Mode		 = 4;
//	optional uint32 Count		 = 5;
//	repeated PBTeamItem Item	 = 6;
//	optional uint32	ItemCharID	 = 7;	// 队伍数据发送目标
//};
//
//// ********************************************************************** //
//// CMessageUpdateGateTeamRequest 游戏服务器 -> 网关服务器
//// ********************************************************************** //
//
//message CMessageUpdateGateTeamRequest
//{
//	optional uint32 HeadCharID	 = 1;
//	optional string HeadName	 = 2;
//	optional uint32 Type		 = 3;
//	optional uint32 ItemCharID	 = 4;
//	optional string ItemName	 = 5;
//	optional PBTeamItem Item	 = 6;
//};
//
//message CMessageUpdateItemProRequest
//{
//	optional uint32	HeadCharID	= 1;
//	repeated PBTeamItem ItemPro = 2;
//};
//
//// ********************************************************************** //
//// CMessageUpdateGateTeamResponse 网关服务器 -> 游戏服务器 
//// ********************************************************************** //
//
//message CMessageUpdateGateTeamResponse
//{
//	optional uint32 HeadCharID	  = 1;
//	optional string HeadName	  = 2;
//	optional uint32 ItemCharID	  = 3;
//	optional string ItemName	  = 4;
//	optional uint32 ItemAccID	  = 5;
//	optional uint32 Type		  = 6;
//	optional PBTeamItem Item	  = 7;
//};
//
////**************************************************
//// CMessageProduceCRMsg
//// 产生一条求组、招募信息
////**************************************************
//
//message CMessageProduceCRMsg
//{
//	optional PBMsg	Msg = 1;
//	optional uint32 IsAdd = 2;	// true == 增加， false == 删除
//	optional uint32	CharID = 3;	// 主角
//};
//
//// ********************************************************************** //
//// CMessageObtainAllCRMsgRequest 游戏服务器 -> 网关服务器
//// 获得所有在线玩家的求组招募信息
//// ********************************************************************** //
//message CMessageObtainAllCRMsgRequest
//{
//	optional uint32 CharID		  = 1;	// 请求的玩家
//	optional uint32 Type		  = 2;	// 类型，1--求组信息，2--招募信息
//	optional uint32 RootStyle	  = 3;	// 一级类型 地宫、副本
//	optional uint32 SubStyle	  = 4;	// 二级类型
//	optional uint32 Metier		  = 5;	// 职业
//	optional uint32 Level		  = 6;	// 
//	optional uint32 MaxLevel	  = 7;
//	optional uint32 MinLevel	  = 8;
//	optional uint32	PageIndex	  = 9;	// 分页号
//	optional uint32	Nationality	  = 10;	// 国籍
//};
//
//// ********************************************************************** //
//// CMessageObtainCRmsgNotify  网关服务器 -> 游戏服务器 
//// 通知游戏服务器，获取求组招募信息
//// ********************************************************************** //
//message CMessageObtainCRMsgNotify
//{
//	repeated PBMsg	Msg = 1;
//	optional uint32	MsgSize = 2;	
//	optional uint32 CharID = 3;
//	optional uint32 Type = 4;
//	optional uint32 PageIndex = 5;
//	optional uint32 TotalPage = 6;
//};

message PBMsg
{
	optional uint32		Type = 1;		// 求组 == TEAM_ASKFOR？招募 == TEAM_CONSCRIBE
	optional uint32		CharID = 2;
	optional string		Name = 3;
	optional uint32		Metier = 4;
	optional uint32		Level = 5;
	optional uint32		MaxLevel = 6;
	optional uint32		MinLevel = 7;
	optional string		Content = 8;
	optional uint32		Count = 9;
	optional uint32		RootStyle = 10;  // 一级条件
	optional uint32		SubStyle = 11;	 // 二级条件
	optional uint32		Nationality = 12;	// 国籍 
};

// ********************************************************************** //
// 定义队员信息 
// ********************************************************************** //
message PBTeamItem
{
	optional uint32			CharID		= 1;
	optional string			Name		= 2;
	optional uint32			AccountID	= 3;
	optional uint32			Type		= 4;
	optional uint32			Face		= 5;
	optional uint32			Metier		= 6;
	optional uint32			Level		= 7;
	optional uint32			Sex			= 8;
	optional uint32			HP			= 9;
	optional uint32			MP			= 10;
	optional uint32			AP			= 11;
	optional uint32			PosX		= 12;	
	optional uint32			PosY		= 13;	
	optional uint32			LineID		= 14;	
	optional uint32			MapID		= 15;
	optional uint32			Status		= 16;
	optional uint32			Nationality	= 17;
	optional PBBuffList		BuffList	= 18;
	optional uint32			RepetionMapIndex = 19;
	optional uint32			Order		= 20; // 在队伍中的序号
};



message PBTeamInfo
{
	optional uint32 HeaderCharID		= 1;
	optional uint32 DistributeMode		= 2;
	optional string HeadName			= 3;
	optional uint32	Count				= 4;
	optional uint32	TeamID				= 5;
	repeated PBTeamItem Members			= 6;
};



//message PBMemberPro
//{
//	optional uint32			CharID		= 1;
//	optional uint32			HP			= 2;
//	optional uint32			MP			= 3;
//	optional uint32			PosX		= 4;	
//	optional uint32			PosY		= 5;	
//	optional uint32			LineID		= 6;	
//	optional uint32			MapID		= 7;
//	optional uint32			Status		= 8;
//};


// ********************************************************************** //
// CMessageGateShutDownNotice 网关服务器 -> 登录服务器 
// 通知网关服务器停止
// ********************************************************************** //
message CMessageGateShutDownNotice
{
        optional uint32 WorldID = 1;
};

// ********************************************************************** //
// CMessageKickOffPlayerRequest网关服务器 -> 游戏服务器
// 踢掉玩家请求
// ********************************************************************** //
message CMessageKickOffPlayerRequest
{
    optional uint32 AccountID	= 1;
    optional uint32 CharID		= 2;
    optional int32  Reason		= 3;
    optional uint32 AnotherIP	= 4; 	//only for relogin reason.
	optional uint32	RoleWorldID = 5;	// 玩家所在区组世界ID
};

// ********************************************************************** //
// CMessageKickOffPlayerResponse  游戏服务器 -> 网关服务器
// 踢掉玩家请求回应
// ********************************************************************** //
message CMessageKickOffPlayerResponse
{
    optional uint32 AccountID = 1;
    optional uint32 CharID = 2;
    optional int32 Result = 3;  // 0 成功, 其它失败
	optional uint32 RoleWorldID = 4; // 玩家所在区组世界ID
};

// ********************************************************************** //
// CMessageGetPlayerMailiInfo  游戏服务器 -> 网关服务器
// 获取玩家邮件相关信息
// ********************************************************************** //
message CMessageGetPlayerMailiInfoRequest
{
	optional int32  CharID		= 1;		 // 哪个玩家的消息
	optional string RoleName	= 2;		 // 待查询的rolenames 	
	optional uint32 RoleID		= 3;		 // 待查的玩家角色id	
	optional int32  QueryWay	= 4;		 // 查询方式：1.使用CharID 2.使用RoleName
	optional uint32	QueryType	= 5;		 // 查询种类1发送邮件时查询2.婚姻系统的查询
	repeated uint32	QueryData	= 6;		 // 查询需要返回的数据
};

// ********************************************************************** //
// CMessageGetPlayerMailiInfo  游戏服务器 -> 网关服务器
// 获取玩家邮件相关信息
// ********************************************************************** //
message CMessageGetPlayerMailInfoResponse
{
	optional int32  CharID			= 1;   // 原消息中的entityID
	optional int32  ErrorCode		= 2;   // 1 玩家存在, 2 玩家不存在
	optional uint32 RoleID			= 3;   // 玩家角色ID
	optional string RoleName		= 4;   // 玩家角色名
	optional int32  RoleOfCountry	= 5;   // 玩家国籍
	optional int32  RoleInCountry	= 6;   // 玩家现在所在的国家 
	optional int32  RoleOnline		= 7;   // 玩家是否在线
	optional int32  RoleMailCount	= 8;   // 玩家邮件数量
	optional uint32	QueryType		= 9;   // 查询种类1发送邮件时查询2.婚姻系统的查询
	repeated uint32	QueryData		= 10;  // 查询需要返回的数据
};
// ********************************************************************** //
// CMessageGetPlayerMailiInfo  游戏服务器 -> 网关服务器
// 获取玩家邮件相关信息
// ********************************************************************** //
message CMessageChangeMailCountNotify
{
	optional int32  CharID		= 1;		// 玩家id
	optional string CharName	= 2;		// 玩家姓名
	optional int32  QueryWay	= 3;		// 检索方式：1.用角色ID 2. 用角色名
	optional int32  NumChanged	= 4;		// 变化的数量
};	

message CMessageUserIbExchangeNotice
{
    optional uint64 PurchaseID = 1;
    optional uint32 AccountID = 2;
    optional uint32 RoleID = 3;
    optional uint32 GenderID = 4;
    optional uint32 MetierID = 5;
    optional uint32 Level = 6;
    optional uint32 RatingID = 7;
    optional string IBCode = 8;
    optional uint32 PackageFlag = 9;
    optional uint32 Count = 10;
    optional int32 PayTime = 11;
    optional uint32 UserIP = 12;
    optional uint32 SubjectID = 13;
    optional uint32 AuditFlag = 14;
    optional int32 Price = 15;
    optional int32 DiscountPrice = 16;
};

message CMessageUserUseIbItemNotice
{
    optional uint32 AccountID = 1;
    optional uint32 RoleID = 2;
    optional uint64 PurchaseID = 3;
    optional uint32 GenderID = 4;
    optional uint32 MetierID = 5;
    optional uint32 Level = 6;
};

message CMessageAddFriendRequest
{
	optional fixed32 SrcID    = 1;
	optional fixed32 DesID    = 2;
	optional uint32  SrcLevel = 3;
};

message CMessageAddFriendResponse
{
	optional uint32 SendBack	  = 1;	// 是不是返回消息
	optional uint32 RetCode		  = 2;	// 状态码

	optional uint32         SrcCharID		= 3;	// 邀请者的ID，需要客户端存储这个ID
	optional uint32			SrcMetier		= 4;	// 邀请者的职业ID，也是门派ID
	optional uint32			SrcLevel		= 5;	// 邀请者的等级
	optional uint32			SrcSex			= 6;	// 邀请者的性别
	optional uint32			SrcHead			= 7;	// 邀请者的头像ID
	optional string			SrcName			= 8;	// 邀请者的名字
	optional uint32         DesCharID		= 9;	// 被邀请者的ID
	optional string			DesName			= 10;	// 被邀请者的名字
};

// 根据名字查char id
message CMessageGetCharIDRequest
{
	optional uint32 TypeID			= 1;
	optional string Name			= 2;
	optional fixed32 SrcEntityID	= 3;
};

message CMessageGetCharIDResponse
{
	optional uint32  TypeID			= 1;
	optional fixed32 CharID			= 2;
	optional fixed32 SrcEntityID	= 3;

	// 扩展 可以带回其他数据 根据type判断 
	optional string  Name			= 4;
	optional uint32  Head			= 5;
	optional uint32  Sex			= 6;

};

message CMessageRoleChangeCampNotice
{
    optional int32 RoleID = 1; 
    optional int32 Camp = 2;
};

message CMessageLoginSvrStartUpNotice
{
    optional int32 Status = 1;  //login上各种server的状态，用位来描述, 0 位: eRating状态 1 位: gmServer状态
};

message CMessageGate2LoginHeartBeatNotice
{
	optional uint32 PlayerAtGate = 1; //  网关上的玩家数
};

// ********************************************************************** //
// CMessagePersonalRankInfoNotify  游戏服务器 -> 网关服务器
// 发送个人排行榜信息
// ********************************************************************** //
message PBPersonalRankInfo
{
	optional uint32 CharID			= 1; // 角色ID
	optional string CharName		= 2; // 角色名
	optional uint32 Level			= 3; // 等级
	optional uint32 Money			= 4; // 非绑定金
	optional uint32 BoundedMoney	= 5; // 绑定金
	optional uint32 KillEnemyNumIn	= 6; // 国内杀敌数(护国)
	optional uint32 KillEnemyNumOut	= 7; // 国外杀敌数(出征)
	optional uint32 Honor			= 8; // 功勋
	optional uint32 Comprehensive	= 9; // 综合实力
	optional uint32 DieNum			= 10; // 死亡次数
	optional uint32 CompletedTasks	= 11; // 完成任务数	
	optional uint32 Country			= 12; // 国籍
	optional uint32 Metier			= 13; // 职业
	optional uint32 Title			= 14; // 称号
	optional uint32 OnLineTime		= 15; // 在线时长
	optional uint32 PantaoMark		= 16; // 蟠桃副本积分
	optional uint32 LonggongMark	= 17; // 龙宫副本积分
	optional uint64 Exp				= 18; // 玩家经验 用于等级排行辅助
	optional uint32 OriginReliveTimes	= 19;	// 原地复活次数
	optional uint32 KillSameNationNum	= 20;   // 杀本阵营人数
	optional uint32 KillOgreNum			= 21;	// 杀怪数
	optional uint32 UpFailNum			= 22;	// 精炼失败次数
	optional uint32 TodayKillEnemyNumIn	= 23; // 今日国内杀敌数(护国)
	optional uint32 TodayKillEnemyNumOut= 24; // 今日国外杀敌数(出征)
	optional uint32 TodayHonor			= 25; // 今日荣誉
	optional uint32 Dayanta				= 26; // 大雁塔
	optional uint32 Lianhuadong			= 27; // 莲花洞
	optional uint32 Huangfengdong		= 28; // 黄蜂洞
	optional uint32 AchivePoint			= 29; // 成就点
	optional uint32 CharmValue			= 30; // 魅力值
	optional uint32 FlyDungMark			= 31; // 飞行副本积分
	optional uint32 Zhangong			= 32; // 战功（在荣誉下面）
	optional uint32 Zhiguo				= 33; // 治国力（在才华下面）
	optional uint32 Tongshuai			= 34; // 统率力（在才华下面）
	optional uint32 Wisall				= 35; // 才华总值（在才华下面）
	optional uint32 YunXiao				= 36; // 云霄塔副本积分
	optional uint32 WeekIncCharm		= 37; // 本周累计魅力值
	optional uint32 Shide				= 38; // 师德
	optional uint32 XianYuanMark		= 39; // 仙缘
	optional uint32 Chushitu			= 40; // 出师徒弟数排行
	optional uint32 YunxiaoLevel		= 41; // 云霄塔层数排行
	optional uint32 WudidongMark		= 42; // 无底洞积分
    optional uint32 DaLuanDou		    = 43; // 大乱斗当前积分
	optional uint32 DaLuanDouTotal		= 44; // 大乱斗总积分
	optional uint32 GlobalWar			= 45; // 跨服战积分
	optional uint32 SpiritScore			= 46; // 铸灵评分
};		


message CMessagePersonalRankInfoNotify
{
	repeated PBPersonalRankInfo PersonalRanks = 1; // 多条个人排行榜信息
	optional uint32 Flag = 2; // 如果是1 表示是网关主动要的
	optional uint32 RType = 3; // 刷新类型 是半小时还是每天
	optional uint32 IsOver = 4; // 是否已结束(分包发的)
};

message PBRankTopTen
{
	optional uint32	RankType = 1;
	repeated uint32	CharList = 2;
};

message CMessageRankRemove
{
	optional string CharName	= 1;
	optional uint32 RankType	= 2;
	optional uint32 Refresh		= 3; // 1. 标志是否是重新刷新排行榜，用在排行榜出错没有的时候，手动刷新出来
};

// 网关问场景要玩家的信息
message CMessagePersonalRankInfoRequest
{
	repeated PBRankTopTen TopTenList = 1;
	optional uint32		  RType = 2; // 刷新类型 是半小时还是每天
};

message CMessageNewRankTopTenNotify
{
	repeated PBRankTopTen TopTenList = 1;
};

// ********************************************************************** //
// CMessageRoleGameInfoReportToGateNotify  游戏服务器 -> 网关服务器
// 发送个人游戏信息到网关
// ********************************************************************** //
message CMessageRoleGameInfoReportToGateNotify
{
	optional uint32 CharID		= 1; // 角色ID
	optional uint32 Level		= 2; // 等级
	optional uint32 MapID		= 3; // 地图ID
	optional uint32 LineID		= 4; // 线ID
	optional uint32 Metier		= 5; // 职业
	optional uint32 Country		= 6; // 国籍
	repeated uint32	MasterID	= 7; // 前辈ID
	optional uint32 PKValue		= 8;
};

// ********************************************************************** //
// CMessageBattleTeamWait  游戏服务器 -> 网关服务器
// 队伍进入战场等待列表
// ********************************************************************** //
message CMessageBattleTeamWait
{
	optional uint32 CharID		= 1; // 角色ID
	optional uint32 Level		= 2; // 等级
	optional uint32 BattleID	= 3; // 副本ID
	optional uint32 MapID		= 4;
};

// ********************************************************************** //
// CMessageBattleWaitLeave  游戏服务器 -> 网关服务器
// 从等待列表中离开
// ********************************************************************** //
message CMessageBattleWaitLeaveRequest
{
	optional uint32 CharID		= 1; // 角色ID
	optional uint32 BattleID	= 2; // 副本ID
	optional uint32 MapID		= 3;
};


// ********************************************************************** //
// CMessageBattleJoinNotice  网关服务器 -> 游戏服务器
// 玩家可以进入战场
// ********************************************************************** //
message CMessageBattleJoinNotice
{
	optional uint32 CharID		= 1; // 角色ID
	optional uint32 BattleID	= 2; // 副本ID
	optional uint32	MapID		= 3;
	optional uint32 Type		= 4;
	optional uint32 BattleTime	= 5; // 战争开始时间
	optional uint32 Key			= 6; //key
	optional uint32	IntoLastRepetion=7;	// 是否副本重进入
};

// ********************************************************************** //
// CMessageBattlePlayerJoin  游戏服务器 -> 网关服务器
// 玩家进入战场
// ********************************************************************** //
message CMessageBattlePlayerJoin
{
	optional uint32 CharID		 = 1; // 角色ID
	optional uint32 BattleID	= 2; // 副本ID
	optional uint64 Key			= 3; // key
	optional uint32 CampID		= 4; // 阵营
	optional uint32 MapID		= 5;
};

// ********************************************************************** //
// CMessageBattleWaitNotice  网关服务器 -> 游戏服务器
// 玩家可以进入战场等待队列
// ********************************************************************** //
message CMessageBattleWaitNotice
{
	optional uint32 CharID		= 1; // 角色ID
	optional uint32 BattleID	= 2; // 副本ID
	optional uint32 MapID		= 3;
};	 



// ********************************************************************** //
// CMessageServerValidateRequest  登录服务器 -> 验证服务器
// 进行服务器验证的请求
// ********************************************************************** //
message CMessageServerValidateRequest
{
	optional string MacAddr	= 1;	//		Mac地址
	optional string IPAddr	= 2;	//		IP地址
};

// ********************************************************************** //
// CMessageBattleWaitLeaveResponse  网关服务器 -> 游戏服务器
// 玩家可以进入战场等待队列
// ********************************************************************** //
message CMessageBattleWaitLeaveResponse
{
	optional uint32 CharID		= 1; // 角色ID
	optional uint32 BattleID	= 2; // 副本ID 
	optional uint32 MapID		= 3;
};


// ********************************************************************** //
// CMessageServerValidateResponse  验证服务器->登录服务器
// 进行服务器验证的回应
// ********************************************************************** //
message CMessageServerValidateResponse
{
	optional uint32 CheckResult	= 1;
};


// ********************************************************************** //
// CMessageNotifyTracedPlayer  网关服务器->场景服务器
// 通知需要被追踪的玩家
// ********************************************************************** //
message CTracedPlayerInfo
{
	optional uint32 AccountID	= 1;
	repeated uint32 RoleID		= 2;
};

message CMessageNotifyTracedPlayer
{ 
	repeated CTracedPlayerInfo PlayerList = 1;	
};

// ********************************************************************** //
// CMessageBattleCreateRequest  sceneserver->gateserver
// 创建BattleID
// ********************************************************************** //
message CMessageBattleCreateRequest
{
	optional uint32	BattleID		= 1;
	optional uint32 Level			= 2;
	optional uint32 MapID			= 3;
}
// ********************************************************************** //
// CMessageBattleCreateResponse  gateserver->sceneserver
// 创建BattleID
// ********************************************************************** //
message CMessageBattleCreateResponse
{
	optional uint32 BattleID		= 1;
	optional uint64 Key				= 2;
	optional uint32 MapID			= 3;
}


// 通知场景服务器ID名字对应关系
message CMessageFamilyIDNameNotify
{
	repeated uint32 FamilyID	= 1;
	repeated string FamilyName	= 2;
};

message PBCorpsNpc
{
	optional uint32	NpcID = 1;
	optional uint32	CorpsID = 2;
	optional string	CorpsName = 3;
	optional uint32	Type	= 4;	// add \ del
};

// 通知场景服务器NPC家族对应关系
message CMessageNpcListNotify
{
	repeated uint32 NpcID		= 1;
	repeated uint32 FamilyID	= 2;
	optional uint32	Type		= 3; // 0:family; 1:corps
	repeated PBCorpsNpc CorpsNpc = 4;
};

// 场景服务器通知竞价截止时间到
message CMessageBidTimeOutNotify
{
	optional uint32 NpcID		= 1;
	optional uint32 First		= 2;
};

// 场景服务器通知进入战场
message CMessageBattleStartRequest
{
	optional uint32 NpcID		= 1;
};

// 回复场景服务器竞价截止时间到
message CMessageBattleStartResponse
{
	optional uint32 NpcID		= 1;
	repeated uint32 CharID		= 2;
	repeated string CharName	= 3;
	repeated uint32 Money		= 4;
};

// 场景服务器通知家族战结果
message CMessageBattleEndNotify
{
	optional uint64	Key				= 1;
	optional uint32 WinSide			= 2;
};

// 通知场景服务器竞价重新开始
message CMessageBidRestartNotify
{
	optional uint32 NpcID		= 1;
};

message CMessageObtainTopTenRequest
{
	optional uint32	CharID = 1;
};

message PBRankTitle
{
	optional uint32	Index = 1;
	optional uint32	RankType = 2;
};

message CMessageObtainTopTenResponse
{
	optional uint32 CharID = 1;
	repeated PBRankTitle RankTitle = 2;
};

message CMessageCorpsBidTimerStopRequest
{
	optional uint32	NpcID = 1;
	optional uint32 BattleId = 2;
};

message CMessageCorpsJoinBattleNotice
{
	optional uint32	CharID = 1;
	optional uint32	NpcID = 2;
};

message CMessageNotifyBattleResult2Gate
{
	optional uint32	BattleID = 1;	// 战场ID
	optional uint32	WinCamp = 2;
	optional uint64	BattleKey = 3;
};

message CMessageCorpsBattleFlagRequest
{
	optional uint32 CharID = 1;
	optional uint32	CorpsID = 2;	// 通知对象的军团ID
	optional uint32 CampID = 3;		// 大旗归属的阵营ID
	optional uint32	BattleID = 4;
	optional uint32	IsAll = 5;
	optional uint64 BattleKey = 6;
};

message CMessageCorpsBattleFlagResponse
{
	optional uint32	CharID = 1;
	optional string CorpsName = 2;
	optional uint32	BattleID = 3;
	optional uint32 CampID = 4;
	optional uint32	IsAll = 5;
};

message CMessageCreateRepetionNotify
{
	optional uint32 RepetionIndex = 1;
	optional uint64 Key = 2;
	optional uint32 Parame1 = 3;
	optional uint32 Parame2 = 4;
	optional uint32 LineID = 5;
	optional uint32	Creator = 6;
	optional uint32 RemoveItem = 7;
};

message CMessageCreateRepetionErr
{
	optional uint32 RepetionIndex = 1;
	optional uint64 Key = 2;
	optional uint32 ErrCode = 3;
};

message CMessageTeamIntoRepetion
{
	optional uint32 MapID	= 1;
	optional uint32 CharID	= 2;
	optional uint32 PosX	= 3;
	optional uint32 PosY	= 4;
	optional uint32 Parame1 = 5;
	optional uint32 Parame2 = 6;
	optional uint32 Level	= 7;
	optional uint32 RemoveItem = 8;
};

message CMessageSingleIntoRepetion
{
	optional uint32 MapID	= 1;
	optional uint32 CharID	= 2;
	optional uint32 PosX	= 3;
	optional uint32 PosY	= 4;
	optional uint32 Parame1 = 5;
	optional uint32 Parame2 = 6;
	optional uint32 Level	= 7;
	optional uint32 RemoveItem = 8;
};

message CMessageCorpsStartBattleRequest
{
	optional uint32	NpcID = 1;
	optional uint32	BattleID = 2;
};

message CMessageIntoRepetionNotify
{
	optional uint32 LineID	= 1;
	optional uint32 MapID	= 2;
	optional uint64 Key		= 3;
	optional uint32 PosX	= 4;
	optional uint32 PosY	= 5;
	optional uint32 CharID	= 6;
	optional uint32 RepetionIndex = 7;
};



// *************************************************************************** //
// CMessageChangeFamilyContributeNotify
// 改变玩家家族贡献度的通知
// *************************************************************************** //
message CMessageChangeFamilyContributeNotify
{		
	optional uint32				RoleID			=	1;		// 玩家角色ID
	optional uint32				Contribute		=	2;		// 贡献度	
	optional uint32				ChangeType		=	3;		// 0表示增加1表示减少
};

// *************************************************************************** //
// CMessageChangeFamilyGloryNotify
// 改变家族荣耀的通知
// *************************************************************************** //
message CMessageChangeFamilyGloryNotify
{															
	optional uint32				FamilyID		=	1;		// 家族ID
	optional uint32				Glory			=	2;		// 家族荣耀值
	optional uint32				IfReduced		=	3;		// 是否减少	
	optional uint32				RoleID			=	4;		// 角色ID
	optional uint32				Reason			=	5;		// 家族荣耀改变的原因	
	optional string				RoleName		=	6;		// 角色名称
};

// *************************************************************************** //
// CMessageChangeFamilyMoneyNotify
// 改变家族金钱的通知
// *************************************************************************** //
message CMessageChangeFamilyMoneyNotify
{			
	optional uint32				Money		=	1;		// 金钱
	optional uint32				IfPayout	=	2;		// 是否减少
	optional uint32				FamilyID	=	3;		// 家族ID
	optional uint32				RoleID		=	4;		// 角色ID
	optional uint32				Reason		=	5;		// 改变金钱的原因
	optional string				RoleName	=	6;		// 角色名称
};


// *************************************************************************** //
// CMessageSynIbStoreRequest
// 同步商城物品请求
// *************************************************************************** //
message CMessageSynIbStoreRequest
{
};

message CMessageSynIbStoreResponse
{
    repeated PBIbItemOnSale Goods   = 1;  // 商城中在售的商品列表
    repeated PBIbStore IbStore      = 2;  // 货架, 目前只有两个根据金银元宝区分
    optional int32  Version	    = 3;  // 商城版本号
    repeated PBIbSubTab  FocusTab   = 4;  // 重点关注区域的物品ID
    optional string Bulletin        = 5;  // 商城公告 
};

// *************************************************************************** //
// CMessageSynFuncStoreRequest
// 同步功能付费数据请求
// *************************************************************************** //
message CMessageSynFuncStoreRequest
{
};

message CMessageSynFuncStoreResponse
{
	optional int32			Version		= 1;	//版本号
	repeated PBFuncStore	FuncStore	= 2;	//功能项
};

// *************************************************************************** //
// CMessageChangeCorpsContributeNotify
// 改变玩家军团贡献度的通知
// *************************************************************************** //
message CMessageChangeCorpsContributeNotify
{		
	optional uint32				RoleID		=	1;		// 玩家角色ID
	optional uint32				Contribute	=	2;		// 贡献度
	optional uint32				ChangeType 	=	3;		// 0表示增加1表示减少	
};

// *************************************************************************** //
// CMessageChangeCorpsGloryNotify
// 改变军团荣耀的通知
// *************************************************************************** //
message CMessageChangeCorpsGloryNotify
{
	optional uint32				CorpsID		=	1;		// 家族ID
	optional uint32				Glory		=	2;		// 家族荣耀值
	optional uint32				IfReduced	=	3;		// 是否减少	
	optional uint32				RoleID		=	4;		// 角色ID
	optional uint32				Reason		=	5;		// 原因	
	optional string				RoleName	=	6;		// 角色名称
};

// *************************************************************************** //
// CMessageChangeCorpsMoneyNotify
// 改变军团金钱的通知
// *************************************************************************** //
message CMessageChangeCorpsMoneyNotify
{
	optional uint32	CorpsID		=	1;		// 家族ID
	optional uint32	Money		=	2;		// 金钱
	optional uint32	IfPayout	=	3;		// 是否减少
	optional uint32	RoleID		=	4;		// 角色ID
	optional uint32	Reason		=	5;		// 金钱改变的原因
	optional string	RoleName	=	6;		// 角色名称
};

// *************************************************************************** //
// CMessageChangeCorpsBossTimes
// 军团boss 添加次数
// *************************************************************************** //
message CMessageChangeCorpsBossTimes
{
	optional uint32 CorpsID		=  1;		//军团id
	optional uint32	Times		=  2;		//添加次数
	optional uint32 RoleID		=  3;		//角色ID
	optional uint32 BossID		=  4;		//Boss id
	optional string RoleName	=  5;		//角色名
	optional uint32 Reason		=  6;		//原因
};
// *************************************************************************** //
//CMessageChangeCorpsBossRefineValue
//军团boss炼化值改变
// *************************************************************************** //
message CMessageChangeCorpsBossRefineValue
{
	optional uint32 CorpsID			= 1;	// 军团ID
	optional uint32 RefineValue		= 2;	// 炼化值
	optional uint32 Reason			= 3;	// 炼化值改变原因
	optional uint32 RoleID			= 4;	// 角色ID
	optional string RoleName		= 5;	// 角色名
};
// *************************************************************************** //
// CMessageChangeCorpsCollectActive
// 军团采集活跃度改变
// *************************************************************************** //
message CMessageChangeCorpsCollectActive
{
	optional uint32 RoleID			= 1;	// 角色ID
	optional uint32 ActiveValue		= 2;	// 改变的活跃度值
	optional uint32 ChangeType		= 3;	// 改变类型   0表示增加1表示减少
}




// CMessageFamilyApExpChangedNotify
// 家族成员经验和ap值改变的通知
// *************************************************************************** //
message CMessageFamilyApExpChangedNotify
{ 
	optional uint32 RoleID		=	1;			// 成员角色ID
	optional uint32 FamiyID		=	2;			// 家族ID
	optional uint32	AP			=	3;			// ap值
	optional uint32	Exp			=	4;			// 经验值
};

// *************************************************************************** //
// CMessageCorpsApExpChangedNotify
// 军团成员经验和ap值改变的通知
// *************************************************************************** //
message CMessageCorpsApExpChangedNotify
{
	optional uint32 RoleID		=	1;			// 成员角色ID
	optional uint32 CorpsID		=	2;			// 家族ID
	optional uint32	AP			=	3;			// ap值
	optional uint32	Exp			=	4;			// 经验值
};

message CMessageChangeJudgeScoreReuqest
{
	optional uint32	RoleID		= 1;
	optional uint32	Total		= 2;
};


// *************************************************************************** //
// CMessageChangeWizardRequest
// 改变才华请求
// *************************************************************************** //
message CMessageChangeWizardRequest
{
	optional uint32		CharID			= 1;		// 角色ID
	optional uint32		LineID			= 2;
	optional uint32		CountryID		= 3;
	optional uint32		ChangeType		= 4;		// 改变类别
	repeated uint32		WizardValue		= 5;		// 才华值
};

// *************************************************************************** //
// CMessageChangeWizardResponse
// 改变才华回应
// *************************************************************************** //
message CMessageChangeWizardResponse
{
	optional uint32		CharID			= 1;		// 角色ID
	optional uint32		Result			= 2;		// 错误码
};

// *************************************************************************** //
// CMessageChangeConstructRequest
// 改变建设请求
// *************************************************************************** //
message CMessageChangeConstructRequest
{
	optional uint32		CharID			= 1;		// 角色ID
	optional uint32		LineID			= 2;
	optional uint32		CountryID		= 3;
	optional uint32		ChangeType		= 4;		// 改变类别
	repeated uint32		ConstructType	= 5;		// 建设类型
	repeated uint32		ConstructValue	= 6;		// 建设值
};

// *************************************************************************** //
// CMessageChangeConstructResponse
// 改变建设回应
// *************************************************************************** //
message CMessageChangeConstructResponse
{
	optional uint32		CharID			= 1;		// 角色ID
	optional uint32		Result			= 2;		// 错误码
};

// *************************************************************************** //
// CMessageChangePowerRequest
// 改变声望请求
// *************************************************************************** //
message CMessageChangePowerRequest
{
	optional uint32		CharID			= 1;		// 角色ID
	optional uint32		LineID			= 2;
	optional uint32		CountryID		= 3;
	optional uint32		ChangeType		= 4;		// 改变类别
	optional uint32		PowerValue		= 5;		// 声望值
};

// *************************************************************************** //
// CMessageChangePowerResponse
// 改变声望回应
// *************************************************************************** //
message CMessageChangePowerResponse
{
	optional uint32		CharID			= 1;		// 角色ID
	optional uint32		Result			= 2;		// 错误码
};

message CMessageUseFunctionNotify
{
	optional uint32			CharID				= 1;	// 发起者ID,服务器用
	optional uint32			FunctionType		= 2;	// 功能类型
	optional uint32			FunctionOrder		= 3;	// 功能编号
	optional uint32			TargetID			= 4;	// 目标ID
	optional uint32			WorldID				= 5;	// 阵营ID
	optional uint32			CountryID			= 6;	// 国家ID
	optional string			SrcName				= 7;	// 发起者名字
	optional TeleportData	Teleport			= 8;	// 传送数据
	optional uint32			CorpsID				= 9;	// 军团ID
	optional uint32			FamilyID			= 10;	// 家族ID
	optional uint32			TokenID				= 11;	// 传送标志
};

message CMessageSetOfficialNotify
{
	optional uint32			CharID				= 1;	// 发起者ID,服务器用
	optional uint32			OfficialLevel		= 2;	// 官员类型
	optional uint32			OfficialOrder		= 3;	// 官员编号
	optional uint32			TargetID			= 4;	// 目标ID
};

message CMessageChangeFriendNotify
{
	optional uint32			CharID				= 1;	// 发起者ID,服务器用
	optional uint32			ChangeType			= 2;	// 改变类型
	optional uint32			SrcWorld			= 3;	// 源阵营
	optional uint32			DesWorld			= 4;	// 目的阵营
	optional uint32			ChangeValue			= 5;	// 改变值
};

message CMessageKillPersonRequest
{
	optional uint32			SrcCharID			= 1;	// 发起者ID
	optional uint32			DesCharID			= 2;	// 被杀者ID
	optional uint32			Honor				= 3;	// 是否荣誉击杀
};

message CMessageKillPersonResponse
{
	optional uint32			SrcCharID			= 1;	// 发起者ID
	optional uint32			DesCharID			= 2;	// 被杀者ID
	optional uint32			PKValue				= 3;	// 改变的pk值
};

message CMessageSyncNationNotify
{
	repeated uint32			NationStatus		 = 1;	// 国家关系
};

// *************************************************************************** //
// CMessageBuyFamilyItemRequest
// 购买家族物品的消息请求,目前只有家族红包
// *************************************************************************** //
message CMessageBuyFamilyItemRequest
{
	optional uint32		RoleID		= 1;		// 角色ID
	optional uint32		EntityID	= 2;		// 实体ID
	optional uint32		FamilyID	= 3;		// 家族ID
	optional uint32		ItemID		= 4;		// 物品ID
	optional uint32		ItemNum		= 5;		// 红包数量
	optional uint32		TotalCost	= 6;		// 所有红包的花费( 服务器用 )
	optional uint32		NpcTmpID	= 7;		// 所选Npc的模板ID	
};

// *************************************************************************** //
// CMessageBuyFamilyItemResponse
// 购买家族物品的消息回应,目前只有家族红包
// *************************************************************************** //
message CMessageBuyFamilyItemResponse
{
	optional uint32		Errcode		= 1;		// 错误码
	optional uint32		RoleID		= 2;		// 角色ID
	optional uint32		EntityID	= 3;		// 实体ID	
	optional uint32		ItemID		= 4;		// 物品ID
	optional uint32		ItemNum		= 5;		// 红包数量
};


// *************************************************************************** //
// CMessageBuyCorpsItemRequest
// 购买军团物品的消息请求,目前只有军团红包
// *************************************************************************** //
message CMessageBuyCorpsItemRequest
{
	optional uint32		RoleID		= 1;		// 角色ID
	optional uint32		EntityID	= 2;		// 实体ID
	optional uint32		CorpsID		= 3;		// 家族ID
	optional uint32		ItemID		= 4;		// 物品ID
	optional uint32		ItemNum		= 5;		// 红包数量
	optional uint32		TotalCost	= 6;		// 所有红包的花费( 服务器用 )
	optional uint32		NpcTmpID	= 7;		// 所选Npc的模板ID	
	optional uint32		CityMapID	= 8;		// 如果是战争商店则表示改商店所在的城市地图ID
};

// *************************************************************************** //
// CMessageBuyCorpsItemResponse
// 购买军团物品的消息回应,目前只有军团红包
// *************************************************************************** //
message CMessageBuyCorpsItemResponse
{
	optional uint32		Errcode		= 1;		// 错误码
	optional uint32		RoleID		= 2;		// 角色ID
	optional uint32		EntityID	= 3;		// 实体ID	
	optional uint32		ItemID		= 4;		// 物品ID
	optional uint32		ItemNum		= 5;		// 红包数量
};


// *************************************************************************** //
// CMessageClearFamilyRepetionInfoNotify
// 清除家族副本信息的通知
// *************************************************************************** //
message CMessageClearFamilyRepetionInfoNotify
{ 
	optional uint32		FamilyID			=	1;		//	家族ID
	optional uint32		RepetionScore		=	2;		//	副本得分
};

// 通知场景评价修改成功 [12/8/2009 Macro]
message CMessageJudgeModifySuccessNotice
{
	optional uint32	 CharID	= 1;	// 评价者
};

// 查询元宝申请
message CMessage4EQueryYuanBaoRequest
{
	optional uint32 RoleID = 1;	// 查看者的RoleID
};

// 查询元宝回应
message CMessage4EQueryYuanBaoResponse
{
	optional int32	ResultCode = 1;	// 回应代码
	optional uint32 RoleID = 2;	// 角色ID
	repeated PBIbMoney Remain = 3;	// 帐户余额
};

// 领取元宝申请
message CMessage4EDrawYuanBaoRequest
{
	optional uint32 RoleID 	= 1;	// 申请者的RoleID		
	optional PBIbMoney Draw = 2;	// 申请获取的金钱
};

// 领取元宝回应
message CMessage4EDrawYuanBaoResponse
{
	optional uint32 RoleID		= 1;	// 申请者的RoleID
	optional int32	ResultCode 	= 2;	// 回应代码
	optional int32  Amount		= 3;	// ERating解释	
	optional int32  LeftAmount	= 4;	// ERating解释	
};

// 进行家族兑换的消息请求
message CMessageFamilyExchangeRequest
{
	optional uint32	RoleID		= 1;	// 角色ID
	optional uint32	NpcTmpID	= 2;	// npc的模板ID
	optional uint32	ComposeID	= 3;	// 复杂合成的ID
	optional uint32	FamilyID	= 4;	// 家族ID
	optional uint32	ItemNum		= 5;	// 兑换的数量
};

// 进行家族兑换的消息回应
message CMessageFamilyExchangeResponse
{
	optional uint32	Errcode		= 1;	// 错误码
	optional uint32	RoleID		= 2;	// 角色ID
	optional uint32	NpcTmpID	= 3;	// npc的模板ID
	optional uint32	ComposeID	= 4;	// 复杂合成的ID	
	optional uint32	ItemNum		= 5;	// 兑换的数量
};

// 进行军团兑换的消息请求
message CMessageCorpsExchangeRequest
{
	optional uint32	RoleID		= 1;	// 角色ID
	optional uint32	NpcTmpID	= 2;	// npc的模板ID
	optional uint32	ComposeID	= 3;	// 复杂合成的ID	
	optional uint32	CorpsID		= 4;	// 军团ID
	optional uint32	ItemNum		= 5;	// 兑换的数量
};

// 进行军团兑换的消息回应
message CMessageCorpsExchangeResponse
{
	optional uint32	Errcode		= 1;	// 错误码
	optional uint32	RoleID		= 2;	// 角色ID
	optional uint32	NpcTmpID	= 3;	// npc的模板ID
	optional uint32	ComposeID	= 4;	// 复杂合成的ID
	optional uint32	ItemNum		= 5;	// 兑换生成的物品数量	
};

// 增加家族兑换物品的消息请求
message CMessageAddFamilyExchangeItemNotify
{
	optional uint32	RoleID			= 1;	// 角色ID
	optional uint32	FamilyID		= 2;	// 家族ID	
	optional uint32	LeagueBagTmpID	= 3;	// 特供物品的模板ID
};

// 增加军团兑换物品的消息请求
message CMessageAddCorpsExchangeItemNotify
{
	optional uint32	RoleID			= 1;	// 角色ID
	optional uint32	CorpsID			= 2;	// 军团ID	
	optional uint32	LeagueBagTmpID	= 3;	// 特供物品的模板ID
}
message CMessageGateEntityGMMove
{
	optional string RoleName	= 1;
	optional int32	LineID		= 2;
	optional int32 	MapID		= 3;
	optional int32  PosX		= 4;
	optional int32	PosY		= 5;

	optional int64  Key			= 6;
};

message CMessageSceneEntityGMMove
{
	optional int32 RoleID		= 1;
    optional int32  LineID      = 2;
    optional int32  MapID       = 3;
    optional int32  PosX        = 4;
    optional int32  PosY        = 5;

	optional int64  Key			= 6;
};

/// 以下 活动消息
message SingleActivityObj
{
	optional uint32 ActivityID		= 1;
	optional uint32 TimeType		= 2;
	optional uint32 TimeInfo	    = 3;
	optional uint32 LmtLevel		= 4;
	optional uint32 MaxTimes		= 5;
	optional uint32 State			= 6; // 状态
};

message DropInfo
{
	optional uint32 OgreID		= 1;
	optional uint32 DropID		= 2;
};

message MapDrop
{
	optional uint32 DropID			= 1; // 掉落ID
	optional uint32 DropType		= 2; // 掉落类型
	repeated uint32 MapIDs			= 3; // 地图ID
};

// 活动综合静态信息
message SingleActivityInfo
{
	optional uint32   ActivityID		= 1;
	repeated DropInfo dropinfo			= 2;	// 掉落信息
	repeated uint32   ogreindex			= 3;	// 布怪信息
};

// 网关通知场景今日活动列表
message CMessageTodayActivitysNotify
{
	repeated SingleActivityObj  ActivityList = 1;
	repeated SingleActivityInfo ActivityInfo = 2;
};

message SingleActivity
{
	optional uint32 ActivityID		= 1;
	optional uint32 ActivityState   = 2;
	repeated MapDrop  MapDrops		= 3;	// 地图掉落数据
};

// 网关通知场景XXX活动开始/结束了
message CMessageActivityChangeNotify
{
	repeated SingleActivity ActivityList = 1;
};

// 场景通知玩家安全密码状态改变的通知
message CMessageChangePasswordStatusNotify
{
	optional uint32	RoleID		=	1;	// 角色ID
	optional uint32	PwdStatus	=	2;	// 密码状态
	optional string	Password	=	3;	// 密码
};

// 通知场景，国家称号更新 [1/28/2010 Macro]
message CMessageUpdateNationTitleNotice
{
	optional int32	RoleID	= 1;	// 玩家ID
	optional uint32	TitleID	= 2;	// 称号ID
	optional uint32	IsAdd	= 3;	// 增加、删除
};

// 请求所在军团的国家称号列表
message CMessageGetNationTitleListRequest
{
	optional int32	RoleID	= 1;	// 玩家ID
};

message CMessageGetNationTitleListResponse
{
	optional int32	RoleID	= 1;	// 玩家ID
	repeated uint32	TitleList	= 2;	// 称号列表
};

// 场景通知网关会员标志发生改变
message CMessageSGProVipFlagNotice
{
	optional int32	RoleID	= 1;	// 玩家ID
	optional uint32	VipFlag	= 2;	// 会员标志
};

// 网关通知场景重设玩家财产密码
message CMessageResetRolePasswordNotify
{
	optional uint32	RoleID = 1;	
}

// 通知安全关闭
message CMessgeSafeShutdownNotify
{
	optional uint32 ServerType	= 1; // 服务器类型
	optional uint32 ServerID	= 2; // 服务器ID
	optional uint32 OpType		= 3; // 关闭方式
};

// 通知安全关闭回应
message CMessgeSafeShutdownAck
{
	optional uint32 ServerType	= 1; // 服务器类型
	optional uint32 ServerID	= 2; // 服务器ID
	optional uint32 OpType		= 3; // 关闭方式
};

// 军团站金钱回滚
message CMessageRollCorpsMoneyRequest
{
	optional uint32	IsAll	= 1;
	optional uint32	CorpsID = 2;
	optional uint32	NpcID	= 3;
};

// 军团战地图通知
message CMessageCorpsBattleMapNotice
{
	optional uint64 BattleKey = 1;
	optional uint32	RoleID = 2;
};

// 申请召集请求
message CMessageTrySummonRequest
{
	optional uint32	CharID	=	1;
	optional string	CharName =	2;
	optional uint32 ItemIndex = 3;
	optional uint32	SummonType = 4;
};

// 申请召集回应
message CMessageTrySummonResponse
{
	optional uint32	CharID	=	1;
	optional string	CharName =	2;
	optional uint32	Result	=	3;
	optional uint32 ItemIndex = 4;
	optional uint32	SummonType = 5;
};

// 实际召集请求
message CMessageRealSummonRequest
{
	optional uint32	CharID	=	1;
	optional string	CharName =	2;
	optional uint32	SummonType	= 3;
	optional uint32	LineID		=	4;
	optional uint32	MapID		=	5;
	optional uint32	PosX		=	6;
	optional uint32	PosY		=	7;
};

// 实际召集通知
message CMessageRealSummonNotify
{
	optional string SrcCharName	=	1;
	optional uint32	LineID		=	2;
	optional uint32	MapID		=	3;
	optional uint32	PosX		=	4;
	optional uint32	PosY		=	5;
	optional uint32	TokenID		=	6;
	optional uint32 DesCharID	=	7;
	optional uint32	FamilyID	=	8;
	optional uint32	CorpsID		=	9;
	optional uint32	SrcCharID	=	10;
};

// 锁定物品个数通知
message CMessageLockGoodsRequest
{
	optional uint32 GoodsID 	= 1;  // 商品ID
	optional int32  LockCount 	= 2;  // 购买的商品数量
	optional uint32 RoleID 		= 3;  // 购买物品的角色ID
	optional int32  TransObjID	= 4;  // 此次购买的交易对象ID，非交易ID
};

message CMessageLockGoodsResponse
{
	optional int32 Result 		= 1;  // 购买结果
	optional uint32 GoodsID 	= 2;  // 购买商品ID
	optional uint32 RoleID 		= 3;  // 购买角色ID
	optional int32  LockCount 	= 4;  // 购买的数量
	optional int32  Remain		= 5;  // 剩余的数量
	optional int32  TransObjID 	= 6;  // 交易的对象ID
};

message CMessageS2GRoleMove
{
	optional string SrcRole	= 1;
	optional string DesRole	= 2;
};

message CMessageG2SRoleMove
{
	optional string SrcRole = 1;
	optional string DesRole = 2;
};

message CMessageWarChangeNotify
{
	optional uint32	WorldID			=	1;
	optional uint32	SeedType		=	2;
	optional uint32	SeedValue		=	3;
	optional uint32	SourceType		=	4;
	optional uint32	SourceValue		=	5;
	optional uint32	MapID			=	6;
};

message CMessageReliveChangeNotify
{
	repeated uint32	WorldID			=	1;
	repeated uint32	CampID			=	2;
	optional uint32	WarStatus		=	3;
};

message WarHonorData
{
	optional uint32 CharID			=	1;
	optional uint32	WarHonor		=	2;
	optional uint32	WarKill			=	3;
};

message CMessageObtainWarHonorNotify
{
	repeated WarHonorData	WarHonor	=	1;
	optional uint32			IsKill		=	2;
};


message CMessageBourseG2SLoadList
{
	optional uint32	ListID					= 1;
	optional PBBourseDetailList BuyList     = 2;
	optional PBBourseDetailList SellList    = 3;
};

message CMessageBourseS2GLoadList
{
};


message CMessageSyncWarStatusNotify
{
	optional uint32 WarStatus		=	1;
	optional uint32	WarSerial		=	2;
};

message CMessageWarControlNotify
{
	optional uint32	ControlState	=	1;
	optional int32	GMHour			=	2;
	optional int32	GMMin			=	3;
	optional int32	GMDay			=	4;
	optional uint32 WarSerial		=	5;
};

message CMessageBourseTradeNotice
{
	optional PBBourseDetail BuyDetail		= 1;
	optional PBBourseDetail SellDetail		= 2;
};
// 请求场景服务器再次存盘(从文件)
message CMessageReSavePlayerRequest
{
	optional uint32 RoleID			= 1; // 角色ID
};

// 请求场景服务器再次存盘 回应
message CMessageReSavePlayerResponse
{
	optional uint32 RoleID			= 1; // 角色ID
	optional uint32 Result			= 2; // 结果 0成功
	optional uint32 LineID			= 3; // 当前线ID
	optional uint32 MapID			= 4; // 当前地图ID
	optional uint32 AccountID		= 5; // 玩家账号ID
	optional uint32 PosX			= 6; // 当前坐标X
	optional uint32 PosY			= 7; // 当前坐标Y
};


message CMessageBoursePlayerTradeBuyNotice
{
	optional PBBourseDetail Detail      = 1;
};

message CMessageBoursePlayerTradeSellNotice
{
	optional PBBourseDetail Detail      = 1;
};

message CMessageWarStoneOverNotify
{
	optional uint32	 MapID			=	1;
};

message CMessageCreateWarStoneNotify
{
	optional uint32	 StoneLevel		=	1;
	optional uint32	 MapID			=	2;
};

message CMessageAddTaxNotify
{
	optional uint32	 Money			=	1;
};

// 请求场景服务器再次存盘 回应
message CMessageSendSysMailToPlayer
{
    optional string role_name	= 1;    // 接收者姓名
    optional uint32 role_id		= 2;    // 接收者ID
    optional string sender_name	= 3;    // 发送者姓名
    optional string title		= 4;    // 邮件标题
    optional string body		= 5;    // 邮件内容
    optional uint32 receive_countryid= 6; // 接收者所在的阵营
    optional uint32 gateway_id  = 7;    // 网关ID
    optional uint32 game_id     = 8;    // 游戏ID
    optional uint32 fd          = 9;
    optional uint32 type        = 10;
    optional uint32 session_id  = 11;	
    repeated uint32	ItemID		= 13;	// 物品ID
    repeated uint32	ItemNum		= 14;	// 物品数量	 
    optional uint32	MailSendType= 15;	// 邮件发送类型()
};

// 向阵营发送用于弱国判定积分的荣誉值
message CMessageCampAddHonerScore
{
	optional uint32 role_id			= 1;    // 角色ID
	optional uint32 honer_value		= 2;    // 荣誉值
	optional uint32 camp_id			= 3;    // 阵营ID
};

// ********************************************************************** //
// CMessageBattleTeamFixWait  游戏服务器 -> 网关服务器
// 队伍进入定时开启的副本等待列表
// ********************************************************************** //
message CMessageBattleTeamFixWait
{
	optional uint32 CharID		= 1; // 角色ID
	optional uint32 Level		= 2; // 等级
	optional uint32 BattleID	= 3; // 副本ID
	optional uint32 MapID		= 4;
};

// 通知场景某些活动事件触发
message CMessageActivityEventNotify
{
	optional uint32 ActivityID	= 1;	// 活动ID
	optional uint32 EventID		= 2;	// 事件ID
	repeated uint32 IntParams   = 3;	// 传递的整型参数
	repeated string StrParams   = 4;	// 传递的字符串参数
};


message CMessageAddMasterPointRequest
{
	optional uint32	MasterID	= 1;
	optional uint32	Point		= 2;
};

message CMessageAddMasterPointNotify
{
	optional uint32	CharID		= 1;
	optional uint32	Point		= 2;
};

// ********************************************************************** //
// CMessageBattleSingleFixWait  游戏服务器 -> 网关服务器
// 单人进入定时定时报名的副本等待列表
// ********************************************************************** //
message CMessageBattleSingleFixWait
{
	optional uint32 CharID		= 1; // 角色ID
	optional uint32 Level		= 2; // 等级
	optional uint32 BattleID	= 3; // 副本ID
	optional uint32 MapID		= 4;
};

message CMessageSpecialEventNotify
{
	optional uint32	SrcWorldID	= 1;
	optional uint32	DesWorldID	= 2;
	optional uint32	EventType	= 3;
	optional uint32	CharID		= 4;
};

message CMessageAcquireMasterNotify
{
	optional uint32	SlaveID		= 1;
	optional uint32	MasterID	= 2;
};
// ********************************************************************** //
// CMessageBattleWinnerNotify  游戏服务器 -> 网关服务器 
// 单人进入定时定时报名的副本等待列表
// ********************************************************************** //
message CMessageBattleWinnerNotify
{
	optional uint32 HasWinner	= 1; // 角色ID
	optional uint32 BattleID	= 2; // 副本ID
	optional uint32 Level		= 3; // 等级
	optional uint32 CharID		= 4; // 角色ID
};

// *************************************************************************** //
// CMessageChangeFamilyStabilityNotify
// 改变家族安定度
// *************************************************************************** //
message CMessageChangeFamilyStabilityNotify
{
	optional uint32				FamilyID	=	1;		// 家族ID	
	optional uint32				RoleID		=	3;		// 角色ID
	optional uint32				Reason		=	4;		// 原因	
	optional string				RoleName	=	5;		// 角色名称
	optional uint32				OnLineTime	=	6;		// 在线时间	
	optional uint32				RoleLevel	=	7;		// 玩家等级
	optional uint32				Value		=	9;		// 改变的数值(有正有负)
};

// *************************************************************************** //
// CMessageS2GJionOnTimeOpenBattleRequest
// 通知gateserver 玩家进入定时开放副本，不包括报名副本
// *************************************************************************** //
message CMessageS2GJoinOnTimeOpenBattleRequest
{
	optional uint32 BattleID= 1;
	optional uint32 MapID	= 2;
	optional uint32 CharID	= 3;
	optional uint32 PosX	= 4;
	optional uint32 PosY	= 5;
	optional uint32 Level	= 6;
};

message CMessageSyncGTAndLWNotify
{
	repeated GTAndLWInfo GTInfo	= 1;
	repeated GTAndLWInfo LWInfo = 2;
};

message CMessageS2GBattleClose
{
	optional uint32 BattleID = 1;
	optional uint32 MapID	 = 2;
};

//进入军团boss副本
message CMessageS2GIntoCorpsBossRepetion
{
	optional uint32	BattleID	= 1;
	optional uint32	MapID		= 2;
	optional uint32 CharID		= 3;
	optional uint32 CorpsID		= 4;
	optional uint32 PostX		= 5;
	optional uint32 PostY		= 6;
};

//军团boss副本结束
message CMessgeS2GEndCorpsBossRepetion
{
	optional uint32 BattleID	= 1;
	optional uint32	MapID		= 2;
	optional uint32	CorpsID		= 3;
};

// 发布任务
//**********************************************
//网关回应的发布任务消息
// *******************************************

message PBGroupTasks
{
	optional uint32 TaskID			= 1;
	optional uint32 TaskState		= 2;
};

//  查看发布展板的请求  
message CMessageGateCheckGroupPublishRequest
{
	optional uint32	CharID			= 1;
	optional uint32 PublishType		= 2;
};


//  发送到网关  发布任务 请求网关记录
message CMessageGatePublishGroupRequest
{
	optional uint32 CharID			= 1;
	optional uint32 TaskID			= 2;
	optional uint32 PublishType		= 3;
	optional uint32 TaskState		= 4;
	optional uint32	PayGroupMoney	= 5;
};


//***************************************
//网关回应消息

message PBGroupInfo
{
	optional uint32 GroupID			= 1;
	optional uint32 GroupLevel		= 2;
	optional uint32 GroupMoney		= 3;
	optional uint32 PlayerPosition	= 4;
};

//网关回应  展板请求
message CMessageGateCheckGroupPublishResponse
{

	optional uint32 CharID			= 1;
	optional uint32 PublishType		= 2;
	repeated PBGroupTasks	Tasks	= 3;
	optional PBGroupInfo GroupInfo	= 4;
	optional uint32 Result			= 5;
};

// 网关回应  发布任务
message CMessageGatePublishGroupResponse
{
	optional uint32 CharID			= 1;
	optional uint32 TaskID			= 2;
	optional uint32 PublishType		= 3;
	optional uint32 TaskState		= 4;
	optional uint32 TaskAcion		= 5;
	optional uint32 Result			= 6;		// 网关回应  成功与否
};