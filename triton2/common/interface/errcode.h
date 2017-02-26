#ifndef __ERR_CODE_H__
#define __ERR_CODE_H__

//ErrorCode define rule:
//20XXX ever server base error code
//21XXX in loginserver error
//22XXX in dbserver error
//23XXX in proxyserver error
//24XXX in gateserver error 
//26XXX in sceneserver error

//base err code( 0, -20000 - -20999 ) only use in server
#define		SUCCESS							( 0 )
#define		ERR_UNKNOWN						( -20000 )
#define		ERR_NO_ENOUGH_MEMORY			( -20001 )
#define		ERR_INVALID_PARAMETER			( -20002 )
#define		ERR_FOPNE						( -20003 )
#define		ERR_NOT_FOUND					( -20004 )
#define		ERR_PENGING						( -20005 )
#define		ERR_FULL						( -20006 )
#define		ERR_EXIST						( -20007 )
#define		ERR_BUSY						( -20008 )
#define		ERR_PARSEPB						( -20009 ) // 解析PB blob字段失败


#include "langcode.h"

//////////////////////////////sceneserver///////////////////////////
//property		( -26000 - -29999 )

// 绑定结果 ( -26049 - -26000 )
enum ItemBindResult
{
	ITEM_BINDRESULT_SUCESS  	=	-26049	,					// 绑定成功
	ITEM_BINDRESULT_BINDED	=	-26048	,						// 物品已经绑定
	ITEM_BINDRESULT_NOEXIST	=	-26047	,						// 物品模板不存在
	ITEM_BINDRESULT_WRONGBINDWAY	=	-26046	,				// 绑定类型不匹配 
	ITEM_BINDRESULT_CANNOTBIND	=	-26045	,					// 物品不能手动绑定
	ITEM_BINDRESULT_CONSUMEITEMERROR	=	-26044	,			// 绑定石错误
	ITEM_FREEITEM_SUCESS 	=	-26043	,						// 解除绑定成功
	ITEM_FREEITEM_NONEED	=	-26042	,						// 装备没有手动绑定
	ITEM_THROW_BINDED                      	=	-26041	,		// 扔物品时装备绑定
	ITEM_TRADE_BINDED                      	=	-26040	,		// 交易时物品绑定
	ITEM_SELL_BINDED                       	=	-26039	,		// 卖商店时物品绑定	
	ITEM_STALL_BINDED	=	-26038	,							// 摆摊放置物品时绑定
	ITEM_MAIL_BINDED	=	-26037	,							// 发送邮件时物品绑定
	ITEM_EATEQUIP_BINDED                   	=	-26036	,		// 吃装备时装备已手动绑定
	ITEM_RESUMEBIND_SUCESS	=	-26035	,						// 取消解除手动绑定成功		
	ITEM_BINDRESULT_ERROR	=	-26034	,						// 其他错误	
	ITEM_STALL_BINDEDBYITEM	=	-26033	,						// 手动绑定的物品不能进行摆摊 
	ITEM_BINDRESULT_MONEYNOTENOUGH	=	-26032	,				// 绑定费用不足	 
	ITEM_FREEBINDRESULT_MONEYNOTENOUGH	=	-26031	,			// 取消手动绑定时金钱不够
	ITEM_RESUMEBINDRESULT_MONEYNOTENOUGH	=	-26030	,		// 恢复绑定时金钱不够
	ITEM_SENDMAIL_BINDEDBYITEM	=	-26029	,					// 不能将手动绑定的物品进行邮递
	ITEM_EATEQUIP_BINDEDBYITEM	=	-26028	,					// 将手动绑定的物品进行炼丹炉炼化
	ITEM_UPGE_BINDEDBYITEM	=	-26027	,						// 将手动绑定的物品进行装备升级
	ITEM_ADDBINDSTONE_NOTBINDSTEON	=	-26026	,				// 手动绑定界面，将非绑定石类物品放入石头栏
	ITEM_BINDITEM_HASNOEQUIP	=	-26025	,					// 没有放入装备，点击绑定按钮
	ITEM_BINDITEM_HASNOBINDSTONE	=	-26024	,				// 没有放入绑定石，点击绑定按钮
	ITEM_BINDITEM_HASBINDED	=	-26023	,						// 放入手动绑定属性的装备后，点击绑定按钮
	ITEM_BINDITEM_CANNOTBINDED	=	-26022	,					// 将非装备类物品放入手动解绑界面
	ITEM_FREEITEM_HASNOTBINDED	=	-26021	,					// 放入的装备属性为非手动绑定状态，点击解绑按钮	
	ITEM_COMMONBIND_WRONGBINDITEM	=	-26020	,					// 普通绑定时绑定石类型不对
	ITEM_COMMONBIND_ITEMBINDED		=	-26019	,					// 物品已经进行过普通绑定
	ITEM_COMMONBIND_PIPELIMIT		=	-26018	,					// 可堆叠的物品不能进行普通绑定
	ITEM_COMMONBIND_BINDBYITEM		=	-26017	,					// 手动绑定的物品不能进行普通绑定

};

// 道具基础错误码 ( -26099 - -26050 )
enum EItemECode
{
	ERROR_ITEM_ITEMNOTREGISTER	=	-26099	,			// 道具类型没有注册
	ERROR_ITEM_DATANOTEXIST	=	-26098	,				// 道具数据不存在
	ERROR_SKILL_DATANOTEXIST	=	-26097	,			// 技能数据不存在
	ERROR_SKILLTREE_DATANOTEXIST	=	-26096	,		// 技能树数据不存在
	ERROR_SKILLTREE_INDEXOVERFLOW	=	-26095	,		// 技能树索引越界
	ERROR_METIER_DATANOTEXIST	=	-26094	,			// 职业数据不存在
	ERROR_DEBUG_CMDNOTFOUND	=	-26093	,				// 调试指令没有找到
	ERROR_SPECIAL_TASK_ITEM_ERROR	=	-26092	,		// 特殊任务道具不可重复获得
	ERROR_TASK_ITEM_NONEED	=	-26091	,				// 你不需要这个任务道具
	ERROR_ITEM_MINEINFRONT	=	-26090	,				// 还未到，宝藏在前方
	ERROR_ITEM_NOTEVENT	=	-26089	,					// 没有事件
	ERROR_ITEM_HASCARD	=	-26088	,					// 已经有了这个卡片
	ERROR_INVILID_ITEM	=	-26087	,					// 过期的物品
	ERROR_OVERMAX_TELE_LISTS	=	-26086	,			// 超过最大记录点数量
	ERROR_CANT_ADD_TELEPOINT	=	-26085	,			// 不能添加传送点
	ERROR_ITEM_INVALID	=	-26084	,					// 物品已失效或次数已用完
	ERROR_FLOWER_DES_INVILID	=	-26083	,			// 使用鲜花时对方不在线或玩家不存在
	ERROR_FLOWER_OVER_MAX_TIMES 	=	-26082	,		// 今天已超过鲜花最大使用次数
	ERROR_GIFT_OVER_MAX_TIMES		= -26081,			// 今日使用礼包已超过次数
	ERROR_ITEM_INWEDDINGHALL		=	-26080	,		// 在礼堂不能使用云游符
	ERROR_ITEM_REAUEST_MENPAI		=	-26079	,		// 必须有门派才能开
	ERROR_ITEM_OVER_TIME			= -26078,			// 使用物品已达次数
	ERROR_TREASUREBOWL_CANNOTUSED	= -26077,			// 物品不能被用于聚宝
	ERROR_TREASUREBOWL_TOTALTIMELIMIT= -26076,			// 总共聚宝次数已达到当日上限
	ERROR_TREASUREBOWL_ITEMTIMELIMIT= -26075,			// 物品聚宝次数已达到当日上限
	ERROR_TREASUREBOWL_BAGGAGEFULL = -26074,			// 包裹空间不够，不能聚宝
	ERROR_TREASUREBOWL_FAILED = -26073,					// 聚宝失败
	ERROR_TREASUREBOWL_MONEYNOTENOUGH	= -26072,		// 聚宝时金钱不足
};

// 移动道具时的错误码 ( -26149 - -26100 )
enum EMoveItem
{
	ERROR_MOVEITEM_SUCCESS							=	-26149	,			// 移动道具时，移动成功
	ERROR_MOVEITEM_SAMEINDEX							=	-26148	,		// 移动道具时，源和目标相同
	ERROR_MOVEITEM_ITEMNOTEXIST							=	-26147	,		// 移动道具时，道具不存在
	ERROR_MOVEITEM_DESINVALID							=	-26146	,		// 移动道具时，移动的目标无效
	ERROR_MOVEITEM_SRCINVALID							=	-26145	,		// 移动道具时，移动的源无效
	ERROR_MOVEITEM_SLOTINVALID							=	-26144	,		// 移动道具时，格子类型无效
	ERROR_MOVEITEM_ITEMLOCKED							=	-26143	,		// 移动道具时，道具被锁定
	ERROR_MOVEITEM_NOTSERVICE							=	-26142	,		// 移动道具时，没有处于仓库服务状态
	ERROR_MOVEITEM_KITBAGINVALID							=	-26141	,	// 往背包移动物品，背包过期已经失效
	ERROR_MOVEITEM_DESINDEXINVALID							=	-26140	,	// 目标索引不合法
};

// 添加道具时的错误码 ( -26199 - -26150 )
enum EInsertItem
{
	ERROR_INSERTITEM_SUCCESS		= -26199,		// 添加道具时，添加成功
	ERROR_INSERTITEM_NOSPACE= -26198,						// 添加道具时，空间不够了
	ERROR_INSERTITEM_INVALIDNUM= -26197,					// 添加道具时，个数不正确
};

// 删除道具时的错误码 ( -26249 - -26200 )
enum ERemoveItem
{
	ERROR_REMOVEITEM_SUCCESS		= -26249,		// 删除道具时，删除成功
	ERROR_REMOVEITEM_ITEMNOTEXIST= -26248,					// 删除道具时，道具不存在
	ERROR_REMOVEITEM_ITEMLOCKED= -26247,					// 删除道具时，道具被锁定
};

// 装备道具时的错误码 ( -26299 - -26250 )
enum EEquipItem
{
	ERROR_EQUIPITEM_SUCCESS							=	-26299	,			// 装备道具时，移动成功
	ERROR_EQUIPITEM_ITEMNOTEXIST							=	-26298	,	// 装备道具时，道具不存在
	ERROR_EQUIPITEM_NOTEQUIPABLE							=	-26297	,	// 装备道具时，不是可以装备的道具
	ERROR_EQUIPITEM_NOSPACE							=	-26296	,			// 装备道具时，空间不够了
	ERROR_EQUIPITEM_NEEDMORECON							=	-26295	,		// 装备道具时，需要更多的体质
	ERROR_EQUIPITEM_NEEDMORESTA							=	-26294	,		// 装备道具时，需要更多的耐力
	ERROR_EQUIPITEM_NEEDMORESPR							=	-26293	,		// 装备道具时，需要更多的精神
	ERROR_EQUIPITEM_NEEDMOREWIS							=	-26292	,		// 装备道具时，需要更多的智慧
	ERROR_EQUIPITEM_NEEDMORESTR							=	-26291	,		// 装备道具时，需要更多的力量
	ERROR_EQUIPITEM_NEEDMORELEVEL							=	-26290	,	// 装备道具时，需要更多的等级
	ERROR_EQUIPITEM_NEEDSEX							=	-26289	,			// 装备道具时，需要正确的性别
	ERROR_EQUIPITEM_NEEDMETIER							=	-26288	,		// 装备道具时，需要正确的职业
	ERROR_EQUIPITEM_INVALIDTIME							=	-26287,			// 装备道具时，已经过期
	ERROR_EQUIPITEM_CANOTEXCHANGE						=	-26286,			// 不能用已经过期的包裹中的装备替换身上的装备
};

// 卸下道具时的错误码 ( -26349 - -26300 )
enum EUnEquipItem
{
	ERROR_UNEQUIPITEM_SUCCESS		= -26349,		// 脱下装备时，脱下成功
	ERROR_UNEQUIPITEM_DESINVALID= -26348,					// 脱下装备时，目标无效
	ERROR_UNEQUIPITEM_ITEMNOTEXIST= -26347,					// 脱下装备时，道具不存在
	ERROR_UNEQUIPITEM_EXPIREDBAG  = -26346,					// 脱下装备时，目标包裹已经过期
};

// 使用技能时的错误码 ( -26399 - -26350 )
enum EUseSkill
{
	ERROR_USESKILL_SKILLNOTFOUND							=	-26399	,				// 使用技能时，技能不存在
	ERROR_USESKILL_TEMPATENOTFOUND							=	-26398	,				// 使用技能时，技能模版不存在
	ERROR_USESKILL_TARGETNOTFOUND							=	-26397	,				// 使用技能时，目标找不到
	ERROR_USESKILL_TARGETDIE							=	-26396	,					// 使用技能时，目标已经死亡
	ERROR_USESKILL_WEAPONNEED							=	-26395	,					// 使用技能时，手持武器类型不匹配
	ERROR_USESKILL_NEEDMOREMANA							=	-26394	,					// 使用技能时，需要更多的法力值
	ERROR_USESKILL_NEEDMORELIFE							=	-26393	,					// 使用技能时，需要更多的生命值
	ERROR_USESKILL_NEEDMOREITEM							=	-26392	,					// 使用技能时，需要更多的道具
	ERROR_USESKILL_SOURCEDIE							=	-26391	,					// 使用技能时，施法者已经死亡
	ERROR_USESKILL_OUTOFRANGE							=	-26390	,					// 使用技能时，距离不够
	ERROR_USESKILL_NOTCOOLDOWN							=	-26389	,					// 使用技能时，没有冷却
	ERROR_USESKILL_SILENCE							=	-26388	,						// 使用技能时，沉默状态下不能使用技能
	ERROR_USESKILL_TARGETINVALID							=	-26387	,				// 使用技能时，不能对该目标使用
	ERROR_USESKILL_PKPROTECT							=	-26386	,					// 使用技能时，你不能对那个目标使用
	ERROR_USESKILL_ERRDEST							=	-26385	,						// 无效目标
	ERROR_USESKILL_UNUSE							=	-26384	,						// 不能使用技能
	ERROR_USESKILL_EPTUNAVAILABLE          							=	-26383	,		// 装备已经损坏
	ERROR_USESKILL_ONHORSE							=	-26382	,						// 在坐骑上无法攻击
	ERROR_USESKILL_INSERTBUFF_DISSATISFY							=	-26381	,		// Buff条件不满足
	ERROR_USESKILL_INSERTBUFF_TIMEADD							=	-26380	,			// 时间叠加
	ERROR_USESKILL_INSERTBUFF_UNTEMPLATE							=	-26379	,		// 无摸板
	ERROR_USESKILL_INSERTBUFF_LARGE							=	-26378	,				// 更强效果
	ERROR_USESKILL_INSERTBUFF_FULL							=	-26377	,				// buff满
	ERROR_USESKILL_SRCRESIST								=	-26376,					// 对方无法变身
	ERROR_USESKILL_TARGETLEVELHIGH							=	-26375,					// 使用技能时，对方等级过高
	ERROR_USESKILL_BUFFTIME_LIMIT							=   -26374,					// buff时间叠加达到上限
	ERROR_USESKILL_MW_DISALLOW_MAP							=   -26373,					// 当前法宝不能在当前地图使用技能
	ERROR_USESKILL_INSERTBUFF_REMOVECOUNT					=	-26372,					// 减少buff层数
};

// 移动错误码 ( -26449 - -26400 )
enum
{
	ERR_PLAYER_CHANNETMOVE = -26449,
};

// 查询属性时的错误码 ( -26499 - -26450 )
enum EProperty
{
	ERROR_PROPERTY_ENTITYNOTFOUND							=	-26499	,		// 查询属性时，实体不存在
	ERROR_PROPERTY_ENTITYTYPEINVALID							=	-26498	,	// 查询属性时，实体类型错误
	ERROR_PROPERTY_DIPLOIDLESS							=	-26497	,			// 双倍时间不足
	ERROR_PROPERTY_HASFREEZE							=	-26496	,			// 已冻结，请解冻
	ERROR_PROPERTY_NOTDIPLOID							=	-26495	,			// 没有领取双倍，无需冻结
	ERROR_PROPERTY_NOTFREEZE							=	-26494	,			// 非冻结状态，无需解冻
	ERROR_PROPERTY_NOTNEEDFREEZE							=	-26493	,		// 已经冻结，无需再次冻结
	ERROR_PROPERTY_NOTVALIDDIPLOID							=	-26492	,		// 双倍时间无效
	ERROR_PROPERTY_OVERFLOWOBTAIN							=	-26491	,		// 双倍时间最多能领取4小时
	ERROR_PROPERTY_MONEYUPDATEERROR							=	-26490	,		// 金钱改变量太大，每次最多1000铜
	ERROR_PROPERTY_DIPLOIDSUCCESS							=	-26489	,		// 领取双倍成功
	ERROR_PROPERTY_FREEZESUCCESS							=	-26488	,		// 双倍冻结成功
	ERROR_PROPERTY_UNFREEZESUCCESS							=	-26487	,		// 双倍解冻成功
	ERROR_PROPERTY_DIPLOIDDISAPPEAR							=	-26486	,		// 双倍状态消失
	ERROR_PROPERTY_MONEYNOTENOUGH							=	-26485	,		// 复活时，金钱不够
	ERROR_PROPERTY_TITLENOTEXIST							=	-26484	,		// 称号不存在
	ERROR_PROPERTY_TITLEHASEXIST							=	-26483	,		// 称号已存在
	ERROR_PROPERTY_NOTONLINE								=	-26482	,		// 角色不在线
    	ERROR_PROPERTY_NOTSHOWEQUIP                             =   -26481  ,       // 角色不显示装备
};

// 配置技能点时的错误码 ( -26549 - -26500 )
enum EClickSP
{
	ERROR_CLICKSP_SUCCESS							=	-26549	,			// 配置技能点时，配置成功
	ERROR_CLICKSP_NEEDMOREPOINT							=	-26548	,		// 配置技能点时，需要更多的技能点
	ERROR_CLICKSP_NEEDSKILLPOINT							=	-26547	,	// 配置技能点时，指定需要前导技能的技能点不够
	ERROR_CLICKSP_NEEDGENERPOINT							=	-26546	,	// 配置技能点时，指定需要前导本系别技能点不够
	ERROR_CLICKSP_NEEDMORELEVEL							=	-26545	,		// 配置技能点时，需要更多的等级
	ERROR_CLICKSP_NEEDMETIER							=	-26544	,		// 配置技能点时，新手不能配置其它技能点
	ERROR_CLICKSP_SKILLOVERFLOW							=	-26543	,		// 配置技能点时，不能在学习更多的技能了
	ERROR_CLICKSP_NEEDMONEY							=	-26542	,			// 配置技能点时，需要更多的金钱
	ERROR_CLICKSP_NONEPLAYER							=	-26541	,		// 配置技能点时，找不到玩家实体(客户端可以不显示)
	ERROR_CLICKSP_NONESKILL							=	-26540	,			// 配置技能点时，找不到要学习的技能
	ERROR_CLICKSP_MONEYNOTENOUGH							=	-26539	,	// 金币不够学习技能

	ERROR_FANGCUN_HASLEARN							= -26538,				// 已经学过了
	ERROR_FANGCUN_NOPRESKILL						= -26537,				// 没有前置技能
	ERROR_FANGCUN_NOITEM							= -26536,				// 没有技能书
	ERROR_FANGCUN_LOWLEVEL							= -26535,				// 等级不够
};

// 技能点重置时的错误码 ( -26599 - -26550 )
enum EResetASP
{
	ERROR_RESETASP_SUCCESS			= -26599,			// 重置技能点时，重置成功
	ERROR_RESETASP_FAILED= -26598,								// 重置技能点失败
	ERROR_RESETASP_MAXNUM= -26597,								// 重置技能点失败,超过最大次数
	ERROR_RESETTALENT_SUCCESS = -26596,					// 重置造化成功
	ERROR_RESETTALENT_STEPLOW = -26595,					// 重置造化等级低
	ERROR_RESETTALENT_UNKNOWN = -26594,					// 重置造化未知错误
};

// 职业重置时的错误码 ( -26649 - -26600 )
enum EResetMetier
{
	ERROR_RESETMETIER_SUCCESS		= -26649,			// 重置职业时，重置成功
};

// 技能点重置一点时的错误码 ( -26699 - -26650 )
enum EResetSSP
{
	ERROR_RESETSSP_SUCCESS			= -26699,			// 重置技能点一点时，重置成功
	ERROR_RESETSSP_FAIL= -26698,								// 重置技能点一点时，重置失败
};

// 配置属性点时的错误码 ( -26749 - -26700 )
enum EClickPP
{
	ERROR_CLICKPP_SUCCESS			= -26749,			// 配置属性点时，配置成功
	ERROR_CLICKPP_NEEDMOREPOINT= -26748,						// 配置属性点时，需要更多的属性点
	ERROR_CLICKPP_PROPERTYINVALID= -26747,						// 配置属性点时，指定的属性无效
};

// 属性点重置一点时的错误码 ( -26799 - -26750 )
enum EResetSPP
{
	ERROR_RESETSPP_SUCCESS			= -26799,			// 重置属性点一点时,重置成功
	ERROR_RESETSPP_OUTOFPROPERTY= -26798,						// 重置属性点一点时,属性点不够了
	ERROR_RESETSPP_PROPERTYINVALID= -26797,						// 重置属性点一点时,指定的属性无效
};

// 属性点重置所有点时的错误码 ( -26849 - -26800 )
enum EResetAPP
{
	ERROR_RESETAPP_SUCCESS			= -26849,			// 重置属性点所有点时,重置成功
	ERROR_RESETAPP_FAILED= -26848,								// 重置属性点失败
	ERROR_RESETAPP_MAXNUM= -26847,								// 重置属性点失败,超过最大次数
};

// 普通攻击技能类错误 ( -26899 - -26850 )
enum ENormalAttack
{
	ERROR_NORMALATTACK_NOWEAPON				= -26899,		// 手里没有拿着武器
	ERROR_NORMALATTACK_NOTWEAPON= -26898,							// 手里拿着的不是武器
	ERROR_NORMALATTACK_OUTOFRANGE= -26897,							// 距离不够
	ERROR_NORMALATTACK_SRCSTATEINVALID= -26896,						// 源状态不正确
	ERROR_NORMALATTACK_DESSTATEINVALID= -26895,						// 目标状态不正确
	ERROR_NORMALATTACK_NOTCOOLDOWN= -26894,						// 技能还不能使用
};

// 点击NPC错误码 ( -26949 - -26900 )
enum EClickNpc
{
	ERROR_CLICKNPC_OUTOFRANGE		=	-26949,		// 距离不够
	ERROR_CLICKNPC_STATEINVALID=	-26948,					// 状态无效
	ERROR_CLICKNPC_TARGETINVALID=	-26947,					// 目标无效
	ERROR_CLICKNPC_OUTOFMONEY=	-26946,						// 金币不够
	ERROR_CLICKNPC_NOTMETIER=	-26945,						// 职业不匹配
	ERROR_CLICKNPC_BOXOWNER_ERR=	-26944,					// 这个宝箱不属于您
    ERROR_CLICKNPC_BOXHASPICKED= -26943,                    // 这个宝箱您已经拾取过了
};

