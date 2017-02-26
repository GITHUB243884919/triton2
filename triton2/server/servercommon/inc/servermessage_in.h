#ifndef _SERVERMESSAGE_IN_
#define _SERVERMESSAGE_IN_

#include "base.h" 
#include "message_factory.h" 
#include "message_pb.hxx.pb.h" 
#include "proxymessage_pb.hxx.pb.h"
#include "log.h"
#include "lk_assert.h"

void pbmsg_delmsg(CMessage* pMessage);

void pbmsg_sethead(CMessage* tMessage, unsigned int nMsgID, unsigned int nMsgSeq, unsigned int nMsgType,
		unsigned int nSrcFE, unsigned int nDstFE, unsigned int nSrcID, unsigned int nDstID);

void pbmsg_setproxy(CProxyHead* pProxy, unsigned int nSrcFE, unsigned int nDstFE, unsigned int nSrcID, 
		unsigned int nDstID, EMTransType emTransTp = TRANS_P2P, EMProxyCmd emProxyCmd = CMD_NULL, 
		unsigned char* szOption = NULL, unsigned int nOptlen = 0);

int pbmsg_getheadmsg(unsigned char* szBuffer, unsigned int unBufferLength, CProxyHead* pHeader, CMessage* tpMessage);

int pbmsg_getheadpb(char* szBuffer, unsigned int unBufferLength, 
		CProxyHead* pHeader, CMessage* tpMessage, char* szProtoBuf, unsigned short *punProtoBufLen);


template <class T>
int pbmsg_setmsg(T& tHeader, CMessage& tMessage, char* szBuffer, unsigned short unBufferSize)
{
	// 首先做类型强转
	Message* pPara = (Message *) (tMessage.msgpara());

	if (pPara == NULL)
	{
		LOG_ERROR( "default", "The Message Para is NULL" );
		return 0;
	}

	// 设置整个消息包的长度
	int iPkgSize =  tHeader.ByteSize() + tMessage.ByteSize() + pPara->ByteSize() + 2*4;

	if (iPkgSize > 0xffff || iPkgSize < 0)
	{
		LOG_ERROR("default", " The Message(%d) PackSize(%d) more than 0xFFFF, Pls check it",tMessage.msghead().messageid(), iPkgSize);
		return 0;
	}

	// 断言的unBufferSize的长度一定大于输出的整个消息包的长度, 否则就是程序出问题了
	if (unBufferSize < iPkgSize) 
		return (unBufferSize - iPkgSize); 

	// 为整个消息的长度赋值
	char * tpBuffer = szBuffer;
	*((unsigned short*)tpBuffer) = iPkgSize;
	tpBuffer += 2; 

	// 记录转发消息头的长度
	*((unsigned short*)tpBuffer) = tHeader.GetCachedSize();
	tpBuffer += 2;

	// 将转发消息头压到szBuffer中；
	// 之前已经计算过长度 一定在unBufferSize之内了，因此下面使用这个长度应该是安全的
	bool ret = tHeader.SerializeToArray(tpBuffer, unBufferSize);
	if( ret != true )
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] pHeader->SerializeToArray error",
				__LK_FILE__, __LINE__, __FUNCTION__ );
		return 0;
	} 
	
	tpBuffer += tHeader.GetCachedSize();


	// 记录消息头的长度
	*((unsigned short*)tpBuffer) = tMessage.GetCachedSize();
	tpBuffer += 2;

	// 将消息头压到缓冲中
	ret = tMessage.SerializeToArray(tpBuffer, unBufferSize);
	if( ret != true )
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] tMessage.SerializeToArray error",
				__LK_FILE__, __LINE__, __FUNCTION__ );
		return 0;
	}

	tpBuffer += tMessage.GetCachedSize();

	// 记录消息参数体的长度
	*((unsigned short*)tpBuffer) = pPara->GetCachedSize();
	tpBuffer += 2;

	// 将消息参数体压到缓冲区
	ret = pPara->SerializeToArray(tpBuffer, unBufferSize);
	if( ret != true )
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] pPara->SerializeToArray error",
				__LK_FILE__, __LINE__, __FUNCTION__ );

		return 0;
	}

	tpBuffer += pPara->GetCachedSize();

	// 此处断言一下  iPkgSize == tpBuffer - sBuffer;
	assert(iPkgSize == (tpBuffer - szBuffer));

	return iPkgSize;
}

// 把消息解析到buffer中
int CodeMsgToBuffer( char *pBuffer, int nBufferLength, CMessage &tMsg );

// 从buffer中解析消息
int DecodeMsgFromBufer( char *pBuffer, int nBufferLength, CMessage &tMsg );

#endif

