//本文件定义所有发送到客户端的，需要弹出滚动消息的消息代码

#ifndef		_FAMILY_CORPS_DEFINE_
#define		_FAMILY_CORPS_DEFINE_


	#define		FAMILY_APPLY_PASSED				 100	//申请通过，可以输入名字了
	#define		FAMILY_CREATE_FAIL_MONEYLESS	 101	//创建失败-金钱不足
	#define		FAMILY_CREATE_FAIL_ADDED		 102	//创建失败-已经加入了家族
	#define		FAMILY_CREATE_FAIL_LEVELLESS 	 103	//创建失败-等级不足
	#define		FAMILY_CREATE_FAIL_EXIST_NAME	 104	//家族名称重名
	#define		FAMILY_CREATE_NAME_TOO_LONG		 105	//家族名称过长
	#define		FAMILY_CREATE_NAME_TOO_SHORT	 106	//家族名称过短
	#define		FAMILY_CREATE_NAME_ILL			 107	//非法名字
	#define		FAMILY_CREATE_SUCCESS			 108	//创建成功

						// 族长加人相关代码

	#define		REQUEST_ERROR_ADDEDFAMILY				 201	//邀请失败-已经加入了家族
	#define		REQUEST_ERROR_FAMILY_MAXPEOPLE			 202	//邀请失败-人数已满
	#define		REQUEST_ERROR_NOT_B						 203	//邀请失败-没有权限
	#define		REQUEST_ERROR_OTHER						 204	//其他原因失败（少见）
	#define		REQUEST_ERROR_READD						 205	//重复邀请
	#define		REQUEST_ERROR_PLAYER_LEVEL_LESS			 206	//目标玩家等级不足
	#define		REQUEST_NO_SUCH_PLAYER					 207	//没有这个玩家
	#define		REQUEST_PLAYER_OFFLINE					 208	//对方不在线
	#define		REQUEST_SUCCESS							 209	//发送邀请无错误

									// 宗正任职后返回信息

	#define		CHANGE_POS_FLAG_OK						 300	//任职成功
	#define		CHANGE_POS_FLAG_EXSITED_B				 301	//任职失败-已经存在族长
	#define		CHANGE_POS_FLAG_EXSITED_C				 302	//任职失败-已经存在长老
	#define		CHANGE_POS_FLAG_EXSITED_D				 303	//已经存在家族商人，请先免职
	#define		CHANGE_POS_FLAG_NO						 304	//查无此人
	#define		CHANGE_POS_FLAG_NOTA					 305	//不是宗正
	#define		CHANGE_POS_FLAG_SELF					 306    //不能给自己任职！
	#define		CHANGE_POS_FLAG_BAD_POS					 307	//错误的职位

									//免职返回信息

	#define		TAKE_OFF_POS_OK							 400 //免职成功
	#define		LEAVE_POS_OK							 401 //离职成功
	#define		TAKE_OFF_POST_ERROR					     402 //职位错误
	#define		TAKE_OFF_IS_A							 403 //宗正不得离职
	#define		TAKE_OFF_IS_E							 404 //您是族人！(族人还离什么职啊)
	#define		TAKE_OFF_NOT_A							 405 //没有权限	

//解散家族相关信息
	#define		KILL_FAIL								 501	//您无法解散家族，只有宗正才能解散
									// 一般代码

	#define		COMMON_NOT_FAMILY_MEMBER				 10001	//不是家族成员
	#define		COMMON_FAMILYLIST_FIRST_PAGE			 10002	//已经到达第一页
	#define		COMMON_FAMILYLIST_LAST_PAGE				 10003	//已经到达最后一页
	#define		COMMON_ERROR_FAMILY						 10004	//不存在的家族
	#define		COMMON_LEVEL_LESS						 10005	//等级太小，不能加入家族
	#define		COMMON_EXSITED_FAMILY					 10006	//已经有家族，不能申请
	#define		COMMON_APPLYED							 10007	//今天已经申请过了
	#define		COMMON_APPLY_OK							 10008	//申请成功
	#define		COMMON_SUCCESS							 10009	//操作成功。适用于各种成功
	#define		COMMON_FAIL								 10010	//操作失败。适用于各种失败
	//#define		COMMON_BE_CORPS_KICKED					 10011	//被军团踢出
	#define		COOL_CREATE_FAIL						 10011		//冷却时间未到，不能创建家族(冷却时间：24小时，下同)
	#define		COOL_SET_POST_FAIL						 10012		//冷却时间未到，不能任职
	#define		COOL_ADD_FAMILY_FAIL					 10013		//冷却时间未到，不能加入家族	(Msg:要加入家族的玩家必须距离离开家族24小时)
	#define		FAMILY_KILLED							 10014	//您的家族已被解散！


