#pragma once
#ifndef NAME_LENGTH
#define NAME_LENGTH 32
#endif
#include <string.h>
#include "errcode.h"
#include "template.h"

#ifdef LINUX
#include "lk_assert.h"
#endif

#define  MAXBAGINDEX 24
#define  MAXEQUIPMENTINDEX 14
#define  BINDITEM_CONSUMENUM 1
#define  BINDSTONEID     26338    

// 新手帮助需要的整数的个数
#define  MAX_HELPACTON_INT_NUM 6

// 一次发送的最大仓库物品数量
#define  MAX_STORAGEITEM    30
  
// 队伍的人数上限
#define TEAMNUM 6

// 绑定解除的物品数量
#define   ITEM_FREE_COUNT   50	

// 取消手动绑定需要的金钱
#define	  MONEY_RESUMEBIND_NEED 200	

// 金钱改变量上限
#define MONEY_CHANGE_MAX 1000

// NPC最后一次售出的道具
#define ITEM_LASTSELL	-1	

// 服务器的所有百分比用万分比代替
#define SERVER_PERCENT_FLOAT 10000.0f
#define SERVER_PERCENT_INT 10000 
#define SERVER_DIPLOID_EXP 2

#define BOURSE_ACCOUNT_MONEY_MAX		( 900000000 )
#define BOURSE_ACCOUNT_YB_MAX			( 900000000 )
#define BOURSE_EXCHANGE_AMOUNT_MAX		( 900000000 )


// 一周时间
#define WEEK_SECOND (7*24*60*60)

// 玩家身上的双倍上限
#define DIPLOID_LIMIT ( 4 * 60 * 60 * 1000 )

// 技能公共冷却时间
#define SKILL_COMMON_COOLDOWN 1000

// 道具保护时间
#define ITEM_PROTECT_TIME	60000 // 1分钟

// 道具消失时间
#define ITEM_DISAPPER_TIME	120000 // 2分钟

// 内丹保护时间
#define REDSTONE_PROTECT_TIME 30000 // 30秒

// 内丹消失时间
#define  REDSTONE_DISAPPER_TIME 120000

// 内丹读秒超时时间
#define  REDSTONE_PROCESSTIMER_OUTTIME 5000	// 5秒

// 属性模块protobuff长度
#define MAX_PRO_PB_DATA_LEN 45000	

// npc 处答题的最大题目数量
#define  MAX_NPC_QUESTNUM			10 

// 采集NPC 有效格子数(玩家距离植物的距离要小于等于这个值)
#define MAX_COLLECT_GRID			2

// 绑定解除的时间
#define ITEMBINDFREETIME       ( 7*24*3600 )

// 领取每日福利的最低等级					
#define WELFARE_LEVLE_NEED		10					

// 功能NPC死亡的消失/复活时间(毫秒)
#define FUNC_NPC_DISAPPEAR		2000
#define FUNC_NPC_RELIFE			5000

// 玩家和NPC谈话(建立连接)的最大合法距离
#define MAX_NPC_TALK_GIRD 5	

// 生活技能可学最大数量
#define  MAXLIFESKILL	  2

// 法宝技能初始等级
#define MW_SKILL_INIT_LEVEL		0

// 延迟删除列表 容量
#define	DELETE_LIST_SIZE	4000

// 内丹每次分配经验的最大人数
#define  MAX_REDSTONE_ROLENUM 2000

// 礼堂中的最大人数
#define  MAX_WEDDINGHALL_PLAYENUM 500

// 聚宝盆每天聚宝的物品种类数量
#define  TREASUREBOWL_DAILYEXCHANGENUM  500

// 宏定义 判断是否是任务道具
#define IS_TASK_ITEM(x) ( (x)->mItemType == CTplItem::ITEM_TASK \
	|| (x)->mItemType == CTplItem::ITEM_SPECIAL_TASK \
	|| (x)->mItemType == CTplItem::ITEM_TOKEN \
	|| (x)->mItemType == CTplItem::ITEM_CALL )

#define IS_NOT_TASK_ITEM(x) ( (x)->mItemType != CTplItem::ITEM_TASK \
	&& (x)->mItemType != CTplItem::ITEM_SPECIAL_TASK \
	&& (x)->mItemType != CTplItem::ITEM_TOKEN \
	&& (x)->mItemType != CTplItem::ITEM_CALL )

#define CARDBOOK_TYPE	7
#define CARDBOOK_NUM	100		
#define MATERIALINDEXNUM 3 
#define TITLEMAX_SELF	200	// 身上最多称号数量
#define TOPTEN			10
#define NATIONLIMIT		10
#define MIN_JEWCOM_NUM	3	// 宝石合成最少个数
#define MIN_COUNTRY_LEVEL 10
#define UPG_ITEM_NUM	10
#define MAX_DROP_TURN	16	// 掉落轮数
#define MAX_AWARD_NUM	100	// 最大领取数
#define PROTECT_TIME	600000

#define DEFAULT_RATE			5			// 默认税率索引

#define CHANGE_CAMP_LEVEL    30 //转阵营等级限制

#define FAME_TOTAL_LIMIT			99999	// 声望总上限


// 采集职业技能
enum EM_CollectSkill
{
	COLLECTSKILL_WOOD			= 1,	// 植物采集
	COLLECTSKILL_MINERAL		= 2,	// 矿物采集
	COLLECTSKILL_MATERIALMAKER	= 3,	// 材料加工	
};

// 学习配方的方式
enum EM_ComposeGetWay
{ 
	COMPOSEGETWAY_BYNPC			=	0x01,	// 通过npc学习
	COMPOSEGETWAY_BYCREAT		=	0x02,	// 通过研究获得
	COMPOSEGETWAY_BYSCROLL		=	0x04,	// 通过卷轴学习	
};
// 生活技能的种类
enum EM_LifeSkillKind
{
	LIFESKILLKIND_NONE				= 1,		// 未学习状态
	LIEFSKILLKIND_MAINSKILL			= 2,		// 主技能
	LIEFSKILLKIND_SUBSKILL			= 3,		// 辅修技能	
};

// 背包状态
enum EM_KITBAGSTATUS
{
	KITBAG_STATUS_CLEAN		= 0,			// 清空背包后再存储
	KITBAG_STATUS_NOTCLEAN	= 1,			// 不清空数据直接存储	
};

// 背包发送状态
enum EM_KITBAGSENDSTATUS
{
	KITBAG_SENDSTATUS_HASLEFTINFO = 0,	   // 还有背包信息未发送
	KITBAG_SENDSTATUS_INFOEND	  = 1,	   // 背包信息发送完毕
};

// 内丹使用状态
enum EM_RedStoneStatus
{
	REDSTONE_STATUS_PROTECT,				// 掉落后处于专属保护状态
	REDSTONE_STATUS_DROP,					// 专属保护消失，处于无主状态
	REDSTONE_STATUS_PROCESSTIMEER,			// 内丹点击后读秒状态	
	REDSTONE_STATUS_USED,					// 处于噬炼过程
};

enum EM_StorageStatus
{
	STORAGE_STATUS_INVALID = 1 ,	// 仓库不可用状态：物品信息还没有发往客户端
	STORAGE_STATUS_INITIALIZED ,	// 仓库已经初始化：物品信息已经发往客户端
};


// 物品绑定的状态
enum ItemBindStatus
{
	ITEM_BINDSTATUS_FREE          = 0,	  // 物品未绑定
	ITEM_BINDSTATUS_BINDED        = 1,    // 绑定状态
	ITEM_BINDSTATUS_BYITEM        = 2,    // 手动绑定
	ITEM_BINDSTATUS_FREEBIND      = 4,    // 解除手动绑定状态
};

// 绑定物品的方法
enum ItemBindWay
{
	ITEM_BINDWAY_BYPICKUP = 0 ,
	ITEM_BINDWAY_BYEQUIP,
	ITEM_BINDWAY_BYUSE,
	ITEM_BINDWAY_BYITEM,
	ITEM_BINDWAY_FREE,
};


// 召唤兽操作模式
enum EMPetMode
{
	EM_PET_MODE_FOLLOW = 0,
	EM_PET_MODE_ASSIST,
};

// 查找对象
enum EObjectECode
{
	ERROR_OBJECT_NOTFOUND	= -1,
	ERROR_OBJECT_USEDUP		= -2,
};


enum ETaskTip
{
	TASKTIP_NONE					= 0,			// 和任务没有任何关系
	TASKTIP_ACTIVE					= 1,			// 有任务可以接
	TASKTIP_BEGIN					= 2,			// 有任务可以交( 没有完成 )
	TASKTIP_COMPLETE				= 3,			// 有任务可以交( 已经完成 )
	TASKTIP_LOOPTYPE				= 4,			// 组队环任务的可接任务
};

/*
enum ETaskProgress
{
	TASK_BEGIN						= 0,			// 任务已经接受，进行中
	TASK_COMPLETE					= 1,			// 任务已经完成，但还没有交
	TASK_END						= 2,			// 任务已经结束
	TASK_ACTIVE						= 3,			// 任务已经激活
};
*/
enum EResultType
{

	RESULT_NONE						= 0,			// 无
	RESULT_NORMAL					= 1,			// 正常攻击
	RESULT_DEATHHIT					,			// 爆击
	RESULT_NORMALDUCK				,			// 闪避
	RESULT_SKILLDUCK				,			// 技能残影
	RESULT_BUFFRESIST				,			// BUFF抵抗
	RESULT_IMMUNITY					,			// 免疫
	RESULT_OBSORB					,			// 吸收
	RESULT_STEALMP					,			// 偷取MP
	RESULT_STEALHP					,			// 偷取HP
	RESULT_PROTECTED				,			// 守护
	RESULT_COST						,			// 技能消耗
	RESULT_USEITEM					,			// 使用药品
	RESULT_DAMAGELESS				,			// 抵消伤害
	RESULT_SKILLCARTOON				,			// 技能动画
	RESULT_REBOUND					,			// damage rebound
	RESULT_NORMALREFRESH			,			// 正常回血，回蓝
	RESULT_STATEON					,			// 获得状态
	RESULT_STATEOFF					,			// 删除状态
	RESULT_CHARGE					,			// 冲锋
	RESULT_TELEPORT					,			// 传送
	RESULT_TELESYMBOL				,			// 使用传送符
	RESULT_GIFT						,			// 使用礼包
	RESULT_USEBYITEM				,			// 由物品使用
};

