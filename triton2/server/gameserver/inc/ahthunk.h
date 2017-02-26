#ifndef _AHTHUNK_H_
#define _AHTHUNK_H_
/******************************************************************************
	版权所有 (C ), 2008-2018, 蓝港在线（北京）科技有限公司

******************************************************************************
	文件名		: ahthunk.h
	版本号		: 1.0
	作者		: ZHL
	生成日期	: 16:6:2009
	最近修改	: 
	功能描述	: Apex反外挂接口文件

	函数列表	: 
	修改历史	: 

	嵌入说明：
	1. 将 \ApexItemServer 拷贝到 gameserver 上(目录是不限定的); 
	2. 将 \ApexProxy\AHThunkConfig.xml 和 \ApexProxy\ApexProxy.so 拷贝到 gameserver 根目录下(即和 gameserver 的 .exe 在相同的目录);
	3. 将 \Client\ACDC.Dat，rdmex.dll，rdm.exe 拷贝到游戏客户端的根目录下;
	4. 用 \Interface\Client\AHClientInterface.lib 和Interface\Client\ApexClient.h 编译游戏客户端;
	5. 用 \Interface\Server\ApexProxy.h 和 \Interface\Server\ApexProxy.cpp编译游戏服务器端;
	6. 在 gameserver 根目录下建立文件夹 hook_log.

	说明：嵌入时所用配置文件一定要用原件,不要使用翻译后的。
	\ApexItemServer\config\AHNetServerConfig.xml 修改 IP 及 port。
	\ApexProxy\AHThunkConfig.xml 修改连接AHNetServer的 IP 及 port。
	

*******************************************************************************/


#include "servertool.h"
#include "lk_vector.h"


// 反外挂延迟处理数据
class CAHDelayData
{
public:
	int mCharID; // 角色id
	int mReason; // 原因
};


// 反外挂控制管理器
class CAHContorlManager : public CSingleton<CAHContorlManager>
{
	typedef lk::hash_map< int, CAHDelayData, 500 >	PLAYERID_VEC;	// 玩家容器

public:

	CAHContorlManager();
	~CAHContorlManager();

	int Initailize( );
	int Resume( );

	int OnTimer( unsigned int vTickCount, unsigned int vTickOffset );


	int AddToDelayProcessList( const CAHDelayData& rDelayData );

	// 延迟下线列表(charid)
	PLAYERID_VEC	mPlayerDelayProcList;
};



/* GameServer提供给Apex Proxy 的发送函数;
参数：pBuffer: 发送数据的缓冲区
		nLen : 发送数据的长度
*/
long NetSendToGameClient(signed int nSendId, const char * pBuffer, int nLen);

/* GameServer提供的踢人函数
参数:nId: 玩家的charid
实现这个函数,把一个玩家踢下线;
*/
long GameServerKillUser( signed int nId, int nAction );

/* 启动Apex服务
*/
int StartApexProxy();

/* 停止Apex服务
*/
int StopApexProxy();

/* 通知Apex玩家登陆
参数:nSendId: 玩家的charid
*/
int NoticeApexProxy_UserLogin( int nSendId );

/* 通知Apex玩家登陆
参数:nSendId: 玩家的charid
*/
int	NoticeApexClient_Ret( int nSendId, const char * pRet );

/* 通知Apex玩家退出
参数:nSendId: 玩家的charid
*/
int	NoticeApexProxy_UserLogout( int nSendId );

/* 通知Apex玩家反外挂数据
参数:nSendId: 玩家的charid
	:pBuf: 2进制数据
	:nBuffLen: 数据长度
*/
int	NoticeApexProxy_UserData( int nSendId, const char * pBuf, int nBufLen );


#endif //_AHTHUNK_H_

