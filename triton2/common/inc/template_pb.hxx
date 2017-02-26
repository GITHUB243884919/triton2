/******************************************************************************
版权所有 (C ), 2008-2018, 蓝港在线（北京）科技有限公司
******************************************************************************
文件名		: template_pb.hxx
版本号		: 1.0
作者		: Edwin
生成日期	: 2009-03-03
最近修改	: 
功能描述	: 定义的结构与template.h中的各类型相对应
			: [注] PB 全称[Protocal Buffers -- google开发协议]
函数列表	: 
修改历史	: 
修改内容	: 生成
*******************************************************************************/


/****************************	字段限制   ************************************
*
*	required: 必须赋值的字段，一定要想好了才能用这种类型
*	optional: 可有可无的字段
*	repeated: 可重复字段(变长字段)
*
******************************************************************************/

// 以最快速度序列/反序列
option optimize_for = SPEED;

message PBItemSet
{
	repeated PBItemBase				Item			= 1;	// 普通道具
	repeated PBItemBindStone		BindStone		= 2;	// 绑定石
	repeated PBItemBluePrint		BluePrint		= 3;	// 
	repeated PBItemBook				ItemBook		= 4;	// 技能书
	repeated PBItemCard				ItemCard		= 5;	// 卡片
	repeated PBItemExp				ItemExp			= 6;
	repeated PBItemHorse			Horse			= 7;
	repeated PBItemJewel			Jewel			= 8;
	repeated PBItemPetEgg			PetEgg			= 9;
	repeated PBItemPetFood			PetFood			= 10;
	repeated PBItemPhysic			Physic			= 11;	// 药
	repeated PBItemQuestion			Question		= 12;	// 答题
	repeated PBItemRelife			Relife			= 13;	// 复活
	repeated PBItemReturn			Return			= 14;	// 返回符
	repeated PBItemReturnPaper		ReturnPaper		= 15;	// 返回卷
	repeated PBItemRune				Rune			= 16;	// 
	repeated PBItemScroll			Scroll			= 17;	// 卷轴
	repeated PBItemSequence			Sequence		= 18;	// 道具排序
	repeated PBItemShout			Shout			= 19;	// 喊话
	repeated PBItemSkill			ItemSkill		= 20;	// 
	repeated PBItemStove			Stove			= 21;	// 炉石
	repeated PBItemStuff			Stuff			= 22;	// 原料
	repeated PBItemTask				ItemTask		= 23;	// 任务道具
	repeated PBItemTeleport			Teleport		= 24;	// 传送物品
	repeated PBItemTeleSymbol		TeleSymbol		= 25;	// 指定传送符
	repeated PBItemUpgLeast			UpgLeast		= 26;	// 升级保底符
	repeated PBItemUpgLuck			UpgLuck			= 27;	// 
	repeated PBItemYuanBug			YuanBug			= 28;	// 元气虫
	repeated PBItemYuanEgg			YuanEgg			= 29;	// 元气蛋
	repeated PBRedStone				RedStone		= 30;
	repeated PBItemAutoShout		AutoShout		= 31;
	repeated PBItemKitBag			KitBag			= 32;
	repeated PBItemUpg				ItemUpg			= 33;
	repeated PBItemRemove			ItemRemove		= 34;
	repeated PBItemChange			ItemChange		= 35;
	repeated PBItemAdapter			ItemAdapter		= 36;
	repeated PBItemMineMap			MineMap			= 37;	
	repeated PBItemCall				ItemCall		= 38;
	repeated PBItemGiftBag			GiftBag			= 39;
	repeated PBItemMagicWeapon		MagicWeapons	= 40;
	repeated PBItemToken			Token			= 41;
	repeated PBFuncItem				FuncItem	= 42;
	repeated PBItemSkillBook		SkillBook	= 43;	
	repeated PBItemRedPaper			RedPaper		= 44;
	repeated PBItemMagicStone		MagicStone		= 45;
	repeated PBItemVip				VipItem			= 46;
	repeated PBItemClean			CleanItem		= 47;
	repeated PBAwardInfo			AwardInfo		= 48;
	repeated PBLeagueBag			LeagueBag		= 49;
	repeated PBInvitation			Invitation		= 50;
	repeated PBSysInvitation		SysInvitation	= 51;	
	repeated PBItemRefresh			ItemRefresh		= 52;
	repeated PBItemSummon			ItemSummon		= 53;
	repeated PBItemDirect			ItemDirect		= 54;
	repeated PBItemGetStar			ItemGetStar		= 55;
	repeated PBItemAutoCollect		ItemAutoCollect	= 56;
	repeated PBItemBlackPills		ItemBlackPills	= 57;	
	repeated PBItemExpContainer		ExpContainer	= 58;	// 修炼经验丹
	repeated PBItemExpPills			ExpPills		= 59;	// 经验丹
	repeated PBSpiritItem			SpiritItem		= 60;	// 真灵
	repeated PBItemExpTome		ExpTome		=	61; //修炼心德
};


message PBConfigTable
{	
	repeated PBBeginner				Beginner		= 1;	// 新手配置
	repeated PBData					Data			= 2;	// 配置表
	repeated PBDiploidTable			Diploid			= 3;	// 领双表
	repeated PBDropLevel			DropLevel		= 4;	// 掉落等级
	repeated PBDropTable			DropTable		= 5;	// 掉落表
	repeated PBRedStoneExpTable		RedStoneTable	= 6;
	repeated PBReliveTable			ReliveTable		= 7;	// 复活表
	repeated PBSellTable			SellTable		= 8;	// 出售表
	repeated PBTeleTable			TeleTable		= 9;	// 传送表
	repeated PBAddPKTable			AddPkTable		= 10;
	repeated PBOfficialTable		OfficialTable	= 11;
	repeated PBOgreTran				OgreTran		= 12;
	repeated PBTplMagicWeaponProIncTable MagicWeaponProTable = 13;
	repeated PBBindChargeTable		BindChargeTabel = 14;	
	repeated PBPKValueTable			PKTable			= 15;
	repeated PBMagicWeaponSkillBoxProTable MagicWeaponSkillBox = 16;	
	repeated PBEquipExchangeTable	EquipExchange	= 17;
	repeated PBMapTable				MapTable		= 18;
	repeated PBMultiAttackTable		MultiAttackTable = 19;
	repeated PBMatrixTable			MatrixTable		= 20;
	repeated PBTemplateMarriageConfig MarriageConfig = 21; 	
	repeated PBProGressConfig		ProgressConfig	= 22;
	repeated PBCountTable			CountTable		= 23;
	repeated PBActivityConfig		ActivityConfig	= 24;	// 活动配置表
	repeated PBDirectTable			DirectTable		= 25;
	repeated PBWorldBattleConfig	BattleConfig	= 26;
	repeated PBLevelMoneyLmt		MoneyLmt		= 27;
    repeated PBHandinTable          HandinTable     = 28;   // 上缴奖励对照表
    repeated PBHandinEncourage      HandinEncourage = 29;   // 上缴奖励
	repeated PBItemChangeCamp       changeCamp      = 30;   // 转阵营
	repeated PBAccuseConfig			AccuseConfig	= 31;
	//repeated PBRuneConfig			RuneConfig      = 32; //符文转换升级
	repeated PBComRuneConfig		ComRuneConfig   = 32; //复杂符文升级配置
	repeated PBMagicWeaponDecomposeTable	MWDecomposTab = 33; // 法宝分解表
	repeated PBMagicWeaponQuickUpTable		MWQuickUpTab = 34; // 法宝升级表
	repeated PBGlobalMapConfig		GlobalMapConfig	= 35;
	repeated PBOffLineExpConfig		OffLineExpConfig = 36;
	repeated PBRepeFameTable		RepeFameTable	= 37;
	repeated PBCastingSpiritTable	CastingSpiritTable	= 38;
	
};

message PBSkillSet
{
	repeated PBAureoleSkill			AureoleSkill	= 1;	// 
	repeated PBBuff					Buff			= 2;	// Buff
	repeated PBNormalSkill			NormalSkill		= 3;	// 普通技能
	repeated PBSkill				Skill			= 4;	// 技能
	repeated PBSkillTree			SkillTree		= 5;	// 技能树
	repeated PBSkillTreeNode		SkillTreeNode	= 6;	// 技能树节点
	repeated PBTrapSkill			TrapSkill		= 7;
	repeated PBMultiBuff			MultiBuff		= 8;
	repeated PBFangCunTree			FangCunTree		= 9;	// 方寸山技能树
	repeated PBFangCunTreeNode		FangCunNode		= 10;	// 方寸山节点
};

message PBEquipSet
{
	repeated PBEquipArmor			EquipArmor		= 1;	
	repeated PBEquipFashion			EquipFashion	= 2;	// 时装
	repeated PBEquipment			Equipment		= 3;	// 普通装备
	repeated PBEquipUpg				EquipUpg		= 4;	// 装备升级
	repeated PBEquipWeapon			Weapon			= 5;	// 武器
	repeated PBOpenSlot				OpenSlot		= 6;	// 开槽
	repeated PBProduceEquip			ProduceEquip	= 7;	// 生产装备
	repeated PBProduceExp			ProduceExp		= 8;	
	repeated PBProducePhysic		ProducePhysic	= 9;
	repeated PBSpell				Spell			= 10;
	repeated PBSuitNormal			SuitNormal		= 11;	// 普通套装
	repeated PBSuitProducer			SuitProducer	= 12;	// 生产者套装
	repeated PBMultiCompose			MultiCompose	= 13;	// 复杂合成
	repeated PBCompose				Compose			= 14;	// 合成
	repeated PBCTemplateDecompound	Decompound		= 15;
	repeated PBLightTable			LightTable		= 16;
	repeated PBMWProJudgeEffTable	MWJudgeEff		= 17;
	repeated PBMWProJudgeSpirTable	MWJudgeSpir		= 18;
	repeated PBMWLevelQuotietyTab	MWQuotiety		= 19;
};	

message PBTemplate
{
	optional PBItemSet				ItemSet			= 1;
	optional PBConfigTable			ConfigTable		= 2;
	optional PBSkillSet				SkillSet		= 3;
	optional PBEquipSet				EquipSet		= 4;

	repeated PBBattlePet			BattlePet		= 5;	// 战斗宠

	repeated PBHonorObtain			HonorObtain		= 6;	// 功勋获得
	repeated PBHonorToExp			HonorToExp		= 7;	// 功勋转经验
	repeated PBLevelExp				LevelExp		= 8;	// 等级经验转换表
	repeated PBLevelProperty		LevelPro		= 9;	// 等级属性表
	repeated PBMetier				Metier			= 10;	// 职业
	repeated PBMetierPro			MetierPro		= 11;	// 职业属性
	repeated PBMine					Mine			= 12;	// 矿
	repeated PBNpc					Npc				= 13;	// NPC
	repeated PBNPCCollect			NpcCollect		= 15;	// 采集npc
	repeated PBOgreBox				Box				= 16;	// 宝箱
	repeated PBOgreProperty			OgreProperty	= 17;
	repeated PBPetLevelExp			PetLevelExp		= 18;
	repeated PBQuestionExp			QuestionExp		= 19;
	repeated PBShowPet				ShowPet			= 20;	// 观赏宠
	repeated PBSummonBeast			SummonBeast		= 21;	// 召唤兽	
	repeated PBTemplateProperty		TplProperty		= 22;
	repeated PBTemplatePropertyTable TplProTable	= 23;
	repeated PBTotalDrop			TotalDrop		= 24;
	repeated PBBlock				Block			= 25;	
	repeated PBTplHierogram			Hierogram		= 26;	
	repeated PBTemplateTalent		Talent			= 27;
	repeated PBTplBuffImmunity		BuffImmunity	= 28;
	repeated PBTemplateLifeSkill	LifeSkill		= 29;
			
	repeated PBLifeSkillScroll		LifeSkillScroll	= 30;
	repeated PBTitleManager			TitleManager	= 31;
	repeated PBSkillStorage				 MagicWeaponSkillStorage = 32;
	
	
	repeated PBCDGroupTable			CDGroupTable	= 33;					  
	repeated PBApConfig				ApConfig		= 34;
		
	repeated PBJudgeConfig			JudgeConfig		= 35;	
	repeated PBWizardTable			WizardTable		= 36;	
	
	repeated PBTrend				Trend			= 37;
	repeated PBAchieveAward			AchieveAward	= 38;
	repeated PBPower				Power			= 39;
	repeated PBOfficial				Official		= 40;	
	repeated PBConstructExp			ConstructExp	= 41;	
	repeated PBTemplateTalentStage	TalentStage		= 42;
	repeated PBCorpsBOSS			CorpsBOSS		= 43;

};

message PBHandinEncourage
{
    optional uint32     TempId               = 1; // 模板ID
    optional uint32     HandinItemType       = 2; // 上缴物品类型
    optional uint32     HandinItemID         = 3; // 上缴物品ID
    optional uint32		HandinItemNum        = 4; // 上缴物品数量
    optional uint32		EncourageType        = 5; // 奖励物品类型
    optional uint32		EncourageId          = 6; // 奖励物品ID
    optional uint32		EncourageNum         = 7; // 奖励物品数量	
};

//**********************************
// PBItemBase 道具基类型
//**********************************
message PBHandinTable
{
    optional uint32     TempID              = 1; // 模板编号
    optional uint32     ActivityId          = 2; // 活动编号
    repeated uint32     EncourageTplID      = 3; // 活动奖励表ID
};

//----------------------------------------------------
// 道具, PBItem### 对应template.h 中 CTplItem###
//----------------------------------------------------

//**********************************
// PBItemBase 道具基类型
//**********************************
message PBItemBase
{
	optional uint32		TempID				= 1;	// 模板ID
	optional uint32		ItemType			= 2;	// 道具类型
	optional uint32		ProcessType			= 3;	// 处理方式
	optional uint32		PileLimit			= 4;	// 堆叠上限
	optional string		ItemName			= 5;	// 道具显示名称
	optional string		ModelName			= 6;	// 道具掉落模型名称
	optional string		IconName			= 7;	// 道具图标名称
	optional uint32		PurchasePrice		= 8;	// 道具店卖价
	optional uint32		SalePrice			= 9;	// 道具卖店价
	optional uint32		PurchaseBindMoney	= 10;	// 店卖价是否支持绑定金钱
	optional uint32		SaleBindMoney		= 11;	// 卖店价是否支持绑定金钱
	optional string		Describe			= 12;	// 详细说明
	optional uint32		LastTime			= 13;	// 有效时间
	optional uint32		StopAttack			= 14;	// 使用后是否停止自动攻击
	optional string		TypeDesc			= 15;	// 类型描述
	optional uint32		CanUse				= 16;	// 右键点击是否可以使用
	optional uint32		SkillID				= 17;	// 使用的技能ID
	optional uint32		Level				= 18;	// 物品等级
	optional uint32		Disappear			= 19;	// 使用后是否消失
	repeated uint32		CDGroup				= 20;	// 冷却组
	repeated uint32		LmtMapID			= 21;	// 限制持有地图ID
	optional uint32		ValidTime			= 22;	// 有效时间
	optional uint32		ValidType			= 23;	// 有效时间计时类型
	optional uint32		CommonID			= 24;	// 公共组ID
	optional uint32		EatType				= 25;	// 化金类型
	optional uint32		EatValue			= 26;	// 化金值
	optional uint32		CountType			= 27;	// 使用类型
	optional uint32		LmtLineID			= 28;	// 限制使用的阵营ID
	repeated uint32		LmtUseMapID			= 29;	// 限制使用地图ID
	repeated uint32		LmtRepetionID		= 30;
	optional uint32		CompID				= 31;	// 合成配方ID
	optional uint32		InvilidDate			= 32;   // 到期绝对日期
};

//**********************************
// PBItemReturn 回城卷轴
//**********************************

message PBItemReturn
{
	optional PBItemBase	BaseInfo			= 1;	// 道具基本信息
	optional uint32		Timer				= 2;	// 道具使用时间
};

//**********************************
// PBItemSkillBook 法宝技能书
//**********************************

message PBItemSkillBook
{
	optional PBItemBase	BaseInfo			= 1;	// 道具基本信息
	optional uint32		Color				= 2;	// 对应技能框颜色
	optional uint32		SkillID				= 3;	// 技能ID
	optional uint32		SuccProb			= 4;	// 成功概率
	optional uint32		NeedMoneyType		= 5;	// 需要金钱类型
	optional uint32		NeedMoney			= 6;	// 需要金钱
};

//**********************************
// PBItemBindStone 绑定石
//**********************************

message PBItemBindStone
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		FaceGroupID			= 2;	// 表情组ID
	optional uint32      BindStoneType      = 3; //绑定石类型
};

//**********************************
// PBItemScroll 仙轴
//**********************************

message PBItemScroll
{
	optional PBItemBase	BaseInfo			= 1;	// 道具基本信息
	optional uint32		GetTaskID			= 2;	// 使用后触发任务
};


//**********************************
// PBItemCall 召唤类物品
//**********************************

message PBItemCall
{
	optional PBItemBase	BaseInfo			= 1;	// 道具基本信息
	optional uint32		CallNpcID			= 2;	// 召唤出来的NPC ID
	optional int32		X					= 3;	// 坐标X
	optional int32		Y					= 4;	// 坐标Y
	optional int32		IsDestroy			= 5;	// 使用后是否消失
	optional int32		MapID				= 6;	// 召唤地图ID
	optional int32		LmtMapID			= 7;	// 限制使用地图ID
	optional int32		LmtX				= 8;	// 限制使用坐标X
	optional int32		LmtY				= 9;	// 限制使用坐标Y
	optional int32		LmtWide				= 10;	// 限制宽度
	optional int32		LmtHeight			= 11;	// 限制高度
};


//**********************************
// PBItemCard 卡片
//**********************************

message PBItemCard
{
	optional PBItemBase	BaseInfo			= 1;	// 道具基本信息
	optional string		ShowText			= 2;	// 显示名称
	optional uint32		Order				= 3;	// 排名
	optional string		Weapon				= 4;	// 武器显示
	optional string		Skill				= 5;	// 绝技显示
	optional string		Level				= 6;	// 等级显示
	optional uint32		Exp					= 7;	// 经验
	optional uint32		CanExchange			= 8;	// 是否可以兑换
	optional uint32		CanChallenge		= 9;	// 是否可以挑战
	optional uint32		Type				= 10;	// 类型
	optional uint32		Pos					= 11;	// 位置
	optional uint32		Bonus				= 12;	// 奖励点
	optional uint32		SkillID1			= 13;	// 技能ID1
	optional uint32		SkillID2			= 14;	// 技能ID2
};

//**********************************
// PBItemTeleport 传送物品
//**********************************

message PBItemTeleport
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		MapID				= 2;	// 传送到达的地图ID
	optional uint32		UseTimes			= 3;	// 可以使用的次数
};

//**********************************
// PBItemReturnPaper 门派返回卷
//**********************************

message PBItemReturnPaper
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		LineID				= 2;	// 传送到达的线ID
	optional uint32		MapID				= 3;	// 传送道道的地图ID
	optional uint32		PosX				= 4;	// 传送到达的X坐标
	optional uint32		PosY				= 5;	// 传送到达的Y坐标
};

//**********************************
// PBItemTeleSymbol 传送符
//**********************************

message PBItemTeleSymbol
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		TeleTable			= 2;	// 传送表ID
	optional uint32		UseTimes			= 3;	// 可以使用的次数
};

//**********************************
// PBItemStove 炉石
//**********************************

