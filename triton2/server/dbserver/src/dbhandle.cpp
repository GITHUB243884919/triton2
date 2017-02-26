#include "dbhandle.h"
#include "dbctrl.h"
#include "QueryResultMysql.h"
#include "DatabaseMysql.h"
#include "servermessage_in.h"
#include "db_msg_pb.hxx.pb.h"
#include "clock.h"
#include "performance.h"
#include "lk_proto.h"
#include "errcode.h"
#include "coretype.h"
#include "servermessage_pb.hxx.pb.h"
#include "protype.h"
#include "logservertype.h"

CSharedMem* CDBHandle::ms_pCurrentShm = NULL;
pthread_mutex_t CDBHandle::stDBConnMutex;

int CDBHandle::m_iDBSvrID = -1;
short CDBHandle::m_sDBOperMode = -1;


void* CDBHandle::operator new( size_t nSize )
{
    BYTE* pTemp;

    if(!ms_pCurrentShm)
    {
        return (void *)NULL;
    }

    pTemp = (BYTE *)ms_pCurrentShm->CreateSegment( nSize );

    return (void *)pTemp;
}

void CDBHandle::operator delete( void *pMem )
{
}

int CDBHandle::CountSize()
{
    int iTemp = 0;
    int iTempSingle = 0;

    iTemp += sizeof(CDBHandle);
    LOG_INFO( "dbctrl", "CDBHandle %d", iTemp-iTempSingle );
    iTempSingle = iTemp;

    iTemp += CCodeQueue::CountQueueSize(INPUTQUEUELENGTH);
    LOG_INFO( "dbctrl", "INPUTQUEUELENGTH %d", iTemp-iTempSingle );
    iTempSingle = iTemp;

    return iTemp;
}

CDBHandle::CDBHandle()
{
    if(!ms_pCurrentShm)
    {
        return ;
    }

    // 初始化输入队列对象
    CCodeQueue::pCurrentShm = ms_pCurrentShm;
    m_pInputQueue = new CCodeQueue(INPUTQUEUELENGTH);
	if( m_pInputQueue == NULL )
	{
		printf( "in CDBHandle::CDBHandle(), new CCodeQueue failed" );
		exit(1);
	}

	m_iHandleID = -1;
    
	//m_pProxySvrdConns = NULL;
    //m_iLDBInited = False;

    m_tLastCheckTime = time(NULL);

	m_pDatabase = new DatabaseMysql;
	if( m_pDatabase == NULL )
	{
		printf( "in CDBHandle::CDBHandle(), new DatabaseMysql failed" );
		exit(1);
	}

#ifdef _ASYNC_SQL_

    m_iPipeFD = -1;
#endif

    if(ms_pCurrentShm->GetInitMode() == SHM_INIT)
    {
        TRACE_DEBUG("Construct db handle in init mode.\n");
        //OtherInit();
        ////m_stItemDirtyNodeSet.ClearSet();
        //m_stSessionDirtyNodeSet.ClearSet();
    }
    else
    {
        TRACE_DEBUG("Construct db handle in resume mode.\n");
        //OtherResume();
    }
	m_nSavePerfLogFlag = 0;
	mField2Table.initailize();

	mField2Table.insert(CField2Table::value_type("role_id",0));
	mField2Table.insert(CField2Table::value_type("role_hp",0));
	mField2Table.insert(CField2Table::value_type("role_mp",0));
	mField2Table.insert(CField2Table::value_type("role_ap",0));
	mField2Table.insert(CField2Table::value_type("role_basic",0));
	mField2Table.insert(CField2Table::value_type("role_skill",0));
	mField2Table.insert(CField2Table::value_type("role_buff",0));
	mField2Table.insert(CField2Table::value_type("role_cdgroup",0));
	mField2Table.insert(CField2Table::value_type("role_package",0));
	mField2Table.insert(CField2Table::value_type("role_equipment",0));
	mField2Table.insert(CField2Table::value_type("role_storage",0));
	mField2Table.insert(CField2Table::value_type("role_title",0));
	mField2Table.insert(CField2Table::value_type("role_activity",0));
	mField2Table.insert(CField2Table::value_type("role_task_package",0));
	mField2Table.insert(CField2Table::value_type("role_repetion",0));
	mField2Table.insert(CField2Table::value_type("role_pvp",0));
}

CDBHandle::~CDBHandle()
{
}

int CDBHandle::OtherInit()
{
      return 0;
}

int CDBHandle::OtherResume()
{
      return 0;
}

int CDBHandle::IsToBeBlocked()
{
    if(!m_pInputQueue)
    {
        return True;
    }

    return m_pInputQueue->IsQueueEmpty();

}

#ifdef _ASYNC_SQL_
int CDBHandle::Initialize(int iHandleID, CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH>* pProxySvrdConns, int iPipeFD /* = -1 */ )
#else
int CDBHandle::Initialize(int iHandleID, CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH>* pProxySvrdConns )
#endif
{
    TFName szThreadLogFile;

    if(!pProxySvrdConns)
    {
        return -1;
    }

    m_iHandleID = iHandleID;
    m_pProxySvrdConns = pProxySvrdConns;

	// 设置服务器日志
    //snprintf( szThreadLogFile, sizeof(szThreadLogFile)-1,"../log/dbhandle%d_", m_iHandleID);

    //ThreadLogInit(szThreadLogFile, 0x1000000, 5, 0);

	strncpy( m_szHostInfoString, CDBCtrl::GetSingletonPtr()->GetDBSvrConf().m_szMysql, sizeof(m_szHostInfoString)-1 );

	//m_pDatabase = new DatabaseMysql;
	//LK_ASSERT( m_pDatabase != NULL, return -1 );

	// 初始化日志
	InitLog( NULL, NULL, LEVEL_DEBUG );

	m_pDatabase->Initialize ( m_szHostInfoString );

	//m_pDatabase->InitLog( szThreadLogFile, szThreadLogFile, LEVEL_DEBUG, 0x1000000, 5 );

#ifdef _ASYNC_SQL_

    m_iPipeFD = iPipeFD;
    m_sstmPipe.SetUpStream(m_iPipeFD);
#endif

    return 0;
}

int CDBHandle::GetOneCode(int& nCodeLength, BYTE* pCode)
{
    int iTempRet = 0;

    if(!pCode)
    {
        return -1;
    }

    pthread_mutex_lock( &m_stMutex );
    iTempRet = m_pInputQueue->GetHeadCode( pCode, &nCodeLength );
    pthread_mutex_unlock( &m_stMutex );

    return iTempRet;
}

int CDBHandle::PostOneCode(int nCodeLength, BYTE* pCode)
{
    int iTempRet = 0;

    if(!pCode || nCodeLength <= 0)
    {
        return -1;
    }

    pthread_mutex_lock( &m_stMutex );
    iTempRet = m_pInputQueue->AppendOneCode((const BYTE *)pCode, nCodeLength);
    pthread_mutex_unlock( &m_stMutex );

    return iTempRet;
}


// ***************************************************************
//  Function: 	ConvertCodeToMsg   
//  Description:把网络包转换成CProxyHead和CMessage.
//				0 成功;1 失败
//  Date: 		10/15/2008
// 
// ***************************************************************
int CDBHandle::ConvertCodeToMsg(int nCodeLength, BYTE* pCode, CProxyHead* pProxyHead, CMessage* pMsg)
{
	// 跳过前4 bytes的proxy idx 长度
	if( pbmsg_getheadmsg( (unsigned char *)(pCode+4), nCodeLength-4, pProxyHead, pMsg) == 0 )
	{
		return -1;
	}
	else
	{
		return 0;
	}
    
}

int CDBHandle::ConvertMsgToCode(int& nCodeLength, BYTE* pCode, CProxyHead* pProxyHead, CMessage* pMsg)
{
    BYTE abyBody[MAX_CODE_LEN];
    int nBodyLength;
    int nMaxCodeLength = nCodeLength;

    if(nMaxCodeLength <= 0 || !pCode || !pProxyHead || !pMsg)
    {
        TRACE_ERROR("In ConvertMsgToCode, invalid input para.\n");
        return -1;
    }

 
    return 0;
}

int CDBHandle::SendMessageTo(CMessage *pMsg, short nDstEnType, short nDstEnID, unsigned int unUin)
{
	if( pMsg == NULL || m_pProxySvrdConns == NULL )
	{
		TRACE_ERROR("in CDBHandle::SendMessageTo, pmsg is null");
		return -1;
	}

	CProxyHead stProxyHead;
	BYTE abyCodeBuf[MAX_CODE_LEN];
	int nCodeLength = sizeof(abyCodeBuf);
	int iTempRet = 0;

	stProxyHead.set_totallength(0);
	stProxyHead.set_srcfe(m_stCurrentProxyHead.dstfe());
	stProxyHead.set_srcid(m_stCurrentProxyHead.dstid());
	stProxyHead.set_dstfe(m_stCurrentProxyHead.srcfe());
	stProxyHead.set_transfertype(TRANS_P2P);
	stProxyHead.set_dstid(m_stCurrentProxyHead.srcid());
	stProxyHead.set_uin(m_stCurrentProxyHead.uin());
	stProxyHead.set_desgameid(m_stCurrentProxyHead.srcgameid());
	stProxyHead.set_srcgameid(m_stCurrentProxyHead.desgameid());

	nCodeLength = pbmsg_setmsg( stProxyHead, *pMsg, (char*)abyCodeBuf, nCodeLength );
	if( nCodeLength <= 0 )
	{
		TRACE_ERROR( "default", "Encode Message Faild" );
		return -1;
	}
	
	// TODO: 死代码，因为西游的结构只有一台proxy
	int nSendReturn = m_pProxySvrdConns[0].GetSocket()->SendOneCode( nCodeLength, abyCodeBuf );
	if( nSendReturn < 0 )
	{
		TRACE_ERROR( "default", "Send Code(len:%d) To Proxy faild(error=%d)", nCodeLength, nSendReturn );
		return -1;
	}

	return 0;
}

int CDBHandle::SendResponse(CMessage *pMsg)
{
    if( pMsg == NULL )
    {
        TRACE_ERROR("in CDBHandle::SendResponse, pmsg is null");
        return -1;
    }

    return SendMessageTo(pMsg, m_stCurrentProxyHead.srcfe(), m_stCurrentProxyHead.srcid(), m_stCurrentProxyHead.uin() );
}



int CDBHandle::ConnectToLocalDB()
{
	bool bInitRet = m_pDatabase->Initialize( m_szHostInfoString );
	

	return bInitRet ? 0 : -1;
}

int CDBHandle::InitLogFile( const char* vLogName, const char* vLogDir, LogLevel vPriority  /*= LEVEL_NOTSET*/ , unsigned int vMaxFileSize  /*= 10*1024*1024*/ , unsigned int vMaxBackupIndex  /*= 1*/ , bool vAppend  /*= true*/  )
{
	TFName szThreadLogFile;

	// 设置服务器日志
	snprintf( szThreadLogFile, sizeof(szThreadLogFile)-1,"../log/dbhandle%d_", m_iHandleID);

	ThreadLogInit( szThreadLogFile, vMaxFileSize, vMaxBackupIndex, 0, vPriority);

	LK_ASSERT( m_pDatabase != NULL, return -1 );

	m_pDatabase->InitLog( szThreadLogFile, szThreadLogFile, vPriority, vMaxFileSize,vMaxBackupIndex );
}


#ifdef _ASYNC_SQL_
int CDBHandle::ExecSqlAsync(const char *szSql)
{
	// buff 最大长度30k
    BYTE abyCode[MAX_CODE_LEN];

    if(!szSql || m_iPipeFD <= 0)
    {
        return -1;
    }

    int nTmpLen = strlen(szSql) + 1;

	LK_ASSERT(nTmpLen < sizeof(abyCode) - sizeof(unsigned short), return -1);
    memcpy((void *)&abyCode[sizeof(unsigned short)], (const void *)szSql, nTmpLen);

    nTmpLen += sizeof(unsigned short);


	unsigned short usTempLength = (unsigned short)nTmpLen;
    *(unsigned short *)abyCode = htons(usTempLength);
    int iTempRet = m_sstmPipe.SendOneCode(nTmpLen, abyCode);
    if(iTempRet)
    {
        //g_stServerStatisticForPrint.m_iExecSqlAsyncFailedNum++;
        TRACE_ERROR("Send SQL string to Execer failed %d.\n", iTempRet );
        return -1;
    }

    //TRACE("Successfully send sql \"%s\" to execer.\n", szSql);

    return 0;
}
#endif



int CDBHandle::ProcessThreadEnd()
{
    TRACE_DEBUG("---Handle End Process Begin---\n");
	/* ProcessCheckPonits();*/
    TRACE_DEBUG("---Handle End Process End---\n");
    return 0;
}




int CDBHandle::Event(CMessage *pMsg)
{
	if( pMsg == NULL )
	{
		TRACE_ERROR("In CDBHandle::Event, input msg null" );
		return -1;
	}

	switch ( pMsg->msghead().messageid() )
	{
	case ID_G2D_GETBOURSE_INFO:
		{
			PERF_DB( "GetBrouseInfo", OnMessageGetBourseInfo( pMsg ) );
		}
		break;
	case ID_S2D_BOURSE_GET_ACCOUNT:
		{
			PERF_DB( "GetBourseAccount", OnMessageGetBourseAccount( pMsg ) );
		}
		break;
	case ID_S2D_BOURSE_INC_ACCOUNT:
		{
			PERF_DB( "BourseIncAccount", OnMessageBourseIncAccount( pMsg ) );
		}
		break;
	case ID_S2D_BOURSE_DEC_ACCOUNT:
		{
			PERF_DB( "BourseDecAccount", OnMessageBourseDecAccount( pMsg ) );
		}
		break;
	case ID_S2S_EXECUTESQL_REQUEST:
		{
			ProcessExecuteSqlRequest( pMsg );
			break;
		}
	case ID_S2D_LOADPLAYERITEMINFO_REQUEST:
		{
			PERF_DB( "LoadPlayerItem", ProcessLoadPlayerItemInfoRequest( pMsg ) );
			break;
		}
	case ID_S2D_SAVEPLAYERITEMINFO_REQUEST:
		{
			PERF_DB( "SavePlayerItem", ProcessSavePlayerItemInfoRequest( pMsg ) );
			break;
		}
	case ID_S2D_SAVEPLAYER_REQUEST:
		{
			PERF_DB( "SavePlayerInfo", ProcessSavePlayerRequest( pMsg ) );
			break;
		}
	case ID_S2D_LOADPLAYERINFO_REQUEST:
		{
			PERF_DB( "LoadPlayerInfo", ProcessLoadPlayerInfo( pMsg ) );
			break;	
		}
		break;
	case ID_S2D_LOADPLAYERPROPERTY_REQUEST:
		{
			PERF_DB( "LoadPlayerProperty", ProcessLoadPlayerProperty( pMsg ) );	
		}
		break;
	case ID_S2D_BOURSE_ADD_DOC:
		{
			PERF_DB( "AddDoc", ProcessBourseAddDoc( pMsg ) );
		}
		break;
	case ID_S2D_BOURSE_PLAYER_LIST_GET:
		{
			PERF_DB( "BoursePlayerList", ProcessBoursePlayerListGet( pMsg ) );
		}
		break;
	case ID_G2D_BOURSE_TRADE:
		{
			PERF_DB( "BourseTrade", ProcessBourseTrade( pMsg ) );
		}
		break;
	case ID_S2D_BOURSE_DEC_BUY_DOC:
		{
			PERF_DB( "BourseDecBuyDoc",	ProcessBourseDecBuyDoc( pMsg ) );
		}
		break;
	case ID_S2D_BOURSE_DEC_SELL_DOC:
		{
			PERF_DB( "BourseDecSellDoc",	ProcessBourseDecSellDoc( pMsg ) );
		}
		break;
	case ID_S2D_BOURSE_LOG_QUERY:
		{
			PERF_DB( "BourseLogQuery",		ProcessBourseLogQuery( pMsg ) );
		}
		break;
	case ID_S2D_UPDATEPLAYERINFO_REQUEST:
		{
			PERF_DB( "UpdatePlayerInfoRequest", UpdatePlayerInfoRequest( pMsg ) );
		}
		break;
	default:
		{
			break;
		}
	}

	return 0;
}

int CDBHandle::PrepareToRun()
{
    TRACE_DEBUG("Handle %d prepare to run.\n", m_iHandleID);

    if(ConnectToLocalDB())
    {
        TRACE_ERROR("In CDBHandle::PrepareToRun, Connect to database failed.\n");
        printf("In CDBHandle::PrepareToRun, Connect to database failed.\n");
		//TODO:连不上数据库就退出吧，本来也没有重连机制
		//return -1;
		exit(0);
    }

    return 0;
}