enum
{
	FUNC_NONE						= 0,			// 无功能 ( value1=0, value2=0, value3=0, value4=0, value5=0 )
	FUNC_ATTACK,									// 使用技能( value1=目标EntityID, value2=技能ID， value3=技能等级, value4=目标坐标X, value5=目标坐标Y )( 技能ID=0 代表普通攻击 )
	FUNC_ATTACKED,									// 被技能攻击( value1=源EntityID, value2=技能ID， value3=技能等级,value4=源坐标X, value5=源坐标Y )( 技能ID = 0 代表普通攻击 )
	FUNC_INCHP,										// 增加HP ( value1=HP改变量, value2=HP当前值, value3=HP最大值, value4=0, value5=0 )
	FUNC_INCMP,										// 增加MP ( value1=MP改变量, value2=MP当前值, value3=MP最大值, value4=0, value5=0 )
	FUNC_INCAP,										// 增加AP ( value1=0, value2=0, value3=0, value4=0, value5=0 )
	FUNC_DECHP,										// 减少HP ( value1=HP改变量, value2=HP当前值, value3=HP最大值, value4=0, value5=0 )
	FUNC_DECMP,										// 减少MP ( value1=MP改变量, value2=MP当前值, value3=MP最大值, value4=0, value5=0 )
	FUNC_DECAP,										// 减少AP ( value1=0, value2=0, value3=0, value4=0, value5=0 )
	FUNC_INSERTBUFF,								// 添加Buff ( value1=buff模板ID, value2=Buff唯一ID, value3=剩余时间)
	FUNC_REMOVEBUFF,								// 删除Buff ( value1=buff模板ID, value2=Buff唯一ID )
	FUNC_DIE,										// 死亡 ( value1=x坐标, value2=y坐标, value3=0, value4=0, value5=0 )
	FUNC_COOLDOWN,									// 技能冷却完毕 ( value1=技能ID, value2=技能等级, value3=0, value4=0, value5=0 )
	FUNC_PEACE,										// 离开战斗 ( value1=0, value2=0, value3=0, value4=0, value5=0 )
	FUNC_BATTLE,									// 进入战斗 ( value1=0, value2=0, value3=0, value4=0, value5=0 )
	FUNC_BEGINFIRE,									// 开始技能吟唱 ( value1=0, value2=0, value3=0, value4=0, value5=0 )
	FUNC_ENDFIRE,									// 结束技能吟唱 ( value1=0, value2=0, value3=0, value4=0, value5=0 )
	FUNC_PKRED,										// PK状态变红, PK值加1 ( value1=0, value2=0, value3=0, value4=0, value5=0 )
	FUNC_PKYELLOW,									// PK状态变黄 ( value1=0, value2=0, value3=0, value4=0, value5=0 )
	FUNC_PKWHITE,									// PK状态变白 ( value1=0, value2=0, value3=0, value4=0, value5=0 )
	FUNC_PKREDBDEC,									// PK状态红名大幅减少，减少1点 ( value1=0, value2=0, value3=0, value4=0, value5=0 )
	FUNC_PKREDSDEC,									// PK状态红名少量减少，减少0.01点 ( value1=0, value2=0, value3=0, value4=0, value5=0 )
	FUNC_CMCOOLDOWN,								// 公共冷却完毕 ( value1=0, value2=0, value3=0, value4=0, value5=0 )
	FUNC_PHCOOLDOWN,								// 药品冷却完毕 ( value1=CD组ID, value2=0, value3=0, value4=0, value5=0 )
	FUNC_CHANGESTATE,								// 状态改变 ( value1=状态类型, value2=参数1, value3=参数2, value4=X坐标, value5=Y坐标 )
	FUNC_CHANGESPEED,								// 速度改变 ( value1=当前速度, value2=0, value3=0, value4=0, value5=0 )
	FUNC_CHANGEPOS,									// 位置改变 ( value1=X坐标, value2=Y坐标, value3=0, value4=0, value5=0 )
	FUNC_PETLEVELUP,								// 宠物升级 ( value1=当前等级, value2=0, value3=0, value4=0, value5=0 )
	FUNC_LEVELUP,									// 等级提升 ( value1=当前等级, value2=HP当前值, value3=HP最大值, value4=MP当前值, value5=MP最大值 )
	FUNC_TRAPONTOUCH,								// 陷阱被触发(value1=技能ID, value2=技能等级, value3=x坐标, value4=y坐标, value5=0 )
	FUNC_EXP,										// 获得经验(value1=获得经验, value2=当前经验(高32bit), value3=升级等级, value4=(低32bit), value5=0 )
	FUNC_HONOR,										// 获得功勋(value1=功勋改变, value2=当前功勋, value3=杀敌数, value4=本日经验已兑换, value5=本日物品已兑换 )
	FUNC_PKVALUE,									// pk值改变(value1=pk值, value2=0, value3=0, value4=0, value5=0 )
	FUNC_TALENT,									// 修为改变(value1=当前修为, value2=当前造化, value3=改变量, value4=造化等级, value5=0 )
	FUNC_WIZARD,									// 才华改变(value1=才华类型, value2=当前才华值, value3=当前才华等级, value4=0, value5=0 )
	FUNC_SYNCHPMP,									// 同步HP，MP( value1=当前HP， value2=最大HP, value3=当前MP, value4=最大MP
	FUNC_CHANGEPROTECT,								// 保护状态改变( value1=保护状态 0 1 )
	FUNC_CHARM,										// 魅力值改变(value1=当前魅力, 其余都无效 )
	FUNC_ITEM,										// 获得物品(value1=物品ID, value2=0, value3=0, value4=0, value5=0 )
	FUNC_MONEY,										// 获得金钱(value1=数量, value2=0, value3=0, value4=0, value5=0 )
	FUNC_BINDMONEY,									// 获得绑定金钱(value1=数量, value2=0, value3=0, value4=0, value5=0 )
	FUNC_YUANBAO,									// 获得元宝(value1=数量, value2=0, value3=0, value4=0, value5=0 )
	FUNC_OFFLINEEXP,								// 获得离线经验(value1=获得的经验, value2=剩余离线经验, value3=0, value4=0, value5=0 )
};
//
//enum ETaskTick
//{
//	TICK_KILLNPC					= 0,			// 杀怪计数
//	TICK_TALKNPC					= 1,			// 对话计数
//	TICK_REGIONARRIVAL				= 2,			// 达到指定区域
//	TICK_MONEY						= 3,			// 达到指定金币
//	TICK_LEVEL						= 4,			// 达到制定等级
//	TICK_ITEM						= 5,			// 道具计数
//};

enum ERelive
{
	RELIVE_WEAKGRAVE				= 1,			// 虚弱墓地复活
	RELIVE_NORMAL					= 2,			// 正常原地复活
	RELIVE_SKILL					= 3,			// 技能原地复活
	RELIVE_SYSTEM					= 4,			// 系统强行复活
};

enum EInsertBuff
{
	INSERTBUFF_SUCCESS				= -1,			// 添加Buff成功
	INSERTBUFF_RESIST				= -2,			// 添加Buff抵抗
	INSERTBUFF_BUFFNOTFOUND			= -3,			// 添加Buff没有找到
	INSERTBUFF_FULL					= -4,			// 添加Buff时候Buff已满
};

enum EExchageBaggage
{
	EXCHAGE_SUCCESS					= 0,			// 交易成功
	EXCHAGE_SRCISNOTPLAYER			= 1,			// 源不是玩家
	EXCHAGE_DESISNOTPLAYER			= 2,			// 目标不是玩家
	EXCHAGE_SRCOUTOFSPACE			= 3,			// 源包裹空间不够
	EXCHAGE_DESOUTOFSPACE			= 4,			// 目标包裹空间不够
	EXCHAGE_SRCMONEYNOTENOUGH		= 5,			// 源包裹金币不够
	EXCHAGE_DESMONEYOVERFLOW		= 6,			// 目标包裹金币溢出
	EXCHAGE_MYMONEYOVERFLOW			= 7,			// 我的包裹金币溢出
};

enum EMail
{
	ERROR_MAIL_SUCCESS				= 0,
	ERROR_MAIL_ITEMNOTFOUND			= 1,			// 邮件道具时，道具没有找到
	ERROR_MAIL_MONEYNOTENOUGH		= 2,			// 邮件道具时，金币不足
	ERROR_MAIL_ITEMLOCKED			= 3,			// 邮件道具时，道具锁定中
	ERROR_MAIL_NOSPACE				= 4,			// 邮件道具时，包裹空间不够
};

//enum EStall
//{
//	ERROR_STALL_SUCCESS				= 0,
//	ERROR_STALL_ENTITYNOTFOUND		= 1,			// 摆摊交易时，实体没有找到
//	ERROR_STALL_NOTENOUGHMONEY		= 2,			// 摆摊交易时，实体金钱不够
//	ERROR_STALL_NOSPACE				= 3,			// 摆摊交易时，实体包裹不够
//	ERROR_STALL_ITEMNOTFOUND		= 4,			// 摆摊交易时，指定索引不存在道具
//	ERROR_STALL_ITEMLOCKED			= 5,			// 摆摊交易时，道具被锁定
//	ERROR_STALL_ITEMNOTENOUGH		= 6,			// 摆摊交易时，源玩家没有足够的道具
//
//};

