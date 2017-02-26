option optimize_for = SPEED;
import "familydbinfo_pb.hxx";
import "corpsinfo_pb.hxx";

enum CorpsMessageID
{ 
	ID_C2S_REQUEST_CREATCORPS					= 0x3800;		// 创建军团的请求
	ID_S2C_RESPONSE_CREATCORPS 					= 0x3801;		// 创建军团的回应消息
	
	ID_C2S_REQUEST_APPLYCORPS					= 0x3802;		// 申请加入军团的请求消息
	ID_S2C_RESPONSE_APPLYCORPS					= 0x3803;		// 申请加入军团的回应消息
	ID_S2C_NOTIFY_CORPSMEMBERAPPLY				= 0x3804;		// 有家族申请加入军团的消息通知
	ID_C2S_REQUEST_REPLYCORPSMEMBERAPPLY		= 0x3805;		// 回复家族申请加入军团的消息请求
	ID_S2C_RESPONSE_REPLYCORPSMEMBERAPPLY		= 0x3806;		// 回复家族申请加入军团的消息回应
	ID_S2C_NOTIFY_APPLYCORPSRESULT				= 0x3807;		// 申请加入军团的结果通知
	
	ID_C2S_REQUEST_INVITECORPSMEMBER			= 0x3808;		// 邀请家族加入军团的消息请求
	ID_S2C_RESPONSE_INVITECORPSMEMBER			= 0x3809;		// 邀请家族加入军团的消息回应
	ID_S2C_NOTIFY_CORPSINVITE					= 0x380A;		// 邀请加入军团的消息通知
	ID_C2S_REQUEST_REPLYCORPSINVITE				= 0x380B;		// 答复军团邀请的消息请求
	ID_S2C_RESPONSE_REPLYCORPSINVITE			= 0x380C;		// 答复军团邀请的消息回复
	ID_S2C_NOTIFY_CORPSINVITERESULT				= 0x380D;		// 邀请家族加入军团的结果通知
	
	ID_S2C_NOTIFY_FAMILYJOINCORPS				= 0x380E;		// 有家族加入军团的消息通知
	
	ID_C2S_REQUEST_LEAVECORPS					= 0x380F;		// 家族离开军团的消息请求
	ID_S2C_RESPONSE_LEAVECORPS					= 0x3810;		// 家族离开军团的消息回应
	
	ID_S2C_NOTIFY_FAMILYLEAVED					= 0x3811;		// 家族离开军团的消息通知
	
	ID_C2S_REQUEST_KICKFAMILY					= 0x3812;		// 从军团踢出家族的消息请求
	ID_S2C_RESPONSE_KICKFAMILY					= 0x3813;		// 从军团踢出家族的消息回应
	
	ID_S2C_NOTIFY_FAMILYKICKED					= 0x3814;		// 家族从军团里面被踢掉的通知	
	
	
	ID_C2S_REQUEST_GETCORPSFAMILYINFO			= 0x3815;		// 获取军团家族成员信息的请求
	ID_S2C_RESPONSE_GETCORPSFAMILYINFO			= 0x3816;		// 获取军团家族成员信息的回应消息
	
	ID_C2S_REQUEST_GETCORPSPROPERTYINFO			= 0x3817;		// 获取军团属性信息的请求消息
	ID_S2C_RESPONSE_GETCORPSPROPERTYINFO		= 0x3818;		// 获取军团属性信息的回应消息
	
	ID_C2S_REQUEST_GETCOPRSFAMILYMEMBER			= 0x3819;		// 获取军团中某个家族的成员信息的消息请求
	ID_S2C_RESPONE_GETCOPRSFAMILYMEMBER			= 0x381A;		// 获取军团中某个家族的成员信息的消息回应
	
	ID_C2S_REQUEST_CHANGECORPSBULLETIN			= 0x381B;		// 改变军团公告的消息请求
	ID_S2C_RESPONSE_CHANGECORPSBULLETIN			= 0x381C;		// 改变军团公告的消息回复
	
	ID_S2C_NOTIFY_CHANGECORPSBULLETIN			= 0x381D;		// 改变军团公告的消息通知
	
	ID_C2S_REQUEST_CHANGECORPSHEADER			= 0x381E;		// 改变军团首领的消息请求
	ID_S2C_RESPONSE_CHANGECORPSHEADER			= 0x381F;		// 改变军团首领的消息回复
	
	ID_S2C_NOTIFY_CHANGECORPSHEADER				= 0x3820;		// 改变军团首领的消息通知
	
	ID_C2S_REQUEST_GETCORPSAPPLYLIST			= 0x3821;		// 获取申请列表的消息请求
	ID_S2C_RESPONSE_GETCORPSAPPLYLIST			= 0x3822;		// 获取申请列表的消息回应
	
	ID_C2S_REQUEST_DISBANDCORPS					= 0x3823;		// 解散军团的消息请求
	ID_S2C_RESPONSE_DISBANDCORPS				= 0x3824;		// 解散军团的消息回应
	
	ID_S2C_NOTIFY_DISBANDCORPS					= 0x3825;		// 解散军团的消息通知
	
	ID_C2S_REQUEST_CANCELCORPSDISBAND			= 0x3826;		// 取消解散家族的消息请求
	ID_S2C_RESPONSE_CANCELCORPSDISBAND			= 0x3827;		// 取消解散家族的消息回应
	
	ID_C2S_REQUEST_CLEARCORPSAPPLY				= 0x3828;		// 清除军团申请列表消息请求
	ID_S2C_RESPONSE_CLEARCORPSAPPLY				= 0x3829;		// 清除军团申请列表的消息回应
	
	ID_C2S_REQUEST_REFRESHCORPSINFO				= 0x382A;		// 刷新军团信息的请求
	ID_S2C_RESPONSE_REFRESHCORPSINFO			= 0x382B;		// 刷新军团信息的消息回复
	
	ID_C2S_REQUEST_GETCOPRSLIST					= 0x382C;		// 获取军团信息的消息请求
	ID_S2C_RESPONSE_GETCORPSLIST				= 0x382D;		// 获取军团信息的消息回应

	ID_C2S_REQUEST_BATTLEBID					= 0x382E;		// 军团战报名竞价请求
	ID_S2C_RESPONSE_BATTLEBID					= 0x382F;		// 军团战报名竞价回复

	ID_C2S_REQUEST_QUERYBIDRANK					= 0x3830;		// 查看竞价排行榜请求
	ID_S2C_RESPONSE_QUERYBIDRANK				= 0x3831;		// 查看竞价排行榜回复

	ID_C2S_REQUEST_SELECTFAMILY					= 0x3832;		// 选择家族赋予参战资格请求
	ID_S2C_RESPONSE_SELECTFAMILY				= 0x3833;		// 选择家族赋予参战资格回复
	
	ID_C2S_REQUEST_SETCORPSPOST					= 0x3834;		// 设置军团成员职位的消息请求
	ID_S2C_RESPONSE_SETCORPSPOST				= 0x3835;		// 设置军团成员职位的消息回应	
	
	ID_C2S_REQUEST_LEVELUPCORPS					= 0x3836;		// 升级军团的消息请求
	ID_S2C_RESPONSE_LEVELUPCORPS				= 0x3837;		// 升级军团的消息回应
	
	ID_C2S_REQUEST_CONTRIBUTECORPSMONEY			= 0x3838;		// 贡献军团金钱的消息请求
	ID_S2C_RESPONSE_CONTRIBUTECORPSMONEY		= 0x3839;		// 贡献军团金钱的消息回应
	
	ID_C2S_REQUEST_FAMILYCONTRIBUTEMONEY		= 0x383A;		// 家族给军团贡献金钱的消息请求
	ID_S2C_RESPONSE_FAMILYCONTRIBUTEMONEY		= 0x383B;		// 家族给军团贡献金钱的消息回应
	
	ID_C2S_REQUEST_SETMEMBERNAME				= 0x383C;		// 设置军团成员称号的消息请求
	ID_S2C_RESPONSE_SETMEMBERNAME				= 0x383D;		// 设置军团成员称号的消息回应
	
	ID_S2C_NOTIFY_SETCORPSPOST					= 0x383E;		// 设置军团职位的通知
	
	ID_S2C_NOTIFY_CORPSLEVELUP					= 0x383F;		// 军团升级的通知
	
	ID_S2C_NOTIFY_CORPSPVPSTARLEVELUP				= 0x3840;		// 军团pvp等级提升的通知	

	ID_S2C_NOTIFY_CORPSBATTLERIGHTS				= 0x3841;		// 军团资格通知
	ID_S2C_NOTIFY_RECBATTLERIGHTS				= 0x3842;		// 家族参战资格通知

	ID_C2S_REQUEST_SELECTBATTLE					= 0x3843;			// 选择成员参加战斗请求
	ID_S2C_RESPONSE_SELECTBATTLE				= 0x3844;			// 选择成员参加战斗回应

	ID_S2C_NOTIFY_MEMBERBATTLERIGHTS			= 0x3845;			// 族人参战资格通知
	
	ID_S2C_NOTIFY_CORPSMONEYCHANGED				= 0x3846;			// 消耗军团金钱的通知
	ID_S2C_NOTIFY_CORPSGLORYCHANGED				= 0x3847;			// 军团荣耀改变的通知
	ID_S2C_NOTIFY_CORPSCONTRIBUTECHANGED		= 0x3848;			// 军团个人贡献度改变的通知 

	ID_C2S_REQUEST_MENDSHENGHUOTAI				= 0x3849;	// 修理圣火台请求
	ID_S2C_NOTIFY_CORPSPROPERTY					= 0x384A;	// 军团属性更新消息

	ID_C2S_REQUEST_CHALLEGE						= 0x384B;	// 请求挑战
	ID_S2C_RESPONSE_CHALLEGE					= 0x384C;	// 挑战回复
	ID_S2C_NOTIFY_CHALLEGE						= 0x384D;	// 挑战通知

	ID_C2S_REQUEST_ROOMOPERATION				= 0x384E;	// 房间请求
	ID_S2C_RESPONSE_ROOMOPERATION				= 0x384F;	// 房间回复
	ID_S2C_NOTIFY_ROOM							= 0x3850;	// 房间通知

