#pragma once
#define MakeSession( object, func, param1, param2 )															\
	CSession< CDBReceiver >( object, (CSession<CDBReceiver>::DataProc) &func, param1, param2 )				\

#include "message_pb.hxx.pb.h"
#include "db_msg_pb.hxx.pb.h"
#include "object.h"
#include "servertool.h"

// sql 执行函数可选参数
enum 
{
	SQL_NONE			= 0,	// 无回执，无返回自增长字段值
	SQL_NEED_CALLBACK	= 0x01, // 需要回执
	SQL_LATEST_INC_ID	= 0x02, // 需要返回最近一次的auto increment id , 自增长字段
	SQL_REMOTE_HOST		= 0x04,	// 某些server的特殊需求，比如logserver,表示是去远程mysql执行
};


enum CSessionType
{
	saveplayer = 0,
	loadplayerforlogin,
	mailrecenothing,
	dbmailrece,
	getmailboxinforrece,
	dbmsgrece,
	getmailcountrece,
	addblackcallback,
	commonsqlcallback,
	playeraddfriendcallback,
	loadfriendscallback,
	loadfriendscallback2,
	getmaildetailrece,
	saveplayer_charprofile,
	saveplayer_storage,
	saveplayer_baggage,
	loadplayerforlogin_charprofile,
	loadplayerforlogin_baggage,
	loadplayerforlogin_storage,
	checknewmail,
	saveinvestanswer,
	save_clienthost,
	saveplayer_storagekitbag,		  
	loadplayerforlogin_storagekitbag,	
	ibitemused,
	loadplayerforlogin_baggagekitbag,
	saveplayer_baggagekitbag,	
	em_dbsession_pay_amount,
	loadplayerforlogin_yuanbao,
	freeze_account,
	unfreeze_account,
	updatesilveryb,
	em_dbsession_achieve_save,
	em_dbsession_achieve_query,
	em_dbsession_save_stat,
	em_dbsession_load_stat,
	load_task_global,
	save_task_global,
	checkmailindb,
	em_dbsession_judge_query,
	saveplayer_property_all,
	loadplayer_property_all,
	SESSION_LOAD_PLAYER_BAGGAGE,			// 加载玩家道具信息.参数说明: 0背包,1仓库,2装备
	SESSION_SAVE_PLAYER_BAGGAGE,			// 保存玩家道具信息,参数说明：0背包,1仓库,2装备
	session_creatmarriageinfo,				// 创建婚姻数据库信息
	session_marriage_loadmarriagenum,		// 获取数据库中婚姻记录的数量
	session_marriage_loadmarriageinfo,		// 获取数据库中婚姻记录的数量	
	session_marriage_loadcampinfo,			// 获取数据库中阵营信息
	session_marriage_loadplayermarriageinfo,// 获取玩家的婚姻信息
	session_creatcampinfo,					// 创建阵营表信息	
	session_deletemarriageinfo,				// 创建阵营表信息
	ssion_updatemarriageid,					// 更新玩家的婚姻ID	
	ssion_updatemarriageinfo,				// 更新婚礼信息		 	
	ssion_updatecampinfo,					// 更新阵营信息
	bourse_load_by_account,
	bourse_inc_account,
	bourse_dec_account,
	bourse_add_doc_buy,
	bourse_add_doc_sell,
	bourse_dec_doc_buy,
	bourse_dec_doc_sell,
    session_yaoqianshu_loadplayerinfo,    // 获取用户成长度以及上缴仙露个数
    session_yaoqianshu_loadplayerpick,    // 获取用户以拾取获得宝箱信息
    session_yaoqianshu_updateplayerinfo,  // 更新用户成长度以及上缴仙露的个数
    session_yaoqianshu_insertplayerpick,  // 更新用户拾取宝箱信息
    session_yaoqianshu_loadcampinfo,      // 获取摇钱树阵营信息
    session_yaoqianshu_loadplayerrankinfo,// 获取用户贡献度排名
	
	yb_stall_exchange,					// 元宝摆摊交易

	SESSION_GETSEQUEST,
	mail_changecamp,					// 转阵营的邮件操作
	em_dbsession_pay_func,				// 功能付费请求
	em_dbsession_createswornteam,			// 创建结拜信息
	em_dbsession_getswornteam,			// 获取结拜信息
	em_dbsession_updateswornteam,		// 更新结拜信息
	em_dbsession_updateroleswornid,		// 更新玩家结拜ID
	em_dbsession_deleteswornteam,		// 删除结拜的数据库信息
	em_dbsession_savechatmsg,			// 存储聊天信息
	em_dbsession_loadchatmsg,			// 获取聊天信息
	em_dbsession_deletechatmsg,			// 获取聊天信息
};

// ********************************************************************** //
// CGuidGen
// ********************************************************************** //

class CGuidGen
{
	static int sGuid;
public:
	static int GenID( )
		{ return sGuid ++; }
};

 //********************************************************************** //
 //CDatabaseMessage
 //用于访问数据库消息基类
 //********************************************************************** //

// ********************************************************************** //
// CDBReceiver
// ********************************************************************** //

