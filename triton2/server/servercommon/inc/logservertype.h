#ifndef LOGSERVERTYPE_H
#define LOGSERVERTYPE_H

#define MAX_TASK_ITEM_NUM	5	// 任务奖励最大物品种类数量

#define MAX_SQL_LENGTH	1024*30*100
#define MAX_TABLENAME_LENGTH	1024

// 按照事件类型进行分表
enum emEventType
{
	EM_EVENTTYPE_NONE				= 0,	// 事件类型
	EM_EVENTTYPE_OBTAINEXP,					// 获得经验事件
	EM_EVENTTYPE_TASKOVER,					// 完成任务事件
	EM_EVENTTYPE_PLAYERDIE,					// 玩家死亡事件
	EM_EVENTTYPE_ENTERFB,					// 进入副本事件
	EM_EVENTTYPE_EXITFB,					// 离开副本事件
	EM_EVENTTYPE_OBTAINITEM,				// 获得物品事件
	EM_EVENTTYPE_OBTAINMONEY,				// 获得金币事件
    EM_EVENTTYPE_OBTAINBUFF,                // 获得BUFF事件
};

// 每天添加一个事件,需要填写对应的HANDLE号并在后面加以注释
// 需要添加HANDLE,需要修改log服务器上的相应宏

#define CONTAINER_ITEM_NUM		16		// 容器日志分表总数
#define MAX_HANDLE_NUM			8		// 最大日志线程数

enum emEventHandle
{
	EIHANDLE_ONE		= 0,	// 1号handle (记录获得经验)
	EIHANDLE_TWO,				// 2号handle (玩家死亡)
	EIHANDLE_THREE,				// 3号handle (获得金币)
	EIHANDLE_FOUR,				// 4号handle (获得物品)
	EIHANDLE_FIVE,				// 5号handle (进入副本)
	EIHANDLE_SIX,				// 6号handle (退出副本)
	EIHANDLE_SEVEN,				// 7号handle (任务完成)
	EIHANDLE_EIGHT,				// 8号handle
};


/*
 * 时间，地点应该是默认就有的，因此不用在行为参数中记录
 * 模式（获得，失去)  行为（可枚举） 行为参数(3个，每种行为不一样，按照重要性排序)
 * 新增加的行为必须定义，否则数据库不可管理
 *
 */



enum emExchangeOperate
{
	EXCHANGE_ACCOUNT_ADD			= 0,	// 交易行帐号添加
	EXCHANGE_ACCOUNT_DEC,					// 交易行帐号减少
	EXCHANGE_TRADE_BUY_ADD,					// 交易行添加买单
	EXCHANGE_TRADE_BUY_DEC,					// 交易行取消买单
	EXCHANGE_TRADE_SELL_ADD,				// 添加卖单
	EXCHANGE_TRADE_SELL_DEC,				// 取消卖单
	EXCHANGE_TRADE_LOG_BUY,					// 买单成交
	EXCHANGE_TRADE_LOG_SELL,				// 卖单成交
};


#endif
