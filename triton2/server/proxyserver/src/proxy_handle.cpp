#include "proxy_handle.h"
#include "proxy_ctrl.h"
#include "message_pb.hxx.pb.h"
#include "proxymessage_pb.hxx.pb.h"
#include "base.h"
#include <sys/time.h>
CProxyHandle::CProxyHandle()
{
	m_iHandleType = 0;

	m_pstCfg = NULL;
	//m_nMainCount = 0;
	//m_nDBCount = 0;
	//m_nOtherCount = 0;

	m_pMainConn = NULL;
	m_pDBConn = NULL;
	m_pOtherConn = NULL;
	m_pReadConn = NULL;


	memset((void *)&m_tvLastCheck, 0, sizeof(m_tvLastCheck));
	
}

CProxyHandle::~CProxyHandle()
{
}

int CProxyHandle::Initialize(int iHandleType, CMyTCPConn* pMainConn, CMyTCPConn* pDBConn, CMyTCPConn* pOtherConn, CProxySet *pProxyHandle, CMyTCPConn* pDunGeonHandle, CMyTCPConn* pGlobalProxy, TProxyCfg *pstCfg )
{
	char szLogBase[256];

	m_iHandleType = iHandleType;
	m_pMainConn = pMainConn;
	m_pDBConn = pDBConn;
	m_pOtherConn = pOtherConn;
	m_pProxyConn = pProxyHandle;
	m_pDungeonConn = pDunGeonHandle;
	m_pGlobalProxy = pGlobalProxy;

	m_pstCfg = pstCfg;
	//m_nMainCount = nMainCount;
	//m_nDBCount = nDBCount;
	//m_nOtherCount = nOtherCount;
	
	switch( m_iHandleType )
	{
		case CET_GAMESERVER:
		{
			m_pReadConn = m_pMainConn;
			m_nReadCount = MAX_GAME_SERVER;
			break;
		}
		case CET_DBSERVER:
		{
			m_pReadConn = m_pDBConn;
			m_nReadCount = MAX_DB_SERVER;
			break;
		}
		case CET_OTHERSERVER:
		{
			m_pReadConn = m_pOtherConn;
			m_nReadCount = MAX_OTHER_SERVER;
			break;
		}
		
		case CET_GLOBALPROXY:	// 全局代理服务器句柄
		{
			 m_pReadConn = pGlobalProxy;			
			 m_nReadCount = 1;
			break;
		}

		// 全局副本器用
		case CET_PROXYSERVER:
		{
			m_pReadConn = pProxyHandle->GetTcpConn();
			m_nReadCount = MAX_PROXY_NUM;
			break;
		}	

		case CET_DUNGEON:
		{
			m_pReadConn = pDunGeonHandle;
			m_nReadCount = MAX_DUNGEON_NUM;
			break;
		}
		default:
		{
			return -1;
		}
	}

	
	TFName szThreadLogFile;
	snprintf( szThreadLogFile, sizeof(szThreadLogFile)-1,"../log/proxyhandle%d_", m_iHandleType);

#ifdef _DEBUG_
	ThreadLogInit(szThreadLogFile, 0x1000000, 100, 0, LEVEL_DEBUG );
#else
	ThreadLogInit(szThreadLogFile, 0x1000000, 100, 0, LEVEL_NOTICE );
#endif

	// 统计分析
	memset( &m_stStat, 0, sizeof(m_stStat) );

	return 0;
}

int CProxyHandle::IsToBeBlocked()
{
	int i;
	CMyTCPConn *pMonConn = NULL;

	if( !m_pReadConn )
	{
		return True;
	}

	for( i = 0; i < m_nReadCount; i++ )
	{
		
		//if( m_pReadConn[i].GetSocket()->GetSocketFD() > 0 )
		if( m_pReadConn[i].IsConnCanRecv() )
		
		{
			return False;
		}
	}

	return True;
}

