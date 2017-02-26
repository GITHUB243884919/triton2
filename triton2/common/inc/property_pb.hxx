/******************************************************************************
	版权所有 (C ), 2008-2018, 蓝港在线（北京）科技有限公司

******************************************************************************
	文件名		: property_pb.hxx
	版本号		: 1.0
	作者		: ZHL
	生成日期	: 2008-09-10
	最近修改	: 
	功能描述	: 1 - 人物属性对应的PB格式定义，原型来自proclient_svr.h和其他模块，如taskmodule.h。
				: 2 - 作用是将人物属性用PB格式存到数据库中的Blob字段里。
				: 3 - 本文件以后将是版本控制的核心文件，也是数据库中Blob字段强制升级工具的基础。
				: 4 - 每个字段一定要注释详细
				: 5 - 字段只能增加，不能删除，删除的字段要加上版本号
				: [注] PB 全称[Protocal Buffers -- google开发协议]
	函数列表	: 
	修改历史	: 
		1 日期	: 2008-09-10
		作者	: ZHL
		修改内容: 生成
*******************************************************************************/


/****************************	字段限制   ************************************
*
*	required: 必须赋值的字段，一定要想好了才能用这种类型
*	optional: 可有可无的字段
*	repeated: 可重复字段(变长字段)
*
******************************************************************************/


/***************************   数据类型 ***************************************
*	double
*	float
*	int32		Uses variable-length encoding. Inefficient for encoding negative numbers 
*				– if your field is likely to have negative values, use sint32 instead. 
*	int64		Uses variable-length encoding. Inefficient for encoding negative numbers 
*				– if your field is likely to have negative values, use sint64 instead. 
*	uint32		Uses variable-length encoding. 
*	uint64		Uses variable-length encoding. 
*	sint32		Uses variable-length encoding. 
*				Signed int value. These more efficiently encode negative numbers than regular int32s. 
*	sint64		Uses variable-length encoding. Signed int value.
*				These more efficiently encode negative numbers than regular int64s. 
*	fixed32		Always four bytes. More efficient than uint32 if values are often greater than 228. 
*	fixed64		Always eight bytes. More efficient than uint64 if values are often greater than 256. 
*	sfixed32	Always four bytes.  
*	sfixed64	Always eight bytes.
*	bool
*	string		A string must always contain UTF-8 encoded or 7-bit ASCII text.
*	bytes		May contain any arbitrary sequence of bytes.
*
*	注： PB没有定义1Byte,2Byte的整型，都是4Byte起; 但是会根据整型范围动态调整字节数。
*		
******************************************************************************/

// 以最快速度序列/反序列
option optimize_for = SPEED;

message PBPropertyValue1
{
	optional uint32		InitValue	= 1;
	optional uint32		DeltaValue	= 2;
};

// 法宝技能框
message PBSkillBox
{
	optional uint32			Skills			= 1;			// 已激活技能 0表示是空技能框
	optional uint32			SkillActNum		= 2;			// 已鉴定次数
	optional uint32			SkillBookID		= 3;			// 已使用的技能书
	optional uint32			AddedSkillPoints= 4;			// 对应技能已加的技能点
	optional uint32			SkillInitLevel  = 5;			// 对应技能初始等级
	optional uint32         ActiveState		= 6;			// 激活状态
};

/* 物品通用类型
**
** 所有物品序列化后都存成通用的PB格式，然后再根据每种物品的类型，提取PB中感兴趣的数值
** 
**/
message PBItemObject
{
	optional uint32			ItemID				= 1;			// 道具ID
	optional uint32			Number				= 2;			// 道具数量
	optional uint32			Index				= 3;			// 道具在包裹中的索引
	optional uint32			InvalidTime			= 4;			// 到期时间
	optional uint32			Cooldown			= 5;			// 冷却时间
	optional uint32         BindStatus			= 6;			// 物品绑定状态
	optional uint64			ObjGuID				= 7;			// 生成时的唯一标识
	optional uint32		    UsedTimes			= 8;			// 已使用次数
	optional PBItemContent	ItemContent			= 9;			// 物品内容
	optional uint64         PurchaseID			= 10;           // 购买物品时候的交易ID
	optional string			GroomName			= 11;			// 新郎名称
	optional string			BrideName			= 12;			// 新娘名称
	optional uint32			WeddingTime			= 13;			// 婚礼时间
	optional uint32			WeddingTimeLength	= 14;			// 婚礼	  	
};

/* 物品内容
** 一般情况下一种物品会有下面属性中的一种，特殊物品可能会同时有多种属性
**/
message PBItemContent
{
	optional uint32			RemainHP		= 1;			// 剩余HP
	optional uint32			RemainMP		= 2;			// 剩余MP
	optional uint32			RemainAP		= 3;			// 剩余AP
	optional uint32			Current			= 4;			// 当前存储的元气
	repeated PBTelePostion  TelePos			= 5;			// 传送信息(普通传送物品只会有一个点；特殊类物品可能有多个传送点)
	optional PBItemObjMW	ItemMW			= 6;			// 如果是法宝 则有这个字段
	optional PBItemObjEquip ItemEquip		= 7;			// 如果是装备 则有这个字段
	optional uint32			LeftOffLineTime	= 8;			// 黑狗丸可用的剩余离线时间		
	optional uint32			Exp				= 9;			// 修炼经验丹的经验
};

