#ifndef _GM_SERVICE_H_
#define _GM_SERVICE_H_

#include <time.h>
#include "loginlogic.h"
#include "gm_command.h"
#include "servermessage_in.h"
#include "db_msg_pb.hxx.pb.h"
//#include "IpData.h"
class CIpData
{
public:
	CIpData();
	~CIpData();
	bool	Init();
	void	Free();
	void	SetID(int nID);					//设置id
	void	SetProvince(unsigned short* pcProvince);	//设置省份
	void	SetCity(unsigned short* pcCity);			//设置城市
	void	SetStartIp(unsigned int  nIp);			//设置开始IP
	void	SetEndIp(unsigned int  nIp);				//设置结束IP
	int		GetID();						//获得id
	unsigned short *	GetProvince();					//获得省份
	unsigned short *	GetCity();						//获得城市
	unsigned int 	GetStartIp();					//获得开始IP
	unsigned int 	GetEndIp();						//获得结束IP
	int     GetProvinceLen();			
	int     GetCityLen();
	bool    IpIsHere(unsigned int  nIp);				//判断指定ip是否属于这个段
	int		m_nProvinceLen;
	int		m_nCityLen;
private:
	int		m_nID;
	
	unsigned short  m_pcProvince[32] ;
	
	unsigned short  m_pcCity[32] ;
	unsigned int 	m_nStartIp;
	unsigned int 	m_nEndIp;
};

class CGmServer: public CSingleton < CGmServer >
{
public:
	bool CreateGmCmdServer(const char* szIpPort);
	void OnGmCmdServerAccept(int fd);
	void RecvGmCmdServerMsg(int vMsgCount);
	void UnRegistGateSockfd(int fd);

	CGmServer();
	~CGmServer();

	// 消息响应
    void OnGMMessageSendMail(int nCommHandle, CMessage* tpMessage);

	void OnGMServerMessage(int nCommHandle, CMessage* tpMessage);

	void DealMsgBySessionAndFD(CMessage* tpMessage, 
			int nSessionID, int nfd, int(*pfflash)(CMessage*, CMaxString&));

	void OnGMCmdMessageResult(CMessage* tpMessage);

	template < class TC > 
	void OnGMCmdNormalResult(CMessage* tpMessage, int (*pfflash) (CMessage*, CMaxString&))
	{
		LK_ASSERT(tpMessage != NULL, return);
		LK_ASSERT(tpMessage->has_msgpara()  && tpMessage->msgpara() != 0, return);

		TC* pGmCmd = (TC*) tpMessage->msgpara();

		DealMsgBySessionAndFD(tpMessage, pGmCmd->session_id(), pGmCmd->fd(), pfflash);
	}

	//RoleDetail
	void OnGMMessageRoleDetailCmd(int nCommHandle, CMessage* tpMessage);
	void OnGmMessageRoleDetailResult(CMessage* tpMessage, int nSessionID, int nfd);

	void OnGMMessageNpcDetailCmd(int nCommHandle, CMessage* tpMessage);

	//RoleProto
	void OnGMMessageRoleProtoCmd(int nCommHandle, CMessage* tpMessage);
	void OnGmMessageRoleProtoResult(CMessage* tpMessage, int nSessionID, int nfd);

	//UserSocialInfo
	void OnGMMessageUserSocialInfoCmd(int nCommHandle, CMessage* tpMessage);
	void OnGmMessageUserSocialInfoResult(CMessage* tpMessage, int nSessionID, int nfd);

	//IbStore
	void OnGMMessageIbInfoCmd(int nCommHandle, CMessage* tpMessage);
	void OnGmMessageIbInfoResult(CMessage* tpMessage, int nSessionID, int nfd);

	void OnGMMessageSetIbPriceCmd(int nCommHandle, CMessage* tpMessage);
	void OnGMMessageSetIbLocationCmd(int nCommHandle, CMessage* tpMessage);
	void OnGMMessageIbOnSaleCmd(int nCommHandle, CMessage* tpMessage);
	void OnGMMessageIbNotSaleCmd(int nCommHandle, CMessage* tpMessage);
	void OnGMMessageIbDescriptCmd(int nCommHandle, CMessage* tpMessage);

	//IpBind
	void OnGMMessageIpBindCmd(int nCommHandle, CMessage* tpMessage);

	void OnGmMessageNormalSqlDmlResult(CMessage* tpMessage, int nSessionID, int nfd);

