import "servermessage_pb.hxx";
import "property_pb.hxx";

// 以最快速度序列/反序列
option optimize_for = SPEED;


enum LOG_MSG_ID
{	
	ID_S2L_LOGMESSAGE					= 0xC400;		// 场景服务器->日志服务器
	ID_L2L_HANDLETIMEOUT				= 0xC401;		// 通知每个HANDLE线程,计时存储时间到
	ID_S2L_ROLE_OPTASK					= 0xC402;		// 玩家任务事件
	ID_S2L_ROLE_KILLED					= 0xC403;		// 玩家死亡事件
	ID_S2L_ROLE_LOGOUT					= 0xC404;		// 玩家离开场景事件
	ID_S2L_ROLE_OPEXP					= 0xC405;		// 玩家获得或失去道具事件
	ID_S2L_ROLE_OPMONEY					= 0xC406;		// 获得获得或失去金钱事件
	ID_S2L_ROLE_OPITEM					= 0xC407;		// 玩家对物品的行为日志
	ID_S2L_ROLE_GHOST					= 0xC408;		// 玩家物品镜像
	ID_S2L_ROLE_UPGRADE					= 0xC409;		// 玩家升级事件
	ID_S2L_ITEM_UPGRADE					= 0xC40A;		// 物品升级事件
	ID_S2L_ORGE_DROPED					= 0xC40B;		// 怪物掉落
	ID_S2L_ITEM_DISPEAR					= 0xC40C;		// 物品消
	ID_S2L_FAMILY_ACTION				= 0xC40D;		// 家族操作
	ID_S2L_CORPS_ACTION					= 0xC40E;		// 军团操作
	ID_S2L_MAIL							= 0xC40F;		// 邮件日志	
    ID_S2L_SERVER_STAT_INFO_NOTICE		= 0xC410;		// 服务器统计日志
	ID_G2L_FAMILY_BATTLE				= 0xC411;		// 家族战
	ID_G2L_CORPS_BATTLE					= 0xC412;		// 军团战
	ID_G2L_ROLE_CHAT					= 0xC413;		// 聊天日志
	ID_G2L_REPETION						= 0xC414;
	ID_S2L_VALUABLE_EQUIP				= 0xC415;		// 贵重物品备份
	ID_S2L_BOSSORGEDIE					= 0xC416;		// boos怪死亡的记录
	ID_S2L_LIFESKILL					= 0xC417;		// 生活技能的记录
	ID_S2L_USEITEM_DROP					= 0xC418;		// 使用物品掉落
	ID_S2L_EXCHANGELOG_LOG				= 0xC479;
	ID_S2L_MATRIX						= 0xC47A;		// 仙缘
	ID_S2L_WORLD						= 0xC47B;		// 世界事件
	ID_S2L_ROLE_DAMAGE					= 0xC47C;		// 伤害
	ID_S2L_ROLE_BUFFER					= 0xC47D;       // 玩家获得或者解除buffer
	ID_S2L_ROLE_REPE_SCORE				= 0xC47E;		// 玩家副本积分记录

};

// ID_L2L_HANDLETIMEOUT
message CMessageHandleTimeOut
{

};

// ID_S2L_ROLE_LOGOUT	玩家登出日志
message CMessageLogRoleLogout
{
	optional uint32 RoleID		= 1;
	optional uint32 OpTime		= 2;
	optional int32  LineID		= 3;
	optional uint64 RepetionID	= 4;
	optional int32  MapID		= 5;
	optional int32  RoleLevel	= 6;
	optional int32  MetierID	= 7;
	optional uint32	LoginIP		= 8;
	optional uint32 LoginTime	= 9;
	optional uint32 LogoutTime	= 10;
	optional EMLeaveReason LeaveReason = 11;
	optional int32	SaveReason	= 12;
	optional EMSaveResult SaveResult = 13;
	optional int32 	KickReason	= 14;
	optional int32 	KickCode	= 15;
};

// 角色物品Ghost
enum EPackType
{
	EM_ROLE_EQUIPMENT	= 1;
	EM_ROLE_PACKAGE		= 2;
	EM_ROLE_PACKAGE_KITBAG	= 3;
	EM_ROLE_STORAGE		= 4;
	EM_ROLE_STORAGE_KITBAG 	= 5;
};

// ID_S2L_ROLE_GHOST
message CMessageLogRoleGhost
{
	optional uint32 RoleID 		= 1;
	optional uint32 OpTime		= 2;
	optional EPackType PackType	= 3; 
	//optional PBItemBox PackInfo	= 4;
};

// 角色升级
enum EUpgradeType
{
	EM_UPGRADE_ROLE_EXP	= 0;
	EM_UPGRADE_TALENT_EXP	= 1;
	EM_UPGRADE_LIFE_EXP	= 2;
	EM_UPGRADE_PRODUCE_EXP	= 3;
	EM_UPGRADE_MW_EXP	= 4;
};

