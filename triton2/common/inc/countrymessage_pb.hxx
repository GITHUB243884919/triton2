// 以最快速度序列/反序列
option optimize_for = SPEED;
import "corpsinfo_pb.hxx";


enum CountryMessageID // MESSAGE_COUNTRY = 0x5400
{
	ID_C2S_REQUEST_COUNTRYINFO			= 0x5401;	// 获得国家信息请求
	ID_S2C_RESPONSE_COUNTRYINFO			= 0x5402;	// 获得国家信息回应

	ID_C2S_REQUEST_JOINCOUNTRY			= 0x5403;	// 加入国家请求
	ID_S2C_RESPONSE_JOINCOUNTRY			= 0x5404;	// 加入国家回应

	ID_C2S_REQUEST_LEAVECOUNTRY			= 0x5405;	// 离开国家请求
	ID_S2C_RESPONSE_LEAVECOUNTRY		= 0x5406;	// 离开国家回应

	ID_C2S_REQUEST_SETOFFICIAL			= 0x5407;	// 国王任命官职请求
	ID_S2C_RESPONSE_SETOFFICIAL			= 0x5408;	// 国王任命官职回应

	ID_C2S_REQUEST_UNSETOFFICIAL		= 0x5409;	// 国王解除官职请求
	ID_S2C_RESPONSE_UNSETOFFICIAL		= 0x540A;	// 国王解除官职回应

	ID_C2S_REQUEST_USEFUNCTION			= 0x540B;	// 使用国家功能请求
	ID_S2C_RESPONSE_USEFUNCTION			= 0x540C;	// 使用国家功能回应

	ID_C2S_REQUEST_OFFICIALINFO			= 0x540D;	// 官职信息请求
	ID_S2C_RESPONSE_OFFICIALINFO		= 0x540E;	// 官职信息回应

	ID_C2S_REQUEST_USETIME				= 0x540F;	// 查询使用时间请求
	ID_S2C_RESPONSE_USETIME				= 0x5410;	// 查询使用时间回应

	ID_C2S_REQUEST_WORLDQUEST			= 0x5411;	// 世界操作请求
	ID_S2C_RESPONSE_WORLDQUEST			= 0x5412;	// 世界操作回应

	ID_S2C_NOTIFY_WORLDQUEST			= 0x5413;	// 世界操作通知

	ID_C2S_REQUEST_WORLDANSWER			= 0x5414;	// 回复世界请求
	ID_S2C_RESPONSE_WORLDANSWER			= 0x5415;	// 回复世界回应

	ID_S2C_NOTIFY_WORLDRESULT			= 0x5416;	// 世界结果通知

	ID_C2S_REQUEST_WORLDINFO			= 0x5417;	// 世界信息请求
	ID_S2C_RESPONSE_WORLDINFO			= 0x5418;	// 世界信息回应

	ID_S2C_NOTIFY_JOINCOUNTRY			= 0x5419;	// 加入国家通知
	ID_S2C_NOTIFY_OFFICIALCHANGE		= 0x541A;	// 官员改变通知
	ID_S2C_NOTIFY_WORLDANSWER			= 0x541B;	// 世界回复通知

	ID_C2S_REQUEST_PLAYERGSINFO			= 0x541C;		// 获取网关上玩家数据
	ID_S2C_RESPONSE_PLAYERGSINFO		= 0x541D;
	ID_S2C_NOTIFY_PLAYERGSINFO			= 0x541E;

	ID_S2C_NOTIFY_KINGCHANGE			= 0x5420;	// 国王改变通知

	ID_C2S_REQUEST_GETAWARD				= 0x5421;	// 领取奖励请求
	ID_S2C_RESPONSE_GETAWARD			= 0x5422;	
	ID_S2C_NOTIFY_STARTCOUNTRYREDSTONE	= 0x5423; 		// 开启国家内丹的通知	
	ID_S2C_NOTIFY_SUMMONPLAYER			= 0x5424;

	ID_C2S_REQUEST_SUBMITWAR			= 0x5425;	// 国战报名请求
	ID_S2C_RESPONSE_SUBMITWAR			= 0x5426;	// 国战报名回应

