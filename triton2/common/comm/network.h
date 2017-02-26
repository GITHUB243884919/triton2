#pragma once
#include "message_pb.hxx.pb.h"

typedef unsigned char BYTE;
#define MAX_PACKAGE_LEN 45000 // 服务器和客户端间最大包长度

// ********************************************************************** //
// CStatistics
// ********************************************************************** //

class CStatistics
{
public:
	int		mBytesSent;			// 总发送字节数
	int		mBytesRecv;			// 总接收字节数
	int		mPackagesSent;		// 总发送包数
	int		mPackagesRecv;		// 总接收包数
	int		mBytesSentAvg;		// 每秒发送字节数
	int		mBytesRecvAvg;		// 每秒接收字节数
	int		mPackagesSentAvg;	// 每秒发送包数
	int		mPackagesRecvAvg;	// 每秒接收包数
	int		mPingInterval;		// ping值
};
//
//// ********************************************************************** //
//// CMessage
//// ********************************************************************** //
//
//class CMessage
//{
//public:
//
//	// 消息实体
//	enum enMessageFE
//	{
//		FE_CLIENT = 0,		// 客户端
//		FE_SCENCESERVER,	// 场景服务器
//		FE_LOGINSERVER,		// 登陆服务器
//		FE_INFOSERVER,		// 信息服务器
//		FE_GATESERVER,		// 网关服务器
//		FE_DATASERVER,		// 数据服务器
//		FE_ROUTERSERER,		// 路有服务器
//	};
//
//	// 消息类型
//	enum enMessageType
//	{
//		REQUEST = 0,		// 请求
//		RESPONSE,			// 回复
//		NOTIFY,				// 通知
//		ACK,				// 应答
//	};
//
//public:
//	unsigned short	mMessageSize;
//	unsigned short	mMessageID;
//	unsigned int	mCheckSum;
//
//	int				mMsgSeq;	// 消息序列号
//	short			mMsgType;	// 消息类型
//	char			mSrcFE;		// 消息源实体
//	char			mDstFE;		// 消息目的实体
//	int				mSrcID;		// 消息源ID
//	int				mDstID;		// 消息目的ID
//
//public:
//	
//	virtual int Encode();
//	virtual int Decode();
//	virtual int Print();
//
//public:
//	CMessage( unsigned short vMessageSize, unsigned short vMessageID, short vMsgType = REQUEST, int vMsgSeq = 0, char vSrcFE = FE_CLIENT, char vDstFE = FE_SCENCESERVER )
//		: mMessageSize( vMessageSize ), mMessageID( vMessageID ), mCheckSum( BuildCheckSum( ) ), mMsgType( vMsgType ), mMsgSeq( vMsgSeq ), mSrcFE( vSrcFE ), mDstFE( vDstFE ) { }
//
//	unsigned int BuildCheckSum( )
//		{ return mCheckSum = ~( ( (unsigned int) mMessageSize << 16 ) | (unsigned int) mMessageID ); }
//
//	void SetMessageSize( unsigned short vMessageSize )
//		{ mMessageSize = vMessageSize; mCheckSum = BuildCheckSum( ); }
//
//	void SetMessageID( unsigned short vMessageID )
//		{ mMessageID = vMessageID; mCheckSum = BuildCheckSum( ); }
//
//	bool CheckHeader( )
//		{ return mCheckSum == BuildCheckSum( ); }
//
//};
//
//// ********************************************************************** //
//// CCSHead:主要是客户端到场景服务器间的消息用, 加密算法, 协议版本号, 以及将来扩充协议用
//// ********************************************************************** //
//#define MAXOPTLENGTH		127			//C/S协议可选部分的最大长度
//#define CSHEADBASELENGTH	18			//C/S协议头部的基本大小
//
//class CCSHead
//{
//public:
//	short				mTotalLength;
//	short				mVer;
//	int					mDialogID;	// 服务器与客户端的会话ID
//	int					mSeq;
//	unsigned int		mUin;
//	unsigned char		mBodyFlag;
//	unsigned char		mOptLength;
//	unsigned char		mOption[MAXOPTLENGTH];
//
//public:
//	CCSHead() : mTotalLength( 0 ), mVer( 0 ), mDialogID( 0 ), mSeq( 0 ), mUin( 0 ), mBodyFlag( 0 ), mOptLength( 0 ) {}
//	~CCSHead() {}
//}; 


// ********************************************************************** //
// CLockObject
// ********************************************************************** //

