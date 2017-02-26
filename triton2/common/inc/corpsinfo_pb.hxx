// 以最快速度序列/反序列
import "familydbinfo_pb.hxx";
option optimize_for = SPEED;


// 军团详细属性信息(军团成员可见)
message CorpsProperty
{
	optional uint32 CorpsMoney			 = 		1; 	// 军团金钱
	optional string CorpsRooterName		 = 		2; 	// 军团首领姓名
	optional uint32 CorpsFamilyNum		 = 		3; 	// 军团家族数量
	optional uint32 CorpsLevel			 = 		4; 	// 军团等级
	optional string CorpsButtullin		 = 		5; 	// 军团公告
	optional uint32 CorpsMemberNum		 = 		6; 	// 军团成员数量
	optional uint32	CorpsRooterID		 = 		7; 	// 军团首领角色ID
	optional uint32 CorpsID				 = 		8; 	// 军团ID
	optional string CorpsName			 = 		9; 	// 军团名称	 
	optional uint32	CorpsGlory			 =		10;	// 军团荣誉
	optional uint32 CorpsDevelop		 =		11;	// 军团发展度	
	
	optional uint32 WarRank				 =		12;			// 军团战综合排名
	optional uint32	WarWinNum			 =		13;			// 军团战争胜利次数
	optional uint32	WarLostNum			 =		14;			// 军团战争失败次数
	optional uint32	FamilyStarLevel		 =		15;			// 军团星级
	optional uint32	WarExp				 =		16;			// 军团战争积分
	optional uint32 WarMaxExp			 =		17;			// 军团战争积分最大值
	optional uint32	MaxFamilyNum		 =		18;			// 最大家族数量	    
	optional uint32	CanLevelUp			 =		19;			// 是否可以升级 
	optional uint32	ChallengeStatus		 =		20;			// 挑战状态
	optional uint32	ChallengeMoney		 =		21;			// 挑战金钱	
	optional uint32	MaxGeneralNum		 =		22;			// 前锋将军的最大数量
	optional uint32	MaxCounsellorNum	 =		23;			// 军团军师的最大数量      

	repeated PBCorpsBattle	BattleInfo	 =		24;			// 军团战争信息
	optional uint32	CountryID			 =		25;			// 军团所属国家ID	

	optional uint32	Color				 =		26;			// 军团颜色
	optional uint32	MainCityMap			 =		27;			// 主城地图ID
	optional uint32	LevelGlory		 =		29;			// 当前等级下升级需要的军团经验
	optional uint32	LevleMoney		 =		30;			// 当前等级下升级需要的军团金钱
	optional uint32 BOSSRefineValue		 =		31;		// 军团的炼化值
	optional uint32	RefineValue			 =		32;		// 炼化值上限
	repeated PBCorpsBoss BossList	 =		33;		// boss列表
	optional uint32 BossCDLimit			= 34;				//boss的cd时间
	optional uint32 BossCDCool			= 35;			//boss需要冷却的cd
	optional uint32 BossRepetionOpen	= 36;			// 1 open ,0 not open 
	optional uint32 CorpsCurBossID			= 37;					//当前召唤的boss id
	optional uint32	CorpsBossScore			= 38;		//	军团boss积分
	optional uint32 CorpsCollectActive		= 39;		// 军团采集活跃度
	optional uint32 CorpsReptionOpenTime	= 40;		// 军团boss副本开启时间
};

// 军团战信息结构
message PBCorpsBattle
{
	optional uint32	NpcID			= 1;	// 竞价NPC
	optional uint32	NpcStatus		= 2;	// NPC状态
	optional uint32 WarStatus		= 3;	// 战争状态
	optional string	DesCorpsName	= 4;	// 对战军团名称
	optional uint32	Type			= 5;	// 主城战、野外战
	optional uint32	BidMoney		= 6;	// 竞价金钱
	optional uint32 DesCorpsID		= 7;	// 对战军团ID
	optional uint32	MapID			= 8;	// 地图ID
	optional uint32 Time			= 9;	// 占领时间
	optional uint32	CorpsID			= 10;	// 占领军团ID
	optional string CorpsName		= 11;	// 占领军团名称
	optional string RooterName		= 12;	// 军团大将军
};