// 扔错误码 ( -26959 - -26950 )
enum EThrow
{
	ERROR_THROW_OUTOFMONEY			=	-26959,		// 金币不够
	ERROR_PICK_CANNOT=	-26958,								// 无拾取权利的时候拾取
};

// 使用道具错误码 ( -27049 - -27000 )
enum EUseItem
{
	ERROR_ITEMSUCCESS							=	-27049	,			// 使用成功
	ERROR_ITEMNOTEXIST							=	-27048	,			// 物品不存在
	ERROR_TPLNOTEXIST							=	-27047	,			// 模板不存在
	ERROR_WRONGTYPE							=	-27046	,				// 类型错误
	ERROR_NOTENOUGHPOINT							=	-27045	,		// 点数不足
	ERROR_NOTCOOLDOWN							=	-27044	,			// 没有冷却
	ERROR_RECOVERING							=	-27043	,			// 道具恢复中
	ERROR_RANDOMTP							=	-27042	,				// 随机传送
	ERROR_RETURNTP							=	-27041	,				// 入口传送
	ERROR_REMEMBERTP							=	-27040	,			// 记忆传送
	ERROR_TOWNTP							=	-27039	,				// 回城传送
	ERROR_SRCDEAD							=	-27038	,				// 使用者死亡
	ERROR_UNUSEITEM							=	-27037	,				// 处于无法吃药状态
	ERROR_NOTTASKIITEM							=	-27036	,			// 不是任务道具
	ERROR_CANTUSE							=	-27035	,				// 该任务道具不能使用
	ERROR_OUTOFRANGE							=	-27034	,			// 该任务道具必须在指定地点使用		
	ERROR_OUTNATION							=	-27033	,				// 只能在本国使用
	ERROR_BAGGAGEFULL							=	-27032	,			// 包满了，不能再产生道具了
	ERROR_LEVELLOW							=	-27031	,				// 等级过低
	ERROR_METIERDIF							=	-27030	,				// 职业不相符
	ERROR_HORSE							=	-27029	,					// 坐骑逻辑
	ERROR_FULLHP							=	-27028	,				// 吃药血蓝满
	ERROR_FULLMP							=	-27027	,				
	ERROR_RETURNPAPER							=	-27026	,			// 门派返回卷
	ERROR_TELESYMBOL							=	-27025	,			// 传送符
	ERROR_STOVE							=	-27024	,					// 指定炉石
	ERROR_CANNOTRIDE							=	-27023	,			// 无法使用坐骑
	ERROR_FOREIGNUSE							=	-27022	,			// 外国无法使用道具
	ERROR_ITEMLOCKED							=	-27021	,				// 物品绑定中
	ERROR_NOSENDNPC							=	-27020	,				// 您没有护送NPC！ 
	ERROR_NPCNOBUFF							=	-27019	,				// 您的护送NPC身上没有这个BUFF
	ERROR_APFULL							=	-27018	,				// 您的活力值已满，不需要使用药品
	ERROR_CANNUSEITEM							=	-27017	,			// 不能使用物品
	ERROR_ISVIP							=	-27016	,					// 已经是会员
	ERROR_INVALIDVIP							=	-27015	,			// 无效会员道具
	ERROR_USEVIPITEMSUCCESS							=	-27014	,		// 会员开通成功
	ERROR_LOWERCLEANITEM							=	-27013	,		// 不能使用低级洗点符	-27013
	ERROR_LEVELLOWER							=	-27012	,			// 低于30级不能使用潜能洗点符 -27012
	ERROR_PPINIT							=	-27011	,				// 潜能点已经是初始点 -27011
	ERROR_SPINIT							=	-27010	,				// 技能点已经是初始点 -27010
	ERROR_WRONGPLACE 							=	-27009	,			// 使用的位置不正确 -27009
	ERROR_REDNOUSE							=	-27008	,				// 红名不能使用
	ERROR_CANNOTOFF							=   -27007	,				// 阻挡不能下马
	ERROR_RIDENOAP							=	-27006,					// 活力不足不能骑
	ERROR_LEVELHIGH							=	-27005,					// 等级过高
	ERROR_INWEDDINGHALL						=	-27004,					// 在礼堂不能传送类道具
	ERROR_NOTRED							=	-27003,					// 白名不能使用	
	ERROR_SUMMONOFFLINE						=	-27002,					// 使用召集时玩家不在线
	ERROR_SUMMONDIFFCOUNTRY					=	-27001,					// 召集时阵营不同
	ERROR_SUMMONWRONGPLACE					=	-27000,					// 该场景不能召集
	ERROR_NOSTAR							=	-26999,					// 无法摘星
	ERROR_STARHIGHLEVEL						=	-26998,					// 加星等级高
	ERROR_STARHIGHSTAR						=	-26997,					// 加星星级高
	ERROR_NOITEM							=   -26996,					// 使用时缺少物品
	ERROR_BLACKPILLSNONEED					=   -26995,					// 没有有效的离线时间，不用吃黑狗丸
	ERROR_USERED							=	-26994,					// 使用菩萨经
	ERROR_NOTUSEMAP							=	-26993,					// 使用地图错误
	ERROR_BLACKPILLS_MAXLEVEL				=	-26992,					// 已达到最大等级，无需使用黑狗丸
	ERROR_BLACKPILLS_MAXTIME				=	-26991,					// 黑狗丸的累积时间超过最大
	ERROR_BLACKPILLS_DIFFRENTBLACKPILLID	=	-26990,					// 使用的黑狗丸和已使用的黑狗丸不是同一类型
	ERROR_EXPPILLS_MAXLEVEL					=	-26989,					// 最大等级不能使用经验丹
	ERROR_EXPPILLS_LEVELLOW					=	-26988,					// 等级不够不能使用经验丹
	ERROR_BAIHUA_TIMEOUT					=	-26987,					// 您的百花仙子礼包已过期(使用时提示)
	ERROR_BAIHUA_BAGFULL					=	-26986,					// 包裹满 不能开启百花仙子礼包
	ERROR_BAIHUA_OVERTM						=	-26985,					// 您的百花仙子称号已过期。您的百花仙子道具已过期。您的百花仙子技能已过期。系统将过期物品等全部收回。
	ERROR_DIGITALFIREWORKS_WRONGITEM		=	-26984,					// 使用的物品不是数字礼花
	ERROR_DIGITALFIREWORKS_WRONGTYPE		=	-26983,					// 该数字礼花不能使用该特效
	ERROR_DIGITALFIREWORKS_TO0MUCHWORDS		=	-26982,					// 该数字礼花不能使用这么多的字符
	ERROR_FAME_LIMIT						=	-26981,					// 声望达上限无法使用
	ERROR_EXPTOME_MAXLEVEL	=	-26980,		//使用修炼心德等级过高
	ERROR_EXPTOME_LOWLEVEL	=	-26979,		//使用修炼心德等级过低
};

// 生产错误码 ( -27099 - -27050 )

enum EProduceItem
{
	ERROR_PROSUCCESS			=	-27099	,			// 生产成功
	ERROR_PRONOPRODUCE			=	-27098	,			// 无法生产此类物品
	ERROR_PRONOMATERIAL			=	-27097	,		// 材料不足
	ERROR_PRONOSPACE			=	-27096	,			// 包裹空间不足
	ERROR_PRONOTYPE			=	-27095	,			// 类型错误
	ERROR_ATTACKCANCEL			=	-27094	,			// 受到攻击，生产中止
	ERROR_MOVECANCEL			=	-27093	,			// 移动，生产中止
	ERROR_PROLOWLEVEL			=	-27092	,			// 等级不足
	ERROR_PRONOMONEY			=	-27091	,			// 金钱不足
	EEROR_PROLEVELUP			=	-27090	,			// 生产升级
	ERROR_PROCANCEL			=	-27089	,			// 生产中断
};

// 升级装备错误码 ( -27149 - -27100 )
enum EUpgItem
{
	ERROR_UPGSUCCESS			= -27149,				// 升级成功
	ERROR_UPGNOTYPE= -27148,									// 升级物品类型不对
	ERROR_UPGLIMIT= -27147,										// 该装备已经升到最高		
	ERROR_UPGNOTEXIST= -27146,									// 索引指定的道具不存在
	ERROR_UPGCANCEL= -27145,									// 升级中断
	ERROR_UPGEQUIPBINDBYITEM= -27144,							// 滴血的装备不能升级
};

// 修理装备错误码 ( -27199 - -27150 )
enum EMendResult
{
	EUIPMENT_SUCCESS				= -27199,		// 修理成功
	EUIPMENT_MONEYNOTENOUGH= -27198,						// 金钱不够
	EUIPMENT_MENDNOTNEED= -27197,							// 装备不需要修理 
	EUIPMENT_NOTEUIPMENT= -27196,							// 错误的装备

};

// 镶嵌符文 ( -27249 - -27200 )
enum EINSERTRUNE
{
	ERROR_RUNESUCCESS			= -27249,				// 镶嵌成功
	ERROR_RUNENOTYPE= -27248,									// 符文物品类型不对
	ERROR_CANNOTINSERT= -27247,									// 无法镶嵌
	ERROR_RUNEEQUIPBINDEDBYITEM= -27246,						// 滴血装备不能进行符文镶嵌
};

// 镶嵌宝石 ( -27299 - -27250 )
enum EINSERTJEWEL
{
	ERROR_JEWELSUCCESS							=	-27299	,					// 镶嵌成功
	ERROR_JEWELNOTYPE							=	-27298	,					// 物品类型不对
	ERROR_JEWELWRONGSLOT							=	-27297	,				// 不能镶进指定的凹槽
	ERROR_JEWELSLOTCLOSED							=	-27296	,				// 凹槽未打开
	ERROR_JEWELOPENFAIL							=	-27295	,					// 无法打开凹槽
	ERROR_JEWELLESSEGG							=	-27294	,					// 打开槽的元气蛋不够
	ERROR_JEWELEATFAIL							=	-27293	,					// 吃装备失败
	ERROR_COMPOSEFAIL							=	-27292	,					// 合成失败
	ERROR_CANNOTEAT							=	-27291	,						// 不能被炼丹炉吃掉的道具放入炼化界面
	ERROR_EQUIPBINDEDBYITEM							=	-27290	,				// 手动绑定的物品不能被炼化
	ERROR_JEWELEUIQPBINDEDBYITEM							=	-27289	,		// 手动绑定的物品不能镶嵌宝石
	ERROR_OPENSLOT_EQUIPBINDEDBYITEM							=	-27288	,	// 手动绑定的物品不能装备开槽
	ERROR_EATWRONGTYPE							= -27287,						// 吃装备类型不对
	ERROR_CARVE_NOCARD							= -27286,						// 雕琢符类型不对
	ERROR_CARVE_CANNOT							= -27285,						// 不能雕琢
	ERROR_CARVE_NOMONEY							= -27284,						// 金钱不足
	ERROR_CARVE_NOSPACE							= -27283,						// 需要至少一个空格
	ERROR_CARVE_BINDED							= -27282,						// 已经锁定
};

// 玩家传送 ( -27349 - -27300 )
enum ETELEPORT
{
	ERROR_TELESUCCESS		= -27349,					// 传送成功
	ERROR_TELENONPC= -27348,									// NPC没有传送功能
	ERROR_TELENOFEE= -27347,									// 费用不足
	ERROR_INSERVICE= -27346,									// 服务中，不能传送
	ERROR_TELEPORT_NOITEM= -27345,								// 无传送需要物品
	ERROR_TELEPORT_MAPLEVELLIMIT= -27344,						// 传送时地图等级限制
	ERROR_TELEPORT_CALLTIMEOUT= -27343,							// 传送时召唤已经超时
	ERROR_TELEPORT_NOBUFF	= -27342,							// 传送时无buff
	ERROR_TELEPORT_NOLEVEL	= -27341,							// 传送时等级不足
};

// 改变PK模式 ( -27399 - -27350 )
enum ECHANGEPKMODE
{
	ERROR_CHANGEPKMODE_SUCCESS				= -27399,		// 改变PK模式成功
	ERROR_CHANGEPKMODE_NEEDMORELEVEL= -27398,						// 等级不足
	ERROR_CHANGEPKMODE_NONEPKMODE= -27397,							// 无效的PK模式
	ERROR_CHANGEPKMODE_SAMEPKMODE= -27396,							// 相同的PK模式,无需切换
	ERROR_CHANGEPKMODE_UNKNOWN= -27395,								// 未知错误
	ERROR_CHANGEPKMODE_BADPK= -27394,								// 恶意PK无法切换PK模式
	ERROR_CHANGEPKMODE_WORLDWAR = -27393,							// 阵营大战无法切换PK模式
};

// 玩家新定义操作 ( -27449 - -27400 )
enum EOPERATION
{
	ERROR_ADDSLOTPUSHERROR							=	-27449	,			// 增加凹槽放入失败
	ERROR_ADDSLOTPOPERROR							=	-27448	,			// 增加凹槽拿出失败
	ERROR_ADDSLOTERROR							=	-27447	,				// 增加凹槽失败
	ERROR_REMOVEJEWELPUSHERROR							=	-27446	,		// 摘除宝石放入失败
	ERROR_REMOVEJEWELPOPERROR							=	-27445	,		// 摘除宝石拿出失败
	ERROR_REMOVEJEWELERROR							=	-27444	,			// 摘除宝石失败
	ERROR_JUDGEPUSHERROR							=	-27443	,			// 鉴定装备放入失败
	ERROR_JUDGEPOPERROR							=	-27442	,				// 鉴定装备拿出失败
	ERROR_JUDGEERROR							=	-27441	,				// 鉴定装备失败
	ERROR_CHANGEPUSHERROR							=	-27440	,			// 转换抗性放入失败
	ERROR_CHANGEPOPERROR							=	-27439	,			// 转换抗性拿出失败
	ERROR_CHANGEERROR							=	-27438	,				// 转换抗性失败
	ERROR_JEWCOMPUSHERROR							=	-27437	,			// 宝石合成放入失败
	ERROR_JEWCOMPOPERROR							=	-27436	,			// 宝石合成拿出失败
	ERROR_JEWCOMERROR							=	-27435	,				// 宝石合成失败
	ERROR_MULTICOMERROR							=	-27434	,				// 复杂合成失败
	ERROR_MULTICOMNOTENOUGH							=	-27433	,			// 复杂合成数值不足
	ERROR_MULTICOMFULL							=	-27432	,				// 复杂合成包裹满
	ERROR_CLICKTALENTFAILED							=	-27431	,			// 点击造化点失败
	ERROR_INCREASETALENTFAILED							=	-27430	,		// 造化进阶失败
	ERROR_CLICKTALENT_NOMONEY							=	-27429	,		// 点击造化点钱不足
	ERROR_CLICKTALENT_MAX							=	-27428	,			// 点击造化点已经达到最大
	ERROR_CLICKTALENT_NOPOINT							=	-27427	,		// 点击造化点点数不足
	ERROR_MULTIPUSHERROR							=	-27426	,			// 复杂合成放入失败
	ERROR_MULTIPOPERROR							=	-27425	,				// 复杂合成拿出失败
	ERROR_ADDWIZARD_UNKNOWN							=	-27424	,			// 才华加点失败
	ERROR_ADDWIZARD_NOTENOUGH							=	-27423	,		// 才华加点点数不足
	ERROR_GETTREND_NOMONEY							=	-27422	,			// 算命钱不够
	ERROR_JEWCOMFAIL							=	-27421	,				// 宝石合成失败
	ERROR_JEWREMOVE_EQUIPBINDEDBYITEM							=	-27420,	// 滴血的装备不能移除宝石,
	ERROR_ADDSLOT_EQUIPBINDEDBYITEM 							=	-27419,	// 滴血的装备不能增加凹槽,
	ERROR_JUDGE_EQUIPBINDEDBYITEM 							=	-27418	,	// 滴血的装备不能进行装备鉴定
	ERROR_CHANGERES_EQUIPBINDEDBYITEM 							=	-27417,		// 滴血的装备不能进行装备鉴定	,
	ERROR_CHANGEEQUIPERROR									= -27416,		// 转换装备属性失败
	ERROR_CANNOTEXTRA										= -27415,		// 无法进行刻印
	ERROR_HASNOMARRINTIMACY									= -27414,		// 没有结婚不能进行结婚装备的升级
	ERROR_MARRINTIMACYNOTENOUGH								= -27413,		// 夫妻亲密度不够,不能兑换装备
	ERROR_NOTINSWORNTEAM									= -27412,		// 没有结拜不能进行结拜装备兑换
	ERROR_SWORNCOMPOSE_NUMBERNOTENOUGH						= -27411,		// 结拜兑换时人数不
	ERROR_SWORNCOMPOSE_NOTINSAMESWORNTEAM					= -27410,		// 结拜兑换时队伍中有人不属于结拜团体
	ERROR_SWORNCOMPOSE_INTIMACYNOTENOUGH					= -27409,		// 结拜兑换时结拜亲密度不够
};

//map			( -30999 - -30000 )
//friend		( -32999 - -31000 )

enum EFriendModuleErrorCode
{
	ERROR_DES_PLAYER_IS_NOT_ONLINE							=	-32999	,	 // 对方玩家不在线
	ERROR_YOUR_LIST_FULL							=	-32998	,				   // 你的好友列表满了，添加好友失败
	ERROR_DES_LIST_FULL							=	-32997	,					   // 对方好友列表满了，添加好友失败
	ERROR_CANT_ADD_SELF							=	-32996	,					   // 不能添加自己
	ERROR_GROUP_NAME_TOO_LONG							=	-32995	,		   // 分组名太长
	ERROR_MOVE_FRIEND_FAIL							=	-32994	,			 // 移动好友分组失败
	ERROR_MESSAGE_TOO_LONG							=	-32993	,			 // 私聊消息太长
	ERROR_YOU_ARE_BLACK							=	-32992	,				 // 你在对方黑名单中，不能私聊
	ERROR_DES_OFFLINE			    				=	-32991	,			 // 对方不在线，不能私聊
	ERROR_MOVE_BLACK_FAIL							=	-32990	,			 // 移动好友到黑名单失败
	ERROR_INVALID_PLAYER							=	-32989	,			 // 玩家不存在
	ERROR_LOCKENEMYFAIL							=	-32988	,					// 锁定仇敌失败
	ERROR_STRANGERBLOCK							=	-32987	,					// 向开启了“拒绝陌生人信息”的玩家发送消息" ShowText="对方屏蔽
	ERROR_ADDOTHERNATIONFRIEND							=	-32986	,			// "将他国玩家加为好友" ShowText="不能与他国玩家建立好友关系
	ERROR_ADDSELFTOBLACK							=	-32985	,				// 不能加自己到黑名单
	ERROR_ISFRIEND							=	-32984	,						// 已经是好友了，添加失败
	ERROR_YOU_ARE_BLACK_ADD							=	-32983	,				// 你在对方黑名单，加好友失败
	ERROR_ADDBLACKFAIL							=	-32982	,					// 超出上限
	ERROR_ADDBLACKFAIL_EXSIT							=	-32981	,			// 已存在的黑名单
	ERROR_DES_IN_YOUR_BLACK							=	-32980	,				// 此角色已被您屏蔽、不允许发送消息。
	ERROR_MASTER_LOWLEVEL							=	-32979	,				// 前辈玩家等级不足
	ERROR_MASTER_NOTONLINE							=	-32978	,				// 前辈玩家不在线
	ERROR_MASTER_HIGHLEVEL							=	-32977	,				// 后辈玩家等级高
	ERROR_MASTER_NOTNULL							=	-32976	,				// 已经有了前辈
	ERROR_MASTER_FULL							=	-32975	,					// 前辈玩家后辈已满
	ERROR_MASTER_NOSLAVE						=	-32974	,					// 删除时没有此后辈
	ERROR_JEWCOM_LOWLEVEL						=	-32973	,					// 宝石合成等级低
	ERROR_MASTER_NOMASTER						=	-32972	,					// 删除时没有此玩家
	ERROR_MASTER_CHANGENOMASTER					=	-32971	,					// 改变感恩时不是前辈
	ERROR_MASTER_CHANGEOVERFLOW					=	-32970	,					// 改变感恩时超出范围
	ERROR_MASTER_NOMOREMASTER					=	-32969	,					// 不能添加新前辈
	ERROR_MASTER_NOTIMES						=	-32968	,					// 添加前辈次数已满
	ERROR_MASTER_NOITEM							=	-32967	,					// 缺少道具解除
	ERROR_MASTER_NOMONEY						=	-32966	,					// 缺少钱解除
	ERROR_MASTER_REFUSED						=	-32965	,					// 对方拒绝
	ERROR_MASTER_NOTSAMEWORLD					=	-32964	,					// 不同阵营不能拜师
	ERROR_SWORN_KICK_HASNORIGHT					=	-32963	,					// 结拜踢人时没有权限
	ERROR_SWORN_KICK_NOTINSWORNTEAM				=	-32962	,					// 结拜踢人时所踢之人不在结拜团体里面
	ERROR_SWORN_LEAVE_NOTINSWORNTEAM			=	-32961	,					// 离开结拜时不在结拜中
	ERROR_SWORN_DISBAND_HSARIGHT				=	-32960	,					// 解散结拜时没有权限
	ERROR_SWORN_CREATE_MEMBERNOTENOUGH			=	-32959	,					// 创建结拜时人数不够(一个人不能创建)
	ERROR_SWORN_CREATE_HAVENOTEAM				=	-32958	,					// 没有队伍不能创建结拜
	ERROR_SWORN_CREATE_NOTHEADER				=	-32957	,					// 不是队长不能结拜
	ERROR_SWORN_CREATE_INSWORNTEAM				=	-32956	,					// 队伍中有人已经结拜，不能进行结拜
	ERROR_SWORN_CREATE_INTIMACYNOTENOUGH		=	-32955	,					// 队伍中有人亲密度不够，不能结拜
	ERROR_SWORN_CREATE_HASDITYWORD				=	-32954	,					// 名称含有脏词	
	ERROR_SWORN_DISBAND_HASNORIGHT				=	-32953	,					// 解散结拜团体时没有权限
	ERROR_SWORN_CHANGENAME_HASNOWRIGHT			=	-32952	,					// 结拜改名时没有权利
	ERROR_SWORN_CHANGENAME_HASDIRTYWORD			=	-32951	,					// 结拜改名时名称含有脏词
	ERROR_SWORN_CREATE_MEMBERTOOFAR				=	-32950	,					// 创建结拜时有成员距离太远
	ERROR_SWORN_CREATE_HASNOTIME				=	-32949	,					// 创建结拜时没有道具
	ERROR_SWORN_KICKMEMBER_HASNOTIME			=	-32948	,					// 踢人时没有道具
	ERROR_SWORN_DISBAND_HASNOTIME				=	-32947	,					// 解散时没有道具
	ERROR_SWORN_CHANGENAME_HASNOTIME			=	-32946	,					// 改名时没有道具
	ERROR_DELFRIEND_SPOURSE						=	-32945	,					// 结婚关系不能删除好友
	ERROR_DELFRIEND_INSAMESWORNTEAM				=	-32944	,					// 结拜关系不能删除好友
	ERROR_MOVEBLACK_INSAMESWORNTEAM				=	-32943	,					// 结拜关系不能移动到黑名单
	ERROR_MOVEBLACK_MARRIED						=	-32942	,					// 有结婚关系不能移动到黑名单
	ERROR_SWORN_REBUILD_HASNOTEAM				=	-32941	,					// 没有组队不能重新加人
	ERROR_SWORN_REBUILD_NOTHEADER				=	-32940	,					// 不是结拜老大不能重新加人
	ERROR_SWORN_REBUILD_HASNOSWORN				=	-32939	,					// 没有结拜不能重新加人
	ERROR_SWORN_REBUILD_MEBERTOOFAR				=	-32938	,					// 加新人时有人距离过远
	ERROR_SWORN_REBUILD_NEWMEMBERHASSWORN		=	-32936	,					// 新人已经有结拜
	ERROR_SWORN_REBUILD_MEMBEROFFLIEN			=	-32935	,					// 有结拜成员不在队伍中，无法结拜
	ERROR_SWORN_REBUILD_HASNONEWMEMBER			=	-32934	,					// 结拜加人时队伍中没有新成员
	ERROR_SWORN_REBUILD_HASNOREBUILDITEM		=	-32933	,					// 有新成员没有加入结拜需要的道具
	ERROR_SWORN_REBUILD_INTIMACYNOTENOUGH		=	-32932	,					// 加入新成员时有人亲密度不够
};
//team			( -35999 - -33000 )

