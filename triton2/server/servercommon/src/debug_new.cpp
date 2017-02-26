/*
 * debug_new.cpp  1.11 2003/07/03
 *
 * Implementation of debug versions of new and delete to check leakage
 *
 * By Wu Yongwei
 *
 */

#ifdef LEAK_CHK_DEBUG_NEW // 开关


#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include <execinfo.h>


#ifdef _MSC_VER
#pragma warning(disable: 4073)
#pragma init_seg(lib)
#endif

#ifndef DEBUG_NEW_HASHTABLESIZE
#define DEBUG_NEW_HASHTABLESIZE 16384
#endif

#ifndef DEBUG_NEW_HASH
#define DEBUG_NEW_HASH(p) (((unsigned)(p) >> 8) % DEBUG_NEW_HASHTABLESIZE)
#endif

// The default behaviour now is to copy the file name, because we found
// that the exit leakage check cannot access the address of the file
// name sometimes (in our case, a core dump will occur when trying to
// access the file name in a shared library after a SIGINT).
#ifndef DEBUG_NEW_FILENAME_LEN
#define DEBUG_NEW_FILENAME_LEN	1024
#endif
#if DEBUG_NEW_FILENAME_LEN == 0 && !defined(DEBUG_NEW_NO_FILENAME_COPY)
#define DEBUG_NEW_NO_FILENAME_COPY
#endif
#ifndef DEBUG_NEW_NO_FILENAME_COPY
#include <string.h>
#endif

// 线程互斥信号量
static pthread_mutex_t __mutex_debug_new;	


bool new_verbose_flag = false;
bool new_autocheck_flag = true;
bool new_backtrace_flag = false;



////////////////////


class CMemInfo
{
public:
	const char*	mpFileName;
	size_t		mLine;
	size_t		mSize;
	int			mAllocID;		// 分配唯一标识符
	CMemInfo*	mpNext;
	CMemInfo*	mpPrev;

public:
	CMemInfo( ) : mpFileName( NULL ), mLine( 0 ), mSize( 0 ),  mAllocID( 0 ), mpNext( NULL ), mpPrev( NULL )
	{
	}
};

template< int SIZE >
class CMemoryPiece
{
public:
	int						mBlockType;
	size_t					mUsedSize;
	char					mBuffer[ SIZE ];
	CMemoryPiece< SIZE >*	mpNext;

public:
	CMemoryPiece( ) :  mBlockType( SIZE ), mpNext( NULL )
	{
	}
};

class CMemoryPoolInterface
{
public:
	virtual void* Alloc( size_t vSize ) = 0;
	virtual void Free( void* pBuffer )	= 0;
	virtual bool OutOfSpace( )			= 0;
	virtual size_t GetPieceSize( )		= 0;
	virtual int GetUsedCount( )			= 0;
	virtual void Show()					= 0;
};

template< int SIZE, int NUMBER >
class CMemoryPool : public CMemoryPoolInterface
{
	typedef CMemoryPiece< SIZE > TYPE;
public:
	TYPE		mPool[ NUMBER ];
	TYPE*		mpHead;
	TYPE*		mpTail;
	int							mUseCount;

public:
	CMemoryPool( ) : mUseCount( 0 )
	{
		mpHead = &mPool[ 0 ];
		mpTail = &mPool[ NUMBER - 1 ];
		for ( int i = 0; i < NUMBER; i ++ )
		{
			if ( i == NUMBER - 1 )
				mPool[ i ].mpNext = NULL;
			else
				mPool[ i ].mpNext = &mPool[ i + 1 ];
		}
	}

public:
	virtual void* Alloc( size_t vSize )
	{
		// 如果空间不够了
		if ( OutOfSpace( ) == true )
			return NULL;

		mUseCount ++;
		void* tpMem = mpHead->mBuffer;
		mpHead->mBlockType	= SIZE;
		mpHead->mUsedSize	= vSize;
		mpHead = mpHead->mpNext;
		return tpMem;
	}

	virtual void Free( void* pBuffer )
	{
		mUseCount --;
		mpTail->mpNext = (CMemoryPiece< SIZE >*) pBuffer;
		mpTail->mpNext->mpNext = NULL;
		mpTail = mpTail->mpNext;
	}

	virtual void Show( )
	{
		FILE* tpMemLog = ::fopen( "./memleak.txt", "a+" );
		
		fprintf( tpMemLog, "---memory pool[size:%d]---\n", NUMBER );

		TYPE* p = mpHead;	
		while( p != NULL )
		{
			fprintf( tpMemLog, "%d\n", p->mUsedSize );
		}

		fclose( tpMemLog );
	}