	// GoodHistory
	void  OnGMMsgGoodsHistoryToNpcCmd(int nCommHandle, CMessage* tpMessage);
	void  OnGMMsgGoodsHistoryFromNpcCmd(int nCommHandle, CMessage* tpMessage);
	void  OnGMMsgGoodsHistoryBetweenUsersCmd(int nCommHandle, CMessage* tpMessage);
	void  OnGMMsgGoodsHistoryCmd(int nCommHandle, CMessage* tpMessage);
	void  OnGMMsgOperationDetailCmd(int nCommHandle, CMessage* tpMessage);

	void OnGmMessageGoods2NpcResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnGmMessageGoods4NpcResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnGmMessageGoodsAtUsersResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnGmMessageGoodsHistoryResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnGmMessageOperationDetailResult(CMessage* tpMessage, int nSessionID, int nfd);

	// PostBulletin
	void OnGmMessagePostBulletinCmd(int nCommHandle, CMessage* tpMessage);

	void OnGMMsgRoleLevelInfoCmd(int nCommHandle, CMessage* tpMessage);
	void OnGmMsgRoleLevelInfoResult(CMessage* tpMessage, int nSessionID, int nfd);

	void OnGMMsgRoleItemOperate(int nCommHandle, CMessage* tpMessage);
	//void OnGMMsgRolePickItemFromBoss(int nCommHandle, CMessage* tpMessage);
	void OnGMMsgRoleMoneyOperate(int nCommHandle, CMessage* tpMessage);
	void OnGMMsgRoleExpOperate(int nCommHandle, CMessage* tpMessage);
	void OnGMMsgItemUpgrade(int nCommHandle, CMessage* tpMessage);

	void OnGmMessageRoleItemOperateResult(CMessage* tpMessage, int nSessionID, int nfd);
	//void OnGMMsgRolePickItemFromBossResult(CMessage* tpMessage,int nSessionID, int nfd);
	void OnGmMessageRoleMoneyOperateResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnGmMessageRoleExpOperateResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnGmMessageItemUpgradeResult(CMessage* tpMessage, int nSessionID, int nfd);

	// freeze mac
	void OnGMMsgFreezeMacCmd(int nCommHandle, CMessage* tpMessage);

	// CorpsTotal
	void OnGmMessageCorpsTotalResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnGMMsgCorpsTotalCmd(int nCommHandle, CMessage* tpMessage);

	// CorpsDetail
	void OnGmMessageCorpsDetailResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnGMMsgCorpsDetailCmd(int nCommHandle, CMessage* tpMessage);

	// CorpsInfo
	void OnGmMsgCorpsLogResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnGmMsgFamilyLogResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnGMMsgCorpsLogCmd(int nCommHandle, CMessage* tpMessage);

	// RoleInfo
	void OnGmMsgQueryDeathResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnGmMsgQueryTaskResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnGmMsgQueryLoginResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnGMMsgRoleLogCmd(int nCommHandle, CMessage* tpMessage);

	// RecoverRoleItem
	void OnGmMsgQueryRoleEquipResult(CMessage* tpMessage, int nSessionID, int nfd);
	void OnGMMsgRecoverRoleEquipCmd(int nCommHandle, CMessage* tpMessage);

	// 查询玩家邮件
	void OnGmMsgSqlQueryResult( CMessage* tpMessage);
	void OnGmMsgQueryRoleMailCmd(int nCommHandle, CMessage* tpMessage);

	// 查询特定IP下的玩家
	void OnGmMsgRoleInfoByIPCmd( int nCommHandle, CMessage* tpMessage);
	void OnGmMsgUserInfoByIPResult( CMessage* tpMessage);
	// 从数据库返回结果 
	void OnGmMsgQueryLogoutIPResult(CMessage* tpMessage, int nSessionID, int nfd);

	// 从数据库返回股票交易查询结果
	void OnGmMsgQueryStockResult(CMessage* tpMessage, int nSessionID, int nfd);
	
	// 从数据库返回元宝摆摊交易查询结果
	void OnGmMsgQueryYBStallResult(CMessage* tpMessage, int nSessionID, int nfd);

	// 初始化支持的gm命令
	void IniAvailableGmCmd();

	// 装载Helper
	void LoadProtoHelper();

	// 装载Npc
	void LoadMapNpc();

	void LoadCityIpfile();

	void AddIpData(int nID, unsigned short * pcProvince, int prolen,unsigned short * pcCity,int citylen, unsigned int  nStartIp, unsigned int  nEndIp);//增加ip数据节点
	
	void ClearIpData();
    const unsigned short * GetProvince(int nstartIP);
	const int  GetCityLen(int nstartIP);

	void ReloadCfg();

