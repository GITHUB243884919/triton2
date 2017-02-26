#pragma once

#include "modulemanager.h"
#include "scenecfg_manager.h"
#include "lk_hashmap.h"
#include "coretype.h"

using namespace SceneServer;



//#include "memorymanager.h"



#define SAVE_REDIRECT		1		// 玩家跨服务器时的标志               

#define SAVE_RETURN2GATE	2		// 玩家返回网关时的标志                  

#define SAVE_LEAVEGAME		3		// 玩家离开游戏时的标志                 

#define SAVE_KICKOUT		4		// 玩家被踢下线时的标志                    
                                         
#define SAVE_TIMETRIGER		5		// 玩家定时保存时的标志       

#define SAVE_FORCEEXIT		6		// 玩家强制退出时的标志

#define SAVE_FILE			7		// 保存文件中

#define RESAVE_FROMFILE		8		// 从文件重启存盘                                                              


// class: CSearchArray
// 功能：封装数组类的对象 提供快速查找函数 （二分）以及插入删除函数
template<typename T, int SIZE>
class CSearchArray
{
private:
	T	m_members[SIZE];
	int m_num;

public:
	CSearchArray()
	{ 
		if ( CObj::msCreateMode)
		{ memset(m_members, 0, sizeof(m_members)); m_num = 0; }
	}

	~CSearchArray(){}

private:
	std::pair<T*, T*> search( const T& key )
	{
		return std::equal_range( m_members, m_members + m_num, key );
	}

public:
	T*	find( const T& key )
	{
		std::pair<T*, T* > r = search( key );

		if( r.first != r.second) 
		{
			return r.first;
		}
		return NULL;
	}

	// 成功：0  已存在 -1 满 -2
	int	insert( const T& item )
	{
		if (m_num >= SIZE)
			return -2;

		std::pair<T*, T* > r = search( item );

		if( r.first != r.second) // 已存在
		{
			return -1;
		}
		else
		{
			std::copy_backward( r.first, m_members + m_num, m_members + m_num + 1 );
			*(r.first) = item;
			++ m_num;
		}
		return 0;
	}

	// 成功：0
	int remove( const T& item )
	{
		std::pair<T*, T* > r = search( item );
		if( r.first != r.second) //找到了
		{
			std::copy( r.first +1, m_members + m_num, r.first );
			-- m_num;
			return 0;
		}
		return -1;
	}

	int GetNum() const
	{
		return m_num;
	}

	const T* GetByIndex( int nIndex ) const
	{
		if ( nIndex >= 0 && nIndex < m_num )
			return &m_members[nIndex];
		else
			return NULL;
	}

	T* GetByIndex( int nIndex ) 
	{
		if ( nIndex >= 0 && nIndex < m_num )
			return &m_members[nIndex];
		else
			return NULL;
	}

	void Clear()
	{ memset(m_members, 0, sizeof(m_members)); m_num = 0; }

};

int GetRand( int arr[], int num );

struct SingleControlData
{
	int nFunID	;	// 功能ID
	int nOnOFF	;	// 开关
	int nParam1	;	// 参数1
	int nParam2	;	// 参数2
	char nParam3[CLI_CTRL_PARM3_LEN]	;	// 参数3
	SingleControlData()
	{
		nFunID	= 0;
		nOnOFF	= 0;
		nParam1 = 0;
		nParam2 = 0;
		memset( nParam3, 0, sizeof(nParam3) );
	}
};
class CMessageClientRunTimeParamNotify;
typedef lk::vector<SingleControlData, 100> CLIENT_CONTROL_TYPE;

// 客户端模块控制数据
class CClientControlData
{
private:
	CLIENT_CONTROL_TYPE m_Data;
public:
	void LoadFromXml( const char* pFileName );
	void FillMessage( CMessageClientRunTimeParamNotify* pMsg );
	void Send2AllPlayers( );
	CClientControlData()
	{
		m_Data.initailize();
	}
};

int GetYYYYMMDD(time_t nTm);