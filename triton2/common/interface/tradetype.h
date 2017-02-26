#pragma once

#include "protype.h"

#define TRADE_COUNT_MAX			6			// 交易物品列表最大值	
#define TRADE_LENGTH_MAX		10			// 有效距离
#define SERVER_TRADEUNIT_MAX	1000		// 单个服务器运行时最多支持的交易行为
#define INVITE_UPPER_LIMIT		20			// 邀请人数上限

#define TRADE_MONEY_MAX			900000000	// 最大交易金额
#define TRADE_MONEY_MIN			0			// 最小交易金额

#define TRADE_NONESTATE			-1			// 无状态

// 交易行为类型
enum TRADE_TYPE
{
	emAddGoods	= 0x1001,		// 添加物品
	emDelGoods,					// 移除物品
	emUpdateMoney,				// 更新交易金额
};

// 玩家交易状态
enum TRADE_STATE
{
	emInTrade	= 0x2001,		// 交易中
	emOffTrade,					// 无交易,交易结束
	emLockTrade,				// 交易锁定,等待确认
	emWaitTrade,				// 确认交易
	emNoneState,				// 无状态
	emUnlockTrade,			//解锁状态
};

// 操作码
enum TRADE_OPERATE_CODE
{
	emTradeAgree = 0x3001 ,		// 同意
	emTradeRefuse,				// 拒绝
	emTradeIgnore,				// 忽略
};

// 关闭交易情形
enum TRADE_CLOST_TYPE
{
	emNormalFinish = 0x4001,	// 正常结束
	emInterrupt,				// 异常中断
	emCancelTrade,				// 取消交易
};

// 
enum TRADE_PROCESS_CODE
{
	emCanDoTrade = 0x5001,		// 可以交易
	emNeedTarget,				// 对待对方确认
};

struct _TRADE_GOODS_
{
	unsigned int mPackIndex;
	unsigned int mCount;
	char mGoodsBuf[ ITEMOBJ_BUFFLEN ];
};