message PBItemStove
{
	optional PBItemBase	BaseInfo			= 1;	// 道具基本信息
	optional uint32		LineID				= 2;	// 传送到达的线ID
	optional uint32		MapID				= 3;	// 传送到达的地图ID
	optional uint32		PosX				= 4;	// 传送到达的X坐标
	optional uint32		PosY				= 5;	// 传送到达的Y坐标
	optional uint32		CoolTime			= 6;	// 冷却时间
	optional uint32		MetierLimit			= 7;	// 职业限制
	optional uint32		LevelLimit			= 8;	// 等级限制
};

//**********************************
// PBItemPetEgg 宠物蛋
//**********************************

message PBItemPetEgg
{
	optional PBItemBase	BaseInfo			= 1;	// 道具基本信息
	optional uint32		PetTempID			= 2;	// 要传送的地图ID
	optional uint32		NeedItemID			= 3;	// 需要的道具ID
	optional uint32		RevertPrice			= 4;	// 还原所需金钱
	optional uint32		InitAdvice			= 5;	// 初始忠诚度
	optional uint32		InitLevel			= 6;	// 初始等级
	optional uint32		InitExp				= 7;	// 初始经验
	optional uint32		InitSkillPoint		= 8;	// 初始技能点
	repeated uint32		InitSkillID			= 9;	// 初始技能ID
	repeated uint32		InitSkillLevel		= 10;	// 初始技能等级
};

//**********************************
// PBItemPetFood 宠物食物
//**********************************

message PBItemPetFood
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		InitLevel			= 2;	// 初始等级
	optional uint32		FullLevel			= 3;	// 增加的饱食度
	optional uint32		TypeMask			= 4;	// 食物所属类别
};

//**********************************
// PBItemRelife 复活卷轴
//**********************************

message PBItemRelife
{
	optional PBItemBase	BaseInfo			= 1;	// 道具基本信息
	optional uint32		Timer				= 2;	// 使用时间
	optional uint32		CoolTime			= 3;	// 冷却时间
	optional uint32		ShareCD				= 4;	// 是否和其他物品共用冷却
};

//**********************************
// PBItemSkill 调用技能的物品
//**********************************

message PBItemSkill
{
	optional PBItemBase	BaseInfo			= 1;	// 道具基本信息
	optional uint32		LevelLimit			= 2;	// 等级限制
	optional uint32		SkillID				= 3;	// 调用的技能ID
	optional uint32		CoolTime			= 4;	// 冷却时间
	optional uint32		ShareCD				= 5;	// 是否和其他物品共用冷却
	optional uint32		OprType				= 6;	// 操作类型
	optional uint32     BuffID				= 7;    // BUFF ID
};

//**********************************
// PBItemBook 技能书
//**********************************

message PBItemBook
{
	optional PBItemBase	BaseInfo			= 1;	// 道具基本信息
	optional uint32		OPType				= 2;	// 操作的类型
};

//**********************************
// PBItemUpg 精炼石
//**********************************

message PBItemUpg
{
	optional PBItemBase	BaseInfo			= 1;	// 道具基本信息
	optional uint32		MaxLevel			= 2;	// 可以使用的等级
	optional uint32		Type				= 3;	// 类别
	optional uint32		Limit				= 4;	// 档次
	optional uint32		Fail				= 5;	// 是否失败
	repeated uint32		Luck				= 6;	// 升级成功附加
	optional uint32		UpType				= 7;	// 升级对象类型
};

//**********************************
// PBItemUpgLuck 装备升级幸运符
//**********************************

message PBItemUpgLuck
{
	optional PBItemBase	BaseInfo			= 1;	// 道具基本信息
	optional uint32		IncLuck1			= 2;	// 提高精炼成功的概率
	optional uint32		IncLuck2			= 3;	// 提高精炼失败后只降一级的概率
	repeated uint32		IncLuck3			= 4;	// 提高精炼成功附加升n级的概率
	optional uint32		Level				= 5;	// 需求的等级
};

//**********************************
// PBItemUpgLeast 装备升级保底符
//**********************************

message PBItemUpgLeast
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		IncLuck1			= 2;	// 提高精炼成功的概率
	optional uint32		IncLuck2			= 3;	// 提高精炼失败后只降一级的概率
	optional uint32		Type				= 4;
};

//**********************************
// PBItemRemove 宝石摘除符
//**********************************

message PBItemRemove
{
	optional PBItemBase	BaseInfo			= 1;	// 道具基本信息
	repeated uint32		Rand				= 2;	// 摘除降档的概率
	optional uint32		Bind				= 3;	// 摘除后宝石是否绑定
	optional uint32		Level				= 4;	// 档次
	optional uint32		Low					= 5;	// 
};

//**********************************
// PBItemChange 抗性转化石
//**********************************

message PBItemChange
{
	optional PBItemBase	BaseInfo			= 1;	// 道具基本信息
	optional uint32		Type				= 2;	// 转化的种类
	optional uint32		Res					= 3;	// 转化后的类型
	optional uint32		MetierRequire		= 4;	// 职业限制
};

//**********************************
// PBItemQuestion 答题卷
//**********************************

message PBItemQuestion
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	repeated uint32		QuestionRate		= 2;	// 答题卷出现n道题时的概率
	optional uint32		ExpAddRae			= 3;	// 答题卷的经验加成百分比
	optional uint32		EachTimeLength		= 4;	// 答题卷每道题目的答题时间限制,秒数，-1 表示没有限制
	optional uint32		TotalTimeLength		= 5;	// 答题卷所有题目的答题时间限制,秒数，-1表示没有限制
	optional uint32		RepeatTimeAllowed	= 6;	// 答题卷每道题目允许达错的次数，-1表示没有限制
};

//**********************************
// PBItemTask 任务道具
//**********************************

message PBItemTask
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		UseTimes			= 2;	// 使用次数
	optional uint32		ValidTime			= 3;	// 使用时走条时间
	optional string		ShowText			= 4;	// 走条时客户端显示文字
	optional uint32		MapID				= 5;	// 使用限制区域所在地图ID
	optional uint32		PosX				= 6;	// 使用限制区域左上角X
	optional uint32		PosY				= 7;	// 使用限制区域左上角Y
	optional uint32		Width				= 8;	// 使用限制区域宽度
	optional uint32		Height				= 9;	// 使用限制区域高度
	optional uint32		GetTaskID			= 10;	// 使用后触发任务
	optional uint32		GiveItemID			= 11;	// 使用后发放道具
	optional uint32		ChangeTime			= 12;	// x秒后变成其他物品
	optional uint32		ChangeItem			= 13;	// 变成的物品
	optional uint32		FailTaskID			= 14;   // 变化后强制让哪个任务失败
};

//**********************************
// PBItemStuff 生产原料
//**********************************

message PBItemStuff
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		ShowColor			= 2;	// 显示颜色
	optional uint32		ComposeID			= 3;	// 合成的配方ID
};

//**********************************
// PBItemExp 增加经验道具
//**********************************

message PBItemExp
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		IncRate				= 2;	// 增加倍率. Rate * 10000
	optional uint32		Time				= 3;	// 持续时间
};

//**********************************
// PBItemExp 增加经验道具
//**********************************

message PBItemChangeCamp
{
	optional uint32		TmpID				= 1;	// 模板ID
	optional uint32		m_itemId			= 2;	// 道具id
	optional uint32		m_giftId			= 3;	// 礼包id
	optional uint32     m_cd_time           = 4;    // cd 时间

};
//message PBRuneConfig
//{
//	optional uint32  TmpID					=1;  //模板ID
//	optional uint32  m_changeRuneItemId     =2;  //转换道具id
//
//}
message PBComRuneUnit
{
	optional uint32   m_whitenum			=1;	 //白色个数
	optional uint32   m_bluenum				=2;	 //蓝色个数
	optional uint32   m_purplenum			=3;	 //紫色个数
	optional uint32   m_rand				=4;	//%d升级概率
	optional uint32   m_money				=5;	//升级符文金钱
	optional uint32   m_runeLevel			=6;	//升级后的等级
};

message PBComRuneConfig //复杂符文升级配置
{
	optional uint32  TmpID					=1;  //模板ID
	optional uint32   m_upRuneRand			=2;  //%d升级符概率
	optional uint32   m_max					=3; //最大升级个数
	optional uint32   m_min					=4; //最小升级个数
	repeated PBComRuneUnit m_upRuneUnit     =5; //每种升级类型


}
 

//**********************************
// PBItemShout 世界喇叭
//**********************************

message PBItemShout
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		FaceGroupID			= 2;	// 表情组ID
};

//**********************************
// PBItemAutoShout 自动喊话
//**********************************

message PBItemAutoShout
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		FaceGroupID			= 2;	// 表情组ID
};

//**********************************
// PBItemPhysic 药品
//**********************************

message PBItemPhysic
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		ShowType			= 2;	// 生产类型
	optional uint32		BuffID				= 3;	// Buff编号
	optional uint32		LevelLimit			= 4;	// 等级限制
	optional uint32		HPMaxValue			= 5;	// HP最大值
	optional uint32		HPTime				= 6;	// HP恢复所需要的时间
	optional uint32		HPIncTime			= 7;	// HP每次恢复的时间间隔
	optional uint32		HPInvValue			= 8;	// HP恢复值
	optional uint32		MPMaxValue			= 9;	// MP最大值
	optional uint32		MPTime				= 10;	// MP恢复所需要的时间
	optional uint32		MPIncTime			= 11;	// MP每次恢复的时间间隔
	optional uint32		MPInvValue			= 12;	// MP恢复值
	optional uint32		CoolTime			= 13;	// 冷却时间
	optional uint32		ShareCD				= 14;	// 是否和其他物品共用冷却
	optional uint32		HPPercent			= 15;	// 自动恢复的红百分比
	optional uint32		MPPercent			= 16;	// 自动恢复的蓝百分比
	optional uint32		MaxHPPercent		= 17;	// 最多回复的红百分比
	optional uint32		MaxMPPercent		= 18;	// 最多回复的蓝百分比
	optional uint32		LifeSkilType		= 19;	// 生活技能类型
	optional uint32		DegreeValue			= 20;	// 使用后生活技能增长的熟练度
	optional uint32		PhisicType			= 21;	// 药品类型
	optional uint32		ApValue				= 22;	// 所含活力值
	optional uint32		Color				= 23;	// 药品颜色
};

//**********************************
// PBItemHorse 坐骑
//**********************************

message PBItemHorse
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		MetierLimit			= 2;	// 职业限制
	optional uint32		LevelLimit			= 3;	// 等级限制
	optional uint32		PersonLimit			= 4;	// 人数限制
	optional uint32		UseTime				= 5;	// 使用时间
	optional uint32		IncreaseSpeedPer	= 6;	// 速度增加百分比
	optional string		ModelName			= 7;	// 坐骑对应模型路径
	optional uint32		CoolTime			= 8;	// 冷却时间
	optional uint32		ShareCD				= 9;	// 是否和其他物品共用冷却
	optional uint32		SpeedBuff			= 10;	// 对应加速BUFF
	optional uint32		UsingTime			= 11;	// 使用时走条时间
	optional string		ShowText			= 12;	// 走条文字
	optional uint32		OffHorseProb		= 13;	// 被攻击下马概率
	optional uint32		OffBuffID			= 14;	// 下马后上的BUFF
	optional uint32		InitAP				= 15;	// 上马消耗的活力值
	optional uint32		DeltaAP				= 16;	// 飞行每秒消耗的活力值
};

//**********************************
// PBItemBluePrint 配方道具
//**********************************

message PBStuff
{
	optional uint32		StuffID		= 1;
	optional uint32		Number		= 2;
};
message PBProduct
{
	optional uint32		ProductID	= 1;
	optional uint32		Percent		= 2;	// 产出概率
};

message PBItemBluePrint
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		TempID				= 2;	// 模板ID
	optional uint32		NeedSkillID			= 3;	// 需要的技能ID
	optional uint32		NeedSkillLevel		= 4;	// 需要的技能等级
	optional uint32		ProduceCount		= 5;	// 一次生产的个数
	optional uint32		ProduceCost			= 6;	// 一次生产的手续费
	optional uint32		ObtainExp			= 7;	// 每次生产获得的基础经验
	repeated PBStuff	Stuff				= 8;	// 原料
	repeated PBProduct	Product				= 9;	// 产品
};

//**********************************
// PBItemRune 符文道具
//**********************************

message PBItemRune
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		SuitID				= 2;	// 套装编号
	optional string		Word				= 3;	// 符文字
	optional uint32		OrderID				= 4;	// 套装中的顺序
	optional uint32		InsertCharge		= 5;	// 镶嵌费用
	optional uint32		PropertyID			= 6;	// 新增附加属性ID
	optional uint32     RuneIndex			= 7;	// 符文等级列表
	optional uint32		ShowColor			= 8;	// 显示颜色
};

//**********************************
// PBItemJewel 珠宝道具
//**********************************

message PBItemJewel
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		TypeID				= 2;	// 珠宝等级
	optional uint32		Pos					= 3;	// 装备位置
	optional uint32		ShowColor			= 4;	// 显示颜色
	optional uint32		Property			= 5;	// 附加属性
	optional uint32		Unique				= 6;	// 是否唯一
	repeated uint32		JewelID				= 7;	// 降档得到的宝石ID
	optional uint32		ComposeID			= 8;	// 配方ID
	optional uint32		InsertCharge		= 9;	// 镶嵌费用
	optional uint32		RemoveCharge		= 10;	// 摘除费用
	optional uint32		ComposeCharge		= 11;	// 合成费用
	optional uint32		Level				= 12;	// 珠宝档次
	repeated uint32		Rand				= 13;
	optional uint32		NewJewelID			= 14;	// ID
	repeated uint32		NewRand				= 15;
	repeated uint32		CarveJewelID		= 16;	// 雕琢得到的宝石ID
	repeated uint32		CarveRand			= 17;	// 雕琢得到的概率
	optional uint32		CarveCharge			= 18;	// 雕琢费用
	optional uint32		CardID				= 19;	// 雕琢符ID
	optional uint32		CanCarve			= 20;	// 是否可以雕琢
	optional uint32		AffectRand			= 21;	// 影响合成概率
	optional uint32		JewComID			= 22;	// 宝石合成公用标志
	optional uint32		ComLevel			= 23;	// 合成等级
};

//**********************************
// PBItemYuanBug 元气虫
//**********************************

message PBItemYuanBug
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		Yuan				= 2;	// 存储上限
	optional uint32		EggID				= 3;	// 转化道具
	optional uint32		YuanType			= 4;	// 化金类型
};

//**********************************
// PBItemYuanEgg 元气蛋道具
//**********************************

message PBItemYuanEgg
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		Level				= 2;	// 等级
	optional uint32		ComposeID			= 3;	// 合成配方ID
	optional uint32		MaxEquip			= 4;	// 最大装备等级
};

//**********************************
// PBItemSequence 道具排列顺序
//**********************************

message PBItemSequence
{
	optional uint32		TempID				= 1;
	repeated uint32		Sequence			= 2;	// 道具类型
};

//---------------------------------------------
// 装备基础属性, 继承道具类型
//---------------------------------------------

//**********************************
// PBEquipment 装备道具
//**********************************

message PBEquipment
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional string		ModalName			= 2;	// 装备模型名称
	optional uint32		ShowLevel			= 3;	// 装备等级(0-15)
	optional uint32		EquipType			= 4;	// 装备部位
	optional uint32		ShowColor			= 5;	// 显示颜色
	repeated uint32		StrRequire			= 6;	// 力量条件
	repeated uint32		ConRequire			= 7;	// 体质条件
	repeated uint32		StaRequire			= 8;	// 耐力限制
	repeated uint32		WisRequire			= 9;	// 智力限制
	repeated uint32		SprRequire			= 10;	// 精神限制
	optional uint32		MetierRequire		= 11;	// 职业限制
	optional uint32		LevelRequire		= 12;	// 等级限制
	optional uint32		SexRequire			= 13;	// 性别限制
	optional uint32		YuanValue			= 14;	// 元气值
	optional uint32		EquipDurability		= 15;	// 装备耐久度
	optional uint32		BaseMendCharge		= 16;	// 基础修理金额
	optional uint32		NpcAbrasionRate		= 17;	// 与npc交互1次的损伤万分比
	optional uint32		PlayerAbrasionRate	= 18;	// 玩家交互1次的损伤万分比
	optional uint32		CanUpGrade			= 19;	// 是否可以升级
	optional uint32		UpgCharge			= 20;	// 升级需要的手续费
	optional uint32		NeedBindMoney		= 21;	// 是否需要绑定金钱
	repeated uint32		UpgPropertyID		= 22;	// 升级增加的属性ID
	repeated uint32		UpgValue			= 23;	// 生n级增加到的第m项属性值; value[m][n](0<=m<3,0<=n<15)
	repeated uint32		MaxSlotNumber1		= 24;	// 最大属性槽个数
	optional uint32		MaxSlotNumber2		= 25;	// 最大属性槽个数
	repeated uint32		PropertyPercent		= 26;	// 生成时附加属性数为n的概率(0<=n<5)
	repeated uint32		PropertyTableID		= 27;	// 附加属性表模板ID
	repeated uint32		PropertyPercentEx	= 28;	// 生成时附加属性数为n的概率(0<=n<5)
	repeated uint32		PropertyTableIDEx	= 29;	// 附加属性表模板ID
	repeated uint32		PAttackDiff			= 30;	// 升级增加的物理攻击附加
	repeated uint32		JudgePercent1		= 31;	// 第一次鉴定后属性为n的概率
	repeated uint32		JudgePercent2		= 32;	// 第二次鉴定后属性为n的概率
	repeated uint32		ResDefPercent		= 33;	// 抗性的概率
	repeated uint32		ResAttPercent		= 34;	// 属性攻击的概率
	repeated uint32		ResDefValue			= 35;	// 抗性升级的数值
	repeated uint32		ResAttValue			= 36;	// 属性攻击升级的数值
	optional uint32		SuitID				= 37;	// 套装ID
	optional uint32		SuitPos				= 38;	// 套装编号
	repeated uint32		MAttackDiff			= 39;	// 升级增加的法术攻击附加
	optional uint32		OpenCharge			= 40;	// 开槽费用
	optional uint32		AddCharge			= 41;	// 加槽费用
	optional uint32		JudgeCharge			= 42;	// 鉴定费用
	optional uint32		ReJudgeCharge		= 43;	// 再次鉴定费用
	optional uint32		DefUpgCharge		= 44;	// 抗性强化费用
	optional uint32		DefChangeCharge		= 45;	// 抗性转化费用
	optional uint32		AttUpgCharge		= 46;	// 属性强化费用
	optional uint32		AttChangeCharge		= 47;	// 属性转化费用
	optional uint32		CanDefUpgrade		= 48;	// 是否可以抗性升级
	optional uint32		CanAttUpgrade		= 49;	// 是否可以属性升级	
	optional uint32		BindBonus			= 50;	// 滴血增加属性比
	optional uint32		MultiCompose		= 51;	// 复杂合成配方
	optional uint32		CanChange			= 52;	// 是否可以转换
	optional uint32		ChangeCharge		= 53;	// 转换属性费用
	optional uint32		CanSplit			= 54;	// 是否可以拆分
	repeated uint32		SplitID				= 55;	// 拆分的ID
	repeated uint32		SplitNum			= 56;	// 拆分的数量
	optional uint32		SplitCharge			= 57;	// 拆分费用
	repeated uint32		UpgExtra1			= 58;	// 属性的铭刻值1
	repeated uint32		UpgExtra2			= 59;	// 属性的铭刻值2
	optional uint32		ExtCharge			= 60;	// 铭刻需要的手续费
	optional uint32		ResistTran			= 61;	// 是否能抵抗变身
	optional uint32		IsShenQi			= 62;	// 是否是神器


};

