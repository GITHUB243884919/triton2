#ifndef _GM_COMMAND_H_
#define _GM_COMMAND_H_

#include <string>
#include <list>
#include "lk_string.h"
#include "google/protobuf/message.h"
#include "message_pb.hxx.pb.h"
#include "message_factory.h"
#include "log_msg_pb.hxx.pb.h"


#define CMD_RESPONSE_HEAD "HTTP/1.1 200 OK\nDate: %s\nServer: XiYou(%d)\nSession-ID: %d\n\
Content-Length: %d\nConnection: close\nContent-Type: text/xml\n\n%s\n"

//args[0], Response TimeStampt like Tue, 16 Sep 2008 07:41:43 GMT
//args[1], WorldID
//args[2], Session-ID is %d, like 1234567
//args[3], Content-Length TODO: I DONOT know
//args[4], command-Result XML String

#define MAX_HTML_LENGTH 2048000
#define SHORT_LENGTH 2048

typedef lk::string<MAX_HTML_LENGTH> CMaxString;
typedef lk::string<SHORT_LENGTH> C2KString;

typedef std::vector< std::pair<std::string, std::string> > replace_t;
typedef const char* (*PF_ID2NAME) ( int );
typedef struct proto_func_t{
	const char* name;
	PF_ID2NAME method;
}proto_func_t;


void protobuf2xml(const Message& pmessage, CMaxString& debug_string, 
		void (*pformat)(CMaxString&, const char*, const char*, const char*));

int code_convert(const char* from_charset, const char* to_charset, char* inbuf, size_t inlen, char* outbuf, size_t* outlen);

int DecodeFromGMStream(int nCommHandle, char* szStream, CMessage* tpMessage);
int FlashToGMStream(CMessage* tpMessage, char* szStream, 
		unsigned int StreamLength, int nSessionID, int(*pfflash)(CMessage*, CMaxString&));


