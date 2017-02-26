#pragma  once 

#define  WEDDING_TIME_DUARING 360		// 婚期的间隔为6分钟			   
#define  WEDDING_VALIDETIME	900			// 婚礼开始后可以举行婚礼的有效时间
#define  DEFAULT_WEDDINGTIMELENGTH	1800	// 默认的婚礼时长
#define  WEDDING_TIPSTIME		3600	// 提醒时长
#define  DEFAULT_WEDDINGMAXTIMELENGTH 3600	// 婚礼的最大时长

#define  WENDDINGDAYS	3				// 婚期可选距离当前时间几天内的时间
#define  DAYSECOUNDS	86400			// 一天的秒数
#define  MARRIAGELOADNUM	30			// 婚姻信息一次获取的总数
#define  MAX_WEDDNGTIMEAREA_NUM	 20		// 最大的婚礼时间段数目

#define MARRY_APPLY_VALIDTIMELENGTH	300000	//	申请的有效时间长度300秒   
#define WEDDING_ONSTREET_TIMELENGTH  600	// 在婚礼游行的时间长度(600秒)	
#define SYSMAIL_INVITATION_ID 31			// 发送邮件使用的系统邮件ID 
#define  WEDDING_NOTIFY_INTERVAL	20		// 发送结婚通知的时间间隔(秒)
#define  VALIDTIMELENGTH_TOWEDING	600		// 选择婚礼时间时，婚礼举行时间距离现在有效时长

#define  WEDDINGNPC_TIMER_LENGTH	300			// 检查婚礼npc的时间周期
#define  MARRIAGESTATUS_TIMER_LENGTH	3000	// 检查婚姻状态npc的时间周期
#define  BAITANGSTATUS_TIMER_LENGTH	10000		// 检查拜堂情况的计时器

//  最大的npc路径点
#define  MAX_WEDDINGNPCPATH_STEPS 300	

//  婚礼的最大定时器个数
#define MAX_WEDDING_TIMERNUM	20		   


// 结婚状态
enum EM_MarriageStatus
{
	MARRIAGESTATUS_NOTMARRIED		= 0,		// 未婚状态
	MARRIAGESTATUS_MARRY			= 1,		// 结婚
	MARRIAGESTATUS_WEDDINGINWAIT	= 2,		// 等待婚礼的举行
	MARRIAGESTATUS_WEDDINGONSTREET	= 3,		// 婚礼游行状态
	MARRIAGESTATUS_WEDDINGONHALL	= 4,		// 礼堂婚礼
	MARRIAGESTATUS_WEDDINGEND		= 5,		// 婚礼结束
	MARRIAGESTATUS_WEDDINGFAILED	= 6,		// 设置婚礼时间但是未能举行婚礼
};

// 婚礼状态
enum EM_WeddingStatus
{
	WEDDINGSTATUS_NOTSET		=	1,			// 婚礼未安排
	WEDDINGSTATUS_TIMENEAR		=	2,			// 婚礼时间即到(一小时)
	WEDDINGSTATUS_INTIME		=	3,			// 已经在婚礼时间里面( 婚礼开始10分钟之内 )
	WEDDINGSTATUS_EXPIRED		=	4,			// 婚礼已经过期( 已经超过婚礼开始时间10分钟 )
	WEDDINGSTATUS_NOTINTIME		=	5,			// 婚礼时间未到(距离婚礼开始大于1小时)
	WEDDINGSTATUS_FINISHED		=	6,			// 婚礼已经结束(举行完毕或者未举行)
};

enum EKeepsakeStatus
{
	KEEPSAKSTATUS_EMPTY			=	0,			// 信物未放置状态
	KEEPSAKESTATUS_PUT			=	1,			// 信物处于放置状态
	KEEPSAKESTATUS_EXCHANGE		=	2,			// 信物处于交换状态
};

// 婚礼的一些时间状态信息
enum Em_WeddingEventStatus
{
	EVENT_WEDDINGNOTIFY			=	( 1 ),					// 婚礼提醒的事件
	EVENT_WEDDINGINTIMENOTIFY	=	( 1 << 1 ),				// 已经在婚期的事件
	EVENT_WEDDINGBAITANG		=	( 1 << 2 ),				// 婚礼拜堂
	EVENT_BAITANGEND			=	( 1 << 3 ),				// 拜堂结束的事件
	EVENT_EXCHANGEKEEPSAKE		=	( 1 << 4 ),				// 交换信物状态
}; 

// 婚礼的开始类型
enum EWeddingStartType
{
	WEDDINGSTARTTYPE_PLAYER	=	1,		// 玩家开始婚礼
	WEDDINGSTARTTYPE_SYS	=	2,		// 系统开始婚礼
};