int CDBHandle::Run()
{
  
    BYTE abyCodeBuf[MAX_SERVER_TRANS_LEN];
	int nTempCodeLength = sizeof(abyCodeBuf);
    CMessage stTempMsg;

    while(True)
    {
        CondBlock();

		int iTempRet = GetOneCode(nTempCodeLength, (BYTE *)abyCodeBuf);
		if(iTempRet < 0)
		{
			TRACE_ERROR("Get one code from input queue returns %d.\n", iTempRet);
			continue;
		}

		if(nTempCodeLength <= 0)
        {
            TRACE_ERROR("Got one empty code from input queue, ignore it.\n");
            continue;
        }

        if(nTempCodeLength <= sizeof(int))
        {
            TRACE_ERROR("Got one empty code from input queue, ignore it.\n");
            continue;
        }
		
        if( ConvertCodeToMsg( nTempCodeLength, abyCodeBuf, &m_stCurrentProxyHead, &stTempMsg) != 0 )
        {  
            TRACE_ERROR("Convert code to message failed.\n");
            continue;
        }

		//TRACE("Handle message from (%d, %d), Current Proxy ID = %d.\n", m_stCurrentProxyHead.m_nSrcFE, m_stCurrentProxyHead.m_nSrcID, m_iCurrentReqProxyID);
        iTempRet = Event( &stTempMsg );

        if(iTempRet)
        {
			TRACE_ERROR("Handle event returns %d.\n", iTempRet);
        }

		pbmsg_delmsg(&stTempMsg);

		// 检查写统计日志
		if ( m_nSavePerfLogFlag == 1 )	// 该写日志了
		{
			char perf_key[60] = {0};
			strncpy( perf_key, m_stLogCfg.szThreadKey, 40 );
			strcat( perf_key , "_perf" ) ;
			CPerfStat::LogPerfInfo( msPerfMap, perf_key );
			m_nSavePerfLogFlag = 0;
		}
    }
}


int CDBHandle::ProcessExecuteSqlRequest( CMessage* pMsg )
{
	TRACE_DEBUG( " ---- %s -----\n" , __FUNCTION__ );

	if( pMsg == NULL )
	{
		TRACE_ERROR( "Error: [%s][%d][%s], invalid input.\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return -1;
	}
	
	CMessageExecuteSqlRequest* pReqMsg = (CMessageExecuteSqlRequest*)( pMsg->msgpara() );

	if( pReqMsg == NULL )
	{
		TRACE_ERROR( "Error: [%s][%d][%s], msgpara null.\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return -1;
	}

	if( pReqMsg->sql().length() <= 0 )
	{
		TRACE_ERROR( "Error: [%s][%d][%s], sql len invalid.\n", __LK_FILE__, __LINE__, __FUNCTION__ );
		return -1 ;
	}

	bool bRet = false;
	
#ifdef _DEBUG_
	TRACE_DEBUG( "%s \n", pReqMsg->ShortDebugString().c_str() );
#endif

	// 需要回执
	if( pReqMsg->callback() == 1 )
	{
		CMessage tMsg;
		tMsg.mutable_msghead()->set_messageid( ID_S2S_EXECUTESQL_RESPONSE );
		//tMsg.mutable_msghead()->set_msgtype( RESPONSE );
		tMsg.mutable_msghead()->set_srcfe( FE_DATASERVER );
		tMsg.mutable_msghead()->set_srcid( pMsg->msghead().dstid() );
		tMsg.mutable_msghead()->set_dstfe( pMsg->msghead().srcfe() );
		tMsg.mutable_msghead()->set_dstid( pMsg->msghead().srcid() );
		
		CMessageExecuteSqlResponse3 tSqlResMsg;
		tSqlResMsg.set_sessionid( pReqMsg->sessionid() );

		tMsg.set_msgpara( (uint32)&tSqlResMsg );

		// 统计 取SQL语句前10字节
		char tag[PERF_SQL_LEN] = {0};
		strncpy( tag, pReqMsg->sql().c_str(), PERF_SQL_LEN-1 );
		PERF_BEGIN_DB( tag );
		if( pReqMsg->sqltype() == SELECT )
		{
			QueryResult* res = m_pDatabase->Query( pReqMsg->sql().c_str() );
			
			if( res == NULL )
			{
				TRACE_ERROR( "sql %s exec, but no resultset returned", pReqMsg->sql().c_str() );
				tSqlResMsg.set_resultcode( 0 );
			}
			else
			{
				tSqlResMsg.set_resultcode( 1 );
				tSqlResMsg.set_rowcount( res->GetRowCount() );
				tSqlResMsg.set_colcount( res->GetFieldCount() );

				//TODO: 列名 + 列类型 暂时不赋值

				if( res->GetRowCount() > 0 )
				{
					// 列值 和 值长度
					do
					{
						Field* pField = res->Fetch();
						if( pField == NULL )
						{
							TRACE_ERROR("ERROR: do sql %s success, row[%d], col[%d], but some row is null\n",pReqMsg->sql().c_str(), res->GetRowCount(), res->GetFieldCount() );

							//TODO: 出错设为0
							tSqlResMsg.set_rowcount( 0 );
							break;
						}

						for( int j = 0; j < res->GetFieldCount(); j++ )
						{
							tSqlResMsg.add_fieldvalue( pField[j].GetString(), pField[j].GetValueLen() );
							tSqlResMsg.add_fieldvaluelen( pField[j].GetValueLen() );
						}

					}while(  res->NextRow() );
				}
			}

			// 安全释放结果集
			if( res != NULL )
			{
				delete res;
				res = NULL;
			}

			m_pDatabase->ClearResult();
		}
		else
		{
			bool bExecResult = m_pDatabase->DirectExecute( pReqMsg->sql().c_str() );
			tSqlResMsg.set_resultcode( bExecResult ? 1 : 0 );
		}
		PERF_END_DB(tag);

		// 取自增长字段
		if( pReqMsg->has_latestautoincid() )
		{
			char tag2[PERF_SQL_LEN] = {0};
			strncpy( tag2, "select_last_insert_id", PERF_SQL_LEN-1 );
			PERF_BEGIN_DB(tag2);

			QueryResult* res = m_pDatabase->Query( "select last_insert_id()" );
			if( res != NULL )
			{
				if( res->GetRowCount() == 1 )
				{
					Field* pField = res->Fetch();
					if( pField != NULL )
					{
						tSqlResMsg.set_latestautoincid( pField->GetUInt32() );
					}
				}
				delete res;
				res = NULL;
			}
			PERF_END_DB(tag2);
		}

		// 回复客户端
		SendResponse( &tMsg );

	}
	else
	{
#ifndef _ASYNC_SQL_
		// 同步执行
		// 统计 取SQL语句前10字节
		char tag[PERF_SQL_LEN] = {0};
		strncpy( tag, pReqMsg->sql().c_str(), PERF_SQL_LEN-1 );
		PERF_BEGIN_DB( tag );
		if( m_pDatabase->DirectExecute( pReqMsg->sql().c_str() ) != true )
		{
			TRACE_ERROR( "Error: [%s][%d][%s], direct exec sql %s faild.\n", __LK_FILE__, __LINE__, __FUNCTION__, pReqMsg->sql().c_str() );
		}
		PERF_END_DB( tag );
#else
		// 异步执行sql
		if( ExecSqlAsync( pReqMsg->sql().c_str() ) < 0 )
		{
			TRACE_ERROR( "Error: [%s][%d][%s], exec async sql %s faild.\n", __LK_FILE__, __LINE__, __FUNCTION__, pReqMsg->sql().c_str() );
		}
#endif
	}


	return 0;
}



//************************************
// Method:    ProcessLoadPlayerItemInfoRequest
// FullName:  CDBHandle::ProcessLoadPlayerItemInfoRequest
// Access:    private 
// Returns:   int
// Qualifier: 处理场景的加载玩家道具信息请求，按照PB格式返回给场景
// Parameter: CMessage * pMsg
//************************************
int CDBHandle::ProcessLoadPlayerItemInfoRequest( CMessage* pMsg )
{
	//TRACE_DEBUG( "------%s-----\n", __FUNCTION__);

	CMessageLoadPlayerItemInfoRequest* pReqMsg = (CMessageLoadPlayerItemInfoRequest*)pMsg->msgpara();

	int nAccountID = pReqMsg->accountid();
	int nCharID = pReqMsg->charid();
	int nSessionID = pReqMsg->sessionid();
	
	char szSqlBuff[256] = {0};
	
	//TODO: 下面都默认snprintf不会出错。buff足够大
	int nTmpSPRet = 0;
	int nSQLLen = 0;
	
	nTmpSPRet = snprintf( szSqlBuff, sizeof(szSqlBuff)-1,"select " );
	if( nTmpSPRet < 0 )
	{
		TRACE_ERROR( "ERROR: [%s:%d] buff overflow", __FUNCTION__, __LINE__ );
		return -1;
	}
	nSQLLen += nTmpSPRet;

	for( int i = 0; i <  pReqMsg->packageidx_size(); i++ )
	{
		int nPkgIdx = pReqMsg->packageidx( i );

		const char* pColumName = NULL;

		switch( nPkgIdx )
		{
		case PACKAGE_BAGGAGE:
			{
				pColumName = "role_package" ;
				break;
			}
		case PACKAGE_EQUIPMENT:
			{
				pColumName ="role_equipment" ;
				break;
			}
		case PACKAGE_TASK:
			{
				pColumName = "role_task_package" ;
				break;
			}
		case PACKAGE_STORAGE:
			{
				pColumName ="role_storage" ;
				break;
			}
		default:
			{
				TRACE_ERROR( "[%s:%d] Invalid pkg idx %d", __FUNCTION__, __LINE__, nPkgIdx );
				return -1;
			}

		}

		nTmpSPRet = snprintf( szSqlBuff+nSQLLen, sizeof(szSqlBuff)-1-nSQLLen, pColumName );
		if( nTmpSPRet < 0 )
		{
			TRACE_ERROR( "ERROR: [%s:%d] buff overflow", __FUNCTION__, __LINE__ );
			return -1;
		}
		nSQLLen += nTmpSPRet;

		if( i+1 < pReqMsg->packageidx_size() )
		{
			nTmpSPRet = snprintf( szSqlBuff+nSQLLen, sizeof(szSqlBuff)-1-nSQLLen,"," );
			if( nTmpSPRet < 0 )
			{
				TRACE_ERROR( "ERROR: [%s:%d] buff overflow", __FUNCTION__, __LINE__ );
				return -1;
			}
			nSQLLen += nTmpSPRet;
		}
	}


	nTmpSPRet = snprintf( szSqlBuff+nSQLLen, sizeof(szSqlBuff)-1-nSQLLen, 
		" from UMS_ROLE_DETAIL_%02d where role_id = %d",UIN_TO_ITEMTABLEID(nAccountID), nCharID );
	if( nTmpSPRet < 0 )
	{
		TRACE_ERROR( "ERROR: [%s:%d] buff overflow", __FUNCTION__, __LINE__ );	
		return -1;
	}

	CMessage tMsg;
	CMessageLoadPlayerItemInfoResponse tResPara ;

	tMsg.mutable_msghead()->set_messageid( ID_D2S_LOADPLAYERITEMINFO_RESPONSE );
	tMsg.mutable_msghead()->set_srcfe( FE_DATASERVER );
	tMsg.mutable_msghead()->set_srcid( pMsg->msghead().dstid() );
	tMsg.mutable_msghead()->set_dstfe( pMsg->msghead().srcfe() );
	tMsg.mutable_msghead()->set_dstid( pMsg->msghead().srcid() );

	tMsg.set_msgpara( (unsigned int)(&tResPara) );

	tResPara.set_accountid( nAccountID );
	tResPara.set_charid( nCharID );
	
	tResPara.set_sessionid( nSessionID );
	
	QueryResult* res = m_pDatabase->Query( szSqlBuff );
	if( res == NULL )
	{
		TRACE_ERROR( "sql %s exec, but no resultset returned", szSqlBuff );
		tResPara.set_errcode(CMessageLoadPlayerItemInfoResponse::SQL_FAILED);
		SendResponse( &tMsg );
		return 0;
	}

	int nRow = res->GetRowCount() ;
	int nColumn = res->GetFieldCount() ;

	Field* pField = res->Fetch();
	if( pField == NULL )
	{
		TRACE_ERROR("ERROR: do sql %s success,row[%d], col[%d], but some row is null\n",
			szSqlBuff,nRow, nColumn );

		tResPara.set_errcode(CMessageLoadPlayerItemInfoResponse::SQL_FAILED);
		SendResponse( &tMsg );

		// 安全释放结果集
		if( res != NULL )
		{
			delete res;
			res = NULL;
		}
		return 0;
	}
	
	// 是否有记录
	if( nRow == 0 )
	{
		TRACE_ERROR("ERROR: do sql %s success, row[%d], col[%d], but record is empty \n",
			szSqlBuff,nRow, nColumn );
		tResPara.set_errcode(CMessageLoadPlayerItemInfoResponse::RECORD_EMPTY);
		SendResponse( &tMsg );
		// 安全释放结果集
		if( res != NULL )
		{
			delete res;
			res = NULL;
		}
		return 0;
	}

	// 判断返回的列数是否正确
	if( nColumn != pReqMsg->packageidx_size() )
	{
		TRACE_ERROR( "ERROR: select %d pkg col but return %d", pReqMsg->packageidx_size(), nColumn );
		tResPara.set_errcode(CMessageLoadPlayerItemInfoResponse::RECORD_EMPTY);
		SendResponse( &tMsg );
		// 安全释放结果集
		if( res != NULL )
		{
			delete res;
			res = NULL;
		}
		return 0;
	}

	for( int i = 0; i <  pReqMsg->packageidx_size(); i++ )
	{
		tResPara.add_packageidx( pReqMsg->packageidx(i) );
		PBItemBox* pItemBox = tResPara.add_itembox();
		if( pItemBox == NULL )
		{
			TRACE_ERROR("ERROR: add PBItemBox faild, maybe run out of mem\n");
			return 0;
			// 安全释放结果集
			if( res != NULL )
			{
				delete res;
				res = NULL;
			}
		}

		if( pItemBox->ParseFromArray(  pField[i].GetString(),  pField[i].GetValueLen() ) == false )
		{
			tResPara.set_errcode(CMessageLoadPlayerItemInfoResponse::DATA_CRASH);
			SendResponse( &tMsg );
			// 安全释放结果集
			if( res != NULL )
			{
				delete res;
				res = NULL;
			}
			return 0;
		}
	}

	tResPara.set_errcode(CMessageLoadPlayerItemInfoResponse::SUCCES);
	SendResponse( &tMsg );

	// 安全释放结果集
	if( res != NULL )
	{
		delete res;
		res = NULL;
	}

	return 0;
}

//************************************
// Method:    ProcessSavePlayerItemInfoRequest
// FullName:  CDBHandle::ProcessSavePlayerItemInfoRequest
// Access:    private 
// Returns:   int
// Qualifier: 处理存储玩家道具信息请求
// Parameter: CMessage * pMsg
//************************************
int CDBHandle::ProcessSavePlayerItemInfoRequest( CMessage* pMsg )
{
	//TRACE_DEBUG( "------%s-----\n", __FUNCTION__);

	CMessageSavePlayerItemInfoRequest* pReqMsg = (CMessageSavePlayerItemInfoRequest*)pMsg->msgpara();

	int nAccountID = pReqMsg->accountid();
	int nCharID = pReqMsg->charid();
	int nSessionID = pReqMsg->sessionid();

	CMessage tMsg;
	CMessageSavePlayerItemInfoResponse tResPara ;

	tMsg.mutable_msghead()->set_messageid( ID_D2S_SAVEPLAYERITEMINFO_RESPONSE );
	tMsg.mutable_msghead()->set_srcfe( FE_DATASERVER );
	tMsg.mutable_msghead()->set_srcid( pMsg->msghead().dstid() );
	tMsg.mutable_msghead()->set_dstfe( pMsg->msghead().srcfe() );
	tMsg.mutable_msghead()->set_dstid( pMsg->msghead().srcid() );

	tMsg.set_msgpara( (unsigned int)(&tResPara) );

	tResPara.set_accountid( nAccountID );
	tResPara.set_charid( nCharID );
	tResPara.set_sessionid( nSessionID );

	static const int snItemBoxBlobLen = 204800;

	char cSQL[ snItemBoxBlobLen * 2 ] = {0};
	char cTempItemBuff[snItemBoxBlobLen] = {0};
	char szFilterBuff[snItemBoxBlobLen] = {0};

	int nTempLen = 0;

	char* pszTmpStr = NULL;
	PBItemBox* pItemBox = NULL;

	int nErrCode = CMessageSavePlayerItemInfoResponse::SUCCES; 

	int nTmpSPRet = 0;
	int nLen = 0;
	
	// 拼SQL首部
	nTmpSPRet = snprintf( cSQL, sizeof(cSQL)-1, "update UMS_ROLE_DETAIL_%02d set ", UIN_TO_ITEMTABLEID(nAccountID) );
	if( nTmpSPRet < 0 )
	{
		TRACE_ERROR( "ERROR: [%s:%d] buff overflow", __FUNCTION__, __LINE__ );
		return -1;
	}
	nLen += nTmpSPRet;

	for( int i = 0; i < pReqMsg->packageidx_size(); i++ )
	{
		tResPara.add_packageidx( pReqMsg->packageidx( i ) );

		pItemBox = pReqMsg->mutable_itembox( i );
		if( pItemBox == NULL )
		{
			TRACE_ERROR( "FATAL: not enough memory! \n" );
			return -1;
		}

		pszTmpStr = szFilterBuff;
		if( pItemBox->SerializeToArray( cTempItemBuff, sizeof(cTempItemBuff)-1  ) == false )
		{
			nErrCode = CMessageSavePlayerItemInfoResponse::SERALIZE_FAILED;
			break;
		}
		int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szFilterBuff)-1, cTempItemBuff, pItemBox->ByteSize() );
		if( tRealLen == -1 )
		{
			TRACE_ERROR( "[%s:%d] while escape_data_string, dst buff overflow\n" , __FUNCTION__, __LINE__ );
			nErrCode = CMessageSavePlayerItemInfoResponse::FILTER_FAILED ;
			break;
		}

		const char* pChar = NULL;

		switch( pReqMsg->packageidx( i ) )
		{
		case PACKAGE_EQUIPMENT:
			{
				pChar =  "role_equipment = '%s'";
				break;
			}
		case PACKAGE_BAGGAGE:
			{
				pChar =  "role_package = '%s'";
				break;
			}
		case PACKAGE_TASK:
			{
				pChar =  "role_task_package = '%s'";
				break;
			}
		case PACKAGE_STORAGE:
			{
				pChar = "role_storage = '%s'";
				break;
			}
		default:
			{
				TRACE_ERROR( "invalide pkgidx %d\n", pReqMsg->packageidx( i ) );
				return -1;
			}
		}

		if( nErrCode == CMessageSavePlayerItemInfoResponse::SUCCES )
		{
			// 拼列名和值
			nTmpSPRet = snprintf( cSQL+nLen, sizeof(cSQL)-1-nLen, pChar, szFilterBuff );
			if( nTmpSPRet < 0 )
			{
				TRACE_ERROR( "ERROR: [%s:%d] buff overflow", __FUNCTION__, __LINE__ );
				return -1;
			}
			nLen += nTmpSPRet; 

			// 如果不是结尾，拼,
			if( i+1 < pReqMsg->packageidx_size() )
			{
				nTmpSPRet = snprintf( cSQL+nLen, sizeof(cSQL)-1-nLen, "," );
				if( nTmpSPRet < 0 )
				{
					TRACE_ERROR( "ERROR: [%s:%d] buff overflow", __FUNCTION__, __LINE__ );
					return -1;
				}
				nLen += nTmpSPRet; 
			}
		}

	}
	
	if( nErrCode == CMessageSavePlayerItemInfoResponse::SUCCES )
	{
		// 拼结尾字符串
		nTmpSPRet = snprintf( cSQL+nLen, sizeof(cSQL)-1-nLen," where role_id = %d", nCharID );
		if( nTmpSPRet < 0 )
		{
			TRACE_ERROR( "ERROR: [%s:%d] buff overflow", __FUNCTION__, __LINE__ );
			return -1;
		}
		nLen += nTmpSPRet;

		if( m_pDatabase->DirectExecute( cSQL ) != true )
		{
			TRACE_ERROR( "Error: [%s][%d][%s], direct exec sql %s faild.\n", __LK_FILE__, __LINE__, __FUNCTION__, cSQL );
			nErrCode = CMessageSavePlayerItemInfoResponse::SQL_FAILED;
		}
	
		//TRACE_DEBUG( "Exec: %s\n", cSQL );
	}

	tResPara.set_errcode(nErrCode);
	SendResponse( &tMsg );

	return 0;
}