enum ELockItem
{
	LOCKTYPE_NONE					= 0,			// 没有被锁定
	LOCKTYPE_EXCHANGE				= 1,			// 交易锁
	LOCKTYPE_MAIL					= 2,			// 邮件锁
	LOCKTYPE_AUCTION				= 3,			// 拍卖锁
	LOCKTYPE_STALL					= 4,			// 摆摊锁
	LOCKTYPE_UPG					= 5,			// 升级锁
	LOCKTYPE_RUNE					= 6,			// 符文锁
	LOCKTYPE_JEWEL					= 7,			// 宝石锁
	LOCKTYPE_SLOT					= 8,			// 开槽锁
	LOCKTYPE_EAT					= 9,			// 吃装备锁
	LOCKTYPE_COMPOSE				= 10,			// 合成锁
	LOCKTYPE_SHOUT                  = 11,           // 喊话道具锁
	LOCKTYPE_TASK					= 12,			// 任务道具
	LOCKTYPE_BIND                   = 13,           // 绑定锁
	LOCKTYPE_QUESIONPAPER			= 14,			// 题卷锁	
	LOCKTYPE_TASK_RECVITEM			= 15,			// 交任务物品锁(普通物品)
	LOCKTYPE_LOCKCANUSE				= 16,			// 锁定包裹，但可以使用(传送符打开传送表时)
	LOCKTYPE_ADD					= 17,			// 加槽锁
	LOCKTYPE_REMOVE					= 18,			// 摘除锁
	LOCKTYPE_JUDGE					= 19,			// 鉴定锁
	LOCKTYPE_CHANGE					= 20,			// 转化抗性锁
	LOCKTYPE_JEWCOM					= 21,			// 宝石合成锁
	LOCKTYPE_LIFESKILL				= 22,			// 生活技能索
	LOCKTYPE_MAGICWEAPON			= 22,			// 法宝锁
	LOCKTYPE_MAGICWEAPON_STONE		= 23,			// 法宝鉴定石锁
	LOCKTYPE_MULTI					= 24,			// 复杂合成锁
	LOCKTYPE_DECOMPOUND				= 25,			// 装备分解锁
	LOCKTYPE_KEEPSAKE				= 26,			// 婚姻信物锁
};

enum EMaxLength
{
	ITEMOBJ_BUFFLEN					= 1024,			// 道具对象字节流长度
};

enum EClientState
{	CSTATE_HUMAN,
	CSTATE_LION,
	CSTATE_LEOPARD,
};

enum EPKMode
{
	PKMODE_TRAINING,	// 练级模式
	PKMODE_ALL,			// 屠杀模式
	PKMODE_CORPS,		// 军团模式
	PKMODE_ALLY,		// 盟国模式
	PKMODE_EVIL,		// 善恶保护
	
	//PKMODE_ALL,			// 全体保护
	//PKMODE_FAMILY,		// 家族保护
	//PKMODE_GUILD,		// 帮会保护
	//PKMODE_NATION,		// 国家保护
	//PKMODE_EVIL,		// 善恶保护
	//PKMODE_ALLY,		// 盟国保护（包括本国）
};

// 过渡用，即将被postype替代
enum ESlotType
{
	SLOT_EQUIPMENT			= 0,						// 装备栏
	SLOT_BAGGAGE,										// 包裹栏
	SLOT_STORAGE,										// 仓库栏
	SLOT_STORKITBAG,									// 仓库的背包
	SLOT_BAGGAGEKITBAG,									// 包裹的背包
};

// 包类型
enum EBagType
{
	BAGTYPE_BASEBAG = 1,								// 基础包( 仓库包裹包 )
	BAGTYPE_KITBAG  = 2,								// 背包
};

enum EPosType
{
	POS_NULL   			= 0x00,			// 不限制的位置
	POS_EQUIPMENT		= 0x01,			// 装备栏
	POS_BAGGAGE			= 0x02,			// 包裹栏
	POS_TASK_BAGGAGE	= 0x04,			// 任务包裹栏
	POS_STORAGE			= 0x08,			// 仓库栏 
};


enum EPropertyType
{
	PROPERTY_STA			= 1,						// 耐力
	PROPERTY_WIS,										// 智慧
	PROPERTY_STR,										// 力量
	PROPERTY_CON,										// 体魄
	PROPERTY_SPR,										// 精神
};


enum EState
{
	STATE_NONE						= 0,					// 无功能
	STATE_IMMUNITYDAMAGE,								// 无敌
	STATE_IMMUNITYREVERSD,									// immunity debuf 免疫负面BUFF
	STATE_CANNTMOVE,										// 无法移动
	STATE_REBOUND,											// 反弹伤害			目前无处理, 伤害反弹是直接由Buff来决定的(2009.10.9 lixi)
	STATE_CUREHPLESS,										// less cure hp 降低治疗量
	STATE_SILENCE,											// 沉默				目前无添加删除状态地方( 2009.10.9 lixi)
	STATE_CONFUSION,										// 混乱
	STATE_FASTING,											// 禁食
	STATE_SNEER,											// 嘲讽
	STATE_MAGICSHIELD,										// 魔法盾			目前维护状态， 但无应用, 功能由Buff实现(2009.10.9 lixi)
	STATE_PDEFENCEZERO,										// 物理防御为0a		目前无状态维护,无处理(2009.10.9 lixi)
	STATE_MDEFENCEZERO,										// 法术防御为0		目前无状态维护,无处理(2009.10.9 lixi)
	STATE_IGNORECLOTH,										// 忽略衣服计算		目前无处理, 无维护(2009.10.9 lixi)
	STATE_UNARM,											// 忽略武器计算		目前无状态维护,无处理(2009.10,9 lixi )
	STATE_INCGENERDAMAGEFIX,								// 增加受到某系技能伤害的固定值 (暂时未使用) 同上
	STATE_INCGENERDAMAGEPER,								// 增加受到某系技能伤害的百分比 目前无处理, 功能由Buff实现(2009.10.9 lixi)
	STATE_PGENERATTACKFIX,									// 增加指定系别攻击固定值		目前无状态删除(2009.10.9 lixi)
	STATE_NEXTDEATHHITFIX,									// 增加下次攻击致命等级固定值	目前无删除维护， 无处理, 功能由Buff维护(2009.10.9 lixi)
	STATE_NEXTDEATHRATEFIX,									// 增加下次攻击致命伤害加成固定值	目前无删除维护, 无处理, 功能由Buff实现(2009.10.9 lixi)
	STATE_INCHONORSCALE,									// 增加功勋获得万分比
	STATE_PROTECTED,										// 守护				目前无处理, 功能由Buff功能实现(2009.10.9 lixi)
	STATE_UNATTACK,											// 无法攻击
	STATE_DAMAGELESS,										// 抵消伤害			目前无处理, 功能由Buff功能实现(2009.10.9 lixi)
	STATE_INCREAMSPEED,										// 增加移动速度万分比
	STATE_UNCREAMSPEED,										// 降低移动速度万分比
	STATE_KILLERRENEW,										// 杀怪取血
	STATE_FIREEVENT,										// 成功释放技能使自身获得一个DEBUFF		目前无处理, 功能由Buff功能实现(2009.10.9 lixi)
	STATE_ATTACKEVENT,										// 攻击时有几率使自身获得特殊效果		目前无处理, 功能由Buff功能实现(2008.10,9 lixi)		
	STATE_RANDMOVE,											// 随机移动			目前无处理( 2009.10.9 lixi)
	STATE_INCDESPDAMAGEFIX,									// 增加被攻击时所受物理伤害固定值		目前无处理, 功能有Buff功能实现( 2008.10,9 lixi ) 
	STATE_INCDESPDAMAGEPER,									// 增加被攻击时所受物理伤害百分比		同上
	STATE_DECDESPDAMAGEFIX,									// 降低被攻击时所受物理伤害固定值		同上
	STATE_DECDESPDAMAGEPER,									// 降低被攻击时所受物理伤害百分比		同上
	STATE_INCDESMDAMAGEFIX,									// 增加被攻击时所受法术伤害固定值		同上
	STATE_INCDESMDAMAGEPER,									// 增加被攻击时所受法术伤害百分比		同上
	STATE_DECDESMDAMAGEFIX,									// 降低被攻击时所受法术伤害固定值		同上
	STATE_DECDESMDAMAGEPER,									// 降低被攻击时所受法术伤害百分比		同上
	STATE_INCGENERATTACK,									// 增加某系技能的攻击力 (暂时未使用)	目前无维护， 无处理( 2008.10,9 lixi ) 
	STATE_AUREOLE,											// aurlole skill 光环技能				
	STATE_INCEXP,											// 增加经验万分比
	STATE_DECALLDAMAGEPER,									// 减少所有伤害的伤害百分比
	STATE_TRANSFORM,										// 变身的状态
	STATE_INCCORPSGLORY,									// 增加军团荣耀的buff
	STATE_INCAP,											// 增加玩家功勋的buff


	STATE_UNATTACK_PLAYER,									// 不能攻击玩家
	STATE_PLAYER_UNATTACK,									// 不能被玩家攻击
	STATE_RIDE_HORSE,										// 骑马状态
	STATE_INVISIBLE,										// 隐身
	STATE_DISINVISIBLE,										// 反影身
	STATE_FLY,												// 飞行
	STATE_STONE,											// 灵石
	STATE_HIDE,												// 对主动怪隐身
	STATE_MASKED,											//蒙面人
	STATE_INCOFFLINEEXP,									// 增加离线经验获得万分比

		// 状态快50个了，继续添加的话请注意
	STATE_COUNT_MAX,		
	
};

enum EService
{
	SERVICE_NONE			= 0,						// 无服务状态
	SERVICE_SHOP			= 1,						// 商店服务状态
	SERVICE_MAIL			= 2,						// 邮件服务状态
	SERVICE_STORAGE			= 3,						// 仓库服务状态
	SERVICE_UPG				= 4,						// 升级服务状态
	SERVICE_TELE			= 5,						// 传送服务状态
	SERVICE_STUDYSKILL		= 6,						// 学习技能状态
	SERVICE_TOWN			= 7,						// 回城服务状态
	SERVICE_BIND			= 8,						// 装备绑定状态
	SERVICE_WELFARE			= 9,						// 每日福利状态
	SERVICE_STALL			= 10,						// 摆摊状态
	SERVICE_TRADE			= 11,						// 交易状态
	SERVICE_CHANGE			= 12,						// 转换状态
	SERVICE_JUDGE			= 13,						// 鉴定状态
	SERVICE_EXCHANGE		= 14,						// 兑换状态
	SERVICE_HONOR			= 15,						// 功勋状态
	SERVICE_BATTLE			= 16,						// 战场服务
	SERVICE_MWJUDGEPRO		= 17,						// 法宝属性鉴定服务
	SERVICE_MWJUDGESKILL	= 18,						// 法宝技能鉴定服务
	SERVICE_GOLDENYB		= 19,						// 元宝领取服务
	SERVICE_HONOR_TOKEN		= 20,						// 荣誉兑换令牌服务
};

enum EPKStatus
{
	PKSTATUS_WHITE			= 0,						// 白色状态
	PKSTATUS_AZURY,										// 浅蓝名
	PKSTATUS_BLUE,										// 蓝名
	PKSTATUS_NAVYBLUE,									// 深蓝名
	PKSTATUS_ROSINESS,									// 浅红色
	PKSTATUS_RED,										// 红名
	PKSTATUS_CARMINE,									// 深红名
	PKSTATUS_PKTOPLIMIT,								// PK值上限
	PKSTATUS_YELLOW,									// 黄名
};

