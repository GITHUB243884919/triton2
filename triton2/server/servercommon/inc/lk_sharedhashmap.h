#pragma once
/******************************************************************************
	  版权所有 (C ), 2008-2018, 蓝港在线（北京）科技有限公司	

******************************************************************************
	文件名			: lk_hashmap.h
	版本号			: 1.0
	作者			: ZHL
	生成日期		: 2008-06-26
	最近修改		: 
	功能描述		: 模拟Boots库hashmap实现定长hashmap
	函数列表		: 
	修改历史		: 
	1 日期			: 2008-06-26
		作者		: ZHL
		修改内容	: 生成
*******************************************************************************/


#include "array_list.h"
#include "hash_function.h"
#include "equal_function.h"

namespace lk
{
	template <class _Kty, class _Ty, std::size_t _Cap, std::size_t _BCap >
	class shared_hash_map
	{

	public:

		typedef _Kty	key_type;
		typedef _Ty		mapped_type;

		typedef std::pair<key_type, mapped_type> value_type;
		typedef fixed_size_memory_pool<value_type, _Cap, 2> hash_table;
		typedef std::ptrdiff_t difference_type;

		typedef typename hash_table::node_type node_type;
		typedef typename hash_table::size_type size_type;
		typedef typename hash_table::offset_type offset_type;
		typedef typename hash_table::pointer pointer;
		typedef typename hash_table::const_pointer const_pointer;

		typedef shared_array_link_list_iterator<value_type, 2 > iterator;
		typedef shared_array_link_list_const_iterator<value_type, 2 > const_iterator;

		typedef typename hash_table::iterator base_iterator;
		typedef typename hash_table::const_iterator	const_base_iterator;

		typedef std::pair<iterator, bool> _Pairib;

		// Get an iterator for the beginning of the map.
		iterator begin()
		{
			return iterator( reinterpret_cast<node_type*>( (*hash_table_)[begin__] ) );
		}

		// Get an iterator for the end of the map.
		iterator end()
		{
			return iterator( 0 );
		}

		// Get an iterator for the beginning of the map.
		const_iterator begin() const
		{
			return const_iterator( reinterpret_cast<node_type*>( (*hash_table_)[begin__] ) );
		}

		// Get an iterator for the end of the map.
		const_iterator end() const
		{
			return const_iterator( 0 );
		}

		// Initailize the map.
		void initailize()
		{
			for( int t = 0; ( unsigned int ) t < _BCap; ++t )
			{
				buckets_[t].first = -1;
				buckets_[t].last = -1;
			}

			//hash_table_->initailize_pool();

			begin__ = -1;
			end__ = -1;
			size__ = 0;
		}


		size_type size() const
		{
			return size__;
		}

		// Check whether the map is empty.
		bool empty() const
		{
			return  size__ == 0;
		}


		// Insert a new entry into the map.
		_Pairib insert(const value_type& v)
		{
			lk::hash<key_type> _hash;
			std::size_t bucket = _hash(v.first) % _BCap;

			offset_type begin = buckets_[bucket].first;
			base_iterator it_begin = base_iterator( reinterpret_cast<node_type*>( (*hash_table_)[begin]) );

			pointer p = NULL;
			iterator it_return;

			// 没有冲突
			if ( begin == -1  )
			{	
				assert( buckets_[bucket].last == -1);
				p = hash_table_->allocate(v);
				if( !p )
				{
					return _Pairib( iterator(it_begin.ptr_), false);
				}
				buckets_[bucket].first = buckets_[bucket].last = hash_table_->node_offset(p);
				
				it_return = iterator( reinterpret_cast<node_type*>( (*hash_table_)[buckets_[bucket].first]) );
			}
			else
			{
				assert( buckets_[bucket].last != -1 );
				offset_type end = buckets_[bucket].last;
				base_iterator it_end  = base_iterator( reinterpret_cast<node_type*>( (*hash_table_)[end]) );

				++it_end;

				while ( it_begin != it_end )
				{
					if (it_begin->first == v.first)
					{
						return _Pairib( iterator(it_begin.ptr_), false);
					}
					++it_begin;
				}

				p = hash_table_->allocate(v);
				if( !p )
				{
					return _Pairib( iterator(it_begin.ptr_), false);
				}

				hash_table_->insert_head_before(  (*hash_table_)[begin] );

				it_begin = base_iterator( reinterpret_cast<node_type*>( (*hash_table_)[begin]) );

				-- it_begin ;

				buckets_[bucket].first = hash_table_->node_offset(&*it_begin);

				assert( buckets_[bucket].last != -1 );

				it_return = iterator(it_begin.ptr_);
			}

			node_type* current_node = reinterpret_cast<node_type*>(p);

			if( begin__ == -1 && end__ == -1 )
			{
				begin__ = hash_table_->node_offset(p);
				end__ = begin__;

				current_node->other_link_[0] = 0;
				current_node->other_link_[1] = 0;
			}
			else
			{

				node_type* end_node = reinterpret_cast<node_type*>( (*hash_table_)[end__]);

				end_node->other_link_[1] = current_node - end_node ; 
				current_node->other_link_[0] = end_node - current_node;
				current_node->other_link_[1] = 0;

				end__ = hash_table_->node_offset(p);
			}

			size__++;

			return _Pairib(it_return, true);
		}