//**********************************
// PBEquipFashion 时装装备
//**********************************

message PBEquipFashion
{
	optional PBEquipment EquipInfo			= 1;	// 装备基本信息
	optional uint32	BuffID					= 2;
};

//**********************************
// PBEquipWeapon 武器装备
//**********************************

message PBEquipWeapon
{
	optional PBEquipment EquipInfo			= 1;	// 装备基本信息
	optional uint32		 WeaponType			= 2;	// 武器种类
	optional uint32		 AttackType			= 3;	// 攻击类型	
	optional uint32		 AttackDistance		= 4;	// 攻击距离		
};

//**********************************
// PBEquipArmor  防具武器
//**********************************

message PBEquipArmor
{
	optional PBEquipment EquipInfo			= 1;	// 装备基本信息
	optional uint32		ShowType			= 2;	// 防具类型
	optional string		ArmorSuit			= 3;	// 防具套装资源
};

//---------------------------------------------
// 装备属性, 附加属性表
//---------------------------------------------

//**********************************
// PBTemplateProperty 装备附加属性
//**********************************

message PBTemplateProperty
{
	optional uint32		TempID				= 1;	// 附加属性模板ID
	optional string		PropertyName		= 2;	// 附加属性名称
	optional uint32		PropertyFunc		= 3;	// 属性公式
	optional uint32		CanOut				= 4;	// 是否可以被转出
	optional uint32		IntMinValue			= 5;	// 固定值属性数值下限
	optional uint32		IntMaxValue			= 6;	// 固定值属性数值上限
	optional uint32		FloatMinValue		= 7; 	// 百分比属性数值下限
	optional uint32		FloatMaxValue		= 8;	// 百分比属性数值上限
	optional uint32		SkillID				= 9;	// 相关技能ID
	optional uint32		Area				= 10;	// 所属区域
};

//**********************************
// PBTemplatePropertyTable 附加属性表
//**********************************

message PBPropertyPercent
{
	optional uint32		TempID		= 1;
	optional uint32		Percent		= 2;	
};

message PBTemplatePropertyTable
{
	optional uint32			TempID			= 1;
	repeated PBPropertyPercent	Property	= 2;
};

//---------------------------------------------
// 玩家属性
//---------------------------------------------

//**********************************
// PBMetier  职业属性
//**********************************

message PBMetier
{
	optional uint32			TempID			= 1;	// 模板ID
	optional string			MetierName		= 2;	// 职业名称
	optional uint32			MetierID		= 3;	// 职业ID
	repeated uint32			GenerID			= 4;	// 职业的第n项技能系别ID
	optional uint32			SkillPoint		= 5;	// 每升一级获得技能点
	optional uint32			PropertyPoint	= 6;	// 每升一级获得属性点
	optional uint32			WalkSpeed		= 7;	// 职业行走速度
	optional uint32			RunSpeed		= 8;	// 职业奔跑速度
	optional uint32			InitLevel		= 9;	// 职业初始等级
	optional uint32			InitHP			= 10;	// 职业初始最大HP
	optional uint32			InitMP			= 11;	// 职业初始最大MP
	optional uint32			InitPAttack		= 12;	// 职业初始物理攻击
	optional uint32			InitPDefense	= 13;	// 职业初始物理防御
	optional uint32			InitMAttack		= 14;	// 职业初始法术攻击
	optional uint32			InitMDefense	= 15;	// 职业初始法术防御
	optional uint32			InitAbnResist	= 16;	// 职业初始异常状态抗性
	optional uint32			InitWindResist	= 17;	// 职业初始怒风状态抗性
	optional uint32			InitFireResist	= 18;	// 职业初始炽焰状态抗性
	optional uint32			InitWaterResist	= 19;	// 职业初始玄冰状态抗性
	optional uint32			InitLightResist	= 20;	// 职业初始纯阳状态抗性
	optional uint32			InitEarthResist	= 21;	// 职业初始凝尘状态抗性
	optional uint32			InitSadowResist	= 22;	// 职业初始寒阴状态抗性
	optional uint32			InitDeathRate	= 23;	// 职业初始暴击率
	optional uint32			InitDeathValue	= 24;	// 职业初始暴击伤害加成
	optional uint32			InitNmlHitrate	= 25;	// 职业初始普攻命中
	optional uint32			InitNormalDuck	= 26;	// 职业初始普攻闪避
	optional uint32			InitSkillDuck	= 27;	// 职业初始技能残影
	optional uint32			DeltaHP			= 28;	// 职业每升一级增加最大HP
	optional uint32			DeltaMP			= 29;	// 职业每升一级增加最大MP
	optional uint32			DeltaPAttack	= 30;	// 职业每升一级增加物理攻击
	optional uint32			DeltaPDefense	= 31;	// 职业每升一级增加物理防御
	optional uint32			DeltaMAttack	= 32;	// 职业每升一级增加法术攻击
	optional uint32			DeltaMDefense	= 33;	// 职业每升一级增加法术防御
	optional uint32			DeltaAbnResist	= 34;	// 职业每升一级增加异常状态抗性
	optional uint32			DeltaWindResist	= 35;	// 职业每升一级增加怒风状态抗性
	optional uint32			DeltaFireResist	= 36;	// 职业每升一级增加炽焰状态抗性
	optional uint32			DeltaWaterResist = 37;	// 职业每升一级增加玄冰状态抗性
	optional uint32			DeltaLightResist = 38;	// 职业每升一级增加纯阳状态抗性
	optional uint32			DeltaEarthResist = 39;	// 职业每升一级增加凝尘状态抗性
	optional uint32			DeltaShadowResist = 40;	// 职业每升一级增加寒阴状态抗性
	optional uint32			DeltaDeathRate	= 41;	// 职业每升一级增加暴击率
	optional uint32			DeltaDeathValue = 42;	// 职业每升一级增加暴击伤害加成
	optional uint32			DeltaNormalHitrate = 43;// 职业每升一级增加普攻命中
	optional uint32			DeltaNormalDuck = 44;	// 职业每升一级增加普攻闪避
	optional uint32			DeltaSkillDuck	= 45;	// 职业每升一级增加技能闪避
	optional uint32			PAttackPerStr	= 46;	// 每点力量对应物理攻击
	optional uint32			DeathValuePerStr = 47;	// 每点力量对应暴击伤害加成
	optional uint32			MaxHPPerStr		= 48;	// 每点力量对应HP
	optional uint32			PDefensePerCon	= 49;	// 每点体质对应物理防御
	optional uint32			HPRSpeedPerCon	= 50;	// 每点体质对应HP恢复速度
	optional uint32			MaxHPPerSta		= 51;	// 每点耐力对应最大HP
	optional uint32			AbnResistPerSta	= 52;	// 每点耐力对应最大每点耐力对应异常状态抗性
	optional uint32			MAttackPerWis	= 53;	// 每点智力对应法术攻击
	optional uint32			MaxMPPerWis		= 54;	// 每点智力对应最大MP
	optional uint32			DeathValuePerWis = 55;	// 每点智力对应暴击伤害
	optional uint32			MDefensePerSpr	= 56;	// 每点精神对应法术防御
	optional uint32			MaxMPPerSpr		= 57;	// 每点精神对应最大MP
	optional uint32			MPRSpeedPerSpr	= 58;	// 每点精神对应MP恢复速度
	optional uint32			NormalHPRSpeed	= 59;	// 普通状态HP恢复速度
	optional uint32			NormalSitHPRSpeed = 60;	// 普通打坐状态HP恢复速度
	optional uint32			BattleHPRSpeed	= 61;	// 战斗状态HP恢复速度
	optional uint32			BattleSitHPRSpeed = 62;	// 战斗状态打坐HP恢复速度
	optional uint32			NormalMPRSpeed	 = 63;	// 普通状态MP恢复速度
	optional uint32			NormalSitMPRSpeed = 64;	// 普通打坐状态MP恢复速度
	optional uint32			BattleMPRSpeed	 = 65;	// 战斗状态MP恢复速度
	optional uint32			BattleSitMPRSpeed = 66;	// 战斗状态打坐MP恢复速度

	optional uint32			InitIgnoreDuck	 = 67;	// 职业初始忽视闪避	
	optional uint32			DeltaIgnoreDuck = 68;	// 职业每升一级增加忽视闪避
	optional uint32			MAttackPerStr	= 69;	// 每点力量对应法术攻击
	optional uint32			DeathRatePerStr = 70;	// 每点力量对应致命等级
	optional uint32			MaxMPPerStr		= 71;	// 每点力量对应MP
	optional uint32			PDefensePerStr	= 72;	// 每点力量对应物理防御
	optional uint32			MDefensePerStr	= 73;	// 每点力量对应法术防御
	optional uint32			NmlHitratePerStr	= 74;	// 每点力量对应普攻命中
	optional uint32			NormalDuckPerStr	= 75;	// 每点力量对应普攻闪避
	optional uint32			IgnoreDuckPerStr	= 76;	// 每点力量对应忽视闪避
	optional uint32			PAttackPerCon	= 77;	// 每点体质对应物理攻击
	optional uint32			MAttackPerCon	= 78;	// 每点体质对应法术攻击
	optional uint32			DeathRatePerCon = 79;	// 每点体质对应致命等级
	optional uint32			DeathValuePerCon = 80;	// 每点体质对应暴击伤害加成
	optional uint32			MaxHPPerCon		= 81;	// 每点体质对应HP
	optional uint32			MaxMPPerCon		= 82;	// 每点体质对应MP
	optional uint32			MDefensePerCon	= 83;	// 每点体质对应法术防御
	optional uint32			NmlHitratePerCon	= 84;	// 每点体质对应普攻命中
	optional uint32			NormalDuckPerCon	= 85;	// 每点体质对应普攻闪避
	optional uint32			IgnoreDuckPerCon	= 86;	// 每点体质对应忽视闪避
	optional uint32			PReducePerSta		= 87;	// 每点耐力对应物理减伤百分比
	optional uint32			MReducePerSta		= 88;	// 每点耐力对应法术减伤百分比
	optional uint32			PAttackPerWis	= 89;	// 每点智力对应物理攻击
	optional uint32			MaxHPPerWis		= 90;	// 每点智力对应最大HP
	optional uint32			DeathRatePerWis = 91;	// 每点智力对应致命等级
	optional uint32			PDefensePerWis	= 92;	// 每点智力对应物理防御
	optional uint32			MDefensePerWis	= 93;	// 每点智力对应法术防御
	optional uint32			NmlHitratePerWis	= 94;	// 每点智力对应普攻命中
	optional uint32			NormalDuckPerWis	= 95;	// 每点智力对应普攻闪避
	optional uint32			IgnoreDuckPerWis	= 96;	// 每点智力对应忽视闪避
	optional uint32			PAttackPerSpr	= 97;	// 每点精神对应物理攻击
	optional uint32			MAttackPerSpr	= 98;	// 每点精神对应法术攻击
	optional uint32			DeathRatePerSpr = 99;	// 每点精神对应致命等级
	optional uint32			DeathValuePerSpr = 100;	// 每点精神对应暴击伤害加成
	optional uint32			MaxHPPerSpr		= 101;	// 每点精神对应HP
	optional uint32			PDefensePerSpr	= 102;	// 每点精神对应物理防御
	optional uint32			NmlHitratePerSpr	= 103;	// 每点精神对应普攻命中
	optional uint32			NormalDuckPerSpr	= 104;	// 每点精神对应普攻闪避
	optional uint32			IgnoreDuckPerSpr	= 105;	// 每点精神对应忽视闪避
	optional uint32			InitDeathDefense	 = 106;	// 职业初始致命防御	
	optional uint32			DeltaDeathDefense	= 107;	// 职业每升一级增加致命防御
	optional uint32			DeathDefensePerStr	= 108;	// 每点力量对应致命防御
	optional uint32			DeathDefensePerCon	= 109;	// 每点体质对应致命防御
	optional uint32			DeathDefensePerSta	= 110;	// 每点耐力对应致命防御
	optional uint32			DeathDefensePerWis	= 111;	// 每点智力对应致命防御
	optional uint32			DeathDefensePerSpr	= 112;	// 每点精神对应致命防御
};

//---------------------------------------------
// NPC 模板属性
//---------------------------------------------

//**********************************
// PBNpc 服务NPC (NPC继承Ogre)
//**********************************

message PBNpc
{
	optional PBOgreProperty	Ogre			= 1;	// Ogre属性
	optional uint32			FuncType		= 2;	// NPC类型
	optional string			Talk			= 3;	// 开场白
	optional uint32			StageID			= 4;	// 驿站ID
	optional uint32			TalkServiceID	= 5;	// 谈话服务ID
	optional uint32			SellServiceID	= 6;	// 出售服务ID
	optional uint32			MailService		= 7;	// 邮件服务
	optional uint32			StorageService	= 8;	// 仓库服务
	optional uint32			UpgService		= 9;	// 升级服务
	optional uint32			EqtBindService	= 10;	// 装备绑定服务
	optional uint32			WelfareService	= 11;	// 每日福利服务
	optional string			Describe		= 12;	// 称号
	optional uint32			StudySkillID	= 13;	// 学习技能职业ID
	optional uint32			TownX			= 14;	// 回城点X坐标
	optional uint32			TownY			= 15;	// 回城点Y坐标
	optional uint32			StorageTaskNpc	= 16;	// 皇榜任务npc
	optional uint32			StorageTaskID	= 17;	// npc挂的皇榜任务ID
	optional uint32			DownLevel		= 18;	// 皇榜任务等级下限
	optional uint32			UpLevel			= 19;	// 皇榜任务等级上限
	optional uint32			DiploidTable	= 20;	// 领双表
	optional uint32			ResetPService	= 21;	// 洗点
	optional uint32			FamilyService	= 22;	// 家族服务
	optional uint32			CorpService		= 23;	// 军团服务
	optional uint32			ChangeService	= 24;	// 转化服务
	optional uint32			JudgeService	= 25;	// 鉴定服务
	optional uint32			ActivityService = 26;	// 活动服务
	optional uint32			ExchangeService = 27;	// 兑换服务
	optional uint32			HonorService	= 28;	// 功勋服务
	optional uint32			Battle			= 29;	// 战场服务
	optional uint32			LifeSkillService= 30;	// 生活技能服务
	optional uint32			BidDay			= 31;	// 竞价周期
	optional uint32			BidHour			= 32;	// 竞价时
	optional uint32			BidMinute		= 33;	// 竞价分
	optional uint32			CoprsBattle		= 34;	// 军团战服务
	optional uint32			MinBidPrice		= 35;	// 竞价加价最小值
	optional uint32			BattleCity		= 36;	// 城池ID
	optional uint32			MWProJudgeService		= 37;	// 法宝属性鉴定服务
	optional uint32			MWSkillJudgeService		= 38;	// 法宝技能鉴定服务
	optional uint32			EquipExchangeService	= 39;	// 装备兑换的服务
	optional uint32			ExchangeTableID			= 40;	// 装备兑换表的ID	
	optional uint32			CorpsBidDay			= 41;	// 军团战竞价周期
	optional uint32			CorpsBidHour		= 42;	// 军团战竞价时
	optional uint32			CorpsBidMinute		= 43;	// 军团战竞价分		
	optional uint32			MWClearPointService		= 44;	// 法宝洗点服务
	optional uint32			MWSecondJudgeService	= 45;	// 法宝二次鉴定服务
	optional uint32			UnionService			= 46;	// 军团联盟服务
	optional uint32			CountryService			= 47;	// 国籍服务
	optional uint32			FriendService			= 48;	// 外交服务
	optional uint32			CountryType				= 49;	// NPC国籍
	optional uint32			GoldenYBService			= 50;	// 元宝领取服务
	optional uint32			ChairManTaskService		= 51;	// 掌门任务服务
	optional uint32			ChairManTaskID			= 52;	// 掌门任务ID
	optional uint32			MarriageService			= 53;	// 结婚服务
	optional uint32			MatrixService			= 54;	// 仙缘服务
	optional uint32			FamilyType				= 55;	// 家族NPC类型
	optional uint32			StoneBuff				= 56;	// 灵石BUFF
	optional uint32			Disappear				= 57;	// 获取后是否消失
	optional uint32			RefreshService			= 58;	// 刷属性服务
	optional uint32			PedalIndex				= 59;	// 查询的传送点索引
	optional uint32			DirectService			= 60;	// 摘星服务
	optional uint32			ExtraService			= 61;	// 印鉴服务
	optional uint32			WarService				= 62;	// 国战服务
	optional uint32			WarOpType				= 63;	// 国战属性
	optional uint32			WarValue				= 64;	// 国战值
	optional uint32			WarCamp					= 65;	// 国战阵营
	optional uint32			EquipLevelService		= 66;	// 装备升级的服务
	optional uint32			WarHonor				= 67;	// 国战战功
	optional uint32			FangCunService			= 68;	// 学习方寸山技能服务
	optional uint32			ResistTran				= 69;	// 是否能抵抗变身
	optional uint32			FBFunc					= 70;	// 副本特殊功能
	optional uint32			FBFunc_Parame1			= 71;	// 副本特殊功能参数1
	optional uint32			FBFunc_Parame2			= 72;	// 副本特殊功能参数2
	optional string			FBFunc_Str				= 73;	// 副本特殊功能字符串
	optional uint32			TaxService				= 74;	// 收税服务
	optional uint32			ChangeCampService		= 75;   // 转阵营服务 add by qiufufu
	optional uint32			WeakProtectService		= 76;	// 弱国保护服务
    optional uint32         YaoQianShuRank          = 77;   // 摇钱树排行榜
    optional uint32         Handin                  = 78;   // 上缴服务
	optional uint32         SpecialEvent            = 79;   // 特殊事件类型
    optional uint32         YaoQianInfo             = 80;   // 摇钱树活动说明
	optional uint32			TreasureBowl			= 81;	// 聚宝盆服务
	optional uint32			MWDecomposeService		= 82;	
	optional uint32			MWImprintService		= 83;	
	optional uint32			MWUpdateService			= 84;	
	optional uint32			MWUpdateExService		= 85;	
	optional uint32			CorpsBidStartDay		= 86;	// 军团战竞价开始周期
	optional uint32			CorpsBidStartHour		= 87;	// 军团战竞价开始时
	optional uint32			CorpsBidStartMinute		= 88;	// 军团战竞价开始分		
	optional uint32         LastOneService			= 89;   // 大逃杀说明
	optional uint32			FamilyBidStartHour		= 90;	// 家族战竞价开始时
	optional uint32			FamilyBidStartMinute	= 91;	// 家族战竞价开始分	
	optional uint32			MWQLComposeService		= 92;	
	optional uint32			RandomTeleService		= 93;	// 副本内的随机传送
	optional uint32			IntroduceMapID			= 94;	
	optional uint32			ShiTuService			= 95;	// 师徒副本说明
	optional uint32			SwornService			= 96;	// 结拜服务
	optional uint32			GlobalWarService		= 97;	// 跨服战说明
	optional uint32			SuperEquipExchange		= 98;	// 是否是神器兑换
};

//**********************************
// PBSellTable 出售表
//**********************************