enum EProDebug
{
	COMMAND_INSERTITEM		 = 1,						// 获得道具
	COMMAND_MOVEITEM,									// 移动道具
	COMMAND_OBTAINSKILL,								// 获得技能
	COMMAND_EQUIPITEM,									// 装备道具
	COMMAND_UNEQUIPITEM,								// 卸下道具
	COMMAND_OBTAINEXP,									// 获得经验
	COMMAND_CHANGEMETIER,								// 转换职业
	COMMAND_ADDTASK,									// 添加任务
	COMMAND_REMOVETASK,									// 删除任务
};

//enum EUseItem
//{
//	ERROR_ITEMSUCCESS			= 1400,					// 使用成功
//	ERROR_ITEMNOTEXIST,									// 物品不存在
//	ERROR_TPLNOTEXIST,									// 模板不存在
//	ERROR_WRONGTYPE,									// 类型错误
//	ERROR_NOTENOUGHPOINT,								// 点数不足
//	ERROR_NOTCOOLDOWN,									// 没有冷却
//	ERROR_RECOVERING,									// 道具恢复中
//	ERROR_RANDOMTP,										// 随机传送
//	ERROR_RETURNTP,										// 入口传送
//	ERROR_REMEMBERTP,									// 记忆传送
//	ERROR_TOWNTP,										// 回城传送
//	ERROR_SRCDEAD,										// 使用者死亡
//	ERROR_UNUSEITEM,									// 处于无法吃药状态
//};

enum EEquipPart
{
	EQUIPMENT_WEAPON,									// 武器
	EQUIPMENT_CHEST,									// 胸部护具
	EQUIPMENT_LEG,										// 腿部护具
	EQUIPMENT_HEAD,										// 头部护具
	EQUIPMENT_NECK,										// 项链
	EQUIPMENT_RINGLEFT,									// 左戒指
	EQUIPMENT_RINGRIGHT,								// 右戒指
	EQUIPMENT_SASH,										// 腰带
	EQUIPMENT_CUFFLEFT,									// 左护腕
	EQUIPMENT_CUFFRIGHT,								// 右护腕
	EQUIPMENT_CHARMLEFT,								// 左护符
	EQUIPMENT_CHARMRIGHT,								// 右护符
	EQUIPMENT_BALL,										// 法球	
	EQUIPMENT_CLOAK,									// 披风	
	EQUIPMENT_FASHION,									// 时装
	EQUIPMENT_WIZARD1,									// 才华装备
	EQUIPMENT_WIZARD2,
	EQUIPMENT_WIZARD3,							
	EQUIPMENT_MAGICWEAPON,								// 法宝		
	EQUIPMENT_MARRY,									// 结婚道具
	EQUIPMENT_MAXPARTCOUNT								// 装备最大数量
};


enum EUpgResult
{
	UPG_SUCCESS					= 0,					// 升级成功
	UPG_REMAIN,											// 失败但保底
	UPG_FAIL,											// 失败消失
	UPG_DOWN,											// 失败降级
	UPG_STOP,											// 没成功需要停止
	UPG_EXT,											// 失败但不降级
};

enum EProduceType
{
	EQUIP_WEAPON				= 0,					// 武器
	EQUIP_HELM,											// 帽子
	EQUIP_BODY,											// 衣服
	EQUIP_SHOE,											// 鞋子
	EQUIP_BRACER,										// 手腕
	EQUIP_RING,											// 戒指
	EQUIP_NECKLACE,										// 项链
	EQUIP_CHARM,										// 护符
	EQUIP_CLOAK,										// 披风
	EQUIP_BELT,											// 腰带
	PHYSICS_RECOVER,									// 恢复
	PHYSICS_BUFF,										// BUFF
	PHYSICS_DEXP,										// 双倍经验
};

enum EProduceLevel
{
	EQUIP_LEVEL_BEGIN			= 0,					// 装备等级范围
	EQUIP_LEVEL_END				= 11,
	HP_LEVEL_BEGIN				= 100,					// HP药等级范围
	HP_LEVEL_END				= 103,
	MP_LEVEL_BEGIN				= 104,					// MP药等级范围
	MP_LEVEL_END				= 107,
	HMP_LEVEL_BEGIN				= 108,					// HMP药等级范围
	HMP_LEVEL_END				= 111,
	RES_LEVEL_BEGIN				= 112,					// 抗性药等级范围
	RES_LEVEL_END				= 115,
	PRO_LEVEL_BEGIN				= 116,					// 属性药等级范围
	PRO_LEVEL_END				= 119,
	EXP_LEVEL					= 120,
};

// 材料的ID，TYPE1-4为青龙，白虎，朱雀，玄武，RANK1-4
enum EMaterialID
{
	TYPE1RANK1 = 25397,
	TYPE1RANK2,
	TYPE1RANK3,
	TYPE1RANK4,
	TYPE2RANK1 = 25404,
	TYPE2RANK2,
	TYPE2RANK3,
	TYPE2RANK4,
	TYPE3RANK1 = 25390,
	TYPE3RANK2,
	TYPE3RANK3,
	TYPE3RANK4,
	TYPE4RANK1 = 25411,
	TYPE4RANK2,
	TYPE4RANK3,
	TYPE4RANK4,
	PHYTYPE	   = 25369,						// 药品材料的起始
	PHYDEXP	   = 25373,						// 双倍经验药
};

// 策略：触发器的条件类型
enum TriggerType
{
	ONTIMER			= 0x01,	// 定时器到时
	HPLESSTHAN		= 0x02,	// HP少于%
	HPMORETHAN		= 0x04,	// HP多于%
	BATTLESTART		= 0x08,	// 战斗开始
	BATTLEEND		= 0x10,	// 战斗结束
	RANDOMT			= 0x20,	// 随机
	KILLPLAYER		= 0x40,	// 杀死玩家
	KILLOGRE		= 0x80,	// 怪物死亡
};

// 策略：操作类型
enum ActionType
{
	ATTACK = 0,				// 开始攻击
	USESKILL,				// 使用技能
	SPEAK,					// 说话
	RESETENMITY,			// 重置仇恨列表
	RUNTRIGGER,				// 执行触发器
	STOPTRIGGER,			// 停止触发器
	ACTIVETRIGGER,			// 激活触发器	
	FLEE,					// 逃跑
	TARFIRSTENMITY,			// 把目标仇恨置为最高
	TARLASTENMITY,			// 把目标仇恨置为最低
	REDUCEHALFENMITY,		// 减少目标仇恨50%
	BREAK,					// 跳过后面操作
	OGRECONTROL,			// 激活怪物控制器
	CREATETIMER,			// 创建定时器
	STOPTIMER,				// 停止定时器
};

// 策略：目标类型
enum TargetType
{
	FIRSTENMITY		= 0x0100,		// 仇恨最高
	SECONDENMITY	= 0x0200,		// 第二仇恨
	RANDOMENMITY	= 0x0400,		// 除第一外仇恨列表里的随机
	MAXHP			= 0x0800,		// hp最多
	MAXMP			= 0x1000,		// mp最多
	MINHP			= 0x2000,		// hp最少
	SELF			= 0x4000,		// 自己
	OTHEROGRE		= 0x8000,		// 其他怪物
	PROF1			= 0x0001,		// 职业1-6
	PROF2			= 0x0002,
	PROF3			= 0x0004,
	PROF4			= 0x0008,
	PROF5			= 0x0010,
	PROF6			= 0x0020,
	PROF7			= 0x0040,

};


enum ETitleAction
{
	TITLE_ADD		=	0,	// 增加称号
	TITLE_REPLACE,			// 替换称号
	TITLE_DELETE,			// 删除称号
};

enum CLOSEWINDOW
{
	CLOSE_UPG		= 0x001,							// 关闭升级
	CLOSE_PRO		= 0x002,							// 关闭生产
	CLOSE_RUNE		= 0x004,							// 关闭符文
	CLOSE_JEWEL		= 0x008,							// 关闭镶嵌
	CLOSE_YUAN		= 0x010,							// 关闭元气虫
	CLOSE_SLOT		= 0x020,							// 关闭开槽
	CLOSE_COMPOSE	= 0x040,							// 关闭合成
	CLOSE_MAIL		= 0x080,							// 关闭邮件
	CLOSE_BIND		= 0x100,							// 关闭装备绑定
	CLOSE_WELFARE	= 0x200,
	CLOSE_TASK		= 0x400,							// 关闭交任务界面
	CLOSE_TELESYMBOL = 0x800,							// 关闭传送符界面
	CLOSE_ADD		= 0x1000,							// 关闭加槽
	CLOSE_REMOVE	= 0x2000,							// 关闭摘除
	CLOSE_CHANGE	= 0x4000,							// 关闭转化
	CLOSE_JEWCOM	= 0x8000,							// 关闭宝石合成
	CLOSE_JUDGE		= 0x10000,							// 关闭鉴定
	CLOSE_COMPOSECREAT		= 0x20000,					// 创造配方的界面关闭
	CLOSE_DECOMPOUNDEQUIP	= 0x40000,					// 装备分解的界面关闭
};

//玩家队伍职务状态
#define TEAM_NONE				0						//无队伍玩家

#define MAX_TRIP_ENTITY			8						// 最大陷阱个数
#define SHOP_PAGEINDEX_MAX		6						// NPC商店交易界面最多页数
#define SHOP_ITEMINDEX_MAX		12						// NPC商店每页最多道具数

// 摆摊
enum STALLMACRO
{
	STALL_NAME			= 0x1001,		// 摊位名称
	STALL_ADCONTENT,					// 广告
	STALL_PUSH,							// 上架
	STALL_DOWN,							// 下架
};

// 进入玩家视野的创建方式
enum EPlayerCreatMethod
{	
	PLAYERCREAT_MOVE		= 0,		// 通过移动进入视野
	PLAYERCREAT_TELEPORT	= 1,		// 通过传送进入玩家视野		
};

// 六种才华
enum EWizardType
{
	WIZARD_ARTS	= 0,					// 文采
	WIZARD_TACTICS,						// 兵法
	WIZARD_POWER,						// 武力
	WIZARD_WISDOM,						// 谋略
	WIZARD_POLITICS,					// 政治
	WIZARD_EDUCATE,						// 修养	
	WIZARD_COMMON,						// 通用
	WIZARD_NUM,
};