	void LoadToken2String();
	// 
	const char* Label(const char* proto, const char* field , const char**pDomain = NULL, int * pFormat = 0);
	const char* Label(const char* proto, int nEnumID);

	// 查找特定技能的gm命令
	void OnGMMessageRoleSkillDetailCmd(int nCommHandle, CMessage* tpMessage);

	// 根据命令ID取得命令的ID
	int GetGmMessageID(const char* szCmdName);

	// 方法
	const char* GetMapName(int nMapID)	
	{
		MAP_DICT::iterator iter4map = mMapDict.find(nMapID);
		return  (iter4map == mMapDict.end() ? NULL : iter4map->second.map_name );
	}

	// Token2String
	const char* Token2String(const char* szToken)
	{
		if ( szToken[0] != '#') return szToken;

		TOKEN2STRING::iterator iter = mToken2String.find(lk::CString32(&szToken[1]));
		return (iter == mToken2String.end() ? szToken : iter->second.c_str());
	}

	/*
	const char* GetCountryName(unsigned int nCountryID)
	{
		static const char* szCountryName[] = {
			NULL,
			"尘埃世界",
			"无物世界",
			"非台世界",
			"明镜世界",
			"无树世界",
			"菩提世界"
		};

		return (nCountryID <= sizeof(szCountryName)/sizeof(const char*) ? szCountryName[nCountryID] : NULL);
	}
	*/

	int GetStatus() { return mGmCmdServer.GetStatus(); }

	const char* UmsFamilyFields() const { return mUmsFamilyFields.c_str(); }
	const char* UmsCorpsFields() const { return mUmsCorpsFields.c_str(); } 
	const char* UmsUnionFields() const { return mUmsUnionFields.c_str(); } 
	const char* UmsWorldFields() const { return mUmsWorldFields.c_str(); } 

	void Send2GMNormalResult(int nResult, int nSessionID, int nfd);

	// 通过时间来为GM2.0请求指定DB; 0 默认； 1 远程服务器db
	int ChoseDBByTimeRegion( const char* pszBeginTime, const char* pszEndTime );

	

private:
	enum EFDType
	{ 
		FD_IS_LISTENER, 
		FD_FROM_GMSERVER, 
	};

	// 与 socket 相关的
	enum 
	{
		MAX_SOCK_NUM = 128
	};

	int maxfd;
	struct 
	{
		int fd;				// file descriptor

		time_t create_ts; 	// carete timestampt
		time_t active_ts;	// active timestampt
		EFDType from;

		int addr;			// from ip
		int port;			// from port
		CTCPSocket<RECVBUFLENGTH, POSTBUFLENGTH> *pactor;	

		int session_id;		// session_id, default 0;
	}mSocks[MAX_SOCK_NUM+1];  // fd的信息存储在mSock[fd]的位置

	// 短链接，大概1个连接只会同时处理一个或几个事务
	// list<int> mSessionList;

	CTCPSocket<RECVBUFLENGTH, POSTBUFLENGTH> mGmCmdServer;
	typedef lk::string<256> CString256;

	typedef struct TNpcDefine
	{
		int  npc_id;
		char npc_name[NAME_LENGTH+1];
		int  map_id;
		int  x;
		int  y;
	}TNpcDefine;

	typedef struct TMapDefine
	{
		int map_id;
		char map_name[NAME_LENGTH+1];
	}TMapDefine;

	typedef struct TFieldHelper
	{
		int id;
		lk::CString32 name;
		CString256 label;
		lk::CString32 domain;	// 值域类型
		int format;				// 格式
	}TFieldHelper;

	typedef lk::hash_map< lk::CString32, TNpcDefine, 1000> NPC_DICT;
	typedef lk::hash_map< int, TMapDefine, 256>  MAP_DICT;

	NPC_DICT mNpcDict;
	MAP_DICT mMapDict;

	typedef std::map<std::string, int> CMDTYPEDICT;
	CMDTYPEDICT mGmCmdList;

	//typedef map< pair<int, int>, TFieldHelper > HELPER_DICT;
	typedef std::map< lk::CString64, TFieldHelper > HELPER_DICT;
	HELPER_DICT mHelperDict;

	typedef std::map<lk::CString32, CString256 > TOKEN2STRING;
	TOKEN2STRING mToken2String;

	lk::string<1024> mUmsFamilyFields; 
	lk::string<1024> mUmsCorpsFields; 
	lk::string<1024> mUmsUnionFields; 
	lk::string<1024> mUmsWorldFields; 

   std::vector<CIpData *> m_ipDatalist;
   BYTE *m_tpTempBuffer;

};


#endif