	ID_C2S_REQUEST_ENTERWAR				= 0x5427;	// 国战进入战场请求
	ID_S2C_RESPONSE_ENTERWAR			= 0x5428;	// 国战进入战场回应

	ID_C2S_REQUEST_QUERYWARCAMP			= 0x5429;	// 国战查询战场分配请求
	ID_S2C_RESPONSE_QUERYWARCAMP		= 0x542A;	// 国战查询战场分配回应

	ID_C2S_REQUEST_QUERYWARNUM			= 0x542B;	// 国战查询战场人数请求
	ID_S2C_RESPONSE_QUERYWARNUM			= 0x542C;	// 国战查询战场人数回应

	ID_C2S_REQUEST_QUERYWARINFO			= 0x542D;	// 国战查询战场信息请求
	ID_S2C_RESPONSE_QUERYWARINFO		= 0x542E;	// 国战查询战场信息回应

	ID_C2S_REQUEST_QUERYRANK			= 0x5430;	// 国战查询职业排名请求
	ID_S2C_RESPONSE_QUERYRANK			= 0x5431;	// 国战查询职业排名回应

	ID_S2C_NOTIFY_WARSOURCE				= 0x5432;	// 战争资源通知

	ID_C2S_REQUEST_BIDCAMP				= 0x5433;	// 国战营地竞价请求
	ID_S2C_RESPONSE_BIDCAMP				= 0x5434;	// 国战营地竞价回应

	ID_C2S_REQUEST_CAMPBIDINFO			= 0x5435;	// 国战查询营地竞价请求
	ID_S2C_RESPONSE_CAMPBIDINFO			= 0x5436;	// 国战查询营地竞价回应

	ID_C2S_REQUEST_CAMPRESINFO			= 0x5437;	// 国战查询营地结果请求
	ID_S2C_RESPONSE_CAMPRESINFO			= 0x5438;	// 国战查询营地结果回应

	ID_C2S_REQUEST_GETWARAWARD			= 0x5439;	// 国战获得奖励请求
	ID_S2C_RESPONSE_GETWARAWARD			= 0x543A;	// 国战获得奖励回应

	ID_S2C_NOTIFY_WARINFO				= 0x543B;	// 国战信息通知

	ID_C2S_REQUEST_QUERYWARAWARD		= 0x543C;	// 国战查询奖励请求
	ID_S2C_RESPONSE_QUERYWARAWARD		= 0x543D;	// 国战查询奖励回应

	ID_S2C_NOTIFY_WARHONOR				= 0x543E;	// 国战获得战功通知
	ID_S2C_NOTIFY_WARCAMP				= 0x5440;	// 国战阵营通知

	ID_C2S_REQUEST_MODIFYRATE			= 0x5441;	// 修改税率请求
	ID_S2C_RESPONSE_MODIFYRATE			= 0x5442;	// 修改税率回应
	ID_S2C_NOTIFY_MODIFYRATE			= 0x5443;	// 修改税率通知

	ID_C2S_REQUEST_GETTAX				= 0x5444;	// 获得税金请求
	ID_S2C_RESPONSE_GETTAX				= 0x5445;	// 获得税金回应

	ID_S2C_NOTIFY_QUERYSTONE			= 0x5446;	// 查询灵石通知
	ID_C2S_REQUEST_REFUSESUMMON			= 0x5447;	// 拒绝召唤请求
	ID_S2C_NOTIFY_REFUSESUMMON			= 0x5448;	// 拒绝召唤通知

	ID_C2S_REQUEST_ACCUSEKING			= 0x5449;	// 弹劾天子请求
	ID_S2C_RESPONSE_ACCUSEKING			= 0x544A;	// 弹劾天子回应
	ID_S2C_NOTIFY_ACCUSEKING			= 0x544B;	// 弹劾天子通知
	ID_S2C_NOTIFY_ACCUSESTATUS			= 0x544C;	// 弹劾广播通知
	ID_C2S_REQUEST_POWERINFO			= 0x544D;	// 声望查询请求
	ID_S2C_RESPONSE_POWERINFO			= 0x544E;	// 声望查询回应
	ID_S2C_NOTIFY_MAKEGUOTAN			= 0x5450;	// 发布国探通知
	ID_S2C_NOTIFY_MAKELUANWU			= 0x5451;	// 发布乱舞通知
};