	ID_C2S_REQUEST_REPLYCHALLEGE				= 0x3851;	// 回复挑战请求
	ID_S2C_RESPONSE_REPLYCHALLEGE				= 0x3852;	// 回复挑战请求的回复

	ID_C2S_REQUEST_QUERYROOM					= 0x3853;	// 查询房间请求
	ID_S2C_RESPONSE_QUERYROOM					= 0x3854;	// 查询房间回复

	ID_C2S_REQUEST_QUERYCHALLEGELIST			= 0x3855;	// 查询挑战列表
	ID_S2C_RESPONSE_QUERYCHALLEGELIST			= 0x3856;	// 查询挑战列表

	ID_C2S_REQUEST_QUERYRECCHALLEGE				= 0x3857;	// 收到的挑战列表查询
	ID_S2C_RESPONSE_QUERYRECCHALLEGE			= 0x3858;	// 收到的挑战列表查询
	
	ID_C2S_REQUEST_QUERYCORPSWELFARE			= 0x3859;	// 查询军团npc福利的消息请求
	ID_S2C_RESPONSE_QUERYCORPSWELFARE			= 0x385A;	// 查询军团npc福利的消息回应
	
	ID_C2S_REQUEST_OBTAINCORPSWELFARE			= 0x385B;	// 领取军团福利的消息请求
	ID_S2C_RESPONSE_OBTAINCORPSWELFARE			= 0x385C;	// 领取军团福利的消息回应
	
	ID_C2S_REQUEST_GETCORPSMONEY				= 0x385D;	// 把军团金钱领取为个人金钱的请求
	ID_S2C_RESPONSE_GETCORPSMONEY				= 0x385E;	// 把军团金钱领取为个人金钱的消息回应	

	ID_C2S_REQUEST_ABORTBATTLECITY				= 0x385F;
	ID_S2C_RESPONSE_ABORTBATTLECITY				= 0x3860;
	ID_S2C_NOTIFY_ABORTBATTLECITY				= 0x3861;
	
	ID_S2C_NOTIFY_CORPSCANLEVELUP				= 0x3862;	// 军团可以升级的消息通知

	ID_S2C_NOTIFY_CORPSBATTLERESULT				= 0x3863;

	ID_S2C_NOTIFY_NPCOWNERUPDATE				= 0x3864;	
	
	ID_S2C_NOTIFY_CORPSNAMECHANGED				= 0x3865;	// 军团称号改变的通知 


	ID_C2S_REQUEST_CREATEUNION					= 0x386E;		// 创建联军请求
	ID_S2C_RESPONSE_CREATEUNION					= 0x386F;		// 创建联军回应
	ID_S2C_NOTIFY_CREATEUNION					= 0x3870;		// 创建联军通知

	ID_C2S_REQUEST_APPLYUNION					= 0x3871;		// 申请加入联军的请求消息
	ID_S2C_RESPONSE_APPLYUNION					= 0x3872;		// 申请加入联军的回应消息
	ID_S2C_NOTIFY_UNIONMEMBERAPPLY				= 0x3873;		// 有军团申请加入联军的消息通知
	ID_C2S_REQUEST_REPLYUNIONMEMBERAPPLY		= 0x3874;		// 回复军团申请加入联军的消息请求
	ID_S2C_RESPONSE_REPLYUNIONMEMBERAPPLY		= 0x3875;		// 回复军团申请加入联军的消息回应
	ID_S2C_NOTIFY_APPLYUNIONRESULT				= 0x3876;		// 申请加入联军的结果通知

	ID_C2S_REQUEST_INVITEUNIONMEMBER			= 0x3877;		// 邀请军团加入联军的消息请求
	ID_S2C_RESPONSE_INVITEUNIONMEMBER			= 0x3878;		// 邀请军团加入联军的消息回应
	ID_S2C_NOTIFY_UNIONINVITE					= 0x3879;		// 邀请加入联军的消息通知
	ID_C2S_REQUEST_REPLYUNIONINVITE				= 0x387A;		// 答复联军邀请的消息请求
	ID_S2C_RESPONSE_REPLYUNIONINVITE			= 0x387B;		// 答复联军邀请的消息回复
	ID_S2C_NOTIFY_UNIONINVITERESULT				= 0x387C;		// 邀请军团加入联军的结果通知

	ID_S2C_NOTIFY_CORPSJOINUNION				= 0x387D;		// 有军团加入联军的消息通知

	ID_C2S_REQUEST_LEAVEUNION					= 0x387E;		// 军团离开联军的消息请求
	ID_S2C_RESPONSE_LEAVEUNION					= 0x387F;		// 军团离开联军的消息回应

	ID_S2C_NOTIFY_CORPSLEAVED					= 0x3880;		// 军团离开联军的消息通知

	ID_C2S_REQUEST_KICKCORPS					= 0x3881;		// 从联军踢出军团的消息请求
	ID_S2C_RESPONSE_KICKCORPS					= 0x3882;		// 从联军踢出军团的消息回应
	ID_S2C_NOTIFY_CORPSKICKED					= 0x3883;		// 军团从联军里面被踢掉的通知

	ID_C2S_REQUEST_MAKEUNION					= 0x3884;		// 创建联军请求
	ID_S2C_RESPONSE_MAKEUNION					= 0x3885;		// 创建联军回应
	ID_S2C_NOTIFY_MAKEUNION						= 0x3886;		// 创建联军通知
	ID_C2S_REQUEST_UNIONINFO					= 0x3887;		// 联军信息请求
	ID_S2C_RESPONSE_UNIONINFO					= 0x3888;		// 联军信息回应
	ID_C2S_REQUEST_DISMISSUNION					= 0x3889;		// 解散联军请求
	ID_S2C_RESPONSE_DISMISSUNION				= 0x388A;		// 解散联军回应
	ID_S2C_NOTIFY_DISMISSUNION					= 0x388B;		// 解散联军通知
	ID_C2S_REQUEST_UNIONAPPLYLIST				= 0x388C;		// 获得联军申请列表请求
	ID_S2C_RESPONSE_UNIONAPPLYLIST				= 0x388D;		// 获得联军申请列表回应
	ID_C2S_REQUEST_ALLUNIONLIST					= 0x388E;		// 获得联军列表请求
	ID_S2C_RESPONSE_ALLUNIONLIST				= 0x388F;		// 获得联军列表回应

	ID_C2S_REQUEST_GETOTHERMEMBER				= 0x3890;		// 获得其他军团参战人员列表
	ID_S2C_RESPONSE_GETOTHERMEMBER				= 0x3891;		// 获得其他军团参战人员列表

	ID_C2S_REQUEST_UNIONSUBMIT					= 0x3892;		// 军联盟参战
	ID_S2C_RESPONSE_UNIONSUBMIT					= 0x3893;		// 军联盟参战

	ID_C2S_REQUEST_GETALLMAP					= 0x3894;		// 所有已占地图
	ID_S2C_RESPONSE_GETALLMAP					= 0x3895;		// 所有已占地图

	ID_C2S_REQUEST_APPOINTPRINCE				= 0x3896;
	ID_S2C_RESPONSE_APPOINTPRINCE				= 0x3897;
	ID_S2C_NOTIFY_APPOINTPRINCE					= 0x3898;

	ID_C2S_REQUEST_MOVECAPITAL					= 0x3899;
	ID_S2C_RESPONSE_MOVECAPITAL					= 0x389A;

	ID_C2S_REQUEST_SELECTCOLOR					= 0x389B;
	ID_S2C_RESPONSE_SELECTCOLOR					= 0x389C;
	
	ID_C2S_REQUEST_GETCORPSGOODSLIST			= 0x389F;			// 获取军团商品列表的消息请求
	ID_S2C_RESPONSE_GETCORPSGOODSLIST			= 0x38A0;;			// 获取军团商品列表的消息回应

	ID_C2S_REQUEST_GETCORPSEXCHANGEGOODSLIST	= 0x38A1;		// 获取军团兑换商品列表的消息请求
	ID_S2C_RESPONSE_GETCORPSEXCHANGEGOODSLIST 	= 0x38A2;		// 获取军团兑换商品列表的消息回应
	
	ID_S2C_NOTIFY_CHANGECORPSGOODSLIST			= 0x38A3;		// 军团商店商品列表发生变化的通知
	ID_S2C_NOTIFY_CHANGEXCHANGEECORPSGOODSLIST	= 0x38A4;		// 军团兑换商店商品列表发生变化的通知	

	ID_C2S_REQUEST_CHANGEUNIONNAME				= 0x38A5;	// 军团联盟改名
	ID_S2C_RESPONSE_CHANGEUNIONNAME				= 0x38A6;
	ID_S2C_NOTIFY_CHANGEUNIONNAME				= 0x38A7;  
	
	ID_S2C_NOTIFY_USECORPSLEAGUEITEM			= 0x38A8;	// 使用军团特供物品的通知  

	ID_C2S_REQUEST_LOGINBATTLE					= 0x38A9;	// 进入战场请求
	ID_S2C_RESPONSE_LOGINBATTLE					= 0x38AA;	// 进入战场回复
	ID_S2C_CORPSBOSSTIMES_CHANGED_NOTIFY		= 0x38AB;	// 通知军团boss次数改变
	ID_S2C_CORPSBOSSREFINEVALUE_CHANGED_NOTIFY	= 0x38AC;	// 通知军团boss的炼化值

	ID_C2S_SUMMON_CORPSBOSS_REQUEST				= 0x38AD;	// 召唤军团boss
	ID_S2C_SUMMON_CORPSBOSS_RESPONSE			= 0x38AE;	// 召唤军团boss回应
	ID_S2C_CORPSBOSSREPETION_OPEN_NOTIFY		= 0x38AF;	// 军团boss开启通知
	ID_S2C_INTO_CORPSBOSS_REPETION_RESPONSE		= 0x38B0;	// 进入军团boss副本回应
	ID_S2C_CORPSBOSSREPETION_END_NOTIFY			= 0x38B1;	// 军团副本结束

