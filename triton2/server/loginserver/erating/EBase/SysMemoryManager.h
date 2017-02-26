#ifndef _SYS_MEMORY_MANAGER_H__
#define _SYS_MEMORY_MANAGER_H__

#define MEMORY_MAX_HANDLER 1024
#include "Macro.h"

class Engine_Export SysMemoryManager
{
public:
	SysMemoryManager(void);
	virtual int initialize() = 0;
	inline int setStepSize(int nStepSize)
	{
		m_nStepSize = nStepSize;
        return S_SUCCESS;
	};
public:
	virtual ~SysMemoryManager(void);
private:
	int m_nStepSize;

};

#endif