//// 攻击打断
//#define CONTATT_OVER 0xFFFFFFFF			// 攻击被打断
//enum CONTINUEDATTACK
//{
//	CONTATT_NONE		= 0x00000000,	// 攻击正常
//	CONTATT_MOVE		= 0x00000001,	// 移动打断
//	CONTATT_POSDIFF		= 0x00000002,	// 持续攻击中心点改变打断
//};

// *************************************************************************** //
// 功能结果, 技能,道具和Buff造成的结果都是功能组成的
// *************************************************************************** //
class CFuncResult
{
public:
	unsigned int	mEntityID;	// 目标实体ID
	int				mFunc;
	int				mResult;	// 执行结果
	unsigned int	mValue1;
	unsigned int 	mValue2;
	unsigned int	mValue3;
	unsigned int	mValue4;
	unsigned int	mValue5;
	int				mPosX;
	int				mPosY;
	int				mDirection;
	int				mListID;
	unsigned int	mSrcEntityID;	// 源实体ID,有可能为0,用于客户端显示用
	bool			mBroadCast;		// 是否群发

public:
	CFuncResult( unsigned int vSrcEntity, unsigned int vEntityID, unsigned short vFunc, unsigned short vResult, bool vBroadCast, int vListID, unsigned int vValue1, unsigned int vValue2, unsigned int vValue3 = 0, unsigned int vValue4 = 0, unsigned int vValue5 = 0 ) 
		: mEntityID( vEntityID ), mFunc( vFunc ), mResult( vResult ), mValue1( vValue1 ), mValue2( vValue2 ), mValue3( vValue3 ), mValue4( vValue4 ), mValue5( vValue5 ), 
		mPosX(0), mPosY(0), mDirection(0), mListID( vListID ),mSrcEntityID( vSrcEntity ), mBroadCast( vBroadCast )
	{

	}
	CFuncResult(){};

public:
	unsigned int	GetEntityID( )	{ return mEntityID; }
	unsigned short	GetFunc( )		{ return mFunc; }		// 得到执行的功能码
	unsigned short	GetResult( )	{ return mResult; }		// 得到执行的结果
	unsigned int	GetValue1( )	{ return mValue1; }
	unsigned int	GetValue2( )	{ return mValue2; }
	unsigned int	GetValue3( )	{ return mValue3; }
	unsigned int	GetValue4( )	{ return mValue4; }
	unsigned int	GetValue5( )	{ return mValue5; }
	int				GetPosX( )		{ return mPosX; }
	int				GetPosY( )		{ return mPosY; }
	int				GetDirection( )	{ return mDirection; }
	int				GetListID( )	{ return mListID; }
	unsigned int	GetSrcEntityID( ) { return mSrcEntityID; }
	bool			GetBroadCast( )	{ return mBroadCast; }
};

// 法宝数值类型
enum enMagicWeaponValueTyle
{
	MAGIC_WEAPON_LEVEL			= 0x000001 ,		// 法宝等级
	MAGIC_WEAPON_EXP			= 0x000002 ,		// 法宝经验
	MAGIC_WEAPON_MP				= 0x000004 ,		// 法力值
	MAGIC_WEAPON_HP				= 0x000008 ,		// 灵气值
	MAGIC_WEAPON_KEENPOINTS		= 0x000010 ,		// 当前锐力
	MAGIC_WEAPON_CALMPOINTS		= 0x000020 ,		// 当前定力
	MAGIC_WEAPON_MAGICPOINTS	= 0x000040 ,		// 当前法力
	MAGIC_WEAPON_BIERPOINTS		= 0x000080 ,		// 当前灵力
	MAGIC_WEAPON_IMMORTALPOINTS = 0x000100 ,		// 当前仙力
	MAGIC_WEAPON_PROSPIR		= 0x000200 ,		// 灵性
	MAGIC_WEAPON_LEFTPOINTS		= 0x000400 ,		// 当前剩余潜能点
	MAGIC_WEAPON_LEFTSKILLPOINTS= 0x000800 ,		// 当前剩余技能点
	MAGIC_WEAPON_MP_INC			= 0x001000 ,		// 法力值每次恢复点数
	MAGIC_WEAPON_HP_INC			= 0x002000 ,		// 灵气值每次恢复点数
	MAGIC_WEAPON_PROBASE		= 0x004000 ,		// 属性基础值
	MAGIC_WEAPON_PROUP			= 0x008000 ,		// 属性成长值
	MAGIC_WEAPON_PROEFF			= 0x010000 ,		// 属性影响度
	MAGIC_WEAPON_PROCESSVAL		= 0x020000 ,		// 当前进度值
	MAGIC_WEAPON_MAXMP			= 0x040000 ,		// 法力值上限
	MAGIC_WEAPON_MAXHP			= 0x080000 ,		// 灵气值上限
};

// 法宝回复MP、减少HP时间 (ms)
#define MW_INDEC_TIMER	300000

// 法宝额外等级影响附加值
#define MW_LEVEL_EXTRA_EFF 100

// 法宝定时器枚举
enum enMagicWeaponTimerType
{
	MW_TIMWR_MP_INC		=	0,		// 增加MP定时器
	MW_TIMWR_HP_DEC			 ,		// 扣减MP定时器

	MW_TIMER_NUM			 ,		// 数组下标
};

// 法宝影响人的公式
class CMagicWeaponFormula
{
public:
 	// 提升玩家攻击力
	// 参数表：法宝的模板ID，当前锐力，当前属性影响度，当前等级，当前精炼等级，灵性, 灵气值
	static int GetAttack( int nTempID, int nCurKeenpoints, int nProEff, int nMWLevel, int nLevel, int nProSpir, int nHP )
	{
		if ( nHP == 0 ) {return 0;}
		CTemplateMagicWeapon* tpItemMW = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( nTempID );
#ifdef LINUX
		LK_ASSERT( tpItemMW!=NULL, return 0 );
#endif
		if ( tpItemMW == NULL )
		{
			return 0;
		}

		// 计算力量(本体)影响：
		int nHardValue = (int)((double)nCurKeenpoints*tpItemMW->mAttackInc/SERVER_PERCENT_INT*nProEff/SERVER_PERCENT_INT);

		// 先得到精炼系数
		CTplMWLevelQuotietyTab* tpTable = (CTplMWLevelQuotietyTab*)CDataStatic::SearchTpl( tpItemMW->mLevelQuotietyTabID );
		int nLevelEff = 0;
		if ( tpTable != NULL && nLevel >= 0 && nLevel < 20 )
		{
			nLevelEff = tpTable->mKeenQuotiety[nLevel];
		}

		// 计算等级影响：
		int nLevelValue = (nMWLevel+MW_LEVEL_EXTRA_EFF)*nLevelEff/SERVER_PERCENT_INT;

		return (int)(nHardValue+nLevelValue)*nProSpir/SERVER_PERCENT_INT;
	}

	// 提升玩家防御力
	// 参数表：法宝的模板ID，当前定力，当前属性影响度，当前等级，当前精炼等级，灵性
	static int GetRecovery( int nTempID, int nCurCalmpoints, int nProEff, int nMWLevel, int nLevel, int nProSpir, int nHP )
	{
		if ( nHP == 0 ) {return 0;}
		CTemplateMagicWeapon* tpItemMW = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( nTempID );
#ifdef LINUX
		LK_ASSERT( tpItemMW!=NULL, return 0 );
#endif
		if ( tpItemMW == NULL )
		{
			return 0;
		}

		// 计算力量(本体)影响：
		int nHardValue = (int)((double)nCurCalmpoints*tpItemMW->mRecovInc/SERVER_PERCENT_INT*nProEff/SERVER_PERCENT_INT);

		// 先得到精炼系数
		CTplMWLevelQuotietyTab* tpTable = (CTplMWLevelQuotietyTab*)CDataStatic::SearchTpl( tpItemMW->mLevelQuotietyTabID );
		int nLevelEff = 0;
		if ( tpTable != NULL && nLevel >= 0 && nLevel < 20 )
		{
			nLevelEff = tpTable->mCalmQuotiety[nLevel];
		}

		// 计算等级影响：
		int nLevelValue = (nMWLevel+MW_LEVEL_EXTRA_EFF)*nLevelEff/SERVER_PERCENT_INT;

		return (nHardValue+nLevelValue)*nProSpir/SERVER_PERCENT_INT;
	}

	// 提升玩家MP上限
	// 参数表：法宝的模板ID，当前法力，当前属性影响度，当前等级，当前精炼等级，灵性
	static int GetMaxMp( int nTempID, int nCurMagicpoints, int nProEff, int nMWLevel, int nLevel, int nProSpir, int nHP )
	{
		if ( nHP == 0 ) {return 0;}
		CTemplateMagicWeapon* tpItemMW = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( nTempID );
#ifdef LINUX
		LK_ASSERT( tpItemMW!=NULL, return 0 );
#endif
		if ( tpItemMW == NULL )
		{
			return 0;
		}

		// 计算力量(本体)影响：
		int nHardValue = (int)((double)nCurMagicpoints*tpItemMW->mMaxMPInc/SERVER_PERCENT_INT*nProEff/SERVER_PERCENT_INT);

		// 先得到精炼系数
		CTplMWLevelQuotietyTab* tpTable = (CTplMWLevelQuotietyTab*)CDataStatic::SearchTpl( tpItemMW->mLevelQuotietyTabID );
		int nLevelEff = 0;
		if ( tpTable != NULL && nLevel >= 0 && nLevel < 20 )
		{
			nLevelEff = tpTable->mMagicQuotiety[nLevel];
		}

		// 计算等级影响：
		int nLevelValue = (nMWLevel+MW_LEVEL_EXTRA_EFF)*nLevelEff/SERVER_PERCENT_INT;

		return (nHardValue+nLevelValue)*nProSpir/SERVER_PERCENT_INT;
	}