	virtual bool OutOfSpace( )		{ return mpHead == mpTail; }

	virtual size_t GetPieceSize( )	{ return SIZE; }

	virtual int GetUsedCount( )		{ return mUseCount; }
};

#define RegisterPool( vSize, vNumber )													\
{																					\
	char* tpPool = (char*) malloc( sizeof( CMemoryPool< vSize, vNumber > ) );		\
	mpPools[ mPoolIndex ++ ]	= new( tpPool ) CMemoryPool< vSize, vNumber >( );	\
	mpPools[ mPoolIndex ]		= NULL;												\
}																					\

class CMemoryManager
{
public:
	CMemoryPoolInterface*		mpPools[ 64 ];
	size_t						mPoolIndex;
	size_t						mMemCommit;			// 提交请求的内存
	size_t						mMemUsed;			// 实际使用的内存
	size_t						mMemCommitTimes;	// 内存提交次数
	static const char*			spFileName;
	static char*				spAppName;
	static int					sLine;
	static CMemInfo*			spHead;
	static CMemInfo*			spTail;
	static int					sAllocCount;

public:
	CMemoryManager( ) :  mPoolIndex( 0 ), mMemCommit( 0 ), mMemUsed( 0 ), mMemCommitTimes( 0 )
	{
#ifdef MEMPOOL
		RegisterPool( 8,		10000 );
		RegisterPool( 16,		100000 );
		RegisterPool( 32,		100000 );
		RegisterPool( 64,		50000 );
		RegisterPool( 128,		10000 );
		RegisterPool( 256,		10000 );
		RegisterPool( 512,		5000 );
		RegisterPool( 1024, 	5000 );
		RegisterPool( 10240,	1000 );
		RegisterPool( 20480,	100 );
		RegisterPool( 51200,	100 );
		RegisterPool( 102400, 	10 );
		RegisterPool( 204800, 	10 );
		RegisterPool( 2048000, 	5 );
#endif
	}
public:
	~CMemoryManager( )
	{
#ifdef MEMPOOL
		for ( size_t i = 0; i < mPoolIndex; i ++ )
			free( mpPools[ i ] );
#endif
	}

public:
	void SetAppName( char* pAppName )
	{
		spAppName	= pAppName;
	}

	void SetOwner( const char* pFileName, int vLine )
	{
		spFileName	= pFileName;
		sLine		= vLine;
	}

	size_t GetPieceUsed( size_t vSize )
	{
		// 从已经定义的内存池中分配内存
		for ( size_t i = 0; i < mPoolIndex; i ++ )
		{
			if ( vSize == mpPools[ i ]->GetPieceSize( ) )
				return mpPools[ i ]->GetUsedCount( );
		}

		return 0;
	}

	void ShowAllPieceUsed( )
	{
		FILE* pLeak = fopen( "./memleak.txt", "a+");
		time_t tTime = time( NULL );
		tm* tpTime = ::localtime( &tTime );
		::fprintf( pLeak, "[%04d-%02d-%02d %02d:%02d:%02d]\n", 
			1900 + tpTime->tm_year, tpTime->tm_mon+1, tpTime->tm_mday, tpTime->tm_hour, tpTime->tm_min, tpTime->tm_sec );

		for ( size_t i = 0; i < mPoolIndex; i ++ )
		{		
			fprintf( pLeak, "Pool[Size:%d] used: %d\n", mpPools[ i ]->GetPieceSize( ),mpPools[ i ]->GetUsedCount( ) );
		}
		fclose( pLeak );
	}

