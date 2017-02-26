#pragma once

typedef unsigned char BYTE;

unsigned int MakeKey( const char* pFile, int vId );
BYTE* CreateShareMem( unsigned int vKey, int vSize );
int DestroyShareMem( unsigned int vKey );
 