	// 提升玩家HP上限
	// 参数表：法宝的模板ID，当前灵力，当前属性影响度，当前等级，当前精炼等级，灵性
	static int GetMaxHp( int nTempID, int nCurBierpoints, int nProEff, int nMWLevel, int nLevel, int nProSpir, int nHP )
	{
		if ( nHP == 0 ) {return 0;}
		CTemplateMagicWeapon* tpItemMW = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( nTempID );
#ifdef LINUX
		LK_ASSERT( tpItemMW!=NULL, return 0 );
#endif
		if ( tpItemMW == NULL )
		{
			return 0;
		}

		// 计算力量(本体)影响：
		int nHardValue = (int)((double)nCurBierpoints*tpItemMW->mMaxHPInc/SERVER_PERCENT_INT*nProEff/SERVER_PERCENT_INT);

		// 先得到精炼系数
		CTplMWLevelQuotietyTab* tpTable = (CTplMWLevelQuotietyTab*)CDataStatic::SearchTpl( tpItemMW->mLevelQuotietyTabID );
		int nLevelEff = 0;
		if ( tpTable != NULL && nLevel >= 0 && nLevel < 20 )
		{
			nLevelEff = tpTable->mBierQuotiety[nLevel];
		}

		// 计算等级影响：
		int nLevelValue = (nMWLevel+MW_LEVEL_EXTRA_EFF)*nLevelEff/SERVER_PERCENT_INT;

		return (nHardValue+nLevelValue)*nProSpir/SERVER_PERCENT_INT;
	}

	// 提升玩家致命等级
	// 参数表：法宝的模板ID，当前仙力，当前属性影响度，当前等级，当前精炼等级，灵性
	static int GetDeadLine( int nTempID, int nCurImmortalpoints, int nProEff, int nMWLevel, int nLevel, int nProSpir, int nHP )
	{
		if ( nHP == 0 ) {return 0;}
		CTemplateMagicWeapon* tpItemMW = (CTemplateMagicWeapon*)CDataStatic::SearchTpl( nTempID );
#ifdef LINUX
		LK_ASSERT( tpItemMW!=NULL, return 0 );
#endif
		if ( tpItemMW == NULL )
		{
			return 0;
		}

		// 计算力量(本体)影响：
		int nHardValue = (int)((double)nCurImmortalpoints*tpItemMW->mDeadLineInc/SERVER_PERCENT_INT*nProEff/SERVER_PERCENT_INT);

		// 先得到精炼系数
		CTplMWLevelQuotietyTab* tpTable = (CTplMWLevelQuotietyTab*)CDataStatic::SearchTpl( tpItemMW->mLevelQuotietyTabID );
		int nLevelEff = 0;
		if ( tpTable != NULL && nLevel >= 0 && nLevel < 20 )
		{
			nLevelEff = tpTable->mImmorQuotiety[nLevel];
		}

		// 计算等级影响：
		int nLevelValue = (nMWLevel+MW_LEVEL_EXTRA_EFF)*nLevelEff/SERVER_PERCENT_INT;

		return (nHardValue+nLevelValue)*nProSpir/SERVER_PERCENT_INT;
	}
};

// 获得技能方式
enum enGetSkillType
{
	GETSKILL_LEARN = 1,		// 领悟
	GETSKILL_JUDGE,			// 鉴定
	GETSKILL_USEBOOK,		// 使用技能书
};

// 法宝锁定类型
enum enMWLockType
{
	MWLOCK_SKILLJUDGE = 1,		// 技能鉴定
	MWLOCK_PROJUDGE		 ,		// 属性鉴定（法宝）
	MWLOCK_PROJUDGE_STONE ,		// 属性鉴定宝石
};

//// 组合buff类型
//enum enMultiBuffType
//{
//	MULTIBUFF_SHITU = 1,	// 师徒四人
//	MULTIBUFF_MARRY,		// 结婚
//	MULTIBUFF_4METIER,		// 4职业组队奖励
//	MULTIBUFF_5METIER,		// 5职业组队奖励
//	MULTIBUFF_6METIER,		// 6职业组队奖励
//};

// 用户类型
enum enUserType
{
	USERTYPE_NONE = 0,
	USERTYPE_GM  = 1,		// 线上GM
	USERTYPE_OFFICE = 2,	// 有特殊官职的玩家
	USERTYPE_TITLE = 3,		// 有特殊称号的玩家
	USERTYPE_KING = 4,		// 天子
};

// 法宝洗点类型
enum enMW_ClearType
{
	CLEARTYPE_BIG,	// 大洗
	CLEARTYPE_SMALL,	// 小洗
	CLEARTYPE_SP,	// 洗技能
};

// 法宝潜能点类型
enum enMW_POINT_TYPE
{
	KEENPOINTS		=	0,		
	CALMPOINTS	,	
	MAGICPOINTS	,
	BIERPOINTS	,	
	IMMORTALPOINTS,


	POINT_TYPE_NUM,
};


// 药品类型
enum enPhysicType
{
	PHYSIC_BLUE = 1,	// 蓝药
	PHYSIC_RED,			// 红药
	PHYSIC_AP,			// 活力药
	PHYSIC_FB_HP,		// 法宝法力药
	PHYSIC_FB_MP,		// 法宝灵气药
	PHYSIC_MEND,		// 金刚钻
};

#define GET_TITLE_TYPE( x ) ( x & 0xF000 )

// 检查是否是国籍称号
#define IS_NATION_TITLE( a, b ) ( \
	( a == CTplTitleManager::SPECIAL_NATION ) && \
	( b == CTplTitleManager::SCOPE_NATIONALITY ) )

// 称号类型
enum enTitleType
{
	EN_RANK_TYPE		= 0x1000,	// 排行榜类	
	EN_STATISTIC_TYPE	= 0x2000,	// 统计类
	EN_CONDITION_TYPE	= 0x3000,	// 条件类
	EN_SPECIAL_TYPE		= 0x4000,	// 特殊类
};


enum EWorld
{
	WORLD_START		= 0,
	WORLD_FIRST		= 1,
	WORLD_SECOND	= 2,
	WORLD_THIRD		= 3,
	WORLD_FOURTH	= 4,
	WORLD_NUM		= 5,
};

#define IS_WORLDID(x)	( ( (x) > 0 && (x) < 5 ) ? true : false )

enum ECountry
{
	COUNTRY_ALL			= -2,			// 新增全体国家
	COUNTRY_NONE		= -1,
	COUNTRY_CHANGAN		= 0,
	COUNTRY_WUJI		= 1,
	COUNTRY_CHECHI		= 2,
	COUNTRY_BAOXIANG	= 3,
	COUNTRY_NVER		= 4, 
	COUNTRY_NUM			= 5,
};

enum CountryMapID
{
	COUNTRY_MAP_NVER		= 45,
	COUNTRY_MAP_CHANGAN		= 62,
	COUNTRY_MAP_WUJI		= 68,
	COUNTRY_MAP_CHECHI		= 69,
	COUNTRY_MAP_BAOXIANG	= 70,	
	COUNTRY_MAP_QITIAN		= 61,
};

enum EChangeType
{
	CHANGE_ADD	= 0,
	CHANGE_DEL	= 1,
};

enum EWorldOperationType
{
	WORLD_OP_INVITE		= 0x01,			// 请求结盟 
	WORLD_OP_PEACE		= 0x02,			// 请求中立
	WORLD_OP_WAR		= 0x04,			// 请求宣战
	WORLD_OP_DISMISS	= 0x08,			// 请求解盟
};

enum EWorldResult
{
	WORLD_RESULT_AGREE	= 1,
	WORLD_RESULT_REFUSE	= 2,
};

enum EWorldStatus
{
	WORLD_STATUS_NONE		= 0,		// 中立
	WORLD_STATUS_FRIEND		= 1,		// 友好
	WORLD_STATUS_WAR		= 2,		// 宣战
};

// 帮助系统 数值枚举
// 参数1 今天已经完成的次数 参数2 今天可以完成的最大次数
enum enHelpData
{
	HELPID_DOUBLE			= 1, // 双倍经验 按周来算 算时间 不算次数
	HELPID_QUESTION			= 2, //	每日答题
	HELPID_LONGGONG			= 3, // 闹龙宫
	HELPID_PANTAO			= 4, // 蟠桃劫
	HELPID_XIANTAO			= 5, // 品仙桃
	HELPID_HUANGFENGD		= 6, //	黄风洞
	HELPID_BATTLE			= 7, //	大战花果山
	HELPID_DAYANTA			= 8, // 大雁塔
	HELPID_LIANHUADONG		= 9, // 莲花洞
	HELPID_QUXIANTAO		= 10,// 大圣取仙桃
	HELPID_FAMILY_XIANTAO	= 11,// 品仙桃_家族
	HELPID_NPC_BATTLE		= 12,//	NPC争夺战
	HELPID_FAMILY_REDSTONE	= 13,// 家族内丹
	HELPID_FAMILY_BATTLE	= 14,//	家族挑战赛
	HELPID_NVERGUO			= 15,// 求娶女儿国
	HELPID_FORCEBATTLE		= 16,//	势力争夺战
	HELPID_KING_LOGIN		= 17,// 车迟国登基站
	HELPID_SONOFKING		= 18,// 天子登基战
	HELPID_CORPSBATTLE		= 19,// 军团挑战赛
	HELPID_TODAY_LOOPS		= 20,// 本日完美师门任务
	HELPID_WEEK_LOOPS		= 21,// 本周完美师门任务
	HELPID_CHAIRMANTASK		= 22,// 掌门任务
	HELPID_STORAGETASK		= 23,// 完美皇榜任务
	HELPID_MINEMAP			= 24,//	杀妖夺宝任务
	HELPID_FAMILY_SEND		= 25,// 家族护送任务
	HELPID_FAMILY_CHALLEGE  = 26,// 家族挑战任务
	HELPID_CORPS_RAO		= 27,// 军团扰边任务
	HELPID_CORPS_SHOU		= 28,// 军团守边任务
	HELPID_CORPS_CHALLEGE	= 29,//	军团挑战任务
	HELPID_CORPS_SHOUQI		= 30,//	军团守旗任务
	HELPID_SELLSELF			= 31,//	毛遂自荐	
	HELPID_FAMILY_HONOR		= 32,//	家族荣耀任务
	HELPID_COUNTRY_BUILD	= 33,//	国家建设任务任务

	HELPID_COUNTRY_HONOR	= 34,//	阵营荣誉任务	
	HELPID_COUNTRY_FLAG		= 35,//	大破正黄旗任务	
	HELPID_COUNTRY_DOOR		= 36,//	大破边境门任务

	HELPID_COUNTRY_JIAOZI	= 37,//	春节送饺子
	HELPID_COUNTRY_QINGHUA	= 38,//	灌溉情花
	HELPID_COUNTRY_WENQING	= 39,//	红线问情

