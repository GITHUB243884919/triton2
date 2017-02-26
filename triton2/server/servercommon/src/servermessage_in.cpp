#include "servermessage_in.h"
#include "message_factory.h" 
#include "message_pb.hxx.pb.h"
#include "proxymessage_pb.hxx.pb.h" 
#include "client_comm_engine.h"

// 清除Cmessage 中 msgpara() 占用的内存空间
void pbmsg_delmsg(CMessage* pMessage)
{
	if(pMessage->msgpara())
	{
#ifdef _USE_MESSAGE_BUFF_ // 如果使用了placement new, 只析构不删除
		Message* pMessagePara = (Message*)(pMessage->msgpara());
		pMessagePara->~Message();
#else
		delete (Message*)(pMessage->msgpara());
#endif
		pMessage->set_msgpara(0);

	}
}

void pbmsg_setproxy(CProxyHead* pProxy, unsigned int nSrcFE, unsigned int nDstFE, unsigned int nSrcID, 
		unsigned int nDstID, EMTransType emTransTp, EMProxyCmd emProxyCmd, unsigned char* szOption, unsigned int nOptlen)
{
	if (nSrcFE) pProxy->set_srcfe( nSrcFE );
	if (nDstFE) pProxy->set_dstfe( nDstFE );
	if (nSrcID) pProxy->set_srcid( nSrcID );
	if (nDstID) pProxy->set_dstid( nDstID );

	pProxy->set_transfertype(emTransTp);
	pProxy->set_opflag(emProxyCmd);

	if (szOption && nOptlen)
	{
		pProxy->set_optlength(nOptlen);
		pProxy->set_option(szOption, nOptlen);
	}
}

/*
 * 功能： 为简化消息头的复制写的函数
 *
 */
void pbmsg_sethead(CMessage* tMessage, unsigned int nMsgID, unsigned int nMsgSeq, unsigned int nMsgType,
		unsigned int nSrcFE, unsigned int nDstFE, unsigned int nSrcID, unsigned int nDstID)
{
	CMessageHead* tpHead = tMessage->mutable_msghead();

	tpHead->set_messageid( nMsgID );
	//if (nMsgType) tpHead->set_msgtype( nMsgType );
	if (nMsgSeq) tpHead->set_msgseq( nMsgSeq );
	if (nSrcFE) tpHead->set_srcfe( nSrcFE );
	if (nDstFE) tpHead->set_dstfe( nDstFE );
	if (nSrcID) tpHead->set_srcid( nSrcID );
	if (nDstID) tpHead->set_dstid( nDstID );
}

/*
 *
 *  形成消息流: 输入：转发消息头, 消息头, 消息内容, 形成消息流的最大长度 输出：流, 以及流的长度
 *
 *  net_message struct:
 *
 *  total_lenth + head_length + head_content + message_length + message_content + para_length + para_content
 *  2 bytes           2 bytes     m  bytes        2 bytes            n bytes        2 bytes        x bytes
 *
 *  total_length = lenght of (net_message)
 *  head_length = length of (head_content) 
 *  message_length = length of (message_content) 
 *  para_length = length of (para_content)
 *
 *  messaget_content = message_head + [point to para_object (4 bytes)]
 *  message_head = msg_id + optional some fields
 *
 *  功能: 将tHeader, tMessage 形成消息流到 unBufferSize 大小的 szBuffer 中
 *  
 *  输入:
 *
 *  T:  客户端服务器是 CCSHead, 服务器之间是 CProxyHead
 *  tMessage: 需要压进去的Message
 *  szBuffer: 形成的字节流
 *  unBufferSize: szBuffer 的大小， 注意不是生成的流的长度
 *
 *  返回值:
 *
 * 	== 0 : 失败, protobuf 内部解包失败
 * 	>  0 : 生成的流的长度
 * 	<  0 : szBuffer 还差多少字节的才能合成包
 * 
 */

