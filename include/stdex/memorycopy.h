#	pragma once

#	include <algorithm>

#	ifdef _MSC_VER
#	include <iterator>
#	endif

namespace stdex
{	
	inline void memorycopy( void * _dist, const void * _src, size_t _size )
	{
#	ifdef _MSC_VER
		std::copy( (unsigned char *)_src, (unsigned char *)_src + _size, stdext::unchecked_array_iterator<unsigned char *>((unsigned char *)_dist) );
#	else
		std::copy( (unsigned char *)_src, (unsigned char *)_src + _size, (unsigned char *)_dist );
#	endif
	}

	template<class T>
	inline void memorycopy_pod( T * _dist, const T * _src, size_t _size )
	{
#	ifdef _MSC_VER
		std::copy( (unsigned char *)_src, (unsigned char *)_src + _size * sizeof(T), stdext::unchecked_array_iterator<unsigned char *>((unsigned char *)_dist) );
#	else
		std::copy( (unsigned char *)_src, (unsigned char *)_src + _size * sizeof(T), (unsigned char *)_dist );		
#	endif
	}

	inline bool memorycopy_safe( void * _dist, size_t _offset, size_t _capacity, const void * _src, size_t _size )
	{
		if( _offset + _size > _capacity )
		{
			return false;
		}

#	ifdef _MSC_VER
		std::copy( (unsigned char *)_src, (unsigned char *)_src + _size, stdext::unchecked_array_iterator<unsigned char *>((unsigned char *)_dist + _offset) );		
#	else
		std::copy( (unsigned char *)_src, (unsigned char *)_src + _size, (unsigned char *)_dist + _offset );
#	endif

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
#	ifdef _MSC_VER
		std::copy( (unsigned char *)_src, (unsigned char *)_src + _size * sizeof(T), stdext::unchecked_array_iterator<unsigned char *>((unsigned char *)_dist + _offset * sizeof(T)) );
#	else
		std::copy( (unsigned char *)_src, (unsigned char *)_src + _size * sizeof(T), (unsigned char *)_dist + _offset * sizeof(T) );
#	endif

		return true;
	}
}
