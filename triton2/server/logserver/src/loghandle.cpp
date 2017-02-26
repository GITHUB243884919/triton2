#include "loghandle.h"
#include "logctrl.h"
#include "QueryResultMysql.h"
#include "DatabaseMysql.h"
#include "servermessage_in.h"
#include "log_msg_pb.hxx.pb.h"
#include "logevent.h"
#include "base.h"
#include <sys/time.h>
#include "dbinterface.h"

CSharedMem* CLogHandle::ms_pCurrentShm = NULL;
pthread_mutex_t CLogHandle::stLogConnMutex;

int CLogHandle::m_iLogSvrID = -1;
short CLogHandle::m_sLogOperMode = -1;


void* CLogHandle::operator new( size_t nSize )
{
    BYTE* pTemp;

    if(!ms_pCurrentShm)
    {
        return (void *)NULL;
    }

    pTemp = (BYTE *)ms_pCurrentShm->CreateSegment( nSize );

    return (void *)pTemp;
}

void CLogHandle::operator delete( void *pMem )
{
}

int CLogHandle::CountSize()
{
    int iTemp = 0;
    int iTempSingle = 0;

    iTemp += sizeof(CLogHandle);
    LOG_INFO( "logctrl", "CLogHandle %d", iTemp-iTempSingle );
    iTempSingle = iTemp;

    iTemp += CCodeQueue::CountQueueSize(INPUTQUEUELENGTH);
    LOG_INFO( "logctrl", "INPUTQUEUELENGTH %d", iTemp-iTempSingle );
    iTempSingle = iTemp;

    return iTemp;
}

CLogHandle::CLogHandle()
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
		printf( "in CLogHandle::CLogHandle(), new CCodeQueue failed" );
		exit(1);
	}

	m_iHandleID = -1;

    m_tLastCheckTime = time(NULL);

	m_pDatabase = new DatabaseMysql;
	if( m_pDatabase == NULL )
	{
		printf( "in CLogHandle::CLogHandle(), new DatabaseMysql failed" );
		exit(1);
	}

    if(ms_pCurrentShm->GetInitMode() == SHM_INIT)
    {
        TRACE_DEBUG("Construct log handle in init mode.\n");
    }
    else
    {
        TRACE_DEBUG("Construct log handle in resume mode.\n");
    }
}

CLogHandle::~CLogHandle()
{
}

int CLogHandle::OtherInit()
{
      return 0;
}

int CLogHandle::OtherResume()
{
      return 0;
}

int CLogHandle::IsToBeBlocked()
{
    if(!m_pInputQueue)
    {
        return True;
    }

    return m_pInputQueue->IsQueueEmpty();

}

int CLogHandle::Initialize(int iHandleID, CTCPConn<RECVBUFLENGTH, POSTBUFLENGTH>* pProxySvrdConns, const char* pMysqInfo )
{
    TFName szThreadLogFile;

    if(!pProxySvrdConns)
    {
        return -1;
    }

    m_iHandleID = iHandleID;
    m_pProxySvrdConns = pProxySvrdConns;

	strncpy( m_szHostInfoString, pMysqInfo, sizeof(m_szHostInfoString)-1 );

	// 初始化日志
	InitLog( NULL, NULL, LEVEL_DEBUG );

	m_pDatabase->Initialize ( m_szHostInfoString );
	m_pRemoteDatabase = NULL;

	m_nMonitorTimer = 0;
    return 0;
}

int CLogHandle::GetOneCode(int& nCodeLength, BYTE* pCode)
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

int CLogHandle::PostOneCode(int nCodeLength, BYTE* pCode)
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
//  Date: 		04/21/2009
// 
// ***************************************************************
int CLogHandle::ConvertCodeToMsg(int nCodeLength, BYTE* pCode, CProxyHead* pProxyHead, CMessage* pMsg)
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

int CLogHandle::ConvertMsgToCode(int& nCodeLength, BYTE* pCode, CProxyHead* pProxyHead, CMessage* pMsg)
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

