#pragma once  
//---------------------------------------------
// Declare PB class
//---------------------------------------------
#ifdef TEMPEDIT
#pragma pack(4)
#endif
class PBTemplate;
class PBItemBase;
class PBItemReturn;
class PBItemBindStone;
class PBItemScroll;
class PBItemCall;
class PBItemCard;
class PBItemTeleport;
class PBItemReturnPaper;
class PBItemTeleSymbol;
class PBItemStove;
class PBItemPetEgg;
class PBItemPetFood;
class PBItemRelife;
class PBItemSkill;
class PBItemBook;
class PBItemUpgLuck;
class PBItemUpgLeast;
class PBItemQuestion;
class PBItemTask;
class PBItemStuff;
class PBItemExp;
class PBItemShout;
class PBItemAutoShout;
class PBItemPhysic;
class PBItemHorse;
class PBStuff;
class PBProduct;
class PBItemBluePrint;
class PBItemRune;
class PBItemJewel;
class PBItemYuanBug;
class PBItemYuanEgg;
class PBItemSequence;
class PBEquipment;
class PBEquipFashion;
class PBEquipWeapon;
class PBEquipArmor;
class PBTemplateProperty;
class PBPropertyPercent;
class PBTemplatePropertyTable;
class PBMetier;
class PBNpc;
class PBSellItem;
class PBSellTable;
class PBTotalDrop;
class PBDropItem;
class PBDropTable;
class PBNPCCollect;
class PBOgreProperty;
class PBBox_Click;
class PBOgreBox;
class PBShowPet;
class PBBattlePet;
class PBMineOutput;
class PBMineOgre;
class PBMine;
class PBSkillTreeNode;
class PBSkillTree;
class PBCost;
class PBUseCondition;
class PBSkill;
class PBAureoleType;
class PBAureoleSkill;
class PBNormalSkill;
class PBTrapSkill;
class PBBuffFunc;
class PBBuff;
class PBLevelExp;
class PBPetLevelExp;
class PBProduceExp;
class PBUpg;
class PBEquipUpg;
class PBSuitProducer;
class PBSuitNormal;
class PBSpell;
class PBProduceEquip;
class PBProducePhysic;
class PBOpenSlot;
class PBCompose;
class PBTeleTable;
class PBData;
class PBPetData;
class PBSummonBeast;
class PBBeginner;
class PBDropLevel;
class PBOgreTran;
class PBMetierPro;
class PBLevelProperty;
class PBHonorToExp;
class PBHonorObtain;
class PBDiploidTable;
class PBRedStone;
class PBRedStoneExpTable;
class PBReliveTable;
class PBQuestionExp;
class PBBindChargeTable;
class PBItemMineMap;
class PBItemUpg;
class PBPKValueTable;
class PBItemToken;
class PBBlock;
class PBRelatively_Pos;
class PBItemKitBag;
class PBItemRemove;
class PBItemChange;
class PBItemAdapter;
class PBMultiCompose;
class PBTplHierogram;
class PBItemGiftBag;
class PBCardBook;
class PBTitleManager;
class PBTplBuffImmunity;
class PBTemplateTalent;
class PBTemplateLifeSkill;
class PBCTemplateDecompound;
class PBItemMagicWeapon;
class PBTplMagicWeaponProIncTable;
class PBItemMagicStone;
class PBLifeSkillScroll;
class PBSkillStorage;
class PBMagicWeaponSkillBoxProTable;
class PBFuncItem;
class PBItemSkillBook;
class PBEquipExchangeTable;
class PBMultiBuff;
class PBCDGroupTable;
class PBApConfig;
class PBLightTable;
class PBJudgeConfig;
class PBItemRedPaper;
class PBWizardTable;
class PBTrend;
class PBMWProJudgeEffTable;
class PBMWProJudgeSpirTable;
class PBMWLevelQuotietyTab;
class PBAchieveAward;
class PBPower;
class PBOfficial;
class PBOfficialTable;
class PBConstructExp;
class PBAddPKTable;
class PBMapTable;
class PBMultiAttackTable;
class PBItemVip;
class PBItemClean;
class PBAwardInfo;
class PBLeagueBag;
class PBMatrixTable;
class PBTemplateMarriageConfig; 	
class PBInvitation;
class PBProGressConfig;
class PBSysInvitation;
class PBCountTable;
class PBActivityConfig;
class PBItemRefresh;
class PBItemSummon;
class PBDirectTable;
class PBItemDirect;
class PBItemGetStar;
class PBWorldBattleConfig;
class PBLevelMoneyLmt;
class PBItemAutoCollect;
class PBFangCunTreeNode;
class PBFangCunTree;
class PBItemBlackPills;
class PBTemplateTalentStage;
class PBItemExpContainer;
class PBItemExpPills;
class PBItemExpTome;
class PBHandinTable;
class PBHandinEncourage;
class PBItemChangeCamp;
class PBAccuseConfig;
class PBComRuneConfig;
class PBMagicWeaponDecomposeTable;
class PBMagicWeaponQuickUpTable;
class PBGlobalMapConfig;
class PBCorpsBOSS;
class PBOffLineExpConfig;
class PBRepeFameTable;
class PBCastingSpiritTable;
class PBSpiritItem;
//---------------------------------------------
// End Declare
//---------------------------------------------

#define TEMPLATE_CACHE_SIZE 25 * 1024 * 1024	// 大Cache。

#ifndef ARRAY_CNT
#define ARRAY_CNT( a ) ( sizeof(a)/(sizeof(a[0])) )	
#endif
#define ARRAY_ROW( a ) ( sizeof(a)/(sizeof(a[0])) )		// 二维数组行数
#define ARRAY_COL( a ) ( sizeof(a[0])/sizeof(a[0][0]))	// 二维数组列数

#define STRING32					32
#define STRING256					256
#define MAX_TEMPLATE				100000	// 最大职业模版数
#define MAX_DIALOG					32		// 最大对话数
#define MAX_SKILL					10000	// 最大技能模版数
#define MAX_LEVEL					100		// 最大技能等级
#define MAX_METIER					10		// 最大职业数
#define MAX_GENER					150		// 最大技能树数
#define MAX_TASK					20000	// 最大任务数
#define MAX_EQUIP					11		// 一共的等级数
#define MAX_SUIT					150		// 套装数
#define MAX_GRADE					120		// 最大等级
#define MAX_MAGICWEAPON_GRADE		120		// 法宝最大等级
#define TMP_MAX_GRADE				161		// 模板配置的等级上限
#define MAGICWEAPON_MAX_GRADE		161		// 法宝的等级上限
#define MAX_QUESTION_NUM			10		// 每个答题卷最大的题目数量
#define MAX_RULE_NUM				 3		// 最多答题规则个数
#define MAX_MINEPOS					25		// 最多宝藏挖掘点
#define MAX_MINEEVENT					8		// 最多藏宝图概率事件个数
#define MAX_BOXLEVEL				5		// 宝箱级数 
#define MAX_COMPOSE					100		// 最多合成数
#define TOTAL_TALENT_STAGE			100		// 造化小阶总数
#define STEP_SWITCH					3		// 小阶切换数
#define TOTAL_TALENT_POINT			10000	// 最大天赋点

#define MAX_BUFFFUN_COUNT			( 6 )	// Buff功能数

#define PROPERTY_OPTION				2		// 属性表示种类数
#define CUR_ADD_PROPERTY			0		// 当前等级增加属性点
#define CUR_TOTAL_PROPERTY			1		// 当前等级总属性点
#define MAX_PROPERTY				6		// 最大属性数
#define OPTION_NONE					0		// 无选项 ( 主要用于跟protype.h中属性枚举想对应 )
#define	OPTION_STA					1		// 耐力选项
#define	OPTION_WIS					2		// 智力选项
#define	OPTION_STR					3		// 力量选项
#define	OPTION_CON					4		// 体质选项
#define	OPTION_SPR					5		// 精神选项

#define REDNAME_SECTION				4		// 红名4个阶段
#define PKSATE_COUNT				8		// pk 状态数量

#define PRODUCE_RANK	4			// 装备生产中材料的等级数，即1档，2档，3档，4档
#define PRODUCE_LEVEL	16			// 装备生产中生成的等级数，即1-160级
#define PRODUCE_DETAIL2 6			// 装备生产中的第二级描述，已经不用
#define PRODUCE_TYPE	10			// 装备生产中的主要类型，如武器，衣服，帽子等
#define PRODUCE_DETAIL1	11			// 装备生产中的第一级描述，如枪，重甲，物理法术等

#define MAX_FAIL_TASKS 10			// NPC消失 最多可能引起多少任务失败
#define MAX_TITLE		500			// 称号数上线

#define MAX_LIFESKILL_DEGREESTEP 10		// 自身熟练度与配方熟练度的差值阶层
#define MAX_LIFESKILL_LEVEL		  10	// 生活技能的最高等级
#define MAX_LIFESKILL_COMPOSENUM  300	// 每种生活技能的最大配方书目

#define SKILL_STORAGE_LMT 100			// 技能库容量
#define IMMU_SKILL_LMT	  30			// 法宝佩戴时可以免疫的技能列表的上限
#define MAX_MAGICSTONE_USEDTIMES 5		// 附魔物品的最大附魔次数	
#define MAX_COMPOSEkIND_NUM	 23			// 生活技能类型个数
#define DECOMPOUND_MAXTABLEINDEX   32	// 装备分解表的最大等级分段(5级为一段)
#define SKILL_STORAGE_LMT 100		// 技能库容量
#define SKILL_BOX_LMT	  5			// 法宝技能框最大数量
#define MAX_EQUIPCOMPOSEID_NUM	100	// 装备兑换的最大配方ID数量

#define MAX_MUTIBUFFEVENT		11	// 最大组队buff事件
#define MAX_EVENTPARAM	6

// 才华最大等级
#define MAX_WIZARD_GRADE	99
// 才华最大模板数
#define MAX_WIZARD_TEMP		100
#define MAX_JUDGE_LEVEL	12	// 评价级别上限
#define MAX_JUDGE_OTHER	20	// 最多可评价人数

#define MAX_PAPERVALUE_NUM	10	// 红包每种金钱的掉落所有可能的数量

#define MAX_PHYSIC_NUM 5	// 药品类型数量

#define MAX_MW_PERCENT_NUM	200		// 法宝百分比数值的数量

#define MAX_ACHIEVE_ITEM	5	// 成就每项奖励物品上限
#define MAX_ACHIEVE_AWARD	100	// 成就奖励项上限
#define MAX_POWER			100	// 官职权力最大数
#define MAX_OFFICAL_NUM		6	// 每级官员最大数量
#define MAX_CONSTRUCT_LEVEL	5	// 建设度最高等级
#define MAX_PEACE_TYPE		7	// 友好关系的种类
#define MAX_MAP_NUM			200	// 最大地图数量
#define MAX_LEAGUEITEMINBAG	20	// 每个家族军团特供物品能够开出的最大物品数量
#define MAX_MATRIX_LEVEL	4	// 传销最大等级
#define MAX_MATRIX_MENBER	130	// 传销最大数量
#define MAX_WEDDING_MODULENUM 3	// 婚礼的最大规模数量
#define MAX_WEDDING_NPCNUM	15	// 婚礼的最大npc数量
#define MAX_WEDDING_KEEPSNAKENUM 20 // 信物的最大数量
#define MAX_PROGRESSFUNC_NUM	50	// 使用进度条检查的最大功能数
#define MAX_COUNT_NUM			100
#define MAX_HYDRANGEA_SUGARNUM  20	// 绣球抢亲活动中最大喜糖种类
#define MAX_AUTOCOLLECT_MATERIALSNUM 20 //  自动采集的产出品数量

#define MAX_PRELIMINARY_TIMELYLISTNUM	10	// 及时排行榜的最大人数
#define MAX_MASTER_NUM		3	// 最大师傅数量
#define	MAX_BEGINNER_SLAVE_NUM	30	// 记名弟子数量
#define MAX_MASTER_AWARD_NUM	15	// 最大仙缘奖励数量

#define MAX_HANDINENCOURAGE_NUM     20      // 最大上缴奖励数
#define MAX_HANDINTABLE             10      // 最大上缴奖励对应表数
#define MAX_HANDINENCOURAGE         200     // 最大上缴奖励表数
#define MAX_RUNECOMTYPE				9    // 最大升级符文上限

#define MAX_CORPSBOSSNUM			20	 // 最大的军团boss数
#define MAX_CASTINGSPIRIT_POSITION			6	 // 铸灵部位数
#define MAX_CASTINGSPIRIT_TYPE				3	 // 铸灵系别数
#define MAX_CASTINGSPIRIT_SLOT				8	 // 铸灵每个系别孔数
#define MAX_SPIRITITEM_GRADE				8	 // 真灵道具最大等级
#define MAX_SPIRITUPGRADE_ITEMNUM			4	 // 聚灵需要的个数上限（灵珠格数）

enum EM_FameType							// 声望类型
{
	FAME_TYPE_COMBAT			= 1,
	FAME_TYPE_REPE				= 2,
	FAME_TYPE_COMMON			= 3,
};


//---------------------------------------------
// Main Section
//---------------------------------------------

enum TEMPTYPE
{
	TEMP_LEVELEXP       = 0,				// 等级经验表
	TEMP_DATA       = 1,					// 数值修正表
	TEMP_UPGRADE       = 2,					// 装备升级表
	TEMP_OPENSLOT       = 3,				// 开槽表
	TEMP_PROSUIT       = 4,					// 生产者套装
	TEMP_PROEXP       = 5,					// 生产等级的经验
	TEMP_BEGINNER       = 6,				// 新手设置
	TEMP_DROPLEVEL       = 7,				// 掉落装备等级表
	TEMP_HONORTOEXP       = 8,				// 功勋经验转换表
	TEMP_HONOROBTAIN       = 9,				// 功勋获得表
	TEMP_RELIVETABLE       = 10,			// 复活花费表
	TEMP_QUESTIONEXPTABLE       = 11,		// 答题经验表
	TEMP_REDSTONEEXPTABLE       = 12,		// 内丹经验表
	TEMP_BINDCHARGETABLE       = 13,		// 绑定花费表
	TEMP_PKVALUE       = 14,				// PK值范围表
	TEMP_TITLE       = 15,					// 称号配置表
	TEMP_TALENT       = 16,					// 造化表
	TEMP_MAGIC_WEAPON_PRO       = 17,		// 法宝属性升级表
	TEMP_MULTIBUFF       = 18,				// buff组合
	TEMP_CDGROUP       = 19,				// 冷却时间
	TEMP_LIGHT       = 20,					// 发光配置
	TEMP_APCONFIG       = 21,				// 活力配置表
	TEMP_WIZARD       = 22,					// 才华经验表
	TEMP_JUDGECONFIG       = 23,			// 评价配置表
	TEMP_OFFICAL       = 24,				// 官职功能表
	TEMP_OFFICALLIST       = 25,			// 官职配置表
	TEMP_KING       = 26,					// 国王功能
	TEMP_CONSTRUCTEXP       = 27,			// 建设度表
	TEMP_ADDPKTABLE       = 28,				// PK附加表
	TEMP_MAPTABLE       = 29,				// 地图势力表
	TEMP_MULTIATTACKTABLE       = 30,		// 群攻伤害表
	TEMP_DOUBLE       = 31,					// 双倍时间表
	TEMP_MATRIXTABLE       = 32,			// 传销配置表
	TEMP_COUNT       = 33,					// 计数表
	TEMP_DIRECT		= 34,					// 直升配置表
	TEMP_BATTLECONFIG = 35,					// 国战配置表
	TEMP_MONEYLMT_CFG = 36,					// 金钱上限配置表
	TEMP_FANGCUNTREE = 37,					// 方寸山技能
	TEMP_TALENTSTAGE = 38,					// 造化大阶
	TEMP_CHANGECAMP = 39,					// 转阵营
	TEMP_ACCUSECONFIG = 40,					// 弹劾配置
	TEMP_RUNECOMCONFIG = 41,					// 符文升级配置置
	TEMP_ACHIEVEAWARD = 42,					// 成就奖励表
	TEMP_GLOBALMAPCONFIG = 43,				// 全局地图配置表	
	TEMP_CORPSBOSSTABLE = 44,				// 军团boss表
	TEMP_OFFLINEEXPCONFIG = 45,				// 离线经验配置表
	TEMP_REPEFAMETABLE = 46,				// 副本声望配置表
	TEMP_CASTING_SPIRIT_TABLE = 47,			// 铸灵配置表
};

//---------------------------------------------
// Main Section
//---------------------------------------------

enum ITEMTYPE
{
	ITEM_RETURN       = 1,							// 回城卷轴
	ITEM_TELEPORT       = 2,						// 传送物品
	ITEM_EQUIPUPG       = 3,						// 装备升级物品
	ITEM_RELIFE       = 4,							// 复活卷轴
	ITEM_PETEGG       = 5,							// 宠物蛋
	ITEM_PETFOOD       = 6,							// 宠物食品
	ITEM_UPGLUCK       = 7,							// 装备升级幸运符
	ITEM_UPGLEAST       = 8,						// 装备升级保底符
	ITEM_REFRESHTASK       = 9,						// 刷任务用道具
	ITEM_TASK       = 10,							// 任务道具
	ITEM_PRODUCE       = 11,						// 生产原料
	ITEM_SHOUT       = 12,							// 喇叭喊话道具
	ITEM_PHYSIC       = 13,							// 药品
	ITEM_SKILL       = 14,							// 调用技能的物品
	ITEM_BOOK       = 15,							// 技能书
	ITEM_EXP       = 16,							// 增加经验道具
	ITEM_WEAPON       = 17,							// 武器装备
	ITEM_ARMOR       = 18,							// 防护装备
	ITEM_FASHION       = 19,						// 时装装备
	ITEM_RUNE       = 20,							// 符文道具
	ITEM_JEWEL       = 21,							// 珠宝道具
	ITEM_YUANBUG       = 22,						// 元气虫道具
	ITEM_YUANEGG       = 23,						// 元气蛋道具
	ITEM_SPECIAL_TASK       = 24,					// 特殊任务物品
	ITEM_AUTOSHOUT                = 25,		        // 自动喊话道具
	ITEM_HORSE       = 26,							// 坐骑道具
	ITEM_BINDSTONE                = 27,		        // 绑定石	
	ITEM_SCROLL       = 28,							// 仙轴
	ITEM_CARD       = 29,							// 卡片
	ITEM_RETURNPAPER       = 30,					// 指定传送卷
	ITEM_TELESYMBOL       = 31,						// 指定传送符
	ITEM_REDSTONE       = 32,						// 内丹
	ITEM_STOVE       = 33,							// 指定炉石
	ITEM_QUESTIONPAPER       = 34,					// 题卷
	ITEM_MINEMAP       = 35,						// 藏宝图
	ITEM_TOKEN       = 36,							// 惩妖令牌
	ITEM_KITBAG       = 37,							// 背包
	ITEM_REMOVE       = 38,							// 宝石摘除符
	ITEM_CHANGE       = 39,							// 抗性转化石
	ITEM_ADAPTER       = 40,						// 物品适配器
	ITEM_CALL       = 41,							// 召唤类物品
	ITEM_GIFTBAG       = 42,						// 礼包类物品
	ITEM_GOLDSTONE       = 43,						// 点金石
	ITEM_PLANT       = 44,							// 采集的植物
	ITEM_MINERAL       = 45,						// 采集产生的矿物  	
	ITEM_MAGICWEAPON       = 46,					// 法宝
	ITEM_MAGICSTONE       = 47,						// 附魔物品
	ITEM_LIFESKILLSCROLL       = 48,				// 生活技能配方卷轴
	ITEM_FUNCITEM       = 49,						// 功能性物品
	ITEM_SKILLBOOK       = 50,						// 法宝技能书
	ITEM_CORPSTOKEN       = 51,						// 家族将军令
	ITEM_REDPAPER       = 52,						// 红包
	ITEM_VIP       = 53,							// 会员道具
	ITEM_CLEAN       = 54,							// 洗点道具
	ITEM_LEAGUEBAG       = 55,						// 家族军团特供物品
	ITEM_INVITATION       = 56,						// 结婚请帖
	ITEM_SYSINVITATION       = 57,					// 系统发放的请帖	
	ITEM_REFRESH       = 58,						// 洗属性道具
	ITEM_SUMMON			= 59,						// 召唤道具
	ITEM_DIRECT			= 60,						// 直升道具
	ITEM_GETSTAR		= 61,						// 摘星物品
	ITEM_AUTOCOLLECT	= 62,						// 自动采集道具
	ITEM_BLACKPILLS		= 63,						// 黑狗丸
	ITEM_EXPCONTAINER	= 64,							// 修炼经验丹
	ITEM_EXPPILLS		= 65,							// 经验丹
	ITEM_SPIRIT_CASTING	= 67,						// 灵珠
	ITEM_EXP_TOME	= 70,								//修炼心德， 新版修炼经验丹
};

enum ADAPTERTYPE
{
	ADAPTER_NPC			= 1,			// NPC适配器
	ADAPTER_OGRE		= 2,			// 怪物适配器	
	ADAPTER_HIEROGRAM	= 3,			// 神符适配器
	ADAPTER_MONEY		= 4,			// 金钱适配器
	ADAPTER_BINDMONEY	= 5,			// 绑定金适配器
	ADAPTER_YUANBAO		= 6,			// 元宝适配器
	ADAPTER_EXP			= 7,			// 经验适配器
	ADAPTER_COLLECTNPC	= 8,			// 采集NPC适配器
};

enum SKILLITEMOPRTYPE
{
	TYPE_ADDBUFF2SENDNPC = 1,			// 给正在护送的NPC加个BUFF
	TYPE_REMOVEBUFF2SENDNPC = 2,		// 给正在护送的NPC去掉BUFF
};


// 模板类型，用于从基类区分各种类型模板
enum emTemplateType
{
	EM_TEMPTYPE_NONE	= 0,
	EM_TEMPTYPE_ITEM	= 1,	// 物品类
	EM_TEMPTYPE_NPC		= 2,	// NPC类
	EM_TEMPTYPE_OGRE	= 3,	// 怪物
	EM_TEMPTYPE_BLOCK	= 4,	// 阻挡
	EM_TEMPTYPE_COLLECT	= 5,	// 采集NPC
	EM_TEMPTYPE_BOX		= 6,	// 宝箱
};



// 所有模板的基类
class CTemplate
{
public:
	int		mTempID;

#ifndef TEMPEDIT
	emTemplateType		mTemplateType;
#endif

	CTemplate() 
	{
		mTempID = 0;
#ifndef TEMPEDIT
		mTemplateType = EM_TEMPTYPE_NONE;
#endif
	}

	~CTemplate() {}

};

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			        ## 代表生成到数据库中
// name		= 上缴奖励表		    ## 模版名称
// version	= 1				        ## 版本号
// path		= config/上缴奖励表     ## 路径
// parent	=           	        ## 父亲模版
// ********************************************************************** //

class CTplHandinEncourage : public CTemplate
{
public:
    // 上缴物品类型
    enum EN_HANDINITEM_TYPE
    {
        EN_HIIT_ITEM    = 0    // 上缴物品
    };
    // 奖励物品类型
    enum EN_ENCOURAGEITEM_TYPE
    {
        EN_EIT_EXP      = 0    // 经验奖励
    };
public:
    // FieldName: 上缴物品类型
    // FieldType: INT4
    // Type:	  COMBOENUM(EN_HANDINITEM_TYPE)
    int		      mHandinItemType;	

    // FieldName: 上缴物品ID
    // FieldType: INT4
    // Type:	  EDITNUMBER(0,100000)
    int		      mHandinItemID;

    // FieldName: 上缴物品数量
    // FieldType: INT4
    // Type:	  EDITNUMBER(0,100000)
    int		      mHandinItemNum;

    // FieldName: 奖励物品类型
    // FieldType: INT4
    // Type:	   COMBOENUM(EN_ENCOURAGEITEM_TYPE)
    int		      mEncourageType;

    // FieldName: 奖励物品ID
    // FieldType: INT4
    // Type:	  EDITNUMBER(0,100000)
    int		      mEncourageId;	

    // FieldName: 奖励物品数量
    // FieldType: INT4
    // Type:	  EDITNUMBER(0,100000)
    int		      mEncourageNum;	

public:

    void SetFromPB( bool bServer, PBHandinEncourage* pHandinEncourage );
    void CreateTlvPB( bool bServer, PBHandinEncourage* pHandinEncourage );
    int Initialize();
    CTplHandinEncourage()
    {
        Initialize();
    }
    ~CTplHandinEncourage(){}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			        ## 代表生成到数据库中
// name		= 上缴奖励对照表		## 模版名称
// version	= 1				        ## 版本号
// path		= config/上缴奖励对照表 ## 路径
// parent	=           	        ## 父亲模版
// ********************************************************************** //

class CTplHandinTable : public CTemplate
{
public:
    // FieldName: 活动编号
    // FieldType: INT4
    // Type:	  EDITNUMBER(0,1000)
    int		      mActivityId;	

    // FieldName: 第%d种上缴物品所应得到的
    // FieldType: INT4
    // Type:	  TEMPPATH
    int		      mEncourageTplId[MAX_HANDINENCOURAGE_NUM];	

public:

    void SetFromPB( bool bServer, PBHandinTable* pHandinTable );
    void CreateTlvPB( bool bServer, PBHandinTable* pHandinTable );
    int Initialize();
    CTplHandinTable()
    {
        Initialize();
    }
    ~CTplHandinTable(){}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= true			## 代表不生成到数据库中
// name		= 道具基本信息	## 模版名称
// version	=				## 版本号
// path		=				## 路径
// parent	=				## 父亲模版
// ********************************************************************** //

class CTplItem : public CTemplate
{
public:	

	enum
	{
		ITEM_RETURN       = 1,								// 回城卷轴
		ITEM_TELEPORT       = 2,							// 传送物品
		ITEM_EQUIPUPG       = 3,							// 装备升级物品
		ITEM_RELIFE       = 4,								// 复活卷轴
		ITEM_PETEGG       = 5,								// 宠物蛋
		ITEM_PETFOOD       = 6,								// 宠物食品
		ITEM_UPGLUCK       = 7,								// 装备升级幸运符
		ITEM_UPGLEAST       = 8,							// 装备升级保底符
		ITEM_REFRESHTASK       = 9,							// 刷任务用道具
		ITEM_TASK       = 10,								// 任务道具
		ITEM_PRODUCE       = 11,							// 生产原料
		ITEM_SHOUT       = 12,								// 喇叭喊话道具
		ITEM_PHYSIC       = 13,								// 药品
		ITEM_SKILL       = 14,								// 调用技能的物品
		ITEM_BOOK       = 15,								// 技能书
		ITEM_EXP       = 16,								// 增加经验道具
		ITEM_WEAPON       = 17,								// 武器装备
		ITEM_ARMOR       = 18,								// 防护装备
		ITEM_FASHION       = 19,							// 时装装备
		ITEM_RUNE       = 20,								// 符文道具
		ITEM_JEWEL       = 21,								// 珠宝道具
		ITEM_YUANBUG       = 22,							// 元气虫道具
		ITEM_YUANEGG       = 23,							// 元气蛋道具
		ITEM_SPECIAL_TASK       = 24,						// 特殊任务物品
		ITEM_AUTOSHOUT                = 25,		            // 自动喊话道具
		ITEM_HORSE       = 26,								// 坐骑道具
		ITEM_BINDSTONE                = 27,		            // 绑定石
		ITEM_SCROLL       = 28,								// 仙轴
		ITEM_CARD       = 29,								// 卡片
		ITEM_RETURNPAPER       = 30,						// 指定传送卷
		ITEM_TELESYMBOL       = 31,							// 指定传送符
		ITEM_REDSTONE       = 32,							// 内丹
		ITEM_STOVE       = 33,								// 指定炉石
		ITEM_QUESTIONPAPER       = 34,						// 题卷
		ITEM_MINEMAP       = 35,							// 藏宝图
		ITEM_TOKEN       = 36,								// 惩妖令牌
		ITEM_KITBAG       = 37,								// 背包
		ITEM_REMOVE       = 38,								// 宝石摘除符
		ITEM_CHANGE       = 39,								// 抗性转化石
		ITEM_ADAPTER       = 40,							// 物品适配器
		ITEM_CALL       = 41,								// 召唤类物品
		ITEM_GIFTBAG       = 42,							// 礼包类物品
		ITEM_GOLDSTONE       = 43,							// 点金石
		ITEM_PLANT       = 44,								// 采集的植物
		ITEM_MINERAL       = 45,							// 采集产生的矿物  	 
		ITEM_MAGICWEAPON       = 46,						// 法宝
		ITEM_MAGICSTONE       = 47,							// 附魔物品
		ITEM_LIFESKILLSCROLL       = 48,					// 生活技能配方卷轴
		ITEM_FUNCITEM       = 49,							// 功能性物品
		ITEM_SKILLBOOK       = 50,							// 法宝技能书
		ITEM_CORPSTOKEN       = 51,							// 家族将军令
		ITEM_REDPAPER       = 52,							// 红包
		ITEM_VIP       = 53,								// 会员道具
		ITEM_CLEAN       = 54,								// 洗点道具
		ITEM_LEAGUEBAG       = 55,							// 家族军团特供物品包
		ITEM_INVITATION       = 56,							// 结婚请帖
		ITEM_SYSINVITATION       = 57,						// 系统发放的请帖	
		ITEM_REFRESH       = 58,							// 洗属性道具
		ITEM_SUMMON			= 59,							// 召唤道具
		ITEM_DIRECT			= 60,							// 直升道具
		ITEM_GETSTAR		= 61,							// 摘星物品
		ITEM_AUTOCOLLECT	= 62,							// 自动采集道具
		ITEM_BLACKPILLS		= 63,							// 黑狗丸
		ITEM_EXPCONTAINER	= 64,							// 修炼经验丹
		ITEM_EXPPILLS		= 65,							// 经验丹	
		ITEM_SPIRIT_CASTING	= 67,							// 灵珠
		ITEM_EXP_TOME	= 70,								//修炼心德， 新版修炼经验丹
	};

	enum
	{
		PROCESSTYPE_CANSALE		= 0x0001,	// 是否可以卖店
		PROCESSTYPE_MAKEGUID	= 0x0002,	// 是否生成唯一ID
		PROCESSTYPE_BINDONPICK	= 0x0004,	// 是否在拾取后绑定
		PROCESSTYPE_BINDONEQUIP = 0x0008,	// 是否在装备后绑定
		PROCESSTYPE_BINDONUSER	= 0x0010,	// 是否在使用后绑定
		PROCESSTYPE_CANBIND		= 0x0020,	// 是否可手动绑定
		PROCESSTYPE_ISRMBITEM	= 0x0040,	// 是否人民币物品
		PROCESSTYPE_ISREFRESHTASK= 0x0080,	// 能否刷新库任务
	};

public:

	// FieldName: 道具类型
	// FieldType: INT4
	// Type:	  COMBOENUM(ITEMTYPE)
	int		mItemType;			

	// FieldName: 处理方式
	// FieldType: INT4
	// Type:	  CHECKMUTI(是否可以卖店,是否生成唯一ID,是否在拾取后绑定,是否在装备后绑定,是否在使用后绑定,是否可手动绑定,是否人民币物品,能否刷新库任务)
	int		mProcessType;			

	// FieldName: 堆叠上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,1000)
	int		mPileLimit;				 

	// FieldName: $道具显示名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mItemName[ STRING32 ];

	// FieldName: 道具掉落模型名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mModelName[ STRING32 ];	

	// FieldName: 道具图标名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mIconName[ STRING32 ];

	// FieldName: 道具卖店价
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int		mPriceSale;

	// FieldName: 道具店卖价
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int		mPricePurchase;	

	// FieldName: $物品详细说明
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char	mDescribe[ STRING256 ];

	// FieldName: $道具类型描述
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mTypeDesc[ STRING32 ];

	// FieldName: 店卖价是否支持绑定金钱
	// FieldType: INT4
	// Type:	  COMBOSINGLE(是=0,否=1)
	int		mPurchaseBindMoney;

	// FieldName: 有效时间
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int		mLastTime;	

	// FieldName: 卖店价是否支持绑定金钱
	// FieldType: INT4
	// Type:	  COMBOSINGLE(是=0,否=1)
	int		mSaleBindMoney;	

	// FieldName: 使用后是否停止自动攻击
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不停止=0,停止=1)
	int		mStopAttack;

	// FieldName: 右键点击是否可以使用
	// FieldType: INT4
	// Type:	  COMBOSINGLE(是=0,否=1)
	int		mCanUse;

	// FieldName: 使用的技能ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mSkillID;

	// FieldName: 物品等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mLevel;

	// FieldName: 使用后是否消失
	// FieldType: INT4
	// Type:	  COMBOSINGLE(是=0,否=1)
	int		mDisappear;

	// FieldName: 冷却组第%d项
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mCDGroup[ 25 ];

	// FieldName: 限制使用地图ID(0表示不限制)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mUseMapID;

	// FieldName: 自获得有效时间(分钟)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,90000000)
	int		mValidTime;

	// FieldName: 有效时间计时类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(使用开始计时=0,进包开始计时=1,装备开始计时=2)
	int		mValidType;

	// FieldName: 限制第%d个持有地图ID(0表示不限制)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mLmtMapID[ 5 ];

	// FieldName: 公用组标志
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mCommonID;

	// FieldName: 化金类型
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mEatType;

	// FieldName: 化金值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mEatValue;

	// FieldName: 使用类型
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mCountType;

	// FieldName: 限制使用的阵营ID
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无=0,聚仙=1,乾坤=2,菩提=3,混元=4)
	int		mLmtLineID;

	// FieldName: 限制副本中使用%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mLmtRepetionID[ 5 ];

	// FieldName: 限制第%d个使用地图ID(0表示不限制)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mLmtUseMapID[ 20 ];

	// FieldName: 合成配方ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mCompID;

	// FieldName: 到期绝对日期
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,29991231)
	int		mInvilidDate;

public:

	bool IsEquipable( ) const
		{ return mItemType == ITEM_WEAPON || mItemType == ITEM_ARMOR; }
	bool IsMagicWeapon() const
	{
		return mItemType == ITEM_MAGICWEAPON;
	}

	void SetFromPB( bool bServer, PBItemBase* pItem );
	void CreateTlvPB( bool bServer, PBItemBase* pItem );
	int Initialize();

	CTplItem() { 
#ifndef TEMPEDIT
		mTemplateType = EM_TEMPTYPE_ITEM; 
#endif
		Initialize();
	}
	~CTplItem() { }

};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 回城卷轴		## 模版名称
// version	= 1				## 版本号
// path		= 物品/回城卷轴	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplItemReturn : public CTplItem
{
public:
	// FieldName: 道具使用时间
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mTimer;	

public:

	void SetFromPB( bool bServer, PBItemReturn* pItem );
	void CreateTlvPB( bool bServer, PBItemReturn* pItem );
	int Initialize();
	CTplItemReturn()
	{
		Initialize();
	}
	~CTplItemReturn(){}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 法宝技能书	## 模版名称
// version	= 1				## 版本号
// path		= 物品/法宝技能书	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplItemSkillBook : public CTplItem
{
public:
	enum BOOK_COLOR
	{
		COLOR_NONE    = 0,  // 无限制
		COLOR_WHITE   = 1,  // 白色
		COLOR_BLUE    = 2,  // 蓝色
		COLOR_PURPLE  = 3,  // 紫色
		COLOR_GOLD    = 4,  // 金色
	};

	// FieldName: 技能书颜色
	// FieldType: INT4
	// Type:	  COMBOENUM(BOOK_COLOR)
	int			mBookColor;

	// FieldName: 对应技能
	// FieldType: INT4
	// Type:	  SKILLPATH
	int			mSkillID;

	// FieldName: 成功概率(万分比)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mSuccProb;

	// FieldName: 消耗的金钱类型
	// FieldType: INT4
	// Type:	  COMBOENUM(MW_DECOMPOS_MONEY_TYPE)
	int			mNeedMoneyType;	

	// FieldName: 消耗的金钱数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int			mNeedMoney;	

public:

	void SetFromPB( bool bServer, PBItemSkillBook* pItem );
	void CreateTlvPB( bool bServer, PBItemSkillBook* pItem );
	int Initialize();
	CTplItemSkillBook(){ Initialize(); }
	~CTplItemSkillBook() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// 功能码
enum FUNC_CODE
{
	FUNCCODE_RECVMWHP	= 1,  // 恢复法宝HP  参数0是增加的点数
	FUNCCODE_RECVMWMP	= 2,		  // 恢复法宝MP  参数0是增加的点数
	FUNCCODE_JUDGEMWPRO	= 3,	  // 鉴定法宝属性 参数0:法宝对应的影响度鉴定表 参数1:法宝对应的灵性鉴定表
	FUNCCODE_FAMILYEXPCARD= 4,	  // 家族经验令牌
	FUNCCODE_CORPSTOKEN	  = 5,	  // 军团扩军令
	FUNCCODE_MW_WASH_SMALL= 6,	  // 法宝小洗石  无参数
	FUNCCODE_MW_WASH_BIG  = 7,	  // 法宝大洗石  无参数
	FUNCCODE_WIZARDVALUE  = 8,	  // 获得才华值道具  参数0 类别 参数1 才华值
	FUNCCODE_GETTITLE     = 9,		  // 称号卷轴 参数0 称号ID
	FUNCCODE_LIHUA		  = 10,	  // 礼花道具 0:播放方式 1:数最小值 2:数最大值 3:播放个数4:buff编号5:倍率6:经验表id
	FUNCCODE_XIANHUA	  = 11,		  // 鲜花道具 参数0:对应内丹经验表ID 参数1:魅力值 参数2:是否广播 参数3:播放特效的参数 参数4:获取的亲密度值
	FUNCCODE_MATRIX		  = 12,		  // 传销道具 参数0 奖励的等级 参数1 奖励的点数	
	FUNCCODE_LEVEL		  = 13,			  // 升级道具 参数0 最高使用等级
	FUNCCODE_MONEYTREE	  = 14,		  // 摇钱树
	FUNCCODE_REPAIR		  = 15,		  // 修理道具
	FUNCCODE_COMMONBIND	  = 16,	  // 普通绑定道具
	FUNCCODE_NEEDLE		  = 17,		  // 绣花针	参数0：绣花针适用的最大背包格子数 参数1：背包使用时间(分钟)
	FUNCCODE_LUCKYBAG	  = 18,		  // 乾坤袋 参数0：类别ID(默认填模板ID) 参数1:是否必须有门派才能开启
	FUNCCODE_CLEARRED	  = 19,		// 洗红名道具 参数0：限制等级
	FUNCCODE_FAMILY_MONEY = 20,		// 家族金锭 参数0： 金锭钱数
	FUNCCODE_CORPS_MONEY = 21,		// 军团金锭 参数0： 金锭钱数
	FUNCCODE_DUNGEON_TIME_DEL = 22,	// 副本次数清除符 参数0: 副本ID (暂时就支持减一次) 
	FUNCCODE_EXTRA		 = 23,		// 印鉴物品 参数0 : 印类型	
	FUNCCODE_POINT		 = 24,		// 加潜能点技能点 参数0 潜能点数 参数1 技能点数  参数2 使用等级  
	FUNCCODE_EXP		= 25,		// 经验卷轴 参数0：经验数1:经验表模板id 2:经验万分比 
	FUNCCODE_ACTIVITY	= 26,		// 活动道具 参数0:活动ID 1:触发事件ID
    FUNCCODE_XIANLU     = 27,       // 仙露道具 参数0：0为小仙露，1为中仙露，2为大仙露
    FUNCCODE_JINPING    = 28,       // 晶瓶道具 参数0：0为晶玉瓶（中）1为晶玉瓶（大）
    FUNCCODE_JINYEZHI   = 29,       // 金叶子道具
	FUNCCODE_FASHIONNEEDLE= 30,       // 时装绣花针	参数0:时装类型:(1.白2.蓝4.紫8.金 :使用时颜色值加和) 参数1:时装延长的时间(分钟)
	FUNCCODE_CLEARMWSP  = 31,       // 法宝清除技能点道具
	FUNCCODE_CHANGERUNE = 32,       // 转换符
	FUNCCODE_DIGITALFIREWORKS = 33, // 数字礼花	参数0:可以播放礼花数目	 参数1:特效类型
	FUNCCODE_UPRUNE     = 34,       //升级符道具 参数0：概率
	FUNCCODE_QILING     = 35,       // 法宝器灵 参数0-4:加成锐定法灵仙力点数 5:替换后法宝技能框表 6:颜色 7:品级 8:印记概率 9:印记费用 10:合成金币类型 11:合成金币 12:产出ID 13:产出概率
	FUNCCODE_MWPIECE     = 36,       // 法宝碎片 参数0:品级
	FUNCCODE_MWPRNTLUCKY     = 37,       // 法宝印记幸运符 参数0:增加概率万分比 参数1:品级
	FUNCCODE_MWGETEXP     = 38,       // 法宝经验丹 参数0:经验表ID
	FUNCCODE_CORPCRYSTAL  = 39,		  // BOSS魂魄 参数0: BOSS ID	  参数1:经验值  参数2: 军团声望 参数3:军团贡献度
	FUNCCODE_CORPBOSSSTONE = 40,	  // 炼妖丹  参数0:炼化度 参数1:军团贡献度
	FUNCCODE_FAME		 = 41,		  // 声望 参数0：类别（1竞技2副本3历练）参数1：增加值
};

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 功能性物品	## 模版名称
// version	= 1				## 版本号
// path		= 物品/功能性物品	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplFuncItem : public CTplItem
{
public:

	// FieldName: 道具功能
	// FieldType: INT4
	// Type:	  COMBOENUM(FUNC_CODE)
	int		mFuncCode;	

	// FieldName: 参数0
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,100000000)

	// FieldName: 参数1
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,100000000)

	// FieldName: 参数2
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,100000000)

	// FieldName: 参数3
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,100000000)

	// FieldName: 参数4
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,100000000)

	// FieldName: 参数5
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,100000000)

	// FieldName: 参数6
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,100000000)

	// FieldName: 参数7
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,100000000)

	// FieldName: 参数8
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,100000000)

	// FieldName: 参数9
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,100000000)

	// FieldName: 参数-10
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,100000000)

	// FieldName: 参数-11
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,100000000)

	// FieldName: 参数-12
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,100000000)

	// FieldName: 参数-13
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,100000000)

	// FieldName: 参数-14
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,100000000)

	// FieldName: 参数-15
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,100000000)
	int		mParams[16];	

public:
	void SetFromPB( bool bServer, PBFuncItem* pItem );
	void CreateTlvPB( bool bServer, PBFuncItem* pItem );
	int Initialize();
	CTplFuncItem() {Initialize();}
	~CTplFuncItem(){}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 绑定石		## 模版名称
// version	= 1				## 版本号
// path		= 物品/绑定石	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplItemBindStone : public CTplItem
{
public:
	//绑定石类型
	enum BINDSTONE_TYPE
	{
		BINDSTONE_EQUIP                = 0,			// 装备绑定石
		BINDSTONE_MAGICWEAPON  = 1,			// 法宝绑定石
	};
public:
	// FieldName: 表情组ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mFaceGroupID;	

	// FieldName: 绑定石类型
	// FieldType: INT4
	// Type:	COMBOENUM(BINDSTONE_TYPE)
	int			mBindStoneType;

public:
	void SetFromPB( bool bServer, PBItemBindStone* pItem );
	void CreateTlvPB( bool bServer, PBItemBindStone* pItem );
	int Initialize();
	CTplItemBindStone(){Initialize();}
	~CTplItemBindStone() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 仙轴			## 模版名称
// version	= 1				## 版本号
// path		= 物品/仙轴		## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplItemScroll : public CTplItem
{
public:

	// FieldName: 使用后触发任务
	// FieldType: INT4
	// Type:	  TASKPATH
	int		mGetTaskID;	

public:
	void SetFromPB( bool bServer, PBItemScroll* pItem );
	void CreateTlvPB( bool bServer, PBItemScroll* pItem );
	int Initialize();
	CTplItemScroll(){Initialize();}
	~CTplItemScroll(){}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //



// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 召唤类物品	## 模版名称
// version	= 1				## 版本号
// path		= 物品/召唤类物品	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplItemCall : public CTplItem
{
public:

	// FieldName: 使用后召唤NPC
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mCallNpcID;	

	// FieldName: 召唤NPC位置的x坐标 -1表示身边
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1,1000)
	int		mX;	

	// FieldName: 召唤NPC位置的y坐标 -1表示身边
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1,1000)
	int		mY;	

	// FieldName: 使用后是否消失
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int		mIsDestroy;	

	// FieldName: 召唤NPC所在的地图
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMapID;	

	// FieldName: 使用限制区域所在地图ID(0不限制)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mLmtMapID;	

	// FieldName: 使用限制区域中心X
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1,100000)
	int		mLmtX;	

	// FieldName: 使用限制区域中心Y
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1,100000)
	int		mLmtY;	

	// FieldName: 使用限制区域宽度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mLmtWide;	

	// FieldName: 使用限制区域高度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mLmtHeight;	


public:
	void SetFromPB( bool bServer, PBItemCall* pItem );
	void CreateTlvPB( bool bServer, PBItemCall* pItem );
	int Initialize();
	CTplItemCall(){	Initialize();}
	~CTplItemCall() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 卡片			## 模版名称
// version	= 1				## 版本号
// path		= 物品/卡片		## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplItemCard : public CTplItem
{
	enum EType
	{
		CARD_YMZ	= 0,	// 妖魔志
		CARD_SBB,			// 神兵榜
		CARD_MCP,			// 名臣谱
		CARD_FSJ,			// 封神记
		CARD_FWL,			// 风物录
	};

public:
	// FieldName: $显示名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mShowText[ STRING32 ];

	// FieldName: 排名
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mOrder;	

	// FieldName: $武器
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mWeapon[ STRING32 ];

	// FieldName: $绝技
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mSkill[ STRING32 ];

	// FieldName: $等级
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mLevel[ STRING32 ];

	// FieldName: 经验
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mExp;	

	// FieldName: 是否可以兑换
	// FieldType: INT4
	// Type:	  COMBOSINGLE(是=0,否=1)
	int		mCanExchange;

	// FieldName: 是否可以挑战
	// FieldType: INT4
	// Type:	  COMBOSINGLE(是=0,否=1)
	int		mCanChallenge;

	// FieldName: 卡片类型
	// FieldType: INT4
	// Type:	  COMBOENUM(EType) 
	int		mType;	

	// FieldName: 位置
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mPos;

	// FieldName: 奖励点
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mBonus;

	// FieldName: 技能ID1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mSkillID1;

	// FieldName: 技能ID2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mSkillID2;

public:
	void SetFromPB( bool bServer, PBItemCard* pItem );
	void CreateTlvPB( bool bServer, PBItemCard* pItem );
	int Initialize();
	CTplItemCard(){ Initialize();}
	~CTplItemCard() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 传送物品		## 模版名称
// version	= 1				## 版本号
// path		= 物品/传送物品	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplItemTeleport : public CTplItem
{
public:
	// FieldName: 转送的类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(随机传送=1,地宫传送=2,记忆传送=3,列表型传送道具=4)
	int		mMapID;		

	// FieldName: 可以使用的次数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mUseTimes;		

public:
	void SetFromPB( bool bServer, PBItemTeleport* pItem );
	void CreateTlvPB( bool bServer, PBItemTeleport* pItem );
	int Initialize();
	CTplItemTeleport() {Initialize();}
	~CTplItemTeleport() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 门派返回卷	## 模版名称
// version	= 1				## 版本号
// path		= 物品/门派返回卷	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplReturnPaper : public CTplItem
{
public:
	// FieldName: 大地图ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mLineID;

	// FieldName: 场景地图ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMapID;

	// FieldName: x坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mPosX;

	// FieldName: y坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mPosY;

public:
	void SetFromPB( bool bServer, PBItemReturnPaper* pItem );
	void CreateTlvPB( bool bServer, PBItemReturnPaper* pItem );
	int Initialize();
	CTplReturnPaper() { Initialize(); }
	~CTplReturnPaper() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 传送符		## 模版名称
// version	= 1				## 版本号
// path		= 物品/传送符	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplTeleSymbol : public CTplItem
{
public:
	// FieldName: 传送表ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mTeleTable;

	// FieldName: 使用次数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mUseTimes;

public:
	void SetFromPB( bool bServer, PBItemTeleSymbol* pItem );
	void CreateTlvPB( bool bServer, PBItemTeleSymbol* pItem );
	int Initialize();
	CTplTeleSymbol() { Initialize(); }
	~CTplTeleSymbol() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 炉石			## 模版名称
// version	= 1				## 版本号
// path		= 物品/炉石	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplStove : public CTplItem
{
public:
	// FieldName: 大地图ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mLineID;

	// FieldName: 场景地图ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMapID;

	// FieldName: x坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mPosX;

	// FieldName: y坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mPosY;

	// FieldName: 冷却时间
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mCoolTime;

	// FieldName: 职业限制
	// FieldType: INT4
	// Type:	  CHECKMUTI(任意=0,战士,剑客,猎人,法师,道士,幻术师,新手)
	int		mMetierRequire;
	
	// FieldName: 等级限制
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,160)
	int		mLevelRequire;

public:
	void SetFromPB( bool bServer, PBItemStove* pItem );
	void CreateTlvPB( bool bServer, PBItemStove* pItem );
	int Initialize();
	CTplStove() { Initialize(); }
	~CTplStove() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 宠物蛋		## 模版名称
// version	= 1				## 版本号
// path		= 物品/宠物蛋	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplPetEgg : public CTplItem
{
public:
	// FieldName: 转送到达的地图ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mPetTempID;				

	// FieldName: 孵化需要的道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mNeedItemID;			

	// FieldName: 还原需要的价格
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mRevertPrice;			

	// FieldName: 初始忠诚度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mInitAdvice;	

	// FieldName: 初始等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int		mInitLevel;

	// FieldName: 初始经验
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int		mInitExp;				

	// FieldName: 初始技能点
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int		mInitSkillPoint;		

	// FieldName: 第1项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第2项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第3项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第4项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第5项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第6项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第7项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第8项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第9项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第10项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第11项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第12项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第13项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第14项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第15项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第16项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第17项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第18项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第19项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第20项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第21项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第22项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第23项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第24项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第25项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第26项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第27项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第28项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第29项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第30项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第31项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第32项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH
	int		mInitSkillID[ 32 ];		

	// FieldName: 第1项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第2项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第3项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第4项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第5项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第6项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第7项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第8项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第9项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第10项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第11项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第12项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第13项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第14项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第15项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第16项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第17项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第18项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第19项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第20项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第21项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第22项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第23项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第24项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第25项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第26项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第27项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第28项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第29项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第30项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第31项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 第32项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)
	int		mInitSkillLevel[ 32 ];	 

public:
	void SetFromPB( bool bServer, PBItemPetEgg* pItem );
	void CreateTlvPB( bool bServer, PBItemPetEgg* pItem );
	int Initialize();
	CTplPetEgg() { Initialize(); }
	~CTplPetEgg() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 宠物食品		## 模版名称
// version	= 1				## 版本号
// path		= 物品/宠物食品	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplPetFood : public CTplItem
{
public:
	enum
	{
		FOOD_FUITE = 1,		// 水果类食品
		FOOD_MEET  = 2,		// 肉类食品
		FOOD_WATER = 3,		// 饮料类食品
	};

public:
	// FieldName: 初始等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)
	int		mInitLevel;	

	// FieldName: 增加的饱食度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)
	int		mFullLevel;				

	// FieldName: 食物所属类别
	// FieldType: INT4
	// Type:	  COMBOSINGLE(水果类食品,肉类食品,饮料类食品)
	int		mTypeMask;	

public:
	void SetFromPB( bool bServer, PBItemPetFood* pItem );
	void CreateTlvPB( bool bServer, PBItemPetFood* pItem );
	int Initialize();
	CTplPetFood() { Initialize(); }
	~CTplPetFood() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 复活卷轴		## 模版名称
// version	= 1				## 版本号
// path		= 物品/复活卷轴	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //
class CTplItemRelife : public CTplItem
{
public:
	// FieldName: 使用时间
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mTimer;				

	// FieldName: 冷却时间（毫秒）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mCooldown;			

	// FieldName: 是否和其他物品共用冷却
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int		mShareCD;	

public:
	void SetFromPB( bool bServer, PBItemRelife* pItem );
	void CreateTlvPB( bool bServer, PBItemRelife* pItem );
	int Initialize();
	CTplItemRelife() { Initialize(); }
	~CTplItemRelife() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= 调用技能的物品		## 模版名称
// version	= 1						## 版本号
// path		= 物品/调用技能的物品	## 路径
// parent	= 道具基本信息			## 父亲模版
// ********************************************************************** //
class CTplItemSkill : public CTplItem
{
public:
	// FieldName: 操作类型
	// FieldType: INT4
	// Type:	  COMBOENUM(SKILLITEMOPRTYPE)
	int		mOprType;

	// FieldName: 使用的等级限制
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,160)
	int		mLevelLimit;		

	// FieldName: 调用的技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH
	int		mSkillID;	

	// FieldName: 冷却时间（毫秒）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mCooldown;				

	// FieldName: 是否和其他物品共用冷却
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int		mShareCD;

	// FieldName: 相关BUFF ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mBuffID;

public:
	void SetFromPB( bool bServer, PBItemSkill* pItem );
	void CreateTlvPB( bool bServer, PBItemSkill* pItem );
	int Initialize();
	CTplItemSkill() { Initialize(); }
	~CTplItemSkill() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 装备升级物品		## 模版名称
// version	= 1					## 版本号
// path		= 物品/装备升级物品	## 路径
// parent	= 道具基本信息		## 父亲模版
// ********************************************************************** //
class CTplItemEquipUpg : public CTplItem
{
public:
	enum enUpType
	{
		UPTYPE_EQUIT = 0, // 精炼装备
		UPTYPE_MW    = 1, // 精炼法宝
	};
	// FieldName: 可以使用的等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mMaxLevel;	

	// FieldName: 类别
	// FieldType: INT4
	// Type:	  CHECKMUTI(装备升级=0,抗性升级,攻击升级)
	int			mType;	

	// FieldName: 级别
	// FieldType: INT4
	// Type:	  CHECKMUTI(低=0,中,高)
	int			mLimit;	

	// FieldName: 是否升级失败
	// FieldType: INT4
	// Type:	  CHECKMUTI(否=0,是=1)
	int			mFail;	 

	// FieldName: 升级成功附加升%d级的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mLuck[ 4 ];

	// FieldName: 精炼目标类别
	// FieldType: INT4
	// Type:	  COMBOSINGLE(装备=0,法宝=1)
	int			mUpType;	 

public:
	void SetFromPB( bool bServer, PBItemUpg* pItem );
	void CreateTlvPB( bool bServer, PBItemUpg* pItem );
	int Initialize();
	CTplItemEquipUpg() { Initialize(); }
	~CTplItemEquipUpg() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= D类物品		## 模版名称
// version	= 1				## 版本号
// path		= 物品/D类物品	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //
class CTplItemBook : public CTplItem
{
public:

	enum OPType
	{
		TYPE_NONE		= 0,		// 无类型
		TYPE_ADDSLOT	= 1,		// 开槽
		TYPE_JUDGE		= 2,		// 鉴定
		TYPE_UNION		= 3,		// 创建军团
		TYPE_JEWEL		= 4,		// 宝石合成
		TYPE_KEY		= 5,		// 开箱子
		TYPE_CARVE		= 6,		// 雕琢符
		TYPE_OPENEXPTOME	=	7, // 开启修炼心德
	};

	// FieldName: 操作类型
	// FieldType: INT4
	// Type:	  COMBOENUM(OPType) 
	int			mOPType;

public:
	void SetFromPB( bool bServer, PBItemBook* pItem );
	void CreateTlvPB( bool bServer, PBItemBook* pItem );
	int Initialize();
	CTplItemBook() { Initialize(); }
	~CTplItemBook() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= 装备升级幸运符		## 模版名称
// version	= 1						## 版本号
// path		= 物品/装备升级幸运符	## 路径
// parent	= 道具基本信息			## 父亲模版
// ********************************************************************** //
class CTplUpgLuck : public CTplItem
{
public:
	// FieldName: 提高精炼成功的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mIncLuck1;	

	// FieldName: 提高精炼失败后只降一级的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mIncLuck2;	

	// FieldName: 提高精炼成功附加升%d级的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mIncLuck3[ 4 ];

	// FieldName: 需求的等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mLevel;	

public:
	void SetFromPB( bool bServer, PBItemUpgLuck* pItem );
	void CreateTlvPB( bool bServer, PBItemUpgLuck* pItem );
	int Initialize();
	CTplUpgLuck() { Initialize(); }
	~CTplUpgLuck() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= 装备升级保底符		## 模版名称
// version	= 1						## 版本号
// path		= 物品/装备升级保底符	## 路径
// parent	= 道具基本信息			## 父亲模版
// ********************************************************************** //
class CTplUpgLeast : public CTplItem
{
public:
	// FieldName: 提高精炼成功的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mIncLuck1;			
	
	// FieldName: 提高精炼失败后只降一级的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mIncLuck2;	

	// FieldName: 使用的类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(装备=0,宝石=1)
	int			mType;

public:
	void SetFromPB( bool bServer, PBItemUpgLeast* pItem );
	void CreateTlvPB( bool bServer, PBItemUpgLeast* pItem );
	int Initialize();
	CTplUpgLeast() 
	{
		Initialize();
	}
	~CTplUpgLeast() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 宝石摘除符	## 模版名称
// version	= 1				## 版本号
// path		= 物品/宝石摘除符	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //
class CTplItemRemove : public CTplItem
{
public:
	// FieldName: 降低到%d档的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mRand[ 9 ];

	// FieldName: 摘除后是否绑定
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)
	int		mBind;

	// FieldName: 档次
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mLevel;

	// FieldName: 是否剥离
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)
	int		mLow;

public:
	void SetFromPB( bool bServer, PBItemRemove* pItem );
	void CreateTlvPB( bool bServer, PBItemRemove* pItem );
	int Initialize();
	CTplItemRemove() 
	{
		Initialize();
	}
	~CTplItemRemove(){}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 抗性转化石	## 模版名称
// version	= 1				## 版本号
// path		= 物品/抗性转化石	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //
class CTplItemChange : public CTplItem
{
public:
	// FieldName: 转化的种类
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1)
	int			mType;

	// FieldName: 转化后的类型
	// FieldType: INT4
	// Type:	  CHECKMUTI(类别1,类别2,类别3,类别4,类别5,类别6)
	int		mRes;

	// FieldName: 职业限制
	// FieldType: INT4
	// Type:	  CHECKMUTI(任意=0,战士,剑客,猎人,法师,道士,幻术师,新手)
	int		mMetierRequire;

public:
	void SetFromPB( bool bServer, PBItemChange* pItem );
	void CreateTlvPB( bool bServer, PBItemChange* pItem );
	int Initialize();
	CTplItemChange() { Initialize(); }
	~CTplItemChange() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= 题卷					## 模版名称
// version	= 1						## 版本号
// path		= 物品/题卷				## 路径
// parent	= 道具基本信息			## 父亲模版
// ********************************************************************** //
class CTplQuestionPaper : public CTplItem
{
public:
	// FieldName: 答题卷出现%d道题时的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mQuestionRate[MAX_QUESTION_NUM];	
		
	// FieldName: 答题卷的经验加成百分比
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mExpAddRae;	
	
	// FieldName: 答题卷每道题目的答题时间限制,秒数，-1 表示没有限制
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1,1000000)
	int			mEachTimeLength;	
	
	// FieldName: 答题卷所有题目的答题时间限制,秒数，-1表示没有限制
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1,100000000)
	int			mTotalTimeLength;	
	
	// FieldName: 答题卷每道题目允许达错的次数，-1表示没有限制
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1,100)
	int			mRepeatTimeAllowed;	

public:
	void SetFromPB( bool bServer, PBItemQuestion* pItem );
	void CreateTlvPB( bool bServer, PBItemQuestion* pItem );
	int Initialize();
	CTplQuestionPaper() 
	{
		Initialize();
	}
	~CTplQuestionPaper() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //



// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 刷任务用道具		## 模版名称
// version	= 1					## 版本号
// path		= 物品/刷任务用道具	## 路径
// parent	= 道具基本信息		## 父亲模版
// ********************************************************************** //
class CTplItemTask : public CTplItem
{
public:
	// FieldName: 可以使用的次数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,2000000000)
	int		mUseTimes;	

	// FieldName: 使用时走条时间(秒,0=不走条)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)
	int		mUsingTime;	

	// FieldName: $走条时客户端显示文字
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char	mShowText[ STRING256 ];
    
	// FieldName: 使用限制区域所在地图ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMapID;	

	// FieldName: 使用限制区域中心X
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1,100000)
	int		mX;	

	// FieldName: 使用限制区域中心Y
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1,100000)
	int		mY;	

	// FieldName: 使用限制区域宽度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mWide;	

	// FieldName: 使用限制区域高度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mHeight;	

	// FieldName: 使用后触发任务
	// FieldType: INT4
	// Type:	  TASKPATH
	int		mGetTaskID;	

	// FieldName: 使用后发放道具
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mGiveItemID;	

	// FieldName: 多少秒后会变成另一物品(要求不可堆叠！)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mChangeTime;	

	// FieldName: 到时间后变成的另一物品
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mChangeItem;	

	// FieldName: 到时间强制让哪个任务失败
	// FieldType: INT4
	// Type:	  TASKPATH
	int		mFailTaskID;	

public:
	void SetFromPB( bool bServer, PBItemTask* pItem );
	void CreateTlvPB( bool bServer, PBItemTask* pItem );
	int Initialize();
	CTplItemTask() 
	{
		Initialize();
	}
	~CTplItemTask();
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 任务道具			## 模版名称
// version	= 1					## 版本号
// path		= 物品/任务道具		## 路径
// parent	= 刷任务用道具		## 父亲模版
// ********************************************************************** //
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 特殊任务物品		## 模版名称
// version	= 1					## 版本号
// path		= 物品/特殊任务物品	## 路径
// parent	= 刷任务用道具		## 父亲模版
// ********************************************************************** //
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 生产原料		## 模版名称
// version	= 1				## 版本号
// path		= 物品/生产原料	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //
class CTplItemStuff : public CTplItem
{
public:
	// FieldName: 显示颜色（白蓝紫金）
	// FieldType: INT4
	// Type:	  COMBOSINGLE(白色原料,蓝色原料,紫色原料,金色原料)
	int		mShowColour;

	// FieldName: 合成的配方ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int mComposeID;

public:
	void SetFromPB( bool bServer, PBItemStuff* pItem );
	void CreateTlvPB( bool bServer, PBItemStuff* pItem );
	int Initialize();
	CTplItemStuff()
	{
		Initialize();
	}
	~CTplItemStuff() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 增加经验道具		## 模版名称
// version	= 1					## 版本号
// path		= 物品/增加经验道具	## 路径
// parent	= 道具基本信息		## 父亲模版
// ********************************************************************** //
class CTplItemExp : public CTplItem
{
public:
	// FieldName: 增加赔率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,2)
	int		mIncRate;		 

	// FieldName: 持续时间（秒）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mTime;	

public:
	void SetFromPB( bool bServer, PBItemExp* pItem );
	void CreateTlvPB( bool bServer, PBItemExp* pItem );
	int Initialize();
	CTplItemExp() 
	{
		Initialize();
	}
	~CTplItemExp(){}
};


// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 世界喇叭		## 模版名称
// version	= 1				## 版本号
// path		= 物品/世界喇叭	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //
class CTplItemShout : public CTplItem
{
public:	
	// FieldName: 表情组ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mFaceGroupID;

public:
	void SetFromPB( bool bServer, PBItemShout* pItem );
	void CreateTlvPB( bool bServer, PBItemShout* pItem );
	int Initialize();
	CTplItemShout()
	{
		Initialize();
	}
	~CTplItemShout() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 自动喊话		## 模版名称
// version	= 1				## 版本号
// path		= 物品/自动喊话	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //
class CTplItemAutoShout : public CTplItem
{
public:	
	// FieldName: 表情组ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mFaceGroupID;

public:
	void SetFromPB( bool bServer, PBItemAutoShout* pItem );
	void CreateTlvPB( bool bServer, PBItemAutoShout* pItem );
	int Initialize();
	CTplItemAutoShout() 
	{
		Initialize();
	}
	~CTplItemAutoShout() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 药品			## 模版名称
// version	= 1				## 版本号
// path		= 物品/药品		## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //
class CTplItemPhysic : public CTplItem
{
public:
	enum EM_PhisicType
	{
		PHISICTYPE_COMMON		= 1,	// 普通药品
		PHISICTYPE_LIFESKILL	= 2,	// 生活技能药品
		PHISICTYPE_AP			= 3,	// 活力药品
		PHISICTYPE_SPECIAL		= 4,	// 特殊药品
	};

	enum EM_PHYSIC_COLOR
	{
		PHYSIC_WHITE	= 0,			// 白色
		PHYSIC_BLUE,					// 蓝色
		PHYSIC_PURPLE,					// 紫色
		PHYSIC_GOLD,					// 金色
		PHYSIC_COLOR,					// 彩色
	};
public:
	// FieldName: 生产类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无类别=0,红1级=100,红2级,红3级,红4级,蓝1级,蓝2级,蓝3级,蓝4级,紫1级,紫2级,紫3级,紫4级,黄1级,黄2级,黄3级,黄4级,白1级,白2级,白3级,白4级,经验药,生活技能药)
	int			mShowType;	

	// FieldName: BUFF编号
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mBuffID;

	// FieldName: 等级要求
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,160)
	int			mLevelRequire;	
	
	// FieldName: HP最大值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int			mHPValue;			

	// FieldName: HP恢复需要时间（毫秒）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mHPTime;

	// FieldName: HP每次恢复间隔（毫秒）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mHPIncTime;

	// FieldName: HP每次恢复值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int			mHPIncValue;

	// FieldName: MP最大值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int			mMPValue;			

	// FieldName: MP恢复需要时间（毫秒）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mMPTime;

	// FieldName: MP每次恢复间隔（毫秒）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mMPIncTime;

	// FieldName: MP每次恢复值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mMPIncValue;

	// FieldName: 冷却时间（毫秒）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int			mCooldown;			

	// FieldName: 是否和其他物品共用冷却
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mShareCD;	
	
	// FieldName: 自动回复的红百分比
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mHPPercent;

	// FieldName: 自动回复的蓝百分比
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mMPPercent;

	// FieldName: 最多回复的红百分比
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mMaxHPPercent;

	// FieldName: 最多回复的蓝百分比
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mMaxMPPercent;
	
	// FieldName: 生活技能类型
	// FieldType: INT4
	// Type:	  COMBOENUM(EM_LIFESKILL)
	int			mLifeSkillType;
	
	// FieldName: 使用后生活技能增长的熟练度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mDegreeValue;
	
	// FieldName: 药品类型
	// FieldType: INT4
	// Type:	  COMBOENUM(EM_PhisicType)
	int		mPhysicType;	

	// FieldName: 所含活力值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mApValue;
	
	// FieldName: 药品颜色
	// FieldType: INT4
	// Type:	  COMBOENUM(EM_PHYSIC_COLOR)
	int		mColor;
public:
	void SetFromPB( bool bServer, PBItemPhysic* pItem );
	void CreateTlvPB( bool bServer, PBItemPhysic* pItem );
	int Initialize();
	CTplItemPhysic() 
	{
		Initialize();
	}
	~CTplItemPhysic() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 坐骑			## 模版名称
// version	= 1				## 版本号
// path		= 物品/坐骑		## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //
class CTplItemHorse : public CTplItem
{
public:
	// FieldName: 职业限制
	// FieldType: INT4
	// Type:	  CHECKMUTI(任意=0,战士,剑客,猎人,法师,道士,幻术师,新手)
	int			mMetierRequire;

	// FieldName: 等级限制
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mLevelRequire;

	// FieldName: 人数限制
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mPersonRequire;

	// FieldName: 使用时间（毫秒）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mUseTime;

	// FieldName: 速度增加百分比（填万分比）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mIncreaseSpeedPer;

	// FieldName: 坐骑对应模型路径
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char		mModelName[ STRING32 ];

	// FieldName: 冷却时间（毫秒）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int			mCooldown;

	// FieldName: 是否和其他物品共用冷却
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mShareCD;

	// FieldName: 对应加速BUFF
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mSpeedBuff;

	// FieldName: 使用时走条时间(秒,0=不走条)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,500000)
	int		mUsingTime;	

	// FieldName: $走条时客户端显示文字
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char	mShowText[ STRING256 ];

	// FieldName: 被攻击后下马概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mOffHorseProb;

	// FieldName: 被攻击下马后上的BUFF
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mOffBuffID;

	// FieldName: 上马消耗的活力值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mInitAP;

	// FieldName: 飞行每秒消耗的活力值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mDeltaAP;

public:
	void SetFromPB( bool bServer, PBItemHorse* pItem );
	void CreateTlvPB( bool bServer, PBItemHorse* pItem );
	int Initialize();
	CTplItemHorse() 
	{
		Initialize();
	}
	~CTplItemHorse() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 背包			## 模版名称
// version	= 1				## 版本号
// path		= 物品/背包		## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //
class CTplKitBag : public CTplItem
{	
public:	
	// FieldName: 表情组ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mFaceGroupID;

	// FieldName: 背包格子数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,48)
	int			mIndexNum;	  
public:	
	void SetFromPB( bool bServer, PBItemKitBag* pItem );
	void CreateTlvPB( bool bServer, PBItemKitBag* pItem );
	int Initialize();
	CTplKitBag() 
	{
		Initialize();
	}
	~CTplKitBag() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 礼包类物品	## 模版名称
// version	= 1				## 版本号
// path		= 物品/礼包类物品	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //
class CTplGiftBag : public CTplItem
{	
public:	
	// FieldName: 使用时需要玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mMinLevel;

	// FieldName: 金钱最小值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mMinMoney;

	// FieldName: 金钱最大值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mMaxMoney;

	// FieldName: 绑定金钱最小值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mMinBindMoney;

	// FieldName: 绑定金钱最大值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mMaxBindMoney;

	// FieldName: 绑定元宝最小值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mMinSilverYB;

	// FieldName: 绑定元宝最大值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mMaxSilverYB;

	// FieldName: 经验最小值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mMinExp;

	// FieldName: 经验最大值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mMaxExp;

	// FieldName: 掉落表
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mDropTableID;

	// FieldName: 掉落物品是否直接进包 
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)
	int			mDropInBag;

	// FieldName: 是否检查包裹空间 
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)
	int			mCheckBag;

	// FieldName: 第%d等级掉落表
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mLevelTableID[TMP_MAX_GRADE];

	// FieldName: 对应的礼花ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mLiHuaID;

	// FieldName: 开启需要的第%d个物品ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mOpenItemID[ 5 ];

	// FieldName: 开启的第%d个掉落表
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mOpenTableID[ 5 ];

public:	
	void SetFromPB( bool bServer, PBItemGiftBag* pItem );
	void CreateTlvPB( bool bServer, PBItemGiftBag* pItem );
	int Initialize();
	CTplGiftBag()
	{
		Initialize();
	}
	~CTplGiftBag() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= 装备附加属性			## 模版名称
// version	= 1						## 版本号
// path		= 装备附加属性			## 路径
// parent	=						## 父亲模版
// ********************************************************************** //
class CTemplateProperty : public CTemplate
{
public:
	enum PROPERTY_FUNC
	{
		PROPERTYINC_PATTACKPER       = 1,		// 增加物攻百分比
		PROPERTYINC_MATTACKPER       = 2,		// 增加法攻百分比
		PROPERTYINC_PDEFENCEPER       = 3,		// 增加物防百分比
		PROPERTYINC_MDEFENCEPER       = 4,		// 增加法防百分比
		PROPERTYINC_PATTACKFIX       = 5,		// 增加物攻固定值
		PROPERTYINC_MATTACKFIX       = 6,		// 增加法攻固定值
		PROPERTYINC_PDEFENCEFIX       = 7,		// 增加物防固定值
		PROPERTYINC_MDEFENCEFIX       = 8,		// 增加法防固定值
		PROPERTYINC_STA       = 9,				// 增加耐力固定值
		PROPERTYINC_WIS       = 10,				// 增加智力固定值
		PROPERTYINC_CON       = 11,				// 增加体质固定值
		PROPERTYINC_SPR       = 12,				// 增加精神固定值
		PROPERTYINC_STR       = 13,				// 增加力量固定值
		PROPERTYINC_MAXHPPER       = 14,		// 增加HP上限百分比
		PROPERTYINC_MAXMPPER       = 15,		// 增加MP上限百分比
		PROPERTYINC_MAXHPFIX       = 16,		// 增加HP上限固定值
		PROPERTYINC_MAXMPFIX       = 17,		// 增加MP上限固定值
		PROPERTYINC_PRESIST       = 18,			// 增加物抗固定值
		PROPERTYINC_MRESIST       = 19,			// 增加法抗固定值
		PROPERTYDEC_PDAMAGEPER       = 20,		// 减免被攻击时物伤百分比
		PROPERTYDEC_MDAMAGEPER       = 21,		// 减免被攻击时法伤百分比
		PROPERTYDEC_PDAMAGEFIX       = 22,		// 减免被攻击时物伤固定值
		PROPERTYDEC_MDAMAGEFIX       = 23,		// 减免被攻击时法伤固定值
		PROPERTYINC_DAMAGEFIX       = 24,		// 附加无视防御伤害固定值
		PROPERTYINC_NORMALHITRATE       = 25,	// 增加普攻命中值
		PROPERTYINC_NORMALDUCK       = 26,		// 增加普攻闪避值
		PROPERTYINC_SKILLHITRATE       = 27,	// 增加技能命中固定值
		PROPERTYINC_SKILLDUCK       = 28,		// 增加残影固定值
		PROPERTYINC_DEATHRATE       = 29,		// 增加致命一击等级固定值
		PROPERTYINC_DEATHVALUE       = 30,		// 增加致命一击伤害加成
		PROPERTYINC_EXPRATE       = 31,			// 增加获得经验时加成
		PROPERTYINC_SPEED       = 32,			// 增加移动速度百分比
		PROPERTYINC_HPRSPEEDFIX       = 33,		// 增加HP恢复速度固定值
		PROPERTYINC_MPRSPEEDFIX       = 34,		// 增加MP恢复速度固定值
		PROPERTYINC_HPRSPEEDPER       = 35,		// 增加HP恢复速度百分比
		PROPERTYINC_MPRSPEEDPER       = 36,		// 增加MP恢复速度百分比
		PROPERTYFUNC_HPABSORB       = 37,		// 生命吸取
		PROPERTYFUNC_MPABSORB       = 38,		// 法力吸取
		PROPERTYFUNC_PATTACKDAMAGE       = 39,	// 增加物理伤害百分比
		PROPERTYFUNC_MATTACKDAMAGE       = 40,	// 增加法术伤害百分比
		PROPERTYFUNC_REFLECT       = 41,		// 100%反弹伤害百分比		
		PROPERTYFUNC_RESDEF1       = 42,		// 抗性1
		PROPERTYFUNC_RESDEF2       = 43,		// 抗性2
		PROPERTYFUNC_RESDEF3       = 44,		// 抗性3
		PROPERTYFUNC_RESDEF4       = 45,		// 抗性4
		PROPERTYFUNC_RESDEF5       = 46,		// 抗性5
		PROPERTYFUNC_RESDEF6       = 47,		// 抗性6
		PROPERTYFUNC_RESATT1       = 48,		// 属性攻击1
		PROPERTYFUNC_RESATT2       = 49,		// 属性攻击2
		PROPERTYFUNC_RESATT3       = 50,		// 属性攻击3
		PROPERTYFUNC_RESATT4       = 51,		// 属性攻击4
		PROPERTYFUNC_RESATT5       = 52,		// 属性攻击5
		PROPERTYFUNC_RESATT6       = 53,		// 属性攻击6
		PROPERTYFUNC_IGNOREDUCK       = 54,		// 忽视闪避
		PROPERTYFUNC_DEATHDEFENSE       = 55,	// 致命防御
		PROPERTYFUNC_OBTAINSKILL       = 56,	// 获得某个技能
		PROPERTYINC_SKILLLEVEL       = 57,		// 增加技能等级（某个）
		PROPERTYINC_SKILLGENRELEVEL       = 58,	// 增加技能等级（某系）			
		PROPERTYINC_ALL       = 59,				// 增加全属性
		PROPERTYFUNC_REDUCECOOLFIX       = 60,	// 减少某技能冷却时间固定值	
		PROPERTYFUNC_DEATHRATEFIX       = 61,	// 增加某个技能致命等级
		PROPERTYFUNC_DEATHVALUEFIX       = 62,	// 增加某个技能暴击伤害百分比
		PROPERTYFUNC_REDUCELEVEL       = 63,	// 降低装备等级需求
		PROPERTYFUNC_HITSKILL       = 64,		// 击中时使用的技能
		PROPERTYFUNC_BEHITSKILL       = 65,		// 被击中时使用的技能
		PROPERTYFUNC_DEATHSKILL       = 66,		// 暴击时使用的技能
		PROPERTYFUNC_HPSKILL       = 67,		// 减血时使用的技能
		PROPERTYINC_WIZARD1       = 68,			// 才华等级1
		PROPERTYINC_WIZARD2       = 69,			// 才华等级2		
		PROPERTYINC_WIZARD3       = 70,			// 才华等级3
		PROPERTYINC_WIZARD4       = 71,			// 才华等级4
		PROPERTYINC_WIZARD5       = 72,			// 才华等级5
		PROPERTYINC_WIZARD6       = 73,			// 才华等级6
		PROPERTYFUNC_RESDEFALL    = 74,			// 全抗性
		PROPERTYFUNC_RESATTALL    = 75,			// 全属性
	};

public:
	
	// FieldName: $附加属性名称	
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char	mPropertyName[ STRING256 ];	

	// FieldName: 属性公式
	// FieldType: INT4
	// Type:	  COMBOENUM(PROPERTY_FUNC)
	int		mPropertyFunc;				

	// FieldName: 使用的目标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1)
	int		mCanOut;					

	// FieldName: 固定值属性数值下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mIntMinValue;					

	// FieldName: 固定值属性数值上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mIntMaxValue;		

	// FieldName: 所在区
	// FieldType: INT4
	// Type:	  COMBOSINGLE(一区=1,二区)
	int		mArea;

	// FieldName: 百分比属性数值下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mFloatMinValue;				

	// FieldName: 百分比属性数值上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mFloatMaxValue;				

	// FieldName: 相关技能ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mSkillID;

public:
	void SetFromPB( bool bServer, PBTemplateProperty* pProperty );
	void CreateTlvPB( bool bServer, PBTemplateProperty* pProperty );
	int Initialize();
	CTemplateProperty()
	{
		Initialize();
	}
	~CTemplateProperty() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 附加属性表	## 模版名称
// version	= 1				## 版本号
// path		= 附加属性表	## 路径
// parent	=				## 父亲模版
// ********************************************************************** //
class CTemplatePropertyTable : public CTemplate
{
public:
	class CPropertyPercent
	{
	public:
		int		mTempID;		// 附加属性模版ID
		int		mPercent;		// 附加属性得到百分比
	};
public:
	
	// FieldName: 第1项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第1项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第2项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第2项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第3项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第3项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第4项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第4项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第5项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第5项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第6项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第6项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第7项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第7项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第8项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第8项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第9项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第9项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第10项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第10项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第11项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第11项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第12项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第12项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第13项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第13项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第14项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第14项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第15项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第15项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第16项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第16项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第17项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第17项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第18项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第18项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第19项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第19项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第20项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第20项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第21项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第21项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第22项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第22项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第23项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第23项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第24项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第24项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第25项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第25项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第26项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第26项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第27项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第27项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第28项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第28项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第29项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第29项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第30项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第30项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第31项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第31项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第32项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第32项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第33项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第33项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第34项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第34项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第35项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第35项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第36项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第36项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第37项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第37项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第38项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第38项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第39项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第39项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第40项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第40项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第41项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第41项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第42项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第42项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第43项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第43项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第44项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第44项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第45项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第45项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第46项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第46项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第47项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第47项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第48项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第48项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第49项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第49项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第50项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第50项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第51项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第51项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第52项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第52项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第53项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第53项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第54项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第54项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第55项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第55项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第56项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第56项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第57项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第57项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第58项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第58项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第59项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第59项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第60项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第60项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第61项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第61项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第62项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第62项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第63项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第63项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第64项附加属性的模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第64项附加属性的得到概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	CPropertyPercent	mProperty[ 64 ];

public:
	void SetFromPB( bool bServer, PBTemplatePropertyTable* pTable );
	void CreateTlvPB( bool bServer, PBTemplatePropertyTable* pTable );
	int Initialize();
	CTemplatePropertyTable()
	{
		Initialize();
	}
	~CTemplatePropertyTable() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= true			## 代表生成到数据库中
// name		= 装备基础属性	## 模版名称
// version	= 1				## 版本号
// path		=				## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //
class CTemplateEquipment : public CTplItem
{
public:
	enum
	{
		PART_RING		= 1,				// 戒指类装备
		PART_CUFF,							// 护腕类装备
		PART_WEAPON,						// 武器类装备
		PART_CHEST,							// 护胸类装备
		PART_LEG,							// 护腿类装备
		PART_HEAD,							// 护头类装备
		PART_NECK,							// 项链类装备
		PART_SASH,							// 腰饰类装备
		PART_CHARM,							// 护符类装备
		PART_BALL,							// 法球类装备
		PART_CLOAK,							// 披风类装备
		PART_FASHION,						// 时装类装备
		PART_MAGICWEAPON,					// 法宝类装备
		PART_WIZARD,						// 才华类装备
		PART_MARRY,							// 结婚类装备
	};

	enum
	{
		EQUIPMENT_WHITE	= 1,				// 白色装备
		EQUIPMENT_BLUE,						// 蓝色装备
		EQUIPMENT_PURPLE,					// 紫色装备
		EQUIPMENT_GOLD,						// 金色装备
	};

	enum
	{
		EXTRA_NONE	=	0,					// 无附加
		EXTRA_FIRST	=	1,					// 1类附加
		EXTRA_SECOND =	2,					// 2类附加
	};

public:
	// FieldName: 装备模型名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mEquipModal[ STRING32 ];	

	// FieldName: 装备等级（０－１５）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,15)
	int		mShowLevel;			

	// FieldName: 装备部位
	// FieldType: INT4
	// Type:	  COMBOSINGLE(戒指类装备,护腕类装备,武器类装备,护胸类装备,护腿类装备,护头类装备,项链类装备,腰饰类装备,护符类装备,法球类装备,披风类装备,时装类装备,法宝装备,才华类装备,结婚类装备)
	int		mEquipType;					

	// FieldName: 显示颜色（白蓝紫金）
	// FieldType: INT4
	// Type:	  COMBOSINGLE(白色装备,蓝色装备,紫色装备,金色装备)
	int		mShowColour;		

	// FieldName: 力量条件%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mStrRequire[ 6 ];			

	// FieldName: 体质条件%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mConRequire[ 6 ];

	// FieldName: 耐力限制%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mStaRequire[ 6 ];				

	// FieldName: 智力限制%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mWisRequire[ 6 ];

	// FieldName: 精神限制%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mSprRequire[ 6 ];

	// FieldName: 职业限制
	// FieldType: INT4
	// Type:	  CHECKMUTI(任意=0,战士,剑客,猎人,法师,道士,幻术师,新手)
	int		mMetierRequire;

	// FieldName: 等级限制
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mLevelRequire;				

	// FieldName: 性别限制
	// FieldType: INT4
	// Type:	  COMBOSINGLE(任意,男性,女性)
	int		mSexRequire;	

	// FieldName: 转化的元气值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mYuan;

	// FieldName: 装备耐久度(-1表示永远不会损坏)
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1,1000)
	int    mEquipmentDurability;

	// FieldName: 基础修理金额
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int    mBaseMendCharge;

	// FieldName: 与npc交互一次的损伤万分比
	// FieldType: INT4
	// Type:	 EDITNUMBER(0,10000)
	int   mNpcAbrasionRate;

	// FieldName: 玩家交互1次的损伤万分比
	// FieldType: INT4
	// Type:     EDITNUMBER(0,10000)	
	int mPlayerAbrasionRate;

	// FieldName: 是否可以升级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1)
	int    mCanUpgrade;

	// FieldName: 是否可以抗性升级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1)
	int    mCanDefUpgrade;

	// FieldName: 是否可以属性升级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1)
	int    mCanAttUpgrade;

	// FieldName: 升级需要的手续费
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mUpgCharge;	

	// FieldName: 费用是否支持绑定金钱
	// FieldType: INT4
	// Type:	  COMBOSINGLE(是=0,否=1)
	int		mNeedBindMoney;

	// FieldName: 升级增加的第%d项属性ID
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无=0,增加物攻固定值=5,增加法攻固定值=6,增加物防固定值=7,增加法防固定值=8,增加耐力固定值=9,增加智力固定值=10,增加敏捷固定值=11,增加精神固定值=12,增加力量固定值=13,增加命中固定值=25,增加闪避固定值=26,增加忽闪固定值=54)
	int		mUpgProperty[ 3 ];	

	// FieldName: 第%d项属性值在升%d级增加到
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int		mUpgValue[ 3 ][ 16 ];	

	// FieldName: 升%d级增加的物理攻击附加
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int		mPAttackDiff[ 16 ];

	// FieldName: 升%d级增加的法术攻击附加
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int		mMAttackDiff[ 16 ];

	// FieldName: 初始属性槽个数为%d的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mSlotNumber1[ 5 ];	

	// FieldName: 最大可加属性槽个数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int		mSlotNumber2;	

	// FieldName: 生成时一区附加属性数为%d的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)	
	int		mPropertyPercent[ 5 ];		

	// FieldName: 一区第%d个附加属性表模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mPropertyTableID[ 3 ];

	// FieldName: 生成时二区附加属性数为%d的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)	
	int		mPropertyPercentEx[ 6 ];		

	// FieldName: 二区第%d个附加属性表模版ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mPropertyTableIDEx[ 5 ];

	// FieldName: 第一次鉴定后属性为%d的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)	
	int		mJudgePercent1[ 25 ];

	// FieldName: 第二次鉴定后属性为%d的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)	
	int		mJudgePercent2[ 25 ];

	// FieldName: 生成时抗性为%d的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)	
	int		mResDefPercent[ 7 ];

	// FieldName: 生成时属性攻击为%d的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)	
	int		mResAttPercent[ 7 ];

	// FieldName: 升%d级增加到的抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int		mResDefValue[ 7 ];

	// FieldName: 升%d级增加到的属性攻击
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int		mResAttValue[ 7 ];

	// FieldName: 套装ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,200)
	int    mSuitID;

	// FieldName: 套装编号
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,200)
	int    mSuitPos;

	// FieldName: 开槽费用
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mOpenCharge;

	// FieldName: 加槽费用
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mAddCharge;

	// FieldName: 鉴定费用
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mJudgeCharge;

	// FieldName: 再次鉴定费用
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mReJudgeCharge;

	// FieldName: 抗性强化费用
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mDefUpgCharge;

	// FieldName: 抗性转化费用
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mDefChangeCharge;

	// FieldName: 属性强化费用
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mAttUpgCharge;

	// FieldName: 属性转化费用
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mAttChangeCharge;	

	// FieldName: 滴血增加比例
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mBindBonus;

	// FieldName: 复杂合成配方
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mMultiCompose;

	// FieldName: 是否可以转换属性
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)
	int		mCanChange;

	// FieldName: 属性转换费用
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mChangeCharge;

	// FieldName: 是否可以拆分
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)
	int		mCanSplit;

	// FieldName: 第%d种拆分的ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int		mSplitID[ 3 ];

	// FieldName: 第%d种拆分的数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)	
	int		mSplitNum[ 3 ];

	// FieldName: 拆分费用
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mSplitCharge;

	// FieldName: 第%d项属性的铭刻值1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mUpgExtra1[ 3 ];

	// FieldName: 第%d项属性的铭刻值2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mUpgExtra2[ 3 ];

	// FieldName: 铭刻费用
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mExtCharge;

	// FieldName: 是否能抵抗变身
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int		mResistTran;

	// FieldName: 是否是神器
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int		mIsShenQi;

public:
	void SetFromPB( bool bServer, PBEquipment* pItem );
	void CreateTlvPB( bool bServer, PBEquipment* pItem );
	int Initialize();
	CTemplateEquipment() { Initialize(); }
	~CTemplateEquipment() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 时装			## 模版名称
// version	= 1				## 版本号
// path		= 装备/时装		## 路径
// parent	= 装备基础属性	## 父亲模版
// ********************************************************************** //
class CTemplateFashion : public CTemplateEquipment
{
public:
	// FieldName: 附带的BuffID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mBuffID;

public:
	void SetFromPB( bool bServer, PBEquipFashion* pEquip );
	void CreateTlvPB( bool bServer, PBEquipFashion* pEquip );
	int Initialize();
	CTemplateFashion()
	{
		Initialize();
	}
	~CTemplateFashion() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 武器			## 模版名称
// version	= 1				## 版本号
// path		= 装备/武器		## 路径
// parent	= 装备基础属性	## 父亲模版
// ********************************************************************** //

enum ATTACK_TYPE
{
	ATTACKTYPE_NONE		= 0,		// 无攻击类型
	ATTACKTYPE_PHYSICS	= 1,		// 物理攻击类型
	ATTACKTYPE_MAGIC	= 2,				// 法术攻击类型
	ATTACKTYPE_NORMAL	= 3,				// 通用攻击类型
};
class CTemplateWeapon : public CTemplateEquipment
{
public:
	enum WEAPON_TYPE
	{
		WEAPON_UNLIMIT	= 0,			// 无限制
		WEAPON_SPEAR	= 1,					// 枪
		WEAPON_BLADE	= 2,					// 双刀
		WEAPON_BOW		= 3,						// 弓
		WEAPON_SHIELD	= 4,					// 法盾
		WEAPON_STAFF	= 5,					// 法杖
		WEAPON_TOMAHAWK	= 6,				// 钺
		WEAPON_SWORD	= 7,					// 剑
		WEAPON_WRIST	= 8,					// 护腕
	};


public:
	// FieldName: 武器种类
	// FieldType: INT4
	// Type:	  COMBOSINGLE(枪,双刀,弓,法盾,法杖,钺,剑,护腕)
	WEAPON_TYPE		mWeaponType;

	// FieldName: 攻击类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(物理攻击,法术攻击)
	int		mAttackType;

	// FieldName: 攻击距离
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)
	int		mAttackDistance;

public:
	void SetFromPB( bool bServer, PBEquipWeapon* pEquip );
	void CreateTlvPB( bool bServer, PBEquipWeapon* pEquip );
	int Initialize();
	CTemplateWeapon() { Initialize(); }
	~CTemplateWeapon() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 防具			## 模版名称
// version	= 1				## 版本号
// path		= 装备/防具		## 路径
// parent	= 装备基础属性	## 父亲模版
// ********************************************************************** //
class CTemplateArmor : public CTemplateEquipment
{
public:
	enum
	{
		LDEFENSE	= 1,		// 重甲防御
		LLIFE		= 2,					// 重甲生命
		MDEFENSE	= 3,				// 轻甲防御
		MLIFE		= 4,					// 轻甲生命
		SDEFENSE	= 5,				// 袍子防御
		SLIFE		= 6,					// 袍子生命
	};
public:
	// FieldName: 防具类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无=0,重甲,轻甲,布甲)
	int		mShowType;

	// FieldName: 防具套装资源
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mArmorSuit[ STRING32 ];

public:
	void SetFromPB( bool bServer, PBEquipArmor* pEquip );
	void CreateTlvPB( bool bServer, PBEquipArmor* pEquip );
	int Initialize();
	CTemplateArmor() 
	{
		Initialize();
	}
	~CTemplateArmor() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 法宝基础属性	## 模版名称
// version	= 1				## 版本号
// path		= 法宝基础属性	## 路径
// parent	= 装备基础属性	## 父亲模版
// ********************************************************************** //
class CTemplateMagicWeapon : public CTemplateEquipment
{
public:

	// FieldName: $法宝显示品级
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mMWShowLevel[ STRING32 ];

	// FieldName: $法宝显示类别
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mShowType[ STRING32 ];

	// FieldName: 装备法宝后更换玩家武器资源名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char		mNewWeaponName[ STRING32 ];

	// FieldName: 法宝初始等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int				mInitLevel;

	// FieldName: 法宝初始法力值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int				mInitMp;

	// FieldName: 法宝初始灵气值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int				mInitHp;

	// FieldName: 法宝初始法力值上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int				mInitMaxMp;

	// FieldName: 法宝初始灵气值上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int				mInitMaxHp;

	// FieldName: 法宝属性基础值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int				mInitBase;

	// FieldName: 法宝属性成长率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int				mInitShape;

	// FieldName: 法宝属性影响度(万分比)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int				mInitEffect;

	// FieldName: 法宝初始灵性(万分比)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int				mInitSpir;

	// FieldName: 每点定力增加法宝法力值上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int				mIncMaxMp;

	// FieldName: 每点定力增加法宝灵气值上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int				mIncMaxHp;

	// FieldName: 法力值每次增加值(负数表示减少)
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,10000)
	int			mMPIncValue;

	// FieldName: 灵气值每次增加值(负数表示减少)
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,10000)
	int			mHPIncValue;

	// FieldName: 每10000点法力影响法力值每次增加值(负数表示减少)
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,10000)
	int			mMPIncValuePerMagic;

	// FieldName: 每10000点灵力影响灵气值每次增加值(负数表示减少)
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,10000)
	int			mHPIncValuePerMagic;

	// FieldName: 每10000点锐力影响法宝攻击力增加值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,500000000)
	int			mAttackInc;

	// FieldName: 每10000点定力影响法宝防御力增加值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,500000000)
	int			mRecovInc;

	// FieldName: 每10000点法力影响法宝提升玩家MP上限值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,500000000)
	int			mMaxMPInc;

	// FieldName: 每10000点灵力影响法宝提升玩家HP上限值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,500000000)
	int			mMaxHPInc;

	// FieldName: 每10000点仙力影响法宝提升玩家致命等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,500000000)
	int			mDeadLineInc;
	
	// FieldName: 能否使用技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)
	int			mCanUseBook;

	// FieldName: 升级时自行领悟新技能概率(万分比)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mGetSkillScl;

	// FieldName: 升级属性表
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mLevelTableID;

	// FieldName: 技能框属性表
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mSkillBoxTableID;

	// FieldName: 玩家死亡减少的灵气值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mDieOffHp;

	// FieldName: 属性鉴定收费
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mProJudgeCost;

	// FieldName: 法宝佩戴免疫BUFF表
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mUmmiBuffTableID;

	// FieldName: 位移模块资源名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char		mDispResName[ STRING32 ];

	// FieldName: 默认技能1(自动覆盖第0个技能框)
	// FieldType: INT4
	// Type:	  SKILLPATH
	int			mDefaultSkill;

	// FieldName: 默认技能2(自动覆盖第1个技能框)
	// FieldType: INT4
	// Type:	  SKILLPATH
	int			mDefaultSkill2;

	// FieldName: 法宝对应的影响度鉴定表
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mJudgeEffTableID;

	// FieldName: 法宝对应的灵性鉴定表
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mJudgeSpirTableID;

	// FieldName: 法宝对应的精炼等级表
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mLevelQuotietyTabID;	

	// FieldName: 法宝分解表
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mDecomposeTabID;	

	// FieldName: 法宝品级（数字）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int			mLvByNum;

	// FieldName: 法宝飞升表
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mQuickUpTabID;	

	// FieldName: 法宝升品表
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mUpLevelTabID;	

public:
	void SetFromPB( bool bServer, PBItemMagicWeapon* pItem );
	void CreateTlvPB( bool bServer, PBItemMagicWeapon* pItem );
	int Initialize();
	CTemplateMagicWeapon()
	{
		Initialize();
	}
	~CTemplateMagicWeapon() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

class CMagicWeaponProIncValue
{
public:
	unsigned long long	mLevelExp;	
	unsigned int	mLevelPoints;
	unsigned int	mLevelProcessVal;
	unsigned int	mLevelProcessCurel;
	unsigned int	mLevelProcessCurelVal;
	unsigned int	mLevelKeen;		
	unsigned int	mLevelCalm;		
	unsigned int	mLevelMagic;	
	unsigned int	mLevelBier;		
	unsigned int	mLevelImmortal;
};

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 法宝属性点增长表			 ## 模版名称
// version	= 1						 ## 版本号
// path		= Config/法宝属性点增长表	 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //
class CTplMagicWeaponProIncTable : public CTemplate
{
public:

	// FieldName: 第%d级升级到下一级的经验数值
	// FieldType: INT8
	// Type:	  EDITNUMBER(0,72057594037927935)

	// FieldName: 升级到第%d级增加的技能点数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)

	// FieldName: 升级到第%d级获得的技能框进度值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)

	// FieldName: 升级到第%d级获得的技能框进度值暴击概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)

	// FieldName: 升级到第%d级获得的技能框进度值暴击数值(还要加应获得的进度值)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)

	// FieldName: 升级到第%d级的锐力增长值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,3000000000)

	// FieldName: 升级到第%d级的定力增长值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,3000000000)

	// FieldName: 升级到第%d级的法力增长值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,3000000000)

	// FieldName: 升级到第%d级的灵力增长值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,3000000000)

	// FieldName: 升级到第%d级的仙力增长值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,3000000000)

	CMagicWeaponProIncValue mValues[ MAGICWEAPON_MAX_GRADE ];
public:
	void SetFromPB( bool bServer, PBTplMagicWeaponProIncTable* pItem );
	void CreateTlvPB( bool bServer, PBTplMagicWeaponProIncTable* pItem );
	int Initialize();
	CTplMagicWeaponProIncTable()
	{
		Initialize();
	}
	~CTplMagicWeaponProIncTable() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// 法宝分解表
class CSingleMWDecomposeData
{
public:
	int			mTempID; // 模板ID
	int			mNumMin; // 最少数量
	int			mNumMax; // 最大数量
	int			mProb;   // 获得几率(万分比)
};

enum  MW_DECOMPOS_MONEY_TYPE
{
	MW_DECOMPOS_MONEY_TYPE_NONE = 0,  // 不消耗
	MW_DECOMPOS_MONEY_TYPE_BINDMONEY , // 消耗绑定钱
	MW_DECOMPOS_MONEY_TYPE_NOTBINDMONEY , // 消耗非绑定钱
};

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 法宝分解表			 ## 模版名称
// version	= 1						 ## 版本号
// path		= Config/法宝分解表	 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //
class CMagicWeaponDecomposeTable : public CTemplate
{
public:

	// FieldName: 分解消耗的金钱类型
	// FieldType: INT4
	// Type:	  COMBOENUM(MW_DECOMPOS_MONEY_TYPE)
	int			mNeedMoneyType;	

	// FieldName: 分解消耗的金钱/元宝数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int			mNeedMoney;	

	// FieldName: 第%d个分解物品的模板
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第%d个分解物品的最小数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)

	// FieldName: 第%d个分解物品的最大数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)

	// FieldName: 第%d个分解物品的获得几率(万分比)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	CSingleMWDecomposeData  mItems[5];

public:
	void SetFromPB( bool bServer, PBMagicWeaponDecomposeTable* pItem );
	void CreateTlvPB( bool bServer, PBMagicWeaponDecomposeTable* pItem );
	int Initialize();
	CMagicWeaponDecomposeTable()
	{
		Initialize();
	}
	~CMagicWeaponDecomposeTable() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 法宝飞升、升品表			 ## 模版名称
// version	= 1						 ## 版本号
// path		= Config/法宝飞升、升品表	 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //
class CMagicWeaponQuickUpTable : public CTemplate
{
public:

	// FieldName: 可以飞升的最低等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mNeedLevel;	

	// FieldName: 飞升需要的物品ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mNeedItem;	

	// FieldName: 飞升需要的物品数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int			mNeedItemNum;	

	// FieldName: 飞升消耗的金钱类型
	// FieldType: INT4
	// Type:	  COMBOENUM(MW_DECOMPOS_MONEY_TYPE)
	int			mNeedMoneyType;	

	// FieldName: 飞升消耗的金钱数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int			mNeedMoney;	

	// FieldName: 飞升成功概率(万分比)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mSuccessProb;	

	// FieldName: 飞升成功后替换的法宝ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mNewMWID;	

	// FieldName: 升品需要的副法宝最低等级差
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mLeastLeve;	


public:
	void SetFromPB( bool bServer, PBMagicWeaponQuickUpTable* pItem );
	void CreateTlvPB( bool bServer, PBMagicWeaponQuickUpTable* pItem );
	int Initialize();
	CMagicWeaponQuickUpTable()
	{
		Initialize();
	}
	~CMagicWeaponQuickUpTable() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


class CTplSkillStorageBase
{
public:
	int mSkillID;
	int mFirstProb;
	int mOtherProb;
};

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 法宝技能库表			 ## 模版名称
// version	= 1						 ## 版本号
// path		= Config/法宝技能库表	 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //
class CTplSkillStorage : public CTemplate
{
public:


	// FieldName: 第%d个技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第%d个技能第一次鉴定概率(万分比)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第%d个技能以后鉴定概率(万分比)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	CTplSkillStorageBase		  mValues[SKILL_STORAGE_LMT];	

public:
	void SetFromPB( bool bServer, PBSkillStorage* pTable );
	void CreateTlvPB( bool bServer, PBSkillStorage* pTable );
	int Initialize();
	CTplSkillStorage() 
	{
		Initialize();
	}
	~CTplSkillStorage() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


class CSkillBoxProperty
{
public:
	int			mSkillBoxColor;
	int			mSkillBoxNeedVal;
	int			mInitState;
	int			mInitSkillID;
	int			mSkillBoxType;
	int			mSkillBoxSkill;
	int			mSkillBoxSkillStorage;
	int			mSkillBoxActMoney;
};

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 法宝技能框属性表			 ## 模版名称
// version	= 1						 ## 版本号
// path		= Config/法宝技能框属性表	 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //
class CTplMagicWeaponSkillBoxProTable : public CTemplate
{
public:
	enum MagicWeapon_COLOR
	{
		COLOR_NONE    = 0,  // 无
		COLOR_WHITE   = 1,  // 白色
		COLOR_BLUE    = 2,  // 蓝色
		COLOR_PURPLE  = 3,  // 紫色
		COLOR_GOLD    = 4,  // 金色
	};

	enum MagicWeapon_SKILLBOX_TYPE
	{
		TYPE_STATIC    = 0,		// 固定技能
		TYPE_STORAGE   ,		// 从技能库中选
	};

	// FieldName: 第%d个技能框颜色
	// FieldType: INT4
	// Type:	  COMBOENUM(MagicWeapon_COLOR)

	// FieldName: 激活第%d个技能框需要的进度值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第%d个技能框的初始状态
	// FieldType: INT4
	// Type:	  COMBOSINGLE(未激活=0,已激活=1)

	// FieldName: 第%d个技能框的初始技能(可以留空)
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第%d个技能框的激活方式
	// FieldType: INT4
	// Type:	  COMBOENUM(MagicWeapon_SKILLBOX_TYPE)

	// FieldName: 第%d个技能框对应的技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第%d个技能框对应的技能库
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第%d个技能框的鉴定收费
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	CSkillBoxProperty	mValues[SKILL_BOX_LMT];
public:
	void SetFromPB( bool bServer, PBMagicWeaponSkillBoxProTable* pTable );
	void CreateTlvPB( bool bServer, PBMagicWeaponSkillBoxProTable* pTable );
	int Initialize();
	CTplMagicWeaponSkillBoxProTable()
	{
		Initialize();
	}
	~CTplMagicWeaponSkillBoxProTable() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false							 ## 代表生成到数据库中
// name		= 法宝属性鉴定影响度表			 ## 模版名称
// version	= 1								 ## 版本号
// path		= Config/法宝属性鉴定影响度表	 ## 路径
// parent	=								 ## 父亲模版
// ********************************************************************** //

enum MW_JUDGE_LEVEL
{
	MW_COMMON = 1, // 普通
	MW_GOOD,// 优秀
	MW_POLISH,// 精良
	MW_BEST,// 卓越
};

class CTplMWProJudgeEffTable : public CTemplate
{
public:

	// FieldName: 百分之%d影响度第一次鉴定概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mProbFirst[MAX_MW_PERCENT_NUM];

	// FieldName: 百分之%d影响度第二次鉴定概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mProbSecond[MAX_MW_PERCENT_NUM];

	// FieldName: $百分之%d影响度的显示名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mShowName[MAX_MW_PERCENT_NUM][ STRING32 ];

	// FieldName: 百分之%d影响度的级别
	// FieldType: INT4
	// Type:	  COMBOSINGLE(普通=1,优秀,精良,卓越)
	int		mJudgeLevel[MAX_MW_PERCENT_NUM];
	
public:
	void SetFromPB( bool bServer, PBMWProJudgeEffTable* pTable );
	void CreateTlvPB( bool bServer, PBMWProJudgeEffTable* pTable );
	int Initialize();
	CTplMWProJudgeEffTable()
	{
		Initialize();
	}
	~CTplMWProJudgeEffTable()  {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false							 ## 代表生成到数据库中
// name		= 法宝属性鉴定灵性表			 ## 模版名称
// version	= 1								 ## 版本号
// path		= Config/法宝属性鉴定灵性表  	 ## 路径
// parent	=								 ## 父亲模版
// ********************************************************************** //
class CTplMWProJudgeSpirTable : public CTemplate
{
public:

	// FieldName: 百分之%d灵性第一次鉴定概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mProbFirst[MAX_MW_PERCENT_NUM];

	// FieldName: 百分之%d灵性第二次鉴定概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mProbSecond[MAX_MW_PERCENT_NUM];

	// FieldName: $百分之%d灵性的显示名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mShowName[MAX_MW_PERCENT_NUM][ STRING32 ];		

	// FieldName: 百分之%d影响度的级别
	// FieldType: INT4
	// Type:	  COMBOENUM(MW_JUDGE_LEVEL)
	int		mJudgeLevel[MAX_MW_PERCENT_NUM];

public:
	void SetFromPB( bool bServer, PBMWProJudgeSpirTable* pTable );
	void CreateTlvPB( bool bServer, PBMWProJudgeSpirTable* pTable );
	int Initialize();
	CTplMWProJudgeSpirTable() 
	{
		Initialize();
	}
	~CTplMWProJudgeSpirTable(){}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false							 ## 代表生成到数据库中
// name		= 法宝精炼系数表				 ## 模版名称
// version	= 1								 ## 版本号
// path		= Config/法宝精炼系数表		  	 ## 路径
// parent	=								 ## 父亲模版
// ********************************************************************** //
class CTplMWLevelQuotietyTab : public CTemplate
{
public:

	// FieldName: 锐力精炼等级%d对应的精炼系数(万分比)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mKeenQuotiety[20];

	// FieldName: 定力精炼等级%d对应的精炼系数(万分比)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mCalmQuotiety[20];

	// FieldName: 法力精炼等级%d对应的精炼系数(万分比)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mMagicQuotiety[20];

	// FieldName: 灵力精炼等级%d对应的精炼系数(万分比)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mBierQuotiety[20];

	// FieldName: 仙力精炼等级%d对应的精炼系数(万分比)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mImmorQuotiety[20];

public:
	void SetFromPB( bool bServer, PBMWLevelQuotietyTab* pTable );
	void CreateTlvPB( bool bServer, PBMWLevelQuotietyTab* pTable );
	int Initialize();
	CTplMWLevelQuotietyTab() 
	{
		Initialize();
	}
	~CTplMWLevelQuotietyTab() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表不生成到数据库中
// name		= 职业属性			## 模版名称
// version	= 1					## 版本号
// path		= Config/职业属性	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
    enum METIER_GENERID
    {
		GENDERID_NONE         = 0,			    //无 
		WARRIOR_PRED                  = 1,	  //战士前导
		WARRIOR_WEAPIN                = 2,	  //战士武器
		WARRIOR_RECOVER               = 3,	  //战士防御
		WARRIOR_FRENZY       = 4,				//战士狂暴
		SWORDMAN_PRED                 = 5,	  //剑客前导
		SWORDMAN_PUTOUT               = 6,	  //剑客刺杀
		SWORDMAN_COLDBLOODNESS        = 7,	  //剑客冷血
		SWORDMAN_RISK       = 8,				//剑客舍命
		HUNTER_PRED                   = 9,	  //猎人前导
		HUNTER_FIRE                   = 10,	  //猎人射击
		HUNTER_HOOK                   = 11,	  //猎人陷阱
		HUNTER_MAGGIC                 = 12,	  //猎人法术
		RABBI_PRED                    = 13,	  //法师前导
		RABBI_ICE                     = 14,	  //法师玄冰
		RABBI_FIRE                    = 15,	  //法师炽炎
		RABBI_WIND                    = 16,	  //法师怒风
		TAOIST_PRED                   = 17,	  //道士前导
		TAOIST_RIGHT                  = 18,	  //道士纯阳
		TAOIST_WARROR                 = 19,	  //道士寒阴
		TAOIST_MITZVAH                = 20,	  //道士戒律
		MAGIC_PRED                    = 21,	  //幻术师前导
		MAGIC_WRESTIE                 = 22,	  //幻术师格斗
		MAGIC_LION                    = 23,	  //幻术师狮之幻化
		MAGIC_PANTER                  = 24,	  //幻术师豹之幻化
		FRESH_PRED                    = 25,	  //新手
		TALENT       = 26,						// 造化
		OGRE       = 27,						// 妖魔志
		FANGCUN		= 28,						// 方寸山
    };
    enum METIER_TYPE
    {
		ANYMETIER	= 0,					//任意
        WARRIOR		= 1,	                //战士 
        SWORDMAN	= 2,                   //剑客
        HUNTER		= 3,                     //猎人
        RABBI		= 4,                      //法师
        TAOIST		= 5,                     //道士
        MAGIC		= 6,                      //幻术师
        FRESH		= 7,                      //新手
    };
class CTemplateMetier : public CTemplate
{
public:

	// FieldName: $职业名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mMetierName[ STRING32 ];		

	// FieldName: 职业ID
	// FieldType: INT4
	// Type:	  COMBOENUM(METIER_TYPE)
	int		mMetierID;						

	// FieldName: 职业的第1项技能系别ID
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无=0,战士前导,战士武器,战士防御,战士狂暴,剑客前导,剑客刺杀,剑客冷血,剑客舍命,猎人前导,猎人射击,猎人陷阱,猎人法术,法师前导,法师玄冰,法师炽炎,法师怒风,道士前导,道士纯阳,道士寒阴,道士戒律,幻术师前导,幻术师格斗,幻术师狮之幻化,幻术师豹之幻化,新手)

	// FieldName: 职业的第2项技能系别ID
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无=0,战士前导,战士武器,战士防御,战士狂暴,剑客前导,剑客刺杀,剑客冷血,剑客舍命,猎人前导,猎人射击,猎人陷阱,猎人法术,法师前导,法师玄冰,法师炽炎,法师怒风,道士前导,道士纯阳,道士寒阴,道士戒律,幻术师前导,幻术师格斗,幻术师狮之幻化,幻术师豹之幻化,新手)

	// FieldName: 职业的第3项技能系别ID
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无=0,战士前导,战士武器,战士防御,战士狂暴,剑客前导,剑客刺杀,剑客冷血,剑客舍命,猎人前导,猎人射击,猎人陷阱,猎人法术,法师前导,法师玄冰,法师炽炎,法师怒风,道士前导,道士纯阳,道士寒阴,道士戒律,幻术师前导,幻术师格斗,幻术师狮之幻化,幻术师豹之幻化,新手)

	// FieldName: 职业的第4项技能系别ID
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无=0,战士前导,战士武器,战士防御,战士狂暴,剑客前导,剑客刺杀,剑客冷血,剑客舍命,猎人前导,猎人射击,猎人陷阱,猎人法术,法师前导,法师玄冰,法师炽炎,法师怒风,道士前导,道士纯阳,道士寒阴,道士戒律,幻术师前导,幻术师格斗,幻术师狮之幻化,幻术师豹之幻化,新手)

	// FieldName: 职业的第5项技能系别ID
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无=0,战士前导,战士武器,战士防御,战士狂暴,剑客前导,剑客刺杀,剑客冷血,剑客舍命,猎人前导,猎人射击,猎人陷阱,猎人法术,法师前导,法师玄冰,法师炽炎,法师怒风,道士前导,道士纯阳,道士寒阴,道士戒律,幻术师前导,幻术师格斗,幻术师狮之幻化,幻术师豹之幻化,新手)
	int		mGenerID[ 5 ];

	// FieldName: 每升一级获得技能点
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int		mSkillPoint;		

	// FieldName: 职业行走速度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mWalkSpeed;		

	// FieldName: 每升一级获得属性点
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int		mPropertyPoint;	
	

	// FieldName: 职业奔跑速度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mRunSpeed;		

	// FieldName: 职业初始等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int		mInitLevel;		

	// FieldName: 职业初始最大HP
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mInitHP;			

	// FieldName: 职业初始最大MP
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mInitMP;			


	// FieldName: 职业初始物理防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mInitPDefense;		

	// FieldName: 职业初始物理攻击
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mInitPAttack;

	// FieldName: 职业初始法术攻击
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mInitMAttack;		

	// FieldName: 职业初始法术防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mInitMDefense;		

	// FieldName: 职业初始异常状态抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mInitAbnResist;			

	// FieldName: 职业初始怒风状态抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mInitWindResist;		

	// FieldName: 职业初始炽焰状态抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mInitFireResist;		

	// FieldName: 职业初始玄冰状态抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mInitWaterResist;		

	// FieldName: 职业初始纯阳状态抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mInitLightResist;		

	// FieldName: 职业初始凝尘状态抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mInitEarthResist;		

	// FieldName: 职业初始寒阴状态抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mInitShadowResist;		

	// FieldName: 职业初始致命等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mInitDeathRate;		

	// FieldName: 职业初始暴击伤害加成
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mInitDeathValue;		

	// FieldName: 职业初始普攻命中
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mInitNormalHitrate;	

	// FieldName: 职业初始普攻闪避
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mInitNormalDuck;	

	// FieldName: 职业初始技能残影(残影)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mInitSkillDuck;		

	// FieldName: 职业初始忽视闪避
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mInitIgnoreDuck;

	// FieldName: 职业初始致命防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mInitDeathDefense;

	// FieldName: 职业每升一级增加最大HP
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mDeltaHP;					

	// FieldName: 职业每升一级增加最大MP
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mDeltaMP;					

	// FieldName: 职业每升一级增加物理攻击
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeltaPAttack;				

	// FieldName: 职业每升一级增加物理防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeltaPDefense;				

	// FieldName: 职业每升一级增加法术攻击
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeltaMAttack;				

	// FieldName: 职业每升一级增加法术防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeltaMDefense;				

	// FieldName: 职业每升一级增加异常状态抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeltaAbnResist;		

	// FieldName: 职业每升一级增加怒风状态抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeltaWindResist;				

	// FieldName: 职业每升一级增加炽焰状态抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeltaFireResist;				

	// FieldName: 职业每升一级增加玄冰状态抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeltaWaterResist;		

	// FieldName: 职业每升一级增加纯阳状态抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeltaLightResist;				

	// FieldName: 职业每升一级增加凝尘状态抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeltaEarthResist;				

	// FieldName: 职业每升一级增加寒阴状态抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeltaShadowResist;				

	// FieldName: 职业每升一级增加致命等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeltaDeathRate;			

	// FieldName: 职业每升一级增加暴击伤害加成
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeltaDeathValue;		

	// FieldName: 职业每升一级增加普攻命中
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeltaNormalHitrate;		

	// FieldName: 职业每升一级增加普攻闪避
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeltaNormalDuck;			

	// FieldName: 职业每升一级增加技能闪避(残影)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeltaSkillDuck;	

	// FieldName: 职业每升一级增加忽视闪避
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeltaIgnoreDuck;

	// FieldName: 职业每升一级增加致命防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeltaDeathDefense;

	// FieldName: 每点力量对应物理攻击
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mPAttackPerStr;	

	// FieldName: 每点力量对应法术攻击
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mMAttackPerStr;	

	// FieldName: 每点力量对应致命等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mDeathRatePerStr;

	// FieldName: 每点力量对应暴击伤害加成
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mDeathValuePerStr;	

	// FieldName: 每点力量对应HP
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mMaxHPPerStr;

	// FieldName: 每点力量对应MP
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mMaxMPPerStr;

	// FieldName: 每点力量对应物理防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mPDefensePerStr;

	// FieldName: 每点力量对应法术防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mMDefensePerStr;

	// FieldName: 每点力量对应普攻命中
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mNormalHitratePerStr;		

	// FieldName: 每点力量对应普攻闪避
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mNormalDuckPerStr;	

	// FieldName: 每点力量对应忽视闪避
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mIgnoreDuckPerStr;

	// FieldName: 每点力量对应致命防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeathDefensePerStr;

	// FieldName: 每点体质对应物理攻击
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mPAttackPerCon;	

	// FieldName: 每点体质对应法术攻击
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mMAttackPerCon;	

	// FieldName: 每点体质对应致命等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mDeathRatePerCon;

	// FieldName: 每点体质对应暴击伤害加成
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mDeathValuePerCon;	

	// FieldName: 每点体质对应HP
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mMaxHPPerCon;

	// FieldName: 每点体质对应MP
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mMaxMPPerCon;

	// FieldName: 每点体质对应物理防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mPDefensePerCon;

	// FieldName: 每点体质对应法术防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mMDefensePerCon;

	// FieldName: 每点体质对应普攻命中
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mNormalHitratePerCon;		

	// FieldName: 每点体质对应普攻闪避
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mNormalDuckPerCon;	

	// FieldName: 每点体质对应忽视闪避
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mIgnoreDuckPerCon;

	// FieldName: 每点体质对应致命防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeathDefensePerCon;

	// FieldName: 每点体质对应HP恢复速度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mHPRSpeedPerCon;			

	// FieldName: 每点耐力对应最大HP
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mMaxHPPerSta;

	// FieldName: 每点耐力对应物理减伤百分比
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int	mPReducePerSta;

	// FieldName: 每点耐力对应法术减伤百分比
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int	mMReducePerSta;

	// FieldName: 每点耐力对应最大每点耐力对应异常状态抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mAbnResistPerSta;

	// FieldName: 每点耐力对应致命防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeathDefensePerSta;

	// FieldName: 每点智力对应物理攻击
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mPAttackPerWis;

	// FieldName: 每点智力对应法术攻击
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mMAttackPerWis;	

	// FieldName: 每点智力对应最大HP
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mMaxHPPerWis;

	// FieldName: 每点智力对应最大MP
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mMaxMPPerWis;

	// FieldName: 每点智力对应致命等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mDeathRatePerWis;

	// FieldName: 每点智力对应暴击伤害
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mDeathValuePerWis;	

	// FieldName: 每点智力对应物理防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mPDefensePerWis;

	// FieldName: 每点智力对应法术防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mMDefensePerWis;

	// FieldName: 每点智力对应普攻命中
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mNormalHitratePerWis;		

	// FieldName: 每点智力对应普攻闪避
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mNormalDuckPerWis;	

	// FieldName: 每点智力对应忽视闪避
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mIgnoreDuckPerWis;

	// FieldName: 每点智力对应致命防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeathDefensePerWis;

	// FieldName: 每点精神对应物理攻击
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mPAttackPerSpr;

	// FieldName: 每点精神对应法术攻击
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mMAttackPerSpr;	

	// FieldName: 每点精神对应最大HP
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mMaxHPPerSpr;

	// FieldName: 每点精神对应最大MP
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mMaxMPPerSpr;

	// FieldName: 每点精神对应致命等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mDeathRatePerSpr;

	// FieldName: 每点精神对应暴击伤害
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mDeathValuePerSpr;	

	// FieldName: 每点精神对应物理防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mPDefensePerSpr;

	// FieldName: 每点精神对应法术防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mMDefensePerSpr;

	// FieldName: 每点精神对应普攻命中
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mNormalHitratePerSpr;		

	// FieldName: 每点精神对应普攻闪避
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mNormalDuckPerSpr;	

	// FieldName: 每点精神对应忽视闪避
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mIgnoreDuckPerSpr;

	// FieldName: 每点精神对应致命防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mDeathDefensePerSpr;

	// FieldName: 每点精神对应MP恢复速度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,63)
	int	mMPRSpeedPerSpr;			

	// FieldName: 普通状态HP恢复速度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mNormalHPRSpeed;	

	// FieldName: 普通打坐状态HP恢复速度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mNormalSitHPRSpeed;			

	// FieldName: 战斗状态HP恢复速度	
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mBattleHPRSpeed;			

	// FieldName: 战斗状态打坐HP恢复速度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mBattleSitHPRSpeed;			

	// FieldName: 普通状态MP恢复速度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mNormalMPRSpeed;		

	// FieldName: 普通打坐状态MP恢复速度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mNormalSitMPRSpeed;			

	// FieldName: 战斗状态MP恢复速度	
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mBattleMPRSpeed;			

	// FieldName: 战斗状态打坐MP恢复速度	
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mBattleSitMPRSpeed;	

public:
	void SetFromPB( bool bServer, PBMetier* pMetier );
	void CreateTlvPB( bool bServer, PBMetier* pMetier );
	int Initialize();
	CTemplateMetier()
	{
		Initialize();
	}
	~CTemplateMetier() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 出售表			## 模版名称
// version	= 1					## 版本号
// path		= Config/出售表		## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateSellTable : public CTemplate
{
public:
	enum EM_SELLTABLETYPE
	{
		SELLTABLETYPE_PLAYERMONEYNEEDED = 1,	// 出售表中的商品需要消耗玩家金钱
		SELLTABLETYPE_FAMILYMONEYNEEDED = 2,	// 出售表中的商品需要消耗家族金钱
		SELLTABLETYPE_CORPSMONEYNEEDED  = 3,	// 出售表中的商品需要消耗军团金钱
		SELLTABLETYPE_FAMILYWAR			= 4,	// 家族战商人的出售表
		SELLTABLETYPE_CORPSWAR			= 5,	// 军团战商人的出售表
	};
public:
	class CSellItem
	{
	public:
		char	mPage[ STRING32 ];
		int		mItemID[ 42 ];
	};

public:


	// FieldName: $第1页名称
	// FieldType: STRING32
	// Type:	  EDITSTRING

	// FieldName: 第1页第1项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第1页第2项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第1页第3项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第4项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第5项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第6项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第7项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第8项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第9项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第10项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第11项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第12项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第1页第13项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第14项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第15项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第16项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第17项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第18项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第19项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第20项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第21项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第22项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第1页第23项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第24项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第25项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第26项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第27项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第28项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第29项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第1页第30项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第31项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第32项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第1页第33项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第34项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第35项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第1页第36项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第37项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第38项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第39项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第1页第40项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第41项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第1页第42项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: $第2页名称
	// FieldType: STRING32
	// Type:	  EDITSTRING

	// FieldName: 第2页第1项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第2页第2项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第2页第3项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第4项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第5项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第6项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第7项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第8项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第9项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第10项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第11项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第12项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第2页第13项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第14项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第15项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第16项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第17项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第18项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第19项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第20项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第21项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第22项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第2页第23项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第24项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第25项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第26项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第27项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第28项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第29项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第2页第30项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第31项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第32项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第2页第33项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第34项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第35项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第2页第36项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第37项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第38项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第39项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第2页第40项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第41项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第2页第42项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: $第3页名称
	// FieldType: STRING32
	// Type:	  EDITSTRING

	// FieldName: 第3页第1项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第3页第2项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第3页第3项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第4项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第5项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第6项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第7项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第8项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第9项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第10项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第11项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第12项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第3页第13项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第14项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第15项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第16项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第17项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第18项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第19项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第20项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第21项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第22项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第3页第23项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第24项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第25项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第26项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第27项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第28项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第29项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第3页第30项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第31项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第32项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第3页第33项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第34项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第35项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第3页第36项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第37项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第38项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第39项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第3页第40项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第41项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第3页第42项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: $第4页名称
	// FieldType: STRING32
	// Type:	  EDITSTRING

	// FieldName: 第4页第1项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第4页第2项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第4页第3项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第4项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第5项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第6项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第7项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第8项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第9项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第10项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第11项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第12项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第4页第13项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第14项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第15项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第16项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第17项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第18项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第19项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第20项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第21项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第22项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第4页第23项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第24项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第25项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第26项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第27项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第28项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第29项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第4页第30项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第31项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第32项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第4页第33项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第34项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第35项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第4页第36项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第37项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第38项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第39项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第4页第40项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第41项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第4页第42项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: $第5页名称
	// FieldType: STRING32
	// Type:	  EDITSTRING

	// FieldName: 第5页第1项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第5页第2项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第5页第3项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第4项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第5项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第6项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第7项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第8项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第9项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第10项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第11项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第12项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第5页第13项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第14项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第15项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第16项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第17项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第18项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第19项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第20项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第21项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第22项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第5页第23项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第24项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第25项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第26项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第27项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第28项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第29项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第5页第30项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第31项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第32项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第5页第33项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第34项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第35项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第5页第36项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第37项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第38项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第39项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第5页第40项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第41项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第5页第42项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: $第6页名称
	// FieldType: STRING32
	// Type:	  EDITSTRING

	// FieldName: 第6页第1项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第6页第2项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第6页第3项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第4项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第5项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第6项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第7项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第8项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第9项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第10项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第11项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第12项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第6页第13项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第14项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第15项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第16项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第17项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第18项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第19项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第20项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第21项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第22项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第6页第23项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第24项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第25项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第26项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第27项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第28项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第29项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第6页第30项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第31项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第32项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第6页第33项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第34项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第35项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第6页第36项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第37项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第38项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第39项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第6页第40项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第41项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 第6页第42项道具ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	CSellItem	mItem[ 6 ];	
	
	
	// FieldName: 出售表类型
	// FieldType: INT4
	// Type:	  COMBOENUM(EM_SELLTABLETYPE)
	int mSellTableType;	
	

	// FieldName: 第%d页第%d个商品的金锭价钱
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int	mItemGoldPrice[6][42];		 	
	
	// FieldName: 第%d页第%d个商品的最大出售数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int	mItemNum[6][42];	
	
	// FieldName: 如果用作占领商店时所在占领城市的地图ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int	mWarCityMapID; 
	
	// FieldName: 第%d页第%d个商品的刷新时间(小时)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int	mItemRefreshDay[6][42];	
		
public:
	void SetFromPB( bool bServer, PBSellTable* pTable );
	void CreateTlvPB( bool bServer, PBSellTable* pTable );
	int Initialize();
	CTemplateSellTable()
	{
		Initialize();
	}
	~CTemplateSellTable() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 掉落母表			## 模版名称
// version	= 1					## 版本号
// path		= Config/掉落母表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //

class CTemplateTotalDrop : public CTemplate
{
public:

	// FieldName: 掉落子表%d的id
	// FieldType: INT4
	// Type:	  TEMPPATH	

	int		mDropTableID[ 32 ];

	// FieldName: 掉落子表%d的次数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	int		mDropCount[ 32 ];

public:
	void SetFromPB( bool bServer, PBTotalDrop* pDrop );
	void CreateTlvPB( bool bServer, PBTotalDrop* pDrop );
	int Initialize();
	CTemplateTotalDrop()
	{
		Initialize();
	}
	~CTemplateTotalDrop() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

enum DropNoticeType
{
	NONE_NOTICE		= 0,		// 无通知
	MINEMAP_NOTICE	= 1,		// 藏宝图通知
	COUNTRY_NOTICE	= 2,		// 阵营掉落通知
	WORLD_NOTICE	= 3,		// 世界掉落通知
	SPECIAL_NOTICE	= 4,		// 特殊广播通知
};

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 掉落子表			## 模版名称
// version	= 1					## 版本号
// path		= Config/掉落子表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateDrop : public CTemplate
{
public:
	class CDropItem
	{
	public:
		int		mDropItemID;
		int		mDropItemProbability;
		int		mLevelProbability;
		int		mNotify;
		int		mSpecial;
		int		mShow;

	};

public:
	
	// FieldName: 掉落大轮数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,31)
	int		mDropTurn;

	// FieldName: 每大轮掉落0个物品的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 每大轮掉落1个物品的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	
	// FieldName: 每大轮掉落2个物品的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	
	// FieldName: 每大轮掉落3个物品的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	
	// FieldName: 每大轮掉落4个物品的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int	mDropNumberProbability[ 5 ];	

	// FieldName: 掉落第%d个物品的id
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 掉落第%d个物品的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 掉落第%d个物品的级数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,15)

	// FieldName: 掉落第%d个物品的提示
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,藏宝图=1,阵营通知=2,世界通知=3,特殊提示=4)

	// FieldName: 掉落第%d个物品的特殊提示码
	// FieldType: INT4
	// Type:	  EDITNUMBER(-100000,100000)

	// FieldName: 掉落第%d个物品是否显示
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,显示=1)

	CDropItem	mDrops[ 50 ];

public:
	void SetFromPB( bool bServer, PBDropTable* pDrop );
	void CreateTlvPB( bool bServer, PBDropTable* pDrop );
	int Initialize();
	CTemplateDrop()
	{
		Initialize();
	}
	~CTemplateDrop(){}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 采集NPC			## 模版名称
// version	= 1					## 版本号
// path		= 采集NPC			## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateCollect : public CTemplate
{
public:
	enum ECollectType
	{
		LIFESKILL_TASK	= 0,	// 任务的采集npc
		LIFESKILL_LIFESKILL,	// 生活技能的采集npc
	}; 	

	enum ELifSkillKind
	{
		SKILLKIND_NULL	= 0,	// 无		
		SKILLKIND_PLANT,	// 植物
		SKILLKIND_MINERAL,	// 矿物
	};	

	enum ECollectTaskType
	{	
		TASKTYPE_COMMON = 0,	// 普通采集任务
		TASKTYPE_CORPSCOLLECT = 1,		// 军团采集任务
	};

	class CDegreeInfo	
	{	
		public:							
		int		mValue;			// 玩家本身熟练度与要求的熟练度的差值
		int		mProbality;		// 在差值范围内获取熟练度的概率
		int		mDegreeValue;	// 在差值范围内获取熟练度的值
	};
public:


	// FieldName: $NPC名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mName[ STRING32 ];			

	// FieldName: 图形资源名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mModelName[ STRING32 ];

	// FieldName: 第%d个任务
	// FieldType: INT4
	// Type:      TASKPATH
	int     mTaskID[ 10 ];

	// FieldName: 第%d个任务需要物品
	// FieldType: INT4
	// Type:      TEMPPATH
	int     mItemID[ 10 ];
	
	// FieldName: 第%d个任务需要物品的数量
	// FieldType: INT4
	// Type:      EDITNUMBER(0,100) 
	int     mItemNum[ 10 ];
	
	// FieldName: 第%d个任务需要的产出概率
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000) 
	int     mItemProbality[ 10 ];

	// FieldName: 第%d个任务额外产出的数量
	// FieldType: INT4
	// Type:      EDITNUMBER(0,100)
	int		mExtraItemNum[ 10 ];

	// FieldName: 第%d个任务额外产出的概率
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)
	int		mExtraItemProbality[ 10 ];

	// FieldName: 最大采集次数
	// FieldType: INT4
	// Type:      EDITNUMBER(0,1000) 
	int     mNumber;

	// FieldName: 掉落母表
	// FieldType: INT4
	// Type:      TEMPPATH 
	int     mDropID;	
	
	// FieldName: 采集需要的技能类型
	// FieldType: INT4
	// Type:      COMBOENUM(ECollectType) 
	int		mSkillType;
	
	// FieldName: 采集需要的技能类型归属类别
	// FieldType: INT4
	// Type:      COMBOENUM(ELifSkillKind) 
	int		mSkillKind;
	
	// FieldName: 生活技能采集需要消耗的活力值
	// FieldType: INT4
	// Type:      EDITNUMBER(0,1000) 
	int		mApCost; 
	
	// FieldName: 采集需要的熟练度
	// FieldType: INT4	
	// Type:      EDITNUMBER(0,1000) 
	int		mCollectDegree;
	
	// FieldName: 自身熟练度和采集需要的熟练度的第%d个差值区间
	// FieldType: INT4	
	// Type:      EDITNUMBER(0,1000) 

	
	// FieldName: 自身熟练度和采集需要的熟练度的第%d个差值区间的获取数量度的概率
	// FieldType: INT4	
	// Type:      EDITNUMBER(0,10000) 

	
	// FieldName: 自身熟练度和采集需要的熟练度的第%d个差值区间的获取数量度的值
	// FieldType: INT4	
	// Type:      EDITNUMBER(0,10000) 

	CDegreeInfo mDegreeInfo[MAX_LIFESKILL_DEGREESTEP];
	
	// FieldName: 采集需要的等级
	// FieldType: INT4	
	// Type:      EDITNUMBER(0,1000) 
	int		mCollectLevel;

	// FieldName: 采集的任务类型
	// FieldType: INT4	
	// Type:      COMBOENUM(ECollectTaskType) 
	int		mCollectTaskType;

public:
	void SetFromPB( bool bServer, PBNPCCollect* pNpc );
	void CreateTlvPB( bool bServer, PBNPCCollect* pNpc );
	int Initialize();
	CTemplateCollect()
	{
#ifndef TEMPEDIT
		mTemplateType = EM_TEMPTYPE_COLLECT; 
#endif
		Initialize();
	}
	~CTemplateCollect() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= true				## 代表生成到数据库中
// name		= 怪物和NPC			## 模版名称
// version	= 					## 版本号
// path		=					## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateOgre : public CTemplate
{
public:
	enum
	{
		PATROL_RAND,		// 随机走动
		PATROL_STAND,		// 站立不动
		PATROL_FIX,			// 固定走动
	};

	enum
	{
		OGRESHOW_NONE	= 1,		
		OGRESHOW_TASK,
		OGRESHOW_VIP,		
		OGRESHOW_NORMAL,
		OGRESHOW_BOSS,
	};	
	
	enum ESpecialEvent
	{
		EVENT_NONE				=	0,
		EVENT_GATE				=	1,		// 城门类型
		EVENT_FLAG				=	2,		// 大旗类型
	};

public:

	// FieldName: $NPC名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mName[ STRING32 ];			

	// FieldName: 图形资源名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mModelName[ STRING32 ];		

	// FieldName: 怪物类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(物理攻击=1,法术攻击)
	int		mOgreType;	
	
	// FieldName: 掉落方式
	// FieldType: INT4
	// Type:	  COMBOSINGLE(攻击顺序=1,伤害顺序)
	int		mDropType;	
	
	// FieldName: 策略ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mStrategyID;

	// FieldName: 掉落ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mDropID;	

	// FieldName: 怪物等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,160)
	int		mLevel;				

	// FieldName: 怪物显示等级
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无=1,首领怪,精英怪,普通怪,BOSS)
	int		mDisplayLevel;
	
	// FieldName: 攻击频率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mAttackRate;

	// FieldName: 攻击动作时间
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mAttackTime;
	
	// FieldName: 怪物回血速度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mHPRSpeed;					

	// FieldName: 怪物回蓝速度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mMPRSpeed;							

	// FieldName: 怪物奔跑速度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mRunSpeed;		

	// FieldName: 怪物初始等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mInitLevel;		

	// FieldName: 怪物初始最大HP
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mHP;				

	// FieldName: 怪物初始最大MP
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mMP;				

	// FieldName: 怪物初始物理攻击
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mPAttack;			

	// FieldName: 怪物初始物理防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mPDefence;			

	// FieldName: 怪物初始物理攻击距离
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mPAttackRange;		

	// FieldName: 怪物初始物理减伤
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mPReducePer;

	// FieldName: 怪物初始法术攻击
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mMAttack;					

	// FieldName: 怪物初始法术防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mMDefence;			

	// FieldName: 怪物初始法术攻击距离
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMAttackRange;

	// FieldName: 怪物初始法术减伤
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mMReducePer;

	// FieldName: 怪物初始暴击率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mDeathRate;			

	// FieldName: 怪物初始暴击伤害加成
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mDeathValue;	

	// FieldName: 怪物初始暴击防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mDeathDefense;

	// FieldName: 怪物初始普攻命中
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mNormalHitrate;		

	// FieldName: 怪物初始普攻闪避
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mNormalDuck;

	// FieldName: 怪物初始忽视闪避
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mIgnoreDuck;

	// FieldName: 怪物跑回是否回血
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1)
	int		mRecover;	

	// FieldName: 怪物初始抗性%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mResDef[ 6 ];

	// FieldName: 怪物初始属性攻击%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mResAtt[ 6 ];

	// FieldName: 怪物是否远程攻击
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int		mNeedDelay;			

	// FieldName: 巡逻类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(随机走动=0,站立不动,固定走动)
	int		mPatrolType;	

	// FieldName: 是否主动攻击怪物
	// FieldType: INT4
	// Type:	  COMBOSINGLE(被动怪=0,主动怪)
	int		mIsInitiative;		

	// FieldName: 休闲距离
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mWalkDistance;

	// FieldName: 传送时间
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mTeleTime;

	// FieldName: 仇恨距离
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mEnmityDistance;	

	// FieldName: 仇恨时间(秒为单位)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mEnmityTime;	

	// FieldName: 罚站时间(秒为单位)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mClearTimer;			

	// FieldName: 怪物死亡后得到经验
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int		mExp;					

	// FieldName: 怪物死亡后得到初始金钱
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mInitMoney;					

	// FieldName: 怪物死亡后得到金钱浮动值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mDeltaMoney;				 

	// FieldName: 免疫状态列表
	// FieldType: INT4
	// Type:	  CHECKMUTI(定身,眩晕,沉默)
	int		mResist;					

	// FieldName: 怪物转换ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mTranID;

	// FieldName: PK类型
	// FieldType: INT4
	// Type:	  CHECKMUTI(对怪物仇视,被怪物仇视,对本国玩家友好,本国玩家对其友好,对本国玩家中立,本国玩家对其中立,对本国玩家敌对,本国玩家对其敌对,对外国玩家友好,外国玩家对其友好,对外国玩家中立,外国玩家对其中立,对外国玩家敌对,外国玩家对其敌对)
	int		mPKType;

	// FieldName: 阵营ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mGroupID;

	// FieldName: 技能ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mSkillID;

	// FieldName: 技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mSkillLevel;

	// FieldName: 搜索距离
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mSearchDistance;

	// FieldName: 存活时间(秒)(到时间就干掉)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mLifeTime;

	// FieldName: 抗性显示
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无=-1,抗性1=0,抗性2,抗性3,抗性4,抗性5,抗性6)
	int		mResShow;

	// FieldName: 死亡后提供的正义值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int		mJusticeValue;

	// FieldName: NPC消失后引发任务%d失败
	// FieldType: INT4
	// Type:	  TASKPATH
	int		mFailTaskID[MAX_FAIL_TASKS];

	// FieldName: 免疫Buff功能列表
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mBuffImmunity;

	// FieldName: 杀死本怪玩家获得称号ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mObtainTitle;

	// FieldName: 领取奖励
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mAwardID;

	// FieldName: 限制的服务器ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int		mLmtLineID;
	
	// FieldName: 阵营内丹ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int		mRedstoneID;

	// FieldName: 怪物特殊事件
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无=0,城门=1,大旗=2)
	int			mOgreEvent;

	// FieldName: 炼化度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mCorpsValue;

	// FieldName: 掉落描述id
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mDropsDesc;

	// FieldName: 怪物难度系数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mDiffcultIndex;

	// FieldName: 怪物难度奖励积分
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mDiffcultAward;



public:
	void SetFromPB( bool bServer, PBOgreProperty* pOgre );
	void CreateTlvPB( bool bServer, PBOgreProperty* pOgre );
	int Initialize();
	CTemplateOgre()
	{
#ifndef TEMPEDIT
		mTemplateType = EM_TEMPTYPE_OGRE; 
#endif
		Initialize();
	}
	~CTemplateOgre() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 宝箱				## 模版名称
// version	= 1					## 版本号
// path		= 宝箱				## 路径
// parent	= 怪物和NPC			## 父亲模版
// ********************************************************************** //
class CTemplateBox : public CTemplateOgre
{
public:
	enum STORTBOX_EVENT
	{
		EVENT_NOTHING		= 0,	// 无事件
		EVENT_DROPITEM,				// 掉落事件
		EVENT_MACHINERY,			// 机关事件
        EVENT_PICKFORYQS,           // 用户在摇钱树活动中拾取宝箱
		EVENT_PICKFORCORPSBOSS,		//  用户在军团boss副本中拾取宝箱
	};

	enum BOX_TYPE
	{
		BOX_NEEDKEY			= 0,	// 宝箱需要钥匙
		BOX_NOTNEEDKEY,				// 宝箱不需要钥匙
	};

	enum STORTBOX_TYPE
	{
		TYPE_NEEDKEY		= 0,	// 需要钥匙的宝箱
		TYPE_NEEDNOTKEY	,			// 不需要钥匙的宝箱
	};

	struct STORTBOX_CLICK
	{
		STORTBOX_EVENT	mEvent;	
		int				mProbability;
	};
public:
	// FieldName: 宝箱第%d事件
	// FieldType:	INT4
	// Type:      COMBOENUM(STORTBOX_EVENT)

	// FieldName: 宝箱第%d事件概率
	// FieldType:	INT4
	// Type:      EDITNUMBER(0,10000)
	STORTBOX_CLICK mClick[ 5 ];

	// FieldName: 宝箱类型
	// FieldType: INT4
	// Type:	  COMBOENUM(BOX_TYPE)
	BOX_TYPE		mType;

	// FieldName: %d档掉落表(开了几个箱子)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int				mDropTable[ MAX_BOXLEVEL ];
	
	// FieldName: 钥匙ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int				mKey;	

public:
	void SetFromPB( bool bServer, PBOgreBox* pBox );
	void CreateTlvPB( bool bServer, PBOgreBox* pBox );
	int Initialize();
	CTemplateBox() 
	{
#ifndef TEMPEDIT
		mTemplateType = EM_TEMPTYPE_BOX; 
#endif
		Initialize();
	}
	~CTemplateBox() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 阻挡NPC				## 模版名称
// version	= 1					## 版本号
// path		= 阻挡NPC				## 路径
// parent	= 怪物和NPC			## 父亲模版
// ********************************************************************** //

class CTemplateBlock : public CTemplateOgre
{
public:
	enum IS_SELECT
	{
		CAN_SELECT			= 0,	//可以被选择
		NO_SELECT,					//不可以被选择
	};

	struct Relatively_Pos
	{
		int mX;
		int mY;
	};
public:
	

	// FieldName: 相对偏移第%d个坐标X
	// FieldType:	INT4
	// Type:      EDITNUMBER(-50,50)

	// FieldName: 相对偏移第%d个坐标Y
	// FieldType:	INT4
	// Type:      EDITNUMBER(-50,50)
	Relatively_Pos mPos[ 30 ];

	// FieldName: 是否可以被选择
	// FieldType: INT4
	// Type:	  COMBOENUM(IS_SELECT)
	IS_SELECT		mSelect;

public:
	void SetFromPB( bool bServer, PBBlock* pBlock );
	void CreateTlvPB( bool bServer, PBBlock* pBlock );
	int Initialize();
	CTemplateBlock()
	{
#ifndef TEMPEDIT
		mTemplateType = EM_TEMPTYPE_BLOCK; 
#endif
		Initialize();
	}
	~CTemplateBlock() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 观赏宠			## 模版名称
// version	= 1					## 版本号
// path		= 宠物/观赏宠		## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateShowPet : public CTemplate
{
public:

	// FieldName: $游戏中的显示名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mName[ STRING32 ];			

	// FieldName: 模型路径
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mModel[ STRING32 ];			

	// FieldName: 图标路径
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mIconName[ STRING32 ];		

	// FieldName: 适合的职业
	// FieldType: INT4
	// Type:	  COMBOSINGLE(战士=1,剑客,猎人,法师,道士,幻术师,新手职业)
	int		mMetierID;

	// FieldName: 宠物等级上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,150)
	int		mMaxLevel;

	// FieldName: 对玩家的等级要求
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,150)
	int		mRequireLevel;

	// FieldName: 最多可以高出玩家多少级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,150)
	int		mExceedLevel;

	// FieldName: 从宠物蛋孵化成宠物需要的rmb道具id
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mPetItemID;

	// FieldName: 从宠物还原成宠物蛋需要收取的虚拟币
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mRevertCost;

	// FieldName: 初始仓库格子数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int		mInitStorage;

	// FieldName: 宠物每升多少级仓库格子数量增加
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int		mLevelIncStorage;
	
	// FieldName: 宠物因为升级每次增加的仓库格子数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int		mIncStorage;

	// FieldName: 每分钟观赏宠获得的经验
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mExpPerMin;

	// FieldName: 饱食度上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mRepletionLimit;

	// FieldName: 当宠物的饱食度达到什么数值时让主人战斗状态下回血
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mRepletionHP;

	// FieldName: 当宠物让主人在战斗状态下回血时一次回多少
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mRevertHPValue;

	// FieldName: 状态1的饱食度下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMinRepletion1;

	// FieldName: 状态1的饱食度上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)	
	int		mMaxRepletion1;

	// FieldName: 在状态1下宠物获得经验的速度是标准速度的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mExpSpeedRate1;

	// FieldName: 状态2的饱食度下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMinRepletion2;

	// FieldName: 状态2的饱食度上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)	
	int		mMaxRepletion2;

	// FieldName: 在状态2下宠物获得经验的速度是标准速度的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mExpSpeedRate2;

	// FieldName: 状态3的饱食度下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMinRepletion3;

	// FieldName: 状态3的饱食度上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)	
	int		mMaxRepletion3;

	// FieldName: 在状态3下宠物获得经验的速度是标准速度的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mExpSpeedRate3;

	// FieldName: 状态4的饱食度下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMinRepletion4;

	// FieldName: 状态4的饱食度上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)	
	int		mMaxRepletion4;

	// FieldName: 在状态4下宠物获得经验的速度是标准速度的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mExpSpeedRate4;

	// FieldName: 状态5的饱食度下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMinRepletion5;

	// FieldName: 状态5的饱食度上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)	
	int		mMaxRepletion5;

	// FieldName: 在状态5下宠物获得经验的速度是标准速度的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mExpSpeedRate5;

	// FieldName: 可吃的食物组合
	// FieldType: INT4
	// Type:	  CHECKMUTI(蔬菜,水果,肉类)
	int		mFoodCombo;

public:
	void SetFromPB( bool bServer, PBShowPet* pPet );
	void CreateTlvPB( bool bServer, PBShowPet* pPet );
	int Initialize();
	CTemplateShowPet() 
	{
		Initialize();
	}
	~CTemplateShowPet() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 战斗宠			## 模版名称
// version	= 1					## 版本号
// path		= 宠物/战斗宠		## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateBattlePet : public CTemplate
{
public:

	// FieldName: $游戏中的显示名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mName[ STRING32 ];			

	// FieldName: 模型路径
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mModel[ STRING32 ];			

	// FieldName: 图标路径
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mIconName[ STRING32 ];		

	// FieldName: 适合的职业
	// FieldType: INT4
	// Type:	  COMBOSINGLE(战士,剑客,猎人,法师,道士,幻术师,新手职业)
	int		mMetierID;

	// FieldName: 宠物等级上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,150)
	int		mMaxLevel;

	// FieldName: 对玩家的等级要求
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,150)
	int		mRequireLevel;

	// FieldName: 最多可以高出玩家多少级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,150)
	int		mExceedLevel;

	// FieldName: 从宠物蛋孵化成宠物需要的rmb道具id
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mPetItemID;

	// FieldName: 从宠物还原成宠物蛋需要收取的虚拟币
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mRevertCost;

	// FieldName: 饱食度上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mRepletionLimit;

	// FieldName: 状态1的饱食度下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMinRepletion1;

	// FieldName: 状态1的饱食度上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)	
	int		mMaxRepletion1;

	// FieldName: 在状态1下宠物获得经验的速度是标准速度的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mExpSpeedRate1;

	// FieldName: 在状态1下宠物的hp上限是正常的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mHPRate1;

	// FieldName: 在状态1下宠物的攻击是正常的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mAttackRate1;

	// FieldName: 状态2的饱食度下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMinRepletion2;

	// FieldName: 状态2的饱食度上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)	
	int		mMaxRepletion2;

	// FieldName: 在状态2下宠物获得经验的速度是标准速度的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mExpSpeedRate2;

	// FieldName: 在状态1下宠物的hp上限是正常的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mHPRate2;

	// FieldName: 在状态1下宠物的攻击是正常的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mAttackRate2;

	// FieldName: 状态3的饱食度下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMinRepletion3;

	// FieldName: 状态3的饱食度上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)	
	int		mMaxRepletion3;

	// FieldName: 在状态3下宠物获得经验的速度是标准速度的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mExpSpeedRate3;

	// FieldName: 在状态1下宠物的hp上限是正常的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mHPRate3;

	// FieldName: 在状态1下宠物的攻击是正常的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mAttackRate3;

	// FieldName: 状态4的饱食度下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMinRepletion4;

	// FieldName: 状态4的饱食度上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)	
	int		mMaxRepletion4;

	// FieldName: 在状态4下宠物获得经验的速度是标准速度的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mExpSpeedRate4;

	// FieldName: 在状态1下宠物的hp上限是正常的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mHPRate4;

	// FieldName: 在状态1下宠物的攻击是正常的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mAttackRate4;

	// FieldName: 状态5的饱食度下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMinRepletion5;

	// FieldName: 状态5的饱食度上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)	
	int		mMaxRepletion5;

	// FieldName: 在状态5下宠物获得经验的速度是标准速度的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mExpSpeedRate5;

	// FieldName: 在状态1下宠物的hp上限是正常的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mHPRate5;

	// FieldName: 在状态1下宠物的攻击是正常的
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int	mAttackRate5;

	// FieldName: 生命值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mHPValue;

	// FieldName: 生命值潜力
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mHPPotential;

	// FieldName: 法力值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMPValue;

	// FieldName: 法力值潜力
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMPPotential;

	// FieldName: 物理攻击力
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mPAttack;

	// FieldName: 物理攻击力潜力
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mPAttackPotential;

	// FieldName: 法术攻击力
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMAttack;

	// FieldName: 法术攻击力潜力
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMAttackPotential;

	// FieldName: 物理防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mPDefence;

	// FieldName: 物理防御潜力
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mPDefencePotential;

	// FieldName: 法术防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMDefence;

	// FieldName: 法术防御潜力
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMDefencePotential;

	// FieldName: 物理免疫
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mPResist;

	// FieldName: 物理免疫潜力
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mPResistPotential;

	// FieldName: 法术免疫
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMResist;

	// FieldName: 法术免疫潜力
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMResistPotential;

	// FieldName: 异常状态基础抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mAbnResist;

	// FieldName: 异常状态基础抗性潜力
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mAbnResistPotential;

	// FieldName: 炽焰系抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mFireResist;

	// FieldName: 炽焰系抗性潜力
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mFireResistPotential;

	// FieldName: 怒风系抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mWindResist;

	// FieldName: 怒风系抗性潜力
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mWindResistPotential;

	// FieldName: 纯阳系抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mLightResist;

	// FieldName: 纯阳系抗性潜力
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mLightResistPotential;

	// FieldName: 寒阴系抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mShadowResist;

	// FieldName: 寒阴系抗性潜力
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mShadowResistPotential;

	// FieldName: 玄冰系抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mWaterResist;

	// FieldName: 玄冰系抗性潜力
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mWaterResistPotential;

	// FieldName: 凝尘系抗性
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mEarthResist;

	// FieldName: 凝尘系抗性潜力
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mEarthResistPotential;

	// FieldName: 可吃的食物组合
	// FieldType: INT4
	// Type:	  CHECKMUTI(蔬菜,水果,肉类)
	int		mFoodCombo;

public:
	void SetFromPB( bool bServer, PBBattlePet* pPet );
	void CreateTlvPB( bool bServer, PBBattlePet* pPet );
	int Initialize();
	CTemplateBattlePet()
	{
		Initialize();
	}
	~CTemplateBattlePet() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 配方				## 模版名称
// version	= 1					## 版本号
// path		= 配方				## 路径
// parent	= 道具基础信息		## 父亲模版
// ********************************************************************** //
class CTemplateBlueprint : public CTplItem
{
protected:
	class CStuff
	{
	public:
		int		mStuffID;		// 道具ID
		int		mNumber;
	};

	class CProduct
	{
	public:
		int		mProductID;		// 道具ID
		int	mPercent;		// 产出概率
	};

public:

	// FieldName: 需要的技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH
	int			mNeedSkillID;

	// FieldName: 需要的技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)
	int			mNeedSkillLevel;

	// FieldName: 一次生产的个数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)
	int			mProduceCount;

	// FieldName: 一次生产的手续费
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,5000)
	int			mProduceCost;

	// FieldName: 每次生产获得的基础经验
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,5000)
	int			mObtainExp;

	// FieldName: 需要的第1项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 需要的第1项原料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 需要的第2项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 需要的第2项原料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 需要的第3项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 需要的第3项原料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 需要的第4项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 需要的第4项原料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 需要的第5项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 需要的第5项原料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 需要的第6项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 需要的第6项原料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 需要的第7项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 需要的第7项原料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 需要的第8项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 需要的第8项原料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 需要的第9项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 需要的第9项原料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 需要的第10项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 需要的第10项原料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 需要的第11项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 需要的第11项原料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 需要的第12项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 需要的第12项原料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 需要的第13项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 需要的第13项原料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 需要的第14项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 需要的第14项原料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 需要的第15项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 需要的第15项原料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 需要的第16项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 需要的第16项原料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	CStuff		mStuff[ 16 ];

	// FieldName: 得到的第1项产品ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 得到的第1项产品概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 得到的第2项产品ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 得到的第2项产品概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 得到的第3项产品ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 得到的第3项产品概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 得到的第4项产品ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 得到的第4项产品概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 得到的第5项产品ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 得到的第5项产品概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 得到的第6项产品ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 得到的第6项产品概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 得到的第7项产品ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 得到的第7项产品概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 得到的第8项产品ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 得到的第8项产品概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 得到的第9项产品ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 得到的第9项产品概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 得到的第10项产品ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 得到的第10项产品概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 得到的第11项产品ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 得到的第11项产品概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 得到的第12项产品ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 得到的第12项产品概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 得到的第13项产品ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 得到的第13项产品概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 得到的第14项产品ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 得到的第14项产品概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 得到的第15项产品ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 得到的第15项产品概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 得到的第16项产品ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 得到的第16项产品概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	CProduct	mProduct[ 16 ];

public:
	void SetFromPB( bool bServer, PBItemBluePrint* pItem );
	void CreateTlvPB( bool bServer, PBItemBluePrint* pItem );
	int Initialize();
	CTemplateBlueprint()
	{
		Initialize();
	}
	~CTemplateBlueprint() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 怪物属性			## 模版名称
// version	= 1					## 版本号
// path		= 怪物属性			## 路径
// parent	= 怪物和NPC			## 父亲模版
// ********************************************************************** //
enum EFamilyService
{
	FAMILYSERVICE_NONE = 0,			// 无家族服务
	FAMILYSERVICE_FAMILYNPC = 1,		// 家族NPC
	FAMILYSERVICE_AREANPC	= 2,			// 领地NPC
	FAMILYSERVICE_CONTRIBUTENPC = 3,	// 贡献NPC
	FAMILYSERVICE_TASKNPC		= 4,			// 任务NPC
	FAMILYSERVICE_ACTIVITYNPC = 5,		// 家族活动NPC
	FAMILYSERVICE_WARNPC	 = 6,			// 家族战争NPC
	FAMILYSERVICE_HOMENPC     = 7,			// 家园NPC	
};
enum ELifeSkillService
{
	LIFESKILLSERVICE_NONE		= 0,		// 无生活技能服务
	LIFESKILLSERVICE_COLLECT	= 1,		// 采集技能服务	
	LIFESKILLSERVICE_GOLDMAKE	= 2,		// 点金技能服务
	LIFESKILLSERVICE_ARTIFICER	= 3,		// 巧匠技能服务
	LIFESKILLSERVICE_PILLMAKER	= 4,		// 炼丹技能服务
	LIFESKILLSERVICE_KOOKER		= 5,		// 烹饪技能服务
};
class CTemplateNpc : public CTemplateOgre
{
public:
	enum EFuncType
	{
		FUNCNPC_SERVICE		= 1,
		FUNCNPC_GUARDER,
		FUNCNPC_TASK,
	};

	enum EWDayType
	{
		WDAY_MONDAY			= 0x0001,		// 周一
		WDAY_TUESDAY		= 0x0002,		// 周二
		WDAY_WEDENSDAY		= 0x0004,		// 周三
		WDAY_THURSDAY		= 0x0008,		// 周四
		WDAY_FRIDAY			= 0x0010,		// 周五
		WDAY_SATURDAY		= 0x0020,		// 周六
		WDAY_SUNDAY			= 0x0040,		// 周日
	};
	
	enum EMarriageType
	{
		MARRIAGETYPE_NOSERVICE		=	0,		// 没有婚礼服务		
		MARRIAGETYPE_MARRIAGE		=	1,		// 月老npc
		MARRIAGETYPE_WEDDINGHALL	=	2,		// 礼堂接引人
		MARRIAGETYPE_BAITANG		=	3,		// 拜堂npc		
		MARRIAGETYPE_EXCHANGE		=	4,		// 结婚兑换
	};

	enum ESwornType
	{
		SWORNTYPE_NOSERVICE			=	0,		// 没有结拜服务
		SWORNTYPE_SWORN				=	1,		// 结拜服务
		SWORNTYPE_EXCHANGE			=	2,		// 结拜兑换
	};

	enum EFamilyHomeType
	{
		FAMILYHOME_NONE				=	0,		// 无家园功能
		FAMILYHOME_MAIN				=	1,		// 家族主屋
		FAMILYHOME_WUFANG			=	2,		// 家族武坊
		FAMILYHOME_GONGFANG			=	3,		// 家族工坊
		FAMILYHOME_SHUFANG			=	4,		// 家族书坊
		FAMILYHOME_ITEM				=	5,		// 家族装饰
	};

	enum EWarOpType
	{
		WAR_OP_NONE					=	0,		// 无类型
		WAR_OP_CHARACTER			=	1,		// 占领据点
		WAR_OP_STONE				=	2,		// 交灵石
		WAR_OP_DAQI					=	3,		// 大旗
		WAR_OP_FLAG					=	4,		// 占领旗
	};

	enum EWarServiceType
	{
		WAR_SERVICE_NONE		=	0,		// 无
		WAR_SERVICE_MAIN	=	0x01,		// 主要
		WAR_SERVICE_CAMP	=	0x02,		// 营地
		WAR_SERVICE_AWARD	=	0x04,		// 奖励
		WAR_SERVICE_TELE	=	0x08,		// 传送
		WAR_SERVICE_BUFF	=	0x10,		// 领buff
	};

	enum ETaxServiceType
	{
		TAX_SERVICE_NONE		=	0,		// 无
		TAX_SERVICE_INTRO		=	0x01,	// 介绍
		TAX_SERVICE_MODIFY		=	0x02,	// 设置
		TAX_SERVICE_GET			=	0x04,	// 获得
	};

	enum ELastOneType
	{
		LO_SERVICE_NONE			= 0,		//无
		LO_SERVICE_INTRO		= 0x01,		//介绍
	};
public:

	// FieldName: NPC类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(服务NPC=1,卫兵,任务NPC)
	int			mFuncType;	
	
	// FieldName: $开场白
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char		mTalk[ STRING256 ];

	// FieldName: 驿站ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mStageID;	

	// FieldName: 谈话服务ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mTalkServiceID;

	// FieldName: 出售服务ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mSellServiceID;

	// FieldName: 邮件服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mMailService;

	// FieldName: 仓库服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mStorageService;
	
	// FieldName: 升级服务
	// FieldType: INT4
	// Type:	  CHECKMUTI(否=0,本体,抗性,属性)
	int			mUpgService;
	
	// FieldName: 装备绑定服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mEqtBindService;		
	
	// FieldName: 每日福利服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mWelfareService;		

	// FieldName: $称号
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char		mDesc[ STRING32 ];

	// FieldName: 学习技能职业ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,6)
	int			mStudySkillMetierID;	

	// FieldName: 回城点X
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mTownX;

	// FieldName: 回城点Y
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mTownY;

	// FieldName: 皇榜任务NPC
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mStorageTaskNPC;

	// FieldName: NPC挂的皇榜任务ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,200)
	int			mStorageTaskID;

	// FieldName: 皇榜任务等级下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mDownLevel;

	// FieldName: 皇榜任务等级上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mUpLevel;	

	// FieldName: 洗点服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mResetPService;	

	// FieldName: 领取双倍时间列表ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mDiploidTime;
	
	// FieldName: 家族服务
	// FieldType: INT4
	// Type:	   COMBOENUM(EFamilyService) 
	int			mFamilyService;	

	// FieldName: 军团服务
	// FieldType: INT4
	// Type:	   COMBOENUM(EFamilyService)
	int			mCorpService;

	// FieldName: 鉴定服务
	// FieldType: INT4
	// Type:	  CHECKMUTI(否=0,首次,再次)
	int			mJudgeService;

	// FieldName: 转化服务
	// FieldType: INT4
	// Type:	  CHECKMUTI(否=0,抗性,属性)
	int			mChangeService;	

	// FieldName: 活动服务
	// FieldType: INT4
	// Type:	   COMBOSINGLE(否=0,是)
	int			mActivityService;	

	// FieldName: 兑换服务
	// FieldType: INT4
	// Type:	  CHECKMUTI(否=0,普通,套装)
	int			mExchangeService;

	// FieldName: 功勋服务
	// FieldType: INT4
	// Type:	  CHECKMUTI(否=0,经验,才华,物品)
	int			mHonorService;

	// FieldName: 战场服务ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mBattle;

	// FieldName: 生活技能服务
	// FieldType: INT4
	// Type:	   COMBOENUM(ELifeSkillService) 
	int			mLifeSkillService;	
	// FieldName: 竞价周期
	// FieldType: INT4
	// Type:	  CHECKMUTI(周一,周二,周三,周四,周五,周六,周日)
	int			mBidDay;

	// FieldName: 竞价时
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,23)
	int			mBidHour;

	// FieldName: 竞价分
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,59)
	int			mBidMinute;

	// FieldName: 军团战报名服务
	// FieldType: INT4
	// Type:    COMBOSINGLE(否=0,是=1)
	int			mCorpsBattle;

	// FieldName: 报名竞价加价最低值
	// FieldType: INT4
	// Type:	EDITNUMBER(0,100000000)
	int			mMinBidPrice;

	// FieldName: 军团战城池ID
	// FieldType: INT4
	// Type:	EDITNUMBER(0,100000000)
	int			mBattleCity;

	// FieldName: 法宝属性鉴定服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mMWProJudgeService;

	// FieldName: 法宝技能鉴定服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mMWSkillJudgeService;
	
	// FieldName: 装备兑换服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mEquipExchangeService;
	
	// FieldName: 装备兑换表ID
	// FieldType: INT4
	// Type:	EDITNUMBER(0,100000000)
	int			mExChangeTableID;	

	// FieldName: 军团战竞价周期
	// FieldType: INT4
	// Type:	  CHECKMUTI(周一,周二,周三,周四,周五,周六,周日)
	int			mCorpsBidDay;

	// FieldName: 军团战竞价时
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,23)
	int			mCorpsBidHour;

	// FieldName: 军团战竞价分
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,59)
	int			mCorpsBidMinute;

	// FieldName: 法宝洗点服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mMWClearPointService;

	// FieldName: 法宝二次鉴定
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mMWSecondJudgeService;

	// FieldName: 军团联盟服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mUnionService;

	// FieldName: 国籍服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mCountryService;

	// FieldName: 外交服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mFriendService;

	// FieldName: NPC国籍
	// FieldType: INT4
	// Type:	  COMBOSINGLE(长安=0,乌鸡=1,车迟=2,宝象=3,女儿=4)
	int			mCountryType;
	
	// FieldName: 金元宝领取服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mGoldenYBService;

	// FieldName: 掌门任务服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mChairManTaskService;

	// FieldName: 掌门任务ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,399)
	int			mChairManTaskID;
	
	// FieldName: 结婚服务
	// FieldType: INT4
	// Type:	 COMBOENUM(EMarriageType)
	int			mMarriageService;

	// FieldName: 仙缘服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mMatrixService;

	// FieldName: 家族NPC的类型
	// FieldType: INT4
	// Type:	   COMBOENUM(EFamilyHomeType) 
	int			mFamilyType;	

	// FieldName: 获取的灵石Buff
	// FieldType: INT4
	// Type:	EDITNUMBER(0,100000000)
	int			mStoneBuff;

	// FieldName: 获取后是否消失
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mDisappear;

	// FieldName: 刷属性服务
	// FieldType: INT4
	// Type:	   COMBOSINGLE(否=0,是)
	int			mRefreshService;

	// FieldName: 查询的传送点索引
	// FieldType: INT4
	// Type:	EDITNUMBER(0,100000000)
	int			mPedalIndex;

	// FieldName: 是否有摘星功能
	// FieldType: INT4
	// Type:	COMBOSINGLE(否=0,是)
	int			mDirectService;

	// FieldName: 是否有印鉴功能
	// FieldType: INT4
	// Type:	COMBOSINGLE(否=0,是)
	int			mExtraService;

	// FieldName: 是否有国战功能
	// FieldType: INT4
	// Type:	  CHECKMUTI(无=0,主要,营地,奖励,传送,领buff)
	int			mWarService;

	// FieldName: 国战属性
	// FieldType: INT4
	// Type:	  COMBOENUM(EWarOpType)
	int			mWarOpType;

	// FieldName: 国战操作值
	// FieldType: INT4
	// Type:	EDITNUMBER(0,100000000)
	int			mWarValue;

	// FieldName: 初始归属的阵营
	// FieldType: INT4
	// Type:	EDITNUMBER(0,100000000)
	int			mWarCamp;
	
	// FieldName: 是否有升级装备服务
	// FieldType: INT4
	// Type:	COMBOSINGLE(否=0,是)
	int			mEquipLevelService;

	// FieldName: 国战战功值
	// FieldType: INT4
	// Type:	EDITNUMBER(0,100000000)
	int			mWarHonor;

	// FieldName: 学习方寸山技能服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mFangCunService;

	// FieldName: 是否能抵抗变身
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int		mResistTran;

	// FieldName: 副本功能
	// FieldType: INT4
	// Type:    EDITNUMBER(0,100000000)
	int		mFBFunc;

	// FieldName: 副本功能参数1
	// FieldType: INT4
	// Type:    EDITNUMBER(0,100000000)
	int		mFBFuncParame1;

	// FieldName: 副本功能参数2
	// FieldType: INT4
	// Type:    EDITNUMBER(0,100000000)
	int		mFBFuncParame2;

	// FieldName: 副本功能提示字符串
	// FieldType: STRING128
	// Type:    EDITSTRING
	char	mFBFuncStr[ 128 ];

	// FieldName: 是否有收税功能
	// FieldType: INT4
	// Type:	  CHECKMUTI(无=0,介绍,设置,获得)
	int			mTaxService;

	// FieldName: 转阵营服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mChangeCampService;

	// FieldName: 弱国保护服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mWeakProtectService;

    // FieldName: 摇钱树排行榜
    // FieldType: INT4
    // Type:      COMBOSINGLE(否=0,是)
    int         mYaoQianShuRankService;

    // FieldName: 上缴服务
    // FieldType: INT4
    // Type:      COMBOSINGLE(否=0,是)
    int         mHandInService;

	// FieldName: 特殊事件类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无=0,城门=1,大旗=2)
	int			mSpecialEvent;

    // FieldName: 摇钱树说明
    // FieldType: INT4
    // Type:      EDITNUMBER(0,100000000)
    int         mYaoQianShuServiceInfo;
	
	// FieldName: 聚宝盆服务器
	// FieldType: INT4
	// Type:      COMBOSINGLE(否=0,是)
	int         mTreasureBowlService;

	// FieldName: 法宝分解服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mMWDecomposeService;

	// FieldName: 法宝器灵印记服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mMWImprintService;

	// FieldName: 法宝飞升服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mMWUpdateService;

	// FieldName: 法宝升品服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mMWUpdateExService;

	// FieldName: 军团战竞价开始周期
	// FieldType: INT4
	// Type:	  CHECKMUTI(周一,周二,周三,周四,周五,周六,周日)
	int			mCorpsBidStartDay;

	// FieldName: 军团战竞价开始时
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,23)
	int			mCorpsBidStartHour;

	// FieldName: 军团战竞价开始分
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,59)
	int			mCorpsBidStartMinute;
	
	// FieldName: 大逃杀说明
	// FieldType: INT4
	// Type:	  CHECKMUTI(无=0,介绍)
	int			mLastOneService;  

	// FieldName: 家族战竞价开始时
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,23)
	int			mFamilyBidStartHour;

	// FieldName: 家族战竞价开始分
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,59)
	int			mFamilyBidStartMinute;

	// FieldName: 法宝器灵合成服务
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mMWQLComposeService;
	
	// FieldName: 副本内特殊传送服务
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)
	int			mRandomTeleService;

	// FieldName: 地图说明
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mIntroduceMapID;

	// FieldName: 师徒说明
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mShiTuService;
	
	// FieldName: 结拜服务
	// FieldType: INT4
	// Type:	 COMBOENUM(ESwornType)
	int			mSwornService;

	// FieldName: 跨服战说明
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mGlobalWarService;

	// FieldName: 是否是神器兑换
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			mSuperEquipExchange;

public:
	void SetFromPB( bool bServer, PBNpc* pNpc );
	void CreateTlvPB( bool bServer, PBNpc* pNpc );
	int Initialize();
	CTemplateNpc()
	{
#ifndef TEMPEDIT
		mTemplateType = EM_TEMPTYPE_NPC; 
#endif
		Initialize();
	}
	~CTemplateNpc() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 矿				## 模版名称
// version	= 1					## 版本号
// path		= NPC/矿			## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateMine : public CTemplate
{
public:
	class CMineOutput
	{
	public:
		int		mStuffID;
		int	mPrecent;
	};

	class CMineOgre
	{
	public:
		int		mNpcID;		
		int		mNumber;
		int		mPrecent;
	};

public:

	// FieldName: $游戏中的显示名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char		mName[ STRING32 ];

	// FieldName: 等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int			mLevel;

	// FieldName: 等级限制
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int			mLevelRequire;

	// FieldName: 所需要的工具类型
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mToolID;

	// FieldName: 消耗采掘用具
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mDecRate;

	// FieldName: 采矿时间下限（秒）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int			mMineTimeLow;

	// FieldName: 采矿时间上限（秒）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int			mMineTimeHigh;

	// FieldName: 模型路径名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char		mModel[ STRING32 ];
	
	// FieldName: 产出的第1项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 产出的第1项原料概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	
	// FieldName: 产出的第2项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 产出的第2项原料概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 产出的第3项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 产出的第3项原料概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 产出的第4项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 产出的第4项原料概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 产出的第5项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 产出的第5项原料概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	
	// FieldName: 产出的第6项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 产出的第6项原料概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 产出的第7项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 产出的第7项原料概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 产出的第8项原料ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 产出的第8项原料概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	CMineOutput mOutput[ 8 ];

	// FieldName: 出现的第1项怪物ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 出现的第1项怪物数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 出现的第1项怪物概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 出现的第2项怪物ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 出现的第2项怪物数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 出现的第2项怪物概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 出现的第3项怪物ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 出现的第3项怪物数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 出现的第3项怪物概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 出现的第4项怪物ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 出现的第4项怪物数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 出现的第4项怪物概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	CMineOgre	mOgre[ 4 ];

public:
	void SetFromPB( bool bServer, PBMine* pMine );
	void CreateTlvPB( bool bServer, PBMine* pMine );
	int Initialize();
	CTemplateMine()
	{
		Initialize();
	}
	~CTemplateMine() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// CSkillTreeNode
// ********************************************************************** //

class CSkillTreeNode
{
public:
	int		mSkillID;				// 技能ID
	int		mMaxLevel;				// 最大等级( 最大技能点投入额度 )
	int		mNeedSkillID;			// 需要的前导技能
	int		mNeedSkillPoint;		// 需要的前导技能投入额度
	int		mNeedGenerID;			// 需要投入的系别ID
	int		mNeedGenerPoint;		// 需要投入的系别投入额度
	int		mLevel;					// 需要的玩家等级
	int		mNeedBook;				// 是否需要对应的技能书

public:
	void SetFromPB( bool bServer, PBSkillTreeNode* pNode );
	void CreateTlvPB( bool bServer, PBSkillTreeNode* pNode );
};

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 技能树			## 模版名称
// version	= 1					## 版本号
// path		= 技能/技能树		## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateSkillTree  : public CTemplate
{
public:
	enum
	{
		GENER_WARRIOR_NOVICE       = 1,
		GENER_WARRIOR_WEAPON       = 2,
		GENER_WARRIOR_RAGE       = 3,
		GENER_WARRIOR_DEFENCE       = 4,
		GENER_ROGUE_NOVICE       = 5,
		GENER_ROGUE_STING       = 6,
		GENER_ROGUE_COLD       = 7,
		GENER_ROGUE_BRAVO       = 8,
		GENER_HUNTER_NOVICE       = 9,
		GENER_HUNTER_SHOT       = 10,
		GENER_HUNTER_TRAP       = 11,
		GENER_HUNTER_MAGIC       = 12,
		GENER_SORCERER_NOVICE       = 13,
		GENER_SORCERER_WATER       = 14,
		GENER_SORCERER_FIRE       = 15,
		GENER_SORCERER_WIND       = 16,
		GENER_PRIEST_NOVICE       = 17,
		GENER_PRIEST_HOLY       = 18,
		GENER_PRIEST_SHADOW       = 19,
		GENER_PRIEST_MITZVAH       = 20,
		GENER_DRUID_NOVICE       = 21,
		GENER_DRUID_WRESTLE       = 22,
		GENER_DRUID_LION       = 23,
		GENER_DRUID_LEOPARD       = 24,
		GENER_NOVICE       = 25,
		GENER_TALENT	= 100,
	};

public:

	// FieldName: $系别名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char			mGenerName[ 32 ];		

	// FieldName: 系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 
	int				mGenerID;		

	// FieldName: 第1行第1列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第1行第1列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第1行第1列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第1行第1列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第1行第1列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第1行第1列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第1行第1列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第1行第1列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第2行第1列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第2行第1列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第2行第1列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第2行第1列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第2行第1列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第2行第1列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第2行第1列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第2行第1列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第3行第1列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第3行第1列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第3行第1列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第3行第1列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第3行第1列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第3行第1列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第3行第1列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第3行第1列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第4行第1列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第4行第1列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第4行第1列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第4行第1列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第4行第1列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第4行第1列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第4行第1列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第4行第1列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第5行第1列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第5行第1列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第5行第1列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第5行第1列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第5行第1列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第5行第1列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第5行第1列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第5行第1列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第6行第1列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第6行第1列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第6行第1列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第6行第1列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第6行第1列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 


	// FieldName: 第6行第1列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第6行第1列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第6行第1列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第7行第1列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第7行第1列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第7行第1列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第7行第1列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第7行第1列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第7行第1列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第7行第1列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第7行第1列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第8行第1列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第8行第1列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第8行第1列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第8行第1列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第8行第1列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第8行第1列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第8行第1列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第8行第1列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第9行第1列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第9行第1列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第9行第1列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第9行第1列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第9行第1列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第9行第1列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第9行第1列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第9行第1列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)
	
	// FieldName: 第10行第1列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第10行第1列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第10行第1列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第10行第1列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第10行第1列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第10行第1列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第10行第1列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第10行第1列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第1行第2列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第1行第2列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第1行第2列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第1行第2列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第1行第2列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第1行第2列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第1行第2列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第1行第2列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第2行第2列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第2行第2列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第2行第2列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第2行第2列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第2行第2列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第2行第2列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第2行第2列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第2行第2列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第3行第2列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第3行第2列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第3行第2列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第3行第2列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第3行第2列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第3行第2列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第3行第2列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第3行第2列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第4行第2列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第4行第2列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第4行第2列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第4行第2列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第4行第2列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第4行第2列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第4行第2列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第4行第2列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第5行第2列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第5行第2列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第5行第2列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第5行第2列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第5行第2列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第5行第2列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第5行第2列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第5行第2列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第6行第2列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第6行第2列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第6行第2列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第6行第2列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第6行第2列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第6行第2列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第6行第2列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第6行第2列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第7行第2列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第7行第2列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第7行第2列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第7行第2列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第7行第2列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第7行第2列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第7行第2列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第7行第2列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第8行第2列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第8行第2列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第8行第2列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第8行第2列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第8行第2列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第8行第2列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第8行第2列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第8行第2列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第9行第2列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第9行第2列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第9行第2列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第9行第2列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第9行第2列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第9行第2列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第9行第2列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第9行第2列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)
	
	// FieldName: 第10行第2列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第10行第2列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第10行第2列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第10行第2列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第10行第2列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第10行第2列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第10行第2列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第10行第2列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第1行第3列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第1行第3列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第1行第3列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第1行第3列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第1行第3列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第1行第3列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第1行第3列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第1行第3列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第2行第3列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第2行第3列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第2行第3列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第2行第3列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第2行第3列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第2行第3列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第2行第3列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第2行第3列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第3行第3列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第3行第3列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第3行第3列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第3行第3列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第3行第3列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第3行第3列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第3行第3列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第3行第3列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第4行第3列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第4行第3列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第4行第3列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第4行第3列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第4行第3列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第4行第3列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第4行第3列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第4行第3列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第5行第3列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第5行第3列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第5行第3列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第5行第3列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第5行第3列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第5行第3列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第5行第3列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第5行第3列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第6行第3列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第6行第3列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第6行第3列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第6行第3列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第6行第3列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第6行第3列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第6行第3列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第6行第3列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第7行第3列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第7行第3列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第7行第3列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第7行第3列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第7行第3列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第7行第3列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第7行第3列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第7行第3列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第8行第3列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第8行第3列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第8行第3列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第8行第3列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第8行第3列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第8行第3列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第8行第3列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第8行第3列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第9行第3列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第9行第3列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第9行第3列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第9行第3列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第9行第3列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第9行第3列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第9行第3列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第9行第3列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)
	
	// FieldName: 第10行第3列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第10行第3列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第10行第3列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第10行第3列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第10行第3列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第10行第3列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第10行第3列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第10行第3列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第1行第4列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第1行第4列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第1行第4列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第1行第4列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第1行第4列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第1行第4列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第1行第4列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第1行第4列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第2行第4列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第2行第4列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第2行第4列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第2行第4列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第2行第4列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 
	
	// FieldName: 第2行第4列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第2行第4列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第2行第4列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第3行第4列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第3行第4列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第3行第4列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第3行第4列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第3行第4列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第3行第4列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第3行第4列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第3行第4列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第4行第4列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第4行第4列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第4行第4列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第4行第4列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第4行第4列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第4行第4列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第4行第4列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第4行第4列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第5行第4列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第5行第4列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第5行第4列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第5行第4列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第5行第4列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第5行第4列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第5行第4列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第5行第4列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第6行第4列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第6行第4列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第6行第4列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第6行第4列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第6行第4列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第6行第4列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第6行第4列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第6行第4列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第7行第4列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第7行第4列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第7行第4列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第7行第4列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第7行第4列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 
	
	// FieldName: 第7行第4列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第7行第4列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第7行第4列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第8行第4列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第8行第4列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第8行第4列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第8行第4列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第8行第4列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第8行第4列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第8行第4列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第8行第4列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)

	// FieldName: 第9行第4列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第9行第4列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第9行第4列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第9行第4列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第9行第4列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第9行第4列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第9行第4列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第9行第4列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)
	
	// FieldName: 第10行第4列技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第10行第4列最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第10行第4列需要的前导技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第10行第4列需要的前导技能投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第10行第4列需要投入的系别ID
	// FieldType: INT4
	// Type:      COMBOENUM(METIER_GENERID) 

	// FieldName: 第10行第4列需要投入的系别投入额度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第10行第4列需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第10行第4列是否需要对应的技能书
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不需要技能书=0,需要技能书)
	CSkillTreeNode	mLayerNode[ 4 ][ 10 ];	

public:
	void SetFromPB( bool bServer, PBSkillTree* pSkillTree );
	void CreateTlvPB( bool bServer, PBSkillTree* pSkillTree );
	int Initialize();
	CTemplateSkillTree() 
	{
		Initialize();
	}
	~CTemplateSkillTree() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= true			## 代表不生成到数据库中
// name		= 技能基本信息	## 模版名称
// version	= 1				## 版本号
// path		=				## 路径
// parent	=				## 父亲模版
// ********************************************************************** //
class CTemplateSkill : public CTemplate
{
public:
	enum FIRE_TYPE
	{
		FIRETYPE_NORMAL		= 1,	// 普通施放形态
		FIRETYPE_LION,				// 狮形施放形态
		FIRETYPE_LEOPARD,			// 豹形施放形态
		FIRETYPE_ANY,				// 任意施放形态
	};

	enum FIRE_TIME_TYPE
	{
		TIMETYPE_NORMAL		= 1,	// 普通，需要吟唱
		TIMETYPE_INSTANT,			// 瞬发，不需要吟唱
	};

	enum PASSINC_TYPE
	{
		PASSINC_NONE				= 0,	// 无效果
		PASSINC_MAXHPFIX			= 1,	// 增加HP上限固定值
		PASSINC_MAXMPFIX			= 2,	// 增加MP上限固定值
		PASSINC_MAXHPPER			= 3,	// 增加HP上限百分比
		PASSINC_MAXMPPER			= 4,	// 增加MP上限百分比
		PASSINC_HPRSPEEDFIX			= 5,	// 增加HP恢复速度固定值
		PASSINC_MPRSPEEDFIX			= 6,	// 增加MP恢复速度固定值
		PASSINC_HPRSPEEDPER			= 7,	// 增加HP恢复速度百分比
		PASSINC_MPRSPEEDPER			= 8,	// 增加MP恢复速度百分比
		PASSINC_STR					= 9,	// 增加力量
		PASSINC_STA					= 10,	// 增加耐力
		PASSINC_SPR					= 11,	// 增加精神
		PASSINC_CON					= 12,	// 增加体质
		PASSINC_WIS					= 13,	// 增加智慧
		PASSINC_SPEED				= 14,	// 增加移动速度
		PASSINC_PDEFENCEFIX			= 15,	// 增加物理防御固定值
		PASSINC_MDEFENCEFIX			= 16,	// 增加法术防御固定值
		PASSINC_PDEFENCEPER			= 17,	// 增加物理防御百分比
		PASSINC_MDEFENCEPER			= 18,	// 增加法术防御百分比
		PASSINC_NORMALHITRATE		= 19,	// 增加普通攻击命中固定值
		PASSINC_NORMALDUCK			= 20,	// 增加普通攻击闪避固定值
		PASSINC_SKILLDUCK			= 21,	// 增加技能闪避固定值
		PASSINC_DEATHRATE			= 22,	// 增加致命一击等级
		PASSINC_DEATHVALUE			= 23,	// 增加致命一击伤害加成
		PASSINC_DAMAGEFIX			= 24,	// 增加伤害固定值
		PASSINC_NEEDMPFIX			= 25,	// 减少技能消耗MP固定值
		PASSINC_RELIVE				= 26,	// 被动复活
		PASSINC_DAMAGELESSFIX		= 27,	// 增加伤害减免固定值
		PASSINC_DAMAGELESSPER		= 28,	// 增加伤害减免百分比
		PASSINC_PATTACKFIX			= 29,	// 增加物理攻击固定值
		PASSINC_MATTACKFIX			= 30,	// 增加法术攻击固定值
		PASSINC_NEEDMPPER			= 31,	// 减少技能消耗MP百分比

		PASSINC_IGNOREDUCK			= 32,	// 增加忽视闪避固定值
		PASSINC_DEATHDEFENSE		= 33,	// 增加致命一击防御
		PASSINC_ALL					= 34,	// 增加全属性
		PASSINC_REDUCECOOLFIX		= 35,	// 使某个技能冷却时间减少固定值
		PASSINC_DEATHRATEFIX		= 36,	// 增加某个技能致命等级
		PASSINC_DEATHVALUEFIX		= 37,	// 增加某个技能暴击伤害百分比
		PASSINC_HITSKILL			= 38,	// 击中时使用技能
		PASSINC_BEHITSKILL			= 39,	// 被击中时使用技能
		PASSINC_DEATHSKILL			= 40,	// 暴击时使用技能
		PASSINC_HPSKILL				= 41,	// 减血时使用技能
		PASSINC_KEENPOINTS			= 42,	// 增加法宝的锐力 
		PASSINC_CALMPOINTS			= 43,	// 增加法宝的定力 
		PASSINC_MAGICPOINTS			= 44,	// 增加法宝的法力 
		PASSINC_BIERPOINTS			= 45,	// 增加法宝的灵力 
		PASSINC_IMMORTALPOINTS		= 46,	// 增加法宝的仙力 
		PASSINC_MWHPMAX				= 47,   // 增加法宝的灵气值上限
		PASSINC_MWMPMAX				= 48,   // 增加法宝的法力值上限
		PASSINC_DUCKRATE			= 49,	// 增加闪避率
		PASSINC_RESDEF				= 50,   // 增加抗性
		PASSINC_RESATT				= 51,   // 增加属性
	};
	
	enum COST_TYPE
	{
		COST_NONE		= 0,					// 无消耗
		COST_HP			= 1,					// 消耗HP
		COST_MP			= 2,					// 消耗MP
		COST_ITEM		= 3,					// 消耗道具
		COST_AP			= 4,					// 消耗AP
	};

	enum SKILL_TYPE
	{
		SKILLTYPE_ATTACK       = 1,				// 攻击类技能
		SKILLTYPE_BLESS       = 2,				// 祝福类技能
		SKILLTYPE_CUSS       = 3,				// 诅咒类技能
		SKILLTYPE_PETBLESS       = 4,			// 宠物祝福类技能
		SKILLTYPE_SELFBLESS       = 5,			// 自身祝福类技能
		SKILLTYPE_TEAMBLESS       = 6,			// 小队祝福类技能
		SKILLTYPE_PASSIVITY       = 7,			// 被动类技能
		SKILLTYPE_RELIVE       = 8,				// 复活类技能
		SKILLTYPE_TELEPORT       = 9,			// 传送类技能
		SKILLTYPE_TRIP       = 10,				// 陷阱类技能
		SKILLTYPE_SUMMON       = 11,			// 召唤类技能	
		SKILLTYPE_AUREOLE       = 12,			// 光环类技能
		SKILLTYPE_NORMALATTACK       = 13,		// 普通攻击类技能
		SKILLTYPE_PETTELEPORT       = 14,		// 召唤兽传送类技能
		SKILLTYPE_SELFRANDTELEPORT       = 15,	// 自身随机传送
		SKILLTYPE_RANDTELEPORT       = 16,		// 使他人随机传送
		SKILLTYPE_CONTINUEDATTACK       = 17,	// 持续攻击类技能
		SKILLTYPE_CONTINUEDBLESS       = 18,	// 持续祝福类技能
		SKILLTYPE_CONTINUEDCUSS       = 19,		// 持续诅咒类技能
		SKILLTYPE_TRANSFORM			  = 20,		// 变身技能
		SKILLTYPE_1PFRIENDINBLESS	= 21,		// 一屏范围内友方祝福(填自身BUFF组)
		SKILLTYPE_SUMMONNPC			= 22,		// 召唤固定点NPC
	};

	enum SKILL_USECONDITION
	{
		SKILL_USECONDITION_NONE		= 0,	// 无条件
		SKILL_USECONDITION_HPPER	= 1,			// HP不低于多少百分比
		SKILL_USECONDITION_MPPER	= 2,			// MP不低于多少百分比
		SKILL_USECONDITION_ITEM		= 3,			// 包裹中必须有某个物品
	};

	enum SKILLLIST_SHOWTYPE
	{
		SKILLlIST_SHOWTYPE_SINGLE		= 0,	// 单体攻击
		SKILLLIST_SHOWTYPE_UNSINGLE		= 1,			// 群体攻击
		SKILLLIST_SHOWTYPE_ASSISTANT	= 2,			// 辅助类
		SKILLLIST_SHOWTYPE_PASSIVITY	= 3,			// 被动类
		SKILLLIST_SHOWTYPE_NOTSHOW		= 4,			// 不显示
	};

	enum SKILL_TEMPLATE_TYPE
	{
		SKILL_TEMPLATE_TYPE_AUREOLE		= 0,	// 光环技能
		SKILL_TEMPLATE_TYPE_NORMAL		= 1,				// 普通技能
		SKILL_TEMPLATE_TYPE_TRIP		= 2,				// 陷阱技能
		SKILL_TEMPLATE_TYPE_SUMMONBEAST	= 3,		// 召唤兽技能
	};

	class CCost
	{
	public:
		int		mCostType;
		int		mCostParam[ 2 ];
	};

	class CUseCondition
	{
	public:
		SKILL_USECONDITION mConditionType;
		int mParam;								// SKILL_USECONDITION_HPPER		HP万分比
												// SKILL_USECONDITION_MPPER		MP万分比
												// SKILL_USECONDITION_ITEM		ITEM模板ID
	};
public:

	// FieldName: $技能名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mName[ STRING32 ];	

	// FieldName: 技能ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,65535)
	int		mSkillID;				

	// FieldName: 技能需要的武器类别
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无限制=0,枪,双刀,弓,法盾,杖,钺)
	CTemplateWeapon::WEAPON_TYPE		mRequireWeapon;			

	// FieldName: 技能所属职业
	// FieldType: INT4
	// Type:	  COMBOENUM(METIER_TYPE)
	int		mMetierID;			

	// FieldName: 技能所属职业系别
	// FieldType: INT4
	// Type:	  COMBOENUM(METIER_GENERID) 
	int		mGenerID;		

	// FieldName: 技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50000000)
	unsigned int mSkillLevel;		
	
	// FieldName: 技能图标
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mSkillIcon[ STRING32 ];	

	// FieldName: $技能说明
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char	mSkillDesc[ STRING256 ];		

	// FieldName: 施放形态
	// FieldType: INT4
	// Type:	  COMBOSINGLE(普通,豹形,狮形,任意)
	FIRE_TYPE		mFireType;

	// FieldName: 技能类型
	// FieldType: INT4
	// Type:	  COMBOENUM(SKILL_TYPE)
	SKILL_TYPE		mSkillType;

	// FieldName: 时间类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(普通,瞬发)
	FIRE_TIME_TYPE		mTimeType;

	// FieldName: 施放时间即动画时间(毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mAniTime;

	// FieldName: 吟唱时间(毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mFireTime;

	// FieldName: 冷却时间(毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int		mCDTime;
	
	// FieldName: 第一项消耗类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无消耗=0,生命,法力,道具,活力)

	// FieldName: 第一项消耗数值1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第一项消耗数值2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第二项消耗类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无消耗=0,生命,法力,道具,活力)

	// FieldName: 第二项消耗数值1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第二项消耗数值2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第三项消耗类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无消耗=0,生命,法力,道具,活力)

	// FieldName: 第三项消耗数值1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第三项消耗数值2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第四项消耗类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无消耗=0,生命,法力,道具,活力)

	// FieldName: 第四项消耗数值1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第四项消耗数值2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	CCost	mCost[ 4 ];

	// FieldName: 对应第1项被动效果ID
	// FieldType: INT4
	// Type:	   COMBOENUM(PASSINC_TYPE)
	PASSINC_TYPE  mSkillFuncID1;

	// FieldName: 技能是否需要延迟计算伤害
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int		mNeedDelay;

	// FieldName: 第1项效果ID对应数值1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第1项效果ID对应数值2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第1项效果ID对应数值3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第1项效果ID对应数值4
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第1项效果ID对应数值5
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	unsigned int  mParameter1[ 5 ];

	// FieldName: 对应第2项被动效果ID
	// FieldType: INT4
	// Type:	  COMBOENUM(PASSINC_TYPE)
	PASSINC_TYPE  mSkillFuncID2;
	
	// FieldName: 第2项效果ID对应数值1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第2项效果ID对应数值2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第2项效果ID对应数值3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第2项效果ID对应数值4
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第2项效果ID对应数值5
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	unsigned int  mParameter2[ 5 ];

	// FieldName: 对应第3项被动效果ID
	// FieldType: INT4
	// Type:	  COMBOENUM(PASSINC_TYPE)
	PASSINC_TYPE  mSkillFuncID3;

	// FieldName: 第3项效果ID对应数值1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第3项效果ID对应数值2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第3项效果ID对应数值3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第3项效果ID对应数值4
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第3项效果ID对应数值5
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	unsigned int  mParameter3[ 5 ];

	// FieldName: 对应第4项被动效果ID
	// FieldType: INT4
	// Type:	  COMBOENUM(PASSINC_TYPE)
	PASSINC_TYPE  mSkillFuncID4;

	// FieldName: 第4项效果ID对应数值1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第4项效果ID对应数值2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第4项效果ID对应数值3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第4项效果ID对应数值4
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 第4项效果ID对应数值5
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	unsigned int  mParameter4[ 5 ];
	
	// FieldName: 是否自动重复
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不可重复=0,可以重复)
	int		mAutoRepeat;	

	// FieldName: 技能动画
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mSkillAction[ STRING32 ];

	// FieldName: 是否冷却所有技能
	// FieldType: INT4
	// Type:	  COMBOSINGLE(是=0,否)
	unsigned int  mAutoCoolingSkill;

	// FieldName: 技能使用特殊条件类型1
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无条件=0,HP百分比不低于,MP百分比不低于,包裹中携带物品)

	// FieldName: 条件参数1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)

	// FieldName: 技能使用特殊条件类型2
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无条件=0,HP百分比不低于,MP百分比不低于,包裹中携带物品)

	// FieldName: 条件参数2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)

	// FieldName: 技能使用特殊条件类型3
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无条件=0,HP百分比不低于,MP百分比不低于,包裹中携带物品)

	// FieldName: 条件参数3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	CUseCondition	mUseCondition[ 3 ];

	// FieldName: 学习技能所需金币
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int				mStudyGold;

	// FieldName: 是否自动普通攻击
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不可普攻=0,可以普攻)
	int		mAutoNormalAttack;
	
	// FieldName: 学习技能所需金币是否支持绑定金
	// FieldType: INT4
	// Type:	  COMBOSINGLE(是=0,否=1)
	int				mBindStudyGold;

	// FieldName: 技能列表显示类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(单体攻击=0,群体攻击,辅助类,被动类,不显示)
	SKILLLIST_SHOWTYPE mSkillListShowType;
	
	// FieldName: 技能模板类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(光环技能=0,普通技能,陷阱技能,召唤兽技能)
	SKILL_TEMPLATE_TYPE mSkillTemplateType;

	// FieldName: 学习技能所需人物等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,160)
	int					mStudySkillLevel;

	// FieldName: 是否进入自动重复攻击状态
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int					mInsertAutoState;

	// FieldName: 是否打断自动重复攻击状态
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int					mBreakAutoState;

	// FieldName: 是否需要攻击目标
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是,特殊)
	int					mNeedTarget;

	// FieldName: 是否需要召唤兽
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int					mNeedPet;

	// FieldName: 可施放阵营
	// FieldType: INT4
	// Type:	  COMBOSINGLE(友方=0,敌方)
	int					mActionCamp;

	// FieldName: 伤害类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(物理攻击=1,法术攻击=2,通用攻击=3)
	int			  mDamageType;

	// FieldName: 技能攻击力固定值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mAttackFix;			

	// FieldName: 技能攻击力百分比
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int	mAttackPer;	

	// FieldName: 属性攻击类别
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int		mResType;

	// FieldName: 属性攻击数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mResValue;

	// FieldName: $技能类型描述
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mSkillTypeDesc[ STRING32 ];

	// FieldName: 是否法宝技能
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			 mIsMagicWeaponSkill;

	// FieldName: 法宝是否偏移位置
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			 mIsMWOffset;

	// FieldName: 冷却组第%d项
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mCDGroup[ 25 ];

	// FieldName: 是否不可以被洗
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int  mCanClean;

	// FieldName: 施放目标等级限制
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int			mTargetLevel;

	// FieldName: 召唤的NPC索引
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int			mSummonIndex;

public:
	void SetFromPB( bool bServer, PBSkill* pSkill );
	void CreateTlvPB( bool bServer, PBSkill* pSkill );
	int Initialize();
	CTemplateSkill()
	{
		Initialize();
	}
	~CTemplateSkill() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //



// ********************************************************************** //
// BEGINMAKE
// virtual  = false       
// name     = 光环技能 
// version  = 1             
// path     = 技能/光环技能
// parent   = 技能基本信息
// ********************************************************************** //
class CTemplateAureoleSkill : public CTemplateSkill
{
public:
	enum AUREOLE_BOOL
	{
		emTRUE	=	0, //是
		emFLASE = 1,			//否
	};

	enum AUREOLE_AREA
	{
		AUREOLE_NONE	= 0,	//无
		AUREOLE_SELF	= 1,			//自己
		AUREOLE_TEAM	= 2,			//小队
		AUREOLE_FOE		= 3,			//敌对
		AUREOLE_CORPS	= 4,			//军团
		AUREOLE_ALL		= 5,			//全部
	};

	struct AUREOLE_TYPE
	{
		AUREOLE_AREA	mAureoleArea;
		int				mBuffID;	
	};

public:
    // FieldName: 是否持续消耗
    // FieldType: INT4
    // Type:      COMBOENUM(AUREOLE_BOOL) 
	AUREOLE_BOOL  mIncessary;

    // FieldName: 持续消耗时间间隔
    // FieldType: INT4
    // Type:      EDITNUMBER(1000,100000)
	int			  mIncessaryTime;

    // FieldName: 持续消耗第%d项类型 
    // FieldType: INT4
    // Type:      COMBOENUM(COST_TYPE) 

    // FieldName: 持续消耗第%d项数值1
    // FieldType: INT4
    // Type:      EDITNUMBER(0,100000) 

    // FieldName: 持续消耗第%d项数值2
    // FieldType: INT4
    // Type:      EDITNUMBER(0,100000)
    CCost   mCost[ 3 ];

	// FieldName: 影响范围
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			  mAureoleDistance;
	
	// FieldName: 光环技能第%d项功能类型
	// FieldType: INT4
	// Type:	  COMBOENUM(AUREOLE_AREA)

	// FieldName: 光环技能第%d项功能对应BuffID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	AUREOLE_TYPE  mAureoleType[ 3 ];

public:
	void SetFromPB( bool bServer, PBAureoleSkill* pAureoleSkill );
	void CreateTlvPB( bool bServer, PBAureoleSkill* pAureoleSkill );
	int Initialize();
	CTemplateAureoleSkill()
	{
		Initialize();
	}
	~CTemplateAureoleSkill() {}

};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 普通技能		## 模版名称
// version	= 1				## 版本号
// path		= 技能/普通技能	## 路径
// parent	= 技能基本信息	## 父亲模版
// ********************************************************************** //
class CTemplateNormalSkill : public CTemplateSkill
{
public:
	enum RANGE_TYPE
	{
		RANGE_SINGLE	= 1,// 对单体使用
		RANGE_FRONTRECT	= 2,	// 对自己面前的矩形使用
		RANGE_FRONTANGLE = 3,	// 对自己面前的三角使用
		RANGE_SELFCIRCLE = 4,	// 对以自己为中心的圆形使用
		RANGE_TARGETCIRCLE = 5,	// 对以选择目标为中心的圆形使用
		RANGE_BOUND		= 6,		// 弹跳攻击
		RANGE_PLAYER	=7,		//只能对玩家使用
	};

public:
	// FieldName: 技能准确度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mSkillHitrate;				

	// FieldName: 开始吟唱距离
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mMinDistance;	

	// FieldName: 追击距离
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mMaxDistance;		

	// FieldName: 最大作用数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mEffectMaxNumber;

	// FieldName: 攻击范围类型
	// FieldType: INT4
	// Type:	  COMBOENUM(RANGE_TYPE) 
	RANGE_TYPE	mAttackRange;	

	// FieldName: 范围攻击第1参数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	
	
    // FieldName: 范围攻击第2参数 
    // FieldType: INT4
    // Type:      EDITNUMBER(0,100000)
	int		mRandParame[ 2 ];	

	// FieldName: 对应目标第1项Buff组
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 对应目标第2项Buff组
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 对应目标第3项Buff组
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 对应目标第4项Buff组
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 对应自身Buff组
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 对应小队Buff组
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mBuffGroupID[ 6 ];	

	// FieldName: 技能打断条件
	// FieldType: INT4
	// Type:	  CHECKMUTI(不被打断=0,移动打断,中心点改变打断)
	int		mContinuedRule;
	
	// FieldName: 持续攻击间隔时间
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,60000000)
	int		mContinuedInterval;

	// FieldName: 持续次数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)
	int		mContinuedNum;

	// FieldName: 忽略护甲值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)
	int		mIgnoreArmor;
	
public:
	void SetFromPB( bool bServer, PBNormalSkill* pNormalSkill );
	void CreateTlvPB( bool bServer, PBNormalSkill* pNormalSkill );
	int Initialize();
	CTemplateNormalSkill()
	{
		Initialize();
	}
	~CTemplateNormalSkill() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 陷阱技能		## 模版名称
// version	= 1				## 版本号
// path		= 技能/陷阱技能	## 路径
// parent	= 技能基本信息	## 父亲模版
// ********************************************************************** //
class CTemplateTrapSkill : public CTemplateSkill
{
public:
	enum APPLY_TYPE
	{
		APPLYTYPE_SINGLE	 = 1,	// 单体作用
		APPLYTYPE_RANGE,			// 范围作用
	};

public:
	// FieldName: 持续时间（单位毫秒）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int				mLastTime;
 
	// FieldName: 作用类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(单体作用,范围作用)
	APPLY_TYPE		mApplyType;

	// FieldName: 触发范围（半径）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mTouchRange;

	// FieldName: 作用范围（半径）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mApplyRange;
	
	// FieldName: 陷阱资源
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mTrapRes[ 32 ];

	// FieldName: 对应第1项Buff组
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 对应第2项Buff组
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 对应第3项Buff组
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 对应第4项Buff组
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mBuffGroupID[ 4 ];	

	// FieldName: 对应第1项激活条件
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 对应第2项激活条件
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 对应第3项激活条件
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 对应第4项激活条件
	// FieldType: INT4
	// Type:	  SKILLPATH
	int		mCondition[ 4 ];

	// FieldName: 最大作用人数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mEffectMaxNumber;
public:
	void SetFromPB( bool bServer, PBTrapSkill* pTrapSkill );
	void CreateTlvPB( bool bServer, PBTrapSkill* pTrapSkill );
	int Initialize();
	CTemplateTrapSkill()
	{
		Initialize();
	}
	~CTemplateTrapSkill() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

class CTemplateSummonSkill : public CTemplateSkill
{
};

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= Buff				## 模版名称
// version	= 1					## 版本号
// path		= 技能/Buff			## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateBuff : public CTemplate
{
public:
	enum BUFF_TYPE
	{
		BUFF_INC		= 1,		// 正面BUFF
		BUFF_DEC		= 2,					// 负面BUFF
	};

	enum
	{
		RESIST_WIND		= 1,		// 怒风系
		RESIST_FIRE		= 2,				// 炽焰系
		RESIST_WATER	= 3,				// 玄冰系
		RESIST_LIGHT	= 4,				// 纯阳系
		RESIST_EARTH	= 5,				// 凝尘系
		RESIST_SHADOW	= 6,				// 寒阴系
		RESIST_BASE		= 7,				// 基础
	};

	enum BUFF_FUN_TYPE
	{
		BUFFINC_NONE				= 0,	// 无功能
		BUFFINC_PATTACKPER			= 1,	// 增加物理攻击百分比
		BUFFINC_MATTACKPER			= 2,	// 增加法术攻击百分比
		BUFFINC_PATTACKFIX			= 3,	// 增加物理攻击固定值
		BUFFINC_MATTACKFIX			= 4,	// 增加法术攻击固定值
		BUFFINC_PDEFENCEPER			= 5,	// 增加物理防御百分比
		BUFFINC_MDEFENCEPER			= 6,	// 增加法术防御百分比
		BUFFINC_PDEFENCEFIX			= 7,	// 增加物理防御固定值
		BUFFINC_MDEFENCEFIX			= 8,	// 增加法术防御固定值
		BUFFINC_MAXHPFIX			= 9,	// 增加HP上限固定值
		BUFFINC_MAXMPFIX			= 10,	// 增加MP上限固定值
		BUFFINC_MAXHPPER			= 11,	// 增加HP上限百分比
		BUFFINC_MAXMPPER			= 12,	// 增加MP上限百分比
		BUFFINC_INCSPEEDPER			= 13,	// 增加移动速度固定值
		BUFFINC_STRFIX				= 14,	// 增加力量固定值
		BUFFINC_CONFIX				= 15,	// 增加体质固定值
		BUFFINC_STAFIX				= 16,	// 增加耐力固定值
		BUFFINC_SPRFIX				= 17,	// 增加精神固定值
		BUFFINC_WISFIX				= 18,	// 增加智力固定值
		BUFFINC_NORMALHITRATEFIX	= 19,	// 增加普攻命中固定值
		BUFFINC_SKILLHITRATEFIX		= 20,	// 增加技能命中固定值
		BUFFINC_DEATHHITFIX			= 21,	// 增加致命等级固定值
		BUFFINC_DEATHRATEFIX		= 22,	// 增加致命伤害加成固定值
		BUFFINC_NORMALDUCKFIX		= 23,	// 增加普攻闪躲固定值
		BUFFINC_SKILLDUCKFIX		= 24,	// 增加技能闪躲固定值
		BUFFINC_INCSRCDAMAGEFIX		= 25,	// 增加攻击时造成伤害固定值
		BUFFINC_INCSRCDAMAGEPER		= 26,	// 增加攻击时造成伤害百分比
		BUFFINC_DECSRCDAMAGEFIX		= 27,	// 降低攻击时造成伤害固定值
		BUFFINC_DECSRCDAMAGEPER		= 28,	// 降低攻击时造成伤害百分比
		BUFFINC_INCDESPDAMAGEFIX	= 29,	// 增加被攻击时所受物理伤害固定值
		BUFFINC_INCDESPDAMAGEPER	= 30,	// 增加被攻击时所受物理伤害百分比
		BUFFINC_DECDESPDAMAGEFIX	= 31,	// 降低被攻击时所受物理伤害固定值
		BUFFINC_DECDESPDAMAGEPER	= 32,	// 降低被攻击时所受物理伤害百分比
		BUFFINC_INCDESMDAMAGEFIX	= 33,	// 增加被攻击时所受法术伤害固定值
		BUFFINC_INCDESMDAMAGEPER	= 34,	// 增加被攻击时所受法术伤害百分比
		BUFFINC_DECDESMDAMAGEFIX	= 35,	// 降低被攻击时所受法术伤害固定值
		BUFFINC_DECDESMDAMAGEPER	= 36,	// 降低被攻击时所受法术伤害百分比
		BUFFINC_DECMAXHPFIX			= 37,	// 降低HP上限固定值
		BUFFINC_DECMAXHPPER			= 38,	// 降低HP上限百分比
		BUFFINC_HPRSPEEDFIX			= 39,	// 增加HP恢复速度固定值
		BUFFINC_MPRSPEEDFIX			= 40,	// 增加MP恢复速度固定值
		BUFFINC_HPRSPEEDPER			= 41,	// 增加HP恢复速度百分比
		BUFFINC_MPRSPEEDPER			= 42,	// 增加MP恢复速度百分比
		BUFFINC_DECSPEEDPER			= 43,	// 降低速度百分比
		BUFFINC_DECPATTACKFIX		= 44,	// 降低物理攻击固定值
		BUFFINC_DECMATTACKFIX		= 45,	// 降低法术攻击固定值
		BUFFINC_DECPDEFENCEFIX		= 46,	// 降低物理防御固定值
		BUFFINC_DECMDEFENCEFIX		= 47,	// 降低法术防御固定值
		BUFFINC_DECPATTACKPER		= 48,	// 降低物理攻击百分比
		BUFFINC_DECMATTACKPER		= 49,	// 降低法术攻击百分比
		BUFFINC_DECPDEFENCEPER		= 50,	// 降低物理防御百分比
		BUFFINC_DECMDEFENCEPER		= 51,	// 降低法术防御百分比
		BUFFFUNC_PGENERATTACKFIX	= 52,	// 增加指定系别攻击固定值
		BUFFFUNC_NEXTDEATHHITFIX	= 53,	// 增加下次攻击致命等级固定值
		BUFFFUNC_NEXTDEATHRATEFIX	= 54,	// 增加下次攻击致命伤害加成固定值
		BUFFFUNC_PERSISTINCFIXHP	= 55,	// 持续回血
		BUFFFUNC_PERSISTINCFIXMP	= 56,	// 持续回蓝
		BUFFFUNC_PERSISTDECFIXHP	= 57,	// 持续伤害
		BUFFFUNC_PERSISTDECFIXMP	= 58,	// 持续减蓝
		BUFFFUNC_INCFIXHP			= 59,	// 医疗HP固定值
		BUFFFUNC_INCPERHP			= 60,	// 医疗HP百分比
		BUFFFUNC_INCFIXMP			= 61,	// 恢复MP固定值
		BUFFFUNC_INCPERMP			= 62,	// 恢复MP百分比
		BUFFFUNC_DECFIXHP			= 63,	// 减少HP固定值
		BUFFFUNC_DECPERHP			= 64,	// 减少HP百分比
		BUFFFUNC_DECFIXMP			= 65,	// 减少MP固定值
		BUFFFUNC_DECPERMP			= 66,	// 减少MP百分比
		BUFFFUNC_PERSISTINCFIXEXP	= 67,	// 持续获得经验值
		BUFFFUNC_INCHONORPER		= 68,	// 增加功勋获得百分比
		BUFFFUNC_PROTECTED			= 69,	// 守护
		BUFFFUNC_TELEPORT			= 70,	// 瞬移
		BUFFFUNC_RELIVE				= 71,	// 复活
		BUFFFUNC_HITBACK			= 72,	// 击退
		BUFFFUNC_MAGICSHIELD		= 73,	// 魔法盾
		BUFFFUNC_IMMUNITYDAMAGE		= 74,	// 免疫伤害
		BUFFFUNC_REBOUNDDAMAGE		= 75,	// 反弹伤害
		BUFFFUNC_OBLITERATEDAMAGE	= 76,	// 受到伤害删除Buff
		BUFFFUNC_UNMOVE				= 77,	// 无法移动
		BUFFFUNC_UNUSEITEM			= 78,	// 无法吃药
		BUFFFUNC_UNATTACK			= 79,	// 无法攻击
		BUFFFUNC_RANDMOVE			= 80,	// 随即走动
		BUFFFUNC_REMOVEREVERSDBUFF	= 81,	// 清除反面Buff
		BUFFFUNC_IMMUNITYREVERSD	= 82,	// 免疫负面Buff
		BUFFFUNC_SNEER				= 83,	// 嘲讽
		BUFFFUNC_HITDEATH			= 84,	// 致死
		BUFFFUNC_IGNORECLOTH		= 85,	// 忽略衣服的属性计算
		BUFFFUNC_UNARM				= 86,	// 忽略武器的属性计算
		BUFFFUNC_IMBIBITIONMP		= 87,	// 吸取MP固定值
		BUFFFUNC_SUICIDE			= 88,	// 减血换伤
		BUFFFUNC_STEALHP			= 89,	// 按伤害百分比偷HP
		BUFFFUNC_STEALMP			= 90,	// 按伤害百分比偷MP
		BUFFFUNC_INCGENERDAMAGEFIX	= 91,	// 受某系技能攻击则增加受到的伤害固定值
		BUFFFUNC_INCGENERDAMAGEPER	= 92,	// 受某系技能攻击则增加受到的伤害百分比
		BUFFFUNC_CHARGE				= 93,  // 冲锋
		BUFFFUNC_ATTACKEFFECT		= 94,	// 增加下N次攻击时附带特殊效果
		BUFFFUNC_HPBACKEFFECT		= 95,  // 生命恢复效果降低百分比
		BUFFFUNC_LEAVEAREA			= 96,	// 离开中debuff时所处范围会受到特殊效果
		BUFFFUNC_SUMMON				= 97,	// 拉人
		BUFFFUNC_MAGICFIRE			= 98,	// 法力燃烧
		BUFFFUNC_DAMAGELESS			= 99,	// 抵消N次伤害
		BUFFFUNC_KILLERRENEW		= 100,	// 杀怪取血
		BUFFFUNC_ENDEVENT			= 101,	// Buff消失时候特殊效果
		BUFFFUNC_CHANGEPATH			= 102,	// 交换位置
		BUFFFUNC_EXCHANGE			= 103,	// 换血
		BUFFFUNC_DAMAGEFORMP		= 104,	// 按所缺失的法力值的百分比掉血
		BUFFFUNC_DAMAGEDEVENT		= 105,	// 受到伤害时令目标产生特殊效果
		BUFFFUNC_CIRCUMGYRATE		= 106,	// 旋灯
		BUFFFUNC_CUREDAMAGE			= 107,	// 治疗目标并范围伤害目标周围
		BUFFFUNC_FIREEVENT			= 108,	// 成功放技能会受到特殊效果
		BUFFFUNC_TREE				= 109,	// 周围长树
		BUFFFUNC_ATTACKEVENT		= 110,	// 攻击附带效果
		BUFFFUNC_FIREWALL			= 111,	// 火墙
		BUFFFUNC_INCEXP				= 112,	// 增加经验百分比
		BUFFFUNC_HITDEATHHP			= 113,	// 致死对N点包括N生命值以下的目标致死对N以上的造成M点伤害
		BUFFFUNC_DECALLDAMAGEPER	= 114,	// 减少所有受到伤害的百分比包括BUFF
		BUFFFUNC_DAMAGEDEVENTSELF	= 115,	// 受到伤害令自己产生特殊效果
		BUFFFUNC_CLEARBUFF			= 116,	// 清除正面BUFF
		BUFFFUNC_CLEARDEBUFF		= 117,	// 清除负面BUFF
		BUFFFUNC_SAVEDAMAGE			= 118,	// 吸收伤害并在BUFF结束后将吸收的伤害返回
		BUFFFUNC_RANGEDAMAGE		= 119,	// 范围固定伤害
		BUFFFUNC_TRANSFORM			= 120,	// 变身BUFF
		BUFFFUNC_PERSISTINCPERHP	= 121,	// 持续按MaxHP百分比恢复CurHP
		BUFFFUNC_PERSISTINCPERMP	= 122,	// 持续按MaxMP百分比恢复CurMP

		BUFFINC_STRPER				= 123,	// 增加力量百分比
		BUFFINC_CONPER				= 124,	// 增加体质百分比
		BUFFINC_STAPER				= 125,	// 增加耐力百分比
		BUFFINC_SPRPER				= 126,	// 增加精神百分比
		BUFFINC_WISPER				= 127,	// 增加智力百分比
		BUFFINC_IGNOREDUCK			= 128,	// 增加忽视防御
		BUFFINC_DEATHDEFENSE		= 129,	// 增加致命防御
		BUFFINC_RESDEF1				= 130,	// 增加抗性1
		BUFFINC_RESDEF2				= 131,	// 增加抗性2
		BUFFINC_RESDEF3				= 132,	// 增加抗性3
		BUFFINC_RESDEF4				= 133,	// 增加抗性4
		BUFFINC_RESDEF5				= 134,	// 增加抗性5
		BUFFINC_RESDEF6				= 135,	// 增加抗性6
		BUFFINC_RESATT1				= 136,	// 增加属性1
		BUFFINC_RESATT2				= 137,	// 增加属性2
		BUFFINC_RESATT3				= 138,	// 增加属性3
		BUFFINC_RESATT4				= 139,	// 增加属性4
		BUFFINC_RESATT5				= 140,	// 增加属性5
		BUFFINC_RESATT6				= 141,	// 增加属性6
		BUFFFUNC_INCAP				= 142,	// 增加功勋
		BUFFFUNC_INCCORPSGORY		= 143,	// 增加军团经验
	
		BUFFFUNC_PLAYER_UNATTACK	= 144,	// 无法被玩家攻击 
		BUFFFUNC_UNATTACK_PLAYER	= 145,	// 无法攻击玩家
		BUFFFUNC_HORSE				= 146,	// 坐骑状态
		BUFFFUNC_INVISIBLE			= 147,	// 隐身 
		BUFFFUNC_USESKILL_REMOVE	= 148,	// 使用技能删除BUFF
		BUFFFUNC_DISINVISIBLE		= 149,	// 反影身
		BUFFFUNC_UNFUNC				= 150,	// 特定BUFF功能无效
		BUFFINC_DUCKRATE			= 151,	// 增加闪避率
		BUFFFUNC_FLY				= 152,	// 飞行
		BUFFINC_DEFALLPER			= 153,	// 增加防御百分比
		BUFFINC_DEFALLFIX			= 154,	// 增加防御固定值
		BUFFINC_ATTALLPER			= 155,	// 增加攻击百分比（无）
		BUFFINC_ATTALLFIX			= 156,	// 增加攻击固定值（无）
		BUFFFUNC_STONE				= 157,	// 灵石
		BUFFINC_RESDEFALL			= 158,	// 全抗
		BUFFINC_RESATTALL			= 159,	// 全属
		BUFFFUNC_ADDSKILL			= 160,	// 获得技能
        BUFFFUNC_REFRESHSKILLCD     = 161,  // 刷新技能
		BUFFFUNC_HIDE				= 162,	// 对主动怪隐身
		BUFFFUNC_PERSISTDECPERHP	= 163,	// 持续百分比伤害
		BUFFFUNC_OBTEXP_BYTAB		= 164,	// 根据经验表加经验
		BUFFFUNC_IGNOREPIMUPER		= 165,	// 忽视物理免伤百分比
		BUFFFUNC_IGNOREMIMUPER		= 166,	// 忽视法术免伤百分比
		BUFFFUNC_INCOFFLINEEXP		= 167,	// 增加离线经验百分比
		BUFFFUNC_IGNOREPDEFENCE		= 168,	// 忽视物理防御百分比
		BUFFFUNC_IGNOREMDEFENCE		= 169,	// 忽视法术防御百分比
		BUFFFUNC_BUFFAFTERBUFF		= 170,	//成功或失败后获得的特殊效果
		BUFFFUNC_REDUCEHPTO			= 171,	//降低玩家HP至固定值
		BUFFFUNC_REDUCEMPTO			= 172,	//降低玩家MP至固定值
	};
	// BUFFFUNC_REFRESHSKILLCD:
	// mBuffParam[0] = 0 刷新一个技能 mBuffParam[1] 要刷新的技能id mBuffParam[2] 不刷新的技能id 
	//                 = 1 刷新一个系的技能 mBuffParam[1] 要刷新的技能系的id mBuffParam[2] 不刷新的技能id 
	//                 = 2 刷新全体技能 mBuffParam[1] 无意义  mBuffParam[2] 不刷新的技能id 

	class CBuffFunc
	{
	public:
		BUFF_FUN_TYPE		mBuffFunc;
		int					mBuffParam[ 4 ];
	};

	enum BUFF_STYLE
	{
		BUFF_COMM					= 0,	// 普通Buff
		BUFF_AUREOLE				= 1,						// 光环Buff
	};

	enum EMBUFF_TIMER_TYPE
	{
		BUFF_TIMER_TYPE_GAME = 0,			// 游戏时间
		BUFF_TIMER_TYPE_REAL = 1,				// 真实时间
	};

	enum BUFF_CANCLE_TYPE
	{
		BUFF_CANCEL				= 0,	// 要清除
		BUFF_NOTCANCEL			= 1,	// 不清除				
	};

	enum BUFF_MAP_TYPE
	{
		NON_BUFF				= 0,		// 无
		BATTLE_BUFF				= 1,			// 战场Buff
		FB_BUFF					= 2,			// FB Buff
		COMM_BUFF				= 3,			// 普通Buff
		GROUND_BUFF				= 4,			// 地宫Buff
	};

	enum BUFF_SUPERPOSITION_TYPE
	{
		NONE_SUPERPOSITION			= 0,		// 无叠加
		TIME_SUPERPOSITION,						// 时间叠加
		FUNC_SUPERPOSITION,						// 功能叠加
	};

	enum NUFF_STONE_TYPE
	{
		STONE_COLLECT				= 1,		// 采集型
		STONE_FLAG					= 2,		// 大旗型
	};

public:

	// FieldName: Buff组系列ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mBuffSeriesID;

	// FieldName: Buff组评估值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mIdenValue;


	// FieldName: Buff作用类型
	// FieldType: INT4
	// Type:	  COMBOENUM(BUFF_STYLE)
	BUFF_STYLE mBuffStyle; 

    // FieldName: Buff功能首次作用时间间隔(0表示立即作用;否则必须和作用时间间隔相同)
    // FieldType: INT4
    // Type:      EDITNUMBER(0,1000000) 
	int		mBuffFirstActionTime;

    // FieldName: Buff功能作用时间间隔(0表示不是间隔性作用;否则必须能被buff持续时间整除)
    // FieldType: INT4
    // Type:      EDITNUMBER(0,1000000) 
	int		mBuffActionTime;

	// FieldName: Buff的第%d项功能码
	// FieldType: INT4
	// Type:	  COMBOENUM(BUFF_FUN_TYPE)

	// FieldName: Buff的第%d项功能码功能数值1
	// FieldType: INT4
	// Type:	  EDITNUMBER(-100000,100000000)

	// FieldName: Buff的第%d项功能码功能数值2
	// FieldType: INT4
	// Type:	  EDITNUMBER(-100000,100000000)

	// FieldName: Buff的第%d项功能码功能数值3
	// FieldType: INT4
	// Type:	  EDITNUMBER(-100000,100000000)

	// FieldName: Buff的第%d项功能码功能数值4
	// FieldType: INT4
	// Type:	  EDITNUMBER(-100000,100000000)

	CBuffFunc	mBuff[ MAX_BUFFFUN_COUNT ]; 

	// FieldName: $Buff的显示名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mBuffName[ 32 ];

	// FieldName: Buff的图标
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mBuffIcon[ 32 ];

	// FieldName: $Buff的显示悬浮提示
	// FieldType: STRING128
	// Type:	  EDITSTRING
	char	mBuffDesc[ 128 ];

	// FieldName: Buff的绝对命中概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mHitrate;
	
	// FieldName: Buff值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mBuffValue;
	
	// FieldName: Buff抗性系别
	// FieldType: INT4
	// Type:	  COMBOSINGLE(怒风系,炽焰系,玄冰系,纯阳系,凝尘系,寒阴系,基础)
	int		mResistType;
	
	// FieldName: Buff持续时间毫秒(0表示立刻作用1次,仅1次,-1为无限)
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1,100000000)
	int		mLastTime;

	// FieldName: Buff的类型(正面或负面)
	// FieldType: INT4
	// Type:	  COMBOENUM(BUFF_TYPE)
	BUFF_TYPE	mBuffType;

	// FieldName: Buff是否可以手动取消
	// FieldType: INT4
	// Type:	  COMBOENUM(BUFF_CANCLE_TYPE)
	int		mCancelBuff;

	// FieldName: Buff是否进入战斗
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不进战斗=0,进入战斗)
	int		mEnableBattle;

	// FieldName: Buff的动画特效
	// FieldType: STRING32
	// Type:	  EDITSTRING	
	char	mBuffAni[ 32 ];

	// FieldName: Buff显示排数
	// FieldType: INT4
	// Type:	  COMBOSINGLE(不显示=0,第一排=1,第二排,第三排)
	unsigned int mBuffRow;

	// FieldName: Buff死亡后是否消失
	// FieldType: INT4
	// Type:	  COMBOENUM(BUFF_CANCLE_TYPE)
	unsigned int mBuffDieDel;

	// FieldName: Buff的时间类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(游戏时间=0,真实时间)
	int			 mTimerType;

	// FieldName: Buff能否清除
	// FieldType: INT4
	// Type:      COMBOENUM(BUFF_CANCLE_TYPE)
	int			 mBuffClear;

	// FieldName: 特定地图ID
	// FieldType: INT4
	// Type:      EDITNUMBER(0,65535)
	int			 mBuffMapID;

	// FieldName: 特定线ID
	// FieldType: INT4
	// Type:      EDITNUMBER(0,65535)
	int			 mBuffLineID;

	// FieldName: 伤害参数
	// FieldType: INT4
	// Type:      EDITNUMBER(0,65535)
	int			 mSkillLevel;

	// FieldName: 国家Buff
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int			  mCountryBuff;

	// FieldName: Buff地图类型
	// FieldType: INT4
	// Type:	  COMBOENUM(BUFF_MAP_TYPE)
	BUFF_MAP_TYPE mBuffMapType;

	// FieldName: Buff叠加类型
	// FieldType: INT4
	// Type:      COMBOENUM(BUFF_SUPERPOSITION_TYPE)
	BUFF_SUPERPOSITION_TYPE	mBuffSuperposition;	

	// FieldName: Buff的新命中概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mAccurate;

	// FieldName: 下线是否清除
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int		mOffRemove;

	// FieldName: Buff是否复制变身
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	int		mTransform;	

	// FieldName: Buff去掉叠加的ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mRemoveCountID;
public:
	void SetFromPB( bool bServer, PBBuff* pBuff );
	void CreateTlvPB( bool bServer, PBBuff* pBuff );
	int Initialize();
	CTemplateBuff()
	{
		Initialize();
	}
	~CTemplateBuff() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 等级经验表		## 模版名称
// version	= 1					## 版本号
// path		= Config/等级经验表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateLevelExp : public CTemplate
{
public:

	// FieldName: 第%d级的经验数值
	// FieldType: INT8
	// Type:	  EDITNUMBER(0,72057594037927935)
	unsigned long long	mLevelExp[ TMP_MAX_GRADE ];	

public:
	void SetFromPB( bool bServer, PBLevelExp* pLevelExp );
	void CreateTlvPB( bool bServer, PBLevelExp* pLevelExp );
	int Initialize();
	CTemplateLevelExp() 
	{
		Initialize();
	}
	~CTemplateLevelExp() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

struct CSingleMoneyLmt
{
	unsigned int   mBagMoneyLmt;	
	unsigned int   mBagBindMoneyLmt;	
	unsigned int   mStorageMoneyLmt;	
	unsigned int   mStorageBindMoneyLmt;	
};
// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 等级金钱上限表	## 模版名称
// version	= 1					## 版本号
// path		= Config/等级金钱上限表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateLevelMoneyLmt : public CTemplate
{
public:

	// FieldName: 第%d级限制包裹携带的非绑定金
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,900000000)

	// FieldName: 第%d级限制包裹携带的绑定金
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,900000000)

	// FieldName: 第%d级限制仓库存放的非绑定金
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,900000000)

	// FieldName: 第%d级限制仓库存放的绑定金
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,900000000)
	CSingleMoneyLmt  mLmtTable[ TMP_MAX_GRADE ];

public:
	void SetFromPB( bool bServer, PBLevelMoneyLmt* pLevelMoney );
	void CreateTlvPB( bool bServer, PBLevelMoneyLmt* pLevelMoney );
	int Initialize();
	CTemplateLevelMoneyLmt() 
	{
		Initialize();
	}
	~CTemplateLevelMoneyLmt() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 宠物经验表		## 模版名称
// version	= 1					## 版本号
// path		= Config/宠物经验表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplatePetLevelExp : public CTemplate
{
public:

	// FieldName: 第1级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第2级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第3级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第4级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第5级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第6级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第7级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第8级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第9级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第10级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第11级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第12级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第13级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第14级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第15级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第16级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第17级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第18级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第19级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第20级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第21级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第22级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第23级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第24级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第25级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第26级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第27级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第28级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第29级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第30级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第31级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第32级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第33级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第34级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第35级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第36级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第37级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第38级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第39级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第40级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第41级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第42级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第43级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第44级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第45级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第46级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第47级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第48级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第49级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第50级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第51级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第52级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第53级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第54级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第55级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第56级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第57级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第58级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第59级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第60级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第61级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第62级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第63级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第64级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第65级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第66级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第67级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第68级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第69级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第70级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第71级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第72级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第73级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第74级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第75级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第76级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第77级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第78级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第79级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第80级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第81级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第82级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第83级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第84级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第85级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第86级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第87级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第88级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第89级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第90级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第91级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第92级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第93级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第94级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第95级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第96级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第97级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第98级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第99级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第100级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第101级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第102级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第103级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第104级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第105级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第106级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第107级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第108级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第109级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第110级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第111级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第112级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第113级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第114级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第115级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第116级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第117级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第118级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第119级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第120级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第121级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第122级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第123级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第124级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第125级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第126级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第127级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第128级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第129级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第130级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第131级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第132级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第133级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第134级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第135级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第136级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第137级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第138级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第139级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第140级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第141级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第142级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第143级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第144级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第145级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第146级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第147级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第148级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第149级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第150级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第151级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第152级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第153级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第154级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第155级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第156级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第157级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第158级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第159级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第160级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	unsigned int	mLevelExp[ TMP_MAX_GRADE ];	

public:
	void SetFromPB( bool bServer, PBPetLevelExp* pLevelExp );
	void CreateTlvPB( bool bServer, PBPetLevelExp* pLevelExp );
	int Initialize();
	CTemplatePetLevelExp() 
	{
		Initialize();
	}
	~CTemplatePetLevelExp() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 生产经验表		## 模版名称
// version	= 1					## 版本号
// path		= Config/生产经验表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateProExp : public CTemplate
{
public:


	// FieldName: 第1级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第2级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第3级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第4级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第5级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第6级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第7级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第8级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第9级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第10级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第11级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第12级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第13级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第14级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第15级经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	unsigned int	mLevelExp[ 15 ];	

public:
	void SetFromPB( bool bServer, PBProduceExp* pExp );
	void CreateTlvPB( bool bServer, PBProduceExp* pExp );
	int Initialize();
	CTemplateProExp()
	{
		Initialize();
	}
	~CTemplateProExp() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 装备升级表		## 模版名称
// version	= 1					## 版本号
// path		= Config/装备升级表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateEquipUpg : public CTemplate
{
public:
	class CEquipUpg
	{
	public:
		int		mSuccess;		// 成功的概率
		int		mFail;			// 消失的概率
		int		mResult;		// 失败后的结果
		int		mIncLevel;		// 升级成功后提升的等级
		int		mExtSuccess;	// 不掉级的成功概率
	};

public:

	// FieldName: %d类%d级装备升级的成功率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)

	// FieldName: %d类%d级装备升级的消失率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)

	// FieldName: %d类%d级装备升级失败的结果
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1,100)

	// FieldName: %d类%d级装备升级成功后提升的等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: %d类%d级装备不掉级的成功率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	CEquipUpg	mEquipUpg[ 3 ][ 15 ];

public:
	void SetFromPB( bool bServer, PBEquipUpg* pEquip );
	void CreateTlvPB( bool bServer, PBEquipUpg* pEquip );
	int Initialize();
	CTemplateEquipUpg() 
	{
		Initialize();
	}
	~CTemplateEquipUpg() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 生产者套装		## 模版名称
// version	= 1					## 版本号
// path		= 套装/生产者套装	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateSuitProducer : public CTemplate
{
public:

	// FieldName: 全身4件装备由同1玩家生产激活的附加属性ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 全身5件装备由同1玩家生产激活的附加属性ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 全身6件装备由同1玩家生产激活的附加属性ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 全身7件装备由同1玩家生产激活的附加属性ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 全身8件装备由同1玩家生产激活的附加属性ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 全身9件装备由同1玩家生产激活的附加属性ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 全身10件装备由同1玩家生产激活的附加属性ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mPropertyTempID[ 7 ];

	// FieldName: 全身+%d装备有%d件激活的附加属性ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mUpgTempID[ 16 ][ 21 ];

	// FieldName: 全身有%d件铭刻的附加属性ID%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mExtTempID[ 21 ][ 5 ];

	// FieldName: $附加属性%d描述
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char		mDesText[ 21 ][ STRING256 ];

public:
	void SetFromPB( bool bServer, PBSuitProducer* pSuit );
	void CreateTlvPB( bool bServer, PBSuitProducer* pSuit );
	int Initialize();
	CTemplateSuitProducer() 
	{
		Initialize();
	}
	~CTemplateSuitProducer() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 普通套装			## 模版名称
// version	= 1					## 版本号
// path		= 套装/普通套装		## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateSuitNormal : public CTemplate
{
public:
	
	// FieldName: $套装名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char		mName[ STRING32 ];
	
	// FieldName: 套装需要的装备1ID
	// FieldType: INT4
	// Type:	  EDITNUMBER
	
	// FieldName: 套装需要的装备2ID
	// FieldType: INT4
	// Type:	  EDITNUMBER
	
	// FieldName: 套装需要的装备3ID
	// FieldType: INT4
	// Type:	  EDITNUMBER
	
	// FieldName: 套装需要的装备4ID
	// FieldType: INT4
	// Type:	  EDITNUMBER
	
	// FieldName: 套装需要的装备5ID
	// FieldType: INT4
	// Type:	  EDITNUMBER
	
	// FieldName: 套装需要的装备6ID
	// FieldType: INT4
	// Type:	  EDITNUMBER
	
	// FieldName: 套装需要的装备7ID
	// FieldType: INT4
	// Type:	  EDITNUMBER
	
	// FieldName: 套装需要的装备8ID
	// FieldType: INT4
	// Type:	  EDITNUMBER
	
	// FieldName: 套装需要的装备9ID
	// FieldType: INT4
	// Type:	  EDITNUMBER
	
	// FieldName: 套装需要的装备10ID
	// FieldType: INT4
	// Type:	  EDITNUMBER
	
	// FieldName: 套装需要的装备11ID
	// FieldType: INT4
	// Type:	  EDITNUMBER
	
	// FieldName: 套装需要的装备12ID
	// FieldType: INT4
	// Type:	  EDITNUMBER

	// FieldName: 套装需要的装备13ID
	// FieldType: INT4
	// Type:	  EDITNUMBER
	int			mEquipTempID[ 13 ];
	
	// FieldName: $套装激活的玩家称号
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char		mActiveTitle[ STRING32 ];

	// FieldName: 套装激活的玩家附加属性1ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 套装激活的玩家附加属性2ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 套装激活的玩家附加属性3ID
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 套装激活的玩家附加属性4ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mPropertyTempID[ 4 ];

public:
	void SetFromPB( bool bServer, PBSuitNormal* pSuit );
	void CreateTlvPB( bool bServer, PBSuitNormal* pSuit );
	int Initialize();
	CTemplateSuitNormal( )
	{
		Initialize();
	}
	~CTemplateSuitNormal() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 符文物语			## 模版名称
// version	= 1					## 版本号
// path		= 套装/符文物语		## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateSpell : public CTemplate
{
public:

	// FieldName: $该套符文显示名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char		mName[ STRING32 ];

	// FieldName: 套装编号
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mSuitID;

	// FieldName: 该套符文需要的文字1
	// FieldType: INT4
	// Type:	  TEMPPATH
	
	// FieldName: 该套符文需要的文字2
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文需要的文字3
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文需要的文字4
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文需要的文字5
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文需要的文字6
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文需要的文字7
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文需要的文字8
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文需要的文字9
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文需要的文字10
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文需要的文字11
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文需要的文字12
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文需要的文字13
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mLetterID[ 13 ];

	// FieldName: 该套符文激活的第一项附加属性需要符文数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 该套符文激活的第二项附加属性需要符文数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 该套符文激活的第三项附加属性需要符文数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 该套符文激活的第四项附加属性需要符文数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 该套符文激活的第五项附加属性需要符文数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	int			mRequire[ 5 ];

	// FieldName: 该套符文激活的第一项附加属性1
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第一项附加属性2
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第一项附加属性3
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第一项附加属性4
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第二项附加属性1
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第二项附加属性2
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第二项附加属性3
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第二项附加属性4
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第三项附加属性1
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第三项附加属性2
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第三项附加属性3
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第三项附加属性4
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第四项附加属性1
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第四项附加属性2
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第四项附加属性3
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第四项附加属性4
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第五项附加属性1
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第五项附加属性2
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第五项附加属性3
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 该套符文激活的第五项附加属性4
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mPropertyTempID[ 5 ][ 4 ];

	// FieldName: $附加属性1描述
	// FieldType: STRING256
	// Type:	  EDITSTRING

	// FieldName: $附加属性2描述
	// FieldType: STRING256
	// Type:	  EDITSTRING

	// FieldName: $附加属性3描述
	// FieldType: STRING256
	// Type:	  EDITSTRING

	// FieldName: $附加属性4描述
	// FieldType: STRING256
	// Type:	  EDITSTRING

	// FieldName: $附加属性5描述
	// FieldType: STRING256
	// Type:	  EDITSTRING

	char		mDesText[ 5 ][ STRING256 ];

public:
	void SetFromPB( bool bServer, PBSpell* pSpell );
	void CreateTlvPB( bool bServer, PBSpell* pSpell );
	int Initialize();
	CTemplateSpell()
	{
		Initialize();
	}
	~CTemplateSpell() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 装备生产			## 模版名称
// version	= 1					## 版本号
// path		= 物品/装备生产		## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateProEquip : public CTemplate
{
public:

	// FieldName: 装备类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(武器=0,帽子,衣服,护腿,护腕,戒指,项链,护符,披风,腰饰,法宝)
	int			mType;

	// FieldName: 装备的详细类别
	// FieldType: INT4
	// Type:	  COMBOSINGLE(枪=0,双刀,弓,法盾,法杖,钺,重甲,轻甲,布甲,物理,法术)
	int			mDetailID1;

	// FieldName: 防具的详细类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无=0,物理防御,法术防御,全防御,全生命,物理生命,法术生命)
	int			mDetailID2;

	// FieldName: 装备等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int			mLevel;

	// FieldName: $装备名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char		mName[ STRING32 ];

	// FieldName: 材料ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mMaterialID;

	// FieldName: 材料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int			mMaterialNum;

	// FieldName: 材料等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int			mMaterialRank;

	// FieldName: 本体1ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mItemID1;

	// FieldName: 本体1概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mItemRan1;

	// FieldName: 本体2ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mItemID2;

	// FieldName: 本体2概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mItemRan2;

	// FieldName: 本体3ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mItemID3;

	// FieldName: 本体3概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mItemRan3;

	// FieldName: 所需银两
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mMoney;

	// FieldName: 是否支持绑定金钱
	// FieldType: INT4
	// Type:	  COMBOSINGLE(是=0,否=1)
	int			mBindMoney;

	// FieldName: 图标ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mIconID;

	// FieldName: $装备说明
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char		mDesc[ STRING256 ];

	// FieldName: 所得经验
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mExp;

public:
	void SetFromPB( bool bServer, PBProduceEquip* pEquip );
	void CreateTlvPB( bool bServer, PBProduceEquip* pEquip );
	int Initialize();
	CTemplateProEquip()
	{
		Initialize();
	}
	~CTemplateProEquip() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 药品生产			## 模版名称
// version	= 1					## 版本号
// path		= 物品/药品生产		## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateProPhysics : public CTemplate
{
public:

	// FieldName: 药品类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(红=0,蓝,力量BUFF,智力BUFF,体质BUFF,精神BUFF,耐力BUFF,经验)
	int			mType;

	// FieldName: 药品等级
	// FieldType: INT4
	// Type:	  COMBOSINGLE(1级=0,2级,3级,4级)
	int			mLevel;

	// FieldName: $药品名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char		mName[ STRING32 ];

	// FieldName: 材料ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mMaterialID;

	// FieldName: 材料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int			mMaterialNum;

	// FieldName: 本体ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mItemID;

	// FieldName: 所得经验
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mExp;

	// FieldName: 所需银两
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mMoney;

	// FieldName: 是否支持绑定金钱
	// FieldType: INT4
	// Type:	  COMBOSINGLE(是=0,否=1)
	int			mBindMoney;

public:
	void SetFromPB( bool bServer, PBProducePhysic* pPhysic );
	void CreateTlvPB( bool bServer, PBProducePhysic* pPhysic );
	int Initialize();
	CTemplateProPhysics() 
	{
		Initialize();
	}
	~CTemplateProPhysics() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 符文				## 模版名称
// version	= 1					## 版本号
// path		= 物品/符文			## 路径
// parent	= 道具基本信息		## 父亲模版
// ********************************************************************** //
class CTplItemRune : public CTplItem
{
public:

	// FieldName: 套装编号
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mSuitID;

	// FieldName: $符文字
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char		mWord[ STRING32 ];

	// FieldName: 套装中的顺序
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mOrderID;

	// FieldName: 镶嵌费用
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mInsertCharge;

	// FieldName: 新增附加属性ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mPropertyID;

	// FieldName: 符文顺序标号
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mRuneIndex;

	// FieldName: 显示颜色（白蓝紫金彩）
	// FieldType: INT4
	// Type:	  COMBOSINGLE(白色=0,蓝色,紫色,金色,彩色)
	int			mShowColor;



public:
	void SetFromPB( bool bServer, PBItemRune* pRuneItem );
	void CreateTlvPB( bool bServer, PBItemRune* pRuneItem );
	int Initialize();
	CTplItemRune() 
	{ 
		Initialize();
	}
	~CTplItemRune()  {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 珠宝				## 模版名称
// version	= 1					## 版本号
// path		= 物品/珠宝			## 路径
// parent	= 道具基本信息		## 父亲模版
// ********************************************************************** //
class CTplItemJewel : public CTplItem
{
public:

	enum
	{
		POS_RING		= 0x01,				// 戒指类装备
		POS_CUFF		= 0x02,				// 护腕类装备
		POS_WEAPON		= 0x04,				// 武器类装备
		POS_CHEST		= 0x08,				// 护胸类装备
		POS_LEG			= 0x10,				// 护腿类装备
		POS_HEAD		= 0x20,				// 护头类装备
		POS_NECK		= 0x40,				// 项链类装备
		POS_SASH		= 0x80,				// 腰饰类装备
		POS_CHARM		= 0x100,			// 护符类装备
		POS_BALL		= 0x200,			// 法球类装备
		POS_CLOAK		= 0x400,			// 披风类装备
		POS_FASHION		= 0x800,			// 时装类装备
		POS_MAGICWEAPON	= 0x1000,			// 法宝类装备
	};

	enum
	{
		JEWEL_WHITE	= 0,				// 白色宝石
		JEWEL_BLUE  = 1,						// 蓝色宝石
		JEWEL_PURPLE = 2,					// 紫色宝石
		JEWEL_GOLD   = 3,						// 金色宝石
		JEWEL_COLOR  = 4,					// 彩色宝石
	};	

	// FieldName: 珠宝等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mTypeID;

	// FieldName: 装备位置
	// FieldType: INT4
	// Type:	  CHECKMUTI(戒指类装备=1,护腕类装备,武器类装备,护胸类装备,护腿类装备,护头类装备,项链类装备,腰饰类装备,护符类装备,法球类装备,披风类装备,法宝类装备)
	int			mPos;

	// FieldName: 显示颜色（白蓝紫金彩）
	// FieldType: INT4
	// Type:	  COMBOSINGLE(白色=0,蓝色,紫色,金色,彩色)
	int			mShowColor;

	// FieldName: 附加属性
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mProperty;

	// FieldName: 是否唯一
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mUnique;

	// FieldName: 降到%d档得到的宝石ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mJewelID[ 9 ];

	// FieldName: 宝石配方ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mComposeID;

	// FieldName: 镶嵌费用
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mInsertCharge;

	// FieldName: 摘除费用
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mRemoveCharge;

	// FieldName: 合成费用
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mComposeCharge;

	// FieldName: 珠宝档次
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mLevel;

	// FieldName: %d种概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mRand[ 5 ];

	// FieldName: 剥离得到的宝石ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int			mNewJewelID;

	// FieldName: %d个的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mNewRand[ 5 ];

	// FieldName: 雕琢得到的宝石ID%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mCarveJewelID[ 3 ];

	// FieldName: 雕琢得到的概率%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mCarveRand[ 3 ];

	// FieldName: 雕琢费用
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mCarveCharge;

	// FieldName: 需要的雕琢符ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mCardID;

	// FieldName: 是否可以雕琢
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)
	int			mCanCarve;

	// FieldName: 对合成概率影响
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mAffectRand;

	// FieldName: 宝石合成公用标志
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mJewComID;

	// FieldName: 合成等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mComLevel;

public:
	void SetFromPB( bool bServer, PBItemJewel* pJewelItem );
	void CreateTlvPB( bool bServer, PBItemJewel* pJewelItem );
	int Initialize();
	CTplItemJewel() 
	{
		Initialize();
	}
	~CTplItemJewel() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 元气虫			## 模版名称
// version	= 1					## 版本号
// path		= 物品/元气虫		## 路径
// parent	= 道具基本信息		## 父亲模版
// ********************************************************************** //
class CTplItemYuanBug : public CTplItem
{
public:

	// FieldName: 储存上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mYuan;

	// FieldName: 转化成的道具
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mEggID;

	// FieldName: 化金类型啊
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mYuanType;

public:
	void SetFromPB( bool bServer, PBItemYuanBug* pBugItem );
	void CreateTlvPB( bool bServer, PBItemYuanBug* pBugItem );
	int Initialize();
	CTplItemYuanBug( )
	{
		Initialize();
	}
	~CTplItemYuanBug() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 元气蛋			## 模版名称
// version	= 1					## 版本号
// path		= 物品/元气蛋		## 路径
// parent	= 道具基本信息		## 父亲模版
// ********************************************************************** //
class CTplItemYuanEgg : public CTplItem
{
public:

	// FieldName: 等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int			mLevel;

	// FieldName: 合成的配方ID
	// FieldType: INT4
	// Type:	  TEMPPATH
	int mComposeID;

	// FieldName: 最大装备等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mMaxEquip;

public:
	void SetFromPB( bool bServer, PBItemYuanEgg* pEggItem );
	void CreateTlvPB( bool bServer, PBItemYuanEgg* pEggItem );
	int Initialize();
	CTplItemYuanEgg() 
	{
		Initialize();
	}
	~CTplItemYuanEgg() {}
};	 

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 附魔物品			## 模版名称
// version	= 1					## 版本号
// path		= 物品/附魔物品		## 路径
// parent	= 道具基本信息		## 父亲模版
// ********************************************************************** //
class CTplItemMagicStone : public CTplItem
{
public:
	enum EPart
	{
		PART_RING			= ( 1 << 0 ),		// 戒指类装备
		PART_CUFF			= ( 1 << 1 ),		// 护腕类装备
		PART_WEAPON			= ( 1 << 2 ),		// 武器类装备
		PART_CHEST			= ( 1 << 3 ),		// 护胸类装备
		PART_LEG			= ( 1 << 4 ),		// 护腿类装备
		PART_HEAD			= ( 1 << 5 ),		// 护头类装备
		PART_NECK			= ( 1 << 6 ),		// 项链类装备
		PART_SASH			= ( 1 << 7 ),		// 腰饰类装备
		PART_CHARM			= ( 1 << 8 ),		// 护符类装备
		PART_BALL			= ( 1 << 9 ),		// 法球类装备
		PART_CLOAK			= ( 1 << 10 ),		// 披风类装备
		PART_FASHION		= ( 1 << 11 ),		// 时装类装备
		PART_MAGICWEAPON	= ( 1 << 12 ),		// 法宝类装备
	};		
		
public:
	// FieldName: 附魔的部位
	// FieldType: INT4
	// Type:	  CHECKMUTI(戒指类装备,护腕类装备,武器类装备,护胸类装备,护腿类装备,护头类装备,项链类装备,腰饰类装备,护符类装备,法球类装备,披风类装备,时装类装备,法宝类装备)
	int		mPart;

	// FieldName: 同类附魔石第%d次附魔时生成的属性ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mAttribute[MAX_MAGICSTONE_USEDTIMES];
	
	// FieldName: 可以使用的次数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mUsedTimes;
	
	// FieldName: 第%d次使用附魔物品附魔成功的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mProbability[MAX_MAGICSTONE_USEDTIMES];
	
	// FieldName: 附魔装备等级要求
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mLevelLimit;
	
public:
	void SetFromPB( bool bServer, PBItemMagicStone* pMagicStone );
	void CreateTlvPB( bool bServer, PBItemMagicStone* pMagicStone );
	int Initialize();
	CTplItemMagicStone() 
	{
		Initialize();
	}
	~CTplItemMagicStone() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= 生活技能配方卷轴		## 模版名称
// version	= 1						## 版本号
// path		= 物品/生活技能配方卷轴	## 路径
// parent	= 道具基本信息			## 父亲模版
// ********************************************************************** //
class CTplItemLifeSkillScroll : public CTplItem
{											
public:
	// FieldName: 生活配方ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mComposeID;
	
	// FieldName: 配方类型
	// FieldType: INT4
	// Type:	  COMBOENUM(EM_LIFESKILL)
	int		mComposeType;

public:
	void SetFromPB( bool bServer, PBLifeSkillScroll* pScroll );
	void CreateTlvPB( bool bServer, PBLifeSkillScroll* pScroll );
	int Initialize();
	CTplItemLifeSkillScroll()
	{
		Initialize();
	}
	~CTplItemLifeSkillScroll() {}
};	
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** // 

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 开槽表			## 模版名称
// version	= 1					## 版本号
// path		= Config/开槽表		## 路径
// parent	=					## 父亲模版
// ********************************************************************** //

class CTemplateOpenSlot : public CTemplate
{
public:

	// FieldName: 打开第1个槽需要的等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 打开第2个槽需要的等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 打开第3个槽需要的等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 打开第4个槽需要的等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 打开第5个槽需要的等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	int		mSlotLevel[ 5 ];

	// FieldName: 打开第1个槽需要的个数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 打开第2个槽需要的个数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 打开第3个槽需要的个数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 打开第4个槽需要的个数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 打开第5个槽需要的个数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	int		mSlotNumber[ 5 ];

public:
	void SetFromPB( bool bServer, PBOpenSlot* pSlot );
	void CreateTlvPB( bool bServer, PBOpenSlot* pSlot );
	int Initialize();
	CTemplateOpenSlot()
	{
		Initialize();
	}
	~CTemplateOpenSlot() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 合成				## 模版名称
// version	= 1					## 版本号
// path		= 物品/合成			## 路径
// parent	=					## 父亲模版
// ********************************************************************** //

class CTemplateCompose : public CTemplate 
{
public:

	// FieldName: 合成需要的材料ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	int		mMaterialID;

	// FieldName: 合成需要的材料数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	int		mMaterialNum;

	// FieldName: 合成得到的物品ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	int		mItemID;

	// FieldName: 合成得到的物品数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	int		mItemNum;

public:
	void SetFromPB( bool bServer, PBCompose* pCompose );
	void CreateTlvPB( bool bServer, PBCompose* pCompose );
	int Initialize();
	CTemplateCompose() 
	{
		Initialize();
	}
	~CTemplateCompose() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 复杂合成			## 模版名称
// version	= 1					## 版本号
// path		= 物品/复杂合成		## 路径
// parent	=					## 父亲模版
// ********************************************************************** //  

enum EM_LIFESKILL	
{	
	LIFESKILL_NONE			= 0,	// 无生活技能
	LIFESKILL_COLLECT		= 1,	// 采集技能
	LIFESKILL_GOLDMAKE		= 2,	// 点金	
	LIFESKILL_ARTIFICER		= 3,	// 巧匠
	LIFESKILL_PILLMAKER		= 4,	// 炼丹
	LIFESKILL_KOOKER		= 5,	// 烹饪
	LIFESKILL_MAILSKILL		= 6,	// 主修技能
	LIFESKILL_SUBSKILL		= 7,	// 辅修技能
};
	

class CTemplateMultiCompose : public CTemplate
{
public:
	enum EM_LifSkillKind
	{		
		SKILLKIND_COLLECT_STONE				=	1,	// 采集职业的金石配方
		SKILLKIND_COLLECT_MAKER				=	2,	// 采集职业的织造配方
		SKILLKIND_PILLMAKER_HPITEM			=	3,	// 炼丹职业的生命药剂配方
		SKILLKIND_PILLMAKER_MPITEM			=	4,	// 炼丹职业的魔法药剂配方		
		SKILLKIND_PILLMAKER_BUFFITEM		=	5,	// 炼丹职业的BUFF药剂配方
		SKILLKIND_PILLMAKER_SPECIALITEM		=	6,	// 炼丹职业的特殊药剂配方					
		SKILLKIND_ARTIFICER_KITBAG			=	7,	// 巧匠职业的背包制作配方
		SKILLKIND_ARTIFICER_MAGICPAPER		=	8,	// 巧匠职业的符类制作配方
		SKILLKIND_ARTIFICER_SPECIALITEM		=	9,	// 巧匠职业的特殊物品制作配方
		SKILLKIND_GOLDMAKE_HEAD				=	10, // 点金职业的护头类点金石制作配方
		SKILLKIND_GOLDMAKE_RING				=	11, // 点金职业的戒指类点金石制作配方
		SKILLKIND_GOLDMAKE_CUFF				=	12, // 点金职业的护腕类点金石制作配方
		SKILLKIND_GOLDMAKE_WEAPON			=	13,	// 点金职业的武器类点金石制作配方			
		SKILLKIND_GOLDMAKE_CHEST			=	14,	// 点金职业的护胸类点金石制作配方	
		SKILLKIND_GOLDMAKE_LEG				=	15,	// 点金职业的护腿类点金石制作配方							
		SKILLKIND_GOLDMAKE_NECK				=	16,	// 点金职业的项链类点金石制作配方													
		SKILLKIND_GOLDMAKE_SASH				=	17,	// 点金职业的腰饰类点金石制作配方															
		SKILLKIND_GOLDMAKE_CHARM			=	18,	// 点金职业的护符类点金石制作配方																	
		SKILLKIND_GOLDMAKE_BALL				=	19,	// 点金职业的法球类点金石制作配方																			
		SKILLKIND_GOLDMAKE_CLOAK			=	20,	// 点金职业的披风类点金石制作配方																							
		SKILLKIND_GOLDMAKE_FASHION			=	21,	// 点金职业的时装类点金石制作配方
		SKILLKIND_GOLDMAKE_OTHER			=	22,	// 点金职业其他点金石制作配方																											
		SKILLKIND_KOOKER_COOLMAKE			=	23,	// 烹饪职业的凉拌类物品制作配方
		SKILLKIND_KOOKER_BOIL				=	24,	// 烹饪职业的煮类物品制作配方
		SKILLKIND_KOOKER_DRYBYFIRE			=	25,	// 烹饪职业的炒类物品制作配方
		SKILLKIND_KOOKER_FRY				=	26,	// 烹饪职业的炸类物品制作配方
		SKILLKIND_KOOKER_VAPORIZE			=	27,	// 烹饪职业的蒸类物品制作配方
	};	
	  
	enum EM_ComposeType
	{
		COMPOSETYPE_CARDEXCHANGE		=	0,			// 卡片兑换配方
		COMPOSETYPE_LIFESKILL			=	1,			// 生活技能配方		
		COMPOSETYPE_EQUIPMENTCHANGE		=	2,			// 装备兑换
		COMPOSETYPE_FAMILYEXCHANGE		=	3,			// 家族兑换
		COMPOSETYPE_CORPSEXCHANGE		=	4,			// 军团兑换
		COMPOSETYPE_MARRIAGEEXCHANGE	=	5,			// 结婚的请帖兑换		
		COMPOSETYPE_EQUIPUPGRADE		=	6,			// 装备升级
		COMPOSETYPE_USEVALUES			=	7,			// 带数值兑换
	}; 	

	// 兑换需要的数值类型
	enum EM_NeedValueType
	{
		NEEDVALUETYPE_NONE				= 0,		// 不需要
		NEEDVALUETYPE_CHARM				= 1,		// 魅力值
		NEEDVALUETYPE_SLAVE				= 2,		// 仙缘值
		NEEDVALUETYPE_MASTER			= 3,		// 师德值
		NEEDVALUETYPE_LASTONE			= 4,		// 大乱斗积分值
		NEEDVALUETYPE_MARRYINTIMACY		= 5,		// 夫妻之间的亲密度
		NEEDVALUETYPE_SWORNINTIMACY		= 6,		// 结拜成员的亲密度
	};

public:	
	class CDegreeInfo	
	{	
	public:							
		int		mValue;			// 玩家本身熟练度与要求的熟练度的差值
		int		mProbality;		// 在差值范围内获取熟练度的概率
		int		mDegreeValue;	// 在差值范围内获取熟练度的值
	};		
		
public:
	
	// FieldName: 合成编号
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	int		mOrder;

	// FieldName: $合成描述
	// FieldType: STRING32
	// Type:	  EDITSTRING	

	char	mDescribe[ STRING32 ];
	
	// FieldName: 配方类型
	// FieldType: INT4
	// Type:	  COMBOENUM(EM_ComposeType)
	int		mComposeType;

	// FieldName: 合成需要的材料ID%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000) 
	int		mMaterialID[ 10 ];	   	

	// FieldName: 合成需要的材料数量%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	int		mMaterialNum[ 10 ];

	// FieldName: 合成需要的数值类型%d
	// FieldType: INT4
	// Type:	  COMBOENUM(EM_NeedValueType)
	int		mNeedValueType[ 10 ];	   	

	// FieldName: 合成需要的数值%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	int		mNeedValue[ 10 ];

	// FieldName: 合成得到的物品ID%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	int		mItemID[ 10 ];

	// FieldName: 合成得到的物品数量%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	int		mItemNum[ 10 ];

	// FieldName: 合成得到的经验
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int		mExp;  	 			
	
	// FieldName: 合成得到的物品的概率%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mItemProbability[ 10 ];
	
	// FieldName: 生活技能类型
	// FieldType: INT4
	// Type:	  COMBOENUM(EM_LIFESKILL)
	int		mLifeSkillType;
	
	// FieldName: 生活技能所属的技能种类
	// FieldType: INT4
	// Type:	  COMBOENUM(EM_LifSkillKind)
	int		mSkillKind;	  
	
	// FieldName: 学习生活技能配方需要的熟练度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int		mSpecializedDegree; 	
	
	// FieldName: 该生活技能类型
	// FieldType: INT4
	// Type:	  CHECKMUTI(npc处学习的配方,可以研究生成的配方,通过卷轴学习的配方)
	int		mLifeComposeType;
	
	// FieldName: 该生活技能配方创建所需的火候度
	// FieldType: INT4
	// Type:	   EDITNUMBER(0,100000000)
	int		mCreatValue;	
	
	// FieldName: 学习该生活技能配方所需的金钱
	// FieldType: INT4
	// Type:	   EDITNUMBER(0,100000000)
	int		mStudyMoney;		
	
	// FieldName: 创造新配方获取的熟练度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mSkillCreatDegree;
	
	// FieldName: 自身熟练度和配方需要的熟练度的第%d个差值区间
	// FieldType: INT4	
	// Type:      EDITNUMBER(0,10000) 


	// FieldName: 自身熟练度和配方需要的熟练度的第%d个差值区间的获取数量度的概率
	// FieldType: INT4	
	// Type:      EDITNUMBER(0,10000) 


	// FieldName: 自身熟练度和配方需要的熟练度的第%d个差值区间的获取数量度的值
	// FieldType: INT4	
	// Type:      EDITNUMBER(0,10000) 

	CDegreeInfo mDegreeInfo[MAX_LIFESKILL_DEGREESTEP];
	
	// FieldName: 每次使用配方需要消耗的AP值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)		
	int		mApNeedCost;	
	
	// FieldName: 配方的使用次数(-1表示没有次数限制)
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1,10000)		
	int		mUsedTimes;		
	
	// FieldName: 差值最小范围( 创造配方时火候值和模板值配置的火候值小于该值时就算创造成功)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)		
	int		mMinValue;	
	// FieldName: 源装备ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mSrcEquipID;

	// FieldName: 生成装备ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mDesEquipID;
		
	// FieldName: 合成需要的贡献度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000) 	
	int		mContribute;

	// FieldName: %d级附加需求的物品ID%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mUpgItemID[ 20 ][ 3 ];

	// FieldName: %d附加需求的物品数量%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mUpgItemNum[ 20 ][ 3 ];
		
public:
	void SetFromPB( bool bServer, PBMultiCompose* pCompose );
	void CreateTlvPB( bool bServer, PBMultiCompose* pCompose );
	int Initialize();
	CTemplateMultiCompose()
	{
		Initialize();
	}
	~CTemplateMultiCompose() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 传送表			## 模版名称
// version	= 1					## 版本号
// path		= Config/传送表		## 路径
// parent	=					## 父亲模版
// ********************************************************************** //

class CTemplateTeleTable : public CTemplate
{
public:


	// FieldName: 大地图ID%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	int		mLineID[ 20 ];

	// FieldName: 场景地图ID%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	int		mMapID[ 20 ];

	// FieldName: X坐标%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)	

	int		mPosX[ 20 ];

	// FieldName: Y坐标%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	int		mPosY[ 20 ];

	// FieldName: $描述%d
	// FieldType: STRING32
	// Type:	  EDITSTRING	

	char		mDes[ 20 ][ STRING32 ];	

	// FieldName: 费用%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)	
	int		mFee[ 20 ];

	// FieldName: 需求物品%d
	// FieldType: INT4
	// Type:		EDITNUMBER(0,100000)
	int			mNeedTempID[ 20 ];

	// FieldName: 需求buff%d
	// FieldType: INT4
	// Type:		EDITNUMBER(0,100000)
	int			mNeedBuffID[ 20 ];

	// FieldName: 需求等级%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	int			mNeedLevel[ 20 ];

public:
	void SetFromPB( bool bServer, PBTeleTable* pTable );
	void CreateTlvPB( bool bServer, PBTeleTable* pTable );
	int Initialize();
	CTemplateTeleTable() 
	{
		Initialize();
	}
	~CTemplateTeleTable() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 数值修正表		## 模版名称
// version	= 1					## 版本号
// path		= Config/数值修正表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateData : public CTemplate
{
public:

	// FieldName: 人物与怪物差级1掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级2掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级3掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级4掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级5掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级6掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级7掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级8掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级9掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级10掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级11掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级12掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级13掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级14掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级15掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级16掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级17掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级18掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级19掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级20掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级21掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级22掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级23掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级24掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级25掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级26掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级27掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级28掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级29掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级30掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级31掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级32掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级33掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级34掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级35掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级36掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级37掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级38掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级39掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级40掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级41掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级42掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级43掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级44掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级45掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级46掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级47掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级48掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级49掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级50掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级51掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级52掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级53掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级54掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级55掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级56掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级57掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级58掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级59掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级60掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级61掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级62掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级63掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级64掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级65掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级66掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级67掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级68掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级69掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级70掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级71掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级72掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级73掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级74掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级75掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级76掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级77掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级78掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级79掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级80掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级81掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级82掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级83掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级84掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级85掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级86掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级87掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级88掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级89掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级90掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级91掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级92掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级93掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级94掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级95掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级96掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级97掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级98掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级99掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 人物与怪物差级100掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)
	int		mLevelFix[ 100 ];

	// FieldName: 组队人数1的掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 组队人数2的掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 组队人数3的掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 组队人数4的掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 组队人数5的掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 组队人数6的掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 组队人数7的掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 组队人数8的掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 组队人数9的掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)

	// FieldName: 组队人数10的掉落修正
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)
	int		mTeamFix[ 10 ];
	
public:
	void SetFromPB( bool bServer, PBData* pData );
	void CreateTlvPB( bool bServer, PBData* pData );
	int Initialize();
	CTemplateData()
	{
		Initialize();
	}
	~CTemplateData() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 召唤兽			## 模版名称
// version	= 1					## 版本号
// path		= 召唤兽			## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateSummonBeast : public CTemplate
{
public:
	enum COLOR_TYPE
	{
		EM_COLOR_TYPE_WHITE = 0,	// RGB(255,255,255)
		EM_COLOR_TYPE_GREEN = 1,		// RGB(109,207,246)
		EM_COLOR_TYPE_BULE  = 2,			// RGB(0,0,255)
	};

	enum ATTACK_TYPE
	{
		EM_ATTACK_TYPE_PHY = 1,		// 物理攻击
		EM_ATTACK_TYPE_MGC = 2,			// 法术攻击
	};

	enum
	{
		PET_LEVEL_ONE = 0,
		PET_LEVEL_TWO = 1,
		PET_LEVEL_THREE = 2,
	};

	class CPetData
	{
	public:
		// 召唤兽名称颜色
		COLOR_TYPE mNameColor;
		// 召唤兽攻击类型
		ATTACK_TYPE mAttackType;
		// 召唤兽物理攻击力上限
		int mPAttackUp;
		// 召唤兽物理攻击力下限
		int mPAttackLow;
		// 召唤兽法力攻击力上限
		int mMAttackUp;
		// 召唤兽法力攻击力下限
		int mMAttackLow;
		// 召唤兽移动速度
		int mSpeed;
		// 召唤兽物理防御
		int mPDefence;
		// 召唤兽法术防御
		int mMDefence;
		// 召唤兽物理减伤
		int mPReducePer;
		// 召唤兽法术减伤
		int mMReducePer;
		// 召唤兽HP上限
		int mMaxHP;
		// 召唤兽HP恢复速率
		int mHPRSpeed;
		// 召唤兽致命几率
		int mDeathRate;
		// 召唤兽致命伤害
		int mDeathValue;
		// 召唤兽致命防御
		int mDeathDefense;
		// 召唤兽普攻闪躲值
		int mNormalDuck;
		// 召唤兽技能闪躲值
		int mSkillDuck;
		// 召唤兽忽视闪躲值
		int mIgnoreDuck;
		// 召唤兽技能ID
		int mSkillID;
		// 召唤兽技能等级
		int mSkillLevel;
		// 召唤兽物理攻击范围
		int mPAttackRange;
		// 召唤兽法力攻击范围
		int mMAttackRange;
		// 抗性
		int mResDef[ 6 ];
		// 属性攻击
		int mResAtt[ 6 ];
		// 攻击加成
		int mAttPercent;
		// 防御加成
		int mDefPercent;
		// 生命加成
		int mHPPercent;
	};
public:

	// FieldName: $游戏中的显示名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mName[ STRING32 ];			

	// FieldName: 模型路径
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mModel[ STRING32 ];	

	// FieldName: 召唤兽等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,160)
	int		mLevel;
	
	// FieldName: 升至1级所需时间(单位：毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1200000)

	// FieldName: 升至2级所需时间(单位：毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1200000)

	// FieldName: 升至3级所需时间(单位：毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1200000)
	int		mLevelUpDelayTime[ 3 ];

	// FieldName: 1级召唤兽名称颜色
	// FieldType: INT4
	// Type:	  COMBOSINGLE(白色=0,绿色,蓝色)

	// FieldName: 1级召唤兽攻击类型
	// FieldType: INT4
	// Type:	  COMBOENUM(ATTACK_TYPE)

	// FieldName: 1级召唤兽物理攻击力上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 1级召唤兽物理攻击力下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 1级召唤兽法术攻击力上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 1级召唤兽法术攻击力下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 1级召唤兽移动速度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 1级召唤兽物理防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 1级召唤兽法术防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 1级召唤兽物理减伤
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 1级召唤兽法术减伤
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 1级召唤兽HP上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 1级召唤兽HP恢复速率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 1级召唤兽致命几率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 1级召唤兽致命伤害
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 1级召唤兽致命防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 1级召唤兽普攻闪躲值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 1级召唤兽技能残影值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 1级召唤兽忽视闪躲值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 1级召唤兽技能ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)

	// FieldName: 1级召唤兽技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 1级召唤兽物理攻击范围
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,15)

	// FieldName: 1级召唤兽法力攻击范围
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,15)

	// FieldName: 1级召唤兽抗性0
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 1级召唤兽抗性1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 1级召唤兽抗性2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 1级召唤兽抗性3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 1级召唤兽抗性4
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 1级召唤兽抗性5
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 1级召唤兽属性攻击0
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 1级召唤兽属性攻击1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 1级召唤兽属性攻击2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 1级召唤兽属性攻击3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 1级召唤兽属性攻击4
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 1级召唤兽属性攻击5
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 1级召唤兽攻击加成
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 1级召唤兽防御加成
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 1级召唤兽生命加成
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 2级召唤兽名称颜色
	// FieldType: INT4
	// Type:	  COMBOSINGLE(白色=0,绿色,蓝色)

	// FieldName: 2级召唤兽攻击类型
	// FieldType: INT4
	// Type:	  COMBOENUM(ATTACK_TYPE)

	// FieldName: 2级召唤兽物理攻击力上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 2级召唤兽物理攻击力下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 2级召唤兽法术攻击力上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 2级召唤兽法术攻击力下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 2级召唤兽移动速度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 2级召唤兽物理防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 2级召唤兽法术防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 2级召唤兽物理减伤
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 2级召唤兽法术减伤
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 2级召唤兽HP上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 2级召唤兽HP恢复速率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 2级召唤兽致命几率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 2级召唤兽致命伤害
	// FieldType: INT4
	// Type:	 EDITNUMBER(0,1000000)

	// FieldName: 2级召唤兽致命防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 2级召唤兽普攻闪躲值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 2级召唤兽技能残影值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 2级召唤兽忽视闪躲值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 2级召唤兽技能ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)

	// FieldName: 2级召唤兽技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 2级召唤兽物理攻击范围
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,15)

	// FieldName: 2级召唤兽法力攻击范围
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,15)

	// FieldName: 2级召唤兽抗性0
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 2级召唤兽抗性1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 2级召唤兽抗性2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 2级召唤兽抗性3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 2级召唤兽抗性4
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 2级召唤兽抗性5
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 2级召唤兽属性攻击0
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 2级召唤兽属性攻击1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 2级召唤兽属性攻击2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 2级召唤兽属性攻击3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 2级召唤兽属性攻击4
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 2级召唤兽属性攻击5
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 2级召唤兽攻击加成
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 2级召唤兽防御加成
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 2级召唤兽生命加成
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 3级召唤兽名称颜色
	// FieldType: INT4
	// Type:	  COMBOSINGLE(白色=0,绿色,蓝色)

	// FieldName: 3级召唤兽攻击类型
	// FieldType: INT4
	// Type:	  COMBOENUM(ATTACK_TYPE)

	// FieldName: 3级召唤兽物理攻击力上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 3级召唤兽物理攻击力下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 3级召唤兽法术攻击力上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 3级召唤兽法术攻击力下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 3级召唤兽移动速度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 3级召唤兽物理防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 3级召唤兽法术防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 3级召唤兽物理减伤
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 3级召唤兽法术减伤
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 3级召唤兽HP上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 3级召唤兽HP恢复速率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 3级召唤兽致命几率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 3级召唤兽致命伤害
	// FieldType: INT4
	// Type:	 EDITNUMBER(0,1000000)

	// FieldName: 3级召唤兽致命防御
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,9999)

	// FieldName: 3级召唤兽普攻闪躲值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 3级召唤兽技能残影值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 3级召唤兽忽视闪躲值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 3级召唤兽技能ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)

	// FieldName: 3级召唤兽技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 3级召唤兽物理攻击范围
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,15)

	// FieldName: 3级召唤兽法力攻击范围
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,15)

	// FieldName: 3级召唤兽抗性0
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 3级召唤兽抗性1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 3级召唤兽抗性2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 3级召唤兽抗性3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 3级召唤兽抗性4
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 3级召唤兽抗性5
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 3级召唤兽属性攻击0
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 3级召唤兽属性攻击1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 3级召唤兽属性攻击2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 3级召唤兽属性攻击3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 3级召唤兽属性攻击4
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 3级召唤兽属性攻击5
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 3级召唤兽攻击加成
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 3级召唤兽防御加成
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 3级召唤兽生命加成
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	CPetData mPetData[ 3 ];

	// FieldName: 召唤兽攻击频率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int mAttackFrequency;

public:
	void SetFromPB( bool bServer, PBSummonBeast* pBeast );
	void CreateTlvPB( bool bServer, PBSummonBeast* pBeast );
	int Initialize();
	CTemplateSummonBeast() 
	{
		Initialize();
	}
	~CTemplateSummonBeast() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 新手配置				## 模版名称
// version	= 1					## 版本号
// path		= Config/新手配置			## 路径
// parent	=					## 父亲模版
// ********************************************************************** //

class CTemplateBeginner : public CTemplate
{
public:

	// FieldName: 身上物品ID1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 身上物品ID2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 身上物品ID3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mEquipItemID[ 3 ];

	// FieldName: 身上物品数量1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 身上物品数量2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 身上物品数量3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mEquipItemNum[ 3 ];

	// FieldName: 包裹物品ID1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 包裹物品ID2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 包裹物品ID3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mBaggageItemID[ 3 ];

	// FieldName: 包裹物品数量1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 包裹物品数量2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	// FieldName: 包裹物品数量3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mBaggageItemNum[ 3 ];

	// FieldName: 技能ID1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 技能ID2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 技能ID3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mSkillID[ 3 ];

	// FieldName: 技能等级1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 技能等级2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)

	// FieldName: 技能等级3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mSkillLevel[ 3 ];

	// FieldName: 力量点
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mStrNum;

	// FieldName: 体质点
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mConNum;

	// FieldName: 耐力点
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mStaNum;

	// FieldName: 智力点
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mWisNum;

	// FieldName: 精神点
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mSprNum;

	// FieldName: 技能点
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mSkillNum;

	// FieldName: 出生X坐标1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mPosX1;

	// FieldName: 出生X坐标2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mPosX2;

	// FieldName: 出生Y坐标1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mPosY1;

	// FieldName: 出生Y坐标2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mPosY2;

	// FieldName: 转战士第1项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 转战士第2项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 转战士第3项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	int mSkillID1[ 3 ];

	// FieldName: 转战士第1项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 转战士第2项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 转战士第3项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	int mSkillLevel1[ 3 ];

	// FieldName: 转剑客第1项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 转剑客第2项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 转剑客第3项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	int mSkillID2[ 3 ];

	// FieldName: 转剑客第1项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 转剑客第2项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 转剑客第3项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	int mSkillLevel2[ 3 ];

	// FieldName: 转猎人第1项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 转猎人第2项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 转猎人第3项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	int mSkillID3[ 3 ];

	// FieldName: 转猎人第1项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 转猎人第2项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 转猎人第3项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	int mSkillLevel3[ 3 ];

	// FieldName: 转法师第1项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 转法师第2项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 转法师第3项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	int mSkillID4[ 3 ];

	// FieldName: 转法师第1项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 转法师第2项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 转法师第3项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	int mSkillLevel4[ 3 ];

	// FieldName: 转道士第1项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 转道士第2项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 转道士第3项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	int mSkillID5[ 3 ];

	// FieldName: 转道士第1项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 转道士第2项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 转道士第3项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	int mSkillLevel5[ 3 ];

	// FieldName: 转幻术师第1项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 转幻术师第2项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 转幻术师第3项初始技能
	// FieldType: INT4
	// Type:	  SKILLPATH

	int mSkillID6[ 3 ];

	// FieldName: 转幻术师第1项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 转幻术师第2项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	// FieldName: 转幻术师第3项初始技能等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,50)

	int mSkillLevel6[ 3 ];

	// FieldName: 新手保护等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,160)
	int mBeginnerProtectLevel;

	// FieldName: 跨场景保护时间
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,3600000)
	int mTransProtectTime;

	// FieldName: 出生地图
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,50)
	int mMapID;

	// FieldName: 玩家死亡时装备所掉耐久度占装备全部耐久度的万分比
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int mDeathDuraRate;	

	// FieldName: 修理装备时是否能使用绑定金钱(0表示可以1表示不可以)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1)
	int mCanMendUseBindMoney;	

	// FieldName: 出生时活力值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mBeginnerAP;
public:
	void SetFromPB( bool bServer, PBBeginner* pBeginner );
	void CreateTlvPB( bool bServer, PBBeginner* pBeginner );
	int Initialize();
	CTemplateBeginner() 
	{
		Initialize();
	}
	~CTemplateBeginner() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 道具排列顺序	## 模版名称
// version	= 1				## 版本号
// path		= Config/道具排列顺序	## 路径
// parent	=				## 父亲模版
// ********************************************************************** //

class CItemSequence : public CTemplate 
{
public:

	// FieldName: 道具类型%d
	// FieldType: INT4
	// Type:	  COMBOENUM(ITEMTYPE)
	int mSequence[20];	

public:
	void SetFromPB( bool bServer, PBItemSequence* pItem );
	void CreateTlvPB( bool bServer, PBItemSequence* pItem );
	int Initialize();
	CItemSequence() 
	{
		Initialize();
	}
	~CItemSequence() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 掉落等级			## 模版名称
// version	= 1					## 版本号
// path		= Config/掉落等级	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //

class CTemplateDropLevel : public CTemplate
{
public:

	// FieldName: 填写%d级掉%d的概率
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000)
	int     mRan[ 16 ][ 16 ];
	
public:
	void SetFromPB( bool bServer, PBDropLevel* pLevel );
	void CreateTlvPB( bool bServer, PBDropLevel* pLevel );
	int Initialize();
	CTemplateDropLevel()
	{
		Initialize();
	}
	~CTemplateDropLevel() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= 怪物转换表			## 模版名称
// version	= 1						## 版本号
// path		= Config/怪物转换表		## 路径
// parent	=						## 父亲模版
// ********************************************************************** //

class CTemplateOgreTran : public CTemplate
{
public:

	// FieldName: 第%d级的掉落表
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int     mDrop[ TMP_MAX_GRADE ];

	// FieldName: 第%d级的HP
	// FieldType: INT4
	// Type:      EDITNUMBER(0,1000000000)
	int     mHP[ TMP_MAX_GRADE ];

	// FieldName: 第%d级的MP
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int     mMP[ TMP_MAX_GRADE ];

	// FieldName: 第%d级的物攻
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int     mPA[ TMP_MAX_GRADE ];

	// FieldName: 第%d级的法攻
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int     mMA[ TMP_MAX_GRADE ];

	// FieldName: 第%d级的物防
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int     mPD[ TMP_MAX_GRADE ];

	// FieldName: 第%d级的法防
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int     mMD[ TMP_MAX_GRADE ];

	// FieldName: 第%d级的命中
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int     mHit[ TMP_MAX_GRADE ];

	// FieldName: 第%d级的残影
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int     mDodge[ TMP_MAX_GRADE ];

	// FieldName: 第%d级的忽闪
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int     mIgnore[ TMP_MAX_GRADE ];

	// FieldName: 第%d级的暴防
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int     mDeath[ TMP_MAX_GRADE ];

	// FieldName: 第%d级的攻击%d
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int     mResAtt[ TMP_MAX_GRADE ][ 7 ];	

	// FieldName: 第%d级的抗性%d
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int     mResDef[ TMP_MAX_GRADE ][ 7 ];	

	// FieldName: 第%d级的经验
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int     mExp[ TMP_MAX_GRADE ];

public:
	void SetFromPB( bool bServer, PBOgreTran* pOgre );
	void CreateTlvPB( bool bServer, PBOgreTran* pOgre );
	int Initialize();
	CTemplateOgreTran() 
	{
		Initialize();
	}
	~CTemplateOgreTran() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= (职业)属性表			## 模版名称
// version	= 1						## 版本号
// path		= Config/(职业)属性表	## 路径
// parent	=						## 父亲模版
// ********************************************************************** //

class CTemplateMetierProperty : public CTemplate
{
public:

	// FieldName: 职业限制
	// FieldType: INT4
	// Type:	  COMBOSINGLE(任意=0,战士,剑客,猎人,法师,道士,幻术师,新手)
	int		mMetierRequire;

	// FieldName: 类型选项
	// FieldType: INT4
	// Type:	  CHECKMUTI(当前增加=0,当前总值)
	int		mOption;

	// FieldName: 填写等级%d对应第%d项属性的值
	// FieldType: INT4
	// Type:      EDITNUMBER(0,99999)
	int     mMetierProperty[ TMP_MAX_GRADE ][ MAX_PROPERTY ];

public:
	void SetFromPB( bool bServer, PBMetierPro* pMetier );
	void CreateTlvPB( bool bServer, PBMetierPro* pMetier );
	int Initialize();
	CTemplateMetierProperty()
	{
		Initialize();
	}
	~CTemplateMetierProperty() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= (等级)属性表			## 模版名称
// version	= 1						## 版本号
// path		= Config/(等级)属性表	## 路径
// parent	=						## 父亲模版
// ********************************************************************** //

class CTemplateLevelProperty : public CTemplate
{
public:

	// FieldName: 类型选项
	// FieldType: INT4
	// Type:	  CHECKMUTI(当前增加=0,当前总值)
	int		mOption;

	// FieldName: 等级%d对应的属性值
	// FieldType: INT4
	// Type:      EDITNUMBER(0,99999)
	int     mLevelProperty[ TMP_MAX_GRADE ];

public:
	void SetFromPB( bool bServer, PBLevelProperty* pLevel );
	void CreateTlvPB( bool bServer, PBLevelProperty* pLevel );
	int Initialize();
	CTemplateLevelProperty()
	{
		Initialize();
	}
	~CTemplateLevelProperty() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= 功勋经验表			## 模版名称
// version	= 1						## 版本号
// path		= Config/功勋经验表		## 路径
// parent	=						## 父亲模版
// ********************************************************************** //
class CTemplateHonorToExp : public CTemplate
{
public:

	// FieldName: 等级%d得到的经验值
	// FieldType: INT4
	// Type:      EDITNUMBER(0,9999999)
	int     mExp[ TMP_MAX_GRADE ];

	// FieldName: 等级%d得到的才华值
	// FieldType: INT4
	// Type:      EDITNUMBER(0,9999999)
	int     mWizard[ MAX_WIZARD_TEMP ];

	// FieldName: 每日兑换经验的荣誉上限
	// FieldType: INT4
	// Type:      EDITNUMBER(0,9999999)
	int		mExpLimit;

	// FieldName: 兑换得到的物品
	// FieldType: INT4
	// Type:      EDITNUMBER(0,9999999)
	int		mItemID;

	// FieldName: 兑换物品所需荣誉
	// FieldType: INT4
	// Type:      EDITNUMBER(0,9999999)
	int		mItemHonor;

	// FieldName: 每日兑换物品的荣誉上限
	// FieldType: INT4
	// Type:      EDITNUMBER(0,9999999)
	int		mItemLimit;

public:
	void SetFromPB( bool bServer, PBHonorToExp* pHonor );
	void CreateTlvPB( bool bServer, PBHonorToExp* pHonor );
	int Initialize();
	CTemplateHonorToExp()
	{
		Initialize();
	}
	~CTemplateHonorToExp(){}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= 功勋获得表			## 模版名称
// version	= 1						## 版本号
// path		= Config/功勋获得表		## 路径
// parent	=						## 父亲模版
// ********************************************************************** //
class CTemplateHonorObtain : public CTemplate
{
public:

	// FieldName: 最大功勋上限
	// FieldType: INT4
	// Type:      EDITNUMBER(0,999999999)
	int     mInLimit;

	// FieldName: 每天能提供的功勋次数
	// FieldType: INT4
	// Type:      EDITNUMBER(-99999,9999999)
	int     mOutLimit;

	// FieldName: 在外国死亡得到的功勋
	// FieldType: INT4
	// Type:      EDITNUMBER(-99999,99999)
	int     mOutDieExp;

	// FieldName: 在本国死亡得到的功勋
	// FieldType: INT4
	// Type:      EDITNUMBER(-99999,99999)
	int     mInDieExp;

	// FieldName: 在中立死亡得到的功勋
	// FieldType: INT4
	// Type:      EDITNUMBER(-99999,99999)
	int     mBorderDieExp;

	// FieldName: 等级低于目标%d在外国得到的功勋
	// FieldType: INT4
	// Type:      EDITNUMBER(-99999,99999)
	int     mOutLowExp[ TMP_MAX_GRADE ];

	// FieldName: 等级高于目标%d在外国得到的功勋
	// FieldType: INT4
	// Type:      EDITNUMBER(-99999,99999)
	int     mOutHighExp[ TMP_MAX_GRADE ];

	// FieldName: 等级低于目标%d在本国得到的功勋
	// FieldType: INT4
	// Type:      EDITNUMBER(-99999,99999)
	int     mInLowExp[ TMP_MAX_GRADE ];

	// FieldName: 等级高于目标%d在本国得到的功勋
	// FieldType: INT4
	// Type:      EDITNUMBER(-99999,99999)
	int     mInHighExp[ TMP_MAX_GRADE ];

	// FieldName: 等级低于目标%d在中立得到的功勋
	// FieldType: INT4
	// Type:      EDITNUMBER(-99999,99999)
	int     mBorderLowExp[ TMP_MAX_GRADE ];

	// FieldName: 等级高于目标%d在中立得到的功勋
	// FieldType: INT4
	// Type:      EDITNUMBER(-99999,99999)
	int     mBorderHighExp[ TMP_MAX_GRADE ];

	// FieldName: 等级低于目标%d丢失的功勋
	// FieldType: INT4
	// Type:      EDITNUMBER(-99999,99999)
	int     mLostLowExp[ TMP_MAX_GRADE ];

	// FieldName: 等级高于目标%d丢失的功勋
	// FieldType: INT4
	// Type:      EDITNUMBER(-99999,99999)
	int     mLostHighExp[ TMP_MAX_GRADE ];

	// FieldName: %d级基础功勋
	// FieldType: INT4
	// Type:      EDITNUMBER(-99999,99999)
	int     mBasicExp[ TMP_MAX_GRADE ];

public:
	void SetFromPB( bool bServer, PBHonorObtain* pObtain );
	void CreateTlvPB( bool bServer, PBHonorObtain* pObtain );
	int Initialize();
	CTemplateHonorObtain()
	{
		Initialize();
	}
	~CTemplateHonorObtain() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= 双倍时间表			## 模版名称
// version	= 1						## 版本号
// path		= Config/双倍时间表	 ## 路径
// parent	=						## 父亲模版
// ********************************************************************** //

class CTemplateDiploidTimeTable : public CTemplate
{
public:

	// FieldName: 每周可领取的时间(毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mTotalTime;

	// FieldName: 领取方式个数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int mTypeNumber;

	// FieldName: 有效双倍上限(毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mValidMax;

	// FieldName: 半个小时(毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000) 

	// FieldName: 1个小时(毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)

	// FieldName: 2个小时(毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)

	// FieldName: 3个小时(毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)

	// FieldName: 4个小时(毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)

	int mBuffID[ 5 ];

public:
	void SetFromPB( bool bServer, PBDiploidTable* pTable );
	void CreateTlvPB( bool bServer, PBDiploidTable* pTable );
	int Initialize();
	CTemplateDiploidTimeTable()
	{
		Initialize();
	}
	~CTemplateDiploidTimeTable() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 内丹					 ## 模版名称
// version	= 1						 ## 版本号
// path		= 内丹					 ## 路径
// parent	= 道具基本信息			 ## 父亲模版
// ********************************************************************** //
class CTplRedStone: public CTplItem
{
public:
	enum ERedStoneType
	{
		TYPE_TEAM				=	1,		//	队伍内丹
		TYPE_FAMILY				=	2,		//	家族内丹
		TYPE_FAMILYREPETION		=	3,		//	家族副本内丹
		TYPE_CAMP				=	4,		//	阵营内丹
		TYPE_COUNTRY			=	5,		//	国家内丹
		TYPE_MARRIAGE			=	6,		//  结婚内丹
		TYPE_SERVER				=	7,		//	全服内丹
		TYPE_METIER				=	8,		//  门派内丹			
	};	
	
	// FieldName: 噬炼图标名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mMagicIconName[ STRING32 ];			
		
	// FieldName: 开启噬炼的进度条时间(毫秒)
	// FieldType: INT4
	// Type:      EDITNUMBER(0,100000)
	unsigned int   mGuageTime;

	// FieldName: 噬炼的时间(毫秒)
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	unsigned int   mMagicSustainTime;

	// FieldName:  噬炼时分配经验的时间间隔(毫秒)
	// FieldType: INT4
	// Type:      EDITNUMBER(0,100000)
	unsigned int   mExpTime;

	// FieldName:  噬炼时的作用半径
	// FieldType: INT4
	// Type:      EDITNUMBER(0,100000)
	unsigned int   mRadius;
	
	// FieldName: 内丹类型
	// FieldType: INT4
	// Type:      COMBOENUM(ERedStoneType)
	unsigned int   mRedStoneType;
	
	// FieldName:  家族内丹的经验加成
	// FieldType: INT4
	// Type:      EDITNUMBER(0,100000)
	unsigned int   mFamilyExpRate;
	
	// FieldName: 内丹经验表ID
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	unsigned int   mExpTableID;	  	

	// FieldName: 人数是否影响经验分配( 0 )
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	unsigned int   mNumberEffected;
	
	// FieldName: 内丹所属的门派ID
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	unsigned int   mMetierID;

	// FieldName: 必需的第%d种buffid
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	unsigned int   mKeyBuffID[ 20 ];

	// FieldName: 第%d种加成buffid
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	unsigned int   mEncBuffID[ 20 ];

	// FieldName: 第%d种加成百分比
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	unsigned int   mEncPercent[ 20 ];

	// FieldName: 是否是永久(慎用)
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是)
	unsigned int  mIsEternal;
public:
	void SetFromPB( bool bServer, PBRedStone* pStone );
	void CreateTlvPB( bool bServer, PBRedStone* pStone );
	int Initialize();
	CTplRedStone()
	{
		Initialize();
	}
	~CTplRedStone() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 物品适配器			 ## 模版名称
// version	= 1						 ## 版本号
// path		= Config/物品适配器	 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //
class CTplItemAdapter : public CTemplate 
{
public:

	// FieldName: 模板类型
	// FieldType: INT4
	// Type:	  COMBOENUM(ITEMTYPE)
	int		mItemType;		

	// FieldName: 真实类型
	// FieldType: INT4
	// Type:	  COMBOENUM(ADAPTERTYPE)
	int		mRealType;

	// FieldName: 真实模板
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mRealTempID;

	// FieldName: 有效的最大玩家>怪物等级差(0表示不限制)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,200)
	int		mMaxLevelMinus;

	// FieldName: 物品参数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mParam;

	// FieldName: 最大值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mMaxValue;

	// FieldName: 最小值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mMinValue;
public:
	void SetFromPB( bool bServer, PBItemAdapter* pAdapter );
	void CreateTlvPB( bool bServer, PBItemAdapter* pAdapter );
	int Initialize();
	CTplItemAdapter() 
	{
		Initialize();
	}
	~CTplItemAdapter() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 内丹等级经验表		 ## 模版名称
// version	= 1						 ## 版本号
// path		= Config/内丹等级经验表	 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //
class CTemplateRedStoneExpTable : public CTemplate
{
public:

	// FieldName: 使用内丹时%d级玩家每次获得的经验值
	// FieldType: INT4
	// Type:      EDITNUMBER(0,2100000000)
	unsigned int   mExp[ TMP_MAX_GRADE ];	

	// FieldName: 玩家噬炼特效名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char		   mEffectName[ STRING32 ];

public:
	void SetFromPB( bool bServer, PBRedStoneExpTable* pTable );
	void CreateTlvPB( bool bServer, PBRedStoneExpTable* pTable );
	int Initialize();
	CTemplateRedStoneExpTable() 
	{
		Initialize();
	}
	~CTemplateRedStoneExpTable() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //






// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= 复活花费表			## 模版名称
// version	= 1						## 版本号
// path		= Config/复活花费表	## 路径
// parent	=						## 父亲模版
// ********************************************************************** //
class CTemplateReliveTable : public CTemplate
{
public:

	// FieldName: 等级%d复活花费金钱数
	// FieldType: INT4
	// Type:      EDITNUMBER(0,99999999)
	int     mFee[ TMP_MAX_GRADE ];

	// FieldName: 复活金钱系数
	// FieldType: INT4
	// Type:      EDITNUMBER(0,99999999)
	int     mRate;

public:
	void SetFromPB( bool bServer, PBReliveTable* pTable );
	void CreateTlvPB( bool bServer, PBReliveTable* pTable );
	int Initialize();
	CTemplateReliveTable() 
	{
		Initialize();
	}
	~CTemplateReliveTable() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //




// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 答题等级经验表		 ## 模版名称
// version	= 1						 ## 版本号
// path		= Config/答题等级经验表	 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //
class CTemplateQuestionExpTable : public CTemplate
{
public:

	
	// FieldName: 每日福利npc处的题目个数
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,100)
	int			  mNpcQuestionNum;			
	
	// FieldName:  答题时等级为%d的玩家获得的经验
	// FieldType: INT4
	// Type:      EDITNUMBER(0,100000)
	int   mExp[TMP_MAX_GRADE];	

	// FieldName: 每日福利题目经验加成倍数(基础经验的多少倍)
	// FieldType: INT4
	// Type:	  EDITNUMBER(1,100)
	int			mNpcExpAddRate;
	
public:
	void SetFromPB( bool bServer, PBQuestionExp* pExp );
	void CreateTlvPB( bool bServer, PBQuestionExp* pExp );
	int Initialize();
	CTemplateQuestionExpTable() 
	{
		Initialize();
	}
	~CTemplateQuestionExpTable() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 绑定费用表			 ## 模版名称
// version	= 1						 ## 版本号
// path		= Config/绑定费用表		 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //
class CTplBindChargeTable : public CTemplate
{
public:

	// FieldName: 绑定装备需要花费的金钱
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			  mMoneyBindNeed;	
	
	// FieldName: 绑定装备是否支持绑定金钱 1表示支持0表示不支持
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1)
	int			  mCanBindUseBM;	
	
	// FieldName: 取消装备绑定需要的金钱
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			  mMoneyUnBindNeed;		
	
	// FieldName: 取消装备是否支持绑定金钱 1表示支持0表示不支持
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1)
	int			  mCanUnBindUseBM;	
	
	// FieldName: 恢复绑定需要的金钱
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			  mMoneyResumeBindNeed;	
	
	// FieldName:  恢复绑定是否支持绑定金钱 1表示支持0表示不支持
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1)
	int			  mCanResumeBindUseBM;		
	
public:
	void SetFromPB( bool bServer, PBBindChargeTable* pTable );
	void CreateTlvPB( bool bServer, PBBindChargeTable* pTable );
	int Initialize();
	CTplBindChargeTable()
	{
		Initialize();
	}
	~CTplBindChargeTable() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 藏宝图				 ## 模版名称
// version	= 1						 ## 版本号
// path		= 物品/藏宝图			 ## 路径
// parent	= 道具基本信息			 ## 父亲模版
// ********************************************************************** //

class CMinePos
{
public:
	int		mMapID;	
	int		mPosX;
	int		mPosY;
	int		mProbability;
};

class CTplMineMap : public CTplItem
{
public:

	enum EVENT_TYPE
	{
		EVENT_DROPITEM = 0,			// 掉落物品
		EVENT_DESBUFF  = 1,				// 负面BUFF
		EVENT_BRUSHOGRE= 2,			// 附近区域刷出N个与玩家等级相同的怪
		EVENT_BRUSHOGREANDNOTIFY  = 3,	// 附近区域刷出N个与玩家等级相同的怪并通知玩家
		EVENT_BRUSHSPECIALOGRE	  = 4,		// 附近区域刷出N个特定级别怪并通知玩家
		EVENT_LOGINREPETION		  = 5,		// 进入副本
	};

	class CMineEvent
	{
	public:
		EVENT_TYPE	mEventID;	// 事件ID
		int		mTouchRate;	// 事件触发概率
	};

	// FieldName: 第%d个挖掘点MapID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 第%d个挖掘点x坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 第%d个挖掘点y坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 第%d个挖掘点概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	CMinePos	mPos[ MAX_MINEPOS ];

	// FieldName: 第%d个概率事件
	// FieldType: INT4
	// Type:	  COMBOENUM(EVENT_TYPE)

	// FieldName: 第%d个事件触发概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	CMineEvent	mEvent[ MAX_MINEEVENT ];

	// FieldName: 使用半径
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	int			mRadius;

	// FieldName: 掉落表
	// FieldType: INT4
	// Type:	  TEMPPATH

	int			mDropTable;

	// FieldName: 负面BUFF
	// FieldType: INT4
	// Type:	  TEMPPATH

	int			mDecBuff;

	// FieldName: 怪物ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	int			mOgreID;

	// FieldName: 怪物数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)

	int			mOgreNum;

	// FieldName: 副本MapID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	int			mMapID;

	// FieldName: 进入副本后的坐标X
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	int			mPosX;
	
	// FieldName: 进入副本后的坐标Y
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	int			mPosY;

	// FieldName: 是否是特殊藏宝图
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1)
	int			mSpecial;
	
public:
	void SetFromPB( bool bServer, PBItemMineMap* pItem );
	void CreateTlvPB( bool bServer, PBItemMineMap* pItem );
	int Initialize();
	CTplMineMap() 
	{
		Initialize();
	}
	~CTplMineMap() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 惩妖令牌				 ## 模版名称
// version	= 1						 ## 版本号
// path		= 物品/惩妖令牌		 ## 路径
// parent	= 道具基本信息			 ## 父亲模版
// ********************************************************************** //

class CTplToken : public CTplItem
{
public:


	// FieldName: 第%d个召唤怪物点MapID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 第%d个召唤怪物点x坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 第%d个召唤怪物点y坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 第%d个召唤怪物点概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	CMinePos	mPos[ MAX_MINEPOS ];

	// FieldName: 使用半径
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mRadius;

	// FieldName: 怪物ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mOgreID;

	// FieldName: 怪物数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int			mOgreNum;

public:
	void SetFromPB( bool bServer, PBItemToken* pItem );
	void CreateTlvPB( bool bServer, PBItemToken* pItem );
	int Initialize();
	CTplToken()
	{
		Initialize();
	}
	~CTplToken() {}
};



// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			 ## 代表生成到数据库中
// name		= 红包			 ## 模版名称
// version	= 1				 ## 版本号
// path		= 物品/红包		 ## 路径
// parent	= 道具基本信息	 ## 父亲模版
// ********************************************************************** //
class CTplItemRedPaper : public CTplItem
{
public:
	enum EM_REDPAPERTYPE
	{
		REDPAPER_FAMILY = 1,	// 家族红包
		REDPAPER_CORPS	= 2,	// 军团红包
	};
public:
	class CPaperValue 
	{
	public:
		int mValue;
		int mProbality;
	};
public:		
	// FieldName: 购买金额
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mBaseCharge;

	// FieldName: 红包可能开出的第%d个非绑定金钱的数量
	// FieldType: INT4	
	// Type:      EDITNUMBER(0,10000000)	   

	// FieldName: 红包可能开出的第%d个非绑定金钱的概率
	// FieldType: INT4	
	// Type:      EDITNUMBER(0,10000000) 
	CPaperValue mMoney[MAX_PAPERVALUE_NUM];
	
	// FieldName: 红包可能开出的第%d个绑定金钱的数量
	// FieldType: INT4	
	// Type:      EDITNUMBER(0,10000000)	   

	// FieldName: 红包可能开出的第%d个绑定金钱的概率
	// FieldType: INT4	
	// Type:      EDITNUMBER(0,10000000) 
	CPaperValue mBindMoney[MAX_PAPERVALUE_NUM];
	
	// FieldName: 红包类型
	// FieldType: INT4
	// Type:	  COMBOENUM(EM_REDPAPERTYPE)
	int		mRedPaperType;
public:
	void SetFromPB( bool bServer, PBItemRedPaper* pItem );
	void CreateTlvPB( bool bServer, PBItemRedPaper* pItem );
	int Initialize();
	CTplItemRedPaper()
	{
		Initialize();
	}
	~CTplItemRedPaper() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false			 ## 代表生成到数据库中
// name		= 黑狗丸		 ## 模版名称
// version	= 1				 ## 版本号
// path		= 物品/黑狗丸	 ## 路径
// parent	= 道具基本信息	 ## 父亲模版
// ********************************************************************** //
class CTplItemBlackPills : public CTplItem
{
public:
	// FieldName: 可以转换的离线时间(分钟)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int			mExchangeTime;	
	
	// FieldName: 等级经验表ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int			mExpTableID;		
public:
	void SetFromPB(  bool bServer, PBItemBlackPills* pItem );
	void CreateTlvPB( bool bServer, PBItemBlackPills* pItem );
	int Initialize();
	CTplItemBlackPills()
	{
		Initialize();
	}
	~CTplItemBlackPills()
	{
		
	}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 神符			 ## 模版名称
// version	= 1						 ## 版本号
// path		= Config/神符		 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //
class CTplHierogram : public CTemplate
{
public:
	enum HIEROGRAM_TYPE
	{
		USE_SKILL		= 0,			//使用技能
	};
public:

	// FieldName: 资源
	// FieldType: STRING32
	// Type:      EDITSTRING
	char		mRes[ 32 ];

	// FieldName: 存在时间
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mAlive;

	// FieldName: 功能
	// FieldType: INT4
	// Type:      COMBOENUM(HIEROGRAM_TYPE)
	HIEROGRAM_TYPE     mType;
	
	// FieldName: 第%d参数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			  mParame[ 2 ];

public:
	void SetFromPB( bool bServer, PBTplHierogram* pTable );
	void CreateTlvPB( bool bServer, PBTplHierogram* pTable );
	int Initialize();
	CTplHierogram()
	{
		Initialize();
	}
	~CTplHierogram() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false						 ## 代表生成到数据库中
// name		= Buff功能免疫列表		   	 ## 模版名称
// version	= 1							 ## 版本号
// path		= Config/Buff功能免疫列表	 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //
class CTplBuffImmunity : public CTemplate
{
public:
	// FieldName:	第%d个免疫状态
	// FieldType:	INT4
	// Type:		COMBOENUM(BUFF_FUN_TYPE)
	CTemplateBuff::BUFF_FUN_TYPE	mBuffImmunity[ 15 ];
public:
	void SetFromPB( bool vServer, PBTplBuffImmunity* pImmunity );
	void CreateTlvPB( bool vServer, PBTplBuffImmunity* pImmunity );
	int Initialize();
	CTplBuffImmunity()
	{
		Initialize();
	}
	~CTplBuffImmunity() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= PK值范围表			 ## 模版名称
// version	= 1						 ## 版本号
// path		= Config/PK值范围表		 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //
class CTplPKValueTable : public CTemplate
{
public:

	enum PK_NAMESTATE
	{
		STATE_WHITENAME = 0,	// 白名
		STATE_AZURY		= 1,			// 浅蓝名
		STATE_BLUE		= 2,				// 蓝名
		STATE_NAVYBLUE	= 3,			// 深蓝名
		STATE_ROSINESS	= 4,			// 浅红色
		STATE_RED		= 5,				// 红名
		STATE_CARMINE	= 6,			// 深红名
		STATE_PKTOPLIMIT= 7,		// PK值上限
	};

	class CPKDrop
	{
	public:

		PK_NAMESTATE	mState;
		int				mEquipDropRate;
		int				mBagDropRate1;	// 掉落1个
		int				mBagDropRate2;	// 掉落2个
		int				mGoPrisonRate;	// 进监狱概率
		int				mPrisonTime;	// 囚禁时间
	};

	// FieldName: 深蓝值(上限)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			  mNavyBlue;	

	// FieldName: 蓝色值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			  mBlue;	

	// FieldName: 浅蓝值域下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			  mCambridgeBlue;	

	// FieldName: 白色值
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,0)
	int			  mWhite;	

	// FieldName: 浅红值
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,0)
	int			  mRosiness;	

	// FieldName: 中红值
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,0)
	int			  mRed;	

	// FieldName: 深红值(上限)
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1000000,0)
	int			  mCarmine;	

	// FieldName: 黄名时间(单位:秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			  mYellowTime;

	// FieldName: 玩家每在线一小时PK值减少量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			  mDecValue;

	// FieldName: 红名%d阶段玩家死亡损失经验百分比
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			  mDecExpRate[ REDNAME_SECTION ];

	// FieldName: %d级玩家每次损失经验上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			  mTopLimitDecExp[ TMP_MAX_GRADE ];

	// FieldName: 红名玩家%d阶段与NPC交易的价格变动百分比(正负表示上调、下调价格)
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,10000)
	int			  mPriceChange[ REDNAME_SECTION ];

	// FieldName: 监狱线ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int			  mLineID;

	// FieldName: 监狱地图ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int			  mMapID;

	// FieldName: 坐标X
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			  mPosX;

	// FieldName: 坐标Y
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			  mPosY;

	// FieldName: 玩家红名状态%d阶段
	// FieldType: INT4
	// Type:	  COMBOENUM(PK_NAMESTATE)

	// FieldName: %d阶段装备掉落概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: %d阶段掉落1个物品的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: %d阶段掉落2个物品的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: %d阶段传进监狱的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: %d阶段监狱囚禁的时间(单位毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)

	CPKDrop		  mDrop[ REDNAME_SECTION ];

	// FieldName: 从监狱释放传送到目标地图ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			  mFreeMapID;

	// FieldName: 从监狱释放传送到目标坐标X
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			  mFreePosX;

	// FieldName: 从监狱释放传送到目标坐标Y
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			  mFreePosY;

	// FieldName: 监狱囚禁时间(单位毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int			  mPrisonTime;

	// FieldName: PK惩罚基础值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int			  mBasePKValue;

	// FieldName: 等级差(%d+1)*10惩罚值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int			  mPKValueLevel[ PRODUCE_LEVEL ];

	// FieldName: 在敌对国家死亡掉落物品概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int			  mDropRateInEnemy;

	// FieldName: 监狱buffID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int			  mPrisonBuffID;

public:
	void SetFromPB( bool bServer, PBPKValueTable* pTable );
	void CreateTlvPB( bool bServer, PBPKValueTable* pTable );
	int Initialize();
	CTplPKValueTable() 
	{
		Initialize();
	}
	~CTplPKValueTable() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 生活技能表		## 模版名称
// version	= 1					## 版本号
// path		= 物品/生活技能表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //

class CTemplateLifeSkill : public CTemplate
{
public:	
	enum ELifSkillKind
	{
		SKILLKIND_COOK			= 1,	// 烹饪
		SKILLKIND_MINERAL		= 2,	// 矿物采集	
	};
public:

	// FieldName: 生活技能类型	
	// FieldType: INT4
	// Type:	  COMBOENUM(EM_LIFESKILL)
	int			  mLifeSkillType; 
	
	// FieldName: 生活技能归属类别
	// FieldType: INT4
	// Type:	  COMBOENUM(ELifSkillKind)
	int			  mLifeSkillKind; 

	// FieldName: 第%d级生活技能技能的学习所需要的熟练度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			  mStudyDegree[MAX_LIFESKILL_LEVEL];
	
	// FieldName: 第%d级生活技能开启的熟练度上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			  mDegreeStarted[MAX_LIFESKILL_LEVEL];
	
	// FieldName: 第%d级生活技能的学习金钱
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			  mStudyMoney[MAX_LIFESKILL_LEVEL];
	
	// FieldName: 学习第%d级生活技能的等级要求
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			  mLevel[MAX_LIFESKILL_LEVEL]; 
	
	// FieldName: 学习第%d级生活技能后能够获取的熟练度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			  mDegreeValue[MAX_LIFESKILL_LEVEL];
	
	// FieldName: $第%d级生活技能的名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char		  mSkillName[MAX_LIFESKILL_LEVEL][ STRING32 ];
	
	// FieldName: 第%d级生活技能给玩家的称号
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			  mCharacterNameID[MAX_LIFESKILL_LEVEL];	
	
	// FieldName: 如果该生活技能是辅修职业的话,能够达到的等级上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			 mSubSkillLevelLimit;
	
public:
	void SetFromPB( bool bServer, PBTemplateLifeSkill* pLifeSkill );
	void CreateTlvPB( bool bServer, PBTemplateLifeSkill* pLifeSkill );
	int Initialize();
	CTemplateLifeSkill()
	{
		Initialize();
	}
	~CTemplateLifeSkill() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 称号系统				 ## 模版名称
// version	= 1						 ## 版本号
// path		= Config/称号系统		 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //

class CTitle 
{
public:
	char mTitle[ STRING32 ];		// 称号
	int  mType;						// 类别
	int  mObtainCondition;			// 获得条件
	int  mSubCondition;				// 条件子项
	int  mColor;					// 称号颜色
	int  mAddProperty;				// 对应属性提升
	int  mIsHidden;					// 称号是否隐藏
	int  mIsReplace;				// 是否直接替换原来的称号
	int  mValidTime;				// 称号有效时间
	char mDescribe[ STRING256 ];	// 描述
	int	 mIsTeamTitle;				// 团体称号类型
	int	 mParm;						// 参数
	char mDetail[ STRING256 ];		// 称号说明
};

class CTplTitleManager : public CTemplate
{
public:
	enum ETitleType
	{
		RANK_PLAYER_INTEGRATE	= 0x1001,		// 排行榜类称号，人物综合实力
		RANK_PLAYER_LEVEL		= 0x1002,		// 排行榜类称号，人物等级
		RANK_MONEY_OWN			= 0x1003,		// 排行榜类称号，拥有财富
		RANK_KILLENEMY_NUM		= 0x1004,		// 排行榜类称号，杀敌数
		RANK_EXPLOIT			= 0x1005,		// 排行榜类称号，功勋
		RANK_PRSNHONER_FB		= 0x1006,		// 排行榜类称号，副本个人荣誉
		RANK_TEAMHONER_FB		= 0x1007,		// 排行榜类称号，副本团队荣誉
		RANK_NUM_DIE			= 0x1008,		// 排行榜类称号，死亡次数
		RANK_RELIFE				= 0x1009,		// 排行榜类称号，原地复活次数
		RANK_TOTAL_TASK			= 0x100A,		// 排行榜类称号，完成任务总数
		RANK_ONLINE_TIME		= 0x100B,		// 排行榜类称号，在线时间
		RANK_KILLOGRE_NUM		= 0x100C,		// 排行榜类称号，杀怪数
		RANK_REFINEFAIL_NUM		= 0x100D,		// 排行榜类称号，精炼失败次数
		RANK_KILLSAMENATION		= 0x100E,		// 排行榜类称号，杀本阵营数
		RANK_DAYANTA			= 0x100F,		// 排行榜类称号，大雁塔积分（全服）
		RANK_LIANHUADONG		= 0x1010,		// 排行榜类称号，莲花洞积分（全服）
		RANK_HUANGFENGDONG		= 0x1011,		// 排行榜类称号，黄蜂洞积分（全服）
		RANK_ACHIVE_POINT		= 0x1012,		// 排行榜类称号，成就点（全服）
		RANK_FAVOR_POINT		= 0x1013,		// 排行榜类称号，好感度（全服）
		RANK_BAD_POINT			= 0x1014,		// 排行榜类称号，猥琐度（全服）
		RANK_FAMILY_PVP			= 0x1015,		// 排行榜类称号，家族PVP积分（全服）
		RANK_CORPS_PVP			= 0x1016,		// 排行榜类称号，军团PVP积分（全服）
		RANK_FAMILY_TAO			= 0x1017,		// 排行榜类称号，家族品仙桃积分（全服）
		RANK_HONOR_ALL_TODAY	= 0x1018,		// 排行榜类称号，今日世界荣誉
		RANK_HONER_MYNATION_TODAY = 0x1019,		// 排行榜类称号，今日阵营荣誉
		RANK_KILL_PROT_NATION_TODAY	= 0x101A,	// 排行榜类称号，今日杀敌数(护卫)
		RANK_KILL_OUT_TODAY			= 0x101B,	// 排行榜类称号，今日杀敌数(出征)
		RANK_KILL_TOTAL_TODAY		= 0x101C,	// 排行榜类称号，今日本周总杀敌
		RANK_CHARM				= 0x101D,		// 排行榜类称号，奇闻异事魅力值
		RANK_FLYFB				= 0x101E,		// 排行榜类称号，腾云大战副本
		RANK_ZHIGUO					= 0x101F,		// 排行榜类称号，治国力
		RANK_TONGSHUAI				= 0x1020,		// 排行榜类称号，统帅力
		RANK_ZHANGONG				= 0x1021,		// 排行榜类称号，战功
		RANK_WISALL					= 0x1022,		// 排行榜类称号，才华总值
		RANK_YUNXIAOTA				= 0x1023,		// 排行榜类称号，云霄塔副本
		RANK_YUNXIAOTA_LV			= 0x1024,		// 排行榜类称号，云霄塔层数
		RANK_SHIDE					= 0x1025,		// 排行榜类称号，师德积分
		RANK_XIANYUANMARK			= 0x1026,		// 排行榜类称号，仙缘积分
		RANK_CHUSHITU				= 0x1027,		// 排行榜类称号，出师徒弟
		RANK_WUDIDONG				= 0x1028,		// 排行榜类称号，无底洞积分
		RANK_GLOBALWAR				= 0x1029,		// 排行榜类称号，凌云渡积分
		RANK_SPIRIT					= 0x1030,		// 排行榜类称号，铸灵评分
		RANK_CORPSBOSS_SCORE		= 0x1031,		// 排行榜类称号，军团boss积分
		RANK_CORPSCOLLECT_ACTIVE	= 0x1032,		// 排行榜类称号，军团采集活跃度
		RANK_NEW_YUNXIAOTA			= 0x1033,		// 排行榜类称号，云霄塔60层积分（五行幻境副本积分）

		CNDISN_COOKING			= 0x2001,		// 条件类称号，烹饪
		CNDISN_CORPSCLAN		= 0x2002,		// 条件类称号，军团家族
		CNDISN_LIFE				= 0x2003,		// 条件类称号，生活系统
		STATISTIC_TASK			= 0x3001,		// 统计类称号，任务
		STATISTIC_KILLOGRE		= 0x3002,		// 统计类称号，杀怪
		STATISTIC_REFINE		= 0x3003,		// 统计类称号，精炼
		STATISTIC_TRANVEL		= 0x3004,		// 统计类称号，旅行
		STATISTIC_KILLBOSS		= 0x3005,		// 统计类称号，杀BOSS
		STATISTIC_FRIEND		= 0x3006,		// 统计类称号，好友
		STATISTIC_MARRIAGE		= 0x3007,		// 统计类称号，婚姻		
		STATISTIC_YAOMOZHI		= 0x3008,		// 统计类称号，妖魔志
		SPECIAL_KILLBOOS		= 0x4001,		// 特殊类称号，杀BOSS
		SPECIAL_NATION			= 0x4002,		// 特殊类称号，国家类
		SPECIAL_XIANYUAN		= 0x4003,		// 特殊类称号，仙缘类
		MARRIAGE_SPOUSE			= 0x4004,		// 结婚夫妻称号
		TITLETYPE_SWORN			= 0x4005,		// 结拜称号

		TITLETYPE_CAMP			= 0x5000,		// 阵营类称号(后面依次配4个阵营)
		
	};

	enum ECndisnScope
	{
		SCOPE_GLOBAL		= 0,				// 全服务器
		SCOPE_COUNTRY		= 1,						// 本国
		SCOPE_UNBOUNDMONEY  = 2,					// 非绑定金
		SCOPE_TOTAL			= 3,						// 总计
		SCOPE_PROTECT_WEEK  = 4,					// 本周护国
		SCOPE_BATTLE_WEEK	= 5,					// 本周出征
		SCOPE_WORLD_TOTAL	= 6,					// 本周世界总功勋
		SCOPE_COUNTRY_TOTAL = 7,				// 本周本国总功勋
		SCOPE_FB_TOTAL		= 8,						// 副本评价总积分
		SCOPE_PANTAO		= 9,						// 蟠桃劫
		SCOPE_LONGGONG		= 10,						// 龙宫
		SCOPE_TASKFINISH	= 11,					// 完成任务
		SCOPE_KILLOGRENUM	= 12,					// 杀怪数量
		SCOPE_ONLINETIME	= 13,					// 在线时常
		SCOPE_REFINEFAIL	= 14,					// 精炼失败
		SCOPE_FRIENDNUM		= 15,					// 好友数量
		SCOPE_CAIJI			= 16,						// 采集技能
		SCOPE_DIANJIN		= 17,						// 点金技能
		SCOPE_PENGREN		= 18,						// 烹饪技能
		SCOPE_QIAOJIANG		= 19,					// 巧匠技能
		SCOPE_LIANDAN		= 20,						// 炼丹技能
		SCOPE_FIRSTTIME		= 21,					// 杀特殊BOOS
		SCOPE_NATIONALITY	= 22,					// 国籍加入
		SCOPE_OWNNATION		= 23,					// 国家占领
		SCOPE_OWNNORMALMAP	= 24,					// 野外地图占领
	};

	enum ERankLevel
	{
		LEVEL_1		= 0,		// 第1名
		LEVEL_2_4	= 1,				// 第2-4名
		LEVEL_5_10	= 2,				// 第5-10名
		TASK_10		= 3,				// 完成10级之前所有任务
		TASK_20		= 4,				// 完成20级之前所有任务
		TASK_30		= 5,				// 完成30级之前所有任务
		TASK_40		= 6,				// 完成40级之前所有任务
		TASK_50		= 7,				// 完成50级之前所有任务
		TASK_60		= 8,				// 完成60级之前所有任务
		TASK_70		= 9,				// 完成70级之前所有任务
		TASK_80		= 10,				// 完成80级之前所有任务
		TASK_90		= 11,				// 完成90级之前所有任务
		TASK_100	= 12,				// 完成100级之前所有任务
		TASK_110	= 13,				// 完成110级之前所有任务
		KILLOGRE_10000= 14,			// 杀怪数量达到10000只
		KILLOGRE_100000=15,		// 杀怪数量达到100000只
		ONLINE_500	= 16,				// 在线时常达到500小时
		ONLINE_1500	= 17,			// 在线时常达到1500小时
		REFINEFAIL_100= 18,			// 连续精炼失败100次
		FRIEND_100	= 19,				// 好友数量达到100人
		FRIEND_200	= 20,				// 好友数量达到200人
		LEVEL_DASHI = 21,			// 大师级别
		COLLECT_500	= 22,			// 收集点数达到500
		FIRST_TIME	= 23,				// 第一次
		JUNTUANZHANG= 24,			// 军团长
		XIANYUAN_QT	= 25,			// 青铜前辈
		XIANYUAN_BY	= 26,			// 白银前辈
		XIANYUAN_HJ	= 27,			// 黄金前辈
		XIANYUAN_ZS	= 28,			// 钻石前辈
		XIANYUAN_ZZ	= 29,			// 至尊前辈
		LEVEL_2		= 30,			// 第二名
		LEVEL_3		= 31,			// 第三名
	};

	enum EColor
	{
		COLOR_RED			= 0,					// 红色
		COLOR_OGANGE		= 1,					// 橙色
		COLOR_YELLOW		= 2,					// 黄色
		COLOR_GREEN			= 3,					// 绿色
		COLOR_CYAN			= 4,					// 青色
		COLOR_BLUE			= 5,					// 蓝色
		COLOR_PURPLE		= 6,					// 紫色
		COLOR_WHITE			= 7,					// 白色
		COLOR_GRAY			= 8,					// 灰色
		COLOR_COLOR			= 9,					// 彩色
	};	

	enum ETeamType
	{
		TEAM_NONE	= 0,		// 非团体
		TEAM_FAMILY = 1,			// 家族称号
		TEAM_CORPS  = 2,				// 军团称号
		TEAM_UNION  = 3,				// 军联盟称号
		TEAM_NATION = 4,			// 国家称号
		TEAM_CAMP	= 5,				// 阵营称号
	};
	
	// FieldName: $称号名称,第%d个称号
	// FieldType: STRING32
	// Type:	  EDITSTRING

	// FieldName: 类别,第%d个称号
	// FieldType: INT4
	// Type:	  COMBOENUM(ETitleType)

	// FieldName: 获取条件范围,第%d个称号
	// FieldType: INT4
	// Type:　　　COMBOENUM(ECndisnScope)

	// FieldName: 获取子条件,第%d个称号
	// FieldType: INT4
	// Type:　　　COMBOENUM(ERankLevel)

	// FieldName: 称号显示颜色,第%d个称号
	// FieldType: INT4
	// Type:　　　COMBOENUM(EColor)

	// FieldName: 对应属性提升,第%d个称号
	// FieldType: INT4
	// Type:	　EDITNUMBER(0,1000000)

	// FieldName: 称号显示是否隐藏，第%d个称号
	// FieldType: INT4
	// Type:	　COMBOSINGLE(否=0,是=1)

	// FieldName: 是否直接替换原来的称号,第%d个称号
	// FieldType: INT4
	// Type:	　COMBOSINGLE(否=0,是=1)

	// FieldName: 称号有效时间(单位:小时),第%d个称号
	// FieldType: INT4
	// Type:	  EDITNUMBER(-1,100000)

	// FieldName: $称号对应的属性提升描述,第%d个称号
	// FieldType: STRING256
	// Type:	  EDITSTRING

	// FieldName: 团体称号类型,第%d个称号
	// FieldType: INT4
	// Type:	  COMBOENUM(ETeamType)

	// FieldName: 参数,第%d个称号
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)

	// FieldName: $称号说明,第%d个称号
	// FieldType: STRING256
	// Type:	  EDITSTRING

	CTitle		mTitle[ MAX_TITLE ];

public:
	void SetFromPB( bool bServer, PBTitleManager* pManager );
	void CreateTlvPB( bool bServer, PBTitleManager* pManager );	
	int Initialize();
	CTplTitleManager()
	{
		Initialize();
	}
	~CTplTitleManager() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 装备分解表		## 模版名称
// version	= 1					## 版本号
// path		= 物品/装备分解表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
enum EDecompoundResult
{	
	DECOMPOUND_SUCESS		   = 0,
	DECOMPOUND_DEGREENOTENOUGH = 1,	
};

class CTemplateDecompound : public CTemplate
{
public:
	class DecompondTable 
	{		
		public:
			int FstItemID;			// 分解装备生成第一个物品的ID
			int FstProbability;		// 分解装备生成第一个物品的概率
			int FstItemNum;			// 分解装备生成第一个物品的数量
			int SecItemID;			// 分解装备生成第二个物品的ID
			int SecProbability;		// 分解装备生成第二个物品的概率
			int SecItemNum;			// 分解装备生成第二个物品的数量
			int ThirdItemID;		// 分解装备生成第三个物品的ID
			int ThirdProbability;	// 分解装备生成第三个物品的概率
			int ThirdItemNum;		// 分解装备生成第三个物品的数量
			int	DegreeNeeded;		// 分解装备需要的熟练度
	};	
public:
	
	// FieldName: 第%d*5+5级白装分解生成的第一个物品的模板id
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	
	// FieldName: 第%d*5+5级白装分解生成的第一个物品的生成概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	
	// FieldName: 第%d*5+5级白装分解生成的第一个物品的数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	
	// FieldName: 第%d*5+5级白装分解生成的第二个物品的模板id
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	
	// FieldName: 第%d*5+5级白装分解生成的第二个物品的生成概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	
	// FieldName: 第%d*5+5级白装分解生成的第二个物品的数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	
	// FieldName: 第%d*5+5级白装分解生成的第三个物品的模板id
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	
	// FieldName: 第%d*5+5级白装分解生成的第三个物品的生成概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	
	// FieldName: 第%d*5+5级白装分解生成的第三个物品的数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	
	// FieldName: 第%d*5+5级白装分解需要的熟练度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	
	DecompondTable mWhitDecompond[DECOMPOUND_MAXTABLEINDEX];	// 白装的装备分解表
	
	// FieldName: 第%d*5+5级蓝装分解生成的第一个物品的模板id
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 第%d*5+5级蓝装分解生成的第一个物品的生成概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	
	// FieldName: 第%d*5+5级蓝装分解生成的第一个物品的数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 第%d*5+5级蓝装分解生成的第二个物品的模板id
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 第%d*5+5级蓝装分解生成的第二个物品的生成概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	
	// FieldName: 第%d*5+5级蓝装分解生成的第二个物品的数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 第%d*5+5级蓝装分解生成的第三个物品的模板id
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 第%d*5+5级蓝装分解生成的第三个物品的生成概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	
	// FieldName: 第%d*5+5级蓝装分解生成的第三个物品的数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	
	// FieldName: 第%d*5+5级蓝装分解需要的熟练度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	DecompondTable mBlueDecompond[DECOMPOUND_MAXTABLEINDEX];	// 蓝装的装备分解表
	
	// FieldName: 第%d*5+5级紫装分解生成的第一个物品的模板id
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 第%d*5+5级紫装分解生成的第一个物品的生成概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	
	// FieldName: 第%d*5+5级紫装分解生成的第一个物品的数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 第%d*5+5级紫装分解生成的第二个物品的模板id
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 第%d*5+5级紫装分解生成的第二个物品的生成概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	
	// FieldName: 第%d*5+5级紫装分解生成的第二个物品的数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第%d*5+5级紫装分解生成的第三个物品的模板id
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)

	// FieldName: 第%d*5+5级紫装分解生成的第三个物品的生成概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	
	// FieldName: 第%d*5+5级紫装分解生成的第三个物品的数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	
	// FieldName: 第%d*5+5级紫装分解需要的熟练度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	DecompondTable mPurpleDecompond[DECOMPOUND_MAXTABLEINDEX];	// 紫装的装备分解表
	
	// FieldName: 自身熟练度度与分解装备需要的熟练度的第%d个差值区间
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int		mValue[MAX_LIFESKILL_DEGREESTEP];	

	// FieldName: 第%d个的差值区间分解装备获取熟练度的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mProbality[MAX_LIFESKILL_DEGREESTEP];	

	// FieldName: 第%d个的差值区间分解装备获取熟练度的数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int		mDegreeValue[MAX_LIFESKILL_DEGREESTEP];	
	
public:
	void SetFromPB( bool bServer, PBCTemplateDecompound* pCompound );
	void CreateTlvPB( bool bServer, PBCTemplateDecompound* pCompound );
	int Initialize();
	CTemplateDecompound()
	{
		Initialize();
	}
	~CTemplateDecompound() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 造化配置				 ## 模版名称
// version	= 1						 ## 版本号
// path		= Config/造化配置		 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //

class CTemplateTalent : public CTemplate
{
public:
	class CStep
	{
	public:
		int			mStageID;
		char		mStageDesc[ 32 ];
		int			mSkillTree;
		int			mNext[ STEP_SWITCH ];
	  //int			mExp;
		int			mMaxPoint;
		int			mLayer;
	};


	// FieldName: 第%d个小阶的大阶编号
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: $第%d个小阶的描述
	// FieldType: STRING32
	// Type:      EDITSTRING

	// FieldName: 第%d个小阶的技能树
	// FieldType: INT4
	// Type:	  TEMPPATH

	// FieldName: 第%d个小阶的第1种下一小阶
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 第%d个小阶的第2种下一小阶
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 第%d个小阶的第3种下一小阶
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)	

	// FieldName: 第%d个小阶的最大技能点
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 第%d个大阶所在的层
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	CStep	mStep[ TOTAL_TALENT_STAGE ];	

	// FieldName: 第%d个小阶的修为值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int		mExp[ TOTAL_TALENT_POINT ];

public:
	void SetFromPB( bool bServer, PBTemplateTalent* pTalent );
	void CreateTlvPB( bool bServer, PBTemplateTalent* pTalent );
	int Initialize();
	CTemplateTalent()
	{
		Initialize();
	}
	~CTemplateTalent() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 装备兑换表		## 模版名称
// version	= 1					## 版本号
// path		= Config/装备兑换表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTempateEquipExchangeTable : public CTemplate
{
public:
	enum EM_EXCHANGETYPE
	{		
		EXCHANGETYPE_EQUIPMENTCHANGE	=	0,			// 装备兑换
		EXCHANGETYPE_FAMILYEXCHANGE		=	1,			// 家族兑换
		EXCHANGETYPE_CORPSEXCHANGE		=	2,			// 军团兑换
		EXCHANGETYPE_MARRIAGEEXHCNANGE	=	3,			// 结婚的请帖兑换
	}; 
public:

	
	// FieldName: 第%d个配方ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int		mMultiComposeID[MAX_EQUIPCOMPOSEID_NUM];	// 兑换的配方ID
	
	// FieldName: 兑换类型
	// FieldType: INT4
	// Type:	  COMBOENUM(EM_EXCHANGETYPE)
	int		mExchangeType;
	
public:
	void SetFromPB( bool bServer, PBEquipExchangeTable* pTalent );
	void CreateTlvPB( bool bServer, PBEquipExchangeTable* pTalent );
	int Initialize();
	CTempateEquipExchangeTable() 
	{
		Initialize();
	}
	~CTempateEquipExchangeTable() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 组合buff配置表	## 模版名称
// version	= 1					## 版本号
// path		= Config/组合buff配置表 	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //

class CTemplateEvent
{
public:
	int		mEventID;
	int		mTeamFlag;
	int		mDistance;
	int		mBuffID;
    int     mRepetionIndex[3];
};

class CTemplateMultiBuffConfig : public CTemplate
{
public:
	enum EEventType
	{
		EVENT_SHITU = 1,	// 师徒四人
		EVENT_MARRY = 2,		// 婚姻
		EVENT_4METIRE_AWARD = 3,// 4职业组队奖励
		EVENT_5METIRE_AWARD = 4,// 5职业组队奖励
		EVENT_6METIRE_AWARD = 5,// 6职业组队奖励
		EVENT_4METIRE_AWARD_FB = 6,// 4职业副本组队奖励
		EVENT_5METIRE_AWARD_FB = 7,// 5职业副本组队奖励
		EVENT_6METIRE_AWARD_FB = 8,// 6职业副本组队奖励
		EVENT_4SAMEBUFF_AWARD = 9,// 4共同buff组队奖励
		EVENT_5SAMEBUFF_AWARD = 10,// 5共同buff组队奖励
		EVENT_6SAMEBUFF_AWARD = 11,// 6共同buff组队奖励
	};

	// FieldName: 第%d个事件,事件类型
	// FieldType: INT4
	// Type:	  COMBOENUM(EEventType)

	// FieldName: 第%d个事件,是否组队
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)

	// FieldName: 第%d个事件,作用距离
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)

	// FieldName: 第%d个事件,效果buffid
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)

	// FieldName: 第%d个事件,副本索引0
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
    
	// FieldName: 第%d个事件,副本索引1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
    
	// FieldName: 第%d个事件,副本索引2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)

	CTemplateEvent mEvent[ MAX_MUTIBUFFEVENT ];

	// FieldName: 第%d事件,第%d个参数
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)

	int		mEventParam[ MAX_MUTIBUFFEVENT ][ MAX_EVENTPARAM ];

	// FieldName: 第%d事件,第%d个buff
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)

	int		mEventBuff[ MAX_MUTIBUFFEVENT ][ MAX_EVENTPARAM ];

public:
	void SetFromPB( bool bServer, PBMultiBuff* pMBuff );
	void CreateTlvPB( bool bServer, PBMultiBuff* pMBuff );
	int Initialize();
	CTemplateMultiBuffConfig()
	{
		Initialize();
	}
	~CTemplateMultiBuffConfig() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 冷却CD表			## 模版名称
// version	= 1					## 版本号
// path		= Config/冷却CD表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateCDGroupTable : public CTemplate
{
public:

	// FieldName: 第%d个冷却组的时间
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mCDTime[ 200 ];	
public:
	void SetFromPB( bool bServer, PBCDGroupTable* pTable );
	void CreateTlvPB( bool bServer, PBCDGroupTable* pTable );
	int Initialize();
	CTemplateCDGroupTable()
	{
		Initialize();
	}
	~CTemplateCDGroupTable() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 活力配置表		## 模版名称
// version	= 1					## 版本号
// path		= Config/活力配置表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //

class CMetierAp
{
public:
	int mMetier ;	// 职业
	int mAp;		// 每秒回复得活力值
};

class CTplApConfig : public CTemplate
{
public:


	// FieldName: 一分钟挂机时间消耗得活力值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mConsumeHook;	

	// FieldName: 世界频道聊天消耗得活力值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mWorldChat;	

	// FieldName: 阵营频道聊天消耗得活力值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mCampChat;	

	// FieldName: 玩家等级每提升一级，活力值上限增长值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mIncApMax;

	// FieldName: 第%d种职业
	// FieldType: INT4
	// Type:	  COMBOENUM(METIER_TYPE)
	
	// FieldName: 第%d种职业对应得活力回复值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)

	CMetierAp mMetierAp[ MAX_METIER ];

	// FieldName: 自动挂机消耗%d类红药
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	int			mRedPhysic[ MAX_PHYSIC_NUM ];

	// FieldName: 自动挂机消耗%d类蓝药
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	int			mBluePhysic[ MAX_PHYSIC_NUM ];

	// FieldName: 自动挂机消耗%d类活力药
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	int			mApPhysic[ MAX_PHYSIC_NUM ];

	// FieldName: 自动挂机消耗%d类法宝法力药
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	int			mFbHpPhysic[ MAX_PHYSIC_NUM ];

	// FieldName: 自动挂机消耗%d类法宝灵气药
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	int			mFbMpPhysic[ MAX_PHYSIC_NUM ];

	// FieldName: 自动挂机消耗%d类金刚钻
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	int			mMendPhysic[ MAX_PHYSIC_NUM ];
	
public:
	void SetFromPB( bool bServer, PBApConfig* pConfig );
	void CreateTlvPB( bool bServer, PBApConfig* pConfig );
	int Initialize();
	CTplApConfig()
	{
		Initialize();
	}
	~CTplApConfig() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 发光配置表		## 模版名称
// version	= 1					## 版本号
// path		= Config/发光配置表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateLightTable : public CTemplate
{
public:


	// FieldName: 第%d级武器的发光值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mLevelPoint[ 12 ];	

	// FieldName: 第%d档宝石的发光值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mJewelPoint[ 8 ];	

	// FieldName: 第%d种武器发光等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mLevelGrade1[ 5 ];	

	// FieldName: 第%d种防具发光等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mLevelGrade2[ 5 ];	

	// FieldName: 第%d种武器宝石发光等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mJewelGrade1[ 5 ];	

	// FieldName: 第%d种防具宝石发光等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mJewelGrade2[ 5 ];	
public:
	void SetFromPB( bool bServer, PBLightTable* pTable );
	void CreateTlvPB( bool bServer, PBLightTable* pTable );
	int Initialize();
	CTemplateLightTable()
	{
		Initialize();
	}
	~CTemplateLightTable() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 才华经验表			## 模版名称
// version	= 1					## 版本号
// path		= Config/才华经验表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateWizardTable : public CTemplate
{
public:

	// FieldName: 第%d级升级的经验
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mExp[ MAX_WIZARD_TEMP ];	

	// FieldName: 算命不变的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mKeepRand;

	// FieldName: 第%d种倾向概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mTrendRand[ 20 ];

	// FieldName: 第%d种倾向ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mTrendResult[ 20 ];

	// FieldName: 算命的价格
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int		mMoney;
public:
	void SetFromPB( bool bServer, PBWizardTable* pTable );
	void CreateTlvPB( bool bServer, PBWizardTable* pTable );
	int Initialize();
	CTemplateWizardTable()
	{
		Initialize();
	}
	~CTemplateWizardTable() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 好感配置表		## 模版名称
// version	= 1					## 版本号
// path		= Config/好感配置表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //

class CJudgeLevel
{
public:
	int		mType;		// 好感、恶感、陌生
	int		mLevelMin;		// 等级临界值下限
	int		mLevelMax;		// 等级临界值上限
	int		mStar;		// 对应星级
	int		mLevelType;	// 级别类型
};

class CTplJudgeConfig : public CTemplate 
{
public:
	enum JudgeTypeTpl
	{
		JUDGE_FAVOR = 0,	// 好感
		JUDGE_BAD   = 1,			// 恶感
	};
	enum JudgeLevel
	{
		JUDGE_BAD_ENEMY = 0,	// 仇视
		JUDGE_BAD_ABHOR = 1,		// 痛恨
		JUDGE_BAD_DETEST = 2,		// 厌恶
		JUDGE_BAD_REPULSION = 3,	// 排斥
		JUDGE_BAD_REBEL = 4,		// 反感
		JUDGE_BAD_COOL  = 5,			// 冷漠
		JUDGE_FAVOR_COOL = 6,		// 陌生
		JUDGE_FAVOR_FIRST = 7,		// 初识
		JUDGE_FAVOR_ACQ = 8,		// 相识
		JUDGE_FAVOR_FRIEND = 9,		// 友好
		JUDGE_FAVOR_BOSOM  = 10,		// 相知
		JUDGE_FAVOR_INTIMITY= 11,	// 亲近
	};
	enum JudgeStarLevel
	{
		JUDGE_STAR_Y1 = 0,		// 一黄星
		JUDGE_STAR_R1 = 1,			// 一红星
		JUDGE_STAR_R1Y1 = 2,		// 一红一黄星
		JUDGE_STAR_R2  = 3,			// 二红星
		JUDGE_STAR_R2Y1= 4,		// 二红一黄星
		JUDGE_STAR_R3  = 5,			// 三红星
	};
public:


	// FieldName: 开启关闭CD时间
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int		mOpenCD;	

	// FieldName: 第%d级别评价类型
	// FieldType: INT4
	// Type:	  COMBOENUM(JudgeTypeTpl)

	// FieldName: 第%d级别等级临界值下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第%d级别等级临界值上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第%d级别对应星级
	// FieldType: INT4
	// Type:	  COMBOENUM(JudgeStarLevel)

	// FieldName: 第%d级别类型
	// FieldType: INT4
	// Type:	  COMBOENUM(JudgeLevel)

	CJudgeLevel		mJudgeLevel[ MAX_JUDGE_LEVEL ];

	// FieldName: 对个人评价上限基准点
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int		mScoreBase;

	// FieldName: 对个人评价上限系数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int		mScoreRate;

	// FieldName: 可评价等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int		mScoreLevel;

public:
	void SetFromPB( bool bServer, PBJudgeConfig* pTable );
	void CreateTlvPB( bool bServer, PBJudgeConfig* pTable );
	int Initialize();
	CTplJudgeConfig()
	{
		Initialize();
	}
	~CTplJudgeConfig() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 才华倾向			## 模版名称
// version	= 1					## 版本号
// path		= 物品/才华倾向		## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateTrend : public CTemplate
{
public:

	// FieldName: $倾向名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mName[ STRING32 ];	

	// FieldName: $倾向描述
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char	mDesc[ STRING256 ];

	// FieldName: 第%d种才华的倾向
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mBonus[ 6 ];

public:
	void SetFromPB( bool bServer, PBTrend* pTable );
	void CreateTlvPB( bool bServer, PBTrend* pTable );
	int Initialize();
	CTemplateTrend() 
	{
		Initialize();
	}
	~CTemplateTrend() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 成就奖励表			 ## 模版名称
// version	= 1						 ## 版本号
// path		= Config/成就奖励表		 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //

class CAward
{
public:
	int		mItemID;	// 道具ID
	int		mItemNum;	// 道具数量
};

class CTplAchieveAward : public CTemplate
{
public:

	// FieldName: 第%d项第%d种奖励道具ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	int		mAwardID[ MAX_ACHIEVE_AWARD ][ MAX_ACHIEVE_ITEM ];

	// FieldName: 第%d项第%d种奖励道具数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int		mAwardNum[ MAX_ACHIEVE_AWARD ][ MAX_ACHIEVE_ITEM ];

	// FieldName: 第%d项要求点数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mPoint[ MAX_ACHIEVE_AWARD ];
	
	// FieldName: 系统邮件ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mSysMailID;

public:
	void SetFromPB( bool bServer, PBAchieveAward* pAchieveAward );
	void CreateTlvPB( bool bServer, PBAchieveAward* pAchieveAward );
	int Initialize();
	CTplAchieveAward()
	{
		Initialize();
	}
	~CTplAchieveAward() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

enum EOfficialFunction
{
	OFFICIAL_FUNCTION_RENSHI			= 0,	// 吏部
	OFFICIAL_FUNCTION_HUKOU				= 1,	// 户部
	OFFICIAL_FUNCTION_LIBIN				= 2,	// 礼部
	OFFICIAL_FUNCTION_GUOFANG			= 3,	// 兵部
	OFFICIAL_FUNCTION_FAYUAN			= 4,	// 刑部
	OFFICIAL_FUNCTION_GONGCHENG			= 5,	// 工部
	OFFICIAL_FUNCTION_KING				= 6,	// 天子
	OFFICIAL_FUNCTION_NUM				= 7,
};


enum EOfficial
{
	OFFICAL_RENSHI			= 0,	// 吏部
	OFFICAL_HUKOU			= 1,	// 户部
	OFFICAL_LIBIN			= 2,	// 礼部
	OFFICAL_GUOFANG			= 3,	// 兵部
	OFFICAL_FAYUAN			= 4,	// 刑部
	OFFICAL_GONGCHENG		= 5,	// 工部
	OFFICAL_NUM				= 6,
};

enum EOfficialLevel
{
	OFFICIAL_LEVEL_NONE		= 0,		// 无
	OFFICIAL_LEVEL_NINE		= 1,		// 九品
	OFFICIAL_LEVEL_EIGHT	= 2,
	OFFICIAL_LEVEL_SEVEN	= 3,
	OFFICIAL_LEVEL_SIX		= 4,
	OFFICIAL_LEVEL_FIVE		= 5,
	OFFICIAL_LEVEL_FOUR		= 6,
	OFFICIAL_LEVEL_THREE	= 7,
	OFFICIAL_LEVEL_TWO		= 8,
	OFFICIAL_LEVEL_ONE		= 9,		// 一品
	OFFICIAL_LEVEL_ZERO		= 10,		// 特殊
	OFFICIAL_LEVEL_KING		= 11,		// 天子
	OFFICIAL_LEVEL_NUM		= 12,
};

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 官职功能			## 模版名称
// version	= 1					## 版本号
// path		= 物品/官职功能		## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplatePower : public CTemplate
{
public:

	enum EPowerType
	{
		POWER_NONE		= 0,  // 无功能
		POWER_SHUT		= 1,  // 禁言
		POWER_PRISON    = 2,  // 监狱
		POWER_ITEM		= 3,  // 物品
		POWER_BUFF	    = 4,  // BUFF
		POWER_TASK		= 5,  // 任务
		POWER_SUMMON	= 6,  // 召唤个人
		POWER_STONE		= 7,  // 内丹
		POWER_FREE		= 8,  // 释放
		POWER_ALL		= 9,  // 国民功能
		POWER_PASSIVE	= 10, // 被动
		POWER_SUMMONWORLD	= 11, // 召唤阵营
		POWER_SUMMONCOUNTRY	= 12, // 召唤国家
		POWER_SUMMONCORPS	= 13, // 召唤军团
		POWER_SUMMONFAMILY	= 14, // 召唤家族
		POWER_MAKEGT	= 15,	// 发布国探
		POWER_MAKELW	= 16,	// 发布乱舞
	};	


	// FieldName: $功能名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mName[ STRING32 ];	

	// FieldName: $功能描述
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char	mDesc[ STRING256 ];

	// FieldName: 功能类型
	// FieldType: INT4
	// Type:	  COMBOENUM(EPowerType)
	int		mType;

	// FieldName: 限制的品级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int		mLevel;

	// FieldName: 所属部门
	// FieldType: INT4
	// Type:	  COMBOSINGLE(吏部=0,户部,礼部,兵部,刑部,工部,天子特权)
	int		mOfficial;	

	// FieldName: 编号顺序
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mOrder;

	// FieldName: 参数1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int		mParam1;

	// FieldName: 参数2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int		mParam2;

	// FieldName: 第%d级的使用次数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mUseTime[ 12 ];

	// FieldName: 需要建设度
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mConstruct;

	// FieldName: 需要声望
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mPower;

	// FieldName: 需要金钱
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mMoney;

	// FieldName: 图标名称
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char	mIcon[ STRING256 ];

	// FieldName: 特殊国家
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无=-1,大唐,乌鸡,车迟,宝象)
	int		mCountry;

	// FieldName: 是否全部计数
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)
	int		mAllCount;

	// FieldName: 起始时间（小时）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,23)
	int		mStartHour;

	// FieldName: 起始时间（分钟）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,59)
	int		mStartMin;

	// FieldName: 结束时间（小时）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,23)
	int		mEndHour;

	// FieldName: 结束时间（分钟）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,59)
	int		mEndMin;


public:
	void SetFromPB( bool bServer, PBPower* pPower );
	void CreateTlvPB( bool bServer, PBPower* pPower );
	int Initialize();
	CTemplatePower()
	{
		Initialize();
	}
	~CTemplatePower() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 官员信息			## 模版名称
// version	= 1					## 版本号
// path		= 物品/官员信息		## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateOfficial : public CTemplate
{
public:

	enum EOfficialConfig
	{
		OFFICIAL_CFG_RENSHI		= 0x0001,		// 吏部
		OFFICIAL_CFG_HUKOU		= 0x0002,		// 户部
		OFFICIAL_CFG_LIBIN		= 0x0004,		// 礼部
		OFFICIAL_CFG_GUOFANG	= 0x0008,		// 兵部
		OFFICIAL_CFG_FAYUAN		= 0x0010,		// 刑部
		OFFICIAL_CFG_GONGCHENG	= 0x0020,		// 工部
	};


	// FieldName: $官职名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mName[ STRING32 ];	

	// FieldName: $官职描述
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char	mDesc[ STRING256 ];

	// FieldName: 官员类型
	// FieldType: INT4
	// Type:	  CHECKMUTI(无=0,吏部,户部,礼部,兵部,刑部,工部)
	int		mType;

	// FieldName: 需要的才华值%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mWizard[ 6 ];

	// FieldName: 非正常任命扣的声望
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mPower;

	// FieldName: 需要的国家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mCountryLevel;

	// FieldName: 需要的建设度等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mConstructLevel;

public:
	void SetFromPB( bool bServer, PBOfficial* pOfficial );
	void CreateTlvPB( bool bServer, PBOfficial* pOfficial );
	int Initialize();
	CTemplateOfficial()
	{
		Initialize();
	}
	~CTemplateOfficial() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 官员配置表			 ## 模版名称
// version	= 1						 ## 版本号
// path		= Config/官员配置表		 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //
class CTemplateOfficialTable : public CTemplate
{
public:

	// FieldName: $第%d品第%d项官职ID
	// FieldType: INT4
	// Type:	  TEMPPATH	

	int	mOfficial[ 12 ][ 6 ];

public:
	void SetFromPB( bool bServer, PBOfficialTable* pTable );
	void CreateTlvPB( bool bServer, PBOfficialTable* pTable );
	int Initialize();
	CTemplateOfficialTable()
	{
		Initialize();
	}
	~CTemplateOfficialTable() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 建设经验表		## 模版名称
// version	= 1					## 版本号
// path		= Config/建设经验表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateConstructExp : public CTemplate
{
public:


	// FieldName: 第%d级升级到下一级的经验数值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,3000000000)
	int	mExp[ MAX_CONSTRUCT_LEVEL ];

	// FieldName: 第%d级建设度对应增长
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,3000000000)
	int	mBonus[ MAX_CONSTRUCT_LEVEL ];

public:
	void SetFromPB( bool bServer, PBConstructExp* pLevelExp );
	void CreateTlvPB( bool bServer, PBConstructExp* pLevelExp );
	int Initialize();
	CTemplateConstructExp()
	{
		Initialize();
	}
	~CTemplateConstructExp() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= PK附加表			## 模版名称
// version	= 1					## 版本号
// path		= Config/PK附加表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateAddPKTable : public CTemplate
{
public:

	// FieldName: 杀死结盟改变天子声望
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,100000)
	int	mKFChangePower;	

	// FieldName: 杀死中立改变天子声望
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,100000)
	int	mKPChangePower;	

	// FieldName: 杀死宣战改变天子声望
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,100000)
	int	mKWChangePower;	

	// FieldName: 被杀死结盟改变天子声望
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,100000)
	int	mDFChangePower;	

	// FieldName: 被杀死中立改变天子声望
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,100000)
	int	mDPChangePower;	

	// FieldName: 被杀死宣战改变天子声望
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,100000)
	int	mDWChangePower;	

	// FieldName: 杀死结盟改变友好度
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,100000)
	int	mKFChangeFriend;	

	// FieldName: 杀死中立改变友好度
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,100000)
	int	mKPChangeFriend;	

	// FieldName: 杀死宣战改变友好度
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,100000)
	int	mKWChangeFriend;

	// FieldName: 杀死结盟改变PK值
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,100000)
	int	mKFChangePK;	

	// FieldName: 杀死宣战改变PK值
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,100000)
	int	mKWChangePK;

	// FieldName: 杀死第%d种中立改变PK值
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,100000)
	int	mKPChangePK[ MAX_PEACE_TYPE ];

	// FieldName: 第%d种中立的友好度下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(-100000,100000)
	int	mPFriendMin[ MAX_PEACE_TYPE ];

	// FieldName: 第%d种中立的友好度上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(-100000,100000)
	int	mPFriendMax[ MAX_PEACE_TYPE ];

	// FieldName: 杀死结盟改变荣誉
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,100000)
	int	mKFChangeHonor;	

	// FieldName: 杀死中立改变荣誉
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,100000)
	int	mKPChangeHonor;	

	// FieldName: 杀死宣战改变荣誉
	// FieldType: INT4
	// Type:	  EDITNUMBER(-10000,100000)
	int	mKWChangeHonor;

public:
	void SetFromPB( bool bServer, PBAddPKTable* pTable );
	void CreateTlvPB( bool bServer, PBAddPKTable* pTable );
	int Initialize();
	CTemplateAddPKTable()
	{
		Initialize();
	}
	~CTemplateAddPKTable() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 地图势力表		## 模版名称
// version	= 1					## 版本号
// path		= Config/地图势力表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateMapTable : public CTemplate
{
public:

	// FieldName: 第%d地图的等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mLevel[ MAX_MAP_NUM ];	

	// FieldName: 第%d地图的势力值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mPower[ MAX_MAP_NUM ];
public:
	void SetFromPB( bool bServer, PBMapTable* pTable );
	void CreateTlvPB( bool bServer, PBMapTable* pTable );
	int Initialize();
	CTemplateMapTable()
	{
		Initialize();
	}
	~CTemplateMapTable() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 群攻伤害表		## 模版名称
// version	= 1					## 版本号
// path		= Config/群攻伤害表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateMultiAttackTable : public CTemplate
{
public:

	// FieldName: 第%d个目标的伤害降低
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mReduce[ 20 ];
public:
	void SetFromPB( bool bServer, PBMultiAttackTable* pTable );
	void CreateTlvPB( bool bServer, PBMultiAttackTable* pTable );
	int Initialize();
	CTemplateMultiAttackTable()
	{
		Initialize();
	}
	~CTemplateMultiAttackTable() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 会员道具			## 模版名称
// version	= 1					## 版本号
// path		= 物品/会员道具		## 路径
// parent	= 道具基本信息		## 父亲模版
// ********************************************************************** //
class CTplItemVip : public CTplItem
{
public:

	enum EVipType
	{
		emVipNone = 0,	// 非会员
		emVipNormal,	// 普通会员
	};

	// FieldName: 会员级别
	// FieldType: INT4
	// Type:	  COMBOENUM(EVipType)
	int		mVipLevel;

	// FieldName: 会员有效期(单位:小时)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int		mVipTime;
public:
	void SetFromPB( bool bServer, PBItemVip* pItem );
	void CreateTlvPB( bool bServer, PBItemVip* pItem );
	int Initialize();
	CTplItemVip() 
	{
		Initialize();
	}
	~CTplItemVip() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 洗点道具			## 模版名称
// version	= 1					## 版本号
// path		= 物品/洗点道具		## 路径
// parent	= 道具基本信息		## 父亲模版
// ********************************************************************** //
class CTplItemClean : public CTplItem
{
public:

	enum ECleanType
	{
		emTypeNone = 0,		// 无效功能
		emTypeSkill,		// 洗技能点
		emTypePotential,	// 洗潜能点
		emTypeTalent,		// 洗造化点
	};

	// FieldName: 洗点类型
	// FieldType: INT4
	// Type:	  COMBOENUM(ECleanType)
	int		mCleanType;

	// FieldName: 洗点等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int		mCleanLevel;

	// FieldName: 保留到的小阶
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int		mStep;
public:
	void SetFromPB( bool bServer, PBItemClean* pItem );
	void CreateTlvPB( bool bServer, PBItemClean* pItem );
	int Initialize();
	CTplItemClean() 
	{
		Initialize();
	}
	~CTplItemClean() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //  

// ********************************************************************** //
// BEGINMAKE
// virtual	= false						## 代表生成到数据库中
// name		= 家族军团特供物品			## 模版名称
// version	= 1							## 版本号
// path		= 物品/家族军团特供物品		## 路径
// parent	= 道具基本信息				## 父亲模版
// ********************************************************************** //
class CTplLeagueBag : public CTplItem
{
public:
	enum ELeagueBagType
	{
		LEAGUEBAG_FAMILY = 0,		// 家族特供物品
		LEAGUEBAG_CORPS	 = 1,		// 军团特供物品		
	};
public:
	class CLeagueItemInfo 
	{
	public:
		int mItemID;
		int mItemNum;
		int	mProbality;				
	};
	
	// FieldName: 特供物品类型
	// FieldType: INT4
	// Type:	  COMBOENUM(ELeagueBagType)
	int		mLeagueBagType;

	// FieldName: 兑换表ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int		mExchangeTableID;
	
	// FieldName: 第%d个物品的ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	
	// FieldName: 第%d个物品的数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	
	// FieldName: 第%d个物品的的生成概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)			
	CLeagueItemInfo mLeagueInfo[MAX_LEAGUEITEMINBAG];
	
	// FieldName: 物品能够使用的地图ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int		mMapID;
	
	// FieldName: 物品能够使用的横坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int		mPosX;
	
	// FieldName: 物品能够使用的纵坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int		mPosY;
	
public:
	void SetFromPB( bool bServer, PBLeagueBag* pItem );
	void CreateTlvPB( bool bServer, PBLeagueBag* pItem );
	int Initialize();
	CTplLeagueBag()
	{
		Initialize();
	}
	~CTplLeagueBag() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 领取				## 模版名称
// version	= 1					## 版本号
// path		= 物品/领取			## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateAwardInfo : public CTemplate
{
public:

	// FieldName: $领取名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char	mName[ STRING32 ];	

	// FieldName: $领取描述
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char	mDesc[ STRING256 ];

	// FieldName: 第%d种物品ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mItemID[ 3 ];

	// FieldName: 第%d种物品数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mItemNum[ 3 ];

	// FieldName: 添加的BUFF
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mBuffID;

	// FieldName: 每日可领取次数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int		mTimes;

	// FieldName: 需要的NPC
	// FieldType: INT4
	// Type:	  TEMPPATH
	int		mNpcID;

	// FieldName: 需要的国籍
	// FieldType: INT4
	// Type:	  COMBOSINGLE(全体=-2,无=-1,大唐=0,乌鸡=1,车迟=2,宝象=3,女儿=4)
	int		mCountry;

	// FieldName: 非家族军团领取
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)
	int		mNoFamily;

public:
	void SetFromPB( bool bServer, PBAwardInfo* pInfo );
	void CreateTlvPB( bool bServer, PBAwardInfo* pInfo );
	int Initialize();
	CTemplateAwardInfo()
	{
		Initialize();
	}
	~CTemplateAwardInfo() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 传销配置表		## 模版名称
// version	= 1					## 版本号
// path		= Config/传销配置表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateMatrixTable : public CTemplate
{
public:

	// FieldName: 第%d档传销的等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mLevel[ MAX_MATRIX_LEVEL ];	

	// FieldName: 被传销的等级限制
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMaxLimit;	

	// FieldName: 传销者的等级限制
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMinLimit;

	// FieldName: 第%d档第%d种传销奖励物品的ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mMasterItemID[ MAX_MATRIX_LEVEL ][ MAX_MASTER_AWARD_NUM ];

	// FieldName: 第%d档第%d种传销奖励物品的个数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMasterItemNum[ MAX_MATRIX_LEVEL ][ MAX_MASTER_AWARD_NUM ];

	// FieldName: 第%d档系统邮件的ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mMailID[ MAX_MATRIX_LEVEL ];

	// FieldName: 正式弟子等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mFormalLevel;

	// FieldName: 出师等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mEducatedLevel;

	// FieldName: 拜师次数上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mEnterLimit;

	// FieldName: 解除需要物品
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mRemoveItem;

	// FieldName: 解除需要金钱
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mRemoveMoney;

	// FieldName: 返还积分百分比1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mFormalPercent;

	// FieldName: 返还积分百分比2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mEducatedPercent;

	// FieldName: 返还上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int		mReturnLimit;

	// FieldName: 调整感恩上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int		mAlterMax;

	// FieldName: 调整感恩下限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int		mAlterMin;

	// FieldName: 返还邮件的ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mReturnMailID;

	// FieldName: 第%d档第%d种弟子奖励物品的ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int		mSlaveItemID[ MAX_MATRIX_LEVEL ][ 15 ];

	// FieldName: 第%d档第%d种弟子奖励物品的个数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mSlaveItemNum[ MAX_MATRIX_LEVEL ][ 15 ];

	// FieldName: 调整感恩总量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int		mAlterAll;

	// FieldName: 调整感恩默认
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)
	int		mAlterDefault;

	// FieldName: 徒弟邮件的ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int		mSlaveMailID;

public:
	void SetFromPB( bool bServer, PBMatrixTable* pTable );
	void CreateTlvPB( bool bServer, PBMatrixTable* pTable );
	int Initialize();
	CTemplateMatrixTable()
	{
		Initialize();
	}
	~CTemplateMatrixTable() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false						## 代表生成到数据库中
// name		= 结婚系统配置表			## 模版名称
// version	= 1							## 版本号
// path		= Config/结婚系统配置表		## 路径
// parent	=							## 父亲模版
// ********************************************************************** //
class CTemplateMarriageConfig : public CTemplate
{

public:
	class CWeddingInfo
	{
		public:			
			int mWeddingLength;			// 婚礼的基本时长(分钟)
			int mMoneyNeeded;			// 婚礼费用
			int mDelayMoneyNeeded;		// 延长婚礼每分钟需要的费用
			int mWeddingRedstoneID;		// 婚礼内丹ID
			int mWeddingHallRedstoneID;	// 礼堂的内丹ID			
			int mWeddingHallMapID;		// 礼堂的地图ID
			int mInvitationCardID;		// npc发送的请帖的ID
			int mInvitationCardNum;		// npc发送的请帖的数量
			int mMaleKeepsakeID;		// npc发送的男方信物ID
			int mFemaleKeepsakeID;		// npc发送的女方信物ID
			int mPosX;					// 玩家传送到礼堂的x坐标
			int mPosY;					// 玩家传送到礼堂的y坐标
			int mMaleKeepsakeTwo;		// npc发送的第2种男方信物ID
			int mFemaleKeepsakeTwo;		// npc发送的第2种女方信物ID			
			int mMaleKeepsakeThree;		// npc发送的第3种男方信物ID
			int mFemaleKeepsakeThree;	// npc发送的第3种女方信物ID	
			int mMaleBufferID;			// 新郎变身buff的模板ID					
			int mFemaleBufferID;		// 新娘变身buff的模板ID	
			int mMaleItemID;			// 新郎变身物品的ID
			int mFemaleItemID;			// 新娘变身物品的ID			
			int mGroomPosX;				// 新郎拜堂时所在位置的横坐标
			int mGroomPosY;				// 新郎拜堂时所在位置的纵坐标
			int mBridePosX;				// 新娘拜堂时所在位置的横坐标
			int mBridePosY;				// 新娘拜堂时所在位置的纵坐标
			int mGroomBufferID;			// 新郎变身buff的模板ID
			int mBrideBufferID;			// 新娘变身buff的模板ID	
			int mMarriageItemID;		// 结婚道具ID
			int mWeddingScroll;			// 重温补办婚礼需要的道具			
	};		
	
	class CKeepsnakeInfo
	{
		public:
			int mKeepsakeID;			// 信物ID
			int mEquipID;				// 信物兑换的装备的模板ID
	};
public:
	// FieldName: 结婚的最低等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,160)		
	int mMarriageLevel;
	
	// FieldName: 结婚需要消耗的办证金钱
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000000)		
	int mMarriageMoney;		

	
	// FieldName: 第%d种规模婚礼的基本时长(分钟)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)		
	
	// FieldName: 第%d种规模婚礼的婚礼费用
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000000)			

	// FieldName: 第%d种规模婚礼延长婚礼每分钟需要的费用
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)				


	// FieldName: 第%d种规模婚礼的婚礼内丹ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)				
	
	// FieldName: 第%d种规模婚礼的礼堂的内丹ID			
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 第%d种规模婚礼的礼堂地图ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 第%d种规模婚礼的npc发送的请帖的ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	
	// FieldName: 第%d种规模婚礼的npc发送的请帖的数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)					

	// FieldName: 第%d种规模婚礼的npc发送的男方信物ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)						
	
	// FieldName: 第%d种规模婚礼的npc发送的女方信物ID		
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 第%d种规模婚礼传入礼堂的横坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 第%d种规模婚礼传入礼堂的纵坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 第%d种规模婚礼npc发送的第2种男方信物ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 第%d种规模婚礼npc发送的第2种女方信物ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 第%d种规模婚礼npc发送的第3种男方信物ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	

	// FieldName: 第%d种规模婚礼npc发送的第3种女方信物ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 第%d种规模婚礼新郎变身buff的ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 第%d种规模婚礼新娘变身buff的ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 第%d种规模婚礼新郎变身物品的ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 第%d种规模婚礼新娘变身物品的ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 第%d种规模婚礼新郎拜堂时所在位置的横坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 第%d种规模婚礼新郎拜堂时所在位置的纵坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 第%d种规模婚礼新娘拜堂时所在位置的横坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	

	// FieldName: 第%d种规模婚礼新娘拜堂时所在位置的纵坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 第%d种规模婚礼新郎变身buff的模板ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 第%d种规模婚礼新娘变身buff的模板ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 第%d种规模婚礼结婚需要的道具ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 第%d种规模婚礼补办重温需要的道具ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
		
	CWeddingInfo mWeddinInfo[MAX_WEDDING_MODULENUM];
	
	// FieldName: 第%d中规模婚礼的第%d个npc的模板ID		
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)						
	int mWeddingNpc[MAX_WEDDING_MODULENUM][MAX_WEDDING_NPCNUM];
	
	// FieldName: 女方适用的第%d个信物的ID	
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	
	// FieldName: 女方适用的第%d个信物兑换生成的装备的ID	
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)		
	CKeepsnakeInfo mFemaleKeepsakeInfo[MAX_WEDDING_KEEPSNAKENUM];
	
	// FieldName: 男方适用的第%d个信物的ID	
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	

	// FieldName: 男方适用的第%d个信物兑换生成的装备的ID	
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)		
	CKeepsnakeInfo mMaleKeepsakeInfo[MAX_WEDDING_KEEPSNAKENUM];
	
	// FieldName: 从礼堂传出的地图ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int mMapID;
	
	// FieldName: 从礼堂传出的地图横坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int mPosX;
	
	// FieldName: 从礼堂传出的地图纵坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int mPosY;
	
	// FieldName: 离婚需要的道具ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int mDivorceItemID;
	
	// FieldName: 结婚获取的称号ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int mMarriageTitleID;
	
	// FieldName: 不举行婚礼时系统发放的男方信物ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int mMaleKeepsakeID;
	
	// FieldName: 不举行婚礼时系统发放的女方信物ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int mFemaleKeepsakeID;
	
	// FieldName:  不举行婚礼时系统发放的请帖ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int mInvitationID;
	
	// FieldName:  不举行婚礼时系统发放的请帖数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int mInvitationNum;
	
	// FieldName:  不举行婚礼时系统发放的新郎变身物品的ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int mMaleItemID;			
	
	// FieldName:  不举行婚礼时系统发放的新娘变身物品的ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)			
	int mFemaleItemID;	
	
	// FieldName:  礼堂里拜堂时定身buff的ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)			
	int mBaitangBuffID;		
	
	// FieldName: 结婚需要的基础道具ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)			
	int mBasicMarriageItem;		
	
	// FieldName: 结婚需要的基础道具数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)			
	int mBasicMarriageItemNum;		
	
	// FieldName: 结婚npc所在的地图ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)			
	int mMarriageNpcMapID;		
	
	// FieldName: 结婚npc所在的地图横坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)			
	int mMarriageNpcPosX;		
	
	// FieldName: 结婚npc所在的地图纵坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)			
	int mMarriageNpcPosY;		

	// FieldName: 亲密度数值(达到该数值时需要的石头减少)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)			
	int mMarriageIntimacy;		

	// FieldName: 亲密度足够时需要的石头数量 
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)			
	int mIntimacyItemNum;		
	
public:
	void SetFromPB( bool bServer, PBTemplateMarriageConfig* pTable );
	void CreateTlvPB( bool bServer, PBTemplateMarriageConfig* pTable );
	int Initialize();
	CTemplateMarriageConfig()
	{
		Initialize();
	}
	~CTemplateMarriageConfig() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= 请帖					## 模版名称
// version	= 1						## 版本号
// path		= 物品/请帖				## 路径
// parent	= 道具基本信息			## 父亲模版
// ********************************************************************** //
class CTplInvitation : public CTplItem
{	
public:
	// FieldName: 请帖兑换生成的物品ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)			
	int mExchangeItemID;			

public:
	void SetFromPB( bool bServer, PBInvitation* pTable );
	void CreateTlvPB( bool bServer, PBInvitation* pTable );	
	int Initialize();
	CTplInvitation()
	{
		Initialize();
	}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= 系统请帖					## 模版名称
// version	= 1						## 版本号
// path		= 物品/系统请帖				## 路径
// parent	= 道具基本信息			## 父亲模版
// ********************************************************************** //
class CTplSysInvitation : public CTplItem
{	
public:
	// FieldName: 系统请帖能够换取的请帖ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)	
	int mInvitationID;	
public:
	void SetFromPB( bool bServer, PBSysInvitation* pTable );
	void CreateTlvPB( bool bServer, PBSysInvitation* pTable );	
	int Initialize();
	CTplSysInvitation()
	{
		Initialize();
	}
	~CTplSysInvitation() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= 进度条配置信息		## 模版名称
// version	= 1						## 版本号
// path		= Config/进度条配置信息	## 路径
// parent	=						## 父亲模版
// ********************************************************************** // 
class CTemplateProgressConfig:public CTemplate
{ 
public:
	class CProGressTime
	{
	public:
		int mFunctype;
		int mTime;
	};
public:
	enum Em_ProgressFunctype
	{
		PROGRESS_FUCTYPE_HORSE		=	0,		// 坐骑
		PROGRESS_FUCTYPE_COLLECT	=	1,		// 采集
		PROGRESS_FUCTYPE_TELEPORT	=	2,		// 传送符
		PROGRESS_FUCTYPE_BOX		=	3,		// 宝箱
		PROGRESS_FUCTYPE_TELESYMBOL	=	4,		// 回程石
		PROGRESS_FUCTYPE_MINMAP		=	5,		// 藏宝图
		PROGRESS_FUCTYPE_TOKEN		=	6,		// 惩妖令
		PROGRESS_FUCTYPE_STONE		=	7,		// 灵石
		PROGRESS_FUCTYPE_CORPSBANNER=	8,		// 旗帜
		PROGRESS_FUCTYPE_JUDIAN		=	9,		// 据点
		PROGRESS_FUCTYPE_INSTONE	=	10,		// 交石
	};
public:
	// FieldName: 第%d种功能的功能类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(坐骑=0,采集=1,传送符=2,宝箱=3,回程石=4,藏宝图=5,惩妖令=6,灵石=7,旗帜=8,据点=9,交石=10)	

	// FieldName: 第%d种功能进度条的读条时间(毫秒)	
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)		
	CProGressTime mProgressInfo[MAX_PROGRESSFUNC_NUM];
public:
	void SetFromPB( bool bServer, PBProGressConfig* pTable );
	void CreateTlvPB( bool bServer, PBProGressConfig* pTable );	
	int Initialize();
	CTemplateProgressConfig() 
	{
		Initialize();
	}
	~CTemplateProgressConfig() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

enum EClearType
{
	CLEAR_ETH  = 0,			// 永久保留
	CLEAR_DAY  = 1,			// 每天清除
	CLEAR_WEEK = 2,			// 每周清除	
};

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 使用次数表		## 模版名称
// version	= 1					## 版本号
// path		= Config/使用次数表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateCountTable : public CTemplate
{
public:

	class CCount
	{
	public:
		int mCountType;
		int mCountTime;
		int mVipTime;
		int mClearType;
	};

	// FieldName: 第%d种类别
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 第%d种每日一般次数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 第%d种每日VIP次数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 第%d种功能的清除类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(永久=0,每天=1,每周=2)	
	CCount		mCountData[ MAX_COUNT_NUM ];

public:
	void SetFromPB( bool bServer, PBCountTable* pTable );
	void CreateTlvPB( bool bServer, PBCountTable* pTable );
	int Initialize();
	CTemplateCountTable()
	{
		Initialize();
	}
	~CTemplateCountTable() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 活动配置表		## 模版名称
// version	= 1					## 版本号
// path		= Config/活动配置表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateActivityConfig : public CTemplate
{
public:
	// FieldName: 婚礼开始多久后扔绣球(毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mHydrangeaThrowTime;
	
	// FieldName: 第%d种喜糖对应的变身buffer的ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mHydrangeaSugarID[MAX_HYDRANGEA_SUGARNUM];
	
	// FieldName: 多久撒一次喜糖(毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mSugarSpreadTime;
	
	// FieldName: 喜糖最多撒几个人
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mSugarSpreadNum;
	
	// FieldName: 活动的最大持续时间(毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mHydrangeaActivityTime;
	
	// FieldName: 玩家的持球时间(毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mHydrangeaHolderTime;
	
	// FieldName: 绣球的争夺时间(毫秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mHydrangeaGrabTime;
	
	// FieldName: 内丹ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mHydrangeaRedStone;	
	
	// FieldName: 抢绣球活动奖励经验的等级经验表
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mHydrangeaExpTableID;	
	
	
	
	// FieldName: 乡试入榜人获取的称号ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mPreliminaryTitleID;		
	
	// FieldName: 乡试每道题目的基本积分
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mPreliminaryBaseScore;
	
	// FieldName: 乡试及时榜第%d+1名的额外积分
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mPreliminaryExtralScore[MAX_PRELIMINARY_TIMELYLISTNUM];
	
	// FieldName: 乡试开始前多久进行提示(秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mPreliminaryTipsTime;
	
	// FieldName: 发送题目多久后发送题目答案(秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mPreliminaryAnswerTime;
	
	// FieldName: 发送题目答案多久后发送下一道题(秒)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mPreliminaryQuestionTime;
	
	// FieldName: 乡试的题目数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mPreliminaryQuestionNum;
	
	// FieldName: 乡试等级经验表ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int mPreliminaryExpTableID;	
	
	// FieldName: 在线时间和有效离线时间之间的兑换比例
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int mEffectiveOffLineExchangeRate;	
	
	// FieldName: 每天可转换为有效离线时间的最大在线时间(小时)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int mMaxOnLineEffectiveTime;
	
	// FieldName: 最大有效离线时间(小时)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int mMaxEffectiveOffLineTime;
	
	// FieldName: 每天可转换的最大离线时间(小时)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int mMaxOffLineTime;
	
	// FieldName:可转换为经验的离线时间上限(小时)
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int mOffLineTimeLimit;	
	
	// FieldName:  会员在线时间和有效离线时间之间的兑换比例
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int mVipOffLineExchangeRate;		
public:
	void SetFromPB( bool bServer, PBActivityConfig* pTable );
	void CreateTlvPB( bool bServer, PBActivityConfig* pTable );	
	int Initialize();
	CTemplateActivityConfig() 
	{
		Initialize();
	}
	~CTemplateActivityConfig() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= 洗属性物品			## 模版名称
// version	= 1						## 版本号
// path		= 物品/洗属性物品		## 路径
// parent	= 道具基本信息			## 父亲模版
// ********************************************************************** //
class CTplItemRefresh : public CTplItem
{											
public:
	// FieldName: 颜色类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(白=1,蓝=2,紫=3,金=4)
	int			mColor;

	// FieldName: 限制等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	int			mLevel;

	// FieldName: 无功能
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)

	// FieldName: 戒指类装备
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)

	// FieldName: 护腕类装备
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)

	// FieldName: 武器类装备
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)

	// FieldName: 护胸类装备
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)

	// FieldName: 护腿类装备
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)

	// FieldName: 护头类装备
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)

	// FieldName: 项链类装备
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)

	// FieldName: 腰饰类装备
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)

	// FieldName: 护符类装备
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)

	// FieldName: 法球类装备
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)

	// FieldName: 披风类装备
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)

	// FieldName: 时装类装备
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)

	// FieldName: 法宝装备
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)

	// FieldName: 才华类装备
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)

	// FieldName: 结婚类装备
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)
	int			mLimitPos[ 16 ];
public:
	void SetFromPB( bool bServer, PBItemRefresh* pItem );
	void CreateTlvPB( bool bServer, PBItemRefresh* pItem );
	int Initialize();
	CTplItemRefresh()
	{
		Initialize();
	}
	~CTplItemRefresh() {}
};	
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** // 

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 召集类物品	## 模版名称
// version	= 1				## 版本号
// path		= 物品/召集类物品	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplItemSummon : public CTplItem
{
public:
	enum ESummon
	{
		SUMMON_NONE		=	0,		// 无
		SUMMON_SINGLE	=	1,		// 单人
		SUMMON_FAMILY	=	2,		// 家族
		SUMMON_CORPS	=	3,		// 军团
		SUMMON_TEAM		=	4,		// 组队
		SUMMON_MASTER	=	5,		// 师傅
		SUMMON_SLAVE	=	6,		// 徒弟
	};
public:

	// FieldName: 召集类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无=0,单人=1,家族=2,军团=3,组队=4,师傅=5,徒弟=6)
	int			mSummonType;

public:
	void SetFromPB( bool bServer, PBItemSummon* pItem );
	void CreateTlvPB( bool bServer, PBItemSummon* pItem );
	int Initialize();
	CTplItemSummon(){	Initialize();}
	~CTplItemSummon() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 直升配置表		## 模版名称
// version	= 1					## 版本号
// path		= Config/直升配置表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateDirectTable : public CTemplate
{
public:
	// FieldName: 第%d级装备%d星得到的ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mDirectID[ 16 ][ 16 ];

	// FieldName: 第%d星得到的必成石ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mLuckID[ 16 ];

	// FieldName: 得到%d个的概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mLuckRate[ 3 ];

public:
	void SetFromPB( bool bServer, PBDirectTable* pTable );
	void CreateTlvPB( bool bServer, PBDirectTable* pTable );	
	int Initialize();
	CTemplateDirectTable() 
	{
		Initialize();
	}
	~CTemplateDirectTable() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 直升物品		## 模版名称
// version	= 1				## 版本号
// path		= 物品/直升物品	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplItemDirect : public CTplItem
{
public:

	// FieldName: 限制等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int			mLevel;

	// FieldName: 限制星级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int			mStar;

public:
	void SetFromPB( bool bServer, PBItemDirect* pItem );
	void CreateTlvPB( bool bServer, PBItemDirect* pItem );
	int Initialize();
	CTplItemDirect(){	Initialize();}
	~CTplItemDirect() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 摘星物品		## 模版名称
// version	= 1				## 版本号
// path		= 物品/摘星物品	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplItemGetStar : public CTplItem
{
public:

	// FieldName: 限制等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int			mLevel;

public:
	void SetFromPB( bool bServer, PBItemGetStar* pItem );
	void CreateTlvPB( bool bServer, PBItemGetStar* pItem );
	int Initialize();
	CTplItemGetStar(){	Initialize();}
	~CTplItemGetStar() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 修炼经验丹		## 模版名称
// version	= 1				## 版本号
// path		= 物品/修炼经验丹  ## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplItemExpContainer : public CTplItem
{
public:	   	
	// FieldName: 每次获得打怪经验的万分比
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mOgreExpRate;	
	
	// FieldName: 能够存储的最大经验值
	// FieldType: INT8
	// Type:	  EDITNUMBER(0,100000000000000)
	unsigned long long			mMaxExp;
	
	// FieldName: 经验存储满时能够转化的经验丹ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mExpPillsID;	
public:
	void SetFromPB( bool bServer, PBItemExpContainer* pItem );
	void CreateTlvPB( bool bServer, PBItemExpContainer* pItem );
	int Initialize();
	
	CTplItemExpContainer(){ Initialize(); }
	~CTplItemExpContainer(){}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 经验丹		## 模版名称
// version	= 1				## 版本号
// path		= 物品/经验丹	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplItemExpPills : public CTplItem
{
public:

	// FieldName: 能够使用的最低等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mLevelLimit;
	
	// FieldName: 使用后能够获取的经验值
	// FieldType: INT8
	// Type:	  EDITNUMBER(0,100000000000000)
	unsigned long long			mExp;	

public:
	void SetFromPB( bool bServer, PBItemExpPills* pItem );
	void CreateTlvPB( bool bServer, PBItemExpPills* pItem );
	int Initialize();
	
	CTplItemExpPills(){ Initialize(); }
	~CTplItemExpPills(){ }
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 修炼心得		## 模版名称
// version	= 1				## 版本号
// path		= 物品/修炼心得	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplItemExpTome : public CTplItem
{
public:

	// FieldName: 开启需要的第%d个物品ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mOpenItemID[ 5 ];

	// FieldName: 能够使用的最低等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mLevelLimit;

	// FieldName: 使用后能够获取的经验值
	// FieldType: INT8
	// Type:	  EDITNUMBER(0,100000000000000)
	unsigned long long			mExp;	

public:
	void SetFromPB( bool bServer, PBItemExpTome* pItem );
	void CreateTlvPB( bool bServer, PBItemExpTome* pItem );
	int Initialize();

	CTplItemExpTome(){ Initialize(); }
	~CTplItemExpTome(){ }
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 国战配置表		## 模版名称
// version	= 1					## 版本号
// path		= Config/国战配置表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateWorldBattleConfig : public CTemplate
{
public:

	class CWarDate
	{
	public:
		int mDay;
		int mHour;
		int mMinute;
	};

	class CTax
	{
	public:
		int mTaxRate;
		int	mIncomeRate;
		int mLimit;
	};

	// FieldName: 报名开始日
	// FieldType: INT4
	// Type:	  COMBOSINGLE(周日=0,周一,周二,周三,周四,周五,周六)

	// FieldName: 报名开始时
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 报名开始分
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	CWarDate	mSubmitBegin;

	// FieldName: 国战开始日
	// FieldType: INT4
	// Type:	  COMBOSINGLE(周日=0,周一,周二,周三,周四,周五,周六)

	// FieldName: 国战开始时
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 国战开始分
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	CWarDate	mBattleBegin;

	// FieldName: 国战结束日
	// FieldType: INT4
	// Type:	  COMBOSINGLE(周日=0,周一,周二,周三,周四,周五,周六)

	// FieldName: 国战结束时
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 国战结束分
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	CWarDate	mBattleEnd;

	// FieldName: 第%d种国战内丹
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int			mStoneID[ 10 ];

	// FieldName: 初始的BUFFID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mInitBuffID;

	// FieldName: 大旗的资源数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mFlagSource;

	// FieldName: 杀人的资源数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mKillSource;

	// FieldName: 扛旗奖励
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mFlagBonus;

	// FieldName: 第%d战场人数上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mPerNum[ 5 ];

	// FieldName: 第%d战场地图ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mMapID[ 5 ];

	// FieldName: 第%d级报名所需金钱
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mSubmitMoney[ 160 ];

	// FieldName: 报名限制等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mSubmitLevel;

	// FieldName: 第%d阵地坐标X
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mPosX[ 5 ];

	// FieldName: 第%d阵地坐标Y
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mPosY[ 5 ];

	// FieldName: 传出地图ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mOutMapID;

	// FieldName: 传出地图坐标X
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mOutPosX;

	// FieldName: 传出地图坐标Y
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mOutPosY;

	// FieldName: 第%d阵营的索引%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mFlagIndex[ 5 ][ 100 ];

	// FieldName: 竞价结束日
	// FieldType: INT4
	// Type:	  COMBOSINGLE(周日=0,周一,周二,周三,周四,周五,周六)

	// FieldName: 竞价结束时
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 竞价结束分
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	CWarDate	mBidEnd;

	// FieldName: 每日最大战功
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mMaxWarHonor;

	// FieldName: 第%d职业的称号%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mTitle[ 7 ][ 3 ];

	// FieldName: 提示开始日
	// FieldType: INT4
	// Type:	  COMBOSINGLE(周日=0,周一,周二,周三,周四,周五,周六)

	// FieldName: 提示开始时
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 提示开始分
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	CWarDate	mNoticeBegin;

	// FieldName: 称号的有效时间
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000000)
	int			mTitleValidTime;

	// FieldName: 内丹坐标X
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mStonePosX;

	// FieldName: 内丹坐标Y
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100000)
	int			mStonePosY;

	// FieldName: 第%d种税率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第%d种收益率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第%d种上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)
	CTax		mTax[ 15 ];

	// FieldName: 获胜方税率改变
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mWinTax;

public:
	void SetFromPB( bool bServer, PBWorldBattleConfig* pTable );
	void CreateTlvPB( bool bServer, PBWorldBattleConfig* pTable );
	int Initialize();
	CTemplateWorldBattleConfig()
	{
		Initialize();
	}
	~CTemplateWorldBattleConfig() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false			## 代表生成到数据库中
// name		= 自动采集道具	## 模版名称
// version	= 1				## 版本号
// path		= 物品/自动采集道具	## 路径
// parent	= 道具基本信息	## 父亲模版
// ********************************************************************** //

class CTplItemAutoCollect : public CTplItem
{
public:	
	
	class CDegreeInfo	
	{	
	public:							
		int		mValue;			// 玩家本身熟练度与要求的熟练度的差值
		int		mProbality;		// 在差值范围内获取熟练度的概率
		int		mDegreeValue;	// 在差值范围内获取熟练度的值
	};
	
	class CItemInfo
	{
	public:
		int	mItemID;		// 物品ID
		int mItemNum;		// 物品数量
		int mProbality;		// 物品的产出概率
	};
public:
	// FieldName: 第%d种材料的物品ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)		
	
	// FieldName: 第%d种材料的物品数量
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)		
	
	// FieldName: 第%d种材料的物品产出概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)		
	CItemInfo mItemInfo[MAX_AUTOCOLLECT_MATERIALSNUM];	
	
	// FieldName: 自身熟练度和采集需要的熟练度的第%d个差值区间
	// FieldType: INT4	
	// Type:      EDITNUMBER(0,1000) 


	// FieldName: 自身熟练度和采集需要的熟练度的第%d个差值区间的获取数量度的概率
	// FieldType: INT4	
	// Type:      EDITNUMBER(0,10000) 


	// FieldName: 自身熟练度和采集需要的熟练度的第%d个差值区间的获取数量度的值
	// FieldType: INT4	
	// Type:      EDITNUMBER(0,10000)	   
	CDegreeInfo mDegreeInfo[MAX_LIFESKILL_DEGREESTEP];
	
	// FieldName: 道具能够使用的地图ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mMapID;
	
	// FieldName: 道具能够使用的横坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mPosX;
	
	// FieldName: 道具能够使用的纵坐标
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mPosY;
	
	// FieldName: 道具能够使用的距离目标坐标的有效范围
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mGrid;	
	
	// FieldName: 道具能够使用的最大次数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)
	int			mMaxUseTimes;
public:
	void SetFromPB( bool bServer, PBItemAutoCollect* pItem );
	void CreateTlvPB( bool bServer, PBItemAutoCollect* pItem );
	int Initialize();
	CTplItemAutoCollect(){Initialize();}
	~CTplItemAutoCollect() {}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// CFangCunTreeNode
// ********************************************************************** //

class CFangCunTreeNode
{
public:
	int		mSkillID;				// 技能ID
	int		mMaxLevel;				// 最大等级( 最大技能点投入额度 )
	int		mNeedSkillID[ 4 ];		// 需要的前导技能
	int		mNeedSkillPoint[ 4 ];	// 需要的前导技能投入额度
	int		mLevel;					// 需要的玩家等级
	int		mNeedBook;				// 是否需要对应的技能书
	int		mStage;					// 对应的阶
	int		mOrder;					// 阶顺序

public:
	void SetFromPB( bool bServer, PBFangCunTreeNode* pNode );
	void CreateTlvPB( bool bServer, PBFangCunTreeNode* pNode );
};

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 方寸树			## 模版名称
// version	= 1					## 版本号
// path		= 技能/方寸树		## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateFangCunTree  : public CTemplate
{
public:
	// FieldName: 第%d阶第%d种技能ID
	// FieldType: INT4
	// Type:	  SKILLPATH

	// FieldName: 第%d阶第%d种最大等级( 最大技能点投入额度 )
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第%d阶第%d种需要的前导技能1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第%d阶第%d种需要的前导技能2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第%d阶第%d种需要的前导技能3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第%d阶第%d种需要的前导技能4
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第%d阶第%d种需要的前导技能投入额度1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第%d阶第%d种需要的前导技能投入额度2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第%d阶第%d种需要的前导技能投入额度3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第%d阶第%d种需要的前导技能投入额度4
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,120)

	// FieldName: 第%d阶第%d种需要的玩家等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,160)

	// FieldName: 第%d阶第%d种需要的技能书
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	// FieldName: 第%d阶第%d种对应的阶
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,160)

	// FieldName: 第%d阶第%d种阶顺序
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000000)

	CFangCunTreeNode	mNode[ 10 ][ 5 ];

public:
	void SetFromPB( bool bServer, PBFangCunTree* pSkillTree );
	void CreateTlvPB( bool bServer, PBFangCunTree* pSkillTree );
	int Initialize();
	CTemplateFangCunTree() 
	{
		Initialize();
	}
	~CTemplateFangCunTree() {}
};


// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 造化大阶				 ## 模版名称
// version	= 1						 ## 版本号
// path		= Config/造化大阶		 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //

class CTemplateTalentStage : public CTemplate
{
public:
	class CStage
	{
	public:
		int			mStageID;
		int			mNext[ STEP_SWITCH ];
		int			mLayer;
	};

	// FieldName: 第%d个大阶编号
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 第%d个大阶的第1种下一大阶
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 第%d个大阶的第2种下一大阶
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 第%d个大阶的第3种下一大阶
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	// FieldName: 第%d个大阶所在的层
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000)

	CStage	mStage[ 8 ];	

public:
	void SetFromPB( bool bServer, PBTemplateTalentStage* pTalent );
	void CreateTlvPB( bool bServer, PBTemplateTalentStage* pTalent );
	int Initialize();
	CTemplateTalentStage()
	{
		Initialize();
	}
	~CTemplateTalentStage() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 转阵营				 ## 模版名称
// version	= 1						 ## 版本号
// path		= Config/转阵营			 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //
class CTemPlateChangeCampCfg: public CTemplate
{

public:
	// FieldName: 转阵营道具id
	// FieldType: INT4
	// Type:      TEMPPATH
	int  m_itemId; 

	// FieldName: 转阵营到弱阵营礼包id
	// FieldType: INT4
	// Type:      TEMPPATH
	int  m_giftId;

	// FieldName: cd时间单位天
	// FieldType: INT4
	// Type:      EDITNUMBER(0,30)
	unsigned int m_cd_time; 
public:
	void SetFromPB( bool bServer, PBItemChangeCamp* changeCampTable);
	void CreateTlvPB( bool bServer, PBItemChangeCamp* changeCampTable);
	int Initialize();
	CTemPlateChangeCampCfg() 
	{
		Initialize();
	}
	~CTemPlateChangeCampCfg(){}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					 ## 代表生成到数据库中
// name		= 符文转换升级		 ## 模版名称
// version	= 1						 ## 版本号
// path		= Config/符文转换升级	 ## 路径
// parent	=						 ## 父亲模版
// ********************************************************************** //
class CTemplateComRuneCfg: public CTemplate
{
public:
	// FieldName: 升级符概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int  m_upRuneRand;

	// FieldName: 最大升级个数
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10)
	int  m_max;

	// FieldName: 最小升级个数
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10)
	int  m_min;

public:
	class CRuneRand
	{
	public:
		int  m_whitenum;
		int  m_bluenum;
		int  m_purplenum;
		int  m_rand;
		int  m_money;;
		int  m_runeLevel;
	};

	// FieldName: %d白色个数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)

	// FieldName: %d蓝色个数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)

	// FieldName: %d紫色个数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10)

	// FieldName: 第%d升级概率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)

	// FieldName: 第%d升级符文金钱
	// FieldType: INT4
	// Type:      EDITNUMBER(0,2000000000)

	// FieldName: 第%d升级后的等级
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10)
	CRuneRand     m_Rune[MAX_RUNECOMTYPE];


public:
	void SetFromPB( bool bServer, PBComRuneConfig* changeCampTable);
	void CreateTlvPB( bool bServer, PBComRuneConfig* changeCampTable);
	int Initialize();
	CTemplateComRuneCfg() 
	{
		Initialize();
	}
	~CTemplateComRuneCfg(){}
};
// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 弹劾配置表		## 模版名称
// version	= 1					## 版本号
// path		= Config/弹劾配置表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateAccuseConfig : public CTemplate
{
public:

	// FieldName: 开始弹劾时
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,23)
	int			mAccuseBeginHour;

	// FieldName: 开始弹劾分
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,59)
	int			mAccuseBeginMinute;

	// FieldName: 弹劾必须等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,160)
	int			mAccuseLevel;

	// FieldName: 结束弹劾时
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,23)
	int			mAccuseEndHour;

	// FieldName: 结束弹劾分
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,59)
	int			mAccuseEndMinute;

	// FieldName: 弹劾花费金额
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mAccuseMoney;

	// FieldName: 弹劾最少人数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,100)
	int			mAccusePerson;

	// FieldName: 弹劾比例
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int			mAccusePercent;

	// FieldName: 失败奖励声望
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mAwardPower;

	// FieldName: 大门死亡次数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mGateNum;

	// FieldName: 大旗死亡次数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mFlagNum;

	// FieldName: 大门获得声望
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mGatePower;

	// FieldName: 大旗获得声望
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mFlagPower;

	// FieldName: 提示弹劾时
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,23)
	int			mAccuseNoticeHour;

	// FieldName: 提示弹劾分
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,59)
	int			mAccuseNoticeMinute;

	// FieldName: 提示声望
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mNoticePower;

	// FieldName: 大门参数1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mGateParam1;

	// FieldName: 大旗参数1
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mFlagParam1;

	// FieldName: 大门参数2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mGateParam2;

	// FieldName: 大旗参数2
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mFlagParam2;

	// FieldName: 大门参数3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mGateParam3;

	// FieldName: 大旗参数3
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mFlagParam3;

	// FieldName: 乱舞奖励加成
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mLWPercent;

public:
	void SetFromPB( bool bServer, PBAccuseConfig* pTable );
	void CreateTlvPB( bool bServer, PBAccuseConfig* pTable );
	int Initialize();
	CTemplateAccuseConfig()
	{
		Initialize();
	}
	~CTemplateAccuseConfig() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 全局地图配置表	## 模版名称
// version	= 1					## 版本号
// path		= Config/全局地图配置表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateGlobalMapConfig : public CTemplate
{
public:
	// FieldName: 全局地图
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mMapID;

	// FieldName: 需要的buffid
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mBuffID;

	// FieldName: 人数上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mLimit;

	// FieldName: 传出地图
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mOutMap;

	// FieldName: 传出X
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mOutX;

	// FieldName: 传出Y
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mOutY;

	// FieldName: 跨服战逃逸buffID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int			mEscapeBuffID;
public:
	void SetFromPB( bool bServer, PBGlobalMapConfig* pTable );
	void CreateTlvPB( bool bServer, PBGlobalMapConfig* pTable );
	int Initialize();
	CTemplateGlobalMapConfig()
	{
		Initialize();
	}
	~CTemplateGlobalMapConfig() {}
};


// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 军团BOSS表		## 模版名称
// version	= 1					## 版本号
// path		= Config/军团BOSS表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateCorpsBOSSTable : public CTemplate
{
public:

	// FieldName: 第%d个BOSS ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mBOSSID[ MAX_CORPSBOSSNUM ];	

	// FieldName: 第%d个稀有BOSS ID
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int		mHardBOSSID[ MAX_CORPSBOSSNUM ];

	// FieldName: 第%d级军团的炼妖值上限
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int		mValueLimit[ 6 ];

	// FieldName: 第%d个boss的掉落描述
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char	mDropDesc[MAX_CORPSBOSSNUM][ STRING256 ];

	// FieldName: 第%d个稀有boss的掉落描述
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char	mHardDropDesc[MAX_CORPSBOSSNUM][ STRING256 ];

	// FieldName: 第%d个boss被召唤军团最低等级限制
	// FieldType:	INT4
	// Type:	EDITNUMBER(0,200)
	int		mSummonMinLevel[ MAX_CORPSBOSSNUM ];

	// FieldName: 第%d个稀有boss被召唤军团最低等级限制
	// FieldType:	INT4
	// Type:	EDITNUMBER(0,200)
	int		mHardSummonMinLevel[ MAX_CORPSBOSSNUM ];

	// FieldName: 第%d个boss被召唤需求的炼化度
	// FieldType: INT4
	// Type:	EDITNUMBER(0,100000)
	int		mSummonMinValue[ MAX_CORPSBOSSNUM ];

	// FieldName: 第%d个稀有boss被召唤需求的炼化度
	// FieldType: INT4
	// Type:	EDITNUMBER(0,100000)
	int		mHardSummonMinValue[ MAX_CORPSBOSSNUM ];

public:
	void SetFromPB( bool bServer, PBCorpsBOSS* pTable );
	void CreateTlvPB( bool bServer, PBCorpsBOSS* pTable );
	int Initialize();
	CTemplateCorpsBOSSTable()
	{
		Initialize();
	}
	~CTemplateCorpsBOSSTable() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 离线经验配置表	## 模版名称
// version	= 1					## 版本号
// path		= Config/离线经验配置表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateOffLineExpConfig : public CTemplate
{
public:
	// FieldName:由离线获得一次经验的最小单位（秒）
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int mOffLineUnit;	

	// FieldName:由离线获得一次经验的数值%d
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int mOffLineExp[ TMP_MAX_GRADE ];

	// FieldName:由离线获得经验的等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000)
	int mOffLineLevel;

	// FieldName: 第%d级的经验上限
	// FieldType: INT4
	// Type:      EDITNUMBER(0,1000000000)
	int mExpLimit[ TMP_MAX_GRADE ];

	// FieldName:由离线获得经验的比率
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,10000)
	int mOffLinePercent;

public:
	void SetFromPB( bool bServer, PBOffLineExpConfig* pTable );
	void CreateTlvPB( bool bServer, PBOffLineExpConfig* pTable );
	int Initialize();
	CTemplateOffLineExpConfig()
	{
		Initialize();
	}
	~CTemplateOffLineExpConfig() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false				## 代表生成到数据库中
// name		= 副本声望表		## 模版名称
// version	= 1					## 版本号
// path		= Config/副本声望表	## 路径
// parent	=					## 父亲模版
// ********************************************************************** //
class CTemplateRepeFameTable : public CTemplate
{
public:
	enum EM_REPETYPE
	{		
		REPETYPE_NANTIANMEN		=	0,			// 决战南天门
		REPETYPE_LINGYUNDU		=	1,			// 凌云渡
		REPETYPE_QITIAN			=	2,			// 齐天大战
		REPETYPE_LIANHUADONG	=	3,			// 莲花洞
		REPETYPE_HUANGFENGDONG	=	4,			// 黄风洞
		REPETYPE_YUHUAYUAN		=	5,			// 御花园
		REPETYPE_DAYANTA		=	6,			// 大雁塔
		REPETYPE_YUNXIAOTA10	=	7,			// 云霄塔10层
		REPETYPE_YUNXIAOTA20	=	8,			// 云霄塔20层
		REPETYPE_YUNXIAOTA30	=	9,			// 云霄塔30层
		REPETYPE_YUNXIAOTA40	=	10,			// 云霄塔40层
		REPETYPE_YUNXIAOTA50	=	11,			// 云霄塔50层
		REPETYPE_WUDIDONG1		=	12,			// 无底洞1
		REPETYPE_WUDIDONG2		=	13,			// 无底洞2
		REPETYPE_WUDIDONG3		=	14,			// 无底洞3
		REPETYPE_WUDIDONG4		=	15,			// 无底洞4
		REPETYPE_PINXIANTAO		=	16,			// 品仙桃
		REPETYPE_TENGYUN		=	17,			// 腾云大战
		REPETYPE_LONGGONG1		=	18,			// 龙宫1
		REPETYPE_LONGGONG2		=	19,			// 龙宫2
		REPETYPE_LONGGONG3		=	20,			// 龙宫3
		REPETYPE_PANTAO1		=	21,			// 蟠桃1
		REPETYPE_PANTAO2		=	22,			// 蟠桃2
		REPETYPE_PANTAO3		=	23,			// 蟠桃3
	}; 
public:

	
	// FieldName: 第%d个声望类型
	// FieldType: INT4
	// Type:	  COMBOSINGLE(无=0,竞技声望=1,副本声望=2,历练声望=3)
	int		mFameType[ 50 ];	

	// FieldName: 第%d级个声望数值
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int		mFameValue[ 50 ];

	// FieldName: 第%d级个物品数量
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int		mItemValue[ 50 ];

	// FieldName: 竞技物品ID
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int		mCombatItemID;

	// FieldName: 副本物品ID
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int		mRepeItemID;

	// FieldName: 历练物品ID
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int		mCommonItemID;

	// FieldName: 竞技上限
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int		mCombatLimit;

	// FieldName: 副本上限
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int		mRepeLimit;

	// FieldName: 历练上限
	// FieldType: INT4
	// Type:      EDITNUMBER(0,10000000)
	int		mCommonLimit;
	
public:
	void SetFromPB( bool bServer, PBRepeFameTable* pTable );
	void CreateTlvPB( bool bServer, PBRepeFameTable* pTable );
	int Initialize();
	CTemplateRepeFameTable() 
	{
		Initialize();
	}
	~CTemplateRepeFameTable() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= 铸灵配置表			## 模版名称
// version	= 1						## 版本号
// path		= Config/铸灵配置表		## 路径
// parent	=						## 父亲模版
// ********************************************************************** //
class CTplCastingSpiritTable :	public CTemplate
{
public:
	// FieldName: 第%d个部位开放状态
	// FieldType: INT4
	// Type:	  COMBOSINGLE(否=0,是=1)
	int				m_anIsOpen[MAX_CASTINGSPIRIT_POSITION];

	// FieldName: $第%d个部位名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char			m_acPositionName[MAX_CASTINGSPIRIT_POSITION][STRING32];

	// FieldName: $第%d个部位描述
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char			m_acPositionDesc[MAX_CASTINGSPIRIT_POSITION][STRING256];

	// FieldName: $第%d个系别名称
	// FieldType: STRING32
	// Type:	  EDITSTRING
	char			m_acTypeName[MAX_CASTINGSPIRIT_TYPE][STRING32];

	// FieldName: $第%d个系别描述
	// FieldType: STRING256
	// Type:	  EDITSTRING
	char			m_acTypeDesc[MAX_CASTINGSPIRIT_TYPE][STRING256];

	// FieldName: 第%d个孔启灵需要的启灵珠ID
	// FieldType: INT4
	// Type:      EDITNUMBER(0,100000)
	int				m_anSpiritItem[MAX_CASTINGSPIRIT_SLOT];

	// FieldName: 第%d个孔启灵需要的启灵珠个数
	// FieldType: INT4
	// Type:      EDITNUMBER(0,100)
	int				m_anSpiritItemNum[MAX_CASTINGSPIRIT_SLOT];

	// FieldName: 升级到第%d阶真灵需要的非绑定金
	// FieldType: INT4
	// Type:      EDITNUMBER(0,1000000000)
	int				m_anUpgradeMoney[MAX_SPIRITITEM_GRADE];

	// FieldName: 第%d阶真灵退灵需要的非绑定金
	// FieldType: INT4
	// Type:      EDITNUMBER(0,1000000000)
	int				m_anDowngradeMoney[MAX_SPIRITITEM_GRADE];

	// FieldName: 聚灵需要的物品
	// FieldType: INT4
	// Type:      EDITNUMBER(0,100000)
	int				m_nUpgradeItem;

	// FieldName: 退灵需要的物品
	// FieldType: INT4
	// Type:      EDITNUMBER(0,100000)
	int				m_nDowngradeItem;

public:
	void SetFromPB( bool bServer, PBCastingSpiritTable* pTable );
	void CreateTlvPB( bool bServer, PBCastingSpiritTable* pTable );
	int Initialize();
	CTplCastingSpiritTable() 
	{
		Initialize();
	}
	~CTplCastingSpiritTable() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //


// ********************************************************************** //
// BEGINMAKE
// virtual	= false					## 代表生成到数据库中
// name		= 真灵					## 模版名称
// version	= 1						## 版本号
// path		= 物品/真灵				## 路径
// parent	= 道具基本信息			## 父亲模版
// ********************************************************************** //
class CTplSpiritItem : public CTplItem
{
public:
	// FieldName: 阶数
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,8)
	int			m_nGrade;

	// FieldName: 真灵等级
	// FieldType: INT4
	// Type:	  COMBOSINGLE(白=1,蓝=2,紫=3,金=4)
	int			m_nColorGrade;

	// FieldName: 系别
	// FieldType: INT4
	// Type:	  COMBOSINGLE(攻击=0,法防=1,物防=2)
	int			m_nSpiritType;

	// FieldName: 使用限制等级
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,160)
	int			m_nLevelLimit;

	// FieldName: 提升的属性值
	// FieldType: INT4
	// Type:	  EDITNUMBER(0,1000000000)
	int			m_nEffectPoint;

public:
	void SetFromPB( bool bServer, PBSpiritItem* pItem );
	void CreateTlvPB( bool bServer, PBSpiritItem* pItem );
	int Initialize();
	CTplSpiritItem() 
	{
		Initialize();
	}
	~CTplSpiritItem() {}
};

// ********************************************************************** //
// ENDMAKE
// ********************************************************************** //

// ********************************************************************** //
// CDataStatic
// ********************************************************************** //

template< int RANK, int LEVEL, int SIZE >
class CProduceTable
{
public:
	int mTable[ RANK ][ LEVEL ][ SIZE ];
};

struct CComposeInfo
{
	int mComposeID[MAX_LIFESKILL_COMPOSENUM];
	int mComposeNum;
	int mComposeType;
};

struct CCompose
{
	int mComposeID;				// 模板ID
	int mDegreeValue;			// 学习需要的熟练度
};

class CLifeSkillInfo
{
public:
	void* spLifeSkillTable;							// 生活技能表	
	CComposeInfo mComposeInfo[MAX_COMPOSEkIND_NUM];	// 配方信息
	int mComposeKindNum;	
	CCompose mCompose[MAX_LIFESKILL_COMPOSENUM];
	int mComposeNum;
public:
	CLifeSkillInfo( )
	{
		Initialize();
	}
	~CLifeSkillInfo(){  }
public:
	int Initialize( );
	
public:
	int InsertComposeInfo( int nComposeID, int nComposeKind, int nDegreeValue, int nComposeType );
	int InsertCompose( int nComposeID, int nDegreeValue );
};

class CDataStatic
{
public:
	static CTemplate* spTemplate[ MAX_TEMPLATE ];							// 模版表
	static void* spLevelExp;											// 等级经验表
	static void* spData;												// 数值修正表（等级，组队）
	static void* spUpgrade;												// 装备升级表
	static void* spSkill[ MAX_SKILL ][ MAX_LEVEL ];						// 技能表
	static void* spOpenSlot;											// 开槽表
	static void* spProSuit;												// 生产者套装
	static void* spProExp;												// 生产等级的经验
	static void* spBeginner;											// 新手设置
	static void* spDropLevel;											// 掉落装备等级表
	static void* spMetierProperty[ MAX_METIER ][ PROPERTY_OPTION ];		// (职业)属性表
	static void* spLevelProperty[ PROPERTY_OPTION ];					// (等级)属性表
	static void* spHonorToExp;											// 功勋经验换算表
	static void* spHonorObtain;											// 经验获得表
	static void* spReliveTable;											// 复活花费表
	static void* spQuestionExpTable;									// 答题等级经验表
	static void* spRedStoneExpTable;									// 内丹等级经验表 
	static void* spBindChargeTable;										// 绑定花费表
	static void* spPKValueTable;										// PK范围表
	static void* spTitleManager;										// 稱號系統配置
	static void* spTalent;												// 造化表
	static void* spMultiBuff;											// buff组合
	static void* spCDGroup;												// 冷却时间
	static void* spLight;												// 发光配置
	static void* spApConfig;											// 活力配置表
	static void* spWizard;												// 才华升级表
	static void* spJudgeConfig;											// 评价配置表
	static int spOffical[ OFFICAL_NUM ][ MAX_POWER ];					// 官职功能表
	static void* spOfficalTable;										// 官职配置
	static int spKing[ MAX_POWER ];										// 国王功能
	static void* spConstructExp;										// 建设度表
	static void* spAddPKTable;											// PK附加表
	static void* spMapTable;											// 地图势力表
	static void* spMultiAttackTable;									// 群攻伤害表
	static void* spDoubleTimeTable;										// 双倍时间表
	static void* spMatrixTable;											// 传销配置表
	static void* spCountTable;											// 计数表
	static void* spDirectTable;											// 直升表
	static void* spBattleConfig;										// 国战配置表
	static void* spMoneyLmtConfig;										// 金钱上限表
	static void* spFangCunTree;											// 方寸山技能
	static void* spTalentStage;											// 造化大阶
    static void* spHandinTable[MAX_HANDINTABLE];                        // 上缴奖励对照表
    static void* spHandinEncourage[MAX_HANDINENCOURAGE];                // 上缴奖励
	static void* spChangeCamp;											// 转阵营
	static void* spAccuseConfig;										// 弹劾配置表
	//static void* spRuneConfig ;											//转换符文表
	static void * spComRuneConfig;						//升级符文表
	static void* spAchieve;												// 成就奖励表
	static void* spGlobalMapConfig;										// 全局地图配置表
	static void* spOffLineExpConfig;									// 离线经验配置表
	static void* spRepeFameTable;										// 副本声望配置表
	static void* spCastingSpiritTable;									// 铸灵配置表
	
	static unsigned short sMetierID[ MAX_METIER ];						// 职业ID
	static unsigned short sGenerID[ MAX_GENER ];						// 职业技能系别ID
	static unsigned short sComposeID[ MAX_COMPOSE ];					// 合成ID
	static int sTemplateVersion;										// 模板文件版本号
	static int sTaskVersion;											// 任务文件版本号
	static int mFamilyExpID;											// 家族经验令牌id
	static const int sProNum = 16;
	static int mFamilyRedstoneID;										// 家族内丹ID
	static void *mMarriageConfig;										// 结婚配置
	// 生产装备的列表
	static CProduceTable< PRODUCE_RANK, PRODUCE_LEVEL, PRODUCE_DETAIL2 > sProEquip[ PRODUCE_TYPE ][ PRODUCE_DETAIL1 ];				
	static int sProPhys[ 8 ][ 4 ];	
	static int sRuneLevelID[4][400];
	// 生产药品的列表
	static int sSuitID[ MAX_SUIT ];
 	
	static unsigned int sBugleItemId ;											// 喇叭道具id
	static unsigned int sAutoChatItemId ;										// 自动喊话道具id
	
	static void* spDecompund;											//	装备分解表
	
	static CLifeSkillInfo mCollect;											// 采集技能信息
	static CLifeSkillInfo mGoldMake;										// 点金技能信息	
	static CLifeSkillInfo mArtificer;										// 巧匠技能信息
	static CLifeSkillInfo mPillMaker;										// 炼丹技能信息
	static CLifeSkillInfo mCooker;											// 烹饪技能信息		
	static void* spProgressConfig;											// 进度条配置信息
	static void* mActivityConfig;											// 活动配置表(绣球抢亲)	 	
	static void* spCorpsBOSSTable;											// 军团boss表
public:
	static int	InitializeFromPakFile();									// 从压缩包里读模板数据
	static int	Initialize( );
	static void	Finialize( );
	static CTemplate* SearchTpl( unsigned int vTempID );
	static void* SearchSkill( unsigned short vSkillID, unsigned short vLevel );
	static void* SearchMetier( unsigned short vMetierID );
	static void* SearchGener( unsigned short vGenerID );
	static void* GetLevelExp( );
	static void* GetData( );
	static void* GetUpgrade( );
	static void* GetOpenSlot( );
	static void* GetProSuit( );
	static void* GetProExp( );
	static void* GetBeginner( );
	static void* GetDropLevel( );
	static void* GetHonorToExp( );
	static void* GetChangeCamp();
	static void*  GetComRuneConfig();
	static void* GetHonorObtain( );
	static void* GetReliveTable( );	
	static void* GetBindChargeTable();
	static CTemplateBuff::CBuffFunc*	GetBuffFunc( CTemplateBuff* tpBuff, CTemplateBuff::BUFF_FUN_TYPE vType );
	static int GetProNum( );
	static int GetProEquip( unsigned short vType1, unsigned short vType2, unsigned short vLevel, unsigned short vRank = 0, unsigned short vType3 = 0 );
	static int GetProPhy( unsigned short vType1, unsigned short vType2 );
	static int GetChangeRandRuneId(unsigned short vlevel, int itemid );//add by  qiufufu 
	static int  GetMaxRuneIndex(unsigned short level,int maxindex);
	static int GetSuitID( unsigned short vIndex );
	static int GetMetierProperty( int vMetier, int vLevel, int vOption, int vProperty );
	static int GetLevelProperty( int vLevel, int vOption );
	static int GetCompose( int vComposeID );
	static void* GetTemp( TEMPTYPE vType );
	static void Clear();
	static int GetTempVersion();
	static CTitle* GetTitle( int vType, int vRootCndsn, int vSubCndsn, int& vTitleID );
	static CTitle* GetTitle( int vTitleID );
	static int GetTitle( int nParam, int nAction, int nLevel );
	static CJudgeLevel* GetJudgeLevel( int nValue, int nType, int& nNumerator, int& nDenominator );
	static int GetJudgeNumMax( int nLevel );

	static void* GetSkillLifeTable( int nSkillType );
	static void* GetSkillLifeInfo( int nSkillType );
	static int	InsertComposeInfo( int nLifeSkillType, int nComposeID, int nComposeKind, int nDegreeValue, int nComposeType );
	static int	GetCompoundIDList( int nDegreeValue, int nEquipLevel, int nEquipColor, int *pIDList,int *pNumList, int &nNum, int &rDegreeObtained );	
	static int GetWeddingMoney( int nWeddingModule );
	static int GetProgressTimeLength( int nFuncType );
	static bool IsWeddingHall( int nMapID );
	static bool IsInBaitangPos( int nWeddingModue, int nMapID, int nBridePosX, int nBridePosY, int nGroomPosX, int nGroomPosY );
	static int GetPreliminaryScore( int nPreliminaryToplistRank );

    static void* GetHandinEncourage( int nTempId );
    static void* GetHandinTable( int nActivityId );

	static int GetFameLimit( int nFameType );
};
#ifdef TEMPEDIT
#pragma pack(pop)
#endif