/*
template <class T>
int pbmsg_setmsg(T& tHeader, CMessage& tMessage, char* szBuffer, unsigned short unBufferSize)
{
	// 首先做类型强转
	Message* pPara = (Message *) (tMessage.msgpara());

	// 设置整个消息包的长度
	int iPkgSize =  tHeader.ByteSize() + tMessage.ByteSize() + pPara->ByteSize() + 2*4;

#ifdef LK_DEBUG
	printf("==========================Set==========================");
#endif

	// 断言的unBufferSize的长度一定大于输出的整个消息包的长度, 否则就是程序出问题了
	if (unBufferSize < iPkgSize) return (unBufferSize - iPkgSize); 

	// 转发包中记录了整个消息的长度, 注意tHeader里面的totallength必须是fix32
	//tHeader.set_totallength( iPkgSize );


	// 为整个消息的长度赋值
	char * tpBuffer = szBuffer;
	*((unsigned short*)tpBuffer) = iPkgSize;
	tpBuffer += 2; 

	// 记录转发消息头的长度
	*((unsigned short*)tpBuffer) = tHeader.ByteSize();
	tpBuffer += 2;

	// 将转发消息头压到szBuffer中；
	// 之前已经计算过长度 一定在unBufferSize之内了，因此下面使用这个长度应该是安全的
	bool ret = tHeader.SerializeToArray(tpBuffer, unBufferSize);
	if( ret != true )
	{
#ifdef LK_DEBUG
		printf("error at line(%d), tHeader.SerializeToArray \n", __LINE__ );
#endif
		return 0;
	} else
	{	
		tpBuffer += tHeader.ByteSize();

#ifdef LK_DEBUG
		printf("Header Length (%d) context is {\n", tHeader.ByteSize());
		printf(tHeader.DebugString().c_str());
		printf("}\n");
#endif
	}

	// 记录消息头的长度
	*((unsigned short*)tpBuffer) = tMessage.ByteSize();
	tpBuffer += 2;

	// 将消息头压到缓冲中
	ret = tMessage.SerializeToArray(tpBuffer, unBufferSize);
	if( ret != true )
	{
#ifdef LK_DEBUG
		printf("error at line(%d), tMessage.SerializeToArray \n", __LINE__ );
#endif
		return 0;
	}else
	{

		tpBuffer += tMessage.ByteSize();
#ifdef LK_DEBUG
		printf("Message Length (%d) context is { \n", tMessage.ByteSize());
		printf("%s\n", tMessage.DebugString().c_str());
		printf("}\n");
#endif
	}

	// 记录消息参数体的长度
	*((unsigned short*)tpBuffer) = pPara->ByteSize();
	tpBuffer += 2;

	// 将消息参数体压到缓冲区
	ret = pPara->SerializeToArray(tpBuffer, unBufferSize);
	if( ret != true )
	{
#ifdef LK_DEBUG
		printf("error at line(%d), pPara->SerializeToArray \n", __LINE__ );
#endif
		return 0;
	}else
	{
		tpBuffer += pPara->ByteSize();
#ifdef LK_DEBUG
		printf("Para Length (%d), context is { \n", pPara->ByteSize());
		printf("%s\n", pPara->DebugString().c_str());
		printf("}\n");

		printf("pkg size is %d\n", iPkgSize);
#endif
	}

	// 此处断言一下  iPkgSize == tpBuffer - sBuffer;
	assert(iPkgSize == (tpBuffer - szBuffer));

	return iPkgSize;
}
*/

/*
 *  将从fd读出的 长度的流转换为CProxyHead, CMessage,  
 *  返回的CproxyHead* 是局部的静态变量的地址, 不需要外部程序释放； 如果外部程序需要延时处理，请复制 
 *
 *  说明：
 *   	不使用消息队列的时候使用，直接从流中读取字节流，解码返回Header 和 tpMessage; 
 *   	其中tpMessage中的msgpara()是动态生成的, 需要调用的程序处理完之后释放
 *
 *  参数：
 *
 * 		szBuffer:		从socketfd读入的流
 * 		unBufferLength: 从socketfd 读入流的长度 
 * 		pHeader:		从客户端读入的请填写CCSHead, 服务器间的填写CPorxyHead
 * 		pMessage:		经过解包取得的pMessage
 *
 * 返回值:
 *
 * 	== 0 : 失败, protobuf 内部解包失败
 * 	>  0 : 已经解包的长度, 下次再调用，需要偏移这个长度的指针
 * 	<  0 : 还差多少字节的才能解包
 *
 */
