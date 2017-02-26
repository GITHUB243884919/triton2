// ********************************************************************** //
// ServerTool.cpp
// 服务器使用的工具类
// CWTPoint:			坐标
// CWTRect:				矩形
// CWTSize:				尺寸 
// CTlv :				CTlv数据维护 
// CVariant :			变体
// CWTimer :			计时器
// CDispParams :		参数表
// CCommandHandler:		服务器命令处理Handler类
// CCommandCenter:		服务器命令处理中心
// Author :				Black
// ********************************************************************** //
#include "stdafx.hpp"
#include "servertool.h"

// ************************************************************ //
// CLogManager
// ************************************************************ //
template<> CLogManager* CSingleton< CLogManager >::spSingleton = NULL;

//void CLogManager::SystemLog( char* pFormat, ... )
//{
//	time_t	tTime	= time( NULL );
//	tm*		tpTime	= ::localtime( &tTime );
//	char tTimeBuffer[ 2048 ];
//	::sprintf( tTimeBuffer, "[%04d-%02d-%02d %02d:%02d:%02d]", 1900 + tpTime->tm_year, tpTime->tm_mon + 1, tpTime->tm_mday,
//		tpTime->tm_hour, tpTime->tm_min, tpTime->tm_sec );
//
//	::fwrite( tTimeBuffer, ::strlen( tTimeBuffer ), 1, mpSystemLog );
//	char tText[ 2048 ];
//
//	va_list tArgs;
//
//	va_start( tArgs, pFormat );
//	::vsprintf( tText, pFormat, tArgs );
//	va_end( pFormat );
//	
//	::fwrite( tText, ::strlen( tText ), 1, mpSystemLog );
//	::fwrite( "\r\n", 2, 1, mpSystemLog );
//	::fflush( mpSystemLog );
//}
//
//void CLogManager::LogicLog( char* pFormat, ... )
//{
//	time_t	tTime	= time( NULL );
//	tm*		tpTime	= ::localtime( &tTime );
//	char tTimeBuffer[ 2048 ];
//	::sprintf( tTimeBuffer, "[%d-%d-%d %d:%d:%d]", 1900 + tpTime->tm_year, tpTime->tm_mon + 1, tpTime->tm_mday,
//		tpTime->tm_hour, tpTime->tm_min, tpTime->tm_sec );
//
//	::fwrite( tTimeBuffer, ::strlen( tTimeBuffer ), 1, mpLogicLog );
//	char tText[ 2048 ];
//
//	va_list tArgs;
//
//	va_start( tArgs, pFormat );
//	::vsprintf( tText, pFormat, tArgs );
//	va_end( pFormat );
//	
//	::fwrite( tText, ::strlen( tText ), 1, mpLogicLog );
//	::fwrite( "\r\n", 2, 1, mpLogicLog );
//	::fflush( mpLogicLog );
//}

// ************************************************************ //
// CTlv
// ************************************************************ //
int CTlv::PopBuffer( const char* pTagName, char*& pBuffer, int& rSize )
{
	char* tpStart = mpBuffer;
	while( tpStart < mpBuffer + mBufferSize && *tpStart != 0 )
	{
		int tBuffLen = *(int*)( tpStart + TLV_TAGLEN );
		if ( ::memcmp( tpStart, pTagName, TLV_TAGLEN ) == 0 )
		{
			rSize = tBuffLen;
			pBuffer = tpStart + TLV_TAGLEN + sizeof( int );
			return CTlv::ERROR_OK;
		}
		else
		{
			tpStart += TLV_TAGLEN + sizeof( int ) + tBuffLen;
		}
	}

	return CTlv::ERROR_TAGNOTFOUND;
}