short CProxyHandle::GetDstDBIDByKey(u_int uiKey)
{
	short nDstDBID = -1;
	int iHashIdx = -1;

	/*
	if( iCount <= 0 )
	{
		return -1;
	}
	*/

	
#ifdef _POSIX_MT_
	
	//pthread_mutex_lock( &CProxyCtrl::stMutex );
	pthread_mutex_lock( &CProxyCtrl::stMutex[CProxyCtrl::MUTEX_HASHMAP] );
	
#endif
	

	iHashIdx = uiKey  % MAXHASHMAPITEMNUMBER;
	nDstDBID = m_pstCfg->m_astHashMapTable[iHashIdx].m_nCurrentSvrID;

	
#ifdef _POSIX_MT_
	
	//pthread_mutex_unlock( &CProxyCtrl::stMutex );
	pthread_mutex_unlock( &CProxyCtrl::stMutex[CProxyCtrl::MUTEX_HASHMAP] );
	
#endif
	

	return nDstDBID;
}


/*
short CProxyHandle::GetTargetMIDByRT(short nDstID)
{
	short nTargetID, nTargetIdx;


	//if( nDstID < (m_pstCfg->m_iProxyID - 1)*MAXSVRNUMBER ||
	//		nDstID >= m_pstCfg->m_iProxyID*MAXSVRNUMBER )
	//{
	//		return -1;
	//}


	nTargetIdx = nDstID%MAXSVRNUMBER;

#ifdef _POSIX_MT_
	pthread_mutex_lock( &CProxyCtrl::stMutex[CProxyCtrl::MUTEX_ROUTETAB] );
#endif

	nTargetID = m_pstCfg->m_anMRT[nTargetIdx];

#ifdef _POSIX_MT_
	pthread_mutex_unlock( &CProxyCtrl::stMutex[CProxyCtrl::MUTEX_ROUTETAB] );
#endif

	if( nTargetID < 0 )
	{
		nTargetID = nDstID;
	}

	return nTargetID;
}
*/


int CProxyHandle::ModifyHashMap(short nEntityID, int iModifyCount, BYTE *pbyKeys)
{
	short nDstDBID = -1;
	int iHashIdx = -1;
	int i;

	if( !pbyKeys )
	{
		return -1;
	}

#ifdef _POSIX_MT_
	
	//pthread_mutex_lock( &CProxyCtrl::stMutex );
	pthread_mutex_lock( &CProxyCtrl::stMutex[CProxyCtrl::MUTEX_HASHMAP] );
	
#endif

	for( i = 0; i < iModifyCount; i++ )
	{
		iHashIdx = (int)pbyKeys[i];
		m_pstCfg->m_astHashMapTable[iHashIdx].m_nPrevSvrID = m_pstCfg->m_astHashMapTable[iHashIdx].m_nCurrentSvrID;
		m_pstCfg->m_astHashMapTable[iHashIdx].m_nCurrentSvrID = nEntityID;
	}

#ifdef _POSIX_MT_
	
	//pthread_mutex_unlock( &CProxyCtrl::stMutex );
	pthread_mutex_unlock( &CProxyCtrl::stMutex[CProxyCtrl::MUTEX_HASHMAP] );

#endif

	return 0;
}

int CProxyHandle::SaveHashMapToFile()
{
	FILE *fpTempFile = NULL;
	char szLineBuf[1024];
	int  iReadRows = 0;
	int  iReadColomns = 0;
	int  i;

	int iTempHashKey;
	int iTempSvr1, iTempSvr2;

	fpTempFile = fopen(m_pstCfg->m_szMapFile, "w");
	if( !fpTempFile )
	{
		TRACE_ERROR("While save hash map, open file %s failed.\n", m_pstCfg->m_szMapFile);
		return -1;
	}

	for( i = 0; i < MAXHASHMAPITEMNUMBER; i++ )
	{
		fprintf(fpTempFile, "%d\t\t\t%d\t\t\t%d\n", i,
			m_pstCfg->m_astHashMapTable[i].m_nPrevSvrID,
			m_pstCfg->m_astHashMapTable[i].m_nCurrentSvrID);
	}

	fclose(fpTempFile);

	return 0;
}