// 装备
message PBItemObjEquip
{
	repeated uint32			SlotProperty	= 1;			// 装备属性槽是否打开，0未开，1打开
	repeated uint32			Property		= 2;			// 附加属性ID,0表示没有附加属性
	repeated uint32			SlotValue		= 3;			// 装备属性槽的宝石ID
	optional uint32			Level			= 4;			// 装备等级
	optional uint32			SlotNumber		= 5;			// 打开的装备属性槽个数	
	optional uint32			NameLength		= 6;			// 生产者名字长度
	optional string			Name			= 7;			// 生产者名字
	optional uint32			SlotSpell		= 8;			// 镶嵌的符文
	optional uint32         Durability      = 9;           // 装备耐久度	
	optional PBPropertyValue1 MaxSlotNumber	= 10;			// 生成的最大槽数
	repeated uint32			Judge			= 11;			// 已鉴定的本体属性数值
	optional uint32			ResDef			= 12;			// 抗性类别
	optional uint32			ResAtt			= 13;			// 属性攻击类别
	optional uint32			DefLevel		= 14;			// 抗性等级
	optional uint32			AttLevel		= 15;			// 属性攻击等级
	optional uint32			MagicStoneID	= 16;			// 装备附魔石ID
	optional uint32			MagicStoneNum	= 17;			// 附魔次数
	optional uint32         BindFreeTime    = 18;			// 物品手动绑定解除时间
	optional uint32			SpouseID		= 19;			// 配偶ID
	optional string			SpouseName		= 20;			// 配偶名称
	optional uint32			ExtraType		= 21;			// 附加类型
	optional uint32			BindTime		= 22;			// 装备普通绑定时间
};

// 法宝属性
message PBItemObjMW
{
	optional uint32         MWLevel					= 1;			// 法宝等级
	optional uint64         Exp						= 2;			// 法宝经验
	optional uint32         Mp						= 3;			// 法力值
	optional uint32         Hp						= 4;			// 灵气值
	optional uint32         CurKeenpoints			= 5;			// 当前锐力
	optional uint32         CurCalmpoints			= 6;			// 当前定力
	optional uint32         CurMagicpoints			= 7;			// 当前法力
	optional uint32         CurBierpoints			= 8;			// 当前灵力
	optional uint32         CurImmortalpoints		= 9;			// 当前仙力
	optional uint32         LeftPoints				= 10;			// 当前剩余潜能点
	optional uint32         ProBase					= 11;			// 属性基础值
	optional uint32         ProUp					= 12;			// 属性成长值
	optional uint32         ProEff					= 13;			// 属性影响度
	optional uint32         ProSpir					= 14;			// 灵性
	optional uint32         ProcessVal				= 15;			// 当前进度值
	optional uint32         LeftSkillPoints			= 16;			// 当前剩余技能点
	optional uint32         MaxMp					= 17;			// 法力值上限
	optional uint32         MaxHp					= 18;			// 灵气值上限
	optional uint32         IncMp					= 19;			// 法力值每次恢复点数
	optional uint32         IncHp					= 20;			// 灵气值每次恢复点数
	repeated PBSkillBox		SkillBox				= 21;			// 法宝技能框情况(只有已激活的技能框)
	optional uint32         AddKeenpoints			= 22;
	optional uint32         AddCalmpoints			= 23;
	optional uint32         AddMagicpoints			= 24;
	optional uint32         AddBierpoints			= 25;
	optional uint32         AddImmortalpoints		= 26;
	optional PBSkillList    SkillList				= 27;
	optional uint32			JudgeFlag				= 28;		// 属性鉴定标志 0=未鉴定
	optional uint32			ExtraKeenpoints			= 29;	
	optional uint32			ExtraCalmpoints			= 30;	
	optional uint32			ExtraMagicpoints		= 31;	
	optional uint32			ExtraBierpoints			= 32;	
	optional uint32			ExtraImmortalpoints		= 33;
	repeated uint32			PlayerAddPointLastLevel	= 34;	// 玩家加点备份
	optional uint32			LeftPointsLastLevel		= 35;	// 本次升级前玩家的剩余潜能点
	optional uint32			NewAddPoints			= 36;	// 本次升级新加的潜能点
	optional uint32			NewAddSkillPoints		= 37;	// 本次升级新加的技能点
	optional uint32			StepClearState			= 38;	// 玩家小洗状态
	optional uint32			LastAddedSkillBoxIndex	= 39;	// 上次技能加点的技能框索引
	optional uint32			LastLevelProcessVal		= 40;	// 进度值备份
	optional uint32			NextActBoxIndex			= 41;	// 下一个将被激活的技能框
	optional uint32			LastLevelNextActBox		= 42;	// 下一个将被激活的技能框 升级备份
	optional uint32			QLTempID				= 43;	// 印记上去的器灵ID
};

// CItemBox
message PBItemBox
{
	optional uint32			ReleaseIndex	= 1;			// 当前释放到哪个格子
	repeated PBItemObject	ItemObjects		= 2;			// 255个物品
	repeated PBItemObject	KitBag			= 3;			// 扩展包
	repeated uint32			KitLocked		= 4;			// 栏位是否锁定
};