int CLogHandle::SendMessageTo(CMessage *pMsg, short nDstEnType, short nDstEnID, unsigned int unUin)
{
	if( pMsg == NULL || m_pProxySvrdConns == NULL )
	{
		TRACE_ERROR("in CLogHandle::SendMessageTo, pmsg is null");
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

	nCodeLength = pbmsg_setmsg( stProxyHead, *pMsg, (char*)abyCodeBuf, nCodeLength );
	if( nCodeLength <= 0 )
	{
		TRACE_ERROR( "Encode Message Faild" );
		return -1;
	}
	
	// TODO: 死代码，因为西游的结构只有一台proxy
	int nSendReturn = m_pProxySvrdConns[0].GetSocket()->SendOneCode( nCodeLength, abyCodeBuf );
	if( nSendReturn < 0 )
	{
		TRACE_ERROR( "Send Code(len:%d) To Proxy faild(error=%d)", nCodeLength, nSendReturn );
		return -1;
	}

	return 0;
}

int CLogHandle::SendResponse(CMessage *pMsg)
{
    if( pMsg == NULL )
    {
        TRACE_ERROR("in CLogHandle::SendResponse, pmsg is null");
        return -1;
    }

    return SendMessageTo(pMsg, m_stCurrentProxyHead.srcfe(), m_stCurrentProxyHead.srcid(), m_stCurrentProxyHead.uin() );
}



int CLogHandle::ConnectToLocalLog()
{
	bool bInitRet = m_pDatabase->Initialize( m_szHostInfoString );
	

	return bInitRet ? 0 : -1;
}

int CLogHandle::InitLogFile( const char* vLogName, const char* vLogDir, LogLevel vPriority  /*= LEVEL_NOTSET*/ , unsigned int vMaxFileSize  /*= 10*1024*1024*/ , unsigned int vMaxBackupIndex  /*= 1*/ , bool vAppend  /*= true*/  )
{
	TFName szThreadLogFile;

	// 设置服务器日志
	snprintf( szThreadLogFile, sizeof(szThreadLogFile)-1,"../log/loghandle%d_", m_iHandleID);

	ThreadLogInit( szThreadLogFile, vMaxFileSize, vMaxBackupIndex, 0, vPriority);

	LK_ASSERT( m_pDatabase != NULL, return -1 );

	m_pDatabase->InitLog( szThreadLogFile, szThreadLogFile, vPriority, vMaxFileSize,vMaxBackupIndex );
}

int CLogHandle::ProcessThreadEnd()
{
    TRACE_DEBUG("---Handle End Process Begin---\n");
	/* ProcessCheckPonits();*/
    TRACE_DEBUG("---Handle End Process End---\n");
    return 0;
}




int CLogHandle::Event(CMessage *pMsg)
{
	if( pMsg == NULL )
	{
		TRACE_ERROR("In CLogHandle::Event, input msg null" );
		return -1;
	}

	switch ( pMsg->msghead().messageid() )
	{
		case ID_L2L_HANDLETIMEOUT:
		{
			m_nMonitorTimer ++;
			OnMessageHandleTimeOut( );
			break;
		}

		case ID_S2S_EXECUTESQL_REQUEST:
		{
			ProcessExecuteSqlRequest(pMsg);
			break;
		}

		case ID_S2L_ROLE_OPEXP:
		{
			CMessageLogRoleOpExp* pproto = 	(CMessageLogRoleOpExp*) pMsg->msgpara();
			if (pproto != NULL)
			{
				Catalog(EM_SERVER_EXP, pproto->optype(), 
						pproto->exptype(), EM_STAT_ROLE, pproto->deltaexp());
			}

			if (! CLogCtrl::GetSingleton().IsExpLimited(pproto->exptype(), pproto->deltaexp())
			|| CLogCtrl::GetSingleton().IsExcept(pproto->roleid()))
			{
				ProcessNormalLogMessage(pMsg);
			}

			break;
		}

		case ID_S2L_ROLE_OPMONEY:
		{
			CMessageLogRoleOpMoney* pproto = (CMessageLogRoleOpMoney*) pMsg->msgpara();
			if (pproto != NULL)
			{
				// 将向NPC出售金珊瑚单独成类型
				const int EM_GET_BY_SELL_JINSHANHU = 49;

				// 系统邮件获得成单独类型
				const int EM_GET_MONEY_BY_SYSMAIL = 48;
				if ( pproto->optype() == EM_GET_BY_SELL2NPC && pproto->opparam1() == 40131)
				{
					Catalog(EM_SERVER_MONEY, EM_GET_BY_SELL_JINSHANHU,
						pproto->moneytype(), EM_STAT_ROLE, pproto->deltamoney());
				}else if ( pproto->optype() == EM_GET_BY_RECV_MONEY && pproto->opparam2() == 0)
				{
					Catalog(EM_SERVER_MONEY, EM_GET_MONEY_BY_SYSMAIL,
						pproto->moneytype(), EM_STAT_ROLE, pproto->deltamoney());
				}else
				{
					Catalog(EM_SERVER_MONEY, pproto->optype(), 
						pproto->moneytype(), EM_STAT_ROLE, pproto->deltamoney());
				}
			}

			ProcessNormalLogMessage(pMsg);
			break;
		}

		case ID_S2L_ROLE_OPITEM:
		{
			CMessageLogRoleOpItem* pproto = (CMessageLogRoleOpItem*) pMsg->msgpara();
			if (pproto != NULL)
			{
				// 系统邮件获得成单独类型
				const int EM_GET_ITEM_BY_SYSMAIL = 48;
				if ( pproto->optype() == EM_GET_BY_RECV_MAIL && pproto->opparam1() == 0)
				{
					Catalog(EM_SERVER_ITEM, EM_GET_ITEM_BY_SYSMAIL,
						pproto->itemid(), EM_STAT_ROLE, pproto->itemnum());
				}else
				{
					Catalog(EM_SERVER_ITEM, pproto->optype(), 
						pproto->itemid(), EM_STAT_ROLE, pproto->itemnum());
				}
			}

			if ( CLogCtrl::GetSingleton().IsFilterItem(pproto->itemid(), pproto->optype()) != true)
			{
				ProcessNormalLogMessage(pMsg);
			}

			break;
		}

		case ID_S2L_FAMILY_ACTION:
		{
			CMessageLogFamily* pproto = (CMessageLogFamily*) pMsg->msgpara();
			if (pproto != NULL)
			{
				Catalog(EM_SERVER_FAMILY, pproto->optype(), pproto->attrid(), 
						EM_STAT_FAMILY, pproto->param1() - pproto->param2());
			}

			ProcessNormalLogMessage(pMsg);
			break;

		}

		case ID_S2L_CORPS_ACTION:
		{
			CMessageLogCorps* pproto = (CMessageLogCorps*) pMsg->msgpara();
			if (pproto != NULL)
			{
				Catalog(EM_SERVER_CORPS, pproto->optype(), pproto->attrid(), 
						EM_STAT_CORPS, pproto->param1() - pproto->param2());
			}

			ProcessNormalLogMessage(pMsg);
			break;
		}

		case ID_S2L_ORGE_DROPED:
		case ID_S2L_ROLE_OPTASK:
		case ID_S2L_ROLE_KILLED:
		case ID_S2L_ROLE_LOGOUT:
		case ID_S2L_ROLE_UPGRADE:
		case ID_S2L_ITEM_UPGRADE:
		case ID_S2L_ITEM_DISPEAR:
		case ID_S2L_MAIL:
		case ID_S2L_SERVER_STAT_INFO_NOTICE:
		case ID_G2L_ROLE_CHAT:
		case ID_S2L_EXCHANGELOG_LOG:
		case ID_G2L_REPETION:
		case ID_S2L_VALUABLE_EQUIP:
		case ID_S2D_SAVEPLAYERITEMINFO_REQUEST:
		case ID_S2L_BOSSORGEDIE:
		case ID_S2L_LIFESKILL:
		case ID_S2L_MATRIX:
		case ID_S2L_WORLD:
		case ID_S2L_ROLE_DAMAGE:
		case ID_S2L_ROLE_BUFFER:
		case ID_S2L_ROLE_REPE_SCORE:
		{
			ProcessNormalLogMessage(pMsg);
			break;
		}

		default:
		{
			TRACE_ERROR("There is no action for msgid(%d)", pMsg->msghead().messageid());
			break;
		}
	}

	return 0;
}

int CLogHandle::PrepareToRun()
{
    TRACE_DEBUG("Handle %d prepare to run.\n", m_iHandleID);

    if(ConnectToLocalLog())
    {
        TRACE_ERROR("In CLogHandle::PrepareToRun, Connect to database failed.\n");
        return -1;
    }

    return 0;
}

int CLogHandle::Run()
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

        iTempRet = Event( &stTempMsg );

        if(iTempRet)
        {
			TRACE_ERROR("Handle event returns %d.\n", iTempRet);
        }

		pbmsg_delmsg(&stTempMsg);
    }
}

