#ifndef __MACRO_H__
#define __MACRO_H__
#include <assert.h>
#include <stdio.h>
#define SYS_MEMORY_INIT_HANDLERS 1024

#include "OS.h"
#include "OperationCode.h"
#define FILE_MAX_PATH   256
#define MAX_BUFFER_SIZE 1024

//#define HPUX
//#define LINUX
#ifndef ASSERT
#define ASSERT assert
#endif

#ifdef WIN32
#define Engine_Export __declspec(dllexport)
#else
#define Engine_Export
#endif

#ifndef NULL
#define NULL 0
#endif


#define THR_NEW_LWP         0
#define THR_JOINABLE        0
#define THR_INHERIT_SCHED   0

#if !defined (DEFAULT_THREAD_PRIORITY)
#  define DEFAULT_THREAD_PRIORITY (-0x7fffffffL - 1L)
#endif /* DEFAULT_THREAD_PRIORITY */


/////////////////////////////////////////////////////////////
//     Operation Code Definition
/////////////////////////////////////////////////////////////
#define LOGGER SysLogManager::output

#define PROCESS_CHECK_ERROR(X) do{if (!(X)){goto ExitError;}}while(false)
#define PROCESS_CHECK_OK(X)    do{if (X){goto ExitOK;}}while(false)
#define PROCESS_CHECK_ERROR_RET(X, RET, RET_CODE) do{if (!(X)){RET = RET_CODE; goto ExitError;}}while(false)



#endif
