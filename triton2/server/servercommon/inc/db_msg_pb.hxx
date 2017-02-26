
import "property_pb.hxx";
import "boursemessage_pb.hxx";

// 以最快速度序列/反序列
option optimize_for = SPEED;


enum DB_MSG_ID
{	
	ID_S2S_EXECUTESQL_REQUEST			= 0x81FE; // 服务器->服务器 执行SQL语句请求
	ID_S2S_EXECUTESQL_RESPONSE			= 0x81FF; // 服务器->服务器 执行SQL语句回应

	ID_S2D_LOADPLAYERITEMINFO_REQUEST	= 0x7C02;	// 场景服务器->数据服务器 向数据库请求装备信息
	ID_D2S_LOADPLAYERITEMINFO_RESPONSE	= 0x7C03;	// 数据服务器->场景服务器 数据库返回装备信息

	ID_S2D_SAVEPLAYERITEMINFO_REQUEST	= 0x7C04;	// 场景服务器->数据服务器 保存道具信息请求
	ID_D2S_SAVEPLAYERITEMINFO_RESPONSE	= 0x7C05;	// 数据服务器->场景服务器 保存道具信息回应

	ID_S2D_LOADPLAYERINFO_REQUEST		= 0x7C06;	// 场景->数据				Load 玩家基本数据请求
	ID_D2S_LOADPLAYERINFO_RESPONSE		= 0x7C07;	// 数据->场景				Load 玩家基本数据回应	

	ID_S2D_SAVEPLAYER_REQUEST		= 0x7C08;		// 场景->数据				保存玩家数据请求
	ID_D2S_SAVEPLAYER_RESPONSE		= 0x7C09;		// 数据->场景				b保存玩家数据回应

	ID_S2D_LOADPLAYERPROPERTY_REQUEST	= 0x7C0A;
	ID_D2S_LOADPLAYERPROPERTY_RESPONSE	= 0x7C0B;


	ID_S2D_UPDATEPLAYERINFO_REQUEST		= 0x7C0E;	// 直接修改玩家数据请求
	ID_D2S_UPDATEPLAYERINFO_RESPONSE	= 0x7C10;	// 直接修改玩家数据回应

	// 交易行
	ID_G2D_GETBOURSE_INFO				= 0x7C11;
	ID_S2D_BOURSE_INC_ACCOUNT			= 0x7C13;		// 向帐户中添加
	ID_S2D_BOURSE_DEC_ACCOUNT			= 0x7C14;		// 向帐户中提取
	ID_S2D_BOURSE_GET_ACCOUNT			= 0x7C15;		// 获得帐户
	ID_D2S_BOURSE_ACCOUNT_NOTICE		= 0x7C16;		//	帐户信息反馈
	ID_S2D_BOURSE_ADD_DOC				= 0x7C17;		// 添加定单
	ID_S2D_BOURSE_PLAYER_LIST_GET		= 0x7C18;		// 
	ID_D2S_BOURSE_PLAYER_NOTICE			= 0x7C19;
	ID_G2D_BOURSE_TRADE					= 0x7C1A;
	ID_S2D_BOURSE_DEC_BUY_DOC			= 0x7C1B;
	ID_S2D_BOURSE_DEC_SELL_DOC			= 0x7C1C;
	ID_S2D_BOURSE_LOG_QUERY				= 0x7C1D;
	ID_D2S_BOURSE_LOG					= 0x7C1E;
};

enum SQLTYPE {
	SELECT = 1;
	INSERT = 2;
	UPDATE = 3;
	DELETE = 4;
};

//message PBBourseDetail
//{
//	optional string bourse_doc_no	= 1;
//	optional uint32	account_id		= 2;
//	optional uint32 price			= 3;
//	optional uint32 yb_count		= 4;
//	optional uint32 bourse_date		= 5;
//	optional uint32 trade_type		= 6;
//};

//message PBBourseDetailList
//{	 
//	repeated PBBourseDetail list = 1;
//};