int CTlv::PushBuffer( const char* pTagName, const char* pBuffer, int tSize )
{
	if ( ::strlen( pTagName ) >= 16 )
		return CTlv::ERROR_TAGOVERRUN;

	char* tpStart = mpBuffer + mPushPointer;
	char tTagName[ TLV_TAGLEN ] = { 0 };
	::strncpy( tTagName, pTagName, TLV_TAGLEN );

	::memcpy( tpStart, tTagName, TLV_TAGLEN );	tpStart += TLV_TAGLEN;
	::memcpy( tpStart, &tSize, sizeof( int ) );	tpStart += sizeof( int );
	::memcpy( tpStart, pBuffer, tSize );
	mPushPointer += TLV_TAGLEN + sizeof( int ) + tSize;
	return CTlv::ERROR_OK;
}

// ************************************************************ //
// CWTSize
// ************************************************************ //

CWTPoint CWTSize::operator - ( const CWTPoint& rPoint ) const
{
	return rPoint - *this; 
}

CWTRect CWTSize::operator - ( const CWTRect& rRect ) const
{
	return rRect - *this;	
}

CWTPoint CWTSize::operator + ( const CWTPoint& rPoint ) const
{
	return rPoint + *this; 
}

CWTRect CWTSize::operator + ( const CWTRect& rRect ) const
{
	return rRect + *this; 
}

// ************************************************************ //
// CWTPoint
// 2D 坐标
// ************************************************************ //

CWTRect CWTPoint::operator - ( const CWTRect& rRect ) const	
{
	return rRect - *this;
}

CWTRect CWTPoint::operator + ( const CWTRect& rRect ) const	
{
	return rRect + *this;
}

// ************************************************************ //
// CWTRect
// 2D 矩形区域
// ************************************************************ //

bool CWTRect::IntersectRect( const CWTRect& rSrcRect1, const CWTRect& rSrcRect2 )
{
	if ( rSrcRect1.IsRectEmpty( ) || rSrcRect2.IsRectEmpty( ) )
		return false;

	int tLeft	= std::max( rSrcRect1.mTopLeft.mX, rSrcRect2.mTopLeft.mX );
	int tTop	= std::max( rSrcRect1.mTopLeft.mY, rSrcRect2.mTopLeft.mY );
	int tRight	= std::min( rSrcRect1.mBottomRight.mX, rSrcRect2.mBottomRight.mX );
	int tBottom	= std::min( rSrcRect1.mBottomRight.mY, rSrcRect2.mBottomRight.mY );

	if ( CWTRect( tLeft, tTop, tRight, tBottom, false ).IsRectEmpty( ) )
	{
		SetRectEmpty( );
		return false;
	}

	mTopLeft.mX		= tLeft;
	mTopLeft.mY		= tTop;
	mBottomRight.mX = tRight;
	mBottomRight.mY = tBottom;
	return true;
}

void CWTRect::NormalizeRect( )
{
	mTopLeft.mX		= std::min( mTopLeft.mX, mBottomRight.mX );
	mTopLeft.mY		= std::min( mTopLeft.mY, mBottomRight.mY );
	mBottomRight.mX	= std::max( mTopLeft.mX, mBottomRight.mX );
	mBottomRight.mY	= std::max( mTopLeft.mY, mBottomRight.mY );
}

bool CWTRect::PtInRect( const CWTPoint& rPoint ) const
{
	if ( IsRectEmpty( ) )
		return false;

	if ( rPoint.mX < mTopLeft.mX || rPoint.mX > mBottomRight.mX )
		return false;

	if ( rPoint.mY < mTopLeft.mY || rPoint.mY > mBottomRight.mY )
		return false;

	return true;
}