enum EExpAction
{
	EM_EXP_BY_HORNOR		= 1;	// 荣誉转换为经验
	EM_EXP_BY_CARD			= 2;	// 卡片获得经验
	EM_EXP_BY_MULTICOM		= 3;	// 兑换获得经验
	EM_EXP_BY_COMMAND		= 4;	// GM 命令获得
	EM_EXP_BY_TEAM_ALLOT		= 5;	// 内丹获得经验
	EM_EXP_BY_FAMILY_ALLOT		= 6;	// 家族内丹
	EM_EXP_BY_QUESTION		= 7;	// 答题卷
	EM_EXP_BY_INVESTIGATION		= 8;	// 调查问卷
	EM_EXP_BY_ERATING		= 9;	// 天赐官奖励
	EM_EXP_BY_TASK			= 10;	// 任务奖励
	EM_EXP_BY_ENTITY_DIE		= 11; 	// 杀死怪物获得
	EM_EXP_BY_BUFF			= 12; 	// 持续加经验固定值BUFF
	EM_EXP_BY_FLOWER		= 13; 	// 送人鲜花获得经验
	EM_EXP_BY_EXPITEM		= 14;	// 使用经验道具
	EM_EXP_BY_BLACKPILLS	= 15;	// 使用黑狗丸
	EM_EXP_BY_EXPILLS	= 16;	// 使用经验球
    EM_EXP_BY_HANDIN        = 17;   // 上缴物品获取经验
	EM_EXP_BY_OFFLINE		= 18;	// 由离线获得经验
};

enum EBufferAction
{
	EM_BUFF_BY_SKILL			= 1;	// 技能获得BUFF
	EM_BUFF_BY_MAP				= 2;	// MAP给予BUFF
	EM_BUFF_BY_YQSGROW			= 3;	// DEBUG命令获得
	EM_BUFF_BY_BUFFITEM			= 4;	// 使用BUFF道具
	EM_BUFF_BY_SYSTEM		    = 5;	// 系统给予BUFF
    EM_BUFF_BY_TIMEOUT			= 6;    // buff到时
	EM_BUFF_BY_SELF				= 7;    // 自己去掉
	EM_BUFF_BY_DEATH			= 8;    // 死亡失去buff
	EM_BUFF_BY_CLOSEAUREOLE		= 9;    // 失去光环效果
    EM_BUFF_BY_CHANGEMAP		= 10;   // 换地图失去BUFF
    EM_BUFF_BY_LOGOFF			= 11;   // 登出失去BUFF
	EM_BUFF_BY_STATE_STONE      = 12;   // 交灵石失去buff

};

// 人物属性升级 ID_S2L_ROLE_UPGRADE
message CMessageLogRoleUpgrade
{
	optional uint32 RoleID			= 1;
	optional uint32 UpgradeTime		= 2;
	optional uint32 MetierID		= 3;
	optional uint32 RoleLevel		= 4;
	optional EUpgradeType UpgradeType	= 5;
	optional uint32	SubType			= 6;
	optional uint32 CurrentLevel		= 7;
	optional uint32 DeltaLevel 		= 8;
};



// 装备升级 ID_S2L_ITEM_UPGRADE
message CMessageLogItemUpgrade
{
	optional uint32 ItemID 			= 1;	// 物品ID
	optional uint64 ItemGUID		= 2;	// 物品GUID
	optional uint32 UpTime			= 3;	// 升级时间
	optional uint32 UpType			= 4;	// 升级类型
	optional uint32 Result			= 5;	// 升级结果
	optional uint32 RoleID			= 6;	// 角色ID
	optional uint32 PrevLevel		= 7;	// 升级前等级
	optional uint32 CurrLevel		= 8; 	// 升级后等级
	optional uint32 Item1			= 9;	// 物品1 
	optional uint32 Item2			= 10; 	// 物品2
};

// 物品日志 
enum EItemAction
{
	EM_GET_BY_IBSTORE	= 1; 	// 从商城获得物品
	EM_GET_BY_RECV_MAIL	= 2;	// 收邮件获得物品
	EM_GET_BY_STALL_BUY	= 3;	// 通过其他玩家摊位买到物品
	EM_GET_BY_EXCHANGE	= 4;	// 通过玩家间交换得到
	EM_GET_BY_ACCEPT_TASK	= 5;	// 接受任务得到物品
	EM_GET_BY_END_TASK	= 6;	// 交付任务得到物品
	EM_GET_BY_ERATING	= 7;	// 从活动NPC处得到物品
	EM_GET_BY_PICKITEM	= 8;	// 从地上捡起物品
	EM_GET_BY_PRODUCE	= 9;	// 玩家生产得到物品
	EM_GET_BY_CHANGE	= 10;	// 物品转化后获得
	EM_GET_BY_RUN_TASK	= 11;	// 进行任务中获得物品
	EM_GET_BY_NPC_SHOP	= 12;	// 通过从NPC处购买获得
	EM_GET_BY_DEFAULT	= 13;	// 默认初始装备
	EM_GET_BY_TASKITEM	= 14;	// 任务物品获得
	EM_GET_BY_COLLECT	= 15;	// 采集获得
	EM_GET_BY_COMPOSE	= 16;	// 合成获得
	EM_GET_BY_REPETION	= 17;	// 副本奖励
	EM_GET_BY_GMINSERT	= 18;	// GM INSERT
	EM_GET_BY_ROLLBACK	= 19;	// 操作失败后补偿给玩家
	EM_GET_BY_DIRECTINS	= 20;	// 开宝鼎直接获得
	EM_GET_BY_UNBOUND	= 21;	// 接触绑定获得宝石
	EM_GET_BY_DECOMPOUND	= 22;	// 分解后获得
	EM_GET_BY_INVESTIGATION	= 23;	// 调查问卷奖励
	EM_GET_BY_GUAJI_SHOP	= 24;	// 挂机购买
	EM_GET_BY_BUY_BACK	= 25;	// 从NPC处回购物品
	EM_GET_BY_AWARD		= 26;	// 领取奖励
	EM_GET_BY_GETSTAR	= 27;	// 摘星
	EM_GET_BY_HONOR = 28;			// 荣誉兑换
	EM_GET_BY_CARVE	= 29;		// 雕琢获得
	EM_GET_BY_QLCOMPOSE	= 30;		// 法宝器灵合成获得
	EM_GET_BY_TREASUREBOWL = 32;	// 聚宝获得物品
	EM_GET_BY_MWQUICKUP = 33;	// 法宝飞升获得物品
	EM_GET_BY_SPIRIT_DOWNGRID = 34; // 退灵获得物品