message PBTransform
{
	optional uint32			HasTransform	= 1; // 是否存在变身状态
	optional uint32			Sex				= 2;
	optional uint32			StateID			= 3;
	optional uint32			WeaponID		= 4;
	optional uint32			ClothID			= 5;
	optional uint32			FashionID		= 6;
	optional uint32			NpcID			= 7;
};

// CBuffObject
message PBBuffObject
{
	optional uint32			BuffID			= 1;			// Buff模版ID
	optional uint64			PersistTime		= 2;			// Buff持续时间（毫秒为单位）
	optional uint32			SrcEntityID		= 3;			// 释放者ID
	optional uint32			TimerType		= 4;			// 时间类型
	optional uint32			LastInterval	= 5;			// 上次时间间隔
	repeated uint32			Parames			= 6;			// 扩展数据
	repeated uint32			Parames2		= 7;			// 扩展数据2
	optional uint32			BackDamage		= 8;			// 返回伤害的总量
	optional uint32			ListID			= 9;
	optional uint32			SuperCount		= 10;			// Buff叠加次数( 只对时间类型Buff有效果 )
};

// CBuffList
message PBBuffList
{
	optional uint32			BuffNumber		= 1;			// Buff个数
	repeated PBBuffObject	BuffObjects		= 2;			// BUFF列表
	optional PBTransform	Transform		= 3;			// 变身状态
};

message PBStateParam
{
	optional uint32			StateType		= 1;			// 状态类型ID(作为数组下标使用)
	optional uint32			From			= 2;
	optional uint32			Index			= 3;
	optional uint32			Parames1		= 4;
	optional uint32			Parames2		= 5;
	optional uint32			InState			= 6;			// 是否处于该状态
	optional fixed32		BuffAdd			= 7;
};

message PBStateList
{
	repeated PBStateParam	StateParamObj	= 1;
};

// CSkillObject
message PBSkillObject
{
	optional uint32			SkillID			= 1;			// 技能模版ID
	optional uint32			SkillLevel		= 2;			// 技能等级
	optional uint32			Cooldown		= 3;			// 技能冷却时间, 0表示目前技能可以使用
	optional uint32			IncLevel		= 4;			// 增加的技能等级
};

// CSkillList
message PBSkillList
{
	optional uint32			SkillNumber		= 1;			// 技能个数
	repeated PBSkillObject	SkillObjects	= 2;			// 技能列表
};

// CShortcut
message PBShortcut
{
	optional uint32			Shortcut		= 1;
	optional uint32			mType			= 2;
	optional uint32			mIndex			= 3;
	optional uint32			mLabelType		= 4;
	optional uint32			mLabelIndex		= 5;
	optional uint32			Param1			= 6; 
};

// CShortcutList
message PBShortcutList
{
	optional uint32			ShortcutNumber	= 1;
	repeated PBShortcut		Shortcuts		= 2;			// 快捷键列表
	repeated uint32			HelpStatus		= 3;			// 新手指引的动作状态,每一位表示一种状态
};

message PBKillData
{
	optional uint32			KillID			= 1;			// 杀死者的ID
	optional uint32			KillTime		= 2;			// 杀死的时间
};

message PBWizard
{
	repeated uint32			WizardPoint		= 1;			// 才华等级
	repeated uint32			WizardValue		= 2;			// 才华值
	repeated uint32			WizardSubmit	= 3;			// 才华提交量
	repeated uint32			WizardIncrease	= 4;			// 才华加点
	optional uint32			Trend			= 5;			// 成长倾向
	optional uint32			CombatFame		= 6;			
	optional uint32			RepeFame		= 7;
	optional uint32			CommonFame		= 8;
	optional uint32			CombatToday		= 9;
	optional uint32			RepeToday		= 10;
	optional uint32			CommonToday		= 11;
	optional uint32			CombatAll		= 12;
	optional uint32			RepeAll			= 13;
	optional uint32			CommonAll		= 14;
};

// 可传送点信息
message PBTelePostion
{
	optional uint32			Line		= 1;			// 线ID
	optional uint32			Map			= 2;			// 地图ID
	optional uint32			PosX		= 3;			// x坐标
	optional uint32			PosY		= 4;			// y坐标
};

// 人物基础属性
message PBPlayerBasicInfo
{
	optional uint32			Str				= 1;
	optional uint32			Sta				= 2;
	optional uint32			Spr				= 3;
	optional uint32			Con				= 4;
	optional uint32			Wis				= 5;
	optional uint32			RemainPoint		= 6;
	optional uint32			SkillPoint		= 7; // 技能点
	optional uint32			ProLevel		= 8; // 生产等级
	optional uint32			ProExp			= 9; // 生产经验
	optional uint32         	ShowFashion     = 10; // 显示时装
    	optional uint32         	ShowEquip       = 11; // 显示装备
};


// 妖魔志
message PBMonsterRecordInfo
{
	repeated uint32			ActiveCard		= 1;			// 已激活卡片	
	repeated uint32			OwnCard			= 2;			// 已收集卡片
};

// 法宝属性信息
message PBPlayerMWInfo
{
	optional uint32			AssignExpScl	= 1;			// 玩家分配给法宝的经验
};

