
// 以最快速度序列/反序列
option optimize_for = SPEED;				

// 申请列表信息
message ApplyInfo								
{		
	optional string PlayerName			 = 		1; 
	optional uint32 PlayerLevel			 = 		2; 	
	optional uint32 FaceType			 = 		3; 
	optional uint32 LineID				 = 		4; 		
	optional uint32 PlayerCharID		 = 		5; 	
	optional uint32 PlayerOnLine		 = 		6; 
	optional uint32 PlayerMetier		 = 		7; 
	optional uint32 MapID				 = 		8; 
	optional uint32 PlayerSex			 = 		9; 
};

// 申请列表的玩家信息
message PBApplyList
{
	repeated ApplyInfo	  ApplyData	   = 		1; 
};

// 家族信息
message FamilyData
{ 
	optional string FamilyName			 = 		1; 			// 家族名称
	optional uint32 FamilyID			 = 		2; 			// 家族ID
	optional string HeaderName			 = 		3; 			// 宗正的名字
	optional uint32	HeaderRoleID		 = 		4; 			// 宗正的角色ID
	optional uint32 MemberNum			 = 		5; 			// 成员数量
	optional uint32	FamilyLevel			 = 		6; 			// 家族等级
	optional uint32	FamilyGlory			 =		7;			// 家族荣誉
	optional uint32 FamilyDevelop		 =		8;			// 家族发展度	
		
	optional uint32 NpcID				 =		9;			// 家族占领/竞价的NPCID
	optional uint32 BidMoney			 =		10;			// 家族竞价的钱数
	optional uint32	BidTime				 =		11;			// 家族竞价的时间
	optional uint32 WarNpcStatus		 =		12;			// 战争npc的状态0表示已经占领1表示处于争夺当中		
	optional uint32 WarRank				 =		13;			// 家族站综合排名
	optional uint32	WarWinNum			 =		14;			// 家族战争胜利次数
	optional uint32	WarLostNum			 =		15;			// 家族战争失败次数
	optional uint32	FamilyStarLevel		 =		16;			// 家族星级
	optional uint32	WarExp				 =		17;			// 家族战争积分
	optional uint32 WarMaxExp			 =		18;			// 家族战争积分最大值
	optional string EnemyFamilyName		 =		19;			// 对战家族名称	
	optional uint32	WarStatus			 =		20;			// 战争状态
	optional uint32 FamilyNpcNum		 =		21;			// 家族npc数量
	optional uint32	MaxMemberNum		 =		22;			// 最大人口数量
	optional string	CorpsName			 =		23;			// 所属军团的名称		
};

// 家族信息列表
message PBFamilyList
{
	repeated FamilyData	   FamilyInfo  = 		1; 
};


// 家族成员信息
message FamilyMemberInfo
{
	optional uint32 FaceID				 = 		1; 		// 脸型
	optional uint32 LineID				 = 		2; 		// 玩家所在的国家ID
	optional uint32	PlayerLevel			 = 		3; 		// 玩家等级
	optional uint32	PlayerPost			 = 		4; 		// 玩家职位	
	optional uint32 PlayerCharID		 = 		5; 		// 玩家角色ID
	optional uint32 RoleSex				 = 		6; 		// 角色性别
	optional string PlayerName			 = 		7; 		// 玩家姓名
	optional uint32 MetierID			 = 		8; 		// 职业ID
	optional uint32 Online				 = 		9; 		// 1表示在线,0表示不在线
	optional uint32 MapID				 = 		10; 	// 玩家所在的地图ID 
	optional uint32	FamilyWar			 =		11;		// 是否参加家族战
	optional uint32	Contribute			 =		12;		// 个人贡献度
	optional uint32	CorpsContribute		 =		13;		// 个人对军团的贡献度
	optional uint32	CorpsWar			 =		14;		// 是否参加军团战
	optional string	CorpsTitle			 =		15;		// 军团称号
};		  

// 家族成员信息列表
message PBFamilyMemberList
{
	repeated FamilyMemberInfo Members  = 		1;   	
};						


// 家族官职信息
message PBFamilyOrganInfo
{
	optional uint32 Rights		 = 		1; 		// 权利
	optional string OrganName    = 		2; 		// 官职名称
	optional uint32 ID			 = 		3; 		// 官职ID
};