message PBSellItem
{
	optional string		PageName		= 1;
	repeated uint32		ItemID			= 2;
	repeated uint32		ItemGoldPrice	= 3;	// 物品金锭价格
	repeated uint32		ItemNum			= 4;	// 物品出售的最大数量
	repeated uint32		ItemRefreshDay	= 5;	// 物品的刷新天数
};

message PBSellTable
{
	optional uint32			TempID			= 1;
	repeated PBSellItem		Item			= 2;
	optional uint32			SellTableType	= 3;
	optional uint32			WarCityMapID	= 4;		
};

//**********************************
// PBTotalDrop 掉落母表
//**********************************

message PBTotalDrop
{
	optional uint32			TempID			= 1;	// 模板ID
	repeated uint32			DropTableID		= 2;	// 掉落表
	repeated uint32			DropCount		= 3;	// 掉落次数
};

//**********************************
// PBDropTable 掉落子表
//**********************************

message PBDropItem
{
	optional uint32		DropItemID		= 1;
	optional uint32		DropProbability	= 2;	
	optional uint32		LevelProbablity = 3;
	optional uint32		Notify			= 4;
	optional uint32		Special			= 5;
	optional uint32		Show			= 6;
};

message PBDropTable
{
	optional uint32			TempID			= 1;	// 模板ID
	optional uint32			DropTurn		= 2;	// 掉落大轮数
	repeated uint32			DropNumberRate	= 3;	// 每大轮掉落n个物品的概率
	repeated PBDropItem		DropItem		= 4;	// 掉落物品
};

//**********************************
// PBNPCCollect 采集NPC
//**********************************

message PBNPCCollect
{
	optional uint32			TempID			= 1;	// 模板ID
	optional string			Name			= 2;	// NPC名称
	optional string			ModelName		= 3;	// 图形资源名称
	repeated uint32			TaskID			= 4;	// 第n个任务ID
	repeated uint32			ItemID			= 5;	// 第n个任务所需要的物品
	optional uint32			DropID			= 6;	// 掉落母表
	optional uint32			Number			= 7;	// 最大采集数
	optional uint32			SkillType		= 8;	// 生活技能类型
	optional uint32			SkillKind		= 9;	// 生活技能种类
	optional uint32			CollectDegree	= 10;	// 采集需要的熟练度	
	repeated uint32			value			= 11;	// 自身熟练度和采集需要的熟练度的差值区间
	repeated uint32			Probality		= 12;	// 在自身熟练度和采集需要的熟练度的相应差值区间获取数量度的概率
	repeated uint32			DegreeValue		= 13;	// 在自身熟练度和采集需要的熟练度的相应差值区间获取数量度的值	
	optional uint32			apcost			= 14;	// 生活技能采集需要消耗的活力值
	repeated uint32			ItemNum			= 15;	// 生活技能生成物品的数量
	repeated uint32 		ItemProbality	= 16;	// 概率
	optional uint32			CollectLevel	= 17;	// 采集需要的等级
	repeated uint32			ExtraItemNum = 18;	  //第n个任务额外产出的物品数量最大值
	repeated uint32			ExtraItemProbality = 19;   //第n个任务额外产出的概率
	optional uint32			CollectTaskType = 20;		//采集的任务类型

};

//**********************************
// PBOgreProperty 怪物属性
//**********************************

message PBOgreProperty
{
	optional uint32			TempID			= 1;	// 模板ID
	optional string			Name			= 2;	// NPC名称
	optional string			ModelName		= 3;	// 图形资源名称
	optional uint32			OgreType		= 4;	// 怪物类型
	optional uint32			DropType		= 5;	// 掉落方式
	optional uint32			StrategyID		= 6;	// 策略ID
	optional uint32			DropID			= 7;	// 掉落ID
	optional uint32			Level			= 8;	// 怪物等级
	optional uint32			DisplayeLevel	= 9;	// 怪物显示等级
	optional uint32			AttackRate		= 10;	// 攻击频率
	optional uint32			AttackTime		= 11;	// 攻击动作时间
	optional uint32			HPRSpeed		= 12;	// 怪物回血速度
	optional uint32			MPRSpeeds		= 13;	// 怪物回蓝速度
	optional uint32			RunSpeed		= 14;	// 怪物奔跑速度
	optional uint32			InitLevel		= 15;	// 怪物初始等级
	optional uint32			HP				= 16;	// 怪物初始最大HP
	optional uint32			MP				= 17;	// 怪物初始最大MP
	optional uint32			PAttack			= 18;	// 怪物初始物理攻击
	optional uint32			PDefence		= 19;	// 怪物初始物理防御
	optional uint32			PAttackRange	= 20;	// 怪物初始物理攻击距离
	optional uint32			PReducePer		= 21;	// 怪物初始物理减伤
	optional uint32			MAttack			= 22;	// 怪物初始法术攻击
	optional uint32			MDefence		= 23;	// 怪物初始法术防御
	optional uint32			MAttackRange	= 24;	// 怪物初始法术攻击距离
	optional uint32			MReducePer		= 25;	// 怪物初始法术减伤
	optional uint32			DeathRate		= 26;	// 怪物初始暴击率
	optional uint32			DeathValue		= 27;	// 怪物初始暴击伤害加成
	optional uint32			DeathDefense	= 28;	// 怪物初始暴击防御
	optional uint32			NormalHitrate	= 29;	// 怪物初始普攻命中
	optional uint32			NormalDuck		= 30;	// 怪物初始普攻闪避
	optional uint32			Recover			= 31;	// 怪物是否回血
	optional uint32			IgnoreDuck		= 32;	// 怪物初始忽视闪避
	repeated uint32			ResDef			= 33;	// 怪物初始抗性
	repeated uint32			ResAtt			= 34;	// 怪物初始属性攻击
	optional uint32			NeedDelay		= 35;	// 怪物是否远程攻击
	optional uint32			PatrolType		= 36;	// 巡逻类型
	optional uint32			IsInitiative	= 37;	// 是否主动攻击怪物
	optional uint32			WalkDistance	= 38;	// 休闲距离
	optional uint32			TeleTime		= 39;	// 传送时间
	optional uint32			EnmityDistance  = 40;	// 仇恨距离
	optional uint32			EnmityTime		= 41;	// 仇恨时间
	optional uint32			ClearTimer		= 42;	// 罚站时间(秒为单位)
	optional uint32			Exp				= 43;	// 怪物死亡后得到经验
	optional uint32			InitMoney		= 44;	// 怪物死亡后得到初始金钱
	optional uint32			DeltaMoney		= 45;	// 怪物死亡后得到金钱浮动值
	optional uint32			Resist			= 46;	// 免疫状态列表
	optional uint32			TranID			= 47;	// 怪物转换ID
	optional uint32			PkType			= 48;	// PK类型
	optional uint32			GroupID			= 49;	// 阵营ID
	optional uint32			SkillID			= 50;	// 技能ID
	optional uint32			SkillLevel		= 51;	// 技能等级
	optional uint32			SearchDistance	= 52;	// 搜索距离
	optional uint32			LeftTime		= 57;	// 存活时间(秒)(到时间就干掉)
	optional uint32			ResShow			= 58;	// 抗性显示
	repeated uint32		    FailTaskID		= 59;	// NPC消失后 任务失败
	optional uint32			JusticeValue	= 60;	// 怪物死亡后得到的正义值
	optional uint32			BuffImmunity	= 61;	// 免疫Buff状态列表
	optional uint32			ObtainTitle		= 62;	// 杀死本怪获得的称号ID
	optional uint32			AwardID			= 63;	// 领取奖励
	optional uint32			LmtLineID		= 64;	// 限制的服务器ID
	optional uint32			RedstoneID		= 65;	// 内丹ID
	optional uint32			OgreEvent		= 66;	// 怪物特殊事件
	optional uint32			CorpsValue		= 67;	// 怪物的魂魄值
	optional uint32			DropsDescID		= 69;	// 怪物掉落描述
	optional uint32			DiffcultIndex		= 70;	// 怪物难度系数
	optional uint32			DiffcultAward		= 71;	// 怪物难度奖励积分
};

//**********************************
// PBOgreBox 宝箱
//**********************************
message PBBox_Click
{
	optional uint32 Event								= 1;
	optional uint32 Probability							= 2;
};

message PBOgreBox
{
	optional PBOgreProperty	OgreInfo		= 1;	// 怪物属性
	repeated PBBox_Click		Click			= 2;	// 点击事件 
	optional uint32			BoxType			= 3;	// 宝箱类型
	optional uint32			KeyID			= 4;	// 钥匙ID
};

//**********************************
// PBShowPet 观赏宠
//**********************************

message PBShowPet
{
	optional uint32			TempID			= 1;	// 模板ID
	optional string			Name			= 2;	// 游戏中的显示名称
	optional string			Model			= 3;	// 模型路径
	optional string			IconName		= 4;	// 图标路径
	optional uint32			MetierID		= 5;	// 适合的职业
	optional uint32			MaxLevel		= 6;	// 宠物等级上限
	optional uint32			LevelRequire	= 7;	// 对玩家的等级要求
	optional uint32			ExceedLevel		= 8;	// 最多可以高出玩家多少级
	optional uint32			PetItemID		= 9;	// 从宠物蛋孵化成宠物需要的rmb道具id
	optional uint32			RevertCost		= 10;	// 从宠物还原成宠物蛋需要收取的虚拟币
	optional uint32			InitStorage		= 11;	// 初始仓库格子数
	optional uint32			LevelIncStorage = 12;	// 宠物每升多少级仓库格子数量增加
	optional uint32			IncStorage		= 13;	// 宠物因为升级每次增加的仓库格子数
	optional uint32			ExpPerMin		= 14;	// 每分钟观赏宠获得的经验
	optional uint32			RepletionLimit	= 15;	// 饱食度上限
	optional uint32			RepletionHP		= 16;	// 当宠物的饱食度达到什么数值时让主人战斗状态下回血
	optional uint32			RevertHPValue	= 17;	// 当宠物让主人在战斗状态下回血时一次回多少
	optional uint32			MinRepletion1	= 18;	// 状态1的饱食度下限
	optional uint32			MaxRepletion1	= 19;	// 状态1的饱食度上限
	optional uint32			ExpSpeedRate1	= 20;	// 在状态1下宠物获得经验的速度是标准速度的
	optional uint32			MinRepletion2	= 21;	// 状态2的饱食度下限
	optional uint32			MaxRepletion2	= 22;	// 状态2的饱食度上限
	optional uint32			ExpSpeedRate2	= 23;	// 在状态2下宠物获得经验的速度是标准速度的
	optional uint32			MinRepletion3	= 24;	// 状态3的饱食度下限
	optional uint32			MaxRepletion3	= 25;	// 状态3的饱食度上限
	optional uint32			ExpSpeedRate3	= 26;	// 在状态3下宠物获得经验的速度是标准速度的
	optional uint32			MinRepletion4	= 27;	// 状态4的饱食度下限
	optional uint32			MaxRepletion4	= 28;	// 状态4的饱食度上限
	optional uint32			ExpSpeedRate4	= 29;	// 在状态4下宠物获得经验的速度是标准速度的
	optional uint32			MinRepletion5	= 30;	// 状态5的饱食度下限
	optional uint32			MaxRepletion5	= 31;	// 状态5的饱食度上限
	optional uint32			ExpSpeedRate5	= 32;	// 在状态5下宠物获得经验的速度是标准速度的
	optional uint32			FoodCombo		= 33;	// 可吃的食物组合
};

//**********************************
// PBBattlePet 战斗宠
//**********************************

message PBBattlePet
{
	optional uint32			TempID					= 1;	// 模板ID
	optional string			Name					= 2;	// 游戏中的显示名称
	optional string			Model					= 3;	// 模型路径
	optional string			IconName				= 4;	// 图标路径
	optional uint32			MetierID				= 5;	// 适合的职业
	optional uint32			MaxLevel				= 6;	// 宠物等级上限
	optional uint32			LevelRequire			= 7;	// 对玩家的等级要求
	optional uint32			ExceedLevel				= 8;	// 最多可以高出玩家多少级
	optional uint32			PetItemID				= 9;	// 从宠物蛋孵化成宠物需要的rmb道具id
	optional uint32			RevertCost				= 10;	// 从宠物还原成宠物蛋需要收取的虚拟币
	optional uint32			RepletionLimit			= 11;	// 饱食度上限
	optional uint32			MinRepletion1			= 12;	// 状态1的饱食度下限
	optional uint32			MaxRepletion1			= 13;	// 状态1的饱食度上限
	optional uint32			ExpSpeedRate1			= 14;	// 在状态1下宠物获得经验的速度是标准速度的
	optional uint32			HPRate1					= 15;	// 在状态1下宠物的hp上限是正常的
	optional uint32			AttackRate1				= 16;	// 在状态1下宠物的攻击是正常的
	optional uint32			MinRepletion2			= 17;	// 状态2的饱食度下限
	optional uint32			MaxRepletion2			= 18;	// 状态2的饱食度上限
	optional uint32			ExpSpeedRate2			= 19;	// 在状态2下宠物获得经验的速度是标准速度的
	optional uint32			HPRate2					= 20;	// 在状态2下宠物的hp上限是正常的
	optional uint32			AttackRate2				= 21;	// 在状态2下宠物的攻击是正常的
	optional uint32			MinRepletion3			= 22;	// 状态3的饱食度下限
	optional uint32			MaxRepletion3			= 23;	// 状态3的饱食度上限
	optional uint32			ExpSpeedRate3			= 24;	// 在状态3下宠物获得经验的速度是标准速度的
	optional uint32			HPRate3					= 25;	// 在状态3下宠物的hp上限是正常的
	optional uint32			AttackRate3				= 26;	// 在状态3下宠物的攻击是正常的
	optional uint32			MinRepletion4			= 27;	// 状态4的饱食度下限
	optional uint32			MaxRepletion4			= 28;	// 状态4的饱食度上限
	optional uint32			ExpSpeedRate4			= 29;	// 在状态4下宠物获得经验的速度是标准速度的
	optional uint32			HPRate4					= 30;	// 在状态4下宠物的hp上限是正常的
	optional uint32			AttackRate4				= 31;	// 在状态4下宠物的攻击是正常的
	optional uint32			MinRepletion5			= 32;	// 状态5的饱食度下限
	optional uint32			MaxRepletion5			= 33;	// 状态5的饱食度上限
	optional uint32			ExpSpeedRate5			= 34;	// 在状态5下宠物获得经验的速度是标准速度的
	optional uint32			HPRate5					= 35;	// 在状态5下宠物的hp上限是正常的
	optional uint32			AttackRate5				= 36;	// 在状态5下宠物的攻击是正常的
	optional uint32			HPValue					= 37;	// 生命值
	optional uint32			HPPotential				= 38;	// 生命值潜力
	optional uint32			MPValue					= 39;	// 法力值
	optional uint32			MPPotential				= 40;	// 法力值潜力
	optional uint32			PAttack					= 41;	// 物理攻击力
	optional uint32			PAttackPotential		= 42;	// 物理攻击力潜力
	optional uint32			MAttack					= 43;	// 法术攻击力
	optional uint32			MAttackPotential		= 44;	// 法术攻击力潜力
	optional uint32			PDefence				= 45;	// 物理防御
	optional uint32			PDefencePotential		= 46;	// 物理防御潜力
	optional uint32			MDefence				= 47;	// 法术防御
	optional uint32			MDefencePotential		= 48;	// 法术防御潜力
	optional uint32			PResist					= 49;	// 物理免疫
	optional uint32			PResistPotential		= 50;	// 物理免疫潜力
	optional uint32			MResist					= 51;	// 法术免疫
	optional uint32			MResistPotential		= 52;	// 法术免疫潜力
	optional uint32			AbnResist				= 53;	// 异常状态基础抗性
	optional uint32			AbnResistPotential		= 54;	// 异常状态基础抗性潜力
	optional uint32			FireResist				= 55;	// 炽焰系抗性
	optional uint32			FireResistPotential		= 56;	// 炽焰系抗性潜力
	optional uint32			WindResist				= 57;	// 怒风系抗性
	optional uint32			WindResistPotential		= 58;	// 怒风系抗性潜力
	optional uint32			LightResist				= 59;	// 纯阳系抗性
	optional uint32			LightResistPotential	= 60;	// 纯阳系抗性潜力
	optional uint32			ShadowResist			= 61;	// 寒阴系抗性
	optional uint32			ShadowResistPotential	= 62;	// 寒阴系抗性潜力
	optional uint32			WaterResist				= 63;	// 玄冰系抗性
	optional uint32			WaterResistPotential	= 64;	// 玄冰系抗性潜力
	optional uint32			EarthResist				= 65;	// 凝尘系抗性
	optional uint32			EarthResistPotential	= 66;	// 凝尘系抗性潜力
	optional uint32			FoodCombo				= 67;	// 可吃的食物组合
};

//**********************************
// PBMine 矿
//**********************************

message PBMineOutput
{
	optional uint32		StuffID		= 1;
	optional uint32		Percent		= 2;
};
message PBMineOgre
{
	optional uint32		NpcID		= 1;
	optional uint32		Number		= 2;
	optional uint32		Percent		= 3;
};

message PBMine
{
	optional uint32				TempID				= 1;	// 模板ID
	optional string				Name				= 2;	// 游戏中的显示名称
	optional uint32				Level				= 3;	// 等级
	optional uint32				LevelRequire		= 4;	// 等级限制
	optional uint32				ToolID				= 5;	// 所需要的工具类型
	optional uint32				DecRate				= 6;	// 消耗采掘用具
	optional uint32				MineTimeLow			= 7;	// 采矿时间下限（秒）
	optional uint32				MineTimeHigh		= 8;	// 采矿时间上限（秒）
	optional string				Model				= 9;	// 模型路径名称
	repeated PBMineOutput		Output				= 10;	// 产出
	repeated PBMineOgre			MineOgre			= 11;	// 怪
};

//---------------------------------------------
// 技能
//---------------------------------------------

//**********************************
// PBSkillTreeNode 技能树节点
//**********************************

message PBSkillTreeNode
{
	optional uint32				SkillID				= 1;	// 技能ID
	optional uint32				MaxLevel			= 2;	// 最大等级( 最大技能点投入额度 )
	optional uint32				NeedSkillID			= 3;	// 需要的前导技能
	optional uint32				NeedSkillPoint		= 4;	// 需要的前导技能投入额度
	optional uint32				NeedGenerID			= 5;	// 需要投入的系别ID
	optional uint32				NeedGenerPoint		= 6;	// 需要投入的系别投入额度
	optional uint32				Level				= 7;	// 需要的玩家等级
	optional uint32				NeedBook			= 8;	// 是否需要对应的技能书
};

//**********************************
// PBSkillTree 技能树
//**********************************

message PBSkillTree
{
	optional uint32				TempID				= 1;	// 模板ID
	optional string				GenerName			= 2;	// 系别名称
	optional uint32				GenerID				= 3;	// 系别ID
	repeated PBSkillTreeNode	SkillNode			= 4;	// 
};

//**********************************
// PBSkill 技能基本信息
//**********************************

message PBCost
{
	optional uint32		CostType	= 1;
	repeated uint32		CostParam	= 2;
};
message PBUseCondition
{
	optional uint32		ConditionType	= 1;
	optional uint32		Param			= 2;
};

