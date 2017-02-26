#pragma once

enum TEAM_DISTRIBUTE			// 队伍物品分配模式
{
	ALTERNATION			=			0x1001,				// 轮流分配
	RANDOM,												// 自由分配
	PROBABILITY,										// 概率分配
};

enum CONSCRIBE_TYPE				// 求组招募的主类型
{
	TYPE_SUBPALACE		=			0x1003,				// 地宫
	TYPE_COPY,											// 副本
	TYPE_TASK ,											// 任务
	TYPE_ACTIVITY,										// 活动
	TYPE_COMMON,										// 普通
	TYPE_PVP,											// PVP
};

enum AVLIVE_STATE				// 玩家线上状态
{	
	emOnLine			=	0xA001,						// 在线, 在地图中
	emOffLine,											// 离线, 离开游戏
	emDownLine,											// 掉线
	emLogonAgain,										// 重登录
	emRedirect,											// 跨场景
	emItemDead,											// 队员死亡
	emOverDistance,										// 超出有效距离
};

enum MEMBER_TYPE
{
	emTeamHead			=   0,
	emTeamItem			=	1,
};

enum MSG_TYPE
{
	TEAM_ASKFOR			=	0x3001,				// 求组信息
	TEAM_CONSCRIBE,
	TEAM_INITWND,								// 初始求组招募窗口
	TEAM_CLAIMINIT,								// 求组窗口初始化
	TEAM_RECRUITINIT,							// 招募搜索初始化
};

enum ACTIVE_RTN						// 请求的返回结果
{
	emRefuse		=	0,					// 拒绝
	emAgree			=	1,					// 同意
	emIgnore		=	2,					// 忽略
};

#define TEAM_ADD							0x100A		// 新增成员
#define TEAM_DEL							0x100B		// 删除成员
#define TEAM_HEAD							0x100C		// 队长标志
#define TEAM_UPDATE							0x100D		// 更新队伍

#define TEAM_SETUP							0x100E		// 队伍成立
#define TEAM_OVER							0x100F		// 队伍解散

#define _TEAM_INVITE_						0x0100		// 邀请动作
#define _TEAM_INVITE_RTN_					0x0200		// 邀请返回动作
#define _TEAM_REQUEST_						0x0300		// 申请动作
#define _TEAM_REQUEST_RTN_					0x0400		// 申请返回动作
#define _TEAM_CLAIMJOIN_					0x0500		// 求组动作
#define _TEAM_RECRUIT_						0x0600		// 招募动作
#define _TEAM_ATTORN_HEADER_				0x0700		// 转移队长动作
#define _TEAM_ALTER_DISTRIBUTE_				0x0800		// 改变分配模式动作
#define _TEAM_LEAVE_TEAM_					0x0900		// 离开队伍
#define _TEAM_END_TEAM_						0x0A00		// 解散队伍
#define _TEAM_TICK_PLAYER_					0x0B00		// 踢出玩家
#define _TEAM_FOLLOW_ORDER_					0x0C00		// 队伍跟随
#define _TEAM_ASK_FOLLOW_					0x0D00		// 请求跟随
#define _TEAM_CREATETEAM_					0x0E00		// 创建队伍
#define _TEAM_OPERATE_MASK_					0x0F00		// 高位掩码
#define _TEAM_NOT_INSCENE_					0x1000		// 不在本场景
#define _TEAM_COOLING_TIME_					0x1100		// 冷却时间		


#define _TEAM_SUCCESS_						0x0001		// 操作成功					
#define _TEAM_FAILURE_						0x0002		// 操作失败					
#define _TEAM_REFUSE_						0x0003		// 拒绝操作	
#define _TEAM_PLAYER_ADD_					0x0004		// 添加成员
#define _TEAM_PLAYER_DEL_					0x0005		// 删除成员
#define _TEAM_HEADER_						0X0006		// 队长标志
#define _TEAM_PLAYER_NONE_					0x0007		// 玩家不存在				
#define _TEAM_PLAYER_BUSY_					0x0008		// 玩家忙碌中
#define _TEAM_NOT_FOUND_					0x0009		// 队伍不存在
#define _TEAM_FULL_							0x000A		// 队伍已满
#define _TEAM_IN_TEAM_						0x000B		// 玩家在队伍中
#define _TEAM_HEADER_NONE_					0x000C		// 不是队长
#define _TEAM_OUT_TEAM_						0x000D		// 玩家不在队伍中
#define _TEAM_REPEATED_MSG_					0x000E		// 重复发送消息
#define _TEAM_NEW_HEADER_					0x000F		// 成为新队长
#define _TEAM_ON_LINE_						0x0010		// 玩家在线
#define _TEAM_DOWN_LINE_					0x0012		// 玩家掉线
#define _TEAM_MAP_FULL_						0x0013		// 维护的列表已满
#define _TEAM_LEVEL_LOW_					0x0014		// 玩家级别不够
#define _TEAM_MORE_DISTANCE_				0x0015		// 超出有效距离
#define _TEAM_DIFFNATION_					0x0016		// 不同国籍

enum TeamOrderType					// 组队命令
{
	emCancelFollow			= 0,		// 取消跟随
	emOrderFollow			= 1,		// 命令
};

#define TEAM_NUMBER			6
#define TEAM_MSG_LEN		32
#define MAX_INVITE_NUM		6		//最大邀请、申请数
#define MAX_RECINVITE_NUM	5		// 玩家收到的最多邀请
#define PAGE_MAX			8		// 每页显示的最多信息数
#define INVALID_DISTANCE	15		// 
#define TEAM_ORDER_FIRST    1      // 队伍中的序号，从1开始


enum TEAM_UPDATE_TYPE
{
	ACTION_JOIN			= 0,
	ACTION_LEAVE,
	ACTION_KICK,
	ACTION_ENDTEAM,
	ACTION_CHANGEHEADER,
	ACTION_UPATEFLAG,
};

enum CLAIM_RECRUIT_TYPE
{
	TYPE_CLAIM			= 0,
	TYPE_RECRUIT,
};

enum FOLLOW_TYPE
{
	FOLLOW_REQUEST		= 0,
	FOLLOW_ORDER,
};

enum InvReqType
{
	Type_Inv			= 0,
	Type_Req,
};