bool CWTRect::UnionRect( const CWTRect& rSrcRectl, const CWTRect& rSrcRect2 )
{
	int tLeft	= rSrcRectl.mTopLeft.mX < rSrcRect2.mTopLeft.mX ? rSrcRectl.mTopLeft.mX : rSrcRect2.mTopLeft.mX;
	int tTop	= rSrcRectl.mTopLeft.mY < rSrcRect2.mTopLeft.mY ? rSrcRectl.mTopLeft.mY : rSrcRect2.mTopLeft.mY;
	int tRight	= rSrcRectl.mBottomRight.mX > rSrcRect2.mBottomRight.mX ? rSrcRectl.mBottomRight.mX : rSrcRect2.mBottomRight.mX;
	int tBottom	= rSrcRectl.mBottomRight.mY > rSrcRect2.mBottomRight.mY ? rSrcRectl.mBottomRight.mY : rSrcRect2.mBottomRight.mY;

	mTopLeft.mX		= tLeft;
	mTopLeft.mY		= tTop;
	mBottomRight.mX = tRight;
	mBottomRight.mY = tBottom;
	return true;
}

int CWTRect::SubtractRect( const CWTRect& rDesRect, CWTRect* pRectList )
{
	CWTRect tRectResult;
	if ( tRectResult.IntersectRect( *this, rDesRect ) == false )
	{
		pRectList[ 0 ] = rDesRect;
		return 1;
	}

	CWTRect tTopRect;
	tTopRect.mTopLeft			= rDesRect.mTopLeft;
	tTopRect.mBottomRight.mX	= rDesRect.mBottomRight.mX;
	tTopRect.mBottomRight.mY	= mTopLeft.mY - 1;
	
	CWTRect tLeftRect;
	tLeftRect.mTopLeft.mX		= rDesRect.mTopLeft.mX;
	tLeftRect.mTopLeft.mY		= std::max( mTopLeft.mY, rDesRect.mTopLeft.mY );
	tLeftRect.mBottomRight.mX	= mTopLeft.mX - 1;
	tLeftRect.mBottomRight.mY	= std::min( mBottomRight.mY, rDesRect.mBottomRight.mY );

	CWTRect tBottomRect;
	tBottomRect.mTopLeft.mX		= rDesRect.mTopLeft.mX;
	tBottomRect.mTopLeft.mY		= mBottomRight.mY + 1;
	tBottomRect.mBottomRight	= rDesRect.mBottomRight;
	
	CWTRect tRightRect;
	tRightRect.mTopLeft.mX		= mBottomRight.mX + 1;
	tRightRect.mTopLeft.mY		= std::max( mTopLeft.mY, rDesRect.mTopLeft.mY );
	tRightRect.mBottomRight.mX	= rDesRect.mBottomRight.mX;
	tRightRect.mBottomRight.mY	= std::min( mBottomRight.mY, rDesRect.mBottomRight.mY );
	int tRectIndex = 0;
	if ( tTopRect.IsRectEmpty( ) == false )		
		pRectList[ tRectIndex ++ ] = tTopRect;

	if ( tLeftRect.IsRectEmpty( ) == false )	
		pRectList[ tRectIndex ++ ] = tLeftRect;

	if ( tBottomRect.IsRectEmpty( ) == false )	
		pRectList[ tRectIndex ++ ] = tBottomRect;

	if ( tRightRect.IsRectEmpty( ) == false )	
		pRectList[ tRectIndex ++ ] = tRightRect;

	return tRectIndex;
}

// ********************************************************************** //
// CExceptionManager
// ********************************************************************** //
#ifdef WIN32
void* CExceptionManager::spPreviousFilter	= 0;
void* CExceptionManager::spProcessHandle	= 0;
char CExceptionManager::mErrorDesc[ 8192 ];

void CExceptionManager::Initialize( )
{
	::memset( mErrorDesc, 0, sizeof( mErrorDesc ) );
	spPreviousFilter = ::SetUnhandledExceptionFilter( (LPTOP_LEVEL_EXCEPTION_FILTER) ExceptionFilter );
	spProcessHandle	 = ::GetCurrentProcess( );
}

void CExceptionManager::Finalize( )
{
	::SetUnhandledExceptionFilter( (LPTOP_LEVEL_EXCEPTION_FILTER) spPreviousFilter );
}