	void* Alloc( size_t vSize )
	{
#ifdef MEMLEAK
		CMemInfo* tpMemInfo		= (CMemInfo*) ::malloc( sizeof( CMemInfo ) );
		tpMemInfo->mpFileName	= spFileName;
		tpMemInfo->mLine		= sLine;
		tpMemInfo->mSize		= vSize;
		tpMemInfo->mpNext		= NULL;
		tpMemInfo->mpPrev		= NULL;
		//tpMemInfo->mAllocID		= ++ sAllocCount;
		spFileName				= NULL;
		sLine					= 0;
		vSize					= vSize + 4;
		if ( spHead == NULL )
		{
			spHead = tpMemInfo;
			spTail = tpMemInfo;
		}
		else
		{
			spTail->mpNext = tpMemInfo;
			tpMemInfo->mpPrev = spTail;
			spTail = tpMemInfo;
		}
#endif
		mMemCommit		+= vSize;
		mMemCommitTimes ++;
		int* pBuffer	= NULL;
		// 从已经定义的内存池中分配内存
#ifdef MEMPOOL
		for ( size_t i = 0; i < mPoolIndex; i ++ )
		{
			if ( vSize < mpPools[ i ]->GetPieceSize( ) && mpPools[ i ]->OutOfSpace( ) == false )
			{
				mMemUsed	+= mpPools[ i ]->GetPieceSize( );
				pBuffer		= (int*) mpPools[ i ]->Alloc( vSize );
#ifdef MEMLEAK
				*pBuffer	= (int) tpMemInfo;
				tpMemInfo->mAllocID		= (int)(pBuffer + 1);
				return pBuffer + 1;
#endif
				return pBuffer;
			}
		}
#endif
		// 如果内存空间不足，那么使用系统函数
		// 多分配8个字节,用以记录因为malloc分配的
		char* tpBuffer			= (char*) ::malloc( vSize + 8 );
		*(int*)( tpBuffer )		= 0xFFFFFFFF;
		*(int*)( tpBuffer + 4 ) = (int) vSize;
		mMemUsed += vSize;

#ifdef MEMLEAK
		pBuffer		= (int*)( tpBuffer + 8 );
		*pBuffer	= (int) tpMemInfo;
		return pBuffer + 1;
#endif
		return tpBuffer + 8;
	}

	void Free( void* pBuffer )
	{
#ifdef MEMLEAK
		pBuffer = (char*) pBuffer - 4;
		CMemInfo* tpMemInfo = (CMemInfo*)( *(int*) pBuffer );
		CMemInfo* tpPrev = tpMemInfo->mpPrev;
		CMemInfo* tpNext = tpMemInfo->mpNext;
		if ( tpPrev == NULL )
		{
			spHead = tpNext;
			if ( spHead == NULL )
				spTail = NULL;
			else
				tpNext->mpPrev = NULL;
		}
		else if ( tpNext == NULL )
		{
			spTail = tpPrev;
			if ( spTail == NULL )
				spHead = NULL;
			else
				tpPrev->mpNext = NULL;
		}
		else
		{
			tpPrev->mpNext = tpNext;
			tpNext->mpPrev = tpPrev;
		}

		free( tpMemInfo );
#endif
		mMemCommitTimes --;
		int tBlockSize	= *(int*) ( (char*) pBuffer - 8 );
		int tMemSize	= *(int*) ( (char*) pBuffer - 4 );
		mMemCommit		-= tBlockSize;
		mMemUsed		-= tMemSize;
		if ( tBlockSize == (int)0xFFFFFFFF )
		{
			free( (char*) pBuffer - 8 );
			return;
		}
#ifdef MEMPOOL
		for ( size_t i = 0; i < mPoolIndex; i ++ )
		{
			if ( tBlockSize == (int)mpPools[ i ]->GetPieceSize( ) )
			{
				mpPools[ i ]->Free( (char*) pBuffer - 8 );
				break;
			}
		}
#endif
	}

	void Show( )
	{
#ifdef MEMPOOL
		for ( size_t i = 0; i < mPoolIndex; i ++ )
		{
			
			mpPools[ i ]->GetUsedCount();
			
			mpPools[ i ]->Show( ); 
		}
#endif
	}
};

const char*	CMemoryManager::spFileName	= NULL;
char*		CMemoryManager::spAppName	= NULL;
int			CMemoryManager::sLine		= 0;
int			CMemoryManager::sAllocCount = 0;
CMemInfo*	CMemoryManager::spHead		= NULL;
CMemInfo*	CMemoryManager::spTail		= NULL;

CMemoryManager* gpManager = NULL;

void* operator new(size_t size, const char* file, int line)
{
	pthread_mutex_lock( &__mutex_debug_new );

	if ( gpManager == NULL )
		gpManager = new( malloc( sizeof( CMemoryManager ) ) ) CMemoryManager( );

	gpManager->SetOwner(file, line);

	void* pointer = gpManager->Alloc( size );

	if (new_verbose_flag)
		printf("new:  allocated  %p (size %u, %s:%d)\n", pointer, size, file, line);

	pthread_mutex_unlock( &__mutex_debug_new );

	return pointer;
}

void* operator new[](size_t size, const char* file, int line)
{
	return operator new(size, file, line);
}

void* operator new(size_t size)
{
	return operator new(size, "<Unknown>", 0);
}

void* operator new[](size_t size)
{
	return operator new(size);
}