	EM_LOST_BY_SEND_MAIL	= 50;	// 发送邮件失去物品
	EM_LOST_BY_STALL_SELL	= 51;	// 通过摆摊卖掉物品
	EM_LOST_BY_EXCHANGE	= 52;	// 通过玩家间交易失去
	EM_LOST_BY_ACCEPT_TASK	= 53;	// 接受任务失去物品
	EM_LOST_BY_END_TASK	= 54;	// 交付任务失去物品
	EM_LOST_BY_ERATING	= 55;	// 从活动NPC处失去物品
	EM_LOST_BY_THROW_ITEM	= 56;	// 玩家丢弃销毁物品
	EM_LOST_BY_PRODUCE	= 57;	// 玩家通过生产失去材料
	EM_LOST_BY_USE		= 58;	// 使用后失去物品
	EM_LOST_BY_OPEN_SLOT	= 59;	// 开槽后失去物品
	EM_LOST_BY_COMPOSE	= 60;	// 合成后失去物品
	EM_LOST_BY_EAT		= 61;	// 吃掉物品化成元气
	EM_LOST_BY_INSERT_SLOT	= 62;	// 使用宝石镶嵌物品
	EM_LOST_BY_UPGRADE	= 63;	// 精炼物品
	EM_LOST_BY_CHANGE	= 64;	// 转化后失去
	EM_LOST_BY_DECOMPOUND	= 65;	// 分解后失去
	EM_LOST_BY_SELL2NPC	= 66;	// 卖给NPC失去
	EM_LOST_BY_JUDGE	= 67;	// 鉴定后失去
	EM_LOST_BY_BOUND	= 68;	// 滴血后失去
	EM_LOST_BY_ABORTTASK	= 69;	// 放弃任务后失去
	EM_LOST_BY_DESTROY	= 70;	// 玩家直接销毁物品
	EM_LOST_BY_TIMEOUT	= 71;	// 限时物品时间到了系统收回
	EM_LOST_BY_CHANGEEQUIP = 72;	// 转换装备
	EM_LOST_BY_REPETION	= 73;		// 副本钥匙
	EM_LOST_BY_GETSTAR	= 74;		// 摘星
	EM_LOST_BY_ADDSTAR	= 75;		// 加星
	EM_LOST_BY_ADDEXTRA = 76;		// 刻印
	EM_LOST_BY_FANGCUN = 77;		// 学习方寸山技能	
	EM_LOST_BY_MASTER = 78;			// 解除拜师失去
	EM_LOST_BY_CARVE = 79;			// 雕琢失去
	EM_LOST_BY_MWQUICKUP = 80;	// 法宝飞升失去物品
	EM_LOST_BY_QLCOMPOSE	= 81;		// 法宝器灵合成失去
	EM_START_USE_TIME_ITEM = 82;    //开始使用限时道具
	EM_LOST_BY_SWORN		= 83;	// 结拜失去道具
};

enum EMoneyAction
{
	EM_GET_BY_SELL2NPC	= 1;	// 卖给NPC
	EM_GET_BY_PICKMONEY	= 2;	// 从地上捡起
	EM_GET_BY_EXCHANGE_MONEY= 3;	// 交易获得
	EM_GET_BY_GMINSERT_MONEY= 4;	// GM插入
	EM_GET_BY_STORE_MONEY	= 5;	// 从仓库支取金钱
	EM_GET_BY_RED_BAG	= 6;	// 读取红包
	EM_GET_BY_RECV_MONEY	= 7;	// 收邮件得到
	EM_GET_BY_QUESTLIB	= 8;	// 每日答题
	EM_GET_BY_ERATING_MONEY = 9;	// Erating 奖励
	EM_GET_BY_CORPS		= 10; 	// 从军团获取
	EM_GET_BY_FAMILY	= 11;	// 从家族获取
	EM_GET_BY_STALL		= 12;	// 摆摊获得
	EM_GET_BY_TASK		= 13;	// 从任务奖励获得
	EM_GET_BY_GIFT		= 14;	// 礼包获得
	EM_GET_BY_WAR		= 15;	// 国战领奖获得
	EM_GET_BY_BOURSE	= 16;	// 从元宝交易所中获得
	EM_GET_BY_BOURSE_ERR = 17;	// 元宝交易所存钱失败
	EM_GET_BY_TELFAIL_ROLLBACK = 18; // 传送失败系统回滚
	EM_GET_BY_FAME	= 19;		// 副本获得