message CMessageExecuteSqlRequest
{
	optional uint32	SessionID		= 1; // 会话ID
	optional string	SQL				= 2; // SQL语句
	optional uint32	BufSize			= 3; // 传入buf的长度，没有就是0
	optional bytes	Buffer			= 4; // 附带的二进制buf  
	optional uint32	Callback		= 5; // 是否需要返回值； 1-需要；0-不需要
	optional uint32 LatestAutoIncID	= 6; // 是否需要最后的自增长ID; 1-需要；0-不需要
	optional SQLTYPE SqlType		= 7 [default = SELECT];// sql 语句的类型
	optional uint32 SessionType		= 8; // session类型 CSessionType
};

message CMessageExecuteSqlResponse3
{
	optional uint32	SessionID		= 1; // 会话ID
	optional uint32	ResultCode		= 2; // 返回的错误码
	optional uint32	RowCount		= 3[default=0]; // 总行数
	optional uint32	ColCount		= 4[default=0]; // 总列数
	repeated string FieldName		= 5; // 列名称 ( 变长数组个数为 ColCount )
	repeated uint32 FieldType		= 6; // 列类型 ( 变长数组个数为 ColCount )
	repeated bytes	FieldValue		= 7; // 列值 ( 变长数组个数为 RowCount * ColCount )
	repeated uint32	FieldValueLen	= 8; // 列值长度 ( 变长数组个数为 RowCount * ColCount )
	optional uint32 LatestAutoIncID	= 9; // 最后的自增长ID的值
};






// 加载道具信息
enum emPackageIdx
{
	PACKAGE_EQUIPMENT	= 0; // 装备
	PACKAGE_BAGGAGE		= 1; // 包裹
	PACKAGE_STORAGE		= 2; // 仓库
	PACKAGE_TASK		= 3; // 任务包
};

// 部分必须和上面一致
enum emReSaveFlag
{
	SAVE_PACKAGE_EQUIPMENT	= 0; // 装备
	SAVE_PACKAGE_BAGGAGE		= 1; // 包裹
	SAVE_PACKAGE_STORAGE		= 2; // 仓库
	SAVE_PACKAGE_TASK		= 3; // 任务包
	SAVE_PLAYER_INFO			= 4; // 玩家基础信息
	SAVE_PLAYER_PRO			= 5; // 玩家属性信息
	SAVE_ITEM_NUM		= 6; // 总数
	SAVE_ITEM_FULL_FLAG = 0x3F; // 存储标记
};



// 从数据库加载玩家道具信息
message CMessageLoadPlayerItemInfoRequest
{
	optional uint32 AccountID		= 1;
	optional uint32 CharID			= 2;
	repeated uint32 PackageIdx		= 3;	//包裹 emPackageIdx
	optional uint32 SessionID		= 4;	
};

// 数据库返回道具信息
message CMessageLoadPlayerItemInfoResponse
{
	enum emErrorCode
	{
		SUCCES			= 0;		// 执行sql成功
		SQL_FAILED		= -1;		// 执行sql失败
		RECORD_EMPTY	= -2;		// 空记录
		DATA_CRASH		= -3;		// 数据损坏，一般指blob数据
	};
	
	optional int32	ErrCode			= 1;	// 错误码	
	optional uint32 AccountID		= 2;
	optional uint32 CharID			= 3;
	optional uint32 SessionID		= 4;
	repeated uint32 PackageIdx		= 5;	//包裹 emPackageIdx
	repeated PBItemBox ItemBox		= 6;
};

// 保存玩家道具信息请求
message CMessageSavePlayerItemInfoRequest
{
	optional uint32 AccountID		= 1;
	optional uint32 CharID			= 2;
	repeated uint32 PackageIdx		= 3;	//包裹 emPackageIdx
	optional uint32 SessionID		= 4;
	repeated PBItemBox ItemBox		= 6;
};

