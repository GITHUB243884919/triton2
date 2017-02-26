#ifndef _GLOBAL_ENTITY_H_
#define _GLOBAL_ENTITY_H_

#include "object.h"
#include "coretype.h"
#include "log.h"
#include "servertool.h"
#include "dbinterface.h"
#include "lk_string.h"
#include "coretype.h"

#define GS_HOST_MAXLENGTH     40
#define GS_PASSPORT_MAXLENGTH 40
#define GS_PASSWORD_MAXLENGTH 40
#define GS_TOKEN_MAXLENGTH    40
#define GS_LOGIN_MAXCOUNT     32
#define GS_PLAYER_MAXCOUNT    1024*64
#define GS_SOCKET_RECEIVE     1024*1024*4
#define GS_SOCKET_SEND        1024*1024*4
#define PLAYERS_AS_IDDLE    4000    // 空闲
#define PLAYERS_AS_BUSY     7000    // 忙碌
#define PLAYERS_AS_RUSH     10000   // 繁忙
//#define PLAYERS_AS_FULL             //  火爆

//============================================================
// <T>服务器状态类型</T>
//============================================================
enum EGlobalLoginType{
   EGlobalLoginType_Unused, // 未使用(U)
   EGlobalLoginType_Normal, // 正式(N)
   EGlobalLoginType_New,    // 新服(W)
};
//============================================================
class RGlobalLoginType{
public:
   static EGlobalLoginType Parse(int value);
   static const char* ToString(EGlobalLoginType value);
};

//============================================================
// <T>服务器状态类型</T>
//============================================================
enum EGlobalLoginStatusType{
   EGlobalLoginStatusType_System, // 系统(S)
   EGlobalLoginStatusType_Custom, // 控制(C)
};
//============================================================
class RGlobalLoginStatusType{
public:
   static EGlobalLoginStatusType Parse(int value);
   static const char* ToString(EGlobalLoginStatusType value);
};

//============================================================
class RGlobalLoginStatus{
public:
   static EServerGatewayStatus Parse(int value);
   static const char* ToString(EServerGatewayStatus value);
};

//============================================================
// <T>网关信息。</T>
//============================================================
class CGlobalLogin: public CObj{
public:
   bool                   used;                    // 使用标识
   int                    handle;                  // 网络句柄 (socket fd)
   int                    id;                      // 编号      (他们定义的Server Id)
   char                   host[GS_HOST_MAXLENGTH]; // 主机名称
   int                    port;                    // 端口
   EGlobalLoginType       type;                    // 类型
   EGlobalLoginStatusType statusType;              // 状态类型
   EServerGatewayStatus     status;                  // 状态
   int                    playerTotal;             // 玩家总数
   int                    playerCurrent;           // 玩家在线数
   int                    loginCount;              // 认证中次数
   CTCPSocket<RECVBUFLENGTH, POSTBUFLENGTH> socket; // 网络链接
public:
	CGlobalLogin();
   ~CGlobalLogin();
   int Initialize();
   int Resume();
public:
   EServerGatewayStatus GetStatus();
};


//============================================================
// <T>全局用户网关信息。</T>
//============================================================
typedef struct _SGlobalPlayerGateInfo{
   int loginId;   // 登录服务器编号
   int roleCount; // 角色个数
} SGlobalPlayerGateInfo;

//============================================================
// <T>全局用户信息。</T>
//============================================================
class CGlobalPlayer: public CObj{
public:
	bool                  used;
   int                   accountId;                       // 账号编号
   char                  passport[GS_PASSPORT_MAXLENGTH]; // 通行账号
   char                  password[GS_PASSWORD_MAXLENGTH]; // 通行密码
   uint32                token;									 // 临时令牌
   int                   tokenTimeout;                    // 令牌超时
   int                   gateCount;                       // 网关个数
   SGlobalPlayerGateInfo gates[GS_LOGIN_MAXCOUNT];        // 网关信息
	time_t                lastActived;            
public:
	CGlobalPlayer();
   ~CGlobalPlayer();
	int Initialize();
	int Resume();
public:
	int GetRoleCountByServerId(int serverId);
	bool SetRoleCountByServerId(int serverId, int nRoles);
};

// 正在验证的用户
struct CCertifyPlayer{
	bool _inDb;																// 数据库中是否已经含有
	int  _handle;															// 分配给客户的端口
	int  _clientIp;														// 客户的ip地址
	int  _clientPort;														// 客户的端口号
	int  _accountId;														// 客户的账户ID
	char _passport[GS_PASSPORT_MAXLENGTH];							// 通行账号
	char _password[GS_PASSWORD_MAXLENGTH];							// 通行密码
	int  _gateCount;														// 网关个数
	SGlobalPlayerGateInfo _gates[GS_LOGIN_MAXCOUNT];			// 网关信息
	uint32                socketTime; 
};
//============================================================
class RBytes{
protected:
   static char HexChars[16];
   static char HexValues[128];
   static RBytes _instance;
public:
   RBytes();
public:
   static char* FromBytes(char* pValues, int count, char* pBuffer, int length);
   static char* ToChars(char* pValues, int count, char* pBuffer, int length);
};
#endif // _GLOBAL_ENTITY_H_