enum ETeam 
{
	ERROR_DISABLEHANDLE						=	-35999	,					// 当前状态下，无法执行此操作
	ERROR_TEAM_DESBUSY						=	-35998	,					// 对方正忙，无法发送组队操作
	ERROR_TEAM_DIFFNATION						=	-35997	,				// 和对方不同国籍，无法组队
	ERROR_TEAM_FULLTEAM							=	-35996	,				// 队伍已满 
	ERROR_TEAM_OVERFAR						=	-35995	,					// 对方超出有效距离，无法操作
	ERROR_TEAM_NOTEXIST							=	-35994	,				// 对不起，队伍不存在
	ERROR_TEAM_LEVELLESS						=	-35993	,				// 对不起，您级别不够
	ERROR_TEAM_SRCINTEAM						=	-35992	,				// 您已有队伍，不可加入其他队伍
	ERROR_TEAM_DESINTEAM						=	-35991	,				// 对方已有队伍
	ERROR_TEAM_SRCNOTTEAM						=	-35990	,				// 对不起，您没有队伍	(0)
	ERROR_TEAM_DESNOTTEAM						=	-35989	,				// 对不起，对方没有队伍
	ERROR_TEAM_REFUSE						=	-35988	,					// 对不起，对方拒绝了您的请求
	ERROR_TEAM_DESOFFLINE						=	-35987	,				// 对不起，对方掉线，无法操作
	ERROR_TEAM_MONEYLESS						=	-35986	,				// 对不起，金币不够
	ERROR_TEAM_COOLINGTIME						=	-35985	,				// 对不起，冷却时间未到
	ERROR_TEAM_TELEFAILED						=	-35984	,				// 传送失败
	ERROR_TEAM_INSERVICE						=	-35983	,				// 服务中，无法操作
	ERROR_TEAM_SENDAPPLYTOHEADER						=	-35982	,		// 您已向队长发送入队申请
	ERROR_TEAM_NOTHEADER						=	-35981	,				// 对不起，您不是队长，无法操作
	ERROR_TEAM_INVITE_SUCCESS						=	-35980	,			// 组队邀请**成功(0)
	ERROR_TEAM_REQUEST_SUCCESS						=	-35979	,			// 您已向**发送入队申请
	ERROR_TEAM_INVITE_REFUSE						=	-35978	,			// 对不起，**拒绝了您的组队邀请
	ERROR_TEAM_REQUEST_REFUSE						=	-35977	,			// 对不起，**拒绝了您的入队申请
	ERROR_TEAM_CREATE_HASINTEAM							=	-35976	,		// 已在队伍中，无法自建队伍
	ERROR_TEAM_INVITE_DESINTEAM							=	-35975	,		// 对方已有队伍，无法发送组队邀请
	ERROR_TEAM_REQEUST_SRCINTEAM						=	-35974	,		// 您已经在队伍中，无法发送入队申请
	ERROR_TEAM_NEWHEAD_NOTHEADER						=	-35973	,		// 您不是队长，无法进行队长权限转让
	ERROR_TEAM_JOINTEAM							=	-35972	,				// **加入队伍
	ERROR_TEAM_END_SUCCESS						=	-35971	,				// 您所在的队伍已解散
	ERROR_TEAM_NEWHEAD_SUCCESS						=	-35970	,			// A将队长转让给B(0)
	ERROR_TEAM_CREATE_SUCCESS						=	-35969	,			// 自建队伍成功
	ERROR_TEAM_LEAVE_SUCCESS						=	-35968	,			// **离开队伍
	ERROR_TEAM_KICK_SUCCESS							=	-35967	,			// **被请离队伍
	ERROR_TEAM_CLAIM_INTEAM							=	-35966	,			// 队伍中，无法发送求组信息
	ERROR_TEAM_CANTFOLLOW_FAR						=	-35965	,			// 距离太远无法跟随
	ERROR_TEAM_OPEN_FOLLOW						=	-35964	,				// 开启跟随状态
	ERROR_TEAM_CLOSE_FOLLOW							=	-35963	,			// 关闭跟随状态
	ERROR_TEAM_ASSIGN_RANDOM						=	-35962	,			// 分配模式为自由分配
	ERROR_TEAM_ASSIGN_TURN						=	-35961	,				// 分配模式为轮流分配
	ERROR_TEAM_NONATION							=	-35960	,				// 没有国籍，不能组队
	ERROR_TEAM_NOTSAMETEAM						=	-35959	,				// 不同队
	ERROR_TEAM_CHANGED						=	-35958	,					// 队伍已变化
	ERROR_TEAM_NOTINGAME						=	-35957	,				// 有玩家不在场景
	ERROR_TEAM_NOTINVITE						=	-35956	,				// 没有邀请
	ERROR_TEAM_NOTREQUEST						=	-35955	,				// 没有申请
	ERROR_TEAM_MOREREQ						=	-35954	,					// 队伍接受的申请消息达到上限
	ERROR_TEAM_MOREINV						=	-35953	,					// 玩家接受的邀请消息达到上限
	ERROR_TEAM_ASSIGN_PRO						=	-35952	,				// 分配模式为概率分配

};

//stall			( -36999 - -36000 )

enum EStall
{
	ERROR_STALL_NOTSAFTYAREAR							=	-36999	,		// 不在摆摊安全区					(-36999)
	ERROR_STALL_LEVELLESS							=	-36998	,			// 等级不够
	ERROR_STALL_INSAFTYTIME							=	-36997	,			// 安全保护时间内
	ERROR_STALL_DIFFNATION							=	-36996	,			// 国籍不符合
	ERROR_STALL_HASOTHERSTALL							=	-36995	,		// 此地已有其他摊位
	ERROR_STALL_HASSTALL							=	-36994	,			// 您已有摊位
	ERROR_STALL_CANNOTSELLIT							=	-36993	,		// 不能出售该物品
	ERROR_STALL_BINDITEM							=	-36992	,			// 绑定物品无法上架
	ERROR_STALL_MASTERMONEYOUT							=	-36991	,		// 摊主金钱 超过携带上限					
	ERROR_STALL_NOTEXIST							=	-36990	,			// 摊位不存在						(-36990)
	ERROR_STALL_NAMELONGER							=	-36989	,			// 摊名过长
	ERROR_STALL_ADLONGER							=	-36988	,			// 广告内容过长
	ERROR_STALL_OFFLINE							=	-36987	,				// 玩家不在线
	ERROR_STALL_GOODSCHANGED							=	-36986	,		// 商品已更新
	ERROR_STALL_PRICECHANGED							=	-36985	,		// 价格已更新
	ERROR_STALL_PRICEERROR							=	-36984	,			// 价格超范围
	ERROR_STALL_PRICEFAILED							=	-36983	,			// 价格更新失败
	ERROR_STALL_BUYGOODSFAILED							=	-36982	,		// 购买商品失败
	ERROR_STALL_GRIDHASSTALL							=	-36981	,		// 格子上已经有其他玩家摆摊		
	ERROR_STALL_SRCMONEYLESS							=	-36980	,		// 买家金钱不足					(-36980)
	ERROR_STALL_PUSHUPFAILED							=	-36979	,		// 物品上架失败
	ERROR_STALL_CLICKSTALLFAILED							=	-36978	,	// 点击摊位失败
	ERROR_STALL_FULLGOODS							=	-36977	,			// 摊位商品已满
	ERROR_STALL_INVALIDOPERATION							=	-36976	,	// 摆摊状态下无法进行该操作
	ERROR_STALL_TASKITEM							=	-36975	,			// 任务物品不能上架出售
	ERROR_STALL_GOODSNOTEXIST							=	-36974	,		// 该摊位物品无存在
	ERROR_STALL_NOTLOOKENEMY							=	-36973	,		// 不能查看敌国摊位
	ERROR_STALL_INHORSE							=	-36972	,				// 坐骑状态，不能摆摊
	ERROR_STALL_NUMBEROVER							=	-36971	,			// 购买数量超出					
	ERROR_STALL_INOTHERSERVICE							=	-36970	,		// 正处于其他服务中				(036970)
	ERROR_STALL_CREATESTALLSUCCESS							=	-36969	,	// 创建摊位成功
	ERROR_STALL_BAGGAGENOTENOUGH							=	-36968	,	// 顾客购买时包裹空间不足
	ERROR_STALL_UPDATEPRICESUCCESS							=	-36967	,	// 价格修改成功
	ERROR_STALL_ERRORSTALLTYPE						= -36966,				// 摆摊类型错误

};

//chat			( -39999 - -37000 )

enum EChat
{
	ERROR_CHAT_OFFLINE  							=	-39999	,				// 无法找到密语对象
	ERROR_CHAT_EMPTYMSG             							=	-39998	,	// 全部空格的字串没有任何含义，无法发送
	ERROR_CHAT_EMPTYP2PMSG        							=	-39997	,		// 不能发送空悄悄话
	ERROR_CHAT_SETCHANNELNULL       							=	-39996	,	// 设置屏蔽某个频道
	ERROR_CHAT_CANCELCHANNELNULL    							=	-39995	,	// 取消频道
	ERROR_CHAT_SETCHATDIANULL       							=	-39994	,	// 屏蔽聊天气泡
	ERROR_CHAT_CANCELCHATDIANULL    							=	-39993	,	// 取消屏蔽聊天气泡
	ERROR_CHAT_SETWORDCOLNULL       							=	-39992	,	// 屏蔽文字自定义颜色
	ERROR_CHAT_CANCELWORDCOLNULL  							=	-39991	,		// 取消屏蔽文字自定义颜色
	ERROR_CHAT_CANCELPERCHATNULL    							=	-39990	,	// 取消对某人的屏蔽设置
	ERROR_CHAT_CONTRYCHATLEVELLOW   							=	-39989	,	// 本国频道发言等级不够，低于10级
	ERROR_CHAT_CONTRYMSGSUCESS      							=	-39988	,	// 本国频道发言成功
	ERROR_CHAT_CHANNELCONTRYAPLOW   							=	-39987	,	// 本国频道发言AP不够，低于10
	ERROR_CHAT_CHANNELWORLDLEVELLOW							=	-39986	,		// 世界频道等级不够，低于30
	ERROR_CHAT_WORLDMSGSUCESS      							=	-39985	,		// 世界频道发言成功
	ERROR_CHAT_CHANNELWORLDAPLOW    							=	-39984	,	// 世界频道发言AP不够，低于20
	ERROR_CHAT_WORLDCHATFAST        							=	-39983	,	// 玩家在世界频道发言成功的180秒内再次发言
	ERROR_CHAT_SCREENLOCK           							=	-39982	,	// 使用了锁频功能，屏幕锁定当前状态
	ERROR_CHAT_SCREENUNLOCK         							=	-39981	,	// 屏幕解锁
	ERROR_CHAT_NOFAMILY             							=	-39980	,	// 没有家族不能在家族频道发言
	ERROR_CHAT_NOTEAM               							=	-39979	,	// 没有组队不能在组队频道发言
	ERROR_CHAT_NOBEGTEAM            							=	-39978	,	 // 没有团队不能在团队频道发言
	ERROR_CHAT_NOCORP               							=	-39977	,	 // 没有军团不能在军团频道发送信息
	ERROR_CHAT_CHATFORBIDEN         							=	-39976	,	// 被禁言
	ERROR_CHAT_CHATNOTALLOWED       							=	-39975	,	// 禁言还未解除，只能在队伍频道发言
	ERROR_CHAT_HAVENOBUGLEITEM      							=	-39974	,	// 在喇叭频道发言时没有喇叭道具
	ERROR_CHAT_HIGHFREQUENCY	    						=	-39973	,		// 发言过于频繁
	ERROR_CHAT_CHANNELDIRTYWORD     							=	-39972	,	// 自建频道含有非法词
	ERROR_CHAT_DELETECHATNNEL       							=	-39971	,	// 删除了自建频道
	ERROR_CHAT_DELETECHANTNELERROR  							=	-39970	,	// 删除频道错误
	ERROR_CHAT_CHATMSGSAVED        							=	-39969	,		// 聊天信息已经保存
	ERROR_CHAT_SETAUTOCHAT          							=	-39968	,	// 设置了自动喊话功能
	ERROR_CHAT_HASNOAUTOCHATITEM    							=	-39967	,	// 设置自动喊话功能时没有狮子吼
	ERROR_CHAT_AUTOCHATTIMESHORT    							=	-39966	,	// 自动喊话间隔时间小于所选频道的发言间隔时间
	ERROR_CHAT_AUTOCHATTIMELONG     							=	-39965	,	// 玩家设置的自动喊话间隔时间大于5分钟
	ERROR_CHAT_CHANNELBUGLESUCESS   							=	-39964	,	// 喇叭频道发言成功
	ERROR_CHAT_BUGLENEEDWAIT       							=	-39963	,		// 使用喇叭发言，遇到排队情况
	ERROR_CHAT_MSGREPEATET         							=	-39962	,		// 同一句话重复次数过多
	ERROR_CHAT_INCOOLTIME          							=	-39961	,		// 登录或者跨服后不能立即发言
	ERROR_CHAT_WORLDBUGLENO		   					=	-39960	,				// 世界频道喊话时银喇叭不存在
	ERROR_CHAT_BUGLELEVELLIMIT							=	-39959	,				// 喇叭频道喊话等级不够

};
// task         ( -42999 - -40000 )
enum ETask
{
	ERROR_TASK_SUCCESS							=	-42999	,
	ERROR_TASK_HASACCEPT							=	-42998	,					// 任务已接受
	ERROR_TASK_HASDONE							=	-42997	,						// 任务已完成
	ERROR_TASK_OUTOFSPACE							=	-42996	,					// 任务列表已满
	ERROR_TASK_DIALOGIDINVALID							=	-42995	,				// 任务对话ID无效
	ERROR_TASK_TASKIDINVALID							=	-42994	,				// 任务ID无效
	ERROR_TASK_NOTCOMPLETE							=	-42993	,					// 任务还没有完成
	ERROR_TASK_NOSPACE							=	-42992	,						// 没有足够的包裹空间
	ERROR_TASK_ABORTINVALID							=	-42991	,					// 放弃任务出错
	ERROR_TASK_ACCEPTINVALID							=	-42990	,				// 任务接受条件不满足
	ERROR_TASK_MUTI_AWARD_ERROR							=	-42989	,				// 没有选择可选奖励物品
	ERROR_TASK_CANT_DO_INVITE							=	-42988	,				// 对方不能接这个任务(邀请的服务器回应)
	ERROR_TASK_ABORTINVALID_NOITEM  							=	-42987	,		// 任务不能够被放弃	 因为发放的物品不齐全了
	ERROR_TASK_TELEPORTFAIL							=	-42986	,					// 传送失败
	ERROR_TASK_CANTABORTMAIN							=	-42985	,				// 任务不能放弃
	ERROR_TASK_TALKWITHNPC_FAILED							=	-42984	,			// 和NPC对话失败
	ERROR_TASK_STORAGEMAX							=	-42983	,					// 库任务已经最大值
	ERROR_TASK_BAD_RECV_ITEM							=	-42982	,				// 完成任务物品提交错误
	ERROR_TASK_LESS_RECV_ITEM							=	-42981	,				// 提交的完成物品不足
	ERROR_TASK_ITEM_BIND_ERROR							=	-42980	,				// 该物品是手动绑定的 不能用于交任务
	ERROR_TASK_COLLECT_FAIL							=	-42979	,					// 采集失败(可能已经死了)
	ERROR_TASK_COLLECT_GRIDERR							=	-42978	,				// 采集距离太远
	ERROR_TASK_NPC_BUSY							=	-42977	,						// NPC正在被其他玩家占用 
	ERROR_TASK_SENDNPC_LEAVE							=	-42976	,				// 你已离开了护送NPC
	ERROR_TASK_LOOP_CD_NOTEND							=	-42975	,				// 环任务冷却时间未结束
	ERROR_TASK_INVITE_TIMEOUT							=	-42974	,				// 任务邀请：邀请已失效
	ERROR_TASK_NOT_YOUR_NPC							=	-42973	,					// 您不能和非自己召唤的NPC对话
	ERROR_TASK_ON_END_A_NEW_LOOP							=	-42972	,			// 恭喜您完成了本周的环任务
	ERROR_TASK_ON_END_TRAGEDY10							=	-42971	,				// 恭喜你完成了十难
	ERROR_TASK_MAX_RMB_REFRESH							=	-42970	,				// 已达到每日最大使用RMB刷库任务次数
	ERROR_TASK_GOT_CHAIRMAN							=	-42969	,					// 已接过其他系列的掌门任务
	ERROR_TASK_CHAIRMAN_AWARDED							=	-42968	,				// 获得掌门任务额外奖励通知
	ERROR_TASK_TEAMLOOP_MEMBERGOT						=	-42966	,				// 组队环任务：这个玩家已经接取组队环任务
	ERROR_TASK_TEAMLOOP_MEMBERCOMP						=	-42965	,				// 组队环任务：这个玩家的完成次数已经达到上限 
	ERROR_TASK_TEAMLOOP_MEMBERFULL						=	-42964	,				// 组队环任务：这个玩家的任务列表已满
	ERROR_TASK_TEAMLOOP_YOUNOTHEAD						=	-42963	,				// 组队环任务：你不是队长 不能接
	ERROR_TASK_TEAMLOOP_MEMBERLESS						=	-42962	,				// 组队环任务：队伍人数不正确 不能接
	ERROR_TASK_TEAMLOOP_MEMBERERR						=	-42961	,				// 组队环任务：你有队伍成员不在附近
	ERROR_TASK_TEAMLOOP_CANT_ACCEPT						=	-42960	,				// 组队环任务：玩家%s不满足接任务条件
	ERROR_TASK_PUBLISH_PLAYER_LEVEL						=	-42959	,				// 发布任务：不满足发布该颜色任务的条件
	ERROR_TASK_PUBLISH_COLOR_MAX						=	-42958	,				// 发布任务：该颜色发布次数已到上限
	ERROR_TASK_PUBLISH__MAX_TIMES						=   -42957	,				// 发布任务：今天发布任务已经到上限
	ERROR_TASK_PUBLISH_NOT_EXIST						=   -42956	,				// 发布任务：任务数据不存在
	ERROR_TASK_UNPUBLISHED								=   -42955	,				// 发布任务：任务尚未发布
			
	ERROR_TASK_HAS_PUBLISHED							=	-42954	,				// 团体发布任务：任务已发布（防止重复发）
	ERROR_TASK_GROUPPUBLISH_TODAYMAX					=   -42953  ,               // 团体发布任务：今天发布任务已经到上限
	ERROR_TASK_GROUPPUBLISH_WEEKMAX						=   -42952  ,               // 团体发布任务： 本周发布次数已到上限
	ERROR_TASK_PUBLISH_CONDITION_UNENOUGH				=   -42951  ,				// 发布条件不满足
	ERROR_TASK_SINGLE_PUBLISH_NOAWARD					=   -42950  ,				// 没有可领取的奖励


	// 没用的
	ERROR_TASK_ORP_FAILED			,		// 操作失败（交任务失败，或接任务失败，用于客户端进行关闭对话等操作）
};

// trade		( -43999 - -43000 )
enum ETrade
{
	ERROR_TRADE_INTRADE							=	-43999	,			// 交易中
	ERROR_TRADE_OFFTRADE							=	-43998	,		// 离开交易，交易空闲
	ERROR_TRADE_PLAYERNOTFOUND							=	-43997	,	// 玩家不存在
	ERROR_TRADE_GOODSNOTFOUND							=	-43996	,	// 物品不存在
	ERROR_TRADE_GOODSINTRADE							=	-43995	,	// 物品在交易中
	ERROR_TRADE_GOODSOFFTRADE							=	-43994	,	// 物品闲置，不在交易中
	ERROR_TRADE_REFUSE							=	-43993	,			// 交易邀请拒绝
	ERROR_TRADE_INBUSY							=	-43992	,			// 交易繁忙中
	ERROR_TRADE_FAR							=	-43991	,				// 距离太远，无法交易
	ERROR_TRADE_END							=	-43990	,				// 交易结束，打开		(0)
	ERROR_TRADE_SRCBAGLESS							=	-43989	,		// 源包裹空间不够
	ERROR_TRADE_DESBAGLESS							=	-43988	,		// 对方包裹空间不够
	ERROR_TRADE_OFFLINE							=	-43987	,			// 玩家掉线，交易结束
	ERROR_TRADE_DESMONEYOVER							=	-43986	,	// 交易时，对方不能携带更多的金钱
	ERROR_TRADE_SAFETIME							=	-43985	,		// 安全时间内，无法交易
	ERROR_TRADE_BINDITEM							=	-43984	,		// 物品绑定，无法交易
	ERROR_TRADE_INVITESELF							=	-43983	,		// 不能与自己交易
	ERROR_TRADE_GOODSLOCKED							=	-43982	,		// 物品栏锁定，无法变动信息
	ERROR_TRADE_GOODSUNLOCKED							=	-43981	,	// 物品栏未锁定
	ERROR_TRADE_FAILURE							=	-43980	,			// 交易失败	(0)
	ERROR_TRADE_MONEYERROR							=	-43979	,		// 交易金额无效
	ERROR_TRADE_INVITEREPEATED							=	-43978	,	// 重复发送邀请
	ERROR_TRADE_INVITETIMEOVER							=	-43977	,	// 邀请超时无效
	ERROR_TRADE_CANCEL							=	-43976	,			// 玩家取消交易
	ERROR_TRADE_MONEYLESS							=	-43975	,		// 包裹金币不够
	ERROR_TRADE_MONEYOVERFLOW							=	-43974	,	// 交易时，您不能携带更多的金钱
	ERROR_TRADE_INVITEOVERFLOW							=	-43973	,	// 邀请人数超过上限
	ERROR_TRADE_DESBUSY							=	-43972	,			// 对方正忙，无法进行交易
	ERROR_TRADE_MONEYOUT							=	-43971	,		// 金钱溢出
	ERROR_TRADE_INVITE_SUCCESS							=	-43970	,	// 交易邀请成功(0)
	ERROR_TRADE_DIFFNATION							=	-43969	,		// 国籍不同，不能交易
	ERROR_TRADE_NOTPILE								=	-43968	,		// 必须整堆物品交易
	ERROR_TRADE_INEXPIREDKITBAG							=	-43967	,	// 过期背包里面的物品不能用于交易
};