class CDBReceiver
{
public:
	virtual ~CDBReceiver( ) {	}
};

// ********************************************************************** //
// CSession
// ********************************************************************** //

//template< typename T >
class CSession : public CObj
{
private:

	int			mSessionID;
	int			mParameter1;
	int			mParameter2;
	int			mParam[4];	// 兼容性代码：其他参数
	int			mSessionType;
	CWTimer		mTimer;


public:

	CSession()
	{
		if ( CObj::msCreateMode )
		{
			Initialize( );
		} 
		else
		{
			Resume( );
		}
	}

	~CSession() {}

	int Initialize()
	{
		mSessionID = 0;
		return 0;
	}

	int Resume() {return 0;}


	void assign(int session_id, int session_type, int param1, int param2, int timeout = 10000)
	{
		mSessionID = session_id;
		mSessionType = session_type;
		mParameter1 = param1;
		mParameter2 = param2;
		memset( mParam, 0, sizeof(mParam) );
		mTimer = CWTimer( timeout ); 
	}

	void Fire( CMessage* pMessage, int vParameter1, int vParameter2 )
	{
	}


	// get & set
	inline void SetParam1( int nPara ) { mParameter1 = nPara; }
	inline int GetParam1( ) { return mParameter1; }

	inline void SetParam2( int nPara ) { mParameter2 = nPara; }
	inline int GetParam2( ) { return mParameter2; }
	
	inline void SetParam3( int nPara ) { mParam[0] = nPara; }
	inline int GetParam3( ) { return mParam[0]; }

	inline void SetParam4( int nPara ) { mParam[1] = nPara; }
	inline int GetParam4( ) { return mParam[1]; }

	inline void SetParam5( int nPara ) { mParam[2] = nPara; }
	inline int GetParam5( ) { return mParam[2]; }

	// 都是兼容接口，等后面再统一了。
	// index从1开始
	inline void SetParam( int nIdx, int nParam ) 
	{
		switch( nIdx )
		{
		case 1:
			{
				SetParam1( nParam );
				break;
			}
		case 2:
			{
				SetParam2( nParam );
				break;
			}
		case 3:
			{
				SetParam3( nParam );
				break;
			}
		case 4:
			{
				SetParam4( nParam );
				break;
			}
		case 5:
			{
				SetParam4( nParam );
				break;			
			}
		default:
			{
				break;
			}
		}
	}

	inline int GetParam( int nIdx, int nParam )
	{
		switch( nIdx )
		{
		case 1:
			{
				return GetParam1( );
				break;
			}
		case 2:
			{
				return GetParam2(  );
				break;
			}
		case 3:
			{
				return GetParam3(  );
				break;
			}
		case 4:
			{
				return GetParam4(  );
				break;
			}
		case 5:
			{
				return GetParam4(  );
				break;
			}
		default:
			{
				break;
			}
		}
	}



	inline void SetSessionType( int nType ) { mSessionType = nType; }
	inline int GetSessionType( ) { return mSessionType; }
	
	inline void SetTimer( CWTimer rTimer ) { mTimer = rTimer; }
	inline CWTimer& GetTimer( ) { return mTimer; }
};

// ********************************************************************** //
// CSessionDispatch
// ********************************************************************** //
//
//class CSessionDispatch
//{
//public:
//	lk::hash_map< int, CSession >			mSessions;
//
//public:
//	void Elapse( int vTickOffset )
//	{
//		//map< int, CSession< CDBReceiver > >::iterator tBeginIter = mSessions.begin( );
//		//map< int, CSession< CDBReceiver > >::iterator tEndIter = mSessions.end( );
//		//for ( ; tBeginIter != tEndIter; tBeginIter ++ )
//		//{
//		//	// 如果计时到，通知逻辑数据库超时
//		//	if ( tBeginIter->second.mTimer.Elapse( vTickOffset ) == true )
//		//	{
//		//		CMessageExecuteSqlResponse* tpResponse = (CMessageExecuteSqlResponse*) CMessageExecuteSqlResponse::CreateMessage( 
//		//			tBeginIter->second.mSessionID );
//		//		tBeginIter->second.Fire( tpResponse, tBeginIter->second.mParameter1, tBeginIter->second.mParameter2 );
//		//		mSessions.erase( tBeginIter );
//		//		return;
//		//	}
//		//}
//	}
//
//	void Register( CSession& rSession )
//	{
//		mSessions.insert( std::pair< int, CSession >( rSession.mSessionID, rSession ) );
//	}
//
//	void OnMessage( CMessage* pMessage )
//	{
//		CMessageExecuteSqlResponse3* tpExecuteSql = (CMessageExecuteSqlResponse3*)pMessage->msgpara();
//
//		map< int, CSession >::iterator tFindIter = mSessions.find( tpExecuteSql->sessionid() );
//		if ( tFindIter == mSessions.end( ) )
//			return;
//
//		tFindIter->second.Fire( pMessage, tFindIter->second.mParameter1, tFindIter->second.mParameter2 );
//		mSessions.erase( tFindIter );
//
//	}
//};