int CProxyHandle::SendOneCodeTo( unsigned short nCodeLength, BYTE* pbyCode, short nDstFE, short nDstID, BYTE byOpFlag /* = 0 */, int nSrcGameID /* = 0 */, int nDesGameID /* = 0 */ )
{
	CMyTCPConn *pWriteConn = NULL;

	int iTempRet = 0;
	
	
	/*
	if( nCodeLength <= 0 || !pbyCode || nDstID < 0 || nDstID >= MAXSVRNUMBER )
	{
		return -1;
	}
	*/
	if( nCodeLength <= 0 || !pbyCode )
	{
		TRACE_ERROR("While send one code to (%d, %d), null code.\n", nDstFE, nDstID);
		return -1;
	}
/*
	if( nDstFE == CET_GAMESERVER )
	{

		if( nDstID < (m_pstCfg->m_iProxyID - 1)*MAXSVRNUMBER ||
			nDstID >= m_pstCfg->m_iProxyID*MAXSVRNUMBER )
		{
			TRACE("While send one code to(%d, %d), invalid dst id, proxy id = %d\n",
					nDstFE, nDstID, m_pstCfg->m_iProxyID);
			return -1;
		}
	}
*/


	if( nDstID < 0 || nDstID >= MaxServer(nDstFE) )
	{
		TRACE_ERROR("While send one code to (%d, %d), invalid destination id\n",
				nDstFE, nDstID);
		return -1;
	}
	

	TRACE_DEBUG("Send one code to (%d,%d), flag = 0X%02X.\n", nDstFE, nDstID, byOpFlag);
	
	// 除了注册信息外，目标服务器都不应该是游戏代理服务器或者全局代理服务器
	// 逻辑相关的应该不包括CET_PROXYSERVER和CET_GLOBALPROXY(场景发消息时必须携带GameID)
	// 如果是游戏代理服务器而且接收方的gameid>0且不等于本代理服务器的gameid，转发给全局服务器
	// 每个服务器自己填充srcgameid
	if ( m_pstCfg->m_nFlag == 0 && m_pstCfg->m_nGameID != nDesGameID && nDesGameID > 0 )
	{
			pWriteConn = m_pGlobalProxy;	
	}
	else if (  m_pstCfg->m_nFlag == 0 && nDstFE == CET_DUNGEON )
	{
			pWriteConn = m_pGlobalProxy;	
	}	

	// 全局代理服务器只管转发(nDesGameID 大于0说明要转发给某个特定的游戏代理服务器)
	else if( m_pstCfg->m_nFlag == 1 && nDesGameID > 0 )
	{
		pWriteConn = m_pProxyConn->GetTcpConnByEntityID( nDesGameID );
	}
	else
	{
		switch(nDstFE)
		{
		case CET_GAMESERVER:
			{	
				pWriteConn = &(m_pMainConn[nDstID]);	
				break;
			}
		case CET_DBSERVER:
			{
				pWriteConn = &(m_pDBConn[nDstID]);
				break;
			}
		case CET_OTHERSERVER:
			{
				pWriteConn = &(m_pOtherConn[nDstID]);
				break;
			}

		case CET_DUNGEON:
			{
				pWriteConn = &(m_pDungeonConn[nDstID]);
				break;
			}
		default:
			{
				return -1;
			}
		}
	}
	
	if ( pWriteConn == NULL )
	{
		return -1;
	}
		
	
	TRACE_DEBUG("The real rcver is (%d, %d)\n", pWriteConn->GetEntityType(), pWriteConn->GetEntityID());
	

	//return pWriteConn->GetSocket()->SendOneCode(nCodeLength, pbyCode);
	/*
	if( byOpFlag & PROXY_CMD )
	{
		iTempRet = pWriteConn->SendCode(nCodeLength, pbyCode, FLG_CONN_CTRL);
	}
	else if( byOpFlag & PROXY_RESERVE )
	{
		iTempRet = pWriteConn->SendCode(nCodeLength, pbyCode, FLG_CONN_REDO);
	}
	else
	*/
	{
		iTempRet = pWriteConn->SendCode(nCodeLength, pbyCode);
	}
	
	return iTempRet;
}