// 单个冷却组信息
message PBSingleCDGroup
{
	optional uint32			Index			= 1;			// 冷却组索引
	optional uint32			CDTime			= 2;			// CD时间
};

// 冷却组
message PBPlayerCDGroupInfo
{
	repeated PBSingleCDGroup		CDGroup			= 1;			// 冷却组
};

// 记录坐标信息
message PBTelePosInfo
{
	optional PBTelePostion	TownPos			= 1;			// 回城点
	repeated PBTelePostion	TeleList		= 2;			// 飞行道具记忆点列表
};

// 玩家PVP信息
message PBPlayerPVPInfo
{
	optional uint32			PKMode			= 1;			// PK模式
	optional uint32			PKValue			= 2;			// PK值
	optional uint32			KillNum			= 3;			// 杀敌数
	optional uint32			Honor			= 4;			// 功勋
	optional uint32			InHonPerDay		= 5;			// 每天获得的功勋值
	optional uint32			OutHonPerDay	= 6;			// 每天提供的功勋数
	repeated PBKillData		Kill			= 7;			// 杀死的记录
	optional uint32			KillNumIn		= 8;			// 在国内的杀敌数
	optional uint32			YellowStatus	= 9;			// 黄名
	optional uint32			PrisonTime		= 10;			// 监狱时间
	optional uint32			TotalPrisonTime = 11;			// 囚禁总共时间
	optional uint32			TodayHonor		= 12;			// 今日功勋
	optional uint32			TotalWar		= 13;			// 国战累计战功
	optional uint32			TodayWar		= 14;			// 本次国战战功
	optional uint32			TodayOther		= 15;			// 本日其它战功
	optional uint32			FreeWar			= 16;			// 累计可支配战功
	optional uint32			WarSerial		= 17;			// 当前国战计次	
	optional uint32			WarKill			= 18;			// 当前国战击杀	
	optional uint32			ExpHonor		= 19;			// 今日已兑换经验荣誉
	optional uint32			ItemHonor		= 20;			// 今日已兑换物品荣誉
};

// 玩家统计信息
message PBPlayerStatisticInfo
{
	optional uint32			CompletedTasks				= 1;	// 完成的任务数
	optional uint32         OriginReliveTimes			= 2;	// 原地复活次数
	optional uint32         KillSameNationNum			= 3;	// 杀本阵营人数
	optional uint32         KillOgreNum					= 4;	// 杀怪数
	optional uint32         UpFailNum					= 5;	// 精炼失败次数
	optional uint32         TodayKillEnemyIn			= 6;	// 在国内的杀敌数
	optional uint32         TodayKillEnemyOut			= 7;	// 在国外的杀敌数	
	optional uint32			DieNum						= 8;	// 死亡次数
};

// 造化
message PBPlayerTalentInfo
{
	optional uint32			TalentPoint		= 1;			// 造化点
	optional uint32			TalentStep		= 2;			// 当前造化小阶
	optional uint32			TalentStudy		= 3;			// 当前小阶已投入的造化点
	optional PBSkillList	TalentSkill		= 4;			// 造化技能
	optional uint32			TalentExp		= 5;			// 修为值
	optional uint32			TalentLevel		= 6;			// 造化点等级
};

// 称号系统
message PBPlayerTitleInfo
{
	optional uint32			CurrentTitle	= 1;			// 当前称号
	repeated uint32			TitleList		= 2;			// 称号列表
	optional PBSkillList	TitleSkill		= 3;			// 称号技能
	repeated uint32			Time			= 4;			// 获得时间。titlelist 和 time 应该放到一个结构体， 这样做是为了兼容老数据
};

// 活动状态
message PBActivityStateInf
{
	optional uint32			ActivityID					= 1;			// 活动ID
	optional uint32			CurState					= 2;			// 当前状态
	repeated uint32			StateParam					= 3;			// 状态参数
	optional uint32			ChgTm						= 4;			// 时间戳
	optional uint32			RealLastedTime				= 5;			// 真实的本状态持续时间 
};

// 活动信息
message PBPlayerActivityInfo
{
	optional uint32			DiploidTime					= 1;			// 领双时间
	optional uint32			LastRefreshTimeOfDipLoid	= 2;			// 有效双倍时间
	repeated uint32			DiploidState				= 3;			// 双倍经验状态
	optional uint32			DailyWelfareSta				= 4;			// 每日福利的状态
	optional uint32			LastDaylyDisposTime			= 5;			// 上次处理每日事务的时间
	optional uint32			LastWeeklyDisposTime		= 6;			// 上次处理每周事务的时间
	optional uint32			LastFamilyApplyTime			= 7;			// 最后一次申请加入家族的时间
	optional uint32			LastTimeLeaveFamily			= 8;			// 最后一次离开家族的时间
	optional uint32			LastJudgeTime				= 9;			// 上次评价时间
	optional uint32			SaveDate					= 10;			// 某些属性的刷新时间，如：副本，pvp相关信息
	optional uint32			VipFlag						= 11;			// vip 标志
	optional uint32			VipTime						= 12;			// vip 终止日期
	optional PBAwardRecord	AwardRecord					= 13;			// 领取记录
	optional uint32			InvestStatus				= 14;			// 调查问卷的状态
	optional PBAwardRecord	WeeklyAwardRecord			= 15;			// 当周领取记录	
	repeated PBActivityStateInf  ActivityState			= 16;			// 活动状态
	optional uint32			HasVoted					= 17;			// 弹劾投票记录
	optional uint32			SaveTm						= 18;			// 存盘时间戳	
};