message PBSkill
{	
	optional uint32				TempID			= 1;	// 模板ID
	optional string				Name			= 2;	// 技能名称
	optional uint32				SkillID			= 3;	// 技能ID
	optional uint32				RequireWeapon	= 4;	// 技能需要的武器类别
	optional uint32				MetierID		= 5;	// 技能所属职业
	optional uint32				GenerID			= 6;	// 技能所属职业系别
	optional uint32				SkillLevel		= 7;	// 技能等级
	optional string				SkillIcon		= 8;	// 技能图标
	optional string				SkillDescribe	= 9;	// 技能描述
	optional uint32				FireType		= 10;	// 释放形态
	optional uint32				SkillType		= 11;	// 技能类型
	optional uint32				TimeType		= 12;	// 时间类型
	optional uint32				AniTime			= 13;	// 施放时间即动画时间(毫秒)
	optional uint32				FireTime		= 14;	// 吟唱时间(毫秒)
	optional uint32				CDTime			= 15;	// 冷却时间(毫秒)
	repeated PBCost				Cost			= 16;	// 消耗值
	optional uint32				NeedDelay		= 17;	// 技能是否需要延迟计算伤害
	optional uint32				SkillFuncID1	= 18;	// 对应第1项被动效果ID
	repeated uint32				Parameter1		= 19;	// 第1项效果ID对应数值
	optional uint32				SkillFuncID2	= 20;	// 对应第2项被动效果ID
	repeated uint32				Parameter2		= 21;	// 第2项效果ID对应数值
	optional uint32				SkillFuncID3	= 22;	// 对应第3项被动效果ID
	repeated uint32				Parameter3		= 23;	// 第3项效果ID对应数值
	optional uint32				SkillFuncID4	= 24;	// 对应第4项被动效果ID
	repeated uint32				Parameter4		= 25;	// 第4项效果ID对应数值
	optional string				SkillAction		= 26;	// 技能动画
	optional uint32				AutoRepeat		= 27;	// 是否自动重复
	optional uint32				AutoNormalAttack = 28;	// 是否自动普通攻击
	optional uint32				AutoCoolingSkill = 29;	// 是否冷却所有技能
	repeated PBUseCondition		UseCondition	= 30;	// 技能使用条件
	optional uint32				StudyGold		= 31;	// 学习技能所需金币
	optional uint32				StudyBindMoney	= 32;	// 学习技能所需金币是否绑定
	optional uint32				SkillListShowType = 33;	// 技能列表显示类型
	optional uint32				SkillTempType	= 34;	// 技能模板类型
	optional uint32				StudySkillLevel	= 35;	// 学习技能所需人物等级
	optional uint32				InsertAutoState	= 36;	// 是否进入自动重复攻击状态
	optional uint32				BreakAutoState	= 37;	// 是否打断自动重复攻击状态
	optional uint32				NeedTarget		= 38;	// 是否需要攻击目标
	optional uint32				NeedPet			= 39;	// 是否需要召唤兽
	optional uint32				ActionCamp		= 40;	// 可施放阵营
	optional uint32				DamageType		= 41;	// 伤害类型
	optional uint32				BindStudyGold	= 42;	// 学习技能所需金币是否支持绑定金
	optional uint32				AttackFix		= 43;	// 技能攻击力固定值
	optional uint32				AttackPer		= 44;	// 技能攻击力百分比
	optional uint32				ResType			= 45;	// 属性攻击类别
	optional uint32				ResValue		= 46;	// 属性攻击数值
	optional uint32				IsMWSkill		= 47;	// 是否法宝技能
	optional string				SkillTypeDesc	= 48;	// 技能类型描述
	optional uint32				IsMWOffset		= 49;	// 法宝是否偏移位置
	repeated uint32				CDGroup			= 50;	// 冷却组
	optional uint32				CanClean		= 51;	// 是否不可以被洗
	optional uint32				TargetLevel		= 52;	// 施放目标等级限制
	optional uint32				SummonIndex		= 53;	// 召唤的索引
};

//**********************************
// PBAureoleSkill 光环技能
//**********************************

message PBAureoleType
{
	optional uint32		AureoleArea = 1;
	optional uint32		BuffID		= 2;
};

message PBAureoleSkill
{	
	optional PBSkill			SkillInfo		= 1;	// 技能基本信息
	optional uint32				Inceessary		= 2;	// 是否持续消耗
	optional uint32				InceessaryTime	= 3;	// 持续消耗时间间隔
	repeated PBCost				Cost			= 4;
	optional uint32				AureoleDistance	= 5;	// 影响范围
	repeated PBAureoleType		AureoleType		= 6;	// 光环技能类型
};

//**********************************
// PBNormalSkill 普通技能
//**********************************

message PBNormalSkill
{
	optional PBSkill			SkillInfo		= 1;	// 技能基本信息
	optional uint32				SkillHitrate	= 2;	// 技能准确度	
	optional uint32				MinDistance		= 3;	// 开始吟唱距离
	optional uint32				MaxDistance		= 4;	// 追击距离
	optional uint32				EfktMaxNum		= 5;	// 最大作用数量
	optional uint32				AttackRange		= 6;	// 攻击范围类型
	repeated uint32				RandParam		= 7;	// 范围攻击第n参数(1,2)
	repeated uint32				BuffGroupID		= 8;	// Buff组
	optional uint32				ContinuedRule	= 9;	// 技能打断条件
	optional uint32				ContinuedTime	= 10;	// 持续攻击间隔时间
	optional uint32				ContinuedNum	= 11;	// 持续次数
	optional uint32				IgnoreArmor		= 12;	// 忽略护甲值	
};

//**********************************
// PBTrapSkill 陷阱技能
//**********************************

message PBTrapSkill
{
	optional PBSkill			SkillInfo		= 1;	// 技能基本信息
	optional uint32				LastTime		= 2;	// 持续时间
	optional uint32				ApplyType		= 3;	// 作用类型
	optional uint32				TouchRange		= 4;	// 触发范围（半径）
	optional uint32				ApplyRange		= 5;	// 作用范围（半径）
	optional string				TrapRes			= 6;	// 陷阱资源
	repeated uint32				BuffGroupID		= 7;	// Buff组
	repeated uint32				Condition		= 8;	// 激活条件
	optional uint32				EffectMaxNumber = 9;	// 最大作用人数
};

//---------------------------------------------
// Buff
//---------------------------------------------

//**********************************
// PBBuff buff模板
//**********************************

message PBBuffFunc
{
	optional uint32		FuncType		= 1;
	repeated uint32		Parame			= 2;
};

message PBBuff
{
	optional uint32				TempID			= 1;	// 模板ID
	optional uint32				BuffServiceID	= 2;	// Buff组系列ID
	optional uint32				IdenValue		= 3;	// Buff组评估值
	optional uint32				BuffStyle		= 4;	// Buff作用类型
	optional uint32				FirstActiveTime	= 5;	// 首次作用时间间隔
	optional uint32				ActiveTime		= 6;	// 作用时间间隔
	repeated PBBuffFunc			BuffFunc		= 7;	// 第n项功能码对应的功能数值
	optional string				BuffName		= 8;	// Buff的显示名称
	optional string				BuffIcon		= 9;	// Buff的图标
	optional string				BuffDescribe	= 10;	// Buff的显示悬浮提示
	optional uint32				Hitrate			= 11;	// Buff的绝对命中概率
	optional uint32				BuffValue		= 12;	// Buff值
	optional uint32				ResistType		= 13;	// Buff抗性系别
	optional uint32				LastTime		= 14;	// Buff持续时间毫秒(0表示立刻作用1次,仅1次,-1为无限)
	optional uint32				BuffType		= 15;	// Buff类型
	optional uint32				CancelBuff		= 16;	// Buff是否可以手动取消
	optional uint32				EnableBattle	= 17;	// Buff是否能够进入战斗
	optional string				BuffAni			= 18;	// Buff的动画特效
	optional uint32				BuffRow			= 19;	// Buff显示排数
	optional uint32				BuffDieDel		= 20;	// Buff死亡后是否消失
	optional uint32				TimerType		= 21;	// Buff的时间类型
	optional uint32				BuffClear		= 22;	// Buff能否清除
	optional uint32				BuffMapID		= 23;	// 特定地图ID
	optional uint32				BuffLineID		= 24;	// 特定线ID
	optional uint32				SkillLevel		= 25;	// 伤害参数
	optional uint32				BuffMapType		= 26;	// Buff地图类型
	optional uint32				CountryBuff		= 27;	// 国家Buff
	optional uint32				BuffSuperposition = 28; // Buff叠加类型
	optional uint32				Accurate		= 29;	// 命中
	optional uint32				OffRemove		= 30;	// 下线是否消失
	optional uint32				Transform		= 31;	// 是否复制变身
	optional uint32				RemoveCountID	= 32;	// Buff去掉叠加的ID
};

//**********************************
// PBLevelExp 等级经验表
//**********************************

message PBLevelExp
{
	optional uint32				TempID			= 1;	// 模板ID
	repeated uint64				LevelExp		= 2;	// 升级到N级的经验值
};

//**********************************
// PBLevelMoneyLmt 等级金钱限制表
//**********************************
message PBSingleMoneyLmt
{
	optional uint32				BagMoneyLmt				= 1;	// 包裹非绑定金
	optional uint32				BagBindMoneyLmt			= 2;	// 包裹绑定金
	optional uint32				StorageMoneyLmt			= 3;	// 仓库非绑定金
	optional uint32				StorageBindMoneyLmt		= 4;	// 仓库绑定金
};

message PBLevelMoneyLmt
{
	optional uint32				TempID					= 1;	// 模板ID
	repeated PBSingleMoneyLmt	LmtTable				= 2;
};


//**********************************
// PBPetLevelExp 宠物经验表
//**********************************

message PBPetLevelExp
{
	optional uint32				TempID			= 1;	// 模板ID
	repeated uint32				LevelExp		= 2;	// 升级到N级的经验值
};

//**********************************
// PBProduceExp 生产经验表
//**********************************

message PBProduceExp
{
	optional uint32				TempID			= 1;	// 模板ID
	repeated uint32				LevelExp		= 2;	// 第n级的经验值
};

//**********************************
// PBEquipUpg 装备升级表
//**********************************

message PBUpg
{
	optional uint32		Success		= 1;	// 成功的概率
	optional uint32		Failure		= 2;	// 失败的概率
	optional uint32		Result		= 3;	// 失败后的结果
	optional uint32		IncLevel	= 4;	// 升级成功后提升的等级
	optional uint32		ExtSuccess	= 5;	// 不掉级的成功概率
};

message PBEquipUpg
{
	optional uint32				TempID			= 1;	// 模板ID
	repeated PBUpg				EquipUpg		= 2;	// 升级
};

//**********************************
// PBSuitProducer 生产者套装
//**********************************

message PBSuitProducer
{
	optional uint32				TempID			= 1;	// 模板ID
	repeated uint32				PropertyTempID	= 2;	// 全身n件装备由同1玩家生产激活的附加属性ID
	repeated uint32				UpgTempID		= 3;	// 全身+n装备有m件激活的附加属性ID
	repeated uint32				ExtTempID		= 4;	// 全身装备有m件铭刻的附加属性ID
	repeated string				DescribeText	= 5;	// 附加属性n的描述
};

//**********************************
// PBSuitNormal 普通套装
//**********************************

message PBSuitNormal
{
	optional uint32				TempID			= 1;	// 模板ID
	optional string				Name			= 2;	// 套装名称
	repeated uint32				EquipTempID		= 3;	// 套装需要的装备ID
	optional string				ActiveTitle		= 4;	// 套装激活的玩家称号
	repeated uint32				PropertyTempID	= 5;	// 套装激活的玩家附加属性ID
};

//**********************************
// PBSpell 符文物语
//**********************************

message PBSpell
{
	optional uint32				TempID			= 1;	// 模板ID
	optional string				Name			= 2;	// 该套符文显示名称
	optional uint32				SuitID			= 3;	// 套装编号
	repeated uint32				LetterID		= 4;	// 该套符文需要的文字
	repeated uint32				Require			= 5;	// 该套符文激活的第一项附加属性需要符文数
	repeated uint32				PropertyTempID	= 6;	// 该套符文激活的第m项附加属性n  [m][n]
	repeated string				DescribeText	= 7;	// 附加属性n的描述
};

//**********************************
// PBProduceEquip 生产装备
//**********************************

message PBProduceEquip
{
	optional uint32				TempID			= 1;	// 模板ID
	optional uint32				Type			= 2;	// 装备类型
	optional uint32				DetailID1		= 3;	// 装备的详细类别
	optional uint32				DetailID2		= 4;	// 防具的详细类型
	optional uint32				Level			= 5;	// 装备等级
	optional string				Name			= 6;	// 装备名称
	optional uint32				MaterialID		= 7;	// 材料ID
	optional uint32				MaterialNum		= 8;	// 材料数量
	optional uint32				MaterialRank	= 9;	// 材料等级
	optional uint32				ItemID1			= 10;	// 本体1ID
	optional uint32				ItemRand1		= 11;	// 本体1概率
	optional uint32				ItemID2			= 12;	// 本体2ID
	optional uint32				ItemRand2		= 13;	// 本体2概率
	optional uint32				ItemID3			= 14;	// 本体3ID
	optional uint32				ItemRand3		= 15;	// 本体3概率
	optional uint32				Money			= 16;	// 所需银两
	optional uint32				BindMoney		= 17;	// 是否绑定金钱
	optional uint32				IconID			= 18;	// 图标ID
	optional string				Describe		= 19;	// 装备说明
	optional uint32				Exp				= 20;	// 所得经验
};

//**********************************
// PBProducePhysic 药品生产
//**********************************

message PBProducePhysic
{
	optional uint32				TempID			= 1;	// 模板ID
	optional uint32				Type			= 2;	// 药品类型
	optional uint32				Level			= 3;	// 药品等级
	optional string				Name			= 4;	// 药品名称
	optional uint32				MaterialID		= 5;	// 材料ID
	optional uint32				MaterialNum		= 6;	// 材料数量
	optional uint32				ItemID			= 7;	// 本体ID
	optional uint32				Exp				= 8;	// 所得经验
	optional uint32				Money			= 9;	// 所需银两
	optional uint32				BindMoney		= 10;	// 是否绑定金钱
};

//**********************************
// PBOpenSlot 开槽表
//**********************************

message PBOpenSlot
{
	optional uint32				TempID			= 1;	// 模板ID
	repeated uint32				SlotLevel		= 2;	// 打开第n个槽时需要的等级
	repeated uint32				SlotNumber		= 3;	// 打开第n个槽时需要的数量
};

//**********************************
// PBCompose 合成
//**********************************

message PBCompose
{
	optional uint32				TempID			= 1;	// 模板ID
	optional uint32				MaterialID		= 2;	// 合成需要的材料ID
	optional uint32				MaterialNum		= 3;	// 合成需要的材料数量
	optional uint32				ItemID			= 4;	// 合成得到的物品ID
	optional uint32				ItemNum			= 5;	// 合成得到的物品数量
};

//**********************************
// PBMultiCompose 复杂合成
//**********************************

message PBMultiCompose
{
	optional uint32				TempID				= 1;	// 模板ID
	optional string				Describe			= 2;	// 合成描述
	repeated uint32				MaterialID			= 3;	// 合成需要的材料ID
	repeated uint32				MaterialNum			= 4;	// 合成需要的材料数量
	repeated uint32				ItemID				= 5;	// 合成得到的物品ID
	repeated uint32				ItemNum				= 6;	// 合成得到的物品数量
	optional uint32				Exp					= 7;	// 合成得到的经验
	optional uint32				Order				= 8;	// 合成编号		 	
	optional uint32				LifeSkillType		= 9;	// 生活技能类型
	optional uint32				LifeSkillKind		= 10;	// 技能种类
	optional uint32				SpecializedDegree	= 11;	// 学习配方所需要的熟练度
	optional uint32				LifeComposeType		= 12;	// 配方类型
	optional uint32				CreatValue			= 13;	// 创建所需要的火候(烹饪等技能配方研究用)
	optional uint32				StudyMoney			= 14;	// 学习配方需要花费的金钱
	optional uint32				SkillCreatDegree	= 15;	// 研究新配方获取的熟练度
	repeated uint32				value				= 16;	// 自身熟练度和采集需要的熟练度的差值区间
	repeated uint32				Probality			= 17;	// 在自身熟练度和采集需要的熟练度的相应差值区间获取数量度的概率
	repeated uint32				DegreeValue			= 18;	// 在自身熟练度和采集需要的熟练度的相应差值区间获取数量度的值
	optional uint32				ApNeedCost			= 19;	// 配方学习需要消耗的活力值
	optional uint32				UsedTimes			= 20;	// 配方的使用次数
	optional uint32				MinValue			= 21;	// 差值最小范围( 创造配方时火候值和模板值配置的火候值小于该值时就算创造成功 )
	optional uint32				ComposeType			= 22;	// 配方类型
	optional uint32				EquipMetierType		= 23;	// 装备兑换时所适用的职业类型
	optional uint32				SrcEquipID			= 24;	// 源装备ID
	optional uint32				DesEquipID			= 25;	// 生成装备ID
	optional uint32				Contribute			= 26;	// 合成需要的贡献度	
	repeated uint32				UpgItemID			= 27;	// 附加需要的材料ID
	repeated uint32				UpgItemNum			= 28;	// 附加需要的材料数量
	repeated uint32				NeedValueType		= 29;	// 合成需要的数值类型
	repeated uint32				NeedValue			= 30;	// 合成需要的数值
};  

//**********************************
// PBTeleTable 传送表
//**********************************

message PBTeleTable
{
	optional uint32				TempID			= 1;	// 模板ID
	repeated uint32				LineID			= 2;	// 大地图ID(线ID)
	repeated uint32				MapID			= 3;	// 地图ID
	repeated uint32				PosX			= 4;	// X坐标
	repeated uint32				PosY			= 5;	// Y坐标
	repeated string				Describe		= 6;	// 描述
	repeated uint32				Money			= 7;	// 费用
	optional uint32				BindMoney		= 8;	// 是否绑定金钱
	repeated uint32				NeedTempID		= 9;	//  需求物品
	repeated uint32				NeedBuffID		= 10;	// 需求buff
	repeated uint32				NeedLevel		= 11;	// 需求等级
};

//**********************************
// PBData 数值修正表
//**********************************

message PBData
{
	optional uint32				TempID			= 1;	// 模板ID
	repeated uint32				LevelFix		= 2;	// 人物与怪物差级n掉落修正
	repeated uint32				TeamFix			= 3;	// 组队人数n的掉落修正
};

//**********************************
// PBSummonBeast 召唤兽
//**********************************

message PBPetData
{
	optional uint32		NameColor		= 1;	// 召唤兽名称颜色
	optional uint32		AttackType		= 2;	// 攻击类型
	optional uint32		PAttackUp		= 3;	// 召唤兽物理攻击力上限
	optional uint32		PAttackLow		= 4;	// 召唤兽物理攻击力下限
	optional uint32		MAttackUp		= 5;	// 召唤兽法力攻击力上限
	optional uint32		MAttackLow		= 6;	// 召唤兽法力攻击力下限
	optional uint32		Speed			= 7;	// 召唤兽移动速度
	optional uint32		PDefence		= 8;	// 召唤兽物理防御
	optional uint32		MDefence		= 9;	// 召唤兽法术防御
	optional uint32		MaxHP			= 10;	// 召唤兽HP上限
	optional uint32		HPRSpeed		= 11;	// 召唤兽HP恢复速率
	optional uint32		DeathRate		= 12;	// 召唤兽致命几率
	optional uint32		DeathValue		= 13;	// 召唤兽致命伤害
	optional uint32		NormalDuck		= 14;	// 召唤兽普攻闪躲值
	optional uint32		SkillDuck		= 15;	// 召唤兽技能闪躲值
	optional uint32		SkillID			= 16;	// 技能ID
	optional uint32		SkillLevel		= 17;	// 技能等级
	optional uint32		PAttackRange	= 18;	// 召唤兽物理攻击范围
	optional uint32		MAttackRange	= 19;	// 召唤兽法力攻击范围
	optional uint32		PReducePer		= 20;	// 召唤兽物理减伤
	optional uint32		MReducePer		= 21;	// 召唤兽法术减伤
	optional uint32		DeathDefense	= 22;	// 召唤兽致命防御
	optional uint32		IgnoreDuck		= 23;	// 召唤兽忽视闪躲值
	repeated uint32		ResDef			= 24;	// 抗性
	repeated uint32		ResAtt			= 25;	// 属性攻击
	optional uint32		AttPercent		= 26;	// 攻击加成
	optional uint32		DefPercent		= 27;	// 防御加成
	optional uint32		HPPercent		= 28;	// 生命加成
};