int FlashGmCmdNormalResult(CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdRoleDetailResult(CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdUserStorageInfoResult(CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdRoleLocationResult(CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdNpcDetailResult(CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdTaskInfoResult(CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdRoleProtoResult(CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdUserSocialInfoResult(CMessage* tpMessage, CMaxString& xmlbuf);

int FlashGmCmdIbInfoResult(CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdSqlDmlResult( CMessage* tpMessage, CMaxString& xmlbuf);

int FlashGmCmdRoleEquipmentResult( CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdRolePackInfoResult( CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdRoleItemInPackResult( CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdRoleEquipmentDetailResult( CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdRoleStorageDetailResult( CMessage* tpMessage, CMaxString& xmlbuf);

int FlashGmCmdRoleSkillInfoResult( CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdRoleDetailResult( CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdRoleSkillDetailResult( CMessage* tpMessage, CMaxString& xmlbuf);

// 2009-07-01 新增日志查询
int FlashGmCmdGoodAtNpcResult( CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdGoodAtUsersResult( CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdGoodHistoryResult( CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdOperationDetailResult( CMessage* tpMessage, CMaxString& xmlbuf);

// 2010-02-03 重设玩家安全密码
int FlashGmCmdResetRolePassword( CMessage* tpMessage, CMaxString& xmlbuf);

// 2010-02-05
int FlashGmCmdRoleItemOperateResult( CMessage* tpMessage, CMaxString& xmlbuf );
int FlashGmCmdRoleMoneyOperateResult( CMessage* tpMessage, CMaxString& xmlbuf );
int FlashGmCmdRoleExpOperateResult( CMessage* tpMessage, CMaxString& xmlbuf );
int FlashGmCmdItemUpgradeResult( CMessage* tpMessage, CMaxString& xmlbuf );

// ItemID2Name
const char* ItemID2Name(int nItemID);

// TaskID2Name
const char* TaskID2Name(int nTaskID);

// NpcID2Name
const char* NpcID2Name(int nNpcID);

// Desc Item Action
template <int max_width>
lk::string<max_width>& DescItemAction(lk::string<max_width> & desc, const replace_t& repl, const proto_func_t*  func);

// CorpsTotal
int FlashGmCmdCorpsTotalResult(CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdCorpsDetailResult(CMessage* tpMessage, CMaxString& xmlbuf);

int FlashGmCmdCorpsLogResult(CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdFamilyLogResult(CMessage* tpMessage, CMaxString& xmlbuf);

int FlashGmCmdQueryDeathResult(CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdQueryLoginResult(CMessage* tpMessage, CMaxString& xmlbuf);
int FlashGmCmdQueryTaskResult(CMessage* tpMessage, CMaxString& xmlbuf);

// RocoverRoleItem
int FlashGmCmdRecoverRoleEquipResult(CMessage* tpMessage, CMaxString& xmlbuf);

// 查询邮件内容
int FlashGmCmdRoleMailResult(CMessage* tpMessage, CMaxString& xmlbuf);

// 查询玩家称号
int FlashGmCmdRoleTitleResult(CMessage* tpMessage, CMaxString& xmlbuf);

// 查询玩家的等级信息
int FlashGmCmdRoleLevelInfoResult(CMessage* tpMessage, CMaxString& xmlbuf);

// 查询在线玩家
int FlashGmCmdUserInfoByIPResult(CMessage* tpMessage, CMaxString& xmlbuf);

// 查询离线玩家
int FlashGmCmdQueryLogoutIPResult(CMessage* tpMessage, CMaxString& xmlbuf);

int FlashGmCmdQueryStockResult(CMessage* tpMessage, CMaxString& xmlbuf);

// 查询元宝摆摊交易记录
int FlashGmCmdQueryYBStallResult(CMessage* tpMessage, CMaxString& xmlbuf);

enum 
{
	//RoleDetail 发到GateServer, GateServer从RoleInfo中取得玩家的信息，直接返回; LoginServer 构造后返回给gmServer
	//UserStorageInfo 发送到GateServer, 如果玩家在线，向玩家所在Server取得玩家的仓库buffer；
					//如果玩家不在线，从数据库取得玩家仓库数据
	//RoleLocation, 流程同上
	//UserLevelInfo 发送到日志服务器，日志服务器从日志数据库查询后返回
	//UserSocialInfo 玩家的信息查询同UserStorageInfo的查询流程，玩家的社会关系变更走日志服务器的查询流程
	//CorpsTotal 暂缓
	//CorpsDetail
	//CorpsMember
	//GoodsHistoryToNPC 直接走日志服务器查询流程
	//GoodsHistoryFromNPC
	//GoodsHistoryBetweenUsers
	//GoodsHistory
	//PetHistory
	//
	//OperationDetail 角色操作记录(so much)
	//GoodsDropLog 玩家的物品的掉落日志查询, 从日志服务器查询，分别查询物品的掉落日志，和玩家的物品获得?这个地方怎么关联?
	  // 没有唯一的ID，除非游戏世界保持物品始终是唯一的, 目前的唯一是指如果不重新启动。
	//TaskInfo 从游戏服务器获得玩家的任务信息
	//LoginInfo 直接从日志服务器查询
	//OnlineTime
	//IpInfo
	//UserInfoByIp
	//AttrChangeHistory 
	//
	//ShutUp 直接发给游戏服务器路由消息, 路由服务器转发到对应场景, 如果玩家不在，返回错误代码
	//CancelShutUp
	//KickRole
	//FreezeAccount 如果玩家在线，踢人，冻结帐号
	//UnfreezeAccount, 
	//TrusteeUser 对应游戏中的什么东东？
	//UntrusteeUser
	//MoveRole
	//EndTask
	//TrusteeStorage, UntrusteeStorage 
	//RecoverRole, 不明白GMServer想干什么
	//角色转服 手工完成
	//PostBulletin 发布公告
	//MultiExp 双倍经验设置
	//CancelMultiExp 取消双倍经验
	//ChangeMoney ChangeExp ChangeItem 金钱，经验补偿
	//EndFight 不知道干啥，又不是回合制游戏
	//DeleteRole 
	//ClearBag 清空玩家包裹
	//ChangePet 宠物补偿 
	//PostNews  发布新闻
	//SetOLGMAccount  设置线上GM账号
	//IbInfo 商城物品显示
	//SetIbPrice SetIbLocation IbOnSale IbNotSale IbDescript  
	//IpBind 
};

#endif

