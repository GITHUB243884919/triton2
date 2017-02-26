#include "globallogic.h"
#include "entity.h"

//100326 MZYNG ADD-BEGIN

//------------------------------------------------------------
char RBytes::HexChars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
char RBytes::HexValues[128];
RBytes RBytes::_instance;

//============================================================
// <T>根据枚举值值得到服务器类型</T>
//
// @param value枚举值
// @return 服务器类型
//============================================================
EGlobalLoginType RGlobalLoginType::Parse(int value){
	switch(value){
		case 0:
			return EGlobalLoginType_Unused;//未使用
		case 1:
			return EGlobalLoginType_Normal; //正常
		case 2:
			return EGlobalLoginType_New; //新服
	}
	return EGlobalLoginType_Unused;

}

//============================================================
// <T>根据枚举值值得到服务器类型说明</T>
//
// @param value枚举值
// @return 服务器类型说明
//============================================================
const char* RGlobalLoginType::ToString(EGlobalLoginType value){
	switch(value){
		case EGlobalLoginType_Unused:
			return "Unused";
		case EGlobalLoginType_Normal:
			return "Normal";
		case EGlobalLoginType_New:
			return "New";
	}
	return "Unused";
}

//============================================================
// <T>根据枚举值值得到服务器状态类型</T>
//
// @param value枚举值
// @return 服务器状态类型
//============================================================
EGlobalLoginStatusType RGlobalLoginStatusType::Parse(int value){
	switch(value){
		case 0:
			return EGlobalLoginStatusType_System; //系统
		case 1:
			return EGlobalLoginStatusType_Custom; //定制
	}
	return EGlobalLoginStatusType_System;
}

//============================================================
// <T>根据枚举值值得到服务器类型说明</T>
//
// @param value枚举值
// @return 服务器类型说明
//============================================================
const char* RGlobalLoginStatusType::ToString(EGlobalLoginStatusType value){
	switch(value){
		case EGlobalLoginStatusType_System:
			return "System";
		case EGlobalLoginStatusType_Custom:
			return "Custom";
	}
	return "System";
}

//============================================================
// <T>根据枚举值值得到服务器状态</T>
//
// @param value枚举值
// @return 服务器状态
//============================================================
EServerGatewayStatus RGlobalLoginStatus::Parse(int value){
	switch(value){
		case 0:
			return EServerGatewayStatus_Unknown; // 未知(K)
		case 1:
			return EServerGatewayStatus_Update;  // 维护(U)
		case 2:
			return EServerGatewayStatus_Full;    // 火爆(F)
		case 3:
			return EServerGatewayStatus_Rush;    // 繁忙(R)
		case 4:
			return EServerGatewayStatus_Busy;    // 忙碌(B)
		case 5:
			return EServerGatewayStatus_Idle;    // 空闲(I)
	}
	return EServerGatewayStatus_Unknown;
}

//============================================================
// <T>根据枚举值值得到服务器类型说明</T>
//
// @param value枚举值
// @return 服务器类型说明
//============================================================
const char* RGlobalLoginStatus::ToString(EServerGatewayStatus value){
	switch(value){
		case EServerGatewayStatus_Unknown:
			return "Unknown";
		case EServerGatewayStatus_Update:
			return "Update";
      case EServerGatewayStatus_Full:
         return "Full";
		case EServerGatewayStatus_Rush:
			return "Rush";
		case EServerGatewayStatus_Busy:
			return "Busy";
		case EServerGatewayStatus_Idle:
			return "Idle";
	}
	return "Unknown";
}

//============================================================
// <T>创建一个CGlobalLogin实例</T>
//============================================================
CGlobalLogin::CGlobalLogin(){
	if( CObj::msCreateMode){
		Initialize();
	}else{
		Resume();
	}
}

//============================================================
// <T>释放CGlobalLogin实例</T>
//============================================================
CGlobalLogin::~CGlobalLogin(){
}

//============================================================
// <T>初始化CGlobalLogin对象</T>
//============================================================
int CGlobalLogin::Initialize(){
	used = false;
	id = 0;
	memset(host, 0, GS_HOST_MAXLENGTH);
	port = 0;
	type = EGlobalLoginType_Unused;
	statusType = EGlobalLoginStatusType_System;
	status = EServerGatewayStatus_Unknown;
	playerTotal = 0;
	playerCurrent = 0;
   socket.CreateClient();
	return 0;
}