int CProxyHandle::BroadCastOneCode( unsigned short nCodeLength, BYTE* pbyCode, short nDstFE)
{
	CMyTCPConn *pWriteConns = NULL;
	int i;
	int iTempRet = 0;

	if( nCodeLength <= 0 || !pbyCode )
	{
		return -1;
	}

	switch(nDstFE)
	{
		case CET_GAMESERVER:
		{
			pWriteConns = m_pMainConn;
			break;
		}
		case CET_DBSERVER:
		{
			pWriteConns = m_pDBConn;
			break;
		}
		case CET_OTHERSERVER:
		{
			pWriteConns = m_pOtherConn;
			break;
		}
		default:
		{
			return -1;
		}
	}

	for( i = 0; i < MaxServer(nDstFE); i++ )
	{
	
		iTempRet = pWriteConns[i].SendCode(nCodeLength, pbyCode);
		
		if( !iTempRet )
		{
			TRACE_DEBUG("Broadcast code to (%d, %d) OK.\n", nDstFE, i);
		}
	}

	return 0;
}

int CProxyHandle::SendOneCodeByKey( unsigned short nCodeLength, BYTE* pbyCode, short nDstFE, u_int uiKey, BYTE byOpFlag /* = 0 */, int nSrcGameID /* = 0 */, int nDesGameID /* = 0 */ )

{
	short nDstID;

	switch(nDstFE)
	{
		case CET_GAMESERVER:
		{
			//nDstID = GetDstIDByKey(uiKey, m_pstCfg->m_iMainSvrNum);
			break;
		}
		case  CET_DBSERVER:
		{
			//注释掉按照uiKey分表的策略,直接使用nDstID
			//nDstID = GetDstDBIDByKey(uiKey);
			break;
		}
		case CET_OTHERSERVER:
		{
			//nDstID = GetDstIDByKey(uiKey, m_pstCfg->m_iOtherEntityNum);
			break;
		}
		default:
		{
			return -1;
		}
	}

	if( nDstID < 0 || nDstID >= MaxServer(nDstFE) )
	{
		return -1;
	}

	return SendOneCodeTo(nCodeLength, pbyCode, nDstFE, nDstID, byOpFlag, nSrcGameID, nDesGameID );
	
}