//军团相关定义


enum{
				//共用消息
				COMMON_NOT_IN_CORPS, //没加入军团
				COMMON_OK,				//成功
				COMMON_FAIL,			//失败
				COMMON_CORPS_LIST_FIRSTPAGE, //军团列表到达第一页
				COMMON_CORPS_LIST_LASTPAGE, //军团列表到达最后一页
				COMMON_RIGHT_LESS ,//没有权限
				COMMON_NOT_FAMILY_A , //您不是家族宗正
				//以下是申请军团的返回消息
				CORPS_APPLY_NOT_IN_FAMILY,	//未加入家族
				CORPS_ADDED_FAMILY,			//已经加入军团
				CORPS_APPLY_NOT_FAMILY_A ,	//不是家族宗正
				CORPS_APPLY_MONEY_LESS	,	//钱不够
				CORPS_APPLY_OTHER_ERROR	,	//不明错误（很少发生）
				CORPS_APPLY_NAME_TOO_LONG,	//名字太长
				CORPS_CREATE_BAD_NAME	,	//非法名称！
				CORPS_CREATE_OK			,//创建成功
				//大将军点击邀请未加入军团的家族的任意成员的反馈信息,用于判断右键菜单中是否显示相关的项目
				NOT_CORPS_BIG_GEN	, //不是大将军！没有权限！
				PLAYER_NOT_IN_FAMLY	, //目标玩家没有加入家族
				PLAYER_FAMILY_IN_CORPS	, //目标玩家的家族已经加入了军团
				FAMILY_REQUEST_OK	, //符合条件	
				//军团大将军在邀请家族加入后的服务器返回状态.（大将军输入家族名字返回的消息也是这个！！）
				REQUEST_OK, //邀请成功
				REQUEST_OFFLINE	, //对方宗正没在线
				REQUEST_BAD_FAMILY_NAME	, //没有这个家族！！
				REQUEST_RE_REQUEST	, //重复邀请
				REQUEST_ADDED_CORPS	, //这个家族已经加入了军团！
				REQUEST_ME_ADDED_CORPS,	//您已经加入了其他军团！（发给被邀请方)
				REQUEST_REDIRECT ,  //玩家正在跨场景，请稍后再发 (这种情况很少见)
				//申请加入军团消息
				JOIN_CORPS_APPLY_TWICE, //请勿重复申请
				JOIN_CORPS_ADDED_CORPS, //已经加入军团了！！！
				JOIN_CORPS_APPLY_SUCCESS, //申请加入军团成功
				MEMBER_TITLE_SET_OK,		//修改玩家称号成功
				MEMBER_TITLE_TOO_LONG,		//玩家称号太长！
				//和军团金钱有关的消息
				MONEY_GIVE_SUCCESS,	//给军团捐钱成功
				MONEY_GET_SUCCESS,	//大将军取钱成功
				MONEY_ACCOUT_ERROR,		//钱数不正确
				MONEY_NOT_BIG_GEN,		//不是大将军
				MONEY_NOT_ENOUGH,		//钱不够
				MONEY_CARVE_SUCCESS,		//前锋将军分钱成功
				SET_POST_DEST_NOT_A,	//被授权玩家不是家族宗正
				SET_POST_TOO_MANY_C,	//前锋将军太多
				SET_POST_BAD_POSTNAME,	//错误的职位名
				SET_POST_SELF,			//不能给自己任职！
				SET_POST_SUCCESS,		//大将军任职(授权)成功
				TAKEOFF_POST_SUCCESS,		//免职成功
				KICK_OUT_CANT_SELF_FAMILY,	//不能踢出自己所在的家族
				KICK_OUT_SUCCESS,			//踢出家族成功
				KILL_CORPS_SUCCESS,			//解散军团成功（48小时后解散）
				KILL_CORPS_CANCEL_SUCCESS,	//取消解散军团成功
				KILL_CORPS_FAIL,			//解散军团失败！（人数超过）
				CORPS_DIED		,			//军团被解散了！
				COOL_CORPS_SET_POST_FAIL			,		//冷却时间未到，不能给军团成员宗正任职
				COOL_ADD_CORPS_FAIL,							//冷却时间未到，不能加入军团
				CANT_LEAVE_POST,				//不能离职！只有军师和前锋将军才能离职
				LEAVE_POST_OK					//离职成功
};


#endif