	ID_S2C_NOTIFY_CORPSCOLLECTACTIVECHANGED		= 0x38B2;	// 军团采集活跃度变化通知
	ID_S2C_NOTIFY_CORPSPUBLISHTASK				= 0x38B3;	// 军团发布任务通知
};

// *************************************************************************** //
// CorpsHead  
// 军团消息头
// *************************************************************************** //
message CorpsHead
{
	optional uint32   RoleID	=	1;				// 角色ID
	optional uint32   MsgID     =	2;			    // 消息ID
	optional uint32	  EntityID	=	3;				// 在场景中的实体ID
};

// *************************************************************************** //
// CMessageCreatCorpsRequest  
// 创建家族的消息请求
// *************************************************************************** //
message CMessageCreatCorpsRequest
{
	optional CorpsHead	CorpHead	= 1;					// 军团消息头
	optional string		CorpsName	= 2;				    // 军团名称
	optional uint32		NpcEntityID = 3;					// 军团NPC的实体ID
	optional uint32		CorpsColor	= 4;					// 军团颜色
};

// *************************************************************************** //
// CMessageCreatCorpsRequest  
// 创建家族的消息请求
// *************************************************************************** //
message CMessageCreatCorpsResponse
{
	optional CorpsHead	CorpHead	= 1;			// 军团消息头
	optional uint32		Errcode		= 2;			// 0表示成功其他表示错误码
	optional string		CorpsName	= 3;			// 军团名称
	optional uint32		CorpsID		= 4;			// 军团ID
	optional uint32		CorpsColor	= 5;			// 军团颜色
};

// *************************************************************************** //
// CMessageApplyCorpsRequest  
// 申请加入军团的消息请求
// *************************************************************************** //
message CMessageApplyCorpsRequest
{		
	optional CorpsHead	CorpHead		= 1;			// 军团消息头
	optional string		CorpsName		= 2;			//	军团名称
	optional uint32		CorpsID			= 3;			//	军团ID
	optional uint32		RoleID			= 4;			//	玩家角色ID
};



// *************************************************************************** //
// CMessageApplyCorpsResponse  
// 申请加入军团的消息回应
// *************************************************************************** //
message CMessageApplyCorpsResponse
{			
	optional CorpsHead	CorpHead	= 1;			// 军团消息头
	optional uint32		Errcode		= 2;			// 错误码0表示成功其他表示错误码
	optional string		CorpsName	= 3;			// 军团名称
};

// *************************************************************************** //
// CMessageCorpsMemberApplyNotify  
// 有家族申请加入军团的消息通知
// *************************************************************************** //
message CMessageCorpsMemberApplyNotify
{			
	optional CorpsHead	CorpHead	= 1;			// 军团消息头
	optional FamilyData FamilyInfo	= 2;			// 家族属性信息
};

// *************************************************************************** //
// CMessageReplyCorpsMemberApplyRequest  
// 答复家族申请的消息请求
// *************************************************************************** //
message CMessageReplyCorpsMemberApplyRequest
{																		
	optional CorpsHead	CorpHead	= 1;				// 军团消息头
	optional uint32		Result		= 2;				// 对于家族申请的回复1表示同意0表示不同意
	optional uint32		FamilyID	= 3;				// 申请加入的家族的ID		
};

// *************************************************************************** //
// CMessageReplyCorpsMemberApplyResponse  
// 有家族申请加入军团的消息通知
// *************************************************************************** //
message CMessageReplyCorpsMemberApplyResponse
{
	optional CorpsHead	CorpHead	= 1;				// 军团消息头
	optional uint32		Errcode		= 2;				// 0表示操作成功其他表示错误码
	optional uint32		Result		= 3;				// 之前的答复结果
	optional uint32		FamilyID	= 4;				// 家族ID		
};

// *************************************************************************** //
// CMessageApplyCorpsResultNotify  
// 申请加入军团的结果通知
// *************************************************************************** //
message CMessageApplyCorpsResultNotify
{
	optional CorpsHead	CorpHead	= 1;				// 军团消息头
	optional uint32 ResultCode		= 2;				// 1表示加入成功0表示被拒绝其他表示错误码
	optional uint32 CorpsID			= 3;				// 军团ID
	optional string CorpsName		= 4;				// 军团名称
	optional uint32	UnionID			= 5;				// 军团联盟ID
	optional string	UnionName		= 6;				// 军团联盟名称
};


// *************************************************************************** //
// CMessageInviteCorpsMemberRequest  
// 邀请家族加入军团的消息请求
// *************************************************************************** //
message CMessageInviteCorpsMemberRequest
{
	optional CorpsHead	CorpHead	= 1;				// 军团消息头
	optional string FamilyName		= 2;				// 家族名称
	optional uint32 FamilyID		= 3;				// 家族ID,目前不用
	optional uint32	MemberID		= 4;				// 角色ID		
};


// ***************************************************************************//
// CMessageInviteCorpsMemberResponse  
// 邀请家族加入军团的消息回应
// ***************************************************************************//
message CMessageInviteCorpsMemberResponse
{
	optional CorpsHead	CorpHead	= 1;				// 军团消息头
	optional uint32 Errcode			= 2;				// 错误码0表示成功其他表示错误码
	optional string FamilyName		= 3;				// 家族名称
};

// ***************************************************************************//
// CMessageCorpsInviteNotify  
// 邀请加入军团的消息通知
// ***************************************************************************//
message CMessageCorpsInviteNotify
{
	optional CorpsHead	CorpHead		= 1;			// 军团消息头
	optional string InvitingName		= 2;			// 邀请者姓名
	optional uint32 InvitingID			= 3;			// 邀请者ID	
	optional PBCorpsBasicInfo CorpsInfo = 4;			// 军团信息 
};	

// ***************************************************************************//
// CMessageReplyCorpsInviteRequest  
// 答复军团邀请的消息请求
// ***************************************************************************//
message CMessageReplyCorpsInviteRequest
{
	optional CorpsHead	CorpHead		= 1;			// 军团消息头
	optional uint32	 Result				= 2;			// 1表示同意加入军团0表示拒绝加入军团
	optional uint32	 CorpsID			= 3;			// 军团ID
	optional uint32	 InvitingID			= 4;			// 邀请者ID
};

// ***************************************************************************//
// CMessageReplyCorpsInviteResponse  
// 答复军团邀请的消息回复
// ***************************************************************************//
message CMessageReplyCorpsInviteResponse
{
	optional CorpsHead	CorpHead		= 1;			// 军团消息头
	optional uint32	Errcode				= 2;			// 错误码0表示成功其他表示错误码
	optional uint32 Result				= 3;			// 答复结果
	optional string	CorpsName			= 4;			// 军团名称
	optional uint32 CorpsID				= 5;			// 军团ID
	optional uint32	UnionID				= 6;			// 军团联盟ID
	optional string	UnionName			= 7;			// 军团联盟名称	
};

// ***************************************************************************//
// CMessageCorpsInviteResultNotify  
// 邀请家族加入军团的结果
// ***************************************************************************//
message CMessageCorpsInviteResultNotify
{
	optional CorpsHead	CorpHead		= 1;			// 军团消息头
	optional uint32 Result				= 2;			// 1表示邀请成功0表示被拒绝其他表示错误码
	optional string FamilyName			= 3;			// 被邀请的家族的名称
};	 

																						
// ***************************************************************************//
// CMessageFamilyJoinCorpsNotify															  
// 有家族加入军团的消息通知
// ***************************************************************************//
message CMessageFamilyJoinCorpsNotify
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional PBCorpsMember	FamilyInfo		= 2;			// 家族属性信息
	optional uint32			UnionID			= 3;			// 军团联盟ID
	optional string			UnionName		= 4;			// 军团联盟名称	
};

// ***************************************************************************//
// CMessageFamilyLeaveCorpsRequest  
// 家族离开军团的消息请求
// ***************************************************************************//
message CMessageFamilyLeaveCorpsRequest
{
	optional CorpsHead	CorpHead	= 1;			// 军团消息头
	optional uint32		Msgcode		= 2;			// 无用参数
};

// ***************************************************************************//
// CMessageFamilyLeaveCorpsResponse  
// 家族离开军团的消息回应
// ***************************************************************************//
message CMessageFamilyLeaveCorpsResponse
{
	optional CorpsHead	CorpHead	= 1;			// 军团消息头
	optional uint32		Errcode		= 2;			// 错误码0表示成功其他表示错误码	
};

// ***************************************************************************//
// CMessageFamilyLeaveCorpsNotify  
// 家族离开军团的消息通知
// ***************************************************************************//
message CMessageFamilyLeaveCorpsNotify
{
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
	optional uint32	FamilyID				= 2;			// 家族ID
	optional string FamilyName				= 3;			// 家族名称
};

// ***************************************************************************//
// CMessageKickFamilyRequest  
// 从军团踢出家族的消息请求
// ***************************************************************************//
message CMessageKickFamilyRequest
{
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
	optional uint32 FamilyID				= 2;			// 被踢掉的家族ID				
};

// ***************************************************************************//
// CMessageKickFamilyResponse  
// 从军团踢出家族的消息回应
// ***************************************************************************//
message CMessageKickFamilyResponse
{
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
	optional uint32 Errcode					= 2;			// 错误码0表示成功其他表示错误码
	optional string FamilyName				= 3;			// 被踢出的家族名称
	optional uint32 FamilyID				= 4;			// 被踢出的家族ID
};

// ***************************************************************************//
// CMessageKickFamilyNotify  
// 家族从军团里面被踢掉的通知
// ***************************************************************************//
message CMessageKickFamilyNotify
{												
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
	optional uint32 FamilyID				= 2;			// 被踢掉的家族ID
	optional string	FamilyName				= 3;			// 被踢掉的家族名称
};

// ***************************************************************************//
// 获取军团家族成员信息的请求  
// CMessageGetCorpsFamilyInfoRequest
// ***************************************************************************//
message CMessageGetCorpsFamilyInfoRequest
{
	optional CorpsHead		CorpHead  = 1;			// 军团消息头
	optional uint32			MsgCode	  = 2;			// 无用参数
};

