#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_

#include "servertool.h"

/*
 * 产生唯一的序列号
 *
 */
class CSequence : public CSingleton< CSequence >
{
	private:
		// 每次预分配的个数
		enum { em_once_block_size = 5000 };

//		int m_nMinID;
//		int m_nMaxID;
//		int m_nStep;

		int m_nNextMinID;
		int m_nNextLeft;

		int m_nCurID;
		int m_nLeft;
	
		bool m_bGetLasting;
	public:
		CSequence( /* int nMinID, int nMaxID, int nStep */ ) // : m_nMinID(nMinID), m_nMaxID (nMaxID), m_nStep(nStep)
		{
			m_nLeft = 0;
			m_bGetLasting = false;

			m_nCurID = 0;
			m_nNextMinID = 0;
			m_nNextLeft = 0;
		}

		~CSequence() 
		{
		//	if ( m_nLeft <= 0) return;

			//为了避免浪费，将当前值写入文件
		//	FILE* fp = fopen("../config/data/scene_config/sequence.cfg", "w+");
		//	if (fp == NULL) return;

		//	if ( fprintf(fp, "%010d\n", m_nCurID ) < 0) 
		//	{
		//		return;
		//	}
				
		//	if (fclose(fp) != -1)
		//	{
		//		return;
		//	}
		}

		int SetStep( int nNextMinID, int nNextStep )
		{
			m_nNextLeft = nNextStep;
			m_nNextMinID = nNextMinID;
			LOG_NOTICE( "default", "[ %s : %d ][ %s ] Set Step %d, %d", __LK_FILE__, __LINE__, __FUNCTION__, 
				nNextMinID, nNextStep );
			return SUCCESS;	
		}

		//int GetNewSequent( );
		//int CanntGetNewSequent( );

		void GetLastingEnd( )
		{
			m_bGetLasting = false;
			LOG_NOTICE( "default", "[ %s : %d ][ %s ] End Get Sequent %d, %d, %d, %d", __LK_FILE__, __LINE__, 
				__FUNCTION__, m_nCurID, m_nLeft, m_nNextMinID, m_nNextLeft );
		}	

		void GetLastingBegin( )
		{
			m_bGetLasting = true;
			LOG_NOTICE( "default", "[ %s : %d ][ %s ] Begin Get Sequent %d, %d", __LK_FILE__, __LINE__, __FUNCTION__,
				m_nCurID, m_nLeft );
		}

		int NextID()
		{
			// 如果没有剩余，则分配
			if ( m_nLeft == 0 )
			{
				if( m_nNextLeft == 0 )
				{
					LOG_ERROR( "default", "[ %s : %d ][ %s ] Sequence Is Empty", __LK_FILE__, __LINE__, __FUNCTION__ );
					//GetNewSequent( );
					return -1;
				}

				m_nCurID = m_nNextMinID;
				m_nLeft = m_nNextLeft;
				m_nNextLeft = 0;
	//			int lastid = -1;

	//			FILE* fp = fopen("../config/data/scene_config/sequence.cfg", "r+");
	//			if (fp == NULL) return -1;
	//			if ( fscanf(fp, "%d\n", &lastid) <= 0)  return -1;

	//			int nextid = lastid + m_nStep * em_once_block_size;

				// 如果超过了最大值, 为简化操作，立即赋值为最小值
	//			if ( nextid > m_nMaxID || nextid < m_nMinID) 
	//			{
	//				nextid = m_nMinID + m_nStep * em_once_block_size;
	//				lastid = m_nMinID;

	//				if ( nextid > m_nMaxID || nextid < m_nMinID)
	//				{
	//					return -1;
	//				}
	//			}

				// 移动到文件的开始位置
	//			if ( fseek(fp, 0, SEEK_SET) < 0)
	//			{
	//				return -1;
	//			}

				// 将预分配后的nextid写入到文件中
	//			if ( fprintf(fp, "%010d\n", nextid ) < 0) 
	//			{
	//				return -1;
	//			}

	//			if ( fclose(fp) != 0 )
	//			{
	//				return -1;
	//			}

	//			m_nLeft = em_once_block_size;
	//			m_nCurID = lastid;
			}

			m_nCurID++;
			m_nLeft--;

			if( m_nLeft <= 50 )
			{
				if( m_nNextLeft == 0 )
				{
					//GetNewSequent( );
				}
			}

			return m_nCurID;	

	//		m_nCurID += m_nStep;	// 最小的ID不使用
	//		m_nLeft --;
//
//			return m_nCurID;
		}
};

#endif



