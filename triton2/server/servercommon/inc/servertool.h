#ifndef _SERVERTOOL_H_
#define _SERVERTOOL_H_


#include <stdlib.h>
#include "object.h"
#include <algorithm>
#include <time.h>


#ifndef ARRAY_CNT
#define ARRAY_CNT(a)	( sizeof(a) / sizeof(a[0]))
#endif

// 生成一个0-x范围内的随机数,到不了x
#define RAND(x) (int)( ( x )*(rand( )/(float)( RAND_MAX + 1.0 ) ))

#define _COUNTRY_NAME_LEN_  (32)
#define _COUNTRY_COUNT_MAX_ (10)


#define REPE_MAP_COUNT_MAX      ( 50 )

// 副本等级类别
enum REPE_LEVEL
{
	REPE_LEVEL_AVER	=	0,
	REPE_LEVEL_MAX	=	1,
	REPE_LEVEL_MIN	=	2
};


typedef unsigned long long KEY_TYPE;
#define INVITED_KEY_VALUE       			( 0 )
#define GetTimeFromKey( vKey )      		( ( int )( vKey & 0x00000000FFFFFFFF )
#define GetParameFromKey( vKey )			( ( int )( vKey >> 56 ) )


#define _SELF_SERVER_LINE_ID_					( 0 )
#define _NATIONALITY_LINE_ID_					( 100 )

// 战场等待等级列表最大个数
#define WAIT_LIST_MAX               20
// 副本等级段划分最大个数
#define LEVEL_SEGMENT_MAX			20

// 消息ID划分模块
// 0 11111 0 000000000 = 0x7E00
#define REGION_OF_MESSAGE( message_id )  ( message_id & 0x7C00 )


enum CREATE_TYPE
{
    CREATE_COMM             =   0,
    CREATE_SPECIAL_TIME     =   1,
};


struct CountryArrary
{
	char    mName[ _COUNTRY_NAME_LEN_ + 1 ];
};

class CWTSize;
class CWTPoint;
class CWTRect;

//! CWTSize 类
/*!
	CWTSize 比较类似 MFC CSize, 实现了相对坐标  mCX 和 mCY 成员是公开的，另外
	CWTSize 提供了维护这两个数据的成员函数
*/
class CWTSize
{
public:
	int	mCX;			//! 横坐标
	int	mCY;			//! 纵坐标

public:
	//! CWTSize 构造函数, 默认构造一个 CWTSize( 0, 0 ) 对象
	CWTSize( ) : mCX( 0 ), mCY( 0 )						{	}

	//! CWTSize 构造函数, 通过两个整数构造一个 CWTSize 对象
	/*! \param int vCX 横坐标
        \param int vCY 纵坐标 */
	CWTSize( int vCX, int vCY ) : mCX( vCX ), mCY( vCY )	{	}

public:
	//! CWTSize 析造函数
	~CWTSize( )											{	}

public:
	//! != 操作符, 判断两个CWTSize是否不一样
    /*! \param const CWTSize& rSize  对象引用
		\return 如果该对象和rSize不一样返回true, 否则返回false */
	bool operator != ( const CWTSize& rSize ) const			
		{ return mCX != rSize.mCX || mCY != rSize.mCY; }

	//! == 操作符, 判断两个CWTSize是否一样
    /*! \param rSize CWTSize 对象引用
		\return 如果该对象和rSize一样返回true, 否则返回false */
	bool operator == ( const CWTSize& rSize ) const			
		{ return mCX == rSize.mCX && mCY == rSize.mCY; }

	//! -= 操作符, 该CWTSize对象减去一个CWTSize对象
    /*! \param rSize CWTSize 对象引用
		\return 没有返回值 */
	void operator -= ( const CWTSize& rSize )
		{ mCX -= rSize.mCX; mCY -= rSize.mCY; }

	//! += 操作符, 该CWTSize对象加上一个rSize对象
    /*! \param rSize CWTSize 对象引用
		\return 没有返回值 */
	void operator += ( const CWTSize& rSize )
		{ mCX += rSize.mCX; mCY += rSize.mCY; }

	//! - 操作符, 这个操作返回rPoint的值减去该CWTSize对象的值
    /*! \param rSize CWTPoint 对象引用
		\return 返回rPoint的值减去该CWTSize对象的值 */
	CWTPoint operator - ( const CWTPoint& rPoint ) const;

	//! - 操作符, 这个操作返回rRect的值减去该CWTSize对象的值
    /*! \param rRect CWTRect 对象引用
		\return 返回rRect的值减去该CWTSize对象后，构造的CWTRect拷贝 */
	CWTRect operator - ( const CWTRect& rRect ) const;
		
	//! - 操作符, 这个操作返回该CWTSize对象的值减去CWTSize对象后，构造的CWTSize拷贝 */
    /*! \param rSize CWTSize 对象引用
		\return 返回该CWTSize对象的值减去 rSize的值 */
	CWTSize operator - ( const CWTSize& rSize ) const
		{ return CWTSize( mCX - rSize.mCX, mCY - rSize.mCY ); }

	//! - 操作符, 这个操作返回该CWTSize对象的取反后，构造的CWTSize拷贝
    /*! \return 返回该CWTSize对象取反后的值 */
	CWTSize operator - ( ) const
		{ return CWTSize( -mCX, -mCY ); }

	//! + 操作符, 这个操作返回rPoint的值加上该CWTSize对象的值
    /*! \param rPoint CWTPoint 对象引用
		\return 返回rPoint的值加上该CWTSize对象后，构造的CWTPoint拷贝 */
	CWTPoint operator + ( const CWTPoint& rPoint ) const;

	//! + 操作符, 这个操作返回rRect的值加上该CWTSize对象的值
    /*! \param rRect CWTRect 对象引用
		\return 返回rRect的值加上该CWTSize对象后，构造的CWTRect拷贝 */
	CWTRect operator + ( const CWTRect& rRect ) const;

	//! + 操作符, 这个操作返回该CWTSize对象的值加上rSize后，构造的CWTSize拷贝
    /*! \param rSize CWTSize 对象引用
		\return 返回该CWTSize对象加上一个rSize对象后，构造的CWTSize拷贝 */
	CWTSize operator + ( const CWTSize& rSize ) const
		{ return CWTSize( mCX + rSize.mCX, mCY + rSize.mCY ); }
};