// 其他信息（给客户端用)
message PBPlayerClientInfo
{
	optional PBShortcutList			Shortcut			= 1;			// [DB]玩家快捷键列表	
};

// 一个系别的铸灵信息
message PBSpiritTypeInfo
{
	repeated uint32 SlotOpenStatus		= 1;	// 是否已开孔
	repeated uint32 CastingItemID		= 2;	// 铸灵时使用的道具，O表示未铸灵
	repeated uint32 BindStatus			= 3;	// 真灵道具的绑定状态，1表示绑定，0表示非绑 
}

// 一个部位的铸灵信息
message PBSpiritPositionInfo
{
	repeated PBSpiritTypeInfo SpiritTypeInfo	= 1;	// 各系别信息
}

// 铸灵信息
message PBSpiritInfo
{
	repeated PBSpiritPositionInfo PositionInfo	= 1;	// 各部位信息 
}
/* CProperty
** 玩家核心属性信息,游戏最重要的信息都在此
** 按照功能组分类，方便日后扩展
** 如果有对此进行改动的需求，务必考虑清楚后再添加，变量只能加不能删
*/
message	PBProperty
{
	optional uint32					CurHP				= 1;			// [DB]当前HP
	optional uint32					CurMP				= 2;			// [DB]当前MP
	optional uint32					CurAP				= 3;			// [DB]当前AP
	optional PBPlayerBasicInfo		BasicInfo			= 4;			// [DB]基础信息
	optional PBItemBox				Baggage				= 5;			// [DB]玩家包裹
	optional PBItemBox				TaskBaggage			= 6;			// [DB]玩家任务包裹
	optional PBItemBox				Equipment			= 7;			// [DB]玩家装备
	optional PBItemBox				Storage				= 8;			// [DB]玩家仓库
	optional PBSkillList			Skill				= 9;			// [DB]玩家技能列表
	optional PBBuffList				Buff				= 10;			// [DB]玩家buff列表
	optional PBPlayerCDGroupInfo	CDGroup				= 11;			// [DB]CD组
	optional PBMonsterRecordInfo	MonsterRecord		= 12;			// [DB]妖魔志信息
	optional PBTelePosInfo			TelePos				= 13;			// [DB]可传送点信息
	optional PBPlayerPVPInfo		PvPInfo				= 14;			// [DB]PVP信息
	optional PBPlayerTitleInfo		TitleInfo			= 15;			// [DB]称号信息
	optional PBPlayerTalentInfo		TalentInfo			= 16;			// [DB]造化信息
	optional PBPlayerMWInfo			MWInfo				= 17;			// [DB]法宝信息
	optional PBLifeSkillBox			LifeSkillBox		= 18;			// [DB]生活技能
	optional PBPlayerStatisticInfo	StatisticInfo		= 19;			// [DB]统计信息
	optional PBPlayerClientInfo		ClientInfo			= 20;			// [DB]客户度独立信息
	optional PBWizard				Wizard				= 21;			// [DB]才华信息
	optional PBPlayerActivityInfo	Activity			= 22;			// [DB]活动信息
	optional PBAwardRecord			Award				= 23;			// [DB]领取信息
	optional PBMatrixInfo			Matrix				= 24;			// [DB]传销信息
	optional PBExpTimeInfo			ExpTimeInfo			= 25;			// [DB]离线经验丹信息
	optional PBTreasureBowl			TreasureBowl		= 26;			// [DB]聚宝盆信息
	optional PBSpiritInfo			SpiritInfo			= 27;			// [DB]铸灵信息
};


// Task
message PBTask
{
	required  uint32			TaskID			= 1;
	optional  uint32			TaskProgress	= 2;
	optional  int32				GameTimeLeft	= 3;			// 剩余时间  -1表示不是限时任务
	repeated  uint32			TaskData		= 4;
	repeated  uint32			TalkedNpcs		= 5;			// 已经完成的对话NPC
	optional  uint32			ExFlags			= 6;			// 特殊标记 为了提速用
	optional  uint32			BeginTime		= 7;			// 用于记录任务开始的时间
};

// 环任务数据
message PBLoopTaskData
{
	optional  uint32       TodayCompletedCircles	= 1; //  今天已经完成的所有环任务的次数
	optional  uint64       LastCompletedTime		= 2; //  上次完成环任务的时间
	repeated  uint32	   LoopTasks				= 3; //  身上的环任务
	optional  uint32	   CompletedTimes			= 4; //  已完成次数(整轮)
	optional  uint64	   LastAbortTime			= 5; //  本轮上次放弃时间
	optional  int32        CurSubIndex				= 6; //  当前进行中的子任务索引，只有接任务才会更新该值，交任务后该值不变 初始-1
	optional  uint32       WeekLoops				= 7; //  本周已完成的完美环任务次数
	optional  uint32       DayCompletedLoopFlag		= 8; //  今日是否完成一整轮环任务的标志
};

