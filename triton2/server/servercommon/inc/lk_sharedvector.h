/******************************************************************************
	版权所有 (C ), 2008-2018, 蓝港在线（北京）科技有限公司

******************************************************************************
	文件名		: lk_shared_vector.h
	版本号		: 1.0
	作者			: ZHL
	生成日期		: 2008-06-28
	最近修改		: 
	功能描述		: 共享vector, 适用情形：存在多个相同容器，而容器中的最大对象数非常大，	
				：　但平均数非常小，这样，多个容器共享一个对象池，节省空间。
				:	接口类似std::vector,但又不完全相同，尤其是对迭代器的返回，
				:	用之前请看清接口。
	函数列表		: 
	修改历史		: 
		1 日期	: 2008-06-28
		作者		: ZHL
		修改内容	: 生成
*******************************************************************************/

#pragma  once

#include "array_list.h"
#include "lk_assert.h"


namespace lk
{
	template <typename T,int L = 0>
	struct shared_array_link_list_iterator
	{
		typedef lk::array_list_node_base<T,L> node_type;
		typedef shared_array_link_list_iterator<T,L> _Self;
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef std::ptrdiff_t difference_type;
		shared_array_link_list_iterator(const _Self& __x)
			: ptr_(__x.ptr_)
		{
		}

		explicit shared_array_link_list_iterator(node_type* __x)
			: ptr_(__x)
		{
		}

		shared_array_link_list_iterator() : ptr_(0) {}

		reference operator* () const
		{
			return ptr_->value();
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		_Self& operator++()
		{
			ptr_ = ptr_->other_link(1) ;
			return *this;
		}
		_Self operator ++(int)
		{
			_Self __tmp(*this);
			++*this;
			return __tmp;
		}

		_Self& operator--()
		{
			ptr_ = ptr_->other_link(0);
			return *this;
		}
		_Self operator --(int)
		{
			_Self __tmp(*this);
			--*this;
			return __tmp;
		}

		bool operator == (const _Self & __x) const
		{
			return ptr_ == __x.ptr_;
		}

		bool operator != (const _Self & __x) const
		{
			return !(*this == __x);
		}

		_Self& operator = (const _Self& __x)
		{
			if (this != &__x)
			{
				ptr_ = __x.ptr_;
			}
			return * this;
		}
		node_type* ptr_;
	};


	template <typename T,int L = 0>
	struct shared_array_link_list_const_iterator
	{
		typedef lk::array_list_node_base<T,L> node_type;
		typedef shared_array_link_list_const_iterator<T,L> _Self;
		typedef T value_type;
		typedef value_type* pointer;
		typedef value_type& reference;
		typedef std::ptrdiff_t difference_type;
		shared_array_link_list_const_iterator(const _Self& __x)
			: ptr_(__x.ptr_)
		{
		}

		explicit shared_array_link_list_const_iterator(node_type* __x)
			: ptr_(__x)
		{
		}

		shared_array_link_list_const_iterator() : ptr_(0) {}

		reference operator* () const
		{
			return ptr_->value();
		}

		pointer operator->() const
		{
			return &(operator*());
		}

		_Self& operator++()
		{
			ptr_ = ptr_->other_link(1);
			return *this;
		}
		_Self operator ++(int)
		{
			_Self __tmp(*this);
			++*this;
			return __tmp;
		}

		_Self& operator--()
		{
			ptr_ = ptr_->other_link(0);
			return *this;
		}
		_Self operator --(int)
		{
			_Self __tmp(*this);
			--*this;
			return __tmp;
		}

		bool operator == (const _Self & __x) const
		{
			return ptr_ == __x.ptr_;
		}

		bool operator != (const _Self & __x) const
		{
			return !(*this == __x);
		}

		_Self& operator = (const _Self& __x)
		{
			if (this != &__x)
			{
				ptr_ = __x.ptr_;
			}
			return * this;
		}
		node_type* ptr_;
	};



	/************************************************************************/
	/* 对象个数不能暂不超过short个                                                                     */
	/************************************************************************/
	template < typename T, std::size_t count >
	class shared_vector
	{
	public:

		// type definitions
		typedef const T &  const_reference;
		typedef std::ptrdiff_t difference_type;

		typedef lk::fixed_size_memory_pool<T, count, 2> shared_array_list;
		typedef typename shared_array_list::node_type node_type;
		
		typedef typename shared_array_list::size_type size_type;
		typedef typename shared_array_list::offset_type offset_type;

		typedef typename shared_array_list::pointer pointer;


		typedef shared_array_link_list_iterator<T, 2 > iterator;
		typedef shared_array_link_list_const_iterator<T, 2 > const_iterator;