int CALLBACK ErrorWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	SIZE tSize;
	int tAlignX = 0, tAlignY = 0;
	switch( uMsg )
	{
	case WM_CREATE:
		{
			break;			
		}
	case WM_PAINT:
		{
			PAINTSTRUCT tPaint;
			BeginPaint( hWnd, &tPaint );
			for ( int i = 0; i < int( ::strlen( CExceptionManager::mErrorDesc ) ); i ++ )
			{
				if ( CExceptionManager::mErrorDesc[ i ] == '\r' && CExceptionManager::mErrorDesc[ i + 1 ] == '\n' )
				{
					::GetTextExtentPointA( tPaint.hdc, (char*) "Test", 4, &tSize );
					tAlignY += tSize.cy;
					tAlignX = 0;
					i ++;
					continue;
				}

				if ( CExceptionManager::mErrorDesc[ i ] & 0x80 )
				{
					TextOutA( tPaint.hdc, tAlignX, tAlignY, (char*) &CExceptionManager::mErrorDesc[ i ], 2 );
					::GetTextExtentPointA( tPaint.hdc, (char*) &CExceptionManager::mErrorDesc[ i ], 2, &tSize );
					tAlignX += tSize.cx;
					i ++;
				}
				else
				{
                    TextOutA( tPaint.hdc, tAlignX, tAlignY, (char*) &CExceptionManager::mErrorDesc[ i ], 1 );
					::GetTextExtentPointA( tPaint.hdc, (char*) &CExceptionManager::mErrorDesc[ i ], 1, &tSize );
					tAlignX += tSize.cx;
				}
			}
			EndPaint( hWnd, &tPaint );
			return 0;
		}
	}

	return int( DefWindowProc( hWnd, uMsg, wParam, lParam ) );
}