int pbmsg_getheadmsg(unsigned char* szBuffer, unsigned int unBufferLength, CProxyHead* pHeader, CMessage* tpMessage)
{
	unsigned char * tpBuffer = szBuffer;
	unsigned short iPkgSize = *((unsigned short*)(tpBuffer));


	// 校验传进来的长度, 如果长度不够，需要返回继续读
	if (iPkgSize > (unBufferLength)) return (unBufferLength - iPkgSize);

	// 跳过整个包的长度字段
	tpBuffer += 2;

	//每次调用之前，需要重置tHeader
	pHeader->Clear();

	unsigned short unHeadLength = *((unsigned short*)(tpBuffer));
	bool ret = pHeader->ParseFromArray( tpBuffer+2, unHeadLength);
	if( ret != true )
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] pHeader->ParseFromArray error", 
				__LK_FILE__, __LINE__, __FUNCTION__ );

		return 0;
	}
	else
	{
		//iPkgSize -= tHeader.ByteSize();
		tpBuffer += (2 + pHeader->ByteSize());

		//或者使用 tpBuffer = tpBuffer + (2 + unHeadLength);
	}
#ifdef _DEBUG_
	//LOG_DEBUG("default", "PRoxyHead:%s", pHeader->ShortDebugString().c_str());
#endif
	if ( pHeader->opflag() == CMD_KEEPALIVE )
	{
		tpMessage->set_msgpara(0);
		assert(iPkgSize == (tpBuffer - szBuffer));

		return iPkgSize;
	}

	// 解消息头
	unsigned short unMsgHeadLength = *((unsigned short*)(tpBuffer));

	ret = tpMessage->ParseFromArray( tpBuffer+2,  unMsgHeadLength);
	if( ret != true )
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] tpMessage->ParseFromArray error", 
				__LK_FILE__, __LINE__, __FUNCTION__ );

		return 0;
	}else
	{
		//iPkgSize -= tpMessage->ByteSize();
		//或者 tpBuffer += (2 + unMsgHeadLength);
		tpBuffer += (2 + tpMessage->ByteSize());
	}


	unsigned short unMsgID =  tpMessage->msghead().messageid();
	// 判断unMsgID的有效性

	// 解消息体
	Message* tpMsgPara = CMessageFactory::GetSingleton().CreateMessage(unMsgID);

	if (tpMsgPara  == NULL)
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] CMessageFactory::CreateMessage error", 
				__LK_FILE__, __LINE__, __FUNCTION__ );

		return 0;
	}

	unsigned short unMsgParaLength = *((unsigned short*)(tpBuffer));
	ret = tpMsgPara->ParseFromArray( tpBuffer+2, unMsgParaLength);
	if( ret != true )
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] tpMsgPara->ParseFromArray error", 
				__LK_FILE__, __LINE__, __FUNCTION__ );
#ifdef _USE_MESSAGE_BUFF_
		tpMsgPara->~Message();
#else
		delete tpMsgPara;
#endif

		return 0;
	}
	else
	{
		// 将消息体附着在消息上, 消息头采用内部静态变量的
		//iPkgSize -= tpMsgPara->ByteSize();
		//
		tpBuffer += (2 + tpMsgPara->ByteSize()); 
		tpMessage->set_msgpara((unsigned int) tpMsgPara);

	}

	// 断言iPkgSize 应该 同解码的字节数一样
	assert(iPkgSize == (tpBuffer - szBuffer));

	// 返回ProxyHead 
	return iPkgSize;
}