	HELPID_COUNTRY_FULI		= 40,//	国家福利
	HELPID_FAMILY_CHUYAO	= 41,//	家族除妖
	HELPID_CAMP_HONOR		= 42,//	阵营荣誉

	HELPID_XUNXIAN			= 43, // 千里寻仙
	HELPID_SISHEN			= 44, // 四圣试禅心
	HELPID_STONE			= 45, // 采集灵石
	HELPID_STONE2			= 46, // 守卫灵石
	HELPID_HUANGFENG		= 47, // 黄风作乱
	HELPID_CHIHUO			= 48, // 赤火作乱
	HELPID_HEISHUI			= 49, // 黑水作乱
	HELPID_YUANXIAOJIE		= 50, // 元宵节
	HELPID_CORP_TASK		= 51, // 军团任务
	HELPID_FAMILY_TASK		= 52, // 家族任务
	HELPID_COUNTRY_TASK		= 53, // 国家任务
	HELPID_CLOUD_ACTIVITY		= 54, // 云中仙缘
	HELPID_PILE_BATTLE		= 55, // 积雷山争夺
	HELPID_BLAZE_BATTLE		= 56, // 火焰山争夺
	HELPID_CHICK_COUNTRY		= 57, // 乌鸡国登基战
	HELPID_ELEPHANT_COUNTY		= 58, // 宝象国登基战
	HELPID_CAMP_TASK		= 59, // 阵营任务
	HELPID_FLY_FB			= 60, // 腾云副本
	HELPID_FLY_FB_TASK		= 61, // 腾云副本任务
	HELPID_QINGMING			= 62, // 清明节活动
	HELPID_HYDRANGEA		= 63, // 绣球抢亲活动
	HELPID_LONGLING			= 64, // 龙鳞活动
	HELPID_NAZHA			= 65, // 哪吒闹海
	HELPID_LIULI			= 66, // 琉璃宝盏
	HELPID_HOUWANG			= 67, // 真假猴王
	HELPID_EVERYDAY			= 68, // 每日上线
	HELPID_REDPAPER                 = 69, // 开启红包
	HELPID_ROSE			= 70, // 送玫瑰花                
	HELPID_NVERBATTLE	= 71,  // 女儿国争夺战
	HELPID_CITAN				= 72,  // 刺探任务
	HELPID_MEIRISHOUBIAN		= 73,  // 每日守边
	HELPID_YUSHU				= 74,  // 玉树悼念
	HELPID_BAOXIANGXIUQIU		= 75,  // 宝象抛绣球
	HELPID_KILLZHUREN			= 76,  // 杀死主人任务
	HELPID_KILLWEIBIN			= 77,  // 杀死卫兵任务
	HELPID_CHIJIUBATTLE			= 78,  // 持久大作战
	HELPID_YUHUAYUAN			= 79,  // 御花园副本
	HELPID_BAODING				= 80,  // 天降宝鼎活动
	HELPID_ACTIVITY520			= 81,  // 520活动
	HELPID_DUANWU				= 82,  // 端午节活动
	HELPID_CAMP_TAOFA_VNM		= 83,  // 越南阵营讨伐任务
	HELPID_GLOBALTEST			= 84,  // 全球测试活动
	HELPID_DROP_ZONGZI			= 85,  // 天降粽子活动
	HELPID_JINGDOUYUN			= 86,  // 大圣的筋斗云
	HELPID_SONGJING				= 87,  // 师门诵经
	HELPID_DUOBAO1				= 88,  // 夺宝琉璃盏1  限制每天接任务次数 需要指引
	HELPID_DUOBAO2				= 89,  // 夺宝琉璃盏2  限制每天接任务次数 需要指引
	HELPID_QITIANWAR			= 90,  // 齐天大战
	HELPID_SONGJING11			= 91,  // 师门诵经1-1
	HELPID_SONGJING12			= 92,  // 师门诵经1-2
	HELPID_SONGJING13			= 93,  // 师门诵经1-3
	HELPID_SONGJING21			= 94,  // 师门诵经2-1
	HELPID_SONGJING22			= 95,  // 师门诵经2-2
	HELPID_SONGJING23			= 96,  // 师门诵经2-3
	HELPID_MATAIWEI				= 97,  // 马太尉点兵活动
	HELPID_GONGCELIHUA			= 98,  // 公测礼花活动

	HELPID_SONGJING31			= 99,  // 师门诵经3-1
	HELPID_SONGJING32			= 100,  // 师门诵经3-2
	HELPID_SONGJING33			= 101,  // 师门诵经3-3
	HELPID_SONGJING41			= 102,  // 师门诵经4-1
	HELPID_SONGJING42			= 103,  // 师门诵经4-2
	HELPID_SONGJING43			= 104,  // 师门诵经4-3

	HELPID_SONGJING51			= 105,  // 师门诵经5-1
	HELPID_SONGJING52			= 106,  // 师门诵经5-2
	HELPID_SONGJING53			= 107,  // 师门诵经5-3
	HELPID_SONGJING61			= 108,  // 师门诵经6-1
	HELPID_SONGJING62			= 109,  // 师门诵经6-2
	HELPID_SONGJING63			= 110,  // 师门诵经6-3
	HELPID_OGREGONG				= 111,  // 怪物攻城活动

	HELPID_SUOYUAN				= 112,  // 如你所愿【越南活动】
	HELPID_YUNXIAOTA			= 113,  // 云霄塔副本
	HELPID_FANJIAN				= 114,  // 凡间的热闹
	HELPID_QIXI					= 115,  // 七夕活动
	HELPID_ZHONGYUANJIE			= 116,  // 中元节活动
	HELPID_ZHONGYUANJIE2		= 117,  // 中元节活动2
	HELPID_PUDU					= 118,  // 中元普渡
	HELPID_ZHONGQIU1			= 119,  // 中秋节1
	HELPID_ZHONGQIU2			= 120,  // 中秋节2
	HELPID_ZHONGQIU3			= 121,  // 中秋节3
	HELPID_GUOQING1				= 122,  // 国庆1
	HELPID_GUOQING2				= 123,  // 国庆2
	HELPID_YAOQIANSHU			= 124,  // 摇钱树
	HELPID_BAIHUAXIANZI			= 125,  // 百花仙子
	HELPID_WEEKNATION			= 126,  // 弱国任务
	HELPID_ONLINEGIFT			= 127,  // 在线送好礼
	HELPID_MARKET1				= 128,  // 市场宣传1
	HELPID_DATAOSHA				= 129,  // 大逃杀(个人模式)
	HELPID_DATAOSHADUI			= 130,  // 大逃杀(组队模式)
	HELPID_ZHUYUSHU				= 131,  // 茱萸束
	HELPID_JUHUAJIU				= 132,  // 菊花酒
	HELPID_FANGZHIYUAN			= 133,  // 放纸鸢
	HELPID_FANGZHIYUAN2			= 134,  // 放纸鸢2
	HELPID_YQSDROP				= 135,  // 摇钱树掉落
	HELPID_SD1					= 136,  // 圣诞节1
	HELPID_SD2					= 137,  // 圣诞节2
	HELPID_TREASUREBOWL			= 138,  // 聚宝盆活动
	HELPID_NEW_XUNBAO			= 139,  // 新寻宝琉璃盏
	HELPID_NEW_XUNBAO2			= 140,  // 新寻宝琉璃盏2
	HELPID_NEW_FUWENBAO			= 141,  // 符文包领取
	HELPID_NEW_YN_SD			= 142,  // 圣诞活动 印尼
	HELPID_2011_SPRING_TASK		= 143,  // 11春节任务
	HELPID_2011_SPRING_ACT		= 144,  // 11春节活动
	HELPID_2011_OLFPLAYER		= 145,  // 11老玩家活动
	HELPID_2011_SPRING3			= 146,  // 11春节活动3
	HELPID_2011_SPRING4			= 147,  // 11春节活动4
	HELPID_2011_QJ1				= 148,  // 11情人节1
	HELPID_2011_QJ2				= 149,  // 11情人节2
	HELPID_2011_YUANXIAO		= 150,  // 元宵节1
	HELPID_WEAK_CAMP1			= 151,  // 弱阵营1指引
	HELPID_WEAK_CAMP2			= 152,  // 弱阵营2指引
	HELPID_WEAK_CAMP3			= 153,  // 弱阵营3指引
	HELPID_WUDIDONG 			= 154,  // 无底洞副本
	HELPID_FAT_CHICKEN			= 155,  // 肥鸡活动-越南板
	HELPID_CITAN_NEW			= 156,  // 刺探任务 新
	HELPID_RUYI					= 157,  // 如意祈春
	HELPID_ONLINEGIFT2			= 158,  // 在线送好礼2期
	HELPID_HANSHI				= 159,  // 寒食祭忠魂
	HELPID_CHALIU				= 160,  // 插柳辟邪
	HELPID_CHAODU				= 161,  // 超度亡魂		
	HELPID_ZHANYAO_VNM			= 162,  // 越南斩妖夺宝

	HELPID_BHSH					= 163,  // 百花盛会
	HELPID_TNSH					= 164,  // 天女散花
	HELPID_MGCQ4				= 165,  // 玫瑰传情四月	
	HELPID_TEAMLOOP				= 166,  // 组队环任务
	HELPID_WOKOU				= 167,  // 倭寇之乱
	HELPID_GLOBALWAR			= 168,  // 跨服战场活动
	HELPID_STWMG				= 169,  // 师徒未名阁
	HELPID_LINGYD				= 170,  // 凌云渡