// ***************************************************************************//
// CMessageGetCorpsFamilyInfoResponse  
// 获取军团家族成员信息的回应消息
// ***************************************************************************//
message CMessageGetCorpsFamilyInfoResponse
{
	optional CorpsHead			CorpHead			= 1;			// 军团消息头
	repeated PBCorpsMember		CorpsMemberInfo		= 2;			// 家族信息
}; 

// ***************************************************************************//
// CMessageGetCorpsPropertyRequest  
// 获取军团属性信息的请求消息
// ***************************************************************************//
message CMessageGetCorpsPropertyRequest
{
	optional CorpsHead		CorpHead  = 1;			// 军团消息头
	optional uint32			MsgCode	  = 2;			// 无用参数	
};

// ***************************************************************************//
// CMessageGetCorpsPropertyResponse  
// 获取军团属性信息的回应消息
// ***************************************************************************//
message CMessageGetCorpsPropertyResponse
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional CorpsProperty CorpProperty		= 2;			// 军团属性		
};

// ***************************************************************************//
// CMessageGetCorpsFamilyMemberRequest  
// 获取军团中某个家族的成员信息的消息请求
// ***************************************************************************//
message CMessageGetCorpsFamilyMemberRequest
{
	optional CorpsHead		CorpHead  = 1;			// 军团消息头
	optional uint32			FamilyID  = 2;			// 家族ID
};

// ***************************************************************************//
// CMessageGetCorpsFamilyMemberResponse  
// 获取军团中某个家族的成员信息的消息回应
// ***************************************************************************//
message CMessageGetCorpsFamilyMemberResponse
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			FamilyID		= 2;			// 家族ID
	optional string			FamilyName		= 3;			// 家族名称
	optional PBFamilyMemberList MemberList	= 4;			// 家族成员列表
};

// ***************************************************************************//
// CMessageChangeCorpsBulletinRequest  
// 改变军团公告的消息请求
// ***************************************************************************//
message CMessageChangeCorpsBulletinRequest
{
	optional CorpsHead		CorpHead  = 1;			// 军团消息头
	optional string			Bulletin  = 2;			// 公告内容				
};	

// ***************************************************************************//
// CMessageChangeCorpsBulletinResponse  
// 改变军团公告的消息回复
// ***************************************************************************//
message CMessageChangeCorpsBulletinResponse
{
	optional CorpsHead		CorpHead	= 1;			// 军团消息头
	optional uint32			Errcode		= 2;			// 错误码0表示成功其他表示错误码
	optional string			Bulletin	= 3;			// 公告内容		
};

// ***************************************************************************//
// CMessageChangeCorpsBulletinNotify  
// 改变军团公告的消息通知
// ***************************************************************************//
message CMessageChangeCorpsBulletinNotify
{
	optional CorpsHead		CorpHead  = 1;			// 军团消息头
	optional string			Bulletin  = 2;			// 公告内容			
};

// ***************************************************************************//
// CMessageChangeCorpsHeaderRequest  
// 改变军团首领的消息请求
// ***************************************************************************//
message CMessageChangeCorpsHeaderRequest
{
	optional CorpsHead		CorpHead  = 1;			// 军团消息头
	optional uint32			MemberID  = 2;			// 要转让军团首领职位的成员ID
};

// ***************************************************************************//
// CMessageChangeCorpsHeaderResponse  
// 改变军团首领的消息回复
// ***************************************************************************//
message CMessageChangeCorpsHeaderResponse
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			Errcode			= 2;			// 0表示成功其他表示错误码
	optional uint32			MemberID		= 3;			// 角色ID
	optional string			MemberName		= 4;			// 角色名称
}; 

// ***************************************************************************//
// CMessageChangeCorpsHeaderNotify  
// 改变军团首领的消息通知
// ***************************************************************************//
message CMessageChangeCorpsHeaderNotify
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			HeaderRoleID	= 2;			// 首领角色ID
	optional string			HeaderRoleName	= 3;			// 首领名称
	optional uint32			MemberRoleID	= 4;			// 被转让职位的角色ID
	optional string			MemberRoleName	= 5;			// 被转让职位的角色名称
};

// ***************************************************************************//
// CMessageGetCorpsApplyListRequest  
// 获取军团申请列表的消息请求
// ***************************************************************************//
message CMessageGetCorpsApplyListRequest
{												
	optional CorpsHead		CorpHead  = 1;			// 军团消息头
	optional uint32			MsgCode	  = 2;			// 无用参数	
};

// ***************************************************************************//
// CMessageGetCorpsApplyListResponse  
// 获取军团申请列表的消息回应
// ***************************************************************************//
message CMessageGetCorpsApplyListResponse
{
	optional CorpsHead			CorpHead	 = 1;			// 军团消息头
	optional uint32				MsgCode		 = 2;			// 无用参数
	optional PBCorpsApplyList	CorpsApply	 = 3;			// 家族的申请列表信息
};

// ***************************************************************************//
// CMessageDiBandCorps  
// 解散军团的消息请求
// ***************************************************************************//
message CMessageDisbandCorpsRequest
{
	optional CorpsHead		CorpHead	= 1;			// 军团消息头
	optional uint32			NpcEntityID = 2;			// 军团NPC的实体ID
};

// ***************************************************************************//
// CMessageDiBandCorpsResponse  
// 解散军团的消息回应
// ***************************************************************************//
message CMessageDisbandCorpsResponse
{
	optional CorpsHead		CorpHead  = 1;			// 军团消息头
	optional uint32			Errcode	  = 2;			// 无用参数
};


// ***************************************************************************//
// CMessageDiBandCorpsNotify  
// 解散军团的消息通知
// ***************************************************************************//
message CMessageDisbandCorpsNotify
{	
	optional CorpsHead		CorpHead  = 1;			// 军团消息头
	optional uint32			MsgCode	  = 2;			// 无用参数
};


// ***************************************************************************//
// CMessageCancelCorpsDisbandRequest  
// 取消解散家族的消息请求
// ***************************************************************************//
message CMessageCancelCorpsDisbandRequest
{
	 optional CorpsHead		CorpHead  = 1;			// 军团消息头
	 optional uint32		MsgCode	  = 2;			// 无用参数
};

// ***************************************************************************//
// CMessageCancelCorpsDisbandResponse  
// 取消解散家族的消息回应
// ***************************************************************************//
message CMessageCancelCorpsDisbandResponse
{
	optional CorpsHead	CorpHead  = 1;			// 军团消息头
	optional uint32		ErrCode	  = 2;			// 0表示成功其他表示错误码
};

// ***************************************************************************//
// CMessageClearCorpsApplyRequest  
// 清除军团申请列表的消息请求
// ***************************************************************************//
message CMessageClearCorpsApplyRequest
{
	optional CorpsHead	CorpHead  = 1;			// 军团消息头
	optional uint32		MsgCode	  = 2;			// 无用参数
};

// ***************************************************************************//
// CMessageClearCorpsApplyResponse
// 清除军团申请列表的消息回应
// ***************************************************************************//
message CMessageClearCorpsApplyResponse
{
	optional CorpsHead	CorpHead  = 1;			// 军团消息头
	optional uint32		ErrCode	  = 2;			// 0表示操作成功其他表示错误码
};

// ***************************************************************************//
// CMessageRefreshCorpsInfoRequest
// 刷新军团的家族消息的请求
// ***************************************************************************//
message CMessageRefreshCorpsInfoRequest
{
	optional CorpsHead		CorpHead	= 1;			// 军团消息头
	optional uint32			MsgCode		= 2;			// 无用参数
};

// ***************************************************************************//
// CMessageRefreshCorpsInfoResponse
// 刷新军团的家族消息的消息回应
// ***************************************************************************//
message CMessageRefreshCorpsInfoResponse
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	repeated PBCorpsMember	CorpsMember		= 2;			// 家族信息
};

// ***************************************************************************//
// CMessageGetCorpsListRequest
// 获取军团列表的消息请求
// ***************************************************************************//
message CMessageGetCorpsListRequest
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			CurPage			= 2;			// 获取的列表页数
	
};

// ***************************************************************************//
// CMessageGetCorpsListResponse
// 获取军团列表的消息回应
// ***************************************************************************//
message CMessageGetCorpsListResponse
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional CorpsInfoList	CorpsList		= 2;			// 军团信息列表
	optional uint32			CurPage			= 3;			// 获取的列表页数
	optional uint32			TotalPage		= 4;			// 总页数
};


// ***************************************************************************//
// CMessageSetCorpsPostRequest
// 设置军团成员职位的消息请求
// ***************************************************************************//
message CMessageSetCorpsPostRequest
{
	optional CorpsHead	CorpHead		=	1;			// 军团消息头
	optional uint32		MemberID		=	2;			// 成员ID
	optional uint32		Post			=	3;			// 称号	
};

// ***************************************************************************//
// CMessageSetCorpsPostRequest
// 设置军团成员职位的消息请求
// ***************************************************************************//
message CMessageSetCorpsPostResponse
{
	optional CorpsHead	CorpHead		=	1;			// 军团消息头
	optional uint32		MemberID		=	2;			// 成员ID
	optional uint32		Post			=	3;			// 职位
	optional uint32		Errcode			=	4;			// 错误码
};

// ***************************************************************************//
// CMessageSetLevelUPCorpsRequest
// 升级军团的消息请求
// ***************************************************************************//
message CMessageLevelUPCorpsRequest
{
	optional CorpsHead	CorpHead	=	1;		// 军团消息头
	optional uint32		MsgCode		=	2;		// 无用参数	
};

// ***************************************************************************//
// CMessageLevelUPCorpsResponse
// 升级军团的消息回应
// ***************************************************************************//
message CMessageLevelUPCorpsResponse
{
	optional CorpsHead	CorpHead	=	1;		// 军团消息头
	optional uint32		Errcode		=	2;		// 错误码
	optional uint32		Level		=	3;		// 升级后的等级
};

// ***************************************************************************//
// CMessageContributeCorpsMoneyRequest
// 个人贡献军团金钱的消息请求
// ***************************************************************************//
message CMessageContributeCorpsMoneyRequest
{													
	optional CorpsHead	CorpHead	=	1;		// 军团消息头
	optional uint32		Money		=	2;		// 个人捐献的金钱		
};


