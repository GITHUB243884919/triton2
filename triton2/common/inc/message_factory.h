#ifndef __MESSAGE_FACTORY__
#define	__MESSAGE_FACTORY__

#include <google/protobuf/message.h>

#ifdef LINUX
#include "servertool.h"
#endif

typedef ::google::protobuf::Message Message;

// 如果客户端没有单件，需要使用单件 
#ifndef LINUX
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
#endif


/* @class CMessageFactory
**	
** 消息工厂类
** 返回的是 ::google::protobuf::Message, 注意不是CMessage
**
*/
class CMessageFactory
	: public CSingleton< CMessageFactory > 

{
public:

	Message* CreateMessage( unsigned int unMessageID );

private:

	Message* CreateClientMessage( unsigned int unMessageID );
	Message* CreateServerMessage( unsigned int unMessageID );
	
	enum { MAX_MESSAGE_LEN = 60 * 1024 };// 60k 的消息静态缓存区
	static char msMessageBuff[ MAX_MESSAGE_LEN ];
};




#endif // !__MESSAGE_FACTORY__
