/******************************************************************************
	版权所有 (C ), 2008-2018, 蓝港在线（北京）科技有限公司

******************************************************************************
	文件名			: lk_queue.h
	版本号			: 1.0
	作者			: ZHL
	生成日期		: 2008-08-10
	最近修改		: 
	功能描述		: 封装数据类型Queue, “FIFO”，先进先出，用前需初始化
	函数列表		: 
	修改历史		: 
		1 日期		: 2008-08-10
		作者		: ZHL
		修改内容	: 生成
*******************************************************************************/

#ifndef _LK_QUEUE_H
#define _LK_QUEUE_H 1

#include "array_list.h"
#include "lk_assert.h"
#include "lk_vector.h"
#include "lk_hashmap.h"
#include <functional>
#include <algorithm>

namespace lk
{
	template < typename T, std::size_t count >
	class queue
	{
		typedef T value_type;
		typedef  value_type* pointer;
		typedef  value_type* const_pointer;
		typedef  value_type& reference;
		typedef  const value_type& const_reference;

		typedef fixed_size_memory_pool<value_type, count> mem_pool;
		typedef typename mem_pool::iterator iterator;
		typedef typename mem_pool::const_iterator const_iterator;
		typedef typename mem_pool::offset_type offset_type;
		typedef typename mem_pool::node_type node_type;
		typedef typename mem_pool::size_type size_type;

	public:
		void initailize( )
		{
			_front = 0;
			_mempool.initailize_pool();
		}

		void resume( )
		{
			_mempool.resume_pool();
		}

		bool empty( ) const
		{
			return size() == 0;
		}

		size_type size( ) const
		{
			return  count - _mempool.size( );
		}

		size_type max_size( ) const
		{
			return count;
		}

		void push( const value_type& _Val )
		{
			if ( empty() )
			{
				pointer p = _mempool.allocate( _Val );
				LK_ASSERT( p != NULL, return );
				_front = _mempool.node_offset( p );
			}
			else
			{
				pointer p = _mempool.allocate( _Val );
				LK_ASSERT( p != NULL, return );			
			}
		}

		void pop( )
		{
			iterator it = iterator( reinterpret_cast<node_type*>( _mempool[ _front ] ) );
			iterator front = it;
			if ( size() <= 1 )
			{
				_front = 0;
			}
			else
			{
				_front = _mempool.node_offset( &*(--it) );
			}

			_mempool.deallocate( &*front );
		}

		value_type& front( )
		{
			return * ( _mempool[ _front ] ) ;
		}

		const value_type& front( ) const
		{
			return * ( _mempool[ _front ] ) ;
		}

		value_type& back( )
		{
			return * _mempool.begin();
		}

		const value_type& back( ) const
		{
			return * _mempool.begin();
		}

	private:
		offset_type _front;
		mem_pool _mempool;
	};


  /**
   *  @brief  A standard container automatically sorting its contents.
   *
   *  @ingroup Containers
   *  @ingroup Sequences
   *
   *  This is not a true container, but an @e adaptor.  It holds
   *  another container, and provides a wrapper interface to that
   *  container.  The wrapper is what enforces priority-based sorting 
   *  and %queue behavior.  Very few of the standard container/sequence
   *  interface requirements are met (e.g., iterators).
   *
   *  The second template parameter defines the type of the underlying
   *  sequence/container.  It defaults to std::vector, but it can be
   *  any type that supports @c front(), @c push_back, @c pop_back,
   *  and random-access iterators, such as std::deque or an
   *  appropriate user-defined type.
   *
   *  The third template parameter supplies the means of making
   *  priority comparisons.  It defaults to @c less<value_type> but
   *  can be anything defining a strict weak ordering.
   *
   *  Members not found in "normal" containers are @c container_type,
   *  which is a typedef for the second Sequence parameter, and @c
   *  push, @c pop, and @c top, which are standard %queue operations.
   *
   *  @note No equality/comparison operators are provided for
   *  %priority_queue.
   *
   *  @note Sorting of the elements takes place as they are added to,
   *  and removed from, the %priority_queue using the
   *  %priority_queue's member functions.  If you access the elements
   *  by other means, and change their data such that the sorting
   *  order would be different, the %priority_queue will not re-sort
   *  the elements for you.  (How could it know to do so?)
  */
	template<typename _Tp, std::size_t count, typename _Sequence = lk::vector< _Tp, count >,
	   typename _Compare  = std::less<typename _Sequence::value_type> >
    class priority_queue
	{
		// concept requirements
		typedef typename _Sequence::value_type _Sequence_value_type;

		public:
		typedef typename _Sequence::value_type                value_type;
		typedef typename _Sequence::reference                 reference;
		typedef typename _Sequence::const_reference           const_reference;
		typedef typename _Sequence::size_type                 size_type;
		typedef          _Sequence                            container_type;

		protected:
		//  See queue::c for notes on these names.
		_Sequence  c;
		_Compare   comp;

		public:
		void initailize( )
		{
			c.initailize();
		}
		/**
		 *  @brief  Default constructor creates no elements.
		 */
		priority_queue( )
		{ 
		}