//! CWTPoint 类
/*!
	CWTPoint 比较类似 MFC CPoint, 实现了相对坐标  mX 和 mY 成员是公开的，另外
	CWTPoint 提供了维护这两个数据的成员函数
*/
class CWTPoint
{
public:
	int		mX;
	int		mY;

public:
	//! CWTPoint 构造函数, 默认构造一个 CWTPoint( 0, 0 ) 对象
	CWTPoint( ) { }//: mX( 0 ), mY( 0 )						{	}
	//! CWTPoint 构造函数, 通过两个整数构造一个 CWTPoint 对象
	/*! \param vX int 型, 横坐标
        \param vY int 型, 纵坐标 */
	CWTPoint( int vX, int vY ) : mX( vX ), mY( vY )	{	}
public:
	//! CWTPoint 析造函数
	~CWTPoint( )										{	}

public:
	//! 成员函数 Offset, 横坐标和纵坐标分别加上vXOffset, vYOffset
	/*! \param vX int 型, 横坐标偏移量
		\param vY int 型, 纵坐标偏移量 */
	void Offset( int vXOffset, int vYOffset )
		{ mX += vXOffset; mY += vYOffset; }

	//! 成员函数 Offset, 横坐标和纵坐标分别加上rPoint.mX, rPoint.mY
    /*! \param rPoint const CWTPoint型引用, 存储了横向和纵向的偏移量 */
	void Offset( const CWTPoint& rPoint )
		{ mX += rPoint.mX; mY += rPoint.mY; }

	//! 成员函数 Offset, 横坐标和纵坐标分别偏移rSize.mX, rSize.mY */
    /*! \param rSize const CWTSize型引用, 存储了横向和纵向的偏移量 */
	void Offset( const CWTSize& rSize )
		{ mX += rSize.mCX; mY += rSize.mCY; }
	
	void Clear( ) { mX = -1; mY = -1; }

	//! 成员函数 LimitDistance, 判断目标点与本点距离是否在vAmbit内
	bool LimitDistance( const CWTPoint& rDesPos, unsigned int vAmbit )
	{	
//		CWTPoint tLength = *this - rDesPos;
//		return tLength.mX * tLength.mX + tLength.mY * tLength.mY;
		if( (unsigned int )abs( mX - rDesPos.mX ) > vAmbit )
		{
			return false;
		}
		else if( ( unsigned int )abs( mY - rDesPos.mY ) > vAmbit )
		{
			return false;
		}

		return true;
	}

	int Length( const CWTPoint& rDesPos )
	{
		return std::max( abs( mY - rDesPos.mY ), abs( mX - rDesPos.mX ) );
	}
public:
	//! != 操作符, 判断两个CWTPoint是否不一样 */
    /*! \param rPoint const CWTPoint型引用
		\return 如果该对象和rPoint不一样返回true, 否则返回false */
	bool operator != ( const CWTPoint& rPoint ) const
		{ return mX != rPoint.mX || mY != rPoint.mY; }

	//! == 操作符, 判断两个CWTPoint是否一样 */
    /*! \param rPoint const CWTPoint型引用
		\return 如果该对象和rPoint一样返回true, 否则返回false */
	bool operator == ( const CWTPoint& rPoint ) const
		{ return mX == rPoint.mX && mY == rPoint.mY; }

	//! += 操作符, 该操作加上一个CWTSize对象 */
    /*! \param rSize const CWTSize型引用
		\return 没有返回值 */
	void operator += ( const CWTSize& rSize )
		{ mX += rSize.mCX; mY += rSize.mCY; }

	//! += 操作符, 该操作加上一个CWTPoint对象 */
    /*! \param rPoint const CWTPoint型引用
		\return 没有返回值 */
	void operator += ( const CWTPoint& rPoint )
		{ mX += rPoint.mX; mY += rPoint.mY; }

	//! -= 操作符, 该操作减去一个CWTSize对象 */
    /*! \param rSize const CWTSize型引用
		\return 没有返回值 */
	void operator -= ( const CWTSize& rSize )
		{ mX -= rSize.mCX; mY -= rSize.mCY; }

	//! -= 操作符, 该操作减去一个CWTPoint对象 */
    /*! \param rPoint const CWTPoint型引用
		\return 没有返回值 */
	void operator -= ( const CWTPoint& rPoint )
		{ mX -= rPoint.mX; mY -= rPoint.mY; }

	//! - 操作符, 该操作返回该CWTPoint对象减去一个CWTPoint对象后，构造的CWTPoint拷贝 */
    /*! \param rPoint const CWTPoint型引用
		\return 返回该CWTPoint对象减去一个CWTPoint对象后，构造的CWTPoint拷贝 */
	CWTPoint operator - ( const CWTPoint& rPoint ) const
		{ return CWTPoint( mX - rPoint.mX, mY - rPoint.mY ); }

	//! - 操作符, 该操作返回该CWTPoint对象减去一个CWTSize对象后，构造的CWTPoint拷贝
    /*! \param rSize const CWTSize型引用
		\return 返回该CWTPoint对象减去一个CWTSize对象后，构造的CWTPoint拷贝 */
	CWTPoint operator - ( const CWTSize& rSize ) const
		{ return CWTPoint( mX - rSize.mCX, mY - rSize.mCY ); }

	//! - 操作符, 该操作返回CWTRect对象减去该CWTPoint对象后，构造的CWTRect拷贝
    /*! \param rRect const CWTRect型引用
		\return 返回CWTRect对象减去该CWTPoint对象后，构造的CWTRect拷贝 */
	CWTRect operator - ( const CWTRect& rRect ) const;

	//! - 操作符, 该操作返回该CWTPoint对象取反后，构造的CWTPoint拷贝
    /*! \return 返回该CWTPoint对象取反后，构造的CWTPoint拷贝 */
	CWTPoint operator - ( ) const
		{ return CWTPoint( -mX, -mY ); }

	//! + 操作符, 该操作返回该CWTPoint对象加上一个CWTPoint对象后，构造的CWTPoint拷贝
    /*! \param rPoint const CWTPoint型引用
		\return 返回该CWTPoint对象加上一个CWTPoint对象后，构造的CWTPoint拷贝 */
	CWTPoint operator + ( const CWTPoint& rPoint ) const
		{ return CWTPoint( mX + rPoint.mX, mY + rPoint.mY ); }

	//! + 操作符, 该操作返回该CWTPoint对象加上一个CWTSize对象后，构造的CWTPoint拷贝
    /*! \param rSize const CWTSize型引用
		\return 返回该CWTPoint对象加上一个CWTSize对象后，构造的CWTPoint拷贝 */
	CWTPoint operator + ( const CWTSize& rSize ) const
		{ return CWTPoint( mX + rSize.mCX, mY + rSize.mCY ); }