	EM_LOST_BY_SEND_MONEY	= 50;	// 邮件
	EM_LOST_BY_BUY4NPC 	= 51;	// 从NPC处购买
	EM_LOST_BY_REPAIR	= 52;	// 修理装备
	EM_LOST_BY_PRODUCE_EQUIP= 53;	// 生产装备
	EM_LOST_BY_PRODUCE_PHYS	= 54;	// 生产药品
	EM_LOST_BY_UPGRADE_EQUIP= 55;	// 升级装备
	EM_LOST_BY_INSERT_RUNE	= 56;	// 镶嵌符文
	EM_LOST_BY_INSERT_JEWEL	= 57;	// 镶嵌宝石
	EM_LOST_BY_OPEN_SLOT2	= 58;	// 装备开槽
	EM_LOST_BY_STUDY_TALENT	= 59;	// 学习天赋
	EM_LOST_BY_EXCHANGE_MONEY=60;	// 交换失去
	EM_LOST_BY_MONEY_RELIFE	= 61;	// 金钱复活
	EM_LOST_BY_NPC_TELEPORT = 62;	// NPC传送
	EM_LOST_BY_BUY_BACK	= 63;	// 回购物品
	EM_LOST_BY_BIND_ITEM	= 64;	// 绑定物品
	EM_LOST_BY_UNBIND_ITEM	= 65;	// 解除绑定
	EM_LOST_BY_STUDY_SKILL	= 66;	// 学习技能
	EM_LOST_BY_STORE_MONEY	= 67;	// 向仓库存钱
	EM_LOST_BY_RESUME_BIND	= 68;	// 恢复绑定
	EM_LOST_BY_REMOVE_JEWEL	= 69;	// 摘除宝石
	EM_LOST_BY_JUDGE_EQUIP  = 70;	// 鉴定装备
	EM_LOST_BY_CHANGE_RESIST= 71;	// 转换抗性
	EM_LOST_BY_COMPOSE_JEWEL= 72;	// 合成宝石
	EM_LOST_BY_MULTI_COMPOSE= 73;	// 兑换
	EM_LOST_BY_UP_LIFESKILL = 75;	// 玩家升级生活技能消费
	EM_LOST_BY_STUDY_COMPOSE= 76;	// 学习配方
	EM_LOST_BY_JUDGE_FABAO  = 77;	// 鉴定法宝
	EM_LOST_BY_GUAJI_BUY    = 78;	// 挂机买药
	EM_LOST_BY_POSTAL_FEE	= 79;	// 发送邮件的邮资
	EM_LOST_BY_BUILD_CORPS  = 80;	// 创建军团
	EM_LOST_BY_CONTR_CORPS 	= 81;	// 向军团捐献
	EM_LOST_BY_BUILD_FAMILY	= 82;	// 将建家族
	EM_LOST_BY_CONTR_FAMILY	= 83;	// 向家族捐献
	EM_LOST_BY_STALL	= 84;	// 从摆摊处失去
	EM_LOST_BY_EQUIPCHANGE	= 85; // 装备转换
	EM_LOST_BY_ADD_EXTRA		= 86; // 刻印
	EM_LOST_BY_WAR		= 87; // 报名国战
	EM_LOST_BY_BOURSE	= 88; // 元宝交易所
	EM_LOST_BY_TAX		= 89; // 扣税	
	EM_LOST_BY_STALLFEE = 90; //摆摊交易手续费
	EM_LOST_BY_MASTERMONEY	= 91; // 拜师失去
	EM_LOST_BY_CARVEMONEY	= 92;	// 雕琢失去		
	EM_LOST_BY_DECOMPOS	= 93;	// 分解装备/法宝后失去
	EM_LOST_BY_MWPRINT	= 94;	// 法宝印记后失去
	EM_LOST_MONEY_BY_QLCOMPOSE	= 95;	// 法宝器灵合成失去
	EM_LOST_BY_BUY4FUNC		= 96;		//功能付费
	EM_LOST_BY_FAME			= 97;		// 声望失去
	EM_LOST_BY_SPIRIT_UPGRADE  = 98;   // 聚灵失去
	EM_LOST_BY_SPIRIT_DOWNGRADE  = 99;   // 退灵失去
};

// 物品 ID_S2L_ROLE_OPITEM
message CMessageLogRoleOpItem
{
	optional uint32 RoleID			= 1;	// 玩家CHARID
	optional uint32 OpTime			= 2;	// 时间
	optional uint32 LineID			= 3;	// 线ID
	optional uint64 RepetionID		= 4;	// 副本ID
	optional uint32 MapID			= 5;	// 地图ID
	optional uint32	MetierID		= 6;	// 职业
	optional uint32 RoleLevel		= 7;	// 等级
	optional uint32 ItemID			= 8;	// 物品模板ID
	optional uint32 ItemNum			= 9;	// 物品数量
	optional uint64 ItemGUID		= 10;   // 物品的GUID
	optional uint32 ItemTotal		= 11;	// 操作之后此物品的总数量 
	optional uint32 OpType			= 12;   // 行为
	optional int32  OpParam1		= 13;   // 整型行为参数1
	optional int32  OpParam2		= 14;   // 整型行为参数2
	optional uint64 OpParam3		= 15;   // 整型行为参数3
};

