#pragma once 

#define MAX_MASSAGE_LEN 512			  // 消息最大长度
#define MAXBUGLECHATNUM 6000
#define MAXAUTOCHATNUM  3000
#define MAX_ITEMBUFF_LENGTH	2048	  // 聊天信息发送物品信息的buff长度
#define MIN_COUNTRYCHANNEL_LEVEL 10   // 国家频道的发言最低等级 
#define MAX_CHATUSER_NUM	30		  // 聊天玩家身份的最大数量

typedef enum 
{
	CHAT_SUCCESS = 1,                 // 发送消息成功
	CHAT_HIGH_FREQUENCY,		      // 发言频率过快
	CHAT_MSGTIME_FAT,				  // 一句话重复发送次数过多，
	CHATDES_UNACHEIEVE,				  // 发送消息失败
	CHAT_NOTONOUGHPROP,               // 道具不够（千里传音，狮子吼，世界号角，国家号角）
	CHAT_STARTAUTOCHAT,               // 开始自动喊话
	CHAT_STOPSAUTOCHAT,               // 停止自动喊话
	CHAT_MODIFYAUTOCHATOK,            // 修改自动喊话消息内容成功
	CHAT_WORLDCHANELLEVELLOW,         // 世界频道发言等级不够
	CHAT_SENDAUTOCHAT,                // 发送一次自动喊话
	CHAT_CANSENDBUGLEMESS,            // 通知客户端已经将喊话道具锁定，可以打开喇叭频道输入界面
	CHAT_DESPLAYERINBLACK,            // 密语发送对象在黑名单中
	CHAT_FORBIDDEN,	

} CHAT_RES2CLIENT;

enum CHAT_CHANNEL
{
	CHANNEL_ERR		=	   ~0,						//错误
	CHANNEL_VOID	=		0,						//空频道
	CHANNEL_COMMON	=	   ( 1 << 0 ),				//普通
	CHANNEL_WORLD	=	   ( 1 << 1 ),				//世界 ，需要世界号角道具
	CHANNEL_P2P		=	   ( 1 << 2 ),				//私聊
	CHANNEL_CORPS	=	   ( 1 << 3 ),				//军团
	CHANNEL_FAMILY	=	   ( 1 << 4 ),				//家族
	CHANNEL_TEAM	=	   ( 1 << 5 ),				//组队
	CHANNEL_COUNTRY	=	   ( 1 << 6 ),				//国家
	CHANNEL_GM		=	   ( 1 << 7 ),				//GM
	CHANNEL_ECONOMY =	   ( 1 << 8 ),				//经济
	CHANNEL_BEGTEAM =	   ( 1 << 9 ),				//团队
	CHANNEL_ALLIED	=	   ( 1 << 10 ),			    //盟国
	CHANNEL_BUGLE   =      ( 1 << 11 ),             //喇叭频道发言，需要千里传音道具
    CHANNEL_OTHERCOUNTRY=  ( 1 << 12 ),             //他国频道发言，需要国家号角道具
	CHANNEL_AUTOMSG =      ( 1 << 13 ),             //自动喊话，需要狮子吼道具
	CHANNEL_ALL     =      ( 1 << 14 ),				// 所有频道发言，供禁言用
	CHANNEL_SYS		=	   ( 1 << 15 ),				// 系统频道，发布系统公告用
	CHANNEL_WAR		=      ( 1 << 16 ),				// 战争频道
	CHANNEL_CURRENT	=	   ( 1 << 17 ),				// 当前频道	
	CHANNEL_BATTLE	=	   ( 1 << 21 ),				// 战场频道	
	CHANNEL_END		=      ( 1 << 22 ),				// 频道结束
};
// ********************************************************************** //
// 屏蔽词检查结果
// ********************************************************************** //
enum CheckResult 
{
	HAS_DIRTYWORD     = 0,                         // 没有非法词
	HAS_NO_DIRTYWORD  = 1                          // 含有非法词
};

#define CHAT_IN_FORBIDDENCHANNEL(  mForbiddenChannel, vChannel )	( ( ( mForbiddenChannel ) & ( vChannel ) ) != 0 )

enum ETIPS_TYPE
{
	TPYE_OFFICE	=	1,
	TPYE_TITLE	=	2,		
};