	//! + 操作符, 该操作返回CWTRect对象加上该CWTPoint对象后，构造的CWTRect拷贝
    /*! \param rRect const CWTRect型引用
		\return 返回CWTRect对象加上该CWTPoint对象后，构造的CWTRect拷贝 */
	CWTRect operator + ( const CWTRect& rRect ) const;
};

//! CWTRect 类
/*!
	CWTRect 比较类似 MFC CWTRect, 实现了相对矩形坐标  mTopLeft 和 mBottomRight 成员是公开的，另外
	CWTRect 提供了维护这两个数据的成员函数, CWTRect的构造函数默认将调用NormalizeRect, 该函数将使
	得一个CWTRect对象正常化, 比如:mTopLeft( 10, 10 ) 和 mBottomRight( 20, 20 )是一个正常的矩形, 但
	一个mTopLeft( 20, 20 ) 和 mBottomRight( 10, 10 )是一个不正常的矩形, NormalizeRect将使得这样的矩
	形正常化，一个不正常的矩形在使用某些CWTRect的成员函数时可能会发生错误！
*/
class CWTRect
{
public:
	CWTPoint	mTopLeft;					//! CWTPoint 型,矩形的左上角
	CWTPoint	mBottomRight;				//! CWTPoint 型,矩形的右下角

public:
	//! CWTRect 构造函数, 默认构造一个 CWTRect( 0, 0, 0, 0 ) 对象
	CWTRect( )	{ NormalizeRect( ); }

	//! \brief CWTRect 构造函数, 通过4个整数构造 CWTRect 对象, 
	/*! \param vLeft int 型 左上角横坐标
		\param vTop int 型 左上角纵坐标
		\param vRight int 型 右下角横坐标
		\param vBottom int 型 右下角纵坐标
		\param vNormalize bool 型 是不是要正常化该 CWTRect 对象 
		\sa CWTRect */
	CWTRect( int vLeft, int vTop, int vRight, int vBottom, bool vNormalize = true ) : mTopLeft( vLeft, vTop ), mBottomRight( vRight, vBottom )
		{ if ( vNormalize ) NormalizeRect( ); }

	//! CWTRect 构造函数, 通过1个 CWTPoint 和一个 CWTSize 构造 CWTRect 对象, 
	/*! \param rPoint const CWTPoint 型引用 左上角坐标
		\param rSize const CWTSize 型引用 CWTRect尺寸
		\param vNormalize bool 型 是不是要正常化该 CWTRect 对象 
		\sa CWTRect */
	CWTRect( const CWTPoint& rPoint, const CWTSize& rSize, bool vNormalize = true ) : mTopLeft( rPoint ), mBottomRight( rPoint + rSize )
		{ if ( vNormalize ) NormalizeRect( ); }

	//! CWTRect 构造函数, 通过2个 CWTPoint 构造 CWTRect 对象, 
	/*! \param rTopLeft const CWTPoint 型引用 左上角坐标
		\param rBottomRight const CWTPoint 型引用 右下角坐标
		\param vNormalize bool 型 是不是要正常化该 CWTRect 对象 
		\sa CWTRect */
	CWTRect( const CWTPoint& rTopLeft, const CWTPoint& rBottomRight, bool vNormalize = true ) : mTopLeft( rTopLeft ), mBottomRight( rBottomRight )
		{ if ( vNormalize ) NormalizeRect( ); }
public:
	//! CWTRect 析造函数
	~CWTRect( ) {	}

public:
	//! 成员函数 BottomLeft, 
	/*! \return 返回一个CWTPoint 的拷贝 */
	CWTPoint BottomLeft( ) const
		{ return CWTPoint( mTopLeft.mX, mBottomRight.mY );	}

	//! 成员函数 TopRight, 
	/*! \return 返回一个CWTPoint 的拷贝 */
	CWTPoint TopRight( ) const
		{ return CWTPoint( mBottomRight.mX, mTopLeft.mY );	}

	//! 成员函数 BottomRight, 
	/*! \return 返回一个CWTPoint 的拷贝 */
	//CWTPoint BottomRight( ) const
	//	{ return mBottomRight; }

	const CWTPoint& BottomRight() const
		{ return mBottomRight; }

	CWTPoint& BottomRight()
		{ return mBottomRight; }

	//! 成员函数 TopLeft, 
	/*! \return 返回一个CWTPoint 的拷贝 */
	//CWTPoint TopLeft( ) const
	//	{ return mTopLeft; }

	const CWTPoint& TopLeft() const
		{ return mTopLeft; }

	CWTPoint& TopLeft()
		{ return mTopLeft; }

	//! 成员函数 CenterPoint, 
	/*! \return 返回该 CWTRect 对象的中心点, 一个CWTPoint 的拷贝 */
	CWTPoint CenterPoint( ) const
		{ return CWTPoint( ( mTopLeft.mX + mBottomRight.mX ) >> 1, ( mTopLeft.mY + mBottomRight.mY ) >> 1 ); }

	//! 成员函数 EqualRect, 判断两个对象是否相等
	/*! \param rRect const CWTRect 型引用
		\return 返回该 CWTRect 对象和rRect是否相等, 如果相等返回true, 否则返回false */
	bool EqualRect( const CWTRect& rRect )	const
		{ return mTopLeft == rRect.mTopLeft && mBottomRight == rRect.mBottomRight; }

	//! 成员函数 DeflateRect, 缩小该 CWTRect 对象, 缩小指的是像中心靠近
	/*! \param vX int 型 mTopLeft的横坐标增加和mBottomRight的横坐标减少量
		\param vY int 型 mTopLeft的纵坐标增加和mBottomRight的纵坐标减少量
		\return 没有返回值 */
	void DeflateRect( int vX, int vY )
		{ mTopLeft.mX += vX; mTopLeft.mY += vY; mBottomRight.mX -= vX; mBottomRight.mY -= vY; }

	//! 成员函数 DeflateRect, 缩小该 CWTRect 对象, 缩小指的是像中心靠近
	/*! \param rSize const CWTSize 型 rSize的mCX指明了 mTopLeft的横坐标增加和mBottomRight的横坐标减少量
									   rSize的mCY指明了 mTopLeft的纵坐标增加和mBottomRight的纵坐标减少量
		\return 没有返回值 */
	void DeflateRect( const CWTSize& rSize )
	{ 
		mTopLeft.mX		+= rSize.mCX; mTopLeft.mY		+= rSize.mCY; 
		mBottomRight.mX	-= rSize.mCX; mBottomRight.mY	-= rSize.mCY; 
	}