enum EM_CampMessageID								// CAMP_MESSAGE		= 0x6000 - 0x63FF
{
	ID_C2S_CAMPWEAKLIST_REQUEST			= 0x5501;	// 客户端请求弱势阵营信息列表
	ID_S2C_CAMPWEAKLIST_RESPONSE		= 0x5502;	// 服务器回复弱势阵营信息列表

	ID_C2S_CAMPAWARDQUERY_REQUEST		= 0x5503;	// 客户端查询弱势阵营奖励
	ID_S2C_CAMPAWARDQUERY_RESPONSE		= 0x5504;	// 服务器返回弱势阵营奖励信息

	ID_C2S_CAMPGETAWARD_REQUEST			= 0x5505;	// 客户端领取弱势阵营奖励
	ID_S2C_CAMPGETAWARD_RESPONSE		= 0x5506;	// 服务器返回领取弱势阵营奖励确认
};

// 官员的存储结构
message PBOfficialData
{
	optional uint32			CharID			= 1;		// 官员的CharID
	optional string			CharName		= 2;		// 官员的名字
	optional uint32			OfficialLevel	= 3;
	optional uint32			OfficialOrder	= 4;
	repeated uint32			UseTimes		= 5;		// 官员权力的使用次数
	repeated uint32			UseX			= 6;
	repeated uint32			UseY			= 7;
};

// 国家的官员结构
message PBOfficialList
{
	repeated PBOfficialData	OfficialData = 1;		// 官员数据
};

message CMessageCountryInfoRequest
{
	optional uint32		CharID			= 1;
	optional uint32		CountryID		= 2;
	optional uint32		NeedFull		= 3;
	optional uint32		WorldID			= 4;
};

message CMessageCountryInfoResponse
{
	optional uint32		CharID			= 1;
	optional uint32		CountryID		= 2;
	optional uint32		KingID			= 3;		// 国王ID
	optional string		KingName		= 4;		// 国王名称
	optional uint32		CorpsID			= 5;		// 军团ID
	optional string		CorpsName		= 6;		// 军团名称
	optional uint32		UnionID			= 7;		// 联军ID
	optional string		UnionName		= 8;		// 联军名称
	optional uint32		PeopleNum		= 9;		// 国家人数
	optional uint32		KingPower		= 10;		// 国王声望
	optional uint32		Map				= 11;		// 国家势力
	optional uint32		Gold			= 12;		// 国家金库
	repeated uint32		Wizard			= 13;		// 才华值
	repeated uint32		ConstructValue	= 14;		// 建设度
	repeated uint32		ConstructLevel	= 15;		// 建设度
	optional string		Notice			= 16;		// 公告
	optional PBCorpsBattleList	BattleInfo	 =	17;			// 军团战争信息
	optional uint32		NeedFull		= 18;		// 是否需要全部信息
	optional uint32		WorldID			= 19;
	optional uint32		WorldLevel		= 20;		// 阵营(弱势)等级
};

message CMessageJoinCountryRequest
{
	optional uint32		CharID			= 1;
	optional uint32		CountryID		= 2;
	optional uint32		NPCEntityID		= 3;
};

message CMessageJoinCountryResponse
{
	optional uint32		CharID			= 1;
	optional uint32		Result			= 2;
	optional uint32		CountryID		= 3;
};

message CMessageJoinCountryNotify
{
	optional uint32		CharID			= 1;
	optional uint32		CountryID		= 2;
	optional uint32		KingID			= 3;
};

message CMessageLeaveCountryRequest
{
	optional uint32		CharID			= 1;
	optional uint32		CountryID		= 2;
	optional uint32		NPCEntityID		= 3;
};

message CMessageLeaveCountryResponse
{
	optional uint32		CharID			= 1;
	optional uint32		Result			= 2;
	optional uint32		CountryID		= 3;
};

message CMessageSetOfficialRequest
{
	optional uint32		CharID				= 1;	// 发起者ID,服务器用
	optional string		CharName			= 2;	// 角色名
	optional uint32		OfficialLevel		= 3;	// 官职品级
	optional uint32		OfficialOrder		= 4;	// 官职位置
	optional uint32		NormalSet			= 5;
};