// 库任务数据
message PBStorageTaskData
{
	repeated  uint32	   Tasks					= 1; //  身上的当前的库任务列表
	optional  uint32	   TaskNum					= 2; //  库任务的数量
	optional  uint64	   LastClickNpc				= 3; //  玩家上次点击NPC的时间（如果是次日，则刷新任务列表）
	optional  uint32       CurTaskID				= 4; //  当前库任务ID
	repeated  uint32	   GotTasks					= 5; //  已经接了的(当前刷新周期内)
	optional  uint32	   GotNum					= 6; //  已经接在身上的库任务数量
	optional  uint32	   TodayCompleted			= 7; //  今天已完成次数
	optional  uint32	   TodayRMBRefresh			= 8; //  今天RMB道具刷库任务次数
};

// 劫难任务数据
message TragedyStruct
{
	optional uint32		TragedyIndex    = 1; // 劫难任务索引 1开始
	optional fixed32	NextTragedyTask = 2; // 对应的 下一个劫难子任务ID 可能为0
	optional uint32		TragedyState	= 3; // 对应的劫难任务状态
};

message PBTaskGroupData
{
	optional  uint32				GroupID				= 1;	// 组ID
	optional  uint32				TodayCompleted		= 2;	// 今日完成次数
	optional  uint32				TodayAccepted		= 3;	// 今日接取次数
	optional  uint32				WeekCompleted		= 4;	// 本周完成次数
	optional  uint32				WeekAccepted		= 5;	// 本周接取次数
};

message SingleChairManTask
{
	optional uint32 TaskID			=	1;
	optional uint32 TaskState		=	2; // 任务状态，走通用状态
};

message SingleCompletedInf
{
	optional uint32 ChairmanID			=	1;
	optional uint32 CompletedNum		=	2; // 完成数量
};

// 掌门任务数据
message PBChairManTaskData
{
	repeated  SingleChairManTask    Tasks					= 1; //  身上的当前的库任务列表
	optional  uint32				ChairManID				= 2; //  掌门任务ID
	optional  uint32				CompletedNum			= 3; //  已完成数量 <作废>
	optional  uint32				AcceptedNum				= 4; //  已接受数量
	repeated  SingleCompletedInf    Completed				= 5; //  已完成数量信息
};

// 任务颜色（品质）数据
message PBRefreshTaskData
{
	optional  uint32				TaskID				    = 1; //  刷新任务ID
	optional  uint32				TaskColor				= 2; //  任务颜色 （品质）
	optional  uint32				ResultFlag				= 3; //  标记位
};

message GroupPublishData
{
	optional  uint32				TaskID				    = 1; //  发布任务ID
	optional  uint32				TaskState				= 2; //  发布任务状态 
};

message PBTaskList
{
	optional  uint32				TaskNumber		= 1;
	repeated  PBTask				TaskList		= 2;
	optional  PBLoopTaskData		LoopTaskData	= 3;
	repeated  uint32				EventID			= 4;
	repeated  uint32				EventParam		= 5;
	repeated  uint32				EventTick		= 6;
	optional  PBStorageTaskData		StorageTaskData	= 7;
	repeated  TragedyStruct			TragedyTaskData = 8;
	repeated  uint32				CompletedTask	= 9;
	repeated  uint32				FailedTask		= 10;
	repeated  PBTaskGroupData		TaskGroups		= 11;
	optional  uint32				CompletedTragedy= 12;	// 已完成劫难的数量
	optional  fixed32				LastLogOff		= 13; // 上次跨服/下线时间
	optional  uint32				RedirectFailTask= 14;
	optional  PBChairManTaskData	ChairManTask	= 15;	// 掌门任务
	optional  uint32				TodayTeamLoopNum= 16;
	repeated  PBRefreshTaskData		RefreshData		= 17;
	repeated  uint32				RepeateTask		= 18;
	repeated  uint32				EndTime			= 19;
	repeated  GroupPublishData		CorpsPublish	= 20;
	
};

// 地图信息
message PBMapData
{
	optional uint32			Direction	= 1; // 朝向
	optional uint32			PosX		= 2; // x坐标
	optional uint32			PosY		= 3; // y坐标
	optional uint32			AliveStatus	= 4; // 生存状态
};

// friend
message PBFriendGlobalData
{
	repeated  string  GroupNames	= 1; // 分组名字
	optional  uint32  IsRefuseAll	= 2; // 是否自动拒绝添加好友请求
};

message PBFriendData
{
	repeated  uint32  PlayerId		= 1; // 好友charid
	repeated  uint32  GroupID		= 2; // 组ID
	repeated  uint32  Rel2Me		= 3; // 和我的关系
	optional  uint32  BlessTimes	= 4; // 好友祝福升级的次数
	repeated  uint32  IntimacyDegree= 5; // 好友亲密度	
	repeated  uint32  LastTimeAllOnline= 6; // 好友亲密度	
	repeated  uint32  DailyIntimacy = 7;	// 当天组队获取的亲密度
	repeated  uint32  LastUpdateTime = 8;	// 最后一次更新的时间
};

