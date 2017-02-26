
// 以最快速度序列/反序列
option optimize_for = SPEED;

// 消息实体
enum enMessageFE
{
	FE_TIMER			= 0;	// 内部消息(定时器)
	FE_CLIENT			= 1;	// 客户端
	FE_SCENCESERVER		= 2;	// 场景服务器
	FE_LOGINSERVER		= 3;	// 登陆服务器
	FE_INFOSERVER		= 4;	// 信息服务器
	FE_GATESERVER		= 5;	// 网关服务器
	FE_DATASERVER		= 6;	// 数据服务器
	FE_ROUTERSERVER		= 7;	// 路由服务器
	FE_PROXYSERVER		= 8;	// 代理服务器
	FE_GLOBALSERVER		= 9;	// 全局服务器
	FE_COUNTRY			= 10;	// 国家
	FE_FAMILY			= 11;	// 家族
	FE_CORPS			= 12;	// 军团
	FE_DUNGEONSERVER	= 13;	// 全局副本服务器	
	FE_OTHERGATE		= 14;	// 其他区组网关
};

// 消息类型
enum enMessageType
{
	REQUEST		= 0;		// 请求
	RESPONSE	= 1;		// 回复
	NOTIFY		= 2;		// 通知
	ACK			= 3;		// 应答
};

// 客户端与服务器之间的消息头
message CCSHead
{
	optional uint32		Uin				 = 		1; 
	optional uint32		DialogID		 = 		2;  // 服务器与客户端的会话ID
};


// 消息头( server -> client 的可以只填messageid, 但client -> server 的需要填全 )
message CMessageHead
{
	optional uint32 DstFE		 = 		1;  // 消息目的实体
	optional uint32 MsgSeq		 = 		2;  // 消息序列号
	optional uint32 SrcFE		 = 		3;  // 消息源实体
	optional uint32 MessageID	 = 		4;  // 消息ID
	optional uint32 SrcID		 = 		5;  // 消息源ID
	optional uint32 DstID		 = 		6;  // 消息目的ID	
};

// 消息
message CMessage
{
	required CMessageHead	MsgHead  = 		1; 				// 消息头
	optional fixed32		MsgPara  = 		2; 	// 消息体( 指针 4bytes )
};


/*
说明：为了保证服务器各模块第一时间快速响应消息，特别为消息进行分段。##

消息ID是unsigned short类型，占2 bytes. 16 bits.
第1bit标示客户端与服务器消息,还是服务器间消息。( 0 : client - server  1 : server - server )
第2~6bits表示模块(可以标示32个模块),
第7bit标示是否是路由消息( 1 yes/ 0 no )
后9bits标示ID(可以表示512个ID)
*/

/* 消息分段
** 如果是服务器间消息在此基础上	+ 0x8000
** 如果是路由消息在此基础上		+ 0x0200
*/
enum EmModuleFlagForMessage
{
	MESSAGE_GLOBAL		= 0x0000;	// 场景服务器与网关服务器间消息，不参与分段, 纯控制类消息

	MESSAGE_CORE		= 0x0400;	//a. core message		( 0x0400  )	( 0 00001 0 000000000 )
	MESSAGE_PROPERTY	= 0x0800;	//b. pro message		( 0x0800  )	( 0 00010 0 000000000 )
	MESSAGE_MAP			= 0x0C00;	//c. map message		( 0x0C00  )	( 0 00011 0 000000000 )
	MESSAGE_TASK		= 0x1000;	//d. task message		( 0x1000  )	( 0 00100 0 000000000 )  
	MESSAGE_CHAT		= 0x1400;	//e. chat message		( 0x1400  )	( 0 00101 0 000000000 )
	MESSAGE_FRIEND		= 0x1800;	//f. friend message		( 0x1800  )	( 0 00110 0 000000000 )
	MESSAGE_TEAM		= 0x1C00;	//g. team message		( 0x1C00  )	( 0 00111 0 000000000 )
	MESSAGE_MAIL		= 0x2000;	//h. mail message		( 0x2000  )	( 0 01000 0 000000000 )
	MESSAGE_TRADE		= 0x2400;	//i. trade message		( 0x2400  )	( 0 01001 0 000000000 )
	MESSAGE_STALL		= 0x2800;	//j. stall message 		( 0x2800  )	( 0 01010 0 000000000 )
	MESSAGE_REPETION	= 0x2C00;	//k. repetion message	( 0x2C00  ) ( 0 01011 0 000000000 )
	MESSAGE_ACTIVITY	= 0x3000;	//l. activity message   ( 0x3000  ) ( 0 01100 0 000000000 )
	MESSAGE_FAMILY		= 0x3400;	//m. family message   	( 0x3400  ) ( 0 01101 0 000000000 )
	MESSAGE_CORPS		= 0x3800;	//n. corps  message   	( 0x3800  ) ( 0 01110 0 000000000 )
	MESSAGE_RANK		= 0x3C00;	//o. rank message     	( 0x3C00  )	( 0 01111 0 000000000 )
	MESSAGE_GM			= 0x4000;	//p. gm message			( 0x4000  )	( 0 10000 0 000000000 )
	MESSAGE_LOG			= 0x4400;	//q. log message		( 0x4400  )	( 0 10001 0 000000000 )
	MESSAGE_IB			= 0x4800;	//r. ib message
	MESSAGE_JUDGE		= 0x4C00;	//s. judge message	
	MESSAGE_ACHIEVE		= 0x5000;	//t. achieve message
	MESSAGE_COUNTRY		= 0x5400;	//u. country message
	MESSAGE_BOURSE		= 0x5800;	//v. bourse message
	MESSAGE_MARRIAGE	= 0x5C00;	//w. marriage message  
	MESSAGE_CAMP		= 0x6000;	//x. camp massage
	MESSAGE_GLOBALDUNGEON=0x6400;	//y. globaldungeon message	
	// 留给DB
	MESSAGE_DB			= 0x7C00;	//  db messages
};