	public:

		// initailize 
		void initailize()
		{
			//share_list_ptr__->initailize_pool();
			begin__ = -1;
			end__ = -1;
			size__ = 0;
		}

		// iterator support
		iterator begin()
		{
			return iterator( reinterpret_cast<node_type*>( (*share_list_ptr__)[begin__] ) );
		}

		const_iterator begin() const
		{
			return iterator( reinterpret_cast<node_type*>( (*share_list_ptr__)[begin__] ) );
		}

		iterator end()
		{
			//return iterator( reinterpret_cast<node_type*>( (*share_list_ptr__)[end__] ) );
			return iterator(0);
		}

		const_iterator end() const
		{
			//return iterator( reinterpret_cast<node_type*>( (*share_list_ptr__)[end__] ) );
			return iterator(0);
		}

		// 后面插入一个
		iterator push_back(const T & t)
		{
			if( size__ >= count )
			{
				return iterator( 0 );
			}
			pointer p = share_list_ptr__->allocate(t);

			if( p != NULL )
			{
				size__ ++;

				node_type* current_node = reinterpret_cast<node_type*>(p);

				if( begin__ == -1 && end__ == -1 )
				{
					begin__ = share_list_ptr__->node_offset(p);
					end__ = begin__;
					assert( begin__ >=0 && end__ >= 0 );

					current_node->other_link_[0] = 0;
					current_node->other_link_[1] = 0;
				}
				else
				{

					node_type* end_node = reinterpret_cast<node_type*>( (*share_list_ptr__)[end__]);

					end_node->other_link_[1] = current_node - end_node ; /*share_list_ptr__->node_offset(p)*/;
					current_node->other_link_[0] = end_node - current_node;
					current_node->other_link_[1] = 0;

					end__ = share_list_ptr__->node_offset(p);
					assert( end__ >= 0 );
				}

				return iterator( current_node );
			}

			return iterator(  NULL );
		}

		iterator erase(iterator pos)
		{
			iterator ret_it = end();
			iterator endPos = end();
			if (pos != endPos)
			{
				iterator it_begin = iterator( reinterpret_cast<node_type*>( (*share_list_ptr__)[begin__]) );
				iterator it_end	= iterator( reinterpret_cast<node_type*>( (*share_list_ptr__)[end__]) );

				bool is_frist = ( pos == it_begin );
				bool is_last = ( pos == it_end );	

				if( is_frist && is_last )
				{
					begin__ = end__ = -1;
				}
				else if ( is_frist )
				{
					++ it_begin;
					begin__ = share_list_ptr__->node_offset (&*it_begin);
					assert( begin__ >= 0 );

					node_type* begin_node = reinterpret_cast<node_type*>( (*share_list_ptr__)[begin__]);
					begin_node->other_link_[0] = 0;

					ret_it = it_begin;

				}
				else if ( is_last )
				{
					-- it_end;
					end__ = share_list_ptr__->node_offset (&*it_end);
					assert( end__ >= 0 );
					node_type* end_node = reinterpret_cast<node_type*>( (*share_list_ptr__)[end__]);
					end_node->other_link_[1] = 0;

					ret_it = it_end;
				}
				else
				{
					//TODO: 这里没有做安全检查，iterator必须保证来自本vector,　否则会把其他vector的对象删除
					node_type* curr_node = reinterpret_cast<node_type*>(&*pos);
					node_type* prev_node = reinterpret_cast<node_type*>( curr_node->other_link(0) );
					node_type* next_node = reinterpret_cast<node_type*>( curr_node->other_link(1) );

					difference_type offset = next_node - prev_node;
					prev_node->other_link_[1] = offset;
					next_node->other_link_[0] = -offset;

					ret_it = iterator( next_node );

				}

				share_list_ptr__->deallocate( const_cast<pointer>(&*pos) );
				--size__;
			}
			return ret_it;
		}


		void clear()
		{
			iterator it_begin = begin();
			iterator it_end = end();

			while( it_begin != it_end )
			{
				it_begin = erase( it_begin );
			}

			size__ = 0;
			begin__ = -1;
			end__ = -1;
		}

		size_t capacity()
		{
			return count;
		}

		size_type size()
		{
			return size__;
		}

		bool empty()
		{
			return 0 == size__;
		}

	private:
		offset_type begin__;
		offset_type end__;
		size_t		size__;

	public:
		static shared_array_list* share_list_ptr__;
	};

	template < typename T, std::size_t count >
	lk::fixed_size_memory_pool<T, count, 2>* shared_vector<T,count>::share_list_ptr__ = NULL;


};