message PBEnemyData
{
	repeated  uint32  PlayerId		= 1; // 仇敌charid
	repeated  uint32  Killed		= 2; // 杀死他的次数
	repeated  uint32  BeKilled		= 3; // 被他杀的次数
	repeated  uint32  IsLocked		= 4; // 是否锁定
	repeated  uint32  KillTime		= 5; // 时间戳
};

message PBFriend
{
	optional PBFriendGlobalData	 Global			= 1;
	optional PBFriendData		 Friend			= 2;
	optional PBEnemyData		 Enemy			= 3;
	repeated uint32				 BlackList		= 4;  // 黑名单
	repeated uint32				 LatestContact	= 5;  // 最近联系人
};

message PBRepetionData
{
	optional uint32 Index		= 1;
	optional uint32 Data		= 2;
};

message PBRepetionMap
{
	optional uint32	MapID				= 1;
	optional uint32 Count				= 2;
	repeated PBRepetionData Data		= 3;
	optional uint32 WeekCount			= 4;
};

message PBRepetionRepe
{
	repeated PBRepetionData RepetionData= 1;
	optional uint32 RepeIndex			= 2;
	optional uint32 Count				= 3;
	optional uint32 WeekCount			= 4;
	optional uint32 SignUp				= 5;
	optional uint32 IntoTime			= 6;
	optional uint32 DieCount			= 7;
};

message PBRepetion
{
	optional uint32			BackLineID  = 1;
	optional uint32 		BackMapID   = 2;
	optional uint32 		BackPosX 	= 3;
	optional uint32 		BackPosY	= 4;
	repeated PBRepetionMap 	MapInfo 	= 5;
	optional uint64			Key			= 6;
	optional uint32			CampID		= 7;
	repeated PBRepetionRepe RepeInfo	= 8;
	optional uint32			InRepetion	= 9;
	optional uint32			RepetionIndex = 10;
	optional uint32			CheckResult	= 11;	// 副本答题检查结果
	optional uint32			AnswerCount	= 12;	// 回答问题的次数
	optional uint32			GlobalSceneStatus=13;// 全局服务器的状态
	optional uint32			GlobalWarOffLineTime=14;// 离开全局战场的时间
	optional uint64			RepetionKey	= 15;	// 即将进入的副本Key	
};

// ********************************************************************** //
// 定义队员信息 
// ********************************************************************** //
message PBMember
{
	optional uint32			EntityID	= 1;
	optional uint32			CharID		= 2;
	optional string			Name		= 3;
	optional uint32			AccountID	= 4;
	optional uint32			Type		= 5;
	optional uint32			Face		= 6;
	optional uint32			Metier		= 7;
	optional uint32			Level		= 8;
	optional uint32			Sex			= 9;
	optional uint32			LineID		= 10;
	optional uint32			MapID		= 11;
	optional uint32			HP			= 12;	
	optional uint32			MP			= 13;
	optional uint32			AP			= 14;
	optional uint32			XPos		= 15;
	optional uint32			YPos		= 16;
	optional uint32			CommState	= 17;	// 玩家状态：在线、离线、跨服...
	optional PBBuffList		BuffList	= 18;	// buff列表	   	
	optional uint32			RepetionMapIndex	= 19;	// 副本中地图index
	optional uint32        Order		= 20;   // 在队伍中的顺序
};
//********************************************************
// PBTeamMsg
// 求组招募消息
//********************************************************

message PBTeamMsg
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
};

/*
 *	PBStallInfo
 */
message PBStallInfo
{
	optional string		StallName	= 1;
	optional string		AdContent	= 2;
	optional uint32		HasFlag		= 3;	// 摊位是否存在：true--存在，false--不存在
	optional int32		StallType	= 4;  //摆摊类型
};


//********************************************************
// PBLifeSkill
// 生活技能信息
//********************************************************
message PBLifeSkill
{
	optional uint32	LifeSkillType		= 1;		// 生活技能类型
	optional uint32 LifeSkillLevel		= 2;		// 生活技能等级
	optional uint32 SpecializedDegree	= 3;		// 生活技能熟练度
	optional uint32	LifeSkillStatus		= 4;		// 生活技能状态
	optional uint32	LifeSkillKind		= 5;		// 技能种类，主技能还是辅修技能	
	repeated uint32	ComposeID			= 6;		// 已学配方ID
};

message PBLifeSkillBox
{
	repeated PBLifeSkill LifeSkill		= 1;	
};

//********************************************************
// PBExpTimeInfo
// 离线时间相关
//********************************************************
message PBExpTimeInfo
{
	optional uint32 OffLineTimeLength		=	1;			// 实际离线时间	
	optional uint32	EffectiveOffTimeLength	=	2;			// 有效离线时间	
	optional uint32	LastEffectiveTimeLength	=	3;			// 最后计算有效离线时间时当天内的累加长度		
	optional uint32	LastCalculateTime		=	4;			// 上一次计算有效离线时间的时间
	optional uint32	BlackPillsTime			=	5;			// 黑狗丸的时间
	optional uint32	BlackPillID				=	6;			// 黑狗丸的模板ID
	optional uint32	ExpFromOffLine			=	7;			// 离线时间折合的经验	
};