//============================================================
// <T>恢复GlobalLogin</T>
//============================================================
int CGlobalLogin::Resume(){
   socket.CreateClient();
	return 0;
}

//============================================================
// <T>根据枚举值值得到服务器类型说明</T>
//
// @param value枚举值
// @return 服务器类型说明
//============================================================
EServerGatewayStatus CGlobalLogin::GetStatus(){
	if(EGlobalLoginStatusType_System == statusType){
		if(playerCurrent < PLAYERS_AS_IDDLE){
			status = EServerGatewayStatus_Idle;
		}else if(playerCurrent < PLAYERS_AS_BUSY){
			status = EServerGatewayStatus_Busy;
		}else if(playerCurrent < PLAYERS_AS_RUSH){
			status = EServerGatewayStatus_Rush;
		}else{
			status = EServerGatewayStatus_Full;
		}
	}
	return status;
}

//============================================================
// <T>创建全局用户信息实例</T>
//============================================================
CGlobalPlayer::CGlobalPlayer(){
	if( CObj::msCreateMode){
		Initialize();
	}else{
		Resume();
	}
}

//============================================================
// <T>释放全局用户信息实例</T>
//============================================================
CGlobalPlayer::~CGlobalPlayer(){

}

//============================================================
// <T>初始化全局用户信息</T>
//============================================================
int CGlobalPlayer::Initialize(){
	used = false;
	accountId = 0;
	memset(passport, 0, GS_PASSPORT_MAXLENGTH);
	memset(password, 0, GS_PASSWORD_MAXLENGTH);
	token = 0;
	tokenTimeout = 0;
	memset(&gates[0], 0, sizeof(SGlobalPlayerGateInfo) * GS_LOGIN_MAXCOUNT);
	lastActived  = 0;
	return 0;
}

//============================================================
// <T>恢复全局用户信息</T>
//============================================================
int CGlobalPlayer::Resume(){
	return 0;
}

//============================================================
// <T>通过服务器ID获得玩家角色数量</T>
//
// @param serverId服务器ID
// @return 角色个数
//============================================================
int CGlobalPlayer::GetRoleCountByServerId(int serverId){
	for(int i = 0; i < gateCount; i++){
		if(gates[i].loginId == serverId){
			return gates[i].roleCount;
		}
	}
	return 0;
}

//============================================================
// <T>设置某服务器玩家角色数</T>
//
// @param serverId服务器ID，nRoles角色个数
// @return 是否变更了对应serverId的登路服务器上的角色数
//============================================================
bool CGlobalPlayer::SetRoleCountByServerId(int serverId, int nRoles){
	for(int i = 0; i < gateCount; i++){
		if(gates[i].loginId == serverId){
			if(gates[i].roleCount == nRoles){
				return false;
			}
			gates[i].roleCount = nRoles;
			return true;
		}
	}
	gates[gateCount].loginId = serverId;
	gates[gateCount].roleCount = nRoles;
	++gateCount;
	return true;
}

//============================================================
// <T>创建RBytes实例</T>
//============================================================
RBytes::RBytes(){
	for(int n='0'; n<= '9'; n++){
		HexValues[n] = n - '0';
	}
	for(int n='A'; n<= 'F'; n++){
		HexValues[n] = n - 'A' + 10;
	}
}

//============================================================
// <T>将字符串格式化成对应整数</T>
// @return  对应整数
//============================================================
char* RBytes::FromBytes(char* pValues, int count, char* pBuffer, int length){
	char* pPtr = pBuffer;
	while(--count >= 0){
		char ch = *pValues++;
		char cl = *pValues++;
		*pPtr++ = (HexValues[(int)ch] << 4) | HexValues[(int)cl];
	}
	*pPtr = 0;
	return pBuffer;
}

//============================================================
// <T>将整数转换成为字符串</T>
// @return 字符串指针
//============================================================
char* RBytes::ToChars(char* pValues, int count, char* pBuffer, int length){
	char* pPtr = pBuffer;
	while(--count >= 0){
		char value = *pValues++;
		*pPtr++ = HexChars[value >> 4];
		*pPtr++ = HexChars[value & 0x0F];
	}
	*pPtr = 0;
	return pBuffer;
}

//100326 MZYNG ADD-END