// ***************************************************************************//
// CMessageContributeCorpsMoneyResponse
// 个人贡献军团金钱的消息回应
// ***************************************************************************//
message CMessageContributeCorpsMoneyResponse
{
	optional CorpsHead	CorpHead	=	1;		// 军团消息头
	optional uint32		Errcode		=	2;		// 错误码
	optional uint32		Money		=	3;		// 个人捐献的金钱
};

// ***************************************************************************//
// CMessageFamilyContributeMoneyRequest
// 家族给军团贡献金钱的消息请求
// ***************************************************************************//
message CMessageFamilyContributeMoneyRequest
{
	optional CorpsHead	CorpHead	=	1;		// 军团消息头
	optional uint32		Money		=	2;		// 家族金锭
};

// ***************************************************************************//
// CMessageFamilyContributeMoneyResponse
// 家族给军团贡献金钱的消息回应
// ***************************************************************************//
message CMessageFamilyContributeMoneyResponse
{
	optional CorpsHead	CorpHead	=	1;		// 军团消息头
	optional uint32		Errcode		=	2;		// 错误码
	optional uint32		Money		=	3;		// 捐献的家族金钱	
};

// ***************************************************************************//
// CMessageSetMemberNameRequest
// 设置军团成员称号的消息请求
// ***************************************************************************//
message CMessageSetMemberNameRequest
{
	optional CorpsHead	CorpHead	=	1;		// 军团消息头
	optional uint32		MemberID	=	2;		// 成员ID
	optional string		Name		=	3;		// 名称
};

// ***************************************************************************//
// CMessageSetMemberNameResponse
// 设置军团成员称号的消息回应
// ***************************************************************************//
message CMessageSetMemberNameResponse
{  
	optional CorpsHead	CorpHead	=	1;		// 军团消息头
	optional uint32		Errcode		=	2;		// 错误码
	optional uint32		MemberID	=	3;		// 成员ID
	optional string		Name		=	4;		// 称号
};

// ***************************************************************************//
// CMessageSetCorpsPostNotify
// 设置军团职位的通知
// ***************************************************************************//
message CMessageSetCorpsPostNotify
{
	optional CorpsHead	CorpHead		=	1;		// 军团消息头
	optional uint32		ManagerID		=	2;		// 任免人的职位
	optional string		ManagerName		=	3;		// 任免人的名字
	optional uint32		MemberID		=	4;		// 成员ID
	optional string		MemberName		=	5;		// 成员名称
	optional uint32		OriginalPost	=	6;		// 原来的职位
	optional uint32		NewPost			=	7;		// 新职位
};

// ***************************************************************************//
// CMessageCorpsLevelUPNotify
// 军团升级的消息通知
// ***************************************************************************//
message CMessageCorpsLevelUPNotify
{		
	optional CorpsHead	CorpHead			=	1;		// 军团消息头
	optional uint32		Level				=	2;		// 当前等级
	optional uint32		Money				=	3;		// 军团当前金钱
	optional uint32		Glory				=	4;		// 军团当前荣耀
	optional uint32		MaxFamilyNum		=	5;		// 升级后的军团数量上限
	optional uint32		MaxGeneralNum		=	6;		// 前锋将军数量上限
	optional uint32		MaxCounsellorNum	=	7;		// 最大的军师数量 		
	optional uint32		LevelMoney			=	8;		// 升级需要的金钱
	optional uint32		LevelGlory			=	9;		// 升级需要的荣耀
};

// ***************************************************************************//
// CMessageCorpsBattleBidRequest
// 战争竞价报名请求
// ***************************************************************************//

message CMessageCorpsBattleBidRequest
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			NpcID			= 2;			// 竞价NPC
	optional uint32			Money			= 3;			// 竞价金钱
	optional uint32			MinIncMoney		= 4;			// 加价最低值(不需要客户端发)
	optional uint32			MapID			= 5;
};

// ***************************************************************************//
// CMessageCorpsBattleBidResponse
// 战争竞价报名回复
// ***************************************************************************//

message CMessageCorpsBattleBidResponse
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			RtnCode			= 2;			// 竞价报名是否成功
	optional uint32			NpcID			= 3;
	optional uint32			Money			= 4;
};

// ***************************************************************************//
// CMessageCorpsQueryBidRankRequest
// 获取竞价排行榜信息请求
// ***************************************************************************//

message CMessageCorpsQueryBidRankRequest
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			NpcID			= 2;			// 竞价Npc	
	optional uint32			BattleCity		= 3;			// 城池ID(客户端不需要填充)
};

message PBBidRank
{
	optional string		CorpsName		= 1;	// 军团名称
	optional uint32		Money			= 2;	// 竞标价格
	optional uint32		Nationality		= 3;	// 国家ID
	optional uint32		Victory			= 4;	// 战胜次数
	optional uint32		Loser			= 5;	// 战败次数
	optional uint32		StarLevel		= 6;	// 星级
	optional uint32		CorpsID			= 7;	// 军团ID
}; 

// ***************************************************************************//
// CMessageCorpsQueryBidRankResponse
// 获取竞价排行榜信息回复
// ***************************************************************************//

message CMessageCorpsQueryBidRankResponse
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			NpcID			= 2;			// 竞价Npc
	optional string			CityOwner		= 3;			// 城池当前拥有者
	repeated PBBidRank		RankInfo		= 4;			// 竞价排行榜列表
};

// ***************************************************************************//
// CMessageCorpsSelectFamilyBattleRequest
// 赋予军团家族参战资格请求
// ***************************************************************************//

message CMessageCorpsSelectFamilyBattleRequest
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	repeated uint32			FamilyID		= 2;			// 家族ID(选择参战的家族)
};

// ***************************************************************************//
// CMessageCorpsSelectFamilyBattleResponse
// 赋予军团家族参战资格请求的回复
// ***************************************************************************//		
message CMessageCorpsSelectFamilyBattleResponse
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			RtnCode			= 2;			// 
};
	
// ***************************************************************************//
// CMessagePvPStarLevelUpNotify
// 军团pvp星级提高的通知
// ***************************************************************************//		
message CMessageCorpsPvPStarLevelUpNotify
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			PvPLevel		= 2;			// 军团pvp星级
};
 
//***************************************************
// 获得参战资格通知
//***************************************************

message CMessageCorpsBattleRightsNotify
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			NpcID			= 2;
	optional string			DesCorpsName	= 3;
	optional uint32			Type			= 4;			// 战争类型
};

// 通知家族族长获得参战资格
message CMessageCorpsRecBattleRightsNotify
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			NpcID		= 2;		
	optional uint32			SrcCorpsID	= 3;
	optional string			SrcCorpsName = 4;
	optional uint32			DesCorpsID	= 5;
	optional string			DesCorpsName = 6;
	optional uint32			Money = 7;
};

message CMessageCorpsSelectMemberBattleRequest
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	repeated uint32		MemberList		= 2;		// 参战玩家列表(char id)
	optional uint32		BattleCity		= 3;
	optional uint32		Type			= 4;		// 组织类型:军团、军盟
};

message CMessageCorpsSelectMemberBattleResponse
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32		RtnCode			= 2;
};

// 通知族人可以参战

message CMessageCorpsMemberBattleRightsNotify
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32		NpcID		= 2;	// npcid
	repeated uint32		MemberList		= 3;
	optional string		DesCorpsName	= 4;
	optional uint32		Type			= 5;		// 组织类型:军团、军盟
};

// ***************************************************************************//
// CMessageConsumeCorpsMoneyNotify	
// 消耗军团金钱的通知
// ***************************************************************************//
message CMessageCorpsMoneyChangedNotify
{
	optional uint32		Money	= 1;			// 军团现在的金钱
	optional uint32		RoleID	= 2;			// 改变军团金钱的角色ID	
	optional uint32		Reason	= 3;			// 军团金钱改变的原因
	optional string		RoleName= 4;			// 改变军团金钱的角色名称  
	optional int32		Valeu	= 5;			// 差值
};

// ***************************************************************************//
// CMessageCorpsGloryChangedNotify	
// 军团荣耀改变的通知
// ***************************************************************************//
message CMessageCorpsGloryChangedNotify
{
	optional uint32		Glory	= 1;			// 军团现在的荣耀
	optional uint32		RoleID	= 2;			// 改变军团荣耀的角色ID	
	optional uint32		Reason	= 3;			// 军团荣耀改变的原因
	optional string		RoleName= 4;			// 改变军团金钱的角色名称
	optional int32		Value	= 5;			// 差值
};

// ***************************************************************************//
// CMessageCorpsContributeChangedNotify	
// 军团的个贡献度改变的通知
// ***************************************************************************//
message CMessageCorpsContributeChangedNotify
{
	optional CorpsHead	CorpHead	= 1;			// 军团消息头
	optional uint32		Contribute	= 2;			// 玩家的个人贡献度
	optional uint32		RoleID		= 3;			// 玩家的角色ID
	optional int32		Value		= 4;			// 差值
};

// ***************************************************************************//
//
// ***************************************************************************//
message CMessageCorpsBossTimesChangedNotify
{
	optional uint32 BossID		= 1;			//boss id
	optional uint32 RoleID		= 2;			//改变次数的角色ID
	optional uint32 Reason		= 3;			//改变原因
	optional string RoleName	= 4;			//改变角色的名称
	optional uint32 Times		= 5;			//现在可以挑战的次数
	optional uint32 Value		= 6;			//差值
};

message CMessageCorpsBossRefineValueNotify
{
	optional uint32 RefineValue		= 1;		// 军团现在的炼化值
	optional uint32 RoleID			= 2;		// 改变炼化值的 角色ID
	optional uint32	Value			= 3;		// 改变值
	optional uint32 Reason			= 4;		// 改变的原因
	optional string RoleName		= 5;		// 角色的名称
};

