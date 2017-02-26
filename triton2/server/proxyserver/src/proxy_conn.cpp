#define MAX_SEND_BLOCK_NUM       20
#include "proxy_conn.h"
int CMyTCPConn::EstConn(int iAcceptFD)
{
	int iTempRet = 0;

#ifdef _POSIX_MT_
	pthread_mutex_lock( &m_stMutex );
#endif

	iTempRet = GetSocket()->Accept(iAcceptFD);

	m_iConnState = ENTITY_OFF;


#ifdef _POSIX_MT_
	pthread_mutex_unlock( &m_stMutex );
#endif

	return iTempRet;
}

int CMyTCPConn::IsConnCanRecv()
{
	int iTempRet = 0;

#ifdef _POSIX_MT_
	pthread_mutex_lock( &m_stMutex );
#endif
	if( GetSocket()->GetSocketFD() > 0 && GetSocket()->GetStatus() == tcs_connected )
	{
		iTempRet = True;
	}
	else
	{
		iTempRet = False;
	}
#ifdef _POSIX_MT_
	pthread_mutex_unlock( &m_stMutex );
#endif

	return iTempRet;
}

int CMyTCPConn::RegToCheckSet(fd_set *pCheckSet)
{
	int iTempRet = 0;

#ifdef _POSIX_MT_
	pthread_mutex_lock( &m_stMutex );
#endif

	iTempRet = GetSocket()->AddToCheckSet( pCheckSet );

#ifdef _POSIX_MT_
	pthread_mutex_unlock( &m_stMutex );
#endif

	return iTempRet;
}

int CMyTCPConn::IsFDSetted(fd_set *pCheckSet)
{
	int iTempRet = 0;

#ifdef _POSIX_MT_
	pthread_mutex_lock( &m_stMutex );
#endif

	iTempRet = GetSocket()->IsFDSetted( pCheckSet );

#ifdef _POSIX_MT_
	pthread_mutex_unlock( &m_stMutex );
#endif

	return iTempRet;
}

int CMyTCPConn::RecvAllData()
{
	int iTempRet = 0;

#ifdef _POSIX_MT_
	pthread_mutex_lock( &m_stMutex );
#endif

	iTempRet = GetSocket()->RecvData();

#ifdef _POSIX_MT_
	pthread_mutex_unlock( &m_stMutex );
#endif

	return iTempRet;
}

int CMyTCPConn::GetOneCode(unsigned short &nCodeLength, BYTE *pCode)
{
	return GetSocket()->GetOneCode( nCodeLength, pCode );
}

int CMyTCPConn::SendCode(unsigned short nCodeLength, BYTE *pCode, int iFlag /* = FLG_CONN_IGNORE */)
{
	int iTempRet = 0;

#ifdef _POSIX_MT_
	pthread_mutex_lock( &m_stMutex );
#endif

	if( GetEntityType() < 0 || GetEntityID() < 0 )
	{
		//未配置节点直接返回
#ifdef _POSIX_MT_
		pthread_mutex_unlock( &m_stMutex );
#endif
		return -1;
	}

	if( iFlag == FLG_CONN_CTRL || iFlag == FLG_CONN_IGNORE )
	{
		//带外数据和只读操作优先发送,不参与排队
		if( GetSocket()->GetSocketFD() > 0 && GetSocket()->GetStatus() == tcs_connected )
		{
			iTempRet = GetSocket()->SendOneCode(nCodeLength, pCode);
		}
		else
		{
			iTempRet = -1;
		}

#ifdef _POSIX_MT_
		pthread_mutex_unlock( &m_stMutex );
#endif
		return iTempRet;
	}

	/*
	//如果已经有滞留数据先尝试发送
	if((m_iBlockStatus != BLK_EMPTY) && (m_iConnState == ENTITY_ON))
	{
#ifdef _POSIX_MT_
		pthread_mutex_unlock( &m_stMutex );
#endif
		CleanBlockQueue(MAX_SEND_BLOCK_NUM);
#ifdef _POSIX_MT_
		pthread_mutex_lock( &m_stMutex );
#endif
	}

	if( m_iBlockStatus == BLK_EMPTY && m_iConnState == ENTITY_ON )
	{
		iTempRet = GetSocket()->SendOneCode(nCodeLength, pCode);
	}

	if( (m_iConnState == ENTITY_OFF && GetSocket()->GetSocketFD() > 0 && GetSocket()->GetStatus() == tcs_connected) ||
			((m_iBlockStatus != BLK_EMPTY || iTempRet != 0) && iFlag == FLG_CONN_REDO) )
	{
		LOG_DEBUG("default", "Now begin to reserve codes.\n");

		if( m_pBlockQueue == NULL )
		{
			LOG_DEBUG("default", "Create block queue.\n");
			m_pBlockQueue = new CCodeQueue(BLOCKQUEUESIZE);
		}
		if( m_pBlockQueue )
		{
			if( m_iBlockStatus == BLK_EMPTY )
			{
				m_iBlockStatus = BLK_SAVING;
				LOG_DEBUG("default", "Now the block status of (%d,%d) is %d.\n", GetEntityType(), GetEntityID(), m_iBlockStatus);
			}
			int iRsvRet = m_pBlockQueue->AppendOneCode(pCode, nCodeLength);
			if( iRsvRet == -2 )
			{
				if( m_pRedoQueue != m_pBlockQueue )
				{
					TFName szDumpFile;
					snprintf(szDumpFile, sizeof(szDumpFile)-1, "CODE%02d_%02d_%02d.tmp", GetEntityType(), GetEntityID(), m_iCurrentDumpSeq);
					LOG_DEBUG("default", "Block queue is full now, begin to dump to file %s.\n", szDumpFile);
					m_pBlockQueue->DumpToFile(szDumpFile);
					m_pBlockQueue->CleanQueue();
					m_iCurrentDumpSeq++;
				}
				else
				{
					LOG_DEBUG("default", "Block queue is full and redo queue is just the same, so create another block queue.\n");
					m_pBlockQueue = new CCodeQueue(BLOCKQUEUESIZE);
				}
				if( m_pBlockQueue )
				{
					m_pBlockQueue->AppendOneCode(pCode, nCodeLength);
				}
			}
			LOG_DEBUG("default", "Successfully reserved code.\n");
		}
	}

#ifdef _POSIX_MT_
	pthread_mutex_unlock( &m_stMutex );
#endif
	*/

	return iTempRet;
}

