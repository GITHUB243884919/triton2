#pragma once

#include "oi_tea.h"


// 错误码定义
enum 
{
	CLIENT_COMM_ENGINE_SUCCESS = 0,
};

enum EncryptType
{
	EncryptNone	 = 0,
	EnCryptType1 = 1,
	EnCryptType2 = 2,
};


class CCSHead;
class CMessage;



namespace ClientCommEngine
{

static int tKey[ 4 ] = {4356,78534,46677,3545};		// 数值待调
static unsigned char* tpKey = (unsigned char*)&tKey[0];

// ***************************************************************
//  Function: 	ConvertClientCodeToMsg2   
//  Description:把客户端code转换成CCSHead + CMessage, 
//				消息体指针已经在内部赋值给了CMessage.msgpara
//				注意：需要在外部delete msgpara
//  Date: 		09/24/2008
// 
// ***************************************************************
int ConvertClientCodeToMsg2( const void* pBuff, unsigned short usBuffLen, CCSHead* pCshead, CMessage* pMsg, const unsigned char* pEncrypt = tpKey );


// ***************************************************************
//  Function: 	ConvertClientCodeToMsg2   
//  Description:把客户端code转换成CCSHead + CMessage, 
//				不解析消息体，不赋值 (服务器内部使用)
//  Date: 		09/24/2008
// 
// ***************************************************************
int ConvertClientCodeToMsg2 ( void* pBuff, unsigned short usBuffLen, CCSHead* pCshead, CMessage* pMsg, void* pMsgPara, unsigned short& rusParaLen, const unsigned char* pEncrypt = tpKey );


// ***************************************************************
//  Function: 	ConvertMsgToClientCode2   
//  Description:把CCSHead + CMessasge 转换成客户端Code
//  Date: 		09/24/2008
// 
// ***************************************************************
int ConvertMsgToClientCode2(  CCSHead* pCSHead,  CMessage* pMsg, void* pBuff, unsigned short& rusBuffLen, const unsigned char* pEncrypt );

// 只序列化
//int ConvertMsgToClientCode2(  CCSHead* pCSHead,  CMessage* pMsg, void* pBuff, unsigned short& rusBuffLen );
int ConvertMsgToClientCode3( CMessage* pMsg, void* pBuff, unsigned short& rusBuffLen );

//int EncryptMsg( void* pBuff, short& rusBuffLen, const unsigned char* pEncrypt );





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
void EncryptData(short nAlgorithm, const BYTE *pbyKey, const BYTE *pbyIn, int nInLength, BYTE *pbyOut, int *pnOutLength);


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
int DecryptData(short nAlgorithm, const BYTE *pbyKey, const BYTE *pbyIn, int nInLength, BYTE *pbyOut, int *pnOutLength);


};