message CMessageSetOfficialResponse
{
	optional uint32		CharID				= 1;	// 发起者ID,服务器用
	optional uint32		Result				= 2;
	optional string		CharName			= 3;	// 角色名
	optional uint32		OfficialLevel		= 4;	// 官职品级
	optional uint32		OfficialOrder		= 5;	// 官职位置
	optional uint32		NormalSet			= 6;
	optional uint32		TargetID			= 7;	// 设置的ID
};

message CMessageUnSetOfficialRequest
{
	optional uint32		CharID				= 1;	// 发起者ID,服务器用
	optional uint32		OfficialLevel		= 2;	// 官职品级
	optional uint32		OfficialOrder		= 3;	// 官职位置
};

message CMessageUnSetOfficialResponse
{
	optional uint32		CharID				= 1;	// 发起者ID,服务器用
	optional uint32		Result				= 2;
	optional string		CharName			= 3;	// 角色名
	optional uint32		OfficialLevel		= 4;	// 官职品级
	optional uint32		OfficialOrder		= 5;	// 官职位置
};

message CMessageUseFunctionRequest
{
	optional uint32		CharID				= 1;	// 发起者ID,服务器用
	optional uint32		FunctionType		= 2;	// 功能类型
	optional uint32		FunctionOrder		= 3;	// 功能编号
	optional string		TargetName			= 4;	// 目标名称
	optional TeleportData Teleport			= 5;
	optional uint32		IntParam1			= 6;	// 参数1
};

message CMessageUseFunctionResponse
{
	optional uint32			CharID				= 1;	// 发起者ID,服务器用
	optional uint32			Result				= 2;
	optional uint32			FunctionType		= 3;	// 功能类型
	optional uint32			FunctionOrder		= 4;	// 功能编号
	optional string			TargetName			= 5;	// 目标名称
};

message CMessageOfficialInfoRequest
{
	optional uint32		CharID				= 1;	// 发起者ID,服务器用
};

message CMessageOfficialInfoResponse
{
	optional uint32			CharID				= 1;	// 发起者ID,服务器用
	optional uint32			Result				= 2;
	optional PBOfficialList OfficialList		= 3;	// 官员列表
};

message CMessageUseTimeRequest
{
	optional uint32		CharID				= 1;	// 发起者ID,服务器用
};

message CMessageUseTimeResponse
{
	optional uint32			CharID				= 1;	// 发起者ID,服务器用
	optional uint32			Result				= 2;
	optional PBOfficialData OfficialData		= 3;	// 使用次数
};

message CMessageWorldQuestRequest
{
	optional uint32		CharID			= 1;
	optional uint32		WorldID			= 2;
	optional uint32		OpType			= 3;
	optional uint32		NPCEntityID		= 4;
};

message CMessageWorldQuestResponse
{
	optional uint32		CharID			= 1;
	optional uint32		Result			= 2;
	optional uint32		WorldID			= 3;
	optional uint32		OpType			= 4;
};

message CMessageWorldAnswerRequest
{
	optional uint32		CharID			= 1;
	optional uint32		WorldID			= 2;
	optional uint32		OpType			= 3;
	optional uint32		Answer			= 4;
};

message CMessageWorldAnswerResponse
{
	optional uint32		CharID			= 1;
	optional uint32		Result			= 2;
	optional uint32		WorldID			= 3;
	optional uint32		OpType			= 4;
	optional uint32		Answer			= 5;	// 增加返回回复类型
};

message CMessageWorldAnswerNotify
{
	optional uint32		CharID			= 1;
	optional uint32		SrcWorldID		= 2;
	optional uint32		DesWorldID		= 3;
	optional uint32		OpType			= 4;
	optional uint32		Answer			= 5;
};

message CMessageWorldQuestNotify
{
	optional uint32		CharID			= 1;
	optional uint32		WorldID			= 2;
	optional uint32		OpType			= 3;
};

message CMessageWorldResultNotify
{
	optional uint32		CharID			= 1;
	optional uint32		Result			= 2;
	optional uint32		SrcWorldID		= 3;
	optional uint32		DesWorldID		= 4;
	optional uint32		OpType			= 5;
};