int CDBHandle::ProcessSavePlayerRequest( CMessage* vpMsg )
{			
	CMaxString tSql, tSql2;

	CMessageSavePlayerRequest* tpSave = ( CMessageSavePlayerRequest* )vpMsg->msgpara();

	if( tpSave->has_info( ) == true )
	{
		tSql.clear( );

		tSql.append( "update UMS_ROLE set " );

		protobuf2Sql( tpSave->info( ), tSql, NULL, NULL, sql_format );				

		tSql.append( " where role_id=%d", tpSave->roleid( ) );


		CMessageExecuteSqlRequest tReqMsg;

		CMessage tMsg;
		tMsg.mutable_msghead()->set_srcfe( FE_DATASERVER );
		tMsg.mutable_msghead()->set_dstid( vpMsg->msghead().dstid() );
		tMsg.mutable_msghead()->set_srcfe( vpMsg->msghead().srcfe() );
		tMsg.mutable_msghead()->set_srcid( vpMsg->msghead().srcid() );

		tReqMsg.set_sessionid( tpSave->sessionid() );
		tReqMsg.set_sqltype( UPDATE );

		tMsg.set_msgpara( (uint32)&tReqMsg );

		tReqMsg.set_sql( tSql.c_str( ) );

		tReqMsg.set_callback( tpSave->callback( ) );

		ProcessExecuteSqlRequest( &tMsg );

	//	printf( "%s\n", tSql.c_str( ) );
	}	

	if( tpSave->has_property( ) == true )
	{
		tSql.clear( );
		tSql2.clear( );

		tSql.append( "update UMS_ROLE_DETAIL_%02d set ", tpSave->detailid( ) );
		tSql2.append( "update UMS_ROLE_DETAIL1_%02d set ", tpSave->detailid( ) );

		// protobuf2Sql( tpSave->property( ), tSql, NULL, NULL, sql_format );				
		const Message& message = tpSave->property();
		const Reflection* reflection = message.GetReflection();
		std::vector<const FieldDescriptor*> fields;
		reflection->ListFields(message, &fields);

		for (unsigned int f1 = 0, f2 = 0, i = 0; i < fields.size(); i++)
		{
			if ( mField2Table.find(fields[i]->name().c_str()) != mField2Table.end())	
			{
				if ( f1 > 0)  tSql.append( "," );

				f1 ++;
				print_field(message, reflection, fields[i], tSql, NULL, sql_format );
			}else
			{
				if ( f2 > 0)  tSql2.append( "," );

				f2 ++;
				print_field(message, reflection, fields[i], tSql2, NULL, sql_format );
			}
		}

		tSql.append( " where role_id=%d", tpSave->roleid( ) );
		tSql2.append( " where role_id=%d", tpSave->roleid( ) );

		// 统计 取SQL语句前10字节
		char tag[PERF_SQL_LEN] = {0};
		strncpy( tag, tSql.c_str(), PERF_SQL_LEN-1 );
		PERF_BEGIN_DB( tag );

		bool bExecResult = m_pDatabase->DirectExecute( tSql.c_str() ) && m_pDatabase->DirectExecute( tSql2.c_str() );
		PERF_END_DB(tag);


		if (tpSave->callback( ))
		{
			CMessage tMsg;
			tMsg.mutable_msghead()->set_srcfe( FE_DATASERVER );
			tMsg.mutable_msghead()->set_srcid( vpMsg->msghead().dstid() );
			tMsg.mutable_msghead()->set_dstfe( vpMsg->msghead().srcfe() );
			tMsg.mutable_msghead()->set_dstid( vpMsg->msghead().srcid() );

			CMessageExecuteSqlResponse3 tSqlResMsg;
			tMsg.mutable_msghead()->set_messageid( ID_S2S_EXECUTESQL_RESPONSE );
			tSqlResMsg.set_sessionid( tpSave->sessionid() );

			tMsg.set_msgpara( (uint32)&tSqlResMsg );
			tSqlResMsg.set_resultcode( bExecResult ? 1 : 0 );

			SendResponse( &tMsg );
		}

	//	printf( "%s\n", tSql.c_str( ) );
	}

	return 0;
}

int CDBHandle::ProcessLoadPlayerInfo( CMessage* vpMsg )
{
	CMessageLoadPlayerInfoRequest* tpLoad = ( CMessageLoadPlayerInfoRequest* )vpMsg->msgpara( );
	
	char tSql[ 1024 ] = { 0 };
	snprintf( tSql, sizeof( tSql ) - 1, 
				"SELECT role_id, a.account_id, role_name, gender_id, metier_id, "
                    "nationality_id, face_type, line_id, map_id, role_level, "
                    "role_expr, role_money, pos_x, pos_y, is_alive, "
                    "login_time, online_duration, save_status, save_bitflag, bounded_money, "
                    "role_password, a.check_sum, role_status_param, a.silver_yb, "
					"storage_money, storage_bounded_money, user_name, role_protecttime, role_vip_flag, role_vip_time, "
					" create_time, role_marriageid,role_changecamp_time,role_status, role_swornid FROM UMS_ROLE a, UMS_ACCOUNT b WHERE role_id = %d AND a.account_id"
					" = b.account_id", tpLoad->roleid( ) );


	CMessageExecuteSqlRequest tReqMsg;

	CMessage tMsg;
	tMsg.mutable_msghead()->set_srcfe( FE_DATASERVER );
	tMsg.mutable_msghead()->set_dstid( vpMsg->msghead().dstid() );
	tMsg.mutable_msghead()->set_srcfe( vpMsg->msghead().srcfe() );
	tMsg.mutable_msghead()->set_srcid( vpMsg->msghead().srcid() );

	tReqMsg.set_sessionid( tpLoad->sessionid() );
	tReqMsg.set_sqltype( SELECT );

	tMsg.set_msgpara( (uint32)&tReqMsg );

	tReqMsg.set_sql( tSql );

	tReqMsg.set_callback( 1 );

	ProcessExecuteSqlRequest( &tMsg );

	return SUCCESS;
}