// 保存玩家道具信息回应
message CMessageSavePlayerItemInfoResponse
{
	enum emErrorCode
	{
		SUCCES			= 0;		// 执行sql成功
		SQL_FAILED		= -1;		// 执行sql失败
		SERALIZE_FAILED	= -2;		// 序列化失败
		FILTER_FAILED	= -3;		// 为blob数据转义字符串失败
		INVALID_PKGIDX	= -4;		// 无效的包裹索引
	};

	optional int32	ErrCode			= 1;	// 错误码	
	optional uint32 AccountID		= 2;
	optional uint32 CharID			= 3;
	repeated uint32 PackageIdx		= 4;	//包裹 emPackageIdx
	optional uint32 SessionID		= 5;
};



message CMessagePlayerProperty
{
	optional uint32 role_hp					= 1;
	optional uint32 role_mp					= 2;
	optional uint32 role_ap					= 3;
	optional PBPlayerBasicInfo role_basic	= 4;
	optional PBSkillList role_skill			= 5;
	optional PBBuffList role_buff			= 6;
	optional PBPlayerCDGroupInfo role_cdgroup			= 7;
	optional PBPlayerPVPInfo role_pvp		= 8;
	optional PBPlayerTitleInfo role_title				= 9;
	optional PBPlayerActivityInfo role_activity			= 10;
	optional PBPlayerTalentInfo role_talent				= 11;
	optional PBPlayerMWInfo role_mw						= 12;
	optional PBLifeSkillBox role_lifeskill			= 13;
	optional PBPlayerStatisticInfo role_statistic			= 14;
	optional PBMonsterRecordInfo role_monster_record	= 15;
	optional PBTelePosInfo role_tele_pos			= 16;
	optional PBWizard role_wizard				= 17;
	optional PBPlayerClientInfo role_client				= 18;
	optional uint32	save_bitflag			= 19;
	optional PBFriend role_friend			= 20;
	optional PBRepetion role_repetion		= 21;
	optional PBTaskList role_task			= 22;
	optional PBMatrixInfo role_matrix		= 23;
	optional PBCharm role_charm		= 24;
	optional PBGuideData role_guide		= 25;
	optional PBTimes role_times		= 26;
	optional PBExpTimeInfo role_exptimeinfo	= 27;
	optional PBTreasureBowl role_treasurebowl = 28;
	optional PBSpiritInfo role_spiritinfo = 29;
};

message CMessageSavePlayerRequest
{
	optional uint32 RoleID					= 1;
	optional CMessagePlayerInfo Info		= 2;
	optional CMessagePlayerProperty Property= 3;
	optional uint32 SessionID				= 4;
	optional uint32 DetailID				= 5;
	optional uint32 CallBack				= 6;
};

message CMessageLoadPlayerInfoRequest
{
	optional uint32 RoleID					= 1;
	optional uint32 SessionID				= 2;
};

message CMessageLoadPlayerInfoResponse
{
	optional uint32 RoleID					= 1;
	optional uint32 SessionID				= 2;
	optional CMessagePlayerInfo Info		= 3;
	optional uint32 ResultCode				= 4;
};

message CMessageSavePlayerResponse
{
	optional uint32 RoleID					= 1;
	optional uint32 SessionID				= 2;
	optional uint32 ResultCode				= 3;
};

message CMessageLoadPlayerPropertyRequest
{
	optional uint32 RoleID					= 1;
	optional uint32 SessionID				= 2;
	optional uint32 DetailID				= 4;

};

message CMessageLoadPlayerPropertyResponse
{
	optional uint32	RoleID					= 1;
	optional uint32 SessionID				= 2;
	optional CMessagePlayerProperty Property= 3;
	optional uint32 ResultCode				 = 4;
};



enum UpdateType
{
	BasicInfo		= 1;
	SkillList		= 2;
	BuffList		= 3;
	CDGroup			= 4;
	PVPInfo			= 5;
	TitleInfo		= 6;
	ActivityInfo	= 7;
	TalentInfo		= 8;
	MWInfo			= 9;
	LifeSkill		= 10;
	StasticInfo		= 11;
	MonsterInfo		= 12;
	TelePosInfo		= 13;
	Wizard			= 14;
	ClientInfo		= 15;
	Friend			= 16;
	Repetion		= 17;
	TaskList		= 18;
	MatrixInfo		= 19;
	Charm			= 20;
	Guide			= 21;
	Times			= 22;
};