// RedirectMap		( -44999 ~ -44000 )
enum ERedirect
{
	ERR_REDIRECT_PLAYER_IN_MAP      = -44999,
	ERR_REDIRECT_OBJECT_IS_FULL= -44998,
	ERR_REDRIECT_UNKNOW= -44997,
	ERR_REDIRECT_MAP_NOTOPEN= -44996,
	ERR_REDIRECT_MAP_PLAYER_IS_FULL= -44995,
};

// RedirectMap		( -45999 ~ -45000 )
enum ERepetion
{
	ERR_REPETION_PLAYERNUMBER_LESS			= -45999,	// Parame1 Need PlayerNumber, Parame2 Now PlayerNumber
	ERR_REPETION_EVERYDAY_TIME_FULL			= -45998,	// Parame1 EventyDay Time Max, Parame2 PlayerID
	ERR_REPETION_NEED_TASK					= -45997,	// Parame1 TaskID, Parame2 PlayerID
	ERR_REPETION_LEVEL_MIN					= -45996,	// Parame1 Map Need Level, Parame2 Player Level	
	ERR_REPETION_TEAM_MEMBER_FAR			= -45995,	// Parame1 PlayerID
	ERR_REPETION_TEAM_MEMBER_UNINSAMESCENE	= -45994, 	// Parame1 UNINSAMESCENE Player CharID
	ERR_REPETION_ISNOT_HEADER				= -45993,	// 不是队长
	ERR_REPETION_DIE						= -45992,	// Parame1 PlayerID
	ERR_REPETION_HOUR						= -45991,	// 时间不匹配
	ERR_REPETION_PVP_WAITLIST_FULL			= -45990,	// 战场等待队列满
	ERR_REPETION_PVP_LEVEL_NOTFOUND			= -45989,	// 等级不足
	ERR_BATTLE_TIMEOUT						= -45988,	// 战场等待进入超时
	ERR_BATTLE_NOT_PERMITED					= -45987,	// 没有被准许进入战场
	ERR_REPETION_CREATE_FULL				= -45986,	// 创建副本失败( 资源满 )
	ERR_REPETION_CREATE_EXIST				= -45985,	// 创建副本失败( 已经存在 )
	ERR_REPETION_CREATE_NOTFOUND			= -45984,
	ERR_REPETION_WEEK_TIME_FULL				= -45983,	// Parame1 Week Time Max, Parame2 PlayerID
	ERR_REPETION_PLAYER_CAMP_FULL			= -45982,	// 阵营人数已满
	ERR_REPETION_PLAYERNUMBER_MORE			= -45981,	// 队伍人数过多 Parame1 Need PlayerNumber Max, Parame2 Now PlayerNumber
	ERR_REPETION_OVERMONEYLMT_SYSMAIL		= -45980,	// 您的金钱已超过携带上限，副本奖励系统已发送邮件。请查收
	ERR_REPETION_LEVEL_MAX					= -45979,	// Parame1 Map Need Level, Parame2 Player Level	

	REPETION_BATTLE_LEAVE_WAITLIST			= -45978,
	
	REPETION_TOWER_FLOOR					= -45977,	// 云霄塔通过全服务通知
	ERR_REPETION_PVP_FULL					= -45976,	// 副本人数已满
	ERR_REPETION_NO_NECESSITY				= -45975,	// 没有进入副本必须品
	ERR_REPETION_SHITU_SHIP					= -45974,	// 错误的师徒关系
	ERR_REPETION_GLOBALWAR_NOTSAMEREPETION  = -45973,	// 战场踢人时不在同一个战场
	ERR_REPETION_GLOBALWAR_NOTSAMECAMP		= -45972,	// 战场踢人时不在同一个阵营
	ERR_REPETION_GLOBALWAR_CAMPKICKON		= -45971,	// 正在投票表决不能再踢人
	ERR_REPETION_KICKCAMPPLAYER_NOTINBATTLE	= -45970,	// 不在副本中不能踢人
	ERR_REPETION_KICKCAMPPLAYER_NOTONLINE	= -45969,	// 被踢的人不在线
	ERR_REPETION_KICKCAMPPLAYER_VOTED		= -45968,	// 已经投过票
	ERR_REPETION_REPETIONDESTORIED			= -45967,	// 进入副本时副本已经关闭
};

// mail				( -46999 ~ - 46000  )
enum  EMailCode
{	
	ERR_MAIL_GETACCESSORY_NOPACKAGESPACE = -46999,			// 获取邮件附件时包裹空间不足
	ERR_MAIL_MONEYOUT					 = -46998,			// 获取金钱时金钱溢出
	ERR_MAIL_GETACCESSORY_SUCCESS		 = -46997,			// 获取附件成功
	ERR_MAIL_GETMONEY_SUCESS			 = -46996,			// 获取附件金钱成功
	ERR_MAIL_TIMEPROTECTED				 = -46995,			// 安全保护期内点击‘发送’按钮、手动删除邮件
	ERR_MAIL_PRODUCE_OPENMAILBOX		 = -46994,			// 生产过程中打开邮箱
	ERR_MAIL_ADDACCESSORY_BINDED		 = -46993,			// 企图把绑定物品放入附件列表
	ERR_MAIL_ADDACCESSORY_TASKITEM		 = -46992,			// 企图把任务物品放入附件列表
	ERR_MAIL_SENDMAIL_LISTFULL			 = -46991,			// 添加附件时附件列表已经满了
	ERR_MAIL_SENDMAIL_EMPTYNAME			 = -46990,			// 邮件接收人名字为空
	ERR_MAIL_SENDMAIL_EMPTYTITLE		 = -46989,			// 发送邮件时没有邮件主题
	ERR_MAIL_SENDMAIL_NOTINSELFCONTRY	 = -46988,			// 发送邮件时不在自己国家
	ERR_MAIL_SENDMAIL_SELFSEND			 = -46987,			// 发送邮件时邮件接收方为自己
	ERR_MAIL_SENDMAIL_MONEYNOTENOUGH	 = -46986,			// 发送邮件时邮资不足
	ERR_MAIL_SENDMAIL_CONFIRM			 = -46985,			// 各项条件满足时，点击‘发送’按钮
	ERR_MAIL_SENDMAIL_DESNOTEXIST		 = -46984,			// 发送邮件时邮件接收人不存在
	ERR_MAIL_SENDMAIL_DESMAILFULL		 = -46983,			// 发送邮件时对方邮箱满
	ERR_MAIL_SENDMAIL_DESOINOTHERCOUNTRY = -46982,			// 发送邮件时邮件接收方在其他国家
	ERR_MAIL_SENDMAIL_INBLACKLIST		 = -46981,			// 发送邮件时在对方的屏蔽列表中
	ERR_MAIL_SENDMAIL_SUCESS			 = -46980,			// 发送邮件成功
	ERR_MAIL_DELMAIL_EMPTYOPTION		 = -46975,			// 删除邮件时没有选中项
	ERR_MAIL_LOCKMAIL_MONEYNEEDTOPAY	 = -46997,			// 锁定尚未付费的付费邮件，操作失败	
	ERR_MAIL_LOCKMAIL_SUCCESS			 = -46973,			// 锁定邮件成功
	ERR_MAIL_UNLOCKMAIL_SUCCESS			 = -46972,			// 解锁邮件成功
	ERR_MAIL_LOCKMAIL_FULL				 = -46971,			// 锁定邮件时已经到达上限
	ERR_MAIL_DELMAIL_LOCKED				 = -46970,			// 锁定的邮件不能被删除
	ERR_MAIL_DELMAIL_NEEDTOPAY			 = -46969,			// 不能删除还没有付费的邮件
	ERR_MAIL_DELMAIL_HASACCESSORY		 = -46968,			// 含有附件的邮件不能被删除
	ERR_MAIL_DELMAIL_NEWTEXT			 = -46967,			// 手动删除新的纯文本邮件
	ERR_MAIL_DELALLMAIL_HASNOTEXT		 = -46966,			// 删除全部邮件时没有纯文本邮件
	ERR_MAIL_DELALLMAIL_NEWTEXT			 = -46965,			// 删除全部邮件时还有没有删除的纯文本邮件	
	ERR_MAIL_DELALLMAIL_CONFIRM			 = -46964,			// 删除全部邮件时的提示
	ERR_MAIL_DELMAIL_SUCESS				 = -46963,			// 删除邮件成功
	ERR_MAIL_MAILBOX_FULL				 = -46962,			// 邮箱已经满了的提示消息
	ERR_MAIL_GEATACEESSORY_NOOPTION		 = -46961,			// 提取附件时没有			
	ERR_MAIL_BACKMAIL_SUCESS			 = -46960,			// 邮件退信成功
	ERR_MAIL_BACKMAIL_BACKED			 = -46959,			// 不能退回被退回的邮件	
	ERR_MAIL_BACKMAIL_TEXT				 = -46958,			// 纯文本邮件不能被退回
	ERR_MAIL_MONEYNEEDMAIL_HASNOITEM	 = -46957,			// 付费邮件没有放置物品
	ERR_MAIL_MONEYNEEDMAIL_MONEYNOTSET	 = -46956,			// 付费邮件没有填要对方支付的金钱
	ERR_MAIL_MONEYNEEDMAIL_SHOWMONEY	 = -46955,			// 付费邮件提示要被支付的金钱
	ERR_MAIL_MONEYNEEDMAIL_MONEYNOTENOUGH= -46954,			// 提取付费邮件附件时金钱不足	
	ERR_MAIL_SENDMAIL_SENDNOTALLOWED	 = -46952,			// 不允许发送邮件
	
	ERR_MAIL_PAYMAIMONEY_NONEED			 = -46951,			// 普通邮件不需要付费
	ERR_MAIL_PAYMAIMONEY_MONEYNOTENOUGH	 = -46950,			// 金钱不足
	ERR_MAIL_GETACCESSORY_MONEYNOTPAY	 = -46949,			// 获取附件时付费邮件还没有支付金钱
	ERR_MAIL_PAYMONEYFORMAIL_INVALIDE	 = -46948,			// 为付费邮件付费时,邮件已经失效
	ERR_MAIL_BACKMONEY_OVER				 = -46947,			// 加上返回的钱，超过你的携带上限
	ERR_MAIL_GETACCESSORY_EXPIRED		 = -46946,			// 加上返回的钱，超过你的携带上限
};

// judge ( -47499 ~ -47000 )
enum EJudge
{
	ERR_JUDGE_PLAYERNOTEXIST			= -47499,			// 玩家不存在
	ERR_JUDGE_DESOFFLINE				= -47498,			// 对方不在线
	ERR_JUDGE_NEEDSCORE					= -47497,			// 添加评价时，分值不能为零
	ERR_JUDGE_FAILED					= -47496,			// 评价失败
	ERR_JUDGE_SCORENOTENOUGH			= -47495,			// 可支配点数不够
	ERR_JUDGE_NUMFULL					= -47494,			// 评价次数达到上限
	ERR_JUDGE_COOLTIME					= -47493,			// 评价冷却中
	ERR_JUDGE_LEVELLOWER				= -47492,			// 玩家级别不够
	ERR_JUDGE_DESLEVELLOWER				= -47491,			// 对方玩家级别不够
	ERR_JUDGE_MODIFYNONE				= -47490,			// 没有可以修改的玩家

	ERR_ACHIEVE_MAX						= -47489,			// 已经领到最高
	ERR_ACHIEVE_NO						= -47488,			// 点数不足
	ERR_ACHIEVE_FULL					= -47487,			// 包裹已满
};

// activity ( -47999 ~ -47500 )
enum EActitity
{
	ERR_ACTIVITY_INANSWERSTATUS			= -47999,			// 已经处于答题状态，不能重复答题
	ERR_ACTIVITY_HASANSWERED			= -47998,			// 已经领取过每日福利的答题
	ERR_ACTIVITY_LEVELLOW				= -47997,			// 领取福利的等级不够	
	ERR_ACTIVITY_NOSPACE				= -47996,			// 空间不够无法领取npc奖励
	ERR_HYDRANGEA_GRABED				= -47995,			// 已经抢过绣球
	ERR_HYDRANGEA_GRABERFULL			= -47994,			// 抢绣球的人员已满	
	ERR_HYDRANGEA_ACTIVITYEND			= -47993,			// 抢绣球时活动已经结束	
	ERR_THROWHYDRANGEA_ACTIVITYEND		= -47992,			// 抛绣球时活动已经结束
	ERR_THROWHYDRANGEA_NOTALLOWED		= -47991,			// 当前状态不允许抛绣球
	ERR_THROWHYDRANGEA_NOTHOLDER		= -47990,			// 抛绣球时不是绣球持有者
	ERR_THROWHYDRANGEA_FAILED			= -47989,			// 抛绣球时失败继续持有绣球
	ERR_THROWHYDRANGEA_FARDISTANCE		= -47988,			// 超出范围不能抛绣球
	ERR_MONEYOVER_SYSMAIL				= -47987,			// 每日福利给的钱超过金钱携带上限，已发系统邮件，请查收
	ERR_NINETAILFOX_DEATH				= -47986,			// 九尾线狐死时的错误码
	ERR_PRESTIGEMARSHAL_DEATH			= -47985,			// 威风元帅死时的错误码
	ERR_GRASSJELLYEND					= -47984,			// 仙草活动正常结束的错误码
};

// redstone ( -48999 ~ -48000 )
enum ERedstone
{
	ERR_REDSTONE_PROTECTED				= -48999,			// 内丹处于保护状态
	ERR_REDSTONE_MAGIC					= -48988,			// 已经处于噬炼状态	
	ERR_REDSTONE_NOTINTEAM				= -48987,			// 点击内丹时玩家没有队伍
	ERR_REDSTONE_PROCESSTIMER			= -48986,			// 内丹正在被别人开启
	ERR_REDSTONE_SUCESS					= -48985,			// 内丹被成功开启
	ERR_REDSTONE_STARTED				= -48984,			// 内丹已经被开启	
	ERR_REDSTONE_REDSTONEEFFECT			= -48983,			// 每个队伍只能开启一个内丹
};

// password( -49999 ~ - 49000 )
enum EPassword
{
   ERR_PASSWORD_WRONGPASSWORD			= -49999,			// 错误的密码
   ERR_PASSWORD_PASSWORDSETED			= -49998,			// 设置密码时密码已经设置
   ERR_PASSWORD_SETPASSWORDWRONG		= -49997,			// 设置密码错误
   ERR_PASSWORD_SETPASSWORDINPROTECTIME	= -49996,			// 安全时间内不允许修改或者设置密码
   ERR_PASSWORD_WRONGPROTECTTIME		= -49995,			// 非法的安全保护时间
   ERR_PASSWORD_INPROTECTTIME			= -49994,			// 财产保护保护期间不允许该操作
   ERR_PASSWORD_ACTIONNOTALLOWED		= -49993,			// 密码保护期间不允许该操作	   
   ERR_PASSWORD_NOTFREE					= -49992,			// 安全密码保护没有处于解锁状态，不用重新锁定
   ERR_PASSWORD_DESINPROTECTSTATUS		= -49991,			// 交易时对方处于密码保护状态
}; 

// storage( -50999 ~ -50000 ) 
enum EStorage
{
	ERR_STORAGE_STORMONEYFULL			 = -50999,			// 存储金钱时金钱已满	
	ERR_STORAGE_HASNOSOMUCHMONEY		 = -50998,			// 取金钱时仓库没有那么多金钱 
	ERR_STORAGE_BAGMONEYFUULL			 = -50997,			// 从仓库取钱时包裹金钱满
	ERR_KITBAG_CANNOTSWAP				 = -50996,			// 不能用背包替换当前仓库背包
	ERR_KITBAG_CANNOTADD				 = -50995,			// 背包不能被放置到仓库背包栏
	ERR_KITBAG_TAKEKITBAG_NOTEMPTY		 = -50994,			// 取下背包时背包不为空
	ERR_KITBAG_ADDKITBAG_NOTEMPTY		 = -50993,			// 放置背包时目标栏位不为空
	ERR_KITBAGNEEDLE_NOTNEEDLEITEM		 = -50992,			// 放置的物品不是绣花针	
	ERR_KITBAGNEEDLE_NOTKITBAG			 = -50991,			// 被修里的不是背包		
	ERR_KITBAGNEEDLE_NOTSUITABLENEEDLE   = -50990,			// 绣花针能修理的背包格子数小于当前要修理的背包
	ERR_KITBAGNEEDLE_KITBAGNOTEXPIRED	 = -50989,			// 背包还未过期，不用修理 
	ERR_STORAGE_MONEY_UPLMT				 = -50988,			// 仓库金钱达到上限 不能进行该操作
	ERR_BAG_MONEY_UPLMT					 = -50987,			// 包裹金钱达到上限 不能进行该操作
	ERR_STORAGE_BIND_MONEY_UPLMT		 = -50986,			// 仓库绑定金钱达到上限 不能进行该操作
	ERR_BAG_BIND_MONEY_UPLMT			 = -50985,			// 包裹绑定金钱达到上限 不能进行该操作
	ERR_FASHIONNEEDLE_WRONGCOLOR		 = -50984,			// 时装绣花针不能对这种颜色的时装使用
	ERR_FASHIONNEEDLE_NOTFASHION		 = -50983,			// 时装绣花针不能对非时装使用
	ERR_FASHIONNEEDLE_FASHIONNOTEXPIRED  = -50982,			// 时装没有过期，不用使用绣花针修复
};