message	CMessageCorpsCollectActiveChangeNotify
{
	optional CorpsHead	CorpHead		= 1;		// 军团消息头
	optional uint32		ActiveValue		= 2;		// 军团现在的采集活跃度值
	optional uint32		Vaule			= 3;		// 改变值
	optional uint32		ChangeType		= 4;		// 增加或减少
}

// 修理圣火台消息请求
message CMessageMendShengHuoTaiRequest
{
	optional uint32	NpcEntityID = 1;	// 圣火台 entityid
};

// 通知更新军团属性
message CMessageCorpsPropertyNotice
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional CorpsProperty  Property		= 2;
};

// 房间请求
message CMessageCorpsOperateRoomRequest
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	optional uint32			Action			= 2;		// 1:开房间;0:关房间
	optional uint32			Money			= 3;
	optional string			Name			= 4;
};

// 房间回复
message CMessageCorpsOperateRoomResponse
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	optional uint32			Action			= 2;		// 1:开房间;0:关房间
	optional uint32			RtnCode			= 3;
	optional uint32			Money			= 4;
	optional string			Name			= 5;
};

// 房间变化通知军团所有成员
message CMessageCorpsRoomNotice
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	optional uint32			Action			= 2;
	optional string			RooterName		= 3;
};

// 发起挑战请求
message CMessageCorpsChallegeRequest
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	optional uint32			Action			= 2;		// 发起、取消
	optional uint32			DesCropsID		= 3;		// 对方军团ID
};

// 发起挑战回复
message CMessageCorpsChallegeResponse
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	optional uint32			RtnCode			= 2;
	optional uint32			Action			= 3;
	optional uint32			DesCropsID		= 4;		// 对方军团ID
};

// 挑战通知
message CMessageCorpsChallegeNotice
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	optional uint32			SrcCorpsID		= 2;
	optional string			SrcCorpsName	= 3;
	optional uint32			DesCropsID		= 4;		// 对方军团ID
	optional string			DesCorpsName	= 5;		// 对方军团名称
	optional uint32			WinTimes		= 6;
	optional uint32			LostTimes		= 7;
	optional string			SrcRooterName	= 8;
	optional string			DesRooterName	= 9;
	optional uint32			Action			= 10;
};

// 回复挑战请求
message CMessageCorpsReplyChallegeRequest
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	optional uint32			DesCorpsID		= 2;
};

// 操作挑战请求的回复
message CMessageCorpsReplyChallegeResponse
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	optional string			DesCorpsName	= 2;
	optional uint32			RtnCode			= 3;
};

// 查询自己挑战别人的列表请求
message CMessageCorpsQueryChallegeListRequest
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
};

// 查询自己挑战别人的列表回复
message CMessageCorpsQueryChallegeListResponse
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	repeated uint32			CorpsIDList 	= 2;		// 挑战的军团列表
	optional uint32			RtnCode			= 3;
};

message PBChallege
{
	optional uint32	CorpsID = 1;
	optional string CorpsName = 2;
	optional uint32	WinTimes = 3;
	optional uint32	LostTimes = 4;
};

// 查询自己房间收到的挑战列表请求
message CMessageCorpsQueryRecChallegeListRequest
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
};

// 查询自己房间收到的挑战列表回复
message CMessageCorpsQueryRecChallegeListResponse
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	optional string			RoomName		= 2;
	optional uint32			Money			= 3;
	optional uint32			WinTimes		= 4;
	optional uint32			LostTimes		= 5;
	repeated PBChallege		ChallegeList	= 6;
	optional uint32			RtnCode			= 7;
};

// 查询挑战房间列表
message CMessageCorpsQueryRoomListRequest
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
};

message PBCorpsRoom
{
	optional uint32		CorpsID = 1; // 房间的军团ID
	optional string		CorpsName = 2;
	optional string		RoomName = 3;
	optional uint32		RoomState = 4;	// 房间状态
	optional string		DesCorpsName = 5;	// 战斗状态下的挑战军团名、非战斗中为空
	optional uint32		WinTimes	= 6;
	optional uint32		LostTimes	= 7;
	optional uint32		Money = 8;
};

// 查询挑战房间回复
message CMessageCorpsQueryRoomListResponse
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	repeated PBCorpsRoom	RoomList		= 2;
	optional uint32			RtnCode			= 3;
};

// ***************************************************************************//
// CMessageQueryCorpsWelfareRequest	
// 查询军团npc福利的消息请求
// ***************************************************************************//
message CMessageQueryCorpsWelfareRequest
{
	 optional CorpsHead		CorpHead		= 1;		// 军团消息头
	 optional uint32		EntityID		= 2;		// NPC实体ID
	 optional uint32		NpcID			= 3;		// npc的模板ID,服务器用
};

// ***************************************************************************//
// 查询军团npc福利的消息回应	
// CMessageQueryCorpsWelfareResponse
// ***************************************************************************//
message CMessageQueryCorpsWelfareResponse
{	
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	optional uint32			Errcode			= 2;		// 错误码	
	optional uint32			NpcGlory		= 3;		// 可以领取的荣耀
	optional uint32			NpcMoney		= 4;		// 可以领取的家族金钱
};

// ***************************************************************************//
// 领取军团福利的消息请求
// CMessageObtainCorpsWelfareRequest
// ***************************************************************************//
message CMessageObtainCorpsWelfareRequest
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	optional uint32			EntityID		= 2;		// NPC实体ID
	optional uint32			NpcID			= 3;		// npc的模板ID		
};

// ***************************************************************************//
// 领取军团福利的消息回应
// CMessageObtainCorpsWelfareRequest
// ***************************************************************************//
message CMessageObtainCorpsWelfareResponse
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头	
	optional uint32			Errcode			= 2;		// 错误码	
	optional uint32			NpcGlory		= 3;		// 可以领取的荣耀
	optional uint32			NpcMoney		= 4;		// 可以领取的金钱
}; 

// ***************************************************************************//
// 把军团金钱领取为个人金钱的请求
// CMessageGetCorpsMoneyRequest
// ***************************************************************************//
message  CMessageGetCorpsMoneyRequest
{	 
	optional CorpsHead		CorpHead		= 1;		// 军团消息头	
	optional uint32			CorpsMoney		= 2;		// 军团金锭数	
};

// ***************************************************************************//
// 把军团金钱领取为个人金钱的消息回应
// CMessageGetCorpsMoneyResponse
// ***************************************************************************//
message CMessageGetCorpsMoneyResponse
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	optional uint32			Errcode			= 2;		// 错误码		
	optional uint32			CorpsMoney		= 3;		// 领取的军团金锭数	
};

// 放弃占领得城池请求
message CMessageCorpsAbortBattleCityRequest
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	optional uint32			NpcEntityID		= 2;		// 对话npc
};

message CMessageCorpsAbortBattleCityResponse
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	optional uint32			RtnCode			= 2;		// 成功：0
	optional uint32			NpcID			= 3;
	optional string			RooterName		= 4;
};

message CMessageCorpsAbortBattleCityNotice
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	optional uint32			NpcID			= 2;
	optional string			RooterName		= 3;
};

// ***************************************************************************//
// 军团可以升级的消息通知
// CMessageCorpsCanLevelUPNotify
// ***************************************************************************//
message CMessageCorpsCanLevelUPNotify
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头	
};

// 军团发布任务通知
message CMessageCorpsPublishTaskNotify
{
	optional CorpsHead		NotifyHead		= 1;		// 军团消息头	
};

// 战争结果通知
message CMessageCorpsBattleResultNotice
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	optional string			WinCorpsName	= 2;
	optional string			LostCorpsName	= 3;
	optional uint32			WinCampID		= 4;		// 1:攻防赢；2:守方赢
	optional uint32			NpcID			= 5;
	optional uint32			BattleType		= 6;
	optional uint32			WinCorpsID		= 7;
	optional uint32			LostCorpsID		= 8;
};


// npc 归属更新通知
message CMessageCorpsNpcOwnerNotice
{
	optional uint32			Type			= 1;
	optional uint32			NpcID			= 2;
	optional string			CorpsName		= 3;
	optional uint32			CorpsID			= 4;
	optional string			FamilyName		= 5;
	optional uint32			FamilyID		= 6;
};

// ***************************************************************************//
// 军团称号改变的通知
// CMessageCorpsNameChangedNotify
// ***************************************************************************//
message CMessageCorpsNameChangedNotify
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	optional uint32			ManagerID		= 2;		// 管理者姓名
	optional string			ManagerName		= 3;		// 管理者名称
	optional uint32			MemberID		= 4;		// 称号被修改的角色ID	
	optional string			CorpsName		= 5;		// 被修改后的称号
};

// ***************************************************************************//
// 创建联军请求
// CMessageCreateUnionRequest
// ***************************************************************************//
message CMessageCreateUnionRequest
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	repeated uint32			OtherID			= 2;		// 其他军团ID
	optional string			UnionName		= 3;		// 联军名称
	optional string			UnionNotice		= 4;		// 联军公告
	optional uint32			NPCEntityID		= 5;		// NPC ENTITYID
};

// ***************************************************************************//
// 创建联军回应
// CMessageCreateUnionResponse
// ***************************************************************************//
message CMessageCreateUnionResponse
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头	
	optional uint32			Result			= 2;		// 错误码
	repeated uint32			OtherID			= 3;		// 其他军团ID
	optional uint32			UnionID			= 4;		// 联盟ID
}; 

// ***************************************************************************//
// 创建联军通知
// CMessageCreateUnionNotify
// ***************************************************************************//
message CMessageCreateUnionNotify
{
	optional CorpsHead		CorpHead		= 1;		// 军团消息头
	optional uint32			HeadID			= 2;		// 首领军团ID
	optional string			HeadName		= 3;		// 首领军团名称
	repeated uint32			OtherID			= 4;		// 其他军团ID
	repeated string			OtherName		= 5;		// 其他军团名称
	optional uint32			UnionID			= 6;		// 联盟ID
	optional string			UnionName		= 7;		// 联盟名称
}; 


// *************************************************************************** //
// CMessageApplyUnionRequest  
// 申请加入联军的消息请求
// *************************************************************************** //
message CMessageApplyUnionRequest
{		
	optional CorpsHead	CorpHead		= 1;			// 军团消息头
	optional string		UnionName		= 2;			//	联军名称
	optional uint32		UnionID			= 3;			//	联军ID
	optional uint32		RoleID			= 4;			//	玩家角色ID
};