class CLockObject
{
public:
	virtual ~CLockObject( )
	{
	}

public:
	virtual void Lock( )	= 0;
	virtual void Unlock( )	= 0;
};

// ********************************************************************** //
// CClientNetwork
// ********************************************************************** //

class CClientNetwork
{
public:
	typedef void ( *OnConnectProc )( int vCode, int vUserData );
	typedef void ( *OnMessageProc )( BYTE* pMessage, int vMsgLen, int vUserData );
	typedef void ( *OnCloseProc )( int vCode, int vUserData );

public:
	OnConnectProc	mpConnectProc;
	OnMessageProc	mpMessageProc;
	OnCloseProc		mpCloseProc;

public:
	CClientNetwork( OnConnectProc pConnect, OnMessageProc pMessage, OnCloseProc pClose ) : mpConnectProc( pConnect ), mpMessageProc( pMessage ), mpCloseProc( pClose )
	{
	}
	CClientNetwork()
	{
	}

public:
	void Init(OnConnectProc pConnect, OnMessageProc pMessage, OnCloseProc pClose )
	{
		mpConnectProc = pConnect;
		mpMessageProc = pMessage;
		mpCloseProc = pClose;
	}
	void FireConnect( int vCode, int vUserData )
	{
		if ( mpConnectProc != 0 )
			(*mpConnectProc)( vCode, vUserData );
	}

	void FireMessage( BYTE* pMessage, int vMsgLen, int vUserData )
	{
		if ( mpMessageProc != 0 )
			(*mpMessageProc)( pMessage, vMsgLen, vUserData );
	}

	void FireClose( int vCode, int vUserData )
	{
		if ( mpCloseProc != 0 )
			(*mpCloseProc)( vCode, vUserData );
	}

public:
	virtual void SetUserData( int vUserData )				= 0;
	virtual void EnumEvent( )								= 0;
	virtual bool Create( const char* pAddress )				= 0;
	virtual void Shutdown( )								= 0;
	virtual void SendMsg( BYTE* pMessage, int vMsgLen )				= 0;
	virtual void GetStatistics( CStatistics* pStatistics )	= 0;
};

// ********************************************************************** //
// CServerNetwork
// ********************************************************************** //

class CServerNetwork
{
public:
	typedef void ( *OnAcceptProc )( int vCommHandle );
	typedef void ( *OnMessageProc )( int vCommHandle, BYTE* pMessage, int vMsgLen );
	typedef void ( *OnCloseProc )( int vCommHandle );
	typedef void ( *OnTimerProc )( unsigned int vTickCount );

public:
	OnAcceptProc	mpAcceptProc;
	OnMessageProc	mpMessageProc;
	OnCloseProc		mpCloseProc;
	OnTimerProc		mpTimerProc;
	CLockObject*	mpLock;
public:
	CServerNetwork( OnAcceptProc pAccept, OnMessageProc pMessage, OnCloseProc pClose, OnTimerProc pTimer, CLockObject* pLock ) : 
	  mpAcceptProc( pAccept ), mpMessageProc( pMessage ), mpCloseProc( pClose ), mpTimerProc( pTimer ), mpLock( pLock )
	{
	}

public:
	virtual ~CServerNetwork( )
	{
	}

public:
	void FireAccept( int vCommHandle )
	{
		if ( mpAcceptProc != 0 )
			(*mpAcceptProc)( vCommHandle );
	}

	void FireMessage( int vCommHandle, BYTE* pMessage, int vMsgLen )
	{
		if ( mpMessageProc != 0 )
			(*mpMessageProc)( vCommHandle, pMessage, vMsgLen);
	}

	void FireClose( int vCommHandle )
	{
		if ( mpCloseProc != 0 )
			(*mpCloseProc)( vCommHandle );
	}

	void FireTimer( unsigned long vTickCount )
	{
		if ( mpTimerProc != 0 )
			(*mpTimerProc)( vTickCount );
	}

public:
	virtual char* GetRemoteIPString( int vCommHandle )					= 0;
	virtual unsigned short GetRemotePort( int vCommHandle )				= 0;
	virtual bool Create( const char* pAddress )							= 0;
	virtual void Shutdown( )											= 0;
	virtual void SendMsg( int vCommHandle, BYTE* pMessage, int vMsgLen )	= 0;
	virtual void SetKeepAlive( int vCommHandle, unsigned int vTime )	= 0;
	virtual void GetStatistics( int vCommHandle, CStatistics* pStatistics ) = 0;

public:
	void Lock( )	{ mpLock->Lock( );	}
	void Unlock( )	{ mpLock->Unlock( );}
};
