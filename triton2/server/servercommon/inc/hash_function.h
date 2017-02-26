#ifndef HASH_FUNCTION_HEADER_
#define HASH_FUNCTION_HEADER_
#include <cstddef>
#include <string>
#include "lk_string.h"

namespace lk
{
	template<class _Key>
	struct hash
	{
	};

	inline size_t __stl_hash_string(const char* __s)
	{

		unsigned int hash = 5381 ;
		while (*__s)
		{
			hash += (hash << 5 ) + (*__s ++ );
		}
		return size_t(hash & 0x7FFFFFFF );

	}

	template<>
	struct hash<char*>
	{
		size_t operator()(const char* __s) const
		{
			return __stl_hash_string(__s);
		}
	};

	template<>
	struct hash<const char*>
	{
		size_t operator()(const char* __s) const
		{
			return __stl_hash_string(__s);
		}
	};
	
	template<>
	struct hash<std::string>
	{
		size_t operator()(const std::string& __s) const
		{
			return __stl_hash_string(__s.c_str());
		}		
	};


	template<>
	struct hash< lk::string<8> >
	{
		size_t operator()(const lk::string<8>& __s) const
		{
			return __stl_hash_string(__s.c_str());
		}		
	};


	template<>
	struct hash< lk::string<16> >
	{
		size_t operator()(const lk::string<16>& __s) const
		{
			return __stl_hash_string(__s.c_str());
		}		
	};

	template<>
	struct hash< lk::string<32> >
	{
		size_t operator()(const lk::string<32>& __s) const
		{
			return __stl_hash_string(__s.c_str());
		}		
	};

	template<>
	struct hash< lk::string<64> >
	{
		size_t operator()(const lk::string<64>& __s) const
		{
			return __stl_hash_string(__s.c_str());
		}		
	};

	template<>
	struct hash< lk::string<256> >
	{
		size_t operator()(const lk::string<256>& __s) const
		{
			return __stl_hash_string(__s.c_str());
		}		
	};

	template<>
	struct hash<char>
	{
		size_t operator()(char __x) const
		{
			return __x;
		}
	};

	template<>
	struct hash<unsigned char>
	{
		size_t operator()(unsigned char __x) const
		{
			return __x;
		}
	};

	template<>
	struct hash<signed char>
	{
		size_t operator()(unsigned char __x) const
		{
			return __x;
		}
	};

	template<>
	struct hash<short>
	{
		size_t operator()(short __x) const
		{
			return __x;
		}
	};

	template<>
	struct hash<unsigned short>
	{
		size_t operator()(unsigned short __x) const
		{
			return __x;
		}
	};

	template<>
	struct hash<int>
	{
		size_t operator()(int __x) const
		{
			return __x;
		}
	};


	template<>
	struct hash<unsigned int>
	{
		size_t operator()(unsigned int __x) const
		{
			return __x;
		}
	};

	template<>
	struct hash<long>
	{
		size_t operator()(long __x) const
		{
			return __x;
		}
	};

	template<>
	struct hash<unsigned long>
	{
		size_t operator()(unsigned long __x) const
		{
			return __x;
		}
	};

};

#endif /* HASH_FUNCTION_HEADER_ */