long CExceptionManager::ExceptionFilter( void* pExceptionInfo )
{
	EXCEPTION_POINTERS*	tpExceptionPointers	= (EXCEPTION_POINTERS*) pExceptionInfo;
	EXCEPTION_RECORD*	tpExceptionRecord	= tpExceptionPointers->ExceptionRecord;
	CONTEXT*			tpContext			= tpExceptionPointers->ContextRecord;

	//CDiagnoseManager::Output( NULL, "Exception Code : %08X (%s)\r\n",
	//	tpExceptionRecord->ExceptionCode, GetExceptionString( tpExceptionRecord->ExceptionCode ) );

	//CDiagnoseManager::Output( NULL, "Fault address  : %08X\r\n", tpExceptionRecord->ExceptionAddress );

	char tModuleName[1024] = { 0 }; int tSection = 0, tOffset = 0;
	if ( GetLogicalAddress( tpExceptionRecord->ExceptionAddress, tModuleName, 1024, &tSection, &tOffset ) )
	{
		char tBuffer[ 1024 ] = { 0 };
		::sprintf( tBuffer, "Fault Module   : %02X:%08X (%s)\r\n\r\n", tSection, tOffset, tModuleName );
		::strcat( CExceptionManager::mErrorDesc, tBuffer );
	}

	::SymSetOptions( SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME );

	if ( !::SymInitialize( spProcessHandle, 0, true ) )
	{
		::strcat( CExceptionManager::mErrorDesc, "Can Not Find Debug Information\r\n" );

		return EXCEPTION_EXECUTE_HANDLER;
	}

	::strcat( CExceptionManager::mErrorDesc, "============================================================\r\n" );
	::strcat( CExceptionManager::mErrorDesc, "Call Stack( [Address][Frame][Function][SourceFile] )\r\n" );
	::strcat( CExceptionManager::mErrorDesc, "============================================================\r\n\r\n" );

	CONTEXT tTrashableContext = *tpContext;
	DumpStack( &tTrashableContext );

	::SymCleanup( spProcessHandle );
	
	SYSTEMTIME tTime;
	::GetSystemTime( &tTime );
	char tFileName[ 128 ] = { 0 };
	::sprintf( tFileName, "logs\\Crash_%d-%d-%d.txt", tTime.wYear, tTime.wMonth, tTime.wDay );
	HANDLE tFile = ::CreateFileA( tFileName, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	DWORD tBytesWritten = 0;
	::WriteFile( tFile, mErrorDesc, DWORD( ::strlen( mErrorDesc ) ), &tBytesWritten, NULL );
	::CloseHandle( tFile );

	WNDCLASSEXA tWndClass;
	tWndClass.cbSize		= sizeof(WNDCLASSEX); 
	tWndClass.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	tWndClass.lpfnWndProc	= (WNDPROC)ErrorWndProc;
	tWndClass.cbClsExtra	= 0;
	tWndClass.cbWndExtra	= 0;
	tWndClass.hInstance		= (HINSTANCE) 0x00400000;
	tWndClass.hIcon			= NULL;
	tWndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	tWndClass.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	tWndClass.lpszMenuName	= NULL;
	tWndClass.lpszClassName	= "ErrorReport";
	tWndClass.hIconSm		= NULL;

	::RegisterClassExA( &tWndClass );
	// 执行应用程序初始化:

	HWND tWnd = CreateWindowA( "ErrorReport", "Application Crash", WS_OVERLAPPEDWINDOW, 0, 0, 600, 480, NULL, NULL, tWndClass.hInstance, NULL);

	if ( tWnd == NULL )
		return 0;

	ShowWindow( tWnd, SW_SHOW );
	UpdateWindow( tWnd );

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return EXCEPTION_EXECUTE_HANDLER;
}


char* CExceptionManager::GetExceptionString( int vCode )
{
	#define CASEEXCEPTION( x ) case EXCEPTION_##x: return (#x);

	switch ( vCode )
	{
		CASEEXCEPTION( ACCESS_VIOLATION )
		CASEEXCEPTION( DATATYPE_MISALIGNMENT )
		CASEEXCEPTION( BREAKPOINT )
		CASEEXCEPTION( SINGLE_STEP )
		CASEEXCEPTION( ARRAY_BOUNDS_EXCEEDED )
		CASEEXCEPTION( FLT_DENORMAL_OPERAND )
		CASEEXCEPTION( FLT_DIVIDE_BY_ZERO )
		CASEEXCEPTION( FLT_INEXACT_RESULT )
		CASEEXCEPTION( FLT_INVALID_OPERATION )
		CASEEXCEPTION( FLT_OVERFLOW )
		CASEEXCEPTION( FLT_STACK_CHECK )
		CASEEXCEPTION( FLT_UNDERFLOW )
		CASEEXCEPTION( INT_DIVIDE_BY_ZERO )
		CASEEXCEPTION( INT_OVERFLOW )
		CASEEXCEPTION( PRIV_INSTRUCTION )
		CASEEXCEPTION( IN_PAGE_ERROR )
		CASEEXCEPTION( ILLEGAL_INSTRUCTION )
		CASEEXCEPTION( NONCONTINUABLE_EXCEPTION )
		CASEEXCEPTION( STACK_OVERFLOW )
		CASEEXCEPTION( INVALID_DISPOSITION )
		CASEEXCEPTION( GUARD_PAGE )
		CASEEXCEPTION( INVALID_HANDLE )
	}

	return "Unknown Exception";
}

bool CExceptionManager::GetLogicalAddress( void* pAddress, char* pBuffer, int vLength, int* pSection, int* pOffset )
{
	MEMORY_BASIC_INFORMATION tMemoryBasicInfo;

	if ( ::VirtualQuery( pAddress, &tMemoryBasicInfo, sizeof( MEMORY_BASIC_INFORMATION ) ) == 0 )
		return false;

	DWORD tModuleHandle = (DWORD) tMemoryBasicInfo.AllocationBase;

	if ( tModuleHandle > 0x70000000 )
		return false;

	if ( ::GetModuleFileNameA( (HMODULE) tModuleHandle, pBuffer, vLength ) == 0 )
		return false;

	IMAGE_DOS_HEADER*		tpDosHeader		= (IMAGE_DOS_HEADER*) tModuleHandle;
	IMAGE_NT_HEADERS*		tpNTHeader		= (IMAGE_NT_HEADERS*) ( tModuleHandle + tpDosHeader->e_lfanew );
	IMAGE_SECTION_HEADER*	tpSectionHeader	= IMAGE_FIRST_SECTION( tpNTHeader );

	DWORD tRva = (DWORD) pAddress - tModuleHandle, tSectionStart = 0, tSectionEnd = 0;

	for ( int i = 0; i < (int) tpNTHeader->FileHeader.NumberOfSections; i ++, tpSectionHeader ++ )
	{
		tSectionStart	= tpSectionHeader->VirtualAddress;
		tSectionEnd		= tSectionStart + max( tpSectionHeader->SizeOfRawData, tpSectionHeader->Misc.VirtualSize );

		if ( tRva >= tSectionStart && tRva <= tSectionEnd )
		{
			if ( pSection != 0 )
				*pSection = i + 1;

			if ( pOffset != 0 )
				*pOffset = tRva - tSectionStart;

			return true;
		}
	}

	return false;
}

void CExceptionManager::DumpStack( void* pContext )
{
	CONTEXT* tpContext = (CONTEXT*) pContext;

	STACKFRAME tStackFrame = { 0 };
	tStackFrame.AddrPC.Offset		= tpContext->Eip;
	tStackFrame.AddrPC.Mode			= AddrModeFlat;
	tStackFrame.AddrStack.Offset	= tpContext->Esp;
	tStackFrame.AddrStack.Mode		= AddrModeFlat;
	tStackFrame.AddrFrame.Offset	= tpContext->Ebp;
	tStackFrame.AddrFrame.Mode		= AddrModeFlat;
	char tBuffer[ 1024 ] = { 0 };

	while ( true )
	{
		if ( !::StackWalk( IMAGE_FILE_MACHINE_I386, spProcessHandle, ::GetCurrentThread( ),
			 &tStackFrame, tpContext, 0, SymFunctionTableAccess, SymGetModuleBase, 0 ) )
			break;

		if ( tStackFrame.AddrFrame.Offset == 0 )
			break;

		::sprintf( tBuffer, "%08X  %08X ", tStackFrame.AddrPC.Offset, tStackFrame.AddrFrame.Offset );
		::strcat( CExceptionManager::mErrorDesc, tBuffer );

		BYTE tSymbolBuffer[ sizeof( SYMBOL_INFO ) + 1024 ];
		SYMBOL_INFO* pSymbol = (SYMBOL_INFO*) tSymbolBuffer;

		pSymbol->SizeOfStruct	= sizeof( SYMBOL_INFO ) + 1024;
		pSymbol->MaxNameLen		= 1024;

		DWORD64 tSymbolDisplacement = 0;

		if ( ::SymFromAddr( spProcessHandle, tStackFrame.AddrPC.Offset, &tSymbolDisplacement, pSymbol ) )
		{
			::sprintf( tBuffer, "%s + %I64X", pSymbol->Name, tSymbolDisplacement );
			::strcat( CExceptionManager::mErrorDesc, tBuffer );
		}

		IMAGEHLP_LINE tLineInfo = { sizeof( IMAGEHLP_LINE ) }; DWORD tLineDisplacement;

		if ( ::SymGetLineFromAddr( spProcessHandle, tStackFrame.AddrPC.Offset, &tLineDisplacement, &tLineInfo ) )
		{
			::sprintf( tBuffer, "   (%s:%u)", tLineInfo.FileName, tLineInfo.LineNumber );
			::strcat( CExceptionManager::mErrorDesc, tBuffer );
		}

		::strcat( CExceptionManager::mErrorDesc, "\r\n" );
	}
}
#endif