	//! 成员函数 DeflateRect, 缩小该 CWTRect 对象, 缩小指的是像中心靠近
	/*! \param rRect const CWTRect 型 rRect 指明了每一个边的缩小量
		\return 没有返回值 */
	void DeflateRect( const CWTRect& rRect )
	{
		mTopLeft.mX		+= rRect.mTopLeft.mX;		mTopLeft.mY		+= rRect.mTopLeft.mY;		
		mBottomRight.mX -= rRect.mBottomRight.mX;	mBottomRight.mY -= rRect.mBottomRight.mY; 
	}

	//! 成员函数 DeflateRect, 缩小该 CWTRect 对象, 缩小指的是像中心靠近
	/*! \param vLeft int 型 vLeft 指明了 左边 缩小量
		\param vTop int 型 vTop 指明了 上边 缩小量
		\param vRight int 型 vRight 指明了 右边 缩小量
		\param vBottom int 型 vBottom 指明了 下边 缩小量
		\return 没有返回值 */
	void DeflateRect( int vLeft, int vTop, int vRight, int vBottom )
		{ mTopLeft.mX += vLeft; mTopLeft.mY += vTop; mBottomRight.mX -= vRight; mBottomRight.mY -= vBottom; }

	//! 成员函数 InflateRect, 扩大该 CWTRect 对象, 扩大指的是像中心发散
	/*! \param vX int 型 mTopLeft的横坐标减少和mBottomRight的横坐标增加量
		\param vY int 型 mTopLeft的纵坐标减少和mBottomRight的纵坐标增加量
		\return 没有返回值 */
	void InflateRect( int vX, int vY )
		{ mTopLeft.mX -= vX; mTopLeft.mY -= vY; mBottomRight.mX += vX; mBottomRight.mY += vY; }

	//! 成员函数 InflateRect, 扩大该 CWTRect 对象, 扩大指的是像中心发散
	/*! \param rSize const CWTSize 型 rSize的mCX指明了 mTopLeft的横坐标减少和mBottomRight的横坐标增加量
									   rSize的mCY指明了 mTopLeft的纵坐标减少和mBottomRight的纵坐标增加量
		\return 没有返回值 */
	void InflateRect( const CWTSize& rSize )
	{ 
		mTopLeft.mX		-= rSize.mCX;	mTopLeft.mY		-= rSize.mCY;			
		mBottomRight.mX += rSize.mCX;	mBottomRight.mY	+= rSize.mCY; 
	}

	//! 成员函数 InflateRect, 扩大该 CWTRect 对象, 扩大指的是像中心发散
	/*! \param rRect const CWTRect 型 rRect 指明了每一个边的扩大量
		\return 没有返回值 */
	void InflateRect( const CWTRect& rRect )
	{
		mTopLeft.mX		-= rRect.mTopLeft.mX;		mTopLeft.mY		-= rRect.mTopLeft.mY;	
		mBottomRight.mX += rRect.mBottomRight.mX;	mBottomRight.mY += rRect.mBottomRight.mY; 
	}

	//! 成员函数 InflateRect, 扩大该 CWTRect 对象, 扩大指的是像中心发散
	/*! \param vLeft int 型 vLeft 指明了 左边 扩大量
		\param vTop int 型 vTop 指明了 上边 扩大量
		\param vRight int 型 vRight 指明了 右边 扩大量
		\param vBottom int 型 vBottom 指明了 下边 扩大量
		\return 没有返回值 */
	void InflateRect( int vLeft, int vTop, int vRight, int vBottom )
		{ mTopLeft.mX -= vLeft; mTopLeft.mY -= vTop; mBottomRight.mX += vRight; mBottomRight.mY += vBottom; }

	//! 成员函数 Height, 返回该 CWTRect 对象的高度
	/*! \return 返回该 CWTRect 对象的高度 */
	int Height( ) const
		{ return mBottomRight.mY - mTopLeft.mY + 1; }

	//! 成员函数 Width, 返回该 CWTRect 对象的宽度
	/*! \return 返回该 CWTRect 对象的宽度 */
	int	Width( ) const	
		{ return mBottomRight.mX - mTopLeft.mX + 1; }

	//! 成员函数 IsRectEmpty, 返回该 CWTRect 对象的宽和高是不是全大于0
	/*! \return 返回该 CWTRect 对象是否正常 */
	bool IsRectEmpty( ) const
		{ return Width( ) <= 0 || Height( ) <= 0; }

	//! 成员函数 IsRectNull, 返回该 CWTRect 对象的四个边是否全为0
	/*! \return 返回该 CWTRect 对象是否为0 */
	bool IsRectNull( ) const
		{ return mTopLeft.mX == 0 && mTopLeft.mY == 0 && mBottomRight.mX == 0 && mBottomRight.mY == 0; }

	//! 成员函数 MoveToX, 对其该 CWTRect 对象到vX,纵坐标不变
	/*! \param vX int 型 指明对齐的横坐标
		\return 没有返回值 */
	void MoveToX( int vX )	
		{ mBottomRight.mX = vX + Width( ); mTopLeft.mX = vX; }

	//! 成员函数 MoveToY, 对其该 CWTRect 对象到vY,横坐标不变
	/*! \param vY int 型 指明对齐的纵坐标
		\return 没有返回值 */
	void MoveToY( int vY )	
		{ mBottomRight.mY = vY + Height( ); mTopLeft.mY = vY; }

	//! 成员函数 MoveToXY, 对其该 CWTRect 对象到vX, vY
	/*! \param vX int 型 指明对齐的横坐标
		\param vY int 型 指明对齐的纵坐标
		\return 没有返回值 */
	void MoveToXY( int vX, int vY )
	{
		mBottomRight.mX = vX + Width( );	mTopLeft.mX	= vX; 
		mBottomRight.mY = vY + Height( ); 	mTopLeft.mY	= vY;  
	}

	//! 成员函数 MoveToXY, 对其该 CWTRect 对象到rPoint
	/*! \param rPoint const CWTPoint 型引用 指明对齐的坐标
		\return 没有返回值 */
	void MoveToXY( const CWTPoint& rPoint )
	{
		mBottomRight.mX = rPoint.mX + Width( ); 	mTopLeft.mX = rPoint.mX; 
		mBottomRight.mY = rPoint.mY + Height( );	mTopLeft.mY = rPoint.mY; 
	}

	//! 成员函数 OffsetRect, 移动该 CWTRect 对象
	/*! \param vX int 型 指明横坐标的偏移量
		\param vY int 型 指明纵坐标的偏移量
		\return 没有返回值 */
	void OffsetRect( int vX, int vY )					 
		{ *this += CWTPoint( vX, vY ); }

