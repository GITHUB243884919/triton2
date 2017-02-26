#ifndef _SYS_MEMORY_H__
#define _SYS_MEMORY_H__
#include "Macro.h"

#include <vector>
using namespace std;

class Engine_Export SysMemory
{
    typedef struct STRUCT_POINTER 
    {
        void        *pPointer;
        uint32_t    unSize;
        int32_t     nIsFree;
    }POINTER;

    typedef vector<POINTER> POINTERLIST;
public:
	SysMemory(void);
    virtual int initialize();
    int allocateMemory(uint32_t unSize, void **ppNewMemory);
    int freeMemory(const void *pMemoryToFree);
    int clear();
    virtual int release();
public:
	virtual ~SysMemory(void);
private:
    POINTERLIST m_pointerList;
};


#endif
