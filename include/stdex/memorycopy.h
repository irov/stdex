#	pragma once

#	include <algorithm>

#	include "stdex/unchecked_array_iterator.h"

namespace stdex
{	
	inline void memorycopy( void * _dist, const void * _src, size_t _size )
	{
		std::copy( (unsigned char *)_src, (unsigned char *)_src + _size, stdex::unchecked_array_iterator<unsigned char *>((unsigned char *)_dist) );
	}

	template<class T>
	inline void memorycopy_pod( T * _dist, const T * _src, size_t _size )
	{
		std::copy( (unsigned char *)_src, (unsigned char *)_src + _size * sizeof(T), stdex::unchecked_array_iterator<unsigned char *>((unsigned char *)_dist) );
	}

	inline bool memorycopy_safe( void * _dist, size_t _offset, size_t _capacity, const void * _src, size_t _size )
	{
		if( _offset + _size > _capacity )
		{
			return false;
		}

		std::copy( (unsigned char *)_src, (unsigned char *)_src + _size, stdex::unchecked_array_iterator<unsigned char *>((unsigned char *)_dist + _offset) );		

		return true;
	}

	template<class T>
	inline bool memorycopy_safe_pod( T * _dist, size_t _offset, size_t _capacity, const T * _src, size_t _size )
	{
		if( _offset + _size > _capacity )
		{
			return false;
		}

		//unchecked_array_iterator<unsigned char *> chkd_dist((unsigned char *)_dist + _offset);
		std::copy( (unsigned char *)_src, (unsigned char *)_src + _size * sizeof(T), stdex::unchecked_array_iterator<unsigned char *>((unsigned char *)_dist + _offset * sizeof(T)) );

		return true;
	}
}