	//! 成员函数 OffsetRect, 移动该 CWTRect 对象
	/*! \param rPoint const CWTPoint 型引用 指明坐标偏移量
		\return 没有返回值 */
	void OffsetRect( const CWTPoint& rPoint )
		{ *this += rPoint; }

	//! 成员函数 OffsetRect, 移动该 CWTRect 对象
	/*! \param rSize const CWTSize 型引用 指明坐标偏移量
		\return 没有返回值 */
	void OffsetRect( const CWTSize& rSize )				 
		{ *this += rSize; }	

	//! 成员函数 SetRect, 设置 CWTRect 对象
	/*! \param vLeft int 型 CWTRect对象的左上角横坐标
		\param vTop int 型 CWTRect对象的左上角纵坐标
		\param vRight int 型 CWTRect对象的右下角横坐标
		\param vBottom int 型 CWTRect对象的右下角纵坐标
		\return 没有返回值 */
	void SetRect( int vLeft, int vTop, int vRight, int vBottom )
	{
		mTopLeft.mX		= vLeft;	mTopLeft.mY		= vTop; 
		mBottomRight.mX = vRight;	mBottomRight.mY = vBottom; 
	}

	//! 成员函数 SetRect, 通过1个 CWTPoint 和1个 CWTSize 设置 CWTRect 对象, 
	/*! \param rPoint const CWTPoint 型引用 左上角坐标
		\param rSize const CWTSize 型引用 CWTRect尺寸	
		\return 没有返回值 */
	void SetRect( const CWTPoint& rPoint, const CWTSize& rSize )
		{ mTopLeft = rPoint; mBottomRight = rPoint + rSize; }

	//! 成员函数 SetRect, 通过2个 CWTPoint 设置 CWTRect 对象, 
	/*! \param rTopLeft const CWTPoint 型引用 左上角坐标
		\param rBottomRight const CWTPoint 型引用 右下角坐标
		\return 没有返回值 */
	void SetRect( const CWTPoint& rTopLeft, const CWTPoint& rBottomRight )
		{ mTopLeft = rTopLeft; mBottomRight	= rBottomRight; }

	//! 成员函数 SetRectEmpty, 将该 CWTRect 对象置0, 
	/*! \return 没有返回值 */
	void SetRectEmpty( )
		{ mTopLeft.mX = 0; mTopLeft.mY = 0; mBottomRight.mX = 0; mBottomRight.mY = 0; }

	//! 成员函数 Size, 返回该 CWTRect 对象的宽度和高度
	/*! \return 返回该 CWTRect 对象的宽度和高度 */
	CWTSize Size( ) const
		{ return CWTSize( Width( ), Height( ) ); }

	//! 成员函数 NormalizeRect, 正常化该 CWTRect 对象
	/*! \return 没有返回值 */
	void NormalizeRect( );

	//! 成员函数 PtInRect, 判断指定坐标是否在该 CWTRect 对象中
    /*! \param rPoint const CWTPoint型引用 待判断的坐标
		\return 返回指定坐标是不是在该 CWTRect 对象中 */
	bool PtInRect( const CWTPoint&	rPoint ) const;

	//! 成员函数 IntersectRect, 计算两个 CWTRect 对象的交集
	/*! \brief 如果计算产生的矩形不正常, 那么该矩形将被清零
		\brief eg. CWTRect( ).( CWTRect( 0, 0, 10, 10 ), CWTRect( 5, 5, 15, 15 ) ) == CWTRect( 5, 5, 10, 10 )
		\param const CWTRect& rSrcRect1 源矩形
		\param const CWTRect& rSrcRect2 源矩形
		\return 如果传入的两个源矩形或是计算后产生的矩形不正常，将返回false, 否这返回true */
   	bool IntersectRect( const CWTRect& rSrcRect1, const CWTRect& rSrcRect2 );

	//! 成员函数 UnionRect, 计算两个 CWTRect 对象的并集
	/*! \brief 如果传入的两个源矩形有一个不正常, 函数将返回false
		\brief eg. CWTRect( ).( CWTRect( 0, 0, 10, 10 ), CWTRect( 5, 5, 15, 15 ) ) == CWTRect( 0, 0, 15, 15 )
		\param const CWTRect& rSrcRect1 源矩形
		\param const CWTRect& rSrcRect2 源矩形
		\return 如果传入的两个源矩形有一个不正常, 函数将返回false, 否这返回true */
	bool UnionRect( const CWTRect& rSrcRectl, const CWTRect& rSrcRect2 );

	//! 矩形相减
	int SubtractRect( const CWTRect& rDesRect, CWTRect* pRectList );

public:
	//! != 操作符, 判断两个 CWTRect 对象是否不相等
	/*! \param const CWTRect& rRect 
		\return 返回该 CWTRect 对象和rRect是否相等, 如果不相等返回true, 否则返回false */
	bool operator != ( const CWTRect& rRect ) const
		{ return !EqualRect( rRect ); }

	//! == 操作符, 判断两个 CWTRect 对象是否相等
	/*! \param const CWTRect& rRect 
		\return 返回该 CWTRect 对象和rRect是否相等, 如果相等返回true, 否则返回false */
	bool operator == ( const CWTRect& rRect ) const
		{ return EqualRect( rRect ); }

	//! &= 操作符, 计算该 CWTRect 对象和 指定 CWTRect 对象的交集
	/*! \param const CWTRect& rRect 
		\return 没有返回值 */
	void operator &= ( const CWTRect& rRect )
		{ IntersectRect( *this, rRect ); }

	//! |= 操作符, 计算该 CWTRect 对象和 指定 CWTRect 对象的并集
	/*! \param const CWTRect& rRect 
		\return 没有返回值 */
	void operator |= ( const CWTRect& rRect )
		{ UnionRect( *this, rRect ); }

	//! & 操作符, 计算该 CWTRect 对象和 指定 CWTRect 对象的交集
	/*! \param const CWTRect& rRect 
		\return 计算该 CWTRect 对象和 指定 CWTRect 对象的并集, 返回 CWTRect 的拷贝  */
	CWTRect operator & ( const CWTRect& rRect ) const
	{
		CWTRect tRectResult;
		tRectResult.IntersectRect( *this, rRect );
		return tRectResult;
	}

	//! | 操作符, 计算该 CWTRect 对象和 指定 CWTRect 对象的并集
	/*! \param const CWTRect& rRect 
		\return 计算该 CWTRect 对象和 指定 CWTRect 对象的并集, 返回 CWTRect 的拷贝  */
	CWTRect operator | ( const CWTRect& rRect ) const
	{
		CWTRect tRectResult;
		tRectResult.UnionRect( *this, rRect );
		return tRectResult;
	}