int CLogHandle::ProcessExecuteSqlRequest( CMessage* pMsg )
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
	Message* pUnknownMessagePara = (Message*) pMsg->msgpara();
	if ( pUnknownMessagePara != NULL)
	{
		const ::google::protobuf::Descriptor* pDescriptor= pUnknownMessagePara->GetDescriptor();

		TRACE_DEBUG( "MsgName[%s] MsgHead[%s] MsgPara[%s]",
			pDescriptor->name().c_str(), pMsg->ShortDebugString().c_str(), pUnknownMessagePara->ShortDebugString().c_str() );
	}
#endif

	Database* pDatabaseMysql = m_pDatabase;
	
	// 因为每个loghandle都只执行一条固定消息，所以只有是执行sql线程才会走到这里，
	// 也只有执行sql线程才会需要连接远程存储设备。在第一次走到这里时进行远端mysql初始化。
	static bool bInitRemoteMysql = false;
	if( ( bInitRemoteMysql == false ) && ( CLogCtrl::GetSingletonPtr()->GetLogSvrConf().m_szRemoteStorage[0] != 0 ) )
	{
		if ( m_pRemoteDatabase == NULL ) 
		{
			try
			{
				m_pRemoteDatabase = new DatabaseMysql;
			}catch(...)
			{
				TRACE_ERROR( "ERROR: could not new DatabaseMysql for remote database." );
				m_pRemoteDatabase = NULL;
			}
		}

		if( m_pRemoteDatabase != NULL )
		{
			bool bInitRet = m_pRemoteDatabase->Initialize( CLogCtrl::GetSingletonPtr()->GetLogSvrConf().m_szRemoteStorage );
			if( bInitRet == false )
			{
				TRACE_ERROR( "ERROR: connect to remote mysql failed" );
				
				if( m_pRemoteDatabase != NULL )
				{
					delete m_pRemoteDatabase;
				}
				
				m_pRemoteDatabase = NULL;
			}
			else
			{
				bInitRemoteMysql = true;
			}
		}
		else
		{
			TRACE_ERROR( "ERROR: craete remote database mysql failed.");
		}
	}


	// 通过参数来判断是否去远程存储上去查询数据
	if( pReqMsg->callback() & SQL_REMOTE_HOST )
	{
		if ( bInitRemoteMysql = true )
		{
			pDatabaseMysql = m_pRemoteDatabase;
		}
		else
		{
			TRACE_ERROR( "ERROR: use remote mysql connection failed, so use local instead" );
		}

	}


	// 需要回执
	if( pReqMsg->callback() & SQL_NEED_CALLBACK )
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
		
		if( pReqMsg->sqltype() == SELECT )
		{
			QueryResult* res = pDatabaseMysql ? pDatabaseMysql->Query( pReqMsg->sql().c_str() ) : NULL;
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

			if ( pDatabaseMysql ) pDatabaseMysql->ClearResult();
		}
		else
		{
			bool bExecResult = pDatabaseMysql ? pDatabaseMysql->DirectExecute( pReqMsg->sql().c_str() ) : false;
			tSqlResMsg.set_resultcode( bExecResult ? 1 : 0 );
		}
		
		// 取自增长字段
		if( pReqMsg->has_latestautoincid() )
		{
			QueryResult* res = pDatabaseMysql ? pDatabaseMysql->Query( "select last_insert_id()" ): NULL;
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
		}

		// 回复客户端
		SendResponse( &tMsg );

	}
	else
	{
#ifndef _ASYNC_SQL_
		// 同步执行
		if( pDatabaseMysql && pDatabaseMysql->DirectExecute( pReqMsg->sql().c_str() ) != true )
		{
			TRACE_ERROR( "Error: [%s][%d][%s], direct exec sql %s faild.\n", __LK_FILE__, __LINE__, __FUNCTION__, pReqMsg->sql().c_str() );
		}
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


int CLogHandle::OnMessageHandleTimeOut( )
{
	// 执行 sql buffer中的语句
	typedef std::map<int, CLogEvent*> ::const_iterator const_iterator; 

	const_iterator iter = m_EventMap.begin();
	for(; iter != m_EventMap.end(); ++iter)
	{
		CLogEvent* pLogEvent = iter->second;
		if ( pLogEvent != NULL)
		{
			CLogStat& rLogStat = pLogEvent->GetLogStat();
			for (size_t nIndex = 0; nIndex < pLogEvent->size(); ++nIndex)
			{
				const char* stmtsql = pLogEvent->GetSql(nIndex) ;
				if (stmtsql == NULL || stmtsql[0] == 0) continue;

				SaveLogEventToDB(stmtsql, rLogStat);
				pLogEvent->ClearSql(nIndex);

				// 只有定时才清空计数
				rLogStat.m_uiRecvNum = 0;
			}
		}
	}

	if ( m_nMonitorTimer >= CLogCtrl::GetSingleton().GetLogSvrConf().mMonitorTimer)
	{
		typedef std::map<int, CLogMonitor*> ::const_iterator const_iterator; 
		const_iterator iter = m_MonitorMap.begin();
		for(; iter != m_MonitorMap.end(); ++iter)
		{
			if ( iter->second != NULL)
			{
				iter->second->SaveStatInfo(m_pDatabase);
				iter->second->ClearStatInfo();
			}
		}

		m_nMonitorTimer = 0;

		// 固定Handle0去执行统计
		if ( m_iHandleID == 0 )
		{
			const char* szStatMoney = "call ProcStatMoneY()";
			if ( m_pDatabase->DirectExecute( szStatMoney ) != true )
			{
				TRACE_ERROR( "Error: [%s][%d][%s], direct exec sql %s faild.\n", 
					__LK_FILE__, __LINE__, __FUNCTION__,  szStatMoney );
			}
		}
	}
}

int CLogHandle::Catalog(int nCatalogType, int nActionType, int nValueType, int nEntityType, int nDeltaValue)
{
	// 计算CalID
	int nCalID = CLogCtrl::GetSingleton().CatalogID(nCatalogType, nEntityType, nActionType, nValueType);
	if ( nCalID == -1) return 0;

	typedef std::map<int, CLogMonitor*> ::const_iterator const_iterator; 
	typedef std::map<int, CLogMonitor*> ::value_type value_type; 

	// 计算写入哪个表
	int nTableID = CLogCtrl::GetSingleton().ItemLife(nCatalogType, nActionType);
	if ( nTableID != -1 )
	{
		const_iterator iter = m_MonitorMap.find(nTableID);
		if ( iter == m_MonitorMap.end())
		{
			std::pair<const_iterator,  bool> ret = m_MonitorMap.insert(value_type(nTableID, new CLogMonitor));
			if (ret.second)
			{
				TRACE_DEBUG("Debug: Insert into monitor_map %d OK", nTableID);
				iter = ret.first;

				if( iter->second != NULL)
				{
					iter->second->SetTableID(nTableID);
				}
			}
		}

		if (iter->second != NULL)
		{
			iter->second->Catalog(nCalID, nDeltaValue);
		}
	}else
	{
		TRACE_DEBUG("DEBUG: Can't find catalog_type(%d), action_type(%d), value_type(%d), entity_type(%d)",
				nCatalogType, nActionType, nValueType, nEntityType);
	}	

	return 0;
}

int CLogHandle::ProcessNormalLogMessage( CMessage* pMsg )
{
	LK_ASSERT( pMsg != NULL && pMsg->msgpara() != 0, return -1);

	int nMsgID = pMsg->msghead().messageid();

	typedef std::map<int, CLogEvent*> ::const_iterator const_iterator; 
	typedef std::map<int, CLogEvent*> ::value_type value_type; 

	const_iterator iter = m_EventMap.find(nMsgID);
	if ( iter == m_EventMap.end())
	{
		const char* szTableName = CLogCtrl::GetSingleton().GetTableName(nMsgID); 
		const int  nTable = CLogCtrl::GetSingleton().GetTableCount(nMsgID);

		if ( szTableName == NULL || nTable == 0)
		{
			TRACE_ERROR("Error: Can't find the ID(%d)2Table (%s,%d) from Config File", nMsgID, szTableName, nTable);
			return -1;
		}

		CLogEvent* pEvents = new CLogEvent (szTableName, nTable);

		std::pair<const_iterator, bool> ret = m_EventMap.insert(value_type(nMsgID, pEvents));
		if (ret.second )
		{
			TRACE_DEBUG("Debug: Insert into event_map %d: %s(%d) OK", nMsgID, szTableName, nTable);
			iter = ret.first;
		}
	}

	CLogEvent* pEvents = (CLogEvent*)(iter->second);
	if( pEvents == NULL )
	{
		return -1;
	}

	int nSplit = pMsg->msghead().dstid();


	EMAppendResult  rt = em_append_ok;
	switch(nMsgID)
	{	
		case ID_S2L_ORGE_DROPED:
		{
			rt = pEvents->AppendDropItemMessage(nSplit, pMsg );
			break;
		}

		case ID_S2L_SERVER_STAT_INFO_NOTICE:
		{
			rt = pEvents->AppendStatInfoMessage(nSplit, pMsg );
			break;
		}

		case ID_S2L_VALUABLE_EQUIP:
		case ID_S2D_SAVEPLAYERITEMINFO_REQUEST:
		{
			rt = pEvents->AppendItemGhostMessage(nSplit, pMsg);
			break;
		}
	
		default:
		{
			rt = pEvents->AppendMessage(nSplit, pMsg );
		}
	}

	switch(rt)
	{
		case em_append_ok:
		{
			Message* pproto = (Message*) pMsg->msgpara();
			TRACE_DEBUG("log successful msg(%s)", pproto->ShortDebugString().c_str());
			break;
		}

		case em_append_fail:
		{
			Message* pproto = (Message*) pMsg->msgpara();
			TRACE_ERROR("log fail msg(%s)", pproto->ShortDebugString().c_str());
			break;
		}

		case em_append_full:
		{

			CLogEvent* pLogEvent = iter->second;
			const char* stmtsql = pLogEvent->GetSql(nSplit) ;

			if ( pLogEvent != NULL )
			{
				CLogStat& rLogStat = pLogEvent->GetLogStat();
				if (stmtsql != NULL && stmtsql[0] != 0)
				{
					SaveLogEventToDB(stmtsql, rLogStat);
				}

				pLogEvent->ClearSql(nSplit);
			}

			break;
		}
	}

	return 0;
}


// 把缓存的sql语句一次性写入数据库
int CLogHandle::SaveLogEventToDB( const char* stmtsql, CLogStat& rLogStat)
{
	if (stmtsql == NULL || stmtsql[0] == 0) return -1;

	timeval tBeginTime, tEndTime, tElapsTime;
	gettimeofday( &tBeginTime, NULL );

	if ( m_pDatabase->DirectExecute( stmtsql ) != true )
	{
		//continue; 记录错误
		TRACE_ERROR( "Error: [%s][%d][%s], direct exec sql %s faild.\n", 
				__LK_FILE__, __LINE__, __FUNCTION__,  stmtsql );

		return -1;
	}else
	{
		TRACE_DEBUG( "Debug: [%s][%d][%s], direct exec sql %s successfully.\n", 
				__LK_FILE__, __LINE__, __FUNCTION__,  stmtsql );
	}

	gettimeofday( &tEndTime, NULL );


	TimeValMinus( tEndTime, tBeginTime, tElapsTime );

	TimeValPlus( rLogStat.m_tSyncInsertTime, tElapsTime, rLogStat.m_tSyncInsertTime );


	return 0;
}


CLogStat* CLogHandle::GetLogStat( int iMsgID )
{
	std::map<int, CLogEvent*>::iterator itFind = m_EventMap.find( iMsgID );
	if( itFind == m_EventMap.end() )
	{
		return NULL;
	}

	return &(itFind->second)->GetLogStat();
}