//经验 ID_S2L_ROLE_OPEXP
message CMessageLogRoleOpExp
{
	optional uint32 RoleID			= 1;	// 玩家CHARID
	optional uint32 OpTime			= 2;	// 时间
	optional uint32 LineID			= 3;	// 线ID
	optional uint64 RepetionID		= 4;	// 副本ID
	optional uint32 MapID			= 5;	// 地图ID
	optional uint32	MetierID		= 6;	// 职业
	optional uint32 RoleLevel		= 7;	// 等级
	optional EUpgradeType ExpType		= 8;	// 经验类型
	optional uint32 OpType			= 9;	// 获得方式
	optional uint64 CurExp			= 10;	// 当前值
	optional uint32 Team			= 11;	// 当前队伍人数
	optional uint32 DeltaExp 		= 12;	// 改变值
	optional int32  OpParam1		= 13;   // 整型行为参数1
	optional int32  OpParam2		= 14;   // 整型行为参数2
	optional uint64 OpParam3		= 15;   // 64位整型行为参数3
};


// BUFFER ID_S2L_ROLE_BUFFER
message CMessageLogRoleBuffer
{
	optional uint32 RoleID			= 1;	// 玩家CHARID
	optional uint32 OpTime			= 2;	// 时间
	optional uint32 LineID			= 3;	// 线ID
	optional uint32 MapID			= 4;	// 地图ID
	optional uint32 PersistTime		= 5; 	// 持续时间
	optional uint32 OpType			= 6;	// 获得BUFF的方式
    optional uint32 BuffID			= 7;    // BuffID
	optional int32  OpParam1		= 8;    // 整型行为参数1
	optional int32  OpParam2		= 9;    // 整型行为参数2
};



// 金钱 ID_S2L_ROLE_OPMONEY
message CMessageLogRoleOpMoney
{
	optional uint32 RoleID			= 1;	// 玩家CHARID
	optional uint32 OpTime			= 2;	// 时间
	optional uint32 LineID			= 3;	// 线ID
	optional uint64 RepetionID		= 4;	// 副本ID
	optional uint32 MapID			= 5;	// 地图ID
	optional uint32	MetierID		= 6;	// 职业
	optional uint32 RoleLevel		= 7;	// 等级
	optional uint32 MoneyType 		= 8; 	// 金钱类型
	optional uint32 OpType			= 9;	// 获得金钱的方式
	optional int32  CurMoney		= 10;	// 当前金钱
	optional int32  DeltaMoney 		= 11;	// 金钱改变量
	optional int32  OpParam1		= 12;   // 整型行为参数1
	optional int32  OpParam2		= 13;   // 整型行为参数2
	optional uint64 OpParam3		= 14;   // 整型行为参数3
};


// 玩家死亡 ID_S2L_ROLE_KILLED
message CMessageLogRoleBeKilled
{
	optional uint32 RoleID			= 1;	// 玩家CHARID
	optional uint32 OpTime			= 2;	// 时间
	optional uint32 LineID			= 3;	// 线ID
	optional uint64 RepetionID		= 4;	// 副本ID
	optional uint32 MapID			= 5;	// 地图ID
	optional uint32	MetierID		= 6;	// 职业
	optional uint32 RoleLevel		= 7;	// 等级
	optional uint32 KillerType 		= 8;	// 对方类型
	optional uint32 KillerID		= 9;	// 对方ID
	optional uint32 KillerMetier	= 10;	// 对方职业ID
	optional uint32 KillerLevel		= 11;	// 对方等级
};

// 任务
enum ETaskAction
{
	EM_TASK_ACCEPT 				= 1;	// 接受任务
	EM_TASK_END 				= 2;	// 交付任务
	EM_TASK_CANCEL 				= 3;	// 放弃任务
	EM_TASK_ACTION				= 4;	// 任务动作（进行中）
	EM_TASK_REFRESH				= 5;    // 任务刷新奖励
	EM_TASK_PUBLISH				= 6;	// 发布任务
};

// ID_S2L_ROLE_OPTASK
message CMessageLogRoleOpTask
{
	optional uint32 RoleID			= 1;	// 玩家CHARID
	optional uint32 OpTime			= 2;	// 时间
	optional uint32 LineID			= 3;	// 线ID
	optional uint64 RepetionID		= 4;	// 副本ID
	optional uint32 MapID			= 5;	// 地图ID
	optional uint32	MetierID		= 6;	// 职业
	optional uint32 RoleLevel		= 7;	// 等级
	optional uint32 TaskID			= 8; 	// 任务
	optional ETaskAction OpType		= 9;	// 操作方式
	optional int32  OpParam1		= 10;   // 整型行为参数1
	optional int32  OpParam2		= 11;   // 整型行为参数2
	optional uint64 OpParam3		= 13;   // 整型行为参数3
};

message PBDropedItem
{
	optional uint32 ItemID			= 1;	// 掉落物品模板ID
	optional uint32 ItemType		= 2;	// 掉落的物品类型
	optional uint32 ItemLevel 		= 3;	// 掉落物品的等级
};

// NPC 掉落 ID_S2L_ORGE_DROPED
message CMessageLogOrgeDropItem
{
	optional int32	NpcID			= 1;	// NpcID
	optional int32  NpcIndex		= 2;	// Npc的部署ID
	optional int32  NpcLevel		= 3;	// Npc等级
	optional uint32 OpTime			= 4;	// 时间
	optional uint32 LineID			= 5;	// 线ID
	optional uint64 RepetionID		= 6;	// 副本ID
	optional uint32 MapID			= 7;	// 地图ID
	repeated PBDropedItem DropItem		= 8;	// 掉落的物品
};