void* operator new(size_t size, const std::nothrow_t&) throw()
{
	return operator new(size);
}

void* operator new[](size_t size, const std::nothrow_t&) throw()
{
	return operator new[](size);
}

void operator delete(void* pointer)
{
	if (pointer == NULL)
	{
		return;
	}

	pthread_mutex_lock( &__mutex_debug_new );

	if (new_verbose_flag)
	{
		FILE* pLeak = fopen( "./memleak.txt", "a+");

		CMemInfo* tpMemInfo = (CMemInfo*)( * ((int*)pointer - 1) );
		fprintf( pLeak, "delete: freeing  %p (size %u)\n", pointer, tpMemInfo->mSize );
		fclose( pLeak );
	}

	gpManager->Free( pointer );

	pthread_mutex_unlock( &__mutex_debug_new );
}

void operator delete[](void* pointer)
{
	operator delete(pointer);
}


// Some older compilers like Borland C++ Compiler 5.5.1 and Digital Mars
// Compiler 8.29 do not support placement delete operators.
// NO_PLACEMENT_DELETE needs to be defined when using such compilers.
// Also note that in that case memory leakage will occur if an exception
// is thrown in the initialization (constructor) of a dynamically
// created object.
#ifndef NO_PLACEMENT_DELETE
void operator delete(void* pointer, const char* file, int line)
{
	if (new_verbose_flag)
		printf("info: exception thrown on initializing object at %p (%s:%d)\n",
		pointer, file, line);
	operator delete(pointer);
}

void operator delete[](void* pointer, const char* file, int line)
{
	operator delete(pointer, file, line);
}

void operator delete(void* pointer, const std::nothrow_t&)
{
	operator delete(pointer, "<Unknown>", 0);
}

void operator delete[](void* pointer, const std::nothrow_t&)
{
	operator delete(pointer, std::nothrow);
}
#endif // NO_PLACEMENT_DELETE


void SetAppName( char* pAppName )
{
	gpManager->SetAppName( pAppName );
}

void SetOwner( char* pFileName, int vLine )
{
	gpManager->SetOwner( pFileName, vLine );
}

bool CheckLeak( )
{
	FILE* tpMemLog = ::fopen( "./memleak.txt", "a+" );
	time_t tTime = time( NULL );
	tm* tpTime = ::localtime( &tTime );
	fprintf( tpMemLog, "[%04d-%02d-%02d %02d:%02d:%02d]\n", 
		1900 + tpTime->tm_year, tpTime->tm_mon+1, tpTime->tm_mday, tpTime->tm_hour, tpTime->tm_min, tpTime->tm_sec );

	bool bLeaked = false;
	for ( CMemInfo* tpIter = CMemoryManager::spHead; tpIter != NULL; tpIter = tpIter->mpNext )
	{
		bLeaked = true;
		fprintf( tpMemLog, "leaked object at 0x%x (size %u, %s:%d)\n", tpIter->mAllocID, tpIter->mSize , tpIter->mpFileName, tpIter->mLine );
	}

	::fclose( tpMemLog );

	gpManager->ShowAllPieceUsed( );

	return bLeaked;
}

// 内存实际使用量
size_t GetMemoryUsed( )
{
	if ( gpManager == NULL )
		return 0;

	return gpManager->mMemUsed;
}

// 内存提交使用量
size_t GetMemoryCommit( )
{
	if ( gpManager == NULL )
		return 0;

	return gpManager->mMemCommit;
}

// 内存提交次数
size_t GetMemoryTimes( )
{
	if ( gpManager == NULL )
		return 0;

	return gpManager->mMemCommitTimes;
}

int GetMemoryPieceUsed( size_t vSize )
{
	if ( gpManager == NULL )
		return 0;

	return (int) gpManager->GetPieceUsed( vSize );
}

// 打印内存数据
void Show( )
{
	if ( gpManager == NULL )
		return;

	gpManager->ShowAllPieceUsed( );

	//return gpManager->Show();
}

// Proxy class to automatically call check_leaks if new_autocheck_flag is set
class new_check_t
{
public:
	new_check_t() {}
	~new_check_t()
	{
		if (new_autocheck_flag)
		{
			// Check for leakage.
			// If any leaks are found, set new_verbose_flag so that any
			// delete operations in the destruction of global/static
			// objects will display information to compensate for
			// possible false leakage reports.
			if (CheckLeak())
				new_verbose_flag = true;
		}
	}
};
static new_check_t new_check_object;


#endif // !LEAK_CHK_DEBUG_NEW 