//	family( -51999 ~ -51000 )
enum EFamily
{
	ERROR_FAMILY_CREATFAMILY_HASFAMILY 							=	-51999	,				// 创建家族时已经有家族
	ERROR_FAMILY_CREATFAMILY_LEVELLOW  							=	-51998	,				// 没到30级时，创建家族
	ERROR_FAMILY_CREATFAMILY_HASDIRTYWORD 							=	-51997	,			// 创建家族时家族名称含有脏词
	ERROR_FAMILY_CREATFAMILY_NAMEEXIST							=	-51996	,				// 创建家族时家族名称已经存在
	ERROR_FAMILY_CREATFAMILY_MONEYNOTENONGH							=	-51995	,			// 创建家族时金钱不够
	ERROR_FAMILY_INVITE_INDIFFNATION   							=	-51994	,				// 邀请加入家族时和对方国籍不同 
	ERROR_FAMILY_INVITE_INFAMILY	   						=	-51993	,					// 对方已经加入了家族，邀请失败
	ERROR_FAMILY_INVITE_DESLOWLEVEL	   						=	-51992	,					// 对方等级未满30级，邀请失败
	ERROR_FAMILY_INVITE_HASNORIGHT	   						=	-51991	,					// 邀请加入家族时没有权利	::TODO
	ERROR_FAMILY_INVITE_FAMILYFULL	   						=	-51990	,					// 发送邀请信息时家族成员已满 ::TODO
	ERROR_FAMILY_REPLYINVITE_FAMILYFULL							=	-51989	,				// 同意加入家族时对方家族成员已经满了  ::TODO
	ERROR_FAMILY_APPLYLIST_FAMILYFULL  							=	-51988	,				// 处理申请列表时家族成员已满 	::TODO	
	ERROR_FAMILY_REPLYAPPLY_INFAMILY							=	-51987	,				// 同意玩家加入家族时已经在家族中
	ERROR_FAMILY_REPLYAPPLY_INOHERFAMILY							=	-51986	,			// 同意玩家加入家族时对方已经在其他家族中
	ERROR_FAMILY_REPLYAPPLY_FAMILYFULL							=	-51985	,				// 同意加入家族时家族人数已满
	ERROR_FAMILY_REPLYINVITE_INFAMILY							=	-51984	,				// 同意加入家族时已经有家族
	ERROR_FAMILY_APPLY_INFAMILY		   					=	-51983	,						// 有家族时申请加入家族	
	ERROR_FAMILY_APPLY_FAMILYNOTEXIST  							=	-51982	,				// 输入的家族名不存在
	ERROR_FAMILY_APPLY_SUCESS          							=	-51981	,				// 输入的家族名存在  申请成功，请等待家族管理员审核
	ERROR_FAMILY_CHANGNOTICE_DIRTYWORD 							=	-51980	,				// 改变家族公告时有信息含有脏词
	ERROR_FAMILY_LEAVEFAMILY_ISHEADER  							=	-51979	,				// 族长不能离开家族		
	ERROR_FAMILY_HASNORIGHT							=	-51978	,							// 没有权限
	ERROR_FAMILY_CHANGEROOTER_NOTINFAMILY							=	-51977	,			// 转让宗正职位时对方不在本家族中 
	ERROR_FAMILY_KICKPLAYER_KICKSELF							=	-51976	,				// 族长不能踢出自己
	ERROR_FAMILY_POSTAPPOINT_POSTSELF							=	-51975	,				// 不能给自己任职，
	ERROR_FAMILY_REPLYINVITE_FAMILYNOTEXIST							=	-51974	,			// 加入家族时，家族不存在	 	
	ERROR_FAMILY_APPLY_LEVELLOW			 				=	-51973	,						// 没到30级时，申请加入家族 
	ERROR_FAMILY_INVITE_OFFLINE			 				=	-51972	,						// 邀请加入家族时对方不在线	 
	ERROR_FAMILY_INVITE_INVITELISTFULL	 						=	-51971	,				// 被邀请的玩家邀请列表满
	ERROR_FAMILY_APPLY_APPLYLISTFULL							=	-51970	,				// 达到30级没入家族，输入的家族名存在，但申请人数已
	ERROR_FAMILY_APPLY_INAPPLYTIMELIMIT							=	-51969	,				// 达到30级没入家族，输入的家族名存在，但24小时内已
	ERROR_FAMILY_APPLY_INFAMILYLIMIT							=	-51968	,				// 离开家族24小时后不能加入家族
	ERROR_FAMILY_APPOINT_APPOINTSELF							=	-51967	,				// 转让职位时目标为族长本身
	ERROR_FAMILY_APPLY_NOTINSAMECOUNTRY							=	-51966	,				// 达到30及没入家族，输入的家族名为他国家族
	ERROR_FAMILY_LEAVE_INCORPS							=	-51965	,						// 在军团中的家族不能解散
	ERROR_FAMILY_KICK_NOTINFAMILY							=	-51964	,					// 踢人时玩家不在家族中
	ERROR_FAMILY_CHANGHEADER_CORPSHEADER							=	-51963	,			// 军团首领不能转让族长职位
	ERROR_FAMILY_DISBAND_DISBANDSTATUS							=	-51962	,				//  解散家族时已经处于解散状态
	ERROR_FAMILY_DISBAND_HASNOFAMILY							=	-51961	,				// 解散家族时没有家族
	ERROR_FAMILY_CANCELDISBAND_NOTNEED							=	-51960	,				// 取消家族解散时家族不处于解散状态
	ERROR_FAMILY_REPLYINVITE_INDISBANDCOOLTIME							=	-51959	,		// 答复邀请时，对方离开家族的冷却时间未到
	ERROR_FAMILY_STRINGTOOLONG							=	-51958	,						// 字符串过长	
	ERROR_FAMILY_INVITE_INVITED							=	-51957	,						// 已经发送过邀请信息  42
	ERROR_FAMILY_APPLY_APPLIED							=	-51956	,						// 已经申请过加入家族
	ERROR_FAMILY_INVITE_SELFINVITE							=	-51955	,					// 不能邀请自己
	ERROR_FAMILY_REPLYAPPLY_TIMEOUT							=	-51954	,					// 答复申请时申请已经失效
	ERROR_FAMILY_CREATFAMILY_INDISBANDCOOLTIME							=	-51953	,		// 解散家族后家族成员24小时之内不能创建家族
	ERROR_FAMILY_REPLYAPPLY_ROLENOTEXIST							=	-51952	,			// 答复玩家申请时因为被删除或者其他原因角色不存在
	ERROR_FAMILY_APPOINTHEADER_MEMBEROFFLINE							=	-51951	,		// 给家族成员转让职位时成员不在线
	ERROR_FAMILY_APPOINTHEADER_MEMBERLEVELLOW							=	-51950	,		// 给家族成员转让职位时成员等级不够
	ERROR_FAMILY_LEVELUP_LEVELIMIT							=	-51949	,					// 升级家族时已经到达等级上限
	ERROR_FAMILY_LEVELUP_GLORYNOTENOUGH							=	-51948	,				// 升级家族时家族荣耀值不够	
	ERROR_FAMILY_CONSUMEMONEY_MONEYNOTENOUGH							=	-51947	,		// 升级家族时家族荣耀值不够	
	ERROR_FAMILY_SETPOST_POSTFULL							=	-51946	,					// 设置职位时职位已经满了	
	ERROR_FAMILY_BIDNPC_NOTHEADER							=	-51945	,					// 报名争夺时，不是族长
	ERROR_FAMILY_BIDNPC_BIDFAIL							=	-51944	,						// 报名争夺时，操作失败
	ERROR_FAMILY_BIDNPC_HASOWN							=	-51943	,						// 报名争夺时，已经占领过
	ERROR_FAMILY_BIDNPC_SAME							=	-51942	,						// 报名争夺时，重复报名
	ERROR_FAMILY_BIDNPC_NOTHIGHEST							=	-51941	,					// 报名争夺时，报价低
	ERROR_FAMILY_BIDNPC_BELOWOLD							=	-51940	,					// 报名争夺时，报价低于旧的
	ERROR_FAMILY_ABORTNPC_NOTHEADER							=	-51939	,					// 放弃NPC时，不是族长
	ERROR_FAMILY_ABORTNPC_NOTOWN							=	-51938	,					// 放弃NPC时，没有拥有
	ERROR_FAMILY_BIDNPC_WRONGTIME							=	-51937	,					// 报名争夺时，时间不正确
	ERROR_FAMILY_SUBMIT_WRONGTIME							=	-51936	,					// 报名成员时，时间不正确
	ERROR_FAMILY_CONTRIBUTEMONEY_NOTENOUGH							=	-51935	,			// 捐献家族金钱时金钱不足		
	ERROR_FAMILY_MAKEROOM_EXIST							=	-51934	,						// 创建房间时,已经存在
	ERROR_FAMILY_CHAROOM_NOTEXIST							=	-51933	,					// 挑战房间时,不存在
	ERROR_FAMILY_CHAROOM_NOTHEADER							=	-51932	,					// 挑战房间时,不是族长
	ERROR_FAMILY_AGREE_NOTHEADER							=	-51931	,					// 同意挑战时,不是族长
	ERROR_FAMILY_AGREE_UNKNOWN							=	-51930	,						// 同意挑战时,未知错误
	ERROR_FAMILY_QUERY_NOTHEADER							=	-51929	,					// 查询时,不是族长
	ERROR_FAMILY_QUERY_NOTEXIST							=	-51928	,						// 查询时,不存在房间
	ERROR_FAMILY_ABORT_NOTEXIST							=	-51927	,						// 取消时,不存在房间
	ERROR_FAMILY_ABORT_NOTHEADER							=	-51926	,					// 取消时,不是族长
	ERROR_FAMILY_BIDNPC_NOMONEY							=	-51925	,						// 报名争夺时，钱不足
	ERROR_FAMILY_CHAROOM_SAME							=	-51924	,						// 挑战房间时,家族相同
	ERROR_FAMILY_CHAROOM_EXIST							=	-51923	,						// 挑战房间时,已经挑战过
	ERROR_FAMILY_ABORTROOM_BATTLE							=	-51922	,					// 撤销房间时，在战斗中
	ERROR_FAMILY_CHAROOM_BATTLE							=	-51921	,						// 挑战房间时,在战斗中
	ERROR_FAMILY_BIDNPC_DISBAND							=	-51920	,						// 解散状态不能报名争夺
	ERROR_FAMILY_MAKEROOM_DISBAND							=	-51919	,					// 解散状态不能创建房间
	ERROR_FAMILY_CHAROOM_DISBAND							=	-51918	,					// 解散状态不能挑战房间
	ERROR_FAMILY_MAKEROOM_NOTHEADER							=	-51917	,					// 创建房间时,不是族长
	ERROR_FAMILY_AGREE_DISBAND							=	-51916	,						// 同意挑战时,一方处于解散状态	
	ERROR_FAMILY_NOTALLOWDONNPCFIGHT							=	-51915	,				//	处于npc争夺状态不能解散家族
	ERROR_FAMILY_LEAVEFAMILY_ONNPCFIGHT							=	-51914	,				//	处于npc争夺状态不能离开家族
	ERROR_FAMILY_KICKPLAYER_ONNPCFIGHT							=	-51913	,				//	处于npc争夺状态不能踢人	
	ERROR_FAMILY_GETNPCWELFARE_HASGETED							=	-51912	,				//  已经领取过家族npc福利
	ERROR_FAMILY_USEEXPCARD_HASNOITEM							=	-51911	,				// 使用经验令牌时没有该道具
	ERROR_FAMILY_GETEXPCARD_HASITEM							=	-51910	,					// 领取经验令牌时该道具已经存在
	ERROR_FAMILY_GETEXPCARD_GETED							=	-51909	,					// 一天之内第二次领取经验令牌
	ERROR_FAMILY_GETEXPCARD_NOTHEADER							=	-51908	,				// 不是族长不能领取经验令牌
	ERROR_FAMILY_LEVELUP_MONEYNOENOUGH							=	-51907	,				// 家族升级时金钱不够	   
	ERROR_FAMILY_CONTRIBUTE_MONEYLIMIT							=	-51906	,				// 捐钱时家族金钱已达上限
	ERROR_FAMILY_CONTRIBUTE_MONEYINVAL							=	-51905	,				// 捐钱时所捐数目无效
	ERROR_FAMILY_GETMONEY_MONEYINVALIDE							=	-51904	,				// 获取家族金钱时金钱数目无效
	ERROR_FAMILY_MONEYINVALIDE							=	-51903	,						// 金钱数目无效	
	ERROR_FAMILY_REDPAPER_CONTRIBUTENOTENOUGH							=	-51902	,		// 家族兑换时个人贡献度不够	
	ERROR_FAMILY_REDPAPER_SPACENOTENOUGH							=	-51901	,			// 购买红包时包裹空间不足
	ERROR_FAMILY_REDPAPER_MONEYNOTENOUGH							=	-51900	,			// 购买红包时家族金钱不够  -51900
	ERROR_FAMILY_EXCHANGE_HASONFAMILY							=	-51899	,				// 家族兑换时没有家族
	ERROR_FAMILY_BUYFAMILYITE_HASONFAMILY							=	-51898	,			// 购买红包时没有家族
	ERROR_FAMILY_FAMILYBOSS_REPETIONNOTEXIST							=	-51897	,		// 无法进入家族副本，副本已经关闭
	ERROR_FAMILY_FAMILYBOSS_HASNOFAMILY							=	-51896	,				// 没有家族不能进入家族副本
	ERROR_FAMILY_FAMILYBOSS_NOTINOPENTIME							=	-51895	,			// 进入家族副本时时间不对	
	ERROR_GROUPNAME_SERVICE_NOTOPEN							=	-51894	,					// 家族名称服务不可用, 请稍候重试
	ERROR_FAMILY_FAMILYBOSS_REPETIONHASOPENED							=	-51893	,		// 当天已经开启过家族副本
	ERROR_FAMILY_FAMILYBOSS_MONEYNOTENOUGH							=	-51892	,			// 开启家族副本时金钱不够
	ERROR_FAMILY_FAMILYBOSS_GLORYNOTENOUGH							=	-51891	,			// 开启家族副本时荣耀不够	
	ERROR_FAMILY_FAMILYGOODS_HASNOWARNPC							=	-51890	,			// 没有占领npc不能在特产商店买物品
	ERROR_FAMILY_USELEAGUEBAG_HASNOFAMILY							=	-51889	,			// 没有家族不能使用家族特供物品	
	ERROR_FAMILY_FAMILYGOODS_ITEMNOTENGOUGH							=	-51888	,			// 家族物品数量不够
	ERROR_FAMILY_FAMILYGOODS_CANOTBUY							=	-51887	,				// 不能购买该家族商品
	ERROR_FAMILY_EXCHANGE_NOTALLOWED							=	-51886	,				// 不能进行家族兑换
	ERROR_FAMILY_EXCHANGE_ITEMNOTENOUGH 							=	-51885	,			//  家族兑换物品不足
	ERROR_FAMILY_FAMILYGOODS_HASNORIGHT							=	-51884	,				//  购买家族商店物品时权限不够	

};	

//	corps( -52999 ~ -52000 )
enum ECorps
{
	ERROR_CORPS_CREAT_HASDIRTYWORD							=	-52999	,				// 创建军团时含有脏词
	ERROR_CORPS_CREAT_ISNOTFAMILYHEADER							=	-52998	,			// 创建军团时不是家族族长
	ERROR_CORPS_CREAT_LEVELNOTENOUGH							=	-52997	,			// 创建军团时宗正等级不够
	ERROR_CORPS_CREAT_HASNOFAMILY							=	-52996	,				// 创建军团时没有家族
	ERROR_CORPS_CREAT_INCORPS							=	-52995	,					// 创建军团时已经在军团中
	ERROR_CORPS_CREAT_MONEYNOTENOUGH							=	-52994	,			// 创建军团时金钱不够
	ERROR_CORPS_CREAT_NAMEEXIST							=	-52993	,					// 创建军团时名称已经存在
	ERROR_CORPS_APPLY_HASNOFAMILY							=	-52992	,				// 申请加入军团时，玩家不在家族中
	ERROR_CORPS_APPLY_HASNORIGHT							=	-52991	,				// 申请加入军团时，玩家不是族长，没有权限
	ERROR_CORPS_APPLY_NOTINSAMECOUNTRY							=	-52990	,			// 申请加入军团时，家族和军团不在同一个国家0
	ERROR_CORPS_APPLY_CORPSMEMBERFULL							=	-52989	,			// 申请加入军团时，军团成员满
	ERROR_CORPS_APPLY_APPLYLISTFULL							=	-52988	,				// 申请加入军团时，申请列表满
	ERROR_CORPS_APPLY_HASINCORPS							=	-52987	,				// 家族已经在军团中，不能再加入军团
	ERROR_CORPS_APPLY_NOTEXIST							=	-52986	,					// 申请加入军团时，军团不存在
	ERROR_CORPS_APPLY_INCORPS							=	-52985	,					// 申请加入军团时已经在军团中
	ERROR_CORPS_APPOINT_NOTINCORPS							=	-52984	,				// 转让职位时玩家不再本军团中
	ERROR_CORPS_HASNORIGHT							=	-52983	,						// 权限不够
	ERROR_CORPS_APPOINT_NOTFAMILYHEADER							=	-52982	,			// 任职时对方不是家族族长
	ERROR_CORPS_DISBAND_NOTHEADER							=	-52981	,				// 解散军团时不是军团首领
	ERROR_CORPS_DISBAND_NUMLIMIT							=	-52980	,				// 超过500人的军团不能解散0
	ERROR_CORPS_INVITE_FAMILYNOTEXIST							=	-52979	,			// 邀请家族加入军团时家族不存在		20
	ERROR_CORPS_INVITE_NOTINCORPS							=	-52978	,				// 邀请家族加入军团时军团不存在
	ERROR_CORPS_INVITE_INVITELISTFULL							=	-52977	,			// 邀请家族加入军团时家族申请列表满
	ERROR_CORPS_LEAVE_ISCORPSHEADER							=	-52976	,				// 大将军不能离开军团	
	ERROR_CORPS_REPLYAPPLY_INCORPS							=	-52975	,				// 大将军答复家族申请时家族已经加入军团
	ERROR_CORPS_REPLYAPPLY_MEMBERFULL							=	-52974	,			// 答复家族申请时军团成员已满
	ERROR_CORPS_REPLYAPPLY_FAMILYEXIST							=	-52973	,			// 答复家族申请时家族不存在
	ERROR_CORPS_REPLYAPPLY_NOTINCORPS							=	-52972	,			// 答复家族申请时军团已经不存在
	ERROR_CORPS_REPLYINVITE_INCORPS							=	-52971	,				// 答复军团邀请时家族已经在军团中
	ERROR_CORPS_REPLYINVITE_CORPSMEBERFULL							=	-52970	,		// 答复军团邀请时军团成员已满0
	ERROR_CORPS_REPLYINVITE_NOTEXIST							=	-52969	,			// 答复军团邀请时军团不存在
	ERROR_CORPS_APPOINTHEADER_SELFAPPOINT							=	-52968	,		// 不能给自己转让职位
	ERROR_CORPS_KICKFAMILY_SELFFAMILY							=	-52967	,			// 不能踢出自己的家族
	ERROR_CORPS_APPLY_ONDISBANDSTATUS							=	-52966	,			// 家族解散期间不能加入军团
	ERROR_CORPS_INVITE_ONDISBANDSTATUS							=	-52965	,			// 不能邀请处于解散状态的家族加入军团
	ERROR_CORPS_INVITE_INCORPS							=	-52964	,					// 邀请家族加入时家族已经在军团中
	ERROR_CORPS_DISBAND_DISBANDSTATUS							=	-52963	,			// 解散军团时已经处于解散状态
	ERROR_CORPS_CANCLEDISBAND_NOTNEED							=	-52962	,			// 取消解散军团时军团没有处于解散状态
	ERROR_CORPS_APPLY_INCOOLTIME							=	-52961	,				// 申请加入军团时处于冷却时间
	ERROR_CORPS_JOINCORPS_INLEAVECOOLTIME							=	-52960	,		// 加入军团时处于冷却时间 0
	ERROR_CORPS_STRINGTOOLONG							=	-52959	,					// 字符串过长	  40
	ERROR_CORPS_APPLY_INAPPLYLIST							=	-52958	,				// 申请加入军团时，家族的申请已经在申请列表里面
	ERROR_CORPS_MEMBERFULL							=	-52957	,						// 家族成员满
	ERROR_CORPS_INVITE_NOTINOTHERCOUNTRY							=	-52956	,		// 邀请时和对方家族不在同一个国家
	ERROR_CORPS_REPLYINVITE_ONDISBANDSTATUS							=	-52955	,		// 处于解散状态的家族不能加入军团
	ERROR_CORPS_CREATCORPS_INDISBANDCOOLTIME							=	-52954	,	// 解散军团24小时内同一个家族不能再创建军团
	ERROR_CORPS_CREAT_ONDISBANDSTATUS							=	-52953	,			// 家族处于解散状态时不能创建军团
	ERROR_CORPS_LEVELUP_LEVELIMIT							=	-52952	,				// 军团已经不能再进行升级
	ERROR_CORPS_LEVELUP_GLORYNOTENOUGH							=	-52951	,			// 军团升级时荣耀值不够
	ERROR_CORPS_SETPOST_POSTFULL							=	-52950	,				// 设置军团职位时，职位已经满了
	ERROR_CORPS_CONSUMEMONEY_MONEYNOTENGOUGH							=	-52949	,	// 消耗军团金钱时金钱不足
	ERROR_CORPS_HASBID							=	-52948	,							// 已经参与过竞价
	ERROR_CORPS_BIDTIMENOTOPEN							=	-52947	,					// 不是竞价开放时间
	ERROR_CORPS_MONEYNOTENOUGH							=	-52946	,					// 军团金钱不够
	ERROR_CORPS_BIDMONEYLOWER							=	-52945	,					// 竞价金钱太低
	ERROR_CORPS_BIDSUCCESS							=	-52944	,						// 竞价报名成功
	ERROR_CORPS_NOTROOTER							=	-52943	,						// 不是军团长
	ERROR_CORPS_NOTEXIST							=	-52942	,						// 军团不存在
	ERROR_CORPS_CANTBATTLE							=	-52941	,						// 没有参战资格
	ERROR_CORPS_SELECTBATTLE_SUCCESS							=	-52940	,			// 任命参战成功
	ERROR_CORPS_FAMILYNOTIN							=	-52939	,						// 有家族不属于军团
	ERROR_CORPS_NOTINFAMILY							=	-52938	,						// 有成员不属于家族		
	ERROR_CORPS_FAMILYMONEYNOTENOUGH							=	-52937	,			// 家族给军团贡献金钱时军团金钱不够	
	ERROR_CORPS_HASHOLDCITY							=	-52936	,						// 已经占领有城池
	ERROR_CORPS_MEMBEROVERFLOW							=	-52935	,					// 参战人数过多 -52935
	ERROR_CORPS_INBATTLE							=	-52934	,						// 战斗中
	ERROR_CORPS_ROOMEXIST							=	-52933	,						// 已报名
	ERROR_CORPS_ROOMNOTFOUND							=	-52932	,					// 没有报名
	ERROR_CORPS_YOURSELF							=	-52931	,						// 不能挑战自己
	ERROR_CORPS_BIDINUNION							=	-52930	,						// 不能对同盟军团竞价
	ERROR_CORPS_NPCWELFAREOBTAINED							=	-52929	,				// 福利已经领取过	
	ERROR_CORPS_HASNOCORPSTOKEN							=	-52928	,					// 创建军团时没有扩军令
	ERROR_CORPS_LEVELUP_MONEYNOTENOUGH							=	-52927	,			// 军团升级时金钱不足
	ERROR_CORPS_MONEYLIMIT							=	-52926	,						// 金钱已经达到上限
	ERROR_CORPS_MONEYINVALIDE							=	-52925	,					// 金钱数目无效	
	ERROR_CORPS_INSUBMITTING							=	-52924	,					// 报名中，无法竞价
	ERROR_CORPS_CORPSCANNOTBEDISBANDED							=	-52923	,			// 处于竞价,战争和战斗状态的军团不能被解散
	ERROR_CORPS_CANOTLEAVECORPS							=	-52922	,					// 处于竞价,战争和战斗状态时家族不能离开军团
	ERROR_CORPS_CANOTKICKFAMILY							=	-52921	,					// 处于竞价,战争和战斗状态时家族不能踢出家族	
	ERROR_CORPS_BUYREDPAPER_CONTRIBUTENOTENOUGH							=	-52920	,	// 军团兑换时个人贡献度不够		
	ERROR_CORPS_BUYREDPAPER_MONEYNOTENOUGH							=	-52919	,		// 购买红包时家族金钱不足
	ERROR_CORPS_BUYREDPAPER_HASNOSPACE							=	-52918	,			// 购买红包时包裹空间不足
	ERROR_CORPS_EXCHANGE_HASNOCORPS							=	-52917	,				// 军团兑换时没有军团	-52919	
	ERROR_CORPS_BUYFAMILYITE_HASONCORPS							=	-52916	,			// 购买红包时没有军团
	ERROR_UNION_REACHLIMIT							=	-52915	,						// 已达联军数上限
	ERROR_UNION_NOHEADER							=	-52914	,						// 创建时没有首领
	ERROR_UNION_APPLY_NOCORPS							=	-52913	,					// 申请联军时没有军团
	ERROR_UNION_REPLYAPPLY_NOTINCORPS							=	-52912	,			// 回应联军时没有军团
	ERROR_UNION_INVITE_NOTINCORPS							=	-52911	,				// 邀请联军时没有军团 
	ERROR_UNION_APPLY_NOTEXIST							=	-52910	,					// 申请时联军不存在
	ERROR_UNION_APPLY_HASNOCORPS							=	-52909	,				// 申请时没有军团
	ERROR_UNION_APPLY_HASNORIGHT							=	-52908	,				// 申请时没有权限
	ERROR_UNION_APPLY_ONDISBANDSTATUS							=	-52907	,			// 申请时正在解散
	ERROR_UNION_APPLY_NOTINSAMECOUNTRY							=	-52906	,			// 申请时不是同一个国家
	ERROR_UNION_APPLY_HASINUNION							=	-52905	,				// 申请时已经在联军
	ERROR_UNION_APPLY_APPLYLISTFULL							=	-52904	,				// 申请时列表已满
	ERROR_UNION_APPLY_INAPPLYLIST							=	-52903	,				// 申请时已经在列表中
	ERROR_UNION_REPLYAPPLY_NOTEXIST							=	-52902	,				// 回应申请时不存在军团
	ERROR_UNION_REPLYAPPLY_INUNION							=	-52901	,				// 回应时已经在联军
	ERROR_UNION_REPLYAPPLY_MEMBERFULL							=	-52900	,			// 回应时成员已满
	ERROR_UNION_INVITE_CORPSNOTEXIST							=	-52899	,			// 邀请时没有军团
	ERROR_UNION_INVITE_INUNION							=	-52898	,					// 邀请时已经在军团
	ERROR_UNION_INVITE_ONDISBANDSTATUS							=	-52897	,			// 邀请时正在解散
	ERROR_UNION_INVITE_NOTINOTHERCOUNTRY							=	-52896	,		// 邀请时不在同一国家
	ERROR_UNION_REPLYINVITE_NOTEXIST							=	-52895	,			// 答复邀请时联军不存在
	ERROR_UNION_REPLYINVITE_ONDISBANDSTATUS							=	-52894	,		// 答复邀请时正在解散
	ERROR_UNION_REPLYINVITE_INUNION							=	-52893	,				// 答复邀请时在联军中
	ERROR_UNION_REPLYINVITE_MEMBERFULL							=	-52892	,			// 答复邀请时成员已满
	ERROR_UNION_LEAVE_ISUNIONHEADER							=	-52891	,				// 退出时是首领
	ERROR_UNION_CANOTLEAVEINBATTLE							=	-52890	,				// 战斗中不能离开
	ERROR_UNION_KICKCORPS_SELFCORPS							=	-52889	,				// 不能踢出自己
	ERROR_UNION_MAKE_SELFNOTHEADER							=	-52888	,				// 创建时己方权限不足
	ERROR_UNION_MAKE_OTHERNOTHEADER							=	-52887	,				// 创建时对方权限不足
	ERROR_UNION_MAKE_INVITESELF							=	-52886	,					// 创建时邀请了自己
	ERROR_UNION_MAKE_INUNION							=	-52885	,					// 创建时已经在军团
	ERROR_UNION_MAKE_INVITEBUSY							=	-52884	,					// 创建时邀请忙碌
	ERROR_UNION_INFO_NOTEXIST							=	-52883	,					// 获取信息时不存在
	ERROR_UNION_DISBAND_NOTHEADER							=	-52882	,				// 解散时不是团长
	ERROR_UNION_DISBAND_INBATTLE							=	-52881	,				// 战斗中无法解散
	ERROR_UNION_APPLYLIST_NOUNION							=	-52880	,				// 获得列表时没有联盟
	ERROR_UNION_LESSCORPS							=	-52879	,						// 创建联军时，其余军团不足
	ERROR_WORLD_INFO_NOCOUNTRY							=	-52878	,					// 找不到指定国家
	ERROR_WORLD_JOIN_ALREADYIN							=	-52877	,					// 已经在国家中
	ERROR_WORLD_JOIN_NOCOUNTRY							=	-52876	,					// 无法加入指定国家
	ERROR_WORLD_LEAVE_NOCOUNTRY							=	-52875	,					// 离开国家时没有国家
	ERROR_WORLD_SET_NOCOUNTRY							=	-52874	,					// 任命时找不到国家
	ERROR_WORLD_SET_NOTKING							=	-52873	,						// 任命时不是国王
	ERROR_WORLD_SET_NOOFFICIAL							=	-52872	,					// 任命时官职不正确
	ERROR_WORLD_SET_NOTONLINE							=	-52871	,					// 被任命的人不在线
	ERROR_WORLD_SET_ISKING							=	-52870	,						// 被任命的人是国王
	ERROR_WORLD_SET_ERRCOUNTRY							=	-52869	,					// 被任命的人不是一国
	ERROR_WORLD_UNSET_NOCOUNTRY							=	-52868	,					// 免职时找不到国家
	ERROR_WORLD_UNSET_NOTKING							=	-52867	,					// 免职时不是国王
	ERROR_WORLD_UNSET_NOOFFICIAL							=	-52866	,				// 免职时官职不正确
	ERROR_WORLD_USE_NOCOUNTRY							=	-52865	,					// 使用时找不到国家
	ERROR_WORLD_USE_NOTOFFICIAL							=	-52864	,					// 使用时权力不足
	ERROR_WORLD_USE_NOFUNCTION							=	-52863	,					// 使用时没有功能
	ERROR_WORLD_USE_NOTONLINE							=	-52862	,					// 被使用的人不在线
	ERROR_WORLD_USE_ISKING							=	-52861	,						// 被使用的人是国王
	ERROR_WORLD_USE_ERRCOUNTRY							=	-52860	,					// 被使用的人不是一国
	ERROR_WORLD_USE_NOTIME							=	-52859	,						// 次数已满
	ERROR_WORLD_USE_NOPOWER							=	-52858	,						// 使用时没有声望
	ERROR_WORLD_USE_NOMONEY							=	-52857	,						// 使用时没有钱
	ERROR_WORLD_USE_NOCONSTRUCT							=	-52856	,					// 使用时建设度不等
	ERROR_WORLD_OP_ERRWORLD							=	-52855	,						// 世界操作时世界错误
	ERROR_WORLD_OP_SAMEWORLD							=	-52854	,					// 世界操作时同一世界
	ERROR_WORLD_OP_NOTKING							=	-52853	,						// 世界操作时不是天子
	ERROR_WORLD_OP_REPLYNOSRC							=	-52852	,					// 世界操作应答时没有发起者
	ERROR_WORLD_OP_WRONGSTATUS							=	-52851	,					// 世界操作时状态不正确
	ERROR_WORLD_OP_NUMLIMIT							=	-52850	,						// 世界操作时操作次数满
	ERROR_CORPS_FULL_CHALL_TIMES							=	-52849	,				// 挑战太频繁
	ERROR_UNION_MAKE_ISOWNER							=	-52848	,					// 一方是执政军团
	ERROR_UNION_MAKE_NAMEERR							=	-52847	,					// 创建时名字或公告错误
	ERROR_WORLD_LEAVE_ISCORE							=	-52846	,					// 执政军团不能离开国家
	ERROR_UNION_DISBAND_ISLEADER							=	-52845	,				// 解散军团时是盟主
	ERROR_UNION_MAKE_NOITEM							=	-52844	,						// 邀请签字时没有物品
	ERROR_UNION_MAKE_DISBAND							=	-52843	,					// 创建联盟时处于解散状态
	ERROR_UNION_CREAT_HASDIRTYWORD							=	-52842	,				// 创建军团名字有脏词
	ERROR_UNION_CREAT_NAMEEXIST							=	-52841	,					// 创建军团时名字已经存在
	ERROR_WORLD_SET_HASOFFICIAL							=	-52840	,					// 任命时已经有官职
	ERROR_UNION_NOTSAME							=	-52839	,							// 不是同一个军联盟= -52839
	ERROR_UNION_HASBEENPRINCE							=	-52838	,					// 已经是国王了
	ERROR_UNION_MAPHASOWNER							=	-52837	,						// 地图已经有国王了
	ERROR_UNION_DESROOTOFFLINE							=	-52836	,					// 对方军团长不在线
	ERROR_UNION_ISTIANZI							=	-52835	,						// 已经是天子
	ERROR_UNION_NOTPRINCE							=	-52834	,						// 不是国王
	ERROR_UNION_MAPERROR							=	-52833	,						// 迁都城池不正确
	ERROR_CORPS_CHANGE_ISKING							=	-52832	,					// 转让时是天子
	ERROR_WORLD_SET_LOWLEVEL							=	-52831	,					// 任命官员时国家等级不足
	ERROR_WORLD_SET_LOWCONSTRUCT							=	-52830	,				// 任命官员时建设度等级不足	
	ERROR_WORLD_AWARD_FULLTIMES							=	-52829	,					// 领取奖励时次数已满
	ERROR_WORLD_AWARD_FULLBAGGAGE							=	-52828	,				// 领取奖励时包裹已满
	ERROR_WORLD_AWARD_NOTOWNER							=	-52827	,					// 领取奖励时没有占领
	ERROR_WORLD_AWARD_NOCOUNTRY							=	-52826	,					// 领取奖励时国家不对
	ERROR_CORPS_GETGOODLIST_HASNOTOWNCITY							=	-52825	,		// 不占领该城市不能使用该战争商店	
	ERROR_CORPS_USELEAGUEBAG_HASNOCORPS							=	-52824	,			// 没有军团不能使用军团特供物品	 
	ERROR_CORPS_BUYCORPSGOOD_NOTALLOWED							=	-52823	,			// 不能购买改军团商店的物品
	ERROR_CORPS_BUYCORPSGOOD_ITEMNOTENOUGH							=	-52822	,		// 军团商店物品不足
	ERROR_CORPS_EXCHANGE_NOTALLOWED							=	-52821	,				// 不能兑换
	ERROR_CORPS_EXCHANGE_ITEMNOTENOUGH							=	-52820	,			// 物品数量不足，不能兑换
	ERROR_UNION_CHANGENOTHEAD							=	-52819	,					// 改名时不是军团长
	ERROR_UNION_CHANGEERRNAME							=	-52818	,					// 改名时名字错误
	ERROR_WORLD_SUMMONINREP							=	-52817	,								// 召唤时在副本中
	ERROR_WORLD_SUMMONINPRI							=	-52816	,								// 召唤时在监狱中
	ERROR_CORPS_BUYCORPSITEM_HASNORIGHT							=	-52815	,			// 购买军团商店的物品时没有权限
	ERROR_WORLD_SUMMONINGROUND							=	-52814	,					// 召唤时在地宫中
	ERROR_WORLD_SUMMONINFOREIGN							=	-52813	,					// 召唤时在外国
	ERROR_WORLD_SUMMONINSTALL							=	-52812	,					// 召唤时在摆摊
	ERROR_WORLD_SUMMONEDINPRI							=	-52811	,					// 被召唤时在监狱中
	ERROR_WORLD_BATTLEUSE							=	-52810	,						// 不能对战场玩家使用
	ERROR_WORLD_SUMMONEDINSTA							=	-52809	,					// 摆摊时不能被召唤
																						