// 玩家使用物品后掉落 
message CMessageLogUseItemDropItem
{
	optional uint32	CharID			= 1;	// CharID
	optional int32  ItemID			= 2;	// 物品ID
	optional int32	Param1			= 3;	// 参数1
	optional int32	Param2			= 4;	// 参数2
	optional uint32 OpTime			= 5;	// 时间
	optional uint32 LineID			= 6;	// 线ID
	optional uint64 RepetionID		= 7;	// 副本ID
	optional uint32 MapID			= 8;	// 地图ID
	repeated PBDropedItem DropItem	= 9;	// 掉落的物品
};

// 道具自然销毁
message CMessageLogItemDestroy
{
	optional int32	ItemID			= 1;	// Item ID
	optional int32	Level			= 2;	// 等级	
	optional uint32 OpTime			= 4;	// 时间
	optional uint32 LineID			= 5;	// 线ID
	optional uint64 RepetionID		= 6;	// 副本ID
	optional uint32 MapID			= 7;	// 地图ID
	optional uint32 PosX			= 8;	// X
	optional uint32 PosY			= 9;	// Y
};

// 家族属性枚举
enum  EFamilyAttr
{
	EM_FAMILY_NULL			= 0;	// 无
	EM_FAMILY_MEMBER 		= 1;	// 成员数
	EM_FAMILY_MONEY  		= 2;	// 金库
	EM_FAMILY_CHANLENGE 	= 3; 	// 挑战赛
	EM_FAMILY_BATTLE    	= 4; 	// 争霸赛
	EM_FAMILY_CREATION  	= 5;	// 成立状态
	EM_FAMILY_CONTRIBUTION 	= 6;	// 贡献度
	EM_FAMILY_LEVEL 		= 7; 	// 等级
	EM_FAMILY_RANK			= 8;	// 职位
	EM_FAMILY_GLORY			= 9;	// 经验
};

// 军团属性
enum  ECorpsAttr
{
	EM_CORPS_NULL			= 0;	// 无
	EM_CORPS_MEMBER 		= 1;	// 成员数
	EM_CORPS_MONEY  		= 2;	// 金库
	EM_CORPS_CHANLENGE		= 3; 	// 挑战赛
	EM_CORPS_BATTLE			= 4; 	// 争霸赛
	EM_CORPS_CREATION		= 5;	// 成立状态
	EM_CORPS_CONTRIBUTION 	= 6;	// 贡献度
	EM_CORPS_LEVEL 			= 7; 	// 等级
	EM_CORPS_RANK			= 8;	// 职位	
	EM_CORPS_GLORY			= 9;	// 经验
	EM_CORPS_REFINEVALUE	= 10;	// 炼化度
	EM_CORPS_SCORE			= 11;	// 军团积分
	EM_CORPS_ACTIVITY		= 12;	// 活跃度
};

// 操作类型
enum EOrganAction
{
	EM_ORGAN_MEMBERJOIN			= 1;		// 成员加入
	EM_ORGAN_MEMBERLEAVE		= 2;		// 成员离开
	EM_ORGAN_CREATE				= 3;		// 家族(军团)成立
	EM_ORGAN_DISBAND			= 4;		// 家族(军团)解散
	EM_ORGAN_GETMONEY			= 5;		// 获取家族(军团)金钱
	EM_ORGAN_CONTRIBUTEMONEY	= 6;		// 贡献家族(军团)金钱
	EM_ORGAN_BUYITEM			= 7;		// 购买家族(军团)物品(红包)
	EM_ORGAN_LEVELUP			= 8;		// 家族(军团)升级
	EM_ORGAN_SETPOST			= 9;		// 家族(军团)职位设定			
	EM_ORGAN_OPEN_ROOM 			= 10;		// 开房间(报名)
	EM_ORGAN_CLOSE_ROOM 		= 11;		// 关房间(取消报名)
	EM_ORGAN_SEND_CHALLEGE  	= 12;		// 发起挑战
	EM_ORGAN_REC_CHALLEGE		= 13;		// 接受挑战
	EM_ORGAN_BID_				= 14;		// 竞价
	EM_ORGAN_SUBMIT				= 15;		// 报名
	EM_ORGAN_JOIN_BATTLE	  	= 16;		// 开战 
	EM_ORGAN_GETNPCWELFARE		= 17;		// 获取家族(军团)npc福利
	EM_ORGAN_TASK				= 18;		// 家族(军团)任务
	EM_ORGAN_EXCHANGE			= 19;		// 家族(军团)兑换	
	EM_ORGAN_CONFIRM			= 20;		// 确定
	EM_ORGAN_FIN				= 21;		// 结束
	EM_ORGAN_ABORT				= 22;		// 放弃	
	EM_ORGAN_REPETION			= 23;		// 家族副本
	EM_ORGAN_BATTLEVICTORY		= 24;		// 争霸赛胜利
	EM_ORGAN_CHALLENGEACCEPT	= 25;		// 接受挑战	
	EM_ORGAN_REDSTONE			= 26;		// 领取家族内丹
	EM_ORGAN_TAX				= 27;		// 领税
	EM_ORGAN_USEFUNCITEM		= 28;		// 使用功能性物品
	EM_ORGAN_SUMMONCORPSBOSS	= 29;		// 召唤军团boss
	EM_ORGAN_CORPSREPETION		= 30;		// 完成军团副本
};

enum EWorldAction
{
	EM_WORLD_ACCUSEBEGIN		= 1;		// 弹劾开始
	EM_WORLD_ACCUSEVOTE			= 2;		// 弹劾投票
	EM_WORLD_ACCUSEEND			= 3;		// 弹劾结束
	EM_WORLD_POWERADD			= 4;		// 声望增加
	EM_WORLD_POWERDEL			= 5;		// 声望减少
	EM_WORLD_FORCE				= 6;		// 直接下台
};