// 官职信息列表
message PBFamilyOrganList
{ 
	repeated PBFamilyOrganInfo 	Organes  = 		1;  // 职位信息列表
};	


// 家族属性的基本信息
message PBFamilyPropery
{	
	optional uint32 FamilyHeaderID		 = 		1; 
	optional uint32 FamilyMoney			 = 		2; 
	optional uint32 FamilyLevel			 = 		3; 
	optional string FamilyNotice		 = 		4; 
	optional string FamilyHeaderName	 = 		5;   
	optional uint32 MemberNum			 = 		6; 
	optional uint32 FamilyID			 = 		7;	  
	optional uint32 NpcID				 =		8;			
	optional uint32 BidMoney			 =		9;			
	optional uint32	BidTime				 =		10;	
	optional uint32 WarNpc				 =		11;		// 战争npc的id
	optional uint32 WarNpcStatus		 =		12;		// 战争npc的状态0表示已经占领1表示处于争夺当中		
	optional uint32 WarRank				 =		13;		// 家族站综合排名
	optional uint32	WarWinNum			 =		14;		// 家族战争胜利次数
	optional uint32	WarLostNum			 =		15;		// 家族战争失败次数
	optional uint32	FamilyStarLevel		 =		16;		// 家族星级
	optional uint32	WarExp				 =		17;		// 家族战争积分
	optional uint32 WarMaxExp			 =		18;		// 家族战争积分最大值
	optional string EnemyFamilyName		 =		19;		// 对战家族名称
	repeated uint32	TaskID				 =		20;		// 家族任务列表	
	optional uint32	FamilyGlory			 =		21;		// 家族荣誉
	optional uint32 FamilyDevelop		 =		22;		// 家族发展度
	optional uint32	WarStatus			 =		23;		// 战争状态	
	
	optional uint32	ChallengeStatus		 =		24;			// 挑战家族的状态
	optional uint32	ChallengeMoney		 =		25;			// 挑战金额
	optional string	ChallengeStarted	 =		26;			// 最近发起的挑战
	optional string ChallengeReceived	 =		27;			// 最近收到的挑战
	optional uint32	MaxMemberNum		 =		28;			// 最大人口数量
	optional uint32	CanLevelUp			 =		29;			// 是否可以升级	
	
	optional uint32	MaxPresbyterNum		 =		30;			// 家族长老上限
	optional uint32	GloryLimit			 =		31;			// 当前等级下家族经验上限			
	optional uint32	LevelMoney			 =		32;			// 升级所需金钱
	optional uint32	Stability			 =		33;			// 家族安定度
	optional uint32	ReduceStability		 =		34;			// 每天衰减的安定度	
	optional uint32	ExtraRedstonExp		 =		35;			// 家族内丹额外经验加成	
};

// 家族成员信息(数据库存储)
message PBFamilyMemberInfo
{
	optional uint32 RoleID				=	1;	// 角色ID
	optional uint32 FamilyPost			=	2;	// 家族职位ID
	optional uint32	FamilyContribute	=	3;	// 家族贡献度
	optional uint32	CorpsContribute		=	4;	// 军团贡献度 
	optional uint32 CorpsPost			=	5;	// 军团职位ID	
	optional string	CorpsTitle			=	6;	// 军团称号
};

// 家族信息成员列表(数据库存储)
message PBFamilyMemberInfoList
{
	repeated PBFamilyMemberInfo MemberInfo  = 		1;  
}; 

// 家族军团商店的商品信息
message PBGoodItemInfo
{
	optional uint32		ItemID			= 1;
	optional uint32		ItemNum			= 2;
	optional uint32		ComposeID		= 3;
	optional uint32		PurcharseTime	= 4;
};

// 家族军团某个商店的物品信息
message PBLeagueShopInfo
{
	optional uint32		ShopID			= 1;
	optional uint32		ShopType		= 2;
	repeated PBGoodItemInfo	ItemInfo	= 3;			
};

// 家族军团所有商店的消息
message PBLeagueShopsListInfo
{
	repeated PBLeagueShopInfo LeagueShopInfo = 1;	// 家族商店相关	
};