	ERROR_CORPS_LEAVECORPS_NOTFAMILYHEADER			= -52808,	// 不是家族族长没有权限离开军团
	ERROR_CORPS_KICKFAMILY_CORPSHEADERINFAMILY		= -52807,	// 被踢家族是大将军所在家族

	ERROR_CORPS_CANTLOGINBATTLE						= -52806,	// 不能进入战场
	ERROR_WORLD_SUMMONINWEDDINGHALL					= -52805,	// 召唤时在礼堂
	ERROR_WORLD_NOTREDPRISON						= -52804,	// 白名不能关

	ERROR_WAR_SUBMITLOWLEVEL						= -52803,	// 报名等级低
	ERROR_WAR_SUBMITNOMONEY							= -52802,	// 报名没钱
	ERROR_WAR_WARFULL								= -52801,	// 战场已满	
	ERROR_NOT_CORPS_APPLY_TIME						= -52800,	// 战争时间不能修改参战阵容
	ERROR_WAR_NOTKING								= -52799,	// 不是国王
	ERROR_WAR_HASMONEY								= -52798,	// 已经竞价过了
	ERROR_WAR_HASSUBMIT								= -52797,	// 已经报名过
	ERROR_WAR_NOTSUBMITTIME							= -52796,	// 不是报名时间
	ERROR_WAR_NOTSUBMIT								= -52795,	// 还没有报名
	ERROR_WAR_NOTWARTIME							= -52794,	// 不是战争时间
	ERROR_WAR_NOAWARD								= -52793,	// 不能领奖
	ERROR_WAR_MONEYFULL								= -52792,	// 领奖金钱满
	ERROR_WAR_LOSE									= -52791,	// 领奖时战败
	ERROR_WAR_NOTBIDTIME							= -52790,	// 不是竞价时间
	ERROR_WAR_BIDLOW								= -52789,	// 竞价钱少
	ERROR_TAX_GETNOTKING							= -52788,	// 没有权利查询
	ERROR_TAX_OVERTIMES								= -52787,	// 超过设置次数
	ERROR_TAX_SETNOTKING							= -52786,	// 没有权利设置

	ERROR_WORLD_NOTLEADER							= -52785,	// 没有权利召唤

	ERROR_CORPS_BIDINDISBANDING						= -52784,	// 军团处于解散状态不能竞价
	ERROR_WAR_GMNOSUBMIT							= -52783,	// GM设置不能报名
	
	ERROR_CAMP_NotWeakCamp							= -52782,	// 玩家所在阵营不是弱势阵营
	ERROR_CAMP_NotWeakCampL2						= -52781,	// 玩家不是弱势二级以下（含二级）阵营，不能领取物品奖励
	ERROR_CAMP_AlreadyGetAward_Week					= -52780,	// 已经领取过本周的弱势阵营奖励
	ERROR_CAMP_AlreadyGetAward_Day					= -52779,	// 已经领取过当天的弱势阵营奖励

	ERROR_ACCUSE_LOWLEVEL							= -52778,	// 弹劾等级不足
	ERROR_ACCUSE_NOMONEY							= -52777,	// 弹劾没钱
	ERROR_ACCUSE_NOTTIME							= -52776,	// 弹劾时间不对
	ERROR_ACCUSE_HASVOTED							= -52775,	// 已经投过票
	ERROR_CORPS_ACCUSEFORBID						= -52774,	// 军团因弹劾不能竞价
	ERROR_CORPS_SubBidMainCity						= -52773,	// 军盟之中的子军团不能竞价主城

	ERROR_UNOIN_INVITE_CITY_CONTENDER				= -52772,	// 不能邀请正在争夺主城的军团（参与了主城竞价，或等待开战和战斗中的军团） 
	ERROR_UNOIN_APPLY_CITY_CONTENDER				= -52771,	// 本军团已参与争夺主城（参与了主城竞价，或等待开战和战斗中的军团），暂时不能申请加入军盟
	ERROR_UNOIN_REAPPLY_CITY_CONTENDER				= -52770,	// 对方军团已参与争夺主城（参与了主城竞价，或等待开战和战斗中的军团），不能同意对方的申请加入
	ERROR_UNOIN_REINVITE_CITY_CONTENDER				= -52769,	// 本军团已参与争夺主城（参与了主城竞价，或等待开战和战斗中的军团），对方的邀请无效

	ERROR_WORLD_FUNCTIONTIMEERR						= -52768,	// 使用官职功能时间错误
	ERROR_WORLD_CANNOTUSEGT							= -52767,	// 当前不能使用国探
	ERROR_WORLD_CANNOTUSELW							= -52766,	// 当前不能使用乱舞
	ERROR_CANNOT_FIND_CORPS_BOSS					= -52765,	// 不能找到召唤的军团boss
	ERROR_CORPCRYSTAL_NOTENOUGH						= -52764,	// 没有足够的boss魂魄
	ERROR_CORPBOSSREPETION_NOTOPEN					= -52763,	// 军团boss副本没有开启
	ERROR_CORPBOSS_INCD										= -52762,  // 军团boss处于CD中，无法再次召唤
	ERROR_CORPLEVEL_NOTENOUGH							= -52761,  // 军团等级不够，无法召唤军团boss
	ERROR_CORPS_REFINEVAL_NOTENOUGH							= -52760,  // 军团的炼化度不够，无法召唤军团boss

};

//	法宝( -53999 ~ -53000 )
enum EMagicWeapon
{
	ERROR_MW_NOT_ENOUGH_POINTS							=	-53999	,		// 加点时 没有足够多的潜能点
	ERROR_MW_ERRPERCENT							=	-53998	,				// 错误的经验分配比例
	ERROR_MW_SKILLEXSITED							=	-53997	,			// 该技能框里已有技能
	ERROR_MW_SKILLBOXNOTACT							=	-53996	,			// 该技能框还没有激活
	ERROR_MW_ERRSKILL							=	-53995	,				// 错误的技能(内部出错)
	ERROR_MW_NOSUCKSKILL							=	-53994	,			// 没有这个技能
	ERROR_MW_NOTEQUIP							=	-53993	,				// 没有装备法宝
	ERROR_MW_NOTMW							=	-53992	,					// 放上来的不是法宝
	ERROR_MW_NOTMWSTONE							=	-53991	,				// 放上来的不是法宝鉴定石
	ERROR_MW_JUDGENOMONEY							=	-53990	,			// 鉴定的时候钱不够
	ERROR_MW_SKILLNEEDMORELEVEL							=	-53989	,		// 技能需求等级不足 989
	ERROR_MW_SKILLNOPOINTS							=	-53988	,			// 没有足够的技能点
	ERROR_MW_SKILLMAXLEVEL							=	-53987	,			// 法宝技能已到最大等级
	ERROR_MW_NOSTONE							=	-53986	,				// 没有鉴定石
	ERROR_MW_NPCINVALID							=	-53985	,				// NPC距离非法！
	ERROR_MW_ERRORBOXINDEX							=	-53984	,			// 错误的技能框索引
	ERROR_MW_CANTUSEBOOK							=	-53983	,			// 该法宝不能使用技能书
	ERROR_MW_BOOKCOLORERROR							=	-53982	,			// 技能书与技能框颜色不一致
	ERROR_MW_HASEDBOOKSKILL							=	-53981	,			// 已经有该技能了，不能使用技能书合成
	ERROR_MW_SKILLREPEATED							=	-53980	,			// 拥有技能重复
	ERROR_MW_NOTSKILLBOOK							=	-53979	,			// 拖放的不是技能书 979
	ERROR_MW_BADUSE							=	-53978	,					// 该功能类物品不能使用
	ERROR_MW_CANTJUDGE							=	-53977	,				// 这个技能框不能鉴定
	ERROR_MW_LEVELUP							=	-53976	,				// 恭喜你 法宝升级
	ERROR_MW_FULLHP							=	-53975	,					// 你的法宝灵气值已满 不用吃药
	ERROR_MW_FULLMP							=	-53974	,					// 你的法宝法力值已满 不用吃药
	ERROR_MW_STEPCLEARED							=	-53973	,			// 这个等级你已经小洗过了
	ERROR_MW_CLEAR_STONE_ERROR							=	-53972	,		// 放上的法宝洗石头类型不匹配
	ERROR_MW_CLEAR_TYPE_ERROR							=	-53971	,		// 洗点类型错误
	ERROR_MW_CANT_CLEARPOINT							=	-53970	,		// 无法洗点
	ERROR_MW_CANT_CLEARSP								=	-53969	,		// 没装备法宝 无法洗技能点
	ERROR_MW_SP_NOADD									=	-53968	,		// 您还没有加点，不需要洗点
	ERROR_MW_DECPOM_BAGFUL								=	-53967	,		// 分解法宝时，包裹满
	ERROR_MW_DECPOM_LESSMONEY								=	-53966	,		// 分解法宝时，没钱
	ERROR_MW_PRINT_LESSMONEY								=	-53965	,		// 法宝印记时，没钱
	ERROR_MW_PRINT_FAILED								=	-53964	,		// 法宝印记失败
	ERROR_MW_PRINT_ERRPIN								=	-53963	,		// 品级不同，不能印记
	ERROR_MW_QUICKUP_LEVELLESS							=	-53962	,		// 等级不够，不能飞升
	ERROR_MW_QUICKUP_LESSMONEY							=	-53961	,		// 飞升法宝时，没钱
	ERROR_MW_QUICKUP_LESSITEM							=	-53960	,		// 飞升法宝时，物品不足
	ERROR_MW_QUICKUP_FAILED								=	-53959	,		// 飞升法宝时，失败
	ERROR_MW_UPLEVEL_ERROR2NDMW							=	-53958	,		// 飞升升品时，副法宝不符合要求
	ERROR_MW_USESKILLBOOK_FAILED						=	-53957	,		// 使用技能书学习技能 失败
	ERROR_MW_USESKILLBOOK_NOMONEY						=	-53956	,		// 使用技能书学习技能 没钱
	ERROR_MW_CANTIN_INVALIDBAG							=	-53955	,		// 不能使用已过期包裹中的材料
	ERROR_MW_MWCANTIN_INVALIDBAG						=	-53954	,		// 不能使用已过期包裹中的法宝
	ERROR_MW_QL_LESS									=	-53953	,		// 合成时器灵不够
	ERROR_MW_LEVEL_FULL									=	-53952	,		// 法宝已经达到最高等级
	ERROR_MW_PRINT_PIN_ERR								=	-53951	,		// 法宝印记时幸运符和法宝品级不同，不能印记
	ERROR_MW_BIND_BYITEM								=  -53950,		// 法宝被手动绑定，不能执行该操作
};

// 生活技能( -54999 ~ -54000 )
enum EM_LifeSkillError
{	
	ERROR_LIFESKILL_COMPOSENOTEXIST							=	-54999	,						// 学习配方时配方不存在
	ERROR_LIFESKILL_WRONGLIFESKILL							=	-54998	,						// 学习配方时技能类型不对
	ERROR_LIFESKILL_LOWDEGREE							=	-54997	,							// 熟练度不够	
	ERROR_LIFESKILL_WRONGGETWAY							=	-54996	,							// 配方的获取方式不对
	ERROR_LIFESKILL_COMPOSEHASEXIST							=	-54995	,						// 学习配方时配方已经存在
	ERROR_LIFESKILL_HASINITIALIZED							=	-54994	,						// 生活技能已经入门
	ERROR_LIFESKILL_NOTINITIALIZED							=	-54993	,						// 学习生活技能时还没有入门
	ERROR_LIFESKILL_DIFFINITIALSKILL							=	-54992	,					// 学习的生活技能类型与入门的时候技能类型不同
	ERROR_LIFESKILL_LIFESKILLTABLENOTEXIST							=	-54991	,				// 生活技能表不存在
	ERROR_LIFESKILL_CANNOTLEVELUP							=	-54990	,						// 生活技能不可升级
	ERROR_LIFESKILL_LEVELNEEDMOREDEGREE							=	-54989	,					// 生活技能晋级需要更多的熟练度
	ERROR_LIFESKILL_COMPOSENOTSTUDIED							=	-54988	,					// 生活技能并未学习
	ERROR_LIFESKILL_STUDYCOMPOSEFAILED							=	-54987	,					// 创造配方失败
	ERROR_LIFESKILL_SKILLFULL							=	-54986	,							// 生活技能已经学满不能再学
	ERROR_LIFESKILL_SKILLEXIST							=	-54985	,							// 该生活技能已学
	ERROR_LIFESKILL_INITIALLEVELLOW							=	-54984	,						// 学习生活技能时等级不够
	ERROR_LIFESKILL_STUDYMONEYNOTENOUGH							=	-54983	,					// 学习生活技能时金钱不够
	ERROR_LIFESKILL_LEVELUPMONEYNOTENOUGH							=	-54982	,				// 生活技能进阶时金钱不够	
	ERROR_LIFESKILL_LEVELUPLEVELNOTENOUGH							=	-54981	,				// 生活技能进阶时等级不够
	ERROR_LIFESKILL_ITEMCANOTDECOMPOUND							=	-54980	,					// 物品不能被分解
	ERROR_LIFESKILL_DECOMPOUNDWITHOUTSPACE							=	-54979	,				// 分解装备时包裹空间不够
	ERROR_LIFESKILL_DECOMPOUNDWITHOUTGOLDMAKE							=	-54978	,			// 没有点金技能不能分解装备
	ERROR_LIFESKILL_DECOMPOUNDWITHOUTENGOUGHDEGREE							=	-54977	,		// 分解装备时熟练度不够
	ERROR_LIFESKILL_USECOMPOSEWITHOUTLIFESKILL							=	-54976	,			// 使用配方时没有该生活技能
	ERROR_LIFESKILL_COMPOSEDEGREENOTENOUGH							=	-54975	,				// 使用配方时熟练度不够
	ERROR_LIFESKILL_COLLECTWITHOUTRIGHTSKILL							=	-54974	,			// 采集时没有学习采集技能
	ERROR_LIFESKILL_COLLECTDEGREELOW							=	-54973	,					// 进行采集时熟练度不够
	ERROR_LIFESKILL_COLLECTAPLOW							=	-54972	,						// 采集时候ap值不够
	ERROR_LIFESKILL_SETMAGICEUIPLEVELLOW							=	-54971	,				// 附魔时装备等级不够
	ERROR_LIFESKILL_SETMAGICFAILED							=	-54970	,						// 附魔失败
	ERROR_LIFESKILL_SETMAGICWITHOUTSKILL							=	-54969	,				// 附魔时没有点金技能
	ERROR_LIFESKILL_SETEQUIPMAGICWRING							=	-54968	,					// 不能用附魔石给该装备附魔
	ERROR_LIFESKILL_USELIFESKILLPHISICWRONG							=	-54967	,				// 不能使用生活技能药品
	ERROR_LIFESKILL_SPECIALIZEDEGREEFULL							=	-54966	,				// 生活技能熟练度已经到达上限不需要吃药
	ERROR_LIFESKILL_APNOTENOUGH							=	-54965	,							// 学习生活技能时AP值不够	
	ERROR_LIFESKILL_SETNUMLIMIT							=	-54964	,							// 装备附魔时同种附魔石已达附魔上限	
	ERROR_LIFESKILL_COMPOSESTUDYMONEYNOTENOUGH							=	-54963	,			// 学习配方时金钱不足
	ERROR_LIFESKILL_EQUIPNOTCOMPONEDE							=	-54962	,					// 装备不可分解
	ERROR_LIFESKILL_ITEMCANNOTCREATCOMPOSE							=	-54961	,				// 物品不能用与配方创造
	ERROR_LIFESKILL_BAGFULLWHENCOLLECT							=	-54960	,					// 采集时包裹满	
	ERROR_LIFESKILL_COLLECTLEVELLOW								=	-54959	,					// 采集等级低	

};