message CMessageLogFamily
{
	optional uint32 FamilyID		= 1;		// 家族ID
	optional uint32 OpRoleID		= 2;		// 操作人ID
	optional uint32 OpTime			= 3;		// 操作时间
	optional EOrganAction OpType	= 4;		// 行为
	optional EFamilyAttr AttrID		= 5;		// 行为修改的属性
	optional uint32	Param1			= 6;		// 参数1
	optional uint32 Param2			= 7;		// 参数2
	optional uint32 Param3			= 8;		// 参数3
};

message CMessageLogCorps
{
	optional uint32 CorpsID			= 1;		// 军团ID
	optional uint32 OpRoleID		= 2;		// 操作人ID
	optional uint32 OpTime			= 3;		// 操作时间
	optional EOrganAction OpType	= 4;		// 行为
	optional ECorpsAttr AttrID		= 5;		// 行为修改的属性
	optional uint32	Param1			= 6;		// 参数1
	optional uint32 Param2			= 7;		// 参数2
	optional uint32 Param3			= 8;		// 参数3
};

message CMessageLogWorld
{
	optional uint32 WorldID			= 1;		// 阵营ID
	optional uint32 OpTime			= 2;		// 操作时间
	optional uint32	OpType			= 3;		// 行为
	optional uint32	Param1			= 4;		// 参数1
	optional uint32 Param2			= 5;		// 参数2
	optional uint32 Param3			= 6;		// 参数3
};


enum EM_MAILACTION
{ 
	EM_MAIL_SENDMAIL	= 1;		// 发送邮件
	EM_MAIL_RECEIVEMAIL	= 2;		// 收邮件
	EM_MAIL_DELMAIL		= 3;		// 删除邮件
};


// 邮件的日志
message CMessageLogMail
{
	optional uint32 RoleID				= 1;	// 邮件操作者角色ID
	optional uint32 MailID				= 2;	// 邮件ID	
	optional uint32 OpType				= 3;	// 操作类型
	optional uint32 Optime				= 4;	// 操作时间
	optional uint32	MailType			= 5;	// 邮件类型
	optional string	MailTitle			= 6;	// 邮件标题
	optional uint32	TotalNum			= 7;	// 当前邮件总数
	optional uint32	ItemID1				= 8;	// 邮件附件物品1的ID		
	optional uint32	ItemNum1			= 9;	// 邮件附件物品1的数量
	optional uint32	ItemID2				= 10;	// 邮件附件物品2的ID		
	optional uint32	ItemNum2			= 11;	// 邮件附件物品2的ID
	optional uint32	ItemID3				= 12;	// 邮件附件物品3的ID		
	optional uint32	ItemNum3			= 13;	// 邮件附件物品3的ID
	optional uint32	ItemID4				= 14;	// 邮件附件物品4的ID		
	optional uint32	ItemNum4			= 15;	// 邮件附件物品4的ID
	optional uint32	ItemID5				= 16;	// 邮件附件物品5的ID		
	optional uint32	ItemNum5			= 17;	// 邮件附件物品5的ID
	optional uint32	Money				= 18;	// 携带的附件金钱
	optional uint32	MoneyNeedCallBack	= 19;	// 需要对方支付的金钱
	optional uint32	DesRoleID			= 20;	// 邮件相关的另外一方的角色ID
	optional string SrcName				= 21;	// 邮件发送者的名称
};


// *************************************************************************** //
// CMessageServerStatInfoNotify
// 服务器统计信息
// *************************************************************************** //
enum EM_STAT_CATALOG
{
	EM_SERVER_MONEY = 1;	// 个人金钱
	EM_SERVER_EXP	= 2;	// 个人经验
	EM_SERVER_ITEM	= 3;	// 个人物品
	EM_SERVER_EVENT	= 4;	// 个人行为
	EM_SERVER_FAMILY= 5;	// 家族属性 
	EM_SERVER_CORPS	= 6;	// 军团属性
};

message PBAttribute
{
	optional uint32 ID			= 1;	// 属性ID
	optional int64	Value			= 2;	// 属性取值
};

message PBCatalog
{
	optional uint32 ID			= 1;	// 属性组ID
	repeated PBAttribute Attr		= 2;	// 属性
};

message CMessageServerStatInfoNotify
{
	optional uint32 ServerID		= 1;	// 统计的ServerID
	repeated PBCatalog StatInfo		= 2;	// 统计信息
};

message CMessageLogValuableEquip
{
	optional uint32 RoleID		= 1;	// 角色ID
	repeated PBItemObject	ItemObj	= 2;	// 物品信息 
};



// *************************************************************************** //
// CMessageLogChat
// 服务器统计信息
// *************************************************************************** //
message CMessageLogChat 
{
	optional uint32 RoleID		= 1; // 角色ID
	optional string ChatMsg		= 2; // 聊天内容
	optional uint32 GateWayID	= 3; // 网关ID
	optional uint32 Channel		= 4; // 频道
	optional uint32 OpTime		= 5; // 发言时间
	optional uint32 IP			= 6; // 发言者IP
	optional string RoleName	= 7; // 角色姓名
};