// *************************************************************************** //
// CMessageApplyUnionResponse  
// 申请加入联军的消息回应
// *************************************************************************** //
message CMessageApplyUnionResponse
{			
	optional CorpsHead	CorpHead	= 1;			// 军团消息头
	optional uint32		Errcode		= 2;			// 错误码0表示成功其他表示错误码
	optional string		UnionName	= 3;			// 联军名称
};

// *************************************************************************** //
// CMessageUnionMemberApplyNotify  
// 有军团申请加入联军的消息通知
// *************************************************************************** //
message CMessageUnionMemberApplyNotify
{			
	optional CorpsHead	CorpHead	= 1;			// 军团消息头
	optional PBCorpsBasicInfo CorpsInfo	= 2;		// 军团属性信息
};

// *************************************************************************** //
// CMessageReplyUnionMemberApplyRequest  
// 答复军团申请的消息请求
// *************************************************************************** //
message CMessageReplyUnionMemberApplyRequest
{																		
	optional CorpsHead	CorpHead	= 1;				// 军团消息头
	optional uint32		Result		= 2;				// 对于军团申请的回复1表示同意0表示不同意
	optional uint32		CorpsID		= 3;				// 申请加入的军团的ID		
};

// *************************************************************************** //
// CMessageReplyUnionMemberApplyResponse  
// 有军团申请加入联军的消息通知
// *************************************************************************** //
message CMessageReplyUnionMemberApplyResponse
{
	optional CorpsHead	CorpHead	= 1;				// 军团消息头
	optional uint32		Errcode		= 2;				// 0表示操作成功其他表示错误码
	optional uint32		Result		= 3;				// 之前的答复结果
	optional uint32		CorpsID		= 4;				// 军团ID		
};

// *************************************************************************** //
// CMessageApplyUnionResultNotify  
// 申请加入联军的结果通知
// *************************************************************************** //
message CMessageApplyUnionResultNotify
{
	optional CorpsHead	CorpHead	= 1;				// 军团消息头
	optional uint32 ResultCode		= 2;				// 1表示加入成功0表示被拒绝其他表示错误码
	optional uint32 UnionID			= 3;				// 联军ID
	optional string	UnionName		= 4;				// 联军名称
};


// *************************************************************************** //
// CMessageInviteUnionMemberRequest  
// 邀请军团加入联军的消息请求
// *************************************************************************** //
message CMessageInviteUnionMemberRequest
{
	optional CorpsHead	CorpHead	= 1;				// 军团消息头
	optional string CorpsName		= 2;				// 家族名称
	optional uint32	MemberID		= 3;				// 角色ID		
};


// ***************************************************************************//
// CMessageInviteUnionMemberResponse  
// 邀请军团加入联军的消息回应
// ***************************************************************************//
message CMessageInviteUnionMemberResponse
{
	optional CorpsHead	CorpHead	= 1;				// 军团消息头
	optional uint32 Errcode			= 2;				// 错误码0表示成功其他表示错误码
	optional string CorpsName		= 3;				// 军团名称
};

// ***************************************************************************//
// CMessageUnionInviteNotify  
// 邀请加入联军的消息通知
// ***************************************************************************//
message CMessageUnionInviteNotify
{
	optional CorpsHead	CorpHead		= 1;			// 军团消息头
	optional string InvitingName		= 2;			// 邀请者姓名
	optional uint32 InvitingID			= 3;			// 邀请者ID	
	optional PBUnionBasicInfo UnionInfo = 4;			// 军团信息 
};	

// ***************************************************************************//
// CMessageReplyUnionInviteRequest  
// 答复联军邀请的消息请求
// ***************************************************************************//
message CMessageReplyUnionInviteRequest
{
	optional CorpsHead	CorpHead		= 1;			// 军团消息头
	optional uint32	 Result				= 2;			// 1表示同意加入联军0表示拒绝加入联军
	optional uint32	 UnionID			= 3;			// 联军ID
	optional uint32	 InvitingID			= 4;			// 邀请者ID
};

// ***************************************************************************//
// CMessageReplyUnionInviteResponse  
// 答复联军邀请的消息回复
// ***************************************************************************//
message CMessageReplyUnionInviteResponse
{
	optional CorpsHead	CorpHead		= 1;			// 军团消息头
	optional uint32	Errcode				= 2;			// 错误码0表示成功其他表示错误码
	optional uint32 Result				= 3;			// 答复结果
	optional string	UnionName			= 4;			// 联军名称
	optional uint32 UnionID				= 5;			// 联军ID	
};

// ***************************************************************************//
// CMessageUnionInviteResultNotify  
// 邀请家族加入军团的结果
// ***************************************************************************//
message CMessageUnionInviteResultNotify
{
	optional CorpsHead	CorpHead		= 1;			// 军团消息头
	optional uint32 Result				= 2;			// 1表示邀请成功0表示被拒绝其他表示错误码
	optional string CorpsName			= 3;			// 被邀请的军团的名称
};	 


// ***************************************************************************//
// CMessageCorpsJoinUnionNotify  
// 有军团加入联军的消息通知
// ***************************************************************************//
message CMessageCorpsJoinUnionNotify
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional PBCorpsBasicInfo CorpsInfo		= 2;			// 军团属性信息	
	optional uint32			UnionID			= 3;			// 联军ID
	optional string			UnionName		= 4;			// 联军名称
};

// ***************************************************************************//
// CMessageCorpsLeaveUnionRequest  
// 军团离开联军的消息请求
// ***************************************************************************//
message CMessageCorpsLeaveUnionRequest
{
	optional CorpsHead	CorpHead	= 1;			// 军团消息头
	optional uint32		Msgcode		= 2;			// 无用参数
};

// ***************************************************************************//
// CMessageCorpsLeaveUnionResponse  
// 家族离开军团的消息回应
// ***************************************************************************//
message CMessageCorpsLeaveUnionResponse
{
	optional CorpsHead	CorpHead	= 1;			// 军团消息头
	optional uint32		Errcode		= 2;			// 错误码0表示成功其他表示错误码	
};

// ***************************************************************************//
// CMessageCorpsLeaveUnionNotify  
// 家族离开军团的消息通知
// ***************************************************************************//
message CMessageCorpsLeaveUnionNotify
{
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
	optional uint32	CorpsID					= 2;			// 军团ID
	optional string CorpsName				= 3;			// 军团名称
};

// ***************************************************************************//
// CMessageKickCorpsRequest  
// 从联军踢出军团的消息请求
// ***************************************************************************//
message CMessageKickCorpsRequest
{
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
	optional uint32 CorpsID				= 2;				// 被踢掉的军团ID				
};

// ***************************************************************************//
// CMessageKickCorpsResponse  
// 从联军踢出军团的消息回应
// ***************************************************************************//
message CMessageKickCorpsResponse
{
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
	optional uint32 Errcode					= 2;			// 错误码0表示成功其他表示错误码
	optional string CorpsName				= 3;			// 被踢出的军团名称
	optional uint32 CorpsID					= 4;			// 被踢出的军团ID
};

// ***************************************************************************//
// CMessageKickCorpsNotify  
// 军团从联军里面被踢掉的通知
// ***************************************************************************//
message CMessageKickCorpsNotify
{												
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
	optional uint32 CorpsID					= 2;			// 被踢掉的军团ID
	optional string	CorpsName				= 3;			// 被踢掉的军团名称
};

// ***************************************************************************//
// CMessageMakeUnionRequest  
// 创建联军的消息请求
// ***************************************************************************//
message CMessageMakeUnionRequest
{
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
	optional uint32		OpType				= 2;			// 操作类型
	optional string		UnionName			= 3;			// 联军名称	
	optional string		UnionNotice			= 4;			// 联军公告
	optional uint32		CorpsID				= 5;			// 请求加入的军团ID
	optional uint32		TargetID			= 6;			// 目标ID
};

// ***************************************************************************//
// CMessageMakeUnionResponse  
// 创建联军的消息回应
// ***************************************************************************//
message CMessageMakeUnionResponse
{
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
	optional uint32 Errcode					= 2;			// 错误码0表示成功其他表示错误码	
	optional uint32	OpType					= 3;			// 操作类型
};

// ***************************************************************************//
// CMessageMakeUnionNotify  
// 创建联军的通知
// ***************************************************************************//
message CMessageMakeUnionNotify
{												
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
	optional uint32		OpType				= 2;			// 操作类型
	optional string		UnionName			= 3;			// 联军名称	
	optional string		UnionNotice			= 4;			// 联军公告
	repeated uint32		CorpsID				= 5;			// 请求加入的军团ID
	repeated string		CorpsName			= 6;			// 请求加入的军团名称
	optional string		HeadCorpsName		= 7;			// 主军团名称
	optional uint32		HeadCharID			= 8;			// 主军团发起者
};

// ***************************************************************************//
// CMessageGetUnionInfoRequest  
// 获得联军信息的消息请求
// ***************************************************************************//
message CMessageGetUnionInfoRequest
{
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
	optional uint32		UnionID				= 2;			// 联军ID
};

// ***************************************************************************//
// CMessageGetUnionInfoResponse  
// 获得联军信息的消息回应
// ***************************************************************************//
message CMessageGetUnionInfoResponse
{
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
	optional uint32 Errcode					= 2;			// 错误码0表示成功其他表示错误码
	optional PBUnionBasicInfo UnionInfo		= 3;
	repeated PBCorpsBasicInfo CorpsInfo		= 4;			// 军团信息
	optional PBCorpsBattleList	BattleInfo		= 5;			// 军团战争信息
};

// ***************************************************************************//
// CMessageDismissUnionRequest  
// 解散联军的消息请求
// ***************************************************************************//
message CMessageDismissUnionRequest
{
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
	optional uint32		NPCEntityID			= 2;			// NPC EntityID
};