// 军团BOSS信息
message PBCorpsBoss
{
	optional uint32 BossId			= 1;	//boss id
	optional uint32 CanIntoTimes	= 2;	//可进入次数
};

// 战场信息列表
message PBCorpsBattleList
{
	repeated PBCorpsBattle BattleList = 1;
};


// 军团的成员信息
message PBCorpsMember
{
	optional PBFamilyMemberList		FamilyMemberList 	 = 		1;
	optional FamilyData				FamilyProperty		 = 		2; 
};

// 军团的所有成员信息
message PBCorpsMemberList
{
	repeated PBCorpsMember CorpsMemberList  = 		1; 
}; 


// 军团的申请信息列表
message PBCorpsApplyList
{
	repeated FamilyData ApplyList  = 		1; 
};


// 军团的基本信息
message PBCorpsBasicInfo
{		
	optional uint32 CorpsMemberNum		 = 		1; 	// 军团成员数量
	optional uint32 CorpsFamilyNum		 = 		2; 	// 军团家族数量
	optional uint32 CorpsID				 = 		3; 	// 军团ID
	optional uint32 CorpsMoney			 = 		4; 	// 军团金钱	  	
	optional string CorpsRooterName		 = 		5; 	// 军团首领姓名
	optional uint32	CorpsRooterID		 = 		6; 	// 军团首领ID  
	optional uint32 CorpsLevel			 = 		7; 	// 军团等级
	optional string CorpsName			 = 		8; 	// 军团名称	
	optional uint32	CorpsGlory			 =		9;	// 军团荣誉
	optional uint32 CorpsDevelop		 =		10;	// 军团发展度	
	optional uint32	MaxFamilyNum		 =		11;	// 最大家族数量	
	optional uint32	CorpsStarLevel		 =		12;	// 军团的星级       
	optional uint32	MainCityMap			 =		13;	// 主城地图ID	
};

// 军团信息列表
message CorpsInfoList
{	
	repeated PBCorpsBasicInfo CorpsList  = 		1; 
};	


// 军团的家族成员信息
message PBCorpsFamilyInfo
{
	optional uint32 Contribute  = 		1; 	
	optional uint32 FamilyID    = 		2; 	
};

// 军团的家族成员信息列表
message PBCorpsFamilyInfoList
{
	repeated PBCorpsFamilyInfo FamilyInfo  = 		1; 
};

// 联军基本信息
message PBUnionBasicInfo
{
	optional uint32 UnionID			 = 		1; 	// 联军ID	
	optional string UnionName		 = 		2; 	// 联军名称	
	optional uint32 CoreID			 =		3;	// 主军团ID
	optional string CoreName		 =		4;	// 主军团ID
	optional string RooterName		 = 		5; 	// 军团首领姓名
	optional uint32	RooterID		 = 		6; 	// 军团首领ID  	
	optional uint32 CityID			 =		7;	// 主城ID
	optional uint32	CorpsNum		 =		8;	// 军团数
	optional uint32	MemberNum		 =		9;	// 人口数
	repeated uint32	MapID			 =		10;	// 地图数
	optional uint32	Money			 =		11;	// 金钱
};

//  任务发布信息
message PBTaskListInfo
{
	optional uint32 TaskID			 = 		1; 	// 任务ID	
	optional uint32 TaskState		 = 		2; 	// 任务状态	
	optional uint32 EntityID		 = 		3; 	// 发布者
};


// 军团发布信息
message PBCorpsPublishInfo
{
	optional uint32 TodayTimes		= 		1; 	// 天次数
	optional uint32 WeekTimes		= 		2; 	// 周次数
	repeated PBTaskListInfo TaskList= 		3; 	// 任务列表
};
