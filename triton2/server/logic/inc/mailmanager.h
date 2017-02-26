#pragma once

#include "sceneobj_manager.h"
#include "entity.h"
#include "lk_hashmap.h"

// ********************************************************************** //
// CMailManager
// ********************************************************************** //
class CSysMailInfo
{
public:
	int	 mSysMailID;
	char mMailTitle[MAIL_TITLE_LEN];
	char mMailBody[MAIL_BODY_LEN];
	char mName[NAME_LENGTH];
public:
	CSysMailInfo()
	{
		if ( CObj::msCreateMode )
		{
			Initialize();
		}
		else
		{
			Resume();
		}
	}
	
	int Initialize()
	{
		mSysMailID = 0;
		memset( mMailTitle, 0, sizeof( mMailTitle ) );
		memset( mMailBody, 0, sizeof( mMailBody ) );
		memset( mName, 0, sizeof( mName ) );
		return 0;
	}
	
	int Resume()
	{
		return 0;
	}	
};
class CMailManager : public CSingleton< CMailManager >
{
public:	
	void* operator new( size_t size );                   
	void  operator delete( void* pointer );

	void* operator new(size_t size, const char* file, int line);
	void operator delete(void* pointer, const char* file, int line);

	static CSharedMem* mShmPtr;					 // 共享内存指针
public:
  static	unsigned int CountSize(); 
	typedef lk::hash_map< int, CSysMailInfo, MAX_SYSMAILTYPE_NUM >	SysMail;
	SysMail mSysMailList;
public:
	CMailManager( )
	{
		if ( CObj::msCreateMode )
		{
			Initialize();
		}
		else
		{
			Resume();
		}
	}
	~CMailManager( );	
	

	//得到邮件
	CMail* GetMail( CEntityPlayer *pPlayer );			
	//添加玩家
	CMail* AddMail( CEntityPlayer *pPlayer );									
	//删除玩家
	void Erase( CEntityPlayer *pPlayer );  
	
	int Initialize();
	int Resume()
	{
		return 0;
	}

public:
	// 插入一个系统邮件的信息
	int InsertSysMail( int nMailID, CSysMailInfo &rMailInfo );
	
	// 删除一个系统邮件的信息
	int DeleteSysMail( int nMailID );
	
	// 获取一个系统邮件的信息
	CSysMailInfo *GetSysMail( int nMailID );
};