		/**
		 *  @brief  Builds a %queue from a range.
		 *  @param  first  An input iterator.
		 *  @param  last  An input iterator.
		 *  @param  x  A comparison functor describing a strict weak ordering.
		 *  @param  s  An initial sequence with which to start.
		 *
		 *  Begins by copying @a s, inserting a copy of the elements
		 *  from @a [first,last) into the copy of @a s, then ordering
		 *  the copy according to @a x.
		 *
		 *  For more information on function objects, see the
		 *  documentation on @link s20_3_1_base functor base
		 *  classes@endlink.
		 */
		template<typename _InputIterator>
		priority_queue(_InputIterator __first, _InputIterator __last,
		const _Compare& __x = _Compare(), const _Sequence& __s = _Sequence())
		: c(__s), comp(__x)
		{
			c.initailize();
			c.insert(c.end(), __first, __last);
			std::make_heap(c.begin(), c.end(), comp);
		}

		/**
		 *  Returns true if the %queue is empty.
		 */
		bool empty() const 
		{ 
			return c.empty(); 
		}

		/**  Returns the number of elements in the %queue.  */
		size_type size() const { return c.size(); }

		/**
		 *  Returns a read-only (constant) reference to the data at the first
		 *  element of the %queue.
		 */
		const_reference top() const { return c.front(); }

		/**
		 *  @brief  Add data to the %queue.
		 *  @param  x  Data to be added.
		 *
		 *  This is a typical %queue operation.
		 *  The time complexity of the operation depends on the underlying
		 *  sequence.
		 */
		void push(const value_type& __x)
		{
			if ( size() >= count )
			{
				if ( comp(__x, top()) ) 
					pop(); 
				else 
					return;
			}


			c.push_back(__x);
			std::push_heap(c.begin(), c.end(), comp);
		}

		/**
		 *  @brief  Removes first element.
		 *
		 *  This is a typical %queue operation.  It shrinks the %queue
		 *  by one.  The time complexity of the operation depends on the
		 *  underlying sequence.
		 *
		 *  Note that no data is returned, and if the first element's
		 *  data is needed, it should be retrieved before pop() is
		 *  called.
		 */
		void pop()
		{
			std::pop_heap(c.begin(), c.end(), comp);
			c.pop_back();
		}
	};

  // No equality/comparison operators are provided for priority_queue.
  
	template < typename T, int count, typename key_type = int, typename value_type = int >
	class top_list
	{
		public:
			typedef std::pair< value_type, key_type > node_type;
			typedef key_type (*fkey_type)(const T&);
			typedef value_type (*fvalue_type)(const T&);
			typedef bool (*fcomp_type)(const value_type&, const value_type&);
			typedef bool (*hcomp_type)(const node_type&, const node_type&);

		private:
			typedef lk::hash_map< key_type, T, count > data_container;
			typedef lk::vector< node_type, count > sequence_container; 
			typedef typename data_container::iterator container_iterator;
			typedef typename sequence_container::iterator sequence_iterator;
			typedef typename data_container::value_type data_type;

			data_container  data;
			sequence_container c;

			fkey_type 	fkey;
			fvalue_type fvalue;
			fcomp_type  fcomp;
			hcomp_type  hcomp;


		public:
			void initailize()
			{
				fkey = NULL;
				fvalue = NULL;
				fcomp = NULL;

				c.initailize();
				data.initailize();
			}

			void setfunc(fkey_type pfkey, fvalue_type pfvalue, fcomp_type pfcomp)
			{ 
				fkey 	= pfkey; 
				fvalue	= pfvalue; 
				fcomp	= pfcomp;
				hcomp   = (hcomp_type) pfcomp;
			}

			top_list() 
			{

			}

			bool empty() const
			{
				return c.empty();
			}

			unsigned int size() const 
			{ 
				return c.size(); 
			}

			const key_type & top_key() const 
			{ 
				return c.front().second; 
			} 

			const value_type & top_value() const 
			{ 
				return c.front().first; 
			} 

			void push(const T& t)
			{
				key_type nkey = fkey(t);
				value_type nval = fvalue(t);

				container_iterator iter = data.find(nkey);
				if (iter != data.end())
				{
					// 此处存在问题,当val值跌出排行榜的范围,程序不知道该补谁的数值进入排行榜
					iter->second = t;

					sequence_iterator it = c.begin(); 
					for( ; it != c.end(); ++it)	
					{
						if ( it->second == nkey)
						{
							it->first = nval;
							std::make_heap(c.begin(), c.end(),  hcomp);	

							return;
						}
					}
				}else
				{
					if ( data.size() < count )
					{
						if ( data.insert(data_type(nkey, t)).second)
						{
							c.push_back(node_type(nval, nkey));
							std::push_heap(c.begin(), c.end(), hcomp);
						}

						return;
					}


					if (fcomp(nval, top_value()))
					{

						container_iterator iter = data.find(top_key());
						if ( iter != data.end())
						{
							std::pop_heap(c.begin(), c.end(), hcomp);
							c.pop_back();

							data.erase(iter);
							if ( data.insert(data_type(nkey, t)).second )
							{
								c.push_back(node_type(nval, nkey));
								std::push_heap(c.begin(), c.end(), hcomp);
							}
						}
					}
				}
			}

			void foreach( int(*pfunc)(const T&, int) )
			{
				std::sort_heap(c.begin(), c.end(), hcomp);

				sequence_iterator iter = c.begin();
				for ( ;iter != c.end(); ++iter)
				{
					container_iterator it = data.find(iter->second);
					if ( it != data.end())
					{
						if (pfunc(it->second, iter - c.begin())) break;
					}
				}

				std::make_heap(c.begin(), c.end(), hcomp);	
			}
	};
};

#endif
