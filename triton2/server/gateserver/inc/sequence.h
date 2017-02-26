#ifndef _SEQUENCE_H_
#define _SEQUENCE_H_

#include "servertool.h"
#include "gatelogic.h"

/*
 * 产生唯一的序列号
 *
 */
class CSequence : public CSingleton< CSequence >
{
	private:
		// 每次预分配的个数
		enum { em_once_block_size = 5000 };

		int m_nMinID;
		int m_nMaxID;
		int m_nStep;

		int m_nCurID;
		int m_nLeft;
	public:
		CSequence(int nMinID, int nMaxID, int nStep): m_nMinID(nMinID), m_nMaxID (nMaxID), m_nStep(nStep)
		{
			m_nLeft = 0;
		}

		~CSequence() {}

		int NextID()
		{
			// 如果没有剩余，则分配
			if ( m_nLeft == 0 )
			{
				int lastid = -1;

				//FILE* fp = fopen("../config/data/gate_config/sequence.cfg", "r+");
				//CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"gate_config/sequence.cfg")
				FILE* fp = fopen(CGateLogic::GetSingletonPtr()->GetConfigDataFileNamePath(CGateLogic::GetSingleton().mConfig.nGameID,"gate_config/sequence.cfg"), "r+");
				if (fp == NULL) return -1;
				if ( fscanf(fp, "%d\n", &lastid) <= 0)  return -1;

				int nextid = lastid + m_nStep * em_once_block_size;

				// 如果超过了最大值, 为简化操作，立即赋值为最小值
				if ( nextid > m_nMaxID || nextid < m_nMinID) 
				{
					nextid = m_nMinID + m_nStep * em_once_block_size;
					lastid = m_nMinID;

					if ( nextid > m_nMaxID || nextid < m_nMinID)
					{
						return -1;
					}
				}

				// 移动到文件的开始位置
				if ( fseek(fp, 0, SEEK_SET) < 0)
				{
					return -1;
				}

				// 将预分配后的nextid写入到文件中
				if ( fprintf(fp, "%010d\n", nextid ) < 0) 
				{
					return -1;
				}

				if ( fclose(fp) != 0 )
				{
					return -1;
				}

				m_nLeft = em_once_block_size;
				m_nCurID = lastid;
			}

			m_nCurID += m_nStep;	// 最小的ID不使用
			m_nLeft --;

			return m_nCurID;
		}
};

#endif