int CProxyHandle::TransferOneCode(unsigned short nCodeLength, BYTE* pbyCode)
{
	CProxyHead stTmpProxyHead;

	int iTempRet = 0;
	int i;

	if( nCodeLength <= 0 || !pbyCode )
	{
		TRACE_ERROR("ERROR: In TransferOneCode, invalid input params.\n");
		return -1;
	}

	unsigned short iPkgSize = *((unsigned short*)(pbyCode));
	if(iPkgSize != nCodeLength) 
	{
		TRACE_ERROR("ERROR: InLen %d != CodeLen %d", nCodeLength, iPkgSize );
		return -1;
	}

	unsigned short nHeadLength = *((unsigned short*)(pbyCode+2));

	if (stTmpProxyHead.ParseFromArray( pbyCode+4, nHeadLength) == false)
	{
		TRACE_ERROR( "ERROR: decode error in TransferOneCode, stream_length is %d", nHeadLength);
		return -1;
	}
	else
	{
		TRACE_DEBUG( "TransMsg(%d): %s", nCodeLength, stTmpProxyHead.ShortDebugString().c_str());
	}

	if( stTmpProxyHead.dstfe() == CET_PROXYSERVER || stTmpProxyHead.dstfe() == CET_GLOBALPROXY )
	{
		switch( stTmpProxyHead.opflag() )
		{
			case CMD_START:
			{
				m_pReadConn->SetConnState(ENTITY_ON);
				break;
			}
			case CMD_KEEPALIVE:
			{
				CProxyHead stResHead;

				stResHead.set_srcfe(CET_PROXYSERVER); 
				stResHead.set_dstfe(stTmpProxyHead.srcfe());
				stResHead.set_srcid(1);
				stResHead.set_dstid(stTmpProxyHead.srcid());
				stResHead.set_opflag(CMD_KEEPALIVE);
				stResHead.set_transfertype(TRANS_P2P);
			
				// CProxyHead命令包长度一般都很短
				char message_buffer[1024];
				memset(message_buffer, 0, sizeof(message_buffer));
	
				unsigned short usHeadLen  = stResHead.ByteSize();
				unsigned short usTotalLen = usHeadLen + 2 + 2;
				*((unsigned short*) message_buffer)  = usTotalLen;
				*((unsigned short*) (message_buffer+2) ) = usHeadLen;  

				if( stResHead.SerializeToArray(message_buffer+4, sizeof(message_buffer) - 4) )
				{
					int iTempRet = SendOneCodeTo(usTotalLen, (BYTE*) message_buffer, 
										stTmpProxyHead.srcfe(), stTmpProxyHead.srcid());
					if( iTempRet )
					{
						TRACE_ERROR("send keep alive cmd error to (%d, %d) failed of %d.",
								stTmpProxyHead.srcfe(), stTmpProxyHead.srcid(), iTempRet);
					}
				}
				else
				{
					TRACE_ERROR("regist to proxyserver (%d,%d) error", stTmpProxyHead.srcfe(), stTmpProxyHead.srcid());
				}

				break;
			}

			default:
			{
				TRACE_ERROR("Unknown command ID %d.\n",  stTmpProxyHead.opflag());
				break;
			}
		}
		return 0;
	}
	

	TRACE_DEBUG("Transfer code begin, from(%d,%d) To(%d,%d), type = %d, len = %d\n", 
			stTmpProxyHead.srcfe(), stTmpProxyHead.srcid(), 
			stTmpProxyHead.dstfe(), stTmpProxyHead.dstid(),
			stTmpProxyHead.transfertype(), nCodeLength );

	switch(stTmpProxyHead.transfertype())
	{
		case TRANS_P2P:
		{
			iTempRet = SendOneCodeTo(nCodeLength, pbyCode, stTmpProxyHead.dstfe(), stTmpProxyHead.dstid(), 0, stTmpProxyHead.srcgameid(), stTmpProxyHead.desgameid() );
			
			if( iTempRet )
			{
				TRACE_ERROR("transfer one code from (%d, %d) to (%d, %d) failed of %d.\n",
					stTmpProxyHead.srcfe(), stTmpProxyHead.srcid(), stTmpProxyHead.dstfe(), stTmpProxyHead.dstid(), iTempRet);
			}

			m_stStat.m_nTransferCodeNum ++;
			m_stStat.m_nTransferCodeLength += nCodeLength;
			break;
		}

		case TRANS_P2G:
		{
			/*
			for( i = 0; i < stTmpProxyHead.m_stTransferInfo.stSvrs.nFENumber; i++ )
			{
				
				//iTempRet = SendOneCodeTo(nCodeLength, pbyCode, stTmpProxyHead.m_nDstFE, stTmpProxyHead.m_stTransferInfo.stSvrs.anFEs[i]);
				iTempRet = SendOneCodeTo(nCodeLength, pbyCode, 
						stTmpProxyHead.dstfe(), stTmpProxyHead.m_stTransferInfo.stSvrs.anFEs[i], stTmpProxyHead.m_byOpFlag);
			
				if( iTempRet )
				{
					TRACE("transfer one code from (%d, %d) to (%d, %d) failed of %d.\n",
								stTmpProxyHead.srcfe(), stTmpProxyHead.srcid(), stTmpProxyHead.dstfe(),
								stTmpProxyHead.m_stTransferInfo.stSvrs.anFEs[i], iTempRet);
				}
			}
			*/

			for ( int i = 0; i < MaxServer(stTmpProxyHead.dstfe()); ++i)
			{
				SendOneCodeTo(nCodeLength, pbyCode, stTmpProxyHead.dstfe(), i);
			}

			break;
		}

		case TRANS_BROADCAST:
		{
			iTempRet = BroadCastOneCode(nCodeLength, pbyCode, stTmpProxyHead.dstfe());
			if( iTempRet )
			{
				TRACE_ERROR("broadcast one code from (%d, %d) to (%d) failed of %d.\n",
							stTmpProxyHead.srcfe(), stTmpProxyHead.srcid(), stTmpProxyHead.dstfe(), iTempRet);
			}
			break;
		}

		case TRANS_BYKEY:
		{
			/*
			TRACE("Key = %u\n", stTmpProxyHead.m_stTransferInfo.Uin);

			//iTempRet = SendOneCodeByKey(nCodeLength, pbyCode, stTmpProxyHead.m_nDstFE, stTmpProxyHead.m_stTransferInfo.Uin);
			iTempRet = SendOneCodeByKey(nCodeLength, pbyCode, stTmpProxyHead.dstfe(), 
					stTmpProxyHead.uin(), stTmpProxyHead.opflag());
			
			if( iTempRet )
			{
				TRACE("transfer one code from (%d, %d) to (%d) by key(%u) failed of %d.\n",
							stTmpProxyHead.srcfe(), stTmpProxyHead.srcid(), stTmpProxyHead.dstfe(), stTmpProxyHead.uin(), iTempRet);
			}
			*/
			break;
		}
		default:
		{
			return -1;
		}
	}

	TRACE_DEBUG("Transfer code ended.");

	return 0;
}
int CProxyHandle::DoTransfer()
{
	fd_set fds_read;
	struct timeval stMonTime;
	int i, iTempFD = -1;
	int iOpenFDNum = 0;
	int iTransferedCount = 0;
	int iTempRet = 0;

	unsigned short nTmpCodeLength;
	//性能优化分析 
	BYTE  abyCodeBuf[MAX_SERVER_TRANS_LEN];
	//BYTE abyOldCodeBuf[MAX_SERVER_TRANS_LEN];
	//short nTempOldCodeLen;
	int iTempTimes;

	FD_ZERO( &fds_read );
	stMonTime.tv_sec = 0;
	stMonTime.tv_usec = 100000;


	for( i = 0; i < m_nReadCount; i++ )
	{
		
		//if( (iTempFD=m_pReadConn[i].GetSocket()->GetSocketFD()) > 0 )
		if( m_pReadConn[i].IsConnCanRecv() )
	
		{
			//FD_SET( iTempFD, &fds_read );
			
			//m_pReadConn[i].GetSocket()->AddToCheckSet( &fds_read );
			m_pReadConn[i].RegToCheckSet( &fds_read );
			
		}
	}

	iOpenFDNum = select(FD_SETSIZE, &fds_read, NULL, NULL, &stMonTime);

	if( iOpenFDNum <= 0 )
	{
		return 0;
	}

	TRACE_DEBUG("--------Some entity has data to transfer----------.");

	for( i = 0; i < m_nReadCount; i++ )
	{
		
		//if( (iTempFD=m_pReadConn[i].GetSocket()->GetSocketFD()) < 0 )
		if( !m_pReadConn[i].IsConnCanRecv() )
		
		{
			continue;
		}

		//if( FD_ISSET(iTempFD, &fds_read) )
	
		//if( m_pReadConn[i].GetSocket()->IsFDSetted(&fds_read) )
		if( m_pReadConn[i].IsFDSetted(&fds_read) )
		
		{
			TRACE_DEBUG("Entity(%d, %d) has some data to transfer.", m_iHandleType, i);
			
			//m_pReadConn[i].GetSocket()->RecvData();
			m_pReadConn[i].RecvAllData();
			
			TRACE_DEBUG("Now transfer data...");

			//性能优化分析 
			//nTempOldCodeLen = 0;

			
			//while( m_pReadConn[i].GetSocket()->GetOneCode(nTmpCodeLength, (BYTE *)abyCodeBuf) > 0 )
			while(1)
			
			{
				nTmpCodeLength = sizeof(abyCodeBuf);
				iTempRet = m_pReadConn[i].GetOneCode(nTmpCodeLength, (BYTE *)abyCodeBuf);
				if( iTempRet > 0 )
				{
					// 性能优化分析 
					if(nTmpCodeLength < MINPROXYMSGLEN || nTmpCodeLength > MAX_SERVER_TRANS_LEN)
					{
						TRACE_ERROR("Get Error Len : %d Code from Conn %d\n", nTmpCodeLength, m_pReadConn[i].GetEntityID());
						//PrintBinT((char*)abyCodeBuf, nTmpCodeLength);
						TRACE_ERROR("Close Connection of  (%d,%d)", m_pReadConn[i].GetEntityType(), m_pReadConn[i].GetEntityID());
						//关闭掉连接
						m_pReadConn[i].GetSocket()->Close();
						m_pReadConn[i].SetConnState(ENTITY_OFF);
						break;
					}
					

					if(TransferOneCode(nTmpCodeLength, (BYTE *)abyCodeBuf) )
					{
						TRACE_ERROR("ERROR TransferOne code for %d: failed \n",  m_pReadConn[i].GetEntityID());
						//PrintBinT((char*)abyCodeBuf, nTmpCodeLength);
						TRACE_ERROR("ERROR: Close Connection (%d:%d)", m_pReadConn[i].GetEntityType(), m_pReadConn[i].GetEntityID());
						//关闭掉连接
						m_pReadConn[i].GetSocket()->Close();
						m_pReadConn[i].SetConnState(ENTITY_OFF);
						break;
					}
					iTransferedCount++;

					//memcpy((void*)abyOldCodeBuf, (void*)abyCodeBuf, nTmpCodeLength);
					//nTempOldCodeLen = nTmpCodeLength;
				}	
				else
				{
					if( iTempRet == -2 )
					{
						//性能优化分析 
						TRACE_ERROR("len is impossblie! form Conn: %u  \n", m_pReadConn[i].GetEntityID());
						m_pReadConn[i].SetConnState(ENTITY_OFF);
					}
                    			break;
				}
			}	//end while
		
			if( iTempRet == 0 )
			{
				TRACE_DEBUG("All data transfered.");
			}
			else
			{
				TRACE_ERROR("Data error, recv buffer is resetted.");
			}
			
		}	//end if
	}	//end for
	
	TRACE_DEBUG("------ Thread %d do transfer OK -------.", m_iHandleType);

	return iTransferedCount;
}