	//! - 操作符, 把该 CWTRect 反向偏移 rPoint 个单位
	/*! \param const CWTPoint& rPoint
		\return 把该 CWTRect 反向偏移 rPoint 个单位, 返回 CWTRect 的拷贝  */
	CWTRect operator - ( const CWTPoint& rPoint ) const
	{
		CWTRect tRectResult = *this;
		tRectResult.OffsetRect( -rPoint );
		return tRectResult;
	}

	//! - 操作符, 把该 CWTRect 反向偏移 rSize 个单位
	/*! \param const CWTSize& rSize
		\return 把该 CWTRect 反向偏移 rSize 个单位, 返回 CWTRect 的拷贝  */
	CWTRect operator - ( const CWTSize& rSize ) const
	{
		CWTRect tRectResult = *this;
		tRectResult.OffsetRect( -rSize );
		return tRectResult;
	}

	//! - 操作符, 缩小该 CWTRect 对象，缩小的尺寸由 rRect 指定
	/*! \param const CWTRect& rRect
		\return 缩小该 CWTRect 对象，缩小的尺寸由 rRect 指定, 返回 CWTRect 的拷贝  */
	CWTRect operator - ( const CWTRect& rRect ) const
	{
		CWTRect tRectResult = *this;
		tRectResult.DeflateRect( rRect );
		return tRectResult;
	}

	//! + 操作符, 把该 CWTRect 正向偏移 rPoint 个单位
	/*! \param const CWTPoint& rPoint
		\return 把该 CWTRect 正向偏移 rPoint 个单位, 返回 CWTRect 的拷贝  */
	CWTRect operator + ( const CWTPoint& rPoint ) const
	{
		CWTRect tRectResult = *this;
		tRectResult.OffsetRect( rPoint );
		return tRectResult;
	}

	//! + 操作符, 把该 CWTRect 正向偏移 rSize 个单位
	/*! \param const CWTSize& rSize
		\return 把该 CWTRect 正向偏移 rSize 个单位, 返回 CWTRect 的拷贝  */
	CWTRect operator + ( const CWTSize& rSize ) const
	{
		CWTRect tRectResult = *this;
		tRectResult.OffsetRect( rSize );
		return tRectResult;
	}

	//! + 操作符, 扩大该 CWTRect 对象，扩大的尺寸由 rRect 指定
	/*! \param const CWTRect& rRect
		\return 扩大该 CWTRect 对象，扩大的尺寸由 rRect 指定, 返回 CWTRect 的拷贝  */
	CWTRect operator + ( const CWTRect& rRect ) const
	{
		CWTRect tRectResult = *this;
		tRectResult.InflateRect( rRect );
		return tRectResult;
	}

	//! += 操作符, 把该 CWTRect 正向偏移 rPoint 个单位
	/*! \param const CWTPoint& rPoint
		\return 没有返回值  */
	void operator += ( const CWTPoint& rPoint )
		{ mTopLeft += rPoint; mBottomRight += rPoint; }

	//! += 操作符, 把该 CWTRect 正向偏移 rSize 个单位
	/*! \param const CWTSize& rSize
		\return 没有返回值  */
	void operator += ( const CWTSize& rSize )
		{ mTopLeft += rSize; mBottomRight += rSize; }

	//! += 操作符, 扩大该 CWTRect 对象，扩大的尺寸由 rRect 指定
	/*! \param const CWTRect& rRect
		\return 没有返回值  */
	void operator += ( const CWTRect& rRect )
		{ InflateRect( rRect );	}

	//! += 操作符, 把该 CWTRect 反向偏移 rPoint 个单位
	/*! \param const CWTPoint& rPoint
		\return 没有返回值  */
	void operator -= ( const CWTPoint& rPoint )
		{ mTopLeft -= rPoint; mBottomRight -= rPoint; }

	//! += 操作符, 把该 CWTRect 反向偏移 rSize 个单位
	/*! \param const CWTSize& rSize
		\return 没有返回值  */
	void operator -= ( const CWTSize& rSize )
		{ mTopLeft -= rSize; mBottomRight -= rSize;	}

	//! += 操作符, 缩小该 CWTRect 对象，缩小的尺寸由 rRect 指定
	/*! \param const CWTRect& rRect
		\return 没有返回值  */
	void operator -= ( const CWTRect& rRect )
		{ DeflateRect( rRect );	}
};

// ********************************************************************** //
// CSingleton
// ********************************************************************** //

class CTlv
{
public:
	enum
	{
		TLV_TAGLEN			= 16,
		ERROR_OK			= 0,
		ERROR_TAGNOTFOUND	= 1,
		ERROR_TAGOVERRUN	= 2,
	};

	char*	mpBuffer;
	int		mBufferSize;
	int		mPushPointer;

public:
	CTlv( char* pBuffer, int vSize ) : mpBuffer( pBuffer ), mBufferSize( vSize ), mPushPointer( 0 )
	{
	}

	CTlv()
	{
		if ( CObj::msCreateMode )
		{
			mpBuffer = NULL;
			mBufferSize = 0;
			mPushPointer = 0;
		} 
		else
		{
			;
		}
	}

	~CTlv() {}

public:
	int PopBuffer( const char* pTagName, char*& pBuffer, int& rSize );
	int PushBuffer( const char* pTagName, const char* pBuffer, int tSize );
};

// ********************************************************************** //
// CSingleton
// ********************************************************************** //

template< typename T >
class CSingleton
{
private:
	static T* spSingleton;

public:
	CSingleton( )
	{
		spSingleton = static_cast< T* >( this );
	}

	static T* GetSingletonPtr( )
	{
		return spSingleton;
	}

	static T& GetSingleton( )
	{
		return *spSingleton;
	}
};


// ********************************************************************** //
// CLogManager
// ********************************************************************** //

class CLogManager : public CSingleton< CLogManager >
{
public:
	//FILE*	mpSystemLog;		// 系统日志
	//FILE*	mpLogicLog;			// 逻辑日志

public:
	CLogManager( )
	{
		//mpSystemLog	= ::fopen( "logs/systemlog.txt", "wt" );
		//mpLogicLog	= ::fopen( "logs/logiclog.txt", "wt" );
	}

public:
	~CLogManager( )
	{
		//if ( mpSystemLog != NULL )
		//	::fclose( mpSystemLog );

		//if ( mpLogicLog != NULL )
		//	::fclose( mpLogicLog );
	}

public:
	//void SystemLog( char* pFormat, ... );
	//void LogicLog( char* pFormat, ... );
};