		// Find an entry in the map.
		iterator find(const key_type& k)
		{
			lk::hash<key_type> _hash;
			std::size_t bucket = _hash(k) % _BCap;

			offset_type begin = buckets_[bucket].first;
			base_iterator it_begin = base_iterator( reinterpret_cast<node_type*>( (*hash_table_)[begin]) );

			if ( begin == -1 )
			{
				return iterator(0);
			}
			
			assert( buckets_[bucket].last != -1);

			offset_type end = buckets_[bucket].last;
			base_iterator it_end  = base_iterator( reinterpret_cast<node_type*>( (*hash_table_)[end]) );

			++it_end;

			while ( it_begin != it_end )
			{
				if ( it_begin->first == k )
				{
					return iterator(it_begin.ptr_);
				}

				++it_begin;
			}

			return iterator(0);

		}

		// Find an entry in the map.
		//const_iterator find(const key_type& k) const
		//{
		//	lk::hash<key_type> _hash;
		//	std::size_t bucket = _hash(k) % _BCap;

		//	offset_type begin = buckets_[bucket].first;
		//	const_base_iterator it_begin = iterator( reinterpret_cast<node_type*>( (*hash_table_)[begin]) );

		//	if ( begin == -1 )
		//	{
		//		return const_iterator(0);
		//	}

		//	offset_type end = buckets_[bucket].last;
		//	const_base_iterator it_end  = iterator( reinterpret_cast<node_type*>( (*hash_table_)[end]) );

		//	++it_end;

		//	while ( it_begin != it_end )
		//	{
		//		if ( it_begin->first == k )
		//		{
		//			return it_begin;
		//		}

		//		++it_begin;
		//	}

		//	return iterator(0);

		//}


		// Erase an entry from the map.
		void erase( iterator it )
		{
			if ( it == this->end() )
			{
				return;
			}

			lk::hash<key_type> _hash;
			std::size_t bucket = _hash(it->first) % _BCap;

			offset_type begin = buckets_[bucket].first;
			base_iterator it_begin = base_iterator( reinterpret_cast<node_type*>( (*hash_table_)[begin]) );

			offset_type end = buckets_[bucket].last;
			base_iterator it_end  = base_iterator( reinterpret_cast<node_type*>( (*hash_table_)[end]) );

			bool is_first = ( it == iterator(it_begin.ptr_) );
			bool is_last = ( it == iterator(it_end.ptr_) );

			if ( is_first && is_last )
			{
				buckets_[bucket].first = buckets_[bucket].last = -1;
			}
			else if ( is_first )
			{
				++ it_begin;
				buckets_[bucket].first = hash_table_->node_offset(&*it_begin);
				assert(buckets_[bucket].first<=_Cap);
				assert(buckets_[bucket].last != -1);
			}
			else if ( is_last )
			{
				-- it_end;
				buckets_[bucket].last = hash_table_->node_offset(&*it_end);
				assert(buckets_[bucket].last <= _Cap);
				assert(buckets_[bucket].first != -1);
			}

			// 在本hash内是否是第一后最后一个节点
			iterator it_node_begin = iterator( reinterpret_cast<node_type*>( (*hash_table_)[begin__]) );
			iterator it_node_end = iterator( reinterpret_cast<node_type*>( (*hash_table_)[end__]) );

			bool is_frist_node = ( it == it_node_begin );
			bool is_last_node = ( it == it_node_end );	

			if( is_frist_node && is_last_node )
			{
				begin__ = end__ = -1;
			}
			else if ( is_frist_node )
			{
				++ it_node_begin;
				begin__ = hash_table_->node_offset (&*it_node_begin);

				node_type* begin_node = reinterpret_cast<node_type*>( (*hash_table_)[begin__]);
				begin_node->other_link_[0] = 0;

			}
			else if ( is_last_node )
			{
				-- it_node_end;
				end__ = hash_table_->node_offset (&*it_node_end);
				node_type* end_node = reinterpret_cast<node_type*>( (*hash_table_)[end__]);
				end_node->other_link_[1] = 0;
			}
			else
			{
				node_type* curr_node = reinterpret_cast<node_type*>(&*it);

				node_type* prev_node = reinterpret_cast<node_type*>( curr_node->other_link(0) );
				node_type* next_node = reinterpret_cast<node_type*>( curr_node->other_link(1) );

				difference_type offset = next_node - prev_node;

				prev_node->other_link_[1] = offset;
				next_node->other_link_[0] = -offset;

			}
			
			size__ -- ;
			hash_table_->deallocate( &*it);

		}

		size_type erase( const key_type& k )
		{
			erase (find(k));
			return size ();
		}

		// Remove all entries from the map.
		void clear()
		{
			// Clear the values.
			hash_table_->initailize_pool();

			// Initialise all buckets to empty.
			for (size_t i = 0; i < _BCap; ++i)
			{
				buckets_[i].first = buckets_[i].last = -1;
			}
		}


		shared_hash_map() {}
		~shared_hash_map() {}

	private:

		// Nocopyable
		shared_hash_map( const shared_hash_map& other );

	public:

		typedef struct bucket_type
		{		
			offset_type first;
			offset_type last;
		}bucket_type;

		bucket_type	buckets_[_BCap];

		offset_type begin__;
		offset_type end__;
		size_t		size__;

	public:
		static hash_table* hash_table_;

	};

	template < class _Kty, class _Ty, std::size_t _Cap, std::size_t _BCap  >
	lk::fixed_size_memory_pool< std::pair<_Kty, _Ty>, _Cap, 2>* shared_hash_map<_Kty, _Ty,_Cap, _BCap>::hash_table_ = NULL;

};