message CMessageUpdatePlayerInfoRequest
{
	optional uint32	RoleID					= 1;
	optional uint32 SessionID				= 2;
	optional uint32	UpdateType				= 3;
	optional uint32 Param1					= 4;
	optional uint32	Param2					= 5;
	optional uint32	Value					= 6;
	optional uint32	DetailID				= 7;
	optional uint32 SubType					= 8;
};

enum MatrixSubType
{
	RemoveMaster	=	1;
	RemoveSlave		=	2;
	AddMasterPoint	=	3;
	AddSlaveNum		=	4;
	AddFormal		=	5;
};

enum MatrixParam
{
	BeginnerNum		=	1;
	FormalNum		=	2;
	EducatedNum		=	3;
};


message CMessageGetBourseInfo
{
	optional uint32 SessionID				= 1;
	optional uint32 AccountID				= 2;
};

message CMessageBourseInfoNotice
{
	optional uint32			SessionID		= 1;
	optional PBBourseDetailList BuyList			= 2;
	optional PBBourseDetailList SellList		= 3;	
};

message CMessageBourseAddDoc
{
	optional uint32			SessionID		= 1;
	optional uint32			AccountID		= 2;
	optional PBBourseDetail	Detail			= 3;
	optional uint32			Type			= 4;	// 1 buy 2 sell
	
	optional uint32			Tax				= 5;
	optional uint32			RoleID			= 6;
	optional uint32			Level			= 7;
	optional uint32			WorldID			= 8;
	optional uint32			ServerID		= 9;
	optional uint32			TaxID			= 10;
	optional uint32			GameID			= 11;
};


message CMessageBourseS2DIncAccount
{
	optional uint32			SessionID		= 1;
	optional uint32			Money			= 2;
	optional uint32			Yb				= 3;
	optional uint32			AccountID		= 4;
	optional uint32			RoleID			= 5;
};

message CMessageBourseS2DDecAccount
{
	optional uint32			SessionID		= 1;
	optional uint32			Money			= 2;
	optional uint32			Yb				= 3;
	optional uint32			AccountID		= 4;
	optional uint32			RoleID			= 5;
};

message CMessageBourseS2DGetAccount
{
	optional uint32			SessionID		= 1;
	optional uint32			AccountID		= 2;
	optional uint32			Code			= 3;
};

message CMessageBourseD2SAccountNotice
{
	optional uint32			SessionID		= 1;
	optional uint32         Money           = 2;
	optional uint32         Yb              = 3;
	optional uint32			AccountID		= 4;
	optional uint32			Code			= 5;	
	optional uint32			Result			= 6;
	optional uint32			Parame1		= 7;
	optional uint32			Parame2		= 8;
};


message CMessageUpdatePlayerInfoResponse
{
	optional uint32	RoleID					= 1;
	optional uint32 SessionID				= 2;
	optional uint32 ResultCode				= 3;
};


message CMessageBoursePlayerListNotice
{
	optional uint32 AccountID				= 1;
    optional PBBourseDetailList BuyList         = 2;
    optional PBBourseDetailList SellList        = 3;
};

message CMessageBoursePlayerListGet
{
	optional uint32 AccountID				= 1;	
};

message CMessageBourseTrade
{
	optional uint32 BuyPrice				= 1;
	optional uint32 SellPrice				= 2;
	optional uint32 Rating_id				= 3;
	optional uint32 Worldid					= 4;
};

message CMessageS2DBourseDecBuyDoc
{
	optional uint32 Doc						= 1;
	optional uint32 AccountID				= 2;
	optional uint32 RoleID					= 3;
};

message CMessageS2DBourseDecSellDoc
{
	optional uint32 Doc						= 1;
	optional uint32	AccountID				= 2;
	optional uint32 RoleID					= 3;
};

message CMessageBourseLogQuery
{
	optional uint32 AccountID				= 1;
};

message CMessageD2SBourseLog
{
	optional uint32 AccountID							= 1;
	optional PBBourseDetailList		buy_log				= 2;
	optional PBBourseDetailList		sell_log			= 3;

};