message PBSummonBeast
{
	optional uint32			TempID				= 1;	// 模板ID
	optional string			Name				= 2;	// 游戏中的显示名称
	optional string			Model				= 3;	// 模型路径
	optional uint32			Level				= 4;	// 召唤兽等级
	repeated uint32			LevelUpDelayTime	= 5;	// 升级到n级所需时间	
	repeated PBPetData		PetData				= 6;	// 召唤兽数据
	optional uint32			AttackFrequency		= 7;	// 召唤兽攻击频率
};

//**********************************
// PBBeginner 新手配置
//**********************************

message PBBeginner
{
	optional uint32			TempID				= 1;	// 模板ID
	repeated uint32			EquipItemID			= 2;	// 身上物品ID
	repeated uint32			EquipItemNum		= 3;	// 身上物品数量
	repeated uint32			BaggageItemID		= 4;	// 包裹物品ID
	repeated uint32			BaggageItemNum		= 5;	// 包裹物品数量
	repeated uint32			SkillID				= 6;	// 技能ID
	repeated uint32			SkillLevel			= 7;	// 技能等级
	optional uint32			StrNum				= 8;	// 力量点
	optional uint32			ConNum				= 9;	// 体质点
	optional uint32			StaNum				= 10;	// 耐力点
	optional uint32			WisNum				= 11;	// 智力点
	optional uint32			SprNum				= 12;	// 精神点
	optional uint32			SkillNum			= 13;	// 技能点
	optional uint32			PoxX1				= 14;	// 出生X坐标1
	optional uint32			PoxX2				= 15;	// 出生X坐标2
	optional uint32			PoxY1				= 16;	// 出生Y坐标1
	optional uint32			PoxY2				= 17;	// 出生Y坐标2
	repeated uint32			SkillID1			= 18;	// 转战士第n项初始技能
	repeated uint32			SkillLevel1			= 19;	// 转战士第n项初始技能等级
	repeated uint32			SkillID2			= 20;	// 转剑客第n项初始技能
	repeated uint32			SkillLevel2			= 21;	// 转剑客第n项初始技能等级
	repeated uint32			SkillID3			= 22;	// 转猎人第n项初始技能
	repeated uint32			SkillLevel3			= 23;	// 转猎人第n项初始技能等级
	repeated uint32			SkillID4			= 24;	// 转法师第n项初始技能
	repeated uint32			SkillLevel4			= 25;	// 转法师第n项初始技能等级
	repeated uint32			SkillID5			= 26;	// 转道士第n项初始技能
	repeated uint32			SkillLevel5			= 27;	// 转道士第n项初始技能等级
	repeated uint32			SkillID6			= 28;	// 转幻术师第n项初始技能
	repeated uint32			SkillLevel6			= 29;	// 转幻术师第n项初始技能等级
	optional uint32			BeginnerProtectLevel = 30;	// 新手保护等级
	optional uint32			TransProtectTime	= 31;	// 跨场景保护时间
	optional uint32			MapID				= 32;	// 出生地图
	optional uint32			DeathDuraRate		= 33;	// 玩家死亡时装备所掉耐久度占装备全部耐久度的万分比
	optional uint32			CanMendUseBindMoney = 34;	// 修理装备时是否能使用绑定金钱
	optional uint32			BeginnerAP			= 35;	// 出生活力值
};

//**********************************
// PBLoopTaskAwds 门派任务奖励表
//**********************************

//message PBLoopTaskAwds
//{
//	optional uint32			TempID				= 1;	// 模板ID
//	repeated uint32			Exp					= 2;	// 第m级完成第n环奖励经验
//	repeated uint32			Money				= 3;	// 第m级完成第n环奖励金钱
//};

//**********************************
// PBDropLevel 掉落等级
//**********************************

message PBDropLevel
{
	optional uint32			TempID				= 1;	// 模板ID
	repeated uint32			Ran					= 2;	// 填写n级掉n的概率
};

//**********************************
// PBOgreTran 怪物转换表
//**********************************

message PBOgreTran
{
	optional uint32			TempID				= 1;	// 模板ID
	repeated uint32			Drop				= 2;	// 第n级的掉落表
	repeated uint32			HP					= 3;	// 第n级的HP
	repeated uint32			MP					= 4;	// 第n级的MP
	repeated uint32			PA					= 5;	// 第n级的物攻
	repeated uint32			MA					= 6;	// 第n级的法攻
	repeated uint32			PD					= 7;	// 第n级的物防
	repeated uint32			MD					= 8;	// 第n级的法防
	repeated uint32			Hit					= 9;	// 第n级的命中
	repeated uint32			Dodge				= 10;	// 第n级的残影
	repeated uint32			Exp					= 11;	// 第n级的经验
	repeated uint32			Ignore				= 12;	// 第n级的忽闪
	repeated uint32			Death				= 13;	// 第n级的暴防
	repeated uint32			ResDef				= 14;	// 第n级的抗性
	repeated uint32			ResAtt				= 15;	// 第n级的攻击
};

//**********************************
// PBMetierPro 职业属性表
//**********************************

message PBMetierPro
{
	optional uint32			TempID				= 1;	// 模板ID
	optional uint32			MetierRequire		= 2;	// 职业限制
	optional uint32			Option				= 3;	// 类型选项
	repeated uint32			MetierProperty		= 4;	// 填写等级n对应第n项属性的值
};

//**********************************
// PBLevelProperty 等级属性表
//**********************************

message PBLevelProperty
{
	optional uint32			TempID				= 1;	// 模板ID
	optional uint32			Option				= 2;	// 类型选项
	repeated uint32			LevelProperty		= 3;	// 等级n对应的属性值
};

//**********************************
// PBHonorToExp 功勋经验表
//**********************************

message PBHonorToExp
{
	optional uint32			TempID				= 1;	// 模板ID
	repeated uint32			Exp					= 2;	// 等级n得到的经验值
	repeated uint32			Wizard				= 3;	// 等级n得到的才华值
	optional uint32			ExpLimit			= 4;	// 每日兑换的荣誉上限
	optional uint32			ItemID				= 5;	// 兑换得到的物品
	optional uint32			ItemHonor			= 6;	// 兑换物品所需荣誉
	optional uint32			ItemLimit			= 7;	// 每日兑换物品的荣誉上限
};

//**********************************
// PBHonorObtain 功勋获得表
//**********************************

message PBHonorObtain
{
	optional uint32			TempID				= 1;	// 模板ID
	optional uint32			InLimit				= 2;	// 每天能获得的功勋上限
	optional uint32			OutLimit			= 3;	// 每天能获得的功勋次数
	optional uint32			OutDieExp			= 4;	// 在外国死亡得到的功勋
	optional uint32			InDieExp			= 5;	// 在本国死亡得到的功勋
	optional uint32			BorderDieExp		= 6;	// 在中立死亡得到的功勋
	repeated uint32			OutLowExp			= 7;	// 等级低于目标n在外国得到的功勋
	repeated uint32			OutHighExp			= 8;	// 等级高于目标n在外国得到的功勋
	repeated uint32			InLowExp			= 9;	// 等级低于目标n在本国得到的功勋
	repeated uint32			InHighExp			= 10;	// 等级高于目标n在本国得到的功勋
	repeated uint32			BorderLowExp		= 11;	// 等级低于目标n在中立得到的功勋
	repeated uint32			BorderHighExp		= 12;	// 等级高于目标n在中立得到的功勋
	repeated uint32			LostLowExp			= 13;	// 等级低于目标n丢失的功勋
	repeated uint32			LostHighExp			= 14;	// 等级高于目标n丢失的功勋
	repeated uint32			BasicExp			= 15;	// 基础功勋
};

//**********************************
// PBDiploidTable 领双表
//**********************************

message PBDiploidTable
{
	optional uint32			TempID				= 1;	// 模板ID
	optional uint32			TotalTime			= 2;	// 每周可领取的总时间
	optional uint32			ValidMax			= 3;	// 有效双倍上限
	optional uint32			TypeNumber			= 4;	// 领取方式个数
	repeated uint32			BuffID				= 5;	// 领取方式
};

//**********************************
// PBRedStone 内丹
//**********************************

message PBRedStone
{		
	optional PBItemBase		BaseItem			= 1;	// 内丹的基础信息	
	optional string			MagicIconName		= 2;	// 噬炼图标名称
	optional uint32			GuageTime			= 3;	// 开启噬炼的进度条时间(毫秒)
	optional uint32			MagicSustainTime	= 4;	// 噬炼的时间(毫秒)
	optional uint32			ExpTime				= 5;	// 噬炼时分配经验的时间间隔(毫秒)
	optional uint32			Radius				= 6;	// 噬炼时的作用半径
	optional uint32			RedStoneType		= 7;	// 内丹的类型
	optional uint32			FamilyExpRate		= 8;	// 家族内丹的经验加成
	optional uint32			ExpTableID			= 9;	// 内丹经验表ID
	optional uint32			NumberEffected		= 10;	// 内丹是否影响经验加成
	optional uint32			MetierID			= 11;	// 门派ID	
	repeated uint32			KeyBuffID			= 12;	// 必需的第%d种buffid
	repeated uint32			EncBuffID			= 13;	// 第%d种加成buffid
	repeated uint32			EncPercent			= 14;	// 第%d种加成百分比
	optional uint32			IsEternal			= 15;	// 是否是永久
};

//**********************************
// PBRedStoneExpTable 内丹等级经验表
//**********************************

message PBRedStoneExpTable
{
	optional uint32			TempID				= 1;	// 模板ID
	repeated uint32			Exp					= 2;	// 使用内丹时n级玩家每次获得的经验值
	optional string			EffectName			= 3;	// 玩家噬炼特效名称
};

//**********************************
// PBReliveTable 复活花费表
//**********************************

message PBReliveTable
{
	optional uint32			TempID				= 1;	// 模板ID
	repeated uint32			Fee					= 2;	// 等级n复活花费金钱数
	optional uint32			Rate				= 3;	// 系数
};

//**********************************
// PBQuestionExp 答题等级经验表
//**********************************

message PBQuestionExp
{
	optional uint32			TempID				= 1;	// 模板ID
	optional uint32			NpcQuestionNum		= 2;	// 每日福利npc处的题目个数
	repeated uint32			Exp					= 3;	// 答题时等级为n的玩家获得的经验
	optional uint32			NpcExpAddRate		= 4;	// 每日福利题目经验加成倍数(基础经验的多少倍)
};

//**********************************
// PBBindChargeTable 绑定费用表
//**********************************

message PBBindChargeTable
{
	optional uint32			TempID				= 1;
	optional uint32			MoneyBindNeed		= 2; // 绑定装备需要花费的金钱
	optional uint32			CanBindUseBM		= 3; // 绑定装备是否支持绑定金钱 1表示支持0表示不支持
	optional uint32			MoneyUnBindNeed		= 4; // 取消装备绑定需要的金钱
	optional uint32			UnBindUseBM			= 5; // 取消装备是否支持绑定金钱 1表示支持0表示不支持
	optional uint32			MoneyResumeBind		= 6; // 恢复绑定需要的金钱
	optional uint32			CanResumeBind		= 7; // 恢复绑定是否支持绑定金钱 1表示支持0表示不支持
};

//**********************************
// PBItemMineMap 藏宝图
//**********************************

message PBMinePos
{
	optional uint32			MapID		= 1;
	optional uint32			PosX		= 2;
	optional uint32			PosY		= 3;
	optional uint32			Probability = 4;
};

message PBMineEvent
{
	optional uint32			EventID		= 1;
	optional uint32			TouchRate	= 2;
};

message PBItemMineMap
{
	optional PBItemBase		BaseInfo			= 1;
	repeated PBMinePos		MinePos				= 2;	// 挖掘地点
	repeated PBMineEvent	MineEvent			= 3;	// 事件
	optional uint32			Radius				= 4;	// 使用半径
	optional uint32			DropTable			= 5;
	optional uint32			DecBuff				= 6;
	optional uint32			OgreID				= 7;
	optional uint32			OgreNum				= 8;
	optional uint32			MapID				= 9;
	optional uint32			PosX				= 10;
	optional uint32			PosY				= 11;
	optional uint32			Special				= 12;
};

//**********************************
// PBBlock 阻挡NPC
//**********************************
message PBRelatively_Pos
{
	optional int32				X	= 1;
	optional int32				Y	= 2;
};

message PBBlock
{
	optional PBOgreProperty		BaseInfo	= 1;
	optional int32				Select		= 2;
	repeated PBRelatively_Pos	Pos			= 3;
};

message PBPKDrop
{
	optional uint32		State	= 1;
	optional int32		EquipDropRate	= 2;
	optional int32		BagDropRate1	= 3;
	optional int32		BagDropRate2	= 4;
	optional int32		GoPrisonRate	= 5;
	optional int32		PrisonTime		= 6;
};

message PBPKValueTable
{
	optional uint32			TempID				= 1;
	optional int32			NavyBlue			= 2;
	optional int32			Blue				= 3;
	optional int32			CambridgeBlue		= 4;
	optional int32			White				= 5;
	optional int32			Rosiness			= 6;
	optional int32			Red					= 7;
	optional int32			CarMine				= 8;
	optional int32			RedTopLimit			= 9;
	optional int32			YellowTime			= 10;
	optional int32			DecValue			= 11;
	repeated int32			DecExpRate			= 12;
	repeated int32			TopLimitDecExp		= 13;
	repeated int32			PriceChange			= 14;
	optional int32			LineID				= 15;
	optional int32			MapID				= 16;
	optional int32			PosX				= 17;
	optional int32			PosY				= 18;
	repeated PBPKDrop		Drop				= 19;
	optional uint32			FreeMapID			= 20;
	optional uint32			FreePosX			= 21;
	optional uint32			FreePosY			= 22;
	optional uint32			PrisonTime			= 23;
	optional uint32			BasePKValue			= 24;
	repeated uint32			PKValueLevel		= 25;
	optional uint32			DropRateInEnemy		= 26;
	optional uint32			PrisonBuffID		= 27;
};

//**********************************
// PBItemToken, 惩妖令牌，CTplToken
//**********************************

message PBItemToken
{
	optional PBItemBase		BaseInfo			= 1;
	repeated PBMinePos		Pos					= 2;
	optional uint32			Radius				= 3;
	optional uint32			OgreID				= 4;
	optional uint32			OgreNum				= 5;
};

//**********************************
// PBItemKitBag 背包
//**********************************
message PBItemKitBag
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		IndexNum			= 2;	// 背包格子数量
};

//**********************************
// PBItemGiftBag 礼包类物品
//**********************************
message PBItemGiftBag
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		DropTableID			= 2;	// 掉落表ID
    optional uint32 	MinLevel			= 3;	// 使用物品需要的最低等级
	optional uint32 	MinMoney			= 4;	// 金钱最小
	optional uint32		MinBindMoney 		= 5;	// 绑定金最小
	optional uint32		MinSilverYB			= 6; 	// 绑定元宝最小
    optional uint32     DropInBag           = 7;    // 掉落物品是否直接进包
	optional uint32 	MaxMoney			= 8;	// 金钱最大
	optional uint32		MaxBindMoney 		= 9;	// 绑定金最大
	optional uint32		MaxSilverYB			= 10; 	// 绑定元宝最大
	optional uint32		CheckBag			= 11;	// 是否检查包裹	
	optional uint32		MinExp				= 12;
	optional uint32		MaxExp				= 13;
	repeated uint32		LevelTableID		= 14;	// 按等级的掉落表
	optional uint32		LiHuaID				= 15;
	repeated uint32		OpenItemID			= 16;
	repeated uint32		OpenTableID			= 17;
};

//**********************************
// PBItemAdapter 物品适配器
//**********************************

message PBItemAdapter
{
	optional uint32			TempID				= 1;	// 模板ID
	optional uint32			ItemType			= 2;	// 模板类型
	optional uint32			RealType			= 3;	// 真实类型
	optional uint32			RealTempID			= 4;	// 真实模板ID
	optional uint32			MaxLevelMinus		= 5;	// 最大等级差
	optional uint32			Param				= 6;	// 物品参数
	optional uint32			MaxValue			= 7;	// 最大值
	optional uint32			MinValue			= 8;	// 最小值
};

//**********************************
// PBTplHierogram 物品适配器
//**********************************
message PBTplHierogram
{
	optional uint32			TempID				= 1;
	optional string			Res					= 2;
	optional uint32			Alive				= 3;
	optional uint32			Type				= 4;
	optional uint32			Parame1				= 5;
	optional uint32			Parame2				= 6;
};

//**********************************
// PBStep 造化小阶
//**********************************
message PBStep
{
	optional uint32		StageID	= 1;
	optional uint32		SkillTree = 2;
	repeated uint32		Next	= 3;
	//optional uint32		Exp		= 4;
	optional uint32		MaxPoint = 5;
	optional string		StageDesc = 6;
	optional uint32		Layer = 7;
};

message PBTitle
{
	optional string			Title	= 1;	// 称号
	optional uint32			Type	= 2;	// 类别
	optional uint32			ObtainCondition = 3; // 获得条件
	optional uint32			SubCondition = 4;	// 条件子项
	optional uint32			Color	= 5;	// 称号颜色
	optional uint32			AddProperty = 6;	// 对应附加属性
	optional uint32			IsHidden = 7;	// 称号是否隐藏
	optional uint32			IsReplace = 8;	// 是否直接替换原来的称号
	optional string			Descrbe	= 9;	// 描述
	optional uint32			ValidTime = 10;	// 有效时间
	optional uint32			IsTeamTitle = 11;	// 是否是团体称号
	optional uint32			Param = 12;		// 称号参数
	optional string			Detail	= 13;	// 称号说明
};

message PBTitleManager
{
	optional uint32		TempID = 1;
	repeated PBTitle	Title = 2;
};

message PBTplBuffImmunity
{
	optional uint32			TempID				= 1;
	repeated uint32			BuffType			= 2;
};

//**********************************
// PBTemplateTalent 造化配置
//**********************************
message PBTemplateTalent
{
	optional uint32	  TempID = 1;	// 模板ID
	repeated PBStep   Step = 2;
	repeated uint32	  Exp = 3;
};

// 法宝属性 增加表 表元
message PBMagicWeaponProIncValue
{
	optional uint32	LevelPoints			= 1;
	optional uint64	LevelExp			= 2;	// protobuf按8字节对齐！所以放到第二个位置
	optional uint32	LevelProcessVal		= 3;
	optional uint32	LevelProcessCurel	= 4;
	optional uint32	LevelProcessCurelVal= 5;
	optional uint32	LevelKeen			= 6;		
	optional uint32	LevelCalm			= 7;		
	optional uint32	LevelMagic			= 8;	
	optional uint32	LevelBier			= 9;		
	optional uint32	LevelImmortal		= 10;
};