// 商城 ( -55999 ～ -55000 )
enum EMIBStoreError
{
	ERROR_IBITEM_DOESNOT_EXIST 			= -55999,			// 物品不存在
	ERROR_IBITEM_ISNOT_ACTIVE  			= -55998,			// 物品不生效
	ERROR_IBITEM_ISNOT_ENOUGH  			= -55997,			// 物品数目不足
	ERROR_IBITEM_INTERNAL	   			= -55996,			// 数据库内部错误, 服务暂时不可用
	ERROR_PLAYER_HAS_ANOTHER_TRANS		= -55995,			// 玩家有正在进行中的未完交易
	ERROR_PLAYER_HASNO_ENOUGHYB			= -55994,			// 玩家元宝数据不足
	ERROR_PLAYER_HASNO_ACTIVEITEM 		= -55993,			// 玩家没有物品奖励或者已经领取过了
	ERROR_ONLY_ONE_IBITEM_PERMITED		= -55992,			// 不可堆叠的物品只能购买一件
	ERROR_INSERTITEM_INTERNAL			= -55991, 			// 插入物品内部错误
	ERROR_IBITEM_INVALID_NUMBER			= -55990,			// 玩家输入的物品数量不正确
	ERROR_IBSTORE_SAME_VERSION			= -55989,			// 商城版本号一致
	ERROR_IBSTORE_INVALID_INPUT			= -55988,			// 玩家输入非法
	ERROR_IBSTORE_DRAW_SUCCESS			= -55987,			// 申请领取成功
	ERROR_IBSTORE_DRAW_FAILURE			= -55986,			// 申请领取失败
	ERROR_IBSTORE_DRAW_ISNOT_ACTIVE		= -55985,			// 服务不可用
	ERROR_PLAYER_IBITEM_LIMITED			= -55984,			// 购买物品个数限量 
	ERROR_IBSTORE_INVALID_TRANS			= -55983,			// 无效的交易
	ERROR_IBSTORE_LOCK_SUCCESS          = -55982,           // 向网关锁定物品成功
	ERROR_IBSTORE_INVALID_TIME			= -55981,			// 无效的时间段
	ERROR_IBTRANS_NOT_RETURN			= -55920,			// 上次交易未返回，请重新登录。

	ERROR_FUNC_DOESNOT_EXIST			= -55979,			// 功能付费项不存在
	ERROR_FUNC_HASNO_ENOUGHMONEY		= -55978,			// 没有足够的金钱
	ERROR_FUNC_USE						= -55977,			// 功能付费的功能使用失败
	ERROR_FUNC_TRANS_OBJ_FULL			= -55976,			// 功能付费交易对象数满
	ERROR_PLAYER_HASNO_ENOUGHBINDYB		= -55975,			// 玩家绑定元宝数量不足
	ERROR_PLAYER_HASNO_ENOUGHFAME		= -55974,			// 玩家绑定元宝数量不足
};


// 踢人错误码 ( -56100 ~ -56000 )
enum EmKickOutReason
{
	EM_KICKOUT_ANTIBOT					= -56100,		// 反外挂踢人,第三方软件
	EM_KICKOUT_LOCKDATA					= -56099,		// 玩家数据异常，角色被锁定，请联系客服人员
														// 如: 玩家存盘不完整； 玩家数据在新模板里找不到等等，
														// 都会锁定玩家，以确保不会让玩家数据丢失
	EM_KICKOUT_TIMEOUT					= -56098,		// 客户端连接超时
	EM_KICKOUT_SERVERMAINTANCE			= -56097,		// 服务器停机维护
	EM_KICKOUT_GM						= -56096,		// GM踢人
	EM_KICKOUT_ACCOUNTTRUSTEE			= -56095,		// 帐号被托管
	EM_KICKOUT_UNKOWNACTION				= -56094,		// 玩家行为异常( 如：恶意向服务器发大量消息 )
	EM_KICKOUT_RELOGIN					= -56093,		// 玩家重复登录踢人
	EM_KICKOUT_NOTINGATE				= -56092,		// 玩家的网关数据不存在
	EM_KICKOUT_SAVETIMEOUT				= -56091,		// 存盘超时踢人
	EM_KICKOUT_ACCOUNTFROZEN			= -56090,		// 冻结帐号踢人
	EM_KICKOUT_ANTIBOT_MULTICLIENT		= -56089,		// 反外挂踢人： 客户端多开（大于3个）
	EM_KICKOUT_ANTIBOT_VM				= -56088,		// 反外挂踢人： 虚拟桌面		
	EM_KICKOUT_ANTIBOT_MULTIUSER		= -56087,		// 反外挂踢人： 多用户
	EM_KICKOUT_ANTIADDICTION			= -56086,		// 防沉迷踢人
	EM_KICKOUT_SPEEDCHEATING			= -56085,		// 移动速度异常，请离游戏
	EM_KICKOUT_SPEEDCHEATING2			= -56084,		// 移动速度异常，请离游戏
	EM_KICKOUT_MULTICLIENT				= -56083,		// 单IP登陆玩家过多, 请离游戏
	EM_KICKOUT_DATACRASH				= -56082,		// 数据损坏，登陆游戏失败(如：玩家已有数据在模板中找不到；数据库中字段数目不正确)
	EM_KICKOUT_ANTIBOT_OTHER			= -56081,		// 反外挂其它错误
};

enum EmKickOutResult
{
	EM_KICKOUT_NOTHISCHAR				= -56050,		// 玩家不存在
	EM_KICKOUT_SAVING					= -56049,		// 玩家正在存盘, 请稍后
	EM_KICKOUT_KICKING					= -56048,		// 玩家正在被踢过程中，请稍后
	EM_KICKOUT_WITHOUTSAVE				= -56047, 		// 非存盘踢人成功
};

//  婚姻系统( -57999 ~ -57000 )
enum EmMarriageErrcode
{
	EM_MARRIAGE_MARRY_HASNOTEAM					= -57999,		// 结婚时没有组队，或者队伍人数不对
	EM_MARRIAGE_MARRY_SAMESEX					= -57998,		// 同性不能结婚
	EM_MARRIAGE_MARRY_MARRIED					= -57997,		// 已经结婚不能再结婚
	EM_MARRIAGE_MARRY_OTHERMARRIED				= -57996,		// 对方已经结婚不能再结婚
	EM_MARRIAGE_MARRY_NOTTEAMHEADER				= -57995,		// 不是队长不能申请结婚
	EM_MARRIAGE_MARRY_NOTSAMECAMP				= -57994,		// 阵营不同不能结婚	
	EM_MARRIAGE_MARRY_MARRIAGELEVELLOW			= -57993,		// 结婚等级不够
	EM_MARRIAGE_MARRY_WRONGWEDDINGTIME			= -57992,		// 婚期不对
	EM_MARRIAGE_MARRY_WRONGWEDDINGTIMEFAILURE	= -57991,		// 婚期已经失效	
	EM_MARRIAGE_MARRY_MONEYNOTENOUGH			= -57990,		// 金钱不足
	EM_MARRIAGE_MARRY_NOTENOUGHBAGSPACE			= -57989,		// 结婚一方的包裹空间不够
	EM_MARRIAGE_WEDDING_SPOUSEOFFLINE			= -57988,		// 开始婚礼时配偶不在线
	EM_MARRIAGE_ENTERHALL_NOHOLDWEDDING			= -57987,		// 没有正在举行的婚礼
	EM_MARRIAGE_ENTERHALL_NOTENTERHALL			= -57986,		// 婚礼还未进行到礼堂，不能进入礼堂
	EM_MARRIAGE_ENTERHALL_HASNOINVITATION		= -57985,		// 没有请帖不能进入礼堂
	EM_MARRIAGE_PUTKEEPSAKE_WRONGKEEPSAKE 		= -57984,		// 交换戒指时放置的物品不是信物
	EM_MARRIAGE_EXCHANGEKEEPSAKE_NEWLYOFFLINE	= -57983,		// 交换结婚戒指时一方不在线
	EM_MARRIAGE_EXCHANGEKEEPSAKE_EMPTYKEEPSAKE	= -57982,		// 交换结婚戒指时另外一方没有放置戒指
	EM_MARRIAGE_MARRY_OTHERHASMARRIAGEAPPLY		= -57981,		// 结婚时对方已经有婚约
	EM_MARRIAGE_DIVOCE_HASNODOVICEITEM			= -57980,		// 离婚时没有离婚道具
	EM_MARRIAGE_DIVOCE_NOMARRIAGE				= -57979,		// 没有结婚不用离婚
	EM_MARRIAGE_INVITATION_RECEIVERNOEXIST		= -57978,		// 请帖接收人不存在
	EM_MARRIAGE_INVITATION_INDIFFCOUNTRY		= -57977,		// 请帖接收人不在同一个阵营
	EM_MARRIAGE_INVITATION_NOTINVITATION		= -57976,		// 发送请帖时类型不对(不是请帖)
	EM_MARRIAGE_INVITATION_NOTMARRIED			= -57975,		// 没有结婚不能发送请帖
	EM_MARRIAGE_PUTKEEPSAKE_NOTINHALL			= -57974,		// 没在礼堂不能交换信物
	EM_MARRIAGE_PUTKEEPSAKE_WEDDINGNOTHOLD		= -57973,		// 没有举行婚礼不能交换信物
	EM_MARRIAGE_EXCHANGEKEEPSAKE_NOTMARRIED		= -57972,		// 没有结婚不能交换信物
	EM_MARRIAGE_EXCHANGEKEEPSAKE_OTHERNOEREADY	= -57971,		// 交换信物时对方没有准备好
	EM_MARRIAGE_TELEPORT_OTHEROFFLINE			= -57970,		// 由于配偶不在线或者不在本正营无法传送
	EM_MARRIAGE_ENTERHALL_NOMARRIAGE			= -57969,		// 没有结婚不允许进入礼堂
	EM_MARRIAGE_ENTERHALL_WRONGTIME				= -57968,		// 当前不是自己的婚礼不能进入礼堂
	EM_MARRIAGE_BEGINWEDDING_WRONGSTATUS		= -57967,		// 由于没有结婚或者婚礼时间未到不能举行婚礼
	EM_MARRIAGE_INVITATION_SELFSEND				= -57966,		// 不能给自己和配偶发送请帖
	EM_MARRIAGE_MARRY_INVALIDWEDDINGTIME		= -57965,		// 所选婚期已经失效，婚期距离现在时间太近或者已经过了该婚期
	EM_MARRIAGE_MARRY_INVALIDAPPLY				= -57964,		// 答复结婚时申请已经失效
	EM_MARRIAGE_MARRY_FAILED					= -57963,		// 申请失败，稍后再试
	EM_MARRIAGE_PUTKEEPSAKE_SPOUSEOFFLINE		= -57962,		// 交换信物时对方不在线
	EM_MARRIAGE_TELEPORT_ONEQUIP				= -57961,		// 没有装备结婚戒指不能使用
	EM_MARRIAGE_TELEPORT_NOMARRIED				= -57960,		// 没有结婚不能使用结婚装备的传送功能
	EM_MARRIAGE_TELEPORT_NOTMARRIEDEQUIP		= -57959,		// 不	是和当前配偶的结婚戒指不能传送
	EM_MARRIAGE_TELEPORT_INVALIDMAP				= -57958,		// 在副本或者礼堂不能使用结婚戒指传送
	EM_MARRIAGE_DIVORCE_ONWEDDING				= -57957,		// 婚礼游行时不能离婚
	
	EM_MARRIAGE_PUTSAKEEXCHANGE_ONMARRIED		= -57956,		// 没有结婚不能进行信物交换
	EM_MARRIAGE_PUTSAKEEXCHANGE_WITHOUTDISTANCE	= -57955,		// 配偶距离过远不能进行信物交换
	EM_MARRIAGE_INVITATIONEXCHANGE_ITEMNOTEXIST	= -57954,		// 请帖兑换的物品不存在
	EM_MARRIAGE_INVITATIONEXCHANGE_WRONGITEM	= -57953,		// 不是请帖不能进行请帖兑换
	EM_MARRIAGE_INVITATIONEXCHANGE_WEDDINGNOTEND= -57952,		// 婚礼结束前不能兑换请帖
	EM_MARRIAGE_MARRIAGEEQUIP_NOTMARRIED		= -57951,		// 没有结婚不允许使用结婚装备
	EM_MARRIAGE_MARRIAGEEQUIP_NOTSPOUSE			= -57950,		// 对方不是配偶不能使用配偶技能
	EM_MARRIAGE_MARRIAGEEQUIP_NOTMARRIEDKEEPSAKE= -57949,		// 使用配偶技能时使用的装备不对，不是和配偶结婚时的结婚装备
	EM_MARRIAGE_MARRY_NOMARRIAGEITEM			= -57948,		// 申请结婚时没有结婚道具
	EM_MARRIAGE_MARRYAPPLY_WRONGMODULE			= -57947,		// 该时间段不能选择该种规模的婚礼
	EM_MARRIAGE_MARRY_HASNOBASICITEM			= -57946,		// 结婚时办证道具数量不足
	EM_MARRIAGE_REVIEWWEDDING_NOTMARRY			= -57945,		// 没有结婚不能重温补办婚礼
	EM_MARRIAGE_REVIEWWEDDING_WRONGSTATUS		= -57944,		// 婚礼还未举行不用补办	
	EM_MARRIAGE_REVIEWWEDDING_WEDDINGCOOLTIME	= -57943,		// 距离上一次婚礼不足24小时
	EM_MARRIAGE_REVIEWWEDDING_TIMESELECTED		= -57942,		// 婚期不可选
	EM_MARRIAGE_REVIEWWEDDING_TIMETOONEAR		= -57941,		// 选择的婚礼时间距离现在的间隔太短
	EM_MARRIAGE_REVIEWWEDDING_HASNOWEDDINGSCROLL= -57940,		// 没有婚礼卷轴
	EM_MARRIAGE_REVIEWWEDDING_NOSPACE			= -57939,		// 包裹空间不足
	EM_SWORNEQUIPMENT_NOTINSWORNTEAM			= -57938,		// 没有结拜不能使用结拜信物
	EM_SWORNEQUIPMENT_NOTINSAMESWORNTEAM		= -57937,		// 不能对没有结拜的人使用结拜信物技能
};

enum EmActivityYqsErrcode
{
    EM_YQS_ACTIVITYNOTSTART      =  -58001,             // 摇钱树活动为开启
    EM_YQS_USERNOTMATCH          =  -58002,             // 传入用户编号错误
    EM_YQS_ACTIVITYIDNOTMATCH    =  -58003,             // 传入的活动编号错误
    EM_YQS_HANDINITEMINVALIED    =  -58004,             // 上缴物品错误
    EM_YQS_GETGROWINGVALUEFAILED =  -58005,             // 不能获取摇钱树成长值
    EM_YQS_SENDUPDATEMSGTOGATEFAILED =  -58006,         // 向GATE服务器发送消息错误

};

enum Notice
{
	NOTICE_REFRESHBOSS							=	-69999	,			// 刷新BOSS通知			int0 怪物ID int1 MapID int2 PosX int3 PosY 
	NOTICE_PREREFRESHBOSS							=	-69998	,		// 提前刷新BOSS通知		int0 怪物ID(0特殊处理) int1 MapID(不准确需要处理) int2 提前
	NOTICE_PLAYERKILLBOSS							=	-69997	,		// 玩家杀死BOSS通知		int0 怪物ID int1 提示的类型 string1 玩家名
	NOTICE_PLAYERKILLBOSS_1							=	-69996	,		// 客户端占位用
	NOTICE_PLAYERKILLBOSS_2							=	-69995	,		
	NOTICE_PLAYERKILLBOSS_3							=	-69994	,		
	NOTICE_KILLPLAYER							=	-69993	,			// 杀死玩家通知			int0 杀人者阵营 int1 被杀者阵营 int2 地图ID int3 PosX int4
	NOTICE_KILLPLAYER_1							=	-69992	,			// 客户端占位用
	NOTICE_KILLKING							=	-69991	,				// 杀死国王通知			int0 杀人者阵营 int1 被杀者阵营 int2 国家 string1 杀人者名 string2 被杀者名
	NOTICE_KILLKING_1							=	-69990	,			// 客户端占位用
	NOTICE_DOPLAYER							=	-69989	,				// 玩家处置通知			int0 Type int1 Order int2 国家ID	string1 发起者名字
	NOTICE_KINGONLINE							=	-69988	,			// 国王上线				int0 0 off / 1 on int1 阵营ID int2 国家ID string1 国王名字
	NOTICE_OPEN_LUCKYBAG1						=	-69987	,			// 打开乾坤袋开出好东西	int0 玩家角色ID int1 玩家开的物品的模板ID int2 玩家获得的物品ID int3 玩家获得的物品的数量 string0 玩家名字
	NOTICE_OPEN_LUCKYBAG2						=	-69986	,			// 打开乾坤袋开出好东西	int0 玩家角色ID int1 玩家开的物品的模板ID int2 玩家获得的物品ID int3 玩家获得的物品的数量 string0 玩家名字
	NOTICE_OPEN_LUCKYBAG3						=	-69985	,			// 打开乾坤袋开出好东西	int0 玩家角色ID int1 玩家开的物品的模板ID int2 玩家获得的物品ID int3 玩家获得的物品的数量 string0 玩家名字
	NOTICE_OPEN_LUCKYBAG4						=	-69984	,			// 打开乾坤袋开出好东西	int0 玩家角色ID int1 玩家开的物品的模板ID int2 玩家获得的物品ID int3 玩家获得的物品的数量 string0 玩家名字
	NOTICE_OPEN_LUCKYBAG5						=	-69983	,			// 打开乾坤袋开出好东西	int0 玩家角色ID int1 玩家开的物品的模板ID int2 玩家获得的物品ID int3 玩家获得的物品的数量 string0 玩家名字
	NOTICE_OPEN_LUCKYBAG6						=	-69982	,			// 打开乾坤袋开出好东西	int0 玩家角色ID int1 玩家开的物品的模板ID int2 玩家获得的物品ID int3 玩家获得的物品的数量 string0 玩家名字
	NOTICE_OPEN_LUCKYBAG7						=	-69981	,			// 打开乾坤袋开出好东西	int0 玩家角色ID int1 玩家开的物品的模板ID int2 玩家获得的物品ID int3 玩家获得的物品的数量 string0 玩家名字
	NOTICE_OPEN_LUCKYBAG8						=	-69980	,			// 打开乾坤袋开出好东西	int0 玩家角色ID int1 玩家开的物品的模板ID int2 玩家获得的物品ID int3 玩家获得的物品的数量 string0 玩家名字
	NOTICE_OPEN_LUCKYBAG9						=	-69979	,			// 打开乾坤袋开出好东西	int0 玩家角色ID int1 玩家开的物品的模板ID int2 玩家获得的物品ID int3 玩家获得的物品的数量 string0 玩家名字
	NOTICE_OPEN_LUCKYBAG10						=	-69978	,			// 打开乾坤袋开出好东西	int0 玩家角色ID int1 玩家开的物品的模板ID int2 玩家获得的物品ID int3 玩家获得的物品的数量 string0 玩家名字
	NOTICE_MINEMAP								=	-69977	,			// 开藏宝图得东西 int0 玩家角色ID int1 无 int2 玩家获得的物品ID int3 玩家获得的物品的数量 string0 玩家名字
	NOTICE_MINEMAP_SELF_MONEY					=	-69976	,			// 自己的藏宝图消息
	NOTICE_MINEMAP_SELF_ITEM					=	-69975	,			// 自己的藏宝图消息
	NOTICE_MINEMAP_OTHER_MONEY					=	-69974	,			// 别人的藏宝图消息
	NOTICE_MINEMAP_OTHER_ITEM					=	-69973	,			// 别人的藏宝图消息
	NOTICE_IBSTORE_TIMER_SELLING				=  	-69972	, 			// 商城某道具即将开卖
	NOTICE_IBSTORE_TIMER_SELLED					=  	-69971	, 			// 商城某道具已经开卖
	NOTICE_IBSTORE_TIMER_OVER					=   -69970	, 			// 商城某道具已经截止
	NOTICE_IBSTORE_NUMBER						=   -69969	, 			// 商城某道具数量提示
	NOTICE_IBSTORE_ZERO							=   -69968	, 			// 商城某道具售罄提示

	NOTICE_WAR_STATUSCHANGED					=	-69967	,			// 战争状态改变 int0 当前战争状态
	NOTICE_WAR_STONE_CREATE						=	-69966,				// 灵石创建				int0 坐标X int1 坐标Y int3 模板ID
	NOTICE_WAR_STONE_PICK						=	-69965,				// 灵石拿起				int0 坐标X int1 坐标Y 	int2 模板ID int3 阵营ID string1 名字
	NOTICE_WAR_STONE_THROW						=	-69964,				// 灵石丢下				int0 坐标X int1 坐标Y int2 模板ID	int3 阵营ID	 string1 名字
	NOTICE_WAR_STONE_GIVE						=	-69963,				// 灵石交付				int0 坐标X int1 坐标Y int2 模板ID   int3 阵营ID	 string1 名字
	NOTICE_WAR_FLAG_CUTDOWN						=	-69962,				// 大旗砍倒				int0 坐标X int1 坐标Y int2 模板ID	int3 原阵营ID	int4 新阵营ID int5 资源点 string1 名字
	NOTICE_WAR_FLAG_OCCUPY						=	-69961,				// 据点占领				int0 坐标X int1 坐标Y int2 模板ID	int3 原阵营ID	int4 新阵营ID int5 资源点  string1 名字
	NOTICE_WAR_RESULT_CAMP						=	-69960,				// 局部战果				int0 战场ID int1 获胜阵营ID int 分数
	NOTICE_WAR_RESULT_ALL						=	-69959,				// 总体战果				int0 获胜阵营ID string1 天子名字
	
	// 【注意】 加消息码的时候注意乾坤袋预留的号段！
	NOTICE_OPEN_LUCKYBAG_MIN					=   -69800,				// 乾坤袋专用通知消息下限
	NOTICE_OPEN_LUCKYBAG_MAX					=   -69600,				// 乾坤袋专用通知消息上限
	
	NOTICE_OGREKILLBOSS							=   -69599,				// boss被怪物杀死
	NOTICE_OGREKILLBOSSPRENOTICE     			=   -69598,				// 怪物将要去杀boss的通知
	