int CDBHandle::ProcessLoadPlayerProperty( CMessage* vpMsg )
{
	TRACE_DEBUG( "----------[%s]---------", __FUNCTION__ );

	CMessageLoadPlayerPropertyRequest* tpLoad = ( CMessageLoadPlayerPropertyRequest* )vpMsg->msgpara( );
	CMessagePlayerProperty tProperty;

	char tSql[ 1024 ] = { 0 };

	snprintf( tSql, sizeof( tSql ) - 1, "select role_hp, role_mp, role_ap, role_basic, role_skill, role_buff,"
		"role_cdgroup, role_pvp, role_title, role_activity, b.role_talent, b.role_mw, b.role_lifeskill, b.role_statistic,"
		" b.role_task, b.role_monster_record, b.role_tele_pos, b.role_wizard, b.role_friend, role_repetion, b.role_client,"
		" save_bitflag, b.role_charm, b.role_guide, b.role_times, b.role_matrix, b.role_exptimeinfo, b.role_treasurebowl, b.role_spiritinfo"
		" FROM UMS_ROLE_DETAIL_%02d a, UMS_ROLE_DETAIL1_%02d b "
		" WHERE a.role_id = %d and b.role_id = %d and a.role_id = b.role_id ", 
		tpLoad->detailid( ), tpLoad->detailid( ), tpLoad->roleid( ) , tpLoad->roleid( ) );

	QueryResult* res = m_pDatabase->Query( tSql );	
	if( res == NULL )
	{
		TRACE_ERROR( "[ %s : %d ][ %s ]sql %s exec, but no resultset returned", __LK_FILE__, __LINE__, __FUNCTION__,
			tSql );
		return 0;
	}

	int nRow = res->GetRowCount() ;
	int nColumn = res->GetFieldCount() ;

    CMessage tMsg;
    CMessageLoadPlayerPropertyResponse tResPara ;

    tMsg.mutable_msghead()->set_messageid( ID_D2S_LOADPLAYERPROPERTY_RESPONSE );
    tMsg.mutable_msghead()->set_srcfe( FE_DATASERVER );
    tMsg.mutable_msghead()->set_srcid( vpMsg->msghead().dstid() );
    tMsg.mutable_msghead()->set_dstfe( vpMsg->msghead().srcfe() );
    tMsg.mutable_msghead()->set_dstid( vpMsg->msghead().srcid() );

    tMsg.set_msgpara( (unsigned int)(&tResPara) );

	tResPara.set_roleid( tpLoad->roleid( ) );
	tResPara.set_sessionid( tpLoad->sessionid( ) );


	Field* pField = res->Fetch();
	if( pField == NULL )
	{
		TRACE_ERROR("[ %s : %d ][ %s ]ERROR: do sql %s success,row[%d], col[%d], but some row is null\n",
			__LK_FILE__, __LINE__, __FUNCTION__, tSql, nRow, nColumn );

		// 安全释放结果集
		if( res != NULL )
		{
			delete res;
			res = NULL;
		}

		tResPara.set_resultcode( ERR_NOT_FOUND );
		SendMessageTo( &tMsg, m_stCurrentProxyHead.srcfe(), m_stCurrentProxyHead.srcid(), m_stCurrentProxyHead.uin() );	
		return 0;
	}
	
	// 是否有记录
	if( nRow == 0 )
	{
		TRACE_ERROR("[ %s : %d ][ %s ]ERROR: do sql %s success, row[%d], col[%d], but record is empty \n",
			__LK_FILE__, __LINE__, __FUNCTION__, tSql, nRow, nColumn );
		// 安全释放结果集
		if( res != NULL )
		{
			delete res;
			res = NULL;
		}

		tResPara.set_resultcode( ERR_NOT_FOUND );
		SendMessageTo( &tMsg, m_stCurrentProxyHead.srcfe(), m_stCurrentProxyHead.srcid(), m_stCurrentProxyHead.uin() );	
		return 0;
	}

	tProperty.set_role_hp( atoi( pField[ 0 ].GetString( ) ) );
	tProperty.set_role_mp( atoi( pField[ 1 ].GetString( ) ) );	
	tProperty.set_role_ap( atoi( pField[ 2 ].GetString( ) ) );
	
	if( tProperty.mutable_role_basic( )->ParseFromArray( pField[ 3 ].GetString( ), pField[ 3 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_basic failed, len = %d", tpLoad->roleid( ), pField[ 3 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_skill( )->ParseFromArray( pField[ 4 ].GetString( ), pField[ 4 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_skill failed, len = %d", tpLoad->roleid( ), pField[ 4 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_buff( )->ParseFromArray( pField[ 5 ].GetString( ), pField[ 5 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_buff failed, len = %d", tpLoad->roleid( ), pField[ 5 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_cdgroup( )->ParseFromArray( pField[ 6 ].GetString( ), pField[ 6 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_cdgroup failed, len = %d", tpLoad->roleid( ), pField[ 6 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_pvp( )->ParseFromArray( pField[ 7 ].GetString( ), pField[ 7 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_pvp failed, len = %d", tpLoad->roleid( ), pField[ 7 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_title( )->ParseFromArray( pField[ 8 ].GetString( ), pField[ 8 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_title failed, len = %d", tpLoad->roleid( ), pField[ 8 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_activity( )->ParseFromArray( pField[ 9 ].GetString( ), pField[ 9 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_activity failed, len = %d", tpLoad->roleid( ), pField[ 9 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_talent( )->ParseFromArray( pField[ 10 ].GetString( ), pField[ 10 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_talent failed, len = %d", tpLoad->roleid( ), pField[ 10 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_mw( )->ParseFromArray( pField[ 11 ].GetString( ), pField[ 11 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_mw failed, len = %d", tpLoad->roleid( ), pField[ 11 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_lifeskill( )->ParseFromArray( pField[ 12 ].GetString( ), pField[ 12 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_lifeskill failed, len = %d", tpLoad->roleid( ), pField[ 12 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_statistic( )->ParseFromArray( pField[ 13 ].GetString( ), pField[ 13 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_statistic failed, len = %d", tpLoad->roleid( ), pField[ 13 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_task( )->ParseFromArray( pField[ 14 ].GetString( ), pField[ 14 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_task failed, len = %d", tpLoad->roleid( ), pField[ 14 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_monster_record( )->ParseFromArray( pField[ 15 ].GetString( ), pField[ 15 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_monster_record failed, len = %d", tpLoad->roleid( ), pField[ 15 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_tele_pos( )->ParseFromArray( pField[ 16 ].GetString( ), pField[ 16 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_tele_pos failed, len = %d", tpLoad->roleid( ), pField[ 16 ].GetValueLen( ) );	
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_wizard( )->ParseFromArray( pField[ 17 ].GetString( ), pField[ 17 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_wizard failed, len = %d", tpLoad->roleid( ), pField[ 17 ].GetValueLen( ) );	
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_friend( )->ParseFromArray( pField[ 18 ].GetString( ), pField[ 18 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_wizard failed, len = %d", tpLoad->roleid( ), pField[ 18 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_repetion( )->ParseFromArray( pField[ 19 ].GetString( ), pField[ 19 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_repetion failed, len = %d", tpLoad->roleid( ), pField[ 19 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_client( )->ParseFromArray( pField[ 20 ].GetString( ), pField[ 20 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_client failed, len = %d", tpLoad->roleid( ), pField[ 20 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	tProperty.set_save_bitflag( atoi( pField[ 21 ].GetString( ) ) );

	if( tProperty.mutable_role_charm( )->ParseFromArray( pField[ 22 ].GetString( ), pField[ 22 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_charm failed, len = %d", tpLoad->roleid( ), pField[ 22 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_guide( )->ParseFromArray( pField[ 23 ].GetString( ), pField[ 23 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_guide failed, len = %d", tpLoad->roleid( ), pField[ 23 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_times( )->ParseFromArray( pField[ 24 ].GetString( ), pField[ 24 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_times failed, len = %d", tpLoad->roleid( ), pField[ 24 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if( tProperty.mutable_role_matrix( )->ParseFromArray( pField[ 25 ].GetString( ), pField[ 25 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_times failed, len = %d", tpLoad->roleid( ), pField[ 25 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}
	
	if( tProperty.mutable_role_exptimeinfo( )->ParseFromArray( pField[ 26 ].GetString( ), pField[ 26 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_exptimeinfo failed, len = %d", tpLoad->roleid( ), pField[ 26 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}
	
	if ( tProperty.mutable_role_treasurebowl()->ParseFromArray( pField[ 27 ].GetString( ), pField[ 27 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse role_treasurebowl failed, len = %d", tpLoad->roleid( ), pField[ 27 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	if ( tProperty.mutable_role_spiritinfo()->ParseFromArray( pField[ 28 ].GetString( ), pField[ 28 ].GetValueLen( ) ) != true )
	{
		TRACE_ERROR( "Player %d Parse spiritinfo failed, len = %d", tpLoad->roleid( ), pField[ 28 ].GetValueLen( ) );
		tResPara.set_resultcode( ERR_PARSEPB );
	}

	// 安全释放结果集
	if( res != NULL )
	{
		delete res;
		res = NULL;
	}
	
	tResPara.set_resultcode( SUCCESS );
	tResPara.mutable_property( )->CopyFrom( tProperty );

	SendMessageTo( &tMsg, m_stCurrentProxyHead.srcfe(), m_stCurrentProxyHead.srcid(), m_stCurrentProxyHead.uin() );		

	return SUCCESS;	
}

/*
int CDBHandle::GetSequestRequest( CMessage* pMsg )
{
	CMessageGetSequestRequest* tpMsg = ( CMessageGetSequestRequest* )pMsg->msgpara( );
   CMessage tMsg;
    CMessageGetSequestResponse tResPara ;

    tMsg.mutable_msghead()->set_messageid( ID_D2S_GETSEQUEST_RESPONSE );
    tMsg.mutable_msghead()->set_srcfe( FE_DATASERVER );
    tMsg.mutable_msghead()->set_srcid( pMsg->msghead().dstid() );
    tMsg.mutable_msghead()->set_dstfe( pMsg->msghead().srcfe() );
    tMsg.mutable_msghead()->set_dstid( pMsg->msghead().srcid() );

    tMsg.set_msgpara( (unsigned int)(&tResPara) );

	tResPara.set_sessionid( tpMsg->sessionid( ) );

	char tSql[ 128 ] = { 0 };
	snprintf( tSql, sizeof( tSql ), "select sequest from UMS_SEQUEST where server_id=%d", tpMsg->serverid( ) );

	QueryResult* res = m_pDatabase->Query( tSql );
	if( res == NULL )
	{
		TRACE_ERROR( "[ %s : %d ][ %s ]sql %s exec, but no resultset returned", __LK_FILE__, __LINE__, __FUNCTION__,
			tSql );
		return 0;
	}

	int nRow = res->GetRowCount() ;
	int nColumn = res->GetFieldCount() ;


	Field* pField = res->Fetch();
	if( pField == NULL )
	{
		TRACE_ERROR("[ %s : %d ][ %s ]ERROR: do sql %s success,row[%d], col[%d], but some row is null\n",
			__LK_FILE__, __LINE__, __FUNCTION__, tSql, nRow, nColumn );

		// 安全释放结果集
		if( res != NULL )
		{
			delete res;
			res = NULL;
		}

		return 0;
	}
	
	// 是否有记录
	if( nRow == 0 )
	{
		TRACE_ERROR("[ %s : %d ][ %s ]ERROR: do sql %s success, row[%d], col[%d], but record is empty \n",
			__LK_FILE__, __LINE__, __FUNCTION__, tSql, nRow, nColumn );
		// 安全释放结果集
		if( res != NULL )
		{
			delete res;
			res = NULL;
		}

		return 0;
	}

	int tSequest = atoi( pField[ 0 ].GetString( ) );
	if( tSequest >= 100000000 )
	{
		tSequest = 0;
	}

	tResPara.set_nextminid( tSequest );
	tResPara.set_nextleft( 5000 );

	snprintf( tSql, sizeof( tSql ), "update UMS_SEQUEST set sequest=%d where server_id=%d", tSequest + 5000,
		tpMsg->serverid( ) );
		
	if( m_pDatabase->DirectExecute( tSql ) != true )
	{
		return -1;
	}

	SendMessageTo( &tMsg, m_stCurrentProxyHead.srcfe(), m_stCurrentProxyHead.srcid(), m_stCurrentProxyHead.uin() );	

	return SUCCESS;
}
*/

int CDBHandle::OnMessageGetBourseInfo( CMessage* vpMsg )
{
	CMessageGetBourseInfo* tpInfo = ( CMessageGetBourseInfo* )vpMsg->msgpara( );
//	GetBourseInfo( tpInfo->sessionid( ) );

	GetBourseInfo( tpInfo->sessionid( ), &vpMsg->msghead() );	
}

int CDBHandle::GetBourseInfo( int vSessionID, const CMessageHead* vpHead )
{
	char tSql[ 256 ] = { 0 };
	CMessageBourseInfoNotice tInfo;
	CMessage tMsg;

	snprintf( tSql, sizeof( tSql ), "select bourse_doc_no, account_id, role_id, price, sum( yb_count - trade_yb),"
		"  bourse_date, bourse_trade_detail from UMS_BOURSE_TRADE where trade_type = %d and yb_count - trade_yb != 0"
		" and trade_kind = %d group by price order by price desc limit 0, 5", BOURSE_TRADE_ING, BOURSE_TRADE_BUY );

	BourseQueryInsertDetail( tInfo.mutable_buylist( ), tSql );

    snprintf( tSql, sizeof( tSql ), "select bourse_doc_no, account_id, role_id, price, sum( yb_count - trade_yb ), "
		" bourse_date, bourse_trade_detail from UMS_BOURSE_TRADE where trade_type = %d and yb_count - trade_yb != 0"
		"  and trade_kind = %d group by price order by price limit 0, 5", BOURSE_TRADE_ING, BOURSE_TRADE_SELL );

	BourseQueryInsertDetail( tInfo.mutable_selllist( ), tSql );

	tMsg.mutable_msghead()->set_messageid( ID_D2G_BOURSELIST_NOTICE );
    tMsg.mutable_msghead()->set_srcfe( FE_DATASERVER );
    tMsg.mutable_msghead()->set_srcid( vpHead->dstid() );
    tMsg.mutable_msghead()->set_dstfe( vpHead->srcfe() );
    tMsg.mutable_msghead()->set_dstid( vpHead->srcid() );

    tMsg.set_msgpara( (unsigned int)(&tInfo) );

	tInfo.set_sessionid( vSessionID );

    SendMessageTo( &tMsg, m_stCurrentProxyHead.srcfe(), m_stCurrentProxyHead.srcid(), m_stCurrentProxyHead.uin() );

	return SUCCESS;
}

int CDBHandle::OnMessageGetBourseAccount( CMessage* pMsg )
{
	CMessageBourseS2DGetAccount* tpAccount = ( CMessageBourseS2DGetAccount* )pMsg->msgpara( );

	GetBourseAccount( tpAccount->sessionid( ), tpAccount->accountid( ), tpAccount->code( ), &pMsg->msghead(), 0, 0, 0 );
	
	return SUCCESS;
}

int CDBHandle::GetBourseAccount( int vSessionID, int vAccount, int vCode, const CMessageHead* vpHead, int vResultCode,
		int vParame1, int vParame2 )
{
	int tYb = 0;
	int tMoney = 0;

	CMessage tMsg;
    CMessageBourseD2SAccountNotice tResPara ;

    tMsg.mutable_msghead()->set_messageid( ID_D2S_BOURSE_ACCOUNT_NOTICE );
    tMsg.mutable_msghead()->set_srcfe( FE_DATASERVER );
    tMsg.mutable_msghead()->set_srcid( vpHead->dstid() );
    tMsg.mutable_msghead()->set_dstfe( vpHead->srcfe() );
    tMsg.mutable_msghead()->set_dstid( vpHead->srcid() );

    tMsg.set_msgpara( (unsigned int)(&tResPara) );

	tResPara.set_sessionid( vSessionID );
	tResPara.set_accountid( vAccount );	
	tResPara.set_code( vCode );

	int tRtn = GetBoureseAccountDB( vAccount, &tMoney, &tYb );
	if( tRtn != SUCCESS )
	{
		if( tRtn == ERR_NOT_FOUND )
		{
			CreateBourseAccount( vAccount );
		}
		else
		{
			LOG_ERROR( "default", "[ %s : %d ][ %s ] Cannt Get AccountID %d", __LK_FILE__, __LINE__, __FUNCTION__, 
				vAccount );
		}
	}

	tResPara.set_yb( tYb );
	tResPara.set_money( tMoney );
	tResPara.set_result( vResultCode );
	tResPara.set_parame1( vParame1 );
	tResPara.set_parame2( vParame2 );

    SendMessageTo( &tMsg, m_stCurrentProxyHead.srcfe(), m_stCurrentProxyHead.srcid(), m_stCurrentProxyHead.uin() );

	return SUCCESS;
}

int CDBHandle::CreateBourseAccount( int vAccount )
{
	char tSql[ 128 ] = { 0 };

	LOG_NOTICE( "default", "[ %s : %d ][ %s ] Begin Create Account %d", __LK_FILE__, __LINE__,
			__FUNCTION__, vAccount );

	bool bTran = m_pDatabase->BeginTransaction( );		
	bool bAddMoney = false;
	bool bAddYb = false;
	if( bTran == true )
	{
		// 事务开始, 创建帐户
		snprintf( tSql, sizeof( tSql ), "INSERT INTO UMS_BOURSE_ACCOUNT( account_id, currency_type, currency_value )"
				/*currency_lock)*/ " values( %d, %d, 0 )", vAccount, BOURSE_DB_YB_KEY );
		bAddYb = m_pDatabase->DirectExecute( tSql );
		snprintf( tSql, sizeof( tSql ), "INSERT INTO UMS_BOURSE_ACCOUNT( account_id, currency_type, currency_value )"
				/*currency_lock)*/ " values( %d, %d, 0 )", vAccount, BOURSE_DB_MONEY_KEY );
		bAddMoney = m_pDatabase->DirectExecute( tSql );

		if( bAddMoney == true && bAddYb == true )
		{
			// 建立帐户成功, 提交事务
			if( m_pDatabase->CommitTransaction( ) == true )
			{
				LOG_NOTICE( "default", "[ %s : %d ][ %s ] AccountID %d Create End, SUCCESS", __LK_FILE__, __LINE__,
						__FUNCTION__, vAccount );
			}
			else
			{
				LOG_ERROR( "default", "[ %s : %d ][ %s ] AccountID %d Create End, Tran Commit Error", __LK_FILE__,	
						__LINE__, __FUNCTION__, vAccount );
				return ERR_UNKNOWN;
			}
		}	
		else
		{
			LOG_ERROR( "default", "[ %s : %d ][ %s ] AccountID %d Create End, Sql Faild, Tran Back", __LK_FILE__,
					__LINE__, __FUNCTION__, vAccount );
			if( m_pDatabase->RollbackTransaction( ) == false )
			{
				LOG_ERROR( "default", "[ %s : %d ][ %s ] Trans Back Faild", __LK_FILE__, __LINE__, __FUNCTION__ );
				return ERR_UNKNOWN;
			}

			return ERR_UNKNOWN;
		}
	}
	else
	{
		// 事物建立失败
		LOG_ERROR( "default", "[ %s : %d ][ %s ] Cannt Begin Trans", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ERR_UNKNOWN;
	}

	return SUCCESS;
}

int CDBHandle::OnMessageBourseIncAccount( CMessage* pMsg )
{
	char tSql[ 256 ] = { 0 };
	CMessageBourseS2DIncAccount* tpInc = ( CMessageBourseS2DIncAccount* )pMsg->msgpara( );

	int tParame1[ 3 ] = { 0 };
	int tParame2 = 0;

	tParame1[ 0 ] = tpInc->accountid( );
	tParame1[ 1 ] = tpInc->yb( );
	tParame1[ 2 ] = tpInc->money( );
		
	int vResultCode = 0;

	int tRtn = TranDoingEvent( &CDBHandle::IncAccountTran, &vResultCode, tParame1, &tParame2 );
	if( tRtn != SUCCESS )
	{
		// 事务执行失败
		LOG_ERROR( "default", "[ %s : %d ][ %s ] Account %d Inc YB %d Money %d Tran Faild", __LK_FILE__, __LINE__,
			__FUNCTION__, tpInc->accountid( ), tpInc->yb( ),  tpInc->money( ) );
//		return SUCCESS;
		ExchangeAccountLogUnSend( );
	}

	if( vResultCode != SUCCESS )
	{
		        // ê????′DDê§°ü
        LOG_ERROR( "default", "[ %s : %d ][ %s ] Account %d Inc YB %d Money %d Tran OK, Code ERROR %d", __LK_FILE__, 
			__LINE__, __FUNCTION__, tpInc->accountid( ), tpInc->yb( ),  tpInc->money( ), vResultCode );
		ExchangeAccountLogUnSend( );
  //      return SUCCESS;
	}

	if( tRtn == SUCCESS && vResultCode == SUCCESS )
	{
		int tParameLog[ 4 ] = { 0 };
		tParameLog[ 0 ] = tpInc->money( );
		tParameLog[ 1 ] = tpInc->yb( );

		SetExchangeAccountLog( 0, tpInc->accountid( ), tpInc->roleid( ), EXCHANGE_ACCOUNT_ADD, tParameLog );
		ExchangeAccountLogSend( &pMsg->msghead( ) );	
	}

	// 添加交易所帐号成功, 同步Client 数据
	if( GetBourseAccount( tpInc->sessionid( ), tpInc->accountid( ), BOURSE_ACCOUNT_INC, &pMsg->msghead( ),
			vResultCode, tpInc->money( ), tpInc->yb( ) ) != SUCCESS )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] Inc Account %d, Money %d, Yb %d OK, But Get Account %d Faild",
			__LK_FILE__, __LINE__, __FUNCTION__, tpInc->accountid( ), tpInc->money( ), tpInc->yb( ),
			tpInc->accountid( ) );

		return SUCCESS;
	}
	
	return SUCCESS;
}

int CDBHandle::TranDoingEvent( TRAN_EVENT vEvent, int* vTranCode, int* vParame1, int* vParame2 )
{
	bool bTran = m_pDatabase->BeginTransaction( );		
	if( bTran == true )
	{
		LOG_DEBUG( "default", "[ %s : %d ][ %s ] Tran Begin", __LK_FILE__, __LINE__, __FUNCTION__ );
		if( ( this->*vEvent )( vTranCode, vParame1, vParame2 ) == true )
		{
	            // ?¨á￠?ê?§3é1|, ìá??ê???
            if( m_pDatabase->CommitTransaction( ) == true )
            {
                LOG_DEBUG( "default", "[ %s : %d ][ %s ] Tran Commit, SUCCESS", __LK_FILE__, __LINE__,  __FUNCTION__ );
				return SUCCESS;
            }
            else
            {
                LOG_ERROR( "default", "[ %s : %d ][ %s ]  Tran Commit Error", __LK_FILE__, __LINE__, __FUNCTION__ );
                return ERR_UNKNOWN;
            }	
		}	
		else
		{
			LOG_DEBUG( "default", "[ %s : %d ][ %s ] Tran Back", __LK_FILE__, __LINE__, __FUNCTION__ );
			if( m_pDatabase->RollbackTransaction( ) == false )
			{
				LOG_ERROR( "default", "[ %s : %d ][ %s ] Trans Back Faild", __LK_FILE__, __LINE__, __FUNCTION__ );
				return ERR_UNKNOWN;
			}

			return ERR_UNKNOWN;
		}
	}
	else
	{
		// 事物建立失败
		LOG_ERROR( "default", "[ %s : %d ][ %s ] Cannt Begin Trans", __LK_FILE__, __LINE__, __FUNCTION__ );
		return ERR_UNKNOWN;
	}

	return SUCCESS;	
}

bool CDBHandle::IncAccountTran( int* vTranCode, int* vParame1, int* vParame2 )
{
	int tAccountID = vParame1[ 0 ];
	int tYb = vParame1[ 1 ];
	int tMoney = vParame1[ 2 ];
	bool tAddYB = false;
	int tAddYBCount = 0;
	bool tAddMoney = false;
	int tAddMoneyCount = 0;
	bool tChangeYB = false;
	int tChangeYBCount = 0;

	char tSql[ 256 ] = { 0 };

	QueryResultMysql* res = NULL;

	snprintf( tSql, sizeof( tSql ), "UPDATE UMS_BOURSE_ACCOUNT SET currency_value = currency_value + %u where "
		" account_id=%d and currency_type = %d and currency_value + %d <= %d", tYb, tAccountID, 
		BOURSE_DB_YB_KEY, tYb, BOURSE_ACCOUNT_YB_MAX );

	tAddYB = m_pDatabase->DirectExecute( tSql );	
	tAddYBCount = m_pDatabase->AffectedRows( );

	if( ( tAddYB == false || tAddYBCount == 0 ) && tYb != 0 )
	{
		*vTranCode = BOURSE_ACCOUNT_YB_MUCH;
		return false;
	}


	snprintf( tSql, sizeof( tSql ), "UPDATE UMS_BOURSE_ACCOUNT SET currency_value = currency_value + %u where "
        " account_id=%d and currency_type = %d and currency_value + %d <= %d", tMoney, 
		tAccountID, BOURSE_DB_MONEY_KEY, tMoney, BOURSE_ACCOUNT_MONEY_MAX );

	tAddMoney = m_pDatabase->DirectExecute( tSql );
	tAddMoneyCount = m_pDatabase->AffectedRows( );

	if( ( tAddMoney == false ||  tAddMoneyCount == 0 ) && tMoney != 0 )
	{
		*vTranCode = BOURSE_ACCOUNT_MONEY_MUCH;
		return false;
	}

	snprintf( tSql, sizeof( tSql ), "UPDATE UMS_ACCOUNT  set golden_yb=golden_yb-%d where account_id=%d and"
		" golden_yb>=%d", tYb, tAccountID, tYb );
	tChangeYB = m_pDatabase->DirectExecute( tSql );
	tChangeYBCount = m_pDatabase->AffectedRows( );

	if( ( tChangeYB == false || tChangeYBCount == 0 ) && tYb != 0 )
	{
		*vTranCode = ERR_UNKNOWN;
		return false;
	}

	*vTranCode = SUCCESS;

	return true;
}

int CDBHandle::OnMessageBourseDecAccount( CMessage* pMsg )
{
	CMessageBourseS2DDecAccount* tpDec = ( CMessageBourseS2DDecAccount* )pMsg->msgpara( );	

	int tParame1[ 3 ] = { 0 };
	int tParame2 = 0;

	tParame1[ 0 ] = tpDec->accountid( );
	tParame1[ 1 ] = tpDec->yb( );
	tParame1[ 2 ] = tpDec->money( );
		
	int vResultCode = 0;

	int tCurMoney = 0;
	int tCurYb = 0;

    int tRtn = GetBoureseAccountDB( tpDec->accountid( ), &tCurMoney, &tCurYb );
    if( tRtn != SUCCESS )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] Cannt Get AccountID %d", __LK_FILE__, __LINE__, __FUNCTION__,
			tpDec->accountid( ) );
	}

	if( tCurMoney < tpDec->money( ) || tCurYb < tpDec->yb( ) )
	{
        GetBourseAccount( tpDec->sessionid( ), tpDec->accountid( ), BOURSE_ACCOUNT_DEC, &pMsg->msghead( ), ERR_UNKNOWN,
			0, 0 );
		return SUCCESS;
	}

	int tTranRtn = TranDoingEvent( &CDBHandle::DecAccountTran, &vResultCode, tParame1, &tParame2 );
	if( tTranRtn != SUCCESS )
	{
		// 事务执行失败
		LOG_ERROR( "default", "[ %s : %d ][ %s ] Account %d Inc YB %d Money %d Tran Faild", __LK_FILE__, __LINE__,
			__FUNCTION__, tpDec->accountid( ), tpDec->yb( ),  tpDec->money( ) );
		ExchangeAccountLogUnSend( );
	//	return SUCCESS;
	}

	if( vResultCode != SUCCESS )
	{
		        // ê????′DDê§°ü
        LOG_ERROR( "default", "[ %s : %d ][ %s ] Account %d Inc YB %d Money %d Tran OK, Code ERROR %d", __LK_FILE__, 
			__LINE__, __FUNCTION__, tpDec->accountid( ), tpDec->yb( ),  tpDec->money( ), vResultCode );
		ExchangeAccountLogUnSend( );
      //  return SUCCESS;
	}

	if( tTranRtn == SUCCESS && vResultCode == SUCCESS )
	{
		int tParameLog[ 4 ] = { 0 };
		tParameLog[ 0 ] = tpDec->money( );
		tParameLog[ 1 ] = tpDec->yb( );
		SetExchangeAccountLog( 0, tpDec->accountid( ), tpDec->roleid( ), EXCHANGE_ACCOUNT_DEC, tParameLog );
		ExchangeAccountLogSend( &pMsg->msghead( ) );
	}

	// 添加交易所帐号成功, 同步Client 数据
	if( GetBourseAccount( tpDec->sessionid( ), tpDec->accountid( ), BOURSE_ACCOUNT_DEC, &pMsg->msghead( ), vResultCode,
			tpDec->money( ), tpDec->yb( ) ) != SUCCESS )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] Inc Account %d, Money %d, Yb %d OK, But Get Account %d Faild",
			__LK_FILE__, __LINE__, __FUNCTION__, tpDec->accountid( ), tpDec->money( ), tpDec->yb( ),
			tpDec->accountid( ) );

		return SUCCESS;
	}
	
	return SUCCESS;

}

int CDBHandle::GetBoureseAccountDB( int vAccount, int* vpMoney, int* vpYB )
{
	char tSql[ 256 ] = { 0 };
	int nRow = 0;
	int nColumn = 0;
	Field* pField = NULL;
	int tRtn = SUCCESS;

	*vpMoney = 0;
	*vpYB = 0;

	snprintf( tSql, sizeof( tSql ), "SELECT currency_type, currency_value from UMS_BOURSE_ACCOUNT where"
		" account_id=%d and ( currency_type = 1 or currency_type = 2 )", vAccount );

	QueryResult* res = m_pDatabase->Query( tSql );
	if( res == NULL )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] Cannt Get Res", __LK_FILE__, __LINE__, __FUNCTION__ );
		tRtn = ERR_UNKNOWN;
		goto ERR_BACK;
	}

	nRow = res->GetRowCount() ;
	nColumn = res->GetFieldCount() ;

	pField = res->Fetch();
	if( pField == NULL )
	{
		TRACE_ERROR("[ %s : %d ][ %s ]ERROR: do sql %s success,row[%d], col[%d], but some row is null\n",
				__LK_FILE__, __LINE__, __FUNCTION__, tSql, nRow, nColumn );
		tRtn = ERR_NOT_FOUND;
		// 安全释放结果集
		goto ERR_BACK;
	}

	if( nRow == 2 )
	{

		for( int i = 0; i < nRow; i++, res->NextRow( ) )
		{
			pField = res->Fetch();
			if( pField == NULL )
			{
				TRACE_ERROR("[ %s : %d ][ %s ]ERROR: do sql %s success,row[%d], col[%d], but some row is null\n",
						__LK_FILE__, __LINE__, __FUNCTION__, tSql, nRow, nColumn );
				tRtn = ERR_NOT_FOUND;
				// 安全释放结果集
				goto ERR_BACK;;
			}

			if( atoi( pField[  0 ].GetString( ) ) == BOURSE_DB_YB_KEY )
			{
				*vpYB = pField[  1 ].GetInt32( );
			}
			else if( atoi( pField[  0 ].GetString( ) ) == BOURSE_DB_MONEY_KEY )
			{
				*vpMoney = pField[  1 ].GetInt32( );
			}
			else
			{
				LOG_ERROR( "default", "[ %s : %d ][ %s ] Cannt Get Account %d Key %d", __LK_FILE__, __LINE__,
						__FUNCTION__, vAccount, pField[ i * nColumn + 0 ].GetInt32( ) );
				tRtn = ERR_UNKNOWN;
				goto ERR_BACK;
			}
		}			
	}	
	else if( nRow == 0 )
	{
		tRtn = ERR_NOT_FOUND;
		goto ERR_BACK;
	}
	else
	{
		tRtn = ERR_UNKNOWN;
		LOG_ERROR( "default", "[ %s : %d ][ %s ] Get Account %d Key Count Error", __LK_FILE__, __LINE__,
				__FUNCTION__, vAccount );
		goto ERR_BACK;
	}

	goto FREE_RES;

ERR_BACK:
	*vpYB = 0;
	*vpMoney = 0;

FREE_RES:
	if( res != NULL )
	{
		delete res;
	}

	res = NULL;

	return tRtn;

}

bool CDBHandle:: DecAccountTran( int* vTranCode, int* vParame1, int* vParame2 )
{
	int vMoney = 0; 
	int vYB	= 0;

	int tAccountID = vParame1[ 0 ];
	int tDecMoney = vParame1[ 2 ];
	int tDecYb = vParame1[ 1 ];
    bool tAddYB = false;
    int tAddYBCount = 0;
    bool tAddMoney = false;
    int tAddMoneyCount = 0;
    bool tChangeYB = false;
    int tChangeYBCount = 0;
	
	if( GetBoureseAccountDB( tAccountID, &vMoney, &vYB )	!= SUCCESS )
	{
		*vTranCode = ERR_NOT_FOUND;
		return false;
	}

	if( tDecMoney > vMoney )
	{
		*vTranCode = BOURSE_DEC_NOTENOUGH_MONEY;
		return false;		
	}

	if( tDecYb > vYB )
	{
		*vTranCode = BOURSE_DEC_NOTENOUGH_YB;
		return false;
	}

	char tSql[ 256 ] = { 0 };

    snprintf( tSql, sizeof( tSql ), "UPDATE UMS_BOURSE_ACCOUNT SET currency_value = currency_value - %d where "
        " account_id=%d and currency_type = %d AND currency_value>= %d", tDecYb, tAccountID, BOURSE_DB_YB_KEY, tDecYb );

    tAddYB = m_pDatabase->DirectExecute( tSql );
    tAddYBCount = m_pDatabase->AffectedRows( );

	if( ( tAddYB == false || tAddYBCount == 0 ) && tDecYb != 0 )
	{
		*vTranCode = BOURSE_DEC_NOTENOUGH_YB;
		return false;
	}

    snprintf( tSql, sizeof( tSql ), "UPDATE UMS_BOURSE_ACCOUNT SET currency_value = currency_value - %d where "
        " account_id=%d and currency_type = %d AND currency_value>=%d", tDecMoney, tAccountID, BOURSE_DB_MONEY_KEY,
		tDecMoney );

    tAddMoney = m_pDatabase->DirectExecute( tSql );
    tAddMoneyCount = m_pDatabase->AffectedRows( );

	if( ( tAddMoney == false || tAddMoneyCount == 0 ) && tDecMoney != 0 )
	{
		*vTranCode = BOURSE_DEC_NOTENOUGH_YB;
		return false;
	}

    snprintf( tSql, sizeof( tSql ), "UPDATE UMS_ACCOUNT  set golden_yb=golden_yb+%d where account_id=%d ", 
		tDecYb, tAccountID );
    tChangeYB = m_pDatabase->DirectExecute( tSql );
    tChangeYBCount = m_pDatabase->AffectedRows( ); 
	if( ( tChangeYB == false || tChangeYBCount == 0 ) && tDecYb != 0 )
	{
		*vTranCode = BOURSE_DEC_NOTENOUGH_YB;
		return false;
	}

	*vTranCode = SUCCESS;

	return true;
}

int CDBHandle::ProcessBourseAddDoc( CMessage* vpMsg )
{
	CMessageBourseAddDoc* tpAddDoc = ( CMessageBourseAddDoc* )vpMsg->msgpara( );
	PBBourseDetail* tpDetail = tpAddDoc->mutable_detail( );
	int tRtn = SUCCESS;
	int tResultCode = 0;
	int tAccountID = tpAddDoc->accountid( );

	int vParame[ ] = { tpAddDoc->accountid( ), tpAddDoc->roleid( ), tpAddDoc->tax( ), tpAddDoc->level( ),
		tpAddDoc->worldid( ), tpAddDoc->serverid( ), tpAddDoc->taxid( ), tpAddDoc->gameid( ) };
	char tSql[ 256 ] = { 0 };
	PBBourseDetailList tDetail;


	if( tpAddDoc->type( ) == 1 )
	{
		// buy
		snprintf( tSql, sizeof( tSql ), "select bourse_doc_no, account_id, role_id, price, yb_count, bourse_date, "
			" bourse_trade_detail from UMS_BOURSE_TRADE where trade_kind = %d and account_id = %d", BOURSE_TRADE_BUY,
			tpAddDoc->accountid( ) );
		BourseQueryInsertDetail( &tDetail, tSql );	
		if( tDetail.mutable_list( )->size( ) < 15 )
		{
			tRtn = TranDoingEvent( &CDBHandle::AddBuyDoc, &tResultCode, vParame, ( int* )tpDetail );
		}
		else
		{
			tResultCode = BOURSE_BUY_LIST_MORE;
		}
	}
	else if( tpAddDoc->type( ) == 2 )
	{
		// sell
		snprintf( tSql, sizeof( tSql ), "select bourse_doc_no, account_id, role_id, price, yb_count, bourse_date, "
			" bourse_trade_detail from UMS_BOURSE_TRADE where trade_kind = %d and account_id = %d", BOURSE_TRADE_SELL,
			tpAddDoc->accountid( ) );
		BourseQueryInsertDetail( &tDetail, tSql );	
		if( tDetail.mutable_list( )->size( ) < 15 )
		{
			tRtn = TranDoingEvent( &CDBHandle::AddSellDoc, &tResultCode, vParame, ( int* )tpDetail );
		}
		else
		{
			tResultCode = BOURSE_SELL_LIST_MORE;
		}
	}

	if( tRtn != SUCCESS )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] AddDoc Account %d, Price %d, YB %d Type %s, Tran Error, ResultCode %d",
 			__LK_FILE__, __LINE__, __FUNCTION__, tAccountID, tpDetail->price( ), tpDetail->yb_count( ), 
			( tpAddDoc->type( ) == 1 ) ? "buy" : "sell", tResultCode );

		ExchangeAccountLogUnSend( );
	}

	if( tResultCode != SUCCESS )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] AddDoc Account %d, Price %d, YB %d Type %s, Tran OK, ResultCode %d",
 			__LK_FILE__, __LINE__, __FUNCTION__, tAccountID, tpDetail->price( ), tpDetail->yb_count( ), 
			( tpAddDoc->type( ) == 1 ) ? "buy" : "sell", tResultCode );

		ExchangeAccountLogUnSend( );
	}

	ExchangeAccountLogSend( &vpMsg->msghead( ) );

	GetBourseAccount( tpAddDoc->sessionid( ), tAccountID, BOURSE_ADD_DOC_BUY, &vpMsg->msghead( ), tResultCode, 0, 0 );

	if( tResultCode == SUCCESS && tRtn == SUCCESS )
	{
		BoursePlayerListGet( tAccountID, &vpMsg->msghead( ) );	
	}

	return SUCCESS;
}

bool CDBHandle::AddBuyDoc( int* vTranCode, int* vParame1, int* vParame2 )
{
	char tSql[ 640 ] = { 0 };	

	PBBourseDetail* tpDetail = (  PBBourseDetail* )vParame2;
	int tAccountID = vParame1[ 0 ];	
	int tRoleID	= vParame1[ 1 ];
	int tTax = vParame1[ 2 ];
	int tLevel = vParame1[ 3 ];
	int tWorldID = vParame1[ 4 ];
	int tServerID = vParame1[ 5 ];
	int tTaxID	= vParame1[ 6 ];
	int tGameID = vParame1[ 7 ];

	int tPriceAll = tpDetail->price( ) * tpDetail->yb_count( );

	bool tChangeAccount = false;
	int tChangeAccountCount = 0;
	bool tInsert = false;
	int tInsertCount = 0;

	unsigned int tDoc = 0;
	
	int tTime = time( NULL );
	long tLastInsertID = 0;

	if( tPriceAll > BOURSE_EXCHANGE_AMOUNT_MAX )
	{
		*vTranCode = BOURSE_SUM_MONEY_MUCH;
		return false;
	}


	snprintf( tSql, sizeof( tSql ), "UPDATE UMS_BOURSE_ACCOUNT set currency_value=currency_value-%d "
		" WHERE account_id =%d AND currency_type = %d AND currency_value>=%d", 
		tPriceAll + tTax, tAccountID, BOURSE_DB_MONEY_KEY, tPriceAll + tTax );

    tChangeAccount = m_pDatabase->DirectExecute( tSql );
    tChangeAccountCount = m_pDatabase->AffectedRows( );
	if( tChangeAccount == false || tChangeAccountCount != 1 )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] %d, %d, SQL: %s", __LK_FILE__, __LINE__, __FUNCTION__, 
			( tChangeAccount == true ) ? 1 : 0, tChangeAccountCount, tSql );
		*vTranCode = BOURSE_DEC_NOTENOUGH_MONEY;
		return false;
	}

	snprintf( tSql, sizeof( tSql ), "INSERT INTO UMS_BOURSE_TRADE(  account_id, price, yb_count, "
		" trade_type, trade_kind, bourse_date, bourse_tax, role_id, account_game_id ) VALUES( %d, %d, %d, %d, %d,"
		"  %d, %d, %d, %d )", tAccountID, tpDetail->price( ), tpDetail->yb_count( ), BOURSE_TRADE_ING, 
		BOURSE_TRADE_BUY, tTime, tTax, tRoleID, tGameID );

	tInsert = m_pDatabase->DirectExecute( tSql );
	tInsertCount = m_pDatabase->AffectedRows( );

	if( tInsert == false || tInsertCount != 1 )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] %d, %d, SQL: %s", __LK_FILE__, __LINE__, __FUNCTION__, 
			( tInsert == true ) ? 1 : 0, tInsertCount, tSql );
		*vTranCode = ERR_UNKNOWN;
		return false;
	}	

	tLastInsertID = m_pDatabase->GetLastInsertID( );	

//	snprintf( tSql, sizeof( tSql ), "UPDATE UMS_BOURSE_TRADE SET bourse_trade_detail = LAST_INSERT_ID( ) * 1000000 + %d"
//		" WHERE bourse_doc_no = LAST_INSERT_ID( )", tWorldID );
	snprintf( tSql, sizeof( tSql ), "UPDATE UMS_BOURSE_TRADE SET bourse_trade_detail = %d * 1000000 + %d"
		" WHERE bourse_doc_no = %d", tLastInsertID, tWorldID, tLastInsertID );

	tInsert = m_pDatabase->DirectExecute( tSql );
    tInsertCount = m_pDatabase->AffectedRows( );
	if( tInsert == false || tInsertCount != 1 )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] %d, %d, SQL: %s", __LK_FILE__, __LINE__, __FUNCTION__, 
			( tInsert == true ) ? 1 : 0, tInsertCount, tSql );
		*vTranCode = ERR_UNKNOWN;
		return false;
	}
	
	int tParameLog[ 4 ] = { 0 };
	tParameLog[ 0 ] = tpDetail->price( );
	tParameLog[ 1 ] = tpDetail->yb_count( );
	tParameLog[ 2 ] = tTax;
	SetExchangeAccountLog( 0, tAccountID, tRoleID, EXCHANGE_TRADE_BUY_ADD, tParameLog, 
		tLastInsertID * 1000000 + tWorldID );

	snprintf( tSql, sizeof( tSql ), "INSERT INTO UMS_BOURSE_TRADE_HISTORY( bourse_trade_detail, account_id, price, "
		" yb_count, trade_kind, trade_type, bourse_date, bourse_tax, role_id, account_game_id ) SELECT "
		" bourse_trade_detail, account_id, price, yb_count, trade_kind, trade_type, bourse_date, bourse_tax, "
		" role_id, account_game_id FROM UMS_BOURSE_TRADE WHERE bourse_doc_no = LAST_INSERT_ID( )" );
    tInsert = m_pDatabase->DirectExecute( tSql );
    tInsertCount = m_pDatabase->AffectedRows( ); 
	if( tInsert == false || tInsertCount != 1 )
    {
        LOG_ERROR( "default", "[ %s : %d ][ %s ] %d, %d, SQL: %s", __LK_FILE__, __LINE__, __FUNCTION__,
            ( tInsert == true ) ? 1 : 0, tInsertCount, tSql );
        *vTranCode = ERR_UNKNOWN;
        return false;
    }

	return true;
}

bool CDBHandle::AddSellDoc( int* vTranCode, int* vParame1, int* vParame2 )
{
	char tSql[ 640 ] = { 0 };	

	PBBourseDetail* tpDetail = (  PBBourseDetail* )vParame2;
	int tAccountID = vParame1[ 0 ];	
	int tRoleID	= vParame1[ 1 ];
	int tTax = vParame1[ 2 ];
	int tLevel = vParame1[ 3 ];
	int tWorldID = vParame1[ 4 ];
	int tServerID = vParame1[ 5 ];
	int tTaxID = vParame1[ 6 ];
	int tGameID = vParame1[ 7 ];

	int tYBCount = tpDetail->yb_count( );

	bool tChangeAccount = false;
	int tChangeAccountCount = 0;
	bool tInsert = false;
	int tInsertCount = 0;


	if( tYBCount > 100000000 )
	{
		*vTranCode = BOURSE_SUM_YB_MUCH;
		return false;
	}
	
	int tTime = time( NULL );


	snprintf( tSql, sizeof( tSql ), "UPDATE UMS_BOURSE_ACCOUNT set currency_value=currency_value-%d "
		/*" currency_lock=currency_lock+%d*/" WHERE account_id =%d AND currency_type = %d AND currency_value>=%d", 
		tYBCount + tTax, /*tYBCount,*/ tAccountID, BOURSE_DB_YB_KEY, tYBCount + tTax );

    tChangeAccount = m_pDatabase->DirectExecute( tSql );
    tChangeAccountCount = m_pDatabase->AffectedRows( );

	if( tChangeAccount == false || tChangeAccountCount != 1 )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] %d, %d, SQL: %s", __LK_FILE__, __LINE__, __FUNCTION__, 
			( tChangeAccount == true ) ? 1 : 0, tChangeAccountCount, tSql );
		*vTranCode = BOURSE_DEC_NOTENOUGH_YB;
		return false;
	}

	snprintf( tSql, sizeof( tSql ), "INSERT INTO UMS_BOURSE_TRADE( account_id, price, yb_count, "
		" trade_type, trade_kind, bourse_date, bourse_tax, role_id, account_game_id ) VALUES( %d, %d, %d, %d, %d, %d,"
		" %d, %d, %d )", tAccountID, tpDetail->price( ), tpDetail->yb_count( ), BOURSE_TRADE_ING, BOURSE_TRADE_SELL, 
		tTime, tTax, tRoleID, tGameID );

	tInsert = m_pDatabase->DirectExecute( tSql );
	tInsertCount = m_pDatabase->AffectedRows( );

	if( tInsert == false || tInsertCount != 1 )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] %d, %d, SQL: %s", __LK_FILE__, __LINE__, __FUNCTION__, 
			( tInsert == true ) ? 1 : 0, tInsertCount, tSql );
		*vTranCode = ERR_UNKNOWN;
		return false;
	}	

	long tLastInsertID = m_pDatabase->GetLastInsertID( );

//	snprintf( tSql, sizeof( tSql ), "UPDATE UMS_BOURSE_TRADE SET bourse_trade_detail = LAST_INSERT_ID( ) * 1000000 + %d"
	snprintf( tSql, sizeof( tSql ), "UPDATE UMS_BOURSE_TRADE SET bourse_trade_detail = %d * 1000000 + %d"
		" WHERE bourse_doc_no = %d", tLastInsertID, tWorldID, tLastInsertID );
	tInsert = m_pDatabase->DirectExecute( tSql );
    tInsertCount = m_pDatabase->AffectedRows( );
	if( tInsert == false || tInsertCount != 1 )
	{
		LOG_ERROR( "default", "[ %s : %d ][ %s ] %d, %d, SQL: %s", __LK_FILE__, __LINE__, __FUNCTION__, 
			( tInsert == true ) ? 1 : 0, tInsertCount, tSql );
		*vTranCode = ERR_UNKNOWN;
		return false;
	}

	int tParameLog[ 4 ] = { 0 };
	tParameLog[ 0 ] = tpDetail->price( );
	tParameLog[ 1 ] = tpDetail->yb_count( );
	tParameLog[ 2 ] = tTax;
	SetExchangeAccountLog( 0, tAccountID, tRoleID, EXCHANGE_TRADE_SELL_ADD, tParameLog, 
		tLastInsertID * 1000000 + tWorldID );


	snprintf( tSql, sizeof( tSql ), "INSERT INTO UMS_BOURSE_TRADE_HISTORY( bourse_trade_detail, account_id, price, "
		" yb_count, trade_kind, trade_type, bourse_date, bourse_tax, role_id, account_game_id ) SELECT "
		" bourse_trade_detail, account_id, price, yb_count, trade_kind, trade_type, bourse_date, bourse_tax, "
		" role_id, account_game_id FROM UMS_BOURSE_TRADE WHERE bourse_doc_no = LAST_INSERT_ID( )" );
    tInsert = m_pDatabase->DirectExecute( tSql );
    tInsertCount = m_pDatabase->AffectedRows( ); 
	if( tInsert == false || tInsertCount != 1 )
    {
        LOG_ERROR( "default", "[ %s : %d ][ %s ] %d, %d, SQL: %s", __LK_FILE__, __LINE__, __FUNCTION__,
            ( tInsert == true ) ? 1 : 0, tInsertCount, tSql );
        *vTranCode = ERR_UNKNOWN;
        return false;
    }


    snprintf( tSql, sizeof( tSql ), "INSERT INTO GMS_CONSUME_LOG ( account_id, role_id,  gender_id,  metier_id, "
		" role_level, ib_code, package_flag, item_count, pay_time,  user_ip, subject_id,  audit_flag,  price, "
		" discount_price, rating_id, game_id) VALUES ( %d, %d, 0, 0, %d, \'%d\', 1, %d,  UNIX_TIMESTAMP(), 0, 5 , 1,"
		" 1, 1, %d, %d )",  tAccountID, tRoleID, tLevel, tTaxID, tTax, tWorldID, tGameID );
	tInsert = m_pDatabase->DirectExecute( tSql );
    tInsertCount = m_pDatabase->AffectedRows( );
	if( tInsert == false || tInsertCount != 1 )
	{
		*vTranCode = ERR_UNKNOWN;
		return false;
	}

	snprintf( tSql, sizeof( tSql ), "UPDATE GMS_CONSUME_LOG set detail_id = rating_id * 10000000000 + purchase_id"
		" where purchase_id = LAST_INSERT_ID( )" ); 
	tInsert = m_pDatabase->DirectExecute( tSql );
	tInsertCount = m_pDatabase->AffectedRows( );
	if( tInsert == false || tInsertCount != 1 )
    {
        *vTranCode = ERR_UNKNOWN;
        return false;
    }

	snprintf( tSql, sizeof( tSql ), "update UMS_ACCOUNT set consumed_golden_yb = consumed_golden_yb + %d where"
		" account_id = %d", tTax, tAccountID );
	tInsert = m_pDatabase->DirectExecute( tSql );
	tInsertCount = m_pDatabase->AffectedRows( );
    if( tInsert == false || tInsertCount != 1 )
    {
        *vTranCode = ERR_UNKNOWN;
        return false;
    }

	snprintf( tSql, sizeof( tSql ), "INSERT INTO GMS_CONSUME_LOCKED( purchase_id, account_id, role_id, gender_id,  "
		" metier_id, role_level,   ib_code, package_flag, item_count, pay_time,  user_ip, subject_id, audit_flag, "
		" price, discount_price, rating_id, game_id, detail_id ) SELECT purchase_id, account_id, role_id,  "
		" gender_id,  metier_id,  role_level,   ib_code, package_flag, item_count, pay_time,  user_ip, subject_id, "
		" audit_flag,  price, discount_price, rating_id, game_id, detail_id FROM GMS_CONSUME_LOG where "
		" purchase_id= LAST_INSERT_ID()" );

	tInsert = m_pDatabase->DirectExecute( tSql );
    tInsertCount = m_pDatabase->AffectedRows( );
	if( tInsert == false || tInsertCount != 1 )
	{
		*vTranCode = ERR_UNKNOWN;
		return false;
	}

	return true;
}

int CDBHandle::ProcessBoursePlayerListGet( CMessage* pMsg )
{
	CMessageBoursePlayerListGet* tpGet = ( CMessageBoursePlayerListGet* )pMsg->msgpara( );
	return BoursePlayerListGet( tpGet->accountid( ), &pMsg->msghead( ) );
}

int CDBHandle::BourseQueryInsertDetail( PBBourseDetailList* tpList, const char* tpSql )
{
	QueryResult* res = NULL;
	Field* pField = NULL;

	res = m_pDatabase->Query( tpSql );	
	
	if( res == NULL )
	{
		TRACE_ERROR( "sql %s exec, but no resultset returned", tpSql );
		goto FUN_END;
	}

	if( res->GetRowCount( ) == 0 )
	{
		goto FREE_RES;
	}

	pField = res->Fetch();
	if( pField == NULL )
	{
		TRACE_ERROR( "sql %s exec, cannt get res field", tpSql );
		goto FREE_RES;
	}	

	if( res->GetFieldCount( ) != 7 )
	{
		TRACE_ERROR( "sql %s exec, col num err %d", tpSql, res->GetFieldCount( ) );
		goto FREE_RES;
	}
	
	for( int i = 0; i < res->GetRowCount( ); i++, res->NextRow( ) )
	{
		Field* pField = res->Fetch();
		if( pField == NULL )
		{
			break;
		}

		PBBourseDetail* tpDetail = tpList->add_list( );		
		if( tpDetail == NULL )
		{
			break;
		}
		tpDetail->set_bourse_doc_no( atoi( pField[ 0 ].GetString( ) ) );
		tpDetail->set_account_id( atoi( pField[ 1 ].GetString( ) ) );
		tpDetail->set_role_id( atoi( pField[ 2 ].GetString( ) ) );
		tpDetail->set_price( atoi( pField[ 3 ].GetString( ) ) );
		tpDetail->set_yb_count( atoi( pField[ 4 ].GetString( ) ) );
		tpDetail->set_bourse_date( atoi( pField[ 5 ].GetString( ) ) );
		tpDetail->set_bourse_detail( atoll( pField[ 6 ].GetString( ) ) );
	}

FREE_RES:
	if( res != NULL )
	{
		delete res;
	}

	res = NULL;

FUN_END:
	return 0;
}

int CDBHandle::ProcessBourseTrade( CMessage* vpMsg )
{
	CMessageBourseTrade* tpTrade = ( CMessageBourseTrade* )vpMsg->msgpara( );

	int tPrice[] = { tpTrade->buyprice( ), tpTrade->sellprice( ) };
	int tParame2[ 6 ] = { 0 };
	int vResultCode = 0;

	
	PBBourseDetailList tBuyList;
	PBBourseDetailList tSellList;

	char tSql[ 1024 ] = { 0 };

	bool tExecSql = false;
	int tExecSqlCount = 0;

	snprintf( tSql, sizeof( tSql ), "select bourse_doc_no, account_id, role_id, price, yb_count - trade_yb, "
		" bourse_date, bourse_trade_detail from UMS_BOURSE_TRADE where price = %d and trade_type = %d and "
		" yb_count - trade_yb != 0 and trade_kind = %d order by yb_count desc, bourse_date limit 0, 1", 
		tpTrade->buyprice( ), BOURSE_TRADE_ING, BOURSE_TRADE_BUY );

	BourseQueryInsertDetail( &tBuyList, tSql );

	PBBourseDetail* tBuyDetail = NULL;
	PBBourseDetail* tSellDetail = NULL;

	snprintf( tSql, sizeof( tSql ), "select bourse_doc_no, account_id, role_id, price, yb_count - trade_yb, "
		" bourse_date, bourse_trade_detail from UMS_BOURSE_TRADE where price = %d and trade_type = %d and "
		" yb_count - trade_yb != 0 and trade_kind = %d order by yb_count desc, bourse_date limit 0, 1", 
		tpTrade->sellprice( ), BOURSE_TRADE_ING, BOURSE_TRADE_SELL );

	BourseQueryInsertDetail( &tSellList, tSql );

	if( tBuyList.list_size( ) > 0 )
	{
		tBuyDetail = tBuyList.mutable_list( 0 );
	}

	if( tSellList.list_size( ) > 0 )
	{
		tSellDetail = tSellList.mutable_list( 0 );
	}

	if( tBuyDetail == NULL || tSellDetail == NULL )
	{
		return ERR_UNKNOWN;
	}

	tParame2[ 0 ] = (int)tBuyDetail;
	tParame2[ 1 ] = (int)tSellDetail;
	tParame2[ 2 ] = tpTrade->rating_id( );
	tParame2[ 3 ] = tpTrade->worldid( );

	if( TranDoingEvent( &CDBHandle::BourseTradeTran, &vResultCode, tPrice, tParame2 ) != SUCCESS )
	{
		ExchangeAccountLogUnSend( );
		return ERR_UNKNOWN;
	}

	if( vResultCode != SUCCESS )
	{
		ExchangeAccountLogUnSend( );
		return ERR_UNKNOWN;
	}

	int tRealCount = tParame2[ 5 ];

//	PBBourseDetail* tBuyDetail = tBuyList.mutable_list( 0 );
//	PBBourseDetail* tSellDetail = tSellList.mutable_list( 0 );

	ExchangeAccountLogSend( &vpMsg->msghead() );


	if( tBuyDetail == NULL || tSellDetail == NULL )
	{
		return false;
	}

	
	tBuyDetail->set_yb_count( tRealCount );
	tSellDetail->set_yb_count( tRealCount );

	CMessage tMsg;	
	CMessageBourseTradeNotice tTradeNotice;

	tTradeNotice.mutable_buydetail( )->CopyFrom( *tBuyDetail );
	tTradeNotice.mutable_selldetail( )->CopyFrom( *tSellDetail );

	const CMessageHead* tpHead = &vpMsg->msghead();

    tMsg.mutable_msghead()->set_messageid( ID_D2G_BOURSE_TRADE_NOTICE );
    tMsg.mutable_msghead()->set_srcfe( FE_DATASERVER );
    tMsg.mutable_msghead()->set_srcid( tpHead->dstid() );
    tMsg.mutable_msghead()->set_dstfe( tpHead->srcfe() );
    tMsg.mutable_msghead()->set_dstid( tpHead->srcid() );

    tMsg.set_msgpara( (unsigned int)(&tTradeNotice) );

    SendMessageTo( &tMsg, m_stCurrentProxyHead.srcfe(), m_stCurrentProxyHead.srcid(), m_stCurrentProxyHead.uin() );

	return SUCCESS;	
}

bool CDBHandle::BourseTradeTran( int* vTranCode, int* vPrice, int* vParame2 )
{
	int tBuyPrice = vPrice[ 0 ];
	int tSellPrice = vPrice[ 1 ];

	char tSql[ 640 ] = { 0 };

	bool tExecSql = false;
	int tExecSqlCount = 0;

//	PBBourseDetailList* tpBuyList = ( PBBourseDetailList* )vParame2[ 0 ];
//	PBBourseDetailList* tpSellList = ( PBBourseDetailList* )vParame2[ 1 ];
	int tRatingID = vParame2[ 2 ];
	int tWorldID = vParame2[ 3 ];

	const PBBourseDetail* tBuyDetail = ( const PBBourseDetail* )vParame2[ 0 ];	//&tpBuyList->list( 0 );
	const PBBourseDetail* tSellDetail = ( const PBBourseDetail* )vParame2[ 1 ]; //&tpSellList->list( 0 );
	
	if( tBuyDetail == NULL || tSellDetail == NULL )
	{
		*vTranCode = ERR_UNKNOWN;
		return false;
	}

	int tRealYbCount = ( tBuyDetail->yb_count( ) >= tSellDetail->yb_count( ) ) ? tSellDetail->yb_count( ) : 						tBuyDetail->yb_count( );
	
	snprintf( tSql, sizeof( tSql ), "update UMS_BOURSE_TRADE set trade_yb = trade_yb + %d where bourse_doc_no=%d and "
		" yb_count - trade_yb >= %d and trade_type = %d and trade_kind = %d", tRealYbCount, 
		tBuyDetail->bourse_doc_no( ), tRealYbCount, BOURSE_TRADE_ING, BOURSE_TRADE_BUY ); 
	
	tExecSql = m_pDatabase->DirectExecute( tSql );
	tExecSqlCount = m_pDatabase->AffectedRows( );

	if( tExecSql == false || tExecSqlCount == 0 )
	{
		*vTranCode = ERR_UNKNOWN;
		return false;
	}

	snprintf( tSql, sizeof( tSql ), "update UMS_BOURSE_TRADE set trade_yb = trade_yb + %d where bourse_doc_no=%d and "
		" yb_count - trade_yb >= %d and trade_type = %d and trade_kind = %d", tRealYbCount, 
		tSellDetail->bourse_doc_no( ), tRealYbCount, BOURSE_TRADE_ING, BOURSE_TRADE_SELL );

	tExecSql = m_pDatabase->DirectExecute( tSql );
	tExecSqlCount = m_pDatabase->AffectedRows( );

	if( tExecSql == false || tExecSqlCount == 0 )
	{
		*vTranCode = ERR_UNKNOWN;
		return false;
	}

	// 卖家获得金钱
	snprintf( tSql, sizeof( tSql ), "update UMS_BOURSE_ACCOUNT set currency_value = currency_value + %d where "
		" account_id = %d and currency_type = %d and currency_value + %d <= 2100000000", 
		tSellDetail->price( ) * tRealYbCount, tSellDetail->account_id( ), BOURSE_DB_MONEY_KEY, 
		tSellDetail->price( ) * tRealYbCount );

	tExecSql = m_pDatabase->DirectExecute( tSql );
    tExecSqlCount = m_pDatabase->AffectedRows( );
	if( tExecSql == false || tExecSqlCount == 0 )
	{
		if( tExecSql == false )
		{
			*vTranCode = ERR_UNKNOWN;
		}
		else if( tExecSqlCount == 0 )
		{
			*vTranCode = BOURSE_ACCOUNT_MONEY_MUCH;
		}
	
		return false;
	}

	snprintf( tSql, sizeof( tSql ), "update UMS_BOURSE_TRADE set trade_type = %d where trade_type = %d and"
		" bourse_doc_no=%d and yb_count = trade_yb and trade_kind = %d", BOURSE_TRADE_END, BOURSE_TRADE_ING, 
		tBuyDetail->bourse_doc_no( ), BOURSE_TRADE_BUY );
	tExecSql = m_pDatabase->DirectExecute( tSql );
	if( tExecSql == false )
	{
		*vTranCode = ERR_UNKNOWN;
		return false;
	}

	snprintf( tSql, sizeof( tSql ), "update UMS_BOURSE_TRADE set trade_type = %d where trade_type = %d and"
	 " bourse_doc_no=%d and yb_count = trade_yb and trade_kind = %d", BOURSE_TRADE_END, BOURSE_TRADE_ING, 
	tSellDetail->bourse_doc_no( ), BOURSE_TRADE_SELL );
	tExecSql = m_pDatabase->DirectExecute( tSql );
	if( tExecSql == false )
	{
		*vTranCode = ERR_UNKNOWN;
		return false;
	}

	snprintf( tSql, sizeof( tSql ), "update UMS_BOURSE_ACCOUNT set currency_value = currency_value + %d where "
		" account_id = %d and currency_type = %d and currency_value +  %d <= 2100000000", 
		tRealYbCount, tBuyDetail->account_id( ), BOURSE_DB_YB_KEY, tRealYbCount );

	tExecSql = m_pDatabase->DirectExecute( tSql );
    tExecSqlCount = m_pDatabase->AffectedRows( );
	if( tExecSql == false || tExecSqlCount == 0 )
	{
		*vTranCode = ERR_UNKNOWN;	
	
		return false;
	}

	snprintf( tSql, sizeof( tSql ), "insert into UMS_BOURSE_LOG( bourse_buy_detail, bourse_sell_detail,"
		" yb_count, bourse_date, buy_account, buy_price, sell_account, sell_price, rating_id, world_id, sell_role,"
		" buy_role, buy_account_game_id, sell_account_game_id ) values( %lld, %lld, %d, %d, %d, %d, %d, %d, %d, "
		" %d, %d, %d, ( SELECT account_game_id FROM UMS_BOURSE_TRADE where bourse_trade_detail= %lld ), "
		"( SELECT account_game_id FROM UMS_BOURSE_TRADE where bourse_trade_detail= %lld ) )", 
		tBuyDetail->bourse_detail( ), tSellDetail->bourse_detail( ), tRealYbCount, time( NULL ), 
		tBuyDetail->account_id( ), tBuyDetail->price( ), tSellDetail->account_id( ), tSellDetail->price( ), 
		tRatingID, tWorldID, tSellDetail->role_id( ), tBuyDetail->role_id( ), tBuyDetail->bourse_detail( ), 
		tSellDetail->bourse_detail( )  );

	tExecSql = m_pDatabase->DirectExecute( tSql );
    tExecSqlCount = m_pDatabase->AffectedRows( );
	if( tExecSql == false || tExecSqlCount == 0 )
	{
		*vTranCode = ERR_UNKNOWN;	
	
		return false;
	}

	

	long tLastInsertID = 0;
	tLastInsertID = m_pDatabase->GetLastInsertID( );

//	snprintf( tSql, sizeof( tSql ), "UPDATE UMS_BOURSE_LOG set buy_account_game_id = ( SELECT account_game_id FROm"
//		" UMS_BOURSE_TRADE where bourse_detail= %lld ), sell_account_game_id = ( SELECT account_game_id FROm"
//		" UMS_BOURSE_TRADE where bourse_detail= %lld ) where bourse_buy_detail = %lld and bourse_sell_detail = %lld",
//		tBuyDetail->bourse_detail( ), tSellDetail->bourse_detail( ), tBuyDetail->bourse_detail( ), 
//		tSellDetail->bourse_detail( ) );
//	tExecSql = m_pDatabase->DirectExecute( tSql );
  //  tExecSqlCount = m_pDatabase->AffectedRows( );
//    if( tExecSql == false || tExecSqlCount == 0 )
//    {
//        *vTranCode = ERR_UNKNOWN;   
    
//        return false;
//    }

	if( tSellDetail->account_id( ) != tBuyDetail->account_id( ) )
	{
		snprintf( tSql, sizeof( tSql ), "insert into UMS_BOURSE_LOG_LOCKED( bourse_detail, sell_account, buy_account, "
				" yb_count, rating_id, world_id, bourse_date, buy_role, sell_role, buy_account_game_id, "
				" sell_account_game_id ) select bourse_detail, sell_account, buy_account, yb_count, rating_id, "
				" world_id, bourse_date, buy_role, sell_role, buy_account_game_id, sell_account_game_id from"
				" UMS_BOURSE_LOG where  bourse_detail = %d", tLastInsertID );

		tExecSql = m_pDatabase->DirectExecute( tSql );
		tExecSqlCount = m_pDatabase->AffectedRows( );
		if( tExecSql == false || tExecSqlCount == 0 )
		{
			*vTranCode = ERR_UNKNOWN;	

			return false;
		}
	}

	int tParame[ 4 ] = { 0 };
	tParame[ 0 ] = tBuyDetail->account_id( );
	tParame[ 1 ] = tBuyDetail->role_id( );
	tParame[ 2 ] = tRealYbCount;
	tParame[ 3 ] = tSellDetail->price( );
	SetExchangeAccountLog( 0, tSellDetail->account_id( ), tSellDetail->role_id( ), EXCHANGE_TRADE_LOG_SELL,
		tParame, tBuyDetail->bourse_detail( ), tSellDetail->bourse_detail( ) );
	tParame[ 0 ] = tSellDetail->account_id( );
	tParame[ 1 ] = tSellDetail->role_id( );
	SetExchangeAccountLog( 1, tBuyDetail->account_id( ), tBuyDetail->role_id( ), EXCHANGE_TRADE_LOG_BUY,
		tParame, tBuyDetail->bourse_detail( ), tSellDetail->bourse_detail( ) );

	tExecSql = BourseDocHistory( tSellDetail->bourse_doc_no( ), "UMS_BOURSE_TRADE" );
	if( tExecSql == false )
	{
		*vTranCode = ERR_UNKNOWN;
		return false;
	}
	tExecSql = BourseDocHistory( tBuyDetail->bourse_doc_no( ), "UMS_BOURSE_TRADE" );
	if( tExecSql == false )
	{
		*vTranCode = ERR_UNKNOWN;
		return false;
	}


	vParame2[ 5 ] = tRealYbCount;

	return true;
}

int CDBHandle::ProcessBourseDecBuyDoc( CMessage* pMsg )
{
	CMessageS2DBourseDecBuyDoc* tpDoc = ( CMessageS2DBourseDecBuyDoc* )pMsg->msgpara( );
	int tDoc = tpDoc->doc( );
	int tAccountID = tpDoc->accountid( );

	int vResultCode = SUCCESS;

	char tSql[ 256 ] = { 0 };

	snprintf( tSql, sizeof( tSql ), "select bourse_doc_no, account_id, role_id, price, yb_count - trade_yb, "
		"bourse_date, bourse_trade_detail from UMS_BOURSE_TRADE where bourse_doc_no = %d and trade_type = %d and "
		"yb_count - trade_yb != 0 and account_id= %d and trade_kind = %d", tDoc, BOURSE_TRADE_ING, tAccountID, 
		BOURSE_TRADE_BUY );

	PBBourseDetailList tBuyList;
	BourseQueryInsertDetail( &tBuyList, tSql );

	if( tBuyList.list_size( ) == 0 )
	{
		return ERR_UNKNOWN;
	}
	else if( tBuyList.list_size( ) != 1 )
	{
		return ERR_UNKNOWN;
	}

	PBBourseDetail* tpDetail = tBuyList.mutable_list( 0 );
	if( tpDetail == NULL )
	{
		return ERR_UNKNOWN;
	}

	int tRtn = TranDoingEvent( &CDBHandle::DecBuyDoc, &vResultCode, ( int* )tpDetail, &tAccountID );

	if( tRtn == SUCCESS || vResultCode == SUCCESS )
	{
		int tParame[ 4 ] = { 0 };
		tParame[ 0 ] = tpDetail->price( );
		tParame[ 1 ] = tpDetail->yb_count( );
		SetExchangeAccountLog( 0, tAccountID, tpDoc->roleid( ), EXCHANGE_TRADE_BUY_DEC, tParame,
				tpDetail->bourse_detail( ) );
		ExchangeAccountLogSend( &pMsg->msghead( ) ); 
	}

	GetBourseAccount( 0, tAccountID, BOURSE_DEC_DOC_BUY, &pMsg->msghead( ), vResultCode, tpDetail->price( ), 
		tpDetail->yb_count( ) );
	BoursePlayerListGet( tAccountID, &pMsg->msghead( ) );

	return SUCCESS;
}

int CDBHandle::ProcessBourseDecSellDoc( CMessage* pMsg )
{
	CMessageS2DBourseDecSellDoc* tpDoc = ( CMessageS2DBourseDecSellDoc* )pMsg->msgpara( );
	int tDoc = tpDoc->doc( );
	int tAccountID = tpDoc->accountid( );

	int vResultCode = SUCCESS;

	char tSql[ 256 ] = { 0 };

	snprintf( tSql, sizeof( tSql ), "select bourse_doc_no, account_id, role_id, price, yb_count - trade_yb, "
		" bourse_date,  bourse_trade_detail from UMS_BOURSE_TRADE where bourse_doc_no = %d and trade_type = %d and"
		" yb_count - trade_yb != 0 and account_id = %d and trade_kind = %d", tDoc, BOURSE_TRADE_ING, tAccountID, 
		BOURSE_TRADE_SELL );

	PBBourseDetailList tSellList;

	BourseQueryInsertDetail( &tSellList, tSql );

	if( tSellList.list_size( ) == 0 )
	{
		return ERR_UNKNOWN;
	}
	else if( tSellList.list_size( ) != 1 )
	{
		return false;
	}

	PBBourseDetail* tpDetail = tSellList.mutable_list( 0 );
	if( tpDetail == NULL )
	{
		return ERR_UNKNOWN;
	}

	int tRtn = TranDoingEvent( &CDBHandle::DecSellDoc, &vResultCode, ( int* )tpDetail, &tAccountID );

	if( tRtn == SUCCESS || vResultCode == SUCCESS )
	{
		int tParame[ 4 ] = { 0 };
		tParame[ 0 ] = tpDetail->price( );
		tParame[ 1 ] = tpDetail->yb_count( );
		SetExchangeAccountLog( 0, tAccountID, tpDoc->roleid( ), EXCHANGE_TRADE_SELL_DEC, tParame,
				tpDetail->bourse_detail( ) );
		ExchangeAccountLogSend( &pMsg->msghead( ) ); 
	
	}

	ExchangeAccountLogSend( &pMsg->msghead( ) );

	GetBourseAccount( 0, tAccountID, BOURSE_DEC_DOC_SELL, &pMsg->msghead( ), vResultCode, tpDetail->price( ), 
		tpDetail->yb_count( ) );
	BoursePlayerListGet( tAccountID, &pMsg->msghead( ) );

	return SUCCESS;
}

bool CDBHandle::DecBuyDoc( int* vTranCode, int* vParame1, int* vParame2 )
{
	char tSql[ 256 ] = { 0 };
	int tAccountID = *vParame2;
	bool tExecSql = 0;
	int tExecSqlCount = 0;

	PBBourseDetail* tpDetail = ( PBBourseDetail* )vParame1;//tBuyList.mutable_list( 0 );
	if( tpDetail == NULL )
	{
		*vTranCode = BOURSE_CANNT_FIND_DOC;
		return false;
	}

	int tDoc = tpDetail->bourse_doc_no( );

	snprintf( tSql, sizeof( tSql ), "update UMS_BOURSE_TRADE set trade_type = %d where bourse_doc_no = %d", 
		BOURSE_TRADE_CLOSE, tDoc );
	tExecSql = m_pDatabase->DirectExecute( tSql );
    tExecSqlCount = m_pDatabase->AffectedRows( );
	if( tExecSql == false || tExecSqlCount == 0 )
	{
		*vTranCode = BOURSE_CANNT_FIND_DOC;		
		return false;
	}

	snprintf( tSql, sizeof( tSql ), "update UMS_BOURSE_ACCOUNT set currency_value = currency_value + %d where "
		" account_id = %d and currency_type = %d", tpDetail->price( ) * tpDetail->yb_count( ), tpDetail->account_id( ),
		BOURSE_DB_MONEY_KEY );

	tExecSql = m_pDatabase->DirectExecute( tSql );
    tExecSqlCount = m_pDatabase->AffectedRows( );
	if( tExecSql == false || tExecSqlCount == 0 )
	{
		*vTranCode = ERR_UNKNOWN;		
		return false;
	}

	tExecSql = BourseDocHistory( tpDetail->bourse_doc_no( ), "UMS_BOURSE_TRADE" );
	if( tExecSql == false )
	{
		*vTranCode = ERR_UNKNOWN;
		return false;
	}

	return true;
}

int CDBHandle::BoursePlayerListGet( int vAccountID, const CMessageHead* vpHead )
{
	if( vpHead == NULL )
	{
		return ERR_UNKNOWN;
	}

	CMessageBoursePlayerListNotice tNotice;
	char tSql[ 256 ] = { 0 };
	snprintf( tSql, sizeof( tSql ), "SELECT bourse_doc_no, account_id, role_id, price, yb_count - trade_yb, "
		" bourse_date, bourse_trade_detail FROM UMS_BOURSE_TRADE WHERE account_id=%d and yb_count - trade_yb != 0 and "
		" trade_type = %d and trade_kind = %d limit 0, 15", vAccountID, BOURSE_TRADE_ING, BOURSE_TRADE_BUY );

	BourseQueryInsertDetail( tNotice.mutable_buylist( ), tSql );

	snprintf( tSql, sizeof( tSql ), "SELECT bourse_doc_no, account_id, role_id, price, yb_count - trade_yb, "
		" bourse_date, bourse_trade_detail FROM UMS_BOURSE_TRADE WHERE account_id=%d and yb_count - trade_yb != 0  and "
		" trade_type = %d and trade_kind = %d limit 0, 15",  vAccountID, BOURSE_TRADE_ING, BOURSE_TRADE_SELL );

	BourseQueryInsertDetail( tNotice.mutable_selllist( ), tSql );

	tNotice.set_accountid( vAccountID );

	CMessage tMsg;
	const CMessageHead* tpHead = vpHead;

    tMsg.mutable_msghead()->set_messageid( ID_D2S_BOURSE_PLAYER_NOTICE );
    tMsg.mutable_msghead()->set_srcfe( FE_DATASERVER );
    tMsg.mutable_msghead()->set_srcid( tpHead->dstid() );
    tMsg.mutable_msghead()->set_dstfe( tpHead->srcfe() );
    tMsg.mutable_msghead()->set_dstid( tpHead->srcid() );

    tMsg.set_msgpara( (unsigned int)(&tNotice) );


    SendMessageTo( &tMsg, m_stCurrentProxyHead.srcfe(), m_stCurrentProxyHead.srcid(), m_stCurrentProxyHead.uin() );

	return SUCCESS;
}

bool CDBHandle::DecSellDoc( int* vTranCode, int* vParame1, int* vParame2 )
{
	char tSql[ 256 ] = { 0 };
	int tAccountID = *vParame2;
	bool tExecSql = false;
	int tExecSqlCount = 0;

	PBBourseDetail* tpDetail = ( PBBourseDetail* )vParame1;
	if( tpDetail == NULL )
	{
		*vTranCode = BOURSE_CANNT_FIND_DOC;
		return false;
	}

	int tDoc = tpDetail->bourse_doc_no( );
	
	snprintf( tSql, sizeof( tSql ), "update UMS_BOURSE_TRADE set trade_type = %d where bourse_doc_no = %d", 
		BOURSE_TRADE_CLOSE, tDoc );
	tExecSql = m_pDatabase->DirectExecute( tSql );
    tExecSqlCount = m_pDatabase->AffectedRows( );
	if( tExecSql == false || tExecSqlCount == 0 )
	{
		*vTranCode = BOURSE_CANNT_FIND_DOC;		
		return false;
	}

	snprintf( tSql, sizeof( tSql ), "update UMS_BOURSE_ACCOUNT set currency_value = currency_value + %d where "
		" account_id = %d and currency_type = %d", tpDetail->yb_count( ), tpDetail->account_id( ),
		BOURSE_DB_YB_KEY );

	tExecSql = m_pDatabase->DirectExecute( tSql );
    tExecSqlCount = m_pDatabase->AffectedRows( );
	if( tExecSql == false || tExecSqlCount == 0 )
	{
		*vTranCode = ERR_UNKNOWN;		
		return false;
	}

	tExecSql = BourseDocHistory( tpDetail->bourse_doc_no( ), "UMS_BOURSE_TRADE" );
	if( tExecSql == false )
	{
		*vTranCode = ERR_UNKNOWN;		
		return false;
	}


	return true;
}

bool CDBHandle::BourseDocHistory( int vDoc, const char* vTable )
{
	char tSql[ 512 ] = { 0 };
	bool tInsert = false;
	bool tDelete = false;
	
//	snprintf( tSql, sizeof( tSql ), "insert into %s_HISTORY select bourse_doc_no, account_id, price, yb_count, "
//		" trade_type, bourse_date from %s where bourse_doc_no = %d and trade_type != %d", vTable, vTable, vDoc, 
//		BOURSE_TRADE_ING );

	snprintf( tSql, sizeof( tSql ), "UPDATE UMS_BOURSE_TRADE_HISTORY set trade_yb = ( select trade_yb from "
		" UMS_BOURSE_TRADE where bourse_doc_no = %d and trade_type != %d ), trade_type= ( select trade_type from"
		" UMS_BOURSE_TRADE where bourse_doc_no = %d and trade_type != %d ), bourse_end_date = %d where "
		" bourse_trade_detail = ( select bourse_trade_detail from UMS_BOURSE_TRADE where bourse_doc_no = %d and"
		" trade_type != %d )", vDoc, BOURSE_TRADE_ING, vDoc, BOURSE_TRADE_ING, time( NULL ), vDoc, BOURSE_TRADE_ING );


	tInsert = m_pDatabase->DirectExecute( tSql );

	snprintf( tSql, sizeof( tSql ), "delete from %s where bourse_doc_no = %d and trade_type != %d", vTable, vDoc, 
		BOURSE_TRADE_ING );

	tDelete = m_pDatabase->DirectExecute( tSql );

	if( tInsert != true || tDelete != true )
	{
		return false;
	}

	return true;
}

int CDBHandle::SetExchangeAccountLog( unsigned int vIndex, int vAccount, int vRole, int vOperate, int* vParame, 
	int64_t vDetailID1, int64_t vDetailID2 )
{
	LK_ASSERT( vIndex < ARRAY_CNT( mAccountLog ), return ERR_UNKNOWN );

	CExchangeAccountLog* tpLog = &mAccountLog[ vIndex ];

//	int tMoney = 0;
//	int tYb = 0;
//	GetBoureseAccountDB( vAccount, &tMoney, &tYb );

	tpLog->mLog.set_account_id( vAccount );
	tpLog->mLog.set_role_id( vRole );
	tpLog->mLog.set_optime( time( NULL ) );
///	tpLog->mLog.set_money( tMoney );
//	tpLog->mLog.set_yb( tYb );	
	tpLog->mLog.set_operate( vOperate );
	tpLog->mLog.set_detail_id_1( vDetailID1 );
	tpLog->mLog.set_detail_id_2( vDetailID2 );
	if( vParame == NULL )
	{
		tpLog->mLog.set_parame1( 0 );
		tpLog->mLog.set_parame2( 0 );
		tpLog->mLog.set_parame3( 0 );
		tpLog->mLog.set_parame4( 0 );
	}
	else
	{
		tpLog->mLog.set_parame1( vParame[ 0 ] );
		tpLog->mLog.set_parame2( vParame[ 1 ] );
		tpLog->mLog.set_parame3( vParame[ 2 ] );
		tpLog->mLog.set_parame4( vParame[ 3 ] );
	}

	tpLog->mIsNeedSend = true;

	return SUCCESS;
}

int CDBHandle::ExchangeAccountLogUnSend( )
{
	for( unsigned int i = 0; i < ARRAY_CNT( mAccountLog ); i++ )
	{
		mAccountLog[ i ].Clear( );
	}

	return SUCCESS;
}

int CDBHandle::ExchangeAccountLogSend( const CMessageHead* vpHead )
{
	for( unsigned int i = 0; i < ARRAY_CNT( mAccountLog ); i++ )
	{
		if( mAccountLog[ i ].mIsNeedSend == true )
		{
			int tMoney = 0;
			int tYb = 0;

			GetBoureseAccountDB( mAccountLog[ i ].mLog.account_id( ), &tMoney, &tYb );

			mAccountLog[ i ].mLog.set_money( tMoney );
			mAccountLog[ i ].mLog.set_yb( tYb );



			CMessage tMsg;
			const CMessageHead* tpHead = vpHead;

			tMsg.mutable_msghead()->set_messageid( ID_D22_BOURSE_LOG );
			tMsg.mutable_msghead()->set_srcfe( FE_DATASERVER );
			tMsg.mutable_msghead()->set_srcid( tpHead->dstid() );
			tMsg.mutable_msghead()->set_dstfe( tpHead->srcfe() );
			tMsg.mutable_msghead()->set_dstid( tpHead->srcid() );

			tMsg.set_msgpara( (unsigned int)(&mAccountLog[ i ].mLog) );


			SendMessageTo( &tMsg, m_stCurrentProxyHead.srcfe(), m_stCurrentProxyHead.srcid(), 
				m_stCurrentProxyHead.uin() );

			mAccountLog[ i ].Clear( );
		}
	}

	return SUCCESS;
}

int CDBHandle::ProcessBourseLogQuery( CMessage* vpMsg )
{
	CMessageBourseLogQuery* tpQuery = ( CMessageBourseLogQuery* )vpMsg->msgpara( );
	CMessageD2SBourseLog tLog;
	char tSql[ 128 ] = { 0 };

	unsigned int  tTime = time( NULL );
	tTime = tTime - 60 * 60 * 24 * 60;

	snprintf( tSql, sizeof( tSql ), "SELECT 0, 0, 0, buy_price, yb_count, bourse_date, 0 FROM "
		"UMS_BOURSE_LOG where buy_account = %d and bourse_date > %d order by bourse_date desc limit 0, 100", 
		tpQuery->accountid( ), tTime );

	BourseQueryInsertDetail( tLog.mutable_buy_log( ), tSql );

	snprintf( tSql, sizeof( tSql ), "SELECT 0, 0, 0, sell_price, yb_count, bourse_date, 0 FROM "
		"UMS_BOURSE_LOG where sell_account = %d and bourse_date > %d order by bourse_date desc limit 0, 100", 
		tpQuery->accountid( ) );

	BourseQueryInsertDetail( tLog.mutable_sell_log( ), tSql );
	
	tLog.set_accountid( tpQuery->accountid( ) );

	CMessage tMsg;
	const CMessageHead* tpHead = &vpMsg->msghead( );
	tMsg.mutable_msghead( )->set_messageid( ID_D2S_BOURSE_LOG );
    tMsg.mutable_msghead( )->set_srcfe( FE_DATASERVER );
    tMsg.mutable_msghead( )->set_srcid( tpHead->dstid() );
    tMsg.mutable_msghead( )->set_dstfe( tpHead->srcfe() );
    tMsg.mutable_msghead( )->set_dstid( tpHead->srcid() );
    tMsg.set_msgpara( (unsigned int)(&tLog) );

    SendMessageTo( &tMsg, m_stCurrentProxyHead.srcfe(), m_stCurrentProxyHead.srcid(), m_stCurrentProxyHead.uin() );

	return SUCCESS;	
}

int CDBHandle::UpdatePlayerInfoRequest( CMessage* pMsg )
{
	CMessageUpdatePlayerInfoRequest* tpMsg = ( CMessageUpdatePlayerInfoRequest* )pMsg->msgpara( );
	CMessage tMsg;
	CMessageUpdatePlayerInfoResponse tResPara ;

	tMsg.mutable_msghead()->set_messageid( ID_D2S_UPDATEPLAYERINFO_RESPONSE );
	tMsg.mutable_msghead()->set_srcfe( FE_DATASERVER );
	tMsg.mutable_msghead()->set_srcid( pMsg->msghead().dstid() );
	tMsg.mutable_msghead()->set_dstfe( pMsg->msghead().srcfe() );
	tMsg.mutable_msghead()->set_dstid( pMsg->msghead().srcid() );

	tMsg.set_msgpara( (unsigned int)(&tResPara) );

	tResPara.set_sessionid( tpMsg->sessionid( ) );
	tResPara.set_roleid( tpMsg->roleid( ) );

	char tSql[ 500 ] = { 0 };

	snprintf( tSql, sizeof( tSql ), "select account_id from UMS_ROLE where role_id = %d", tpMsg->roleid( ) );
	QueryResult* res0 = m_pDatabase->Query( tSql );
	if( res0 == NULL )
	{
		TRACE_ERROR( "[ %s : %d ][ %s ]sql %s exec, but no resultset returned", __LK_FILE__, __LINE__, __FUNCTION__,
			tSql );
		return 0;
	}
	int nRow0 = res0->GetRowCount() ;
	int nColumn0 = res0->GetFieldCount() ;
	Field* pField0 = res0->Fetch();
	if( pField0 == NULL )
	{
		TRACE_ERROR("[ %s : %d ][ %s ]ERROR: do sql %s success,row[%d], col[%d], but some row is null\n",
			__LK_FILE__, __LINE__, __FUNCTION__, tSql, nRow0, nColumn0 );

		// 安全释放结果集
		if( res0 != NULL )
		{
			delete res0;
			res0 = NULL;
		}

		return 0;
	}

	// 是否有记录
	if( nRow0 == 0 )
	{
		TRACE_ERROR("[ %s : %d ][ %s ]ERROR: do sql %s success, row[%d], col[%d], but record is empty \n",
			__LK_FILE__, __LINE__, __FUNCTION__, tSql, nRow0, nColumn0 );
		// 安全释放结果集
		if( res0 != NULL )
		{
			delete res0;
			res0 = NULL;
		}

		return 0;
	}
	int tAccountID = atoi( pField0[ 0 ].GetString( ) );

	memset( tSql, 0, sizeof(tSql) );

	switch( tpMsg->updatetype() )
	{
	case MatrixInfo:
		{
			snprintf( tSql, sizeof( tSql ), "select role_matrix from UMS_ROLE_DETAIL1_%02d where role_id = %d", tAccountID % 16, tpMsg->roleid( ) );
			break;
		}
	default:
		return 0;
	}	

	QueryResult* res = m_pDatabase->Query( tSql );
	if( res == NULL )
	{
		TRACE_ERROR( "[ %s : %d ][ %s ]sql %s exec, but no resultset returned", __LK_FILE__, __LINE__, __FUNCTION__,
			tSql );
		return 0;
	}

	int nRow = res->GetRowCount() ;
	int nColumn = res->GetFieldCount() ;


	Field* pField = res->Fetch();
	if( pField == NULL )
	{
		TRACE_ERROR("[ %s : %d ][ %s ]ERROR: do sql %s success,row[%d], col[%d], but some row is null\n",
			__LK_FILE__, __LINE__, __FUNCTION__, tSql, nRow, nColumn );

		// 安全释放结果集
		if( res != NULL )
		{
			delete res;
			res = NULL;
		}

		return 0;
	}

	// 是否有记录
	if( nRow == 0 )
	{
		TRACE_ERROR("[ %s : %d ][ %s ]ERROR: do sql %s success, row[%d], col[%d], but record is empty \n",
			__LK_FILE__, __LINE__, __FUNCTION__, tSql, nRow, nColumn );
		// 安全释放结果集
		if( res != NULL )
		{
			delete res;
			res = NULL;
		}

		return 0;
	}	

	static const int snMatrixBlobLen = 2000;
	char cSQL[ snMatrixBlobLen * 2 ] = {0};
	char cTempBuff[snMatrixBlobLen] = {0};
	char szFilterBuff[snMatrixBlobLen] = {0};
	int nTempLen = 0;
	char* pszTmpStr = NULL;
	pszTmpStr = szFilterBuff;

	memset( tSql, 0, sizeof(tSql) );

	switch( tpMsg->updatetype() )
	{
	case MatrixInfo:
		{
			PBMatrixInfo tMatrixInfo;
			if( tMatrixInfo.ParseFromArray( pField[ 0 ].GetString( ), pField[ 0 ].GetValueLen( ) ) != true )
			{
				TRACE_ERROR( "Player %d Update Parse matrix failed, len = %d", tpMsg->roleid( ), pField[ 0 ].GetValueLen( ) );
				tResPara.set_resultcode( ERR_PARSEPB );
			}
			else
			{
				int nSubType = tpMsg->subtype();
				int nValue = tpMsg->value();
				int nParam = tpMsg->param1();
				if ( nSubType == RemoveSlave )
				{
					for ( int i = 0; i < tMatrixInfo.masterid_size(); i++ )
					{
						if ( nValue == tMatrixInfo.masterid(i) )
						{
							tMatrixInfo.set_masterid(i,0);
							tMatrixInfo.set_masterpercent(i,0);
							break;
						}
					}
				}
				else if ( nSubType == RemoveMaster )
				{
					for ( int i = 0; i < tMatrixInfo.slaveid_size(); i++ )
					{
						if ( nValue == tMatrixInfo.slaveid(i) )
						{
							tMatrixInfo.set_slaveid(i,0);
							if ( nParam == BeginnerNum )
							{
								tMatrixInfo.set_beginnernum( std::max( 0, (int)(tMatrixInfo.beginnernum() - 1) ) );
							}
							else if ( nParam == FormalNum )
							{
								tMatrixInfo.set_formalnum( std::max( 0, (int)(tMatrixInfo.formalnum() - 1) ) );
							}
							else if ( nParam == EducatedNum )
							{
								tMatrixInfo.set_slavenum( std::max( 0, (int)(tMatrixInfo.slavenum() - 1) ) );
							}
							break;
						}
					}
				}
				else if ( nSubType == AddMasterPoint )
				{
					tMatrixInfo.set_masterpoint( tMatrixInfo.masterpoint() + nValue );
				}
				else if ( nSubType == AddFormal )
				{
					tMatrixInfo.set_beginnernum( std::max( 0, (int)(tMatrixInfo.beginnernum() - 1) ) );
					tMatrixInfo.set_formalnum( tMatrixInfo.formalnum() + 1 );
				}
				else if ( nSubType == AddSlaveNum )
				{
					for ( int i = 0; i < tMatrixInfo.slaveid_size(); i++ )
					{
						if ( nValue == tMatrixInfo.slaveid(i) )
						{
							tMatrixInfo.set_slaveid(i,0);
						}
					}
					tMatrixInfo.set_formalnum( std::max( 0, (int)(tMatrixInfo.formalnum() - 1) ) );
					tMatrixInfo.set_slavenum( tMatrixInfo.slavenum() + 1 );
				}
				
				if( tMatrixInfo.SerializeToArray( cTempBuff, sizeof(cTempBuff)-1  ) == false )
				{
					tResPara.set_resultcode( ERR_PARSEPB );
					break;
				}
				int tRealLen = EscapeDataString( &pszTmpStr, sizeof(szFilterBuff)-1, cTempBuff, tMatrixInfo.ByteSize() );
				if( tRealLen == -1 )
				{
					TRACE_ERROR( "[%s:%d] while escape_data_string, dst buff overflow\n" , __FUNCTION__, __LINE__ );
					tResPara.set_resultcode( ERR_PARSEPB );
					break;
				}				
				snprintf( tSql, sizeof( tSql ), "update UMS_ROLE_DETAIL1_%02d set role_matrix='%s' where role_id=%d", tAccountID % 16, 
					szFilterBuff, tpMsg->roleid( ) );
			}
			break;
		}
	default:
		return 0;
	}

	if( m_pDatabase->DirectExecute( tSql ) != true )
	{
		return -1;
	}

	SendResponse( &tMsg );

	return SUCCESS;
}