int CMyTCPConn::CleanBlockQueue(int iQueueLength)
{
	int iRedoCount = 0, i, iTempRet = 0;
	int nCodeLength;
	BYTE  abyCodeBuf[MAX_SERVER_TRANS_LEN];

#ifdef _POSIX_MT_
	pthread_mutex_lock( &m_stMutex );
#endif

	if( m_iConnState == ENTITY_ON &&
			GetSocket()->GetSocketFD() > 0 && GetSocket()->GetStatus() == tcs_connected && m_iBlockStatus != BLK_EMPTY &&

			GetSocket()->CleanReserveData() == 0 )

	{
		LOG_DEBUG("default", "Now begin to redo transfer, will recover no more than %d codes.\n", iQueueLength);
		if( m_pRedoQueue == NULL )
		{
			m_pRedoQueue = new CCodeQueue(BLOCKQUEUESIZE);
			LOG_DEBUG("default", "Redo queue created now.\n");
		}
		if( m_pRedoQueue )
		{
			for( i = 0; i < iQueueLength; i++ )
			{

				if(GetSocket()->HasReserveData())
				{
					LOG_ERROR("default", "Redo process is stopped because of data block.\n");
					break;
				}

				m_pRedoQueue->GetHeadCode(abyCodeBuf, &nCodeLength);
				if( nCodeLength == 0 )
				{
					LOG_DEBUG("default", "The redo queue is empty now.\n");
					if( m_iCurrentRedoSeq < m_iCurrentDumpSeq )
					{
						TFName szDumpFile;

						snprintf(szDumpFile, sizeof(szDumpFile)-1, "CODE%02d_%02d_%02d.tmp", GetEntityType(), GetEntityID(), m_iCurrentRedoSeq);
						LOG_DEBUG("default", "Load redo queue from file %s.\n", szDumpFile);
						m_pRedoQueue->LoadFromFile(szDumpFile);
						m_iCurrentRedoSeq++;
						i--;
						unlink((const char *)szDumpFile);
					}
					else if( m_pRedoQueue != m_pBlockQueue )
					{
						delete m_pRedoQueue;
						m_pRedoQueue = m_pBlockQueue;
						i--;
						LOG_DEBUG("default", "Now begin to redo current block queue.\n");
					}
					else
					{
						delete m_pRedoQueue;
						m_pRedoQueue = m_pBlockQueue = NULL;
						m_iBlockStatus = BLK_EMPTY;
						m_iCurrentRedoSeq = m_iCurrentDumpSeq = 0;
						LOG_DEBUG("default", "Now all block queue is cleaned.\n");
						break;
					}
				}
				else
				{
					if (nCodeLength > 0xFFFF || nCodeLength < 0)
					{
						LOG_ERROR("default", "the message length(%d) is too big", nCodeLength);
						continue;
					}

					iTempRet = GetSocket()->SendOneCode(nCodeLength, abyCodeBuf);
					if( iTempRet == 0 )
					{
						iRedoCount++;
						LOG_DEBUG("default", "Redo one code OK.\n");
					}
					else
					{
						LOG_ERROR("default", "Resend one code failed of %d.\n", iTempRet);
						break;
					}
				}	//end if code length == 0
			}	//end for
		} //end if(pRedoQueue)
	}

#ifdef _POSIX_MT_
	pthread_mutex_unlock( &m_stMutex );
#endif

	return iRedoCount;
}

int CMyTCPConn::SetConnState(int iConnState)
{
#ifdef _POSIX_MT_
	pthread_mutex_lock( &m_stMutex );
#endif

	m_iConnState = iConnState;

#ifdef _POSIX_MT_
	pthread_mutex_unlock( &m_stMutex );
#endif

	return 0;
}
int CMyTCPConn::GetConnState()
{
	return m_iConnState ;
}

