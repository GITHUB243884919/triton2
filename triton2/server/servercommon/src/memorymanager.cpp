// ********************************************************************** //
// MemoryManager.cpp
// 服务器内存管理器
// CMemoryPiece :			内存片
// CMemoryPoolInterface :	内存池抽象
// CMemoryPool :			内存池实现
// CMemoryManager :			内存池管理器
// Author :					Black
// ********************************************************************** //
#include "malloc.h"
#include "stdio.h"
#include "time.h"
#include <string.h>
#include <new>
#pragma warning( disable  : 4311 )
#pragma warning( disable  : 4312 )
#pragma warning( disable  : 4996 )

class CMemInfo
{
public:
	char*		mpFileName;
	size_t		mLine;
	size_t		mSize;
	int			mAllocID;		// 分配唯一标识符
	CMemInfo*	mpNext;
	CMemInfo*	mpPrev;

public:
	CMemInfo( ) : mpFileName( NULL ), mLine( 0 ), mSize( 0 ), mpNext( NULL ), mpPrev( NULL ), mAllocID( 0 )
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
	CMemoryPiece( ) : mpNext( NULL ), mBlockType( SIZE )
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
};

template< int SIZE, int NUMBER >
class CMemoryPool : public CMemoryPoolInterface
{
public:
	CMemoryPiece< SIZE >		mPool[ NUMBER ];
	CMemoryPiece< SIZE >*		mpHead;
	CMemoryPiece< SIZE >*		mpTail;
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
	static char*				spFileName;
	static char*				spAppName;
	static int					sLine;
	static CMemInfo*			spHead;
	static CMemInfo*			spTail;
	static int					sAllocCount;

public:
	CMemoryManager( ) : mMemUsed( 0 ), mMemCommit( 0 ), mMemCommitTimes( 0 ), mPoolIndex( 0 )
	{
#ifdef MEMPOOL
		RegisterPool( 8,		10000 );
		RegisterPool( 16,		10000 );
		RegisterPool( 32,		10000 );
		RegisterPool( 64,		10000 );
		RegisterPool( 128,		10000 );
		RegisterPool( 256,		10000 );
		RegisterPool( 512,		10000 );
		RegisterPool( 1024, 	10000 );
		RegisterPool( 10240,	1000 );
		RegisterPool( 20480,	1000 );
		RegisterPool( 51200,	1000 );
		RegisterPool( 102400, 	10 );
		RegisterPool( 204800, 	10 );
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

	void SetOwner( char* pFileName, int vLine )
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

	void* Alloc( size_t vSize )
	{
#ifdef MEMLEAK
		CMemInfo* tpMemInfo		= (CMemInfo*) ::malloc( sizeof( CMemInfo ) );
		tpMemInfo->mpFileName	= spFileName;
		tpMemInfo->mLine		= sLine;
		tpMemInfo->mSize		= vSize;
		tpMemInfo->mpNext		= NULL;
		tpMemInfo->mpPrev		= NULL;
		tpMemInfo->mAllocID		= ++ sAllocCount;
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
		if ( tBlockSize == 0xFFFFFFFF )
		{
			free( (char*) pBuffer - 8 );
			return;
		}
#ifdef MEMPOOL
		for ( size_t i = 0; i < mPoolIndex; i ++ )
		{
			if ( tBlockSize == mpPools[ i ]->GetPieceSize( ) )
			{
				mpPools[ i ]->Free( (char*) pBuffer - 8 );
				break;
			}
		}
#endif
	}
};

char*		CMemoryManager::spFileName	= NULL;
char*		CMemoryManager::spAppName	= NULL;
int			CMemoryManager::sLine		= 0;
int			CMemoryManager::sAllocCount = 0;
CMemInfo*	CMemoryManager::spHead		= NULL;
CMemInfo*	CMemoryManager::spTail		= NULL;

CMemoryManager* gpManager = NULL;
void* operator new[] ( size_t vSize )
{
	if ( gpManager == NULL )
		gpManager = new( malloc( sizeof( CMemoryManager ) ) ) CMemoryManager( );

	return gpManager->Alloc( vSize );
}

void* operator new ( size_t vSize )
{
	if ( gpManager == NULL )
		gpManager = new( malloc( sizeof( CMemoryManager ) ) ) CMemoryManager( );

	return gpManager->Alloc( vSize );
}

void operator delete [] ( void* pBuffer )
{
	if ( pBuffer == NULL )
		return;

	gpManager->Free( pBuffer );
	if ( gpManager->mMemCommitTimes == 0 )
		free( gpManager );
}

void operator delete ( void* pBuffer )
{
	if ( pBuffer == NULL )
		return;

	gpManager->Free( pBuffer );
	if ( gpManager->mMemCommitTimes == 0 )
		free( gpManager );
}

void SetAppName( char* pAppName )
{
	if ( gpManager == NULL )
		gpManager = new( malloc( sizeof( CMemoryManager ) ) ) CMemoryManager( );

	gpManager->SetAppName( pAppName );
}

void SetOwner( char* pFileName, int vLine )
{
	if ( gpManager == NULL )
		gpManager = new( malloc( sizeof( CMemoryManager ) ) ) CMemoryManager( );

	gpManager->SetOwner( pFileName, vLine );
}

void Shutdown( )
{
	char tFileName[ 64 ] = { 0 };
	::sprintf( tFileName, "%s_memleak.txt", CMemoryManager::spAppName );
	FILE* tpMemLog = ::fopen( tFileName, "w" );
	time_t tTime = time( NULL );
	tm* tpTime = ::localtime( &tTime );
	char tTimeBuffer[ 4096 ] = { 0 };
	::sprintf( tTimeBuffer, "进程: [%s]内存泄露检查报告，时间[%04d-%02d-%02d %02d:%02d:%02d]\r\n", CMemoryManager::spAppName, 
		1900 + tpTime->tm_year, tpTime->tm_mon, tpTime->tm_mday, tpTime->tm_hour, tpTime->tm_min, tpTime->tm_sec );
	::fwrite( tTimeBuffer, ::strlen( tTimeBuffer ), 1, tpMemLog );
	
	char tTextBuffer[ 1024 ] = { 0 };
	if ( CMemoryManager::spHead == NULL )
	{
		sprintf( tTextBuffer, "恭喜，没有发现内存泄露\r\n" );
		::fwrite( tTextBuffer, ::strlen( tTextBuffer ), 1, tpMemLog );
		printf( tTextBuffer );
	}

	for ( CMemInfo* tpIter = CMemoryManager::spHead; tpIter != NULL; tpIter = tpIter->mpNext )
	{
		if ( tpIter->mpFileName == NULL )
			sprintf( tTextBuffer, "未知内存泄露, 很有可能发生在stl内部，请检查stl的使用, 内存长度[%d字节], 分配唯一标示符[%d]\r\n", tpIter->mSize, tpIter->mAllocID );
		else
			sprintf( tTextBuffer, "找到内存泄露在文件[%s]的第[%d行], 内存长度[%d字节], 分配唯一标示符[%d]\r\n", tpIter->mpFileName, tpIter->mLine, tpIter->mSize, tpIter->mAllocID );

		printf( tTextBuffer );
		::fwrite( tTextBuffer, ::strlen( tTextBuffer ), 1, tpMemLog );
	}
	const char* tpInfoTag = "参考日志文件memleak.txt\r\n";
	::printf( tpInfoTag );

	const char* tpEndTag = "\r\n\t\t\t\t\t\t\t\treport by black\r\n";
	::fwrite( tpEndTag, ::strlen( tpEndTag ), 1, tpMemLog );
	::fclose( tpMemLog );
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