// ***************************************************************************//
// CMessageDismissUnionResponse  
// 解散联军的消息回应
// ***************************************************************************//
message CMessageDismissUnionResponse
{
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
	optional uint32 Errcode					= 2;			// 错误码0表示成功其他表示错误码
};

// ***************************************************************************//
// CMessageDismissUnionNotify 
// 解散联军的消息通知
// ***************************************************************************//
message CMessageDismissUnionNotify
{
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
};

// ***************************************************************************//
// CMessageUnionApplyListRequest  
// 获得联军申请列表的消息请求
// ***************************************************************************//
message CMessageUnionApplyListRequest
{
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
};

// ***************************************************************************//
// CMessageUnionApplyListResponse  
// 获得联军申请列表的消息回应
// ***************************************************************************//
message CMessageUnionApplyListResponse
{
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
	optional uint32 Errcode					= 2;			// 错误码0表示成功其他表示错误码
	repeated PBCorpsBasicInfo CorpsInfo		= 3;			// 军团信息
};

// ***************************************************************************//
// CMessageAllUnionListRequest  
// 获得联军列表的消息请求
// ***************************************************************************//
message CMessageAllUnionListRequest
{
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
};

// ***************************************************************************//
// CMessageAllUnionListResponse  
// 获得联军列表的消息回应
// ***************************************************************************//
message CMessageAllUnionListResponse
{
	optional CorpsHead	CorpHead			= 1;			// 军团消息头
	optional uint32 Errcode					= 2;			// 错误码0表示成功其他表示错误码
	repeated PBUnionBasicInfo UnionInfo		= 3;			// 军团信息
};

// 联盟主获的可参战列表请求 [12/1/2009 Macro]
message CMessageCorpsGetOtherMemberRequest
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
};

message PBBattleMember
{
	optional uint32	CharID  = 1;
	optional string Name	= 2;
	optional uint32	Level	= 3;
	optional uint32	Metier	= 4;	// 职业
	optional string CorpsName = 5;	// 
	optional uint32	IsJoinUnion = 6;
};

message CMessageCorpsGetOtherMemberResponse
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	repeated PBBattleMember MemberList		= 2;
};

// 查询军盟参战人员列表
message CMessageUnionJoinBattleListRequest
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
};

message CMessageUnionJoinBattleListResponse
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			RtnCode			= 2;
	repeated PBBattleMember	MemberList		= 3;
};

// 获取已被占领的地图列表
message CMessageCorpsGetAllMapRequest
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			ServerID		= 2;			// 阵营ID
};

message PBMapOwner
{
	optional uint32	 MapID		= 1;
	optional uint32  CorpsID	= 2;
	optional string  CorpsName	= 3;	// 
	optional uint32	 Color		= 4;	// 军团颜色 
	optional string  RooterName = 5;	// 军团长姓名
	optional uint32	 CorpsPower = 6;	// 军团势力值
	optional string	 UnionName	= 7;	// 军盟名
	optional uint32	 UnionPower	= 8;	// 军盟势力值
	optional uint32	 PeopleNum	= 9;	// 国家人口
};

message CMessageCorpsGetAllMapResponse
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	repeated PBMapOwner		MapList			= 2;
	optional uint32			ServerID		= 3;			// 阵营ID
};

// 任命诸侯国王
message CMessageCorpsAppointPrinceRequest
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			CorpsID			= 2;			// 任命的军团ID
	optional uint32			MapID			= 3;			// 诸侯国地图ID
};

message CMessageCorspAppointPrinceResponse
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			RtnCode			= 2;			// 成功:0,失败：信息提示
	optional uint32			CorpsID			= 3;			// 任命的军团ID
	optional uint32			MapID			= 4;			// 诸侯国地图ID
};

// 通知新国王 [12/22/2009 Macro]
message CMessageCorpsAppointPrinceNotice
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			CorpsID			= 2;			// 任命的军团ID
	optional uint32			MapID			= 3;			// 诸侯国地图ID
};

// 迁都请求 [12/22/2009 Macro]
message CMessageCorpsMoveCapitalRequest
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			MapID			= 2;			// 新都城地图ID
};

message CMesssageCorpsMoveCapitalResponse
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			RtnCode			= 2;			// 成功:0,失败：信息提示
	optional uint32			MapID			= 3;			// 新都城地图ID
};

// 上报军团颜色请求 [12/22/2009 Macro]
message CMessageCorpsSelectColorRequest
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			Color			= 2;			// 颜色
};

message CMessageCorpsSelectColorResponse
{
	optional CorpsHead		CorpHead		= 1;			// 军团消息头
	optional uint32			RtnCode			= 2;			// 成功:0,失败：信息提示
	optional uint32			Color			= 3;			// 颜色
};

// *************************************************************************** //
// CMessageGetCorpsGoodsListRequest
// 获取军团商品列表的消息请求
// *************************************************************************** //
message CorpsGoodItem
{
	optional uint32 ItemID		= 1;
	optional uint32	ItemNum		= 2;
	optional uint32 ComposeID	= 3;	// 复杂合成的ID,兑换用
};

message CMessageGetCorpsGoodsListRequest
{
	optional CorpsHead CorpHead = 1;			// 军团消息头
	optional uint32	CorpsID		= 2;			// 军团Id(不用填值)
	optional uint32	EntityID	= 3;			// npc实体ID
	optional uint32	NpcTmpID	= 4;			// npc模板ID( 服务器用 )  
};

// *************************************************************************** //
// CMessageGetCorpsGoodsListResponse
// 获取军团商品列表的消息回应
// *************************************************************************** //
message CMessageGetCorpsGoodsListResponse
{	
	optional CorpsHead		CorpHead	= 1;		// 军团消息头
	optional uint32			Errcode		= 2;		// 错误码
	repeated CorpsGoodItem	GoodsList	= 3;		// 军团已经买过的商品列表
	optional uint32			NpcTmpID	= 4;		// npc模板ID	
};

// *************************************************************************** //
// CMessageGetCorpsExchangeGoodsListRequest
// 获取军团兑换商品列表的消息请求
// *************************************************************************** //
message CMessageGetCorpsExchangeGoodsListRequest
{
	optional CorpsHead CorpHead = 1;		// 军团消息头
	optional uint32	CorpsID =	2;		// 军团Id(不用填值)
	optional uint32	EntityID =  3;		// npc实体ID
	optional uint32	NpcTmpID = 4;			// npc模板ID( 服务器用 )  
};

// *************************************************************************** //
// CMessageGetCorpsExchangeGoodsListResponse
// 获取军团兑换商品列表的消息回应
// *************************************************************************** //
message CMessageGetCorpsExchangeGoodsListResponse
{
	optional CorpsHead		CorpHead	= 1;		// 军团族消息头
	optional uint32			Errcode		= 2;		// 错误码
	repeated CorpsGoodItem	GoodsList	= 3;		// 军团能够兑换的商品列表(包括数量)
	optional uint32			NpcTmpID	= 4;		// npc模板ID
};


message CMessageChangeUnionNameRequest
{
	optional CorpsHead	CorpHead  = 1;		// 军团消息头
	optional string	UnionName	=	2;		// 联盟名称
	optional string	UnionNotice	=	3;		// 联盟通知
	optional uint32	CorpsID		=	4;		// 客户端发来的军团ID
};

message CMessageChangeUnionNameResponse
{
	optional CorpsHead	CorpHead  = 1;			// 军团消息头
	optional uint32	Result		=	2;
};

message CMessageChangeUnionNameNotify
{
	optional CorpsHead	CorpHead  = 1;		// 军团消息头
	optional string	UnionName	=	2;		// 联盟名称
	optional string	UnionNotice	=	3;		// 联盟通知
	
};

// *************************************************************************** //
// CMessageChangeCorpsGoodsList
// 改变军团商店物品列表的消息通知
// *************************************************************************** //
message CMessageChangeCorpsGoodsListNotify
{
	repeated CorpsGoodItem	GoodsList	=	1;		// 商品列表
	optional uint32			NpcTmpID	=	2;		// npc的模板ID	
};

// *************************************************************************** //
// CMessageChangeCorpsExchangeGoodsListNotify
// 改变军团兑换商店物品列表的消息通知
// *************************************************************************** //
message CMessageChangeCorpsExchangeGoodsListNotify
{
	repeated CorpsGoodItem	GoodsList	=	1;		// 商品列表
	optional uint32			NpcTmpID	=	2;		// npc的模板ID
};

// *************************************************************************** //
// CMessageUseCorpsLeagueShopItemNofiy
// 使用军团特供物品的通知
// *************************************************************************** //
message CMessageUseCorpsLeagueItemNotify
{
	repeated CorpsGoodItem	GoodsList	=	1;		// 商品列表
	optional uint32			ManagerID	=	2;		// 管理者ID
	optional string			ManagerName	=	3;		// 管理者角色名称	
};

// 进入战场请求
message CMessageCorpsLoginBattleRequest
{
	optional CorpsHead	CorpHead  = 1;		// 军团消息头
	optional uint32		NpcID	  = 2;		// npcid
};

message CMessageCorpsLoginBattleResponse
{
	optional CorpsHead	CorpHead  = 1;		// 军团消息头
	optional uint32		RtnCode	  = 2;		// 返回码
};

message CMessageSummonCorpsBossRequest
{
	optional CorpsHead CorpHead = 1;		//军团消息头
	optional uint32	BossID		= 2;		//boss id
};

message CMessageSummonCorpsBossResponse
{
	optional CorpsHead CorpHead		= 1;		//军团消息头
	optional uint32		ErrorCode	= 2;		//消息错误码 0 表示正确 其他表示错误
};

message CMessageCorpsBossRepetionOpenNotify
{
	optional uint32		BossID		= 1;			//军团boss id
	optional uint32		RoleID		= 2;			//开启人的roleid
	optional string		RoleName	= 3;			//开启人的角色名
	optional uint32		BossCDCool	= 4;			//boss的冷却时间
};

message CMessageIntoCorpsBossRepetionResponse
{
	optional CorpsHead		CorpHead		= 1;		//军团消息头			
	optional uint32			ErrorCode		= 2;		//错误码	
};

message CMessageCorpsBossRepetionEndNotify
{

};