// ********************************************************************** //
// CVariant( 变体 )
// ********************************************************************** //

class CVariant
{
public:
	enum VARTYPE
	{
		VT_CHAR		= 0x01,		// char		类型
		VT_SHORT	= 0x02,		// short	类型
		VT_LONG		= 0x03,		// long		类型
		VT_BOOL		= 0x05,		// bool		类型
		VT_FLOAT	= 0x06,		// float	类型
		VT_DOUBLE	= 0x07,		// double	类型
		VT_UCHAR	= 0x11,		// char		类型
		VT_USHORT	= 0x12,		// short	类型
		VT_ULONG	= 0x13,		// long		类型
		VT_PCHAR	= 0x20,		// char*	类型
		VT_PSHORT	= 0x21,		// short*	类型
		VT_PLONG	= 0x22,		// long*	类型
		VT_PFLOAT	= 0x23,		// float*	类型
		VT_PDOUBLE	= 0x24,		// double*	类型
		VT_PVOID	= 0x25,		// void*	类型	
	};

public:
	unsigned short mVarType;			// 变体类型
	union 
	{
		 char			mCharValue;		// char		类型
		 unsigned char	mUCharValue;	// char		类型
		 short			mShortValue;	// short	类型
		 unsigned short	mUShortValue;	// Ushort	类型
		 long			mLongValue;		// long		类型
		 unsigned long	mULongValue;	// ULong	类型
		 bool			mBoolValue;		// bool		类型
		 float			mFloatValue;	// float	类型
		 double			mDoubleValue;	// double	类型
		 unsigned char* mpCharValue;	// char*	类型
		 short*			mpShortValue;	// short*	类型
		 long*			mpLongValue;	// long*	类型
		 float*			mpFloatVauel;	// float*	类型 
		 double*		mpDoubleValue;  // double*	类型 
		 void*			mpVoidValue;	// void*	类型 
	 };

public:
	CVariant( )
	{
		mVarType	= 0;
		mLongValue	= 0;
	}

public:
	CVariant( unsigned short vVarType, long vVarValue )
	{
		mVarType	= vVarType;
		mLongValue	= vVarValue;
	}
};

// ********************************************************************** //
// CWTimer
// ********************************************************************** //

class CWTimer
{
public:
	int mDuration;
	int mMaxDuration;
public:
	CWTimer()
	{
		if ( CObj::msCreateMode )
		{
			mDuration = 0;
			mMaxDuration = 0;
		} 
		else
		{
			;
		}
	}
	CWTimer( int vDuration ) : mDuration( vDuration ), mMaxDuration( vDuration )	{	}

public:
	// 如果时间到返回true
	inline bool Elapse( int vInterval )
		{ mDuration -= vInterval; return mDuration <= 0 ? mDuration = mMaxDuration, true : false; }
};




enum EGMGateCmdType
{
	CMD_NONE			= 0,
	CMD_FAMILY_MONEY	= 1,	// 家族金钱
	CMD_FAMILY_BIDNPC	= 2,	// 家族NPC
	CMD_FAMILY_BIDMONEY = 3,	// 竞价金钱
	CMD_FAMILY_REPEDATE = 4,	// 副本日期
	CMD_FAMILY_REPETIME	= 5,	// 副本次数
	CMD_CORPS_BIDNPC	= 6,	// 军团NPC
	CMD_CORPS_BIDMONEY	= 7,	// 军团竞价
	CMD_CORPS_BATTLECITY = 8,	// 军团主城
	CMD_CORPS_ADDCITY	 = 9,	// 军团加城
	CMD_CORPS_DELCITY	 = 10,	// 军团减城
	CMD_CORPS_MONEY		 = 11,	// 军团金钱
	CMD_WORLD_POWER		 = 12,	// 国王声望
	CMD_WORLD_WIZARD	 = 13,  // 才华值
	CMD_WORLD_CONVALUE	 = 14,	// 建设值
	CMD_WORLD_CONWIZARD	 = 15,	// 才华建设值
	CMD_WORLD_CORPSID	 = 16,	// 军团ID
	CMD_WORLD_CLEAR		 = 17,	// 清除功能次数
	CMD_WORLD_PEOPLE	 = 18,	// 国家人口
	CMD_FAMILY_CHASTATUS = 19,	// 清家族挑战赛战斗状态
	CMD_CORPS_CHASTATUS	 = 20,	// 军团挑战赛状态
	CMD_FAMILY_WARSTATUS = 21,	// 家族争霸赛状态	 
	CMD_CORPS_NPC_STATUS = 22,	// 军团战NPC的状态
	CMD_WORLD_CALC_WEAK_LEVEL = 23, // 开始计算弱国等级
};

#define HOUR_TO_SECOND( hour )	( ( hour ) * 60 * 60 )
#define MINUTE_TO_SECOND( min ) ( ( min ) * 60 )
#define DAY_SECOND( hour, min, sec ) ( HOUR_TO_SECOND( hour ) + MINUTE_TO_SECOND( min ) + sec )
#define UN_CONFINE_DATE		( 0 )

struct OpenDate
{
	int mYear;			//年
	int mMonth;			//月
	int mWeek;			//周
	int mDay;			//日( 没周的第几天 )
	int mHr;			//小时
	int mMinute;		//分钟
	int mSec;			//秒

	OpenDate( ) //: mYear( UN_CONFINE_DATE ), mMonth( UN_CONFINE_DATE ), mWeek( UN_CONFINE_DATE ),
//		mDay( UN_CONFINE_DATE ), mHr( UN_CONFINE_DATE ), mMinute( UN_CONFINE_DATE ), mSec( UN_CONFINE_DATE )
	{ 
		if( CObj::msCreateMode )
		{
			Clear( );
		}
	}

	void Clear( )
	{
		mYear = UN_CONFINE_DATE;
		mMonth = UN_CONFINE_DATE;
		mWeek = UN_CONFINE_DATE;
		mDay = UN_CONFINE_DATE;
		mHr = UN_CONFINE_DATE;
		mMinute = UN_CONFINE_DATE;
		mSec = UN_CONFINE_DATE;

	}
};

struct DateSpace
{
	OpenDate mBegin;
	OpenDate mEnd;
	
	void Clear( )
	{
		mBegin.Clear( );
		mEnd.Clear( );
	}