message CWorldStatusData
{
	optional uint32		WorldID			= 1;
	optional uint32		Friends			= 2;
	optional uint32		Nation			= 3;
	optional uint32		KingID			= 4;
	optional string		KingName		= 5;
	optional uint32		CorpsID			= 6;
	optional string		CorpsName		= 7;
	optional uint32		UnionID			= 8;
	optional string		UnionName		= 9;
};

message CMessageWorldInfoRequest
{
	optional uint32				CharID			= 1;
	optional uint32				WorldID			= 2;
};

message CMessageWorldInfoResponse
{
	optional uint32				CharID			= 1;
	optional uint32				Result			= 2;
	optional uint32				WorldID			= 3;
	repeated CWorldStatusData	WorldData		= 4;
};

message CMessageOfficialChangeNotify
{
	optional uint32		CharID				= 1;
	optional string		CharName			= 2;	// 角色名
	optional uint32		OfficialLevel		= 3;	// 官职品级
	optional uint32		OfficialOrder		= 4;	// 官职位置
	optional uint32		CountryID			= 5;	// 国家ID
	optional uint32		Type				= 6;
	optional uint32		WorldID				= 7;
};

message CMessagePlayerGSInfoRequest
{
	repeated uint32 SrcCharID	=		1;			// 发起者的CharID
	repeated uint32 InfoCharID	=		2;			// 获取信息的CharID
};

message PlayerGSInfo
{
	optional uint32 CharID		=		1;			
	optional uint32	FamilyID	=		2;
	optional string	FamilyName	=		3;
	optional uint32	FamilyHead	=		4;
	optional uint32	CorpsID		=		5;
	optional string CorpsName	=		6;
	optional uint32	CorpsHead	=		7;
	optional uint32	UnionID		=		8;
	optional string	UnionName	=		9;
	optional uint32	UnionHead	=		10;
	optional uint32 CountryID	=		11;
	optional uint32	OfficialID	=		12;
	optional uint32 KingID		=		13;	
};

message WorldStatusInfo
{
	optional uint32	SrcWorldID	=		1;
	optional uint32	DesWorldID	=		2;
	optional uint32 Status		=		3;	
};

message GTAndLWInfo
{
	optional uint32	SrcWorldID	=		1;		// 发起阵营
	optional uint32	DesWorldID	=		2;		// 目标阵营
	optional uint32	TargetTime	=		3;		// 结束时间
};

message CMessagePlayerGSInfoResponse
{
	optional uint32				SrcCharID	=		1;
	repeated PlayerGSInfo		PlayerInfo	=		2;
	repeated WorldStatusInfo	WorldInfo	=		3;		
};

message CMessagePlayerGSInfoNotify
{
	optional uint32			SrcCharID	=		1;
	repeated PlayerGSInfo	PlayerInfo	=		2;
	repeated WorldStatusInfo	WorldInfo	=		3;
	optional uint32				WarState	=		4;		// 战争状态
	optional uint32				WinWorld	=		5;		// 获胜阵营
	optional uint32				TaxRate		=		6;		// 税率
	repeated GTAndLWInfo		GTInfo		=		7;		// 国探信息
	repeated GTAndLWInfo		LWInfo		=		8;		// 乱舞信息
};

// 国王变更通知
message CMessageKingChangeNotify
{
	optional uint32		WorldID			= 1;
	optional uint32		CountryID		= 2;
	optional uint32		KingID			= 3;
	optional string		KingName		= 4;
};

// 发送召集通知
message CMessageSummonPlayerNotify
{
	optional string		SrcName			= 1;
	optional uint32		LineID			= 2;
	optional uint32		MapID			= 3;
	optional uint32		PosX			= 4;
	optional uint32		PosY			= 5;
	optional uint32		TokenID			= 6;
};

// 领取奖励请求
message CMessageGetAwardRequest
{
	optional uint32		NPCEntityID		= 1;
	optional uint32		AwardID			= 2;
	optional uint32		TempID			= 3;
	optional uint32		Type			= 4;
};

message CMessageGetAwardResponse
{
	optional uint32		Result			= 1;
	optional uint32		AwardID			= 2;
	optional uint32		TempID			= 3;
	optional uint32		Type			= 4;
};