	NOTICE_STARTOGRESIEGE						=   -69597,				// 怪物攻城活动开始
	NOTICE_ENDOGRESIEGE							=   -69596,				// 怪物攻城活动结束
	NOTICE_SHOWOGRELIVETIME						=   -69595,				// 显示怪物的存活时间
	NOTICE_GRASSJELLYLIVETIME					=   -69594,				// 仙草种子提示	
	NOTICE_MARSHALTIMELEFT						=   -69593,				// 威风元帅出现倒计时
	NOTICE_MARSHALFLEETIMELEFT					=   -69592,				// 威风元帅出逃倒计时
	NOTICE_GRASSLIVETIMELEFT					=   -69591,				// 仙草2倒计时

    NOTICE_YQS_IRRIGATEPRIZE                    =   -69590,             // 摇钱树活动-玩家XXX浇灌仙露，幸运的获得了神奇的金叶之奖励!
    NOTICE_YQS_IRRIGATE1                        =   -69589,             // 摇钱树活动-摇钱树成长了!你获得了XXX经验奖励和XXX贡献积分!
    NOTICE_YQS_IRRIGATE2                        =   -69587,             // 摇钱树活动-摇钱树成长了!你获得了XXX经验奖励!
    NOTICE_YQS_GROWING1                         =   -69586,             // 摇钱树活动-本阵营的摇钱树即将发芽了，快来浇灌仙露吧
    NOTICE_YQS_GROWING2                         =   -69585,             // 摇钱树活动-本阵营的摇钱树即将开花了，快来浇灌仙露吧
    NOTICE_YQS_GROWING3                         =   -69584,             // 摇钱树活动-本阵营的摇钱树即将成熟了，快来浇灌仙露吧
    NOTICE_YQS_GROWING4                         =   -69583,             // 摇钱树活动-本阵营的摇钱树即将结果了，快来浇灌仙露吧
    //NOTICE_YQS_FINISHING1                       =   -69582,             // 摇钱树活动-30分钟后即将结果
    //NOTICE_YQS_FINISHING2                       =   -69581,             // 摇钱树活动-20分钟后即将结果
    NOTICE_YQS_FINISHING3                       =   -69580,             // 摇钱树活动%s分钟后即将结果
    NOTICE_YQS_FINISHING4                       =   -69579,              // 摇钱树活动-结果，即将掉落奖励

	NOTICE_ACCUSE_LESSPEOPLE					=	-69578,				// 弹劾投票人数不足 int0 阵营ID int1 赞成 int2 反对	string1 国王名字
	NOTICE_ACCUSE_LESSPERCENT					=	-69577,				// 弹劾比例不足		int0 阵营ID int1 赞成 int2 反对	string1 国王名字
	NOTICE_ACCUSE_SUCCESS						=	-69576,				// 弹劾成功			int0 阵营ID int1 赞成 int2 反对	string1 国王名字
	NOTICE_ACCUSE_DIRECT						=	-69575,				// 弹劾直接下台		int0 阵营ID string1 国王名字
	NOTICE_ACCUSE_KILLGATE						=	-69574,				// 城门被杀死	int0 源阵营ID int1 目的阵营ID string1 杀死者名字
	NOTICE_ACCUSE_KILLFLAG						=	-69573,				// 大旗被杀死	int0 源阵营ID int1 目的阵营ID string1 杀死者名字
	NOTICE_ACCUSE_CHANGEPOWER					=	-69572,				// 声望改变		int0 源阵营ID int1 目的阵营ID int2 源声望 int3 目的声望
	NOTICE_ACCUSE_PREPARE						=	-69571,				// 即将开始			int0 阵营ID string1 国王名字
	NOTICE_ACCUSE_BEGIN							=	-69570,				// 弹劾开始			int0 阵营ID string1 国王名字
	NOTICE_ACCUSE_HALFHOUR						=	-69569,				// 半小时公告		int0 阵营ID string1 国王名字
	NOTICE_ACCUSE_TOBE							=	-69568,				// 今晚弹劾			int0 阵营ID string1 国王名字
	NOTICE_ACCUSE_POWERCHANGE					=	-69567,				// 声望改变			int0 原来声望 int1 新声望
	NOTICE_ACCUSE_GATEATTACKED					=	-69566,				// 大门被攻击		int0 大门ID

	NOTICE_KILLOFFICIAL							=	-69565,				// 杀死官员			int0 杀人者阵营 int1 被杀者阵营 int2 国家 int3 官职ID string1 杀人者名 string2 被杀者名
	NOTICE_GETITEM								=	-69564	,			// 获得东西			int0 玩家角色ID int1 如果不为0则为修仙任务等级 int2 玩家获得的物品ID int3 玩家获得的物品的数量 string0 玩家名字
	NOTICE_BATTLE_WINNER						=	-69543,				// 通知战争胜利者	int0 副本ID		int1 阵营ID int2 等级 string1 名字
	NOTICE_BATTLE_NOWINNER						=   -69542,				// 通知战争无胜利者 int0 副本ID     int1 等级

	NOTIFY_OPENITEM_SPECIAL1					=	-69541,				// 开包特殊提示1	int0 CharID	int2 ItemID int3 Num string1 开包人名字
	NOTIFY_OPENITEM_SPECIAL2					=	-69540,				// 开包特殊提示2
	NOTIFY_OPENITEM_SPECIAL3					=	-69539,				// 开包特殊提示3
	NOTIFY_OPENITEM_SPECIAL4					=	-69538,				// 开包特殊提示4
	NOTIFY_BATTLE_ONTIME_OPEN					=	-69537,				// 通知副本按时开启 int0 副本ID		int1 地图ID
	NOTIFY_BATTLE_CLOSE							=	-69536,				// 通知副本结束

	NOTIFY_OPENITEM_SPECIAL5					=	-69535,				// 开包特殊提示5
	NOTIFY_OPENITEM_SPECIAL6					=	-69534,				// 开包特殊提示6
	NOTIFY_OPENITEM_SPECIAL7					=	-69533,				// 开包特殊提示7
	NOTIFY_OPENITEM_SPECIAL8					=	-69532,				// 开包特殊提示8
	NOTIFY_OPENITEM_SPECIAL9					=	-69531,				// 开包特殊提示9
	NOTIFY_OPENITEM_SPECIAL_MAX					=	-69430,				// 开包特殊提示最大值

};

enum BOURSE_CODE
{
	BOURSE_DEC_NOTENOUGH_YB				= -79999,				// 帐户元宝不足
	BOURSE_DEC_NOTENOUGH_MONEY			= -79998,				// 帐户非绑定金不足

	BOURSE_ACCOUNT_MONEY_MUCH			= -79997,				// 帐户金钱过多
	BOURSE_ACCOUNT_YB_MUCH				= -79996,				// 帐户元宝不足

	BOURSE_SUM_MONEY_MUCH				= -79995,
	BOURSE_SUM_YB_MUCH					= -79994,				// 

	BOURSE_MONEY_NOTENOUGH				= -79993,				// 非绑定浇鸩蛔?
	BOURSE_YB_NOTENOUGH					= -79992,				// 元宝不足

	BOURSE_CANNT_FIND_DOC				= -79991,				// 

	BOURSE_BUY_LIST_MORE				= -79990,				// 卖单数量大于或等于15
	BOURSE_SELL_LIST_MORE				= -79989,				// 买单数量大于或等于15
	
	BOURSE_BUY_NOTIFY					= -79988,				// 全服务器卖单通知
	BOURSE_SELL_NOTIFY					= -79987,				// 全服务器买单通知
};

// 铸灵消息码 -80149 ~ -80100
enum ESpiritCode
{
	SPIRIT_OPEN_PRESLOT_NOTOPEN			= -80149,  // 前置孔未开启
	SPIRIT_OPEN_SLOT_ALREADY_OPEN		= -80148,  // 已开启
	SPIRIT_OPEN_LACK_ITEM				= -80147,  // 启灵道具不足
	SPIRIT_CASTING_SLOT_NOTOPEN			= -80146,  // 尚未开孔
	SPIRIT_CASTING_ALREADY				= -80145,  // 已经铸灵
	SPIRIT_CASTING_LACK_ITEM			= -80144,  // 缺少灵珠道具
	SPIRIT_CASTING_TYPE_ERROR			= -80143,  // 灵珠系别不对
	SPIRIT_CASTING_PLAYER_LEVEL_LOW		= -80142,  // 人物等级不够
	SPIRIT_UPGRADE_LACK_ITEM			= -80141,  // 聚灵灵珠数量不够
	SPIRIT_UPGARDE_LACK_UPGARDEIEM		= -80140,  // 聚灵缺少聚灵珠
	SPIRIT_UPGARDE_LACK_MONEY			= -80139,  // 聚灵缺少非绑定金
	SPIRIT_UPGARDE_INSERTNEW_ERROR		= -80138,  // 插入新灵珠失败
	SPIRIT_UPGRADE_ERROR				= -80137,  // 聚灵常规错误
	SPIRIT_DOWNGRADE_ERROR				= -80136,  // 退灵常规错误
	SPIRIT_DOWNGARDE_LACK_DOWNGARDEIEM	= -80135,  // 退灵缺少退灵珠
	SPIRIT_DOWNGARDE_LACK_MONEY			= -80134,  // 退灵缺少非绑定金
	SPIRIT_DOWNGARDE_INSERTNEW_ERROR	= -80133,  // 插入退下的灵珠失败
	SPIRIT_CASTING_ERROR				= -80132,  // 铸灵常规错误
	SPIRIT_UPGARDE_GRADE_ERROR			= -80131,  // 聚灵时真灵阶数错误（已达最高阶等）
};


////////////////////////////////gateserver 24xxx//////////////////////////
enum EGateServer
{
	// 登录网关流程
	EGATE_THESCENE_ISNOT_ONLINE 		= -24001,	// 登录游戏失败, 因为目的场景未开启
	EGATE_TEMPLATE_ISNOTWITH_CLIENT		= -24002,	// 登录游戏失败，因为客户端与服务器的版本不一致
	EGATE_PLAYERTOKEN_ISNOTAS_LOGIN		= -24003,	// 登录游戏失败, 因为登录网关的TOKEN与LOGIN的TOKEN不一致
	EGATE_PLAYER_ISCREATED_BYSCENE		= -24004,	// 登录游戏失败, 场景在准备玩家数据返回失败
	EGATE_ERATING_VERIFY_ENTERGAME_FAILURE	= -24005,	// 登录游戏失败, ERating认证玩家进入游戏失败
	EGATE_ERATING_VERIFY_ENTERGAME_TIMEOUT 	= -24006,// 登录游戏失败, ERating认证玩家进入游戏超时
	EGATE_PLAYER_STILLINGATE_TOLONG		= - 24007,	// 玩家在场景的时间过久，被服务器强迫下线
	EGATE_SCENE_VERIFY_ENTERGAME_TIMEOUT = -24008,
	EGATE_THESCENE_IS_FULLIST			= -24009,
	EGATE_PLAYER_WILL_DISCONNECT 		= -24010,
// 重复登录流程
	EGATE_PLAYERSTATE_ISNOT_PERMITED 	= -24021,	// 登录游戏失败, 当前玩家的状态不允许登录
	EGATE_PLAYERSTATE_IS_REDIRECTING 	= -24022,	// 玩家正在跨场景中, 不允许重复登录
	EGATE_ROLE_ISNOT_LOGINEDROLE 		= -24023,	// 重复登录的角色不是已经在场景的角色
	EGATE_PLAYER_ANOTHER_LOGIN			= -24024,	// 玩家因为其他地方登录，被迫下线
	EGATE_KICKOFFPLAYER_NOTIN_SCENE		= -24025,	// 需要踢掉的玩家不在相应的场景
	EGATE_KICKOFFPLAYERSTATE_NOT_PERMIT	= -24026,	// 需要踢掉的玩家在场景中的状态不允许踢下线
	EGATE_KICKOFFPLAYER_TOOLONG_TIME    = -24027,

	// 创建角色流程
	EGATE_CREATECHAR_ON_DUPNAME 	= -24031,		// 创建角色失败, 因为角色名称重复

	EGATE_CREATECHAR_ON_AGTIMEOUT 	= -24032, 		// 创建角色失败, 因为ERating超时
	EGATE_CREATECHAR_ON_DBTIMEOUT 	= -24033,		// 创建角色失败, 因为MYSQL超时	

	EGATE_CREATECHAR_ON_LISTFULL 	= -24034,		// 创建角色失败，因为角色列表已满

	EGATE_CREATECHAR_ON_NOACCOUNT 	= -24035,		// 创建角色失败，因为玩家帐号不存在

	EGATE_CREATECHAR_ON_DBSQL 		= -24036,		// 创建角色失败，因为数据库SQL执行失败
	EGATE_CREATECHAR_ON_AGDBSQL 	= -24037,		// 创建角色失败, 因为ERating数据库执行失败
	EGATE_CREATECHAR_ON_AGCODE 		= -24038,		// 创建角色失败, 因为ERating返回其它失败


	EGATE_ROLENAME_DOES_EXIST 		= -24039,		// 创建角色失败，因为角色名称已经存在	
	EGATE_ROLESEX_ISNOT_PERMITED 	= -24040,		// 创建角色失败, 因为角色性别非法
	EGATE_ROLENAME_IS_TOOLONG 		= -24041,		// 创建角色失败，因为角色名称过长
	EGATE_ROLENAME_HAS_INVALIDCHAR 	= -24042,		// 创建角色失败, 因为角色名称中含有非法字符

	EGATE_CREATECHAR_ON_AGNOTOPEN 	= -24043,		// 创建角色失败, 因为ERATING 服务未开启
	EGATE_ROLECOUNTRY_ISNOT_PERMITED= -24044,		// 创建角色失败, 因为所选国家代码错误

	// 获取角色列表流程
	EGATE_ROLELIST_ON_DBTIMEOUT	= -24051,		// 获取列表失败，因为MYSQL回应超时
	EGATE_ROLELIST_ON_AGTIMEOUT	= -24052,		// 获取列表失败，因为ERating回应超时
	EGATE_ROLELIST_ON_AGCODE	= -24053,		// 获取列表失败, 因为ERating返回失败
	EGATE_ROLELIST_ON_DBSQL		= -24054,		// 获取列表失败，因为执行MySQL语句失败
	EGATE_ROLELIST_ON_NOACCOUNT	= -24055,		// 获取列表失败, 因为帐号不存在
	EGATE_ROLE_DOESNOT_EXIST	= -24056,		// 角色名无效
	EGATE_ROLE_ENTERGAME_ONDELETESTATUS	= -24057,// 删除等待状态不能进入游戏


	// 删除角色流程
	EGATE_DELETEROLE_ON_USINGROLE 	= -24061,		// 删除角色失败, 因为此角色正在被使用中
	EGATE_DELETEROLE_ON_NOACCOUT 	= -24062,		// 删除角色失败, 因为角色不存在

	EGATE_DELETEROLE_ON_DBSQL 		= -24063,		// 删除角色失败, 因为执行SQL语句失败
	EGATE_DELETEROLE_ON_DBTIMEOUT 	= -24064,		// 删除角色失败, 因为执行SQL语句超时

	EGATE_DELETEROLE_ON_AGTIMEOUT 	= -24065,		// 删除角色失败, 因为ERATING 回应超时
	EGATE_DELETEROLE_ON_AGCODE 		= -24066,		// 删除角色失败, 因为ERATING返回其他错误
	EGATE_DELETEROLE_ON_AGDBSQL 	= -24067,		// 删除角色失败, 因为ERATING返回数据库操作失败
	EGATE_DELETEROLE_ON_AGNOTOPEN	= -24068,		// 删除角色失败，因为ERATING密码认证未开启

	EGAME_SCENESERVER_MAP_NOT_EXIST	= -24083,		// 进入游戏时，地图不存在	
	EGAME_DELETEROLE_ON_FAMILYHEADER= -24082,		// 删除角色失败，家族首领不能删除角色
	EGAME_SCENESERVER_IS_FULLLIST	= -24081,		// 场景服务器满
	EGAME_SCENESERVER_MAP_IS_EMPTY  = -24080,		// 地图创建失败
	
	EGAME_DELETEROLE_DELETEPROTECT  = -24079,		// 删除角色时角色处于删除保护状态
	EGAME_DELETEROLE_DELETECOOLTIME = -24078,		// 删除角色时角色处于删除操作冷却状态
	EGAME_DELETEROLE_DELETESUCCESS  = -24077,		// 删除角色成功，开始处于角色冷却时间
	EGAME_DELETEROLE_WRONGPASSWORD  = -24076,		// 角色密码不正确

   // 100310 MAOCY ADD-BEGIN
   EGATE_ROLE_QUESTION_FAILURE     = -24090,		// 角色登录问题验证失败。
   // 100310 MAOCY ADD-END
};

////////////////////////////////loginserver 21xxx //////////////////////////
enum ELoginServer
{
	ELOGIN_THEWORLD_ISNOT_OPEN			= -21001,		// 登录失败, 世界未开启
	ELOGIN_TEMPLATE_ISNOTWITH_CLIENT	= -21002,		// 登录失败, 模板版本与客户端不一致
	ELOGIN_THEWORLD_IS_FULLLIST			= -21003,		// 登录失败, 世界人数已经达到上限
	ELOGIN_USERNAME_DOESNOT_EXIST		= -21004, 		// 登录失败, 帐号不存在
	ELOGIN_USERPSW_DOESNOT_MATCH		= -21005,		// 登录失败, 密码不匹配
	ELOGIN_USER_IN_OTHERWORLD			= -21006,		// 登出失败, 玩家已经登录到其它游戏世界
	ELOGIN_PLAYER_IS_FROZEN				= -21007,		// 登录失败, 玩家帐号被冻结
	ELOGIN_VERIFY_ON_AGDBSQL			= -21008,		// 登录失败, ERATING校验执行SQL语句失败, 使用本地认证
	ELOGIN_VERIFY_ON_AGCODE				= -21009,		// 登录失败, ERATING校验未通过, 不能使用本地认证
	ELOGIN_PLAYER_IN_LOGINING			= -21010,		// 登录游戏失败, 因为玩家正在登录过程中	
	ELOGIN_PLAYER_IN_RELOGINING			= -21011,		// 登录游戏失败, 玩家正在重复登录过程中
	ELOGIN_DBSESSION_IS_FULL			= -21012,		// 登录游戏失败, 因为目前系统允许的与数据库的Session达到上线
	ELOGIN_VERIFY_ON_DBSQL				= -21013,		// 登录游戏失败, 数据库执行验证的SQL语句未执行成功
	ELOGIN_PLAYER_IN_GAMING				= -21014,		// 登录游戏失败，因为玩家已经在游戏中
	ELOGIN_VERIFY_BYMYSQL_TIMEOUT		= -21015,		// 登录游戏失败, 因为本地验证超时
	ELOGIN_VERIFY_BYERATING_TIMEOUT		= -21016,		// 登录游戏失败, 因为本地验证超时
	ELOGIN_CLIENT_VERSION_NOTMATCH 		= -21017, 		// 客户端版本与服务器不匹配
	ELOGIN_SERVER_BE_MAINTENANCE		= -21018,		// 服务器维护中，暂时不能登录
	ELOGIN_CLIENT_MD5CODE_NOTMATCH 		= -21019, 		// 客户端传入的MD5不正确
	ELOGIN_CLIENT_NEED_MATRIXCODE		= -21020,		// 需要客户端传入动态密保码
	ELOGIN_CLIENT_NEED_STATICMATRIXCODE	= -21021,		// 需要客户端传入密保卡
	ELOGIN_CLIENT_NOTIN_LIMITIP			= -21022,		// 没有在绑定IP地址登陆
};
///////////////////////////////

// 全局服务器23xxx
enum EDungeonErr
{
	EDUNGEON_ERROR_LEVELLOW			= -23999,		// 报名时等级不够
	EDUNGEON_ERROR_SERVERNOTOPEN	= -23998,		// 报名时全局副本服务器没开
	EDUNGEON_ERROR_NOTAPPLIED		= -23997,		// 没有报名不能进入跨服战场
	EDUNGEON_ERROR_HASAPPLIED		= -23996,		// 已经报名不能进入战场
	EDUNGEON_ERROR_MAXAPPLYNUM		= -23995,		// 报名人数已达上限
	EDUNGEON_ERROR_APPLYTIMEOUT		= -23994,		// 进入超时
	EDUNGEON_ERROR_CREATEREPETIONERR= -23993,		// 无法创建战场
	EDUNGEON_ERROR_MAPFULL			= -23992,		// 地图已满
	EDUNGEON_ERROR_NOPLACEACTION	= -23991,		// 此地图无法进行此操作
	EDUNGEON_ERROR_CANNOTGLOBALTELE = -23990,		// 当前区组玩家不能传送到全局服务器
	EDUNGEON_ERROR_DEATH			= -23989,		// 死亡状态不能进入跨服战场
	EDUNGEON_ERROR_HASESCAPEBUFF	= -23988,		// 有逃逸buff不能报名
	EDUNGEON_ERROR_HASTIMELIMITBUFF	= -23987,		// 有时间限制的buff不能报名(西行卫士buff)
};

enum ChangeCamp
{
	ERROR_CHANGE_CAMP_NO_HAVE_ITEM			= -80001,	//判断是否有转阵营道具	
	ERROR_CHANGE_CAMP_IN_CD					= -80002,	//判断是否有转阵营cd
	ERROR_CHANGE_CAMP_IN_SAME				= -80003,	//是否 是 转入原来阵营
	ERROR_CHANGE_CAMP_IN_BEST				= -80004,	//判断转入的是否是最强阵营
	ERROR_CHANGE_CAMP_IN_TEAM				= -80005, 	//判断是否有组队
	ERROR_CHANGE_CAMP_IN_FAMILY				= -80006,	//判断有家族
	ERROR_CHANGE_CAMP_IN_COUNTRY			= -80007,	//判断是否有国家	
	ERROR_CHANGE_CAMP_IN_MASTER_PRENTICE	= -80008,	//判断是否有师徒
	ERROR_CHANGE_CAMP_IN_MARRIAGE			= -80009,	//判断是否有结婚
	ERROR_CHANGE_CAMP_IN_PAYMAIL			= -80010,	//判断是否有付费邮件
	ERROR_CHANGE_CAMP_IN_AUCTION			= -80011,	//判断是否有拍卖
	ERROR_CHANGE_CAMP_NO_CAMP				= -80012,   //原始无阵营 不让转让
	ERROR_CHANGE_CAMP_IN_TRDE				= -80013,   //正在交易，无法转阵营
	ERROR_CHANGE_CAMP_NO_ENOUGH_LEVEL       = -80014,   //等级没有达到30级以上
	ERROR_CHANGE_CAMP_INSWORNTEAM			= -80015,   //有结拜不能转阵营

};
//符文升级
enum  RuneCom
{
	ERROR_RUNE_COM_LOSE				 = -80050,//升级符文 失败
};
//转换符文失败
enum  RuneChange
{
	ERROR_RUNE_CHANGE_ERROR			 = -80060,//转换符文失败
	ERROR_RUNE_STORE				 = -80061,//背包符文不让转换
};

#endif