message CMessageLogRepetion
{
	optional uint64 RepetionKey = 1;
	optional uint32	Type		= 2;
	optional uint32 Index		= 3;
	optional uint32 LineID		= 4;
	optional uint32 Parame1		= 5;
	optional uint32	Parame2		= 6;
	optional uint32 OpTime		= 7;
};

// boss怪死亡的记录
message CMessageLogBossOgreDie
{
	optional uint32	DeathTime	=	1;		// 死亡时间
	optional uint32	CampID		=	2;		// 阵营ID
	optional uint32	MapID		=	3;		// 地图ID
	optional uint32	PosX		=	4;		// 死亡时的横坐标
	optional uint32	PosY		=	5;		// 死亡时的纵坐标
	optional uint32	TempID		=	6;		// 怪物模板ID
	optional uint32	KillerID	=	7;		// 怪物归属玩家的角色ID
	optional string	DamageInfo	=	8;		// 伤害信息	
};

// 生活技能的信息
enum EM_LIFESKILL_ACTIONKIND
{
	EM_LIFESKILLACTION_INITIAL			=	1;		// 学习某项生活技能
	EM_LIFESKILLACTION_STUDYCOMPOSE		=	2;		// 学习配方
	EM_LIFESKILLACTION_LEVELUP			=	3;		// 生活技能进阶
	EM_LIFESKILLACTION_FORGETLIFESKILL	=	4;		// 遗忘生活技能
	EM_LIFESKILLACTION_CREATCOMPOSE		=	5;		// 创造配方
	EM_LIFESKILLACTION_USECOMPOSESCROLL	=	6;		// 使用配方卷轴学习配方
};

message CMessageLifeSkill
{
	optional uint32	RoleID				=	1;		// 角色ID
	optional uint32	OpTime				=	2;		// 操作时间
	optional uint32	LifeSkillType		=	3;		// 生活技能类型(EM_LIFESKILL)
	optional uint32	LifeSkillKind		=	4;		// 主修还是辅修(EM_LifeSkillKind)			
	optional uint32	LifeSkillLevel		=	5;		// 生活技能的等级
	optional uint32	SpecializedDegree	=	6;		// 生活技能熟练度
	optional uint32	ActionKind			=	7;		// 操作类型	
	optional uint32	ComposeID			=	8;		// 配方ID
};

message CMessageExchangeLog
{
	optional uint32 account_id			= 1;
	optional uint32 role_id				= 2;
	optional uint32 optime				= 3;
	optional uint32 money				= 4;
	optional uint32 yb					= 5;
	optional uint32 operate				= 6;
	optional uint64 detail_id_1			= 7;
	optional uint64 detail_id_2			= 8;
	optional uint32 parame1				= 9;
	optional uint32 parame2				= 10;
	optional uint32 parame3				= 11;
	optional uint32 parame4				= 12;	
};

enum EMatrixAction
{
	EM_MATRIX_ADDSLAVE					= 1;		// 师傅收徒
	EM_MATRIX_ADDMASTER					= 2;		// 徒弟拜师
	EM_MATRIX_ADDPOINT					= 3;		// 返点
	EM_MATRIX_DELSLAVE					= 4;		// 师傅解除
	EM_MATRIX_DELMASTER					= 5;		// 徒弟解除
	EM_MATRIX_MASTERPERCENT				= 6;		// 分配比例
};

message CMessageLogMatrix
{
	optional uint32	slave_id			= 1;
	optional uint32	master_id			= 2;
	optional uint32 optime				= 3;
	optional uint32 operate				= 4;
	optional uint32 parame1				= 5;
	optional uint32 parame2				= 6;
	optional uint32 parame3				= 7;
};

enum ERoleDamage
{
	EM_DAMAGE_NORMAL_MAX				= 8000;
	EM_DAMAGE_DEATH_MAX					= 20000;
	EM_DAMAGE_NORMAL_MIN				= 1000;
	EM_DAMAGE_DEATH_MIN					= 3000;
	EM_MIN_LEVEL						= 70;
};

message CMessageLogRoleDamage
{
	optional uint32	srccharid			= 1;
	optional uint32	descharid			= 2;
	optional uint32	optime				= 3;
	optional uint32	skillid				= 4;
	optional uint32	calcdamage			= 5;
	optional uint32	realdamage			= 6;
	optional uint32	srcpatt				= 7;
	optional uint32	srcmatt				= 8;
	optional uint32	despdef				= 9;
	optional uint32	desmdef				= 10;
	optional uint32	despimu				= 11;
	optional uint32	desmimu				= 12;
	optional uint32	srcdhit				= 13;
	optional uint32	desddef				= 14;
	optional uint32	srctitle			= 15;
	optional uint32 destitle			= 16;
	optional string	srcres				= 17;
	optional string desres				= 18;	
	optional string	srcbuff				= 19;
	optional string desbuff				= 20;
};

// 副本积分日志

enum ERoleRepeScore
{
	EM_SCORE_COMMON_ORGE				= 1;
	EM_SCORE_BOSS_ORGE					= 2;
	EM_SCORE_EXTRAL						= 3;
	EM_SCORE_KILL_PLAYER				= 4;
};

message CMessageLogRoleRepeScore
{
	optional uint32	charid				= 1;
	optional uint32	lineid				= 2;
	optional uint32	optime				= 3;
	optional uint32	mapid				= 4;
	optional uint32	repeid				= 5;
	optional uint32	currentscore		= 6;
	optional uint32	deltascore			= 7;
	optional uint32	scoretype			= 8;
};