// *************************************************************************** //
// CMessageStartCountryRedstoneNotify
// 开启国家内丹的消息通知
// *************************************************************************** //
message CMessageStartCountryRedstoneNotify
{
	optional string ManagerName	=	1;		// 使用者名称
	optional uint32	ManagerID	=	2;		// 使用者角色ID
	optional uint32	LineID		=	3;		// 使用者所在线ID
	optional uint32	MapID		=	4;		// 使用者地图ID
	optional uint32	PosX		=	5;		// 使用者x坐标
	optional uint32	PosY		=	6;		// 使用者y坐标
	optional uint32	CountryID	=	7;		// 国家ID
	optional uint32	RedstoneType =	8;		// 内丹类型，国家内丹还是阵营内丹( CTplRedStone::ERedStoneType )
};

message TeleportData
{
	optional uint32			LineID		= 1;
	optional uint32			MapID		= 2;
	optional uint32			PosX		= 3;
	optional uint32			PosY		= 4;
	optional uint32			TokenID		= 5;
};

message CMessageSubmitWarRequest
{
	optional uint32			CharID			= 1;
	optional uint32			NPCEntityID		= 2;
	optional uint32			WarSerial		= 3;
};

message CMessageSubmitWarResponse
{
	optional uint32			CharID		= 1;
	optional uint32			Result		= 2;
	optional uint32			WarSerial	= 3;
};

message CMessageEnterWarRequest
{
	optional uint32			CharID			= 1;
	optional uint32			WarID			= 2;		// 需要进入的战场ID
	optional uint32			NPCEntityID		= 3;
	optional uint32			WarSerial		= 4;	
};

message CMessageEnterWarResponse
{
	optional uint32			CharID		= 1;
	optional uint32			WarID		= 2;		// 需要进入的战场ID
	optional uint32			Result		= 3;
	optional uint32			PosX		= 4;
	optional uint32			PosY		= 5;
};

message CMessageQueryWarCampRequest
{
	optional uint32			CharID			= 1;
	optional uint32			NPCEntityID		= 2;
};

message CMessageQueryWarCampResponse
{
	optional uint32			CharID		= 1;
	repeated uint32			WorldID		= 2;		// 阵营ID
	repeated uint32			CampID		= 3;		// 对应的战场ID
	repeated uint32			Money		= 4;		// 钱
};

message CMessageQueryWarNumRequest
{
	optional uint32			CharID			= 1;
	optional uint32			NPCEntityID		= 2;
};

message WarNumData
{
	optional uint32			WorldID			= 1;
	optional uint32			CampID			= 2;		// 战场ID
	optional uint32			CurrentNum		= 3;		// 当前人数
};

message CMessageQueryWarNumResponse
{
	optional uint32			CharID		= 1;
	repeated WarNumData		WarData		= 2;	
	optional uint32			CampID		= 3;
};

message WarDetailInfo
{
	optional uint32			WorldID		= 1;		// 阵营ID
	optional uint32			CurrentNum	= 2;		// 当前人数
	optional uint32			Power		= 3;		// 战斗力
	optional uint32			Flag		= 4;		// 占领据点
	optional uint32			Resource	= 5;		// 资源
	optional uint32			Kill		= 6;		// 击杀
	optional uint32			CampID		= 7;		// 战场ID
};

message CMessageQueryWarInfoRequest
{
	optional uint32			CharID			= 1;
	repeated uint32			CampID			= 2;		// 战场ID
	optional uint32			NPCEntityID		= 3;
};

message CMessageQueryWarInfoResponse
{
	optional uint32			CharID		= 1;
	repeated WarDetailInfo	CampInfo	= 2;		// 战场信息
};

message WarRankInfo
{
	optional uint32			WorldID		= 1;		// 阵营ID
	optional uint32			MetierID	= 2;		// 职业ID
	optional uint32			Kill		= 3;		// 击杀
	optional string			CharName	= 4;		// 角色名
	optional uint32			Rank		= 5;		// 排名
	optional uint32			Honor		= 6;		// 战功
};

message CMessageQueryWarRankRequest
{
	optional uint32			CharID			= 1;
	optional uint32			NPCEntityID		= 2;
};