//**********************************
// PBTemplateLifeSkill 生活技能模板
//**********************************
message PBTemplateLifeSkill
{
	optional uint32	TempID					=	1;		// 模板ID
	optional uint32	LifeSkillType			=	2;		// 生活技能类型
	optional uint32	LifeSkillKind			=	3;		// 生活技能种类	
	repeated uint32	DegreeStarted			=	4;		// 每一级生活技能开启的熟练度上限
	repeated uint32	StudyMoney				=	5;		// 每一级生活技能的学习金钱
	repeated string SkillName				=   6;		// 每一级的名称
	repeated uint32 CharacterNameID			=	7;		// 每一级技能给玩家的称号
	repeated uint32	StudyDegree				=	8;		// 每一级生活技能开启的熟练度上限
	repeated uint32	Level					=	9;		// 学习每一级生活技能需要的等级	
	optional uint32 SubSkillLevelLimit		=	10;		// 辅修技能能够学习的等级上限
	repeated uint32	DegreeValue				=	11;		// 生活技能晋级后获取的熟练度
};

//**********************************
// PBCTemplateDecompound 装备分解表
//**********************************
message PBDecompondTable
{
	optional uint32	FstItemID			=	1;			// 分解装备生成第一个物品的ID
	optional uint32	FstProbability		=	2;			// 分解装备生成第一个物品的概率	
	optional uint32	SecItemID			=	3;			// 分解装备生成第二个物品的ID
	optional uint32	SecProbability		=	4;			// 分解装备生成第二个物品的概率
	optional uint32	ThirdItemID			=	5;			// 分解装备生成第三个物品的ID
	optional uint32	ThirdProbability	=	6;			// 分解装备生成第三个物品的概率
	optional uint32	DegreeNeeded		=	7;			// 分解装备需要的熟练度
	optional uint32	FstItemNum			=	8;			// 分解装备生成第一个物品的ID
	optional uint32	SecItemNum			=	9;			// 分解装备生成第二个物品的ID
	optional uint32	ThirdItemNum		=	10;			// 分解装备生成第三个物品的ID
};
message PBCTemplateDecompound
{ 
	optional	uint32				TempID			=	1;		// 模板ID
	repeated 	PBDecompondTable	WhiteDecompond	=	2;		// 白装分解表
	repeated 	PBDecompondTable	BlueDecompond	=	3;		// 蓝装分解表
	repeated 	PBDecompondTable	PurpleDecompond	=	4;		// 蓝装分解表
	repeated	uint32				Value			=	5;		// 自身熟练度与分解表需要的熟练度的差值区间
	repeated	uint32				Probability		=	6;		// 分解装备获取熟练度的概率			
	repeated	uint32				DegreeValue		=	7;		// 分解装备获取熟练度的数值	
};
// 法宝属性 增加表
message PBTplMagicWeaponProIncTable
{
	optional uint32	  TempID = 1;	// 模板ID
	repeated PBMagicWeaponProIncValue Values	= 3;
};

// 法宝 分解表
message SingleMWDecomposeData
{
	optional uint32		TempID= 1; // 模板ID
	optional uint32		NumMin= 2; // 最少数量
	optional uint32		NumMax= 3; // 最大数量
	optional uint32		Prob  = 4; // 获得几率(万分比)
};							 
			
message PBMagicWeaponDecomposeTable
{
	optional uint32		TempID				= 1;	// 模板ID
	optional uint32		NeedMoneyType		= 2;
	optional uint32		NeedMoney			= 3;
	repeated SingleMWDecomposeData Items	= 4;
};

// 法宝飞升表
message PBMagicWeaponQuickUpTable
{
	optional uint32	TempID				= 1;	// 模板ID
	optional uint32	NeedItem		=	2;
	optional uint32	NeedItemNum	=	3;
	optional uint32	NeedMoneyType	=	4;
	optional uint32	NeedMoney		=	5;
	optional uint32	SuccessProb	=	6;
	optional uint32	NewMWID		=	7;
	optional uint32	NeedLevel		=	8;
	optional uint32	LeastLeve		=	9;
};

//**********************************
// PBItemMagicWeapon, 法宝
//**********************************

message PBItemMagicWeapon
{
	optional PBEquipment	Base					= 1;
	optional string			ShowLevel				= 2;
	optional string			ShowType				= 3;
	optional string			NewWeaponName			= 4;
	optional uint32			UmmiBuffID				= 5;
	optional uint32			InitLevel				= 6;
	optional uint32			InitMp					= 7;
	optional uint32			InitHp					= 8;
	optional uint32			InitMaxMp				= 9;
	optional uint32			InitMaxHp				= 10;
	optional uint32			InitBase				= 11;
	optional uint32			InitShape				= 12;
	optional uint32			InitEffect				= 13;
	optional uint32			InitSpir				= 14;
	optional uint32			IncMaxMp				= 15;
	optional uint32			IncMaxHp				= 16;
	optional uint32			MPIncValue				= 17;
	optional uint32			HPIncValue				= 18;
	optional uint32			MPIncValuePerMagic		= 19;		
	optional uint32			HPIncValuePerMagic		= 20;		
	optional uint32			AttackInc				= 21;
	optional uint32			RecovInc				= 22;
	optional uint32			MaxMPInc				= 23;
	optional uint32			MaxHPInc				= 24;
	optional uint32			DeadLineInc				= 25;
	optional uint32			CanUseBook				= 26;
	optional uint32			GetSkillScl				= 27;
	optional uint32			LevelTableID			= 28;	
	optional uint32			SkillBoxTableID			= 29;	
	optional uint32			DieOffHp				= 30;
	optional uint32			ProJudgeCost			= 31;
	optional string			DispResName				= 32;
	optional uint32			DefaultSkill			= 33;
	optional uint32			DefaultSkill2			= 34;
	optional uint32			JudgeEffTableID			= 35;	
	optional uint32			JudgeSpirTableID		= 36;	
	optional uint32			LevelQuotietyTabID		= 37;	
	optional uint32			DecomposeTabID			= 38;	
	optional uint32			LvByNum					= 39;
	optional uint32			QuickUpTabID			= 40;
	optional uint32			UpLevelTabID			= 41;	
};

// 法宝技能库
message PBSingleSkillStorage
{
	optional uint32			SkillID				= 1;
	optional uint32			FirstProb			= 2;
	optional uint32			OtherProb			= 3;
};

//**********************************
// PBItemMagicStone, 附魔物品
//**********************************
message PBItemMagicStone
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		Part				= 2;	// 附魔位置
	repeated uint32		Attribute			= 3;	// 附魔属性
	optional uint32		UsedTimes			= 4;	// 使用的次数
	repeated uint32		Probability			= 5;	// 每次附魔的概率
	optional uint32		LevelLimit			= 6;	// 附魔的等级限制
};

//**********************************
// PBLifeSkillScroll, 生活技能卷轴
//**********************************
message PBLifeSkillScroll
{
	optional PBItemBase BaseInfo			= 1;	// 道具基本信息
	optional uint32		ComposeID			= 2;	// 配方ID
	optional uint32		ComposeType			= 3;	// 配方类型		
};
message PBSkillStorage
{
	optional uint32					TempID		= 1;
	repeated PBSingleSkillStorage	Values		= 2;
};

// 法宝技能框属性表
message PBSkillBoxProperty
{
	optional uint32			SkillBoxColor		= 1;
	optional uint32			SkillBoxNeedVal		= 2;
	optional uint32			InitState			= 3;
	optional uint32			InitSkillID			= 4;
	optional uint32			SkillBoxType		= 5;
	optional uint32			SkillBoxSkill		= 6;
	optional uint32			SkillBoxActMoney	= 7;
	optional uint32			SkillBoxSkillStorage= 8;
};

message PBMagicWeaponSkillBoxProTable
{
	optional uint32					TempID		= 1;
	repeated PBSkillBoxProperty		Values		= 2;
};

// 法宝属性鉴定影响度表
message PBMWProJudgeEffTable
{
	optional uint32		TempID		= 1;
	repeated uint32		ProbFirst	= 2;
	repeated uint32		ProbSecond	= 3;
	repeated string		ShowName	= 4;
	repeated uint32		JudgeLevel	= 5;
};

// 法宝属性鉴定灵性表
message PBMWProJudgeSpirTable
{
	optional uint32		TempID		= 1;
	repeated uint32		ProbFirst	= 2;
	repeated uint32		ProbSecond	= 3;
	repeated string		ShowName	= 4;
	repeated uint32		JudgeLevel	= 5;
};

// 法宝精炼系数表
message PBMWLevelQuotietyTab
{
	optional uint32		TempID			= 1;
	repeated uint32		KeenQuotiety	= 2;
	repeated uint32		CalmQuotiety	= 3;
	repeated uint32		MagicQuotiety	= 4;
	repeated uint32		BierQuotiety	= 5;
	repeated uint32		ImmorQuotiety	= 6;
};

//**********************************
// PBFuncItem 功能性物品
//**********************************
message PBFuncItem
{
	optional PBItemBase	BaseInfo			= 1;	// 道具基本信息
	optional uint32		FuncCode			= 2;	// 功能码
	repeated uint32     Params				= 3;	// 参数
};

//**********************************
// PBFuncItem 功能性物品
//**********************************
message PBEquipExchangeTable
{
	optional uint32					TempID		 = 1;	// 模板ID
	repeated uint32					ComposeID	 = 2;	// 配方ID
	optional uint32					ExchangeType = 3;	// 兑换类型	
};

message PBMultiBuffEvent
{
	optional uint32	EventID = 1;
	optional uint32	TeamFlag = 2;
	optional uint32	Distance = 3;
	optional uint32	BuffID = 4;
	repeated uint32	RepetionIndex= 5;
};

message PBMultiBuff
{
	optional uint32	TempID = 1;
	repeated PBMultiBuffEvent BuffEvent = 2;
	repeated uint32	Param = 3;
	repeated uint32	NewBuff	= 4;
};

message PBCDGroupTable
{
	optional uint32	  TempID = 1;	// 模板ID
	repeated uint32	  CDTime = 2;
};

// 活力配置

message PBMetierAp
{
	optional uint32	Metier = 1;
	optional uint32	Ap = 2;
};

message PBApConfig
{
	optional uint32	TempID = 1;
	optional uint32	ConsumeHook = 2;
	optional uint32	WorldChat = 3;
	optional uint32	CampChat = 4;
	optional uint32	IncApMax = 5;
	repeated PBMetierAp MetierAp = 6;
	repeated uint32	RedPhysic = 7;
	repeated uint32	BluePhysic = 8;
	repeated uint32	ApPhysic = 9;
	repeated uint32 FbHpPhysic = 10;
	repeated uint32	FbMpPhysic = 11;
	repeated uint32	MendPhysic = 12;
};

message PBLightTable
{
	optional uint32	  TempID		= 1;	// 模板ID
	repeated uint32	  LevelPoint	= 2;
	repeated uint32	  JewelPoint	= 3;
	repeated uint32	  LevelGrade1	= 4;
	repeated uint32	  JewelGrade1	= 5;
	repeated uint32	  LevelGrade2	= 6;
	repeated uint32	  JewelGrade2	= 7;
};

message PBWizardTable
{
	optional uint32	TempID		=	1;
	repeated uint32	Exp			=	2;
	optional uint32 KeepRand	=	3;
	repeated uint32 TrendRand	=	4;
	repeated uint32 TrendResult =	5;
	optional uint32 Money		=	6;
};

message PBJudgeLevel
{
	optional uint32 JudgeType = 1;
	optional uint32	LevelMin	= 2;
	optional uint32	LevelMax	= 3;
	optional uint32	Star	= 4;
	optional uint32	LevelType = 5;
};

message PBJudgeConfig
{
	optional uint32		TempID		= 1;	// 模板ID
	optional uint32		OpenCD		= 2;	// 
	repeated PBJudgeLevel JudgeLevel = 3;
	optional uint32		ScoreBase	= 4;
	optional uint32		ScoreRate	= 5;
	optional uint32		ScoreLevel	= 6;
};


// 红包物品
message PBPaperValue
{	
	optional uint32	 Value		=	1;	//	数值
	optional uint32	 Probality	=	2;	//	概率
};

message PBItemRedPaper
{
   optional PBItemBase		BaseInfo			= 1;	// 道具基本信息
   optional uint32			BaseCharge			= 2;	// 购买金锭数 
   repeated PBPaperValue    MoneyInfo			= 3;	// 非绑定金钱的掉落信息
   repeated PBPaperValue	BindMoneyInfo		= 4;	// 绑定金钱的掉落信息
   optional uint32			RedPaperType		= 5;	// 红包类型
};

message PBTrend
{
	optional uint32		TempID		= 1;	// 模板ID
	repeated uint32		Bonus		= 2;
	optional string		Name		= 3;
	optional string		Desc		= 4;
};

message PBAward
{
	optional uint32		ItemID		= 1;
	optional uint32		ItemNum		= 2;
};

message PBAchieveAward
{
	optional uint32			TempID	= 1;
	repeated uint32			AwardID = 2;
	repeated uint32			AwardNum = 3;
	repeated uint32			Point = 4;
	optional uint32			SysMailID = 5;
};

message PBPower
{
	optional uint32		TempID		= 1;	// 模板ID
	optional string		Name		= 2;
	optional string		Desc		= 3;
	optional uint32		Type		= 4;
	optional uint32		Level		= 5;
	optional uint32		Official	= 6;
	optional uint32		Order		= 7;
	optional uint32		Param1		= 8;
	optional uint32		Param2		= 9;
	repeated uint32		UseTime		= 10;
	optional uint32		Construct	= 11;
	optional uint32		Power		= 12;
	optional uint32		Money		= 13;
	optional string		Icon		= 14;
	optional uint32		Country		= 15;
	optional uint32		AllCount	= 16;
	optional uint32		StartHour	= 17;
	optional uint32		StartMin	= 18;
	optional uint32		EndHour		= 19;
	optional uint32		EndMin		= 20;
};

message PBOfficial
{
	optional uint32		TempID		= 1;
	optional string		Name		= 2;
	optional string		Desc		= 3;
	optional uint32		Type		= 4;
	repeated uint32		Wizard		= 5;
	optional uint32		Power		= 6;
	optional uint32		CountryLevel = 7;
	optional uint32		ConstructLevel = 8;
};

message PBOfficialTable
{
	optional uint32		TempID		= 1;
	repeated uint32	OfficialData	= 2;	
};

message PBConstructExp
{
	optional uint32		TempID		= 1;
	repeated uint32		Exp			= 2;	
	repeated uint32		Bonus		= 3;
};

message PBAddPKTable
{
	optional uint32		TempID			= 1;
	optional uint32		KFChangePower	= 2;
	optional uint32		KPChangePower	= 3;
	optional uint32		KWChangePower	= 4;
	optional uint32		DFChangePower	= 5;
	optional uint32		DPChangePower	= 6;
	optional uint32		DWChangePower	= 7;
	optional uint32		KFChangeFriend	= 8;
	optional uint32		KPChangeFriend	= 9;
	optional uint32		KWChangeFriend	= 10;
	optional uint32		KFChangePK	= 11;
	optional uint32		KWChangePK	= 12;
	repeated uint32		KPChangePK	= 13;
	repeated uint32		PFriendMin	= 14;
	repeated uint32		PFriendMax	= 15;
	optional uint32		KFChangeHonor	= 16;
	optional uint32		KPChangeHonor	= 17;
	optional uint32		KWChangeHonor	= 18;
};

message PBMapTable
{
	optional uint32		TempID		= 1;
	repeated uint32		Level		= 2;	
	repeated uint32		Power		= 3;
};

message PBMultiAttackTable
{
	optional uint32		TempID		= 1;
	repeated uint32		Reduce		= 2;	
};

message PBItemVip
{
	optional PBItemBase BaseInfo = 1;
	optional uint32		VipLevel = 2;
	optional uint32		VipTime	 = 3;
};

message PBItemClean
{
	optional PBItemBase BaseInfo = 1;
	optional uint32		CleanType = 2;
	optional uint32		CleanLevel	 = 3;
	optional uint32		Step		= 4;
};

message PBAwardInfo
{
	optional uint32		TempID		= 1;
	optional string		Name		= 2;
	optional string		Desc		= 3;
	repeated uint32		ItemID		= 4;
	repeated uint32		ItemNum		= 5;
	optional uint32		BuffID		= 6;
	optional uint32		Times		= 7;
	optional uint32		NpcID		= 8;
	optional uint32		Country	    = 9;
	optional uint32		NoFamily	= 10;
};


// 家族军团特供物品
message PBLeagueItem
{
	optional uint32	ItemID		=	1;		// 物品ID
	optional uint32	ItemNum		=	2;		// 物品数量
	optional uint32	Probality	=	3;		// 物品的生成概率
};
message PBLeagueBag
{
	optional PBItemBase	  ItemBase		=  1;	// 物品基础信息
	repeated PBLeagueItem LeagueItemInfo = 2;	// 特供物品的生成物品
	optional uint32	LeagueBagType		 = 3;	// 特供物品的种类
	optional uint32	ExchangeTableID		 = 4;	// 特供物品所在的兑换表ID
	optional uint32	MapID				 = 5;	// 能够使用的地图ID
	optional uint32	PosX				 = 6;	// 能够使用的横坐标
	optional uint32	PosY				 = 7;	// 能够使用的纵坐标
};

message PBMatrixTable
{
	optional uint32		TempID				=	1;
	repeated uint32		Level				=	2;
	optional uint32		MaxLimit			=	3;
	optional uint32		MinLimit			=	4;
	repeated uint32		MasterItemID		=	5;
	repeated uint32		MasterItemNum		=	6;
	repeated uint32		MailID				=	7;
	optional uint32		FormalLevel			=	8;
	optional uint32		EducatedLevel		=	9;
	optional uint32		EnterLimit			=	10;
	optional uint32		RemoveItem			=	11;
	optional uint32		RemoveMoney			=	12;
	optional uint32		FormalPercent		=	13;
	optional uint32		EducatedPercent		=	14;
	optional uint32		ReturnLimit			=	15;
	optional uint32		AlterMax			=	16;
	optional uint32		AlterMin			=	17;
	optional uint32		ReturnMailID		=	18;
	repeated uint32		SlaveItemID			=	19;
	repeated uint32		SlaveItemNum		=	20;
	optional uint32		AlterAll			=	21;
	optional uint32		AlterDefault		=	22;
	optional uint32		SlaveMailID			=	23;
};

