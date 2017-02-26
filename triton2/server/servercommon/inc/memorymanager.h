#pragma once
void SetOwner( char* pFileName, int vLine );
void SetAppName( char* pAppName );
void Shutdown( );

void* operator new[] ( size_t vSize );
void* operator new ( size_t vSize );
void operator delete [] ( void* pBuffer );
void operator delete ( void* pBuffer );
#define new ( SetOwner( __FILE__, __LINE__ ), false ) ? 0 : new

size_t GetMemoryUsed( );
size_t GetMemoryCommit( );
size_t GetMemoryTimes( );
int GetMemoryPieceUsed( size_t vSize );