/**
 *
 * 说明: 在向消息队列的时候追加消息的调用, 消息队列中存储的是流
 *
 * 参数: 
 *
 * szBuffer:		从socketfd读入的流
 * unBufferLength:  从socketfd 读入流的长度 
 * pHeader:			从客户端读入的请填写CCSHead, 服务器间的填写CPorxyHead
 * pMessage:		经过解包取得的pMessage->msghead(), 注意只解析了pMessage->msghead, pMessage->msgpara = 0;
 * szProtoBuf:		未解包的pMessage->msgpara()
 * punProtoBufLen:	未解包的pMessage->msgpara()的长度 
 *
 *
 * 返回:
 * == 0 : 失败, protobuf 内部解包失败
 * >  0 : 已经解包的长度, 下次再调用，需要偏移这个长度的指针
 * <  0 : 还差多少字节的才能解包
 *
 */

int pbmsg_getheadpb(char* szBuffer, unsigned int unBufferLength, 
		CProxyHead* pHeader, CMessage* tpMessage, char* szProtoBuf, unsigned short *punProtoBufLen)
{
	char * tpBuffer = szBuffer;
	unsigned short iPkgSize = *((unsigned short*)(tpBuffer));


	// 校验传进来的长度, 如果长度不够，需要返回继续读
	if (iPkgSize > (unBufferLength)) return unBufferLength - iPkgSize;

	// 跳过整个包的长度字段
	tpBuffer += 2;

	//每次调用之前，需要重置tHeader
	pHeader->Clear();

	unsigned short unHeadLength = *((unsigned short*)(tpBuffer));
	bool ret = pHeader->ParseFromArray( tpBuffer+2, unHeadLength);
	if( ret != true )
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] pHeader->ParseFromArray error", 
				__LK_FILE__, __LINE__, __FUNCTION__ );

		return 0;
	}
	else
	{
		//iPkgSize -= tHeader.ByteSize();
		tpBuffer += (2 + pHeader->ByteSize());

		//或者使用 tpBuffer = tpBuffer + (2 + unHeadLength);
	}

	if ( pHeader->has_opflag() && pHeader->opflag() == CMD_KEEPALIVE )
	{
		tpMessage->set_msgpara(0);
		assert(iPkgSize == (tpBuffer - szBuffer));

		return iPkgSize;
	}

	if (tpMessage != NULL)
	{
		// 解消息头
		unsigned short unMsgHeadLength = *((unsigned short*)(tpBuffer));

		ret = tpMessage->ParseFromArray( tpBuffer+2,  unMsgHeadLength);
		if( ret != true )
		{
			LOG_ERROR("default", "[ %s : %d ][ %s ] tpMessage->ParseFromArray error", 
				__LK_FILE__, __LINE__, __FUNCTION__ );

			return 0;
		}else
		{
			//iPkgSize -= tpMessage->ByteSize();
			//或者 tpBuffer += (2 + unMsgHeadLength);
			tpBuffer += (2 + tpMessage->ByteSize());
		}

		if (szProtoBuf != NULL)
		{
			*punProtoBufLen= *((unsigned short*)(tpBuffer));

			// 断言iPkgSize 应该 同解码的字节数一样
			assert(iPkgSize == 2 + 2 + unHeadLength + 2 + unMsgHeadLength + 2 + (*punProtoBufLen));
			memcpy(szProtoBuf, tpBuffer + 2, *punProtoBufLen);
		}
	}

	// 返回ProxyHead 
	return iPkgSize;
}