int CProxyHandle::CheckBlockCodes()
{
	timeval tvNow, tvGap;
	int iCleanCount = 0;
	int i;

	gettimeofday( &tvNow, NULL );
	TimeValMinus(tvNow, m_tvLastCheck, tvGap);
	if( tvGap.tv_sec < 1 )
	{
		return 0;
	}

	m_tvLastCheck.tv_sec = tvNow.tv_sec;
	m_tvLastCheck.tv_usec = tvNow.tv_usec;

	//TRACE("---- Now begin to clean block codes ----\n");
	for( i = 0; i < m_nReadCount; i++ )
	{
		if( m_pReadConn[i].IsConnCanRecv() )
		{
			iCleanCount += m_pReadConn[i].CleanBlockQueue(MAXCLEANBLOCKCODES);
		}
	}

	if( iCleanCount > 0 )
	{
		TRACE_ERROR("---- Clean block codes ended, %d block codes is cleaned ----", iCleanCount);
	}

	return iCleanCount;
}


int CProxyHandle::PrepareToRun()
{
	//性能优化分析 
	time(&m_tLastCheckTime);
	return 0;
}

int CProxyHandle::Run()
{
	//性能优化分析 
	time_t tCurrentTime;

	while( True )
	{
		CondBlock();

		DoTransfer();

	
		CheckBlockCodes();


		 //性能优化分析 
		if(CET_DBSERVER == m_iHandleType)
		{
			time( &tCurrentTime );
			if(tCurrentTime - m_tLastCheckTime > 60  )
			{
				m_tLastCheckTime = tCurrentTime;
				for( int i = 0; i < m_nReadCount; i++ )
				{
					if( m_pReadConn[i].IsConnCanRecv() )
					{
						// by lihui
						//m_pReadConn[i].GetSocket()->PrintfSocketStat();
					}
				}
			}
		}
	}
	
	return 0;
}

//性能优化分析 
void CProxyHandle::PrintBinT(char *pBuffer, int iLength)
{
	int i;

	char tmpBuffer[4*MAX_SERVER_TRANS_LEN];
	char strTemp[32];

	if ( iLength <= 0 || iLength > sizeof(tmpBuffer) || pBuffer == NULL )
	{
		return ;
	}

	tmpBuffer[0] = '\0';
	for ( i = 0; i < iLength ; i++ )
	{
		if ( !(i % 16) )
		{
			sprintf(strTemp, "\n%04d>    ", i / 16 + 1);
			strcat(tmpBuffer, strTemp);
		}
		sprintf(strTemp, "%02X ", (unsigned char)pBuffer[i]);
		strcat(tmpBuffer, strTemp);
	}

	// strcat(tmpBuffer, "\n");
	TRACE_DEBUG("Print Hex:%s \n", tmpBuffer);
	return ;
}

STProxyStat& CProxyHandle::GetProxyStat( )
{
	return m_stStat; 
}