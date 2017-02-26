#include "client_comm_engine.h"
#include "message_pb.hxx.pb.h"
#include "message_factory.h"
#include "oi_tea.h"
#include "network.h"

#ifdef LINUX
#include "lk_assert.h"
#else
#include <assert.h>
#define LK_ASSERT_STR(a,handle) assert(a)
#endif

namespace ClientCommEngine
{


// ***************************************************************
//  Function:		ConvertClientCodeToMsg2
//  Description:	客户端服务器通用，转换数据包到消息格式，中间会序列化和解密
//	Input:			
//	OutPut:			
//	Return:			
//	Others:			这里面的判断要尤其严格，因为可能会有非法的数据包，
//					要保证收到什么数据都不能崩溃。发布release版时服务器要-DNDEBUG宏
//  Date:			02/05/2009
// 
// ***************************************************************
int ConvertClientCodeToMsg2( const void* pBuff, unsigned short usBuffLen, CCSHead* pCshead, CMessage* pMsg, const unsigned char* pEncrypt )
{
	if( (pBuff == NULL) || ( usBuffLen < sizeof(unsigned short)*4 ) || (pCshead == NULL) || (pMsg == NULL) )
	{
		return -1;
	}

	bool bParseRet = false;
	char* tpBuff = (char*)pBuff;
	int usTmpLen = 0;

	// 总长度
	int usTmpTotalLen = (int) *((unsigned short*)tpBuff);

	// 验证收到的包长和数据包中的包长是否匹配
	if( usTmpTotalLen != (int)(usBuffLen) )
	{
		LK_ASSERT_STR(0, return -2, "usTmpTotalLen %d != usBuffLen %d", usTmpTotalLen, usBuffLen );
	}

	tpBuff += sizeof(unsigned short); 
	usTmpTotalLen -= sizeof(unsigned short);
	
	// 反序列化 CCSHead
	usTmpLen = (int) *((unsigned short*)tpBuff);
	tpBuff += sizeof(unsigned short);
	usTmpTotalLen -= sizeof(unsigned short);

	//TODO: 安全性判断,这里 usTmpTotalLen 还不会< 0
	if( usTmpLen >= usTmpTotalLen || usTmpLen < 0 )
	{
		LK_ASSERT_STR(0, return -3, "usTmpLen %d", usTmpLen );
	}

	bParseRet = pCshead->ParseFromArray( tpBuff, usTmpLen );
	if( bParseRet != true )
	{
		return -4;
	}
	tpBuff += usTmpLen;
	usTmpTotalLen -= usTmpLen;

	if ( usTmpTotalLen <= 0 )
	{
		LK_ASSERT_STR(0, return -5, "usTmpTotalLen %d", usTmpTotalLen);
	}

	// 这里开始解密
	static unsigned char tEncryBuff[ MAX_PACKAGE_LEN ] = {0}; 
	unsigned char *tpEncryBuff = &tEncryBuff[0];
	int tOutLen = MAX_PACKAGE_LEN;
	
	DecryptData( EnCryptType1, pEncrypt, (unsigned char*)tpBuff, usTmpTotalLen, tpEncryBuff, &tOutLen );
	
	// 解密后的长度不能超过最大ushort , 也不能比输入的未解密前长度长
	if( tOutLen >= 65535 || tOutLen > usTmpTotalLen )
	{
		LK_ASSERT_STR(0, return -6, "tOutLen %d", tOutLen );
	}

	// 反序列化 CMessage
	usTmpLen = (int) *((unsigned short*)tpEncryBuff);
	tpEncryBuff += sizeof(unsigned short);
	tOutLen -= sizeof(unsigned short);
	if( usTmpLen <= 3 || usTmpLen >= tOutLen || tOutLen <= 0 ) // 客户端发上来的Message不可能小于3
	{
#ifdef _DEBUG_ // 这里解码错误将会很多，主要是因为密钥变化，release版屏蔽掉日志。
		LK_ASSERT_STR(0, return -7, "usTmpLen %d", usTmpLen );
#else
		return -7;
#endif
	}

	bParseRet = pMsg->ParseFromArray( tpEncryBuff, usTmpLen );
	if( bParseRet != true )
	{
		return -8;
	}

	tpEncryBuff += usTmpLen;
	tOutLen -= usTmpLen;
	if( tOutLen <= 0 )
	{
		LK_ASSERT_STR(0, return -9, "tOutLen %d", tOutLen );
	}

	// 反序列化 CMessagePara
	usTmpLen = (int) *((unsigned short*)tpEncryBuff);
	if( usTmpLen < 0 || usTmpLen > tOutLen || usTmpLen >= 65535 )
	{
		LK_ASSERT_STR(0, return -10, "usTmpLen %d", usTmpLen );
	}

	tOutLen -= sizeof(unsigned short);

	// 消息工厂
	Message* tpMsgPara = CMessageFactory::GetSingletonPtr( )->CreateMessage( pMsg->msghead().messageid() );
	if( tpMsgPara == NULL )
	{
		LK_ASSERT_STR(0, return -11, "msgfactory can't create id %d ", pMsg->msghead().messageid() );
		return -11;
	}	

	tpEncryBuff += sizeof(unsigned short);
	bParseRet = tpMsgPara->ParseFromArray( tpEncryBuff, usTmpLen );

	if( bParseRet != true )
	{
#ifdef _USE_MESSAGE_BUFF_ // 如果使用了placement new，new在了一块静态存储的buffer,只能析构，不能delete
		tpMsgPara->~Message();
#else
		delete tpMsgPara;
#endif
		
		pMsg->set_msgpara(0);
		
		return -12;
	}

	tOutLen -= usTmpLen;
	if( tOutLen != 0 )
	{
		LK_ASSERT_STR(0, return -13, "tOutLen %d", tOutLen );
	}

	pMsg->set_msgpara( (unsigned int)tpMsgPara );
	
	return 0;
}


// ***************************************************************
//  Function:		ConvertClientCodeToMsg2
//  Description:	服务器专用，转换数据包到消息格式，中间会序列化和解密
//	Input:			
//	OutPut:			
//	Return:			
//	Others:			这里面的判断要尤其严格，因为可能会有非法的数据包，
//					要保证收到什么数据都不能崩溃。里面用到assert,发布release版时服务器要-DNDEBUG宏
//  Date:			02/05/2009
// 
// ***************************************************************
int ConvertClientCodeToMsg2 (  void* pBuff, unsigned short usBuffLen, CCSHead* pCshead, CMessage* pMsg, void* pMsgPara, unsigned short& rusParaLen, const unsigned char* pEncrypt )
{
	if( (pBuff == NULL) || ( usBuffLen < sizeof(unsigned short)*4 ) || (pCshead == NULL) || (pMsg == NULL) || (pMsgPara == NULL) )
	{
		return -1;
	}

	bool bParseRet = false;
	char* tpBuff = (char*)pBuff;
	int usTmpLen = 0;

	// 总长度
	int usTmpTotalLen = (int) *((unsigned short*)tpBuff);

	// 验证收到的包长和数据包中的包长是否匹配
	if( usTmpTotalLen != (int) usBuffLen )
	{
		LK_ASSERT_STR(0, return -2, "usTmpTotalLen %d != usBuffLen %d", usTmpTotalLen, usBuffLen );
	}

	tpBuff += sizeof(unsigned short); 
	usTmpTotalLen -= sizeof(unsigned short);


	// 反序列化 CCSHead
	usTmpLen = (int) *((unsigned short*)tpBuff);
	tpBuff += sizeof(unsigned short);
	usTmpTotalLen -= sizeof(unsigned short);

	//TODO: 安全性判断,这里 usTmpTotalLen 还不会<=0
	if( usTmpLen >= usTmpTotalLen || usTmpLen <= 0 )
	{
		LK_ASSERT_STR(0, return -3, "usTmpLen %d", usTmpLen );
	}

	bParseRet = pCshead->ParseFromArray( tpBuff, usTmpLen );
	if( bParseRet != true )
	{
		return -4;
	}
	tpBuff += usTmpLen;
	usTmpTotalLen -= usTmpLen;

	if ( usTmpTotalLen <= 0 )
	{
		LK_ASSERT_STR(0, return -5, "usTmpTotalLen %d", usTmpTotalLen );
		return -5;
	}

	// 反序列化 CMessage
	// 这里开始解密

	static unsigned char tEncryBuff[ MAX_PACKAGE_LEN ] = {0}; 
	unsigned char *tpEncryBuff = &tEncryBuff[0];
	int tOutLen = MAX_PACKAGE_LEN;
	DecryptData( EnCryptType1, pEncrypt, (unsigned char*)tpBuff, usTmpTotalLen, tpEncryBuff, &tOutLen );

	// 解密后的长度不能超过最大ushort , 也不能比输入的未解密前长度长
	if( tOutLen >= 65535 || tOutLen > usTmpTotalLen )
	{
		LK_ASSERT_STR(0, return -6, "tOutLen %d", tOutLen);
	}

	usTmpLen = (int) *((unsigned short*)tpEncryBuff);
	tpEncryBuff += sizeof(unsigned short);
	tOutLen -= sizeof(unsigned short);

	if( usTmpLen <= 0 || usTmpLen >= tOutLen || tOutLen <= 0 )
	{
		LK_ASSERT_STR(0, return -7, "usTmpLen %d", usTmpLen);
	}

	bParseRet = pMsg->ParseFromArray( tpEncryBuff, usTmpLen );
	if( bParseRet != true )
	{
		return -8;
	}
	
	tpEncryBuff += usTmpLen;
	tOutLen -= usTmpLen;
	if( tOutLen <= 0 )
	{
		LK_ASSERT_STR(0, return -9, "tOutLen %d", tOutLen );
	}

	// 消息体返回到外部
	usTmpLen = *((unsigned short*)tpEncryBuff);
	tpEncryBuff += sizeof(unsigned short);

	memcpy( pMsgPara, tpEncryBuff, usTmpLen );
	rusParaLen = usTmpLen;

	return 0;
}



//int ConvertMsgToClientCode2( CCSHead* pCSHead, CMessage* pMsg, void* pBuff, unsigned short& rusBuffLen )
//{
//	if( (pCSHead == NULL) || (pMsg == NULL) || (pBuff == NULL) || (pMsg->msgpara() == 0)
//		|| (rusBuffLen < sizeof(unsigned short)*4)  )
//	{
//		return -1;
//	}
//
//	bool bSerializeRet = false;
//	char* tpBuff = (char*)pBuff;
//	unsigned short usTmpLen = 0;
//	unsigned short usTmpLeftLen = rusBuffLen;
//
//	// 空出总长度2bytes
//	tpBuff += sizeof(unsigned short);
//	usTmpLeftLen -= sizeof(unsigned short);
//
//	// 序列化CCSHead
//	// 总长度先不用
//	pCSHead->set_totallength( 0 );
//	*((unsigned short*)tpBuff) = pCSHead->ByteSize();
//	tpBuff += sizeof(unsigned short);
//	usTmpLeftLen -= sizeof(unsigned short);
//	
//	bSerializeRet = pCSHead->SerializeToArray( tpBuff, usTmpLeftLen );
//	if( bSerializeRet != true )
//	{
//		return -1;
//	}
//
//	tpBuff += pCSHead->GetCachedSize();
//	usTmpLeftLen -= pCSHead->GetCachedSize();
//
//
//	// 序列化 CMessage
//
//	// 这里开始加密
//	//unsigned char tEncryBuff[ MAX_PACKAGE_LEN ] = {0};
//	//unsigned char *tpEncryBuff = &tEncryBuff[0];
//
//	/**((unsigned short*)tpBuff) = pMsg->ByteSize();
//	tpBuff += sizeof(unsigned short);
//	usTmpLeftLen -= sizeof(unsigned short);*/
//
//	*((unsigned short*)tpBuff) = pMsg->ByteSize();
//	tpBuff += sizeof(unsigned short);
//	usTmpLeftLen -= sizeof(unsigned short);
//
//	bSerializeRet = pMsg->SerializeToArray( tpBuff, usTmpLeftLen );
//	if( bSerializeRet != true )
//	{
//		return -1;
//	}
//
//	/*tpBuff += pMsg->ByteSize();
//	usTmpLeftLen -= pMsg->ByteSize();*/
//
//	tpBuff += pMsg->GetCachedSize();
//	usTmpLeftLen -= pMsg->GetCachedSize();
//
//	// 序列化 CMessagePara
//	Message* tpMsgPara = (Message*)pMsg->msgpara();
//	if( tpMsgPara == NULL )
//	{
//		return -1;
//	}
//
//	/**((unsigned short*)tpBuff) = tpMsgPara->ByteSize();
//	tpBuff += sizeof(unsigned short);
//	usTmpLeftLen -= sizeof(unsigned short);*/
//
//	*((unsigned short*)tpBuff) = tpMsgPara->ByteSize();
//	tpBuff += sizeof(unsigned short);
//	usTmpLeftLen -= sizeof(unsigned short);
//
//	bSerializeRet = tpMsgPara->SerializeToArray( tpBuff, usTmpLeftLen );
//	if( bSerializeRet != true )
//	{
//		return -1;
//	}
//
//	tpBuff += tpMsgPara->GetCachedSize();
//	usTmpLeftLen -= tpMsgPara->GetCachedSize();
//
//	//int tEncryptLen = MAX_PACKAGE_LEN;
//	//tpEncryBuff = &tEncryBuff[0];
//	//EncryptData( EnCryptType1, pEncrypt, tpEncryBuff, sizeof(unsigned short)*2 + pMsg->GetCachedSize() + tpMsgPara->GetCachedSize(), (unsigned char*)tpBuff, &tEncryptLen );
//
//	// 压入总长度
//	//rusBuffLen = sizeof(unsigned short)*4 + pCSHead->ByteSize() + pMsg->ByteSize() + tpMsgPara->ByteSize();
//	rusBuffLen = sizeof(unsigned short)*4 + pCSHead->ByteSize() + pMsg->ByteSize() + tpMsgPara->ByteSize();
//	*((unsigned short*)pBuff) = rusBuffLen;
//
//	return 0;
//}
//

// ***************************************************************
//  Function:	 
//  Description:	只序列化CMessage部分
//	Input:			
//	OutPut:			
//	Return:			
//	Others:			2bytes msghead长度 + msg序列化后的buff + 2bytes msgpara长度 + msgpara序列化后的buff
//  Date:			02/03/2009
// 
// ***************************************************************
int ConvertMsgToClientCode3( CMessage* pMsg, void* pBuff, unsigned short& rusBuffLen )
{
	if( (pMsg == NULL) || (pBuff == NULL) || (pMsg->msgpara() == 0) || (rusBuffLen < sizeof(unsigned short)*3)  )
	{
		return -1;
	}

	bool bSerializeRet = false;
	char* tpBuff = (char*)pBuff;
	unsigned short usTmpLeftLen = rusBuffLen;

	//// 空出总长度2bytes
	//tpBuff += sizeof(unsigned short);
	//usTmpLeftLen -= sizeof(unsigned short);

	// 序列化 CMessage
	*((unsigned short*)tpBuff) = pMsg->ByteSize();
	tpBuff += sizeof(unsigned short);
	usTmpLeftLen -= sizeof(unsigned short);

	bSerializeRet = pMsg->SerializeToArray( tpBuff, usTmpLeftLen );
	if( bSerializeRet != true )
	{
		return -1;
	}

	tpBuff += pMsg->GetCachedSize();
	usTmpLeftLen -= pMsg->GetCachedSize();

	// 序列化 CMessagePara
	Message* tpMsgPara = (Message*)pMsg->msgpara();
	if( tpMsgPara == NULL )
	{
		return -1;
	}

	*((unsigned short*)tpBuff) = tpMsgPara->ByteSize();
	tpBuff += sizeof(unsigned short);
	usTmpLeftLen -= sizeof(unsigned short);

	bSerializeRet = tpMsgPara->SerializeToArray( tpBuff, usTmpLeftLen );
	if( bSerializeRet != true )
	{
		return -1;
	}

	tpBuff += tpMsgPara->GetCachedSize();
	usTmpLeftLen -= tpMsgPara->GetCachedSize();

	rusBuffLen = sizeof(unsigned short)*2 + pMsg->GetCachedSize() + tpMsgPara->GetCachedSize();
	return 0;
}



int ConvertMsgToClientCode2( CCSHead* pCSHead, CMessage* pMsg, void* pBuff, unsigned short& rusBuffLen, const unsigned char* pEncrypt )
{
	if( (pCSHead == NULL) || (pMsg == NULL) || (pBuff == NULL) || (pMsg->msgpara() == 0)
		|| (rusBuffLen < sizeof(unsigned short)*4)  )
	{
		return -1;
	}

	bool bSerializeRet = false;
	char* tpBuff = (char*)pBuff;
	unsigned short usTmpLeftLen = rusBuffLen;

	// 空出总长度2bytes
	tpBuff += sizeof(unsigned short);
	usTmpLeftLen -= sizeof(unsigned short);

	// 序列化CCSHead
	// 总长度先不用
	//pCSHead->set_totallength( 0 );
	*((unsigned short*)tpBuff) = pCSHead->ByteSize();
	tpBuff += sizeof(unsigned short);
	usTmpLeftLen -= sizeof(unsigned short);

	bSerializeRet = pCSHead->SerializeToArray( tpBuff, usTmpLeftLen );
	if( bSerializeRet != true )
	{
		return -2;
	}

	tpBuff += pCSHead->GetCachedSize();
	usTmpLeftLen -= pCSHead->GetCachedSize();


	// 序列化 CMessage

	// 这里开始加密
	unsigned char tEncryBuff[ MAX_PACKAGE_LEN ] = {0};
	unsigned char *tpEncryBuff = &tEncryBuff[0];

	*((unsigned short*)tpEncryBuff) = pMsg->ByteSize();
	tpEncryBuff += sizeof(unsigned short);
	usTmpLeftLen -= sizeof(unsigned short);

	bSerializeRet = pMsg->SerializeToArray( tpEncryBuff, usTmpLeftLen );
	if( bSerializeRet != true )
	{
		return -3;
	}

	tpEncryBuff += pMsg->GetCachedSize();
	usTmpLeftLen -= pMsg->GetCachedSize();

	// 序列化 CMessagePara
	Message* tpMsgPara = (Message*)pMsg->msgpara();
	if( tpMsgPara == NULL )
	{
		return -4;
	}

	*((unsigned short*)tpEncryBuff) = tpMsgPara->ByteSize();
	tpEncryBuff += sizeof(unsigned short);
	usTmpLeftLen -= sizeof(unsigned short);

	bSerializeRet = tpMsgPara->SerializeToArray( tpEncryBuff, usTmpLeftLen );
	if( bSerializeRet != true )
	{
		return -5;
	}

	tpEncryBuff += tpMsgPara->GetCachedSize();
	usTmpLeftLen -= tpMsgPara->GetCachedSize();

	int tEncryptLen = MAX_PACKAGE_LEN;
	tpEncryBuff = &tEncryBuff[0];
	EncryptData( EnCryptType1, pEncrypt, tpEncryBuff, sizeof(unsigned short)*2 + pMsg->GetCachedSize() + tpMsgPara->GetCachedSize(), (unsigned char*)tpBuff, &tEncryptLen );

	// 压入总长度
	rusBuffLen = sizeof(unsigned short)*2 + pCSHead->GetCachedSize() + tEncryptLen;
	*((unsigned short*)pBuff) = rusBuffLen;

	return 0;
}

//int EncryptMsg( void* pBuff, short& rusBuffLen, const unsigned char* pEncrypt )
//{
//	if( (pBuff == NULL) || (rusBuffLen < sizeof(unsigned short)*4)  )
//	{
//		return -1;
//	}
//
//	char* tpBuff = (char*)pBuff;
//
//	tpBuff += rusBuffLen;
//
//	char* tpIniBuff = tpBuff;
//	unsigned short usTmpLen = 0;
//	//unsigned short usTmpLeftLen = rusBuffLen;
//
//	// 空出总长度2bytes
//	unsigned short tTotalLen = *(unsigned short*)tpBuff;
//	tpBuff += sizeof(unsigned short);
//	//usTmpLeftLen -= sizeof(unsigned short);
//
//	unsigned short tHeadLen = *(unsigned short*)tpBuff;
//	tpBuff += sizeof(unsigned short);
//	//usTmpLeftLen -= sizeof(unsigned short);
//
//	tpBuff += tHeadLen;
//	//usTmpLeftLen -= tHeadLen;
//
//
//	// 序列化 CMessage
//
//	// 这里开始加密
//	unsigned char tEncryBuff[ MAX_PACKAGE_LEN ] = {0};
//	unsigned char *tpEncryBuff = &tEncryBuff[0];
//
//	int tEncryptLen = MAX_PACKAGE_LEN;
//	memcpy( tpEncryBuff, tpBuff, tEncryptLen );
//	short tInLen = tTotalLen - tHeadLen - sizeof(unsigned short)*2;
//	if ( tInLen <= 0 )
//	{
//		return -1;
//	}
//	EncryptData( EnCryptType1, pEncrypt, tpEncryBuff, tInLen, (unsigned char*)tpBuff, &tEncryptLen );
//
//	// 压入总长度
//	//rusBuffLen = sizeof(unsigned short)*4 + pCSHead->ByteSize() + pMsg->ByteSize() + tpMsgPara->ByteSize();
//	tTotalLen = sizeof(unsigned short)*2 + tHeadLen + tEncryptLen;
//	*((unsigned short*)tpIniBuff) = tTotalLen;
//	rusBuffLen += tTotalLen;
//
//	return 0;
//}
//
//
//

// ***************************************************************
//  Function:		EncryptData 
//  Description:	加密数据
//	Input:			nAlgorithm	- 加密算法类型
//					pbyKey		- 密钥
//					pbyIn		- 输入buff
//					nInLength	- 输入长度
//					pbyOut		- 输出buff(加密数据)
//					pnOutLength	- 输出长度
//	OutPut:			无
//	Return:			无
//	Others:			pbyOut为密文格式,pnOutLength为pbyOut的长度,是8byte的倍数,至少应预留nInLength+17;
//  Date:			12/18/2008
// 
// ***************************************************************
void EncryptData(short nAlgorithm, const BYTE *pbyKey, const BYTE *pbyIn, int nInLength, BYTE *pbyOut, int *pnOutLength)
{
	if ( pbyKey == NULL || pbyIn == NULL || pbyOut == NULL || pnOutLength == NULL )
	{
		return ;
	}

	switch( nAlgorithm )
	{
	case 0:
		{
			memcpy( pbyOut, pbyIn, nInLength );
			*pnOutLength = nInLength;
			break;
		}
	case 1:
		{
			oi_symmetry_encrypt( pbyIn, nInLength, pbyKey, pbyOut, pnOutLength );
			break;
		}
	case 2:
		{
			oi_symmetry_encrypt2( pbyIn, nInLength, pbyKey, pbyOut, pnOutLength );
			break;
		}
	default:
		{
			break;
		}

	}

	return ;
}
//
//
// ***************************************************************
//  Function:		DecryptData
//  Description:	解密数据
//	Input:			nAlgorithm	- 加密算法类型
//					pbyKey		- 密钥
//					pbyIn		- 输入buff(加密数据)
//					nInLength	- 输入长度
//					pbyOut		- 输出buff(解密数据)
//					pnOutLength	- 输出长度
//	OutPut:			无
//	Return:			1 - 解密成功; 0 - 解密失败
//	Others:
//  Date:			12/18/2008
// 
// ***************************************************************
int DecryptData(short nAlgorithm, const BYTE *pbyKey, const BYTE *pbyIn, int nInLength, BYTE *pbyOut, int *pnOutLength)
{
	if( pbyKey == NULL || pbyIn == NULL || pbyOut == NULL || pnOutLength == NULL )
	{
		return 0;
	}

	int nDecryResult = 0;

	switch( nAlgorithm )
	{
	case 0:
		{
			memcpy( pbyOut, pbyIn, nInLength );
			*pnOutLength = nInLength;
			nDecryResult= 1;
			break;
		}
	case 1:
		{
			nDecryResult = oi_symmetry_decrypt( pbyIn, nInLength, pbyKey, pbyOut, pnOutLength );
			break;
		}
	case 2:
		{
			nDecryResult = oi_symmetry_decrypt2( pbyIn, nInLength, pbyKey, pbyOut, pnOutLength );
			break;
		}
	default:
		{
			break;
		}
	}

	return nDecryResult;

}




}