//  把消息解析到buffer中
int CodeMsgToBuffer( char *pCodeBuffer, int nCodeLength, CMessage &tMsg )
{ 
	char pBuffer[MAX_CODE_LEN];
	int	 nBufferLength = sizeof( pBuffer );
	LK_ASSERT( pBuffer != NULL && nBufferLength > 0, return -1 )

	// 保留两个字节记录总长度
	char *tpMsgBuffer = pBuffer+sizeof( unsigned short );
	int	  tBufferLen =	nBufferLength-sizeof( unsigned short );		
	
	// 解析消息
	*( unsigned short * )tpMsgBuffer = tMsg.ByteSize();
	tpMsgBuffer += sizeof( unsigned short );
	tBufferLen -= sizeof( unsigned short );

	int tRet = tMsg.SerializeToArray( tpMsgBuffer, tBufferLen );
	LK_ASSERT( tRet == true, return -1 )
	tpMsgBuffer += tMsg.ByteSize();
	tBufferLen -=   tMsg.ByteSize();


	// 序列化 CMessagePara
	Message* tpMsgPara = (Message*)tMsg.msgpara();
	if( tpMsgPara == NULL )
	{
		return -1;
	}

	*((unsigned short*)tpMsgBuffer) = tpMsgPara->ByteSize();
	tpMsgBuffer += sizeof(unsigned short);	
	tBufferLen	-= sizeof(unsigned short);	  

	tRet = tpMsgPara->SerializeToArray( tpMsgBuffer, tBufferLen );
	if( tRet != true )
	{
		return -1;
	}
	tpMsgBuffer += tpMsgPara->ByteSize();	
	tBufferLen	-= tpMsgPara->ByteSize(); 
	
	int tTotalLen =	 nBufferLength - tBufferLen + 2;	
	*( unsigned short * )pBuffer = 	tTotalLen;
	
	// 加密	 
	nCodeLength = MAX_CODE_LEN;
	ClientCommEngine::EncryptData( EnCryptType1, ClientCommEngine::tpKey, ( BYTE * )( pBuffer ), tTotalLen, ( BYTE * )( pCodeBuffer+2 ), &nCodeLength );
	*( unsigned short *)pCodeBuffer = nCodeLength+2;
	return nCodeLength+2;	
}

// 从buffer中解析消息
int DecodeMsgFromBufer( char *pCodeBuffer, int nCodeBufferLength, CMessage &tMsg )
{ 	
	unsigned short nCodeLength = *( ( unsigned short* )( pCodeBuffer ) );		
	LK_ASSERT( nCodeLength == nCodeBufferLength, return 0  )
	nCodeLength -= sizeof( unsigned short );
	
	// 解密		
	char pBuffer[MAX_CODE_LEN];
	int	 nBufferLength = MAX_CODE_LEN;
	int tRet = ClientCommEngine::DecryptData( EnCryptType1, ClientCommEngine::tpKey, ( BYTE * )( pCodeBuffer+2 ), nCodeLength, ( BYTE * )pBuffer, &nBufferLength );			
	LK_ASSERT( tRet == 1, return 0 )
	
	// 解析消息
	unsigned char * tpBuffer = ( unsigned char * )pBuffer;
	LK_ASSERT( pBuffer != NULL && nBufferLength > 0, return -1 )
	tpBuffer += sizeof( unsigned short );

	//每次调用之前，需要重置tMsg
	tMsg.Clear();

	unsigned short unHeadLength = *((unsigned short*)(tpBuffer));
	bool ret = tMsg.ParseFromArray( tpBuffer+2, unHeadLength);
	if( ret != true )
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] pHeader->ParseFromArray error", 
			__LK_FILE__, __LINE__, __FUNCTION__ );

		return 0;
	}
	else
	{ 	
		tpBuffer += ( 2 + tMsg.ByteSize() ); 	
	}

	// 解消息头	 
	unsigned short unMsgID =  tMsg.msghead().messageid();	

	// 解消息体
	Message* tpMsgPara = CMessageFactory::GetSingleton().CreateMessage( unMsgID );

	if (tpMsgPara  == NULL)
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] CMessageFactory::CreateMessage error", 
			__LK_FILE__, __LINE__, __FUNCTION__ );

		return 0;
	}

	unsigned short unMsgParaLength = *((unsigned short*)(tpBuffer));
	ret = tpMsgPara->ParseFromArray( tpBuffer+2, unMsgParaLength);
	if( ret != true )
	{
		LOG_ERROR("default", "[ %s : %d ][ %s ] tpMsgPara->ParseFromArray error", 
			__LK_FILE__, __LINE__, __FUNCTION__ );
#ifdef _USE_MESSAGE_BUFF_
		tpMsgPara->~Message();
#else
		delete tpMsgPara;
#endif

		return 0;
	}
	else
	{ 
		tpBuffer += (2 + tpMsgPara->ByteSize()); 
		tMsg.set_msgpara((unsigned int) tpMsgPara);	  
	}

	// 断言iPkgSize 应该 同解码的字节数一样
	//assert( nBufferLength == ( tpBuffer - pBuffer ) );

	// 返回ProxyHead
	return nBufferLength;
}