	bool InDateSpace( struct tm* vTm )
	{
		if( mBegin.mYear != UN_CONFINE_DATE )
		{
			if( ( vTm->tm_year + 1900 ) >= mBegin.mYear )
			{
				if( mEnd.mYear != UN_CONFINE_DATE )
				{
					if( ( vTm->tm_year + 1900 ) > mEnd.mYear )
					{
						return false;
					}
				}
			}
			else
			{
				return false;
			}
		}

		if( mBegin.mMonth != UN_CONFINE_DATE )
		{
			if( vTm->tm_mon < mBegin.mMonth )
			{
				return false;
			}

			if( mEnd.mMonth != UN_CONFINE_DATE )
			{
				if( vTm->tm_mon > mEnd.mMonth )
				{
					return false;
				}
			}
		}

		if( mBegin.mWeek != UN_CONFINE_DATE )
		{
			int tWeek = vTm->tm_wday;//( abs( vTm->tm_mday - vTm->tm_wday + 1 ) / 7 ) + 1;
			tWeek = ( vTm->tm_wday != 0 ) ? vTm->tm_wday : 7;
			if( tWeek < mBegin.mWeek )
			{
				return false;
			}

			if( mEnd.mWeek != UN_CONFINE_DATE )
			{
				if( tWeek > mEnd.mWeek )
				{
					return false;
				}
			}
		}

		if( mBegin.mDay != UN_CONFINE_DATE )
		{
			if( vTm->tm_mday < mBegin.mDay )
			{
				return false;
			}

			if( mEnd.mDay != UN_CONFINE_DATE )
			{
				if( vTm->tm_mday > mEnd.mDay )
				{
					return false;
				}
			}
		}

		if( mBegin.mHr != UN_CONFINE_DATE || mEnd.mHr != UN_CONFINE_DATE )
		{
			if( mEnd.mHr != UN_CONFINE_DATE )
			{
				unsigned int tDayTimeNow = DAY_SECOND( vTm->tm_hour, vTm->tm_min, vTm->tm_sec );
				unsigned int tDayTimeBegin = DAY_SECOND( mBegin.mHr, mBegin.mMinute, mBegin.mSec );
				unsigned int tDayTimeEnd = DAY_SECOND( mEnd.mHr, mEnd.mMinute, mEnd.mSec );

				if( ( tDayTimeNow >= tDayTimeBegin ) && ( tDayTimeNow < tDayTimeEnd ) )
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				if( vTm->tm_hour >= mBegin.mHr )
				{
					return true;
				}
			}
		}

		return true;
	}
};



//
//// ********************************************************************** //
//// CDispParams　接口参数信息(输入或者输出)
//// ********************************************************************** //
//
//class CDispParams
//{
//public:
//	unsigned int	mNumberOfArgs;		// 有几个参数
//	CVariant		mVariants[ 16 ];		// 参数表, 最大8个参数
//
//public:
//	CDispParams( ) : mNumberOfArgs( 0 )
//	{
//	}
//
//public:
//	void PushParam( const CVariant& rVar )
//	{
//		mVariants[ mNumberOfArgs ] = rVar;
//		mNumberOfArgs ++ ;
//	}
//};
//
//// ********************************************************************** //
//// CCommandHandler
//// ********************************************************************** //
//
//template < typename T >
//class CCommandHandler
//{
//	typedef void ( T::*CommandProc )( std::vector< std::string >& rParam );
//
//public:	
//	std::string		mCommand;
//	CommandProc		mpHandler;
//	T*				mpClass;
//
//public:
//	CCommandHandler( const char* pCommand = "", T* pClass = NULL, CommandProc pHandler = NULL ) : mCommand( pCommand ), mpClass( pClass ), mpHandler( pHandler )
//	{
//	}
//
//public:
//	void Fire( std::vector< std::string >& rParam )
//		{ ( mpClass->*mpHandler )( rParam ); }
//};
//
//// ********************************************************************** //
//// CCommandCenter
//// ********************************************************************** //
//
//template < typename T >
//class CCommandCenter
//{
//protected:
//	std::vector< CCommandHandler< T > >		mCommandList;
//	bool									mRun;
//
//public:
//	CCommandCenter( ) : mRun( true )
//	{
//	}
//
//public:
//	void Exit( )
//	{
//		mRun = false;
//	}
//
//public:
//	// 注册处理函数
//	void RegisterCommand( CCommandHandler< T > vHandler )
//		{ mCommandList.push_back( vHandler ); }
//
//	// 执行
//	//增加 守护进程处理判断
//	void Run( const int nDaemon = 0)
//	{
//		std::string tCommandString;
//		std::vector< std::string > tParam;
//		while( mRun == true )
//		{
//			if (nDaemon==0)
//			{
//				int tChar = ::getchar( );
//				if ( tChar == ' ' || tChar == '\n' )
//				{
//					tParam.push_back( tCommandString );
//					tCommandString.clear( );
//				}
//				else
//					tCommandString.push_back( tChar );
//
//				if ( tChar == '\n' )
//				{
//					size_t i = 0;
//					for ( ; i < mCommandList.size( ); i ++ )
//					{
//						if ( strcmp( mCommandList[ i ].mCommand.c_str( ), tParam[ 0 ].c_str( ) ) == 0 )
//						{
//							mCommandList[ i ].Fire( tParam );
//							break;
//						}
//					}
//
//					if ( i == mCommandList.size( ) && tParam[ 0 ].empty( ) == false )
//					{
//						printf( "Invalid Command!\r\n" );
//					}
//					tParam.clear( );
//				}
//			}
//#ifdef LINUX
//			else
//				sleep(1);
//#endif
//
//		}
//	}
//};
//
//#ifdef WIN32
//// ********************************************************************** //
//// CException
//// ********************************************************************** //
//
//struct CException
//{
//	char*	mpFileName;
//	int		mLineNumber;
//	char	mDesc[2048];
//
//	CException( char* pFileName, int vLineNumber ) : mpFileName( pFileName ), mLineNumber( vLineNumber ) { }
//};
//
//// ********************************************************************** //
//// CExceptionManager
//// ********************************************************************** //
//
//class CExceptionManager
//{
//public:
//	static void*	spPreviousFilter;
//	static void*	spProcessHandle;
//	static char		mErrorDesc[ 8192 ];
//public:
//	static void Initialize( );
//	static void Finalize( );
//
//	static long __stdcall ExceptionFilter( void* pExceptionInfo );
//
//	static char* GetExceptionString( int vCode );
//	static bool	 GetLogicalAddress( void* pAddress, char* pBuffer, int vLength, int* pSection, int* pOffset );
//
//	static void DumpStack( void* pContext );
//};


//#endif
#endif