// 结婚系统的配置文件
message PBWeddingInfo
{
	optional uint32 WeddingLength			= 1;			// 婚礼的基本时长(分钟)
	optional uint32 MoneyNeeded				= 2;			// 婚礼费用
	optional uint32	DelayMoneyNeeded		= 3;			// 延长婚礼每分钟需要的费用
	optional uint32 WeddingRedstoneID		= 4;			// 婚礼内丹ID
	optional uint32	WeddingHallRedstoneID	= 5;			// 礼堂的内丹ID			
	optional uint32	WeddingHallMapID		= 6;			// 礼堂的地图ID
	optional uint32 InvitationCardID		= 7;			// npc发送的请帖的ID
	optional uint32 InvitationCardNum		= 8;			// npc发送的请帖的数量
	optional uint32 FemaleKeepsakeID		= 9;			// npc发送的女方信物ID
	optional uint32	MaleKeepsakeID			= 10;			// npc发送的男方信物ID
	optional uint32	PosX					= 11;			// 传入礼堂的横坐标
	optional uint32	PosY					= 12;			// 传入礼堂的纵坐标	
	optional uint32	FemaleKeepsakeTwo		= 13;			// npc发送的第二种女方信物ID
	optional uint32	MaleKeepsakeTwo			= 14;			// npc发送的第二种男方信物ID	
	optional uint32	FemaleKeepsakeThree		= 15;			// npc发送的第三种女方信物ID
	optional uint32	MaleKeepsakeThree		= 16;			// npc发送的第三种男方信物ID
	optional uint32	MaleBufferID			= 17;			// 新郎变身buff的模板ID
	optional uint32	FemaleBufferID			= 18;			// 新娘变身buff的模板ID	
	optional uint32	MaleItemID				= 19;			// 新郎变身物品的ID
	optional uint32	FemaleItemID			= 20;			// 新娘变身物品的ID	
	optional uint32	GroomPosX				= 21;			// 新郎拜堂时所在位置的横坐标
	optional uint32	GroomPosY				= 22;			// 新郎拜堂时所在位置的纵坐标
	optional uint32	BridePosX				= 23;			// 新娘拜堂时所在位置的横坐标
	optional uint32	BridePosY				= 24;			// 新娘拜堂时所在位置的纵坐标
	optional uint32	GroomBufferID			= 25;			// 新郎变身buff的模板ID
	optional uint32	BrideBufferID			= 26;			// 新娘变身buff的模板ID
	optional uint32	MarriageItemID			= 27;			// 结婚需要的道具ID
	optional uint32	WeddingScroll			= 28;			// 重温补办婚礼需要的婚礼卷轴	
};

message PBKeepsnakeInfo
{
	optional uint32	KeepsakeID				= 1;			// 信物ID
	optional uint32	EquipID					= 2;			// 信物兑换而成的装备ID	
};

message PBTemplateMarriageConfig
{
	optional uint32				TempID				= 1;		// 模板ID
	repeated PBWeddingInfo		WeddingInfo			= 2;		// 婚礼信息
	repeated PBKeepsnakeInfo 	FemaleKeepsakeInfo 	= 3;		// 女方信物信息
	repeated PBKeepsnakeInfo 	MaleKeepsakeInfo 	= 4;		// 男方信物信息
	repeated uint32				WeddingNpc			= 5;		// 婚礼npcID
	optional uint32				MarriageLevel		= 6;		// 结婚的最低等级
	optional uint32				MarriageMoney		= 7;		// 办证金钱	
	optional uint32				MapID				= 8;		// 从礼堂传出的地图ID
	optional uint32				PosX				= 9;		// 从礼堂传出的横坐标
	optional uint32				PosY				= 10;		// 从礼堂传出的纵坐标
	optional uint32				DivorceItemID		= 11;		// 离婚道具的ID
	optional uint32				MarriageTitleID		= 12;		// 结婚获取的称号ID	
	optional uint32				MaleKeepsakeID		= 13;		// 不举行婚礼时系统发放的男方信物ID	
	optional uint32				FemaleKeepsakeID	= 14;		// 不举行婚礼时系统发放的女方信物ID	
	optional uint32				InvitationID		= 15;		// 不举行婚礼时系统发放的请帖ID	
	optional uint32				InvitationNum		= 16;		// 不举行婚礼时系统发放的请帖数量
	optional uint32				MaleItemID			= 17;		// 不举行婚礼时系统发放的新郎变身物品的ID			
	optional uint32				FemaleItemID		= 18;		// 不举行婚礼时系统发放的新娘变身物品的ID
	optional uint32				BaitangBuffID		= 19;		// 礼堂里面拜堂时定身buff的ID	
	optional uint32				BasicMarriageItem	= 20;		// 结婚需要扣除的办证道具
	optional uint32				BasicMarriageItemNum= 21;		// 结婚需要扣除的办证道具的数量  
	optional uint32				MarriageNpcMapID	= 22;		// 结婚npc所在的地图ID
	optional uint32				MarriageNpcPosX		= 23;		// 结婚npc所在的横坐标
	optional uint32				MarriageNpcPosY		= 24;		// 结婚npc所在的纵坐标
	optional uint32				MarriageIntimacy	= 25;		// 结婚亲密度
	optional uint32				IntimacyItemNum		= 26;		// 亲密度足够时需要的石头
};

message PBInvitation
{
	optional PBItemBase		ItemBase			=	1;		// 道具基础信息	
	optional uint32			ExchangeItemID		=	2;		// 物品道具ID
};

// 进度条配置表
message ProGressInfo
{
	optional uint32	 FuncType		=	1;		// 功能类型
	optional uint32	 ProGressTime	=	2;		// 读进度条的时间
};
message PBProGressConfig
{
	optional uint32		  TmpID		=	1;		// 模板ID
	repeated ProGressInfo Progress	=	2;		// 进度条信息
};

// 系统请帖
message PBSysInvitation
{
	optional PBItemBase ItemBase		=	1;		// 道具基础信息
	optional uint32		InvitationID	=	2;		// 能够换取的请帖ID	
};

message PBCount
{
	optional uint32		CountType		=	1;
	optional uint32		CountTime		=	2;
	optional uint32		VipTime			=	3;
	optional uint32		ClearType		=	4;
};

message PBCountTable
{
	optional uint32		TempID			= 1;
	repeated PBCount	CountData		= 2;
};

// 活动配置表
message PBActivityConfig
{		
	optional uint32	 TempID								= 1;	// 模板ID
	optional uint32  HydrangeaThrowTime					= 2;	// 活动开始多久后扔绣球
	repeated uint32	 HydrangeaSugarID					= 3;	// 活动的喜糖变身buff的ID	
	optional uint32	 SugarSpreadTime					= 4;	// 多久撒一次喜糖
	optional uint32  SugarSpreadNum						= 5;	// 喜糖每次最多撒几个人		
	optional uint32  HydrangeaActivityTime				= 6;	// 抢亲活动的最大活动时间	
	optional uint32	 HydrangeaHolderTime				= 7;	// 每个玩家的绣球持有时间	
	optional uint32  HydrangeaRabTime					= 8;	// 绣球的抢夺时间	
	optional uint32	 HydrangeaRedstoneID				= 9;	// 抢绣球活动的内丹ID
	optional uint32	 HydrangeaExpTableID				= 10;	// 抢绣球活动经验奖励的等级经验表ID
	
	optional uint32	 PreliminaryTitleID					= 11;	// 乡试上榜后获取的称号		
	optional uint32	 PreliminaryBaseScore				= 12;	// 每答对一道题获取的基础积分
	repeated uint32	 PreliminaryExtralScore				= 13;	// 及时榜上榜后的额外积分
	optional uint32	 PreliminaryTipsTime				= 14;	// 发送乡试题目的预提醒时间
	optional uint32	 PreliminaryAnswerTime				= 15;	// 发送题目后多久发送答案
	optional uint32	 PreliminaryQuestionTime			= 16;	// 发送答案多久后发送题目
	optional uint32	 PreliminaryQuestionNum				= 17;	// 乡试的题目数量
	optional uint32	 PreliminaryExpTableID				= 18;	// 乡试的等级经验表ID	
	optional uint32  EffectiveOffLineExchangeRate		= 19;	// 在线时间和有效离线时间之间的兑换比例
	optional uint32	 MaxOnLineEffectiveTime				= 20;	// 每天可转换为有效离线时间的最大在线时间
	optional uint32	 MaxEffectiveOffLineTime			= 21;	// 最大的有效离线时间
	optional uint32	 MaxOffLineTime						= 22;	// 每天可转换的最大离线时间
	optional uint32	 OffLineTimeLimit					= 23;	// 可转换为经验的离线时间上线
	optional uint32	 VipOffLineExchangeRate				= 24;	// 会员在线时间和有效离线时间之间的兑换比例
};

// 属性刷新道具
message PBItemRefresh
{
	optional PBItemBase ItemBase		=	1;		// 道具基础信息
	optional uint32		Color			=	2;
	optional uint32		Level			=	3;
	repeated uint32		LimitPos		=	4;		// 限制部位
};

// 召集道具
message PBItemSummon
{
	optional PBItemBase ItemBase		=	1;		// 道具基础信息
	optional uint32		SummonType		=	2;
};

// 活动配置表
message PBDirectTable
{		
	optional uint32	 TempID					= 1;	// 模板ID
	repeated uint32	 DirectID				= 2;	// 得到的ID
	repeated uint32	 LuckID					= 3;	// 必成石ID
	repeated uint32	 LuckRate				= 4;	// 必成石概率
};

// 加星道具
message PBItemDirect
{
	optional PBItemBase ItemBase		=	1;		// 道具基础信息
	optional uint32		Level			=	2;		// 限制等级
	optional uint32		Star			=	3;		// 限制星级
};

// 摘星道具
message PBItemGetStar
{
	optional PBItemBase ItemBase		=	1;		// 道具基础信息
	optional uint32		Level			=	2;		// 限制等级
};

message PBWarDate
{
	optional uint32		Day				=	1;
	optional uint32		Hour			=	2;
	optional uint32		Minute			=	3;
};

message PBTax
{
	optional uint32		TaxRate			=	1;
	optional uint32		IncomeRate		=	2;
	optional uint32		Limit			=	3;
};

// 国战配置表
message PBWorldBattleConfig
{
	optional uint32		TempID			=	1;
	optional PBWarDate	SubmitBegin		=	2;		// 报名开始时间		
	optional PBWarDate	BattleBegin		=	3;		// 国战开始时间
	optional PBWarDate	BattleEnd		=	4;		// 国战结束时间
	repeated uint32		StoneID			=	5;		// 国战创建的内丹ID(1-10)
	optional uint32		InitBuffID		=	6;		// 初始的BUFFID
	optional uint32		FlagSource		=	7;		// 大旗的资源数
	optional uint32		KillSource		=	8;		// 杀人的资源数
	optional uint32		FlagBonus		=	9;		// 扛旗奖励
	repeated uint32		PerNum			=	10;		// 战场人数限制
	repeated uint32		MapID			=	11;		// 战场地图ID
	repeated uint32		SubmitMoney		=	12;		// 报名费
	optional uint32		SubmitLevel		=	13;		// 报名等级
	repeated uint32		PosX			=	14;		// 坐标X
	repeated uint32		PosY			=	15;		// 坐标Y
	optional uint32		OutMapID		=	16;		// 传出地图ID
	optional uint32		OutPosX			=	17;		// 传出坐标X
	optional uint32		OutPosY			=	18;		// 传出坐标Y
	repeated uint32		FlagIndex		=	19;		// 大旗索引
	optional PBWarDate	BidEnd			=	20;		// 竞价结束时间
	optional uint32		MaxWarHonor		=	21;		// 每日最大战功
	repeated uint32		Title			=	22;		// 职业称号
	optional PBWarDate	NoticeBegin		=	23;		// 提示开始时间
	optional uint32		TitleValidTime	=	24;		// 称号的有效时间
	optional uint32		StonePosX		=	25;		// 内丹坐标X
	optional uint32		StonePosY		=	26;		// 内丹坐标Y
	repeated PBTax		Tax				=	27;		// 税率
	optional uint32		WinTax			=	28;		// 获胜税率
};

// 自动采集道具
message PBDegreeInfo
{
	optional uint32	Value		= 1;		// 玩家本身熟练度与要求的熟练度的差值
	optional uint32	Probality	= 2;		// 在差值范围内获取熟练度的概率
	optional uint32	DegreeValue = 3;		// 在差值范围内获取熟练度的值
};
message PBItemInfo
{
	optional uint32	ItemID		= 1;	// 物品ID
	optional uint32	ItemNum		= 2;	// 物品数量
	optional uint32	Probality	= 3;	// 物品产出的概率
};
message PBItemAutoCollect
{
	repeated PBItemInfo ItemInfo		= 1;
	repeated PBDegreeInfo DegreeInfo	= 2;	
	optional uint32		MapID			= 3;
	optional uint32		PosX			= 4;	
	optional uint32		PosY			= 5;
	optional uint32		Grid			= 6;	
	optional uint32		MaxUseTimes   	= 7;
};

//**********************************
// PBFangCunTreeNode 方寸山节点
//**********************************

message PBFangCunTreeNode
{
	optional uint32				SkillID				= 1;	// 技能ID
	optional uint32				MaxLevel			= 2;	// 最大等级( 最大技能点投入额度 )
	repeated uint32				NeedSkillID			= 3;	// 需要的前导技能
	repeated uint32				NeedSkillPoint		= 4;	// 需要的前导技能投入额度
	optional uint32				Level				= 5;	// 需要的玩家等级
	optional uint32				NeedBook			= 6;	// 是否需要对应的技能书
	optional uint32				Stage				= 7;	// 阶
	optional uint32				Order				= 8;	// 顺序
};

//**********************************
// PBFangCunTree 方寸山技能树
//**********************************

message PBFangCunTree
{
	optional uint32				TempID				= 1;	// 模板ID
	repeated PBFangCunTreeNode	SkillNode			= 2;	// 
};

//**********************************
// PBBlackPills 黑狗丸
//**********************************
message PBItemBlackPills
{
	optional PBItemBase ItemBase		=	1;		// 物品基础信息
	optional uint32		ExchangeTime	=	2;		// 可以兑换的时间
	optional uint32		ExpTableID		=	3;		// 等级经验表
};

message PBStage
{
	optional uint32		StageID	= 1;
	repeated uint32		Next	= 2;
	optional uint32		Layer = 3;
};

message PBTemplateTalentStage
{
	optional uint32	  TempID = 1;	// 模板ID
	repeated PBStage  Stage = 2;
};

//**********************************
// PBItemExpContainer 修炼经验丹
//**********************************
message PBItemExpContainer
{
	optional PBItemBase ItemBase	=	1;	// 物品基础信息
	optional uint64 MaxExp			=	2;	// 能够存储的最大经验
	optional uint32	OgreExpRate		=	3;	// 打怪经验的百分比
	optional uint32 ExpPillsID		=	4;	// 经验存储满后能够获取的经验丹ID		
};

//**********************************
// PBItemExpPills 经验丹
//**********************************
message PBItemExpPills
{
	optional PBItemBase ItemBase	=	1;	// 物品基础信息
	optional uint64		Exp			=	2;	// 能够存储的最大经验
	optional uint32		LevelLimit	=	3;	// 使用等级限制
};


//**********************************
// PBItemExpPills 修炼心德
//**********************************
message PBItemExpTome
{
	optional PBItemBase ItemBase	=	1;	// 物品基础信息
	optional uint64		Exp			=	2;	// 能够存储的最大经验
	optional uint32		LevelLimit	=	3;	// 使用等级限制
	repeated uint32	OpenItemID	=	4; //所需物品 
};

// 弹劾配置表
message PBAccuseConfig
{
	optional uint32		TempID				=	1;
	optional uint32		AccuseBeginHour		=	2;		// 开始弹劾时
	optional uint32		AccuseBeginMinute	=	3;		// 开始弹劾分
	optional uint32		AccuseLevel			=	4;		// 弹劾必须等级
	optional uint32		AccuseEndHour		=	5;		// 结束弹劾时
	optional uint32		AccuseEndMinute		=	6;		// 结束弹劾分
	optional uint32		AccuseMoney			=	7;		// 弹劾花费金额
	optional uint32		AccusePerson		=	8;		// 弹劾最少人数
	optional uint32		AccusePercent		=	9;		// 弹劾比例
	optional uint32		AwardPower			=	10;		// 失败奖励声望
	optional uint32		GateNum				=	11;		// 大门死亡次数
	optional uint32		FlagNum				=	12;		// 大旗死亡次数
	optional uint32		GatePower			=	13;		// 大门获得声望
	optional uint32		FlagPower			=	14;		// 大旗获得声望
	optional uint32		AccuseNoticeHour	=	15;		// 提示弹劾时
	optional uint32		AccuseNoticeMinute	=	16;		// 提示弹劾分
	optional uint32		NoticePower			=	17;		// 提示声望
	optional uint32		GateParam1			=	18;		// 大门参数1
	optional uint32		FlagParam1			=	19;		// 大旗参数1
	optional uint32		GateParam2			=	20;		// 大门参数2
	optional uint32		FlagParam2			=	21;		// 大旗参数2
	optional uint32		GateParam3			=	22;		// 大门参数3
	optional uint32		FlagParam3			=	23;		// 大旗参数3
	optional uint32		LWPercent			=	24;		// 乱舞奖励加成
};

// 全局地图配置表
message PBGlobalMapConfig
{
	optional uint32		TempID			=	1;
	optional uint32		MapID			=	2;		// 全局地图
	optional uint32		BuffID			=	3;		// 需要的buffid
	optional uint32		Limit			=	4;		// 人数上限
	optional uint32		OutMap			=	5;
	optional uint32		OutX			=	6;
	optional uint32		OutY			=	7;
	optional uint32		EscapeBuffID	=	8;		// 跨服战场buffID
};

//军团boss表

message PBCorpsBOSS
{
	optional uint32	TempID		= 1;
	repeated uint32 BOSSID		= 2;	//boss id
	repeated uint32 HardBOSSID	= 3;	//稀有 boss id
	repeated uint32	ValueLimit	= 4;	//炼化值上限
	repeated string	DropDesc	= 5;	//boss掉落描述
	repeated string HardDropDesc= 6;	//hard boss 掉落描述
	repeated uint32 SummonMinLevel = 7; //召唤boss需要军团的最低等级
	repeated uint32 HardSummonMinLevel = 8; //召唤hard boss需要军团的最低等级
	repeated uint32 SummonMinValue = 9; //召唤boss时需求的最低炼化度
	repeated uint32 HardSummonMinValue = 10; //召唤hard boss时需求的最低炼化度
};


// 离线经验配置表
message PBOffLineExpConfig
{
	optional uint32		TempID				=	1;
	optional uint32		OffLineUnit			=	2;
	repeated uint32		OffLineExp			=	3;		
	optional uint32		OffLineLevel		=	4;
	repeated uint32		ExpLimit			=	5;
	optional uint32		OffLinePercent		=	6;
};

message PBRepeFameTable
{
	optional uint32		TempID				=	1;
	repeated uint32		FameType			=	2;
	repeated uint32		FameValue			=	3;
	repeated uint32		ItemValue			=	4;
	optional uint32		CombatItemID		=	5;
	optional uint32		RepeItemID			=	6;
	optional uint32		CommonItemID		=	7;
	optional uint32		CombatLimit			=	8;
	optional uint32		RepeLimit			=	9;
	optional uint32		CommonLimit			=	10;
};

message PBCastingSpiritTable
{
	optional uint32		TempID				=	1;
	repeated uint32		OpenStatus			=	2;
	repeated string		PositionName		=	3;
	repeated string		PositionDesc		=	4;
	repeated string		SpiritTypeName		=	5;
	repeated string		SpiritTypeDesc		=	6;
	repeated uint32		SpiritItem			=	7;
	repeated uint32		SpiritItemNum		=	8;
	repeated uint32		UpgradeMoney		=	9;
	repeated uint32		DowngradeMoney		=	10;
	optional uint32		UpgradeItem			=	11;
	optional uint32		DowngradeItem		=	12;
};

message PBSpiritItem
{
	optional PBItemBase	BaseInfo			= 1;	// 道具基本信息
	optional uint32		Grade				= 2;
	optional uint32		ColorGrade			= 3;
	optional uint32		SpiritType			= 4;
	optional uint32		LevelLimit			= 5;
	optional uint32		EffectPoint			= 6;
};