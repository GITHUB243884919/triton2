/******************************************************************************
   版权所有 (C ), 2008-2018, 蓝港在线（北京）科技有限公司

*******************************************************************************
  文件名          : objmanager.h
  版本号          : 1.0
  作者            : ZHL
  生成日期        : 2008-06-05
  最近修改        : 
  功能描述        : 对象管理器(创建,删除,查找),效率O(1),保证对象id自增长
  函数列表        : 
  修改历史        : 
  1 日期          : 2008-06-05
    作者          : ZHL
    修改内容      : 生成
*******************************************************************************/
#pragma once

#include "array_list.h"
#include "clock.h"
#include "lk_assert.h"
#include "log.h"

/**
* 递增使用对象id
* 这里使用了两个技术来保证对象的ID是不断递增的，
* 并且通过ID访问对象的速度很快
* 1. 利用了fixed_size_memory_pool释放对象会插入到队列的尾部
* 2. 利用保存last_used_id来保证id向上增加
*/
typedef int id_type;
template <id_type id_start, int count, id_type id_end>
struct incremental_obj_id_policy
{
	incremental_obj_id_policy()
		//: last_used_id(0)
	{
	}

	/**
	* 获取一个有效的对象id
	* 
	* @param offset 对象在数组中的索引(偏移量)
	* 
	* @return 有效的对象ID
	*/
	id_type get_valid_id(int offset)
	{
		id_type tmp_id = id_start + offset;
		if (tmp_id <= last_used_id)
		{
			id_type real_id = tmp_id 
				+ ((last_used_id - tmp_id) / count + 1) * count;
			if (real_id < id_end )
			{
				tmp_id = real_id;
			}
		}
		last_used_id = tmp_id;
		return tmp_id;
	}

	/**
	* 获取一个有效的对象偏移量
	* 
	* @param id     有效的对象ID
	* 
	* @return 对象在数组中的索引(偏移量)
	*         失败返回-1
	*/
	int get_offset_value(id_type id)
	{
		if (id < id_start || id > id_end)
		{
			LK_ASSERT(false, return -1);
		}
		return (id - id_start) % count;
	}

	int last_used_id;
};

template <class obj_name, id_type id_start, int count, id_type id_end>
class ObjManager : public lk::fixed_size_memory_pool<obj_name, count>
{
public:
	typedef incremental_obj_id_policy<id_start, count, id_end> id_policy;
	typedef lk::fixed_size_memory_pool<obj_name, count> super;
	typedef typename super::pointer pointer;
	typedef typename super::reference reference;
	typedef typename super::iterator iterator;
	typedef typename super::offset_type offset_type;
	typedef typename super::node_type node_type;

	ObjManager()
		: last_tick_obj_(0)
	{
		//TODO:
		//super::initailize_pool();
		last_tick_obj_ = super::begin( );
	}

	void initailize()
	{
		policy_.last_used_id = 0;
		super::initailize_pool();
		LOG_NOTICE("default", "init addr %6x", this);
	}

	void resume()
	{
		super::resume_pool();
		LOG_NOTICE("default", "resume addr %6x", this);
	}

	/**
	* 创建一个对象
	* 
	* @return 如果成功，返回指向对象的指针
	*         如果失败，返回0
	*/
	pointer create()
	{
		pointer p = super::allocate();
		//p = check_valid_id(p);	
		if( p )
		{
			p->set_id( get_valid_id(p) );
		}	
		return p;
	}

	/**
	* 使用other作为对象的构造函数来创建对象
	* 目前只支持构造函数只有一个参数
	* 
	* @param other
	* @param v      是任何其它信息的对象
	*               是obj_name构造函数的参数
	* 
	* @return 如果成功，返回指向对象的指针
	*         如果失败，返回0
	*/
	template <typename other>
	pointer create(other& v)
	{
		pointer p = super::allocate(v);
		//p = check_valid_id(p);
		if( p )
		{
			p->set_id( get_valid_id(p) );
		}
		return p;
	}

	/**
	* 释放创建的对象
	* 
	* @param p
	*/
	void remove(pointer p)
	{
		if ( p == NULL )
		{
			return;
		}

		if (super::end() != last_tick_obj_ && (&(*last_tick_obj_) == p))
		{
			++last_tick_obj_;
		}

		super::deallocate(p);
	}

	// 下面这些是从object继承的对象特有的功能
	/**
	* 释放创建的对象
	* 这里的对象必须继承于object才能调用此函数
	* 
	* @param id     对象的ID
	*/
	void remove(id_type id)
	{
		pointer p = find_by_id(id);

		//TODO: 为了发现问题，在这里打印日志
		if ( p == NULL )
		{
			LOG_ERROR( "default", "object id = %d not found", id );
			return;
		}
		remove( p );
	}

	/**
	* 按照ID来查找对象
	* 这里的对象必须继承于object才能调用此函数
	* 
	* @param id     对象的id
	* 
	* @return 如果成功，返回指向对象的指针
	*         如果失败，返回0
	*/
	pointer find_by_id(id_type id)
	{
		int offset = policy_.get_offset_value(id);
		if (offset >= 0)
		{
			pointer p = super::operator [](offset);
			if (p && p->get_id() == id)
			{
				return p;
			}
		}
		return 0;
	}

	// 注意：对象构造函数第一个必须调用此函数，用于获得正确的ID,并设置
	// 由于构造函数可能需要对象的ID,这种方式很丑陋，但暂时只能如此处理
	id_type get_valid_id(pointer p)
	{
		return policy_.get_valid_id(super::node_offset(p));
	}

	template <typename time_type>
	int tick(time_type curTime, int max_num)
	{
		int count = 0;
		for (; last_tick_obj_ != super::end() && count <= max_num;
			++last_tick_obj_, ++count)
		{
			last_tick_obj_->tick(curTime);
		}

		if (last_tick_obj_ ==  super::end())
		{
			last_tick_obj_ = super::begin();
		}

		return count;
	}

	/**
	* 返回对象管理器中的起始对象迭代器
	* 
	* @param 
	* 
	* @return 返回指向对象的迭代器
	*         
	*/
	iterator begin()
	{
		return super::begin();
	}

	/**
	* 返回对象管理器中的结束对象迭代器
	* 
	* @param 
	* 
	* @return 返回指向对象的迭代器
	*         
	*/
	iterator end()
	{
		return super::end();
	}

private:

	pointer check_valid_id(pointer p)
	{
		if (p)
		{
			if (!p->valid())
			{
				remove(p);
				p = 0;
			}
		}
		return  p;
	}

	id_policy policy_;
	iterator last_tick_obj_; // 最后一次tick的对象
};