message CMessageQueryWarRankResponse
{
	optional uint32			CharID		= 1;
	repeated WarRankInfo	RankInfo	= 2;
};

message CMessageWarSourceNotify
{
	optional uint32			WorldID		= 1;
	optional uint32			MapID		= 2;
	optional uint32			Resource	= 3;
	optional uint32			CharID		= 4;
	optional uint32			SourceNum	= 5;
};

message CMessageBidCampRequest
{
	optional uint32			CharID			= 1;
	optional uint32			NPCEntityID		= 2;
	optional uint32			Money			= 3;
	optional uint32			CampID			= 4;
};

message CMessageBidCampResponse
{
	optional uint32			CharID		= 1;
	optional uint32			Result		= 2;
	optional uint32			Money		= 3;
	optional uint32			CampID		= 4;
};

message CMessageCampBidInfoRequest
{
	optional uint32			CharID			= 1;
	optional uint32			NPCEntityID		= 2;
};

message CMessageCampBidInfoResponse
{
	optional uint32			CharID		= 1;
	optional uint32			Result		= 2;
	optional uint32			Money		= 3;
	optional uint32			CampID		= 4;
};

message CMessageCampResInfoRequest
{
	optional uint32			CharID			= 1;
	optional uint32			NPCEntityID		= 2;
};

message CMessageCampResInfoResponse
{
	optional uint32			CharID		= 1;
	optional uint32			Result		= 2;
	repeated uint32			WorldID		= 3;
	repeated uint32			CampID		= 4;
	repeated uint32			Money		= 5;
};

message CMessageGetWarAwardRequest
{
	optional uint32			CharID			= 1;
	optional uint32			WarSerial		= 2;
};

message CMessageGetWarAwardResponse
{
	optional uint32			CharID		= 1;
	optional uint32			Result		= 2;
	optional uint32			Money		= 3;
	optional uint32			Rank		= 4;
	optional uint32			ValidTime	= 5;
};

message NPCWarInfo
{
	optional uint32			PosX		= 1;
	optional uint32			PosY		= 2;
	optional uint32			WorldID		= 3;
	optional uint32			TempID		= 4;
};

message CMessageWarInfoNotify
{
	optional uint32			MapID		= 1;
	repeated NPCWarInfo		WarInfo		= 2;
	optional uint32			StoneID		= 3;
	optional uint32			StoneX		= 4;
	optional uint32			StoneY		= 5;
};

message CMessageQueryWarAwardRequest
{
	optional uint32			CharID			= 1;
	optional uint32			WarSerial		= 2;
};

message CMessageQueryWarAwardResponse
{
	optional uint32			CharID		= 1;
	optional uint32			Result		= 2;
	optional uint32			WinMoney	= 3;
	optional uint32			BestMoney	= 4;
	repeated uint32			WinItem		= 5;
	optional uint32			Rank		= 6;
};

message CMessageWarHonorNotify
{
	optional uint32			TotalHonor		= 1;		// 总计战功
	optional uint32			FreeHonor		= 2;		// 当前战功
	optional uint32			WarHonor		= 3;		// 本次战功
};

message CMessageWarCampNotify
{
	repeated uint32			WorldID		= 1;		// 阵营ID
	repeated uint32			CampID		= 2;		// 对应的战场ID
	repeated uint32			Money		= 3;		// 钱
	repeated uint32			BidCampID	= 4;		// 竞价的战场ID
};

message CMessageModifyRateRequest
{
	optional uint32			RateOrder		= 1;
	optional uint32			CharID			= 2;
	optional uint32			NPCEntityID		= 3;
};

message CMessageModifyRateResponse
{
	optional uint32			RateOrder		= 1;
	optional uint32			Result			= 2;
	optional uint32			CharID			= 3;
};

message CMessageModifyRateNotify
{
	optional uint32			Rate			= 1;
	optional string			Name			= 2;
	optional uint32			Real			= 3;
	optional uint32			WinWorld		= 4;
};

message CMessageGetTaxRequest
{
	optional uint32			CharID			= 1;
	optional uint32			Query			= 2;		// 是否查询金钱
	optional uint32			NPCEntityID		= 3;
};