	HELPID_DAY_SJ				= 171,  // 每日诵经
	HELPID_FOGUANG				= 172,  // 佛光普照
	HELPID_BAOKU				= 173,  // 佛祖宝库 
	HELPID_LEIYINSI				= 174,  // 雷音寺活动
	HELPID_LEIYINSI2			= 175,  // 雷音取真经
	HELPID_XIANGBAO			= 176,  // 香包送好友
	HELPID_QUYUAN			= 177,  // 击杀黑龙慰屈原
	HELPID_YUENAN			= 178,   //越南活动
	HELPID_SHUANGBEIRONGYU	= 179,  // 双倍荣誉乐翻天
	HELPID_HEDANCENG		= 180,  // 贺诞辰，仙妖献礼
	HELPID_XCTJ				= 181,  // 喜从天降
	HELPID_FOGUANGPUZHAO	= 182,  // 佛光普照暑期活动
	HELPID_QIXI_LOVE		= 183,  // 七夕传情
	HELPID_LINGYUN_BATTLE	= 184,  // 决战凌云渡
	HELPID_COOL_SUMMER		= 185,  // 夏日清凉活动
	HELPID_VIP_WELFARE		= 186,  // 会员福利
	HELPID_SWORN_TASK		= 187,  // 结拜任务
	HELPID_MEMBERS_MOONCAKE		= 188,  // 团员月饼
	HELPID_RABBIT_WINDRUG		= 189,  // 玉兔夺药
	HELPID_REINCARNATION_SPIRIT = 190,	// 转世真灵
	HELPID_SPIRIT_COME_TO_EARTH = 191,	// 真灵降世
	HELPID_SIlVER_HAPPY_FROM_HEAVEN = 192,	// 白银喜从天降
	HELPID_HAVOC_IN_HEAVEN			= 193,	  // 72变闹天宫
	HELPID_YAOHOU_HAVOC_IN_CHANGAN  = 194,	  // 妖猴闹长安
	HELPID_GIFTS_PUMPKIN			= 195, 	  // 送礼南瓜灯
	HELPID_STAND					= 196, 	  // 定海神针
	HELPID_STAND_SECOND				= 197, 	  // 定海神针2
	HELPID_CORPS_COLLECTION			= 198,	  // 军团采集
	HELPID_TREASURE_GLASS_FIRST		= 199,	  // 寻宝琉璃盏1
	HELPID_TREASURE_GLASS_SECOND	= 200,	  // 寻宝琉璃盏2
	
	HELPID_SIZE				,	// 总数
	ACTIVITY_SIZE			,   // 活动总数
};

// 物品有效时间计时类型
enum enValidType
{
	VALIDTYPE_USE = 0,
	VALIDTYPE_GET = 1,
	VALIDTYPE_EQUIP = 2, // 装备开始计时
};


/* 可见范围
** X,Y两个参数对性能影响很重要，在满足游戏性的情况下尽可能减小。
** 如果客户端收到创建NPC消息时，距离超出当前可见X,Y,则不创建。
** 等玩家真正走进视野时服务器还会再次发送创建。
**
*/
enum emViewRadius
{
	//VIEW_RADIUS = 15,		// 地图可见范围,用于游戏内部分逻辑判断，这个后期再统一成VIEW_RADIUS_X，VIEW_RADIUS_Y.
	//						// 有可能多给客户端发送消息，但是客户端会自动修复。
	//						// 如离开NPC多远关闭服务；组队距离判断；发送队友陷阱可见/消失
	VIEW_RADIUS_X = 10,		// 水平可见范围
	VIEW_RADIUS_Y = 13,		// 垂直可见范围
};

enum ELookType
{
	LOOK_EQUIPMENT	= 0x01, // 查看装备
	LOOK_WIZARD		= 0x02, // 查看才华
	LOOK_NOUIPOP	= 0x04, // 只查询数据，不弹出UI或者提示
};

enum EHasBindItem
{
	BIND_ALL	= 0,
	BIND_YES	= 1,
	BIND_NO		= 2,
};

// 称号有效时间类型 [12/29/2009 Macro]
enum EProTitleValidType
{
	emTitleInstant	= 0,	// 即时类
	emTitleForever  = -1,	// 永久类
};

// 传送道具类型
enum enTeleItemType
{
	TELE_RANDOM = 1, // 随机传送
	TELE_DIGONG,	 // 地宫传送
	TELE_JIYI,		 // 记忆传送
	TELE_LIST,		 // 列表型传送道具
};

// 每人的记录传送点上限
#define TELE_LIST_SIZE	20

// 会员等级
enum EVipLevel
{
	EM_VIP_NONE		= 0,	// 非会员
	EM_VIP_NORMAL,			// 普通会员
};

// 洗潜能点等级
#define CLEAN_POTENTIAL_LIMIT_LEVEL 30


// 交易所

#define BROUSE_DOC_NO_LENG  ( 30 ) 
#define BROUSE_LIST_COUNT_MAX ( 5 )
#define BROUSE_DB_UID		( 1 )

// 自动挂机买药方式
enum EBuyType
{
	EM_NONE = 0,	// 无类型
	EM_BIND_YB,		// 绑定元宝
	EM_UNBIND_YB,	// 非绑定元宝
	EM_BAG_MONEY,	// 包裹金币
};


enum enTimeMgrType
{
	TIMESMGR_USEFLOWER = 1,	// 使用鲜花
	TIMESMGR_PURCHASE  = 2,	// 购买某物品数量
	TIMESMGR_USEGIFT   = 3, // 礼品
	TIMESMGR_FOGUANG   = 22, //佛光普照指引计次 
};

// 次数管理器上限
#define MAX_TIMER_MGR_LINE	100

// 每天最大使用鲜花次数
#define MAX_USE_FLOWER_TIME_PERDAY (CSceneCfgManager::GetSingleton().GetProperty_cfg()->mFlower_lmt_perday)

// 每天使用礼包次数
#define MAX_USE_GIFT_TIME_PERDAY  10

// 会员每天使用礼包次数
#define MAX_VIP_USE_GIFT_TIME_PERDAY 20

// 读条的时间误差
#define PROGRESS_TIMEERROR		500	 // 500毫秒 

// 读条的失效时间
#define  PROGRESS_INVALIDTIME	2000 // 两秒

// 家族家园的地图ID
#define FAMILY_HOME_MAPID	98

enum EVirtualItem
{
	EM_NOFUNC		=	0,
	EM_MONEY		=	1,		// 钱
	EM_BINDMONEY	=	2,		// 绑定钱
	EM_YUANBAO		=	3,		// 元宝
	EM_BINDYUANBAO	=	4,		// 绑定元宝
};


// 获取经验的原因
enum EExpReason
{
	EXPREASON_OGRE		= 0,	// 打怪获取的经验
	EXPREASON_TASK		= 1,	// 任务获取的经验
	EXPREASON_REDSTONE	= 2,	// 内丹经验
	EXPREASON_EXPITEM	= 3,	// 使用经验物品
	EXPREASON_HANDINITEM = 4,    // 上缴物品
	EXPREASON_BUFF	   = 5,    //  BUFF给的
	EXPREASON_ACTIVITY   = 6,  // 活动
	EXPREASON_EXCHANGE	 = 7,  // 兑换
};


enum WarStatus
{
	WAR_NONE	= 0,		// 无战争状态
	WAR_SUBMIT	= 1,		// 报名状态,不可再领奖
	WAR_BATTLE	= 2,		// 战争状态
	WAR_BIDOVER	= 3,		// 竞价结束状态
	WAR_NOTICE	= 4,		// 提示状态
};

#define MAX_SKILL_CD_MGR_SIZE 50 // 同时进行冷却的法宝技能的数量

#define MAX_WAR_ROOM_SIZE	200		// 战争最大人数限制

#define MAX_WAR_CAMP		3		// 战争战场数

enum BOURSE_TYPE
{
	BOURSE_TRADE_ING			= 0,
	BOURSE_TRADE_END			= 1,
	BOURSE_TRADE_CLOSE			= 2
};

enum BOURSE_KIND
{
	BOURSE_TRADE_BUY			= 0,
	BOURSE_TRADE_SELL			= 1,
};

#define BOURSE_TAX_YB( count )	( ( ( ( count ) * 20 / 1000 ) != 0 ) ? ( ( count ) * 20 / 1000 ) : 1 )
#define BOURSE_TAX_MONEY( count )	( ( ( ( count ) * 20 / 1000 ) != 0 ) ? ( ( count ) * 20 / 1000 )  : 1 )


// 黑狗丸时间的最大值
#define  MAX_BLACKPILLS_TIME 2000000000 // 20亿秒

enum EAccuseType
{
	ACCUSE_NONE		= 0,	// 无状态
	ACCUSE_YES		= 1,	// 弹劾
	ACCUSE_NO		= 2,	// 不弹劾
	ACCUSE_ABORT	= 3,	// 弃权
};

enum UpgNoticeType
{
	NOTICE_UPG_FIRST	=	4,	// 附近
	NOTICE_UPG_SECOND	=	7,	// 阵营
	NOTICE_UPG_THIRD	=	9,	// 世界
};

enum JewNoticeType
{
	NOTICE_JEW_FIRST	=	7,	// 世界
};

enum GlobalNoticeType
{
	UPG_NOTICE			=	1,	// 装备提示
	JEW_NOTICE			=	2,	// 宝石提示
};
enum RuneColorType
{
	WHITE_RUNE       =  0, //白色符文
	BLUE_RUNE		 =  1, //蓝色符文
	PURPLE_RUNE		 =  2, //紫色符文
	GOLD_RUNE		 =  3, //金色符文
};

// 法宝器灵提供的参数取值枚举
enum MWQLParamIdx
{
	QLIDX_KEENPOINTS,       // 增加的锐力
	QLIDX_CALMPOINTS,		// 增加的定力
	QLIDX_MAGICPOINTS,		// 增加的法力
	QLIDX_BIERPOINTS,		// 增加的灵力
	QLIDX_IMMORTALPOINTS,	// 增加的仙力
	QLIDX_REPLYSKILLBOC,	// 替换后法宝技能框表
	QLIDX_COLOR,			// 颜色
	QLIDX_LEVEL,			// 品级
	QLIDX_PROP,				// 印记概率
	QLIDX_MONEY,			// 印记费用
	QLIDX_COMPOS_MONEY_TYPE,	// 合成金币类型
	QLIDX_COMPOS_MONEY,			// 合成金币
	QLIDX_COMPOS_ID,			// 产出ID
	QLIDX_COMPOS_PROB,			// 产出概率
};

// 法宝升级类型
enum EMWUPTYPE
{
	MWUp_Color = 0, // 飞升
	MWUp_Level = 1, // 升品
};

#define MAX_MW_QL_COMPOSE_NUM 5 // 最多多少个器灵合成

// 全局副本的状态
enum EGLobalWar_Status
{
	EGLOBALWAR_STATUS_NULL			= 0,	// 未报名
	EGLOBALWAR_STATUS_APPLY			= 1,	// 报名状态	
	EGLOBALWAR_STATUS_APPLYSUCESS	= 2,	// 报名成功,预备进入战场
	EGLOBALWAR_STATUS_INWAR			= 3,	// 在战场中  	
	EGLOBALWAR_STATUS_INGLOBALSCENE	= 4,	// 在全局服务器中
};