//********************************************************
// 任务需要存数据库的全局数据
//********************************************************
message PBSinglePublishInfo
{
	optional uint32 PubNum		= 1;	// 唯一标识ID（现在是任务ID）
	optional uint32 RecvAwards	= 2;	// 发布状态 见enTaskPublishState
};

message PBTaskPublishInfo
{
	repeated PBSinglePublishInfo SingleData  = 1;	// 分国家存储的数据
	optional uint32 TodayPublishNum			 = 2;
	optional uint32 OncePublishNum			 = 3;
	optional uint32 TotalFinishNum			 = 4;
};

message CMessagePlayerInfo
{
	optional uint32 role_id					= 1;
	optional uint32 account_id				= 2;
	optional string role_name				= 3;
	optional uint32 gender_id				= 4;
	optional uint32 metier_id				= 5;
	optional uint32 nationality_id			= 6;
	optional uint32 face_type				= 7;
	optional uint32 line_id					= 8;
	optional uint32 map_id					= 9;
	optional uint32 role_level				= 10;
	optional uint64 role_expr				= 11;
	optional uint32 role_money				= 12;
	optional int32 pos_x					= 13;
	optional int32 pos_y					= 14;
	optional uint32 is_alive				= 15;
	optional uint32 login_time				= 16;
	optional uint32 online_duration			= 17;
	optional uint32 save_status				= 18;
	optional uint32 save_bitflag			= 19;
	optional uint32 bounded_money			= 20;
	optional uint32 check_sum				= 21;
	optional uint32 silver_yb 				= 22;
	optional uint32 storage_money			= 23;
	optional uint32 storage_bounded_money	= 24;
	optional uint32 role_status_param		= 25;
	optional int32 country_id				= 26;
	optional uint32 role_familyid			= 27;
	optional uint32 role_corpsid			= 28;
	optional uint32 charged_silver_yb		= 29;
	optional uint32 consumed_silver_yb		= 30;
	optional uint32	role_protecttime		= 31;
	optional string	role_password			= 32;
	optional uint32	role_vip_flag			= 33;
	optional uint32	role_vip_time			= 34;
	optional uint32	role_marriageid			= 35;
	optional uint32 role_changecamp_time    = 36;
	optional uint32	role_status				= 37;
	optional uint32	role_swornid			= 38;
};

message PBAwardRecord
{
	repeated uint32	AwardID					= 1;
	repeated uint32	Times					= 2;
};

message PBMatrixInfo
{
	repeated uint32	MasterID				= 1;	// 师傅ID扩充为3个
	repeated uint32	SlaveID					= 2;	// 弟子个数
	repeated uint32 SlaveValue				= 3;	// 第N档的仙缘值
	optional uint32	SlavePoint				= 4;	// 仙缘点数
	optional uint32	MasterPoint				= 5;	// 师德点数
	optional uint32	SlaveTotal				= 6;	// 仙缘累计
	optional uint32	MasterTotal				= 7;	// 师德累计
	repeated uint32	MasterPercent			= 8;	// 感恩比例
	optional uint32	MasterTimes				= 9;	// 拜师次数
	optional uint32	SlaveNum				= 10;	// 出师数量
	optional uint32	MasterGive				= 11;	// 总计给出师德
	optional uint32 BeginnerNum				= 12;	// 新手弟子数量
	optional uint32	FormalNum				= 13;	// 高级弟子数量
};

// 魅力系统
message PBCharm
{
	optional uint32 CharmValue			= 1;	// 累计魅力值
	optional uint32 WeekIncCharm		= 2;	// 本周累计获得
	optional uint32 LastWeekIncCharm	= 3;	// 上周的
};

// 引导系统
message PBGuideData
{
	optional uint32		LevelBak	=	1;	// 今日等级提升
	optional uint32		ExpBak   =	2;	// 今日经验提升
	optional uint32		HonorBak =	3;	// 今日功勋提升
	optional uint32		AchiveBak =	4;	// 今日成就点数提升
	optional uint32		KillBak =	5;	// 今日荣誉击杀提升
};

message PBSingleTimes
{
	optional uint32		Key   = 1;
	optional uint32		Value = 2;
};
// 计次信息
message PBTimes
{
	repeated PBSingleTimes timesinfo = 1;
};

// 玩家的聚宝盆pb结构
message PBTreasureBowlItem
{
	optional uint32	 ItemID		= 1;	// 物品ID
	optional uint32	 UsedTimes	= 2;	// 使用的次数
};
message PBTreasureBowl
{
	optional uint32					TotalNum			= 1;	// 总次数
	optional uint32					ExchangeTime		= 2;	// 聚宝的时间
	repeated PBTreasureBowlItem		TreasureBowlItem	= 3;	// 已经聚宝的物品新列表
};

message PBRepetDataToClient
{
	optional uint32					RepetIndex			= 1;	//副本索引
	optional uint32					RepetData			= 2;	//副本数据
};

message PBRepetDataList
{
	repeated PBRepetDataToClient	RepetDataToClient			= 1;	// 副本数据
};

// 结拜成员信息
message PBSwornMember
{
	optional uint32	RoleID	= 1;	// 角色ID
	optional uint32	TeamPos = 2;	// 结拜排名
};

// 结拜团体信息
message PBSwornTeam
{
	repeated PBSwornMember SwornMember = 1;
};