message CMessageGetTaxResponse
{
	optional uint32			Money			= 1;
	optional uint32			Result			= 2;
	optional uint32			CharID			= 3;
	optional uint32			Query			= 4;		// 是否查询金钱
};

message CMessageQueryStoneNotify
{
	optional uint32			StoneID			= 1;
	optional uint32			StoneX			= 2;
	optional uint32			StoneY			= 3;
};

message CMessageRefuseSummonRequest
{
	optional uint32			AsynTeleTokenID	= 1;
	optional uint32			SrcCharID		= 2;
	optional uint32			DesCharID		= 3;
};

message CMessageRefuseSummonNotify
{
	optional string			SrcName			= 1;		// 拒绝人
	optional string			DesName			= 2;		// 被拒绝的人
	optional uint32			SrcCharID		= 3;
	optional uint32			DesCharID		= 4;
};


// 单个阵营统计信息
message PBCampWeakForceInfo
{
	optional uint32 camp_id				= 1;	// 阵营ID
	optional uint32 level_score			= 2;	// 等级积分
	optional uint32 honer_score			= 3;	// 荣誉积分
	optional uint32 total_score			= 4;	// 总分
	optional uint32 weak_level			= 5;	// 阵营等级
};

// 弱势阵营列表查询
message CMessageCampWeakListRequest
{
	
};

// 弱势阵营列表回复
message CMessageCampWeakListResponse
{
	optional uint32 errcode					= 1;	//结果错误码
	repeated PBCampWeakForceInfo camp_info	= 2;	//弱势阵营列表
};

// 领取弱势阵营奖励请求
message CMessageCampGetAwardRequest
{
	optional uint32 npc_entity_id			= 1;
	optional uint32 award_type				= 2;
};

// 领取弱势阵营奖励回复
message CMessageCampGetAwardResponse
{
	optional uint32 errcode					= 1;	//结果错误码
};

message CMessageAccuseKingRequest
{
	optional uint32	AccuseResult			= 1;	// 弹劾 = 1, 不弹劾 = 2, 弃权= 0
	optional uint32 WorldID					= 2;
	optional uint32 CharID					= 3;
};

message CMessageAccuseKingResponse
{
	optional uint32	ResponseResult			= 1;	// 结果错误码
	optional uint32	AccuseResult			= 2;	// 弹劾的投票
	optional uint32	CharID					= 3;
};

message CMessageAccuseKingNotify
{
	optional string KingName				= 1;	// 被弹劾的名字
	optional uint32	CharID					= 2;
};

message CMessageAccuseStatusNotify
{
	optional string KingName				= 1;	// 被弹劾的名字
	optional uint32 YesNum					= 2;	// 弹劾数量
	optional uint32	NoNum					= 3;	// 不弹劾数量
	optional uint32	AbortNum				= 4;	// 弃权数量
	optional uint32	CharID					= 5;
};

message CMessagePowerInfoRequest
{
	optional uint32	CharID					= 1;
};

message PowerInfo
{
	optional uint32 PowerType				= 1;	// 声望类型
	optional uint32	World					= 2;	// 对方阵营
	optional uint32	ChangeType				= 3;	// 改变类型
	optional uint32 ChangeValue				= 4;	// 改变值
	optional uint32 Time					= 5;	// 时间
};

message CMessagePowerInfoResponse
{
	repeated PowerInfo	PowerHistory		= 1;	// 声望记录
	optional uint32		PowerAdd			= 2;	// 今日增加
	optional uint32		PowerDel			= 3;	// 今日减少
	optional uint32		Result				= 4;	// 结果
	optional uint32		CharID				= 5;
};

message CMessageMakeGuoTanNotify
{
	optional uint32		SrcWorldID			= 1;	// 发布阵营
	optional uint32		DesWorldID			= 2;	// 对方阵营
	optional uint32		TargetTime			= 3;	// 结束时间，0表示已经结束
	optional string		KingName			= 4;	// 天子名字
};

message CMessageMakeLuanWuNotify
{
	optional uint32		SrcWorldID			= 1;	// 发布阵营
	optional uint32		DesWorldID			= 2;	// 对方阵营
	optional uint32		TargetTime			= 3;	// 结束时间，0表示已经结束
	optional string		KingName			= 4;	// 天子名字
};